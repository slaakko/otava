// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.types;

import otava.intermediate.context;
import otava.intermediate.error;
import otava.intermediate.visitor;
import util.align;

namespace otava::intermediate {

Type::Type(const soul::ast::Span& span_, TypeKind kind_, std::int32_t id_) noexcept : span(span_), kind(kind_), id(id_), defaultValue(nullptr)
{
}

Type::~Type()
{
}

bool Type::IsIntegerType() const noexcept
{
    switch (id)
    {
    case sbyteTypeId:
    case byteTypeId:
    case shortTypeId:
    case ushortTypeId:
    case intTypeId:
    case uintTypeId:
    case longTypeId:
    case ulongTypeId:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

bool Type::IsUnsignedType() const noexcept
{
    switch (id)
    {
    case byteTypeId:
    case ushortTypeId:
    case uintTypeId:
    case ulongTypeId:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

bool Type::IsSignedType() const noexcept
{
    switch (id)
    {
    case sbyteTypeId:
    case shortTypeId:
    case intTypeId:
    case longTypeId:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

bool Type::IsFloatingPointType() const noexcept
{
    switch (id)
    {
    case floatTypeId:
    case doubleTypeId:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

Type* Type::AddPointer(IntermediateContext* context) const
{
    if (IsPointerType())
    {
        const PointerType* pointerType = static_cast<const PointerType*>(this);
        return context->GetTypes().MakePointerType(Span(), GetBaseTypeId(pointerType->Id()), GetPointerCount(pointerType->Id()) + 1, context);
    }
    else
    {
        return context->GetTypes().MakePointerType(Span(), Id(), 1, context);
    }
}

Type* Type::RemovePointer(const soul::ast::Span& span, IntermediateContext* context) const
{
    if (IsPointerType())
    {
        const PointerType* pointerType = static_cast<const PointerType*>(this);
        return pointerType->BaseType();
    }
    else
    {
        Error("pointer type expected", span, context);
    }
    return nullptr;
}

void Type::ReplaceForwardReference(FwdDeclaredStructureType* fwdDeclaredType, StructureType* structureType, IntermediateContext* context)
{
}

StructureType* Type::GetStructurePointeeType(const soul::ast::Span& span, IntermediateContext* context) const
{
    Type* pointeeType = RemovePointer(span, context);
    if (pointeeType)
    {
        if (pointeeType->IsStructureType())
        {
            return static_cast<StructureType*>(pointeeType);
        }
        else
        {
            Error("structure type expected", span, context);
        }
    }
    else
    {
        Error("pointer type expected", span, context);
    }
    return nullptr;
}

ArrayType* Type::GetArrayPointeeType(const soul::ast::Span& span, IntermediateContext* context) const
{
    Type* pointeeType = RemovePointer(span, context);
    if (pointeeType)
    {
        if (pointeeType->IsArrayType())
        {
            return static_cast<ArrayType*>(pointeeType);
        }
        else
        {
            Error("array type expected", span, context);
        }
    }
    else
    {
        Error("pointer type expected", span, context);
    }
    return nullptr;
}

bool Type::IsFunctionPtrType() const noexcept
{
    if (IsPointerType())
    {
        const PointerType* pointerType = static_cast<const PointerType*>(this);
        if (pointerType->PointerCount() == 1 && pointerType->BaseType()->IsFunctionType())
        {
            return true;
        }
    }
    return false;
}

void Type::Add(Types* types, IntermediateContext* context)
{
}

void Type::Resolve(Types* types, IntermediateContext* context)
{
}

void Type::Write(util::CodeFormatter& formatter)
{
    formatter.Write(Name());
}

void Type::WriteDeclaration(util::CodeFormatter& formatter)
{
    formatter.Write(Name());
    formatter.Write(" = type ");
}

TypeRef Type::GetTypeRef()
{
    TypeRef typeRef = MakeTypeRef(Span(), id, GetPointerCount(id));
    typeRef.SetType(this);
    return typeRef;
}

void Type::IncCount() noexcept
{
}

VoidType::VoidType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, voidTypeId)
{
}

BoolType::BoolType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, boolTypeId)
{
}

Value* BoolType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetFalseValue();
}

SByteType::SByteType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, sbyteTypeId)
{
}

Value* SByteType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetSByteValue(0);
}

ByteType::ByteType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, byteTypeId)
{
}

Value* ByteType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetByteValue(0u);
}

ShortType::ShortType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, shortTypeId)
{
}

Value* ShortType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetShortValue(0);
}

