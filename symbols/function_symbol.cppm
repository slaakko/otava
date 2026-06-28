// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.function_symbol;

import std;
import otava.symbols.bound_tree;
import otava.symbols.container_symbol;
import otava.symbols.function_kind;
import otava.intermediate.types;
import otava.ast.function;

export namespace otava::symbols {

class ClassParsingMap;
class ClassTypeSymbol;
class Emitter;
class BoundExpressionNode;
class FunctionTypeSymbol;
class ParameterSymbol;
class TemplateDeclarationSymbol;
class TypeSymbol;

class FunctionSymbol : public ContainerSymbol
{
public:
    FunctionSymbol(Module* module_, SymbolId id_);
    FunctionSymbol(Module* module_, SymbolId id_, const std::string& name_);
    ~FunctionSymbol();
    std::string GroupName() const;
    FunctionGroupSymbol* Group(Context* context) const;
    void ResetGroup() { group = nullptr; }
    void SetGroup(FunctionGroupSymbol* group_) noexcept;
    inline Linkage GetLinkage() const noexcept { return linkage; }
    inline void SetLinkage(Linkage linkage_) noexcept { linkage = linkage_; }
    Cardinality Arity() const noexcept;
    Cardinality MemFnArity(Context* context) const;
    Cardinality MinArity(Context* context) const;
    Cardinality MinMemFnArity(Context* context) const;
    inline bool HasDefaultParams(Context* context) const { return MinArity(context) < Arity(); }
    inline bool GetFlag(FunctionSymbolFlags flag) const noexcept { return (flags & flag) != FunctionSymbolFlags::none; }
    inline void SetFlag(FunctionSymbolFlags flag) noexcept { flags = flags | flag; }
    inline void ResetFlag(FunctionSymbolFlags flag) noexcept { flags = flags & ~flag; }
    inline bool IsBound() const noexcept { return GetFlag(FunctionSymbolFlags::bound); }
    inline void SetBound() noexcept { SetFlag(FunctionSymbolFlags::bound); }
    inline bool IsSpecialization() const noexcept { return GetFlag(FunctionSymbolFlags::specialization); }
    inline void SetSpecialization() noexcept { SetFlag(FunctionSymbolFlags::specialization); }
    inline bool IsTrivialDestructor() const noexcept { return GetFlag(FunctionSymbolFlags::trivialDestructor); }
    inline bool ReturnsClass() const noexcept { return GetFlag(FunctionSymbolFlags::returnsClass); }
    inline void SetReturnsClass() noexcept { SetFlag(FunctionSymbolFlags::returnsClass); }
    virtual bool IsInline() const noexcept { return GetFlag(FunctionSymbolFlags::inline_); }
    virtual void SetInline() noexcept { SetFlag(FunctionSymbolFlags::inline_); }
    virtual bool IsUnparsed() const noexcept { return GetFlag(FunctionSymbolFlags::unparsed); }
    virtual void SetUnparsed() noexcept { SetFlag(FunctionSymbolFlags::unparsed); }
    virtual void ResetUnparsed() noexcept { ResetFlag(FunctionSymbolFlags::unparsed); }
    virtual  bool Parsing() const noexcept { return GetFlag(FunctionSymbolFlags::parsing); }
    virtual void SetParsing() noexcept { SetFlag(FunctionSymbolFlags::parsing); }
    virtual void ResetParsing() noexcept { ResetFlag(FunctionSymbolFlags::parsing); }
    inline bool IsGenerated() const noexcept { return GetFlag(FunctionSymbolFlags::generated); }
    inline void SetGenerated() noexcept { SetFlag(FunctionSymbolFlags::generated); }
    inline bool IsConversion() const noexcept { return GetFlag(FunctionSymbolFlags::conversion); }
    inline void SetConversion() noexcept { SetFlag(FunctionSymbolFlags::conversion); }
    inline bool IsConversionMemFn() const noexcept { return GetFunctionKind() == FunctionKind::conversionMemFn; }
    inline void SetConversionMemFn() noexcept { SetFunctionKind(FunctionKind::conversionMemFn); }
    inline bool ContainsStatics() const noexcept { return GetFlag(FunctionSymbolFlags::containsStatics); }
    inline void SetContainsStatics() noexcept { SetFlag(FunctionSymbolFlags::containsStatics); }
    inline bool SkipInvokeChecking() const noexcept { return GetFlag(FunctionSymbolFlags::skipInvokeChecking); }
    inline void SetSkipInvokeChecking() noexcept { SetFlag(FunctionSymbolFlags::skipInvokeChecking); }
    inline bool ContainsLocalVariableWithDestructor() const noexcept { return GetFlag(FunctionSymbolFlags::containsLocalVariableWithDestructor); }
    inline bool Skip() const noexcept { return GetFlag(FunctionSymbolFlags::skip); }
    inline void SetSkip() noexcept { SetFlag(FunctionSymbolFlags::skip); }
    virtual bool IsConst() const noexcept;
    virtual bool IsVirtual() const noexcept;
    virtual bool IsPure() const noexcept;
    void SetVirtual() noexcept;
    virtual bool IsOverride() const noexcept;
    virtual bool IsFinal() const noexcept;
    virtual void SetOverride() noexcept;
    virtual void SetFinal() noexcept;
    virtual bool IsNoExcept() const noexcept;
    virtual void SetNoExcept() noexcept;
    virtual bool IsStatic() const noexcept;
    virtual bool IsExplicit() const noexcept;
    bool IsDestructor() const noexcept;
    bool HasForwardDeclarationType(Context* context) const;
    virtual bool IsPointerCopyAssignment() const noexcept { return false; }
    virtual ParameterSymbol* ThisParam(Context* context) const;
    virtual FunctionKind GetFunctionKind() const noexcept { return functionKind; }
    inline void SetFunctionKind(FunctionKind functionKind_) noexcept { functionKind = functionKind_; }
    virtual TypeSymbol* ConversionParamType() const { return conversionParamType; }
    virtual TypeSymbol* GetConversionParamType(Context* context) const;
    void SetConversionParamType(TypeSymbol* conversionParamType_) noexcept;
    virtual TypeSymbol* ConversionArgType() const { return conversionArgType; }
    virtual TypeSymbol* GetConversionArgType(Context* context) const;
    void SetConversionArgType(TypeSymbol* conversionArgType_) noexcept;
    virtual ConversionKind GetConversionKind() const noexcept { return conversionKind; }
    void SetConversionKind(ConversionKind conversionKind_) noexcept;
    virtual std::int32_t ConversionDistance() const noexcept { return conversionDistance; }
    void SetConversionDistance(std::int32_t conversionDistance_) noexcept;
    virtual bool IsCtorAssignmentOrArrow() const noexcept { return false; }
    virtual bool IsIdentityConversion() const noexcept { return false; }
    virtual bool IsDerivedToBaseConversion() const noexcept { return false; }
    std::string FullName(Context* context) const override;
    void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context) override;
    const std::vector<ParameterSymbol*>& Parameters(Context* context) const;
    const std::vector<ParameterSymbol*>& MemFnParameters(Context* context) const;
    void AddTemporaryParameter(TypeSymbol* paramType, int index, Context* context);
    void ClearTemporaryParameters();
    ParameterSymbol* ReturnValueParam(Context* context) const;
    void SetReturnValueParam(ParameterSymbol* returnValueParam_) noexcept;
    virtual void SetReturnType(TypeSymbol* returnType_, Context* context);
    virtual TypeSymbol* ReturnType(Context* context) const;
    virtual void AddDefinitionToGroup(Context* context);
    inline FunctionQualifiers Qualifiers() const noexcept { return qualifiers; }
    inline void SetFunctionQualifiers(FunctionQualifiers qualifiers_) noexcept { qualifiers = qualifiers_; }
    inline std::int32_t GetIndex() const noexcept { return index; }
    inline void SetIndex(std::int32_t index_) noexcept { index = index_; }
    ClassTypeSymbol* ParentClassType(Context* context) const noexcept override;
    TemplateDeclarationSymbol* ParentTemplateDeclaration(Context* context) const noexcept;
    SpecialFunctionKind GetSpecialFunctionKind(Context* context) const noexcept;
    void SetSpecialization(const std::vector<TypeSymbol*>& specialization_);
    const std::vector<TypeSymbol*>& Specialization();
    virtual bool IsMemberFunction(Context* context) const noexcept;
    bool IsTemplate(Context* context) const noexcept;
    Cardinality TemplateArity(Context* context) const noexcept;
    bool IsMemFnOfClassTemplate(Context* context) const noexcept;
    bool IsExplicitSpecializationDefinitionSymbol(Context* context) const noexcept;
    bool IsExplicitSpecializationDeclaration(Context* context) const noexcept;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void SetFixedIrName(const std::string& fixedIrName_);
    std::string FixedIrName() const;
    std::string IrName(Context* context) const override;
    virtual otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) const;
    virtual void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateVirtualFunctionCall(Emitter& emitter, std::vector<BoundExpressionNode*>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    inline void SetVTabIndex(std::int32_t vtabIndex_) noexcept { vtabIndex = vtabIndex_; }
    virtual std::int32_t VTabIndex() const noexcept { return vtabIndex; }
    std::string NextTemporaryName();
    VariableSymbol* CreateTemporary(TypeSymbol* type, std::int64_t nodeId, Context* context);
    VariableSymbol* GetTemporary(std::int64_t nodeId) const noexcept;
    void AddLocalVariable(VariableSymbol* localVariable, Context* context);
    inline FunctionDefinitionSymbol* Destructor() const noexcept { return destructor; }
    inline void SetDestructor(FunctionDefinitionSymbol* destructor_) noexcept { destructor = destructor_; }
    FunctionTypeSymbol* GetFunctionType(otava::symbols::Context* context);
    void SetCompileUnitId(const std::string& compileUnitId_);
    inline const std::string& CompileUnitId() const noexcept { return compileUnitId; }
    void CheckGenerateClassCopyCtor(const soul::ast::FullSpan& fullSpan, Context* context);
    virtual ClassParsingMap* GetClassParsingMap() const noexcept;
    virtual void SetClassParsingMap(ClassParsingMap* classParsingMap_) noexcept;
    const std::vector<VariableSymbol*>& LocalVariables() const noexcept { return localVariables; }
    void AddClass(ClassTypeSymbol* cls);
    void RemoveClass(ClassTypeSymbol* cls);
    inline const std::vector<ClassTypeSymbol*>& Classes() const noexcept { return classes; }
private:
    FunctionSymbolFlags flags;
    FunctionQualifiers qualifiers;
    Linkage linkage;
    std::int32_t index;
    FunctionKind functionKind;
    std::vector<SymbolId> parameterIds;
    mutable std::vector<ParameterSymbol*> parameters;
    std::vector<std::unique_ptr<ParameterSymbol>> temporaryParams;
    mutable std::vector<ParameterSymbol*> memFnParameters;
    mutable std::unique_ptr<ParameterSymbol> thisParam;
    SymbolId returnValueParamId;
    mutable ParameterSymbol* returnValueParam;
    mutable bool memFnParamsConstructed;
    mutable bool parametersFetched;
    mutable std::string fixedIrName;
    std::vector<SymbolId> specializationIds;
    std::vector<TypeSymbol*> specialization;
    std::string compileUnitId;
    SymbolId returnTypeId;
    mutable TypeSymbol* returnType;
    ConversionKind conversionKind;
    SymbolId conversionParamTypeId;
    mutable TypeSymbol* conversionParamType;
    SymbolId conversionArgTypeId;
    mutable TypeSymbol* conversionArgType;
    std::int32_t conversionDistance;
    std::int32_t vtabIndex;
    std::int32_t nextTemporaryId;
    std::vector<VariableSymbol*> localVariables;
    std::map<std::int64_t, VariableSymbol*> temporaryMap;
    FunctionDefinitionSymbol* destructor;
    SymbolId groupId;
    mutable FunctionGroupSymbol* group;
    ClassParsingMap* classParsingMap;
    std::vector<ClassTypeSymbol*> classes;
    bool destructing;
};

