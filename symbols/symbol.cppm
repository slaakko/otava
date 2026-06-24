// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.symbol;

import otava.symbols.id;
import otava.symbols.scope;
import soul.ast.span;
import std;

export namespace otava::symbols {

enum class DeclarationFlags : std::int32_t
{
    none = 0,
    staticFlag = 1 << 0,
    threadLocalFlag = 1 << 1,
    externFlag = 1 << 2,
    mutableFlag = 1 << 3,
    virtualFlag = 1 << 4,
    explicitFlag = 1 << 5,
    inlineFlag = 1 << 6,
    friendFlag = 1 << 7,
    typedefFlag = 1 << 8,
    constExprFlag = 1 << 9,
    constEvalFlag = 1 << 10,
    constInitFlag = 1 << 11,
    constFlag = 1 << 12,
    volatileFlag = 1 << 13,
    lvalueRefFlag = 1 << 14,
    rvalueRefFlag = 1 << 15,

    charFlag = 1 << 16,
    char8Flag = 1 << 17,
    char16Flag = 1 << 18,
    char32Flag = 1 << 19,
    wcharFlag = 1 << 20,
    boolFlag = 1 << 21,
    shortFlag = 1 << 22,
    intFlag = 1 << 23,
    longFlag = 1 << 24,
    longLongFlag = 1 << 25,
    signedFlag = 1 << 26,
    unsignedFlag = 1 << 27,
    floatFlag = 1 << 28,
    doubleFlag = 1 << 29,
    voidFlag = 1 << 30,
    autoFlag = 1 << 31,

    fundamentalTypeFlags =
    charFlag | char8Flag | char16Flag | char32Flag | wcharFlag | boolFlag | shortFlag | intFlag | longFlag | longLongFlag | signedFlag | unsignedFlag | 
    floatFlag | doubleFlag | voidFlag | autoFlag,

    typedefFlagMask = staticFlag | threadLocalFlag | externFlag | mutableFlag | virtualFlag | explicitFlag | inlineFlag | friendFlag | constExprFlag | 
    constEvalFlag | constInitFlag,