UShortType::UShortType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, ushortTypeId)
{
}

Value* UShortType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetUShortValue(0u);
}

IntType::IntType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, intTypeId)
{
}

Value* IntType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetIntValue(0);
}

UIntType::UIntType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, uintTypeId)
{
}

Value* UIntType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetUIntValue(0u);
}

LongType::LongType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, longTypeId)
{
}

Value* LongType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetLongValue(0);
}

ULongType::ULongType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, ulongTypeId)
{
}

Value* ULongType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetULongValue(0u);
}

FloatType::FloatType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, floatTypeId)
{
}

Value* FloatType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetFloatValue(0.0f);
}

DoubleType::DoubleType() noexcept : Type(soul::ast::Span(), TypeKind::fundamentalType, doubleTypeId)
{
}

Value* DoubleType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetDoubleValue(0.0);
}

TypeRef::TypeRef() noexcept : span(), id(-1), type(nullptr)
{
}

TypeRef::TypeRef(const soul::ast::Span& span_, std::int32_t id_) noexcept : span(span_), id(id_), type(nullptr)
{
}

StructureType::StructureType(const soul::ast::Span& span_, std::int32_t typeId_, const std::vector<TypeRef>& fieldTypeRefs_) :
    Type(span_, TypeKind::structureType, typeId_), fieldTypeRefs(fieldTypeRefs_), sizeAndOffsetsComputed(false), size(0),
    metadataRef(nullptr)
{
}

void StructureType::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StructureType::Add(Types* types, IntermediateContext* context)
{
    types->Add(this, context);
}

void StructureType::Resolve(Types* types, IntermediateContext* context)
{
    for (auto& typeRef : fieldTypeRefs)
    {
        types->ResolveType(typeRef, context);
    }
}

bool StructureType::IsWeakType() const noexcept
{
    int n = FieldCount();
    for (int i = 0; i < n; ++i)
    {
        Type* type = FieldType(i);
        if (type->IsStructureType() || type->IsArrayType() || type->IsFunctionType())
        {
            return false;
        }
    }
    return true;
}

std::int64_t StructureType::Size() const noexcept
{
    if (!sizeAndOffsetsComputed)
    {
        ComputeSizeAndOffsets();
    }
    return size;
}

void StructureType::ComputeSizeAndOffsets() const
{
    sizeAndOffsetsComputed = true;
    std::int64_t offset = 0;
    int n = FieldCount();
    for (int i = 0; i < n; ++i)
    {
        Type* fieldType = FieldType(i);
        std::int64_t memberOffset = offset;
        if (i > 0)
        {
            std::int64_t alignment = fieldType->Alignment();
            memberOffset = alignment * ((offset - 1) / alignment + 1);
        }
        fieldOffsets.push_back(memberOffset);
        offset = memberOffset + fieldType->Size();
    }
    if (n == 0)
    {
        size = 8;
    }
    else
    {
        size = 8 * ((offset - 1) / 8 + 1);
    }
}

Type* StructureType::FieldType(int i) const noexcept
{
    return fieldTypeRefs[i].GetType();
}

std::int64_t StructureType::GetFieldOffset(std::int64_t index) const
{
    if (!sizeAndOffsetsComputed)
    {
        ComputeSizeAndOffsets();
    }
    return fieldOffsets[index];
}

