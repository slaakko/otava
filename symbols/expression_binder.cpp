// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.expression_binder;

import otava.symbols.argument_conversion_table;
import otava.symbols.bound_tree;
import otava.symbols.bound_tree_visitor;
import otava.symbols.context;
import otava.symbols.declaration;
import otava.symbols.declarator;
import otava.symbols.enum_group_symbol;
import otava.symbols.evaluator;
import otava.symbols.exception;
import otava.symbols.expr_parser;
import otava.symbols.function_group_symbol;
import otava.symbols.overload_resolution;
import otava.symbols.scope_resolver;
import otava.symbols.statement_binder;
import otava.symbols.type_resolver;
import otava.symbols.type_symbol;
import otava.symbols.type_compare;
import otava.symbols.variable_group_symbol;
import otava.ast.expression;
import otava.ast.function;
import otava.ast.identifier;
import otava.ast.literal;
import otava.ast.punctuation;
import otava.ast.simple_type;
import otava.ast.statement;
import otava.ast.templates;
import otava.ast.visitor;
import util.text_util;

namespace otava::symbols {

std::pair<VariableSymbol*, int> GetParentTemporary(std::int64_t nodeId, Context* context)
{
    std::pair<VariableSymbol*, int> p;
    int level = 0;
    StatementBinder* parentStatementBinder = context->GetParentStatementBinder();
    if (parentStatementBinder)
    {
        FunctionDefinitionSymbol* fnDefSymbol = parentStatementBinder->GetFunctionDefinitionSymbol();
        if (fnDefSymbol)
        {
            while (fnDefSymbol)
            {
                VariableSymbol* temporary = fnDefSymbol->GetTemporary(nodeId);
                if (temporary)
                {
                    return std::make_pair(temporary, level);
                }
                else
                {
                    fnDefSymbol = fnDefSymbol->ParentFn();
                    ++level;
                }
            }
        }
    }
    return p;
}

class OperatorGroupNameMap
{
public:
    static OperatorGroupNameMap& Instance();
    std::string GetGroupName(otava::ast::NodeKind nodeKind, const soul::ast::FullSpan& fullSpan, Context* context);
private:
    OperatorGroupNameMap();
    std::map<otava::ast::NodeKind, std::string> operatorGroupNameMap;
};

OperatorGroupNameMap& OperatorGroupNameMap::Instance()
{
    static OperatorGroupNameMap instance;
    return instance;
}

OperatorGroupNameMap::OperatorGroupNameMap()
{
    operatorGroupNameMap[otava::ast::NodeKind::plusNode] = "operator+";
    operatorGroupNameMap[otava::ast::NodeKind::minusNode] = "operator-";
    operatorGroupNameMap[otava::ast::NodeKind::mulNode] = "operator*";
    operatorGroupNameMap[otava::ast::NodeKind::divNode] = "operator/";
    operatorGroupNameMap[otava::ast::NodeKind::modNode] = "operator%";
    operatorGroupNameMap[otava::ast::NodeKind::andNode] = "operator&";
    operatorGroupNameMap[otava::ast::NodeKind::inclusiveOrNode] = "operator|";
    operatorGroupNameMap[otava::ast::NodeKind::exclusiveOrNode] = "operator^";
    operatorGroupNameMap[otava::ast::NodeKind::shiftLeftNode] = "operator<<";
    operatorGroupNameMap[otava::ast::NodeKind::shiftRightNode] = "operator>>";
    operatorGroupNameMap[otava::ast::NodeKind::assignNode] = "operator=";
    operatorGroupNameMap[otava::ast::NodeKind::plusAssignNode] = "operator+=";
    operatorGroupNameMap[otava::ast::NodeKind::minusAssignNode] = "operator-=";
    operatorGroupNameMap[otava::ast::NodeKind::mulAssignNode] = "operator*=";
    operatorGroupNameMap[otava::ast::NodeKind::divAssignNode] = "operator/=";
    operatorGroupNameMap[otava::ast::NodeKind::modAssignNode] = "operator%=";
    operatorGroupNameMap[otava::ast::NodeKind::xorAssignNode] = "operator^=";
    operatorGroupNameMap[otava::ast::NodeKind::andAssignNode] = "operator&=";
    operatorGroupNameMap[otava::ast::NodeKind::orAssignNode] = "operator|=";
    operatorGroupNameMap[otava::ast::NodeKind::shiftLeftAssignNode] = "operator<<=";
    operatorGroupNameMap[otava::ast::NodeKind::shiftRightAssignNode] = "operator>>=";
    operatorGroupNameMap[otava::ast::NodeKind::equalNode] = "operator==";
    operatorGroupNameMap[otava::ast::NodeKind::lessNode] = "operator<";
    operatorGroupNameMap[otava::ast::NodeKind::derefNode] = "operator*";
    operatorGroupNameMap[otava::ast::NodeKind::addrOfNode] = "operator&";
    operatorGroupNameMap[otava::ast::NodeKind::notNode] = "operator!";
    operatorGroupNameMap[otava::ast::NodeKind::complementNode] = "operator~";
    operatorGroupNameMap[otava::ast::NodeKind::prefixIncNode] = "operator++";
    operatorGroupNameMap[otava::ast::NodeKind::prefixDecNode] = "operator--";
    operatorGroupNameMap[otava::ast::NodeKind::arrowNode] = "operator->";
    operatorGroupNameMap[otava::ast::NodeKind::subscriptExprNode] = "operator[]";
    operatorGroupNameMap[otava::ast::NodeKind::newOpNode] = "operator new";
}

std::string OperatorGroupNameMap::GetGroupName(otava::ast::NodeKind nodeKind, const soul::ast::FullSpan& fullSpan, Context* context)
{
    auto it = operatorGroupNameMap.find(nodeKind);
    if (it != operatorGroupNameMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowException("group name for node kind '" + otava::ast::NodeKindStr(nodeKind) + "' not found", fullSpan, context);
    }
    return std::string();
}

class GroupNameResolver : public DefaultBoundTreeVisitor
{
public:
    GroupNameResolver(Context* context_);
    inline const std::string& GetGroupName() const { return groupName; }
    void Visit(BoundFunctionGroupNode& node) override;
    void Visit(BoundVariableNode& node) override;
    void Visit(BoundParameterNode& node) override;
    void Visit(BoundConstructTemporaryNode& node) override;
    void Visit(BoundOperatorFnNode& node) override;
private:
    std::string groupName;
    Context* context;
};

GroupNameResolver::GroupNameResolver(Context* context_) : context(context_)
{
}

void GroupNameResolver::Visit(BoundFunctionGroupNode& node)
{
    groupName = node.GetFunctionGroupSymbol()->Name();
}

void GroupNameResolver::Visit(BoundVariableNode& node)
{
    groupName = "operator()";
}

void GroupNameResolver::Visit(BoundParameterNode& node)
{
    groupName = "operator()";
}

void GroupNameResolver::Visit(BoundConstructTemporaryNode& node)
{
    groupName = "operator()";
}

void GroupNameResolver::Visit(BoundOperatorFnNode& node)
{
    groupName = OperatorGroupNameMap::Instance().GetGroupName(node.OperatorFnNodeKind(), node.GetFullSpan(), context);
}

std::string GetGroupName(BoundNode* node, Context* context)
{
    GroupNameResolver groupNameResolver(context);
    node->Accept(groupNameResolver);
    return groupNameResolver.GetGroupName();
}

class FirstArgResolver : public DefaultBoundTreeVisitor
{
public:
    FirstArgResolver(Context* context_);
    void Visit(BoundMemberExprNode& node) override;
    void Visit(BoundVariableNode& node) override;
    void Visit(BoundParentVariableNode& node) override;
    void Visit(BoundParameterNode& node) override;
    void Visit(BoundParentParameterNode& node) override;
    void Visit(BoundFunctionCallNode& node) override;
    void Visit(BoundConstructTemporaryNode& node) override;
    void Visit(BoundPtrToRefNode& node) override;
    inline BoundExpressionNode* GetFirstArg() const { return firstArg; }
private:
    Context* context;
    BoundExpressionNode* firstArg;
    otava::ast::NodeKind op;
};

FirstArgResolver::FirstArgResolver(Context* context_) : context(context_), firstArg(nullptr), op(otava::ast::NodeKind::nullNode)
{
}

void FirstArgResolver::Visit(BoundMemberExprNode& node)
{
    op = node.Op();
    node.Subject()->Accept(*this);
}

void FirstArgResolver::Visit(BoundVariableNode& node)
{
    bool hasConstDerivation = false;
    if (node.ThisPtr())
    {
        TypeSymbol* type = node.ThisPtr()->GetType();
        if (type->IsConstType())
        {
            hasConstDerivation = true;
        }
    }
    if (node.GetType()->IsReferenceType())
    {
        if (hasConstDerivation)
        {
            firstArg = new BoundRefToPtrNode(node.Clone(), node.GetFullSpan(), node.GetType()->RemoveReference(context)->AddPointer(context)->AddConst(context));
        }
        else
        {
            firstArg = new BoundRefToPtrNode(node.Clone(), node.GetFullSpan(), node.GetType()->RemoveReference(context)->AddPointer(context));
        }
    }
    else if (op == otava::ast::NodeKind::arrowNode)
    {
        firstArg = node.Clone();
    }
    else if (op == otava::ast::NodeKind::nullNode)
    {
        if (node.GetType()->IsClassTypeSymbol())
        {
            if (hasConstDerivation)
            {
                firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context)->AddConst(context));
            }
            else
            {
                firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context));
            }
        }
        else
        {
            firstArg = node.Clone();
        }
    }
    else
    {
        if (hasConstDerivation)
        {
            firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context)->AddConst(context));
        }
        else
        {
            firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context));
        }
    }
}

void FirstArgResolver::Visit(BoundParentVariableNode& node)
{
    bool hasConstDerivation = false;
    if (node.GetType()->IsReferenceType())
    {
        if (hasConstDerivation)
        {
            firstArg = new BoundRefToPtrNode(node.Clone(), node.GetFullSpan(), node.GetType()->RemoveReference(context)->AddPointer(context)->AddConst(context));
        }
        else
        {
            firstArg = new BoundRefToPtrNode(node.Clone(), node.GetFullSpan(), node.GetType()->RemoveReference(context)->AddPointer(context));
        }
    }
    else if (op == otava::ast::NodeKind::arrowNode)
    {
        firstArg = node.Clone();
    }
    else if (op == otava::ast::NodeKind::nullNode)
    {
        if (node.GetType()->IsClassTypeSymbol())
        {
            if (hasConstDerivation)
            {
                firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context)->AddConst(context));
            }
            else
            {
                firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context));
            }
        }
        else
        {
            firstArg = node.Clone();
        }
    }
    else
    {
        if (hasConstDerivation)
        {
            firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context)->AddConst(context));
        }
        else
        {
            firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context));
        }
    }
}

void FirstArgResolver::Visit(BoundParameterNode& node)
{
    if (node.GetType()->IsReferenceType())
    {
        firstArg = new BoundRefToPtrNode(node.Clone(), node.GetFullSpan(), node.GetType()->RemoveReference(context)->AddPointer(context));
    }
    else if (op == otava::ast::NodeKind::arrowNode)
    {
        firstArg = node.Clone();
    }
    else
    {
        firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context));
    }
}

void FirstArgResolver::Visit(BoundParentParameterNode& node)
{
    if (node.GetType()->IsReferenceType())
    {
        firstArg = new BoundRefToPtrNode(node.Clone(), node.GetFullSpan(), node.GetType()->RemoveReference(context)->AddPointer(context));
    }
    else if (op == otava::ast::NodeKind::arrowNode)
    {
        firstArg = node.Clone();
    }
    else
    {
        firstArg = new BoundAddressOfNode(node.Clone(), node.GetFullSpan(), node.GetType()->AddPointer(context));
    }
}

void FirstArgResolver::Visit(BoundFunctionCallNode& node)
{
    if (op == otava::ast::NodeKind::arrowNode)
    {
        firstArg = node.Clone();
    }
    else if (op == otava::ast::NodeKind::dotNode && node.GetType()->IsReferenceType())
    {
        firstArg = new BoundRefToPtrNode(node.Clone(), node.GetFullSpan(), node.GetType()->RemoveReference(context)->AddPointer(context));
    }
}

void FirstArgResolver::Visit(BoundPtrToRefNode& node)
{
    firstArg = node.Subject()->Clone();
}

void FirstArgResolver::Visit(BoundConstructTemporaryNode& node)
{
    node.Temporary()->Accept(*this);
    firstArg = new BoundExpressionSequenceNode(node.GetFullSpan(), node.Clone(), firstArg);
}

BoundExpressionNode* GetFirstArg(BoundNode* node, Context* context)
{
    FirstArgResolver resolver(context);
    node->Accept(resolver);
    return resolver.GetFirstArg();
}

