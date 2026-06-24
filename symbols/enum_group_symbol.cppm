// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.enum_group_symbol;

import std;
import otava.symbols.symbol;

export namespace otava::symbols {

class EnumeratedTypeSymbol;
class ForwardEnumDeclarationSymbol;

class EnumGroupSymbol : public Symbol
{
public:
    EnumGroupSymbol(Module* module_, SymbolId id_);
    EnumGroupSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    Symbol* GetSingleSymbol(Context* context) override;
    EnumeratedTypeSymbol* GetEnumType(Context* context);
    inline void SetEnumType(EnumeratedTypeSymbol* enumType_) noexcept { enumType = enumType_; }
    bool IsExportSymbol(Context* context) const noexcept override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Expand(Context* context) override;
private:
    EnumeratedTypeSymbol* enumType;
    SymbolId enumTypeId;
    ForwardEnumDeclarationSymbol* forwardDeclaration;
    EnumGroupSymbol* readOnlyEnumGroup;
    bool expanded;
};

} // namespace otava::symbols