void StructureType::ResolveComment()
{
    if (metadataRef)
    {
        MetadataStruct* metadataStruct = metadataRef->GetMetadataStruct();
        if (metadataStruct)
        {
            MetadataItem* metadataItem = metadataStruct->GetItem("fullName");
            if (metadataItem)
            {
                if (metadataItem->IsMetadataString())
                {
                    MetadataString* metadataString = static_cast<MetadataString*>(metadataItem);
                    SetComment(metadataString->Value());
                }
            }
        }
    }

}

void StructureType::WriteDeclaration(util::CodeFormatter& formatter)
{
    Type::WriteDeclaration(formatter);
    formatter.Write("{ ");
    int n = FieldCount();
    for (int i = 0; i < n; ++i)
    {
        Type* fieldType = FieldType(i);
        if (i > 0)
        {
            formatter.Write(", ");
        }
        fieldType->Write(formatter);
    }
    formatter.Write(" }");
    if (metadataRef)
    {
        formatter.Write(" !" + std::to_string(metadataRef->NodeId()));
    }
    if (!comment.empty())
    {
        formatter.Write(" // " + comment);
    }
}

void StructureType::ReplaceForwardReference(FwdDeclaredStructureType* fwdDeclaredType, StructureType* structureType, IntermediateContext* context)
{
    int n = FieldCount();
    for (int i = 0; i < n; ++i)
    {
        Type* fieldType = FieldType(i);
        if (fieldType == fwdDeclaredType)
        {
            fieldTypeRefs[i] = TypeRef(soul::ast::Span(), structureType->Id());
            context->GetTypes().ResolveType(fieldTypeRefs[i], context);
        }
        else if (fieldType->IsPointerType() && fieldType->BaseType() == fwdDeclaredType)
        {
            PointerType* ptrFieldType = static_cast<PointerType*>(fieldType);
            fieldTypeRefs[i] = TypeRef(soul::ast::Span(), MakePointerTypeId(structureType->Id(), ptrFieldType->PointerCount()));
            context->GetTypes().ResolveType(fieldTypeRefs[i], context);
        }
    }
}

Value* StructureType::MakeDefaultValue(IntermediateContext& context) const
{
    std::vector<Value*> fieldValues;
    int n = FieldCount();
    for (int i = 0; i < n; ++i)
    {
        Type* type = FieldType(i);
        fieldValues.push_back(type->MakeDefaultValue(context));
    }
    return context.MakeStructureValue(soul::ast::Span(), fieldValues, const_cast<StructureType*>(this));
}

void StructureType::SetComment(const std::string& comment_)
{
    comment = comment_;
}

FwdDeclaredStructureType::FwdDeclaredStructureType(const util::uuid& id_, std::int32_t typeId_) noexcept :
    Type(soul::ast::Span(), TypeKind::fwdDeclaredStructureType, typeId_), id(id_)
{
}

void FwdDeclaredStructureType::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FwdDeclaredStructureType::SetComment(const std::string& comment_)
{
    comment = comment_;
}

ArrayType::ArrayType(const soul::ast::Span& span_, std::int32_t typeId_, std::int64_t elementCount_, const TypeRef& elementTypeRef_) noexcept :
    Type(span_, TypeKind::arrayType, typeId_), elementCount(elementCount_), elementTypeRef(elementTypeRef_)
{
}

void ArrayType::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArrayType::Add(Types* types, IntermediateContext* context)
{
    types->Add(this, context);
}

void ArrayType::Resolve(Types* types, IntermediateContext* context)
{
    types->ResolveType(elementTypeRef, context);
}

std::int64_t ArrayType::Size() const noexcept
{
    return util::Align(elementCount * ElementType()->Size(), 8);
}

bool ArrayType::IsWeakType() const noexcept
{
    Type* elementType = ElementType();
    if (elementType->IsStructureType() || elementType->IsArrayType() || elementType->IsFunctionType())
    {
        return false;
    }
    return true;
}