class ExpressionBinder : public otava::ast::DefaultVisitor
{
public:
    ExpressionBinder(Context* context_, SymbolGroupKind symbolGroups_);
    inline std::unique_ptr<BoundExpressionNode> GetBoundExpression() { return std::move(boundExpression); }
    inline Scope* GetScope() const { return scope; }
    void Visit(otava::ast::IntegerLiteralNode& node) override;
    void Visit(otava::ast::FloatingLiteralNode& node) override;
    void Visit(otava::ast::CharacterLiteralNode& node) override;
    void Visit(otava::ast::StringLiteralNode& node) override;
    void Visit(otava::ast::RawStringLiteralNode& node) override;
    void Visit(otava::ast::BooleanLiteralNode& node) override;
    void Visit(otava::ast::NullPtrLiteralNode& node) override;
    void Visit(otava::ast::CharNode& node) override;
    void Visit(otava::ast::Char8Node& node) override;
    void Visit(otava::ast::Char16Node& node) override;
    void Visit(otava::ast::Char32Node& node) override;
    void Visit(otava::ast::WCharNode& node) override;
    void Visit(otava::ast::BoolNode& node) override;
    void Visit(otava::ast::ShortNode& node) override;
    void Visit(otava::ast::IntNode& node) override;
    void Visit(otava::ast::LongNode& node) override;
    void Visit(otava::ast::SignedNode& node) override;
    void Visit(otava::ast::UnsignedNode& node) override;
    void Visit(otava::ast::FloatNode& node) override;
    void Visit(otava::ast::DoubleNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::DestructorIdNode& node) override;
    void Visit(otava::ast::ThisNode& node) override;
    void Visit(otava::ast::TemplateIdNode& node) override;
    void Visit(otava::ast::MemberExprNode& node) override;
    void Visit(otava::ast::InvokeExprNode& node) override;
    void Visit(otava::ast::BinaryExprNode& node) override;
    void Visit(otava::ast::UnaryExprNode& node) override;
    void Visit(otava::ast::SubscriptExprNode& node) override;
    void Visit(otava::ast::PostfixIncExprNode& node) override;
    void Visit(otava::ast::PostfixDecExprNode& node) override;
    void Visit(otava::ast::CppCastExprNode& node) override;
    void Visit(otava::ast::ExpressionListNode& node) override;
    void Visit(otava::ast::SizeOfTypeExprNode& node) override;
    void Visit(otava::ast::SizeOfUnaryExprNode& node) override;
    void Visit(otava::ast::OpNewCall& node) override;
    void Visit(otava::ast::NewExprNode& node) override;
    void Visit(otava::ast::DeletePtrNode& node) override;
    void Visit(otava::ast::BracedInitListNode& node) override;
    void Visit(otava::ast::ThrowExprNode& node) override;
    void Visit(otava::ast::ConditionalExprNode& node) override;
    void Visit(otava::ast::OperatorFunctionIdNode& node) override;
    void Visit(otava::ast::InitConditionNode& node) override;
private:
    void BindBinaryOp(otava::ast::NodeKind op, const soul::ast::FullSpan& fullSpan,
        std::unique_ptr<BoundExpressionNode>&& left, std::unique_ptr<BoundExpressionNode>&& right, otava::ast::Node* node);
    void BindUnaryOp(otava::ast::NodeKind op, const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand, otava::ast::Node* node);
    void BindClassArrow(std::unique_ptr<BoundExpressionNode>&& operand, otava::ast::MemberExprNode* memberExprNode);
    void BindMemberExpr(otava::ast::MemberExprNode* node, std::unique_ptr<BoundExpressionNode>&& subject);
    void BindDeref(const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand);
    void BindAddrOf(const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand);
    void BindPrefixInc(const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand, otava::ast::Node* child);
    void BindPrefixDec(const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand, otava::ast::Node* child);
    void BindDynamicCast(otava::ast::CppCastExprNode& node);
    Context* context;
    std::unique_ptr<BoundExpressionNode> boundExpression;
    Scope* scope;
    SymbolGroupKind symbolGroups;
    bool inhibitCompile;
    bool emptyDestructor;
    bool qualifiedScope;
    bool callOperatorFunction;
    otava::ast::NodeKind operatorFnNodeKind;
};

ExpressionBinder::ExpressionBinder(Context* context_, SymbolGroupKind symbolGroups_) :
    context(context_),
    boundExpression(),
    scope(context->GetSymbolTable()->CurrentScope()),
    symbolGroups(symbolGroups_),
    inhibitCompile(false),
    emptyDestructor(false),
    qualifiedScope(false),
    callOperatorFunction(false),
    operatorFnNodeKind(otava::ast::NodeKind::nullNode)
{
}

void ExpressionBinder::BindBinaryOp(otava::ast::NodeKind op, const soul::ast::FullSpan& fullSpan,
    std::unique_ptr<BoundExpressionNode>&& left, std::unique_ptr<BoundExpressionNode>&& right, otava::ast::Node* node)
{
    if (!left)
    {
        ThrowException("left expression is null", fullSpan, context);
    }
    if (!left->GetType())
    {
        ThrowException("left type is null", left->GetFullSpan(), context);
    }
    std::unique_ptr<BoundExpressionNode> l(std::move(left));
    std::unique_ptr<BoundExpressionNode> r(std::move(right));
    bool isClassType = l->GetType()->IsClassTypeSymbol();
    if (!isClassType)
    {
        switch (op)
        {
        case otava::ast::NodeKind::plusAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::plusNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        case otava::ast::NodeKind::minusAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::minusNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        case otava::ast::NodeKind::mulAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::mulNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        case otava::ast::NodeKind::divAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::divNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        case otava::ast::NodeKind::modAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::modNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        case otava::ast::NodeKind::andAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::andNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        case otava::ast::NodeKind::orAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::inclusiveOrNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        case otava::ast::NodeKind::xorAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::exclusiveOrNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        case otava::ast::NodeKind::shiftLeftAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::shiftLeftNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        case otava::ast::NodeKind::shiftRightAssignNode:
        {
            std::unique_ptr<BoundExpressionNode> clone(l->Clone());
            BindBinaryOp(otava::ast::NodeKind::shiftRightNode, fullSpan, std::move(l), std::move(r), nullptr);
            BindBinaryOp(otava::ast::NodeKind::assignNode, fullSpan, std::move(clone), std::move(boundExpression), nullptr);
            return;
        }
        }
    }
    std::string groupName = OperatorGroupNameMap::Instance().GetGroupName(op, fullSpan, context);
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    args.push_back(std::unique_ptr<BoundExpressionNode>(l->Clone()));
    args.push_back(std::unique_ptr<BoundExpressionNode>(r->Clone()));
    std::unique_ptr<BoundFunctionCallNode> functionCall;
    Exception ex1;
    FunctionMatch match1;
    std::vector<TypeSymbol*> templateArgs;
    if (node)
    {
        context->PushNodeId(node->Id());
    }
    std::unique_ptr<BoundFunctionCallNode> functionCall1 = ResolveOverload(scope, groupName, templateArgs, args, fullSpan, context, ex1, match1);
    if (node)
    {
        context->PopNodeId();
    }
    Exception ex2;
    FunctionMatch match2;
    TypeSymbol* type = l->GetType()->AddPointer(context);
    std::vector<std::unique_ptr<BoundExpressionNode>> args2;
    args2.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(l.release(), fullSpan, type)));
    args2.push_back(std::unique_ptr<BoundExpressionNode>(r.release()));
    context->PushSetFlag(ContextFlags::noPtrOps | ContextFlags::skipFirstPtrToBooleanConversion);
    if (node)
    {
        context->PushNodeId(node->Id());
    }
    std::unique_ptr<BoundFunctionCallNode> functionCall2 = ResolveOverload(scope, groupName, templateArgs, args2, fullSpan, context, ex2, match2);
    if (node)
    {
        context->PopNodeId();
    }
    context->PopFlags();
    if (functionCall1 && !functionCall2)
    {
        if (ex1.Warning())
        {
            PrintWarning(ex1, context);
        }
        functionCall.reset(functionCall1.release());
    }
    else if (functionCall2 && !functionCall1)
    {
        if (ex2.Warning())
        {
            PrintWarning(ex2, context);
        }
        functionCall.reset(functionCall2.release());
    }
    else if (functionCall1 && functionCall2)
    {
        if (BetterFunctionMatch(context)(match1, match2))
        {
            if (ex1.Warning())
            {
                PrintWarning(ex1, context);
            }
            functionCall.reset(functionCall1.release());
        }
        else if (BetterFunctionMatch(context)(match2, match1))
        {
            if (ex2.Warning())
            {
                PrintWarning(ex2, context);
            }
            functionCall.reset(functionCall2.release());
        }
        else
        {
            if (ex1.Warning())
            {
                PrintWarning(ex1, context);
            }
            functionCall.reset(functionCall1.release());
        }
    }
    else
    {
        Exception ex(std::string(ex1.what()) + "\n" + std::string(ex2.what()));
        ThrowException(ex);
    }
    if (node)
    {
        functionCall->SetSource(node->Clone());
    }
    FunctionSymbol* functionSymbol = functionCall->GetFunctionSymbol();
    if (functionSymbol->IsVirtual())
    {
        functionCall->SetFlag(BoundExpressionFlags::virtualCall);
        context->GetBoundCompileUnit()->AddBoundNodeForClass(functionSymbol->ParentClassType(context), fullSpan, context);
    }
    VariableSymbol* classTemporary = nullptr;
    bool foundFromParent = false;
    int level = 0;
    BoundExpressionNode* temporaryDestructorCallArg = nullptr;
    if (functionSymbol->ReturnsClass())
    {
        bool invoke = context->GetFlag(ContextFlags::invoke);
        if (invoke)
        {
            std::pair<VariableSymbol*, int> temporaryLevel = GetParentTemporary(node->Id(), context);
            classTemporary = temporaryLevel.first;
            level = temporaryLevel.second;
            if (classTemporary)
            {
                BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context));
                boundParentVariable->SetLevel(level);
                functionCall->AddArgument(new BoundAddressOfNode(boundParentVariable, fullSpan, classTemporary->GetType(context)->AddPointer(context)));
                temporaryDestructorCallArg = functionCall->Args().back().get();
                foundFromParent = true;
            }
            else
            {
                ThrowException("parent temporary not found from child", node->GetFullSpan(), context);
            }
        }
        else
        {
            classTemporary = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(functionSymbol->ReturnType(context), node->Id(), context);
            functionCall->AddArgument(new BoundAddressOfNode(
                new BoundVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context)), fullSpan, 
                classTemporary->GetType(context)->AddPointer(context)));
            temporaryDestructorCallArg = functionCall->Args().back().get();
        }
    }
    boundExpression.reset(functionCall.release());
    if (classTemporary)
    {
        if (foundFromParent)
        {
            BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context));
            boundParentVariable->SetLevel(level);
            BoundConstructTemporaryNode* ctn = new BoundConstructTemporaryNode(boundExpression.release(), boundParentVariable, fullSpan);
            CheckGenerateTemporaryDestructorCall(ctn, temporaryDestructorCallArg, context);
            boundExpression.reset(ctn);
            boundExpression->SetFlag(BoundExpressionFlags::bindToRvalueRef);
        }
        else
        {
            BoundConstructTemporaryNode* ctn = new BoundConstructTemporaryNode(boundExpression.release(), 
                new BoundVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context)), fullSpan);
            CheckGenerateTemporaryDestructorCall(ctn, temporaryDestructorCallArg, context);
            boundExpression.reset(ctn);
            boundExpression->SetFlag(BoundExpressionFlags::bindToRvalueRef);
        }
    }
}

void ExpressionBinder::BindUnaryOp(otava::ast::NodeKind op, const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand, otava::ast::Node* node)
{
    std::string groupName = OperatorGroupNameMap::Instance().GetGroupName(op, fullSpan, context);
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    args.push_back(std::move(operand));
    Exception ex1;
    Exception ex2;
    std::vector<TypeSymbol*> templateArgs;
    if (node)
    {
        context->PushNodeId(node->Id());
    }
    std::unique_ptr<BoundFunctionCallNode> functionCall = ResolveOverload(scope, groupName, templateArgs, args, fullSpan, context, ex1);
    if (node)
    {
        context->PopNodeId();
    }
    if (functionCall)
    {
        if (ex1.Warning())
        {
            PrintWarning(ex1, context);
        }
    }
    if (!functionCall)
    {
        context->PushSetFlag(ContextFlags::skipFirstPtrToBooleanConversion);
        TypeSymbol* type = args[0]->GetType()->AddPointer(context);
        args[0].reset(new BoundAddressOfNode(args[0].release(), fullSpan, type));
        if (node)
        {
            context->PushNodeId(node->Id());
        }
        functionCall = ResolveOverload(scope, groupName, templateArgs, args, fullSpan, context, ex2);
        if (node)
        {
            context->PopNodeId();
        }
        context->PopFlags();
        if (functionCall)
        {
            if (ex2.Warning())
            {
                PrintWarning(ex2, context);
            }
        }
    }
    if (!functionCall)
    {
        Exception ex(std::string(ex1.what()) + "\n" + std::string(ex2.what()));
        ThrowException(ex);
    }
    if (node)
    {
        functionCall->SetSource(node->Clone());
    }
    FunctionSymbol* functionSymbol = functionCall->GetFunctionSymbol();
    if (functionSymbol->IsVirtual())
    {
        functionCall->SetFlag(BoundExpressionFlags::virtualCall);
        context->GetBoundCompileUnit()->AddBoundNodeForClass(functionSymbol->ParentClassType(context), fullSpan, context);
    }
    VariableSymbol* classTemporary = nullptr;
    BoundExpressionNode* temporaryDestructorCallArg = nullptr;
    int level = 0;
    bool foundFromParent = false;
    if (functionSymbol->ReturnsClass())
    {
        bool invoke = context->GetFlag(ContextFlags::invoke);
        if (invoke)
        {
            std::pair<VariableSymbol*, int> temporaryLevel = GetParentTemporary(node->Id(), context);
            classTemporary = temporaryLevel.first;
            level = temporaryLevel.second;
            if (classTemporary)
            {
                BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context));
                boundParentVariable->SetLevel(level);
                functionCall->AddArgument(new BoundAddressOfNode(boundParentVariable, fullSpan, classTemporary->GetType(context)->AddPointer(context)));
                temporaryDestructorCallArg = functionCall->Args().back().get();
                foundFromParent = true;
            }
            else
            {
                ThrowException("parent temporary not found from child", node->GetFullSpan(), context);
            }
        }
        else
        {
            classTemporary = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(functionSymbol->ReturnType(context), node->Id(), context);
            functionCall->AddArgument(new BoundAddressOfNode(
                new BoundVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context)), fullSpan, 
                classTemporary->GetType(context)->AddPointer(context)));
            temporaryDestructorCallArg = functionCall->Args().back().get();
        }
    }
    boundExpression.reset(functionCall.release());
    if (classTemporary)
    {
        if (foundFromParent)
        {
            BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context));
            boundParentVariable->SetLevel(level);
            BoundConstructTemporaryNode* ctn = new BoundConstructTemporaryNode(boundExpression.release(), boundParentVariable, fullSpan);
            CheckGenerateTemporaryDestructorCall(ctn, temporaryDestructorCallArg, context);
            boundExpression.reset(ctn);
            boundExpression->SetFlag(BoundExpressionFlags::bindToRvalueRef);
        }
        else
        {
            BoundConstructTemporaryNode* ctn = new BoundConstructTemporaryNode(boundExpression.release(), 
                new BoundVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context)), fullSpan);
            CheckGenerateTemporaryDestructorCall(ctn, temporaryDestructorCallArg, context);
            boundExpression.reset(ctn);
            boundExpression->SetFlag(BoundExpressionFlags::bindToRvalueRef);
        }
    }
}

void ExpressionBinder::BindClassArrow(std::unique_ptr<BoundExpressionNode>&& operand, otava::ast::MemberExprNode* memberExprNode)
{
    if (operand->GetType()->IsReferenceType())
    {
        TypeSymbol* type = operand->GetType()->RemoveReference(context)->AddPointer(context);
        operand.reset(new BoundRefToPtrNode(operand.release(), memberExprNode->GetFullSpan(), type));
    }
    else
    {
        TypeSymbol* type = operand->GetType()->AddPointer(context);
        operand.reset(new BoundAddressOfNode(operand.release(), memberExprNode->GetFullSpan(), type));
    }
    BindUnaryOp(otava::ast::NodeKind::arrowNode, memberExprNode->GetFullSpan(), std::move(operand), nullptr);
    while (boundExpression->GetType()->IsClassTypeSymbol())
    {
        BindUnaryOp(otava::ast::NodeKind::arrowNode, memberExprNode->GetFullSpan(), std::move(boundExpression), nullptr);
    }
    BindMemberExpr(memberExprNode, std::move(boundExpression));
}

