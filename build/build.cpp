// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.build.build;

import otava.build.parser;
import otava.build.config;
import otava.build.gen_main_unit;
import otava.build.msbuild;
import otava.build.project_file;
import otava.codegen;
import otava.optimizer;
import otava.parser.translation_unit;
import otava.symbols.id;
import otava.symbols.bound_tree;
import otava.symbols.class_info;
import otava.symbols.classes;
import otava.symbols.context;
import otava.symbols.declarator;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.function_symbol;
import otava.symbols.function_table;
import otava.symbols.modules;
import otava.symbols.scope;
import otava.symbols.statement_binder;
import otava.symbols.symbol_index_map;
import otava.symbols.trace;
import otava.parser.spg.rules;
import otava.parser.module_dependency;
import otava.pp;
import otava.pp.state;
import otava.ast.node;
import otava.ast.visitor;
import otava.ast.modules;
import otava.ast.translation_unit;
import otava.ast.identifier;
import otava.lexer;
import soul.lexer;
import util.path;
import util.unicode;
import util.binary_stream_reader;
import util.binary_stream_writer;
import util.buffered_stream;
import util.file_stream;
import util.sha1;
import util.text_util;
import util.system;

namespace otava::build {

void WriteClassIndex(const std::string& classIndexFilePath, otava::symbols::class_index& index)
{
    util::FileStream fileStream(classIndexFilePath, util::OpenMode::write | util::OpenMode::binary);
    util::BufferedStream bufferedStream(fileStream);
    util::BinaryStreamWriter writer(bufferedStream);
    index.write(writer);
}

void Visit(std::int32_t fileId, Project* project, std::vector<int>& topologicalOrder, std::set<int>& visited)
{
    otava::symbols::Module* module = project->GetModule(fileId);
    for (otava::symbols::Module* dependsOnModule : module->DependsOnModules())
    {
        if (project->GetModule(dependsOnModule->Name()))
        {
            if (dependsOnModule->FileId() != -1)
            {
                if (visited.find(dependsOnModule->FileId()) == visited.cend())
                {
                    visited.insert(dependsOnModule->FileId());
                    Visit(dependsOnModule->FileId(), project, topologicalOrder, visited);
                }
            }
        }
    }
    visited.insert(fileId);
    topologicalOrder.push_back(fileId);
}

std::vector<std::int32_t> MakeTopologicalOrder(const std::vector<std::int32_t>& files, Project* project)
{
    std::set<std::int32_t> visited;
    std::vector<std::int32_t> topologicalOrder;
    for (std::int32_t file : files)
    {
        if (visited.find(file) == visited.cend())
        {
            Visit(file, project, topologicalOrder, visited);
        }
    }
    return topologicalOrder;
}

class ModuleDependencyVisitor : public otava::ast::DefaultVisitor
{
public:
    ModuleDependencyVisitor(std::int32_t file_, Project* project_, const std::string& fileName_, bool implementationUnit_, otava::symbols::Context* context_);
    std::unique_ptr<otava::symbols::Module> GetModule() { return std::move(m); }
    const std::string& InterfaceUnitName() const { return interfaceUnitName; }
    void Visit(otava::ast::ModuleDeclarationNode& node) override;
    void Visit(otava::ast::TranslationUnitNode& node) override;
    void Visit(otava::ast::ExportDeclarationNode& node) override;
    void Visit(otava::ast::ImportDeclarationNode& node) override;
    void Visit(otava::ast::ModuleNameNode& node) override;
private:
    Project* project;
    otava::symbols::Context* context;
    std::string fileName;
    std::int32_t file;
    bool implementationUnit;
    bool exp;
    bool expimp;
    bool imp;
    bool implementation;
    std::string interfaceUnitName;
    std::unique_ptr<otava::symbols::Module> m;
};

ModuleDependencyVisitor::ModuleDependencyVisitor(std::int32_t file_, Project* project_, const std::string& fileName_, bool implementationUnit_, 
    otava::symbols::Context* context_) :
    fileName(fileName_),
    file(file_),
    implementationUnit(implementationUnit_),
    exp(false),
    expimp(false),
    imp(false),
    implementation(false),
    project(project_),
    context(context_)
{
}

void ModuleDependencyVisitor::Visit(otava::ast::ModuleDeclarationNode& node)
{
    if (node.Export())
    {
        exp = true;
        node.ModuleName()->Accept(*this);
        exp = false;
    }
    else if (implementationUnit)
    {
        implementation = true;
        node.ModuleName()->Accept(*this);
        implementation = false;
    }
}

void ModuleDependencyVisitor::Visit(otava::ast::TranslationUnitNode& node)
{
    m.reset(new otava::symbols::Module(project->Name() + "." + fileName));
    m->SetFileId(file);
    if (node.Unit())
    {
        node.Unit()->Accept(*this);
    }
}

void ModuleDependencyVisitor::Visit(otava::ast::ExportDeclarationNode& node)
{
    if (node.Subject() && node.Subject()->IsImportDeclarationNode())
    {
        expimp = true;
        node.Subject()->Accept(*this);
        expimp = false;
    }
}

void ModuleDependencyVisitor::Visit(otava::ast::ImportDeclarationNode& node)
{
    if (!expimp)
    {
        imp = true;
    }
    node.Subject()->Accept(*this);
    imp = false;
}

void ModuleDependencyVisitor::Visit(otava::ast::ModuleNameNode& node)
{
    if (exp)
    {
        std::string moduleName = node.Str();
        m.reset(new otava::symbols::Module(moduleName));
        m->SetKind(otava::symbols::ModuleKind::interfaceModule);
        if (moduleName != "std.type.fundamental")
        {
            m->AddImportedModuleName("std.type.fundamental");
        }
        m->SetFileId(file);
    }
    else if (expimp && m)
    {
        m->AddExportedModuleName(node.Str());
    }
    else if (imp && m)
    {
        m->AddImportedModuleName(node.Str());
    }
    else if (implementation)
    {
        interfaceUnitName = node.Str();
        m.reset(new otava::symbols::Module(util::Path::GetFileName(fileName) + ".cpp"));
        m->SetKind(otava::symbols::ModuleKind::implementationModule);
        m->SetInterfaceUnitName(interfaceUnitName);
        m->AddImportedModuleName("std.type.fundamental");
        m->AddImportedModuleName(interfaceUnitName);
    }
}

void ScanDependencies(Project* project, std::int32_t fileId, bool implementationUnit, std::string& interfaceUnitName, otava::symbols::Context* context)
{
    std::string filePath = project->GetFileMap()->GetFilePath(fileId);
    std::string fileName = util::Path::GetFileNameWithoutExtension(filePath);
    std::string fileContent = util::ReadFile(filePath);
    std::u32string content = util::ToUtf32(fileContent);
    soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t> lexer = otava::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), filePath);
    lexer.SetFile(fileId);
    lexer.SetRuleNameMapPtr(otava::parser::spg::rules::GetRuleNameMapPtr());
    context->SetLexer(&lexer);
    context->SetCurrentProject(project);
    if (project->HasDefine("NOWARN"))
    {
        context->SetFlag(otava::symbols::ContextFlags::noWarnings);
    }
    using LexerType = decltype(lexer);
    std::unique_ptr<otava::ast::Node> node = otava::parser::module_dependency::ModuleDependencyParser<LexerType>::Parse(lexer, context);
    project->GetFileMap()->AddFileContent(fileId, std::move(content), lexer.GetLineStartIndeces());
    ModuleDependencyVisitor visitor(fileId, project, fileName, implementationUnit, context);
    node->Accept(visitor);
    project->SetModule(fileId, visitor.GetModule());
    interfaceUnitName = visitor.InterfaceUnitName();
}