void ArrayType::WriteDeclaration(util::CodeFormatter& formatter)
{
    Type::WriteDeclaration(formatter);
    formatter.Write("[");
    formatter.Write(std::to_string(elementCount));
    formatter.Write(" x ");
    ElementType()->Write(formatter);
    formatter.Write("]");
}

Value* ArrayType::MakeDefaultValue(IntermediateContext& context) const
{
    std::vector<Value*> elements;
    return context.MakeArrayValue(soul::ast::Span(), elements, const_cast<ArrayType*>(this));
}

FunctionType::FunctionType(const soul::ast::Span& span_, std::int32_t typeId_, const TypeRef& returnTypeRef_, const std::vector<TypeRef>& paramTypeRefs_) noexcept :
    Type(span_, TypeKind::functionType, typeId_), returnTypeRef(returnTypeRef_), paramTypeRefs(paramTypeRefs_)
{
}

void FunctionType::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionType::Add(Types* types, IntermediateContext* context)
{
    types->Add(this, context);
}

void FunctionType::Resolve(Types* types, IntermediateContext* context)
{
    types->ResolveType(returnTypeRef, context);
    for (auto& paramTypeRef : paramTypeRefs)
    {
        types->ResolveType(paramTypeRef, context);
    }
}

bool FunctionType::IsWeakType() const noexcept
{
    Type* returnType = ReturnType();
    if (returnType->IsStructureType() || returnType->IsArrayType() || returnType->IsFunctionType())
    {
        return false;
    }
    int n = Arity();
    for (int i = 0; i < n; ++i)
    {
        Type* paramType = ParamType(i);
        if (paramType->IsStructureType() || paramType->IsArrayType() || paramType->IsFunctionType())
        {
            return false;
        }
    }
    return true;
}

void FunctionType::WriteDeclaration(util::CodeFormatter& formatter)
{
    Type::WriteDeclaration(formatter);
    formatter.Write("function ");
    ReturnType()->Write(formatter);
    formatter.Write("(");
    int n = Arity();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            formatter.Write(", ");
        }
        ParamType(i)->Write(formatter);
    }
    formatter.Write(")");
}

bool FunctionType::IsUnaryOperationType() const noexcept
{
    if (Arity() == 1)
    {
        Type* returnType = ReturnType();
        Type* paramType = ParamType(0);
        if (paramType == returnType)
        {
            if (paramType->Size() <= 8)
            {
                return true;
            }
        }
    }
    return false;
}

PointerType::PointerType(const soul::ast::Span& span_, std::int32_t typeId_, std::int8_t pointerCount_, std::int32_t baseTypeId_) noexcept :
    Type(span_, TypeKind::pointerType, typeId_), pointerCount(pointerCount_), baseTypeRef(span_, baseTypeId_)
{
}

Value* PointerType::MakeDefaultValue(IntermediateContext& context) const
{
    return context.GetNullValue(soul::ast::Span(), const_cast<PointerType*>(this));
}

void PointerType::ReplaceForwardReference(FwdDeclaredStructureType* fwdDeclaredType, StructureType* structureType, IntermediateContext* context)
{
    Type* baseType = BaseType();
    if (baseType == fwdDeclaredType)
    {
        baseTypeRef.SetType(structureType);
    }
}

std::string PointerType::Name() const
{
    return baseTypeRef.GetType()->Name() + "*";
}

