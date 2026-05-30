// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.array_type_symbol;

namespace otava::symbols {

ArrayTypeSymbol::ArrayTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_), bound(false)
{
    GetScope()->SetKind(ScopeKind::arrayScope);
}

ArrayTypeSymbol::ArrayTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_), bound(false)
{
    GetScope()->SetKind(ScopeKind::arrayScope);
}

void ArrayTypeSymbol::Bind(const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
}

TypeSymbol* ArrayTypeSymbol::ElementType() const noexcept
{
    // TODO
    return nullptr;
}

bool ArrayTypeSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::classScope:
    case ScopeKind::functionScope:
    {
        return true;
    }
    }
    return false;
}

otava::intermediate::Type* ArrayTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
/*
    util::uuid irId = IrId(fullSpan, context);
    otava::intermediate::Type* type = emitter.GetType(irId);
    if (!type)
    {
        type = emitter.MakeArrayType(size, elementType->IrType(emitter, fullSpan, context));
        emitter.SetType(irId, type);
    }
    return type;
*/
    return nullptr;
}

} // namespace otava::symbols
