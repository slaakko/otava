// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.emitter;

import otava.symbols.bound_tree;
import otava.intermediate.reference_resolver;
import util.unicode;

namespace otava::symbols {

Emitter::Emitter() : context(new otava::intermediate::IntermediateContext()), stack(new IrValueStack()), line(0), check(false), retValue(nullptr)
{
}

Emitter::~Emitter()
{
    delete context;
    delete stack;
}

void Emitter::Emit()
{
    context->WriteFile();
}

void Emitter::SetCompileUnitInfo(const std::string& compileUnitId, const std::string& sourceFilePath)
{
    context->SetCompileUnitInfo(compileUnitId, sourceFilePath);
}

void Emitter::ResolveReferences()
{
    otava::intermediate::ResolveReferences(*context);
}

otava::intermediate::Function* Emitter::CreateFunction(const std::string& name, otava::intermediate::Type* type, bool inline_, bool once, bool child)
{
    otava::intermediate::Function* function = context->AddFunctionDefinition(soul::ast::Span(), type, name, inline_, once, child, true, nullptr);
    context->SetCurrentFunction(function);
    return function;
}

void Emitter::SetRegNumbers()
{
    context->CurrentFunction()->SetNumbers();
}

otava::intermediate::Type* Emitter::MakeStructureType(const std::vector<otava::intermediate::Type*>& elementTypes, const std::string& comment)
{
    std::vector<otava::intermediate::TypeRef> fieldTypeRefs;
    for (otava::intermediate::Type* elementType : elementTypes)
    {
        fieldTypeRefs.push_back(elementType->GetTypeRef());
    }
    otava::intermediate::StructureType* structureType = context->GetStructureType(soul::ast::Span(), context->NextTypeId(), fieldTypeRefs);
    structureType->SetComment(comment);
    return structureType;
}

otava::intermediate::Type* Emitter::MakeFunctionType(otava::intermediate::Type* returnType, const std::vector<otava::intermediate::Type*>& paramTypes)
{
    otava::intermediate::TypeRef returnTypeRef = returnType->GetTypeRef();
    std::vector<otava::intermediate::TypeRef> paramTypeRefs;
    for (otava::intermediate::Type* paramType : paramTypes)
    {
        paramTypeRefs.push_back(paramType->GetTypeRef());
    }
    return context->GetFunctionType(soul::ast::Span(), context->NextTypeId(), returnTypeRef, paramTypeRefs);
}

otava::intermediate::Type* Emitter::MakeArrayType(std::int64_t size, otava::intermediate::Type* elementType)
{
    otava::intermediate::TypeRef elementTypeRef = elementType->GetTypeRef();
    return context->GetArrayType(soul::ast::Span(), context->NextTypeId(), size, elementTypeRef);
}

otava::intermediate::Type* Emitter::GetOrInsertFwdDeclaredStructureType(const util::uuid& id, const std::string& comment)
{
    otava::intermediate::Type* type = context->GetFwdDeclaredStructureType(id);
    if (type)
    {
        return type;
    }
    else
    {
        return MakeFwdDeclaredStructureType(id, comment);
    }
}

void Emitter::ResolveForwardReferences(const util::uuid& typeId, otava::intermediate::StructureType* structureType)
{
    context->ResolveForwardReferences(typeId, structureType);
}

otava::intermediate::Value* Emitter::EmitBool(bool value)
{
    if (value)
    {
        return context->GetTrueValue();
    }
    else
    {
        return context->GetFalseValue();
    }
}

otava::intermediate::Value* Emitter::EmitStringValue(const std::string& value)
{
    otava::intermediate::Value* stringValue = context->MakeStringValue(soul::ast::Span(), value, false);
    otava::intermediate::GlobalVariable* globalVar = context->GetGlobalVariableForString(stringValue, GetByteType());
    return globalVar;
}

otava::intermediate::Value* Emitter::EmitString16Value(const std::string& value)
{
    std::u16string str = util::ToUtf16(value);
    std::vector<otava::intermediate::Value*> elements;
    for (const char16_t x : str)
    {
        elements.push_back(EmitUShort(static_cast<std::uint16_t>(x)));
    }
    elements.push_back(EmitUShort(static_cast<std::uint16_t>(0)));
    otava::intermediate::Value* string16Value = context->MakeStringArrayValue(soul::ast::Span(), 'w', elements);
    otava::intermediate::GlobalVariable* globalVar = context->GetGlobalVariableForString(string16Value, GetUShortType());
    return globalVar;
}

otava::intermediate::Value* Emitter::EmitString32Value(const std::string& value)
{
    std::u32string str = util::ToUtf32(value);
    std::vector<otava::intermediate::Value*> elements;
    for (const char32_t x : str)
    {
        elements.push_back(EmitUInt(static_cast<std::uint32_t>(x)));
    }
    elements.push_back(EmitUInt(static_cast<std::uint32_t>(0)));
    otava::intermediate::Value* string16Value = context->MakeStringArrayValue(soul::ast::Span(), 'u', elements);
    otava::intermediate::GlobalVariable* globalVar = context->GetGlobalVariableForString(string16Value, GetUIntType());
    return globalVar;
}

otava::intermediate::Value* Emitter::EmitCall(otava::intermediate::Value* function, const std::vector<otava::intermediate::Value*>& args)
{
    std::int32_t n = args.size();
    for (std::int32_t i = 0; i < n; ++i)
    {
        otava::intermediate::Value* arg = args[i];
        context->CreateArg(arg);
    }
    return context->CreateCall(function);
}

otava::intermediate::Type* Emitter::GetType(const util::uuid& id) const noexcept
{
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

void Emitter::SetType(const util::uuid& id, otava::intermediate::Type* type)
{
    typeMap[id] = type;
}

otava::intermediate::Value* Emitter::GetIrObject(void* symbol) const noexcept
{
    auto it = irObjectMap.find(symbol);
    if (it != irObjectMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void Emitter::SetIrObject(void* symbol, otava::intermediate::Value* irObject)
{
    irObjectMap[symbol] = irObject;
}

otava::intermediate::Value* Emitter::GetVTabVariable(const std::u32string& className) const noexcept
{
    auto it = vtabVariableMap.find(className);
    if (it != vtabVariableMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void Emitter::SetVTabVariable(const std::u32string& className, otava::intermediate::Value* vtabVariable)
{
    vtabVariableMap[className] = vtabVariable;
}

otava::intermediate::Value* Emitter::EmitClassPtrConversion(otava::intermediate::Value* classPtr, otava::intermediate::Value* delta,
    otava::intermediate::Type* destType, bool checkNull)
{
    if (delta->IsZero())
    {
        return EmitBitcast(classPtr, destType);
    }
    if (checkNull)
    {
        otava::intermediate::BasicBlock* adjust = CreateBasicBlock();
        otava::intermediate::BasicBlock* storeNull = CreateBasicBlock();
        otava::intermediate::BasicBlock* load = CreateBasicBlock();
        otava::intermediate::Type* bytePtrType = MakePtrType(GetByteType());
        otava::intermediate::Value* result = EmitLocal(bytePtrType);
        otava::intermediate::Value* adjustableClassPtr = EmitBitcast(classPtr, bytePtrType);
        otava::intermediate::Value* null = EmitNull(bytePtrType);
        otava::intermediate::Value* isNull = EmitEqual(adjustableClassPtr, null);
        EmitBranch(isNull, storeNull, adjust);
        SetCurrentBasicBlock(storeNull);
        EmitStore(null, result);
        EmitJump(load);
        SetCurrentBasicBlock(adjust);
        otava::intermediate::Value* adjustedClassPtr = EmitPtrOffset(adjustableClassPtr, delta);
        EmitStore(adjustedClassPtr, result);
        EmitJump(load);
        SetCurrentBasicBlock(load);
        otava::intermediate::Value* loadedResult = EmitLoad(result);
        return EmitBitcast(loadedResult, destType);
    }
    else
    {
        otava::intermediate::Type* bytePtrType = MakePtrType(GetByteType());
        otava::intermediate::Value* adjustableClassPtr = EmitBitcast(classPtr, bytePtrType);
        otava::intermediate::Value* adjustedClassPtr = EmitPtrOffset(adjustableClassPtr, delta);
        return EmitBitcast(adjustedClassPtr, destType);
    }
}

} // namespace otava::symbols
