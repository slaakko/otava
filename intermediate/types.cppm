// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.types;

import otava.assembly.instruction;
import soul.ast.span;
import util.code_formatter;
import util.uuid;
import std;

export namespace otava::intermediate {

class Value;
class IntermediateContext;
class Types;
class TypeRef;
class Visitor;
class ArrayType;
class StructureType;
class FwdDeclaredStructureType;
class MetadataRef;

constexpr std::int32_t voidTypeId = 0;
constexpr std::int32_t boolTypeId = 1;
constexpr std::int32_t sbyteTypeId = 2;
constexpr std::int32_t byteTypeId = 3;
constexpr std::int32_t shortTypeId = 4;
constexpr std::int32_t ushortTypeId = 5;
constexpr std::int32_t intTypeId = 6;
constexpr std::int32_t uintTypeId = 7;
constexpr std::int32_t longTypeId = 8;
constexpr std::int32_t ulongTypeId = 9;
constexpr std::int32_t floatTypeId = 10;
constexpr std::int32_t doubleTypeId = 11;
constexpr std::int32_t userTypeId = 12;
constexpr std::int32_t pointerTypeId = static_cast<std::int32_t>(1) << 31;

constexpr bool IsFundamentalTypeId(std::int32_t typeId) noexcept { return typeId >= 0 && typeId < userTypeId; }
constexpr bool IsPointerTypeId(std::int32_t typeId) noexcept { return (typeId & pointerTypeId) != 0; }

inline std::int32_t MakeUserTypeId(const std::string& typeIdStr) noexcept
{
    return userTypeId + std::stoi(typeIdStr.substr(2));
}

constexpr std::int32_t MakeUserTypeId(std::int32_t typeId) noexcept
{
    return typeId - userTypeId;
}

constexpr std::int32_t MakePointerTypeId(std::int32_t baseTypeId, std::int8_t pointerCount) noexcept
{
    return pointerTypeId | (std::int32_t(pointerCount) << (32 - 8)) | baseTypeId;
}

constexpr std::int32_t MakeTypeId(std::int32_t baseTypeId, std::int32_t pointerCount) noexcept
{
    if (pointerCount == 0)
    {
        return baseTypeId;
    }
    else
    {
        return MakePointerTypeId(baseTypeId, pointerCount);
    }
}

constexpr std::int32_t GetBaseTypeId(std::int32_t typeId) noexcept
{
    return typeId & (~(pointerTypeId | (std::int32_t(0x7F) << (32 - 8))) | 0x00FFFFFF);
}

constexpr std::int8_t GetPointerCount(std::int32_t typeId) noexcept
{
    return (typeId & (std::int32_t(0x7F) << (32 - 8))) >> (32 - 8);
}

enum class TypeKind : int
{
    fundamentalType, structureType, fwdDeclaredStructureType, arrayType, functionType, pointerType
};

class Type
{
public:
    Type(const soul::ast::Span& span_, TypeKind kind_, std::int32_t id_) noexcept;
    virtual ~Type();
    virtual void Accept(Visitor& visitor) {}
    virtual std::int64_t Size() const noexcept = 0;
    virtual std::int64_t Alignment() const noexcept = 0;
    virtual otava::assembly::OpCode DataOpCode() const noexcept { return otava::assembly::OpCode::DB; }
    inline TypeKind Kind() const noexcept { return kind; }
    inline bool IsFundamentalType() const noexcept { return kind == TypeKind::fundamentalType; }
    inline bool IsVoidType() const noexcept { return id == voidTypeId; }
    inline bool IsBooleanType() const noexcept { return id == boolTypeId; }
    inline bool IsLongType() const noexcept { return id == longTypeId; }
    inline bool IsCompoundType() const noexcept { return !IsFundamentalType(); }
    inline bool IsArithmeticType() const noexcept { return IsIntegerType() || IsFloatingPointType(); }
    bool IsIntegerType() const noexcept;
    bool IsUnsignedType() const noexcept;
    bool IsSignedType() const noexcept;
    bool IsFloatingPointType() const noexcept;
    inline bool IsFloatType() const noexcept { return id == floatTypeId; }
    inline bool IsDoubleType() const noexcept { return id == doubleTypeId; }
    inline bool IsPointerType() const noexcept { return kind == TypeKind::pointerType; }
    Type* AddPointer(IntermediateContext* context) const;
    Type* RemovePointer(const soul::ast::Span& span, IntermediateContext* context) const;
    inline bool IsFwdDeclaredStructureType() const noexcept { return kind == TypeKind::fwdDeclaredStructureType; }
    virtual void ReplaceForwardReference(FwdDeclaredStructureType* fwdDeclaredType, StructureType* structureType, IntermediateContext* context);
    inline std::int32_t NextTypeId() noexcept { return nextTypeId++; }
    virtual std::string Name() const = 0;
    inline bool IsStructureType() const noexcept { return kind == TypeKind::structureType; }
    StructureType* GetStructurePointeeType(const soul::ast::Span& span, IntermediateContext* context) const;
    ArrayType* GetArrayPointeeType(const soul::ast::Span& span, IntermediateContext* context) const;
    inline bool IsArrayType() const noexcept { return kind == TypeKind::arrayType; }
    inline bool IsFunctionType() const noexcept { return kind == TypeKind::functionType; }
    bool IsFunctionPtrType() const noexcept;
    virtual bool IsWeakType() const noexcept { return true; }
    virtual void Add(Types* types, IntermediateContext* context);
    virtual void Resolve(Types* types, IntermediateContext* context);
    inline const soul::ast::Span& Span() const noexcept { return span; }
    inline std::int32_t Id() const noexcept { return id; }
    void Write(util::CodeFormatter& formatter);
    virtual void WriteDeclaration(util::CodeFormatter& formatter);
    inline Value* DefaultValue() noexcept { return defaultValue; }
    inline void SetDefaultValue(Value* defaultValue_) noexcept { defaultValue = defaultValue_; }
    virtual Value* MakeDefaultValue(IntermediateContext& context) const { return nullptr; }
    TypeRef GetTypeRef();
    virtual Type* BaseType() const noexcept { return const_cast<Type*>(this); }
    virtual void IncCount() noexcept;
private:
    soul::ast::Span span;
    TypeKind kind;
    std::int32_t id;
    Value* defaultValue;
    std::int32_t nextTypeId;
};

class VoidType : public Type
{
public:
    VoidType() noexcept;
    std::string Name() const override { return "void"; }
    std::int64_t Size() const noexcept override { return -1; }
    std::int64_t Alignment() const noexcept override { return -1; }
};

class BoolType : public Type
{
public:
    BoolType() noexcept;
    std::string Name() const override { return "bool"; }
    std::int64_t Size() const noexcept override { return 1; }
    std::int64_t Alignment() const noexcept override { return 1; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class SByteType : public Type
{
public:
    SByteType() noexcept;
    std::string Name() const override { return "sbyte"; }
    std::int64_t Size() const noexcept override { return 1; }
    std::int64_t Alignment() const noexcept override { return 1; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class ByteType : public Type
{
public:
    ByteType() noexcept;
    std::string Name() const override { return "byte"; }
    std::int64_t Size() const noexcept override { return 1; }
    std::int64_t Alignment() const noexcept override { return 1; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class ShortType : public Type
{
public:
    ShortType() noexcept;
    std::string Name() const override { return "short"; }
    std::int64_t Size() const noexcept override { return 2; }
    std::int64_t Alignment() const noexcept override { return 2; }
    otava::assembly::OpCode DataOpCode() const noexcept override { return otava::assembly::OpCode::DW; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class UShortType : public Type
{
public:
    UShortType() noexcept;
    std::string Name() const override { return "ushort"; }
    std::int64_t Size() const noexcept override { return 2; }
    std::int64_t Alignment() const noexcept override { return 2; }
    otava::assembly::OpCode DataOpCode() const noexcept override { return otava::assembly::OpCode::DW; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class IntType : public Type
{
public:
    IntType() noexcept;
    std::string Name() const override { return "int"; }
    std::int64_t Size() const noexcept override { return 4; }
    std::int64_t Alignment() const noexcept override { return 4; }
    otava::assembly::OpCode DataOpCode() const noexcept override { return otava::assembly::OpCode::DD; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class UIntType : public Type
{
public:
    UIntType() noexcept;
    std::string Name() const override { return "uint"; }
    std::int64_t Size() const noexcept override { return 4; }
    std::int64_t Alignment() const noexcept override { return 4; }
    otava::assembly::OpCode DataOpCode() const noexcept override { return otava::assembly::OpCode::DD; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class LongType : public Type
{
public:
    LongType() noexcept;
    std::string Name() const override { return "long"; }
    std::int64_t Size() const noexcept override { return 8; }
    std::int64_t Alignment() const noexcept override { return 8; }
    otava::assembly::OpCode DataOpCode() const noexcept override { return otava::assembly::OpCode::DQ; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class ULongType : public Type
{
public:
    ULongType() noexcept;
    std::string Name() const override { return "ulong"; }
    std::int64_t Size() const noexcept override { return 8; }
    std::int64_t Alignment() const noexcept override { return 8; }
    otava::assembly::OpCode DataOpCode() const noexcept override { return otava::assembly::OpCode::DQ; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class FloatType : public Type
{
public:
    FloatType() noexcept;
    std::string Name() const override { return "float"; }
    std::int64_t Size() const noexcept override { return 4; }
    std::int64_t Alignment() const noexcept override { return 4; }
    otava::assembly::OpCode DataOpCode() const noexcept override { return otava::assembly::OpCode::REAL4; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class DoubleType : public Type
{
public:
    DoubleType() noexcept;
    std::string Name() const override { return "double"; }
    std::int64_t Size() const noexcept override { return 8; }
    std::int64_t Alignment() const noexcept override { return 8; }
    otava::assembly::OpCode DataOpCode() const noexcept override { return otava::assembly::OpCode::REAL8; }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
};

class TypeRef
{
public:
    TypeRef() noexcept;
    TypeRef(const soul::ast::Span& span_, std::int32_t id_) noexcept;
    inline const soul::ast::Span& Span() const noexcept { return span; }
    inline std::int32_t Id() const noexcept { return id; }
    inline void SetType(Type* type_) noexcept { type = type_; }
    inline Type* GetType() const noexcept { return type; }
private:
    soul::ast::Span span;
    std::int32_t id;
    Type* type;
};

inline TypeRef MakeTypeRef(const soul::ast::Span& span, std::int32_t baseTypeId, std::int32_t pointerCount) noexcept
{
    return TypeRef(span, MakeTypeId(baseTypeId, pointerCount));
}

class StructureType : public Type
{
public:
    StructureType(const soul::ast::Span& span_, std::int32_t typeId_, const std::vector<TypeRef>& fieldTypeRefs_);
    void Accept(Visitor& visitor) override;
    void Add(Types* types, IntermediateContext* context) override;
    void Resolve(Types* types, IntermediateContext* context) override;
    std::int64_t Size() const noexcept override;
    std::int64_t Alignment() const noexcept override { return 8; }
    std::string Name() const override { return "$T" + std::to_string(Id() - userTypeId); }
    bool IsWeakType() const noexcept override;
    inline int FieldCount() const noexcept { return fieldTypeRefs.size(); }
    inline const std::vector<TypeRef>& FieldTypeRefs() const noexcept { return fieldTypeRefs; }
    Type* FieldType(int i) const noexcept;
    std::int64_t GetFieldOffset(std::int64_t index) const;
    void WriteDeclaration(util::CodeFormatter& formatter) override;
    void ReplaceForwardReference(FwdDeclaredStructureType* fwdDeclaredType, StructureType* structureType, IntermediateContext* context) override;
    Value* MakeDefaultValue(IntermediateContext& context) const override;
    inline const std::string& Comment() const noexcept { return comment; }
    void SetComment(const std::string& comment_);
    inline void SetMetadataRef(MetadataRef* metadataRef_) noexcept { metadataRef = metadataRef_; }
    inline MetadataRef* GetMetadataRef() const noexcept { return metadataRef; }
    void ResolveComment();
private:
    void ComputeSizeAndOffsets() const;
    std::vector<TypeRef> fieldTypeRefs;
    mutable bool sizeAndOffsetsComputed;
    mutable std::int64_t size;
    mutable std::vector<std::int64_t> fieldOffsets;
    MetadataRef* metadataRef;
    std::string comment;
};

class FwdDeclaredStructureType : public Type
{
public:
    FwdDeclaredStructureType(std::uint32_t id_, std::int32_t typeId_) noexcept;
    void Accept(Visitor& visitor) override;
    std::int64_t Size() const noexcept override { return -1; }
    std::int64_t Alignment() const noexcept override { return -1; }
    std::string Name() const override { return "FWD_DECLARED_STRUCTURE_TYPE"; }
    inline std::uint32_t Id() const noexcept { return id; }
    inline const std::string& Comment() const noexcept { return comment; }
    void SetComment(const std::string& comment_);
private:
    std::uint32_t id;
    std::string comment;
};

class ArrayType : public Type
{
public:
    ArrayType(const soul::ast::Span& span_, std::int32_t typeId_, std::int64_t elementCount_, const TypeRef& elementTypeRef_) noexcept;
    void Accept(Visitor& visitor) override;
    void Add(Types* types, IntermediateContext* context) override;
    void Resolve(Types* types, IntermediateContext* context) override;
    std::int64_t Size() const noexcept override;
    std::int64_t Alignment() const noexcept override { return 8; }
    std::string Name() const override { return "$T" + std::to_string(Id() - userTypeId); }
    bool IsWeakType() const noexcept override;
    inline std::int64_t ElementCount() const noexcept { return elementCount; }
    inline const TypeRef& ElementTypeRef() const noexcept { return elementTypeRef; }
    inline Type* ElementType() const noexcept { return elementTypeRef.GetType(); }
    void WriteDeclaration(util::CodeFormatter& formatter) override;
    Value* MakeDefaultValue(IntermediateContext& context) const override;
private:
    std::int64_t elementCount;
    TypeRef elementTypeRef;
};

class FunctionType : public Type
{
public:
    FunctionType(const soul::ast::Span& span_, std::int32_t typeId_, const TypeRef& returnTypeRef_, const std::vector<TypeRef>& paramTypeRefs_) noexcept;
    void Accept(Visitor& visitor) override;
    void Add(Types* types, IntermediateContext* context) override;
    void Resolve(Types* types, IntermediateContext* context) override;
    bool IsWeakType() const noexcept override;
    inline int Arity() const noexcept { return paramTypeRefs.size(); }
    std::int64_t Size() const noexcept override { return 8; }
    std::int64_t Alignment() const noexcept override { return 8; }
    std::string Name() const override { return "$T" + std::to_string(Id() - userTypeId); }
    inline const TypeRef& ReturnTypeRef() const noexcept { return returnTypeRef; }
    inline Type* ReturnType() const noexcept { return returnTypeRef.GetType(); }
    inline const std::vector<TypeRef>& ParamTypeRefs() const noexcept { return paramTypeRefs; }
    inline Type* ParamType(int index) const noexcept { return paramTypeRefs[index].GetType(); }
    void WriteDeclaration(util::CodeFormatter& formatter) override;
    bool IsUnaryOperationType() const noexcept;
private:
    TypeRef returnTypeRef;
    std::vector<TypeRef> paramTypeRefs;
};

class PointerType : public Type
{
public:
    PointerType(const soul::ast::Span& span_, std::int32_t typeId_, std::int8_t pointerCount_, std::int32_t baseTypeId_) noexcept;
    std::string Name() const override;
    std::int64_t Size() const noexcept override { return 8; }
    std::int64_t Alignment() const noexcept override { return 8; }
    inline std::int8_t PointerCount() const noexcept { return pointerCount; }
    inline const TypeRef& BaseTypeRef() const noexcept { return baseTypeRef; }
    inline TypeRef& BaseTypeRef() noexcept { return baseTypeRef; }
    Type* BaseType() const noexcept override { return baseTypeRef.GetType(); }
    Value* MakeDefaultValue(IntermediateContext& context) const override;
    void ReplaceForwardReference(FwdDeclaredStructureType* fwdDeclaredType, StructureType* structureType, IntermediateContext* context) override;
private:
    std::int8_t pointerCount;
    TypeRef baseTypeRef;
};

Type* GetElemType(Value* ptr, Value* index, const soul::ast::Span& span, IntermediateContext* context);

class Types
{
public:
    Types() noexcept;
    Types(const Types&) = delete;
    void Init();
    Types& operator=(const Types&) = delete;
    inline IntermediateContext* GetContext() const noexcept { return context; }
    inline void SetContext(IntermediateContext* context_) noexcept { context = context_; }
    void AddStructureType(const soul::ast::Span& span, std::int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs, MetadataRef* mdRef);
    void AddArrayType(const soul::ast::Span& span, std::int32_t typeId, std::int64_t size, const TypeRef& elementTypeRef);
    void AddFunctionType(const soul::ast::Span& span, std::int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs);
    void Resolve(IntermediateContext* context);
    void ResolveType(TypeRef& typeRef, IntermediateContext* context);
    void Add(Type* type, IntermediateContext* context);
    Type* Get(std::int32_t id) const noexcept;
    void Map(Type* type);
    void VisitTypeDeclarations(Visitor& visitor);
    inline VoidType* GetVoidType() const noexcept { return const_cast<VoidType*>(&voidType); }
    inline BoolType* GetBoolType() const noexcept { return const_cast<BoolType*>(&boolType); }
    inline SByteType* GetSByteType() const noexcept { return const_cast<SByteType*>(&sbyteType); }
    inline ByteType* GetByteType() const noexcept { return const_cast<ByteType*>(&byteType); }
    inline ShortType* GetShortType() const noexcept { return const_cast<ShortType*>(&shortType); }
    inline UShortType* GetUShortType() const noexcept { return const_cast<UShortType*>(&ushortType); }
    inline IntType* GetIntType() const noexcept { return const_cast<IntType*>(&intType); }
    inline UIntType* GetUIntType() const noexcept { return const_cast<UIntType*>(&uintType); }
    inline LongType* GetLongType() const noexcept { return const_cast<LongType*>(&longType); }
    inline ULongType* GetULongType() const noexcept { return const_cast<ULongType*>(&ulongType); }
    inline FloatType* GetFloatType() const noexcept { return const_cast<FloatType*>(&floatType); }
    inline DoubleType* GetDoubleType() const noexcept { return const_cast<DoubleType*>(&doubleType); }
    PointerType* MakePointerType(const soul::ast::Span& span, std::int32_t baseTypeId, std::int8_t pointerCount, IntermediateContext* context);
    Type* MakePtrType(Type* baseType);
    StructureType* GetStructureType(const soul::ast::Span& span, std::int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs);
    ArrayType* GetArrayType(const soul::ast::Span& span, std::int32_t typeId, std::int64_t size, const TypeRef& elementTypeRef);
    FunctionType* GetFunctionType(const soul::ast::Span& span, std::int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs);
    FwdDeclaredStructureType* GetFwdDeclaredStructureType(std::uint32_t id);
    FwdDeclaredStructureType* MakeFwdDeclaredStructureType(std::uint32_t id, std::int32_t typeId, const std::string& comment);
    void AddFwdDependentType(FwdDeclaredStructureType* fwdType, Type* type);
    void ResolveForwardReferences(std::uint32_t id, StructureType* structureType);
    void ResolveComments();
    inline std::int32_t NextTypeId() noexcept { return nextTypeId++; }
    void Write(util::CodeFormatter& formatter);
private:
    Type* GetFundamentalType(std::int32_t id) const;
    IntermediateContext* context;
    std::vector<std::unique_ptr<Type>> types;
    std::vector<Type*> declaredTypes;
    std::map<std::int32_t, Type*> typeMap;
    std::map<std::pair<std::int32_t, std::int8_t>, PointerType*> pointerTypeMap;
    std::map<std::vector<std::int32_t>, StructureType*> structureTypeMap;
    std::map<std::pair<std::int64_t, std::int32_t>, ArrayType*> arrayTypeMap;
    std::map<std::pair<std::int32_t, std::vector<std::int32_t>>, FunctionType*> functionTypeMap;
    std::unordered_map<std::uint32_t, FwdDeclaredStructureType*> fwdDeclaredStructureTypes;
    std::unordered_map<std::uint32_t, std::vector<Type*>> fwdDeclarationMap;
    VoidType voidType;
    BoolType boolType;
    SByteType sbyteType;
    ByteType byteType;
    ShortType shortType;
    UShortType ushortType;
    IntType intType;
    UIntType uintType;
    LongType longType;
    ULongType ulongType;
    FloatType floatType;
    DoubleType doubleType;
    int32_t nextTypeId;
};

} // otava::intermediate
