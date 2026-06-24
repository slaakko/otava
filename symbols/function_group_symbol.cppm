// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.function_group_symbol;

import otava.symbols.symbol;
import otava.symbols.function_kind;
import std;

export namespace otava::symbols {

class FunctionDefinitionSymbol;
class TemplateDeclarationSymbol;
class TypeSymbol;

class FunctionGroupSymbol : public Symbol
{
public:
    FunctionGroupSymbol(Module* module_, SymbolId id_);
    FunctionGroupSymbol(Module* module_, SymbolId id_, const std::string& name_);
    ~FunctionGroupSymbol();
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    Symbol* GetSingleSymbol(Context* context) override;
    FunctionDefinitionSymbol* GetSingleDefinition(Context* context);
    void AddFunction(FunctionSymbol* function);
    void AddFunctionDefinition(FunctionDefinitionSymbol* definition, Context* context);
    void Remove(FunctionSymbol* fn);
    std::vector<FunctionSymbol*> Functions(Context* context);
    std::vector<FunctionDefinitionSymbol*> Definitions(Context* context);
    FunctionSymbol* ResolveFunction(const std::vector<TypeSymbol*>& parameterTypes, FunctionQualifiers qualifiers, const std::vector<TypeSymbol*>& specialization,
        TemplateDeclarationSymbol* templateDeclaration, bool isSpecialization, Context* context);
    void CollectViableFunctions(Cardinality arity, const std::vector<TypeSymbol*>& templateArgs, std::vector<FunctionSymbol*>& viableFunctions, Context* context);
    FunctionSymbol* GetMatchingSpecialization(FunctionSymbol* specialization, Context* context);
    void SetVTabIndex(FunctionSymbol* function, int vtabIndex, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Expand(Context* context) override;
    bool IsExportSymbol(Context* context) const noexcept override;
private:
    std::vector<SymbolId> functionIds;
    std::vector<FunctionSymbol*> functions;
    std::vector<SymbolId> definitionIds;
    std::vector<FunctionDefinitionSymbol*> definitions;
    bool contentFetched;
    bool expanded;
    std::vector<FunctionGroupSymbol*> readOnlyFunctionGroups;
    void GetContent(Context* context);
    void CollectBestMatchingViableFunctionTemplates(Cardinality arity, 
        const std::vector<TypeSymbol*>& templateArgs, std::vector<FunctionSymbol*>& viableFunctions, Context* context);
    bool ContainsExportFunction(Context* context) const noexcept;
    bool ContainsExportDefinition(Context* context) const noexcept;
};

} // namespace otava::symbols