void ExpressionBinder::BindDeref(const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand)
{
    if (operand->GetType()->IsPointerType())
    {
        if (operand->GetType()->RemovePointer(context)->IsClassTypeSymbol())
        {
            TypeSymbol* type = operand->GetType()->RemovePointer(context)->AddLValueRef(context);
            boundExpression.reset(new BoundPtrToRefNode(operand.release(), fullSpan, type));
        }
        else if (context->GetFlag(ContextFlags::returnRef))
        {
            TypeSymbol* type = operand->GetType()->RemovePointer(context)->AddLValueRef(context);
            boundExpression.reset(new BoundPtrToRefNode(operand.release(), fullSpan, type));
        }
        else
        {
            if (operand->IsBoundConversionNode())
            {
                TypeSymbol* type = operand->GetType()->RemovePointer(context);
                boundExpression.reset(new BoundDereferenceNode(operand.release(), fullSpan, type, OperationFlags::derefAfterConv));
            }
            else
            {
                TypeSymbol* type = operand->GetType()->RemovePointer(context);
                boundExpression.reset(new BoundDereferenceNode(operand.release(), fullSpan, type));
            }
        }
    }
    else
    {
        TypeSymbol* plainSubjectType = operand->GetType()->PlainType(context);
        if (plainSubjectType->IsClassTypeSymbol())
        {
            if (operand->GetType()->IsReferenceType())
            {
                TypeSymbol* type = operand->GetType()->RemoveReference(context)->AddPointer(context);
                boundExpression.reset(new BoundRefToPtrNode(operand.release(), fullSpan, type));
            }
            else
            {
                TypeSymbol* type = operand->GetType()->AddPointer(context);
                boundExpression.reset(new BoundAddressOfNode(operand.release(), fullSpan, type));
            }
            BindUnaryOp(otava::ast::NodeKind::derefNode, fullSpan, std::move(boundExpression), nullptr);
        }
        else
        {
            ThrowException("dereference needs pointer or class type argument", fullSpan, context);
        }
    }
}

void ExpressionBinder::BindAddrOf(const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand)
{
    if (operand->IsLvalueExpression(context))
    {
        if (operand->GetType()->IsReferenceType())
        {
            TypeSymbol* type = operand->GetType()->RemoveReference(context)->AddPointer(context);
            boundExpression.reset(new BoundRefToPtrNode(operand.release(), fullSpan, type));
        }
        else
        {
            TypeSymbol* type = operand->GetType()->AddPointer(context);
            boundExpression.reset(new BoundAddressOfNode(operand.release(), fullSpan, type));
        }
    }
    else
    {
        ThrowException("cannot take address of this kind of expression", fullSpan, context);
    }
}

void ExpressionBinder::BindPrefixInc(const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand, otava::ast::Node* child)
{
    if (operand->GetType()->PlainType(context)->IsClassTypeSymbol())
    {
        if (operand->GetType()->IsReferenceType())
        {
            TypeSymbol* type = operand->GetType()->RemoveReference(context)->AddPointer(context);
            boundExpression.reset(new BoundRefToPtrNode(operand.release(), fullSpan, type));
        }
        else
        {
            TypeSymbol* type = operand->GetType()->AddPointer(context);
            boundExpression.reset(new BoundAddressOfNode(operand.release(), fullSpan, type));
        }
        BindUnaryOp(otava::ast::NodeKind::prefixIncNode, fullSpan, std::move(boundExpression), nullptr);
    }
    else
    {
        otava::ast::BinaryExprNode assignmentExpr(fullSpan.span, fullSpan.fileIndex,
            new otava::ast::AssignNode(fullSpan.span, fullSpan.fileIndex),
            child->Clone(),
            new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex, new otava::ast::PlusNode(fullSpan.span, fullSpan.fileIndex), child->Clone(),
                new otava::ast::IntegerLiteralNode(fullSpan.span, fullSpan.fileIndex, 1, otava::ast::Suffix::none, otava::ast::Base::decimal, std::string())));
        context->PushSetFlag(ContextFlags::suppress_warning);
        boundExpression = BindExpression(&assignmentExpr, context);
        context->PopFlags();
    }
}

void ExpressionBinder::BindPrefixDec(const soul::ast::FullSpan& fullSpan, std::unique_ptr<BoundExpressionNode>&& operand, otava::ast::Node* child)
{
    if (operand->GetType()->PlainType(context)->IsClassTypeSymbol())
    {
        if (operand->GetType()->IsReferenceType())
        {
            TypeSymbol* type = operand->GetType()->RemoveReference(context)->AddPointer(context);
            boundExpression.reset(new BoundRefToPtrNode(operand.release(), fullSpan, type));
        }
        else
        {
            TypeSymbol* type = operand->GetType()->AddPointer(context);
            boundExpression.reset(new BoundAddressOfNode(operand.release(), fullSpan, type));
        }
        BindUnaryOp(otava::ast::NodeKind::prefixDecNode, fullSpan, std::move(boundExpression), nullptr);
    }
    else
    {
        otava::ast::BinaryExprNode assignmentExpr(fullSpan.span, fullSpan.fileIndex,
            new otava::ast::AssignNode(fullSpan.span, fullSpan.fileIndex),
            child->Clone(),
            new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex, new otava::ast::MinusNode(fullSpan.span, fullSpan.fileIndex), child->Clone(),
                new otava::ast::IntegerLiteralNode(fullSpan.span, fullSpan.fileIndex, 1, otava::ast::Suffix::none, otava::ast::Base::decimal, std::string())));
        context->PushSetFlag(ContextFlags::suppress_warning);
        boundExpression = BindExpression(&assignmentExpr, context);
        context->PopFlags();
    }
}

void ExpressionBinder::BindDynamicCast(otava::ast::CppCastExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    TypeSymbol* resultType = ResolveType(node.TypeId(), DeclarationFlags::none, context);
    resultType = resultType->DirectType(context)->FinalType(fullSpan, context);
    node.Child()->Accept(*this);
    TypeSymbol* argumentType = boundExpression->GetType()->DirectType(context)->FinalType(fullSpan, context);
    FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetDynamicPtrCastFn(argumentType, resultType,
        fullSpan, context);
    TypeSymbol* type = conversion->ReturnType(context);
    boundExpression.reset(new BoundConversionNode(boundExpression.release(), conversion, fullSpan, type));
}

void ExpressionBinder::Visit(otava::ast::CppCastExprNode& node)
{
    if (node.Op()->Kind() == otava::ast::NodeKind::dynamicCastNode)
    {
        BindDynamicCast(node);
        return;
    }
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    TypeSymbol* resultType = ResolveType(node.TypeId(), DeclarationFlags::none, context);
    resultType = resultType->DirectType(context)->FinalType(fullSpan, context);
    node.Child()->Accept(*this);
    if (!boundExpression)
    {
        ThrowException("invalid cast", fullSpan, context);
    }
    bool reinterpretCast = false;
    if (node.Op()->Kind() == otava::ast::NodeKind::reinterpretCastNode)
    {
        reinterpretCast = true;
        context->PushSetFlag(ContextFlags::reinterpretCast);
    }
    ArgumentMatch argumentMatch;
    FunctionMatch functionMatch;
    context->PushSetFlag(ContextFlags::cast);
    TypeSymbol* type = boundExpression->GetType()->DirectType(context)->FinalType(fullSpan, context);
    FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(resultType, type,
        boundExpression.get(), fullSpan, argumentMatch, functionMatch, context);
    context->PopFlags();
    if (reinterpretCast)
    {
        context->PopFlags();
    }
    if (conversion)
    {
        if (argumentMatch.preConversionFlags == OperationFlags::addr)
        {
            TypeSymbol* type = boundExpression->GetType()->AddPointer(context);
            boundExpression.reset(new BoundAddressOfNode(
                MakeLvalueExpression(boundExpression.release(), fullSpan, context), fullSpan, type));
        }
        else if (argumentMatch.preConversionFlags == OperationFlags::deref)
        {
            TypeSymbol* type = boundExpression->GetType()->RemoveReference(context);
            boundExpression.reset(new BoundDereferenceNode(boundExpression.release(), fullSpan, type));
        }
        TypeSymbol* type = conversion->ReturnType(context);
        boundExpression.reset(new BoundConversionNode(boundExpression.release(), conversion, fullSpan, type));
    }
    else
    {
        ThrowException("no conversion found", fullSpan, context);
    }
}

otava::ast::Node* MakeTypeNameNodes(const soul::ast::FullSpan& fullSpan, const std::string& fullTypeName)
{
    otava::ast::Node* node = nullptr;
    std::vector<std::string> components = util::Split(fullTypeName, std::string("::"));
    int n = components.size();
    for (int i = n - 1; i >= 0; --i)
    {
        otava::ast::Node* identifierNode = new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, components[i]);
        if (node)
        {
            otava::ast::Node* nnsNode = new otava::ast::NestedNameSpecifierNode(fullSpan.span, fullSpan.fileIndex);
            nnsNode->AddNode(identifierNode);
            nnsNode->AddNode(new otava::ast::ColonColonNode(fullSpan.span, fullSpan.fileIndex));
            node = new otava::ast::QualifiedIdNode(fullSpan.span, fullSpan.fileIndex, nnsNode, node);
        }
        else
        {
            node = identifierNode;
        }
    }
    return node;
}

class NewInitializerMaker : public otava::ast::DefaultVisitor
{
public:
    NewInitializerMaker(const soul::ast::FullSpan& fullSpan);
    void Visit(otava::ast::InvokeExprNode& node) override;
    otava::ast::NewInitializerNode* GetNewInitializer() const { return newInitializer; }
private:
    otava::ast::NewInitializerNode* newInitializer;
};

NewInitializerMaker::NewInitializerMaker(const soul::ast::FullSpan& fullSpan) : newInitializer(new otava::ast::NewInitializerNode(fullSpan.span, fullSpan.fileIndex))
{
}

void NewInitializerMaker::Visit(otava::ast::InvokeExprNode& node)
{
    int n = node.Items().size();
    for (int i = 0; i < n; ++i)
    {
        otava::ast::Node* arg = node.Items()[i]->Clone();
        newInitializer->AddNode(arg);
    }
}

void ExpressionBinder::Visit(otava::ast::IntegerLiteralNode& node)
{
    Value* value = Evaluate(&node, context);
    boundExpression.reset(new BoundLiteralNode(value, node.GetFullSpan(), value->GetType(context)));
}

void ExpressionBinder::Visit(otava::ast::FloatingLiteralNode& node)
{
    Value* value = Evaluate(&node, context);
    boundExpression.reset(new BoundLiteralNode(value, node.GetFullSpan(), value->GetType(context)));
}

void ExpressionBinder::Visit(otava::ast::CharacterLiteralNode& node)
{
    Value* value = Evaluate(&node, context);
    boundExpression.reset(new BoundLiteralNode(value, node.GetFullSpan(), value->GetType(context)));
}

void ExpressionBinder::Visit(otava::ast::StringLiteralNode& node)
{
    Value* value = Evaluate(&node, context);
    boundExpression.reset(new BoundStringLiteralNode(value, node.GetFullSpan(), value->GetType(context)));
}

void ExpressionBinder::Visit(otava::ast::RawStringLiteralNode& node)
{
    Value* value = Evaluate(&node, context);
    boundExpression.reset(new BoundLiteralNode(value, node.GetFullSpan(), value->GetType(context)));
}

void ExpressionBinder::Visit(otava::ast::BooleanLiteralNode& node)
{
    Value* value = Evaluate(&node, context);
    boundExpression.reset(new BoundLiteralNode(value, node.GetFullSpan(), value->GetType(context)));
}

void ExpressionBinder::Visit(otava::ast::NullPtrLiteralNode& node)
{
    Value* value = Evaluate(&node, context);
    boundExpression.reset(new BoundLiteralNode(value, node.GetFullSpan(), value->GetType(context)));
}

void ExpressionBinder::Visit(otava::ast::CharNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::charType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::Char8Node& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::char8Type, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::Char16Node& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::char16Type, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::Char32Node& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::char32Type, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::WCharNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::wcharType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::BoolNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::boolType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::ShortNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::shortIntType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::IntNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::intType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::LongNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::longIntType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::SignedNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::intType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::UnsignedNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::unsignedIntType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::FloatNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::floatType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::DoubleNode& node)
{
    boundExpression.reset(new BoundTypeNode(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::doubleType, context), node.GetFullSpan()));
}

