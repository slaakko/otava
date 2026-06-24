// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.context;

import otava.symbols.bound_tree;
import otava.symbols.modules;
import otava.symbols.id;
import otava.symbols.symbol;
import otava.symbols.symbol_index_map;
import otava.symbols.template_param_compare;
import otava.intermediate.data;
import soul.lexer.base;
import soul.lexer.file_map;
import soul.ast.span;
import otava.ast.node;
import util.uuid;
import std;

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

struct RangeForBlockIds
{
    RangeForBlockIds();
    int compoundBlockId;
    int forActionStatementId;
    int forStatementId;
};

class AliasTypeSymbol;
class SymbolsProject;
struct DeclarationList;
class SymbolTable;
class Module;
class ModuleMapper;
class EvaluationContext;
class StatementBinder;
class InstantiationQueue;
class TraceInfo;
class ClassTemplateSpecializationSymbol;

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
    inline Emitter* GetEmitter() noexcept { return emitter; }
    inline EvaluationContext* GetEvaluationContext() noexcept { return GetModule()->GetEvaluationContext(); }
    OperationRepository* GetOperationRepository() const noexcept;
    inline TraceInfo* GetTraceInfo() const noexcept { return traceInfo; }
    inline void SetTraceInfo(TraceInfo* traceInfo_) noexcept { traceInfo = traceInfo_; }
    inline void SetEmitter(Emitter* emitter_) noexcept { emitter = emitter_; }
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
    std::string FileName() const;
    void SetFileName(const std::string& fileName_);
    int Line() const;
    inline const std::string& Function() const noexcept { return function; }
    void SetFunction(const std::string& function_);
    bool EnableNoDeclSpecFunctionDeclaration() const noexcept;
    bool EnableNoDeclSpecFunctionDefinition() const noexcept;
    void PushNode(otava::ast::Node* node_);
    void PopNode();
    inline otava::ast::Node* GetNode() const noexcept { return node; }
    BoundExpressionNode* GetThisPtr(const soul::ast::FullSpan& fullSpan);
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
    ClassTemplateSpecializationSymbol* GetClassTemplateSpecialization(otava::ast::Node* functionNode) const noexcept;
    void SetClassTemplateSpecialization(otava::ast::Node* functionNode, ClassTemplateSpecializationSymbol* sp);
    void RemoveClassTemplateSpecialization(otava::ast::Node* functionNode);
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
    void PushTemplateParameterMap(std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>* templateParamMap);
    void PopTemplateParameterMap();
    inline std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>* TemplateParameterMap() const noexcept
    { 
        return templateParameterMap; 
    }
    inline void SetDebugOutputStream(std::ostream* s) noexcept { debugOutputStream = s; }
    inline std::ostream* DebugOutputStream() const noexcept { return debugOutputStream; }
    inline void SetPtr(otava::intermediate::Value* ptr_) noexcept { ptr = ptr_; }
    inline otava::intermediate::Value* Ptr() const noexcept { return ptr; }
    inline TypeSymbol* ArgType() noexcept { return argType; }
    inline void SetArgType(TypeSymbol* argType_) noexcept { argType = argType_; }
    inline TypeSymbol* ParamType() noexcept { return paramType; }
    inline void SetParamType(TypeSymbol* paramType_) noexcept { paramType = paramType_; }
    void PushStatementBinder(StatementBinder* statementBinder_);
    void PopStatementBinder();
    inline StatementBinder* GetStatementBinder() const noexcept { return statementBinder; }
    StatementBinder* GetParentStatementBinder() const noexcept;
    inline int NextTrySerial() noexcept { return trySerial++; }
    inline int NextInvokeSerial() noexcept { return invokeSerial++; }
    inline int NextCleanupSerial() noexcept { return cleanupSerial++; }
    inline int NextResultSerial() noexcept { return resultSerial++; }
    inline int NextLabelSerial() noexcept { return labelSerial++; }
    inline int NextEhReturnFromSerial() noexcept { return ehReturnFromSerial++; }
    inline int NextChildControlResultSerial() noexcept { return childControlResultSerial++; }
    inline int NextConditionVariableSerial() noexcept { return conditionVariableSerial++; }
    inline int NextStreamInitVarSerial() noexcept { return streamInitVarSerial++; }
    void AddBoundVTabFunction(BoundFunctionNode* node);
    inline const std::vector<std::unique_ptr<BoundFunctionNode>>& BoundVTabFunctions() const noexcept { return boundVTabFunctions; }
    void ClearBoundVTabFunctions();
    inline int ArgIndex() const noexcept { return argIndex; }
    void SetArgIndex(int argIndex_) noexcept { argIndex = argIndex_; }
    void SetInstantiationIrName(const std::string& instantiationIrName_);
    inline const std::string& InstantiationIrName() const noexcept { return instantiationIrName; }
    void SetInstantiationQueue(InstantiationQueue* instantiationQueue_) noexcept;
    inline InstantiationQueue* GetInstantiationQueue() noexcept { return instantiationQueue; }
    inline std::int64_t NodeId() const noexcept { return nodeId; }
    void PushNodeId(std::int64_t nodeId_);
    void PopNodeId();
    void PushSwitchCondType(TypeSymbol* switchCondType_);
    void PopSwitchCondType();
    inline TypeSymbol* GetSwitchCondType() const noexcept { return switchCondType; }
    RangeForBlockIds& GetRangeForBlockIds(const util::uuid& rangeForId);
    const std::string& ResultVarName() const { return resultVariableName; }
    void PushResultVarName(const std::string& resultVarName_);
    void PopResultVarName();
    const std::string& ChildControlResultVarName() const { return childControlResultVariableName; }
    void PushChildControlResultVarName(const std::string& childControlResultVarName);
    void PopChildControlResultVarName();
    std::string NextResultVarName();
    std::string NextEhReturnFromVarName();
    std::string NextChildControlResultVarName();
    std::string NextConditionVariableName();
    std::string NextStreamInitVarName();
    inline void SetDeclaredInitializerType(TypeSymbol* type) noexcept { declaredInitializerType = type; }
    inline TypeSymbol* DeclaredInitializerType() const noexcept { return declaredInitializerType; }
    inline int ParentStatementIndex() const noexcept { return parentStatementIndex; }
    void PushParentStatementIndex(int parentStatementIndex_);
    void PopParentStatementIndex();
    inline int TotalFunctionsCompiled() const noexcept { return totalFunctionsCompiled; }
    inline int FunctionCallsInlined() const noexcept { return functionCallsInlined; }
    inline int FunctionsInlined() const noexcept { return functionsInlined; }
    inline void SetTotalFunctionsCompiled(int totalFunctionsCompiled_) noexcept { totalFunctionsCompiled = totalFunctionsCompiled_; }
    inline void SetFunctionCallsInlined(int functionCallsInlined_) noexcept { functionCallsInlined = functionCallsInlined_; }
    inline void SetFunctionsInlined(int functionsInlined_) noexcept { functionsInlined = functionsInlined_; }
    inline int Invokes() const noexcept { return invokes; }
    inline void IncInvokes() noexcept { ++invokes; }
    inline int UnresolvedInvokes() noexcept { return unresolvedInvokes; }
    inline void IncUnresolvedInvokes() noexcept { ++unresolvedInvokes; }
    inline void SetInvokes(int invokes_) noexcept { invokes = invokes_; }
    inline void SetUnresolvedInvokes(int unresolvedInvokes_) { unresolvedInvokes = unresolvedInvokes_; }
