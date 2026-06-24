// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.build.gen_main_unit;

import otava.codegen;
import otava.symbols.declaration;
import otava.symbols.declarator;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.expression_binder;
import otava.symbols.instantiator;
import otava.symbols.scope;
import otava.symbols.statement_binder;
import otava.ast.declaration;
import otava.ast.expression;
import otava.ast.function;
import otava.ast.identifier;
import otava.ast.punctuation;
import otava.ast.qualifier;
import otava.ast.simple_type;
import otava.ast.statement;
import util.code_formatter;
import util.path;

namespace otava::build {

std::string GenerateMainWrapper(otava::symbols::Context* context, int numParams)
{
    /*  int __main_wrapper(int argc, const char** argv)
    *   {
    *           __global_init__();
    *           @retval = main(argc, argv);
    *           std::run_at_exits();
    *           __global_done__();
    *           return @retval;
    *   }
    */
    otava::ast::IdentifierNode* mainFn = new otava::ast::IdentifierNode(soul::ast::Span(), -1, "main");
    otava::ast::InvokeExprNode* mainFnCall = new otava::ast::InvokeExprNode(soul::ast::Span(), -1, mainFn);
    if (numParams == 2)
    {
        otava::ast::IdentifierNode* argcNode = new otava::ast::IdentifierNode(soul::ast::Span(), -1, "argc");
        mainFnCall->AddNode(argcNode);
        otava::ast::IdentifierNode* argvNode = new otava::ast::IdentifierNode(soul::ast::Span(), -1, "argv");
        mainFnCall->AddNode(argvNode);
    }
    otava::ast::IdentifierNode* retValDeclarator = new otava::ast::IdentifierNode(soul::ast::Span(), -1, "@retval");
    otava::ast::DeclSpecifierSequenceNode* retValDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(soul::ast::Span(), -1);
    retValDeclSpecifiers->AddNode(new otava::ast::IntNode(soul::ast::Span(), -1));
    otava::ast::InitDeclaratorListNode* retValInitDeclarators = new otava::ast::InitDeclaratorListNode(soul::ast::Span(), -1);
    otava::ast::InitDeclaratorNode* retValInitDeclarator = new otava::ast::InitDeclaratorNode(soul::ast::Span(), -1, retValDeclarator, mainFnCall);
    retValInitDeclarators->AddNode(retValInitDeclarator);
    otava::ast::SimpleDeclarationNode* retValDeclaration = new otava::ast::SimpleDeclarationNode(
        soul::ast::Span(), -1, retValDeclSpecifiers, retValInitDeclarators, nullptr, new otava::ast::SemicolonNode(soul::ast::Span(), -1));
    otava::ast::DeclSpecifierSequenceNode* declSpecifiers = new otava::ast::DeclSpecifierSequenceNode(soul::ast::Span(), -1);
    declSpecifiers->AddNode(new otava::ast::IntNode(soul::ast::Span(), -1));
    otava::ast::CompoundStatementNode* body = new otava::ast::CompoundStatementNode(soul::ast::Span(), -1);
    otava::ast::IdentifierNode* globalInitFn = new otava::ast::IdentifierNode(soul::ast::Span(),-1, "__global_init__");
    otava::ast::InvokeExprNode* globalInitFnCall = new otava::ast::InvokeExprNode(soul::ast::Span(), -1, globalInitFn);
    otava::ast::ExpressionStatementNode* globalInitStmt = new otava::ast::ExpressionStatementNode(soul::ast::Span(), -1, globalInitFnCall, nullptr,
        new otava::ast::SemicolonNode(soul::ast::Span(), -1));
    body->AddNode(globalInitStmt);
    body->AddNode(retValDeclaration);
    otava::ast::Node* runAtExitsFn = otava::symbols::MakeTypeNameNodes(soul::ast::FullSpan(), "std::run_at_exits");
    otava::ast::InvokeExprNode* runAtExitsFnCall = new otava::ast::InvokeExprNode(soul::ast::Span(), -1, runAtExitsFn);
    otava::ast::ExpressionStatementNode* runAtExitsStmt = new otava::ast::ExpressionStatementNode(soul::ast::Span(), -1, runAtExitsFnCall, nullptr,
        new otava::ast::SemicolonNode(soul::ast::Span(), -1));
    body->AddNode(runAtExitsStmt);
    otava::ast::IdentifierNode* globalDoneFn = new otava::ast::IdentifierNode(soul::ast::Span(), -1, "__global_done__");
    otava::ast::InvokeExprNode* globalDoneFnCall = new otava::ast::InvokeExprNode(soul::ast::Span(), -1, globalDoneFn);
    otava::ast::ExpressionStatementNode* globalDoneStmt = new otava::ast::ExpressionStatementNode(soul::ast::Span(), -1, globalDoneFnCall, nullptr,
        new otava::ast::SemicolonNode(soul::ast::Span(), -1));
    body->AddNode(globalDoneStmt);
    otava::ast::IdentifierNode* retValVarId = new otava::ast::IdentifierNode(soul::ast::Span(), -1, "@retval");
    otava::ast::ReturnStatementNode* returnRetValStmt = new otava::ast::ReturnStatementNode(soul::ast::Span(), -1, retValVarId, nullptr, nullptr, soul::ast::Span());
    body->AddNode(returnRetValStmt);
    otava::ast::FunctionBodyNode* functionBody = new otava::ast::FunctionBodyNode(soul::ast::Span(), -1, body);
    otava::ast::ParameterListNode* parameters = new otava::ast::ParameterListNode(soul::ast::Span(), -1);
    otava::ast::ParameterNode* argcParam = new otava::ast::ParameterNode(soul::ast::Span(), -1);
    otava::ast::DeclSpecifierSequenceNode* argcDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(soul::ast::Span(), -1);
    argcDeclSpecifiers->AddNode(new otava::ast::IntNode(soul::ast::Span(), -1));
    argcParam->SetDeclSpecifiers(argcDeclSpecifiers);
    argcParam->SetDeclarator(new otava::ast::IdentifierNode(soul::ast::Span(), -1, "argc"));
    parameters->AddNode(argcParam);
    otava::ast::ParameterNode* argvParam = new otava::ast::ParameterNode(soul::ast::Span(), -1);
    otava::ast::DeclSpecifierSequenceNode* argvDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(soul::ast::Span(), -1);
    argvDeclSpecifiers->AddNode(new otava::ast::ConstNode(soul::ast::Span(), -1));
    argvDeclSpecifiers->AddNode(new otava::ast::CharNode(soul::ast::Span(), -1));
    argvParam->SetDeclSpecifiers(argvDeclSpecifiers);
    otava::ast::PtrDeclaratorNode* argvDeclarator = new otava::ast::PtrDeclaratorNode(soul::ast::Span(), -1);
    argvDeclarator->AddNode(new otava::ast::PtrOperatorNode(soul::ast::Span(), -1, new otava::ast::PtrNode(soul::ast::Span(), -1)));
    argvDeclarator->AddNode(new otava::ast::PtrOperatorNode(soul::ast::Span(), -1, new otava::ast::PtrNode(soul::ast::Span(), -1)));
    argvDeclarator->AddNode(new otava::ast::IdentifierNode(soul::ast::Span(), -1, "argv"));
    argvParam->SetDeclarator(argvDeclarator);
    parameters->AddNode(argvParam);
    otava::ast::FunctionDeclaratorNode* declarator = new otava::ast::FunctionDeclaratorNode(soul::ast::Span(), -1,
        new otava::ast::IdentifierNode(soul::ast::Span(), -1, "__main_wrapper__"), parameters);
    std::unique_ptr<otava::ast::FunctionDefinitionNode> mainWrapperFn(new otava::ast::FunctionDefinitionNode(soul::ast::Span(), -1, nullptr, declSpecifiers, 
        declarator, nullptr, functionBody));
    otava::symbols::InstantiationScope instantiationScope(context->GetModule(), context->GetSymbolTable()->CurrentScope());
    otava::symbols::Instantiator instantiator(context, &instantiationScope);
    context->PushSetFlag(otava::symbols::ContextFlags::saveDeclarations | otava::symbols::ContextFlags::dontBind | otava::symbols::ContextFlags::generateMainWrapper);
    instantiator.SetFunctionNode(mainWrapperFn.get());
    mainWrapperFn->Accept(instantiator);
    context->PopFlags();
    otava::symbols::FunctionSymbol* mainWrapperFnSymbol = instantiator.GetSpecialization();
    if (mainWrapperFnSymbol && mainWrapperFnSymbol->IsFunctionDefinitionSymbol())
    {
        otava::symbols::FunctionDefinitionSymbol* mainWrapperFnDefSymbol = static_cast<otava::symbols::FunctionDefinitionSymbol*>(mainWrapperFnSymbol);
        context->PushBoundFunction(new otava::symbols::BoundFunctionNode(mainWrapperFnDefSymbol, soul::ast::FullSpan()));
        mainWrapperFnDefSymbol = otava::symbols::BindFunction(mainWrapperFn.get(), mainWrapperFnDefSymbol, context);
        context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<otava::symbols::BoundNode>(context->ReleaseBoundFunction()), context);
        context->PopBoundFunction();
        return mainWrapperFnDefSymbol->IrName(context);
    }
    else
    {
        otava::symbols::ThrowException("error instantiating main wrapper: function definition symbol expected", soul::ast::FullSpan(), context);
    }
    return std::string();
}