class FunctionDefinitionSymbol : public FunctionSymbol
{
public:
    FunctionDefinitionSymbol(Module* module_, SymbolId id_);
    FunctionDefinitionSymbol(Module* module_, SymbolId id_, const std::string& name_);
    ~FunctionDefinitionSymbol();
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void SetDeclaration(FunctionSymbol* declaration_, Context* context) noexcept;
    inline FunctionSymbol* Declaration() const noexcept { return declaration; }
    void MapBlock(int blockId, Symbol* block);
    Symbol* GetBlock(int blockId) const noexcept;
    inline FunctionDefinitionSymbol* ParentFn() const noexcept { return parentFn; }
    inline void SetParentFn(FunctionDefinitionSymbol* parentFn_) noexcept { parentFn = parentFn_; }
    inline Scope* ParentFnScope() const noexcept { return parentFnScope; }
    inline void SetParentFnScope(Scope* parentFnScope_) noexcept { parentFnScope = parentFnScope_; }
    bool IsMemberFunction(Context* context) const noexcept;
    inline std::int32_t DefIndex() const noexcept { return defIndex; }
    inline void SetDefIndex(std::int32_t defIndex_) noexcept { defIndex = defIndex_; }
    FunctionKind GetFunctionKind() const noexcept override;
    bool IsConst() const noexcept override;
    bool IsVirtual() const noexcept override;
    bool IsPure() const noexcept override;
    bool IsOverride() const noexcept override;
    bool IsFinal() const noexcept override;
    bool IsNoExcept() const noexcept override;
    void SetNoExcept() noexcept override;
    void SetOverride() noexcept override;
    void SetFinal() noexcept override;
    bool IsInline() const noexcept override;
    void SetInline() noexcept override;
    bool IsUnparsed() const noexcept override;
    void SetUnparsed() noexcept override;
    void ResetUnparsed() noexcept override;
    bool Parsing() const noexcept override;
    void SetParsing() noexcept override;
    void ResetParsing() noexcept override;
    std::int32_t VTabIndex() const noexcept override;
    bool IsStatic() const noexcept override;
    bool IsExplicit() const noexcept override;
    TypeSymbol* ConversionParamType() const noexcept override;
    TypeSymbol* GetConversionParamType(Context* context) const override;
    TypeSymbol* ConversionArgType() const noexcept override;
    TypeSymbol* GetConversionArgType(Context* context) const override;
    std::int32_t ConversionDistance() const noexcept override;
    void AddDefinitionToGroup(Context* context) override;
    TypeSymbol* NonChildFunctionResultType(Context* context) const noexcept;
    void SetResultVarName(const std::string& resultVarName_);
    std::string ResultVarExprStr(TypeSymbol* resultType) const;
    inline void SetFnDefNode(otava::ast::FunctionDefinitionNode* fnDefNode_) { fnDefNode.reset(fnDefNode_); }
    inline bool ContainsGotosOrLabels() const noexcept { return containsGotosOrLabels; }
    inline void SetContainsGotosOrLabels() noexcept { containsGotosOrLabels = true; }
    ClassParsingMap* GetClassParsingMap() const noexcept override;
    void SetClassParsingMap(ClassParsingMap* classParsingMap_) noexcept override;
    std::string IrName(Context* context) const override;
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) const override;
    void SetReturnType(TypeSymbol* returnType_, Context* context) override;