void ReadFilesFile(const std::string& projectFilesPath, soul::lexer::FileMap* fileMap)
{
    util::FileStream filesFile(projectFilesPath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream filesBufStream(filesFile);
    util::BinaryStreamReader filesReader(filesBufStream);
    std::int32_t fileCount = filesReader.ReadInt();
    for (std::int32_t i = 0; i < fileCount; ++i)
    {
        std::int32_t fileId = filesReader.ReadInt();
        std::string filePath = filesReader.ReadUtf8String();
        fileMap->MapFile(filePath, fileId);
    }
}

void WriteFilesFile(const std::string& projectFilesPath, const std::vector<std::pair<std::int32_t, std::string>>& files)
{
    util::FileStream filesFile(projectFilesPath, util::OpenMode::write | util::OpenMode::binary);
    util::BufferedStream filesBufStream(filesFile);
    util::BinaryStreamWriter filesWriter(filesBufStream);
    std::int32_t fileCount = files.size();
    filesWriter.Write(fileCount);
    for (const auto& file : files)
    {
        filesWriter.Write(file.first);
        filesWriter.Write(file.second);
    }
}

void WriteFnFilePathsFile(Project* project, const std::string& config, int optLevel, const std::set<std::string>& configurations, const std::string& fnFilePathsFilePath)
{
    std::ofstream fnFilePathsFile(fnFilePathsFilePath);
    if (!fnFilePathsFile)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("could not create file '" + fnFilePathsFilePath + "'");
    }
    for (const auto& ref : project->ReferencedProjects())
    {
        std::string moduleDirPath = otava::symbols::MakeModuleDirPath(ref->RootDir(), config, optLevel, configurations);
        std::string fnsFilePath = util::GetFullPath(util::Path::Combine(moduleDirPath, "fns.txt"));
        fnFilePathsFile << fnsFilePath << "\n";
    }
    std::string moduleDirPath = otava::symbols::MakeModuleDirPath(project->RootDir(), config, optLevel, configurations);
    std::string fnsFilePath = util::GetFullPath(util::Path::Combine(moduleDirPath, "fns.txt"));
    fnFilePathsFile << fnsFilePath << "\n";
}

