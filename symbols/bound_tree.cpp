// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.bound_tree;

import otava.symbols.argument_conversion_table;
import otava.symbols.class_templates;
import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.function_symbol;
import otava.symbols.function_templates;
import otava.symbols.bound_tree_visitor;
import otava.symbols.inline_functions;
import otava.symbols.operation_repository;
import otava.symbols.type_resolver;
import otava.symbols.type_symbol;
import otava.symbols.variable_symbol;
import otava.ast.error;
import otava.intermediate.error;
import otava.optimizer.error;
import otava.assembly.error;

namespace otava::symbols {

std::string BoundNodeKindStr(BoundNodeKind nodeKind)
{
    switch (nodeKind)
    {
    case BoundNodeKind::boundCompileUnitNode:
    {
        return "boundCompileUnitNode";
    }
    case BoundNodeKind::boundFunctionNode:
    {
        return "boundFunctionNode";
    }
    case BoundNodeKind::boundCompoundStatementNode:
    {
        return "boundCompoundStatementNode";
    }
    case BoundNodeKind::boundIfStatementNode:
    {
        return "boundIfStatementNode";
    }
    case BoundNodeKind::boundSwitchStatementNode:
    {
        return "boundSwitchStatementNode";
    }
    case BoundNodeKind::boundCaseStatementNode:
    {
        return "boundCaseStatementNode";
    }
    case BoundNodeKind::boundDefaultStatementNode:
    {
        return "boundDefaultStatementNode";
    }
    case BoundNodeKind::boundWhileStatementNode:
    {
        return "boundWhileStatementNode";
    }
    case BoundNodeKind::boundDoStatementNode:
    {
        return "boundDoStatementNode";
    }
    case BoundNodeKind::boundForStatementNode:
    {
        return "boundForStatementNode";
    }
    case BoundNodeKind::boundBreakStatementNode:
    {
        return "boundBreakStatementNode";
    }
    case BoundNodeKind::boundContinueStatementNode:
    {
        return "boundContinueStatementNode";
    }
    case BoundNodeKind::boundReturnStatementNode:
    {
        return "boundReturnStatementNode";
    }
    case BoundNodeKind::boundGotoStatementNode:
    {
        return "boundGotoStatementNode";
    }
    case BoundNodeKind::boundConstructionStatementNode:
    {
        return "boundConstructionStatementNode";
    }
    case BoundNodeKind::boundExpressionStatementNode:
    {
        return "boundExpressionStatementNode";
    }
    case BoundNodeKind::boundSequenceStatementNode:
    {
        return "boundSequenceStatementNode";
    }
    case BoundNodeKind::boundSetVPtrStatementNode:
    {
        return "boundSetVPtrStatementNode";
    }
    case BoundNodeKind::boundHandlerNode:
    {
        return "boundHandlerNode";
    }
    case BoundNodeKind::boundValueNode:
    {
        return "boundValueNode";
    }
    case BoundNodeKind::boundLiteralNode:
    {
        return "boundLiteralNode";
    }
    case BoundNodeKind::boundVariableNode:
    {
        return "boundVariableNode";
    }
    case BoundNodeKind::boundParentVariableNode:
    {
        return "boundParentVariableNode";
    }
    case BoundNodeKind::boundParameterNode:
    {
        return "boundParameterNode";
    }
    case BoundNodeKind::boundParentParameterNode:
    {
        return "boundParentParameterNode";
    }
    case BoundNodeKind::boundEnumConstantNode:
    {
        return "boundEnumConstantNode";
    }
    case BoundNodeKind::boundFunctionGroupNode:
    {
        return "boundFunctionGroupNode";
    }
    case BoundNodeKind::boundTypeNode:
    {
        return "boundTypeNode";
    }
    case BoundNodeKind::boundMemberExprNode:
    {
        return "boundMemberExprNode";
    }
    case BoundNodeKind::boundFunctionCallNode:
    {
        return "boundFunctionCallNode";
    }
    case BoundNodeKind::boundEmptyFunctionCallNode:
    {
        return "boundEmptyFunctionCallNode";
    }
    case BoundNodeKind::boundInvokeNode:
    {
        return "boundInvokeNode";
    }
    case BoundNodeKind::boundExpressionSequenceNode:
    {
        return "boundExpressionSequenceNode";
    }
    case BoundNodeKind::boundExpressionListNode:
    {
        return "boundExpressionListNode";
    }
    case BoundNodeKind::boundDisjunctionNode:
    {
        return "boundDisjunctionNode";
    }
    case BoundNodeKind::boundConjunctionNode:
    {
        return "boundConjunctionNode";
    }
    case BoundNodeKind::boundConversionNode:
    {
        return "boundConversionNode";
    }
    case BoundNodeKind::boundAddressOfNode:
    {
        return "boundAddressOfNode";
    }
    case BoundNodeKind::boundDereferenceNode:
    {
        return "boundDereferenceNode";
    }
    case BoundNodeKind::boundRefToPtrNode:
    {
        return "boundRefToPtrNode";
    }
    case BoundNodeKind::boundPtrToRefNode:
    {
        return "boundPtrToRefNode";
    }
    case BoundNodeKind::boundDefaultInitNode:
    {
        return "boundDefaultInitNode";
    }
    case BoundNodeKind::boundConstructTemporaryNode:
    {
        return "boundConstructTemporaryNode";
    }
    case BoundNodeKind::boundConstructExpressionNode:
    {
        return "boundConstructExpressionNode";
    }
    case BoundNodeKind::boundClassNode:
    {
        return "boundClassNode";
    }
    case BoundNodeKind::boundEmptyDestructorNode:
    {
        return "boundEmptyDestructorNode";
    }
    case BoundNodeKind::boundFunctionValueNode:
    {
        return "boundFunctionValueNode";
    }
    case BoundNodeKind::boundVariableAsVoidPtrNode:
    {
        return "boundVariableAsVoidPtrNode";
    }
    }
    return "<unknown bound node>";
}

struct BoundNodeLess
{
    bool operator()(const std::unique_ptr<BoundNode>& left, const std::unique_ptr<BoundNode>& right) const noexcept;
};

bool BoundNodeLess::operator()(const std::unique_ptr<BoundNode>& left, const std::unique_ptr<BoundNode>& right) const noexcept
{
    if (left->Kind() < right->Kind()) return true;
    if (left->Kind() > right->Kind()) return false;
    return left->Index() < right->Index();
}

BoundNode::BoundNode(BoundNodeKind kind_, const soul::ast::FullSpan& fullSpan_) noexcept :
    kind(kind_), index(-1), fullSpan(fullSpan_), source(nullptr), temporaryDestructorCallsObtained(false)
{
}

BoundNode::~BoundNode()
{
}

BoundExpressionNode::BoundExpressionNode(BoundNodeKind kind_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundNode(kind_, fullSpan_), flags(BoundExpressionFlags::none), type(type_), destructTemporariesNode(nullptr)
{
}

BoundExpressionNode::~BoundExpressionNode()
{
    if (destructTemporariesNode)
    {
        delete destructTemporariesNode;
    }
}

void BoundExpressionNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("LOAD not implemented for " + BoundNodeKindStr(Kind()), fullSpan, context);
}

void BoundExpressionNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("STORE not implemented for " + BoundNodeKindStr(Kind()), fullSpan, context);
}

void BoundExpressionNode::SetDestructTemporariesNode(BoundDestructTemporariesNode* destructTemporariesNode_) noexcept
{
    destructTemporariesNode = destructTemporariesNode_;
}

void BoundExpressionNode::DestructTemporaries(Emitter& emitter, Context* context)
{
    if (!destructTemporariesNode) return;
    destructTemporariesNode->Load(emitter, OperationFlags::none, GetFullSpan(), context);
}

Scope* BoundExpressionNode::GetMemberScope(otava::ast::Node* op, const soul::ast::FullSpan& fullSpan, Context* context) const
{
    if (type)
    {
        if (op->IsDotNode())
        {
            TypeSymbol* baseType = type->DirectType(context)->FinalType(fullSpan, context)->GetBaseType(context);
            return baseType->GetScope();
        }
        else if (op->IsArrowNode() && type->IsPointerType())
        {
            TypeSymbol* baseType = type->DirectType(context)->FinalType(fullSpan, context)->RemovePointer(context)->GetBaseType(context);
            return baseType->GetScope();
        }
        else if (op->IsArrowNode() && type->PlainType(context)->IsClassTypeSymbol())
        {
            TypeSymbol* baseType = type->PlainType(context)->DirectType(context)->FinalType(fullSpan, context);
            return baseType->GetScope();
        }
    }
    return nullptr;
}

void BoundExpressionNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (type)
    {
        type = type->FinalType(fullSpan, context)->DirectType(context);
    }
}

BoundValueExpressionNode::BoundValueExpressionNode(otava::intermediate::Value* value_, TypeSymbol* type) :
    BoundExpressionNode(BoundNodeKind::boundValueNode, soul::ast::FullSpan(), type), value(value_)
{
}

void BoundValueExpressionNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundValueExpressionNode::Clone() const
{
    BoundExpressionNode* clone = new BoundValueExpressionNode(value, GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    return clone;
}

void BoundValueExpressionNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    emitter.Stack().Push(value);
}

BoundCompileUnitNode::BoundCompileUnitNode() :
    BoundNode(BoundNodeKind::boundCompileUnitNode, soul::ast::FullSpan()),
    operationRepository(new OperationRepository()),
    argumentConversionTable(new ArgumentConversionTable()),
    functionTemplateRepository(new FunctionTemplateRepository()),
    classTemplateRepository(new ClassTemplateRepository()),
    inlineFunctionRepository(new InlineFunctionRepository()),
    id(),
    compileUnitInitializationFunction(nullptr)
{
}

BoundCompileUnitNode::~BoundCompileUnitNode()
{
    if (compileUnitInitializationFunction)
    {
        delete compileUnitInitializationFunction;
    }
}

void BoundCompileUnitNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundCompileUnitNode::AddBoundNode(std::unique_ptr<BoundNode>&& node, Context* context)
{
    if (node->IsBoundFunctionNode())
    {
        BoundFunctionNode* boundFunction = static_cast<BoundFunctionNode*>(node.get());
        std::set<const Symbol*> visited;
        if (boundFunction->GetFunctionDefinitionSymbol()->IsTemplateParameterInstantiation(context, visited))
        {
            return;
        }
        //FunctionDefinitionSymbolSet* functionDefinitionSymbolSet = context->GetFunctionDefinitionSymbolSet(); TODO?
        FunctionDefinitionSymbol* functionDefinition = boundFunction->GetFunctionDefinitionSymbol();
        functionDefinition->SetFlag(FunctionSymbolFlags::fixedIrName);
        functionDefinition->IrName(context);
/*
        if (functionDefinitionSymbolSet->FunctionDefinitionSymbolFound(functionDefinition))
        {
            return;
        }
        functionDefinitionSymbolSet->AddFunctionDefinitionSymbol(functionDefinition);
*/
    }
    if (context->GetFlag(ContextFlags::generatingVTab))
    {
        if (node->IsBoundFunctionNode())
        {
            context->AddBoundVTabFunction(static_cast<BoundFunctionNode*>(node.release()));
        }
    }
    else
    {
        node->SetIndex(boundNodes.size());
        boundNodes.push_back(std::move(node));
    }
}

void BoundCompileUnitNode::AddBoundNodeForClass(ClassTypeSymbol* cls, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::set<const Symbol*> visited;
    if (cls->IsTemplateParameterInstantiation(context, visited)) return;
    if (boundClasses.find(cls) != boundClasses.end()) return;
    boundClasses.insert(cls);
    AddBoundNode(std::unique_ptr<BoundNode>(new BoundClassNode(cls, fullSpan)), context);
    for (ClassTypeSymbol* base : cls->BaseClasses(context))
    {
        AddBoundNodeForClass(base, fullSpan, context);
    }
}

void BoundCompileUnitNode::Sort()
{
    std::sort(boundNodes.begin(), boundNodes.end(), BoundNodeLess());
}