private:
    FunctionSymbol* declaration;
    SymbolId declarationId;
    FunctionDefinitionSymbol* parentFn;
    SymbolId parentFnId;
    Scope* parentFnScope;
    std::int32_t defIndex;
    std::string resultVarName;
    bool containsGotosOrLabels;
    std::map<int, Symbol*> blockMap;
    std::unique_ptr<otava::ast::FunctionDefinitionNode> fnDefNode;
    mutable std::string irName;
};

class ExplicitlyInstantiatedFunctionDefinitionSymbol : public FunctionDefinitionSymbol
{
public:
    ExplicitlyInstantiatedFunctionDefinitionSymbol(Module* module_, SymbolId id_);
    ExplicitlyInstantiatedFunctionDefinitionSymbol(Module* module_, SymbolId id_, FunctionDefinitionSymbol* functionDefinitionSymbol_, 
        const soul::ast::FullSpan& fullSpan, Context* context);
    std::string IrName(Context* context) const override { return irName; }
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) const override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    FunctionDefinitionSymbol* functionDefinitionSymbol;
    SymbolId functionDefinitionId;
    std::string irName;
};

class CompileUnitInitFn : public FunctionSymbol
{
public:
    CompileUnitInitFn(Module* module_, SymbolId id_);
    CompileUnitInitFn(Module* module_, SymbolId id_, const std::string& name_);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
};

bool FunctionMatches(FunctionSymbol* left, FunctionSymbol* right, Context* context) noexcept;

void PrintXml(FunctionSymbol* function, Context* context);

} // namespace otava::symbols
