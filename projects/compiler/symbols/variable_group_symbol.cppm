// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.variable_group_symbol;

import otava.symbols.id;
import otava.symbols.scope;
import otava.symbols.symbol;
import std;

export namespace otava::symbols {

class Context;
class Module;
class VariableSymbol;
class Writer;
class Reader;

class VariableGroupSymbol : public Symbol
{
public:
    VariableGroupSymbol(Module* module_, SymbolId id_);
    VariableGroupSymbol(Module* module_, SymbolId id_, const std::string& name_);
    ~VariableGroupSymbol();
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    Symbol* GetSingleSymbol(Context* context) override;
    void AddVariable(VariableSymbol* variableSymbol);
    void ResetVariables();
    VariableSymbol* GetVariable(int arity, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    const std::vector<VariableSymbol*>& Variables(Context* context);
    bool IsEmpty() const noexcept;
private:
    std::vector<VariableSymbol*> variables;
    std::vector<SymbolId> variableIds;
    bool variablesFetched;
    std::string groupName;
    void GetVariables(Context* context);
};

} // namespace otava::symbols
