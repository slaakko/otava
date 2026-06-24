// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.variable_group_symbol;

import otava.symbols.symbol;
import std;

export namespace otava::symbols {

class VariableSymbol;

class VariableGroupSymbol : public Symbol
{
public:
    VariableGroupSymbol(Module* module_, SymbolId id_);
    VariableGroupSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    Symbol* GetSingleSymbol(Context* context) override;
    void AddVariable(VariableSymbol* variableSymbol);
    VariableSymbol* GetVariable(int arity, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    const std::vector<VariableSymbol*>& Variables(Context* context);
    void Expand(Context* context) override;
    bool IsEmpty() const noexcept;
private:
    std::vector<VariableSymbol*> variables;
    std::vector<SymbolId> variableIds;
    bool variablesFetched;
    bool expanded;
    VariableGroupSymbol* readOnlyVariableGroup;
    void GetVariables(Context* context);
};

} // namespace otava::symbols
