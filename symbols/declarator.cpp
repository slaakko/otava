// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.declarator;

import otava.symbols.context;
import otava.symbols.declaration;
import otava.symbols.exception;
import otava.symbols.evaluator;
import otava.symbols.function_type_symbol;
import otava.symbols.scope_resolver;
import otava.symbols.type_resolver;
import otava.ast.classes;
import otava.ast.declaration;
import otava.ast.expression;
import otava.ast.function;
import otava.ast.identifier;
import otava.ast.punctuation;
import otava.ast.templates;
import otava.ast.visitor;

namespace otava::symbols {

Declarator::Declarator(DeclaratorKind kind_, const std::string& name_, otava::ast::Node* node_) : kind(kind_), name(name_), node(node_)
{
}

Declarator::~Declarator()
{
}

SimpleDeclarator::SimpleDeclarator(const std::string& name_, otava::ast::Node* node_) : Declarator(DeclaratorKind::simpleDeclarator, name_, node_)
{
}

FunctionDeclarator::FunctionDeclarator(const std::string& name_, otava::ast::Node* node_, FunctionKind kind_, FunctionQualifiers qualifiers_, Scope* scope_) :
    Declarator(DeclaratorKind::functionDeclarator, name_, node_), scope(scope_), kind(kind_), qualifiers(qualifiers_)
{
}

void FunctionDeclarator::AddParameterDeclaration(Declaration&& parameterDeclaration)
{
    parameterDeclarations.push_back(std::move(parameterDeclaration));
}

void FunctionDeclarator::AddQualifier(FunctionQualifiers qualifier)
{
    qualifiers = qualifiers | qualifier;
}

void FunctionDeclarator::SetTemplateArgs(const std::vector<TypeSymbol*>& templateArgs_)
{
    templateArgs = templateArgs_;
}

ArrayDeclarator::ArrayDeclarator(const std::string& name_, otava::ast::Node* node_, std::int64_t size_) :
    Declarator(DeclaratorKind::arrayDeclarator, name_, node_), size(size_)
{
}

Declaration::Declaration() noexcept : flags(DeclarationFlags::none), type(nullptr), declarator(nullptr), value(nullptr), initializer(nullptr), variable(nullptr)
{
}

Declaration::Declaration(DeclarationFlags flags_, TypeSymbol* type_, Declarator* declarator_) noexcept :
    flags(flags_), type(type_), declarator(declarator_), value(nullptr), initializer(nullptr), variable(nullptr)
{
}

class DeclaratorListProcessor : public otava::ast::DefaultVisitor
{
public:
    DeclaratorListProcessor(Context* context_, TypeSymbol* baseType_, DeclarationFlags flags_, otava::ast::Node* declarationNode_);
    std::unique_ptr<DeclarationList> GetDeclarations() { return std::move(declarationList); }
    void Visit(otava::ast::InitDeclaratorListNode& node) override;
    void Visit(otava::ast::MemberDeclaratorListNode& node) override;
private:
    Context* context;
    TypeSymbol* baseType;
    DeclarationFlags flags;
    std::unique_ptr<DeclarationList> declarationList;
    otava::ast::Node* declarationNode;
};

DeclaratorListProcessor::DeclaratorListProcessor(Context* context_, TypeSymbol* baseType_, DeclarationFlags flags_, otava::ast::Node* declarationNode_) :
    context(context_), baseType(baseType_), flags(flags_), declarationList(new DeclarationList()), declarationNode(declarationNode_)
{
}

void DeclaratorListProcessor::Visit(otava::ast::InitDeclaratorListNode& node)
{
    std::int32_t n = node.Items().size();
    for (std::int32_t i = 0; i < n; ++i)
    {
        otava::ast::Node* declarator = node.Items()[i];
        Declaration declaration = ProcessDeclarator(baseType, declarator, declarationNode, flags, FunctionQualifiers::none, context);
        if (declaration.declarator.get())
        {
            declarationList->declarations.push_back(std::move(declaration));
        }
    }
}

void DeclaratorListProcessor::Visit(otava::ast::MemberDeclaratorListNode& node)
{
    std::int32_t n = node.Items().size();
    for (std::int32_t i = 0; i < n; ++i)
    {
        otava::ast::Node* declarator = node.Items()[i];
        Declaration declaration = ProcessDeclarator(baseType, declarator, declarationNode, flags, FunctionQualifiers::none, context);
        if (declaration.declarator.get())
        {
            declarationList->declarations.push_back(std::move(declaration));
        }
    }
}

class DeclaratorProcessor : public otava::ast::DefaultVisitor
{
public:
    DeclaratorProcessor(Context* context_, DeclarationFlags flags_, TypeSymbol* baseType_, FunctionQualifiers qualifiers_, otava::ast::Node* declarationNode_);
    void SetProcessTrailingQualifiers(bool processTrailingQualifiers_) { processTrailingQualifiers = processTrailingQualifiers_; }
    Declaration GetDeclaration() { return std::move(declaration); }
    void Visit(otava::ast::FunctionDeclaratorNode& node) override;
    void Visit(otava::ast::ParenthesizedDeclaratorNode& node) override;
    void Visit(otava::ast::TrailingQualifiersNode& node) override;
    void Visit(otava::ast::InitDeclaratorNode& node) override;
    void Visit(otava::ast::ParameterListNode& node) override;
    void Visit(otava::ast::ParameterNode& node) override;
    void Visit(otava::ast::DestructorIdNode& node) override;
    void Visit(otava::ast::ConversionFunctionIdNode& node) override;
    void Visit(otava::ast::ConversionTypeIdNode& node) override;
    void Visit(otava::ast::OperatorFunctionIdNode& node) override;
    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::TemplateIdNode& node) override;
    void Visit(otava::ast::AbstractDeclaratorNode& node) override;
    void Visit(otava::ast::ArrayDeclaratorNode& node) override;
    void Visit(otava::ast::ConstNode& node) override;
    void Visit(otava::ast::VolatileNode& node) override;
    void Visit(otava::ast::FinalNode& node) override;
    void Visit(otava::ast::OverrideNode& node) override;
    void Visit(otava::ast::PureSpecifierNode& node) override;
    void Visit(otava::ast::LvalueRefNode& node) override;
    void Visit(otava::ast::RvalueRefNode& node) override;
    void Visit(otava::ast::PtrNode& node) override;
    void Visit(otava::ast::NoexceptSpecifierNode& node) override;
    void Visit(otava::ast::NewArrayOpNode& node) override;
    void Visit(otava::ast::NewOpNode& node) override;
    void Visit(otava::ast::DeleteArrayOpNode& node) override;
    void Visit(otava::ast::DeleteOpNode& node) override;
    void Visit(otava::ast::CoAwaitOpNode& node) override;
    void Visit(otava::ast::InvokeOpNode& node) override;
    void Visit(otava::ast::SubscriptOpNode& node) override;
    void Visit(otava::ast::ArrowNode& node) override;
    void Visit(otava::ast::ArrowStarNode& node) override;
    void Visit(otava::ast::ComplementNode& node) override;
    void Visit(otava::ast::NotNode& node) override;
    void Visit(otava::ast::PlusNode& node) override;
    void Visit(otava::ast::MinusNode& node) override;
    void Visit(otava::ast::MulNode& node) override;
    void Visit(otava::ast::DivNode& node) override;
    void Visit(otava::ast::ModNode& node) override;
    void Visit(otava::ast::ExclusiveOrNode& node) override;
    void Visit(otava::ast::AndNode& node) override;
    void Visit(otava::ast::InclusiveOrNode& node) override;
    void Visit(otava::ast::AssignNode& node) override;
    void Visit(otava::ast::PlusAssignNode& node) override;
    void Visit(otava::ast::MinusAssignNode& node) override;
    void Visit(otava::ast::MulAssignNode& node) override;
    void Visit(otava::ast::DivAssignNode& node) override;
    void Visit(otava::ast::ModAssignNode& node) override;
    void Visit(otava::ast::XorAssignNode& node) override;
    void Visit(otava::ast::AndAssignNode& node) override;
    void Visit(otava::ast::OrAssignNode& node) override;
    void Visit(otava::ast::ShiftLeftAssignNode& node) override;
    void Visit(otava::ast::ShiftRightAssignNode& node) override;
    void Visit(otava::ast::EqualNode& node) override;
    void Visit(otava::ast::NotEqualNode& node) override;
    void Visit(otava::ast::LessOrEqualNode& node) override;
    void Visit(otava::ast::GreaterOrEqualNode& node) override;
    void Visit(otava::ast::CompareNode& node) override;
    void Visit(otava::ast::LessNode& node) override;
    void Visit(otava::ast::GreaterNode& node) override;
    void Visit(otava::ast::ConjunctionNode& node) override;
    void Visit(otava::ast::DisjunctionNode& node) override;
    void Visit(otava::ast::ShiftLeftNode& node) override;
    void Visit(otava::ast::ShiftRightNode& node) override;
    void Visit(otava::ast::PrefixIncNode& node) override;
    void Visit(otava::ast::PrefixDecNode& node) override;
    void Visit(otava::ast::CommaNode& node) override;
private:
    Context* context;
    DeclarationFlags flags;
    TypeSymbol* baseType;
    TypeSymbol* returnType;
    std::vector<TypeSymbol*> parameterTypes;
    Declaration declaration;
    bool isFunctionDeclarator;
    bool isDestructorDeclarator;
    bool skipIdFunctionDeclarator;
    bool operatorFunctionId;
    bool trailingQualifiers;
    bool processTrailingQualifiers;
    bool processArrayId;
    bool processingFunctionPtrType;
    FunctionQualifiers qualifiers;
    FunctionDeclarator* functionDeclarator;
    Scope* scope;
    std::string arrayName;
    otava::ast::Node* declarationNode;
};

