// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.alias_type_templates;

import otava.symbols.alias_type_symbol;
import otava.symbols.id;
import otava.symbols.symbol;
import otava.symbols.type_symbol;
import otava.ast.templates;
import soul.ast.span;
import std;

export namespace otava::symbols {

class Context;

class AliasTypeTemplateSpecializationSymbol : public AliasTypeSymbol
{
public:
    AliasTypeTemplateSpecializationSymbol(Module* module_, SymbolId id_);
    AliasTypeTemplateSpecializationSymbol(Module* module_, SymbolId id_, const std::string& name_);
    inline bool Instantiated() const noexcept { return instantiated; }
    inline void SetInstantiated() noexcept { instantiated = true; }
    inline void SetAliasTypeTemplate(TypeSymbol* aliasTypeTemplate_) noexcept { aliasTypeTemplate = aliasTypeTemplate_; }
    TypeSymbol* AliasTypeTemplate(Context* context);
    const std::vector<Symbol*>& TemplateArguments(Context* context);
    void AddTemplateArgument(Symbol* templateArgument);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    bool instantiated;
    SymbolId aliasTypeTemplateId;
    TypeSymbol* aliasTypeTemplate;
    std::vector<SymbolId> templateArgumentIds;
    bool templateArgumentsRead;
    std::vector<Symbol*> templateArguments;
};

TypeSymbol* InstantiateAliasTypeSymbol(TypeSymbol* typeSymbol, const std::vector<Symbol*>& templateArgs, otava::ast::TemplateIdNode* node, Context* context);

} // namespace otava::symbols