otava::intermediate::Value* BoundCompileUnitNode::CreateBoundGlobalVariable(VariableSymbol* globalVariableSymbol, Emitter& emitter, const soul::ast::FullSpan& fullSpan,
    Context* context, bool definition)
{
    otava::symbols::TypeSymbol* type = otava::symbols::ResolveFwdDeclaredType(globalVariableSymbol->GetType(context), fullSpan, context);
    globalVariableSymbol->SetDeclaredType(type, context);
    otava::intermediate::Value* initializer = nullptr;
    otava::intermediate::Type* irType = globalVariableSymbol->GetType(context)->IrType(emitter, fullSpan, context);
    otava::intermediate::Type* ptrIrType = emitter.MakePtrType(irType);
    if (definition)
    {
        if (globalVariableSymbol->GetValue(context))
        {
            initializer = globalVariableSymbol->GetValue(context)->IrValue(emitter, fullSpan, context);
        }
        else
        {
            initializer = irType->DefaultValue();
        }
    }
    otava::intermediate::Value* irVariable = emitter.EmitGlobalVariable(irType, globalVariableSymbol->IrName(context), initializer);
    emitter.SetIrObject(globalVariableSymbol, irVariable);
    return irVariable;
}

void BoundCompileUnitNode::AddDynamicInitialization(BoundExpressionNode* dynamicInitialization, BoundExpressionNode* atExitCall,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!compileUnitInitializationFunction)
    {
        FunctionDefinitionSymbol* compileUnitInitializationFunctionSymbol = new FunctionDefinitionSymbol(context->GetModule(), 
            context->GetNextSymbolId(SymbolKind::functionDefinitionSymbol), "__dynamic_init__" + id);
        compileUnitInitializationFunctionSymbol->SetGenerated();
        context->GetSymbolTable()->GlobalNs()->AddSymbol(compileUnitInitializationFunctionSymbol, fullSpan, context);
        compileUnitInitializationFunction = new BoundFunctionNode(compileUnitInitializationFunctionSymbol, fullSpan);
        compileUnitInitializationFunction->SetBody(new BoundCompoundStatementNode(fullSpan));
    }
    BoundCompoundStatementNode* body = compileUnitInitializationFunction->Body();
    BoundExpressionStatementNode* initExprStmt = new BoundExpressionStatementNode(fullSpan);
    initExprStmt->SetExpr(dynamicInitialization, fullSpan, context);
    body->AddStatement(initExprStmt);
    if (atExitCall)
    {
        BoundExpressionStatementNode* atExitStmt = new BoundExpressionStatementNode(fullSpan);
        atExitStmt->SetExpr(atExitCall, fullSpan, context);
        body->AddStatement(atExitStmt);
    }
}

void BoundCompileUnitNode::AddClassToGenerateDestructorList(ClassTypeSymbol* classType)
{
    if (std::find(generateDestructorList.begin(), generateDestructorList.end(), classType) == generateDestructorList.end())
    {
        generateDestructorList.push_back(classType);
    }
}

BoundCtorInitializerNode::BoundCtorInitializerNode(const soul::ast::FullSpan& fullSpan_) : BoundNode(BoundNodeKind::boundCtorInitializerNode, fullSpan_)
{
}

void BoundCtorInitializerNode::AddBaseInitializer(BoundExpressionNode* baseInitializer)
{
    baseInitializers.push_back(std::unique_ptr<BoundExpressionNode>(baseInitializer));
}

void BoundCtorInitializerNode::AddSetVPtrStatement(BoundStatementNode* setVPtrStatement)
{
    setVPtrStatements.push_back(std::unique_ptr<BoundStatementNode>(setVPtrStatement));
}

void BoundCtorInitializerNode::AddMemberInitializer(BoundExpressionNode* memberInitializer)
{
    memberInitializers.push_back(std::unique_ptr<BoundExpressionNode>(memberInitializer));
}

void BoundCtorInitializerNode::GenerateCode(BoundTreeVisitor& visitor, Emitter& emitter, Context* context)
{
    for (const auto& baseInitializer : baseInitializers)
    {
        baseInitializer->Load(emitter, OperationFlags::defaultInit, GetFullSpan(), context);
    }
    for (const auto& setVPtrStatement : setVPtrStatements)
    {
        setVPtrStatement->Accept(visitor);
    }
    for (const auto& memberInitializer : memberInitializers)
    {
        memberInitializer->Load(emitter, OperationFlags::defaultInit, GetFullSpan(), context);
    }
}

void BoundCtorInitializerNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundCtorInitializerNode::MayThrow() const noexcept
{
    for (const auto& baseInitializer : baseInitializers)
    {
        if (baseInitializer->MayThrow()) return true;
    }
    for (const auto& setVPtrStatement : setVPtrStatements)
    {
        if (setVPtrStatement->MayThrow()) return true;
    }
    for (const auto& memberInitializer : memberInitializers)
    {
        if (memberInitializer->MayThrow()) return true;
    }
    return false;
}

BoundDtorTerminatorNode::BoundDtorTerminatorNode(const soul::ast::FullSpan& fullSpan_) : BoundNode(BoundNodeKind::boundDtorTerminatorNode, fullSpan_)
{
}

void BoundDtorTerminatorNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundDtorTerminatorNode::AddSetVPtrStatement(BoundStatementNode* setVPtrStatement)
{
    setVPtrStatements.push_back(std::unique_ptr<BoundStatementNode>(setVPtrStatement));
}

void BoundDtorTerminatorNode::AddMemberTerminator(BoundExpressionNode* memberTerminator)
{
    memberTerminators.push_back(std::unique_ptr<BoundExpressionNode>(memberTerminator));
}

void BoundDtorTerminatorNode::GenerateCode(BoundTreeVisitor& visitor, Emitter& emitter, Context* context)
{
    for (const auto& memberTerminator : memberTerminators)
    {
        memberTerminator->Load(emitter, OperationFlags::none, GetFullSpan(), context);
    }
}

BoundClassNode::BoundClassNode(ClassTypeSymbol* cls_, const soul::ast::FullSpan& fullSpan_) : BoundNode(BoundNodeKind::boundClassNode, fullSpan_), cls(cls_)
{
}

void BoundClassNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundFunctionNode::BoundFunctionNode(FunctionDefinitionSymbol* functionDefinitionSymbol_, const soul::ast::FullSpan& fullSpan_) :
    BoundNode(BoundNodeKind::boundFunctionNode, fullSpan_),
    functionDefinitionSymbol(functionDefinitionSymbol_),
    body(),
    serial(-1)
{
}

BoundFunctionNode::~BoundFunctionNode()
{
    if (ExceptionThrown() || otava::ast::ExceptionThrown() || otava::intermediate::ExceptionThrown() ||
        otava::optimizer::ExceptionThrown() || otava::assembly::ExceptionThrown())
    {
        body.release();
    }
    else
    {
        body.reset();
    }
}

void BoundFunctionNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundFunctionNode::SetBody(BoundCompoundStatementNode* body_)
{
    body.reset(body_);
}

void BoundFunctionNode::SetCtorInitializer(BoundCtorInitializerNode* ctorInitializer_)
{
    ctorInitializer.reset(ctorInitializer_);
}

void BoundFunctionNode::SetDtorTerminator(BoundDtorTerminatorNode* dtorTerminator_)
{
    dtorTerminator.reset(dtorTerminator_);
}

void BoundFunctionNode::AddDefaultFunctionSymbol(FunctionSymbol* defaultFunctionSymbol)
{
    defaultFunctionSymbols.push_back(std::unique_ptr<FunctionSymbol>(defaultFunctionSymbol));
}

void BoundFunctionNode::SetSetLineStatementNode(otava::ast::Node* setLineStatementNode_)
{
    setLineStatementNode.reset(setLineStatementNode_);
}

void BoundFunctionNode::SetBoundSetLineStatement(BoundStatementNode* boundSetLineStatement_)
{
    boundSetLineStatement.reset(boundSetLineStatement_);
}

void BoundFunctionNode::AddTemporaryDestructorCall(BoundFunctionCallNode* temporaryDestructorCall)
{
    temporaryDestructorCalls.push_back(std::unique_ptr<BoundFunctionCallNode>(temporaryDestructorCall));
}

std::vector<std::unique_ptr<BoundFunctionCallNode>> BoundFunctionNode::GetTemporaryDestructorCalls()
{
    return std::move(temporaryDestructorCalls);
}

BoundStatementNode::BoundStatementNode(BoundNodeKind kind_, const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundNode(kind_, fullSpan_), parent(nullptr), generated(false), postfix(false), statementIndex(-1), destructTemporariesNode(nullptr)
{
}

BoundStatementNode::~BoundStatementNode()
{
    if (destructTemporariesNode)
    {
        delete destructTemporariesNode;
    }
}

bool BoundStatementNode::IsConditionalStatementInBlock(BoundCompoundStatementNode* block) const noexcept
{
    if (this == block)
    {
        return false;
    }
    if (IsIfStatementNode() || IsSwitchStatementNode()) return true;
    BoundStatementNode* parent = Parent();
    if (parent)
    {
        return parent->IsConditionalStatementInBlock(block);
    }
    return false;
}

BoundCompoundStatementNode* BoundStatementNode::Block() noexcept
{
    if (IsBoundCompoundStatementNode()) return static_cast<BoundCompoundStatementNode*>(this);
    BoundStatementNode* parentStatement = Parent();
    if (parentStatement != nullptr)
    {
        return parentStatement->Block();
    }
    else
    {
        return nullptr;
    }
}

void BoundStatementNode::SetDestructTemporariesNode(BoundDestructTemporariesNode* destructTemporariesNode_) noexcept
{
    destructTemporariesNode = destructTemporariesNode_;
}

void BoundStatementNode::DestructTemporaries(Emitter& emitter, Context* context)
{
    if (!destructTemporariesNode) return;
    destructTemporariesNode->Load(emitter, OperationFlags::none, GetFullSpan(), context);
}

BoundEmptyStatementNode::BoundEmptyStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundEmptyStatementNode, fullSpan_)
{
}

void BoundEmptyStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundEmptyStatementNode::Clone() const
{
    BoundEmptyStatementNode* clone = new BoundEmptyStatementNode(GetFullSpan());
    return clone;
}

BoundCompoundStatementNode::BoundCompoundStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundCompoundStatementNode, fullSpan_), blockId(-1)
{
}

void BoundCompoundStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundCompoundStatementNode::Clone() const
{
    BoundCompoundStatementNode* clone = new BoundCompoundStatementNode(GetFullSpan());
    int n = statements.size();
    for (int i = 0; i < n; ++i)
    {
        clone->AddStatement(statements[i]->Clone());
    }
    return clone;
}

int BoundCompoundStatementNode::IndexOf(BoundStatementNode* stmt) noexcept
{
    int n = statements.size();
    for (int i = 0; i < n; ++i)
    {
        BoundStatementNode* statement = statements[i].get();
        if (statement == stmt) return i;
    }
    return -1;
}

void BoundCompoundStatementNode::AddStatement(BoundStatementNode* statement)
{
    statement->SetParent(this);
    statement->SetStatementIndex(NextStatementIndex());
    if (statement->IsBoundSequenceStatementNode())
    {
        BoundSequenceStatementNode* s = static_cast<BoundSequenceStatementNode*>(statement);
        if (s->First()->StatementIndex() == -1)
        {
            s->First()->SetStatementIndex(s->StatementIndex());
        }
        if (s->Second()->StatementIndex() == -1)
        {
            s->Second()->SetStatementIndex(s->StatementIndex());
        }
    }
    statements.push_back(std::unique_ptr<BoundStatementNode>(statement));
}

bool BoundCompoundStatementNode::EndsWithTerminator() const noexcept
{
    if (!statements.empty())
    {
        if (statements.back()->IsTerminator()) return true;
        if (statements.back()->IsBoundCompoundStatementNode() && statements.back()->EndsWithTerminator()) return true;
    }
    return false;
}

bool BoundCompoundStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    for (const auto& stmt : statements)
    {
        if (stmt->ContainsLocalVariableWithDestructor()) return true;
    }
    return false;
}

BoundIfStatementNode::BoundIfStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundIfStatementNode, fullSpan_), blockId(-1)
{
}

void BoundIfStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundIfStatementNode::Clone() const
{
    BoundIfStatementNode* clone = new BoundIfStatementNode(GetFullSpan());
    if (condition)
    {
        clone->SetCondition(condition->Clone());
    }
    clone->SetThenStatement(thenStatement->Clone());
    if (elseStatement)
    {
        clone->SetElseStatement(elseStatement->Clone());
    }
    return clone;
}

bool BoundIfStatementNode::MayThrow() const noexcept
{
    if (condition->MayThrow()) return true;
    if (thenStatement->MayThrow()) return true;
    if (elseStatement && elseStatement->MayThrow()) return true;
    return false;
}

void BoundIfStatementNode::SetCondition(BoundExpressionNode* condition_) noexcept
{
    condition.reset(condition_);
}