std::unordered_set<std::string> SkipInvokeSettingSet()
{
    std::unordered_set<std::string> skipInvokeCheckingFNames;
    skipInvokeCheckingFNames.insert("otava::lexer::MakeLexer(const char32_t*, const char32_t*, const std::string&)");
    skipInvokeCheckingFNames.insert("otava::parser::function::FunctionParser<soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t>>::FunctionDefinition(soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t>&, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::codegen::CodeGenerator::GenerateGlobalInitializationFunction()");
    skipInvokeCheckingFNames.insert("otava::build::BuildSequentially(otava::build::Project*, const std::string&, int, const std::string&, otava::build::BuildFlags, std::basic_ostream<char>*)");
    skipInvokeCheckingFNames.insert("otava::build::Project::LoadModules(otava::symbols::ModuleMapper&, const std::string&, int, const std::set<std::basic_string<char>, std::less<std::basic_string<char>>>&, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("util::CharacterInfoPage::CharacterInfoPage()");
    skipInvokeCheckingFNames.insert("util::ExtendedCharacterInfoPage::ExtendedCharacterInfoPage()");
    skipInvokeCheckingFNames.insert("::__main_wrapper__(int, const char**)");
    skipInvokeCheckingFNames.insert("otava::assembly::AssemblyContext::MakeIntegerLiteral(long long int, int)");
    skipInvokeCheckingFNames.insert("otava::assembly::AssemblyContext::MakeFloatLiteralSymbol(float)");
    skipInvokeCheckingFNames.insert("otava::assembly::AssemblyContext::MakeDoubleLiteralSymbol(double)");
    skipInvokeCheckingFNames.insert("otava::assembly::AssemblyContext::MakeSymbol(const std::string&)");
    skipInvokeCheckingFNames.insert("otava::assembly::AssemblyContext::MakeMacro(const std::string&)");
    skipInvokeCheckingFNames.insert("otava::assembly::AssemblyContext::MakeContent(otava::assembly::Value*)");
    skipInvokeCheckingFNames.insert("otava::assembly::AssemblyContext::MakeSizePrefix(long long int, otava::assembly::Value*)");
    skipInvokeCheckingFNames.insert("otava::assembly::AssemblyContext::MakeBinaryExpr(otava::assembly::Value*, otava::assembly::Value*, otava::assembly::Operator)");
    skipInvokeCheckingFNames.insert("otava::assembly::DeclarationSection::AddPublicDeclaration(otava::assembly::PublicDeclaration*)");
    skipInvokeCheckingFNames.insert("otava::assembly::DeclarationSection::AddLinkOnceDeclaration(otava::assembly::LinkOnceDeclaration*)");
    skipInvokeCheckingFNames.insert("otava::assembly::DataSection::AddData(otava::assembly::Data*)");
    skipInvokeCheckingFNames.insert("otava::assembly::CodeSection::CreateFunction(const std::string&)");
    skipInvokeCheckingFNames.insert("otava::assembly::Function::AddInstruction(otava::assembly::Instruction*)");
    skipInvokeCheckingFNames.insert("otava::assembly::Registers::Registers()");
    skipInvokeCheckingFNames.insert("otava::intermediate::Function::RemoveEntryAndExitBlocks()");
    skipInvokeCheckingFNames.insert("otava::symbols::ArgumentConversionTable::GetAdjustDeletePtrConversionFn(otava::symbols::TypeSymbol*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::ArgumentConversionTable::GetDynamicPtrCastFn(otava::symbols::TypeSymbol*, otava::symbols::TypeSymbol*, const soul::ast::FullSpan&, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::ArgumentConversionTable::GetArgumentConversion(otava::symbols::TypeSymbol*, otava::symbols::TypeSymbol*, otava::symbols::BoundExpressionNode*, const soul::ast::FullSpan&, otava::symbols::ArgumentMatch&, otava::symbols::FunctionMatch&, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::ClassTypeSymbol::GetVTabVariable(otava::symbols::Emitter&, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::Context::PushBoundFunction(otava::symbols::BoundFunctionNode*)");
    skipInvokeCheckingFNames.insert("otava::symbols::Context::AddBoundVTabFunction(otava::symbols::BoundFunctionNode*)");
    skipInvokeCheckingFNames.insert("otava::symbols::Context::AddBoundClass(otava::symbols::ClassTypeSymbol*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EndFunctionDefinition(otava::ast::Node*, int, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::GetIntegerValue(unsigned long long int, otava::symbols::TypeSymbol*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::GetFloatingValue(double, otava::symbols::TypeSymbol*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::GetCharValue(char32_t, otava::symbols::TypeSymbol*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::GetSymbolValue(otava::symbols::Symbol*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::GetInvokeValue(otava::symbols::Value*, const std::vector<otava::symbols::Value*>&, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::GetArrayValue(otava::symbols::TypeSymbol*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::GetStructureValue(otava::symbols::TypeSymbol*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::GetFunctionGroupValue(otava::symbols::Module*, otava::symbols::SymbolId, otava::symbols::FunctionGroupSymbol*, otava::symbols::FunctionSymbol*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::GetTypeValue(otava::symbols::Module*, otava::symbols::SymbolId, otava::symbols::TypeSymbol*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluationContext::AddValue(otava::symbols::Value*)");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<unsigned char>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<signed char>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<unsigned short int>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<short int>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<unsigned int>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<int>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<long long int>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<unsigned long long int>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<float>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::FundamentalTypeValue<double>::IrValue(otava::symbols::Emitter&, const soul::ast::FullSpan&, otava::symbols::Context*) override");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<unsigned char>*, std::negate<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<signed char>*, std::negate<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<short int>*, std::negate<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<unsigned short int>*, std::negate<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<int>*, std::negate<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<unsigned int>*, std::negate<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<long long int>*, std::negate<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::negate<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingUnaryOperator(otava::symbols::FundamentalTypeValue<float>*, std::negate<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingUnaryOperator(otava::symbols::FundamentalTypeValue<double>*, std::negate<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<unsigned char>*, std::bit_not<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<signed char>*, std::bit_not<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<short int>*, std::bit_not<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<unsigned short int>*, std::bit_not<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<int>*, std::bit_not<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<unsigned int>*, std::bit_not<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<long long int>*, std::bit_not<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerUnaryOperator(otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::bit_not<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::plus<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::plus<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::plus<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::plus<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::plus<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::plus<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::plus<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::plus<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::plus<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::plus<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::minus<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::minus<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::minus<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::minus<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::minus<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::minus<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::minus<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::minus<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::minus<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::minus<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::multiplies<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::multiplies<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::multiplies<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::multiplies<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::multiplies<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::multiplies<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::multiplies<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::multiplies<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::multiplies<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::multiplies<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::divides<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::divides<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::divides<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::divides<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::divides<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::divides<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::divides<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::divides<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::divides<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateFloatingBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::divides<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::modulus<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::modulus<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::modulus<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::modulus<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::modulus<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::modulus<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::modulus<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::modulus<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::shift_left<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::shift_left<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::shift_left<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::shift_left<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::shift_left<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::shift_left<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::shift_left<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::shift_left<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateShiftLeft(otava::symbols::Value*, otava::symbols::Value*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateShiftRight(otava::symbols::Value*, otava::symbols::Value*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::shift_right<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::shift_right<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::shift_right<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::shift_right<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::shift_right<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::shift_right<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::shift_right<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::shift_right<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::bit_or<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::bit_or<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::bit_or<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::bit_or<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::bit_or<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::bit_or<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::bit_or<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::bit_or<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::bit_xor<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::bit_xor<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::bit_xor<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::bit_xor<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::bit_xor<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::bit_xor<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::bit_xor<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::bit_xor<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::bit_and<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::bit_and<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::bit_and<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::bit_and<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::bit_and<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::bit_and<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::bit_and<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateIntegerBinaryOperator(otava::symbols::ValueKind, otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::bit_and<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::equal_to<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::equal_to<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::equal_to<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::equal_to<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::equal_to<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::equal_to<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::equal_to<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::equal_to<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::equal_to<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::equal_to<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::not_equal_to<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::not_equal_to<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::not_equal_to<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::not_equal_to<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::not_equal_to<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::not_equal_to<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::not_equal_to<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::not_equal_to<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::not_equal_to<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::not_equal_to<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::less<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::less<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::less<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::less<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::less<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::less<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::less<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::less<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::less<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::less<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::greater<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::greater<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::greater<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::greater<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::greater<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::greater<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::greater<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::greater<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::greater<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::greater<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::less_equal<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::less_equal<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::less_equal<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::less_equal<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::less_equal<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::less_equal<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::less_equal<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::less_equal<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::less_equal<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::less_equal<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned char>*, otava::symbols::FundamentalTypeValue<unsigned char>*, std::greater_equal<unsigned char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<signed char>*, otava::symbols::FundamentalTypeValue<signed char>*, std::greater_equal<signed char>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<short int>*, otava::symbols::FundamentalTypeValue<short int>*, std::greater_equal<short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned short int>*, otava::symbols::FundamentalTypeValue<unsigned short int>*, std::greater_equal<unsigned short int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<int>*, otava::symbols::FundamentalTypeValue<int>*, std::greater_equal<int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned int>*, otava::symbols::FundamentalTypeValue<unsigned int>*, std::greater_equal<unsigned int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<long long int>*, otava::symbols::FundamentalTypeValue<long long int>*, std::greater_equal<long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<unsigned long long int>*, otava::symbols::FundamentalTypeValue<unsigned long long int>*, std::greater_equal<unsigned long long int>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<float>*, otava::symbols::FundamentalTypeValue<float>*, std::greater_equal<float>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::EvaluateComparisonOperator(otava::symbols::FundamentalTypeValue<double>*, otava::symbols::FundamentalTypeValue<double>*, std::greater_equal<double>, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::PrintWarning(const otava::symbols::Exception&, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::CleanupBlock::Add(otava::symbols::BoundExpressionNode*, otava::symbols::Context*)");
    skipInvokeCheckingFNames.insert("otava::symbols::OperatorGroupNameMap::OperatorGroupNameMap()");
    skipInvokeCheckingFNames.insert("otava::symbols::ExpressionBinder::Visit(otava::ast::BracedInitListNode&)");
    skipInvokeCheckingFNames.insert("std::insertion_sort(std::pair<otava::symbols::FunctionSymbol*, int>*, std::pair<otava::symbols::FunctionSymbol*, int>*, otava::symbols::FunctionScoreGreater)");
    skipInvokeCheckingFNames.insert("otava::symbols::StatementBinder::Visit(otava::ast::MemberInitializerNode&)");
    skipInvokeCheckingFNames.insert("otava::symbols::SymbolTable::SymbolTable(otava::symbols::Module*, bool)");
    skipInvokeCheckingFNames.insert("otava::build::Solution::AddProject(otava::build::Project*, bool)");
    return skipInvokeCheckingFNames;
}