    cvQualifierFlagMask = constFlag | volatileFlag
};

constexpr std::int32_t ToUnderlying(DeclarationFlags declarationFlags)
{
    return std::int32_t(declarationFlags);
}

constexpr DeclarationFlags operator|(DeclarationFlags left, DeclarationFlags right) noexcept
{
    return DeclarationFlags(ToUnderlying(left) | ToUnderlying(right));
}

constexpr DeclarationFlags operator&(DeclarationFlags left, DeclarationFlags right) noexcept
{
    return DeclarationFlags(ToUnderlying(left) & ToUnderlying(right));
}

constexpr DeclarationFlags operator~(DeclarationFlags flags) noexcept
{
    return DeclarationFlags(~ToUnderlying(flags));
}

enum class Access : std::uint8_t
{
    none, public_, protected_, private_
};

std::string AccessStr(Access access);

enum class SymbolKind : std::uint8_t
{
    null,
    classGroupSymbol, conceptGroupSymbol, functionGroupSymbol, variableGroupSymbol, aliasGroupSymbol, enumGroupSymbol,
    boolValueSymbol, integerValueSymbol, floatingValueSymbol, stringValueSymbol, charValueSymbol, nullPtrTypeSymbol, arrayValueSymbol, 
    structureValueSymbol,
    aliasTypeSymbol, arrayTypeSymbol, blockSymbol, classTypeSymbol, compoundTypeSymbol,
    conceptSymbol, enumTypeSymbol, enumConstantSymbol, functionSymbol, functionTypeSymbol, functionDefinitionSymbol, explicitlyInstantiatedFunctionDefinitionSymbol,
    fundamentalTypeSymbol, namespaceSymbol, templateDeclarationSymbol, typenameConstraintSymbol, explicitInstantiationSymbol,
    templateParameterSymbol, varArgTypeSymbol, variableSymbol, parameterSymbol,
    classTemplateSpecializationSymbol, aliasTypeTemplateSpecializationSymbol, nestedTypeSymbol, dependentTypeSymbol, nullPtrValueSymbol, symbolValueSymbol, 
    invokeValueSymbol,
    forwardClassDeclarationSymbol, forwardEnumDeclarationSymbol, boundTemplateParameterSymbol, constraintExprSymbol,
    fundamentalTypeNot, fundamentalTypeUnaryPlus, fundamentalTypeUnaryMinus, fundamentalTypeComplement,
    fundamentalTypeAdd, fundamentalTypeSub, fundamentalTypeMul, fundamentalTypeDiv, fundamentalTypeMod,
    fundamentalTypeAnd, fundamentalTypeOr, fundamentalTypeXor, fundamentalTypeShl, fundamentalTypeShr,
    fundamentalTypePlusAssign, fundamentalTypeMinusAssign, fundamentalTypeMulAssign, fundamentalTypeDivAssign,
    fundamentalTypeModAssign, fundamentalTypeAndAssign, fundamentalTypeOrAssign, fundamentalTypeXorAssign, fundamentalTypeShlAssign, fundamentalTypeShrAssign,
    fundamentalTypeEqual, fundamentalTypeLess, fundamentalTypeBoolean,
    fundamentalTypeSignExtension, fundamentalTypeZeroExtension, fundamentalTypeFloatingPointExtension,
    fundamentalTypeTruncate, fundamentalTypeBitcast, fundamentalTypeIntToFloat, fundamentalTypeFloatToInt, fundamentalTypeBoolToInt,
    fundamentalTypeDefaultCtor, fundamentalTypeCopyCtor, fundamentalTypeCopyCtorLiteral, fundamentalTypeMoveCtor, fundamentalTypeCopyAssignment,
    fundamentalTypeMoveAssignment,
    enumTypeDefaultCtor, enumTypeCopyCtor, enumTypeMoveCtor, enumTypeCopyAssignment, enumTypeMoveAssignment, enumTypeEqual, enumTypeLess,
    arrayTypeDefaultCtor, arrayTypeCopyCtor, arrayTypeMoveCtor, arrayTypeCopyAssignment, arrayTypeMoveAssignment,
    arrayTypeBegin, arrayTypeEnd,
    defaultBool, defaultSByte, defaultByte, defaultShort, defaultUShort, defaultInt, defaultUInt, defaultLong, defaultULong, defaultFloat, defaultDouble,
    defaultChar, defaultChar16, defaultChar32,
    functionGroupTypeSymbol, classGroupTypeSymbol, aliasGroupTypeSymbol, templateParamGroupSymbol, friendSymbol, namespaceTypeSymbol,
    intrinsicGetRbp,
    max
};

constexpr std::uint8_t ToUnderlying(SymbolKind kind) noexcept { return std::uint8_t(kind); }

class Emitter;
class Module;
class Writer;
class Reader;
class Scope;
class Context;
class FunctionSymbol;
class ClassTypeSymbol;
class NamespaceSymbol;

std::uint32_t symbolIdShift = 32 - 7;
std::uint32_t symbolIndexMask = 0xFFFFFFFFu >> 7;
std::uint8_t symbolKindMask = 0x7Fu;

constexpr SymbolId MakeSymbolId(SymbolKind kind, Index index)
{
    return SymbolId((std::uint32_t(ToUnderlying(kind)) << symbolIdShift) | (ToUnderlying(index) & symbolIndexMask));
}

constexpr SymbolKind GetSymbolKind(SymbolId symbolId)
{
    return SymbolKind((ToUnderlying(symbolId) >> symbolIdShift) & symbolKindMask);
}

inline bool IsForwardClassDeclarationSymbol(SymbolId id) noexcept { return GetSymbolKind(id) == SymbolKind::forwardClassDeclarationSymbol; }
inline bool IsForwardEnumDeclarationSymbol(SymbolId id) noexcept { return GetSymbolKind(id) == SymbolKind::forwardEnumDeclarationSymbol; }
inline bool IsForwardDeclarationSymbol(SymbolId symbolId) noexcept { return IsForwardClassDeclarationSymbol(symbolId) || IsForwardEnumDeclarationSymbol(symbolId); }

enum class SymbolFlags : std::uint8_t
{
    none = 0, project = 1 << 0, readOnly = 1 << 1
};

constexpr std::uint8_t ToUnderlying(SymbolFlags flags) { return std::uint8_t(flags); }

constexpr SymbolFlags operator|(SymbolFlags left, SymbolFlags right) noexcept
{
    return SymbolFlags(std::uint8_t(left) | std::uint8_t(right));
}

constexpr SymbolFlags operator&(SymbolFlags left, SymbolFlags right) noexcept
{
    return SymbolFlags(std::uint8_t(left) & std::uint8_t(right));
}

constexpr SymbolFlags operator~(SymbolFlags flags) noexcept
{
    return SymbolFlags(~std::uint8_t(flags));
}

std::string SymbolKindStr(SymbolKind kind);

class Symbol
{
public:
    Symbol(Module* module_, SymbolId id_);
    Symbol(Module* module_, SymbolId id_, const std::string& name_);
    virtual ~Symbol();
    inline Module* GetModule() const noexcept { return module; }
    inline void ResetModule() noexcept { module = nullptr; }
    inline SymbolId Id() const noexcept { return id; }
    inline StringOffset NameOffset() const noexcept { return nameOffset; }
    std::string Name() const;
    virtual std::string SimpleName(Context* context) { return Name(); }
    void SetName(const std::string& name_);
    virtual std::string FullName(Context* context) const;
    inline SymbolKind Kind() const noexcept { return kind; }
    inline Access GetAccess() const noexcept { return access; }
    inline void SetAccess(Access access_) noexcept { access = access_; }
    SymbolGroupKind GetSymbolGroupKind() const noexcept;
    inline SymbolFlags Flags() const noexcept { return flags; }
    inline bool GetFlag(SymbolFlags flag) const noexcept { return (flags & flag) != SymbolFlags::none; }
    inline void SetFlag(SymbolFlags flag) noexcept { flags = flags | flag; }
    inline void ResetFlag(SymbolFlags flag) noexcept { flags = flags & ~flag; }
    inline bool IsProject() const noexcept { return GetFlag(SymbolFlags::project); }
    inline void SetProject() noexcept { SetFlag(SymbolFlags::project); }
    inline bool IsReadOnly() const noexcept { return GetFlag(SymbolFlags::readOnly); }
    inline void SetReadOnly() noexcept { SetFlag(SymbolFlags::readOnly); }
    virtual bool IsExportSymbol(Context* context) const noexcept { return IsProject(); }
    inline void SetDeclarationFlags(DeclarationFlags declarationFlags_) noexcept { declarationFlags = declarationFlags_; }
    inline DeclarationFlags GetDeclarationFlags() const noexcept { return declarationFlags; }
    bool CanInstall() const noexcept;
    bool IsTypeSymbol() const noexcept;
    virtual bool IsContainerSymbol() const noexcept { return false; }
    inline bool IsNamespaceSymbol() const noexcept { return kind == SymbolKind::namespaceSymbol; }
    inline bool IsGlobalNamespace() const noexcept { return kind == SymbolKind::namespaceSymbol && parent == nullptr; }
    inline bool IsClassTemplateSpecializationSymbol() const noexcept { return kind == SymbolKind::classTemplateSpecializationSymbol; }
    inline bool IsAliasTypeTemplateSpecializationSymbol() const noexcept { return kind == SymbolKind::aliasTypeTemplateSpecializationSymbol; }
    inline bool IsArrayTypeSymbol() const noexcept { return kind == SymbolKind::arrayTypeSymbol; }
    inline bool IsCompoundTypeSymbol() const noexcept { return kind == SymbolKind::compoundTypeSymbol; }
    inline bool IsExplicitInstantiationSymbol() const noexcept { return kind == SymbolKind::explicitInstantiationSymbol; }
    inline bool IsIntegerValueSymbol() const noexcept { return kind == SymbolKind::integerValueSymbol; }
    inline bool IsFloatingValueSymbol() const noexcept { return kind == SymbolKind::floatingValueSymbol; }
    inline bool IsStringValueSymbol() const noexcept { return kind == SymbolKind::stringValueSymbol; }
    inline bool IsCharValueSymbol() const noexcept { return kind == SymbolKind::charValueSymbol; }
    inline bool IsSymbolValueSymbol() const noexcept { return kind == SymbolKind::symbolValueSymbol; }
    inline bool IsAliasTypeSymbol() const noexcept { return kind == SymbolKind::aliasTypeSymbol || IsAliasTypeTemplateSpecializationSymbol(); }
    inline bool IsAliasGroupSymbol() const noexcept { return kind == SymbolKind::aliasGroupSymbol; }
    inline bool IsClassGroupSymbol() const noexcept { return kind == SymbolKind::classGroupSymbol; }
    inline bool IsClassTypeSymbol() const noexcept { return kind == SymbolKind::classTypeSymbol || IsClassTemplateSpecializationSymbol(); }
    inline bool IsForwardClassDeclarationSymbol() const noexcept { return kind == SymbolKind::forwardClassDeclarationSymbol; }
    inline bool IsConceptGroupSymbol() const noexcept { return kind == SymbolKind::conceptGroupSymbol; }
    inline bool IsConceptSymbol() const noexcept { return kind == SymbolKind::conceptSymbol; }
    inline bool IsEnumGroupSymbol() const noexcept { return kind == SymbolKind::enumGroupSymbol; }
    inline bool IsEnumeratedTypeSymbol() const noexcept { return kind == SymbolKind::enumTypeSymbol; }
    inline bool IsForwardEnumDeclarationSymbol() const noexcept { return kind == SymbolKind::forwardEnumDeclarationSymbol; }
    inline bool IsEnumConstantSymbol() const noexcept { return kind == SymbolKind::enumConstantSymbol; }
    inline bool IsFunctionGroupSymbol() const noexcept { return kind == SymbolKind::functionGroupSymbol; }
    bool IsFunctionSymbol() const noexcept;
    inline bool IsFunctionTypeSymbol() const noexcept { return kind == SymbolKind::functionTypeSymbol; }
    inline bool IsFunctionDefinitionSymbol() const noexcept { return kind == SymbolKind::functionDefinitionSymbol; }
    inline bool IsExplicitlyInstantiatedFunctionDefinitionSymbol() const noexcept { return kind == SymbolKind::explicitlyInstantiatedFunctionDefinitionSymbol; }
    inline bool IsBlockSymbol() const noexcept { return kind == SymbolKind::blockSymbol; }
    inline bool IsFundamentalTypeSymbol() const noexcept { return kind == SymbolKind::fundamentalTypeSymbol; }
    inline bool IsParameterSymbol() const noexcept { return kind == SymbolKind::parameterSymbol; }
    bool IsLocalVariableSymbol(Context* context);
    bool IsMemberVariableSymbol(Context* context);
    bool IsGlobalVariableSymbol(Context* context);
    inline bool IsTemplateParameterSymbol() const noexcept { return kind == SymbolKind::templateParameterSymbol; }
    inline bool IsTemplateParamGroupSymbol() const noexcept { return kind == SymbolKind::templateParamGroupSymbol; }
    inline bool IsBoundTemplateParameterSymbol() const noexcept { return kind == SymbolKind::boundTemplateParameterSymbol; }
    inline bool IsTemplateDeclarationSymbol() const noexcept { return kind == SymbolKind::templateDeclarationSymbol; }
    inline bool IsTypenameConstraintSymbol() const noexcept { return kind == SymbolKind::typenameConstraintSymbol; }
    inline bool IsVariableGroupSymbol() const noexcept { return kind == SymbolKind::variableGroupSymbol; }
    inline bool IsVariableSymbol() const noexcept { return kind == SymbolKind::variableSymbol; }
    inline bool IsConstraintExprSymbol() const noexcept { return kind == SymbolKind::constraintExprSymbol; }
    bool IsValueSymbol() const noexcept;
    virtual bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept { return true; }
    inline bool IsForwardDeclarationSymbol() const noexcept { return IsForwardClassDeclarationSymbol() || IsForwardEnumDeclarationSymbol(); }
    inline bool IsFunctionGroupTypeSymbol() const noexcept { return kind == SymbolKind::functionGroupTypeSymbol; }
    inline bool IsClassGroupTypeSymbol() const noexcept { return kind == SymbolKind::classGroupTypeSymbol; }
    inline bool IsAliasGroupTypeSymbol() const noexcept { return kind == SymbolKind::aliasGroupTypeSymbol; }
    inline bool IsNestedTypeSymbol() const noexcept { return kind == SymbolKind::nestedTypeSymbol; }
    inline bool IsDependentTypeSymbol()  const noexcept { return kind == SymbolKind::dependentTypeSymbol; }
    virtual bool IsCharTypeSymbol() const noexcept { return false; }
    virtual bool IsChar8TypeSymbol() const noexcept { return false; }
    virtual bool IsChar16TypeSymbol() const noexcept { return false; }
    virtual bool IsChar32TypeSymbol() const noexcept { return false; }
    virtual Scope* GetScope() { return nullptr; }
    virtual Symbol* GetSingleSymbol(Context* context) { return this; }
    virtual bool IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const;
    virtual void Write(Writer& writer);
    virtual void Read(Reader& reader);
    Symbol* Parent(Context* context) const;
    void SetParent(Symbol* parent_);
    FunctionSymbol* ParentFunction(Context* context) const noexcept;
    virtual ClassTypeSymbol* ParentClassType(Context* context) const noexcept;
    virtual NamespaceSymbol* ParentNamespace(Context* context) const noexcept;
    bool IsExtern() const noexcept;
    void* IrObject(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    inline void SetFullSpan(const soul::ast::FullSpan& fullSpan_) noexcept { fullSpan = fullSpan_; }
    inline const soul::ast::FullSpan& GetFullSpan() const noexcept { return fullSpan; }
    virtual std::string IrName(Context* context) const;
    virtual int PtrIndex() const noexcept { return -1; }
    virtual void Expand(Context* context);
    void AddModuleSymbolId(const ModuleSymbolId& moduleSymbolId);
    inline const std::vector<ModuleSymbolId>& ModuleSymbolIds() const noexcept { return moduleSymbolIds; }
    inline std::int64_t AstNodeId() const noexcept { return astNodeId; }
    inline void SetAstNodeId(std::int64_t astNodeId_) { astNodeId = astNodeId_; }
private:
    Module* module;
    SymbolId id;
    SymbolFlags flags;
    StringOffset nameOffset;
    const char* name;
    SymbolKind kind;
    SymbolId parentId;
    mutable Symbol* parent;
    DeclarationFlags declarationFlags;
    Access access;
    soul::ast::FullSpan fullSpan;
    std::vector<ModuleSymbolId> moduleSymbolIds;
    std::int64_t astNodeId;
};

} // namespace otava::symbols
