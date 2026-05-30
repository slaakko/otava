// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.function_group_symbol;

import otava.symbols.symbol;
import std;

export namespace otava::symbols {

class FunctionGroupSymbol : public Symbol
{
public:
    FunctionGroupSymbol(Module* module_, SymbolId id_);
    FunctionGroupSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    Symbol* GetSingleSymbol(Context* contex) noexcept override;
    void AddFunction(FunctionSymbol* function);
private:
    std::vector<SymbolId> functionIds;
    std::vector<FunctionSymbol*> functions;
};

} // namespace otava::symbols
