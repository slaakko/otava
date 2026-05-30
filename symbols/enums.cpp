// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.enums;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.evaluator;
import otava.symbols.exception;
import otava.symbols.scope_resolver;
import otava.symbols.type_resolver;
import otava.symbols.value;
import otava.ast.enums;
import otava.ast.identifier;
import otava.ast.visitor;

namespace otava::symbols {

EnumeratedTypeSymbol::EnumeratedTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_), underlyingType(nullptr)
{
    GetScope()->SetKind(ScopeKind::enumerationScope);
}

EnumeratedTypeSymbol::EnumeratedTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_), underlyingType(nullptr)
{
    GetScope()->SetKind(ScopeKind::enumerationScope);
}

bool EnumeratedTypeSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::classScope:
    {
        return true;
    }
    }
    return false;
}

TypeSymbol* EnumeratedTypeSymbol::UnderlyingType() const 
{
    // TODO
    return nullptr;
}

otava::intermediate::Type* EnumeratedTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (underlyingType)
    {
        return underlyingType->IrType(emitter, fullSpan, context);
    }
    else
    {
        return emitter.GetIntType();
    }
}

class EnumCreator : public otava::ast::DefaultVisitor
{
public:
    EnumCreator(Context* context_, bool createEnumeratedType_, bool createEnumerators_);
    void Visit(otava::ast::EnumSpecifierNode& node) override;
    void Visit(otava::ast::EnumHeadNode& node) override;
    void Visit(otava::ast::EnumBaseNode& node) override;
    void Visit(otava::ast::EnumClassNode& node) override;
    void Visit(otava::ast::EnumStructNode& node) override;
    void Visit(otava::ast::EnumNode& node) override;
    void Visit(otava::ast::EnumeratorDefinitionNode& node) override;
    void Visit(otava::ast::EnumeratorNode& node) override;
    void Visit(otava::ast::ElaboratedEnumSpecifierNode& node) override;
    void Visit(otava::ast::OpaqueEnumDeclarationNode& node) override;
    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::UnnamedNode& node) override;
private:
    Context* context;
    EnumTypeKind enumTypeKind;
    TypeSymbol* underlyingType;
    Scope* scope;
    Value* value;
    std::int64_t prevValue;
    bool first;
    bool createEnumeratedType;
    bool createEnumerators;
    bool opaque;
};

EnumCreator::EnumCreator(Context* context_, bool createEnumeratedType_, bool createEnumerators_) :
    context(context_),
    enumTypeKind(EnumTypeKind::enum_),
    underlyingType(nullptr),
    scope(context->GetSymbolTable()->CurrentScope()),
    value(nullptr),
    prevValue(0),
    first(false),
    createEnumeratedType(createEnumeratedType_),
    createEnumerators(createEnumerators_),
    opaque(false)
{
}

void EnumCreator::Visit(otava::ast::EnumSpecifierNode& node)
{
    if (createEnumeratedType)
    {
        node.EnumHead()->Accept(*this);
    }
    else if (createEnumerators)
    {
        first = true;
        VisitListContent(node);
    }
}

void EnumCreator::Visit(otava::ast::EnumHeadNode& node)
{
    node.EnumKey()->Accept(*this);
    if (node.EnumBase())
    {
        node.EnumBase()->Accept(*this);
    }
    node.EnumHeadName()->Accept(*this);
}

void EnumCreator::Visit(otava::ast::EnumBaseNode& node)
{
    underlyingType = ResolveType(node.Child(), DeclarationFlags::none, context);
}

void EnumCreator::Visit(otava::ast::EnumClassNode& node)
{
    enumTypeKind = EnumTypeKind::enumClass;
}

void EnumCreator::Visit(otava::ast::EnumStructNode& node)
{
    enumTypeKind = EnumTypeKind::enumStruct;
}

void EnumCreator::Visit(otava::ast::EnumNode& node)
{
    enumTypeKind = EnumTypeKind::enum_;
}