void BoundIfStatementNode::SetThenStatement(BoundStatementNode* thenStatement_) noexcept
{
    thenStatement.reset(thenStatement_);
}

void BoundIfStatementNode::SetElseStatement(BoundStatementNode* elseStatement_) noexcept
{
    elseStatement.reset(elseStatement_);
}

bool BoundIfStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    if (thenStatement->ContainsLocalVariableWithDestructor()) return true;
    if (elseStatement && elseStatement->ContainsLocalVariableWithDestructor()) return true;
    return false;
}

BoundSwitchStatementNode::BoundSwitchStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundSwitchStatementNode, fullSpan_), blockId(-1)
{
}

void BoundSwitchStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundSwitchStatementNode::Clone() const
{
    BoundSwitchStatementNode* clone = new BoundSwitchStatementNode(GetFullSpan());
    clone->SetCondition(condition->Clone());
    clone->SetStatement(statement->Clone());
    return clone;
}

bool BoundSwitchStatementNode::MayThrow() const noexcept
{
    if (condition->MayThrow()) return true;
    if (statement->MayThrow()) return true;
    return false;
}

void BoundSwitchStatementNode::SetCondition(BoundExpressionNode* condition_) noexcept
{
    condition.reset(condition_);
}

void BoundSwitchStatementNode::SetStatement(BoundStatementNode* statement_) noexcept
{
    statement.reset(statement_);
}

bool BoundSwitchStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    if (statement->ContainsLocalVariableWithDestructor()) return true;
    return false;
}

BoundCaseStatementNode::BoundCaseStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundCaseStatementNode, fullSpan_)
{
}

void BoundCaseStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundCaseStatementNode::Clone() const
{
    BoundCaseStatementNode* clone = new BoundCaseStatementNode(GetFullSpan());
    for (const auto& caseExpr : caseExprs)
    {
        clone->AddCaseExpr(caseExpr->Clone());
    }
    clone->SetStatement(stmt->Clone());
    return clone;
}

bool BoundCaseStatementNode::MayThrow() const noexcept
{
    return stmt->MayThrow();
}

void BoundCaseStatementNode::AddCaseExpr(BoundExpressionNode* caseExpr)
{
    caseExprs.push_back(std::unique_ptr<BoundExpressionNode>(caseExpr));
}

void BoundCaseStatementNode::InsertCaseExprToFront(BoundExpressionNode* caseExpr)
{
    caseExprs.insert(caseExprs.begin(), std::unique_ptr<BoundExpressionNode>(caseExpr));
}

void BoundCaseStatementNode::SetStatement(BoundStatementNode* stmt_) noexcept
{
    stmt.reset(stmt_);
}

bool BoundCaseStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    return stmt->ContainsLocalVariableWithDestructor();
}

BoundDefaultStatementNode::BoundDefaultStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundDefaultStatementNode, fullSpan_)
{
}

void BoundDefaultStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundDefaultStatementNode::Clone() const
{
    BoundDefaultStatementNode* clone = new BoundDefaultStatementNode(GetFullSpan());
    clone->SetStatement(stmt->Clone());
    return clone;
}

bool BoundDefaultStatementNode::MayThrow() const noexcept
{
    return stmt->MayThrow();
}

void BoundDefaultStatementNode::SetStatement(BoundStatementNode* stmt_) noexcept
{
    stmt.reset(stmt_);
}

bool BoundDefaultStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    return stmt->ContainsLocalVariableWithDestructor();
}

BoundWhileStatementNode::BoundWhileStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundWhileStatementNode, fullSpan_), blockId(-1)
{
}

void BoundWhileStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundWhileStatementNode::Clone() const
{
    BoundWhileStatementNode* clone = new BoundWhileStatementNode(GetFullSpan());
    clone->SetCondition(condition->Clone());
    clone->SetStatement(statement->Clone());
    return clone;
}

bool BoundWhileStatementNode::MayThrow() const noexcept
{
    if (condition->MayThrow()) return true;
    if (statement->MayThrow()) return true;
    return false;
}

void BoundWhileStatementNode::SetCondition(BoundExpressionNode* condition_) noexcept
{
    condition.reset(condition_);
}

void BoundWhileStatementNode::SetStatement(BoundStatementNode* statement_) noexcept
{
    statement.reset(statement_);
}

bool BoundWhileStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    return statement->ContainsLocalVariableWithDestructor();
}

BoundDoStatementNode::BoundDoStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundDoStatementNode, fullSpan_)
{
}

void BoundDoStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundDoStatementNode::Clone() const
{
    BoundDoStatementNode* clone = new BoundDoStatementNode(GetFullSpan());
    clone->SetExpr(expr->Clone());
    clone->SetStatement(statement->Clone());
    return clone;
}

bool BoundDoStatementNode::MayThrow() const noexcept
{
    if (expr->MayThrow()) return true;
    if (statement->MayThrow()) return true;
    return false;
}

void BoundDoStatementNode::SetExpr(BoundExpressionNode* expr_) noexcept
{
    expr.reset(expr_);
}

void BoundDoStatementNode::SetStatement(BoundStatementNode* statement_) noexcept
{
    statement.reset(statement_);
}

bool BoundDoStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    return statement->ContainsLocalVariableWithDestructor();
}

BoundForStatementNode::BoundForStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundForStatementNode, fullSpan_), blockId(-1)
{
}

void BoundForStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundForStatementNode::Clone() const
{
    BoundForStatementNode* clone = new BoundForStatementNode(GetFullSpan());
    if (initStatement)
    {
        clone->SetInitStatement(initStatement->Clone());
    }
    if (condition)
    {
        clone->SetCondition(condition->Clone());
    }
    if (loopExpr)
    {
        clone->SetLoopExpr(loopExpr->Clone());
    }
    clone->SetStatement(statement->Clone());
    return clone;
}

bool BoundForStatementNode::MayThrow() const noexcept
{
    if (initStatement && initStatement->MayThrow()) return true;
    if (condition && condition->MayThrow()) return true;
    if (loopExpr && loopExpr->MayThrow()) return true;
    if (statement->MayThrow()) return true;
    return false;
}

void BoundForStatementNode::SetInitStatement(BoundStatementNode* initStatement_) noexcept
{
    initStatement.reset(initStatement_);
}

void BoundForStatementNode::SetCondition(BoundExpressionNode* condition_) noexcept
{
    condition.reset(condition_);
}

void BoundForStatementNode::SetLoopExpr(BoundExpressionNode* loopExpr_) noexcept
{
    loopExpr.reset(loopExpr_);
}

void BoundForStatementNode::SetStatement(BoundStatementNode* statement_) noexcept
{
    statement.reset(statement_);
}

bool BoundForStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    if (initStatement && initStatement->ContainsLocalVariableWithDestructor()) return true;
    if (statement->ContainsLocalVariableWithDestructor()) return true;
    return false;
}

BoundSequenceStatementNode::BoundSequenceStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundSequenceStatementNode, fullSpan_), first(), second()
{
}

BoundSequenceStatementNode::BoundSequenceStatementNode(const soul::ast::FullSpan& fullSpan_, BoundStatementNode* first_, BoundStatementNode* second_) noexcept :
    BoundStatementNode(BoundNodeKind::boundSequenceStatementNode, fullSpan_), first(first_), second(second_)
{
}

void BoundSequenceStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundSequenceStatementNode::Clone() const
{
    BoundSequenceStatementNode* clone = new BoundSequenceStatementNode(GetFullSpan(), first->Clone(), second->Clone());
    return clone;
}

bool BoundSequenceStatementNode::MayThrow() const noexcept
{
    if (first && first->MayThrow()) return true;
    if (second && second->MayThrow()) return true;
    return false;
}

bool BoundSequenceStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    if (first->ContainsLocalVariableWithDestructor()) return true;
    if (second->ContainsLocalVariableWithDestructor()) return true;
    return false;
}

BoundBreakStatementNode::BoundBreakStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundBreakStatementNode, fullSpan_)
{
}

void BoundBreakStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundBreakStatementNode::Clone() const
{
    BoundBreakStatementNode* clone = new BoundBreakStatementNode(GetFullSpan());
    return clone;
}

BoundContinueStatementNode::BoundContinueStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundContinueStatementNode, fullSpan_)
{
}

void BoundContinueStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundContinueStatementNode::Clone() const
{
    BoundContinueStatementNode* clone = new BoundContinueStatementNode(GetFullSpan());
    return clone;
}

BoundReturnStatementNode::BoundReturnStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundReturnStatementNode, fullSpan_)
{
}

void BoundReturnStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundReturnStatementNode::Clone() const
{
    BoundReturnStatementNode* clone = new BoundReturnStatementNode(GetFullSpan());
    if (expr)
    {
        clone->SetExpr(expr->Clone());
    }
    return clone;
}

bool BoundReturnStatementNode::MayThrow() const noexcept
{
    if (expr && expr->MayThrow()) return true;
    return false;
}

void BoundReturnStatementNode::SetExpr(BoundExpressionNode* expr_) noexcept
{
    expr.reset(expr_);
}

void BoundReturnStatementNode::SetExpr(BoundExpressionNode* expr_, const soul::ast::FullSpan& fullSpan, Context* context)
{
    expr.reset(expr_);
    expr->ModifyTypes(fullSpan, context);
}

BoundLabeledStatementNode::BoundLabeledStatementNode(const soul::ast::FullSpan& fullSpan_) :
    BoundStatementNode(BoundNodeKind::boundLabeledStatementNode, fullSpan_), label(), stmt(nullptr), bb(nullptr)
{
}

BoundLabeledStatementNode::BoundLabeledStatementNode(const soul::ast::FullSpan& fullSpan_, const std::string& label_, BoundStatementNode* stmt_) :
    BoundStatementNode(BoundNodeKind::boundLabeledStatementNode, fullSpan_), label(label_), stmt(stmt_), bb(nullptr)
{
}

void BoundLabeledStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundLabeledStatementNode::Clone() const
{
    BoundLabeledStatementNode* clone = new BoundLabeledStatementNode(GetFullSpan(), label, stmt->Clone());
    return clone;
}

otava::intermediate::BasicBlock* BoundLabeledStatementNode::GetBB(Emitter& emitter)
{
    if (!bb)
    {
        bb = emitter.CreateBasicBlock();
    }
    return bb;
}

bool BoundLabeledStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    return stmt->ContainsLocalVariableWithDestructor();
}

BoundGotoStatementNode::BoundGotoStatementNode(const soul::ast::FullSpan& fullSpan_) :
    BoundStatementNode(BoundNodeKind::boundGotoStatementNode, fullSpan_), target(), labeledStatement(nullptr)
{
}

BoundGotoStatementNode::BoundGotoStatementNode(const soul::ast::FullSpan& fullSpan_, const std::string& target_) :
    BoundStatementNode(BoundNodeKind::boundGotoStatementNode, fullSpan_), target(target_), labeledStatement(nullptr)
{
}

void BoundGotoStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundGotoStatementNode::Clone() const
{
    BoundGotoStatementNode* clone = new BoundGotoStatementNode(GetFullSpan(), target);
    return clone;
}

otava::intermediate::BasicBlock* BoundGotoStatementNode::GetBB(Emitter& emitter)
{
    return labeledStatement->GetBB(emitter);
}

BoundConstructionStatementNode::BoundConstructionStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundConstructionStatementNode, fullSpan_), variable(nullptr)
{
}

BoundConstructionStatementNode::BoundConstructionStatementNode(const soul::ast::FullSpan& fullSpan_, BoundExpressionNode* constructorCall_) noexcept :
    BoundStatementNode(BoundNodeKind::boundConstructionStatementNode, fullSpan_), constructorCall(constructorCall_), variable(nullptr)
{
}

void BoundConstructionStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundConstructionStatementNode::Clone() const
{
    BoundConstructionStatementNode* clone = new BoundConstructionStatementNode(GetFullSpan(), static_cast<BoundExpressionNode*>(constructorCall->Clone()));
    if (destructorCall)
    {
        clone->SetDestructorCall(static_cast<BoundExpressionNode*>(destructorCall->Clone()));
    }
    clone->SetVariable(variable);
    return clone;
}

bool BoundConstructionStatementNode::MayThrow() const noexcept
{
    if (constructorCall->MayThrow()) return true;
    return false;
}

void BoundConstructionStatementNode::SetDestructorCall(BoundExpressionNode* destructorCall_) noexcept
{
    destructorCall.reset(destructorCall_);
}

bool BoundConstructionStatementNode::ContainsLocalVariableWithDestructor() const noexcept
{
    if (destructorCall) return true;
    return false;
}

