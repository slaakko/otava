// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.statement_binder;

import otava.symbols.argument_conversion_table;
import otava.symbols.bound_tree;
import otava.symbols.context;
import otava.symbols.declaration;
import otava.symbols.declarator;
import otava.symbols.exception;
import otava.symbols.exception_handling;
import otava.symbols.expr_parser;
import otava.symbols.expression_binder;
import otava.symbols.function_return_path_checker;
import otava.symbols.instantiator;
import otava.symbols.operation_repository;
import otava.symbols.overload_resolution;
import otava.symbols.stmt_parser;
import otava.symbols.trace;
import otava.symbols.type_compare;
import otava.symbols.type_resolver;
import otava.ast.classes;
import otava.ast.declaration;
import otava.ast.expression;
import otava.ast.function;
import otava.ast.identifier;
import otava.ast.literal;
import otava.ast.qualifier;
import otava.ast.simple_type;
import otava.ast.statement;
import otava.ast.templates;
import otava.ast.type;
import util.sha1;

namespace otava::symbols {

struct InitializerLess
{
    bool operator()(const std::pair<int, std::unique_ptr<BoundFunctionCallNode>>& left, const std::pair<int, std::unique_ptr<BoundFunctionCallNode>>& right) const noexcept;
};

bool InitializerLess::operator()(const std::pair<int, std::unique_ptr<BoundFunctionCallNode>>& left,
    const std::pair<int, std::unique_ptr<BoundFunctionCallNode>>& right) const noexcept
{
    return left.first < right.first;
}

struct TerminatorGreater
{
    bool operator()(const std::pair<int, std::unique_ptr<BoundFunctionCallNode>>& left, const std::pair<int, std::unique_ptr<BoundFunctionCallNode>>& right) const noexcept;
};

bool TerminatorGreater::operator()(const std::pair<int, std::unique_ptr<BoundFunctionCallNode>>& left,
    const std::pair<int, std::unique_ptr<BoundFunctionCallNode>>& right) const noexcept
{
    return left.first > right.first;
}

constexpr int maxBaseInitializers = 10000;

BoundExpressionNode* MakeBoundBooleanConversionNode(BoundExpressionNode* condition, Context* context)
{
    soul::ast::FullSpan fullSpan = condition->GetFullSpan();
    FunctionSymbol* conversionFunction = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
        context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
            otava::symbols::FundamentalTypeKind::boolType, context), condition->GetType(), fullSpan, context);
    if (!conversionFunction)
    {
        ThrowException("condition must be convertible to Boolean type value", fullSpan, context);
    }
    if (conversionFunction->GetFunctionKind() == FunctionKind::conversionMemFn && condition->GetType()->PlainType(context)->IsClassTypeSymbol())
    {
        TypeSymbol* conditionType = condition->GetType();
        if (conditionType->IsReferenceType())
        {
            condition = new BoundRefToPtrNode(condition, fullSpan, conditionType->RemoveReference(context)->AddPointer(context));
        }
        else
        {
            condition = new BoundAddressOfNode(condition, fullSpan, conditionType->GetBaseType(context)->AddPointer(context));
        }
        BoundFunctionCallNode* functionCall = new BoundFunctionCallNode(conversionFunction, fullSpan, conversionFunction->ReturnType(context));
        functionCall->AddArgument(condition);
        return functionCall;
    }
    return new BoundConversionNode(condition, conversionFunction, fullSpan, conversionFunction->ReturnType(context));
}

StatementBinder::StatementBinder(Context* context_, FunctionDefinitionSymbol* functionDefinitionSymbol_) :
    context(context_),
    currentClass(nullptr),
    hasCtorInitializer(false),
    ctorInitializer(),
    dtorTerminator(),
    boundStatement(),
    functionDefinitionSymbol(functionDefinitionSymbol_),
    classTypeSymbol(nullptr),
    memberVariableSymbol(nullptr),
    resolveClass(false),
    resolveMemberVariable(false),
    resolveInitializerArguments(false),
    setVPtrStatementsGenerated(false),
    setLineCodeGenerated(false),
    postfix(false),
    globalStaticVariableSymbol(nullptr),
    catchBlock(nullptr),
    lastElse(nullptr),
    handlerBlock(nullptr)
{
}

void StatementBinder::Visit(otava::ast::FunctionDefinitionNode& node)
{
    Symbol* symbol = context->GetSymbolTable()->GetSymbol(&node);
    SpecialFunctionKind specialFunctionKind = functionDefinitionSymbol->GetSpecialFunctionKind(context);
    switch (specialFunctionKind)
    {
    case SpecialFunctionKind::defaultCtor:
    case SpecialFunctionKind::copyCtor:
    case SpecialFunctionKind::moveCtor:
    {
        functionDefinitionSymbol->SetFunctionKind(FunctionKind::constructor);
        break;
    }
    case SpecialFunctionKind::dtor:
    {
        functionDefinitionSymbol->SetFunctionKind(FunctionKind::destructor);
        break;
    }
    }
    if (node.FunctionBody()->IsDefaultedOrDeletedFunctionNode())
    {
        otava::ast::DefaultedOrDeletedFunctionNode* bodyNode = static_cast<otava::ast::DefaultedOrDeletedFunctionNode*>(node.FunctionBody());
        if (bodyNode->DefaultOrDelete()->Kind() == otava::ast::NodeKind::defaultNode)
        {
            functionDefinitionSymbol->SetFunctionQualifiers(functionDefinitionSymbol->Qualifiers() | FunctionQualifiers::isDefault);
            ClassTypeSymbol* classType = symbol->ParentClassType(context);
            if (classType)
            {
                switch (specialFunctionKind)
                {
                case SpecialFunctionKind::defaultCtor:
                {
                    int defIndex = functionDefinitionSymbol->DefIndex();
                    functionDefinitionSymbol = GenerateClassDefaultCtor(classType, node.GetFullSpan(), context);
                    functionDefinitionSymbol->SetDefIndex(defIndex);
                    break;
                }
                case SpecialFunctionKind::copyCtor:
                {
                    int defIndex = functionDefinitionSymbol->DefIndex();
                    functionDefinitionSymbol = GenerateClassCopyCtor(classType, node.GetFullSpan(), context);
                    functionDefinitionSymbol->SetDefIndex(defIndex);
                    break;
                }
                case SpecialFunctionKind::moveCtor:
                {
                    int defIndex = functionDefinitionSymbol->DefIndex();
                    functionDefinitionSymbol = GenerateClassMoveCtor(classType, node.GetFullSpan(), context);
                    functionDefinitionSymbol->SetDefIndex(defIndex);
                    break;
                }
                case SpecialFunctionKind::copyAssignment:
                {
                    int defIndex = functionDefinitionSymbol->DefIndex();
                    functionDefinitionSymbol = GenerateClassCopyAssignment(classType, node.GetFullSpan(), context);
                    functionDefinitionSymbol->SetDefIndex(defIndex);
                    break;
                }
                case SpecialFunctionKind::moveAssignment:
                {
                    int defIndex = functionDefinitionSymbol->DefIndex();
                    functionDefinitionSymbol = GenerateClassMoveAssignment(classType, node.GetFullSpan(), context);
                    functionDefinitionSymbol->SetDefIndex(defIndex);
                    break;
                }
                }
                std::unique_ptr<BoundNode> boundFunction(context->ReleaseBoundFunction());
                context->GetBoundCompileUnit()->AddBoundNode(std::move(boundFunction), context);
                context->PopBoundFunction();
            }
        }
        else if (bodyNode->DefaultOrDelete()->Kind() == otava::ast::NodeKind::deleteNode)
        {
            functionDefinitionSymbol->SetFunctionQualifiers(functionDefinitionSymbol->Qualifiers() | FunctionQualifiers::isDeleted);
        }
        return;
    }
    currentClass = symbol->ParentClassType(context);
    context->GetSymbolTable()->BeginScopeGeneric(symbol->GetScope(), context);
    node.FunctionBody()->Accept(*this);
    if (functionDefinitionSymbol->GetFunctionKind() == FunctionKind::constructor)
    {
        currentClass->SetHasUserDefinedConstructor();
        if (!hasCtorInitializer)
        {
            GenerateDefaultCtorInitializer(node.GetFullSpan());
        }
    }
    else if (functionDefinitionSymbol->GetFunctionKind() == FunctionKind::destructor)
    {
        currentClass->SetHasUserDefinedDestructor();
        GenerateDestructorTerminator(node.GetFullSpan());
    }
    context->GetSymbolTable()->EndScopeGeneric(context);
    functionDefinitionSymbol->CheckGenerateClassCopyCtor(node.GetFullSpan(), context);
}

void StatementBinder::Visit(otava::ast::ConstructorNode& node)
{
    node.Left()->Accept(*this);
    if (ctorInitializer)
    {
        context->GetBoundFunction()->SetCtorInitializer(ctorInitializer.release());
        hasCtorInitializer = true;
    }
    node.Right()->Accept(*this);
    BoundCompoundStatementNode* body = static_cast<BoundCompoundStatementNode*>(boundStatement.release());
    context->GetBoundFunction()->SetBody(body);
    AddConvertingConstructorToConversionTable(functionDefinitionSymbol, node.GetFullSpan(), context);
}

void StatementBinder::Visit(otava::ast::ConstructorInitializerNode& node)
{
    ctorInitializer.reset(new BoundCtorInitializerNode(node.GetFullSpan()));
    node.GetMemberInitializerListNode()->Accept(*this);
}

void StatementBinder::GenerateDefaultCtorInitializer(const soul::ast::FullSpan& fullSpan)
{
    ctorInitializer.reset(new BoundCtorInitializerNode(fullSpan));
    CompleteBaseInitializers(fullSpan);
    std::sort(baseInitializers.begin(), baseInitializers.end(), InitializerLess());
    for (auto& initializer : baseInitializers)
    {
        ctorInitializer->AddBaseInitializer(initializer.second.release());
    }
    GenerateSetVPtrStatements(fullSpan);
    for (auto& setVPtrStatement : setVPtrStatements)
    {
        ctorInitializer->AddSetVPtrStatement(setVPtrStatement.release());
    }
    setVPtrStatements.clear();
    CompleteMemberInitializers(fullSpan);
    std::sort(memberInitializers.begin(), memberInitializers.end(), InitializerLess());
    for (auto& initializer : memberInitializers)
    {
        ctorInitializer->AddMemberInitializer(initializer.second.release());
    }
    context->GetBoundFunction()->SetCtorInitializer(ctorInitializer.release());
}

void StatementBinder::GenerateDestructorTerminator(const soul::ast::FullSpan& fullSpan)
{
    dtorTerminator.reset(new BoundDtorTerminatorNode(fullSpan));
    GenerateSetVPtrStatements(fullSpan);
    for (auto& setVPtrStatement : setVPtrStatements)
    {
        dtorTerminator->AddSetVPtrStatement(setVPtrStatement.release());
    }
    setVPtrStatements.clear();
    GenerateBaseTerminators(fullSpan);
    GenerateMemberTerminators(fullSpan);
    std::sort(memberTerminators.begin(), memberTerminators.end(), TerminatorGreater());
    for (auto& terminator : memberTerminators)
    {
        dtorTerminator->AddMemberTerminator(terminator.second.release());
    }
    context->GetBoundFunction()->SetDtorTerminator(dtorTerminator.release());
}

void StatementBinder::GenerateBaseTerminators(const soul::ast::FullSpan& fullSpan)
{
    if (currentClass)
    {
        for (ClassTypeSymbol* baseClass : currentClass->BaseClasses(context))
        {
            int index = GetBaseInitializerOrTerminatorIndex(baseClass);
            AddBaseTerminator(baseClass, index, fullSpan);
        }
    }
}

void StatementBinder::AddBaseTerminator(TypeSymbol* baseClass, int index, const soul::ast::FullSpan& fullSpan)
{
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    BoundExpressionNode* thisPtr = context->GetThisPtr(fullSpan);
    OperationFlags preConversionFlags = OperationFlags::none;
    FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
        baseClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
    if (conversion)
    {
        args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan, conversion->ReturnType(context))));
        std::vector<TypeSymbol*> templateArgs;
        std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverloadThrow(
            context->GetSymbolTable()->CurrentScope(), "@destructor", templateArgs, args, fullSpan, context);
        std::pair<int, std::unique_ptr<BoundFunctionCallNode>> p(std::move(index), std::move(boundFunctionCall));
        memberTerminators.push_back(std::move(p));
    }
    else
    {
        ThrowException("base class conversion not found", fullSpan, context);
    }
}

void StatementBinder::GenerateMemberTerminators(const soul::ast::FullSpan& fullSpan)
{
    if (currentClass)
    {
        for (VariableSymbol* mermberVar : currentClass->MemberVariables(context))
        {
            AddMemberTerminator(mermberVar, fullSpan);
        }
    }
}

void StatementBinder::AddMemberTerminator(VariableSymbol* memberVar, const soul::ast::FullSpan& fullSpan)
{
    TypeSymbol* memberVarType = memberVar->GetType(context);
    if (memberVarType->IsPointerType() || memberVarType->IsReferenceType()) return;
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    BoundVariableNode* boundVariableNode = new BoundVariableNode(memberVar, fullSpan, memberVar->GetReferredType(context));
    BoundExpressionNode* thisPtr = context->GetThisPtr(fullSpan);
    boundVariableNode->SetThisPtr(thisPtr);
    args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundVariableNode, fullSpan, boundVariableNode->GetType()->AddPointer(context))));
    Exception ex;
    std::vector<TypeSymbol*> templateArgs;
    std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverload(
        context->GetSymbolTable()->CurrentScope(), "@destructor", templateArgs, args, fullSpan, context, ex);
    if (boundFunctionCall)
    {
        if (boundFunctionCall->GetFunctionSymbol()->IsVirtual() || boundFunctionCall->GetFunctionSymbol()->IsOverride() ||
            boundFunctionCall->GetFunctionSymbol()->IsFinal())
        {
            boundFunctionCall->SetFlag(BoundExpressionFlags::virtualCall);
        }
        std::pair<int, std::unique_ptr<BoundFunctionCallNode>> p(std::move(memberVar->GetIndex()), std::move(boundFunctionCall));
        memberTerminators.push_back(std::move(p));
    }
}