void ExpressionBinder::Visit(otava::ast::IdentifierNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    bool invokeOrTryCatch = context->GetFlag(ContextFlags::invoke | ContextFlags::tryCatch);
    bool foundFromParentFn = false;
    bool lookupOnlyFromMemberScope = context->GetFlag(ContextFlags::lookupOnlyFromMemberScope);
    int level = 0;
    SymbolGroupKind groups = symbolGroups;
    if ((groups & SymbolGroupKind::functionSymbolGroup) != SymbolGroupKind::none)
    {
        groups = groups & ~SymbolGroupKind::functionSymbolGroup;
    }
    Symbol* symbol = nullptr;
    if (qualifiedScope || lookupOnlyFromMemberScope)
    {
        symbol = scope->Lookup(node.Str(), groups, ScopeLookup::thisAndBaseScopes, fullSpan, context, LookupFlags::dontResolveSingle);
    }
    if (!symbol)
    {
        symbol = scope->Lookup(node.Str(), groups, ScopeLookup::allScopes, fullSpan, context, LookupFlags::dontResolveSingle);
    }
    if (!symbol)
    {
        std::set<Scope*> visited;
        ClassTemplateSpecializationSymbol* sp = static_cast<ClassTemplateSpecializationSymbol*>(scope->GetClassTemplateSpecialization(visited));
        if (sp)
        {
            symbol = sp->ClassTemplate(context)->GetScope()->Lookup(node.Str(), groups, ScopeLookup::allScopes, fullSpan, context, LookupFlags::dontResolveSingle);
        }
    }
    if (!symbol && boundExpression && boundExpression->IsBoundTypeNode())
    {
        TypeSymbol* type = static_cast<TypeSymbol*>(boundExpression->GetType());
        symbol = type->GetScope()->Lookup(node.Str(), groups, ScopeLookup::allScopes, fullSpan, context, LookupFlags::dontResolveSingle);
    }
    if (!symbol && (symbolGroups & SymbolGroupKind::functionSymbolGroup) != SymbolGroupKind::none)
    {
        groups = SymbolGroupKind::functionSymbolGroup;
        if (qualifiedScope)
        {
            symbol = scope->Lookup(node.Str(), groups, ScopeLookup::thisAndBaseScopes, fullSpan, context, LookupFlags::dontResolveSingle);
        }
        if (!symbol)
        {
            symbol = scope->Lookup(node.Str(), groups, ScopeLookup::allScopes, fullSpan, context, LookupFlags::dontResolveSingle);
        }
        if (!symbol)
        {
            std::set<Scope*> visited;
            ClassTemplateSpecializationSymbol* sp = static_cast<ClassTemplateSpecializationSymbol*>(scope->GetClassTemplateSpecialization(visited));
            if (sp)
            {
                symbol = sp->ClassTemplate(context)->GetScope()->Lookup(node.Str(), groups, ScopeLookup::allScopes, fullSpan, context, LookupFlags::dontResolveSingle);
            }
        }
        if (!symbol && boundExpression && boundExpression->IsBoundTypeNode())
        {
            TypeSymbol* type = static_cast<TypeSymbol*>(boundExpression->GetType());
            symbol = type->GetScope()->Lookup(node.Str(), groups, ScopeLookup::allScopes, fullSpan, context, LookupFlags::dontResolveSingle);
        }
    }
    Symbol* nsSymbol = nullptr;
    if (symbol && symbol->IsNamespaceSymbol())
    {
        nsSymbol = symbol;
        symbol = nullptr;
    }
    Symbol* variableGroupSymbol = nullptr;
    if (invokeOrTryCatch)
    {
        if (symbol && symbol->IsVariableGroupSymbol() && !lookupOnlyFromMemberScope)
        {
            VariableGroupSymbol* variableGroup = static_cast<VariableGroupSymbol*>(symbol);
            Symbol* sym = variableGroup->GetSingleSymbol(context);
            if (sym && sym->IsVariableSymbol())
            {
                VariableSymbol* variable = static_cast<VariableSymbol*>(sym);
                variable = variable->Final();
                Scope* scope = variable->Parent(context)->GetScope();
                if (scope && scope->IsContainerScope())
                {
                    ContainerScope* containerScope = static_cast<ContainerScope*>(scope);
                    ContainerSymbol* containerSymbol = containerScope->GetContainerSymbol();
                    if (containerSymbol && containerSymbol->IsClassTypeSymbol())
                    {
                        variableGroupSymbol = symbol;
                        symbol = nullptr;
                    }
                }
            }
        }
        if (!symbol && !lookupOnlyFromMemberScope)
        {
            StatementBinder* statementBinder = context->GetStatementBinder();
            if (statementBinder)
            {
                FunctionDefinitionSymbol* fnDefSymbol = statementBinder->GetFunctionDefinitionSymbol();
                if (fnDefSymbol)
                {
                    while (fnDefSymbol)
                    {
                        Scope* parentFnScope = fnDefSymbol->ParentFnScope();
                        if (parentFnScope)
                        {
                            symbol = parentFnScope->Lookup(node.Str(), SymbolGroupKind::variableSymbolGroup, ScopeLookup::thisAndBaseAndParentScope, fullSpan,
                                context, LookupFlags::dontResolveSingle);
                            if (symbol)
                            {
                                foundFromParentFn = true;
                                break;
                            }
                            else
                            {
                                fnDefSymbol = fnDefSymbol->ParentFn();
                                ++level;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            if (!symbol)
            {
                level = 0;
                StatementBinder* parentStatementBinder = context->GetParentStatementBinder();
                if (parentStatementBinder)
                {
                    FunctionDefinitionSymbol* fnDefSymbol = parentStatementBinder->GetFunctionDefinitionSymbol();
                    if (fnDefSymbol)
                    {
                        while (fnDefSymbol)
                        {
                            int currentBlockId = context->CurrentBlockId();
                            Symbol* block = fnDefSymbol->GetBlock(currentBlockId);
                            if (block)
                            {
                                Scope* blockScope = block->GetScope();
                                if (blockScope)
                                {
                                    symbol = blockScope->Lookup(node.Str(), SymbolGroupKind::variableSymbolGroup,
                                        ScopeLookup::thisAndBaseAndParentScope, fullSpan, context, LookupFlags::dontResolveSingle);
                                    if (symbol)
                                    {
                                        foundFromParentFn = true;
                                        break;
                                    }
                                    else
                                    {
                                        fnDefSymbol = fnDefSymbol->ParentFn();
                                        ++level;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (!symbol && !lookupOnlyFromMemberScope)
        {
            level = 0;
            StatementBinder* statementBinder = context->GetStatementBinder();
            if (statementBinder)
            {
                FunctionDefinitionSymbol* fnDefSymbol = statementBinder->GetFunctionDefinitionSymbol();
                if (fnDefSymbol)
                {
                    while (fnDefSymbol)
                    {
                        Scope* parentFnScope = fnDefSymbol->ParentFnScope();
                        if (parentFnScope)
                        {
                            symbol = parentFnScope->Lookup(node.Str(), SymbolGroupKind::functionSymbolGroup, ScopeLookup::thisAndBaseAndParentScope, fullSpan,
                                context, LookupFlags::dontResolveSingle);
                            if (symbol)
                            {
                                foundFromParentFn = true;
                                break;
                            }
                            else
                            {
                                fnDefSymbol = fnDefSymbol->ParentFn();
                                ++level;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            if (!symbol)
            {
                level = 0;
                StatementBinder* parentStatementBinder = context->GetParentStatementBinder();
                if (parentStatementBinder)
                {
                    FunctionDefinitionSymbol* fnDefSymbol = parentStatementBinder->GetFunctionDefinitionSymbol();
                    if (fnDefSymbol)
                    {
                        while (fnDefSymbol)
                        {
                            int currentBlockId = context->CurrentBlockId();
                            Symbol* block = fnDefSymbol->GetBlock(currentBlockId);
                            if (block)
                            {
                                Scope* blockScope = block->GetScope();
                                if (blockScope)
                                {
                                    symbol = blockScope->Lookup(node.Str(), SymbolGroupKind::functionSymbolGroup,
                                        ScopeLookup::thisAndBaseAndParentScope, fullSpan, context, LookupFlags::dontResolveSingle);
                                    if (symbol)
                                    {
                                        foundFromParentFn = true;
                                        break;
                                    }
                                    else
                                    {
                                        fnDefSymbol = fnDefSymbol->ParentFn();
                                        ++level;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if (!symbol && nsSymbol)
    {
        symbol = nsSymbol;
    }
    if (!symbol && variableGroupSymbol)
    {
        symbol = variableGroupSymbol;
    }
    if (symbol)
    {
        switch (symbol->Kind())
        {
        case SymbolKind::variableGroupSymbol:
        {
            VariableGroupSymbol* variableGroup = static_cast<VariableGroupSymbol*>(symbol);
            Symbol* sym = variableGroup->GetSingleSymbol(context);
            if (sym && sym->IsVariableSymbol())
            {
                VariableSymbol* variable = static_cast<VariableSymbol*>(sym);
                variable = variable->Final();
                if (foundFromParentFn)
                {
                    BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(variable, fullSpan, variable->GetReferredType(context));
                    boundParentVariable->SetLevel(level);
                    if (variable->IsMemberVariable(context))
                    {
                        boundParentVariable->SetThisPtr(context->GetThisPtr(fullSpan));
                    }
                    boundExpression.reset(boundParentVariable);
                }
                else
                {
                    BoundVariableNode* boundVariable = new BoundVariableNode(variable, fullSpan, variable->GetReferredType(context));
                    if (variable->IsMemberVariable(context))
                    {
                        boundVariable->SetThisPtr(context->GetThisPtr(fullSpan));
                    }
                    boundExpression.reset(boundVariable);
                }
            }
            else
            {
                if (variableGroup->Variables(context).empty())
                {
                    ThrowException("variable '" + variableGroup->Name() + "' not found", fullSpan, context);
                }
                else
                {
                    ThrowException("ambiguous reference to variable '" + variableGroup->Name() + "'", fullSpan, context);
                }
            }
            break;
        }
        case SymbolKind::classGroupSymbol:
        {
            ClassGroupSymbol* classGroup = static_cast<ClassGroupSymbol*>(symbol);
            Symbol* sym = classGroup->GetSingleSymbol(context);
            if (sym && sym->IsClassTypeSymbol())
            {
                ClassTypeSymbol* cls = static_cast<ClassTypeSymbol*>(sym);
                boundExpression.reset(new BoundTypeNode(cls, fullSpan));
            }
            else if (sym && sym == classGroup)
            {
                ClassGroupSymbol* classGroupSymbol = static_cast<ClassGroupSymbol*>(symbol);
                ClassGroupTypeSymbol* classGroupType = context->GetSymbolTable()->MakeClassGroupTypeSymbol(classGroupSymbol, context);
                boundExpression.reset(new BoundClassGroupNode(classGroupSymbol, fullSpan, classGroupType));
            }
            else
            {
                ThrowException("ambiguous reference to class '" + classGroup->Name() + "'", fullSpan, context);
            }
            break;
        }
        case SymbolKind::templateParameterSymbol:
        {
            TypeSymbol* type = static_cast<TypeSymbol*>(symbol);
            boundExpression.reset(new BoundTypeNode(type, fullSpan));
            break;
        }
        case SymbolKind::aliasGroupSymbol:
        {
            AliasGroupSymbol* aliasGroup = static_cast<AliasGroupSymbol*>(symbol);
            Symbol* sym = aliasGroup->GetSingleSymbol(context);
            if (sym && sym->IsAliasTypeSymbol())
            {
                AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(sym);
                TypeSymbol* referredType = aliasType->ReferredType(context);
                while (referredType->IsAliasTypeSymbol())
                {
                    aliasType = static_cast<AliasTypeSymbol*>(referredType);
                    referredType = aliasType->ReferredType(context);
                }
                boundExpression.reset(new BoundTypeNode(referredType, fullSpan));
            }
            else if (sym && sym == aliasGroup)
            {
                AliasGroupTypeSymbol* aliasGroupType = context->GetSymbolTable()->MakeAliasGroupTypeSymbol(aliasGroup, context);
                boundExpression.reset(new BoundAliasGroupNode(aliasGroup, fullSpan, aliasGroupType));
            }
            else
            {
                ThrowException("ambiguous reference to type alias '" + aliasGroup->Name() + "'", fullSpan, context);
            }
            break;
        }
        case SymbolKind::enumGroupSymbol:
        {
            EnumGroupSymbol* enumGroup = static_cast<EnumGroupSymbol*>(symbol);
            EnumeratedTypeSymbol* enm = enumGroup->GetEnumType(context);
            if (enm)
            {
                boundExpression.reset(new BoundTypeNode(enm, fullSpan));
            }
            else
            {
                ThrowException("enumerated type not found from enum group '" + enumGroup->Name() + "'", fullSpan, context);
            }
            break;
        }
        case SymbolKind::enumTypeSymbol:
        {
            EnumeratedTypeSymbol* enm = static_cast<EnumeratedTypeSymbol*>(symbol);
            boundExpression.reset(new BoundTypeNode(enm, fullSpan));
            break;
        }
        case SymbolKind::enumConstantSymbol:
        {
            EnumConstantSymbol* enumConstant = static_cast<EnumConstantSymbol*>(symbol);
            boundExpression.reset(new BoundEnumConstant(enumConstant, fullSpan, enumConstant->GetEnumType(context)));
            break;
        }
        case SymbolKind::parameterSymbol:
        {
            if (foundFromParentFn)
            {
                ParameterSymbol* parameter = static_cast<ParameterSymbol*>(symbol);
                BoundParentParameterNode* boundParentParameter = new BoundParentParameterNode(parameter, fullSpan, parameter->GetReferredType(context));
                boundParentParameter->SetLevel(level);
                boundExpression.reset(boundParentParameter);
            }
            else
            {
                ParameterSymbol* parameter = static_cast<ParameterSymbol*>(symbol);
                boundExpression.reset(new BoundParameterNode(parameter, fullSpan, parameter->GetReferredType(context)));
            }
            break;
        }
        case SymbolKind::functionGroupSymbol:
        {
            FunctionGroupSymbol* functionGroupSymbol = static_cast<FunctionGroupSymbol*>(symbol);
            FunctionGroupTypeSymbol* functionGroupType = context->GetSymbolTable()->MakeFunctionGroupTypeSymbol(functionGroupSymbol, context);
            boundExpression.reset(new BoundFunctionGroupNode(functionGroupSymbol, fullSpan, functionGroupType));
            break;
        }
        case SymbolKind::fundamentalTypeSymbol:
        {
            boundExpression.reset(new BoundTypeNode(static_cast<TypeSymbol*>(symbol), fullSpan));
            break;
        }
        case SymbolKind::namespaceSymbol:
        {
            NamespaceSymbol* ns = static_cast<NamespaceSymbol*>(symbol);
            scope = ns->GetScope();
            qualifiedScope = true;
            break;
        }
        default:
        {
            ThrowException("symbol '" + node.Str() + "' processing not implemented", fullSpan, context);
            break;
        }
        }
    }
    else
    {
        ThrowException("symbol '" + node.Str() + "' not found", fullSpan, context);
    }
}

void ExpressionBinder::Visit(otava::ast::QualifiedIdNode& node)
{
    scope = ResolveScope(node.Left(), context);
    bool prevQualifiedScope = qualifiedScope;
    qualifiedScope = true;
    node.Right()->Accept(*this);
    qualifiedScope = prevQualifiedScope;
}

void ExpressionBinder::Visit(otava::ast::DestructorIdNode& node)
{
    Symbol* s = scope->GetSymbol();
    if (s->IsFundamentalTypeSymbol())
    {
        scope = context->GetSymbolTable()->GlobalNs()->GetScope();
    }
    else
    {
        if (s->IsClassTemplateSpecializationSymbol())
        {
            ClassTemplateSpecializationSymbol* sp = static_cast<ClassTemplateSpecializationSymbol*>(s);
            s = sp->ClassTemplate(context);
        }
        scope = s->GetScope();
    }
    Symbol* symbol = scope->Lookup("@destructor", SymbolGroupKind::functionSymbolGroup, ScopeLookup::thisScope, node.GetFullSpan(),
        context, LookupFlags::dontResolveSingle);
    if (symbol && symbol->IsFunctionGroupSymbol())
    {
        FunctionGroupSymbol* functionGroupSymbol = static_cast<FunctionGroupSymbol*>(symbol);
        FunctionGroupTypeSymbol* functionGroupType = context->GetSymbolTable()->MakeFunctionGroupTypeSymbol(functionGroupSymbol, context);
        boundExpression.reset(new BoundFunctionGroupNode(static_cast<FunctionGroupSymbol*>(symbol), node.GetFullSpan(), functionGroupType));
    }
    else
    {
        emptyDestructor = true;
        boundExpression.reset();
    }
}

void ExpressionBinder::Visit(otava::ast::ThisNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    bool invokeOrTryCatch = context->GetFlag(ContextFlags::invoke | ContextFlags::tryCatch);
    FunctionDefinitionSymbol* parentFn = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->ParentFn();
    if (!parentFn || !invokeOrTryCatch)
    {
        ParameterSymbol* thisParam = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->ThisParam(context);
        if (thisParam)
        {
            boundExpression.reset(new BoundParameterNode(thisParam, fullSpan, thisParam->GetType(context)));
        }
        else
        {
            ThrowException("'this' can only be used in member function context", fullSpan, context);
        }
    }
    else
    {
        int level = -1;
        FunctionDefinitionSymbol* parentFunction = nullptr;
        while (parentFn)
        {
            parentFunction = parentFn;
            parentFn = parentFn->ParentFn();
            ++level;
        }
        ParameterSymbol* thisParam = parentFunction->ThisParam(context);
        if (thisParam)
        {
            BoundParentParameterNode* boundParentParameter = new BoundParentParameterNode(thisParam, fullSpan, thisParam->GetType(context));
            boundParentParameter->SetLevel(level);
            boundExpression.reset(boundParentParameter);
        }
        else
        {
            ThrowException("'this' can only be used in member function context", fullSpan, context);
        }
    }
}

void ExpressionBinder::Visit(otava::ast::TemplateIdNode& node)
{
    Scope* currentScope = context->GetSymbolTable()->CurrentScope();
    currentScope->PushParentScope(scope);
    TypeSymbol* type = ResolveType(&node, DeclarationFlags::none, context, TypeResolverFlags::dontThrow);
    if (type)
    {
        type = type->DirectType(context)->FinalType(node.GetFullSpan(), context);
        boundExpression.reset(new BoundTypeNode(type, node.GetFullSpan()));
    }
    else
    {
        std::unique_ptr<BoundExpressionNode> subject(BindExpression(node.TemplateName(), context));
        if (subject->IsBoundFunctionGroupNode())
        {
            BoundFunctionGroupNode* boundFunctionGroup = static_cast<BoundFunctionGroupNode*>(subject.get());
            for (otava::ast::Node* item : node.Items())
            {
                TypeSymbol* templateArg = ResolveType(item, DeclarationFlags::none, context);
                boundFunctionGroup->AddTemplateArg(templateArg);
            }
            boundExpression.reset(subject.release());
        }
        else
        {
            ThrowException("expression.binder: error binding template id '" + node.TemplateName()->Str() + "': type or function group expected",
                node.GetFullSpan(), context);
        }
    }
    currentScope->PopParentScope();
}

void ExpressionBinder::Visit(otava::ast::MemberExprNode& node)
{
    std::unique_ptr<BoundExpressionNode> subject = BindExpression(node.Child(), context);
    if (!subject)
    {
        ThrowException("member expression subject not resolved", node.GetFullSpan(), context);
    }
    BindMemberExpr(&node, std::move(subject));
}

void ExpressionBinder::BindMemberExpr(otava::ast::MemberExprNode* node, std::unique_ptr<BoundExpressionNode>&& subject)
{
    if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode && subject->GetType()->PlainType(context)->IsClassTypeSymbol())
    {
        BindClassArrow(std::move(subject), node);
        return;
    }
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    std::unique_ptr<BoundExpressionNode> s = std::move(subject);
    Scope* memberScope = s->GetMemberScope(node->Op(), fullSpan, context);
    bool memberScopeSet = false;
    if (memberScope)
    {
        scope = memberScope;
        memberScopeSet = true;
    }
    if (memberScopeSet)
    {
        context->PushSetFlag(ContextFlags::lookupOnlyFromMemberScope);
    }
    node->GetId()->Accept(*this);
    if (memberScopeSet)
    {
        context->PopFlags();
    }
    if (emptyDestructor)
    {
        boundExpression.reset(new BoundEmptyDestructorNode(fullSpan));
        return;
    }
    std::unique_ptr<BoundExpressionNode> member = std::move(boundExpression);
    if (s->IsBoundLocalVariable(context) && member->IsBoundMemberVariable(context))
    {
        BoundVariableNode* localVar = static_cast<BoundVariableNode*>(s.release());
        BoundVariableNode* memberVar = static_cast<BoundVariableNode*>(member.release());
        if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode)
        {
            memberVar->SetThisPtr(localVar);
        }
        else
        {
            std::unique_ptr<BoundExpressionNode> thisPtr;
            if (localVar->GetType()->IsReferenceType())
            {
                thisPtr.reset(new BoundRefToPtrNode(localVar, fullSpan, localVar->GetType()->RemoveReference(context)->AddPointer(context)));
            }
            else
            {
                thisPtr.reset(new BoundAddressOfNode(localVar, fullSpan, localVar->GetType()->AddPointer(context)));
            }
            memberVar->SetThisPtr(thisPtr.release());
        }
        boundExpression.reset(memberVar);
    }
    else if (s->IsBoundMemberVariable(context) && member->IsBoundMemberVariable(context))
    {
        BoundVariableNode* subjectVar = static_cast<BoundVariableNode*>(s.release());
        BoundVariableNode* memberVar = static_cast<BoundVariableNode*>(member.release());
        if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode)
        {
            memberVar->SetThisPtr(subjectVar);
        }
        else
        {
            std::unique_ptr<BoundExpressionNode> thisPtr;
            if (subjectVar->GetType()->IsReferenceType())
            {
                thisPtr.reset(new BoundRefToPtrNode(subjectVar, fullSpan, subjectVar->GetType()->RemoveReference(context)->AddPointer(context)));
            }
            else
            {
                thisPtr.reset(new BoundAddressOfNode(subjectVar, fullSpan, subjectVar->GetType()->AddPointer(context)));
            }
            memberVar->SetThisPtr(thisPtr.release());
        }
        boundExpression.reset(memberVar);
    }
    else if (s->IsBoundParameterNode() && member->IsBoundMemberVariable(context))
    {
        BoundParameterNode* param = static_cast<BoundParameterNode*>(s.release());
        if (param->GetType()->GetBaseType(context)->IsClassTypeSymbol())
        {
            std::unique_ptr<BoundExpressionNode> thisPtr;;
            if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode)
            {
                thisPtr.reset(param);
            }
            else
            {
                if (param->GetType()->IsReferenceType())
                {
                    thisPtr.reset(new BoundRefToPtrNode(param, fullSpan, param->GetType()->RemoveReference(context)->AddPointer(context)));
                }
                else
                {
                    thisPtr.reset(new BoundAddressOfNode(param, fullSpan, param->GetType()->AddPointer(context)));
                }
            }
            BoundVariableNode* memberVar = static_cast<BoundVariableNode*>(member.release());
            memberVar->SetThisPtr(thisPtr.release());
            boundExpression.reset(memberVar);
        }
        else
        {
            boundExpression.reset(new BoundMemberExprNode(param, member.release(), node->Op()->Kind(), fullSpan, nullptr));
        }
    }
    else if (s->IsBoundParentParameterNode() && member->IsBoundParentMemberVariable(context))
    {
        BoundParentParameterNode* param = static_cast<BoundParentParameterNode*>(s.release());
        if (param->GetType()->GetBaseType(context)->IsClassTypeSymbol())
        {
            std::unique_ptr<BoundExpressionNode> thisPtr;
            if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode)
            {
                thisPtr.reset(param);
            }
            else
            {
                if (param->GetType()->IsReferenceType())
                {
                    thisPtr.reset(new BoundRefToPtrNode(param, fullSpan, param->GetType()->RemoveReference(context)->AddPointer(context)));
                }
                else
                {
                    thisPtr.reset(new BoundAddressOfNode(param, fullSpan, param->GetType()->AddPointer(context)));
                }
            }
            BoundParentVariableNode* memberVar = static_cast<BoundParentVariableNode*>(member.release());
            memberVar->SetThisPtr(thisPtr.release());
            boundExpression.reset(memberVar);
        }
        else
        {
            boundExpression.reset(new BoundMemberExprNode(param, member.release(), node->Op()->Kind(), fullSpan, nullptr));
        }
    }
    else if (s->IsBoundParentParameterNode() && member->IsBoundMemberVariable(context))
    {
        BoundParentParameterNode* param = static_cast<BoundParentParameterNode*>(s.release());
        if (param->GetType()->GetBaseType(context)->IsClassTypeSymbol())
        {
            std::unique_ptr<BoundExpressionNode> thisPtr;
            if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode)
            {
                thisPtr.reset(param);
            }
            else
            {
                if (param->GetType()->IsReferenceType())
                {
                    thisPtr.reset(new BoundRefToPtrNode(param, fullSpan, param->GetType()->RemoveReference(context)->AddPointer(context)));
                }
                else
                {
                    thisPtr.reset(new BoundAddressOfNode(param, fullSpan, param->GetType()->AddPointer(context)));
                }
            }
            BoundVariableNode* memberVar = static_cast<BoundVariableNode*>(member.release());
            memberVar->SetThisPtr(thisPtr.release());
            boundExpression.reset(memberVar);
        }
        else
        {
            boundExpression.reset(new BoundMemberExprNode(param, member.release(), node->Op()->Kind(), fullSpan, nullptr));
        }
    }
    else if (s->IsBoundParentLocalVariable(context) && member->IsBoundParentMemberVariable(context))
    {
        BoundParentVariableNode* parentVar = static_cast<BoundParentVariableNode*>(s.release());
        BoundParentVariableNode* memberVar = static_cast<BoundParentVariableNode*>(member.release());
        if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode)
        {
            memberVar->SetThisPtr(parentVar);
        }
        else
        {
            std::unique_ptr<BoundExpressionNode> thisPtr;
            if (parentVar->GetType()->IsReferenceType())
            {
                thisPtr.reset(new BoundRefToPtrNode(parentVar, fullSpan, parentVar->GetType()->RemoveReference(context)->AddPointer(context)));
            }
            else
            {
                thisPtr.reset(new BoundAddressOfNode(parentVar, fullSpan, parentVar->GetType()->AddPointer(context)));
            }
            memberVar->SetThisPtr(thisPtr.release());
        }
        boundExpression.reset(memberVar);
    }
    else if (s->IsBoundParentLocalVariable(context) && member->IsBoundMemberVariable(context))
    {
        BoundParentVariableNode* localVar = static_cast<BoundParentVariableNode*>(s.release());
        BoundVariableNode* memberVar = static_cast<BoundVariableNode*>(member.release());
        if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode)
        {
            memberVar->SetThisPtr(localVar);
        }
        else
        {
            std::unique_ptr<BoundExpressionNode> thisPtr;
            if (localVar->GetType()->IsReferenceType())
            {
                thisPtr.reset(new BoundRefToPtrNode(localVar, fullSpan, localVar->GetType()->RemoveReference(context)->AddPointer(context)));
            }
            else
            {
                thisPtr.reset(new BoundAddressOfNode(localVar, fullSpan, localVar->GetType()->AddPointer(context)));
            }
            memberVar->SetThisPtr(thisPtr.release());
        }
        boundExpression.reset(memberVar);
    }
    else if (s->IsBoundParentMemberVariable(context) && member->IsBoundParentMemberVariable(context))
    {
        BoundParentVariableNode* subjectVar = static_cast<BoundParentVariableNode*>(s.release());
        BoundParentVariableNode* memberVar = static_cast<BoundParentVariableNode*>(member.release());
        if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode)
        {
            memberVar->SetThisPtr(subjectVar);
        }
        else
        {
            std::unique_ptr<BoundExpressionNode> thisPtr;
            if (subjectVar->GetType()->IsReferenceType())
            {
                thisPtr.reset(new BoundRefToPtrNode(subjectVar, fullSpan, subjectVar->GetType()->RemoveReference(context)->AddPointer(context)));
            }
            else
            {
                thisPtr.reset(new BoundAddressOfNode(subjectVar, fullSpan, subjectVar->GetType()->AddPointer(context)));
            }
            memberVar->SetThisPtr(thisPtr.release());
        }
        boundExpression.reset(memberVar);
    }
    else if (s->IsBoundParentMemberVariable(context) && member->IsBoundMemberVariable(context))
    {
        BoundParentVariableNode* subjectVar = static_cast<BoundParentVariableNode*>(s.release());
        BoundVariableNode* memberVar = static_cast<BoundVariableNode*>(member.release());
        if (node->Op()->Kind() == otava::ast::NodeKind::arrowNode)
        {
            memberVar->SetThisPtr(subjectVar);
        }
        else
        {
            std::unique_ptr<BoundExpressionNode> thisPtr;
            if (subjectVar->GetType()->IsReferenceType())
            {
                thisPtr.reset(new BoundRefToPtrNode(subjectVar, fullSpan, subjectVar->GetType()->RemoveReference(context)->AddPointer(context)));
            }
            else
            {
                thisPtr.reset(new BoundAddressOfNode(subjectVar, fullSpan, subjectVar->GetType()->AddPointer(context)));
            }
            memberVar->SetThisPtr(thisPtr.release());
        }
        boundExpression.reset(memberVar);
    }
    else if (s->IsBoundFunctionCallNode() && member->IsBoundMemberVariable(context))
    {
        BoundVariableNode* memberVar = static_cast<BoundVariableNode*>(member.release());
        memberVar->SetThisPtr(s.release());
        boundExpression.reset(memberVar);
    }
    else if (s->IsBoundFunctionCallNode() && member->IsBoundParentMemberVariable(context))
    {
        BoundParentVariableNode* memberVar = static_cast<BoundParentVariableNode*>(member.release());
        memberVar->SetThisPtr(s.release());
        boundExpression.reset(memberVar);
    }
    else
    {
        boundExpression.reset(new BoundMemberExprNode(s.release(), member.release(), node->Op()->Kind(), fullSpan, nullptr));
    }
}

void ExpressionBinder::Visit(otava::ast::InvokeExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    bool thisPtrAdded = false;
    bool suppressWarning = false;
    VariableSymbol* temporary = nullptr;
    BoundExpressionNode* temporaryDestructorCallArg = nullptr;
    int level = 0;
    Scope* subjectScope = scope;
    OverloadResolutionFlags resolutionFlags = OverloadResolutionFlags::none;
    if (node.IsCompileUnitInitFn())
    {
        if (node.Subject()->IsIdentifierNode())
        {
            otava::ast::IdentifierNode* idNode = static_cast<otava::ast::IdentifierNode*>(node.Subject());
            CompileUnitInitFn* compileUnitInitFn = new CompileUnitInitFn(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), idNode->Str());
            compileUnitInitFn->SetReturnType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
                otava::symbols::FundamentalTypeKind::voidType, context), context);
            context->GetModule()->GetSymbolTable()->GlobalNs()->GetScope()->AddSymbol(compileUnitInitFn, fullSpan, context);
            FunctionTypeSymbol* fnTypeSymbol = context->GetSymbolTable()->MakeFunctionTypeSymbol(compileUnitInitFn, context);
            std::unique_ptr<BoundFunctionCallNode> fnCall(new BoundFunctionCallNode(compileUnitInitFn, fullSpan, fnTypeSymbol));
            boundExpression.reset(fnCall.release());
        }
        return;
    }
    if (node.Subject()->IsQualifiedIdNode())
    {
        resolutionFlags = resolutionFlags | OverloadResolutionFlags::dontSearchArgumentScopes;
    }
    std::unique_ptr<BoundExpressionNode> subject = BindExpression(node.Subject(), context,
        SymbolGroupKind::functionSymbolGroup | SymbolGroupKind::aliasSymbolGroup | SymbolGroupKind::classSymbolGroup | SymbolGroupKind::enumSymbolGroup |
        SymbolGroupKind::variableSymbolGroup | SymbolGroupKind::templateParamSymbolGroup | SymbolGroupKind::namespaceSymbolGroup, subjectScope);
    if (subject)
    {
        TypeSymbol* subjectType = subject->GetType();
        if (subjectType && subjectType->IsFunctionPtrType(context))
        {
            std::unique_ptr<BoundFunctionPtrCallNode> call(new BoundFunctionPtrCallNode(fullSpan, subjectType));
            call->AddArgument(subject.release());
            int n = node.Items().size();
            for (int i = 0; i < n; ++i)
            {
                otava::ast::Node* item = node.Items()[i];
                std::unique_ptr<BoundExpressionNode> arg = BindExpression(item, context);
                if (arg->GetType()->RemoveConst(context)->IsClassTypeSymbol())
                {
                    TypeSymbol* type = arg->GetType()->AddPointer(context);
                    arg.reset(new BoundAddressOfNode(arg.release(), fullSpan, type));
                }
                call->AddArgument(arg.release());
            }
            boundExpression.reset(call.release());
            return;
        }
        if (subject->IsBoundEmptyDestructorNode())
        {
            boundExpression.reset(new BoundEmptyFunctionCallNode(fullSpan));
            return;
        }
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        if (subject->IsBoundTypeNode())
        {
            bool invoke = context->GetFlag(ContextFlags::invoke);
            if (invoke)
            {
                std::pair<VariableSymbol*, int> temporaryLevel = GetParentTemporary(node.Id(), context);
                temporary = temporaryLevel.first;
                level = temporaryLevel.second;
                if (temporary)
                {
                    BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(temporary, fullSpan, temporary->GetReferredType(context));
                    boundParentVariable->SetLevel(level);
                    BoundAddressOfNode* boundAddrOfNode = new BoundAddressOfNode(boundParentVariable, fullSpan, temporary->GetType(context)->AddPointer(context));
                    args.push_back(std::unique_ptr<BoundExpressionNode>(boundAddrOfNode));
                    resolutionFlags = resolutionFlags & ~OverloadResolutionFlags::dontSearchArgumentScopes;
                    suppressWarning = true;
                    temporaryDestructorCallArg = args.back().get();
                }
                else
                {
                    ThrowException("parent temporary not found from child", fullSpan, context);
                }
            }
            else
            {
                temporary = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(subject->GetType(), node.Id(), context);
                BoundAddressOfNode* boundAddrOfNode = new BoundAddressOfNode(new BoundVariableNode(temporary, fullSpan, temporary->GetReferredType(context)), fullSpan,
                    temporary->GetType(context)->AddPointer(context));
                args.push_back(std::unique_ptr<BoundExpressionNode>(boundAddrOfNode));
                resolutionFlags = resolutionFlags & ~OverloadResolutionFlags::dontSearchArgumentScopes;
                suppressWarning = true;
                temporaryDestructorCallArg = args.back().get();
            }
        }
        else
        {
            BoundExpressionNode* firstArg = GetFirstArg(subject.get(), context);
            if (firstArg)
            {
                args.push_back(std::unique_ptr<BoundExpressionNode>(firstArg));
            }
            else
            {
                BoundExpressionNode* thisPtr = context->GetThisPtr(fullSpan);
                if (thisPtr)
                {
                    args.push_back(std::unique_ptr<BoundExpressionNode>(thisPtr));
                    thisPtrAdded = true;
                }
            }
        }
        int n = node.Items().size();
        for (int i = 0; i < n; ++i)
        {
            otava::ast::Node* item = node.Items()[i];
            std::unique_ptr<BoundExpressionNode> arg = BindExpression(item, context);
            args.push_back(std::move(arg));
        }
        std::string groupName;
        if (subject->IsBoundTypeNode())
        {
            groupName = "@constructor";
        }
        else
        {
            groupName = GetGroupName(subject.get(), context);
        }
        Exception ex1;
        Exception ex2;
        std::vector<TypeSymbol*> templateArgs;
        if (subject->IsBoundFunctionGroupNode())
        {
            BoundFunctionGroupNode* boundFunctionGroup = static_cast<BoundFunctionGroupNode*>(subject.get());
            templateArgs = boundFunctionGroup->TemplateArgs();
        }
        bool flagsPushed = false;
        if (suppressWarning)
        {
            context->PushSetFlag(ContextFlags::suppress_warning);
            flagsPushed = true;
        }
        context->PushNodeId(node.Id());
        std::unique_ptr<BoundFunctionCallNode> functionCall = ResolveOverload(subjectScope, groupName, templateArgs, args, fullSpan, context, ex1,
            resolutionFlags);
        context->PopNodeId();
        if (flagsPushed)
        {
            context->PopFlags();
            flagsPushed = false;
        }
        if (functionCall)
        {
            if (ex1.Warning())
            {
                PrintWarning(ex1, context);
            }
        }
        if (!functionCall && thisPtrAdded)
        {
            args.erase(args.begin());
            if (suppressWarning)
            {
                context->PushSetFlag(ContextFlags::suppress_warning);
                flagsPushed = true;
            }
            resolutionFlags = resolutionFlags | OverloadResolutionFlags::noMemberFunctions;
            context->PushNodeId(node.Id());
            functionCall = ResolveOverload(subjectScope, groupName, templateArgs, args, fullSpan, context, ex2, resolutionFlags);
            context->PopNodeId();
            if (flagsPushed)
            {
                context->PopFlags();
                flagsPushed = false;
            }
            if (functionCall)
            {
                if (ex2.Warning())
                {
                    PrintWarning(ex2, context);
                }
            }
        }
        if (!functionCall)
        {
            Exception ex(std::string(ex1.what()) + "\n" + std::string(ex2.what()));
            ThrowException(ex);
        }
        functionCall->SetSource(node.Clone());
        FunctionSymbol* functionSymbol = functionCall->GetFunctionSymbol();
        if (functionSymbol->IsVirtual() && !node.Subject()->IsQualifiedIdNode())
        {
            functionCall->SetFlag(BoundExpressionFlags::virtualCall);
            context->GetBoundCompileUnit()->AddBoundNodeForClass(functionSymbol->ParentClassType(context), fullSpan, context);
            if (!functionCall->Args().empty())
            {
                BoundExpressionNode* firstArg = functionCall->Args()[0].get();
                TypeSymbol* firstArgType = firstArg->GetType()->GetBaseType(context);
                if (firstArgType->IsClassTypeSymbol())
                {
                    ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(firstArgType);
                    context->GetBoundCompileUnit()->AddBoundNodeForClass(classType, fullSpan, context);
                }
            }
        }
        VariableSymbol* classTemporary = nullptr;
        if (functionSymbol->ReturnsClass())
        {
            bool invoke = context->GetFlag(ContextFlags::invoke);
            if (invoke)
            {
                std::pair<VariableSymbol*, int> temporaryLevel = GetParentTemporary(node.Id(), context);
                classTemporary = temporaryLevel.first;
                level = temporaryLevel.second;
                if (classTemporary)
                {
                    BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context));
                    boundParentVariable->SetLevel(level);
                    functionCall->AddArgument(new BoundAddressOfNode(boundParentVariable, fullSpan, classTemporary->GetType(context)->AddPointer(context)));
                    temporaryDestructorCallArg = functionCall->Args().back().get();
                }
                else
                {
                    ThrowException("parent temporary not found from child", fullSpan, context);
                }
            }
            else
            {
                classTemporary = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(functionSymbol->ReturnType(context), node.Id(), context);
                functionCall->AddArgument(new BoundAddressOfNode(
                    new BoundVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context)), fullSpan, 
                    classTemporary->GetType(context)->AddPointer(context)));
                temporaryDestructorCallArg = functionCall->Args().back().get();
            }
        }
        boundExpression.reset(functionCall.release());
        if (classTemporary)
        {
            bool invoke = context->GetFlag(ContextFlags::invoke);
            if (invoke)
            {
                BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(classTemporary, fullSpan, classTemporary->GetReferredType(context));
                boundParentVariable->SetLevel(level);
                BoundConstructTemporaryNode* ctn = new BoundConstructTemporaryNode(boundExpression.release(), boundParentVariable, fullSpan);
                CheckGenerateTemporaryDestructorCall(ctn, temporaryDestructorCallArg, context);
                boundExpression.reset(ctn);
                boundExpression->SetFlag(BoundExpressionFlags::bindToRvalueRef);
            }
            else
            {
                BoundConstructTemporaryNode* ctn = new BoundConstructTemporaryNode(boundExpression.release(), new BoundVariableNode(
                    classTemporary, fullSpan, classTemporary->GetReferredType(context)), fullSpan);
                CheckGenerateTemporaryDestructorCall(ctn, temporaryDestructorCallArg, context);
                boundExpression.reset(ctn);
                boundExpression->SetFlag(BoundExpressionFlags::bindToRvalueRef);
            }
        }
        if (subject->IsBoundTypeNode())
        {
            bool invoke = context->GetFlag(ContextFlags::invoke);
            if (invoke)
            {
                BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(temporary, fullSpan, temporary->GetReferredType(context));
                boundParentVariable->SetLevel(level);
                BoundConstructTemporaryNode* ctn = new BoundConstructTemporaryNode(boundExpression.release(), boundParentVariable, fullSpan);
                CheckGenerateTemporaryDestructorCall(ctn, temporaryDestructorCallArg, context);
                boundExpression.reset(ctn);
                boundExpression->SetFlag(BoundExpressionFlags::bindToRvalueRef);
            }
            else
            {
                BoundConstructTemporaryNode* ctn = new BoundConstructTemporaryNode(
                    boundExpression.release(), new BoundVariableNode(temporary, fullSpan, temporary->GetReferredType(context)), fullSpan);
                CheckGenerateTemporaryDestructorCall(ctn, temporaryDestructorCallArg, context);
                boundExpression.reset(ctn);
                boundExpression->SetFlag(BoundExpressionFlags::bindToRvalueRef);
            }
        }
    }
    else
    {
        ThrowException("unkown invoke subject", fullSpan, context);
    }
}

void ExpressionBinder::Visit(otava::ast::BinaryExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    bool booleanChild = false;
    otava::ast::NodeKind op = node.Op()->Kind();
    switch (op)
    {
    case otava::ast::NodeKind::disjunctionNode:
    case otava::ast::NodeKind::conjunctionNode:
    {
        booleanChild = true;
        context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
        break;
    }
    }
    std::unique_ptr<BoundExpressionNode> left = BindExpression(node.Left(), context, booleanChild);
    std::unique_ptr<BoundExpressionNode> right = BindExpression(node.Right(), context, booleanChild);
    switch (op)
    {
    case otava::ast::NodeKind::disjunctionNode:
    case otava::ast::NodeKind::conjunctionNode:
    {
        context->PopFlags();
        break;
    }
    }
    switch (op)
    {
    case otava::ast::NodeKind::assignNode:
    case otava::ast::NodeKind::plusAssignNode:
    case otava::ast::NodeKind::minusAssignNode:
    case otava::ast::NodeKind::mulAssignNode:
    case otava::ast::NodeKind::divAssignNode:
    case otava::ast::NodeKind::modAssignNode:
    case otava::ast::NodeKind::xorAssignNode:
    case otava::ast::NodeKind::andAssignNode:
    case otava::ast::NodeKind::orAssignNode:
    case otava::ast::NodeKind::shiftLeftAssignNode:
    case otava::ast::NodeKind::shiftRightAssignNode:
    case otava::ast::NodeKind::inclusiveOrNode:
    case otava::ast::NodeKind::exclusiveOrNode:
    case otava::ast::NodeKind::andNode:
    case otava::ast::NodeKind::equalNode:
    case otava::ast::NodeKind::lessNode:
    case otava::ast::NodeKind::compareNode:
    case otava::ast::NodeKind::shiftLeftNode:
    case otava::ast::NodeKind::shiftRightNode:
    case otava::ast::NodeKind::plusNode:
    case otava::ast::NodeKind::minusNode:
    case otava::ast::NodeKind::mulNode:
    case otava::ast::NodeKind::divNode:
    case otava::ast::NodeKind::modNode:
    {
        BindBinaryOp(op, fullSpan, std::move(left), std::move(right), &node);
        break;
    }
    case otava::ast::NodeKind::notEqualNode:
    {
        BindBinaryOp(otava::ast::NodeKind::equalNode, fullSpan, std::move(left), std::move(right), nullptr);
        BindUnaryOp(otava::ast::NodeKind::notNode, fullSpan, std::move(boundExpression), nullptr);
        break;
    }
    case otava::ast::NodeKind::greaterNode:
    {
        BindBinaryOp(otava::ast::NodeKind::lessNode, fullSpan, std::move(right), std::move(left), nullptr);
        break;
    }
    case otava::ast::NodeKind::lessOrEqualNode:
    {
        BindBinaryOp(otava::ast::NodeKind::lessNode, fullSpan, std::move(right), std::move(left), nullptr);
        BindUnaryOp(otava::ast::NodeKind::notNode, fullSpan, std::move(boundExpression), nullptr);
        break;
    }
    case otava::ast::NodeKind::greaterOrEqualNode:
    {
        BindBinaryOp(otava::ast::NodeKind::lessNode, fullSpan, std::move(left), std::move(right), nullptr);
        BindUnaryOp(otava::ast::NodeKind::notNode, fullSpan, std::move(boundExpression), nullptr);
        break;
    }
    case otava::ast::NodeKind::disjunctionNode:
    {
        TypeSymbol* boolType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context);
        BoundDisjunctionNode* boundDisjunction = new BoundDisjunctionNode(left.release(), right.release(), fullSpan, boolType);
        VariableSymbol* temporary = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(boolType, node.Id(), context);
        boundDisjunction->SetTemporary(new BoundVariableNode(temporary, fullSpan, temporary->GetReferredType(context)));
        boundExpression.reset(boundDisjunction);
        break;
    }
    case otava::ast::NodeKind::conjunctionNode:
    {
        TypeSymbol* boolType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context);
        BoundConjunctionNode* boundConjunction = new BoundConjunctionNode(left.release(), right.release(), fullSpan, boolType);
        VariableSymbol* temporary = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(boolType, node.Id(), context);
        boundConjunction->SetTemporary(new BoundVariableNode(temporary, fullSpan, temporary->GetReferredType(context)));
        boundExpression.reset(boundConjunction);
        break;
    }
    case otava::ast::NodeKind::commaNode:
    {
        boundExpression.reset(new BoundExpressionSequenceNode(fullSpan, left.release(), right.release()));
        break;
    }
    }
}

void ExpressionBinder::Visit(otava::ast::UnaryExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundExpressionNode> child = BindExpression(node.Child(), context);
    otava::ast::NodeKind op = node.Op()->Kind();
    switch (op)
    {
    case otava::ast::NodeKind::notNode:
    case otava::ast::NodeKind::plusNode:
    case otava::ast::NodeKind::minusNode:
    case otava::ast::NodeKind::complementNode:
    {
        BindUnaryOp(op, fullSpan, std::move(child), &node);
        break;
    }
    case otava::ast::NodeKind::derefNode:
    {
        BindDeref(fullSpan, std::move(child));
        break;
    }
    case otava::ast::NodeKind::addrOfNode:
    {
        BindAddrOf(fullSpan, std::move(child));
        break;
    }
    case otava::ast::NodeKind::prefixIncNode:
    {
        BindPrefixInc(fullSpan, std::move(child), node.Child());
        break;
    }
    case otava::ast::NodeKind::prefixDecNode:
    {
        BindPrefixDec(fullSpan, std::move(child), node.Child());
        break;
    }
    }
}

void ExpressionBinder::Visit(otava::ast::SubscriptExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundExpressionNode> subject = BindExpression(node.Child(), context);
    std::unique_ptr<BoundExpressionNode> index = BindExpression(node.Index(), context);
    TypeSymbol* plainSubjectType = subject->GetType()->PlainType(context);
    if (plainSubjectType->IsClassTypeSymbol())
    {
        BindBinaryOp(otava::ast::NodeKind::subscriptExprNode, fullSpan, std::move(subject), std::move(index), nullptr);
    }
    else  if (plainSubjectType->IsPointerType())
    {
        BindBinaryOp(otava::ast::NodeKind::plusNode, fullSpan, std::move(subject), std::move(index), nullptr);
        BindDeref(fullSpan, std::move(boundExpression));
    }
    else if (plainSubjectType->IsArrayType())
    {
        ArrayTypeSymbol* arrayType = static_cast<ArrayTypeSymbol*>(plainSubjectType->GetBaseType(context));
        TypeSymbol* arrayPtrType = arrayType->AddPointer(context);
        std::unique_ptr<BoundExpressionNode> arrayPtr(new BoundAddressOfNode(subject->Clone(), fullSpan, arrayPtrType));
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            arrayType->ElementType(context)->AddPointer(context), arrayPtrType, fullSpan, context);
        if (conversion)
        {
            TypeSymbol* type = conversion->ReturnType(context);
            std::unique_ptr<BoundExpressionNode> boundConversion(new BoundConversionNode(arrayPtr.release(), conversion, fullSpan, type));
            BindBinaryOp(otava::ast::NodeKind::plusNode, fullSpan, std::move(boundConversion), std::move(index), nullptr);
            BindDeref(fullSpan, std::move(boundExpression));
        }
        else
        {
            ThrowException("no conversion found", fullSpan, context);
        }
    }
    else
    {
        ThrowException("subscript operator can be applied only to pointer, array or class type subject", fullSpan, context);
    }
}

void ExpressionBinder::Visit(otava::ast::ExpressionListNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    BoundExpressionListNode* boundExpressionListNode = new BoundExpressionListNode(fullSpan);
    for (int i = 0; i < node.Items().size(); ++i)
    {
        otava::ast::Node* itemNode = node.Items()[i];
        std::unique_ptr<BoundExpressionNode> boundExpr = BindExpression(itemNode, context);
        boundExpressionListNode->AddExpression(boundExpr.release());
    }
    boundExpression.reset(boundExpressionListNode);
}

void ExpressionBinder::Visit(otava::ast::SizeOfTypeExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    TypeSymbol* type = ResolveType(node.Child(), DeclarationFlags::none, context);
    type = type->DirectType(context)->FinalType(fullSpan, context);
    otava::intermediate::Type* irType = type->IrType(*context->GetEmitter(), fullSpan, context);
    std::int64_t size = irType->Size();
    otava::ast::IdentifierNode size_t_node(fullSpan.span, fullSpan.fileIndex, "size_t");
    TypeSymbol* size_t_type = ResolveType(&size_t_node, DeclarationFlags::none, context);
    while (size_t_type->IsAliasTypeSymbol())
    {
        AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(size_t_type);
        size_t_type = aliasType->ReferredType(context);
    }
    IntegerValue* integerValue = context->GetEvaluationContext()->GetIntegerValue(size, std::to_string(size), size_t_type, context);
    boundExpression.reset(new BoundLiteralNode(integerValue, fullSpan, integerValue->GetType(context)));
}

void ExpressionBinder::Visit(otava::ast::SizeOfUnaryExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    boundExpression = BindExpression(node.Child(), context);
    TypeSymbol* type = boundExpression->GetType();
    type = type->DirectType(context)->FinalType(fullSpan, context);
    otava::intermediate::Type* irType = type->IrType(*context->GetEmitter(), fullSpan, context);
    std::int64_t size = irType->Size();
    otava::ast::IdentifierNode size_t_node(fullSpan.span, fullSpan.fileIndex, "size_t");
    TypeSymbol* size_t_type = ResolveType(&size_t_node, DeclarationFlags::none, context);
    while (size_t_type->IsAliasTypeSymbol())
    {
        AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(size_t_type);
        size_t_type = aliasType->ReferredType(context);
    }
    IntegerValue* integerValue = context->GetEvaluationContext()->GetIntegerValue(size, std::to_string(size), size_t_type, context);
    boundExpression.reset(new BoundLiteralNode(integerValue, fullSpan, integerValue->GetType(context)));
}

class NewInitializerBinder : public otava::ast::DefaultVisitor
{
public:
    NewInitializerBinder(Context* context_, std::vector<std::unique_ptr<BoundExpressionNode>>& boundExpressions_);
    void Visit(otava::ast::NewInitializerNode& node) override;
private:
    Context* context;
    std::vector<std::unique_ptr<BoundExpressionNode>>& boundExpressions;
};

NewInitializerBinder::NewInitializerBinder(Context* context_, std::vector<std::unique_ptr<BoundExpressionNode>>& boundExpressions_) :
    context(context_), boundExpressions(boundExpressions_)
{
}

void NewInitializerBinder::Visit(otava::ast::NewInitializerNode& node)
{
    int n = node.Count();
    for (int i = 0; i < n; ++i)
    {
        otava::ast::Node* initNode = node.Nodes()[i];
        if (!initNode->IsCommaNode())
        {
            std::unique_ptr<BoundExpressionNode> exprNode = BindExpression(initNode, context);
            boundExpressions.push_back(std::move(exprNode));
        }
    }
}

void ExpressionBinder::Visit(otava::ast::OpNewCall& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundExpressionNode> size(BindExpression(node.Child(), context));
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    args.push_back(std::unique_ptr<BoundExpressionNode>(size.release()));
    std::vector<TypeSymbol*> templateArgs;
    context->PushNodeId(node.Id());
    std::unique_ptr<BoundFunctionCallNode> opNewCall = ResolveOverloadThrow(
        context->GetSymbolTable()->CurrentScope(), "operator new", templateArgs, args, fullSpan, context);
    context->PopNodeId();
    boundExpression.reset(opNewCall.release());
}

void ExpressionBinder::Visit(otava::ast::NewExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    TypeSymbol* type = ResolveType(node.Child(), DeclarationFlags::none, context);
    type = type->DirectType(context)->FinalType(fullSpan, context);
    otava::ast::SizeOfTypeExprNode sizeOfNode(fullSpan.span, fullSpan.fileIndex, node.Child()->Clone(), fullSpan.span, fullSpan.span);
    std::unique_ptr<BoundExpressionNode> sizeOfValue(BindExpression(&sizeOfNode, context));
    BoundExpressionNode* size = static_cast<BoundExpressionNode*>(sizeOfValue.get());
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    args.push_back(std::unique_ptr<BoundExpressionNode>(sizeOfValue.release()));
    bool hasPlacement = false;
    if (node.Placement() && node.Placement()->IsNewPlacementNode())
    {
        hasPlacement = true;
        otava::ast::NewPlacementNode* placement = static_cast<otava::ast::NewPlacementNode*>(node.Placement());
        int n = placement->Items().size();
        for (int i = 0; i < n; ++i)
        {
            otava::ast::Node* item = placement->Items()[i];
            std::unique_ptr<BoundExpressionNode> boundItem = BindExpression(item, context);
            args.push_back(std::move(boundItem));
        }
    }
    std::vector<TypeSymbol*> templateArgs;
    context->PushNodeId(node.Id());
    std::unique_ptr<BoundFunctionCallNode> opNewCall = ResolveOverloadThrow(
        context->GetSymbolTable()->CurrentScope(), "operator new", templateArgs, args, fullSpan, context);
    context->PopNodeId();
    if (!hasPlacement && size->IsBoundLiteralNode())
    {
        BoundLiteralNode* value = static_cast<BoundLiteralNode*>(size);
        std::int64_t sizeValue = 0;
        if (value->GetValue()->IsIntegerValue())
        {
            IntegerValue* integerValue = static_cast<IntegerValue*>(value->GetValue());
            sizeValue = integerValue->GetValue();
        }
        else
        {
            ThrowException("integer value expected", fullSpan, context);
        }
        opNewCall->SetSource(new otava::ast::OpNewCall(fullSpan.span, fullSpan.fileIndex,
            new otava::ast::IntegerLiteralNode(fullSpan.span, fullSpan.fileIndex, sizeValue, otava::ast::Suffix::u | otava::ast::Suffix::ll, otava::ast::Base::decimal,
                std::to_string(sizeValue))));
    }
    TypeSymbol* voidPtrType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::voidType, context)->AddPointer(context);
    VariableSymbol* tempVarSymbol = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(type->AddPointer(context), node.Id(), context);
    std::vector<std::unique_ptr<BoundExpressionNode>> ctorArgs;
    std::unique_ptr<BoundVariableNode> tempVar(new BoundVariableNode(tempVarSymbol, fullSpan, tempVarSymbol->GetReferredType(context)));
    TypeSymbol* tempVarPtrType = tempVar->GetType()->AddPointer(context);
    ctorArgs.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(tempVar.release(), fullSpan, tempVarPtrType)));
    FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
        type->AddPointer(context), voidPtrType, fullSpan, context);
    if (conversion)
    {
        TypeSymbol* type = conversion->ReturnType(context);
        BoundConversionNode* boundConversion = new BoundConversionNode(opNewCall.release(), conversion, fullSpan, type);
        ctorArgs.push_back(std::unique_ptr<BoundExpressionNode>(boundConversion));
    }
    else
    {
        ThrowException("no conversion found", fullSpan, context);
    }
    context->PushNodeId(node.Id());
    std::unique_ptr<BoundFunctionCallNode> allocation = ResolveOverloadThrow(
        context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs, ctorArgs, fullSpan, context);
    context->PopNodeId();
    std::vector<std::unique_ptr<BoundExpressionNode>> constructObjectArgs;
    std::unique_ptr<BoundVariableNode> tempVar2(new BoundVariableNode(tempVarSymbol, fullSpan, tempVarSymbol->GetReferredType(context)));
    constructObjectArgs.push_back(std::unique_ptr<BoundExpressionNode>(tempVar2.release()));
    NewInitializerBinder initializerAdder(context, constructObjectArgs);
    if (node.Initializer())
    {
        node.Initializer()->Accept(initializerAdder);
        context->PushNodeId(node.Id());
        std::unique_ptr<BoundFunctionCallNode> constructObjectCall = ResolveOverloadThrow(
            context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs, constructObjectArgs, fullSpan, context);
        context->PopNodeId();
        boundExpression.reset(new BoundConstructExpressionNode(
            allocation.release(), constructObjectCall.release(), tempVarSymbol->GetType(context), hasPlacement, fullSpan));
    }
    else
    {
        ThrowException("initializer not found", fullSpan, context);
    }
}

void ExpressionBinder::Visit(otava::ast::DeletePtrNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundExpressionNode> ptr = BindExpression(node.Child(), context);
    std::vector<std::unique_ptr<BoundExpressionNode>> dtorArgs;
    dtorArgs.push_back(std::unique_ptr<BoundExpressionNode>(ptr->Clone()));
    std::vector<TypeSymbol*> templateArgs;
    context->PushNodeId(node.Id());
    std::unique_ptr<BoundFunctionCallNode> dtorCall = ResolveOverloadThrow(
        context->GetSymbolTable()->CurrentScope(), "@destructor", templateArgs, dtorArgs, fullSpan, context);
    context->PopNodeId();
    std::vector<std::unique_ptr<BoundExpressionNode>> opDeleteArgs;
    if (dtorCall->GetFunctionSymbol()->IsVirtual() || dtorCall->GetFunctionSymbol()->IsOverride() || dtorCall->GetFunctionSymbol()->IsFinal())
    {
        dtorCall->SetFlag(BoundExpressionFlags::virtualCall);
        TypeSymbol* thisPtrBaseType = ptr->GetType()->GetBaseType(context);
        FunctionSymbol* conversionFn = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetAdjustDeletePtrConversionFn(thisPtrBaseType, context);
        TypeSymbol* type = conversionFn->ReturnType(context);
        opDeleteArgs.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(ptr.release(), conversionFn, fullSpan, type)));
    }
    else
    {
        opDeleteArgs.push_back(std::unique_ptr<BoundExpressionNode>(ptr->Clone()));
    }
    context->PushNodeId(node.Id());
    std::unique_ptr<BoundFunctionCallNode> opDeleteCall = ResolveOverloadThrow(context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context),
        "operator delete", templateArgs, opDeleteArgs, fullSpan, context);
    context->PopNodeId();
    if (!dtorCall->GetFunctionSymbol()->IsTrivialDestructor())
    {
        boundExpression.reset(new BoundExpressionSequenceNode(fullSpan, dtorCall.release(), opDeleteCall.release()));
    }
    else
    {
        boundExpression.reset(opDeleteCall.release());
    }
}

void ExpressionBinder::Visit(otava::ast::BracedInitListNode& node)
{
    Value* value = Evaluate(&node, context);
    if (value)
    {
        boundExpression.reset(new BoundLiteralNode(value, node.GetFullSpan(), value->GetType(context)));
        context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(boundExpression.release()), context);
    }
}

