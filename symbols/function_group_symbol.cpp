// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_group_symbol;

import otava.symbols.function_symbol;
import otava.symbols.variable_symbol;

namespace otava::symbols {

FunctionGroupSymbol::FunctionGroupSymbol(Module* module_, SymbolId id_) : Symbol(module_, id_)
{
}

FunctionGroupSymbol::FunctionGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : Symbol(module_, id_, name_)
{
}

bool FunctionGroupSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::templateDeclarationScope:
    case ScopeKind::classScope:
    case ScopeKind::enumerationScope:
    case ScopeKind::arrayScope:
    {
        return true;
    }
    }
    return false;
}

Symbol* FunctionGroupSymbol::GetSingleSymbol(Context* contex) noexcept
{
    if (functions.size() == 1)
    {
        return functions.front();
    }
    else
    {
        return this;
    }
}

void FunctionGroupSymbol::AddFunction(FunctionSymbol* function)
{
    function->SetGroup(this);
    functions.push_back(function);
}

} // namespace otava::symbols