void StatementBinder::GenerateSetVPtrStatements(const soul::ast::FullSpan& fullSpan)
{
    if (!currentClass || !currentClass->IsPolymorphic(context) || setVPtrStatementsGenerated) return;
    if (HasThisInitializer()) return;
    context->GetBoundCompileUnit()->AddBoundNodeForClass(currentClass, fullSpan, context);
    setVPtrStatementsGenerated = true;
    if (!currentClass->ObjectLayoutComputed())
    {
        currentClass->MakeObjectLayout(fullSpan, context);
    }
    BoundExpressionNode* thisPtr = context->GetThisPtr(fullSpan);
    std::vector<ClassTypeSymbol*> vptrHolderClasses = currentClass->VPtrHolderClasses();
    if (vptrHolderClasses.empty())
    {
        ThrowException("no vptr holder classes for the class '" + currentClass->FullName(context) + "'", fullSpan, context);
    }
    for (ClassTypeSymbol* vptrHolderClass : vptrHolderClasses)
    {
        if (vptrHolderClass != currentClass)
        {
            FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                vptrHolderClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
            if (conversion)
            {
                BoundExpressionNode* thisPtrConverted = new BoundConversionNode(thisPtr->Clone(), conversion, fullSpan, conversion->ReturnType(context));
                setVPtrStatements.push_back(std::unique_ptr<BoundStatementNode>(
                    new BoundSetVPtrStatementNode(thisPtrConverted, currentClass, vptrHolderClass, fullSpan)));
            }
            else
            {
                ThrowException("vptr holder class conversion not found", fullSpan, context);
            }
        }
        else
        {
            setVPtrStatements.push_back(std::unique_ptr<BoundStatementNode>(new BoundSetVPtrStatementNode(thisPtr->Clone(),
                currentClass, currentClass, fullSpan)));
        }
    }
}

void StatementBinder::Visit(otava::ast::MemberInitializerListNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    int n = node.Items().size();
    for (int i = 0; i < n; ++i)
    {
        otava::ast::Node* initializer = node.Items()[i];
        initializer->Accept(*this);
    }
    CompleteBaseInitializers(fullSpan);
    std::sort(baseInitializers.begin(), baseInitializers.end(), InitializerLess());
    for (auto& initializer : baseInitializers)
    {
        ctorInitializer->AddBaseInitializer(initializer.second.release());
    }
    GenerateSetVPtrStatements(fullSpan);
    for (auto& setVPtrStatement : setVPtrStatements)
    {
        ctorInitializer->AddSetVPtrStatement(setVPtrStatement.release());
    }
    setVPtrStatements.clear();
    CompleteMemberInitializers(fullSpan);
    std::sort(memberInitializers.begin(), memberInitializers.end(), InitializerLess());
    for (auto& initializer : memberInitializers)
    {
        ctorInitializer->AddMemberInitializer(initializer.second.release());
    }
}

void StatementBinder::CompleteMemberInitializers(const soul::ast::FullSpan& fullSpan)
{
    if (currentClass)
    {
        if (HasThisInitializer()) return;
        for (VariableSymbol* memberVar : currentClass->MemberVariables(context))
        {
            int index = memberVar->GetIndex();
            bool found = false;
            for (const auto& memberInitilizer : memberInitializers)
            {
                if (memberInitilizer.first == index)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                AddDefaultMemberInitializer(memberVar, fullSpan);
                if (memberVar->GetType(context)->IsFundamentalTypeSymbol() || memberVar->GetType(context)->IsEnumeratedTypeSymbol() || 
                    memberVar->GetType(context)->IsPointerType())
                {
                    if (!context->GetFlag(ContextFlags::noWarnings))
                    {
                        PrintWarning("member variable '" + memberVar->Name() + "' not initialized", fullSpan, context);
                    }
                }
            }
        }
    }
}

void StatementBinder::AddDefaultMemberInitializer(VariableSymbol* memberVar, const soul::ast::FullSpan& fullSpan)
{
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    BoundVariableNode* boundVariableNode = new BoundVariableNode(memberVar, fullSpan, memberVar->GetReferredType(context));
    BoundExpressionNode* thisPtr = context->GetThisPtr(fullSpan);
    boundVariableNode->SetThisPtr(thisPtr);
    std::vector<TypeSymbol*> templateArgs;
    args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundVariableNode, fullSpan, boundVariableNode->GetType()->AddPointer(context))));
    std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverloadThrow(
        context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs, args, fullSpan, context);
    std::pair<int, std::unique_ptr<BoundFunctionCallNode>> p(std::move(memberVar->GetIndex()), std::move(boundFunctionCall));
    memberInitializers.push_back(std::move(p));
}

int StatementBinder::GetBaseInitializerOrTerminatorIndex(TypeSymbol* baseClass) const noexcept
{
    Cardinality n = Cardinality(currentClass->BaseClasses(context).size());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        TypeSymbol* baseCls = currentClass->BaseClasses(context)[ToUnderlying(i)];
        if (TypesEqual(baseCls, baseClass, context))
        {
            return int(i) - int(maxBaseInitializers);
        }
    }
    return -1;
}

void StatementBinder::CompleteBaseInitializers(const soul::ast::FullSpan& fullSpan)
{
    if (currentClass)
    {
        if (HasThisInitializer()) return;
        for (ClassTypeSymbol* baseClass : currentClass->BaseClasses(context))
        {
            int index = GetBaseInitializerOrTerminatorIndex(baseClass);
            bool found = false;
            for (const auto& baseInitializer : baseInitializers)
            {
                if (baseInitializer.first == index)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                AddDefaultBaseInitializer(baseClass, index, fullSpan);
            }
        }
    }
}

void StatementBinder::AddDefaultBaseInitializer(TypeSymbol* baseClass, int index, const soul::ast::FullSpan& fullSpan)
{
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    BoundExpressionNode* thisPtr = context->GetThisPtr(fullSpan);
    FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
        baseClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
    if (conversion)
    {
        std::vector<TypeSymbol*> templateArgs;
        args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan, conversion->ReturnType(context))));
        std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverloadThrow(
            context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs, args, fullSpan, context);
        std::pair<int, std::unique_ptr<BoundFunctionCallNode>> p(std::move(index), std::move(boundFunctionCall));
        baseInitializers.push_back(std::move(p));
    }
    else
    {
        ThrowException("base class conversion not found", fullSpan, context);
    }
}

void StatementBinder::Visit(otava::ast::MemberInitializerNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    initializerArgs.clear();
    classTypeSymbol = nullptr;
    memberVariableSymbol = nullptr;
    int index = -1;
    resolveClass = true;
    node.Left()->Accept(*this);
    resolveClass = false;
    if (classTypeSymbol)
    {
        BoundExpressionNode* thisPtr = context->GetThisPtr(fullSpan);
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            classTypeSymbol->AddPointer(context), thisPtr->GetType(), fullSpan, context);
        if (conversion)
        {
            initializerArgs.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan, conversion->ReturnType(context))));
        }
        else
        {
            ThrowException("base class conversion not found", fullSpan, context);
        }
        index = GetBaseInitializerOrTerminatorIndex(classTypeSymbol);
    }
    else
    {
        resolveMemberVariable = true;
        node.Left()->Accept(*this);
        resolveMemberVariable = false;
        initializerArgs.clear();
        if (memberVariableSymbol)
        {
            BoundVariableNode* boundVariableNode = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
            ParameterSymbol* thisParam = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->ThisParam(context);
            BoundParameterNode* thisPtr = new BoundParameterNode(thisParam, fullSpan, thisParam->GetType(context));
            boundVariableNode->SetThisPtr(thisPtr);
            initializerArgs.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(
                new BoundDefaultInitNode(boundVariableNode, fullSpan), fullSpan, boundVariableNode->GetType()->AddPointer(context))));
            index = memberVariableSymbol->GetIndex();
        }
    }
    resolveInitializerArguments = true;
    node.Right()->Accept(*this);
    resolveInitializerArguments = false;
    std::vector<TypeSymbol*> templateArgs;
    context->PushNodeId(node.Id());
    std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverloadThrow(
        context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs, initializerArgs, fullSpan, context);
    context->PopNodeId();
    BoundFunctionCallNode* fnCall = boundFunctionCall.get();
    if (index < 0)
    {
        std::pair<int, std::unique_ptr<BoundFunctionCallNode>> p(std::move(index), std::move(boundFunctionCall));
        baseInitializers.push_back(std::move(p));
    }
    else
    {
        std::pair<int, std::unique_ptr<BoundFunctionCallNode>> p(std::move(index), std::move(boundFunctionCall));
        memberInitializers.push_back(std::move(p));
    }
    if (context->GetBoundFunction()->HasTemporaryDestructorCalls())
    {
        if (!context->GetFlag(ContextFlags::invoke))
        {
            fnCall->SetDestructTemporariesNode(
                new BoundDestructTemporariesNode(fullSpan, context->GetBoundFunction()->GetTemporaryDestructorCalls()));
            fnCall->SetTemporaryDestructorCallsObtained();
        }
    }
}

void StatementBinder::Visit(otava::ast::TemplateIdNode& node)
{
    if (resolveClass)
    {
        TypeSymbol* type = ResolveType(&node, DeclarationFlags::none, context, TypeResolverFlags::none);
        if (type && type->IsClassTypeSymbol())
        {
            classTypeSymbol = static_cast<ClassTypeSymbol*>(type);
        }
    }
}

void StatementBinder::Visit(otava::ast::IdentifierNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    if (resolveClass)
    {
        Symbol* symbol = context->GetSymbolTable()->Lookup(node.Str(), SymbolGroupKind::classSymbolGroup, fullSpan, context);
        if (symbol && symbol->IsClassTypeSymbol())
        {
            classTypeSymbol = static_cast<ClassTypeSymbol*>(symbol);
        }
    }
    else if (resolveMemberVariable)
    {
        Symbol* symbol = context->GetSymbolTable()->Lookup(node.Str(), SymbolGroupKind::variableSymbolGroup, fullSpan, context);
        if (symbol && symbol->IsMemberVariableSymbol(context))
        {
            memberVariableSymbol = static_cast<VariableSymbol*>(symbol);
        }
    }
}

void StatementBinder::Visit(otava::ast::ExpressionListNode& node)
{
    if (resolveInitializerArguments)
    {
        int n = node.Items().size();
        for (int i = 0; i < n; ++i)
        {
            otava::ast::Node* item = node.Items()[i];
            std::unique_ptr<BoundExpressionNode> arg = BindExpression(item, context);
            if (!arg)
            {
                ThrowException("could not bind expression", item->GetFullSpan(), context);
            }
            initializerArgs.push_back(std::move(arg));
        }
    }
}

void StatementBinder::Visit(otava::ast::FunctionBodyNode& node)
{
    node.Child()->Accept(*this);
    context->GetBoundFunction()->SetBody(static_cast<BoundCompoundStatementNode*>(boundStatement.release()));
}

void StatementBinder::Visit(otava::ast::CompoundStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    Symbol* block = context->GetSymbolTable()->GetSymbolNothrow(&node);
    if (!block) return;
    if (node.BlockId() != -1)
    {
        functionDefinitionSymbol->MapBlock(node.BlockId(), block);
    }
    context->PushBlockId(node.BlockId());
    std::unique_ptr<BoundCompoundStatementNode> currentCompoundStatement(new BoundCompoundStatementNode(fullSpan));
    currentCompoundStatement->SetBlockId(node.BlockId());
    context->GetSymbolTable()->BeginScopeGeneric(block->GetScope(), context);
    if (functionDefinitionSymbol->GetFunctionKind() == FunctionKind::constructor && !setVPtrStatementsGenerated)
    {
        GenerateSetVPtrStatements(fullSpan);
        for (auto& setVPtrStatement : setVPtrStatements)
        {
            currentCompoundStatement->AddStatement(setVPtrStatement.release());
        }
        setVPtrStatements.clear();
    }
    int n = node.Count();
    for (int i = 0; i < n; ++i)
    {
        otava::ast::Node* statementNode = node.Nodes()[i];
        std::unique_ptr<BoundStatementNode> boundStatement = BindStatement(statementNode, functionDefinitionSymbol, context);
        if (boundStatement)
        {
            currentCompoundStatement->AddStatement(boundStatement.release());
        }
    }
    if (!setLineCodeGenerated)
    {
        setLineCodeGenerated = true;
        otava::ast::Node* setLineStatementNode = context->GetBoundFunction()->GetSetLineStatementNode();
        if (setLineStatementNode && !context->GetBoundFunction()->GetBoundSetLineStatement())
        {
            std::unique_ptr<BoundStatementNode> boundStatement = BindStatement(setLineStatementNode, functionDefinitionSymbol, context);
            context->GetBoundFunction()->SetBoundSetLineStatement(boundStatement.release());
        }
    }
    context->GetSymbolTable()->EndScopeGeneric(context);
    context->PopBlockId();
    SetStatement(currentCompoundStatement.release());
}

bool StatementBinder::HasThisInitializer() const noexcept
{
    for (const auto& baseInit : baseInitializers)
    {
        if (baseInit.first == -1) return true;
    }
    for (const auto& memberInit : memberInitializers)
    {
        if (memberInit.first == -1) return true;
    }
    return false;
}

void StatementBinder::Visit(otava::ast::SequenceStatementNode& node)
{
    std::unique_ptr<BoundStatementNode> boundFirstStatement = BindStatement(node.FirstStmt(), functionDefinitionSymbol, context);
    std::unique_ptr<BoundStatementNode> boundSecondStatement = BindStatement(node.SecondStmt(), functionDefinitionSymbol, context);
    std::unique_ptr<BoundSequenceStatementNode> boundSequenceStatement(new BoundSequenceStatementNode(node.GetFullSpan(),
        boundFirstStatement.release(), boundSecondStatement.release()));
    SetStatement(boundSequenceStatement.release());
}