void ExpressionBinder::Visit(otava::ast::ThrowExprNode& node)
{
    try
    {
        if (node.Child())
        {
            std::unique_ptr<otava::symbols::BoundExpressionNode> expr(BindExpression(node.Child(), context));
            SymbolId ext = expr->GetType()->GetBaseType(context)->Id();
            std::string throwExprStr;
            throwExprStr.append("ort_throw((ort_is_bad_alloc(").append(
                std::to_string(ToUnderlying(ext)).append(") ? ort_get_bad_alloc() : "));
            std::string exprStr = node.Child()->Str();
            if (expr->IsBoundLiteralNode() ||
                expr->IsBoundVariableNode() || expr->IsBoundParentVariableNode() ||
                expr->IsBoundParameterNode() || expr->IsBoundParentParameterNode())
            {
                std::string typeName = expr->GetType()->FullName(context);
                throwExprStr.append("new ").append(typeName).append("(").append(exprStr).append(")");
            }
            else if (node.Child()->IsInvokeExprNode())
            {
                throwExprStr.append("new ").append(exprStr);
            }
            throwExprStr.append("), ").append(std::to_string(ToUnderlying(ext)).append(")"));
            std::unique_ptr<otava::ast::Node> invokeThrowExprNode = ParseExpression(throwExprStr, context);
            boundExpression = BindExpression(invokeThrowExprNode.get(), context);
        }
        else
        {
            std::string throwExprStr;
            throwExprStr.append("ort_rethrow()");
            std::unique_ptr<otava::ast::Node> invokeThrowExprNode = ParseExpression(throwExprStr, context);
            boundExpression = BindExpression(invokeThrowExprNode.get(), context);
        }
    }
    catch (const std::exception& ex)
    {
        ThrowException(std::string("error parsing throw expression: ") + ex.what(), node.GetFullSpan(), context);
    }
}

