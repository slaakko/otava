// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.class_group_symbol;

import otava.symbols.symbol;
import otava.symbols.template_param_compare;
import std;

export namespace otava::symbols {

class ForwardClassDeclarationSymbol;
class TemplateParameterSymbol;
class TypeSymbol;

enum class TemplateMatchKind
{
    straight, partialSpecialization, explicitSpecialization
};

struct TemplateMatchInfo
{
    inline TemplateMatchInfo() noexcept : kind(TemplateMatchKind::straight), matchValue(0) {}
    inline TemplateMatchInfo(TemplateMatchKind kind_, int matchValue_) noexcept : kind(kind_), matchValue(matchValue_) {}
    TemplateMatchKind kind;
    int matchValue;
    std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual> templateParameterMap;
    std::vector<Symbol*> templateArgs;
};

int Match(Symbol* templateArg, TypeSymbol* specialization, int index, TemplateMatchInfo& info, Context* context);

class ClassGroupSymbol : public Symbol
{
public:
    ClassGroupSymbol(Module* module_, SymbolId id_);
    ClassGroupSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    Symbol* GetSingleSymbol(Context* context) override;
    ClassTypeSymbol* GetBestMatchingClass(const std::vector<Symbol*>& templateArgs, TemplateMatchInfo& matchInfo, Context* context);
    void AddClass(ClassTypeSymbol* cls, Context* context);
    void AddForwardDeclaration(ForwardClassDeclarationSymbol* fwd);
    const std::vector<ClassTypeSymbol*>& Classes(Context* context) const;
    const std::vector<ForwardClassDeclarationSymbol*>& ForwardDeclarations(Context* context) const;
    ClassTypeSymbol* GetClass(Cardinality arity, Context* context) const;
    ForwardClassDeclarationSymbol* GetForwardDeclaration(Cardinality arity, Context* context) const;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    bool IsExportSymbol(Context* context) const noexcept override;
private:
    mutable std::vector<ClassTypeSymbol*> classes;
    mutable std::vector<SymbolId> classIds;
    mutable std::vector<ForwardClassDeclarationSymbol*> forwardDeclarations;
    std::vector<SymbolId> fwdDeclIds;
    bool contentFetched;
    mutable bool classesSet;
    mutable bool forwardDeclarationsSet;
    void GetContent(Context* context);
    bool ContainsExportClassOrFwdDeclaration(Context* context) const noexcept;
};

} // namespace otava::symbols