void StatementBinder::Visit(otava::ast::IfStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    Symbol* block = context->GetSymbolTable()->GetSymbolNothrow(&node);
    if (!block) return;
    if (node.BlockId() != -1)
    {
        functionDefinitionSymbol->MapBlock(node.BlockId(), block);
    }
    context->PushBlockId(node.BlockId());
    context->GetSymbolTable()->BeginScopeGeneric(block->GetScope(), context);
    std::unique_ptr<BoundIfStatementNode> boundIfStatement(new BoundIfStatementNode(fullSpan));
    boundIfStatement->SetBlockId(node.BlockId());
    context->PushResetFlag(ContextFlags::returnRef);
    context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
    std::unique_ptr<BoundExpressionNode> condition = BindExpression(node.Condition(), context);
    if (!condition)
    {
        ThrowException("could not bind expression", node.Condition()->GetFullSpan(), fullSpan, context);
    }
    if (condition->GetType()->IsReferenceType())
    {
        TypeSymbol* type = condition->GetType()->GetBaseType(context);
        condition.reset(new BoundDereferenceNode(condition.release(), fullSpan, type));
    }
    if (!condition->GetType()->IsBoolType())
    {
        condition.reset(MakeBoundBooleanConversionNode(condition.release(), context));
    }
    context->PopFlags();
    context->PopFlags();
    boundIfStatement->SetCondition(condition.release());
    std::unique_ptr<BoundStatementNode> boundThenStatement = BindStatement(node.ThenStatement(), functionDefinitionSymbol, context);
    if (boundThenStatement)
    {
        boundIfStatement->SetThenStatement(boundThenStatement.release());
    }
    if (node.ElseStatement())
    {
        std::unique_ptr<BoundStatementNode> boundElseStatement = BindStatement(node.ElseStatement(), functionDefinitionSymbol, context);
        if (boundElseStatement)
        {
            boundIfStatement->SetElseStatement(boundElseStatement.release());
        }
    }
    SetStatement(boundIfStatement.release());
    context->GetSymbolTable()->EndScopeGeneric(context);
    context->PopBlockId();
}

void StatementBinder::Visit(otava::ast::SwitchStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    Symbol* block = context->GetSymbolTable()->GetSymbolNothrow(&node);
    if (!block) return;
    if (node.BlockId() != -1)
    {
        functionDefinitionSymbol->MapBlock(node.BlockId(), block);
    }
    context->PushBlockId(node.BlockId());
    context->GetSymbolTable()->BeginScopeGeneric(block->GetScope(), context);
    std::unique_ptr<BoundSwitchStatementNode> boundSwitchStatement(new BoundSwitchStatementNode(fullSpan));
    boundSwitchStatement->SetBlockId(node.BlockId());
    context->PushResetFlag(ContextFlags::returnRef);
    context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
    std::unique_ptr<BoundExpressionNode> condition = BindExpression(node.Condition(), context);
    if (!condition)
    {
        ThrowException("could not bind expression", node.Condition()->GetFullSpan(), fullSpan, context);
    }
    if (condition->GetType()->IsReferenceType())
    {
        TypeSymbol* type = condition->GetType()->PlainType(context);
        condition.reset(new BoundDereferenceNode(condition.release(), fullSpan, type));
    }
    TypeSymbol* switchCondType = condition->GetType();
    boundSwitchStatement->SetCondition(condition.release());
    context->PopFlags();
    context->PopFlags();
    context->PushSwitchCondType(switchCondType);
    context->PushSetFlag(ContextFlags::skipInvokeChecking);
    std::unique_ptr<BoundStatementNode> boundStmt = BindStatement(node.Statement(), functionDefinitionSymbol, context);
    context->PopFlags();
    context->PopSwitchCondType();
    if (boundStmt)
    {
        boundSwitchStatement->SetStatement(boundStmt.release());
    }
    SetStatement(boundSwitchStatement.release());
    context->GetSymbolTable()->EndScopeGeneric(context);
    context->PopBlockId();
}

void StatementBinder::Visit(otava::ast::CaseStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    if (!TerminatesCaseOrDefault(node.Statement()))
    {
        PrintWarning("case statement does not terminate in return, break or continue statement, or throw expression", fullSpan, context);
    }
    std::unique_ptr<BoundExpressionNode> caseExpr = BindExpression(node.CaseExpression(), context);
    if (!caseExpr)
    {
        ThrowException("could not bind expression", node.CaseExpression()->GetFullSpan(), fullSpan, context);
    }
    TypeSymbol* switchCondType = context->GetSwitchCondType();
    if (!TypesEqual(caseExpr->GetType(), switchCondType, context))
    {
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            switchCondType, caseExpr->GetType()->DirectType(context)->FinalType(fullSpan, context), fullSpan, context);
        if (conversion)
        {
            caseExpr.reset(new BoundConversionNode(caseExpr.release(), conversion, fullSpan, conversion->ReturnType(context)));
        }
        else
        {
            ThrowException("no conversion found", fullSpan, context);
        }
    }
    context->PushSetFlag(ContextFlags::skipInvokeChecking);
    std::unique_ptr<BoundStatementNode> boundStmt = BindStatement(node.Statement(), functionDefinitionSymbol, context);
    context->PopFlags();
    if (boundStmt)
    {
        if (boundStmt->IsBoundCaseStatementNode())
        {
            BoundCaseStatementNode* caseStmt = static_cast<BoundCaseStatementNode*>(boundStmt.get());
            caseStmt->InsertCaseExprToFront(caseExpr.release());
            SetStatement(boundStmt.release());
        }
        else
        {
            std::unique_ptr<BoundCaseStatementNode> boundCaseStatement(new BoundCaseStatementNode(fullSpan));
            boundCaseStatement->AddCaseExpr(caseExpr.release());
            boundCaseStatement->SetStatement(boundStmt.release());
            SetStatement(boundCaseStatement.release());
        }
    }
}

void StatementBinder::Visit(otava::ast::DefaultStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    if (!TerminatesCaseOrDefault(node.Statement()))
    {
        PrintWarning("default statement does not terminate in return, break or continue statement, or throw expression", fullSpan, context);
    }
    std::unique_ptr<BoundDefaultStatementNode> boundDefaultStatement(new BoundDefaultStatementNode(fullSpan));
    context->PushSetFlag(ContextFlags::skipInvokeChecking);
    std::unique_ptr<BoundStatementNode> boundStmt = BindStatement(node.Statement(), functionDefinitionSymbol, context);
    context->PopFlags();
    if (boundStmt)
    {
        boundDefaultStatement->SetStatement(boundStmt.release());
    }
    SetStatement(boundDefaultStatement.release());
}

void StatementBinder::Visit(otava::ast::WhileStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    Symbol* block = context->GetSymbolTable()->GetSymbolNothrow(&node);
    if (!block) return;
    if (!node.Condition()->IsInitConditionNode())
    {
        if (node.BlockId() != -1)
        {
            functionDefinitionSymbol->MapBlock(node.BlockId(), block);
        }
        context->PushBlockId(node.BlockId());
        context->GetSymbolTable()->BeginScopeGeneric(block->GetScope(), context);
        std::unique_ptr<BoundWhileStatementNode> boundWhileStatement(new BoundWhileStatementNode(fullSpan));
        boundWhileStatement->SetBlockId(node.BlockId());
        context->PushResetFlag(ContextFlags::returnRef);
        context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
        std::unique_ptr<BoundExpressionNode> condition = BindExpression(node.Condition(), context);
        if (!condition)
        {
            ThrowException("could not bind expression", node.Condition()->GetFullSpan(), fullSpan, context);
        }
        if (condition->GetType()->IsReferenceType())
        {
            TypeSymbol* type = condition->GetType()->GetBaseType(context);
            condition.reset(new BoundDereferenceNode(condition.release(), fullSpan, type));
        }
        if (!condition->GetType()->IsBoolType())
        {
            condition.reset(MakeBoundBooleanConversionNode(condition.release(), context));
        }
        context->PopFlags();
        context->PopFlags();
        boundWhileStatement->SetCondition(condition.release());
        std::unique_ptr<BoundStatementNode> boundStmt = BindStatement(node.Statement(), functionDefinitionSymbol, context);
        if (boundStmt)
        {
            boundWhileStatement->SetStatement(boundStmt.release());
        }
        SetStatement(boundWhileStatement.release());
        context->GetSymbolTable()->EndScopeGeneric(context);
        context->PopBlockId();
    }
    else
    {
        bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
        std::string label = "__label" + std::to_string(context->NextLabelSerial());
        std::unique_ptr<otava::ast::CompoundStatementNode> whileBlock(new otava::ast::CompoundStatementNode(fullSpan.span, fullSpan.fileIndex));
        std::unique_ptr<otava::ast::CompoundStatementNode> ifBlock(new otava::ast::CompoundStatementNode(fullSpan.span, fullSpan.fileIndex));
        ifBlock->AddNode(node.Statement()->Clone());
        ifBlock->AddNode(new otava::ast::GotoStatementNode(fullSpan.span, fullSpan.fileIndex,
            new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, label), nullptr,
            nullptr, fullSpan.span));
        std::unique_ptr<otava::ast::IfStatementNode> ifStatement(new otava::ast::IfStatementNode(fullSpan.span, fullSpan.fileIndex, node.Condition()->Clone(),
            ifBlock.release(), nullptr, nullptr, fullSpan.span, fullSpan.span, fullSpan.span, fullSpan.span, fullSpan.span));
        whileBlock->AddNode(ifStatement.release());
        std::unique_ptr<otava::ast::LabeledStatementNode> labeledStatement(new otava::ast::LabeledStatementNode(fullSpan.span, fullSpan.fileIndex,
            new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, label), whileBlock.release(), nullptr, fullSpan.span));
        InstantiationScope instantiationScope(context->GetModule(), context->GetSymbolTable()->CurrentScope());
        Instantiator instantiator(context, &instantiationScope);
        context->PushSetFlag(ContextFlags::saveDeclarations | ContextFlags::dontBind);
        labeledStatement->Accept(instantiator);
        context->PopFlags();
        std::unique_ptr<BoundStatementNode> boundLabeledStatement = BindStatement(labeledStatement.get(), functionDefinitionSymbol, context);
        SetStatement(boundLabeledStatement.release());
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
    }
}

void StatementBinder::Visit(otava::ast::DoStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundDoStatementNode> boundDoStatement(new BoundDoStatementNode(fullSpan));
    context->PushResetFlag(ContextFlags::returnRef);
    context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
    std::unique_ptr<BoundExpressionNode> condition = BindExpression(node.Expression(), context);
    if (!condition)
    {
        ThrowException("could not bind expression", node.Expression()->GetFullSpan(), fullSpan, context);
    }
    if (condition->GetType()->IsReferenceType())
    {
        TypeSymbol* type = condition->GetType()->GetBaseType(context);
        condition.reset(new BoundDereferenceNode(condition.release(), fullSpan, type));
    }
    if (!condition->GetType()->IsBoolType())
    {
        condition.reset(MakeBoundBooleanConversionNode(condition.release(), context));
    }
    context->PopFlags();
    context->PopFlags();
    boundDoStatement->SetExpr(condition.release());
    std::unique_ptr<BoundStatementNode> boundStmt = BindStatement(node.Statement(), functionDefinitionSymbol, context);
    if (boundStmt)
    {
        boundDoStatement->SetStatement(boundStmt.release());
    }
    SetStatement(boundDoStatement.release());
}

class RangeForDeclarationExtractor : public otava::ast::DefaultVisitor
{
public:
    RangeForDeclarationExtractor() noexcept;
    inline otava::ast::DeclSpecifierSequenceNode* GetDeclSpecifierSequence() noexcept { return declSpecifierSequence; }
    inline otava::ast::Node* GetDeclarator() const noexcept { return declarator; }
    void Visit(otava::ast::ForRangeDeclarationNode& node) override;
    void Visit(otava::ast::DeclSpecifierSequenceNode& node) override;
private:
    otava::ast::DeclSpecifierSequenceNode* declSpecifierSequence;
    otava::ast::Node* declarator;
};

RangeForDeclarationExtractor::RangeForDeclarationExtractor() noexcept : declSpecifierSequence(nullptr), declarator(nullptr)
{
}

void RangeForDeclarationExtractor::Visit(otava::ast::ForRangeDeclarationNode& node)
{
    node.Left()->Accept(*this);
    declarator = node.Right();
}

void RangeForDeclarationExtractor::Visit(otava::ast::DeclSpecifierSequenceNode& node)
{
    declSpecifierSequence = &node;
}