void ExpressionBinder::Visit(otava::ast::ConditionalExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::unique_ptr<BoundExpressionNode> condition = BindExpression(node.Condition(), context, true);
    context->PushSetFlag(ContextFlags::acquireTemporaryDestructorCalls);
    std::unique_ptr<BoundExpressionNode> thenExpr = BindExpression(node.ThenExpr(), context);
    std::unique_ptr<BoundExpressionNode> elseExpr = BindExpression(node.ElseExpr(), context);
    context->PopFlags();
    TypeSymbol* type = thenExpr->GetType();
    if (!TypesEqual(type, elseExpr->GetType(), context))
    {
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            type, elseExpr->GetType(), fullSpan, context);
        if (conversion)
        {
            TypeSymbol* type = conversion->ReturnType(context);
            elseExpr.reset(new BoundConversionNode(elseExpr.release(), conversion, fullSpan, type));
        }
        else
        {
            ThrowException("type of right expression (" + elseExpr->GetType()->FullName(context) +
                ") of conditional expression is not convertible to the type of the left expression (" + type->FullName(context) + ")", fullSpan, context);
        }
    }
    BoundConditionalExprNode* boundConditionalExpr = new BoundConditionalExprNode(condition.release(), thenExpr.release(), elseExpr.release(), type);
    VariableSymbol* temporary = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(type, node.Id(), context);
    boundConditionalExpr->SetTemporary(new BoundVariableNode(temporary, fullSpan, temporary->GetReferredType(context)));
    boundExpression.reset(boundConditionalExpr);
}

