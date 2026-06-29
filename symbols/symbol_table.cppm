// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.symbol_table;

import std;
import otava.symbols.id;
import otava.symbols.symbol;
import otava.symbols.array_type_symbol;
import otava.symbols.class_group_symbol;
import otava.symbols.class_templates;
import otava.symbols.class_info;
import otava.symbols.type_symbol;
import otava.symbols.alias_group_symbol;
import otava.symbols.alias_type_symbol;
import otava.symbols.alias_type_templates;
import otava.symbols.block;
import otava.symbols.classes;
import otava.symbols.compound_type_symbol;
import otava.symbols.conversion_table;
import otava.symbols.derivations;
import otava.symbols.enums;
import otava.symbols.enum_group_symbol;
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
import otava.symbols.specialization;
import otava.symbols.templates;
import otava.symbols.template_param_group_symbol;
import otava.symbols.reader;
import otava.symbols.writer;
import otava.ast.node;
import soul.ast.span;

export namespace otava::symbols {

class Module;
class Scope;

enum class MapKind 
{
    none, nodeToSymbol = 1 << 0, symbolToNode = 1 << 1, both = nodeToSymbol | symbolToNode
};

constexpr MapKind operator|(MapKind left, MapKind right) noexcept
{
    return MapKind(int(left) | int(right));
}

constexpr MapKind operator&(MapKind left, MapKind right) noexcept
{
    return MapKind(int(left) & int(right));
}

constexpr MapKind operator~(MapKind kind) noexcept
{
    return MapKind(~int(kind));
}

constexpr int maxArguments = 16;

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
    NamespaceSymbol* GetGlobalNs(Context* context);
    void Init(Context* context);
    inline Scope* CurrentScope() const noexcept { return currentScope; }
    inline void SetCurrentScope(Scope* scope) noexcept { currentScope = scope; }
    Scope* GetNamespaceScope(const std::string& nsName, const soul::ast::FullSpan& fullSpan, Context* context);
    void AddClass(ClassTypeSymbol* cls);
    inline const std::set<ClassTypeSymbol*>& Classes() const { return classes; }
    class_index& ClassIndex() { return index; }
    void PushScope();
    void PopScope();
    void BeginScope(Scope* scope, Context* context);
    void EndScope(Context* context);
    void BeginScopeGeneric(Scope* scope, Context* context);
    void EndScopeGeneric(Context* context);
    void PushTopScopeIndex();
    void PopTopScopeIndex();
    inline void SetTopScopeIndex(int topScopeIndex_) noexcept { topScopeIndex = topScopeIndex_; }
    inline int TopScopeIndex() const noexcept { return topScopeIndex; }
    void BeginNamespace(const std::string& name, otava::ast::Node* node, Context* context);
    void EndNamespace(Context* context);
    void BeginNamespace(otava::ast::Node* node, Context* context);
    void EndNamespace(int level, Context* context);
    void BeginClass(const std::string& name, ClassKind classKind, TypeSymbol* spcialiation, otava::ast::Node* node, Context* context);
    void AddBaseClass(ClassTypeSymbol* baseClass, const soul::ast::FullSpan& fullSpan, Context* context);
    void EndClass(Context* context);
    void AddForwardClassDeclaration(const std::string& name, ClassKind classKind, TypeSymbol* specialization, otava::ast::Node* node, Context* context);
    void AddFriend(const std::string& name, otava::ast::Node* node, Context* context);
    void BeginEnumeratedType(const std::string& name, EnumTypeKind kind, TypeSymbol* underlyingType, otava::ast::Node* node, Context* context);
    void EndEnumeratedType(Context* context);
    void AddForwardEnumDeclaration(const std::string& name, EnumTypeKind enumTypeKind, TypeSymbol* underlyingType, otava::ast::Node* node, Context* context);
    void AddEnumerator(const std::string& name, Value* value, otava::ast::Node* node, Context* context);
    BlockSymbol* BeginBlock(const soul::ast::FullSpan& fullSpan, Context* context);
    void EndBlock(Context* context);
    void RemoveBlock(Context* context);
    void BeginTemplateDeclaration(otava::ast::Node* node, Context* context);
    void EndTemplateDeclaration(Context* context);
    void RemoveTemplateDeclaration(Context* context);
    void AddTemplateParameter(const std::string& name, otava::ast::Node* node, Symbol* constraint, int index, ParameterSymbol* parameter,
        otava::ast::Node* defaultTemplateArgNode, Context* context);
    FunctionSymbol* AddFunction(const std::string& name, const std::vector<TypeSymbol*>& specialization, otava::ast::Node* node, FunctionKind kind,
        FunctionQualifiers qualifiers, DeclarationFlags flags, Context* context);
    void AddFunctionSymbol(Scope* scope, FunctionSymbol* functionSymbol, const soul::ast::FullSpan& fullSpan, Context* context);
    FunctionDefinitionSymbol* AddOrGetFunctionDefinition(Scope* scope, const std::string& name, const std::vector<TypeSymbol*>& specialization,
        const std::vector<TypeSymbol*>& parameterTypes, FunctionQualifiers qualifiers, FunctionKind kind, DeclarationFlags declarationFlags,
        otava::ast::Node* node, otava::ast::Node* functionNode, bool& get, Context* context);
    ParameterSymbol* CreateParameter(const std::string& name, otava::ast::Node* node, TypeSymbol* type, Context* context);
    VariableSymbol* AddVariable(const std::string& name, otava::ast::Node* node, TypeSymbol* declaredType, TypeSymbol* type,
        Value* value, DeclarationFlags flags, Context* context);
    AliasTypeSymbol* AddAliasType(otava::ast::Node* idNnode, otava::ast::Node* aliasTypeNode, TypeSymbol* type, Context* context);
    void AddUsingDeclaration(otava::ast::Node* node, Symbol* symbol, Context* context);
    void AddUsingDirective(NamespaceSymbol* ns, otava::ast::Node* node, Context* context);
    void SetIrId(CompoundTypeSymbol* compoundTypeSymbol, Context* context);
    TypeSymbol* GetCompoundType(TypeSymbol* baseType, Derivations derivations, Context* context);
    TypeSymbol* MakeCompoundType(TypeSymbol* baseType, Derivations derivations, Context* context);
    ArrayTypeSymbol* GetArrayType(TypeSymbol* elementType, std::int64_t size, Context* context);
    ArrayTypeSymbol* MakeArrayType(TypeSymbol* elementType, std::int64_t size, Context* context);
    TypeSymbol* MakeConstCharPtrType(Context* context);
    TypeSymbol* MakeConstChar8PtrType(Context* context);
    TypeSymbol* MakeConstChar16PtrType(Context* context);
    TypeSymbol* MakeConstChar32PtrType(Context* context);
    TypeSymbol* MakeConstWCharPtrType(Context* context);
    AliasTypeTemplateSpecializationSymbol* GetAliasTypeTemplateSpecialization(TypeSymbol* aliasTypeTemplate, const std::vector<Symbol*>& templateArguments,
        Context* context);
    AliasTypeTemplateSpecializationSymbol* MakeAliasTypeTemplateSpecialization(TypeSymbol* aliasTypeTemplate, const std::vector<Symbol*>& templateArguments, 
        Context* context);
    ClassTemplateSpecializationSymbol* GetClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArguments,
        Context* context);
    void SetIrId(ClassTemplateSpecializationSymbol* specialization, Context* context);
    ClassTemplateSpecializationSymbol* MakeClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArguments,
        const soul::ast::FullSpan& fullSpan, Context* context);
    ClassTemplateSpecializationSymbol* MakeClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArguments,
        const soul::ast::FullSpan& fullSpan, Context* context, bool createNew);
    ExplicitInstantiationSymbol* GetExplicitInstantiation(const SpecializationKey& key, Context* context);
    ExplicitInstantiationSymbol* GetExplicitInstantiation(const SpecializationKey& key, Context* context, int level);
    void AddExplicitInstantiation(ExplicitInstantiationSymbol* explicitInstantiationSymbol, const soul::ast::FullSpan& fullSpan, Context* context);
    FunctionTypeSymbol* GetFunctionTypeSymbol(const FunctionTypeSymbolKey& key, Context* context);
    FunctionTypeSymbol* MakeFunctionTypeSymbol(TypeSymbol* returnType, const std::vector<TypeSymbol*>& parameterTypes, Module* functionModule, bool makePtrType, 
        Context* context);
    FunctionTypeSymbol* MakeFunctionTypeSymbol(FunctionSymbol* functionSymbol, Context* context);
    DependentTypeSymbol* MakeDependentTypeSymbol(otava::ast::Node* node, Context* context);
    ClassGroupTypeSymbol* MakeClassGroupTypeSymbol(ClassGroupSymbol* classGroup, Context* context);
    AliasGroupTypeSymbol* MakeAliasGroupTypeSymbol(AliasGroupSymbol* aliasGroup, Context* context);
    FunctionGroupTypeSymbol* MakeFunctionGroupTypeSymbol(FunctionGroupSymbol* functionGroup, Context* context);
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
    void CollectViableFunctions(const std::vector<std::pair<Scope*, ScopeLookup>>& scopeLookups, const std::string& groupName, 
        const std::vector<TypeSymbol*>& templateArgs, Cardinality arity, std::vector<FunctionSymbol*>& viableFunctions, Context* context);
    void MapNode(otava::ast::Node* node);
    void MapNode(otava::ast::Node* node, Symbol* symbol);
    void MapNode(otava::ast::Node* node, Symbol* symbol, MapKind kind);
    otava::ast::Node* GetNodeNothrow(Symbol* symbol) const noexcept;
    otava::ast::Node* GetNode(Symbol* symbol) const;
    Symbol* GetSymbolNothrow(otava::ast::Node* node) const noexcept;
    Symbol* GetSymbol(otava::ast::Node* node) const;
    Symbol* GetSymbol(SymbolId id, Context* context);
    Value* GetValue(SymbolId id, Context* context);
    TypeSymbol* GetTypeSymbol(SymbolId id, Context* context);
    ClassTypeSymbol* GetClassTypeSymbol(SymbolId id, Context* context);
    ArrayTypeSymbol* GetArrayTypeSymbol(SymbolId id, Context* context);
    ParameterSymbol* GetParameterSymbol(SymbolId id, Context* context);
    EnumeratedTypeSymbol* GetEnumeratedTypeSymbol(SymbolId id, Context* context);
    VariableSymbol* GetVariableSymbol(SymbolId id, Context* context);
    FunctionSymbol* GetFunctionSymbol(SymbolId id, Context* context);
    FunctionDefinitionSymbol* GetFunctionDefinitionSymbol(SymbolId id, Context* context);
    ExplicitlyInstantiatedFunctionDefinitionSymbol* GetExplicitlyInstantiatedFunctionDefinitionSymbol(SymbolId id, Context* context);;
    FunctionGroupSymbol* GetFunctionGroupSymbol(SymbolId id, Context* context);
    ClassGroupSymbol* GetClassGroupSymbol(SymbolId id, Context* context);
    NamespaceSymbol* GetNamespaceSymbol(SymbolId id, Context* context);
    AliasTypeTemplateSpecializationSymbol* GetAliasTypeTemplateSpecializationSymbol(SymbolId id, Context* context);
    ClassTemplateSpecializationSymbol* GetClassTemplateSpecializationSymbol(SymbolId id, Context* context);
    ExplicitInstantiationSymbol* GetExplicitInstantiationSymbol(SymbolId id, Context* context);
    AliasTypeSymbol* GetAliasTypeSymbol(SymbolId id, Context* context);
    FunctionTypeSymbol* GetFunctionTypeSymbol(SymbolId id, Context* context);
    AliasGroupSymbol* GetAliasGroupSymbol(SymbolId id, Context* context);
    EnumGroupSymbol* GetEnumGroupSymbol(SymbolId id, Context* context);
    VariableGroupSymbol* GetVariableGroupSymbol(SymbolId id, Context* context);
    TemplateParameterSymbol* GetTemplateParameterSymbol(SymbolId id, Context* context);
    TemplateParamGroupSymbol* GetTemplateParamGroupSymbol(SymbolId id, Context* context);
    ForwardClassDeclarationSymbol* GetForwardClassDeclarationSymbol(SymbolId id, Context* context);
    void AddSymbol(Symbol* symbol);
    const std::vector<Symbol*>& Symbols() const { return symbols; }
    inline Symbol* GetTypenameConstraintSymbol() noexcept { return typenameConstraintSymbol; }
    inline void SetTypenameConstraintSymbol(Symbol* typenameConstraintSymbol_) noexcept { typenameConstraintSymbol = typenameConstraintSymbol_; }
    void MapSymbol(Symbol* symbol);
    void MapFundamentalType(FundamentalTypeSymbol* fundamentalTypeSymbol);
    void MapFundamentalTypeId(FundamentalTypeKind kind, SymbolId fundamentalTypeId);
    TypeSymbol* GetFundamentalTypeSymbol(FundamentalTypeKind kind, Context* context);
    inline Linkage CurrentLinkage() const { return currentLinkage; }
    void PushLinkage(Linkage linkage_);
    void PopLinkage();
    void WriteFundamentalTypeMap(Writer& writer);
    void ReadFundamentalTypeMap();
    void ReadFundamentalTypeMap(Reader& reader);
    void WriteCompoundTypeMaps(Writer& writer);
    void ReadCompoundTypeMaps();
    void ReadCompoundTypeMaps(Reader& reader);
    void WriteAliasTypeTemplateMap(Writer& writer);
    void ReadAliasTypeTemplateMap();
    void ReadAliasTypeTemplateMap(Reader& reader);
    void WriteClassTemplateSpecializationMaps(Writer& writer);
    void ReadClassTemplateSpecializationMaps();
    void ReadClassTemplateSpecializationMaps(Reader& reader);
    void WriteExplicitInstantiationMap(Writer& writer);
    void ReadExplicitInstantiationMap();
    void ReadExplicitInstantiationMap(Reader& reader);
    void WriteFunctionTypeMap(Writer& writer);
    void ReadFunctionTypeMap();
    void ReadFunctionTypeMap(Reader& reader);
    void MapImportedSymbolId(SymbolId symbolId, ModuleId moduleId);
    ModuleId GetModuleIdOfImportedSymbol(SymbolId symbolId) const;
    void WriteSymbolIdVector(Writer& writer);
    void ReadSymbolIdVector();
    void ReadSymbolIdVector(Reader& reader);
    const std::vector<SymbolId>& GetSymbolIds();
    std::int64_t GetArgumentId(int index);
    void AddImportedSymbol(SymbolId symbolId, ModuleId moduleId);
    const std::unordered_map<SymbolId, ModuleId>& AddedImportedSymbolMap() const { return addedImportedSymbolMap; }