void EnumCreator::Visit(otava::ast::EnumeratorDefinitionNode& node)
{
    Symbol* currentSymbol = context->GetSymbolTable()->CurrentScope()->GetSymbol();
    if (!currentSymbol->IsEnumeratedTypeSymbol())
    {
        ThrowException("otava.symbols.enums: EnumCreator(): enum scope expected", node.GetFullSpan(), context);
    }
    EnumeratedTypeSymbol* enumType = static_cast<EnumeratedTypeSymbol*>(currentSymbol);
    TypeSymbol* valueType = enumType->UnderlyingType();
    if (!valueType)
    {
        valueType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context);
    }
    if (node.GetValue())
    {
        Value* val = Evaluate(node.GetValue(), context);
        if (val->IsIntegerValue())
        {
            IntegerValue* intVal = static_cast<IntegerValue*>(val);
            value = context->GetEvaluationContext()->GetIntegerValue(intVal->GetValue(), std::to_string(intVal->GetValue()), valueType);
        }
        if (value && value->IsIntegerValue())
        {
            IntegerValue* integerValue = static_cast<IntegerValue*>(value);
            prevValue = integerValue->GetValue();
        }
        if (first)
        {
            first = false;
        }
    }
    else
    {
        if (first)
        {
            value = context->GetEvaluationContext()->GetIntegerValue(0, std::to_string(0), valueType);
            prevValue = 0;
            first = false;
        }
        else
        {
            value = context->GetEvaluationContext()->GetIntegerValue(prevValue + 1, std::to_string(prevValue + 1), valueType);
            ++prevValue;
        }
    }
    node.Enumerator()->Accept(*this);
}

void EnumCreator::Visit(otava::ast::EnumeratorNode& node)
{
    node.Identifier()->Accept(*this);
}

void EnumCreator::Visit(otava::ast::ElaboratedEnumSpecifierNode& node)
{
    opaque = true;
    node.Child()->Accept(*this);
    opaque = false;
}

void EnumCreator::Visit(otava::ast::OpaqueEnumDeclarationNode& node)
{
    opaque = true;
    node.EnumKey()->Accept(*this);
    if (node.EnumBase())
    {
        node.EnumBase()->Accept(*this);
    }
    node.EnumHeadName()->Accept(*this);
    opaque = false;
}

void EnumCreator::Visit(otava::ast::QualifiedIdNode& node)
{
    scope = ResolveScope(node.Left(), context);
    node.Right()->Accept(*this);
}

void EnumCreator::Visit(otava::ast::IdentifierNode& node)
{
    if (createEnumeratedType)
    {
        context->GetSymbolTable()->BeginScope(scope);
        if (opaque)
        {
            context->GetSymbolTable()->AddForwardEnumDeclaration(node.Str(), enumTypeKind, underlyingType, &node, context);
        }
        else
        {
            context->GetSymbolTable()->BeginEnumeratedType(node.Str(), enumTypeKind, underlyingType, &node, context);
        }
    }
    if (createEnumerators)
    {
        context->GetSymbolTable()->AddEnumerator(node.Str(), value, &node, context);
    }
}

void EnumCreator::Visit(otava::ast::UnnamedNode& node)
{
    context->GetSymbolTable()->BeginScope(scope);
    context->GetSymbolTable()->BeginEnumeratedType(std::string(), enumTypeKind, underlyingType, &node, context);
}

void BeginEnumType(otava::ast::Node* node, Context* context)
{
    EnumCreator creator(context, true, false);
    node->Accept(creator);
}

void AddEnumerators(otava::ast::Node* node, Context* context)
{
    EnumCreator creator(context, false, true);
    node->Accept(creator);
}

void EndEnumType(otava::ast::Node* node, Context* context)
{
    Symbol* currentSymbol = context->GetSymbolTable()->CurrentScope()->GetSymbol();
    if (!currentSymbol->IsEnumeratedTypeSymbol())
    {
        ThrowException("cpp20.symbols.enums: EndEnumeratedType(): enum scope expected", node->GetFullSpan(), context);
    }
    EnumeratedTypeSymbol* enumType = static_cast<EnumeratedTypeSymbol*>(currentSymbol);
    context->GetSymbolTable()->EndEnumeratedType();
    context->GetSymbolTable()->EndScope();
    BindEnumType(enumType, node->GetFullSpan(), context);
}

void BindEnumType(EnumeratedTypeSymbol* enumType, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
}

void ProcessEnumForwardDeclaration(otava::ast::Node* node, Context* context)
{
    EnumCreator creator(context, true, false);
    node->Accept(creator);
    context->GetSymbolTable()->EndScope();
}


} // namespace otava::symbols
