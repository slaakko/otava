// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.alias_group_symbol;

import otava.symbols.symbol;
import std;

export namespace otava::symbols {

class AliasTypeSymbol;

class AliasGroupSymbol : public Symbol
{
public:
    AliasGroupSymbol(Module* module_, SymbolId id_);
    AliasGroupSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    bool IsEmpty() const;
    Symbol* GetSingleSymbol(Context* context) override;
    void AddAliasTypeSymbol(AliasTypeSymbol* aliasTypeSymbol, Context* context);
    AliasTypeSymbol* GetAliasTypeSymbol(Cardinality arity, Context* context);
    AliasTypeSymbol* GetBestMatchingAliasType(const std::vector<Symbol*>& templateArgs, Context* context) noexcept;
    void RemoveAliasType(AliasTypeSymbol* aliasType);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    bool IsExportSymbol(Context* context) const noexcept override;
    void Expand(Context* context) override;
private:
    std::vector<AliasTypeSymbol*> aliasTypeSymbols;
    std::vector<SymbolId> aliasTypeSymbolIds;
    AliasGroupSymbol* readOnlyAliasGroup;
    bool aliasTypeSymbolsFetched;
    bool expanded;
    void GetAliasTypeSymbols(Context* context);
    bool ContainsExportAliasType(Context* context) const noexcept;
};

} // namespace otava::symbols