private:
    Module* module;
    std::unique_ptr<NamespaceSymbol> globalNs;
    std::vector<Symbol*> symbols;
    std::vector<SymbolId> symbolIds;
    bool symbolIdVectorRead;
    std::unordered_map<SymbolId, ModuleId> importedSymbolMap;
    std::unordered_map<SymbolId, ModuleId> addedImportedSymbolMap;
    Scope* currentScope;
    std::vector<Scope*> scopeStack;
    int topScopeIndex;
    std::stack<int> topScopeIndexStack;
    std::vector<std::unique_ptr<Section>> sections;
    std::unordered_map<SectionKind, Section*> sectionMap;
    bool readOnly;
    Access currentAccess;
    std::stack<Access> accessStack;
    bool fundamentalTypeMapRead;
    std::unordered_map<std::uint8_t, SymbolId> fundamentalTypeMap;
    bool compoundTypeMapRead;
    std::unordered_map<CompoundTypeKey, SymbolId, CompoundTypeKeyHash, CompoundTypeKeyEqual> compoundTypeMap;
    std::unordered_map<CompoundTypeKey, SymbolId, CompoundTypeKeyHash, CompoundTypeKeyEqual> irCompoundTypeMap;
    bool aliasTypeTemplateMapRead;
    std::unordered_map<SpecializationKey, SymbolId, SpecializationKeyHash, SpecializationKeyEqual> aliasTypeTemplateMap;
    bool classTemplateSpecializationMapRead;
    std::unordered_map<SpecializationKey, SymbolId, SpecializationKeyHash, SpecializationKeyEqual> classTemplateSpecializationMap;
    std::unordered_map<SpecializationKey, SymbolId, SpecializationKeyHash, SpecializationKeyEqual> irClassTemplateSpecializationMap;
    bool explicitInstantiationMapRead;
    std::unordered_map<SpecializationKey, SymbolId, SpecializationKeyHash, SpecializationKeyEqual> explicitInstantiationMap;
    bool functionTypeMapRead;
    std::unordered_map<FunctionTypeSymbolKey, SymbolId, FunctionTypeSymbolKeyHash, FunctionTypeSymbolKeyEqual> functionTypeMap;
    Linkage currentLinkage;
    bool arrayTypeMapRead;
    std::unordered_map<ArrayTypeKey, SymbolId, ArrayTypeKeyHash, ArrayTypeKeyEqual> arrayTypeMap;
    std::stack<Linkage> linkageStack;
    Symbol* typenameConstraintSymbol;
    int classLevel;
    ConversionTable conversionTable;
    std::map<otava::ast::Node*, Symbol*> nodeSymbolMap;
    std::map<Symbol*, otava::ast::Node*> symbolNodeMap;
    std::map<Symbol*, otava::ast::Node*> specifierNodeMap;
    std::vector<std::int64_t> argumentIds;
    class_index index;
    std::set<ClassTypeSymbol*> classes;
    void SetSectionsReadOnly();
    void MakeFundamentalTypes(Context* context);
    void AddFundamentalType(FundamentalTypeKind fundamentalTypeKind, Context* context);
    void MakeFundamentalTypeOperations(Context* context);
    void MakeArgumentIds();
    void AddIntrinsics(Context* context);
};

} // namespace otava::symbols