void StatementBinder::Visit(otava::ast::RangeForStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    context->PushBlockId(node.BlockId());
    std::unique_ptr<otava::ast::CompoundStatementNode> rangeForCompound(new otava::ast::CompoundStatementNode(fullSpan.span, fullSpan.fileIndex));
    RangeForBlockIds& rangeForBlockIds = context->GetRangeForBlockIds(node.RangeForId());
    if (rangeForBlockIds.compoundBlockId == -1)
    {
        rangeForBlockIds.compoundBlockId = context->NextBlockId();
    }
    if (rangeForBlockIds.forActionStatementId == -1)
    {
        rangeForBlockIds.forActionStatementId = context->NextBlockId();
    }
    if (rangeForBlockIds.forStatementId == -1)
    {
        rangeForBlockIds.forStatementId = context->NextBlockId();
    }
    rangeForCompound->SetBlockId(rangeForBlockIds.compoundBlockId);
    if (node.InitStatement())
    {
        rangeForCompound->AddNode(node.InitStatement()->Clone());
    }
    otava::ast::DeclSpecifierSequenceNode* endIteratorDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex);
    endIteratorDeclSpecifiers->AddNode(new otava::ast::PlaceholderTypeSpecifierNode(fullSpan.span, fullSpan.fileIndex));
    otava::ast::InitDeclaratorListNode* endIteratorDeclarators = new otava::ast::InitDeclaratorListNode(fullSpan.span, fullSpan.fileIndex);
    otava::ast::IdentifierNode* endIteratorDeclarator = new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "@end");
    otava::ast::Node* invokeChild = node.Initializer()->Clone();
    otava::ast::IdentifierNode* invokeId = new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "end");
    otava::ast::MemberExprNode* invokeSubject = new otava::ast::MemberExprNode(fullSpan.span, fullSpan.fileIndex, invokeChild,
        new otava::ast::DotNode(fullSpan.span, fullSpan.fileIndex), invokeId);
    otava::ast::InvokeExprNode* endIteratorInitializer = new otava::ast::InvokeExprNode(fullSpan.span, fullSpan.fileIndex, invokeSubject);
    otava::ast::AssignmentInitNode* endIteratorAssignmentInitializer = new otava::ast::AssignmentInitNode(fullSpan.span, fullSpan.fileIndex, endIteratorInitializer);
    otava::ast::InitDeclaratorNode* endIteratorInitDeclarator = new otava::ast::InitDeclaratorNode(fullSpan.span, fullSpan.fileIndex,
        endIteratorDeclarator, endIteratorAssignmentInitializer);
    endIteratorDeclarators->AddNode(endIteratorInitDeclarator);
    otava::ast::SimpleDeclarationNode* endIteratorDeclaration = new otava::ast::SimpleDeclarationNode(
        fullSpan.span, fullSpan.fileIndex, endIteratorDeclSpecifiers, endIteratorDeclarators, nullptr, nullptr);
    rangeForCompound->AddNode(endIteratorDeclaration);
    otava::ast::DeclSpecifierSequenceNode* forInitDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex);
    forInitDeclSpecifiers->AddNode(new otava::ast::PlaceholderTypeSpecifierNode(fullSpan.span, fullSpan.fileIndex));
    otava::ast::InitDeclaratorListNode* forInitDeclarators = new otava::ast::InitDeclaratorListNode(fullSpan.span, fullSpan.fileIndex);
    otava::ast::Node* forInitInvokeChild = node.Initializer()->Clone();
    otava::ast::MemberExprNode* forInitInvokeSubject = new otava::ast::MemberExprNode(fullSpan.span, fullSpan.fileIndex,
        forInitInvokeChild, new otava::ast::DotNode(fullSpan.span, fullSpan.fileIndex),
        new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "begin"));
    otava::ast::InvokeExprNode* forInitInitializer = new otava::ast::InvokeExprNode(fullSpan.span, fullSpan.fileIndex, forInitInvokeSubject);
    otava::ast::AssignmentInitNode* forInitAssignmentInitializer = new otava::ast::AssignmentInitNode(fullSpan.span, fullSpan.fileIndex, forInitInitializer);
    otava::ast::IdentifierNode* forInitDeclarator = new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "@it");
    otava::ast::InitDeclaratorNode* forInitInitDeclarator = new otava::ast::InitDeclaratorNode(fullSpan.span, fullSpan.fileIndex,
        forInitDeclarator, forInitAssignmentInitializer);
    forInitDeclarators->AddNode(forInitInitDeclarator);
    otava::ast::SimpleDeclarationNode* forInitStmt = new otava::ast::SimpleDeclarationNode(fullSpan.span, fullSpan.fileIndex, forInitDeclSpecifiers,
        forInitDeclarators, nullptr, nullptr);
    otava::ast::BinaryExprNode* forCond = new otava::ast::BinaryExprNode(
        fullSpan.span, fullSpan.fileIndex, new otava::ast::NotEqualNode(fullSpan.span, fullSpan.fileIndex),
        new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "@it"),
        new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "@end"));
    otava::ast::UnaryExprNode* forLoopExpr = new otava::ast::UnaryExprNode(
        fullSpan.span, fullSpan.fileIndex, new otava::ast::PrefixIncNode(fullSpan.span, fullSpan.fileIndex),
        new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "@it"));
    otava::ast::CompoundStatementNode* forActionStmt = new otava::ast::CompoundStatementNode(fullSpan.span, fullSpan.fileIndex);
    forActionStmt->SetBlockId(rangeForBlockIds.forActionStatementId);
    RangeForDeclarationExtractor extractor;
    node.Declaration()->Accept(extractor);
    otava::ast::DeclSpecifierSequenceNode* forActionDeclSpecifiers = extractor.GetDeclSpecifierSequence();
    otava::ast::Node* declarator = extractor.GetDeclarator();
    otava::ast::AssignmentInitNode* forActionAssignmentInit = new otava::ast::AssignmentInitNode(fullSpan.span, fullSpan.fileIndex,
        new otava::ast::UnaryExprNode(fullSpan.span, fullSpan.fileIndex,
            new otava::ast::DerefNode(fullSpan.span, fullSpan.fileIndex), new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "@it")));
    otava::ast::InitDeclaratorNode* forActionInitDeclarator = new otava::ast::InitDeclaratorNode(fullSpan.span, fullSpan.fileIndex,
        declarator->Clone(), forActionAssignmentInit);
    otava::ast::InitDeclaratorListNode* forActionInitDeclaratorList = new otava::ast::InitDeclaratorListNode(fullSpan.span, fullSpan.fileIndex);
    forActionInitDeclaratorList->AddNode(forActionInitDeclarator);
    otava::ast::SimpleDeclarationNode* forActionDeclaration = new otava::ast::SimpleDeclarationNode(
        fullSpan.span, fullSpan.fileIndex, forActionDeclSpecifiers->Clone(), forActionInitDeclaratorList, nullptr, nullptr);
    otava::ast::DeclarationStatementNode* forActionDeclarationStmt = new otava::ast::DeclarationStatementNode(fullSpan.span, fullSpan.fileIndex, forActionDeclaration);
    forActionStmt->AddNode(forActionDeclarationStmt);
    forActionStmt->AddNode(node.Statement()->Clone());
    otava::ast::ForStatementNode* forStmt = new otava::ast::ForStatementNode(fullSpan.span, fullSpan.fileIndex,
        forInitStmt, forCond, forLoopExpr, forActionStmt, nullptr, nullptr,
        fullSpan.span, fullSpan.span, fullSpan.span);
    forStmt->SetBlockId(rangeForBlockIds.forStatementId);
    rangeForCompound->AddNode(forStmt);
    InstantiationScope instantiationScope(context->GetModule(), context->GetSymbolTable()->CurrentScope());
    Instantiator instantiator(context, &instantiationScope);
    context->PushSetFlag(ContextFlags::saveDeclarations | ContextFlags::dontBind);
    rangeForCompound->Accept(instantiator);
    context->PopFlags();
    rangeForCompound->Accept(*this);
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
    context->PopBlockId();
}

void StatementBinder::Visit(otava::ast::ForStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    Symbol* block = context->GetSymbolTable()->GetSymbolNothrow(&node);
    if (!block) return;
    if (node.BlockId() != -1)
    {
        functionDefinitionSymbol->MapBlock(node.BlockId(), block);
    }
    context->PushBlockId(node.BlockId());
    context->GetSymbolTable()->BeginScopeGeneric(block->GetScope(), context);
    std::unique_ptr<BoundForStatementNode> boundForStatement(new BoundForStatementNode(fullSpan));
    boundForStatement->SetBlockId(node.BlockId());
    if (node.InitStatement())
    {
        std::unique_ptr<BoundStatementNode> boundInitStatement = BindStatement(node.InitStatement(), functionDefinitionSymbol, context);
        if (boundInitStatement)
        {
            boundForStatement->SetInitStatement(boundInitStatement.release());
        }
    }
    if (node.Condition())
    {
        context->PushResetFlag(ContextFlags::returnRef);
        context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
        std::unique_ptr<BoundExpressionNode> condition = BindExpression(node.Condition(), context);
        if (!condition)
        {
            ThrowException("could not bind expression", node.Condition()->GetFullSpan(), fullSpan, context);
        }
        if (condition->GetType()->IsReferenceType())
        {
            TypeSymbol* type = condition->GetType()->GetBaseType(context);
            condition.reset(new BoundDereferenceNode(condition.release(), fullSpan, type));
        }
        if (!condition->GetType()->IsBoolType())
        {
            condition.reset(MakeBoundBooleanConversionNode(condition.release(), context));
        }
        context->PopFlags();
        context->PopFlags();
        boundForStatement->SetCondition(condition.release());
    }
    if (node.LoopExpr())
    {
        context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
        std::unique_ptr<BoundExpressionNode> loopExpr = BindExpression(node.LoopExpr(), context);
        if (!loopExpr)
        {
            ThrowException("could not bind expression", node.LoopExpr()->GetFullSpan(), fullSpan, context);
        }
        context->PopFlags();
        boundForStatement->SetLoopExpr(loopExpr.release());
    }
    std::unique_ptr<BoundStatementNode> boundStmt = BindStatement(node.Statement(), functionDefinitionSymbol, context);
    if (boundStmt)
    {
        boundForStatement->SetStatement(boundStmt.release());
    }
    SetStatement(boundForStatement.release());
    context->GetSymbolTable()->EndScopeGeneric(context);
    context->PopBlockId();
}

void StatementBinder::Visit(otava::ast::BreakStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundBreakStatementNode> boundBreakStatement(new BoundBreakStatementNode(fullSpan));
    SetStatement(boundBreakStatement.release());
}

void StatementBinder::Visit(otava::ast::ContinueStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundContinueStatementNode> boundContinueStatement(new BoundContinueStatementNode(fullSpan));
    SetStatement(boundContinueStatement.release());
}

void StatementBinder::Visit(otava::ast::ReturnStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::vector<std::unique_ptr<BoundExpressionNode>> classReturnArgs;
    ParameterSymbol* returnValueParam = nullptr;
    std::unique_ptr<BoundExpressionNode> expression;
    BoundFunctionPtrCallNode* call = nullptr;
    FunctionTypeSymbol* fnType = nullptr;
    std::unique_ptr<BoundExpressionStatementNode> expressionStatement;
    std::vector<std::unique_ptr<BoundExpressionNode>> moveArgs;
    Scope* scope = nullptr;
    std::vector<TypeSymbol*> templateArgs;
    std::unique_ptr<BoundFunctionCallNode> moveExpr;
    std::vector<TypeSymbol*> templateArgs2;
    std::unique_ptr<BoundFunctionCallNode> constructorCall;
    TypeSymbol* returnType = nullptr;
    std::unique_ptr<BoundExpressionNode> returnValueExpr;
    bool flagsPushed = false;
    ArgumentMatch argumentMatch;
    FunctionMatch functionMatch;
    FunctionSymbol* conversion = nullptr;
    if (context->GetFlag(ContextFlags::makeChildFn))
    {
        std::unique_ptr<BoundStatementNode> boundStatement = ConvertReturnStatement(&node, functionDefinitionSymbol, context);
        SetStatement(boundStatement.release());
    }
    else
    {
        TypeSymbol* voidType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::voidType, context);
        std::unique_ptr<BoundReturnStatementNode> boundReturnStatement(new BoundReturnStatementNode(fullSpan));
        if (node.ReturnValue())
        {
            if (!context->GetBoundFunction()->GetFunctionDefinitionSymbol()->ReturnType(context) ||
                TypesEqual(context->GetBoundFunction()->GetFunctionDefinitionSymbol()->ReturnType(context)->DirectType(context)->FinalType(fullSpan, context),
                    voidType, context))
            {
                ThrowException("cannot return a value", node.ReturnValue()->GetFullSpan(), fullSpan, context);
            }
            if (context->GetBoundFunction()->GetFunctionDefinitionSymbol()->ReturnsClass())
            {
                returnValueParam = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->ReturnValueParam(context);
                classReturnArgs.push_back(std::unique_ptr<BoundExpressionNode>(new BoundParameterNode(returnValueParam,
                    fullSpan, returnValueParam->GetReferredType(context))));
                expression = BindExpression(node.ReturnValue(), context);
                if (!expression)
                {
                    ThrowException("could not bind expression", node.ReturnValue()->GetFullSpan(), fullSpan, context);
                }
                if (expression->IsBoundFunctionPtrCallNode())
                {
                    call = static_cast<BoundFunctionPtrCallNode*>(expression.get());
                    fnType = static_cast<FunctionTypeSymbol*>(call->GetType()->GetBaseType(context));
                    fnType->AddParameterType(returnValueParam->GetReferredType(context));
                    fnType->SetReturnType(voidType);
                    expression->SetType(voidType);
                    call->AddArgument(new BoundParameterNode(returnValueParam,
                        fullSpan, returnValueParam->GetReferredType(context)));
                    expressionStatement.reset(new BoundExpressionStatementNode(fullSpan));
                    expressionStatement->SetExpr(expression.release(), fullSpan, context);
                    SetStatement(expressionStatement.release());
                }
                else
                {
                    if (expression->IsBoundLocalVariable(context))
                    {
                        moveArgs.push_back(std::move(expression));
                        scope = context->GetSymbolTable()->GetNamespaceScope("std", fullSpan, context);
                        context->PushNodeId(node.Id());
                        moveExpr = ResolveOverloadThrow(
                            scope, "move", templateArgs, moveArgs, fullSpan, context, OverloadResolutionFlags::dontSearchArgumentScopes);
                        context->PopNodeId();
                        expression = std::unique_ptr<BoundExpressionNode>(moveExpr.release());
                    }
                    classReturnArgs.push_back(std::move(expression));
                    context->PushNodeId(node.Id());
                    constructorCall = ResolveOverloadThrow(
                        context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs2, classReturnArgs, fullSpan, context);
                    context->PopNodeId();
                    expressionStatement.reset(new BoundExpressionStatementNode(fullSpan));
                    expressionStatement->SetExpr(constructorCall.release(), fullSpan, context);
                    if (context->GetBoundFunction()->HasTemporaryDestructorCalls())
                    {
                        if (!context->GetFlag(ContextFlags::invoke))
                        {
                            expressionStatement->SetDestructTemporariesNode(
                                new BoundDestructTemporariesNode(fullSpan, context->GetBoundFunction()->GetTemporaryDestructorCalls()));
                            expressionStatement->SetTemporaryDestructorCallsObtained();
                        }
                    }
                    SetStatement(expressionStatement.release());
                }
            }
            else
            {
                returnType = functionDefinitionSymbol->ReturnType(context)->DirectType(context)->FinalType(fullSpan, context);
                flagsPushed = false;
                if (returnType->IsReferenceType())
                {
                    context->PushSetFlag(ContextFlags::returnRef);
                    flagsPushed = true;
                }
                context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
                returnValueExpr = BindExpression(node.ReturnValue(), context);
                context->PopFlags();
                if (!returnValueExpr)
                {
                    ThrowException("could not bind expression", node.ReturnValue()->GetFullSpan(), fullSpan, context);
                }
                if (!TypesEqual(returnValueExpr->GetType(), returnType, context))
                {
                    if (TypesEqual(returnValueExpr->GetType()->PlainType(context), returnType->PlainType(context), context))
                    {
                        if (!(
                            returnType->IsLValueRefType() && returnValueExpr->GetType()->IsRValueRefType() ||
                            returnType->IsRValueRefType() && returnValueExpr->GetType()->IsLValueRefType()))
                        {
                            if (returnType->IsLValueRefType())
                            {
                                TypeSymbol* type = returnValueExpr->GetType()->AddLValueRef(context);
                                returnValueExpr.reset(new BoundAddressOfNode(returnValueExpr.release(), fullSpan, type));
                            }
                            else if (returnValueExpr->GetType()->IsLValueRefType())
                            {
                                TypeSymbol* type = returnValueExpr->GetType()->RemoveReference(context);
                                returnValueExpr.reset(new BoundDereferenceNode(returnValueExpr.release(), fullSpan, type));
                            }
                        }
                    }
                    else
                    {
                        conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                            returnType, returnValueExpr->GetType()->DirectType(context)->FinalType(fullSpan, context), returnValueExpr.get(),
                            fullSpan, argumentMatch, functionMatch, context);
                        if (conversion)
                        {
                            if (argumentMatch.preConversionFlags == OperationFlags::addr)
                            {
                                TypeSymbol* type = returnValueExpr->GetType()->AddPointer(context);
                                returnValueExpr.reset(new BoundConversionNode(
                                    new BoundAddressOfNode(returnValueExpr.release(), fullSpan, type), conversion, fullSpan, conversion->ReturnType(context)));
                            }
                            else if (argumentMatch.preConversionFlags == OperationFlags::deref)
                            {
                                TypeSymbol* type = returnValueExpr->GetType()->RemoveReference(context);
                                returnValueExpr.reset(new BoundConversionNode(
                                    new BoundDereferenceNode(returnValueExpr.release(), fullSpan, type), conversion, fullSpan, conversion->ReturnType(context)));
                            }
                            else
                            {
                                returnValueExpr.reset(new BoundConversionNode(returnValueExpr.release(), conversion, fullSpan, conversion->ReturnType(context)));
                            }
                        }
                        else
                        {
                            ThrowException("no conversion found", fullSpan, context);
                        }
                    }
                }
                boundReturnStatement->SetExpr(returnValueExpr.release(), fullSpan, context);
                if (flagsPushed)
                {
                    context->PopFlags();
                }
            }
        }
        else if (context->GetBoundFunction()->GetFunctionDefinitionSymbol()->ReturnType(context) &&
            !TypesEqual(context->GetBoundFunction()->GetFunctionDefinitionSymbol()->ReturnType(context)->DirectType(context)->FinalType(fullSpan, context),
                voidType, context))
        {
            ThrowException("must return a value", fullSpan, context);
        }
        SetStatement(boundReturnStatement.release());
    }
}