void ExpressionBinder::Visit(otava::ast::OperatorFunctionIdNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    boundExpression.reset(new BoundOperatorFnNode(node.Right()->Kind(), fullSpan));
}

void ExpressionBinder::Visit(otava::ast::InitConditionNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    Declaration declaration = ProcessInitCondition(&node, context);
    Declarator* declarator = declaration.declarator.get();
    if (declarator->Kind() == DeclaratorKind::simpleDeclarator)
    {
        SimpleDeclarator* simpleDeclarator = static_cast<SimpleDeclarator*>(declarator);
        if (!simpleDeclarator->Name().empty())
        {
            TypeSymbol* initializerType = nullptr;
            if (declaration.initializer)
            {
                std::unique_ptr<BoundExpressionNode> initializer = BindExpression(declaration.initializer, context);
                initializerType = initializer->GetType();
            }
            VariableSymbol* variable = ProcessSimpleDeclarator(simpleDeclarator, declaration.type, initializerType, declaration.value, declaration.flags, context);
            declaration.variable = variable;
        }
        else
        {
            ThrowException("declarator has no name", fullSpan, context);
        }
    }
    else
    {
        ThrowException("simple declarator expected", fullSpan, context);
    }
    VariableSymbol* variable = declaration.variable;
    std::unique_ptr<BoundExpressionNode> initializer = BindExpression(node.Initializer(), context);
    TypeSymbol* initializerType = initializer->GetType()->DirectType(context)->FinalType(fullSpan, context);
    if (variable->GetDeclaredType(context)->GetBaseType(context)->IsAutoTypeSymbol() && variable->GetDeclaredType(context)->GetDerivations() != Derivations::none)
    {
        initializerType = context->GetSymbolTable()->MakeCompoundType(initializerType->GetBaseType(context),
            variable->GetDeclaredType(context)->GetDerivations(), context);
    }
    variable->SetInitializerType(initializerType, context);
    BoundVariableNode* boundVariable = new BoundVariableNode(variable, fullSpan, variable->GetReferredType(context));
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
    arguments.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundVariable, fullSpan,
        boundVariable->GetType()->AddPointer(context))));
    arguments.push_back(move(initializer));
    std::vector<TypeSymbol*> templateArgs;
    context->PushNodeId(node.Id());
    std::unique_ptr<BoundFunctionCallNode> constructorCall = ResolveOverloadThrow(context->GetSymbolTable()->CurrentScope(),
        "@constructor", templateArgs, arguments, fullSpan, context);
    context->PopNodeId();
    context->GetBoundFunction()->GetFunctionDefinitionSymbol()->AddLocalVariable(variable, context);
    BoundExpressionSequenceNode* expressionSequence = new BoundExpressionSequenceNode(fullSpan, constructorCall.release(), boundVariable->Clone());
    boundExpression.reset(expressionSequence);
}

