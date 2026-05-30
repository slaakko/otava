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

export namespace otava::symbols {

class ClassTypeSymbol;
class Emitter;
class BoundExpressionNode;
class ParameterSymbol;
class TemplateDeclarationSymbol;
class TypeSymbol;

class FunctionSymbol : public ContainerSymbol
{
public:
    FunctionSymbol(Module* module_, SymbolId id_);
    FunctionSymbol(Module* module_, SymbolId id_, const std::string& name_);
    std::string GroupName() const;
    FunctionGroupSymbol* Group(Context* context) const;
    inline void SetGroup(FunctionGroupSymbol* group_) noexcept { group = group_; }
    inline Linkage GetLinkage() const noexcept { return linkage; }
    inline void SetLinkage(Linkage linkage_) noexcept { linkage = linkage_; }
    int Arity() const noexcept;
    int MemFnArity(const soul::ast::FullSpan& fullSpan, Context* context) const;
    int MinArity(const soul::ast::FullSpan& fullSpan, Context* context) const;
    int MinMemFnArity(const soul::ast::FullSpan& fullSpan, Context* context) const;
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
    virtual ParameterSymbol* ThisParam(Context* context) const;
    virtual FunctionKind GetFunctionKind() const noexcept { return functionKind; }
    inline void SetFunctionKind(FunctionKind functionKind_) noexcept { functionKind = functionKind_; }
    virtual TypeSymbol* ConversionParamType() const noexcept { return conversionParamType; }
    void SetConversionParamType(TypeSymbol* conversionParamType_) noexcept;
    virtual TypeSymbol* ConversionArgType() const noexcept { return conversionArgType; }
    void SetConversionArgType(TypeSymbol* conversionArgType_) noexcept;
    virtual ConversionKind GetConversionKind() const noexcept { return conversionKind; }
    void SetConversionKind(ConversionKind conversionKind_) noexcept;
    virtual std::int32_t ConversionDistance() const noexcept { return conversionDistance; }
    void SetConversionDistance(std::int32_t conversionDistance_) noexcept;
    virtual bool IsCtorAssignmentOrArrow() const noexcept { return false; }
    virtual bool IsIdentityConversion() const noexcept { return false; }
    virtual bool IsDerivedToBaseConversion() const noexcept { return false; }
    void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context) override;
    const std::vector<ParameterSymbol*>& Parameters(const soul::ast::FullSpan& fullSpan, Context* context) const;
    const std::vector<ParameterSymbol*>& MemFnParameters(const soul::ast::FullSpan& fullSpan, Context* context) const;
    ParameterSymbol* ReturnValueParam(Context* context) const;
    void SetReturnValueParam(ParameterSymbol* returnValueParam_) noexcept;
    virtual void SetReturnType(TypeSymbol* returnType_, const soul::ast::FullSpan& fullSpan, Context* context);
    TypeSymbol* ReturnType(Context* context) const;
    virtual void AddDefinitionToGroup(Context* context);
    inline FunctionQualifiers Qualifiers() const noexcept { return qualifiers; }
    inline void SetFunctionQualifiers(FunctionQualifiers qualifiers_) noexcept { qualifiers = qualifiers_; }
    inline std::int32_t GetIndex() const noexcept { return index; }
    inline void SetIndex(std::int32_t index_) noexcept { index = index_; }
    ClassTypeSymbol* ParentClassType(Context* context) const noexcept override;
    TemplateDeclarationSymbol* ParentTemplateDeclaration() const noexcept;
    SpecialFunctionKind GetSpecialFunctionKind(Context* context) const noexcept;
    bool IsTemplate() const noexcept;
    int TemplateArity() const noexcept;
    bool IsMemFnOfClassTemplate(Context* context) const noexcept;
    bool IsExplicitSpecializationDefinitionSymbol() const noexcept;
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
private:
    FunctionSymbolFlags flags;
    FunctionQualifiers qualifiers;
    Linkage linkage;
    std::int32_t index;
    FunctionKind functionKind;
    std::vector<SymbolId> parameterIds;
    mutable std::vector<ParameterSymbol*> parameters;
    mutable std::vector<ParameterSymbol*> memFnParameters;
    mutable std::unique_ptr<ParameterSymbol> thisParam;
    SymbolId returnValueParamId;
    mutable ParameterSymbol* returnValueParam;
    mutable bool memFnParamsConstructed;
    mutable StringOffset fixedIrNameOffset;
    std::vector<SymbolId> specializationIds;
    std::vector<TypeSymbol*> specialization;
    std::string compileUnitId;
    SymbolId returnTypeId;
    mutable TypeSymbol* returnType;
    ConversionKind conversionKind;
    SymbolId conversionParamTypeId;
    TypeSymbol* conversionParamType;
    SymbolId conversionArgTypeId;
    TypeSymbol* conversionArgType;
    std::int32_t conversionDistance;
    std::int32_t vtabIndex;
    SymbolId groupId;
    mutable FunctionGroupSymbol* group;
};

class FunctionDefinitionSymbol : public FunctionSymbol
{
public:
    FunctionDefinitionSymbol(Module* module_, SymbolId id_);
    FunctionDefinitionSymbol(Module* module_, SymbolId id_, const std::string& name_);
    inline void SetDeclaration(FunctionSymbol* declaration_) noexcept { declaration = declaration_; }
    inline FunctionSymbol* Declaration() const noexcept { return declaration; }
    void MapBlock(int blockId, Symbol* block);
    Symbol* GetBlock(int blockId) const noexcept;
    inline FunctionDefinitionSymbol* ParentFn() const noexcept { return parentFn; }
    inline void SetParentFn(FunctionDefinitionSymbol* parentFn_) noexcept { parentFn = parentFn_; }
    inline Scope* ParentFnScope() const noexcept { return parentFnScope; }
    inline void SetParentFnScope(Scope* parentFnScope_) noexcept { parentFnScope = parentFnScope_; }
    inline std::int32_t DefIndex() const noexcept { return defIndex; }
    inline void SetDefIndex(std::int32_t defIndex_) noexcept { defIndex = defIndex_; }
private:
    FunctionSymbol* declaration;
    FunctionDefinitionSymbol* parentFn;
    Scope* parentFnScope;
    std::int32_t defIndex;
};

} // namespace otava::symbols