void GenerateDiscardWarning(BoundExpressionNode* expr, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!expr->IsBoundConstructTemporaryNode()) return;
    BoundConstructTemporaryNode* ctNode = static_cast<BoundConstructTemporaryNode*>(expr);
    BoundExpressionNode* call = ctNode->ConstructorCall();
    if (call->IsBoundFunctionCallNode())
    {
        BoundFunctionCallNode* fnCall = static_cast<BoundFunctionCallNode*>(call);
        FunctionSymbol* fn = fnCall->GetFunctionSymbol();
        TypeSymbol* returnType = fn->ReturnType(context);
        if (returnType && returnType->IsClassTemplateSpecializationSymbol())
        {
            ClassTemplateSpecializationSymbol* sp = static_cast<ClassTemplateSpecializationSymbol*>(returnType);
            ClassTypeSymbol* ct = sp->ClassTemplate(context);
            ClassGroupSymbol* group = ct->Group(context);
            if (group->Name() == "expected")
            {
                PrintWarning("discarding return value of function '" + fn->FullName(context) + "", fullSpan, context);
            }
        }
    }
}

BoundVariableNode* GetVariableFromArgument(BoundExpressionNode* arg)
{
    while (arg && arg->IsBoundFunctionCallNode())
    {
        BoundFunctionCallNode* fnCall = static_cast<BoundFunctionCallNode*>(arg);
        if (fnCall->Args().empty()) break;
        arg = fnCall->Args()[0].get();
    }
    if (arg && arg->IsBoundAddressOfNode())
    {
        BoundAddressOfNode* addr = static_cast<BoundAddressOfNode*>(arg);
        arg = addr->Subject();
    }
    if (arg && arg->IsBoundVariableNode())
    {
        return static_cast<BoundVariableNode*>(arg);
    }
    return nullptr;
}

void GenerateInitStreamCall(BoundExpressionNode* expr, StatementBinder* statementBinder, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!expr->IsBoundFunctionCallNode()) return;
    BoundFunctionCallNode* fnCall = static_cast<BoundFunctionCallNode*>(expr);
    if (fnCall->GetFunctionSymbol()->GroupName() != "operator<<") return;
    if (fnCall->Args().empty()) return;
    if (!fnCall->Args()[0]->GetType()->GetBaseType(context)->IsClassTemplateSpecializationSymbol()) return;
    ClassTemplateSpecializationSymbol* s = static_cast<ClassTemplateSpecializationSymbol*>(fnCall->Args()[0]->GetType()->GetBaseType(context));
    if (s->ClassTemplate(context)->GroupName(context) != "basic_ostream") return;
    BoundExpressionNode* arg = fnCall->Args()[0].get();
    BoundVariableNode* v = GetVariableFromArgument(arg);
    if (!v) return;
    otava::ast::Node* source = v->Source();
    if (!source) return;
    std::string stmtStr = source->Str();
    stmtStr.append(".init();");
    try
    {
        std::unique_ptr<otava::ast::Node> stmtNode = ParseStatement(stmtStr, context);
        stmtNode->Accept(*statementBinder);
    }
    catch (const std::exception& ex)
    {
        Exception w("could not generate stream initialization call: " + std::string(ex.what()), fullSpan, context);
        w.SetWarning();
        PrintWarning(w, context);
    }
}

void StatementBinder::Visit(otava::ast::ExpressionStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    BoundExpressionStatementNode* boundExpressionStatement = new BoundExpressionStatementNode(fullSpan);
    if (node.Expression())
    {
        if (!context->GetFlag(ContextFlags::invoke))
        {
            context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
        }
        std::unique_ptr<BoundExpressionNode> expr = BindExpression(node.Expression(), context);
        if (!expr)
        {
            ThrowException("could not bind expression", node.Expression()->GetFullSpan(), fullSpan, context);
        }
        if (!context->GetFlag(ContextFlags::invoke))
        {
            context->PopFlags();
        }
        GenerateDiscardWarning(expr.get(), fullSpan, context);
        GenerateInitStreamCall(expr.get(), this, fullSpan, context);
        boundExpressionStatement->SetExpr(expr.release(), fullSpan, context);
    }
    SetStatement(boundExpressionStatement);
}

void StatementBinder::Visit(otava::ast::DeclarationStatementNode& node)
{
    node.Declaration()->Accept(*this);
}

