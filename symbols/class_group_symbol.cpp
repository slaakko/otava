// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.class_group_symbol;

import otava.symbols.classes;

namespace otava::symbols {

ClassGroupSymbol::ClassGroupSymbol(Module* module_, SymbolId id_) : Symbol(module_, id_)
{
}

ClassGroupSymbol::ClassGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : Symbol(module_, id_, name_)
{
}

bool ClassGroupSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::templateDeclarationScope:
    case ScopeKind::classScope:
    case ScopeKind::blockScope:
    {
        return true;
    }
    }
    return false;
}

Symbol* ClassGroupSymbol::GetSingleSymbol(Context* contex) noexcept
{
    if (classes.size() == 1)
    {
        Symbol* front = classes.front();
        return front;
    }
    else if (forwardDeclarations.size() == 1)
    {
        Symbol* front = forwardDeclarations.front();
        return front;
    }
    else
    {
        return this;
    }
}

} // namespace otava::symbols