std::string GenerateMainUnit(otava::symbols::ModuleMapper& moduleMapper, const std::string& mainFilePath, const std::string& mainFunctionIrName, int numParams,
    const std::vector<std::string>& compileUnitInitFnNames, const std::string& config, int optLevel, Project* project, const std::set<std::string>& configurations,
    otava::symbols::Context* parentContext)
{
    otava::symbols::Module* std = moduleMapper.GetModule("std", config, optLevel, configurations, parentContext);
    otava::symbols::Module main("main");
    main.SetId(moduleMapper.GetNextModuleId());
    moduleMapper.MapModule(&main);
    main.AddImportedModuleName(std->Name());
    otava::symbols::Emitter emitter;
    otava::symbols::Context context;
    context.SetModuleMapper(&moduleMapper);
    context.SetEmitter(&emitter);
    context.SetCurrentProject(project);
    context.SetModule(&main);
    main.Init(&context);
    //main.Import(std, moduleMapper, config, optLevel, configurations, parentContext); TODO?
    //std::unique_ptr<otava::symbols::SymbolTable> symbolTable(new otava::symbols::SymbolTable(&main, false));
    //symbolTable->Import(*std->GetSymbolTable(), moduleMapper.GetFunctionDefinitionSymbolSet());
    //context.SetFunctionDefinitionSymbolSet(moduleMapper.GetFunctionDefinitionSymbolSet());
    if (project->HasDefine("DEBUG_MEMORY"))
    {
        context.SetFlag(otava::symbols::ContextFlags::debugMemory);
    }
    if (configurations.find("release") != configurations.end())
    {
        std::string fp = util::GetFullPath(util::Path::Combine(util::Path::GetDirectoryName(util::Path::GetDirectoryName(
            util::Path::GetDirectoryName(util::Path::GetDirectoryName(mainFilePath)))), util::Path::GetFileName(mainFilePath)));
        context.SetFileName(fp);
    }
    else
    {
        std::string fp = util::GetFullPath(util::Path::Combine(util::Path::GetDirectoryName(util::Path::GetDirectoryName(util::Path::GetDirectoryName(mainFilePath))),
            util::Path::GetFileName(mainFilePath)));
        context.SetFileName(fp);
    }
    context.GetBoundCompileUnit()->SetId("main_unit");
    otava::symbols::FunctionSymbol* globalInitFn = context.GetSymbolTable()->AddFunction("__global_init__", std::vector<otava::symbols::TypeSymbol*>(), nullptr,
        otava::symbols::FunctionKind::function, otava::symbols::FunctionQualifiers::none, otava::symbols::DeclarationFlags::none, &context);
    globalInitFn->SetLinkage(otava::symbols::Linkage::c_linkage);
    globalInitFn->SetGenerated();
    otava::symbols::FunctionSymbol* globalDoneFn = context.GetSymbolTable()->AddFunction("__global_done__", std::vector<otava::symbols::TypeSymbol*>(), nullptr,
        otava::symbols::FunctionKind::function, otava::symbols::FunctionQualifiers::none, otava::symbols::DeclarationFlags::none, &context);
    globalDoneFn->SetLinkage(otava::symbols::Linkage::c_linkage);
    globalDoneFn->SetGenerated();
    otava::symbols::FunctionSymbol* mainFn = context.GetSymbolTable()->AddFunction("main", std::vector<otava::symbols::TypeSymbol*>(), nullptr,
        otava::symbols::FunctionKind::function, otava::symbols::FunctionQualifiers::none, otava::symbols::DeclarationFlags::none, &context);
    otava::symbols::TypeSymbol* intType = context.GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        otava::symbols::FundamentalTypeKind::intType, &context);
    mainFn->SetReturnType(intType, &context);
    if (numParams == 2)
    {
        otava::symbols::TypeSymbol* intType = 
            context.GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(otava::symbols::FundamentalTypeKind::intType, &context);
        otava::symbols::ParameterSymbol* argcParam = context.GetSymbolTable()->CreateParameter("argc", nullptr, intType, &context);
        mainFn->AddSymbol(argcParam, soul::ast::FullSpan(), &context);
        otava::symbols::TypeSymbol* constCharPtrPtrType = context.GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
            otava::symbols::FundamentalTypeKind::charType, &context)->AddConst(&context)->AddPointer(&context)->AddPointer(&context);
        otava::symbols::ParameterSymbol* argvParam = context.GetSymbolTable()->CreateParameter("argv", nullptr, constCharPtrPtrType, &context);
        mainFn->AddSymbol(argvParam, soul::ast::FullSpan(), &context);
    }
    std::string mainWrapperIrName = GenerateMainWrapper(&context, numParams);
    int np = numParams;
    std::string asmFilename = otava::codegen::GenerateCode(context, config, optLevel, true, mainWrapperIrName, np, true, compileUnitInitFnNames, configurations);
    std::ofstream mainFile(mainFilePath);
    if (!mainFile)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("could not create file '" + mainFilePath + "'");
    }
    util::CodeFormatter formatter(mainFile);
    formatter.WriteLine("extern \"C\" void ort_init();");
    formatter.WriteLine("extern \"C\" void ort_done();");
    formatter.WriteLine("extern \"C\" void ort_preprocess_args(int argc, const char** argv, int* pargc, const char*** pargv);");
    if (context.GetFlag(otava::symbols::ContextFlags::debugMemory))
    {
        formatter.WriteLine("extern \"C\" void ort_debug_memory();");
        formatter.WriteLine("extern \"C\" void ort_print_memory_leaks();");
    }
    formatter.WriteLine("extern \"C\" int " + mainWrapperIrName + "(int argc, const char** argv); ");
    formatter.WriteLine();
    formatter.WriteLine("int main(int argc, const char** argv)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    if (numParams == 0 || numParams == 2)
    {
        formatter.WriteLine("ort_init();");
        if (context.GetFlag(otava::symbols::ContextFlags::debugMemory))
        {
            formatter.WriteLine("ort_debug_memory();");
        }
        formatter.WriteLine("int pargc;");
        formatter.WriteLine("const char** pargv;");
        formatter.WriteLine("ort_preprocess_args(argc, argv, &pargc, &pargv);");
        formatter.WriteLine("int retVal = " + mainWrapperIrName + "(pargc, pargv);");
        formatter.WriteLine("ort_done();");
        if (context.GetFlag(otava::symbols::ContextFlags::debugMemory))
        {
            formatter.WriteLine("ort_print_memory_leaks();");
        }
    }
    else
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid number of main function parameters");
    }
    formatter.WriteLine("return retVal;");
    formatter.DecIndent();
    formatter.WriteLine("}");
    return asmFilename;
}

} // namespace otava::build