void StatementBinder::Visit(otava::ast::TryStatementNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundStatementNode> boundTryBlockStatement(BindStatement(node.TryBlock(), functionDefinitionSymbol, context));
    std::string tryResultVar = context->NextResultVarName();
    std::string handlerResultVar = context->NextResultVarName();
    std::string ehReturnFromVar = context->NextEhReturnFromVarName();
    std::string childControlResultVar = context->NextChildControlResultVarName();
    TypeSymbol* resultType = functionDefinitionSymbol->NonChildFunctionResultType(context);
    std::unique_ptr<otava::ast::Node> tryVarDeclarationStmtNode;
    std::unique_ptr<otava::ast::Node> handlerVarDeclarationStmtNode;
    std::unique_ptr<BoundStatementNode> boundTryVarDeclarationStatement;
    std::unique_ptr<BoundStatementNode> boundHandlerVarDeclarationStatement;
    std::unique_ptr<BoundStatementNode> boundChildControlResultStatement;
    std::unique_ptr<BoundStatementNode> boundEhReturnFromStatement;
    if (resultType)
    {
        TypeSymbol* nonReferenceType = ConvertRefToPtrType(resultType, context);
        std::string tryVarDeclarationStatementText;
        tryVarDeclarationStatementText.append(nonReferenceType->FullName(context)).append(1, ' ').append(tryResultVar);
        if (nonReferenceType->IsPointerType())
        {
            tryVarDeclarationStatementText.append(" = nullptr;");
        }
        else
        {
            tryVarDeclarationStatementText.append(";");
        }
        tryVarDeclarationStmtNode = ParseStatement(tryVarDeclarationStatementText, context);
        boundTryVarDeclarationStatement = BindStatement(tryVarDeclarationStmtNode.get(), functionDefinitionSymbol, context);
        std::string handlerVarDeclarationStatementText;
        handlerVarDeclarationStatementText.append(nonReferenceType->FullName(context)).append(1, ' ').append(handlerResultVar);
        if (nonReferenceType->IsPointerType())
        {
            handlerVarDeclarationStatementText.append(" = nullptr;");
        }
        else
        {
            handlerVarDeclarationStatementText.append(";");
        }
        handlerVarDeclarationStmtNode = ParseStatement(handlerVarDeclarationStatementText, context);
        boundHandlerVarDeclarationStatement = BindStatement(handlerVarDeclarationStmtNode.get(), functionDefinitionSymbol, context);
    }
    std::string childControlResultStatementText;
    childControlResultStatementText.append("std::child_control_result ").append(childControlResultVar).append(" = std::child_control_result::none;");
    std::unique_ptr<otava::ast::Node> setChildControlResultStmtNode = ParseStatement(childControlResultStatementText, context);
    boundChildControlResultStatement = BindStatement(setChildControlResultStmtNode.get(), functionDefinitionSymbol, context);
    std::string ehReturnFromTryBlockStatementText;
    ehReturnFromTryBlockStatementText.append("std::eh_return_from ").append(ehReturnFromVar).append(" = std::eh_return_from::try_block;");
    std::unique_ptr<otava::ast::Node> ehReturnFromTryBlockStmtNode = ParseStatement(ehReturnFromTryBlockStatementText, context);
    boundEhReturnFromStatement = BindStatement(ehReturnFromTryBlockStmtNode.get(), functionDefinitionSymbol, context);
    FunctionDefinitionSymbol* parentFn = context->GetBoundFunction()->GetFunctionDefinitionSymbol();
    Scope* parentFnScope = context->GetSymbolTable()->CurrentScope();
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    otava::ast::CompoundStatementNode* tryBlock = static_cast<otava::ast::CompoundStatementNode*>(node.TryBlock()->Clone());
    otava::ast::DeclSpecifierSequenceNode* tryDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex);
    tryDeclSpecifiers->AddNode(new otava::ast::VoidNode(fullSpan.span, fullSpan.fileIndex));
    int trySerial = context->NextTrySerial();
    otava::ast::ParameterListNode* tryParameters = new otava::ast::ParameterListNode(fullSpan.span, fullSpan.fileIndex);
    otava::ast::ParameterNode* tryParameter = new otava::ast::ParameterNode(fullSpan.span, fullSpan.fileIndex);
    otava::ast::DeclSpecifierSequenceNode* tryParamDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex);
    tryParamDeclSpecifiers->AddNode(new otava::ast::VoidNode(fullSpan.span, fullSpan.fileIndex));
    tryParameter->SetDeclSpecifiers(tryParamDeclSpecifiers);
    otava::ast::PtrDeclaratorNode* tryPtrDeclarator = new otava::ast::PtrDeclaratorNode(fullSpan.span, fullSpan.fileIndex);
    tryPtrDeclarator->AddNode(new otava::ast::PtrNode(fullSpan.span, fullSpan.fileIndex));
    tryPtrDeclarator->AddNode(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "__parentFrame"));
    tryParameter->SetDeclarator(tryPtrDeclarator);
    tryParameters->AddNode(tryParameter);
    otava::ast::FunctionDeclaratorNode* tryDeclarator = new otava::ast::FunctionDeclaratorNode(fullSpan.span, fullSpan.fileIndex,
        new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "try_" +
            std::to_string(trySerial) + "_" + context->GetBoundCompileUnit()->Id()), tryParameters);
    std::unique_ptr<otava::ast::FunctionDefinitionNode> tryFn(new otava::ast::FunctionDefinitionNode(
        fullSpan.span, fullSpan.fileIndex, nullptr, tryDeclSpecifiers, tryDeclarator, nullptr,
        new otava::ast::FunctionBodyNode(fullSpan.span, fullSpan.fileIndex, tryBlock)));
    InstantiationScope tryInstantiationScope(context->GetModule(), context->GetBoundFunction()->GetFunctionDefinitionSymbol()->Parent(context)->GetScope());
    //tryInstantiationScope.PushParentScope(context->GetSymbolTable()->GetNamespaceScope(U"std", node.GetSourcePos(), context));
    tryInstantiationScope.PushParentScope(context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context));
    context->GetSymbolTable()->BeginScope(&tryInstantiationScope);
    Instantiator tryInstantiator(context, &tryInstantiationScope);
    tryInstantiator.SetFunctionNode(tryFn.get());
    context->PushSetFlag(ContextFlags::instantiateInlineFunction | ContextFlags::saveDeclarations | ContextFlags::dontBind | ContextFlags::tryCatch |
        ContextFlags::setParentBlockIds);
    context->PushParentFn(parentFn);
    tryFn->Accept(tryInstantiator);
    int tryfnScopeCount = tryInstantiator.ScopeCount();
    FunctionDefinitionSymbol* tryFnSymbol = static_cast<FunctionDefinitionSymbol*>(tryInstantiator.GetSpecialization());
    tryFnSymbol->SetParentFn(parentFn);
    tryFnSymbol->SetParentFnScope(parentFnScope);
    context->PushBoundFunction(new BoundFunctionNode(tryFnSymbol, fullSpan));
    context->PushSetFlag(ContextFlags::makeChildFn);
    context->PushResultVarName(tryResultVar);
    context->PushChildControlResultVarName(childControlResultVar);
    tryFnSymbol->SetResultVarName(tryResultVar);
    tryFnSymbol = BindFunction(tryFn.get(), tryFnSymbol, context);
    context->PopParentFn();
    otava::symbols::EndFunctionDefinition(tryFn.get(), tryfnScopeCount, context);
    context->PopChildControlResultVarName();
    context->PopResultVarName();
    context->PopFlags();
    context->PopFlags();
    context->GetSymbolTable()->EndScope();
    tryInstantiationScope.PopParentScope();
    tryFnSymbol->SetFnDefNode(tryFn.release());
    otava::ast::CompoundStatementNode* prevHandlerBlock = handlerBlock;
    handlerBlock = new otava::ast::CompoundStatementNode(fullSpan.span, fullSpan.fileIndex);
    handlerBlock->SetBlockId(context->NextBlockId());
    node.Handlers()->Accept(*this);
    std::string ehReturnFromHandlerStmtText;
    ehReturnFromHandlerStmtText.append(ehReturnFromVar).append(" = std::eh_return_from::handler_block;");
    std::unique_ptr<otava::ast::Node> ehReturnFromHandlerStmt = ParseStatement(ehReturnFromHandlerStmtText, context);
    handlerBlock->AddNode(ehReturnFromHandlerStmt.release());
    InstantiationScope handlerBlockInstantiationScope(context->GetModule(), context->GetSymbolTable()->CurrentScope());
    context->GetSymbolTable()->BeginScope(&handlerBlockInstantiationScope);
    Instantiator handlerBlockInstantiator(context, &handlerBlockInstantiationScope);
    context->PushSetFlag(ContextFlags::saveDeclarations | ContextFlags::dontBind);
    handlerBlock->Accept(handlerBlockInstantiator);
    context->PopFlags();
    std::unique_ptr<BoundStatementNode> boundHandlerBlockStatement(BindStatement(handlerBlock, functionDefinitionSymbol, context));
    context->GetSymbolTable()->EndScope();
    otava::ast::DeclSpecifierSequenceNode* handlerDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex);
    handlerDeclSpecifiers->AddNode(new otava::ast::VoidNode(fullSpan.span, fullSpan.fileIndex));
    otava::ast::ParameterListNode* handlerParameters = new otava::ast::ParameterListNode(fullSpan.span, fullSpan.fileIndex);
    otava::ast::ParameterNode* handlerParameter = new otava::ast::ParameterNode(fullSpan.span, fullSpan.fileIndex);
    otava::ast::DeclSpecifierSequenceNode* handlerParamDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex);
    handlerParamDeclSpecifiers->AddNode(new otava::ast::VoidNode(fullSpan.span, fullSpan.fileIndex));
    handlerParameter->SetDeclSpecifiers(handlerParamDeclSpecifiers);
    otava::ast::PtrDeclaratorNode* handlerPtrDeclarator = new otava::ast::PtrDeclaratorNode(fullSpan.span, fullSpan.fileIndex);
    handlerPtrDeclarator->AddNode(new otava::ast::PtrNode(fullSpan.span, fullSpan.fileIndex));
    handlerPtrDeclarator->AddNode(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "__parentFrame"));
    handlerParameter->SetDeclarator(handlerPtrDeclarator);
    handlerParameters->AddNode(handlerParameter);
    otava::ast::FunctionDeclaratorNode* handlerDeclarator = new otava::ast::FunctionDeclaratorNode(fullSpan.span, fullSpan.fileIndex,
        new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "handler_" +
            std::to_string(trySerial) + "_" + context->GetBoundCompileUnit()->Id()), handlerParameters);
    std::unique_ptr<otava::ast::FunctionDefinitionNode> handlerFn(new otava::ast::FunctionDefinitionNode(fullSpan.span, fullSpan.fileIndex, nullptr,
        handlerDeclSpecifiers, handlerDeclarator, nullptr,
        new otava::ast::FunctionBodyNode(fullSpan.span, fullSpan.fileIndex, handlerBlock)));
    InstantiationScope handlerInstantiationScope(context->GetModule(), context->GetBoundFunction()->GetFunctionDefinitionSymbol()->Parent(context)->GetScope());
    //handlerInstantiationScope.PushParentScope(context->GetSymbolTable()->GetNamespaceScope(U"std", node.GetSourcePos(), context));
    handlerInstantiationScope.PushParentScope(context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context));
    context->GetSymbolTable()->BeginScope(&handlerInstantiationScope);
    Instantiator handlerInstantiator(context, &handlerInstantiationScope);
    handlerInstantiator.SetFunctionNode(handlerFn.get());
    context->PushSetFlag(ContextFlags::instantiateInlineFunction | ContextFlags::saveDeclarations | ContextFlags::dontBind | ContextFlags::tryCatch |
        ContextFlags::setParentBlockIds);
    context->PushParentFn(parentFn);
    handlerFn->Accept(handlerInstantiator);
    int handlerFnScopeCount = handlerInstantiator.ScopeCount();
    FunctionDefinitionSymbol* handlerFnSymbol = static_cast<FunctionDefinitionSymbol*>(handlerInstantiator.GetSpecialization());
    handlerFnSymbol->SetParentFn(parentFn);
    handlerFnSymbol->SetParentFnScope(parentFnScope);
    context->PushBoundFunction(new BoundFunctionNode(handlerFnSymbol, fullSpan));
    context->PushSetFlag(ContextFlags::makeChildFn);
    context->PushResultVarName(handlerResultVar);
    context->PushChildControlResultVarName(childControlResultVar);
    handlerFnSymbol->SetResultVarName(handlerResultVar);
    handlerFnSymbol = BindFunction(handlerFn.get(), handlerFnSymbol, context);
    context->PopParentFn();
    otava::symbols::EndFunctionDefinition(handlerFn.get(), handlerFnScopeCount, context);
    context->PopChildControlResultVarName();
    context->PopResultVarName();
    context->PopFlags();
    context->PopFlags();
    context->GetSymbolTable()->EndScope();
    handlerInstantiationScope.PopParentScope();
    handlerFnSymbol->SetFnDefNode(handlerFn.release());
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
    std::unique_ptr<otava::ast::CompoundStatementNode> invokeOrtTryBlock(new otava::ast::CompoundStatementNode(fullSpan.span, fullSpan.fileIndex));
    int blockId = -1;
    if (!functionDefinitionSymbol->Blocks().empty())
    {
        blockId = functionDefinitionSymbol->Blocks().front()->BlockId();
    }
    invokeOrtTryBlock->SetBlockId(blockId);
    if (resultType && boundTryVarDeclarationStatement && boundHandlerVarDeclarationStatement)
    {
        invokeOrtTryBlock->AddNode(new otava::ast::BoundStatementNode(boundTryVarDeclarationStatement->Clone(), fullSpan.span, fullSpan.fileIndex));
        invokeOrtTryBlock->AddNode(new otava::ast::BoundStatementNode(boundHandlerVarDeclarationStatement->Clone(), fullSpan.span, fullSpan.fileIndex));
    }
    invokeOrtTryBlock->AddNode(new otava::ast::BoundStatementNode(boundChildControlResultStatement.release(), fullSpan.span, fullSpan.fileIndex));
    invokeOrtTryBlock->AddNode(new otava::ast::BoundStatementNode(boundEhReturnFromStatement.release(), fullSpan.span, fullSpan.fileIndex));
    otava::ast::InvokeExprNode* invokeOrtTry = new otava::ast::InvokeExprNode(fullSpan.span, fullSpan.fileIndex,
        new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "ort_try"));
    invokeOrtTry->AddNode(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, tryFnSymbol->Name()));
    invokeOrtTry->AddNode(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, handlerFnSymbol->Name()));
    otava::ast::InvokeExprNode* getFramePtrIntrics = new otava::ast::InvokeExprNode(
        fullSpan.span, fullSpan.fileIndex, new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, "__intrinsic_get_frame_ptr"));
    invokeOrtTry->AddNode(getFramePtrIntrics);
    std::unique_ptr<otava::ast::ExpressionStatementNode> ortTryStmt(new otava::ast::ExpressionStatementNode(fullSpan.span, fullSpan.fileIndex,
        invokeOrtTry, nullptr, nullptr));
    invokeOrtTryBlock->AddNode(ortTryStmt.release());
    if (tryFnSymbol->GetFlag(FunctionSymbolFlags::containsReturnStatement))
    {
        std::string tryBlockReturnStmtText;
        tryBlockReturnStmtText.append("if (").append(ehReturnFromVar).append(" == std::eh_return_from::try_block && ").
            append(childControlResultVar).append(" == std::child_control_result::ret) return ").
            append(tryFnSymbol->ResultVarExprStr(resultType)).append(";");
        std::unique_ptr<otava::ast::Node> tryBlockReturnFromStmt = ParseStatement(tryBlockReturnStmtText, context);
        invokeOrtTryBlock->AddNode(tryBlockReturnFromStmt.release());
    }
    if (handlerFnSymbol->GetFlag(FunctionSymbolFlags::containsReturnStatement))
    {
        std::string handlerBlockReturnStmtText;
        handlerBlockReturnStmtText.append("if (").append(ehReturnFromVar).append(" == std::eh_return_from::handler_block && ").
            append(childControlResultVar).append(" == std::child_control_result::ret) return ").
            append(handlerFnSymbol->ResultVarExprStr(resultType)).append(";");
        std::unique_ptr<otava::ast::Node> handlerBlockReturnStmt = ParseStatement(handlerBlockReturnStmtText, context);
        invokeOrtTryBlock->AddNode(handlerBlockReturnStmt.release());
    }
    InstantiationScope ortTryBlockInstantiationScope(context->GetModule(), context->GetBoundFunction()->GetFunctionDefinitionSymbol()->Parent(context)->GetScope());
    ortTryBlockInstantiationScope.PushParentScope(context->GetSymbolTable()->CurrentScope());
    context->GetSymbolTable()->BeginScope(&ortTryBlockInstantiationScope);
    Instantiator ortTryBlockInstantiator(context, &ortTryBlockInstantiationScope);
    context->PushSetFlag(ContextFlags::saveDeclarations | ContextFlags::dontBind);
    invokeOrtTryBlock->Accept(ortTryBlockInstantiator);
    context->PopFlags();
    std::unique_ptr<BoundStatementNode> stmt = BindStatement(invokeOrtTryBlock.get(), functionDefinitionSymbol, context);
    SetStatement(stmt.release());
    context->GetSymbolTable()->EndScope();
    ortTryBlockInstantiationScope.PopParentScope();
    handlerBlock = prevHandlerBlock;
}

void StatementBinder::Visit(otava::ast::HandlerSequenceNode& node)
{
    lastElse = nullptr;
    for (const auto& handlerNode : node.Nodes())
    {
        handlerNode->Accept(*this);
    }
    if (lastElse)
    {
        context->PushResetFlag(~ContextFlags::sticky);
        std::unique_ptr<otava::ast::Node> resume = ParseStatement("ort_resume();", context);
        context->PopFlags();
        lastElse->AddNode(resume.release());
        lastElse = nullptr;
    }
}

void StatementBinder::Visit(otava::ast::HandlerNode& node)
{
    catchBlock = node.CatchBlock();
    node.Exception()->Accept(*this);
}

void StatementBinder::Visit(otava::ast::ExceptionDeclarationNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    context->PushResetFlag(~ContextFlags::sticky);
    context->PushSetFlag(ContextFlags::dontProcess);
    Declaration declaration = ProcessExceptionDeclaration(&node, context);
    TypeSymbol* type = declaration.type;
    if (type)
    {
        TypeSymbol* plainType = type->PlainType(context);
        TypeSymbol* baseType = type->GetBaseType(context);
        SymbolId ext = baseType->Id();
        std::string beginCatchStr;
        beginCatchStr.append("ort_begin_catch(").append(std::to_string(ToUnderlying(ext)).append("u"));
        std::unique_ptr<otava::ast::Node> beginCatchNode;
        try
        {
            beginCatchNode = ParseExpression(beginCatchStr, context);
        }
        catch (const std::exception& ex)
        {
            ThrowException(std::string("error parsing begin catch string: ") + ex.what(), fullSpan, context);
        }
        std::unique_ptr<otava::ast::CompoundStatementNode> completeCatchBlock(new otava::ast::CompoundStatementNode(fullSpan.span, fullSpan.fileIndex));
        completeCatchBlock->SetBlockId(context->NextBlockId());
        std::unique_ptr<otava::ast::CompoundStatementNode> elseBlock(new otava::ast::CompoundStatementNode(fullSpan.span, fullSpan.fileIndex));
        elseBlock->SetBlockId(context->NextBlockId());
        otava::ast::CompoundStatementNode* thisElse = elseBlock.get();
        TypeSymbol* exceptionPtrType = plainType->AddPointer(context);
        std::string makeExPtrStmtStr;
        makeExPtrStmtStr.append(exceptionPtrType->FullName(context));
        makeExPtrStmtStr.append(" __ex = static_cast<").append(exceptionPtrType->FullName(context)).append(">(ort_get_exception());");
        std::unique_ptr<otava::ast::Node> makeExPtrStmtNode = ParseStatement(makeExPtrStmtStr, context);
        completeCatchBlock->AddNode(makeExPtrStmtNode.release());
        std::string makeExRefStmtStr;
        makeExRefStmtStr.append(plainType->FullName(context));
        makeExRefStmtStr.append(" ").append(node.Declarator()->Str()).append(" = *__ex;");
        std::unique_ptr<otava::ast::Node> makeExRefStmtNode = ParseStatement(makeExRefStmtStr, context);
        completeCatchBlock->AddNode(makeExRefStmtNode.release());
        completeCatchBlock->AddNode(catchBlock->Clone());
        std::string deleteExceptionStmtStr;
        deleteExceptionStmtStr.append("if (!ort_current_ex_is_bad_alloc()) delete __ex;");
        std::unique_ptr<otava::ast::Node> deleteExceptionStmtNode = ParseStatement(deleteExceptionStmtStr, context);
        completeCatchBlock->AddNode(deleteExceptionStmtNode.release());
        std::unique_ptr<otava::ast::Node>  endCatchStmt = ParseStatement("ort_end_catch();", context);
        completeCatchBlock->AddNode(endCatchStmt.release());
        otava::ast::IfStatementNode* ifStmt = new otava::ast::IfStatementNode(fullSpan.span, fullSpan.fileIndex,
            beginCatchNode.release(), completeCatchBlock.release(), elseBlock.release(), nullptr, fullSpan.span, fullSpan.span,
            fullSpan.span, fullSpan.span, fullSpan.span);
        ifStmt->SetBlockId(context->NextBlockId());
        if (lastElse)
        {
            lastElse->AddNode(ifStmt);
        }
        else
        {
            handlerBlock->AddNode(ifStmt);
        }
        lastElse = thisElse;
    }
    else
    {
        if (lastElse)
        {
            lastElse->AddNode(catchBlock->Clone());
        }
        else
        {
            handlerBlock->AddNode(catchBlock->Clone());
        }
        lastElse = nullptr;
    }
    context->PopFlags();
    context->PopFlags();
}

void StatementBinder::Visit(otava::ast::AliasDeclarationNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    ProcessAliasDeclaration(&node, context);
    std::unique_ptr<BoundAliasDeclarationStatementNode> boundAliasDeclarationStatment(new BoundAliasDeclarationStatementNode(fullSpan));
    SetStatement(boundAliasDeclarationStatment.release());
}

