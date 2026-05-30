// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.symbol_table;

import std;
import otava.symbols.id;
import otava.symbols.symbol;
import otava.symbols.array_type_symbol;
import otava.symbols.type_symbol;
import otava.symbols.alias_type_symbol;
import otava.symbols.block;
import otava.symbols.classes;
import otava.symbols.compound_type_symbol;
import otava.symbols.conversion_table;
import otava.symbols.derivations;
import otava.symbols.enums;
import otava.symbols.namespaces;
import otava.symbols.value;
import otava.symbols.variable_symbol;
import otava.symbols.class_group_symbol;
import otava.symbols.function_group_symbol;
import otava.symbols.function_kind;
import otava.symbols.function_symbol;
import otava.symbols.function_type_symbol;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.lookup;
import otava.symbols.scope;
import otava.symbols.section;
import otava.symbols.templates;
import otava.symbols.reader;
import otava.symbols.writer;
import otava.ast.node;
import soul.ast.span;

export namespace otava::symbols {

class Module;
class Scope;

enum class MapKind : std::int32_t
{
    none, nodeToSymbol = 1 << 0, symbolToNode = 1 << 1, both = nodeToSymbol | symbolToNode
};

class SymbolTable
{
public:
    SymbolTable(Module* module_, bool readOnly_);
    inline Module* GetModule() const noexcept { return module; }
    inline bool IsReadOnly() const noexcept { return readOnly; }
    void Write(Writer& writer);
    Section* GetSection(SectionKind sectionKind) const noexcept;
    Section* GetSection(Symbol* forSymbol) const noexcept;
    ConversionTable* GetConversionTable() const noexcept { return &(const_cast<SymbolTable*>(this)->conversionTable); }
    NamespaceSymbol* GlobalNs() const noexcept { return globalNs.get(); }
    void Init(Context* context);
    inline Scope* CurrentScope() const noexcept { return currentScope; }
    inline void SetCurrentScope(Scope* scope) noexcept { currentScope = scope; }
    Scope* GetNamespaceScope(const std::string& nsName, const soul::ast::FullSpan& fullSpan, Context* context);
    void PushScope();
    void PopScope();
    void BeginScope(Scope* scope);
    void EndScope();
    void BeginScopeGeneric(Scope* scope, Context* context);
    void EndScopeGeneric(Context* context);
    void PushTopScopeIndex();
    void PopTopScopeIndex();
    inline void SetTopScopeIndex(int topScopeIndex_) noexcept { topScopeIndex = topScopeIndex_; }
    inline int TopScopeIndex() const noexcept { return topScopeIndex; }
    void BeginNamespace(const std::string& name, otava::ast::Node* node, Context* context);
    void EndNamespace();
    void BeginNamespace(otava::ast::Node* node, Context* context);
    void EndNamespace(int level);
    void BeginClass(const std::string& name, ClassKind classKind, TypeSymbol* spcialiation, otava::ast::Node* node, Context* context);
    void AddBaseClass(ClassTypeSymbol* baseClass, const soul::ast::FullSpan& fullSpan, Context* context);
    void EndClass();
    void AddForwardClassDeclaration(const std::string& name, ClassKind classKind, TypeSymbol* specialization, otava::ast::Node* node, Context* context);
    void AddFriend(const std::string& name, otava::ast::Node* node, Context* context);
    void BeginEnumeratedType(const std::string& name, EnumTypeKind kind, TypeSymbol* underlyingType, otava::ast::Node* node, Context* context);
    void EndEnumeratedType();
    void AddForwardEnumDeclaration(const std::string& name, EnumTypeKind enumTypeKind, TypeSymbol* underlyingType, otava::ast::Node* node, Context* context);
    void AddEnumerator(const std::string& name, Value* value, otava::ast::Node* node, Context* context);
    BlockSymbol* BeginBlock(const soul::ast::FullSpan& fullSpan, Context* context);
    void EndBlock(Context* context);
    void RemoveBlock();
    void BeginTemplateDeclaration(otava::ast::Node* node, Context* context);
    void EndTemplateDeclaration();
    void RemoveTemplateDeclaration();
    void AddTemplateParameter(const std::string& name, otava::ast::Node* node, Symbol* constraint, int index, ParameterSymbol* parameter,
        otava::ast::Node* defaultTemplateArgNode, Context* context);
    FunctionSymbol* AddFunction(const std::string& name, const std::vector<TypeSymbol*>& specialization, otava::ast::Node* node, FunctionKind kind,
        FunctionQualifiers qualifiers, DeclarationFlags flags, Context* context);
    void AddFunctionSymbol(Scope* scope, FunctionSymbol* functionSymbol, soul::ast::FullSpan& fullSpan, Context* context);
    FunctionDefinitionSymbol* AddOrGetFunctionDefinition(Scope* scope, const std::string& name, const std::vector<TypeSymbol*>& specialization,
        const std::vector<TypeSymbol*>& parameterTypes, FunctionQualifiers qualifiers, FunctionKind kind, DeclarationFlags declarationFlags,
        otava::ast::Node* node, otava::ast::Node* functionNode, bool& get, Context* context);
    ParameterSymbol* CreateParameter(const std::string& name, otava::ast::Node* node, TypeSymbol* type, Context* context);
    VariableSymbol* AddVariable(const std::string& name, otava::ast::Node* node, TypeSymbol* declaredType, TypeSymbol* type,
        Value* value, DeclarationFlags flags, Context* context);
    AliasTypeSymbol* AddAliasType(otava::ast::Node* idNnode, otava::ast::Node* aliasTypeNode, TypeSymbol* type, Context* context);
    void AddUsingDeclaration(otava::ast::Node* node, Symbol* symbol, Context* context);
    void AddUsingDirective(NamespaceSymbol* ns, otava::ast::Node* node, Context* context);
    TypeSymbol* MakeCompoundType(TypeSymbol* baseType, Derivations derivations, Context* context);
    ArrayTypeSymbol* MakeArrayType(TypeSymbol* elementType, std::int64_t size, otava::ast::Node* node);
    void AddCompoundType(CompoundTypeSymbol* compoundType);
    void MapCompoundType(CompoundTypeSymbol* compoundType);
    //CompoundTypeSymbol* GetCompoundType(const util::uuid& compoundTypeId) const noexcept;
    TypeSymbol* MakeConstCharPtrType(Context* context);
    TypeSymbol* MakeConstChar8PtrType(Context* context);
    TypeSymbol* MakeConstChar16PtrType(Context* context);
    TypeSymbol* MakeConstChar32PtrType(Context* context);
    TypeSymbol* MakeConstWCharPtrType(Context* context);
    FunctionTypeSymbol* MakeFunctionTypeSymbol(FunctionSymbol* functionSymbol);
    //FunctionGroupTypeSymbol* MakeFunctionGroupTypeSymbol(FunctionGroupSymbol* functionGroup);
    ///ClassGroupTypeSymbol* MakeClassGroupTypeSymbol(ClassGroupSymbol* classGroup);
    //AliasGroupTypeSymbol* MakeAliasGroupTypeSymbol(AliasGroupSymbol* aliasGroup);
    //ConceptSymbol* AddConcept(const std::string& name, otava::ast::Node* node, Context* context);
    ClassTemplateSpecializationSymbol* MakeClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArguments,
        const soul::ast::FullSpan& fullSpan, Context* context);
    //AliasTypeTemplateSpecializationSymbol* MakeAliasTypeTemplateSpecialization(TypeSymbol* aliasTypeTemplate, const std::vector<Symbol*>& templateArguments);
    //ArrayTypeSymbol* MakeArrayType(TypeSymbol* elementType, std::int64_t size, otava::ast::Node* node);
    inline Access CurrentAccess() const noexcept { return currentAccess; }
    void SetCurrentAccess(Access access);
    void PushAccess(Access access);
    void PopAccess();
    otava::ast::Node* GetSpecifierNode(Symbol* symbol) const noexcept;
    void SetSpecifierNode(Symbol* symbol, otava::ast::Node* node);
    Symbol* Lookup(const std::string& name, SymbolGroupKind symbolGroupKind, const soul::ast::FullSpan& fullSpan, Context* context);
    Symbol* Lookup(const std::string& name, SymbolGroupKind symbolGroupKind, const soul::ast::FullSpan& fullSpan, Context* context, LookupFlags flags);
    Symbol* LookupInScopeStack(const std::string& name, SymbolGroupKind symbolGroupKind, const soul::ast::FullSpan& fullSpan, Context* context, LookupFlags flags);
    Symbol* LookupSymbol(Symbol* symbol, Context* context);
    void MapNode(otava::ast::Node* node);
    void MapNode(otava::ast::Node* node, Symbol* symbol);
    void MapNode(otava::ast::Node* node, Symbol* symbol, MapKind kind);
    Symbol* GetSymbol(SymbolId id, Context* context);
    TypeSymbol* GetTypeSymbol(SymbolId id, Context* context);
    ParameterSymbol* GetParameterSymbol(SymbolId id, Context* context);
    FunctionSymbol* GetFunctionSymbol(SymbolId id, Context* context);
    FunctionGroupSymbol* GetFunctionGroupSymbol(SymbolId id, Context* context);
    ClassGroupSymbol* GetClassGroupSymbol(SymbolId id, Context* context);
    void AddSymbol(Symbol* symbol);
    const std::vector<Symbol*>& Symbols() const { return symbols; }
    inline Symbol* GetTypenameConstraintSymbol() noexcept { return typenameConstraintSymbol; }
    inline void SetTypenameConstraintSymbol(Symbol* typenameConstraintSymbol_) noexcept { typenameConstraintSymbol = typenameConstraintSymbol_; }
    void MapSymbol(Symbol* symbol);
    void MapFundamentalType(FundamentalTypeSymbol* fundamentalTypeSymbol);
    void MapFundamentalTypeId(FundamentalTypeKind kind, SymbolId fundamentalTypeId);
    TypeSymbol* GetFundamentalTypeSymbol(FundamentalTypeKind kind, Context* context);
    void MapFunction(FunctionSymbol* function);
    void MapFunctionDefinition(FunctionDefinitionSymbol* functionDefinition);
    void MapVariable(VariableSymbol* variable);
    void MapConstraint(Symbol* constraint);
    void MapFunctionGroup(FunctionGroupSymbol* functionGroup);
    void MapClassGroup(ClassGroupSymbol* classGroup);
    void MapAliasGroup(AliasGroupSymbol* aliasGroup);
    DependentTypeSymbol* MakeDependentTypeSymbol(otava::ast::Node* node);
    //void MapConcept(ConceptSymbol* cncp);
    inline Linkage CurrentLinkage() const { return currentLinkage; }
    void PushLinkage(Linkage linkage_);
    void PopLinkage();
    void WriteFundamentalTypeMap(Writer& writer);
    void ReadFundamentalTypeMap(Reader& reader);
    void MapExportedSymbolId(SymbolId symbolId, ModuleId moduleId);
    ModuleId GetModuleIdOfExportedSymbol(SymbolId symbolId) const;
    void WriteSymbolIdVector(Writer& writer);
    void ReadSymbolIdVector();
    void ReadSymbolIdVector(Reader& reader);
    const std::vector<SymbolId>& SymbolIds();
private:
    Module* module;
    std::unique_ptr<NamespaceSymbol> globalNs;
    std::vector<Symbol*> symbols;
    std::vector<SymbolId> symbolIds;
    bool symbolIdVectorRead;
    std::unordered_map<SymbolId, ModuleId> exportedSymbolMap;
    Scope* currentScope;
    std::vector<Scope*> scopeStack;
    int topScopeIndex;
    std::stack<int> topScopeIndexStack;
    std::vector<std::unique_ptr<Section>> sections;
    std::unordered_map<SectionKind, Section*> sectionMap;
    bool readOnly;
    Access currentAccess;
    std::stack<Access> accessStack;
    std::unordered_map<std::uint8_t, SymbolId> fundamentalTypeMap;
    Linkage currentLinkage;
    std::stack<Linkage> linkageStack;
    Symbol* typenameConstraintSymbol;
    int classLevel;
    ConversionTable conversionTable;
    void SetSectionsReadOnly();
    void MakeFundamentalTypes(Context* context);
    void AddFundamentalType(FundamentalTypeKind fundamentalTypeKind, Context* context);
    void MakeFundamentalTypeOperations(Context* context);
};

} // namespace otava::symbols