DeclaratorProcessor::DeclaratorProcessor(Context* context_, DeclarationFlags flags_, TypeSymbol* baseType_, FunctionQualifiers qualifiers_,
    otava::ast::Node* declarationNode_) :
    context(context_),
    flags(flags_),
    baseType(baseType_),
    returnType(nullptr),
    isFunctionDeclarator(false),
    isDestructorDeclarator(false),
    skipIdFunctionDeclarator(false),
    operatorFunctionId(false),
    trailingQualifiers(false),
    processTrailingQualifiers(false),
    processArrayId(false),
    processingFunctionPtrType(false),
    scope(context->GetSymbolTable()->CurrentScope()),
    functionDeclarator(),
    qualifiers(qualifiers_),
    declarationNode(declarationNode_)
{
}

void DeclaratorProcessor::Visit(otava::ast::FunctionDeclaratorNode& node)
{
    processTrailingQualifiers = true;
    node.Params()->Accept(*this);
    processTrailingQualifiers = false;
    isFunctionDeclarator = true;
    node.Child()->Accept(*this);
    if (node.Params())
    {
        node.Params()->Accept(*this);
    }
    isFunctionDeclarator = false;
    if (processingFunctionPtrType)
    {
        TypeSymbol* fnType = context->GetSymbolTable()->MakeFunctionTypeSymbol(returnType, parameterTypes, nullptr, true, context);
        declaration.type = fnType->AddPointer(context);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ParenthesizedDeclaratorNode& node)
{
    if (isFunctionDeclarator)
    {
        isFunctionDeclarator = false;
        returnType = baseType;
        processingFunctionPtrType = true;
    }
    declaration = ProcessDeclarator(baseType, node.Declarator(), declarationNode, flags, FunctionQualifiers::none, context);
}

void DeclaratorProcessor::Visit(otava::ast::TrailingQualifiersNode& node)
{
    if (!processTrailingQualifiers)
    {
        node.Subject()->Accept(*this);
        return;
    }
    trailingQualifiers = true;
    VisitSequence(node);
    trailingQualifiers = false;
}

void DeclaratorProcessor::Visit(otava::ast::InitDeclaratorNode& node)
{
    node.Left()->Accept(*this);
    if (node.Right())
    {
        context->SetDeclaredInitializerType(declaration.type);
        Value* value = Evaluate(node.Right(), context);
        declaration.value = value;
        declaration.initializer = node.Right();
    }
}

void DeclaratorProcessor::Visit(otava::ast::ParameterListNode& node)
{
    if (processTrailingQualifiers) return;
    int n = node.Items().size();
    for (int i = 0; i < n; ++i)
    {
        otava::ast::Node* item = node.Items()[i];
        item->Accept(*this);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ParameterNode& node)
{
    Declaration declaration = ProcessParameterDeclaration(&node, context);
    if (functionDeclarator)
    {
        functionDeclarator->AddParameterDeclaration(std::move(declaration));
        declaration = Declaration();
    }
    if (processingFunctionPtrType)
    {
        parameterTypes.push_back(declaration.type);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ConstNode& node)
{
    if (!trailingQualifiers) return;
    qualifiers = qualifiers | FunctionQualifiers::isConst;
}

void DeclaratorProcessor::Visit(otava::ast::VolatileNode& node)
{
    if (!trailingQualifiers) return;
    qualifiers = qualifiers | FunctionQualifiers::isVolatile;
}

void DeclaratorProcessor::Visit(otava::ast::FinalNode& node)
{
    if (!trailingQualifiers) return;
    qualifiers = qualifiers | FunctionQualifiers::isFinal;
}

void DeclaratorProcessor::Visit(otava::ast::PureSpecifierNode& node)
{
    if (!trailingQualifiers) return;
    qualifiers = qualifiers | FunctionQualifiers::isPure;
}

void DeclaratorProcessor::Visit(otava::ast::OverrideNode& node)
{
    if (!trailingQualifiers) return;
    qualifiers = qualifiers | FunctionQualifiers::isOverride;
}

void DeclaratorProcessor::Visit(otava::ast::LvalueRefNode& node)
{
    Derivations derivations = Derivations::lvalueRefDerivation;
    if (baseType)
    {
        baseType = context->GetSymbolTable()->MakeCompoundType(baseType, derivations, context);
    }
}

void DeclaratorProcessor::Visit(otava::ast::RvalueRefNode& node)
{
    Derivations derivations = Derivations::none;
    if (!baseType || !baseType->IsLValueRefType())
    {
        derivations = derivations | Derivations::rvalueRefDerivation;
    }
    if (baseType)
    {
        baseType = context->GetSymbolTable()->MakeCompoundType(baseType, derivations, context);
    }
}

void DeclaratorProcessor::Visit(otava::ast::PtrNode& node)
{
    Derivations derivations = Derivations::none;
    derivations = otava::symbols::SetPointerCount(derivations, 1);
    if (baseType)
    {
        baseType = context->GetSymbolTable()->MakeCompoundType(baseType, derivations, context);
    }
}

void DeclaratorProcessor::Visit(otava::ast::NoexceptSpecifierNode& node)
{
    if (!trailingQualifiers) return;
    bool isNoExcept = true;
    if (node.ConstantExpr())
    {
        Value* value = otava::symbols::Evaluate(node.ConstantExpr(), context);
        if (value->IsBoolValue())
        {
            BoolValue* result = static_cast<BoolValue*>(value);
            if (!result->GetValue())
            {
                isNoExcept = false;
            }
        }
    }
    if (isNoExcept)
    {
        qualifiers = qualifiers | FunctionQualifiers::isNoexcept;
    }
}

void DeclaratorProcessor::Visit(otava::ast::DestructorIdNode& node)
{
    isDestructorDeclarator = true;
    node.Child()->Accept(*this);
    isDestructorDeclarator = false;
}

void DeclaratorProcessor::Visit(otava::ast::ConversionFunctionIdNode& node)
{
    node.Right()->Accept(*this);
    functionDeclarator = new FunctionDeclarator("operator_" + baseType->IrName(context), &node, FunctionKind::conversionMemFn, qualifiers, scope);
    declaration = Declaration(flags, baseType, functionDeclarator);
}

void DeclaratorProcessor::Visit(otava::ast::ConversionTypeIdNode& node)
{
    baseType = ResolveType(node.TypeSpecifierSeq(), DeclarationFlags::none, context);
    if (node.ConversionDeclarator())
    {
        node.ConversionDeclarator()->Accept(*this);
    }
}

void DeclaratorProcessor::Visit(otava::ast::OperatorFunctionIdNode& node)
{
    operatorFunctionId = true;
    skipIdFunctionDeclarator = true;
    node.Right()->Accept(*this);
    skipIdFunctionDeclarator = false;
    operatorFunctionId = false;
    if (functionDeclarator)
    {
        declaration = Declaration(flags, baseType, functionDeclarator);
    }
}

void DeclaratorProcessor::Visit(otava::ast::NewArrayOpNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator new[]", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::NewOpNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator new", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::DeleteArrayOpNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator delete[]", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::DeleteOpNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator delete", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::CoAwaitOpNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator co_await", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::InvokeOpNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator()", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::SubscriptOpNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator[]", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ArrowNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator->", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ArrowStarNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator->*", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ComplementNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator~", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::NotNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator!", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::PlusNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator+", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::MinusNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator-", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::MulNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator*", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::DivNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator/", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ModNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator%", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ExclusiveOrNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator^", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::AndNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator&", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::InclusiveOrNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator|", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::AssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator=", &node, FunctionKind::special, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::PlusAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator+=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::MinusAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator-=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::MulAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator*=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::DivAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator/=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ModAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator%=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::XorAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator^=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::AndAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator&=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::OrAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator|=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ShiftLeftAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator<<=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ShiftRightAssignNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator>>=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::EqualNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator==", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::NotEqualNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator!=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::LessOrEqualNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator<=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::GreaterOrEqualNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator>=", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::CompareNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator<=>", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::LessNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator<", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::GreaterNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator>", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ConjunctionNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator&&", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::DisjunctionNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator||", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ShiftLeftNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator<<", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::ShiftRightNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator>>", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::PrefixIncNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator++", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::PrefixDecNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator--", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::CommaNode& node)
{
    if (operatorFunctionId)
    {
        functionDeclarator = new FunctionDeclarator("operator,", &node, FunctionKind::function, qualifiers, scope);
    }
}

void DeclaratorProcessor::Visit(otava::ast::QualifiedIdNode& node)
{
    if (processTrailingQualifiers) return;
    scope = ResolveScope(node.Left(), context);
    node.Right()->Accept(*this);
}

void DeclaratorProcessor::Visit(otava::ast::IdentifierNode& node)
{
    if (processTrailingQualifiers) return;
    if (processArrayId)
    {
        arrayName = node.Str();
    }
    else
    {
        if (!skipIdFunctionDeclarator)
        {
            if (isFunctionDeclarator)
            {
                if (isDestructorDeclarator)
                {
                    functionDeclarator = new FunctionDeclarator("~" + node.Str(), declarationNode, FunctionKind::destructor, qualifiers, scope);
                }
                else
                {
                    FunctionKind kind = FunctionKind::function;
                    Scope* classScope = context->GetSymbolTable()->CurrentScope()->GetClassScope(context);
                    if (classScope)
                    {
                        Symbol* symbol = classScope->GetSymbol();
                        if (symbol->IsClassTypeSymbol())
                        {
                            ClassTypeSymbol* classTypeSymbol = static_cast<ClassTypeSymbol*>(symbol);
                            if (node.Str() == classTypeSymbol->Name())
                            {
                                kind = FunctionKind::constructor;
                            }
                        }
                    }
                    functionDeclarator = new FunctionDeclarator(node.Str(), declarationNode, kind, qualifiers, scope);
                }
                declaration = Declaration(flags, baseType, functionDeclarator);
            }
            else
            {
                declaration = Declaration(flags, baseType, new SimpleDeclarator(node.Str(), &node));
            }
        }
    }
}

void DeclaratorProcessor::Visit(otava::ast::TemplateIdNode& node)
{
    node.TemplateName()->Accept(*this);
    std::vector<TypeSymbol*> templateArgs;
    for (otava::ast::Node* item : node.Items())
    {
        TypeSymbol* type = ResolveType(item, DeclarationFlags::none, context);
        templateArgs.push_back(type);
    }
    if (functionDeclarator)
    {
        functionDeclarator->SetTemplateArgs(templateArgs);
    }
}

void DeclaratorProcessor::Visit(otava::ast::AbstractDeclaratorNode& node)
{
    declaration = Declaration(flags, baseType, new SimpleDeclarator(std::string(), &node));
}

void DeclaratorProcessor::Visit(otava::ast::ArrayDeclaratorNode& node)
{
    processArrayId = true;
    node.Child()->Accept(*this);
    processArrayId = false;
    std::int64_t size = -1;
    if (node.Dimension())
    {
        Value* sizeValue = Evaluate(node.Dimension(), context);
        if (sizeValue->IsIntegerValue())
        {
            IntegerValue* integerValue = static_cast<IntegerValue*>(sizeValue);
            size = integerValue->GetValue();
        }
        else
        {
            ThrowException("integer value expected", node.GetFullSpan(), context);
        }
    }
    ArrayTypeSymbol* arrayType = context->GetSymbolTable()->MakeArrayType(baseType, size, context);
    arrayType->Bind(node.GetFullSpan(), context);
    declaration = Declaration(flags, arrayType, new ArrayDeclarator(arrayName, &node, size));
}

std::unique_ptr<DeclarationList> ProcessInitDeclaratorList(TypeSymbol* baseType, otava::ast::Node* declarationNode, otava::ast::Node* initDeclaratorList,
    DeclarationFlags flags, Context* context)
{
    DeclaratorListProcessor processor(context, baseType, flags, declarationNode);
    initDeclaratorList->Accept(processor);
    return processor.GetDeclarations();
}

std::unique_ptr<DeclarationList> ProcessMemberDeclaratorList(TypeSymbol* baseType, otava::ast::Node* declarationNode,
    otava::ast::Node* memberDeclaratorList, DeclarationFlags flags, Context* context)
{
    DeclaratorListProcessor processor(context, baseType, flags, declarationNode);
    memberDeclaratorList->Accept(processor);
    return processor.GetDeclarations();
}

Declaration ProcessDeclarator(TypeSymbol* baseType, otava::ast::Node* declarator, otava::ast::Node* declarationNode, DeclarationFlags flags,
    FunctionQualifiers qualifiers, Context* context)
{
    DeclaratorProcessor processor(context, flags, baseType, qualifiers, declarationNode);
    if (declarator->Kind() == otava::ast::NodeKind::trailingQualifiersNode)
    {
        processor.SetProcessTrailingQualifiers(true);
        declarator->Accept(processor);
        processor.SetProcessTrailingQualifiers(false);
    }
    declarator->Accept(processor);
    return processor.GetDeclaration();
}

} // namespace otava::symbols
