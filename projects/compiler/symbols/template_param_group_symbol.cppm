// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.template_param_group_symbol;

import otava.symbols.id;
import otava.symbols.scope;
import otava.symbols.symbol;
import std;

export namespace otava::symbols {

class Module;
class TemplateParameterSymbol;
class Reader;
class Writer;

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
private:
    TemplateParameterSymbol* templateParameterSymbol;
    SymbolId templateParamId;
};

} // namespace otava::symbols
