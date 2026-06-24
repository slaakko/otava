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
import otava.symbols.class_info;
import otava.symbols.context;
import otava.symbols.declarator;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.statement_binder;
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
import util.path;
import util.unicode;
import util.binary_stream_reader;
import util.binary_stream_writer;
import util.buffered_stream;
import util.file_stream;
import util.sha1;

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
        m->AddImportedModuleName("std.type.fundamental");
        m->AddImportedModuleName(node.Str());
    }
}

void ScanDependencies(Project* project, std::int32_t fileId, bool implementationUnit, std::string& interfaceUnitName, otava::symbols::Context* context)
{
    std::string filePath = project->GetFileMap()->GetFilePath(fileId);
    std::string fileName = util::Path::GetFileNameWithoutExtension(filePath);
    std::string fileContent = util::ReadFile(filePath);
    std::u32string content = util::ToUtf32(fileContent);
    auto lexer = otava::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), filePath);
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

void ReadModuleCountFile(const std::string& moduleCountFilePath, otava::symbols::ModuleMapper& moduleMapper)
{
    util::FileStream moduleCountFile(moduleCountFilePath, util::OpenMode::read | util::OpenMode::binary);
    util::BinaryStreamReader reader(moduleCountFile);
    otava::symbols::Cardinality moduleCount(otava::symbols::Cardinality(reader.ReadUInt()));
    moduleMapper.SetNextModuleId(moduleMapper.NextModuleId() + moduleCount);
}

void WriteModuleCountFile(const std::string& moduleCountFilePath, otava::symbols::ModuleMapper& moduleMapper)
{
    util::FileStream moduleCountFile(moduleCountFilePath, util::OpenMode::write | util::OpenMode::binary);
    util::BinaryStreamWriter writer(moduleCountFile);
    writer.Write(otava::symbols::ToUnderlying(moduleMapper.ModuleCount()));
}