void StatementBinder::Visit(otava::ast::SimpleDeclarationNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundStatementNode> boundStatement;
    std::unique_ptr<DeclarationList> declarationList = context->ReleaseDeclarationList(&node);
    if (declarationList)
    {
        bool setStatement = true;
        for (auto& declaration : declarationList->declarations)
        {
            VariableSymbol* variable = declaration.variable;
            if (variable->IsStatic())
            {
                BindStaticLocalVariable(variable, declaration.initializer, &node);
                setStatement = false;
            }
            else
            {
                std::unique_ptr<BoundExpressionNode> initializer;
                if (declaration.initializer)
                {
                    bool flagsPushed = false;
                    if (variable->GetDeclaredType(context)->IsReferenceType())
                    {
                        context->PushSetFlag(ContextFlags::returnRef);
                        flagsPushed = true;
                    }
                    context->SetDeclaredInitializerType(variable->GetDeclaredType(context));
                    initializer = BindExpression(declaration.initializer, context);
                    context->SetDeclaredInitializerType(nullptr);
                    if (flagsPushed)
                    {
                        context->PopFlags();
                    }
                }
                if (initializer && initializer->GetType())
                {
                    TypeSymbol* initializerType = initializer->GetType()->DirectType(context)->FinalType(fullSpan, context);
                    if (variable->GetDeclaredType(context)->GetBaseType(context)->IsAutoTypeSymbol() && 
                        variable->GetDeclaredType(context)->GetDerivations() != Derivations::none)
                    {
                        initializerType = context->GetSymbolTable()->MakeCompoundType(initializerType->GetBaseType(context),
                            variable->GetDeclaredType(context)->GetDerivations(), context);
                    }
                    variable->SetInitializerType(initializerType, context);
                }
                std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
                if (variable->FoundFromParent())
                {
                    BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(variable, fullSpan, variable->GetReferredType(context));
                    boundParentVariable->SetLevel(variable->Level());
                    arguments.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundParentVariable, fullSpan,
                        boundParentVariable->GetType()->AddPointer(context))));
                }
                else
                {
                    BoundVariableNode* boundVariable = new BoundVariableNode(variable, fullSpan, variable->GetReferredType(context));
                    arguments.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundVariable, fullSpan,
                        boundVariable->GetType()->AddPointer(context))));
                }
                if (initializer)
                {
                    if (initializer->IsBoundExpressionListNode())
                    {
                        BoundExpressionListNode* exprListNode = static_cast<BoundExpressionListNode*>(initializer.get());
                        int n = exprListNode->Count();
                        for (int i = 0; i < n; ++i)
                        {
                            arguments.push_back(std::unique_ptr<BoundExpressionNode>(exprListNode->ReleaseExpr(i)));
                        }
                    }
                    else
                    {
                        arguments.push_back(std::unique_ptr<BoundExpressionNode>(initializer.release()));
                    }
                }
                std::vector<TypeSymbol*> templateArgs;
                context->PushNodeId(node.Id());
                std::unique_ptr<BoundFunctionCallNode> constructorCall = ResolveOverloadThrow(context->GetSymbolTable()->CurrentScope(),
                    "@constructor", templateArgs, arguments, fullSpan, context);
                context->PopNodeId();
                constructorCall->SetSource(node.Clone());
                std::unique_ptr<BoundConstructionStatementNode> boundConstructionStatement;
                otava::symbols::ClassTypeSymbol* cls = nullptr;
                BoundExpressionNode* firstArg = nullptr;
                FunctionDefinitionSymbol* destructor = nullptr;
                if (constructorCall->CallsClassConstructor(cls, firstArg, destructor))
                {
                    boundConstructionStatement.reset(new BoundConstructionStatementNode(fullSpan, constructorCall.release()));
                    boundConstructionStatement->SetVariable(variable);
                    otava::symbols::BoundFunctionCallNode* destructorCall = MakeDestructorCall(cls, firstArg, destructor, fullSpan, context);
                    if (destructorCall)
                    {
                        if (!context->GetFlag(ContextFlags::invoke | ContextFlags::tryCatch))
                        {
                            boundConstructionStatement->SetDestructorCall(destructorCall);
                        }
                        else
                        {
                            int blockId = context->ParentBlockId();
                            Symbol* block = context->ParentFn()->GetBlock(blockId);
                            if (block && block->IsBlockSymbol())
                            {
                                BlockSymbol* blockSymbol = static_cast<BlockSymbol*>(block);
                                int statementIndex = context->ParentStatementIndex();
                                if (statementIndex != -1)
                                {
                                    blockSymbol->AddDestructorCall(statementIndex, destructorCall);
                                }
                            }
                        }
                        functionDefinitionSymbol->SetFlag(FunctionSymbolFlags::containsLocalVariableWithDestructor);
                    }
                }
                else
                {
                    boundConstructionStatement.reset(new BoundConstructionStatementNode(fullSpan, constructorCall.release()));
                    boundConstructionStatement->SetVariable(variable);
                }
                if (!context->GetFlag(ContextFlags::invoke | ContextFlags::tryCatch))
                {
                    functionDefinitionSymbol->AddLocalVariable(variable, context);
                }
                if (!boundStatement)
                {
                    boundStatement.reset(boundConstructionStatement.release());
                }
                else
                {
                    if (boundStatement->IsBoundConstructionStatementNode())
                    {
                        std::unique_ptr<BoundCompoundStatementNode> compoundStatement(new BoundCompoundStatementNode(fullSpan));
                        compoundStatement->AddStatement(boundStatement.release());
                        boundStatement.reset(compoundStatement.release());
                    }
                    else if (boundStatement->IsBoundCompoundStatementNode())
                    {
                        BoundCompoundStatementNode* compoundStatement = static_cast<BoundCompoundStatementNode*>(boundStatement.get());
                        compoundStatement->AddStatement(boundConstructionStatement.release());
                    }
                    else
                    {
                        ThrowException("bound construction statement or bound compound statement expected", fullSpan, context);
                    }
                }
                if (!context->GetFlag(ContextFlags::invoke))
                {
                    if (context->GetBoundFunction()->HasTemporaryDestructorCalls())
                    {
                        boundStatement->SetDestructTemporariesNode(
                            new BoundDestructTemporariesNode(fullSpan, context->GetBoundFunction()->GetTemporaryDestructorCalls()));
                        boundStatement->SetTemporaryDestructorCallsObtained();
                    }
                }
            }
        }
        if (setStatement)
        {
            SetStatement(boundStatement.release());
        }
    }
}

void StatementBinder::SetStatement(BoundStatementNode* statement)
{
    soul::ast::FullSpan fullSpan = statement->GetFullSpan();
    if (postfix)
    {
        statement->SetPostfix();
    }
    if (boundStatement)
    {
        if (boundStatement->Postfix())
        {
            BoundSequenceStatementNode* sequenceStatement = new BoundSequenceStatementNode(fullSpan, statement, boundStatement.release());
            statement = sequenceStatement;
        }
        else
        {
            BoundSequenceStatementNode* sequenceStatement = new BoundSequenceStatementNode(statement->GetFullSpan(), boundStatement.release(), statement);
            statement = sequenceStatement;
        }
        if (postfix)
        {
            statement->SetPostfix();
        }
    }
    boundStatement.reset(statement);
}

struct FunctionStaticDeclarationExtractor : public otava::ast::DefaultVisitor
{
    FunctionStaticDeclarationExtractor(const soul::ast::FullSpan& fullSpan_, otava::ast::IdentifierNode* globalVarId_, bool isArrayVar_) noexcept;
    void Visit(otava::ast::SimpleDeclarationNode& node) override;
    void Visit(otava::ast::DeclSpecifierSequenceNode& node) override;
    void Visit(otava::ast::InitDeclaratorListNode& node) override;
    void Visit(otava::ast::InitDeclaratorNode& node) override;
    void Visit(otava::ast::PtrDeclaratorNode& node) override;
    void Visit(otava::ast::PtrNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::ArrayDeclaratorNode& node) override;
    soul::ast::FullSpan fullSpan;
    otava::ast::IdentifierNode* globalVarId;
    otava::ast::DeclSpecifierSequenceNode* declSpecifiers;
    otava::ast::InitDeclaratorListNode* initDeclarators;
    otava::ast::PtrDeclaratorNode* ptrDeclaratorNode;
    otava::ast::Node* declaratorNode;
    bool isArrayVar;
};

FunctionStaticDeclarationExtractor::FunctionStaticDeclarationExtractor(const soul::ast::FullSpan& fullSpan_, otava::ast::IdentifierNode* globalVarId_,
    bool isArrayVar_) noexcept :
    fullSpan(fullSpan_), globalVarId(globalVarId_), ptrDeclaratorNode(nullptr), declaratorNode(nullptr), isArrayVar(isArrayVar_), declSpecifiers(nullptr),
    initDeclarators(nullptr)
{
}

void FunctionStaticDeclarationExtractor::Visit(otava::ast::SimpleDeclarationNode& node)
{
    declSpecifiers = new otava::ast::DeclSpecifierSequenceNode(node.GetSpan(), node.FileIndex());
    initDeclarators = new otava::ast::InitDeclaratorListNode(node.GetSpan(), node.FileIndex());
    node.DeclarationSpecifiers()->Accept(*this);
    node.InitDeclaratorList()->Accept(*this);
}

void FunctionStaticDeclarationExtractor::Visit(otava::ast::DeclSpecifierSequenceNode& node)
{
    for (const auto& declSpecifierNode : node.Nodes())
    {
        if (declSpecifierNode->IsStaticNode())
        {
            continue;
        }
        declSpecifiers->AddNode(declSpecifierNode->Clone());
    }
}

void FunctionStaticDeclarationExtractor::Visit(otava::ast::PtrNode& node)
{
    if (ptrDeclaratorNode)
    {
        ptrDeclaratorNode->AddNode(node.Clone());
    }
}

void FunctionStaticDeclarationExtractor::Visit(otava::ast::IdentifierNode& node)
{
    if (ptrDeclaratorNode)
    {
        ptrDeclaratorNode->AddNode(globalVarId->Clone());
    }
    else
    {
        declaratorNode = globalVarId->Clone();
    }
}

void FunctionStaticDeclarationExtractor::Visit(otava::ast::QualifiedIdNode& node)
{
    if (ptrDeclaratorNode)
    {
        ptrDeclaratorNode->AddNode(globalVarId->Clone());
    }
    else
    {
        declaratorNode = globalVarId->Clone();
    }
}

void FunctionStaticDeclarationExtractor::Visit(otava::ast::PtrDeclaratorNode& node)
{
    ptrDeclaratorNode = new otava::ast::PtrDeclaratorNode(node.GetSpan(), node.FileIndex());
    for (const auto& node : node.Nodes())
    {
        node->Accept(*this);
    }
    initDeclarators->AddNode(ptrDeclaratorNode);
    ptrDeclaratorNode = nullptr;
}

void FunctionStaticDeclarationExtractor::Visit(otava::ast::ArrayDeclaratorNode& node)
{
    node.Child()->Accept(*this);
    otava::ast::Node* dimension = nullptr;
    if (node.Dimension())
    {
        dimension = node.Dimension()->Clone();
    }
    declaratorNode = new otava::ast::ArrayDeclaratorNode(node.GetSpan(), node.FileIndex(), declaratorNode, dimension, node.GetSpan(), node.GetSpan());
}

void FunctionStaticDeclarationExtractor::Visit(otava::ast::InitDeclaratorNode& node)
{
    node.Left()->Accept(*this);
    if (isArrayVar)
    {
        declaratorNode = new otava::ast::InitDeclaratorNode(node.GetSpan(), node.FileIndex(), declaratorNode, node.Right()->Clone());
    }
    if (declaratorNode)
    {
        initDeclarators->AddNode(declaratorNode);
        declaratorNode = nullptr;
    }
}

void FunctionStaticDeclarationExtractor::Visit(otava::ast::InitDeclaratorListNode& node)
{
    for (const auto& declNode : node.Nodes())
    {
        declNode->Accept(*this);
        if (declaratorNode)
        {
            initDeclarators->AddNode(declaratorNode);
            declaratorNode = nullptr;
        }
    }
}

void StatementBinder::Visit(otava::ast::BoundStatementNode& node)
{
    SetStatement(static_cast<BoundStatementNode*>(node.GetBoundStatementNode()));
}

void StatementBinder::Visit(otava::ast::GotoStatementNode& node)
{
    functionDefinitionSymbol->SetContainsGotosOrLabels();
    std::unique_ptr<BoundGotoStatementNode> gotoStatement(new BoundGotoStatementNode(node.GetFullSpan(), node.Target()->Str()));
    SetStatement(gotoStatement.release());
}

void StatementBinder::Visit(otava::ast::LabeledStatementNode& node)
{
    functionDefinitionSymbol->SetContainsGotosOrLabels();
    std::unique_ptr<BoundStatementNode> statement = BindStatement(node.Statement(), functionDefinitionSymbol, context);
    std::unique_ptr<BoundLabeledStatementNode> labeledStatement(new BoundLabeledStatementNode(node.GetFullSpan(),
        node.Label()->Str(), statement.release()));
    SetStatement(labeledStatement.release());
}