Type* GetElemType(Value* ptr, Value* index, const soul::ast::Span& span, IntermediateContext* context)
{
    Type* type = ptr->GetType();
    if (type->IsPointerType())
    {
        PointerType* ptrType = static_cast<PointerType*>(type);
        Type* aggregateType = ptrType->BaseType();
        if (aggregateType->IsStructureType())
        {
            StructureType* structureType = static_cast<StructureType*>(aggregateType);
            if (index->IsLongValue())
            {
                std::int64_t idx = static_cast<LongValue*>(index)->GetValue();
                if (idx >= 0 && idx < structureType->FieldCount())
                {
                    return context->MakePtrType(structureType->FieldType(static_cast<int>(idx)));
                }
                else
                {
                    Error("invalid structure element type index", span, context);
                }
            }
            else
            {
                Error("long valued index expected", span, context);
            }
        }
        else if (aggregateType->IsArrayType())
        {
            ArrayType* arrayType = static_cast<ArrayType*>(aggregateType);
            return context->MakePtrType(arrayType->ElementType());
        }
        else if (aggregateType->IsFwdDeclaredStructureType())
        {
            FwdDeclaredStructureType* fwdType = static_cast<FwdDeclaredStructureType*>(aggregateType);
            Error("forward declaration of structure type id " + std::to_string(aggregateType->Id()) + " (" + fwdType->Comment() + ") not resolved", span, context);
        }
        else
        {
            Error("structure or array type expected", span, context);
        }
    }
    else
    {
        Error("pointer type expected", span, context);
    }
    return nullptr;
}

Types::Types() noexcept : context(nullptr), nextTypeId(userTypeId)
{
}

void Types::Init()
{
    boolType.SetDefaultValue(context->GetFalseValue());
    sbyteType.SetDefaultValue(context->GetSByteValue(0));
    byteType.SetDefaultValue(context->GetByteValue(0u));
    shortType.SetDefaultValue(context->GetShortValue(0));
    ushortType.SetDefaultValue(context->GetUShortValue(0u));
    intType.SetDefaultValue(context->GetIntValue(0));
    uintType.SetDefaultValue(context->GetUIntValue(0u));
    longType.SetDefaultValue(context->GetLongValue(0));
    ulongType.SetDefaultValue(context->GetULongValue(0u));
    floatType.SetDefaultValue(context->GetFloatValue(0.0f));
    doubleType.SetDefaultValue(context->GetDoubleValue(0.0));
}

void Types::AddStructureType(const soul::ast::Span& span, std::int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs, MetadataRef* mdRef)
{
    StructureType* structureType = new StructureType(span, typeId, fieldTypeRefs);
    structureType->SetMetadataRef(mdRef);
    types.push_back(std::unique_ptr<Type>(structureType));
}

void Types::AddArrayType(const soul::ast::Span& span, std::int32_t typeId, std::int64_t size, const TypeRef& elementTypeRef)
{
    types.push_back(std::unique_ptr<Type>(new ArrayType(span, typeId, size, elementTypeRef)));
}

void Types::AddFunctionType(const soul::ast::Span& span, std::int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs)
{
    types.push_back(std::unique_ptr<Type>(new FunctionType(span, typeId, returnTypeRef, paramTypeRefs)));
}

void Types::Add(Type* type, IntermediateContext* context)
{
    Type* prev = Get(type->Id());
    if (prev)
    {
        Error("error adding type id " + std::to_string(type->Id()) + ": type id not unique", type->Span(), context, prev->Span());
    }
    Map(type);
    declaredTypes.push_back(type);
}