void BuildSequentially(Project* project, const std::string& config, int optLevel, BuildFlags flags, std::ostream* outFile)
{
    otava::symbols::ResetCompileEnded();
    otava::symbols::ModuleMapper moduleMapper;
    otava::ast::NodeIdFactory nodeIdFactory;
    otava::ast::SetNodeIdFactory(&nodeIdFactory);
    std::set<std::string> configurations;
    ConfigureProject(project, config, (flags & BuildFlags::verbose) != BuildFlags::none, configurations);
    bool releaseCfg = configurations.find("release") != configurations.end();
    optLevel = otava::symbols::GetOptLevel(optLevel, releaseCfg);
    std::string moduleDirPath = otava::symbols::MakeModuleDirPath(project->Root(), config, optLevel, configurations);
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
    project->InitModules();
    std::vector<std::string> asmFileNames;
    std::vector<std::string> cppFileNames;
    std::vector<std::string> resourceFileNames;
    std::vector<std::string> compileUnitInitFunctionNames;
    std::vector<std::string> allCompileUnitInitFunctionNames;
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
        std::string projectFilePath = util::GetFullPath(util::Path::Combine(project->Root(), path));
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
        project->AddReferencedProject(projectReference.release());
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
    moduleContext.GetSymbolIndexMap()->Import(*scanContext.GetSymbolIndexMap());
    if (project->HasDefine("NOWARN"))
    {
        moduleContext.SetFlag(otava::symbols::ContextFlags::noWarnings);
    }
    project->LoadModules(moduleMapper, config, optLevel, configurations, &moduleContext);
    otava::symbols::Index importIndex = otava::symbols::Index(moduleMapper.ModuleCount());
    if (project->GetTarget() == Target::program)
    {
        for (const auto& referenceFilePath : project->ReferenceFilePaths())
        {
            std::string cuInitFileName = util::GetFullPath(util::Path::Combine(project->Root(),
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
        std::string referenceFilesPath = util::GetFullPath(util::Path::Combine(reference->Root(), reference->Name() + ".files"));
        ReadFilesFile(referenceFilesPath, project->GetFileMap());
        std::string countFilePath = util::GetFullPath(util::Path::Combine(
            otava::symbols::MakeModuleDirPath(reference->Root(), config, optLevel, configurations), reference->Name() + ".count"));
        ReadModuleCountFile(countFilePath, moduleMapper);
    }
    std::vector<std::pair<std::int32_t, std::string>> files;
    std::vector<std::int32_t> topologicalOrder = MakeTopologicalOrder(project->InterfaceFiles(), project);
    soul::lexer::FileMap* fileMap = project->GetFileMap();
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
        context->SetCurrentProject(project);
        std::string compileUnitId = "compile_unit_" + util::GetSha1MessageDigest(filePath);
        context->GetBoundCompileUnit()->SetId(compileUnitId);
        context->GetSymbolIndexMap()->Import(*moduleContext.GetSymbolIndexMap());
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
        std::unique_ptr<otava::symbols::Module> module = project->ReleaseModule(file);
        context->SetModule(module.get());
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
        module->ResetNode(translationUnitNode.release());
        module->SetImplementationUnitNames(implementationNameMap[module->Name()]);
        project->Index().import(module->GetSymbolTable()->ClassIndex());
        module->Write(project->Root(), config, optLevel, context.get(), configurations);
        context.reset();
        if (project->GetTarget() == Target::library)
        {
            moduleMapper.RemoveModule(module.get());
        }
    }
    for (std::int32_t file : project->SourceFiles())
    {
        std::string filePath = fileMap->GetFilePath(file);
        if (filePath == "D:/work/otava/std/std.filesystem.cpp")
        {
            int x = 0;
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
        std::unique_ptr<otava::symbols::Module> module = project->ReleaseModule(file);
        context->SetModule(module.get());
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
        project->Index().import(module->GetSymbolTable()->ClassIndex());
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
        if (project->GetTarget() == Target::library)
        {
            moduleMapper.RemoveModule(module.get());
        }
    }
    project->WriteTraceInfo(moduleDirPath);
    project->WriteClassIndex(moduleDirPath);
    ProjectTarget projectTarget = ProjectTarget::library;
    std::string classIndexFilePath;
    std::string traceBinPath;
    if (project->GetTarget() == Target::library)
    {
        std::string compileUnitInitFileName = util::Path::Combine(project->Root(), "cu.init");
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
            libraryDirs.append(";").append(util::GetFullPath(util::Path::Combine(util::Path::Combine(
                util::Path::Combine(util::OtavaRoot(), "std / bin"), config),
                std::to_string(otava::symbols::GetOptLevel(optLevel, true)))));
        }
        else
        {
            libraryDirs.append(";").append(util::GetFullPath(util::Path::Combine(util::Path::Combine(util::OtavaRoot(), "std/bin"), config)));
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
            std::string moduleDirPath = otava::symbols::MakeModuleDirPath(referencedProject->Root(), config, olevel, configurations);
            referencedProject->ReadTraceInfo(moduleDirPath);
            if (referencedProject->HasDefine("TRACE"))
            {
                traceBin.Import(referencedProject->GetTraceInfo());
            }
            referencedProject->ReadClassIndex(moduleDirPath);
            project->Index().import(referencedProject->Index());
        }
        if (project->HasDefine("TRACE"))
        {
            traceBin.Import(project->GetTraceInfo());
            traceBinPath = util::GetFullPath(util::Path::Combine(moduleDirPath, "trace.bin"));
            otava::symbols::WriteTraceBin(traceBin, traceBinPath);
        }
        classIndexFilePath = util::GetFullPath(util::Path::Combine(moduleDirPath, "class_index.bin"));
        WriteClassIndex(classIndexFilePath, project->Index()); 
    }
    for (const auto& resourceFilePath : project->ResourceFilePaths())
    {
        std::string absoluteRCFilePath = util::GetFullPath(util::Path::Combine(project->Root(), resourceFilePath));
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
    MakeProjectFile(project, projectFilePath, asmFileNames, cppFileNames, resourceFileNames,
        libraryDirs, project->ReferencedProjects(), config, optLevel, classIndexFilePath, traceBinPath, projectTarget,
        (flags& BuildFlags::verbose) != BuildFlags::none, configurations);
    MSBuild(projectFilePath, config, configurations);
    std::string projectFilesPath = util::GetFullPath(util::Path::Combine(project->Root(), project->Name() + ".files"));
    WriteFilesFile(projectFilesPath, files);
    std::string countFilePath = util::GetFullPath(util::Path::Combine(moduleDirPath, project->Name() + ".count"));
    WriteModuleCountFile(countFilePath, moduleMapper);
    otava::symbols::SetCompileEnded();
    std::cout << "project '" << project->Name() << "' built successfully" << std::endl;
}

void Visit(Solution* solution, Project* project, std::vector<Project*>& topologicalOrder, std::set<Project*>& visited)
{
    for (const std::string& path : project->ReferenceFilePaths())
    {
        std::string referenceFilePath = util::GetFullPath(util::Path::Combine(project->Root(), path));
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

void BuildSequentially(soul::lexer::FileMap& fileMap, Solution* solution, const std::string& config, int optLevel, BuildFlags flags,
    std::set<Project*, ProjectLess>& projectSet, std::ostream* outFile)
{
    if (solution->IsProjectSolution())
    {
        std::cout << "> building project '" << solution->Name() << "' with dependencies..." << std::endl;
    }
    else
    {
        std::cout << "> building solution '" << solution->Name() << "'..." << std::endl;
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
        Build(fileMap, project, config, optLevel, flags & ~BuildFlags::all, projectSet, outFile);
    }
    if (!solution->IsProjectSolution())
    {
        std::cout << "solution '" << solution->Name() << "' built successfully" << std::endl;
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
        project->AddReferencedProject(referencedProject.release());
        ProjectClosure(fileMap, reference, solution, projectFilePaths);
    }
    solution->AddProject(project, false);
}

void Build(soul::lexer::FileMap& fileMap, Project* project, const std::string& config, int optLevel, BuildFlags flags, std::set<Project*, ProjectLess>& projectSet,
    std::ostream* outFile)
{
    if ((flags & BuildFlags::all) != BuildFlags::none)
    {
        Solution solution(project->FilePath(), project->Name());
        solution.SetProjectSolution();
        std::set<std::string> projectFilePaths;
        ProjectClosure(fileMap, project, &solution, projectFilePaths);
        Build(fileMap, &solution, config, optLevel, flags & ~BuildFlags::all, projectSet, outFile);
    }
    else
    {
        if (projectSet.find(project) != projectSet.end()) return;
        projectSet.insert(project);
        BuildSequentially(project, config, optLevel, flags & ~BuildFlags::all, outFile);
    }
}

void Build(soul::lexer::FileMap& fileMap, Solution* solution, const std::string& config, int optLevel, BuildFlags flags, std::set<Project*, ProjectLess>& projectSet,
    std::ostream* outFile)
{
    BuildSequentially(fileMap, solution, config, optLevel, flags & ~BuildFlags::all, projectSet, outFile);
}

} // namespace otava::build