void StatementBinder::BindStaticLocalVariable(VariableSymbol* variable, otava::ast::Node* initializerNode,
    otava::ast::SimpleDeclarationNode* declarationNode)
{
    /*
        std::atomic_bool variable_initialized;
        if (!variable_initialized)
        {
            std::lock_guard<std::recursive_mutex> lock(std::get_init_lock());
            if (!variable_initialized)
            {
                type variable(initializer);
                variable_initialized = true;
            }
        }
    */
    soul::ast::FullSpan fullSpan = declarationNode->GetFullSpan();
    soul::ast::Span span = declarationNode->GetSpan();
    int fileIndex = fullSpan.fileIndex;
    functionDefinitionSymbol->SetContainsStatics();
    bool isArrayVar = variable->GetDeclaredType(context)->IsArrayTypeSymbol();
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    std::string shaMaterial = functionDefinitionSymbol->FullName(context);
    if (!functionDefinitionSymbol->Specialization().empty())
    {
        for (TypeSymbol* type : functionDefinitionSymbol->Specialization())
        {
            shaMaterial.append(".").append(type->FullName(context));
        }
    }
    shaMaterial.append(".").append(context->GetBoundCompileUnit()->Id()).
        append(1, '_').append(std::to_string(context->GetBoundFunction()->Serial()));
    std::string sha = util::GetSha1MessageDigest(shaMaterial);
    otava::ast::Node* atomicBoolType = MakeTypeNameNodes(fullSpan, "std::atomic_bool");
    otava::ast::DeclSpecifierSequenceNode* declSpecifiers = new otava::ast::DeclSpecifierSequenceNode(span, fileIndex);
    declSpecifiers->AddNode(atomicBoolType);
    otava::ast::InitDeclaratorListNode* initDeclarators = new otava::ast::InitDeclaratorListNode(span, fileIndex);
    std::unique_ptr<otava::ast::IdentifierNode> initializedVarName(new otava::ast::IdentifierNode(span, fileIndex,
        variable->Name() + "_initialized_" + sha));
    initDeclarators->AddNode(initializedVarName->Clone());
    std::unique_ptr<otava::ast::SimpleDeclarationNode> initializedVarDeclaration(new otava::ast::SimpleDeclarationNode(
        span, fileIndex, declSpecifiers, initDeclarators, nullptr, nullptr));
    context->GetSymbolTable()->BeginScope(context->GetSymbolTable()->GlobalNs()->GetScope());
    ProcessSimpleDeclaration(initializedVarDeclaration.get(), nullptr, context);
    std::string globalStaticVarName = variable->Name() + "_global_" + sha;
    std::unique_ptr<otava::ast::IdentifierNode> globalStaticVarId(new otava::ast::IdentifierNode(span, fileIndex, globalStaticVarName));
    FunctionStaticDeclarationExtractor extractor(fullSpan, globalStaticVarId.get(), isArrayVar);
    declarationNode->Accept(extractor);
    otava::ast::DeclSpecifierSequenceNode* globalStaticDeclSpecifiers = extractor.declSpecifiers;
    otava::ast::InitDeclaratorListNode* globalStaticInitDeclarators = extractor.initDeclarators;
    context->PushSetFlag(ContextFlags::noDynamicInit);
    std::unique_ptr<otava::ast::SimpleDeclarationNode> globalStaticVarDeclaration(new otava::ast::SimpleDeclarationNode(span, fileIndex,
        globalStaticDeclSpecifiers, globalStaticInitDeclarators, nullptr, nullptr));
    if (isArrayVar)
    {
        context->SetDeclaredInitializerType(variable->GetDeclaredType(context));
    }
    ProcessSimpleDeclaration(globalStaticVarDeclaration.get(), nullptr, context);
    context->PopFlags();
    context->GetSymbolTable()->EndScope();
    std::unique_ptr<otava::ast::CompoundStatementNode> compound1;
    std::unique_ptr<otava::ast::UnaryExprNode> inititalizedCond;
    std::unique_ptr<otava::ast::CompoundStatementNode> compound2;
    if (!isArrayVar)
    {
        compound1.reset(new otava::ast::CompoundStatementNode(span, fileIndex));
        compound1->SetBlockId(context->NextBlockId());
        otava::ast::TemplateIdNode* lockGuardType = new otava::ast::TemplateIdNode(span, fileIndex, MakeTypeNameNodes(fullSpan, "std::lock_guard"));
        lockGuardType->AddNode(MakeTypeNameNodes(fullSpan, "std::recursive_mutex"));
        otava::ast::DeclSpecifierSequenceNode* lockGuardDeclSpecifiers = new otava::ast::DeclSpecifierSequenceNode(span, fileIndex);
        lockGuardDeclSpecifiers->AddNode(lockGuardType);
        otava::ast::InitDeclaratorListNode* lockGuardInitDeclarators = new otava::ast::InitDeclaratorListNode(span, fileIndex);
        otava::ast::Node* getInitLock = MakeTypeNameNodes(fullSpan, "std::get_init_lock");
        otava::ast::InvokeExprNode* lockInit = new otava::ast::InvokeExprNode(span, fileIndex, getInitLock);
        otava::ast::InitDeclaratorNode* lockGuardInitDeclarator = new otava::ast::InitDeclaratorNode(
            span, fileIndex, new otava::ast::IdentifierNode(span, fileIndex, "lock"), lockInit);
        lockGuardInitDeclarators->AddNode(lockGuardInitDeclarator);
        std::unique_ptr<otava::ast::SimpleDeclarationNode> lockGuardDeclaration(new otava::ast::SimpleDeclarationNode(
            span, fileIndex, lockGuardDeclSpecifiers, lockGuardInitDeclarators, nullptr, nullptr));
        otava::ast::DeclarationStatementNode* lockGuardDeclarationStmt = new otava::ast::DeclarationStatementNode(span, fileIndex, lockGuardDeclaration.release());
        compound1->AddNode(lockGuardDeclarationStmt);
        inititalizedCond.reset(new otava::ast::UnaryExprNode(span, fileIndex, new otava::ast::NotNode(span, fileIndex), initializedVarName->Clone()));
        compound2.reset(new otava::ast::CompoundStatementNode(span, fileIndex));
        compound2->SetBlockId(context->NextBlockId());
    }
    globalStaticVariableSymbol = nullptr;
    Symbol* symbol = context->GetSymbolTable()->Lookup(globalStaticVarName, SymbolGroupKind::variableSymbolGroup, fullSpan, context);
    if (symbol && symbol->IsVariableSymbol())
    {
        globalStaticVariableSymbol = static_cast<VariableSymbol*>(symbol);
    }
    else if (symbol && symbol->IsVariableGroupSymbol())
    {
        std::string irName = globalStaticVarName;
        ThrowException("ir name '" + irName + "' of function static global variable not unique", fullSpan, context);
    }
    else
    {
        ThrowException("function static global not found", fullSpan, context);
    }
    if (!isArrayVar)
    {
        BoundVariableNode* boundVariable = new BoundVariableNode(globalStaticVariableSymbol, fullSpan, globalStaticVariableSymbol->GetReferredType(context));
        std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
        arguments.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundVariable, fullSpan, boundVariable->GetType()->AddPointer(context))));
        std::unique_ptr<BoundExpressionNode> initializer;
        if (initializerNode)
        {
            initializer = BindExpression(initializerNode, context);
        }
        if (initializer)
        {
            if (initializer->IsBoundExpressionListNode())
            {
                BoundExpressionListNode* exprListNode = static_cast<BoundExpressionListNode*>(initializer.get());
                int n = exprListNode->Count();
                for (int i = 0; i < n; ++i)
                {
                    arguments.push_back(std::unique_ptr<BoundExpressionNode>(exprListNode->ReleaseExpr(i)));
                }
            }
            else
            {
                arguments.push_back(std::unique_ptr<BoundExpressionNode>(initializer.release()));
            }
        }
        std::vector<TypeSymbol*> templateArgs;
        std::unique_ptr<BoundFunctionCallNode> constructorCall = ResolveOverloadThrow(context->GetSymbolTable()->CurrentScope(), "@constructor",
            templateArgs, arguments, fullSpan, context);
        constructFunctionStaticStatement.reset(new BoundExpressionStatementNode(fullSpan));
        if (!context->GetFlag(ContextFlags::invoke))
        {
            if (context->GetBoundFunction()->HasTemporaryDestructorCalls())
            {
                constructFunctionStaticStatement->SetDestructTemporariesNode(
                    new BoundDestructTemporariesNode(fullSpan, context->GetBoundFunction()->GetTemporaryDestructorCalls()));
                constructFunctionStaticStatement->SetTemporaryDestructorCallsObtained();
            }
        }
        constructFunctionStaticStatement->SetExpr(constructorCall.release(), fullSpan, context);
        otava::ast::BoundStatementNode* boundStatement = new otava::ast::BoundStatementNode(constructFunctionStaticStatement.release(), span, fileIndex);
        compound2->AddNode(boundStatement);
        std::unique_ptr<BoundFunctionCallNode> atExitCall = MakeAtExitForVariable(globalStaticVariableSymbol, fullSpan, context);
        if (atExitCall)
        {
            atExitStatement.reset(new BoundExpressionStatementNode(fullSpan));
            atExitStatement->SetExpr(atExitCall.release(), fullSpan, context);
            otava::ast::BoundStatementNode* boundStatement = new otava::ast::BoundStatementNode(atExitStatement.release(), span, fileIndex);
            compound2->AddNode(boundStatement);
        }
        otava::ast::BinaryExprNode* setInitializedToTrueExpr = new otava::ast::BinaryExprNode(span, fileIndex, new otava::ast::AssignNode(span, fileIndex),
            initializedVarName->Clone(),
            new otava::ast::BooleanLiteralNode(span, fileIndex, true, "true"));
        std::unique_ptr<otava::ast::ExpressionStatementNode> setInitializedToTrueStmt(new otava::ast::ExpressionStatementNode(
            span, fileIndex, setInitializedToTrueExpr, nullptr, nullptr));
        compound2->AddNode(setInitializedToTrueStmt.release());
        std::unique_ptr<otava::ast::IfStatementNode> innerIf(new otava::ast::IfStatementNode(
            span, fileIndex, inititalizedCond->Clone(), compound2.release(), nullptr, nullptr, span, span, span, span, span));
        compound1->AddNode(innerIf.release());
        std::unique_ptr<otava::ast::IfStatementNode> ifStmt(new otava::ast::IfStatementNode(
            span, fileIndex, inititalizedCond->Clone(), compound1.release(), nullptr, nullptr, span, span, span, span, span));
        InstantiationScope instantiationScope(context->GetModule(), context->GetSymbolTable()->CurrentScope());
        Instantiator instantiator(context, &instantiationScope);
        context->PushSetFlag(ContextFlags::saveDeclarations | ContextFlags::dontBind);
        ifStmt->Accept(instantiator);
        context->PopFlags();
        context->PushSetFlag(ContextFlags::skipInvokeChecking);
        std::unique_ptr<BoundStatementNode> boundIfStmt = BindStatement(ifStmt.get(), functionDefinitionSymbol, context);
        context->PopFlags();
        SetStatement(boundIfStmt.release());
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
    }
    context->SetDeclaredInitializerType(nullptr);
    variable->SetGlobal(globalStaticVariableSymbol);
}

std::unique_ptr<BoundStatementNode> BindStatement(otava::ast::Node* statementNode, FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context)
{
    StatementBinder binder(context, functionDefinitionSymbol);
    statementNode->Accept(binder);
    std::unique_ptr<BoundStatementNode> boundStatement = binder.GetBoundStatement();
    if (!context->GetFlag(ContextFlags::invoke))
    {
        if (context->GetBoundFunction()->HasTemporaryDestructorCalls())
        {
            if (boundStatement->IsBoundSequenceStatementNode())
            {
                BoundSequenceStatementNode* sequenceStatement = static_cast<BoundSequenceStatementNode*>(boundStatement.get());
                if (sequenceStatement->Second()->IsReturnStatementNode())
                {
                    sequenceStatement->First()->SetDestructTemporariesNode(
                        new BoundDestructTemporariesNode(statementNode->GetFullSpan(), context->GetBoundFunction()->GetTemporaryDestructorCalls()));
                    sequenceStatement->First()->SetTemporaryDestructorCallsObtained();
                }
                else
                {
                    boundStatement->SetDestructTemporariesNode(
                        new BoundDestructTemporariesNode(statementNode->GetFullSpan(), context->GetBoundFunction()->GetTemporaryDestructorCalls()));
                    boundStatement->SetTemporaryDestructorCallsObtained();
                }
            }
            else
            {
                boundStatement->SetDestructTemporariesNode(
                    new BoundDestructTemporariesNode(statementNode->GetFullSpan(), context->GetBoundFunction()->GetTemporaryDestructorCalls()));
                boundStatement->SetTemporaryDestructorCallsObtained();
            }
        }
    }
    return boundStatement;
}

FunctionDefinitionSymbol* BindFunction(otava::ast::Node* functionDefinitionNode, FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context)
{
    TraceInfo* traceInfo = context->GetTraceInfo();
    if (traceInfo)
    {
        int64_t sourceFileId = traceInfo->GetSourceFileId(context->GetModule()->FilePath());
        if (sourceFileId != -1)
        {
            traceInfo->AddFunctionTraceInfo(sourceFileId, ToUnderlying(context->GetModule()->Id()), functionDefinitionSymbol->FullName(context));
        }
        else
        {
            SetExceptionThrown();
            throw std::runtime_error("source file id for source file '" + context->GetModule()->FilePath() + "' not set");
        }
    }
    if (!functionDefinitionSymbol->IsSpecialization())
    {
        if (functionDefinitionSymbol->Declaration())
        {
            functionDefinitionSymbol->Declaration()->SetFlag(FunctionSymbolFlags::fixedIrName);
        }
        functionDefinitionSymbol->SetFlag(FunctionSymbolFlags::fixedIrName);
    }
    RemoveTemporaryAliasTypeSymbols(context);
    if (functionDefinitionSymbol->IsBound()) return functionDefinitionSymbol;
    if (functionDefinitionSymbol->IsTemplate(context)) return functionDefinitionSymbol;
    if (context->GetFlag(ContextFlags::parseMemberFunction)) return functionDefinitionSymbol;
    std::set<const Symbol*> visited;
    if (functionDefinitionSymbol->IsTemplateParameterInstantiation(context, visited)) return functionDefinitionSymbol;
#ifdef DEBUG_FUNCTIONS
    std::cout << ">" << functionDefinitionSymbol->FullName(context) << "\n";
#endif
    functionDefinitionSymbol->SetBound();
    if (context->GetFlag(ContextFlags::debugMemory))
    {
        context->SetFunction(functionDefinitionSymbol->FullName(context));
    }
    StatementBinder binder(context, functionDefinitionSymbol);
    context->PushStatementBinder(&binder);
    GenerateEnterFunctionCode(functionDefinitionNode, functionDefinitionSymbol, context);
    functionDefinitionNode->Accept(binder);
    functionDefinitionSymbol = binder.GetFunctionDefinitionSymbol();
    bool hasNoReturnAttribute = false;
    if (functionDefinitionNode->IsFunctionDefinitionNode())
    {
        otava::ast::FunctionDefinitionNode* fndn = static_cast<otava::ast::FunctionDefinitionNode*>(functionDefinitionNode);
        hasNoReturnAttribute = HasNoReturnAttribute(fndn->Attributes());
    }
    if (functionDefinitionSymbol->ReturnType(context) &&
        !functionDefinitionSymbol->ReturnType(context)->IsVoidType() &&
        functionDefinitionSymbol->GroupName() != "main" &&
        functionDefinitionSymbol->GroupName() != "@destructor" &&
        !hasNoReturnAttribute)
    {
        CheckFunctionReturnPaths(functionDefinitionNode, context);
    }
/*
    bool skipInvokeChecking = functionDefinitionSymbol->SkipInvokeChecking();
    bool containsStatics = functionDefinitionSymbol->ContainsStatics();
    if (!functionDefinitionSymbol->IsNoExcept() && !skipInvokeChecking && !containsStatics && !gendoc &&
        functionDefinitionSymbol->ContainsLocalVariableWithDestructor())
    {
        MakeInvokesAndCleanups(context->GetBoundFunction(), context);
    }
*/
#ifdef DEBUG_FUNCTIONS
    std::cout << "<" << functionDefinitionSymbol->FullName(context) << "\n";
#endif
    context->PopStatementBinder();
    return functionDefinitionSymbol;
}

} // namespace otava::symbols