void ExpressionBinder::Visit(otava::ast::PostfixIncExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    bool prevInhibitCompile = inhibitCompile;
    inhibitCompile = true;
    node.Child()->Accept(*this);
    inhibitCompile = prevInhibitCompile;
    if (!inhibitCompile)
    {
        if (boundExpression->GetType()->PlainType(context)->IsClassTypeSymbol())
        {
            std::vector<std::unique_ptr<BoundExpressionNode>> args;
            if (boundExpression->GetType()->IsReferenceType())
            {
                TypeSymbol* type = boundExpression->GetType()->RemoveReference(context)->AddPointer(context);
                args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundRefToPtrNode(boundExpression.release(), fullSpan, type)));
            }
            else
            {
                TypeSymbol* type = boundExpression->GetType()->AddPointer(context);
                args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundExpression.release(), fullSpan, type)));
            }
            args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundTypeNode(
                context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context), fullSpan)));
            std::vector<TypeSymbol*> templateArgs;
            context->PushNodeId(node.Id());
            std::unique_ptr<BoundFunctionCallNode> functionCall = ResolveOverloadThrow(scope, "operator++", templateArgs, args, fullSpan, context);
            context->PopNodeId();
            boundExpression.reset(functionCall.release());
        }
        else
        {
            TypeSymbol* type = boundExpression->GetType();
            VariableSymbol* temporary = context->GetSymbolTable()->AddVariable(context->GetBoundFunction()->GetFunctionDefinitionSymbol()->NextTemporaryName(),
                &node, type, type, nullptr, DeclarationFlags::none, context);
            std::string temporaryName = temporary->Name();
            otava::ast::BinaryExprNode expr(fullSpan.span, fullSpan.fileIndex,
                new otava::ast::CommaNode(fullSpan.span, fullSpan.fileIndex),
                new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex,
                    new otava::ast::CommaNode(fullSpan.span, fullSpan.fileIndex),
                    new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex,
                        new otava::ast::AssignNode(fullSpan.span, fullSpan.fileIndex),
                        new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, temporaryName),
                        node.Child()->Clone()),
                    new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex,
                        new otava::ast::AssignNode(fullSpan.span, fullSpan.fileIndex),
                        node.Child()->Clone(),
                        new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex, new otava::ast::PlusNode(fullSpan.span, fullSpan.fileIndex),
                            node.Child()->Clone(),
                            new otava::ast::IntegerLiteralNode(fullSpan.span, fullSpan.fileIndex, 1, otava::ast::Suffix::none, otava::ast::Base::decimal, std::string())))),
                new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, temporaryName));
            context->PushSetFlag(ContextFlags::suppress_warning);
            boundExpression = BindExpression(&expr, context);
            context->PopFlags();
        }
    }
}

void ExpressionBinder::Visit(otava::ast::PostfixDecExprNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    bool prevInhibitCompile = inhibitCompile;
    inhibitCompile = true;
    node.Child()->Accept(*this);
    inhibitCompile = prevInhibitCompile;
    if (!inhibitCompile)
    {
        if (boundExpression->GetType()->PlainType(context)->IsClassTypeSymbol())
        {
            std::vector<std::unique_ptr<BoundExpressionNode>> args;
            if (boundExpression->GetType()->IsReferenceType())
            {
                TypeSymbol* type = boundExpression->GetType()->RemoveReference(context)->AddPointer(context);
                args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundRefToPtrNode(boundExpression.release(), fullSpan, type)));
            }
            else
            {
                TypeSymbol* type = boundExpression->GetType()->AddPointer(context);
                args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundExpression.release(), fullSpan, type)));
            }
            args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundTypeNode(
                context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context), fullSpan)));
            std::vector<TypeSymbol*> templateArgs;
            context->PushNodeId(node.Id());
            std::unique_ptr<BoundFunctionCallNode> functionCall = ResolveOverloadThrow(scope, "operator--", templateArgs, args, fullSpan, context);
            context->PopNodeId();
            boundExpression.reset(functionCall.release());
        }
        else
        {
            TypeSymbol* type = boundExpression->GetType();
            VariableSymbol* temporary = context->GetSymbolTable()->AddVariable(context->GetBoundFunction()->GetFunctionDefinitionSymbol()->NextTemporaryName(),
                &node, type, type, nullptr, DeclarationFlags::none, context);
            std::string temporaryName = temporary->Name();
            otava::ast::BinaryExprNode expr(fullSpan.span, fullSpan.fileIndex,
                new otava::ast::CommaNode(fullSpan.span, fullSpan.fileIndex),
                new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex,
                    new otava::ast::CommaNode(fullSpan.span, fullSpan.fileIndex),
                    new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex,
                        new otava::ast::AssignNode(fullSpan.span, fullSpan.fileIndex),
                        new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, temporaryName),
                        node.Child()->Clone()),
                    new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex,
                        new otava::ast::AssignNode(fullSpan.span, fullSpan.fileIndex),
                        node.Child()->Clone(),
                        new otava::ast::BinaryExprNode(fullSpan.span, fullSpan.fileIndex, new otava::ast::MinusNode(fullSpan.span, fullSpan.fileIndex),
                            node.Child()->Clone(),
                            new otava::ast::IntegerLiteralNode(fullSpan.span, fullSpan.fileIndex, 1, otava::ast::Suffix::none, otava::ast::Base::decimal, std::string())))),
                new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, temporaryName));
            boundExpression = BindExpression(&expr, context);
        }
    }
}

std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context, SymbolGroupKind symbolGroups, Scope*& scope)
{
    ExpressionBinder binder(context, symbolGroups);
    node->Accept(binder);
    scope = binder.GetScope();
    return binder.GetBoundExpression();
}

std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context)
{
    return BindExpression(node, context, false);
}

std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context, bool booleanChild)
{
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    Scope* scope = nullptr;
    bool acquireTemporaryDestructorCalls = context->GetFlag(ContextFlags::acquireTemporaryDestructorCalls);
    if (acquireTemporaryDestructorCalls)
    {
        context->PushResetFlag(ContextFlags::acquireTemporaryDestructorCalls);
    }
    std::unique_ptr<BoundExpressionNode> expr = BindExpression(node, context, SymbolGroupKind::all, scope);
    if (booleanChild && !expr->GetType()->IsBoolType())
    {
        FunctionSymbol* conversionFunction = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(otava::symbols::FundamentalTypeKind::boolType, context), expr->GetType(),
            fullSpan, context);
        if (!conversionFunction)
        {
            ThrowException("expression must be convertible to Boolean type value", expr->GetFullSpan(), context);
        }
        if (conversionFunction->IsFunctionDefinitionSymbol())
        {
            std::unique_ptr<BoundFunctionCallNode> fnCall = CreateBoundConversionFunctionCall(conversionFunction, expr.release(), fullSpan, context);
            expr.reset(fnCall.release());
        }
        else
        {
            expr.reset(new BoundConversionNode(expr.release(), conversionFunction, fullSpan, conversionFunction->ReturnType(context)));
        }
    }
    if (expr)
    {
        expr->SetSource(node->Clone());
    }
    if (acquireTemporaryDestructorCalls)
    {
        if (!context->GetFlag(ContextFlags::invoke))
        {
            if (context->GetBoundFunction()->HasTemporaryDestructorCalls())
            {
                expr->SetDestructTemporariesNode(
                    new BoundDestructTemporariesNode(fullSpan, context->GetBoundFunction()->GetTemporaryDestructorCalls()));
                expr->SetTemporaryDestructorCallsObtained();
            }
        }
        context->PopFlags();
    }
    return expr;
}

bool MultiplicativeRightIdOperandNotFound(otava::ast::Node* op, otava::ast::Node* rightOperand, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!op->IsMulNode()) return false;
    if (!rightOperand->IsInvokeExprNode()) return false;
    otava::ast::InvokeExprNode* rightinvoke = static_cast<otava::ast::InvokeExprNode*>(rightOperand);
    if (!rightinvoke->Subject()->IsIdentifierNode()) return false;
    otava::ast::IdentifierNode* id = static_cast<otava::ast::IdentifierNode*>(rightinvoke->Subject());
    Symbol* symbol = context->GetSymbolTable()->CurrentScope()->Lookup(id->Str(), SymbolGroupKind::all, ScopeLookup::allScopes, fullSpan, context, LookupFlags::none);
    if (!symbol) return true;
    return false;
}

} // namespace otava::symbols