BoundExpressionStatementNode::BoundExpressionStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundExpressionStatementNode, fullSpan_)
{
}

void BoundExpressionStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundExpressionStatementNode::Clone() const
{
    BoundExpressionStatementNode* clone = new BoundExpressionStatementNode(GetFullSpan());
    if (expr)
    {
        clone->SetExpr(expr->Clone());
    }
    return clone;
}

bool BoundExpressionStatementNode::MayThrow() const noexcept
{
    if (expr && expr->MayThrow()) return true;
    return false;
}

void BoundExpressionStatementNode::SetExpr(BoundExpressionNode* expr_) noexcept
{
    expr.reset(expr_);
}

void BoundExpressionStatementNode::SetExpr(BoundExpressionNode* expr_, const soul::ast::FullSpan& fullSpan, Context* context)
{
    expr.reset(expr_);
    expr->ModifyTypes(fullSpan, context);
}

bool BoundExpressionStatementNode::IsTerminator() const noexcept
{
    return expr && (expr->IsNoReturnFunctionCall());
}

BoundSetVPtrStatementNode::BoundSetVPtrStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundSetVPtrStatementNode, fullSpan_), thisPtr(), forClass(nullptr), vptrHolderClass(nullptr)
{
}

BoundSetVPtrStatementNode::BoundSetVPtrStatementNode(BoundExpressionNode* thisPtr_, ClassTypeSymbol* forClass_, ClassTypeSymbol* vptrHolderClass_,
    const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundSetVPtrStatementNode, fullSpan_), thisPtr(thisPtr_), forClass(forClass_), vptrHolderClass(vptrHolderClass_)
{
}

void BoundSetVPtrStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundSetVPtrStatementNode::Clone() const
{
    BoundSetVPtrStatementNode* clone = new BoundSetVPtrStatementNode(thisPtr->Clone(), forClass, vptrHolderClass, GetFullSpan());
    return clone;
}

BoundAliasDeclarationStatementNode::BoundAliasDeclarationStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundStatementNode(BoundNodeKind::boundAliasDeclarationStatementNode, fullSpan_)
{
}

void BoundAliasDeclarationStatementNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStatementNode* BoundAliasDeclarationStatementNode::Clone() const
{
    BoundAliasDeclarationStatementNode* clone = new BoundAliasDeclarationStatementNode(GetFullSpan());
    return clone;
}

BoundLiteralNode::BoundLiteralNode(Value* value_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundLiteralNode, fullSpan_, type_), value(value_)
{
}

void BoundLiteralNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundLiteralNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (emitter.Line())
    {
        Value* lineValue = context->GetEvaluationContext()->GetIntegerValue(emitter.Line(), std::to_string(emitter.Line()),
            context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context), context);
        emitter.Stack().Push(lineValue->IrValue(emitter, fullSpan, context));
    }
    else
    {
        emitter.Stack().Push(value->IrValue(emitter, fullSpan, context));
    }
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundLiteralNode::Clone() const
{
    BoundExpressionNode* clone = new BoundLiteralNode(value, GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

BoundStringLiteralNode::BoundStringLiteralNode(Value* value_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundStringLiteralNode, fullSpan_, type_), value(value_), irValue(nullptr)
{
}

void BoundStringLiteralNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundStringLiteralNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!irValue)
    {
        irValue = value->IrValue(emitter, fullSpan, context);
    }
    emitter.Stack().Push(irValue);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundStringLiteralNode::Clone() const
{
    BoundExpressionNode* clone = new BoundStringLiteralNode(value, GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

BoundVariableNode::BoundVariableNode(VariableSymbol* variable_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* referredType_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundVariableNode, fullSpan_, referredType_), variable(variable_)
{
}

void BoundVariableNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundVariableNode::SetThisPtr(BoundExpressionNode* thisPtr_) noexcept
{
    thisPtr.reset(thisPtr_);
}

void BoundVariableNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (variable->IsLocalVariable(context))
    {
        if ((flags & OperationFlags::addr) != OperationFlags::none || variable->GetType(context)->IsClassTypeSymbol())
        {
            emitter.Stack().Push(static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context)));
        }
        else if ((flags & OperationFlags::deref) != OperationFlags::none)
        {
            otava::intermediate::Value* value = emitter.EmitLoad(static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context)));
            std::uint8_t derefCount = GetDerefCount(flags);
            for (std::uint8_t i = 0; i < derefCount; ++i)
            {
                value = emitter.EmitLoad(value);
            }
            emitter.Stack().Push(value);
        }
        else
        {
            emitter.Stack().Push(emitter.EmitLoad(static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context))));
        }
    }
    else if (variable->IsMemberVariable(context))
    {
        if (variable->IsStatic())
        {
            otava::intermediate::Value* ptr = static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context));
            if (!ptr)
            {
                ptr = context->GetBoundCompileUnit()->CreateBoundGlobalVariable(variable, emitter, fullSpan, context, true);
            }
            if ((flags & OperationFlags::addr) != OperationFlags::none)
            {
                emitter.Stack().Push(ptr);
            }
            else if ((flags & OperationFlags::deref) != OperationFlags::none)
            {
                otava::intermediate::Value* value = emitter.EmitLoad(ptr);
                std::uint8_t n = GetDerefCount(flags);
                for (std::uint8_t i = 0; i < n; ++i)
                {
                    value = emitter.EmitLoad(value);
                }
                emitter.Stack().Push(value);
            }
            else
            {
                emitter.Stack().Push(emitter.EmitLoad(ptr));
            }
        }
        else
        {
            if (!thisPtr)
            {
                ThrowException("'this ptr' of bound member variable not set", fullSpan, context);
            }
            thisPtr->Load(emitter, OperationFlags::none, fullSpan, context);
            otava::intermediate::Value* ptr = emitter.Stack().Pop();
            std::int32_t layoutIndex = variable->LayoutIndex();
            otava::intermediate::Value* elementPtr = emitter.EmitElemAddr(ptr, emitter.EmitLong(layoutIndex));
            if ((flags & OperationFlags::addr) != OperationFlags::none)
            {
                emitter.Stack().Push(elementPtr);
            }
            else if ((flags & OperationFlags::deref) != OperationFlags::none)
            {
                otava::intermediate::Value* value = emitter.EmitLoad(elementPtr);
                std::uint8_t n = GetDerefCount(flags);
                for (std::uint8_t i = 0; i < n; ++i)
                {
                    value = emitter.EmitLoad(value);
                }
                emitter.Stack().Push(value);
            }
            else
            {
                emitter.Stack().Push(emitter.EmitLoad(elementPtr));
            }
        }
    }
    else if (variable->IsGlobalVariable(context))
    {
        otava::intermediate::Value* ptr = static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context));
        if (!ptr)
        {
            ptr = context->GetBoundCompileUnit()->CreateBoundGlobalVariable(variable, emitter, fullSpan, context, false);
        }
        if ((flags & OperationFlags::addr) != OperationFlags::none)
        {
            emitter.Stack().Push(ptr);
        }
        else if ((flags & OperationFlags::deref) != OperationFlags::none)
        {
            otava::intermediate::Value* value = emitter.EmitLoad(ptr);
            std::uint8_t n = GetDerefCount(flags);
            for (std::uint8_t i = 0; i < n; ++i)
            {
                value = emitter.EmitLoad(value);
            }
            emitter.Stack().Push(value);
        }
        else
        {
            emitter.Stack().Push(emitter.EmitLoad(ptr));
        }
    }
    DestructTemporaries(emitter, context);
}

void BoundVariableNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if ((flags & OperationFlags::addr) != OperationFlags::none)
    {
        ThrowException("invalid operation flag 'addr' for variable store operation", fullSpan, context);
    }
    if (variable->IsLocalVariable(context))
    {
        otava::intermediate::Value* ptr = static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context));
        if (!ptr)
        {
            ThrowException("variable '" + variable->Name() + "': IR object not set", fullSpan, context);
        }
        otava::intermediate::Value* value = emitter.Stack().Pop();
        if ((flags & OperationFlags::deref) != OperationFlags::none)
        {
            ptr = emitter.EmitLoad(ptr);
            std::uint8_t n = GetDerefCount(flags);
            for (std::uint8_t i = 1; i < n; ++i)
            {
                ptr = emitter.EmitLoad(ptr);
            }
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(value, ptr);
        }
        else
        {
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(value, ptr);
        }
    }
    else if (variable->IsMemberVariable(context))
    {
        otava::intermediate::Value* value = emitter.Stack().Pop();
        if (!thisPtr)
        {
            ThrowException("'this ptr' of bound member variable not set", fullSpan, context);
        }
        thisPtr->Load(emitter, OperationFlags::none, fullSpan, context);
        otava::intermediate::Value* ptr = emitter.Stack().Pop();
        std::int32_t layoutIndex = variable->LayoutIndex();
        otava::intermediate::Value* elementPtr = emitter.EmitElemAddr(ptr, emitter.EmitLong(layoutIndex));
        if ((flags & OperationFlags::deref) != OperationFlags::none)
        {
            elementPtr = emitter.EmitLoad(elementPtr);
            std::uint8_t n = GetDerefCount(flags);
            for (std::uint8_t i = 1; i < n; ++i)
            {
                elementPtr = emitter.EmitLoad(elementPtr);
            }
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(elementPtr);
            }
            emitter.EmitStore(value, elementPtr);
        }
        else
        {
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(elementPtr);
            }
            emitter.EmitStore(value, elementPtr);
        }
    }
    else if (variable->IsGlobalVariable(context))
    {
        otava::intermediate::Value* value = emitter.Stack().Pop();
        otava::intermediate::Value* ptr = static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context));
        if (!ptr)
        {
            ptr = context->GetBoundCompileUnit()->CreateBoundGlobalVariable(variable, emitter, fullSpan, context, false);
        }
        if ((flags & OperationFlags::deref) != OperationFlags::none)
        {
            ptr = emitter.EmitLoad(ptr);
            std::uint8_t n = GetDerefCount(flags);
            for (std::uint8_t i = 1; i < n; ++i)
            {
                ptr = emitter.EmitLoad(ptr);
            }
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(value, ptr);
        }
        else
        {
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(value, ptr);
        }
    }
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundVariableNode::Clone() const
{
    BoundVariableNode* clone = new BoundVariableNode(variable, GetFullSpan(), GetType());
    if (thisPtr)
    {
        clone->SetThisPtr(thisPtr->Clone());
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

bool BoundVariableNode::IsBoundLocalVariable(Context* context) const  noexcept
{
    return variable->IsLocalVariable(context);
}

bool BoundVariableNode::IsBoundMemberVariable(Context* context) const noexcept
{
    return variable->IsMemberVariable(context);
}

void BoundVariableNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    if (thisPtr)
    {
        thisPtr->ModifyTypes(fullSpan, context);
    }
}

BoundParentVariableNode::BoundParentVariableNode(VariableSymbol* variable_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* referredType_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundParentVariableNode, fullSpan_, referredType_), variable(variable_), level(0)
{
}

void BoundParentVariableNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundParentVariableNode::SetThisPtr(BoundExpressionNode* thisPtr_) noexcept
{
    thisPtr.reset(thisPtr_);
}

void BoundParentVariableNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (variable->IsLocalVariable(context))
    {
        if ((flags & OperationFlags::addr) != OperationFlags::none || variable->GetType(context)->IsClassTypeSymbol())
        {
            emitter.Stack().Push(static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context)));
        }
        else if ((flags & OperationFlags::deref) != OperationFlags::none)
        {
            otava::intermediate::Value* value = emitter.EmitLoad(static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context)));
            std::uint8_t derefCount = GetDerefCount(flags);
            for (std::uint8_t i = 0; i < derefCount; ++i)
            {
                value = emitter.EmitLoad(value);
            }
            emitter.Stack().Push(value);
        }
        else
        {
            emitter.Stack().Push(emitter.EmitLoad(static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context))));
        }
    }
    else if (variable->IsMemberVariable(context))
    {
        if (!thisPtr)
        {
            ThrowException("'this ptr' of bound member variable not set", fullSpan, context);
        }
        thisPtr->Load(emitter, OperationFlags::none, fullSpan, context);
        otava::intermediate::Value* ptr = emitter.Stack().Pop();
        std::int32_t layoutIndex = variable->LayoutIndex();
        otava::intermediate::Value* elementPtr = emitter.EmitElemAddr(ptr, emitter.EmitLong(layoutIndex));
        if ((flags & OperationFlags::addr) != OperationFlags::none)
        {
            emitter.Stack().Push(elementPtr);
        }
        else if ((flags & OperationFlags::deref) != OperationFlags::none)
        {
            otava::intermediate::Value* value = emitter.EmitLoad(elementPtr);
            std::uint8_t n = GetDerefCount(flags);
            for (std::uint8_t i = 0; i < n; ++i)
            {
                value = emitter.EmitLoad(value);
            }
            emitter.Stack().Push(value);
        }
        else
        {
            emitter.Stack().Push(emitter.EmitLoad(elementPtr));
        }
    }
    else if (variable->IsGlobalVariable(context))
    {
        ThrowException("bound parent variable cannot be global", fullSpan, context);
    }
    DestructTemporaries(emitter, context);
}

void BoundParentVariableNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (variable->IsLocalVariable(context))
    {
        otava::intermediate::Value* ptr = static_cast<otava::intermediate::Value*>(variable->IrObject(emitter, fullSpan, context));
        otava::intermediate::Value* value = emitter.Stack().Pop();
        if ((flags & OperationFlags::deref) != OperationFlags::none)
        {
            ptr = emitter.EmitLoad(ptr);
            std::uint8_t n = GetDerefCount(flags);
            for (std::uint8_t i = 1; i < n; ++i)
            {
                ptr = emitter.EmitLoad(ptr);
            }
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(value, ptr);
        }
        else
        {
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(value, ptr);
        }
    }
    else if (variable->IsMemberVariable(context))
    {
        otava::intermediate::Value* value = emitter.Stack().Pop();
        if (!thisPtr)
        {
            ThrowException("'this ptr' of bound member variable not set", fullSpan, context);
        }
        thisPtr->Load(emitter, OperationFlags::none, fullSpan, context);
        otava::intermediate::Value* ptr = emitter.Stack().Pop();
        std::int32_t layoutIndex = variable->LayoutIndex();
        otava::intermediate::Value* elementPtr = emitter.EmitElemAddr(ptr, emitter.EmitLong(layoutIndex));
        if ((flags & OperationFlags::deref) != OperationFlags::none)
        {
            elementPtr = emitter.EmitLoad(elementPtr);
            std::uint8_t n = GetDerefCount(flags);
            for (std::uint8_t i = 1; i < n; ++i)
            {
                elementPtr = emitter.EmitLoad(elementPtr);
            }
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(elementPtr);
            }
            emitter.EmitStore(value, elementPtr);
        }
        else
        {
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(elementPtr);
            }
            emitter.EmitStore(value, elementPtr);
        }
    }
    else if (variable->IsGlobalVariable(context))
    {
        ThrowException("bound parent variable cannot be global", fullSpan, context);
    }
    DestructTemporaries(emitter, context);
}

bool BoundParentVariableNode::IsBoundParentLocalVariable(Context* context) const noexcept
{
    return variable->IsLocalVariable(context);
}

bool BoundParentVariableNode::IsBoundParentMemberVariable(Context* context) const noexcept
{
    return variable->IsMemberVariable(context);
}

BoundExpressionNode* BoundParentVariableNode::Clone() const
{
    BoundParentVariableNode* clone = new BoundParentVariableNode(variable, GetFullSpan(), GetType());
    if (thisPtr)
    {
        clone->SetThisPtr(thisPtr->Clone());
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundParentVariableNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    if (thisPtr)
    {
        thisPtr->ModifyTypes(fullSpan, context);
    }
}

BoundParameterNode::BoundParameterNode(ParameterSymbol* parameter_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type) noexcept :
    BoundExpressionNode(BoundNodeKind::boundParameterNode, fullSpan_, type), parameter(parameter_)
{
}

void BoundParameterNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundParameterNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if ((flags & OperationFlags::addr) != OperationFlags::none)
    {
        emitter.Stack().Push(static_cast<otava::intermediate::Value*>(parameter->IrObject(emitter, fullSpan, context)));
    }
    else if ((flags & OperationFlags::deref) != OperationFlags::none)
    {
        otava::intermediate::Value* ptr = static_cast<otava::intermediate::Value*>(parameter->IrObject(emitter, fullSpan, context));
        otava::intermediate::Value* value = emitter.EmitLoad(ptr);
        std::uint8_t n = GetDerefCount(flags);
        for (std::uint8_t i = 0; i < n; ++i)
        {
            value = emitter.EmitLoad(value);
        }
        emitter.Stack().Push(value);
    }
    else
    {
        emitter.Stack().Push(emitter.EmitLoad(static_cast<otava::intermediate::Value*>(parameter->IrObject(emitter, fullSpan, context))));
    }
    DestructTemporaries(emitter, context);
}

void BoundParameterNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    otava::intermediate::Value* ptr = static_cast<otava::intermediate::Value*>(parameter->IrObject(emitter, fullSpan, context));
    otava::intermediate::Value* value = emitter.Stack().Pop();
    if ((flags & OperationFlags::addr) != OperationFlags::none)
    {
        ThrowException("invalid operation flag 'addr' for parameter store operation", fullSpan, context);
    }
    else if ((flags & OperationFlags::deref) != OperationFlags::none)
    {
        ptr = emitter.EmitLoad(ptr);
        std::uint8_t n = GetDerefCount(flags);
        for (std::uint8_t i = 1; i < n; ++i)
        {
            ptr = emitter.EmitLoad(ptr);
        }
        if ((flags & OperationFlags::setPtr) != OperationFlags::none)
        {
            context->SetPtr(ptr);
        }
        emitter.EmitStore(value, ptr);
    }
    else
    {
        if ((flags & OperationFlags::setPtr) != OperationFlags::none)
        {
            context->SetPtr(ptr);
        }
        emitter.EmitStore(value, ptr);
    }
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundParameterNode::Clone() const
{
    BoundExpressionNode* clone = new BoundParameterNode(parameter, GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

BoundParentParameterNode::BoundParentParameterNode(ParameterSymbol* parameter_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type) noexcept :
    BoundExpressionNode(BoundNodeKind::boundParentParameterNode, fullSpan_, type), parameter(parameter_), level(0)
{
}

void BoundParentParameterNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundParentParameterNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if ((flags & OperationFlags::addr) != OperationFlags::none)
    {
        emitter.Stack().Push(static_cast<otava::intermediate::Value*>(parameter->IrObject(emitter, fullSpan, context)));
    }
    else if ((flags & OperationFlags::deref) != OperationFlags::none)
    {
        otava::intermediate::Value* ptr = static_cast<otava::intermediate::Value*>(parameter->IrObject(emitter, fullSpan, context));
        otava::intermediate::Value* value = emitter.EmitLoad(ptr);
        std::uint8_t n = GetDerefCount(flags);
        for (std::uint8_t i = 0; i < n; ++i)
        {
            value = emitter.EmitLoad(value);
        }
        emitter.Stack().Push(value);
    }
    else
    {
        emitter.Stack().Push(emitter.EmitLoad(static_cast<otava::intermediate::Value*>(parameter->IrObject(emitter, fullSpan, context))));
    }
    DestructTemporaries(emitter, context);
}

void BoundParentParameterNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    otava::intermediate::Value* ptr = static_cast<otava::intermediate::Value*>(parameter->IrObject(emitter, fullSpan, context));
    otava::intermediate::Value* value = emitter.Stack().Pop();
    if ((flags & OperationFlags::addr) != OperationFlags::none)
    {
        ThrowException("invalid operation flag 'addr' for parameter store operation", fullSpan, context);
    }
    else if ((flags & OperationFlags::deref) != OperationFlags::none)
    {
        ptr = emitter.EmitLoad(ptr);
        std::uint8_t n = GetDerefCount(flags);
        for (std::uint8_t i = 1; i < n; ++i)
        {
            ptr = emitter.EmitLoad(ptr);
        }
        if ((flags & OperationFlags::setPtr) != OperationFlags::none)
        {
            context->SetPtr(ptr);
        }
        emitter.EmitStore(value, ptr);
    }
    else
    {
        if ((flags & OperationFlags::setPtr) != OperationFlags::none)
        {
            context->SetPtr(ptr);
        }
        emitter.EmitStore(value, ptr);
    }
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundParentParameterNode::Clone() const
{
    BoundParentParameterNode* clone = new BoundParentParameterNode(parameter, GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

BoundEnumConstant::BoundEnumConstant(EnumConstantSymbol* enumConstant_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* enumType_)  :
    BoundExpressionNode(BoundNodeKind::boundEnumConstantNode, fullSpan_, enumType_), enumConstant(enumConstant_)
{
}

void BoundEnumConstant::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundEnumConstant::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if ((flags & OperationFlags::addr) != OperationFlags::none)
    {
        ThrowException("cannot take address of an enumeration constant", fullSpan, context);
    }
    else if ((flags & OperationFlags::deref) != OperationFlags::none)
    {
        ThrowException("cannot dereference an enumeration constant", fullSpan, context);
    }
    else
    {
        emitter.Stack().Push(enumConstant->GetValue(context)->IrValue(emitter, fullSpan, context));
    }
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundEnumConstant::Clone() const
{
    BoundExpressionNode* clone = new BoundEnumConstant(enumConstant, GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

BoundFunctionGroupNode::BoundFunctionGroupNode(FunctionGroupSymbol* functionGroupSymbol_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundFunctionGroupNode, fullSpan_, type_), functionGroupSymbol(functionGroupSymbol_)
{
}

void BoundFunctionGroupNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundFunctionGroupNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    emitter.Stack().Push(nullptr);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundFunctionGroupNode::Clone() const
{
    BoundFunctionGroupNode* clone = new BoundFunctionGroupNode(functionGroupSymbol, GetFullSpan(), GetType());
    for (auto templateArg : templateArgs)
    {
        clone->AddTemplateArg(templateArg);
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundFunctionGroupNode::AddTemplateArg(TypeSymbol* templateArg)
{
    templateArgs.push_back(templateArg);
}

BoundClassGroupNode::BoundClassGroupNode(ClassGroupSymbol* classGroupSymbol_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundClassGroupNode, fullSpan_, type_), classGroupSymbol(classGroupSymbol_)
{
}

void BoundClassGroupNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundClassGroupNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    emitter.Stack().Push(nullptr);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundClassGroupNode::Clone() const
{
    BoundClassGroupNode* clone = new BoundClassGroupNode(classGroupSymbol, GetFullSpan(), GetType());
    for (auto templateArg : templateArgs)
    {
        clone->AddTemplateArg(templateArg);
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundClassGroupNode::AddTemplateArg(TypeSymbol* templateArg)
{
    templateArgs.push_back(templateArg);
}

BoundAliasGroupNode::BoundAliasGroupNode(AliasGroupSymbol* aliasGroupSymbol_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundAliasGroupNode, fullSpan_, type_), aliasGroupSymbol(aliasGroupSymbol_)
{
}

void BoundAliasGroupNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundAliasGroupNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    emitter.Stack().Push(nullptr);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundAliasGroupNode::Clone() const
{
    BoundAliasGroupNode* clone = new BoundAliasGroupNode(aliasGroupSymbol, GetFullSpan(), GetType());
    for (auto templateArg : templateArgs)
    {
        clone->AddTemplateArg(templateArg);
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundAliasGroupNode::AddTemplateArg(TypeSymbol* templateArg)
{
    templateArgs.push_back(templateArg);
}

BoundTypeNode::BoundTypeNode(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundTypeNode, fullSpan_, type_)
{
}

void BoundTypeNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundTypeNode::Clone() const
{
    BoundExpressionNode* clone = new BoundTypeNode(GetType(), GetFullSpan());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

BoundMemberExprNode::BoundMemberExprNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundMemberExprNode, fullSpan_, type_), op()
{
}

BoundMemberExprNode::BoundMemberExprNode(BoundExpressionNode* subject_, BoundExpressionNode* member_, otava::ast::NodeKind op_,
    const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundMemberExprNode, fullSpan_, type_), subject(subject_), member(member_), op(op_)
{
}

void BoundMemberExprNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundMemberExprNode::Clone() const
{
    BoundExpressionNode* clone = new BoundMemberExprNode(subject->Clone(), member->Clone(), op, GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundMemberExprNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    subject->ModifyTypes(fullSpan, context);
    member->ModifyTypes(fullSpan, context);
}

BoundFunctionCallNode::BoundFunctionCallNode(FunctionSymbol* functionSymbol_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundFunctionCallNode, fullSpan_, type_), functionSymbol(functionSymbol_)
{
}

void BoundFunctionCallNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundFunctionCallNode::MayThrow() const noexcept
{
    if (!functionSymbol->IsNoExcept()) return true;
    return false;
}

bool BoundFunctionCallNode::HasValue(Context* context) const noexcept
{
    return functionSymbol->ReturnType(context) && !functionSymbol->ReturnType(context)->IsVoidType();
}

void BoundFunctionCallNode::AddArgument(BoundExpressionNode* arg)
{
    args.push_back(std::unique_ptr<BoundExpressionNode>(arg));
}

void BoundFunctionCallNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::vector<BoundExpressionNode*> arguments;
    for (const auto& arg : args)
    {
        arguments.push_back(arg.get());
    }
    if (GetFlag(BoundExpressionFlags::virtualCall))
    {
        flags = flags | OperationFlags::virtualCall;
    }
    functionSymbol->GenerateCode(emitter, arguments, flags, fullSpan, context);
    if ((flags & OperationFlags::deref) != OperationFlags::none && functionSymbol->Name() != "operator_bool")
    {
        otava::intermediate::Value* value = emitter.Stack().Pop();
        std::uint8_t n = GetDerefCount(flags);
        for (std::uint8_t i = 0; i < n; ++i)
        {
            value = emitter.EmitLoad(value);
        }
        emitter.Stack().Push(value);
    }
    DestructTemporaries(emitter, context);
}

void BoundFunctionCallNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if ((flags & OperationFlags::addr) != OperationFlags::none)
    {
        ThrowException("cannot take address of a function call", fullSpan, context);
    }
    else
    {
        otava::intermediate::Value* value = emitter.Stack().Pop();
        std::vector<BoundExpressionNode*> arguments;
        for (const auto& arg : args)
        {
            arguments.push_back(arg.get());
        }
        OperationFlags callFlags = OperationFlags::none;
        if (GetFlag(BoundExpressionFlags::virtualCall))
        {
            callFlags = callFlags | OperationFlags::virtualCall;
        }
        functionSymbol->GenerateCode(emitter, arguments, callFlags, fullSpan, context);
        otava::intermediate::Value* ptr = emitter.Stack().Pop();
        if ((flags & OperationFlags::deref) != OperationFlags::none || GetFlag(BoundExpressionFlags::deref))
        {
            std::uint8_t n = GetDerefCount(flags);
            for (std::uint8_t i = 1; i < n; ++i)
            {
                ptr = emitter.EmitLoad(ptr);
            }
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(value, ptr);
        }
        else
        {
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(emitter.EmitLoad(value), ptr);
        }
    }
    DestructTemporaries(emitter, context);
}

bool BoundFunctionCallNode::IsLvalueExpression(Context* context) const noexcept
{
    TypeSymbol* returnType = functionSymbol->ReturnType(context);
    if (returnType && !returnType->IsVoidType())
    {
        return returnType->IsLValueRefType();
    }
    return false;
}

BoundExpressionNode* BoundFunctionCallNode::Clone() const
{
    BoundFunctionCallNode* clone = new BoundFunctionCallNode(functionSymbol, GetFullSpan(), GetType());
    for (auto& arg : args)
    {
        clone->AddArgument(arg->Clone());
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

bool BoundFunctionCallNode::CallsClassConstructor(ClassTypeSymbol*& cls, BoundExpressionNode*& firstArg, FunctionDefinitionSymbol*& destructor) const noexcept
{
    if (functionSymbol->GetFunctionKind() != otava::symbols::FunctionKind::constructor) return false;
    if (args.empty()) return false;
    firstArg = args[0].get();
    if (!firstArg->IsBoundAddressOfNode()) return false;
    BoundAddressOfNode* addrOf = static_cast<BoundAddressOfNode*>(firstArg);
    TypeSymbol* subjectType = addrOf->Subject()->GetType();
    if (!subjectType->IsClassTypeSymbol()) return false;
    cls = static_cast<ClassTypeSymbol*>(subjectType);
    destructor = functionSymbol->Destructor();
    return true;
}

bool BoundFunctionCallNode::IsNoReturnFunctionCall() const noexcept
{
    if ((functionSymbol->Qualifiers() & FunctionQualifiers::noreturn) != FunctionQualifiers::none)
    {
        return true;
    }
    return false;
}

void BoundFunctionCallNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    if (functionSymbol->ReturnType(context) && functionSymbol->ReturnType(context)->GetBaseType(context)->IsForwardDeclarationSymbol())
    {
        ForwardClassDeclarationSymbol* fwdSymbol = static_cast<ForwardClassDeclarationSymbol*>(functionSymbol->ReturnType(context)->GetBaseType(context));
        if (fwdSymbol->GetClassTypeSymbol(context))
        {
            functionSymbol->SetReturnType(context->GetSymbolTable()->MakeCompoundType(
                fwdSymbol->GetClassTypeSymbol(context), functionSymbol->ReturnType(context)->GetDerivations(), context), context);
            SetType(functionSymbol->ReturnType(context));
        }
    }
    for (auto& arg : args)
    {
        arg->ModifyTypes(fullSpan, context);
    }
}

BoundEmptyFunctionCallNode::BoundEmptyFunctionCallNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundEmptyFunctionCallNode, fullSpan_, nullptr)
{
}

void BoundEmptyFunctionCallNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundEmptyFunctionCallNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundEmptyFunctionCallNode::Clone() const
{
    BoundExpressionNode* clone = new BoundEmptyFunctionCallNode(GetFullSpan());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

BoundFunctionPtrCallNode::BoundFunctionPtrCallNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundFunctionPtrCallNode, fullSpan_, type_)
{
}

void BoundFunctionPtrCallNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundFunctionPtrCallNode::AddArgument(BoundExpressionNode* arg)
{
    args.push_back(std::unique_ptr<BoundExpressionNode>(arg));
}

void BoundFunctionPtrCallNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    otava::intermediate::Value* callee = nullptr;
    std::vector<otava::intermediate::Value*> irArgs;
    bool first = true;
    for (const auto& arg : args)
    {
        arg->Load(emitter, OperationFlags::none, fullSpan, context);
        otava::intermediate::Value* irArg = emitter.Stack().Pop();
        if (first)
        {
            callee = irArg;
            first = false;
        }
        else
        {
            irArgs.push_back(irArg);
        }
    }
    otava::intermediate::Value* value = emitter.EmitCall(callee, irArgs);
    emitter.Stack().Push(value);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundFunctionPtrCallNode::Clone() const
{
    BoundFunctionPtrCallNode* clone = new BoundFunctionPtrCallNode(GetFullSpan(), GetType());
    for (auto& arg : args)
    {
        clone->AddArgument(arg->Clone());
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundFunctionPtrCallNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    for (auto& arg : args)
    {
        arg->ModifyTypes(fullSpan, context);
    }
}

BoundInvokeNode::BoundInvokeNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundInvokeNode, fullSpan_, type_)
{
}

void BoundInvokeNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundInvokeNode::HasValue(Context* context) const noexcept
{
    return invokeCall->HasValue(context);
}

BoundExpressionNode* BoundInvokeNode::Clone() const
{
    BoundInvokeNode* clone = new BoundInvokeNode(GetFullSpan(), GetType());
    if (result)
    {
        clone->SetResult(static_cast<BoundVariableNode*>(result->Clone()));
    }
    if (invokeCall)
    {
        clone->SetInvokeCall(static_cast<BoundFunctionCallNode*>(invokeCall->Clone()));
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundInvokeNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    invokeCall->Load(emitter, OperationFlags::none, fullSpan, context);
    if (result)
    {
        result->Load(emitter, flags, fullSpan, context);
    }
    DestructTemporaries(emitter, context);
}

void BoundInvokeNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if ((flags & OperationFlags::addr) != OperationFlags::none)
    {
        ThrowException("cannot take address of a function call", fullSpan, context);
    }
    else
    {
        otava::intermediate::Value* value = emitter.Stack().Pop();
        invokeCall->Load(emitter, OperationFlags::none, fullSpan, context);
        result->Load(emitter, OperationFlags::none, fullSpan, context);
        otava::intermediate::Value* ptr = emitter.Stack().Pop();
        if ((flags & OperationFlags::deref) != OperationFlags::none || GetFlag(BoundExpressionFlags::deref))
        {
            std::uint8_t n = GetDerefCount(flags);
            for (std::uint8_t i = 1; i < n; ++i)
            {
                ptr = emitter.EmitLoad(ptr);
            }
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(value, ptr);
        }
        else
        {
            if ((flags & OperationFlags::setPtr) != OperationFlags::none)
            {
                context->SetPtr(ptr);
            }
            emitter.EmitStore(emitter.EmitLoad(value), ptr);
        }
    }
    DestructTemporaries(emitter, context);
}


BoundExpressionSequenceNode::BoundExpressionSequenceNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type) :
    BoundExpressionNode(BoundNodeKind::boundExpressionSequenceNode, fullSpan_, type), left(), right()
{
}

BoundExpressionSequenceNode::BoundExpressionSequenceNode(const soul::ast::FullSpan& fullSpan_, BoundExpressionNode* left_, BoundExpressionNode* right_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundExpressionSequenceNode, fullSpan_, right_->GetType()), left(left_), right(right_)
{
}

void BoundExpressionSequenceNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundExpressionSequenceNode::MayThrow() const noexcept
{
    if (left->MayThrow()) return true;
    if (right->MayThrow()) return true;
    return false;
}

bool BoundExpressionSequenceNode::IsLvalueExpression(Context* context) const noexcept
{
    return right->IsLvalueExpression(context);
}

BoundExpressionNode* BoundExpressionSequenceNode::Clone() const
{
    BoundExpressionSequenceNode* clone = new BoundExpressionSequenceNode(GetFullSpan(), left->Clone(), right->Clone());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundExpressionSequenceNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    left->Load(emitter, flags, fullSpan, context);
    if (left->HasValue(context))
    {
        emitter.Stack().Pop();
    }
    right->Load(emitter, flags, fullSpan, context);
    DestructTemporaries(emitter, context);
}

void BoundExpressionSequenceNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    left->Load(emitter, flags, fullSpan, context);
    if (left->HasValue(context))
    {
        emitter.Stack().Pop();
    }
    right->Store(emitter, flags, fullSpan, context);
    DestructTemporaries(emitter, context);
}

bool BoundExpressionSequenceNode::HasValue(Context* context) const noexcept
{
    return right->HasValue(context);
}

void BoundExpressionSequenceNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    left->ModifyTypes(fullSpan, context);
    right->ModifyTypes(fullSpan, context);
}

BoundExpressionListNode::BoundExpressionListNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundExpressionListNode, fullSpan_, nullptr)
{
}

void BoundExpressionListNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundExpressionListNode::Clone() const
{
    BoundExpressionListNode* clone = new BoundExpressionListNode(GetFullSpan());
    for (auto& expr : exprs)
    {
        clone->AddExpression(expr->Clone());
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundExpressionListNode::AddExpression(BoundExpressionNode* expr)
{
    if (exprs.empty())
    {
        SetType(expr->GetType());
    }
    else
    {
        SetType(nullptr);
    }
    exprs.push_back(std::unique_ptr<BoundExpressionNode>(expr));
}

void BoundExpressionListNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    for (auto& expr : exprs)
    {
        expr->ModifyTypes(fullSpan, context);
    }
}

BoundConjunctionNode::BoundConjunctionNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* boolType) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConjunctionNode, fullSpan_, boolType)
{
}

BoundConjunctionNode::BoundConjunctionNode(BoundExpressionNode* left_, BoundExpressionNode* right_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* boolType) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConjunctionNode, fullSpan_, boolType), left(left_), right(right_)
{
}

void BoundConjunctionNode::SetTemporary(BoundVariableNode* temporary_) noexcept
{
    temporary.reset(temporary_);
}

void BoundConjunctionNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundConjunctionNode::MayThrow() const noexcept
{
    if (left->MayThrow()) return true;
    if (right->MayThrow()) return true;
    return false;
}

bool BoundConjunctionNode::HasValue(Context* context) const noexcept
{
    return true;
}

void BoundConjunctionNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    temporary->Load(emitter, OperationFlags::addr, fullSpan, context);
    otava::intermediate::Value* temp = emitter.Stack().Pop();
    left->Load(emitter, OperationFlags::none, fullSpan, context);
    left->DestructTemporaries(emitter, context);
    otava::intermediate::Value* leftValue = emitter.Stack().Pop();
    otava::intermediate::BasicBlock* trueBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* rightBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* falseBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter.CreateBasicBlock();
    emitter.EmitBranch(leftValue, rightBlock, falseBlock);
    emitter.SetCurrentBasicBlock(rightBlock);
    right->Load(emitter, OperationFlags::none, fullSpan, context);
    right->DestructTemporaries(emitter, context);
    otava::intermediate::Value* rightValue = emitter.Stack().Pop();
    emitter.EmitBranch(rightValue, trueBlock, falseBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    emitter.EmitStore(emitter.EmitBool(true), temp);
    emitter.EmitJump(nextBlock);
    emitter.SetCurrentBasicBlock(falseBlock);
    emitter.EmitStore(emitter.EmitBool(false), temp);
    emitter.EmitJump(nextBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
    otava::intermediate::Value* value = emitter.EmitLoad(temp);
    emitter.Stack().Push(value);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundConjunctionNode::Clone() const
{
    BoundConjunctionNode* clone = new BoundConjunctionNode(left->Clone(), right->Clone(), GetFullSpan(), GetType());
    if (temporary)
    {
        clone->SetTemporary(static_cast<BoundVariableNode*>(temporary->Clone()));
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundConjunctionNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    left->ModifyTypes(fullSpan, context);
    right->ModifyTypes(fullSpan, context);
    if (temporary)
    {
        temporary->ModifyTypes(fullSpan, context);
    }
}

BoundDisjunctionNode::BoundDisjunctionNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* boolType) noexcept :
    BoundExpressionNode(BoundNodeKind::boundDisjunctionNode, fullSpan_, boolType), left(), right()
{
}

BoundDisjunctionNode::BoundDisjunctionNode(BoundExpressionNode* left_, BoundExpressionNode* right_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* boolType) noexcept :
    BoundExpressionNode(BoundNodeKind::boundDisjunctionNode, fullSpan_, boolType), left(left_), right(right_)
{
}

void BoundDisjunctionNode::SetTemporary(BoundVariableNode* temporary_) noexcept
{
    temporary.reset(temporary_);
}

void BoundDisjunctionNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundDisjunctionNode::MayThrow() const noexcept
{
    if (left->MayThrow()) return true;
    if (right->MayThrow()) return true;
    return false;
}

bool BoundDisjunctionNode::HasValue(Context* context) const noexcept
{
    return true;
}

void BoundDisjunctionNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    temporary->Load(emitter, OperationFlags::addr, fullSpan, context);
    otava::intermediate::Value* temp = emitter.Stack().Pop();
    left->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* leftValue = emitter.Stack().Pop();
    otava::intermediate::BasicBlock* trueBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* rightBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* falseBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter.CreateBasicBlock();
    emitter.EmitBranch(leftValue, trueBlock, rightBlock);
    emitter.SetCurrentBasicBlock(rightBlock);
    right->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* rightValue = emitter.Stack().Pop();
    emitter.EmitBranch(rightValue, trueBlock, falseBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    emitter.EmitStore(emitter.EmitBool(true), temp);
    emitter.EmitJump(nextBlock);
    emitter.SetCurrentBasicBlock(falseBlock);
    emitter.EmitStore(emitter.EmitBool(false), temp);
    emitter.EmitJump(nextBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
    otava::intermediate::Value* value = emitter.EmitLoad(temp);
    emitter.Stack().Push(value);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundDisjunctionNode::Clone() const
{
    BoundDisjunctionNode* clone = new BoundDisjunctionNode(left->Clone(), right->Clone(), GetFullSpan(), GetType());
    if (temporary)
    {
        clone->SetTemporary(static_cast<BoundVariableNode*>(temporary->Clone()));
    }
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundDisjunctionNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    left->ModifyTypes(fullSpan, context);
    right->ModifyTypes(fullSpan, context);
    if (temporary)
    {
        temporary->ModifyTypes(fullSpan, context);
    }
}

BoundConditionalExprNode::BoundConditionalExprNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConditionalExprNode, fullSpan_, type)
{
}

BoundConditionalExprNode::BoundConditionalExprNode(BoundExpressionNode* condition_, BoundExpressionNode* thenExpr_, BoundExpressionNode* elseExpr_,
    TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConditionalExprNode, condition_->GetFullSpan(), type_),
    condition(condition_), thenExpr(thenExpr_), elseExpr(elseExpr_)
{
}

void BoundConditionalExprNode::SetTemporary(BoundVariableNode* temporary_) noexcept
{
    temporary.reset(temporary_);
}

void BoundConditionalExprNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundConditionalExprNode::MayThrow() const noexcept
{
    if (condition->MayThrow()) return true;
    if (thenExpr->MayThrow()) return true;
    if (elseExpr->MayThrow()) return true;
    return false;
}

void BoundConditionalExprNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    temporary->Load(emitter, OperationFlags::addr, fullSpan, context);
    otava::intermediate::Value* temp = emitter.Stack().Pop();
    condition->Load(emitter, flags, fullSpan, context);
    otava::intermediate::Value* cond = emitter.Stack().Pop();
    otava::intermediate::BasicBlock* trueBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* falseBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter.CreateBasicBlock();
    emitter.EmitBranch(cond, trueBlock, falseBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    thenExpr->Load(emitter, flags, fullSpan, context);
    thenExpr->DestructTemporaries(emitter, context);
    otava::intermediate::Value* left = emitter.Stack().Pop();
    emitter.EmitStore(left, temp);
    emitter.EmitJump(nextBlock);
    emitter.SetCurrentBasicBlock(falseBlock);
    elseExpr->Load(emitter, flags, fullSpan, context);
    elseExpr->DestructTemporaries(emitter, context);
    otava::intermediate::Value* right = emitter.Stack().Pop();
    emitter.EmitStore(right, temp);
    emitter.EmitJump(nextBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
    otava::intermediate::Value* value = emitter.EmitLoad(temp);
    emitter.Stack().Push(value);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundConditionalExprNode::Clone() const
{
    BoundConditionalExprNode* clone = new BoundConditionalExprNode(condition->Clone(), thenExpr->Clone(), elseExpr->Clone(), GetType());
    clone->SetTemporary(static_cast<BoundVariableNode*>(temporary->Clone()));
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundConditionalExprNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    condition->ModifyTypes(fullSpan, context);
    thenExpr->ModifyTypes(fullSpan, context);
    elseExpr->ModifyTypes(fullSpan, context);
    if (temporary)
    {
        temporary->ModifyTypes(fullSpan, context);
    }
}

BoundConversionNode::BoundConversionNode(FunctionSymbol* conversionFunction_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConversionNode, fullSpan_, type_), subject(), conversionFunction(conversionFunction_)
{
}

BoundConversionNode::BoundConversionNode(BoundExpressionNode* subject_, FunctionSymbol* conversionFunction_, 
    const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConversionNode, fullSpan_, type_), subject(subject_), conversionFunction(conversionFunction_)
{
}

void BoundConversionNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundConversionNode::MayThrow() const noexcept
{
    if (subject->MayThrow()) return true;
    if (!conversionFunction->IsNoExcept()) return true;
    return false;
}

void BoundConversionNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    subject->Load(emitter, flags, fullSpan, context);
    std::vector<BoundExpressionNode*> args;
    conversionFunction->GenerateCode(emitter, args, flags, fullSpan, context);
    if ((flags & OperationFlags::derefAfterConv) != OperationFlags::none)
    {
        otava::intermediate::Value* value = emitter.Stack().Pop();
        std::uint8_t derefCount = GetDerefCount(flags);
        for (std::uint8_t i = 0; i < derefCount; ++i)
        {
            value = emitter.EmitLoad(value);
        }
        emitter.Stack().Push(value);
    }
    DestructTemporaries(emitter, context);
}

void BoundConversionNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Load(emitter, flags, fullSpan, context);
    subject->Store(emitter, flags, fullSpan, context);
    DestructTemporaries(emitter, context);
}

bool BoundConversionNode::IsLvalueExpression(Context* context) const noexcept
{
    return false; // todo???
}

BoundExpressionNode* BoundConversionNode::Clone() const
{
    BoundExpressionNode* clone = new BoundConversionNode(subject->Clone(), conversionFunction, GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundConversionNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    subject->ModifyTypes(fullSpan, context);
}

BoundAddressOfNode::BoundAddressOfNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundAddressOfNode, fullSpan_, type_), subject()
{
}

BoundAddressOfNode::BoundAddressOfNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundAddressOfNode, fullSpan_, type_), subject(subject_)
{
}

void BoundAddressOfNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundAddressOfNode::MayThrow() const noexcept
{
    if (subject->IsBoundDereferenceNode())
    {
        BoundDereferenceNode* derefExpr = static_cast<BoundDereferenceNode*>(subject.get());
        if (derefExpr->Subject()->MayThrow()) return true;
    }
    return false;
}

void BoundAddressOfNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!subject->IsBoundDereferenceNode())
    {
        subject->Load(emitter, OperationFlags::addr, fullSpan, context);
    }
    else
    {
        BoundDereferenceNode* derefExpr = static_cast<BoundDereferenceNode*>(subject.get());
        derefExpr->Subject()->Load(emitter, flags, fullSpan, context);
    }
    DestructTemporaries(emitter, context);
}

void BoundAddressOfNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!subject->IsBoundDereferenceNode())
    {
        subject->Store(emitter, OperationFlags::addr | (flags & OperationFlags::setPtr), fullSpan, context);
    }
    else
    {
        BoundDereferenceNode* derefExpr = static_cast<BoundDereferenceNode*>(subject.get());
        derefExpr->Subject()->Store(emitter, flags, fullSpan, context);
    }
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundAddressOfNode::Clone() const
{
    BoundExpressionNode* clone = new BoundAddressOfNode(subject->Clone(), GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundAddressOfNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    subject->ModifyTypes(fullSpan, context);
}

BoundDereferenceNode::BoundDereferenceNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundDereferenceNode, fullSpan_, type_), kind(OperationFlags::deref)
{
}

BoundDereferenceNode::BoundDereferenceNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundDereferenceNode, fullSpan_, type_), subject(subject_), kind(OperationFlags::deref)
{
}

BoundDereferenceNode::BoundDereferenceNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_, OperationFlags kind_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundDereferenceNode, fullSpan_, type_), subject(subject_), kind(kind_)
{
}

void BoundDereferenceNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundDereferenceNode::MayThrow() const noexcept
{
    if (subject->MayThrow()) return true;
    return false;
}

void BoundDereferenceNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!subject->IsBoundAddressOfNode())
    {
        if (GetDerefCount(flags) == 0 && (flags & OperationFlags::addr) != OperationFlags::none)
        {
            subject->Load(emitter, OperationFlags::none, fullSpan, context);
        }
        else
        {
            subject->Load(emitter, SetDerefCount(kind, GetDerefCount(flags) + 1), fullSpan, context);
        }
    }
    else
    {
        BoundAddressOfNode* addressOfExpr = static_cast<BoundAddressOfNode*>(subject.get());
        addressOfExpr->Subject()->Load(emitter, flags, fullSpan, context);
    }
    DestructTemporaries(emitter, context);
}

void BoundDereferenceNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!subject->IsBoundAddressOfNode())
    {
        subject->Store(emitter, SetDerefCount(OperationFlags::deref | (flags & OperationFlags::setPtr), GetDerefCount(flags) + 1), fullSpan, context);
    }
    else
    {
        BoundAddressOfNode* addressOfExpr = static_cast<BoundAddressOfNode*>(subject.get());
        addressOfExpr->Subject()->Store(emitter, flags, fullSpan, context);
    }
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundDereferenceNode::Clone() const
{
    BoundExpressionNode* clone = new BoundDereferenceNode(subject->Clone(), GetFullSpan(), GetType(), kind);
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundDereferenceNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    subject->ModifyTypes(fullSpan, context);
}

BoundRefToPtrNode::BoundRefToPtrNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundRefToPtrNode, fullSpan_, type_), subject()
{
}

BoundRefToPtrNode::BoundRefToPtrNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundRefToPtrNode, fullSpan_, type_), subject(subject_)
{
}

void BoundRefToPtrNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundRefToPtrNode::MayThrow() const noexcept
{
    if (subject->MayThrow()) return true;
    return false;
}

void BoundRefToPtrNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    subject->Load(emitter, flags, fullSpan, context);
    DestructTemporaries(emitter, context);
}

void BoundRefToPtrNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    subject->Store(emitter, flags, fullSpan, context);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundRefToPtrNode::Clone() const
{
    BoundExpressionNode* clone = new BoundRefToPtrNode(subject->Clone(), GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundRefToPtrNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    subject->ModifyTypes(fullSpan, context);
}

BoundPtrToRefNode::BoundPtrToRefNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundPtrToRefNode, fullSpan_, type_), subject()
{
}