void BuildSequentially(Project* project, const std::string& config, int optLevelParam, const std::string& optFn, BuildFlags flags, std::ostream* outFile)
{
    std::unordered_set<std::string> skipInvokeSettingSet = SkipInvokeSettingSet();
    otava::symbols::ResetCompileEnded();
    otava::symbols::ModuleMapper moduleMapper;
    otava::ast::NodeIdFactory nodeIdFactory;
    otava::ast::SetNodeIdFactory(&nodeIdFactory);
    std::set<std::string> configurations;
    ConfigureProject(project, config, (flags & BuildFlags::verbose) != BuildFlags::none, configurations);
    for (const std::string& symbol : GetBuildSymbols())
    {
        project->AddDefine(symbol, 1);
    }
    bool releaseCfg = configurations.find("release") != configurations.end();
    int optLevel = otava::symbols::GetOptLevel(optLevelParam, releaseCfg);
    std::string moduleDirPath = otava::symbols::MakeModuleDirPath(project->RootDir(), config, optLevel, configurations);
    otava::symbols::FunctionTable functionTable;
    otava::symbols::SetFunctionTable(&functionTable);
    util::CreateDirectories(moduleDirPath);
    std::string projectFilePath;
    std::string outputFilePath;
    if (configurations.find("release") != configurations.end())
    {
        projectFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(
            util::Path::GetDirectoryName(project->FilePath()), "bin"), config),
            std::to_string(otava::symbols::GetOptLevel(optLevel, true))), project->Name() + ".vcxproj"));
        outputFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(
            util::Path::GetDirectoryName(project->FilePath()), "bin"), config),
            std::to_string(otava::symbols::GetOptLevel(optLevel, true))), project->Name()));
    }
    else
    {
        projectFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(
            project->FilePath()), "bin"), config), project->Name() + ".vcxproj"));
        outputFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(
            util::Path::GetDirectoryName(project->FilePath()), "bin"), config), project->Name()));
    }
    if (project->GetTarget() == otava::build::Target::program)
    {
        outputFilePath.append(".exe");
    }
    else if (project->GetTarget() == otava::build::Target::library)
    {
        outputFilePath.append(".lib");
    }
    project->SetOutputFilePath(outputFilePath);
    project->AddRoots(moduleMapper);
    project->InitFiles();
    std::vector<std::string> asmFileNames;
    std::vector<std::string> cppFileNames;
    std::vector<std::string> resourceFileNames;
    std::vector<std::string> compileUnitInitFunctionNames;
    std::vector<std::string> allCompileUnitInitFunctionNames;
    std::vector<std::unique_ptr<otava::symbols::Module>> builtModules;
    std::string mainFunctionIrName;
    int mainFunctionParams = 0;
    std::string cppFileName = "__main__.cpp";
    bool inliningEnabled = false;
    int totalFunctionsCompiled = 0;
    int functionsInlined = 0;
    int functionCallsInlined = 0;
    int invokes = 0;
    int unresolvedInvokes = 0;
    std::string libraryDirs;
    std::map<std::string, std::vector<std::string>> implementationNameMap;
    otava::symbols::Context scanContext;
    scanContext.SetModuleMapper(&moduleMapper);
    if (!project->Scanned())
    {
        project->SetScanned();
        for (std::int32_t file : project->InterfaceFiles())
        {
            std::string filePath = project->GetFileMap()->GetFilePath(file);
            std::string interfaceUnitName;
            ScanDependencies(project, file, false, interfaceUnitName, &scanContext);
            otava::symbols::Module* interfaceModule = project->GetModule(file);
            interfaceModule->SetFilePath(filePath);
            interfaceModule->SetKind(otava::symbols::ModuleKind::interfaceModule);
        }
        for (std::int32_t file : project->SourceFiles())
        {
            std::string filePath = project->GetFileMap()->GetFilePath(file);
            std::string interfaceUnitName;
            ScanDependencies(project, file, true, interfaceUnitName, &scanContext);
            otava::symbols::Module* implementationModule = project->GetModule(file);
            implementationModule->SetFilePath(filePath);
            implementationModule->SetKind(otava::symbols::ModuleKind::implementationModule);
            implementationNameMap[interfaceUnitName].push_back(implementationModule->Name());
        }
    }
    if (project->Name() != "std")
    {
        std::string stdProjectPath = moduleMapper.GetProjectFilePath("std");
        if (!stdProjectPath.empty())
        {
            project->AddReferenceFilePath(stdProjectPath);
        }
        else
        {
            otava::symbols::SetExceptionThrown();
            throw std::runtime_error("error: 'std.project' not found");
        }
    }
    for (const auto& path : project->ReferenceFilePaths())
    {
        std::string projectFilePath = util::GetFullPath(util::Path::Combine(project->RootDir(), path));
        std::unique_ptr<Project> projectReference = otava::build::ParseProjectFile(projectFilePath);
        std::string outputFilePath;
        if (configurations.find("release") != configurations.end())
        {
            outputFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(
                util::Path::GetDirectoryName(projectReference->FilePath()), "bin"), config),
                std::to_string(otava::symbols::GetOptLevel(optLevel, true))), projectReference->Name()));
        }
        else
        {
            outputFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(
                util::Path::GetDirectoryName(projectReference->FilePath()), "bin"), config),
                projectReference->Name()));
        }
        if (projectReference->GetTarget() == otava::build::Target::program)
        {
            outputFilePath.append(".exe");
        }
        else if (projectReference->GetTarget() == otava::build::Target::library)
        {
            outputFilePath.append(".lib");
        }
        projectReference->SetOutputFilePath(outputFilePath);
        projectReference->SetFileMap(project->GetFileMap());
        project->AddReferencedProject(std::move(projectReference));
    }
    if ((flags & BuildFlags::rebuild) == BuildFlags::none)
    {
        if (project->UpToDate(config, optLevel, configurations))
        {
            std::cout << "> project '" << project->Name() << "' is up-to-date" << "\n";
            //otava::symbols::SetCurrentContext(prevContext);
            //otava::symbols::SetModuleMapper(prevModuleMapper);
            //otava::ast::SetNodeIdFactory(prevNodeIdFactory);
            otava::symbols::SetCompileEnded();
            return;
        }
    }
    std::cout << "> building project '" << project->Name() << "'..." << "\n";
    otava::symbols::Context moduleContext;
    moduleContext.SetModuleMapper(&moduleMapper);
    moduleContext.SetCurrentConfig(config);
    moduleContext.SetCurrentConfigurations(configurations);
    if (configurations.find("release") != configurations.end())
    {
        moduleContext.SetReleaseConfig();
    }
    moduleContext.SetOptLevel(optLevel);
    moduleContext.SetOptFn(optFn);
    moduleContext.SetCurrentProject(project);
    if (project->HasDefine("NOWARN"))
    {
        moduleContext.SetFlag(otava::symbols::ContextFlags::noWarnings);
    }
    otava::symbols::Index importIndex = otava::symbols::Index(otava::symbols::ToUnderlying(moduleMapper.ModuleCount()));
    if (project->GetTarget() == Target::program)
    {
        for (const auto& referenceFilePath : project->ReferenceFilePaths())
        {
            std::string cuInitFileName = util::GetFullPath(util::Path::Combine(project->RootDir(),
                util::Path::Combine(util::Path::GetDirectoryName(referenceFilePath), "cu.init")));
            util::FileStream cuInitFile(cuInitFileName, util::OpenMode::read | util::OpenMode::binary);
            util::BinaryStreamReader reader(cuInitFile);
            int n = reader.ReadInt();
            for (int i = 0; i < n; ++i)
            {
                std::string compileUnitInitFunctionName = reader.ReadUtf8String();
                allCompileUnitInitFunctionNames.push_back(compileUnitInitFunctionName);
            }
        }
    }
    for (const auto& reference : project->ReferencedProjects())
    {
        std::string referenceFilesPath = util::GetFullPath(util::Path::Combine(reference->RootDir(), reference->Name() + ".files"));
        ReadFilesFile(referenceFilesPath, project->GetFileMap());
        std::string moduleDir = otava::symbols::MakeModuleDirPath(reference->RootDir(), config, optLevel, configurations);
        reference->ReadProjectId(moduleDir);
        moduleMapper.AddProjectId(reference->Name(), reference->GetProjectId());
    }
    project->SetProjectId(moduleMapper.MakeProjectId(project->Name()));
    std::string moduleDir = otava::symbols::MakeModuleDirPath(project->RootDir(), config, optLevel, configurations);
    project->WriteProjectId(moduleDir);
    project->LoadModules(moduleMapper, config, optLevel, configurations, &moduleContext);
    soul::lexer::FileMap* fileMap = project->GetFileMap();
    std::vector<std::pair<std::int32_t, std::string>> files;
    std::vector<std::int32_t> topologicalOrder = MakeTopologicalOrder(project->InterfaceFiles(), project);
    if ((flags & BuildFlags::rebuild) != BuildFlags::none || !project->InterfaceUnitsUpToDate(&moduleContext))
    {
        for (std::int32_t file : topologicalOrder)
        {
            std::string filePath = fileMap->GetFilePath(file);
            std::cout << "> " << filePath << "\n";
            files.push_back(std::make_pair(file, filePath));
            soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t> lexer = otava::lexer::MakeLexer(
                fileMap->GetFileContent(file).c_str(),
                fileMap->GetFileContent(file).c_str() + fileMap->GetFileContent(file).length(), filePath);
            lexer.SetPPHook(otava::pp::PreprocessPPLine);
            otava::pp::state::State* state = otava::pp::state::LexerStateMap::Instance().GetState(&lexer);
            for (const auto& define : project->Defines())
            {
                state->Define(define.symbol, define.value);
            }
            lexer.SetFile(file);
            lexer.SetRuleNameMapPtr(otava::parser::spg::rules::GetRuleNameMapPtr());
            std::unique_ptr<otava::symbols::Context> context(new otava::symbols::Context());
            otava::symbols::Emitter emitter;
            context->SetEmitter(&emitter);
            context->SetModuleMapper(&moduleMapper);
            context->SetCurrentConfig(config);
            context->SetCurrentConfigurations(configurations);
            context->SetOptLevel(optLevel);
            context->SetOptFn(optFn);
            context->SetCurrentProject(project);
            std::string compileUnitId = "compile_unit_" + util::GetSha1MessageDigest(filePath);
            context->GetBoundCompileUnit()->SetId(compileUnitId);
            if ((flags & BuildFlags::debug) != BuildFlags::none)
            {
                context->SetFlag(otava::symbols::ContextFlags::debug);
            }
            if (configurations.find("release") != configurations.end())
            {
                context->SetReleaseConfig();
            }
            if (project->HasDefine("NOWARN"))
            {
                context->SetFlag(otava::symbols::ContextFlags::noWarnings);
            }
            if (project->HasDefine("DEBUG_MEMORY"))
            {
                context->SetFlag(otava::symbols::ContextFlags::debugMemory);
            }
            context->SetFileMap(project->GetFileMap());
            context->SetDebugOutputStream(outFile);
            context->SetSkipInvokeCheckingSet(&skipInvokeSettingSet);
            std::unique_ptr<otava::symbols::Module> module = project->ReleaseModule(file);
            moduleMapper.ResetModule(module->Name());
            context->SetModule(module.get());
            context->SetCompileUnitModule(module.get());
            module->Init(context.get());
            module->SetImportIndex(importIndex++);
            module->SetFilePath(filePath);
            context->SetLexer(&lexer);
            using LexerType = decltype(lexer);
            std::unique_ptr<otava::ast::TranslationUnitNode> translationUnitNode = otava::parser::translation_unit::TranslationUnitParser<LexerType>::Parse(
                lexer, context.get());
            otava::symbols::GenerateDestructors(context->GetBoundCompileUnit(), context.get());
            bool verbose = (flags & BuildFlags::verbose) != BuildFlags::none;
            std::string asmFileName = otava::codegen::GenerateCode(*context.get(), config, optLevel, verbose,
                mainFunctionIrName, mainFunctionParams, false, std::vector<std::string>(), configurations);
            asmFileNames.push_back(asmFileName);
            otava::symbols::BoundFunctionNode* initFn = otava::symbols::GetCompileUnitInitFunction(context->GetBoundCompileUnit());
            if (initFn)
            {
                otava::symbols::FunctionDefinitionSymbol* fnDef = initFn->GetFunctionDefinitionSymbol();
                compileUnitInitFunctionNames.push_back(fnDef->IrName(context.get()));
            }
            totalFunctionsCompiled += context->TotalFunctionsCompiled();
            invokes += context->Invokes();
            unresolvedInvokes += context->UnresolvedInvokes();
            if (context->ReleaseConfig() && otava::optimizer::HasOptimization(otava::optimizer::Optimizations::inlining))
            {
                inliningEnabled = true;
                functionsInlined += context->FunctionsInlined();
                functionCallsInlined += context->FunctionCallsInlined();
            }
            module->ResetNode(translationUnitNode.release());
            module->SetImplementationUnitNames(implementationNameMap[module->Name()]);
            project->GetIndex().import(module->GetSymbolTable()->ClassIndex());
            module->Write(project->RootDir(), config, optLevel, context.get(), configurations);
            context.reset();
            std::string ftabFilePath = util::GetFullPath(util::Path::Combine(moduleDir, util::Path::GetFileName(filePath) + ".fns.txt"));
            module->GetFunctionTable().Write(ftabFilePath);
            moduleMapper.RemoveModule(module.get());
            moduleMapper.AddModuleInfo(module.get());
            builtModules.push_back(std::move(module));
        }
        std::string symbolIndexFilePath = util::GetFullPath(util::Path::Combine(moduleDir, "symbol_index.bin"));
        moduleMapper.GetSymbolIndexMap()->Save(symbolIndexFilePath);
        std::string irIdFilePath = util::GetFullPath(util::Path::Combine(moduleDir, "ir_id_map.bin"));
        project->SaveIrIdMap(irIdFilePath);
    }
    else
    {
        for (int file : topologicalOrder)
        {
            std::unique_ptr<otava::symbols::Module> module = project->ReleaseModule(file);
            moduleMapper.RemoveModule(module.get());
            std::string filePath = fileMap->GetFilePath(file);
            std::string asmFileName = util::Path::GetFileName(filePath) + ".asm";
            asmFileNames.push_back(asmFileName);
            std::string ftabFilePath = util::GetFullPath(util::Path::Combine(moduleDir, util::Path::GetFileName(filePath) + ".fns.txt"));
            if (util::FileExists(ftabFilePath))
            {
                otava::symbols::FunctionTable ftab;
                ftab.Read(ftabFilePath);
                otava::symbols::FunctionTable* globalFTab = otava::symbols::GetFunctionTable();
                if (globalFTab)
                {
                    globalFTab->Add(ftab);
                }
            }
        }
        std::string symbolIndexFilePath = util::GetFullPath(util::Path::Combine(moduleDir, "symbol_index.bin"));
        if (util::FileExists(symbolIndexFilePath))
        {
            moduleMapper.GetSymbolIndexMap()->Load(symbolIndexFilePath);
        }
        std::string irIdFilePath = util::GetFullPath(util::Path::Combine(moduleDir, "ir_id_map.bin"));
        if (util::FileExists(irIdFilePath))
        {
            project->LoadIrIdMap(irIdFilePath);
        }
    }
    for (std::int32_t file : project->SourceFiles())
    {
        std::string filePath = fileMap->GetFilePath(file);
        std::unique_ptr<otava::symbols::Module> module = project->ReleaseModule(file);
        std::string asmFilePath = util::Path::Combine(moduleDir, util::Path::GetFileName(module->FilePath()) + ".asm");
        module->SetAsmFilePath(asmFilePath);
        if (module->UpToDate(&moduleContext))
        {
            moduleMapper.RemoveModule(module.get());
            std::string asmFileName = util::Path::GetFileName(filePath) + ".asm";
            asmFileNames.push_back(asmFileName);
            std::string scountFilePath = util::GetFullPath(util::Path::Combine(moduleDir, util::Path::GetFileName(filePath) + ".scount"));
            if (util::FileExists(scountFilePath))
            {
                otava::symbols::SymbolCounts scounts;
                scounts.Load(scountFilePath);
                moduleMapper.GetSymbolIndexMap()->AddCounts(scounts);
            }
            std::string ftabFilePath = util::GetFullPath(util::Path::Combine(moduleDir, util::Path::GetFileName(filePath) + ".fns.txt"));
            if (util::FileExists(ftabFilePath))
            {
                otava::symbols::FunctionTable ftab;
                ftab.Read(ftabFilePath);
                otava::symbols::FunctionTable* globalFTab = otava::symbols::GetFunctionTable();
                if (globalFTab)
                {
                    globalFTab->Add(ftab);
                }
            }
            continue;
        }
        std::cout << "> " << filePath << "\n";
        files.push_back(std::make_pair(file, filePath));
        soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t> lexer = otava::lexer::MakeLexer(
            fileMap->GetFileContent(file).c_str(),
            fileMap->GetFileContent(file).c_str() + fileMap->GetFileContent(file).length(), filePath);
        lexer.SetPPHook(otava::pp::PreprocessPPLine);
        otava::pp::state::State* state = otava::pp::state::LexerStateMap::Instance().GetState(&lexer);
        for (const auto& define : project->Defines())
        {
            state->Define(define.symbol, define.value);
        }
        lexer.SetFile(file);
        lexer.SetRuleNameMapPtr(otava::parser::spg::rules::GetRuleNameMapPtr());
        std::unique_ptr<otava::symbols::Context> context(new otava::symbols::Context());
        otava::symbols::Emitter emitter;
        context->SetEmitter(&emitter);
        context->SetModuleMapper(&moduleMapper);
        context->SetCurrentConfig(config);
        context->SetCurrentConfigurations(configurations);
        context->SetOptLevel(optLevel);
        context->SetOptFn(optFn);
        context->SetSkipInvokeCheckingSet(&skipInvokeSettingSet);
        std::string compileUnitId = "compile_unit_" + util::GetSha1MessageDigest(filePath);
        context->GetBoundCompileUnit()->SetId(compileUnitId);
        context->SetCurrentProject(project);
        if ((flags & BuildFlags::debug) != BuildFlags::none)
        {
            context->SetFlag(otava::symbols::ContextFlags::debug);
        }
        if (configurations.find("release") != configurations.end())
        {
            context->SetReleaseConfig();
        }
        if (project->HasDefine("NOWARN"))
        {
            context->SetFlag(otava::symbols::ContextFlags::noWarnings);
        }
        if (project->HasDefine("DEBUG_MEMORY"))
        {
            context->SetFlag(otava::symbols::ContextFlags::debugMemory);
        }
        context->SetFileMap(project->GetFileMap());
        context->SetDebugOutputStream(outFile);
        context->SetModule(module.get());
        context->SetCompileUnitModule(module.get());
        module->Init(context.get());
        module->SetImportIndex(importIndex++);
        module->SetFilePath(filePath);
        context->SetLexer(&lexer);
        using LexerType = decltype(lexer);
        std::unique_ptr<otava::ast::TranslationUnitNode> translationUnitNode = otava::parser::translation_unit::TranslationUnitParser<LexerType>::Parse(
            lexer, context.get());
        otava::symbols::GenerateDestructors(context->GetBoundCompileUnit(), context.get());
        bool verbose = (flags & BuildFlags::verbose) != BuildFlags::none;
        std::string asmFileName = otava::codegen::GenerateCode(
            *context.get(), config, optLevel, verbose, mainFunctionIrName, mainFunctionParams, false, std::vector<std::string>(), configurations);
        asmFileNames.push_back(asmFileName);
        project->GetIndex().import(module->GetSymbolTable()->ClassIndex());
        otava::symbols::BoundFunctionNode* initFn = context->GetBoundCompileUnit()->GetCompileUnitInitializationFunction();
        if (initFn)
        {
            compileUnitInitFunctionNames.push_back(initFn->GetFunctionDefinitionSymbol()->IrName(context.get()));
        }
        totalFunctionsCompiled += context->TotalFunctionsCompiled();
        invokes += context->Invokes();
        unresolvedInvokes += context->UnresolvedInvokes();
        if (context->ReleaseConfig() && otava::optimizer::HasOptimization(otava::optimizer::Optimizations::inlining))
        {
            inliningEnabled = true;
            functionsInlined += context->FunctionsInlined();
            functionCallsInlined += context->FunctionCallsInlined();
        }
        context.reset();
        std::string ftabFilePath = util::GetFullPath(util::Path::Combine(moduleDir, util::Path::GetFileName(filePath) + ".fns.txt"));
        module->GetFunctionTable().Write(ftabFilePath);
        std::string scountFilePath = util::GetFullPath(util::Path::Combine(moduleDir, util::Path::GetFileName(filePath) + ".scount"));
        module->GetSymbolCounts().Save(scountFilePath);
        moduleMapper.RemoveModule(module.get());
        moduleMapper.AddModuleInfo(module.get());
        builtModules.push_back(std::move(module));
    }
    project->WriteTraceInfo(moduleDirPath);
    project->WriteClassIndex(moduleDirPath);
    ProjectTarget projectTarget = ProjectTarget::library;
    std::string classIndexFilePath;
    std::string traceBinPath;
    if (project->GetTarget() == Target::library)
    {
        std::string compileUnitInitFileName = util::Path::Combine(project->RootDir(), "cu.init");
        util::FileStream compileUnitInitFile(compileUnitInitFileName, util::OpenMode::write | util::OpenMode::binary);
        util::BinaryStreamWriter writer(compileUnitInitFile);
        std::int32_t n = compileUnitInitFunctionNames.size();
        writer.Write(n);
        for (const auto& fileName : compileUnitInitFunctionNames)
        {
            writer.Write(fileName);
        }
    }
    else if (project->GetTarget() == Target::program)
    {
        cppFileNames.push_back(cppFileName);
        libraryDirs = util::GetFullPath(util::Path::Combine(util::OtavaRoot(), "lib"));
        if (configurations.find("release") != configurations.end())
        {
#ifdef OTAVA
            libraryDirs.append(";").append(util::GetFullPath(util::Path::Combine(util::Path::Combine(
                util::Path::Combine(util::OtavaRoot(), "ooc/std/bin"), config),
                std::to_string(otava::symbols::GetOptLevel(optLevel, true)))));
#else
            libraryDirs.append(";").append(util::GetFullPath(util::Path::Combine(util::Path::Combine(
                util::Path::Combine(util::OtavaRoot(), "std/bin"), config),
                std::to_string(otava::symbols::GetOptLevel(optLevel, true)))));
#endif
        }
        else
        {
#ifdef OTAVA
            libraryDirs.append(";").append(util::GetFullPath(util::Path::Combine(util::Path::Combine(util::OtavaRoot(), "ooc/std/bin"), config)));
#else
            libraryDirs.append(";").append(util::GetFullPath(util::Path::Combine(util::Path::Combine(util::OtavaRoot(), "std/bin"), config)));
#endif
        }
        if (mainFunctionIrName.empty())
        {
            otava::symbols::SetExceptionThrown();
            throw std::runtime_error("program has no main function");
        }
        for (const auto& fileName : compileUnitInitFunctionNames)
        {
            allCompileUnitInitFunctionNames.push_back(fileName);
        }
        otava::symbols::Context mainUnitContext;
        mainUnitContext.SetModuleMapper(&moduleMapper);
        if (project->HasDefine("NOWARN"))
        {
            mainUnitContext.SetFlag(otava::symbols::ContextFlags::noWarnings);
            mainUnitContext.SetSkipInvokeCheckingSet(&skipInvokeSettingSet);
        }
        std::string mainAsmFileName = GenerateMainUnit(moduleMapper, util::Path::Combine(util::Path::GetDirectoryName(projectFilePath), cppFileName),
            mainFunctionIrName, mainFunctionParams, allCompileUnitInitFunctionNames, config, optLevel, project, configurations, &mainUnitContext);
        asmFileNames.push_back(mainAsmFileName);
        projectTarget = ProjectTarget::program;
        otava::symbols::TraceBin traceBin;
        for (const auto& referencedProject : project->ReferencedProjects())
        {
            bool releaseCfg = configurations.find("release") != configurations.end();
            int olevel = otava::symbols::GetOptLevel(optLevel, releaseCfg);
            std::string moduleDirPath = otava::symbols::MakeModuleDirPath(referencedProject->RootDir(), config, olevel, configurations);
            referencedProject->ReadTraceInfo(moduleDirPath);
            if (referencedProject->HasDefine("TRACE"))
            {
                traceBin.Import(referencedProject->GetTraceInfo());
            }
            referencedProject->ReadClassIndex(moduleDirPath);
            project->GetIndex().import(referencedProject->GetIndex());
        }
        if (project->HasDefine("TRACE"))
        {
            traceBin.Import(project->GetTraceInfo());
            traceBinPath = util::GetFullPath(util::Path::Combine(moduleDirPath, "trace.bin"));
            otava::symbols::WriteTraceBin(traceBin, traceBinPath);
        }
        classIndexFilePath = util::GetFullPath(util::Path::Combine(moduleDirPath, "class_index.bin"));
        WriteClassIndex(classIndexFilePath, project->GetIndex());
    }
    for (const auto& resourceFilePath : project->ResourceFilePaths())
    {
        std::string absoluteRCFilePath = util::GetFullPath(util::Path::Combine(project->RootDir(), resourceFilePath));
        resourceFileNames.push_back(absoluteRCFilePath);
    }
    if ((flags & BuildFlags::verbose) != BuildFlags::none)
    {
        std::cout << totalFunctionsCompiled << " functions compiled\n";
        if (inliningEnabled)
        {
            std::cout << functionCallsInlined << " function calls inlined in " << functionsInlined << " functions\n";
        }
        std::cout << invokes << " invokes generated\n";
        std::cout << unresolvedInvokes << " invokes unresolved\n";
    }
    std::string functionTableFilePath = util::Path::Combine(moduleDirPath, "fns.txt");
    functionTable.Write(functionTableFilePath);
    otava::symbols::SetFunctionTable(nullptr);
    MakeProjectFile(project, projectFilePath, asmFileNames, cppFileNames, resourceFileNames,
        libraryDirs, project->ReferencedProjects(), config, optLevel, classIndexFilePath, traceBinPath, projectTarget,
        (flags & BuildFlags::verbose) != BuildFlags::none, configurations);
    MSBuild(projectFilePath, config, configurations);
    std::string projectFilesPath = util::GetFullPath(util::Path::Combine(project->RootDir(), project->Name() + ".files"));
    WriteFilesFile(projectFilesPath, files);
    std::string modulesFilePath = util::GetFullPath(util::Path::Combine(moduleDirPath, project->Name() + ".modules.txt"));
    std::ofstream modulesTextFile(modulesFilePath);
    if (!modulesTextFile)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("could not create file '" + modulesFilePath + "'");
    }
    moduleMapper.PrintModules(modulesTextFile);
    std::string fnFilesPath = util::GetFullPath(util::Path::Combine(moduleDirPath, "fn_file_paths.txt"));
    if (project->GetTarget() == Target::program)
    {
        WriteFnFilePathsFile(project, config, optLevel, configurations, fnFilesPath);
    }
    otava::symbols::SetCompileEnded();
    std::cout << "project '" << project->Name() << "' built successfully" << "\n";
}

