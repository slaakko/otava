// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.template_param_group_symbol;

import otava.symbols.symbol;
import std;

export namespace otava::symbols {

class TemplateParameterSymbol;

class TemplateParamGroupSymbol : public Symbol
{
public:
    TemplateParamGroupSymbol(Module* module_, SymbolId id_);
    TemplateParamGroupSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    Symbol* GetSingleSymbol(Context* context) override;
    inline void SetTemplateParam(TemplateParameterSymbol* templateParameterSymbol_) noexcept { templateParameterSymbol = templateParameterSymbol_; }
    TemplateParameterSymbol* GetTemplateParam(Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Expand(Context* context) override;
private:
    TemplateParamGroupSymbol* readOnlyTemplateParamGroup;
    TemplateParameterSymbol* templateParameterSymbol;
    SymbolId templateParamId;
    bool expanded;
};

} // namespace otava::symbols
