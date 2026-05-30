// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.context;

import std;
import otava.symbols.bound_tree;
import otava.symbols.modules;
import otava.symbols.id;
import otava.symbols.symbol;
import otava.symbols.symbol_index_map;
import otava.intermediate.data;
import soul.lexer.base;
import soul.lexer.file_map;
import otava.ast.node;

export namespace otava::symbols {

enum class ContextFlags : std::int64_t
{
    none = static_cast<std::int64_t>(0),
    hasDefiningTypeSpecifier = static_cast<std::int64_t>(1) << 0,
    friendSpecifier = static_cast<std::int64_t>(1) << 1,
    parsingParameters = static_cast<std::int64_t>(1) << 2,
    parsingConceptDefinition = static_cast<std::int64_t>(1) << 3,
    parsingTemplateId = static_cast<std::int64_t>(1) << 4,
    assumeType = static_cast<std::int64_t>(1) << 5,
    parseSavedMemberFunctionBody = static_cast<std::int64_t>(1) << 6,
    parsingTemplateDeclaration = static_cast<std::int64_t>(1) << 7,
    parseMemberFunction = static_cast<std::int64_t>(1) << 8,
    retMemberDeclSpecifiers = static_cast<std::int64_t>(1) << 9,
    addClassScope = static_cast<std::int64_t>(1) << 10,
    saveDeclarations = static_cast<std::int64_t>(1) << 11,
    linkageDeclaration = static_cast<std::int64_t>(1) << 12,
    instantiateFunctionTemplate = static_cast<std::int64_t>(1) << 13,
    instantiateAliasTypeTemplate = static_cast<std::int64_t>(1) << 14,
    instantiateMemFnOfClassTemplate = static_cast<std::int64_t>(1) << 15,
    instantiateInlineFunction = static_cast<std::int64_t>(1) << 16,
    dontBind = static_cast<std::int64_t>(1) << 17,
    parseSavedCtorInitializer = static_cast<std::int64_t>(1) << 18,
    skipFunctionDefinitions = static_cast<std::int64_t>(1) << 19,
    returnRef = static_cast<std::int64_t>(1) << 20,
    virtualSeen = static_cast<std::int64_t>(1) << 21,
    ignoreClassTemplateSpecializations = static_cast<std::int64_t>(1) << 22,
    generateMainWrapper = static_cast<std::int64_t>(1) << 23,
    noDynamicInit = static_cast<std::int64_t>(1) << 24,
    requireForwardResolved = static_cast<std::int64_t>(1) << 25,
    noPtrOps = static_cast<std::int64_t>(1) << 26,
    reinterpretCast = static_cast<std::int64_t>(1) << 27,
    derefAfterConv = static_cast<std::int64_t>(1) << 28,
    generatingVTab = static_cast<std::int64_t>(1) << 29,
    resolveNestedTypes = static_cast<std::int64_t>(1) << 30,
    release = static_cast<std::int64_t>(1) << 31,
    suppress_warning = static_cast<std::int64_t>(1) << 32,
    resolveDependentTypes = static_cast<std::int64_t>(1) << 33,
    processingAliasDeclation = static_cast<std::int64_t>(1) << 34,
    leaveBoundFunction = static_cast<std::int64_t>(1) << 35,
    skipFirstPtrToBooleanConversion = static_cast<std::int64_t>(1) << 36,
    matchClassGroup = static_cast<std::int64_t>(1) << 37,
    skipNonstaticMemberFunctions = static_cast<std::int64_t>(1) << 38,
    skipInvokeChecking = static_cast<std::int64_t>(1) << 39,
    dontProcess = static_cast<std::int64_t>(1) << 40,
    makeChildFn = static_cast<std::int64_t>(1) << 41,
    invoke = static_cast<std::int64_t>(1) << 42,
    tryCatch = static_cast<std::int64_t>(1) << 43,
    makeCompileUnitInitFn = static_cast<std::int64_t>(1) << 44,
    cast = static_cast<std::int64_t>(1) << 45,
    expected = static_cast<std::int64_t>(1) << 46,
    lookupOnlyFromMemberScope = static_cast<std::int64_t>(1) << 47,
    setParentBlockIds = static_cast<std::int64_t>(1) << 48,
    matchClassTemplateSpecializationConversion = static_cast<std::int64_t>(1) << 49,
    noWarnings = static_cast<std::int64_t>(1) << 50,
    debug = static_cast<std::int64_t>(1) << 51,
    debugMemory = static_cast<std::int64_t>(1) << 52,
    acquireTemporaryDestructorCalls = static_cast<std::int64_t>(1) << 53,
    skipMapIo = static_cast<std::int64_t>(1) << 54,
    sticky = noWarnings | expected
};

constexpr ContextFlags operator|(ContextFlags left, ContextFlags right) noexcept
{
    return ContextFlags(std::int64_t(left) | std::int64_t(right));
}

constexpr ContextFlags operator&(ContextFlags left, ContextFlags right) noexcept
{
    return ContextFlags(std::int64_t(left) & std::int64_t(right));
}

constexpr ContextFlags operator~(ContextFlags flags) noexcept
{
    return ContextFlags(~std::int64_t(flags));
}

int GetOptLevel(int level, bool release) noexcept;

class AliasTypeSymbol;
class SymbolsProject;
struct DeclarationList;
class SymbolTable;
class Module;
class MOduleMapper;
class EvaluationContext;

using Lexer = soul::lexer::LexerBase<char32_t>;

class Context
{
public:
    Context();
    inline Lexer* GetLexer() const noexcept { return lexer; }
    inline void SetLexer(Lexer* lexer_) noexcept { lexer = lexer_; }
    inline void SetModule(Module* module_) noexcept { module = module_; }
    inline Module* GetModule() const noexcept { return module; }
    inline void SetModuleMapper(ModuleMapper* moduleMapper_) noexcept { moduleMapper = moduleMapper_;  }
    inline ModuleMapper* GetModuleMapper() const noexcept { return moduleMapper; }
    inline SymbolTable* GetSymbolTable() const noexcept { return GetModule()->GetSymbolTable(); }
    inline SymbolIndexMap* GetSymbolIndexMap() const noexcept { return moduleMapper->GetSymbolIndexMap(); }
    inline SymbolId GetNextSymbolId(SymbolKind symbolKind) noexcept
    { 
        return MakeSymbolId(symbolKind, GetSymbolIndexMap()->GetNextIndex(symbolKind));
    }
    inline soul::lexer::FileMap* GetFileMap() const noexcept { return fileMap; }
    inline void SetFileMap(soul::lexer::FileMap* fileMap_) noexcept { fileMap = fileMap_; }
    void PushFlags();
    void PopFlags();
    void PushSetFlag(ContextFlags flag);
    void PushResetFlag(ContextFlags flag);
    inline void SetFlag(ContextFlags flag) noexcept { flags = flags | flag; }
    inline bool GetFlag(ContextFlags flag) const noexcept { return (flags & flag) != ContextFlags::none; }
    inline void ResetFlag(ContextFlags flag) noexcept { flags = flags & ~flag; }
    inline bool ReleaseConfig() const noexcept { return GetFlag(ContextFlags::release); }
    inline void SetReleaseConfig() noexcept { SetFlag(ContextFlags::release); }
    int OptLevel() const noexcept;
    inline void SetOptLevel(int optLevel_) noexcept { optLevel = optLevel_; }
    inline void ResetRejectTemplateId() noexcept { rejectTemplateId = false; }
    inline void SetRejectTemplateId() noexcept { rejectTemplateId = true; }
    inline bool RejectTemplateId() const noexcept { return rejectTemplateId; }
    bool EnableNoDeclSpecFunctionDeclaration() const noexcept;
    bool EnableNoDeclSpecFunctionDefinition() const noexcept;
    void PushNode(otava::ast::Node* node_);
    void PopNode();
    inline otava::ast::Node* GetNode() const noexcept { return node; }
    void SetDeclarationList(otava::ast::Node* node, DeclarationList* declarations);
    std::unique_ptr<DeclarationList> ReleaseDeclarationList(otava::ast::Node* node);
    inline int NextBlockId() noexcept { return nextBlockId++; }
    inline int CurrentBlockId() const noexcept { return currentBlockId; }
    void PushBlockId(int blockId);
    void PopBlockId();
    FunctionDefinitionSymbol* ParentFn() const noexcept { return parentFn; }
    void PushParentFn(FunctionDefinitionSymbol* parentFn_);
    void PopParentFn();
    inline int ParentBlockId() const noexcept { return parentBlockId; }
    void PushParentBlockId(int blockId);
    void PopParentBlockId();
    bool IsConstructorNameNode(otava::ast::Node* node) const;
    SymbolsProject* CurrentProject() const noexcept { return currentProject; }
    void SetCurrentProject(SymbolsProject* project) noexcept { currentProject = project; }
    inline BoundCompileUnitNode* GetBoundCompileUnit() const noexcept { return boundCompileUnit.get(); }
    void SetCurrentConfig(const std::string& config);
    void SetCurrentConfigurations(const std::set<std::string>& configurations);
    Module* GetModule(const std::string& moduleName) const;
    Module* GetStdTypeFundamentalModule() const;
    FunctionSymbol* GetSpecialization(otava::ast::Node* functionNode) const noexcept;
    void SetSpecialization(FunctionSymbol* specialization, otava::ast::Node* functionNode);
    void RemoveSpecialization(otava::ast::Node* functionNode);
    void PushBoundFunction(BoundFunctionNode* boundFunction_);
    void PopBoundFunction();
    BoundFunctionNode* GetBoundFunction() const noexcept;
    BoundFunctionNode* ReleaseBoundFunction() noexcept;
    inline int MemFunDefSymbolIndex() const noexcept { return memFunDefSymbolIndex; }
    inline void SetMemFunDefSymbolIndex(int index) noexcept { memFunDefSymbolIndex = index; }
    inline void SetAliasType(AliasTypeSymbol* aliasType_) noexcept { aliasType = aliasType_; }
    inline AliasTypeSymbol* GetAliasType() const noexcept { return aliasType; }
    void AddTemporaryAliasType(AliasTypeSymbol* temporaryAliasType);
    inline const std::vector<AliasTypeSymbol*>& TemporaryAliasTypes() const noexcept { return temporaryAliasTypes; }
    void ClearTemporaryAliasTypes();
    EvaluationContext* GetEvaluationContext() noexcept;
    inline void SetDebugOutputStream(std::ostream* s) noexcept { debugOutputStream = s; }
    inline std::ostream* DebugOutputStream() const noexcept { return debugOutputStream; }
    inline void SetPtr(otava::intermediate::Value* ptr_) noexcept { ptr = ptr_; }
    inline otava::intermediate::Value* Ptr() const noexcept { return ptr; }
    inline TypeSymbol* ArgType() noexcept { return argType; }
    inline void SetArgType(TypeSymbol* argType_) noexcept { argType = argType_; }
    inline TypeSymbol* ParamType() noexcept { return paramType; }
    inline void SetParamType(TypeSymbol* paramType_) noexcept { paramType = paramType_; }
private:
    Module* module;
    mutable Module* stdTypeFundamentalModule;
    ModuleMapper* moduleMapper;
    ContextFlags flags;
    std::stack<ContextFlags> flagStack;
    int optLevel;
    std::stack<otava::ast::Node*> nodeStack;
    otava::ast::Node* node;
    Lexer* lexer;
    bool rejectTemplateId;
    int nextBlockId;
    std::stack<int> blockIdStack;
    int currentBlockId;
    SymbolsProject* currentProject;
    std::unique_ptr<BoundCompileUnitNode> boundCompileUnit;
    std::string currentConfig;
    std::set<std::string> currentConfigurations;
    FunctionDefinitionSymbol* parentFn;
    std::stack<FunctionDefinitionSymbol*> parentFnStack;
    int parentBlockId;
    std::stack<int> parentBlockIdStack;
    int memFunDefSymbolIndex;
    soul::lexer::FileMap* fileMap;
    AliasTypeSymbol* aliasType;
    std::vector<AliasTypeSymbol*> temporaryAliasTypes;
    std::ostream* debugOutputStream;
    otava::intermediate::Value* ptr;
    TypeSymbol* argType;
    TypeSymbol* paramType;
};

} // namespace otava::symbols