Type* Types::Get(std::int32_t id) const noexcept
{
    if (IsFundamentalTypeId(id))
    {
        return GetFundamentalType(id);
    }
    auto it = typeMap.find(id);
    if (it != typeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Type* Types::GetFundamentalType(std::int32_t id) const
{
    switch (id)
    {
    case voidTypeId: return const_cast<VoidType*>(&voidType);
    case boolTypeId: return const_cast<BoolType*>(&boolType);
    case sbyteTypeId: return const_cast<SByteType*>(&sbyteType);
    case byteTypeId: return const_cast<ByteType*>(&byteType);
    case shortTypeId: return const_cast<ShortType*>(&shortType);
    case ushortTypeId: return const_cast<UShortType*>(&ushortType);
    case intTypeId: return const_cast<IntType*>(&intType);
    case uintTypeId: return const_cast<UIntType*>(&uintType);
    case longTypeId: return const_cast<LongType*>(&longType);
    case ulongTypeId: return const_cast<ULongType*>(&ulongType);
    case floatTypeId: return const_cast<FloatType*>(&floatType);
    case doubleTypeId: return const_cast<DoubleType*>(&doubleType);
    }
    return nullptr;
}

void Types::Map(Type* type)
{
    typeMap[type->Id()] = type;
}

void Types::VisitTypeDeclarations(Visitor& visitor)
{
    for (Type* declaredType : declaredTypes)
    {
        declaredType->Accept(visitor);
    }
}

PointerType* Types::MakePointerType(const soul::ast::Span& span, std::int32_t baseTypeId, std::int8_t pointerCount, IntermediateContext* context)
{
    auto it = pointerTypeMap.find(std::make_pair(baseTypeId, pointerCount));
    if (it != pointerTypeMap.cend())
    {
        return it->second;
    }
    PointerType* type = nullptr;
    if (pointerCount > 1)
    {
        type = new PointerType(span, MakePointerTypeId(baseTypeId, pointerCount), pointerCount, MakePointerTypeId(baseTypeId, pointerCount - 1));
        type->SetDefaultValue(context->GetNullValue(soul::ast::Span(), type));
    }
    else if (pointerCount == 1)
    {
        type = new PointerType(span, MakePointerTypeId(baseTypeId, pointerCount), pointerCount, baseTypeId);
        type->SetDefaultValue(context->GetNullValue(soul::ast::Span(), type));
    }
    else
    {
        Error("pointer count > 0 expected", span, context);
    }
    ResolveType(type->BaseTypeRef(), context);
    types.push_back(std::unique_ptr<Type>(type));
    Map(type);
    pointerTypeMap[std::make_pair(baseTypeId, pointerCount)] = type;
    return type;
}

Type* Types::MakePtrType(Type* baseType)
{
    Type* type = baseType->AddPointer(context);
    if (baseType->IsFwdDeclaredStructureType())
    {
        FwdDeclaredStructureType* fwdDeclaredStructureType = static_cast<FwdDeclaredStructureType*>(baseType);
        AddFwdDependentType(fwdDeclaredStructureType, type);
    }
    return type;
}

StructureType* Types::GetStructureType(const soul::ast::Span& span, std::int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs)
{
    std::vector<std::int32_t> fieldTypeIds;
    for (const auto& fieldTypeRef : fieldTypeRefs)
    {
        fieldTypeIds.push_back(fieldTypeRef.Id());
    }
    auto it = structureTypeMap.find(fieldTypeIds);
    if (it != structureTypeMap.cend())
    {
        StructureType* structureType = it->second;
        typeMap[typeId] = structureType;
        return structureType;
    }
    StructureType* structureType = new StructureType(span, typeId, fieldTypeRefs);
    types.push_back(std::unique_ptr<Type>(structureType));
    structureType->Add(this, context);
    structureTypeMap[fieldTypeIds] = structureType;
    int n = structureType->FieldCount();
    for (int i = 0; i < n; ++i)
    {
        Type* fieldType = structureType->FieldType(i);
        if (fieldType && fieldType->BaseType()->IsFwdDeclaredStructureType())
        {
            FwdDeclaredStructureType* fwdType = static_cast<FwdDeclaredStructureType*>(fieldType->BaseType());
            AddFwdDependentType(fwdType, structureType);
        }
    }
    return structureType;
}

ArrayType* Types::GetArrayType(const soul::ast::Span& span, std::int32_t typeId, std::int64_t size, const TypeRef& elementTypeRef)
{
    auto key = std::make_pair(size, elementTypeRef.Id());
    auto it = arrayTypeMap.find(key);
    if (it != arrayTypeMap.cend())
    {
        ArrayType* arrayType = it->second;
        typeMap[typeId] = arrayType;
        return arrayType;
    }
    ArrayType* arrayType = new ArrayType(span, typeId, size, elementTypeRef);
    types.push_back(std::unique_ptr<Type>(arrayType));
    arrayTypeMap[key] = arrayType;
    arrayType->Add(this, context);
    return arrayType;
}

FunctionType* Types::GetFunctionType(const soul::ast::Span& span, std::int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs)
{
    std::int32_t returnTypeId = returnTypeRef.Id();
    std::vector<std::int32_t> paramTypeIds;
    for (const auto& paramTypeRef : paramTypeRefs)
    {
        paramTypeIds.push_back(paramTypeRef.Id());
    }
    auto key = std::make_pair(returnTypeId, paramTypeIds);
    auto it = functionTypeMap.find(key);
    if (it != functionTypeMap.cend())
    {
        FunctionType* functionType = it->second;
        typeMap[typeId] = functionType;
        return functionType;
    }
    FunctionType* functionType = new FunctionType(span, typeId, returnTypeRef, paramTypeRefs);
    types.push_back(std::unique_ptr<Type>(functionType));
    functionTypeMap[key] = functionType;
    functionType->Add(this, context);
    return functionType;
}

FwdDeclaredStructureType* Types::GetFwdDeclaredStructureType(const util::uuid& id)
{
    auto it = fwdDeclaredStructureTypes.find(id);
    if (it != fwdDeclaredStructureTypes.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

FwdDeclaredStructureType* Types::MakeFwdDeclaredStructureType(const util::uuid& id, std::int32_t typeId, const std::string& comment)
{
    FwdDeclaredStructureType* fwdDeclaredType = new FwdDeclaredStructureType(id, typeId);
    fwdDeclaredType->SetComment(comment);
    fwdDeclaredStructureTypes[id] = fwdDeclaredType;
    types.push_back(std::unique_ptr<Type>(fwdDeclaredType));
    Map(fwdDeclaredType);
    return fwdDeclaredType;
}

void Types::AddFwdDependentType(FwdDeclaredStructureType* fwdType, Type* type)
{
    std::vector<Type*>& dependentTypes = fwdDeclarationMap[fwdType->Id()];
    if (std::find(dependentTypes.begin(), dependentTypes.end(), type) == dependentTypes.end())
    {
        dependentTypes.push_back(type);
    }
}

void Types::ResolveForwardReferences(const util::uuid& id, StructureType* structureType)
{
    FwdDeclaredStructureType* fwdType = GetFwdDeclaredStructureType(id);
    if (fwdType)
    {
        auto it = fwdDeclarationMap.find(id);
        if (it != fwdDeclarationMap.end())
        {
            std::vector<Type*>& dependentTypes = it->second;
            for (auto& dependentType : dependentTypes)
            {
                dependentType->ReplaceForwardReference(fwdType, structureType, context);
            }
            fwdDeclarationMap.erase(id);
        }
    }
}

void Types::ResolveComments()
{
    for (const auto& type : types)
    {
        if (type->IsStructureType())
        {
            StructureType* structureType = static_cast<StructureType*>(type.get());
            structureType->ResolveComment();
        }
    }
}

void Types::Resolve(IntermediateContext* context)
{
    for (const auto& type : types)
    {
        type->Add(this, context);
    }
    std::vector<Type*> toResolve;
    for (const auto& type : types)
    {
        toResolve.push_back(type.get());
    }
    for (Type* type : toResolve)
    {
        type->Resolve(this, context);
    }
}

void Types::ResolveType(TypeRef& typeRef, IntermediateContext* context)
{
    Type* type = Get(typeRef.Id());
    if (!type)
    {
        if (IsPointerTypeId(typeRef.Id()))
        {
            type = MakePointerType(typeRef.Span(), GetBaseTypeId(typeRef.Id()), GetPointerCount(typeRef.Id()), context);
        }
    }
    if (!type)
    {
        Error("error resolving type: type id " + std::to_string(typeRef.Id()) + " not found", typeRef.Span(), context);
    }
    typeRef.SetType(type);
}

void Types::Write(util::CodeFormatter& formatter)
{
    if (types.empty()) return;
    formatter.WriteLine("types");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& type : declaredTypes)
    {
        type->WriteDeclaration(formatter);
        formatter.WriteLine();
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

} // otava::intermediate