BoundPtrToRefNode::BoundPtrToRefNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundPtrToRefNode, fullSpan_, type_), subject(subject_)
{
}

void BoundPtrToRefNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundPtrToRefNode::Clone() const
{
    BoundExpressionNode* clone = new BoundPtrToRefNode(subject->Clone(), GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

bool BoundPtrToRefNode::MayThrow() const noexcept
{
    if (subject->MayThrow()) return true;
    return false;
}

void BoundPtrToRefNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    subject->Load(emitter, flags, fullSpan, context);
    DestructTemporaries(emitter, context);
}

void BoundPtrToRefNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    subject->Store(emitter, flags, fullSpan, context);
    DestructTemporaries(emitter, context);
}

void BoundPtrToRefNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    subject->ModifyTypes(fullSpan, context);
}

BoundDefaultInitNode::BoundDefaultInitNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundDefaultInitNode, fullSpan_, type_), subject()
{
}

BoundDefaultInitNode::BoundDefaultInitNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundDefaultInitNode, fullSpan_, subject_->GetType()), subject(subject_)
{
}

void BoundDefaultInitNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundDefaultInitNode::MayThrow() const noexcept
{
    if (subject->MayThrow()) return true;
    return false;
}

void BoundDefaultInitNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    subject->Load(emitter, flags | OperationFlags::defaultInit, fullSpan, context);
    DestructTemporaries(emitter, context);
}

void BoundDefaultInitNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    subject->Store(emitter, flags | OperationFlags::defaultInit, fullSpan, context);
    DestructTemporaries(emitter, context);
}

BoundExpressionNode* BoundDefaultInitNode::Clone() const
{
    BoundExpressionNode* clone = new BoundDefaultInitNode(subject->Clone(), GetFullSpan());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundDefaultInitNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    subject->ModifyTypes(fullSpan, context);
}

BoundTemporaryNode::BoundTemporaryNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundTemporaryNode, fullSpan_, type_), rvalueExpr(), backingStore()
{
}

BoundTemporaryNode::BoundTemporaryNode(BoundExpressionNode* rvalueExpr_, BoundVariableNode* backingStore_, const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundTemporaryNode, fullSpan_, rvalueExpr_->GetType()), rvalueExpr(rvalueExpr_), backingStore(backingStore_)
{
}

void BoundTemporaryNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!backingStore)
    {
        ThrowException("backing store of temporary not set", fullSpan, context);
    }
    rvalueExpr->Load(emitter, OperationFlags::none, fullSpan, context);
    backingStore->Store(emitter, OperationFlags::none, fullSpan, context);
    if ((flags & OperationFlags::addr) != OperationFlags::none)
    {
        backingStore->Load(emitter, OperationFlags::addr, fullSpan, context);
    }
    else if ((flags & OperationFlags::deref) != OperationFlags::none)
    {
        backingStore->Load(emitter, SetDerefCount(OperationFlags::deref, GetDerefCount(flags) + 1), fullSpan, context);
    }
    else
    {
        backingStore->Load(emitter, OperationFlags::none, fullSpan, context);
    }
    DestructTemporaries(emitter, context);
}

void BoundTemporaryNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    backingStore->Store(emitter, OperationFlags::none, fullSpan, context);
    DestructTemporaries(emitter, context);
}

void BoundTemporaryNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundTemporaryNode::MayThrow() const noexcept
{
    if (rvalueExpr->MayThrow()) return true;
    if (backingStore->MayThrow()) return true;
    return false;
}

BoundExpressionNode* BoundTemporaryNode::Clone() const
{
    BoundExpressionNode* clone = new BoundTemporaryNode(rvalueExpr->Clone(), static_cast<BoundVariableNode*>(backingStore->Clone()), GetFullSpan());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundTemporaryNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    rvalueExpr->ModifyTypes(fullSpan, context);
    backingStore->ModifyTypes(fullSpan, context);
}

BoundConstructTemporaryNode::BoundConstructTemporaryNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConstructTemporaryNode, fullSpan_, type_), constructorCall(), temporary()
{
}

BoundConstructTemporaryNode::BoundConstructTemporaryNode(BoundExpressionNode* constructorCall_, BoundExpressionNode* temporary_,
    const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConstructTemporaryNode, fullSpan_, temporary_->GetType()), constructorCall(constructorCall_), temporary(temporary_)
{
}

void BoundConstructTemporaryNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    constructorCall->Load(emitter, OperationFlags::defaultInit, fullSpan, context);
    temporary->Load(emitter, flags, fullSpan, context);
    DestructTemporaries(emitter, context);
}

void BoundConstructTemporaryNode::Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    temporary->Store(emitter, flags, fullSpan, context);
    DestructTemporaries(emitter, context);
}

void BoundConstructTemporaryNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundConstructTemporaryNode::MayThrow() const noexcept
{
    if (constructorCall->MayThrow()) return true;
    if (temporary->MayThrow()) return true;
    return false;
}

BoundExpressionNode* BoundConstructTemporaryNode::Clone() const
{
    BoundExpressionNode* clone = new BoundConstructTemporaryNode(constructorCall->Clone(), temporary->Clone(), GetFullSpan());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundConstructTemporaryNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    constructorCall->ModifyTypes(fullSpan, context);
    temporary->ModifyTypes(fullSpan, context);
}

BoundDestructTemporariesNode::BoundDestructTemporariesNode(const soul::ast::FullSpan& fullSpan_,
    std::vector<std::unique_ptr<BoundFunctionCallNode>>&& temporaryDestructorCalls_) :
    BoundExpressionNode(BoundNodeKind::boundDestructTemporariesNode, fullSpan_, nullptr),
    temporaryDestructorCalls(std::move(temporaryDestructorCalls_))
{
}

void BoundDestructTemporariesNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    for (const auto& call : temporaryDestructorCalls)
    {
        call->Load(emitter, flags, fullSpan, context);
    }
}

void BoundDestructTemporariesNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundDestructTemporariesNode::Clone() const
{
    std::vector<std::unique_ptr<BoundFunctionCallNode>> clonedTemporaryDestructorCalls;
    for (const auto& temporaryDestructorCall : temporaryDestructorCalls)
    {
        clonedTemporaryDestructorCalls.push_back(std::unique_ptr<BoundFunctionCallNode>(static_cast<BoundFunctionCallNode*>(temporaryDestructorCall->Clone())));
    }
    BoundDestructTemporariesNode* clone = new BoundDestructTemporariesNode(GetFullSpan(), std::move(clonedTemporaryDestructorCalls));
    return clone;
}

void BoundDestructTemporariesNode::Merge(BoundDestructTemporariesNode* that)
{
    for (const auto& temporaryDestructorCall : that->temporaryDestructorCalls)
    {
        temporaryDestructorCalls.push_back(std::unique_ptr<BoundFunctionCallNode>(static_cast<BoundFunctionCallNode*>(temporaryDestructorCall->Clone())));
    }
}

BoundConstructExpressionNode::BoundConstructExpressionNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConstructExpressionNode, fullSpan_, type_), allocation(), constructObjectCall(), hasPlacement(false)
{
}

BoundConstructExpressionNode::BoundConstructExpressionNode(BoundExpressionNode* allocation_, BoundExpressionNode* constructObjectCall_,
    TypeSymbol* type_, bool hasPlacement_, const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundConstructExpressionNode, fullSpan_, type_), allocation(allocation_), constructObjectCall(constructObjectCall_),
    hasPlacement(hasPlacement_)
{
}

void BoundConstructExpressionNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    allocation->Load(emitter, flags | OperationFlags::dup, fullSpan, context);
    constructObjectCall->Load(emitter, flags | OperationFlags::storeDeref | OperationFlags::defaultInit, fullSpan, context);
    DestructTemporaries(emitter, context);
}

void BoundConstructExpressionNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

bool BoundConstructExpressionNode::MayThrow() const noexcept
{
    if (allocation->MayThrow()) return true;
    if (constructObjectCall->MayThrow()) return true;
    return false;
}

BoundExpressionNode* BoundConstructExpressionNode::Clone() const
{
    BoundExpressionNode* clone = new BoundConstructExpressionNode(allocation->Clone(), constructObjectCall->Clone(), GetType(), hasPlacement, GetFullSpan());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundConstructExpressionNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    allocation->ModifyTypes(fullSpan, context);
    constructObjectCall->ModifyTypes(fullSpan, context);
}

BoundGlobalVariableDefinitionNode::BoundGlobalVariableDefinitionNode(VariableSymbol* globalVariable_, const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundNode(BoundNodeKind::boundGlobalVariableDefinitionNode, fullSpan_), globalVariable(globalVariable_)
{
}

void BoundGlobalVariableDefinitionNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundEmptyDestructorNode::BoundEmptyDestructorNode(const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundEmptyDestructorNode, fullSpan_, nullptr)
{
}

void BoundEmptyDestructorNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundEmptyDestructorNode::Clone() const
{
    BoundExpressionNode* clone = new BoundEmptyDestructorNode(GetFullSpan());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundEmptyDestructorNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    DestructTemporaries(emitter, context);
}

BoundFunctionValueNode::BoundFunctionValueNode(FunctionSymbol* function_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundFunctionValueNode, fullSpan_, type_), function(function_)
{
}

void BoundFunctionValueNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundFunctionValueNode::Clone() const
{
    BoundExpressionNode* clone = new BoundFunctionValueNode(function, GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundFunctionValueNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    emitter.GetOrInsertFunction(function->IrName(context), static_cast<otava::intermediate::FunctionType*>(function->IrType(emitter, fullSpan, context)));
    otava::intermediate::Value* functionValue = emitter.EmitSymbolValue(function->IrType(emitter, fullSpan, context), function->IrName(context));
    otava::intermediate::Type* voidPtrIrType = emitter.MakePtrType(emitter.GetVoidType());
    otava::intermediate::Value* functionValueAsVoidPtr = emitter.EmitBitcast(functionValue, voidPtrIrType);
    emitter.Stack().Push(functionValueAsVoidPtr);
    DestructTemporaries(emitter, context);
}

BoundVariableAsVoidPtrNode::BoundVariableAsVoidPtrNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundVariableAsVoidPtrNode, fullSpan_, type_)
{
}

BoundVariableAsVoidPtrNode::BoundVariableAsVoidPtrNode(BoundExpressionNode* addrOfBoundVariable_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundVariableAsVoidPtrNode, fullSpan_, type_), addrOfBoundVariable(addrOfBoundVariable_)
{
}

void BoundVariableAsVoidPtrNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundVariableAsVoidPtrNode::Clone() const
{
    BoundExpressionNode* clone = new BoundVariableAsVoidPtrNode(addrOfBoundVariable->Clone(), GetFullSpan(), GetType());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundVariableAsVoidPtrNode::Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context)
{
    addrOfBoundVariable->Load(emitter, flags, fullSpan, context);
    otava::intermediate::Value* variableValue = emitter.Stack().Pop();
    otava::intermediate::Type* voidPtrIrType = emitter.MakePtrType(emitter.GetVoidType());
    otava::intermediate::Value* variableAsVoidPtr = emitter.EmitBitcast(variableValue, voidPtrIrType);
    emitter.Stack().Push(variableAsVoidPtr);
    DestructTemporaries(emitter, context);
}

void BoundVariableAsVoidPtrNode::ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BoundExpressionNode::ModifyTypes(fullSpan, context);
    addrOfBoundVariable->ModifyTypes(fullSpan, context);
}

BoundOperatorFnNode::BoundOperatorFnNode(otava::ast::NodeKind operatorFnNodeKind_, const soul::ast::FullSpan& fullSpan_) noexcept :
    BoundExpressionNode(BoundNodeKind::boundOperatorFnNode, fullSpan_, nullptr), operatorFnNodeKind(operatorFnNodeKind_)
{
}

void BoundOperatorFnNode::Accept(BoundTreeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundExpressionNode* BoundOperatorFnNode::Clone() const
{
    BoundOperatorFnNode* clone = new BoundOperatorFnNode(operatorFnNodeKind, GetFullSpan());
    if (Source())
    {
        clone->SetSource(Source()->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

bool InDirectSwitchStatement(BoundStatementNode* statement)
{
    BoundStatementNode* parent = statement->Parent();
    while (parent)
    {
        if (parent->IsSwitchStatementNode()) return true;
        if (parent->IsWhileStatementNode() || parent->IsDoStatementNode() || parent->IsForStatementNode()) return false;
        parent = parent->Parent();
    }
    return false;
}

} // namespace otava::symbols