private:
    Module* module;
    mutable Module* stdTypeFundamentalModule;
    ModuleMapper* moduleMapper;
    Emitter* emitter;
    TraceInfo* traceInfo;
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
    std::string function;
    std::map<otava::ast::Node*, std::unique_ptr<DeclarationList>> declarationMap;
    SymbolsProject* currentProject;
    std::unique_ptr<BoundFunctionNode> boundFunction;
    std::stack<std::unique_ptr<BoundFunctionNode>> boundFunctionStack;
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
    std::stack<std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>*> templateParameterMapStack;
    std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>* templateParameterMap;
    std::vector<StatementBinder*> statementBinders;
    StatementBinder* statementBinder;
    std::string resultVariableName;
    std::stack<std::string> resultVariableNameStack;
    std::string childControlResultVariableName;
    std::stack<std::string> childControlResultVariableNameStack;
    std::vector<std::unique_ptr<BoundFunctionNode>> boundVTabFunctions;
    std::map<otava::ast::Node*, FunctionSymbol*> specializationMap;
    std::map<otava::ast::Node*, ClassTemplateSpecializationSymbol*> classTemplateSpecializationMap;
    int totalFunctionsCompiled;
    int functionCallsInlined;
    int functionsInlined;
    int invokes;
    int unresolvedInvokes;
    int argIndex;
    int boundFunctionSerial;
    int trySerial;
    int invokeSerial;
    int cleanupSerial;
    int resultSerial;
    int labelSerial;
    int ehReturnFromSerial;
    int childControlResultSerial;
    int conditionVariableSerial;
    int streamInitVarSerial;
    std::string instantiationIrName;
    InstantiationQueue* instantiationQueue;
    std::int64_t nodeId;
    std::stack<std::int64_t> nodeIdStack;
    std::string fileName;
    TypeSymbol* switchCondType;
    std::stack<TypeSymbol*> switchCondTypeStack;
    std::map<util::uuid, RangeForBlockIds> rangeForBlockIdMap;
    TypeSymbol* declaredInitializerType;
    std::stack<int> parentStatementIndexStack;
    int parentStatementIndex;
};

} // namespace otava::symbols