void Visit(Solution* solution, Project* project, std::vector<Project*>& topologicalOrder, std::set<Project*>& visited)
{
    for (const std::string& path : project->ReferenceFilePaths())
    {
        std::string referenceFilePath = util::GetFullPath(util::Path::Combine(project->RootDir(), path));
        Project* dependsOnProject = solution->GetProject(referenceFilePath);
        if (dependsOnProject)
        {
            if (visited.find(dependsOnProject) == visited.cend())
            {
                visited.insert(dependsOnProject);
                Visit(solution, dependsOnProject, topologicalOrder, visited);
            }
        }
        else
        {
            otava::symbols::SetExceptionThrown();
            throw std::runtime_error("reference file path '" + referenceFilePath + "' in solution '" + solution->FilePath() + "' not found");
        }
    }
    visited.insert(project);
    topologicalOrder.push_back(project);
}

std::vector<Project*> MakeTopologicalOrder(Solution* solution)
{
    std::set<Project*> visited;
    std::vector<Project*> topologicalOrder;
    for (Project* project : solution->Projects())
    {
        if (visited.find(project) == visited.cend())
        {
            Visit(solution, project, topologicalOrder, visited);
        }
    }
    return topologicalOrder;
}

void BuildSequentially(soul::lexer::FileMap& fileMap, Solution* solution, const std::string& config, int optLevel, const std::string& optFn, 
    BuildFlags flags, std::set<Project*, ProjectLess>& projectSet, std::ostream* outFile)
{
    if (solution->IsProjectSolution())
    {
        std::cout << "> building project '" << solution->Name() << "' with dependencies..." << "\n";
    }
    else
    {
        std::cout << "> building solution '" << solution->Name() << "'..." << "\n";
    }
    if (!solution->IsProjectSolution())
    {
        std::string root = util::Path::GetDirectoryName(solution->FilePath());
        for (const auto& path : solution->ProjectFilePaths())
        {
            std::string projectFilePath = util::GetFullPath(util::Path::Combine(root, path));
            std::unique_ptr<Project> project = otava::build::ParseProjectFile(projectFilePath);
            project->SetFileMap(&fileMap);
            solution->AddProject(project.release());
        }
    }
    std::vector<Project*> buildOrder = MakeTopologicalOrder(solution);
    for (Project* project : buildOrder)
    {
        Build(fileMap, project, config, optLevel, optFn, flags & ~BuildFlags::all, projectSet, outFile);
    }
    if (!solution->IsProjectSolution())
    {
        std::cout << "solution '" << solution->Name() << "' built successfully" << "\n";
    }
}

void ProjectClosure(soul::lexer::FileMap& fileMap, Project* project, Solution* solution, std::set<std::string>& projectFilePaths)
{
    std::string root = util::Path::GetDirectoryName(project->FilePath());
    for (const auto& referenceFilePath : project->ReferenceFilePaths())
    {
        std::string projectFilePath = util::GetFullPath(util::Path::Combine(root, referenceFilePath));
        if (projectFilePaths.find(projectFilePath) != projectFilePaths.end()) continue;
        projectFilePaths.insert(projectFilePath);
        std::unique_ptr<Project> referencedProject = otava::build::ParseProjectFile(projectFilePath);
        Project* reference = referencedProject.get();
        if (project->Name() != "std" && reference->Name() == "std") continue;
        reference->SetFileMap(&fileMap);
        project->AddReferencedProject(std::move(referencedProject));
        ProjectClosure(fileMap, reference, solution, projectFilePaths);
    }
    solution->AddProject(project, false);
}

void Build(soul::lexer::FileMap& fileMap, Project* project, const std::string& config, int optLevel, const std::string& optFn, 
    BuildFlags flags, std::set<Project*, ProjectLess>& projectSet, std::ostream* outFile)
{
    if ((flags & BuildFlags::all) != BuildFlags::none)
    {
        Solution solution(project->FilePath(), project->Name());
        solution.SetProjectSolution();
        std::set<std::string> projectFilePaths;
        ProjectClosure(fileMap, project, &solution, projectFilePaths);
        Build(fileMap, &solution, config, optLevel, optFn, flags & ~BuildFlags::all, projectSet, outFile);
    }
    else
    {
        if (projectSet.find(project) != projectSet.end()) return;
        projectSet.insert(project);
        BuildSequentially(project, config, optLevel, optFn, flags & ~BuildFlags::all, outFile);
    }
}

void Build(soul::lexer::FileMap& fileMap, Solution* solution, const std::string& config, int optLevel, const std::string& optFn, 
    BuildFlags flags, std::set<Project*, ProjectLess>& projectSet, std::ostream* outFile)
{
    BuildSequentially(fileMap, solution, config, optLevel, optFn, flags & ~BuildFlags::all, projectSet, outFile);
}

} // namespace otava::build
