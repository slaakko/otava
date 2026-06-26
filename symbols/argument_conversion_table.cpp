// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.argument_conversion_table;

import otava.symbols.classes;
import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.function_symbol;
import otava.symbols.function_templates;
import otava.symbols.overload_resolution;
import otava.symbols.type_compare;
import otava.symbols.variable_symbol;

namespace otava::symbols {

class IdentityConversion : public FunctionSymbol
{
public:
    IdentityConversion(TypeSymbol* type_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return type; }
    TypeSymbol* ConversionArgType() const noexcept override { return type; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::implicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 0; }
    bool IsIdentityConversion() const noexcept override { return true; }
private:
    TypeSymbol* type;
};

IdentityConversion::IdentityConversion(TypeSymbol* type_, Context* context) : 
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), type(type_)
{
    SetConversion();
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(type, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(type, context);
    SetNoExcept();
}

void IdentityConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
}

class IdentityArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg,
        ArgumentMatch& argumentMatch, FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* IdentityArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg,
    ArgumentMatch& argumentMatch, FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (TypesEqual(argType->PlainType(context), paramType->PlainType(context), context))
    {
        return new IdentityConversion(argType, context);
    }
    return nullptr;
}

class ClassTemplateSpecializationConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg,
        ArgumentMatch& argumentMatch, FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* ClassTemplateSpecializationConversion::Get(
    TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (context->GetFlag(ContextFlags::matchClassTemplateSpecializationConversion))
    {
        context->PushResetFlag(ContextFlags::matchClassTemplateSpecializationConversion);
        bool found = FindClassTemplateSpecializationMatch(argType, paramType, arg, functionMatch, fullSpan, context);
        context->PopFlags();
        if (found)
        {
            return new IdentityConversion(argType, context);
        }
    }
    return nullptr;
}

class DerivedToBaseConversion : public FunctionSymbol
{
public:
    DerivedToBaseConversion(TypeSymbol* derivedTypePtr_, TypeSymbol* baseTypePtr_, int distance_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsDerivedToBaseConversion() const noexcept override { return true; }
    TypeSymbol* ConversionParamType() const noexcept override { return baseTypePtr; }
    TypeSymbol* ConversionArgType() const noexcept override { return derivedTypePtr; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::implicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return distance; }
private:
    TypeSymbol* derivedTypePtr;
    TypeSymbol* baseTypePtr;
    int distance;
};

DerivedToBaseConversion::DerivedToBaseConversion(TypeSymbol* derivedTypePtr_, TypeSymbol* baseTypePtr_, int distance_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), 
    derivedTypePtr(derivedTypePtr_), baseTypePtr(baseTypePtr_), distance(distance_)
{
    SetConversion();
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(derivedTypePtr, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(baseTypePtr, context);
    SetNoExcept();
}

void DerivedToBaseConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ClassTypeSymbol* derivedClassType = nullptr;
    ClassTypeSymbol* baseClassType = nullptr;
    if (derivedTypePtr->GetBaseType(context)->IsClassTypeSymbol() && baseTypePtr->GetBaseType(context)->IsClassTypeSymbol())
    {
        derivedClassType = static_cast<ClassTypeSymbol*>(derivedTypePtr->GetBaseType(context));
        baseClassType = static_cast<ClassTypeSymbol*>(baseTypePtr->GetBaseType(context));
    }
    else
    {
        ThrowException("class pointer types expected", fullSpan, context);
    }
    std::pair<bool, std::int64_t> p = Delta(derivedClassType, baseClassType, emitter, context);
    bool success = p.first;
    std::int64_t delta = p.second;
    if (!success)
    {
        ThrowException("classes have no inheritance relationship", fullSpan, context);
    }
    otava::intermediate::Value* classPtr = emitter.Stack().Pop();
    otava::intermediate::Value* deltaValue = emitter.EmitLong(delta);
    emitter.Stack().Push(emitter.EmitClassPtrConversion(classPtr, deltaValue, baseTypePtr->IrType(emitter, fullSpan, context), true));
}

class DerivedToBaseArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* DerivedToBaseArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg,
    ArgumentMatch& argumentMatch, FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    int distance = 0;
    if (paramType->PointerCount() == 1 && argType->PointerCount() == 1 && argType->GetBaseType(context)->HasBaseClass(paramType->GetBaseType(context), distance, context))
    {
        if (argType->IsReferenceType() && !paramType->IsReferenceType())
        {
            argumentMatch.preConversionFlags = OperationFlags::deref;
        }
        return new DerivedToBaseConversion(argType, paramType, distance, context);
    }
    else if (paramType->IsReferenceType() && paramType->PointerCount() == 0 && argType->IsReferenceType() && argType->PointerCount() == 0 &&
        argType->GetBaseType(context)->HasBaseClass(paramType->GetBaseType(context), distance, context))
    {
        return new DerivedToBaseConversion(argType, paramType, distance, context);
    }
    else if (argType->GetBaseType(context)->HasBaseClass(paramType->GetBaseType(context), distance, context))
    {
        if (paramType->IsReferenceType() && !argType->IsReferenceType())
        {
            argumentMatch.preConversionFlags = OperationFlags::addr;
        }
        return new DerivedToBaseConversion(argType, paramType, distance, context);
    }
    return nullptr;
}

class BaseToDerivedConversion : public FunctionSymbol
{
public:
    BaseToDerivedConversion(TypeSymbol* baseTypePtr_, TypeSymbol* derivedTypePtr_, int distance_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return derivedTypePtr; }
    TypeSymbol* ConversionArgType() const noexcept override { return baseTypePtr; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::explicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return distance; }
private:
    TypeSymbol* baseTypePtr;
    TypeSymbol* derivedTypePtr;
    int distance;
};

BaseToDerivedConversion::BaseToDerivedConversion(TypeSymbol* baseTypePtr_, TypeSymbol* derivedTypePtr_, int distance_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), 
    baseTypePtr(baseTypePtr_), derivedTypePtr(derivedTypePtr_), distance(distance_)
{
    SetConversion();
    SetConversionKind(ConversionKind::explicitConversion);
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(baseTypePtr, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(derivedTypePtr, context);
    SetNoExcept();
}

void BaseToDerivedConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ClassTypeSymbol* baseClassType = nullptr;
    ClassTypeSymbol* derivedClassType = nullptr;
    if (baseTypePtr->GetBaseType(context)->IsClassTypeSymbol() && derivedTypePtr->GetBaseType(context)->IsClassTypeSymbol())
    {
        baseClassType = static_cast<ClassTypeSymbol*>(baseTypePtr->GetBaseType(context));
        derivedClassType = static_cast<ClassTypeSymbol*>(derivedTypePtr->GetBaseType(context));
    }
    else
    {
        ThrowException("class pointer types expected", fullSpan, context);
    }
    std::pair<bool, std::int64_t> p = Delta(baseClassType, derivedClassType, emitter, context);
    bool success = p.first;
    std::int64_t delta = p.second;
    if (!success)
    {
        ThrowException("classes have no inheritance relationship", fullSpan, context);
    }
    otava::intermediate::Value* classPtr = emitter.Stack().Pop();
    otava::intermediate::Value* deltaValue = emitter.EmitLong(delta);
    emitter.Stack().Push(emitter.EmitClassPtrConversion(classPtr, deltaValue, derivedTypePtr->IrType(emitter, fullSpan, context), true));
}

class BaseToDerivedArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* BaseToDerivedArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    int distance = 0;
    if (paramType->PointerCount() == 1 && argType->PointerCount() == 1 && paramType->GetBaseType(context)->HasBaseClass(argType->GetBaseType(context), distance, context))
    {
        if (argType->IsReferenceType() && !paramType->IsReferenceType())
        {
            argumentMatch.preConversionFlags = OperationFlags::deref;
        }
        distance += 100;
        return new BaseToDerivedConversion(argType, paramType, distance, context);
    }
    else if (paramType->IsReferenceType() && paramType->PointerCount() == 0 && argType->IsReferenceType() && argType->PointerCount() == 0 &&
        paramType->GetBaseType(context)->HasBaseClass(argType->GetBaseType(context), distance, context))
    {
        distance += 100;
        return new BaseToDerivedConversion(argType, paramType, distance, context);
    }
    else if (paramType->GetBaseType(context)->HasBaseClass(argType->GetBaseType(context), distance, context))
    {
        if (paramType->IsReferenceType() && !argType->IsReferenceType())
        {
            argumentMatch.preConversionFlags = OperationFlags::addr;
        }
        distance += 100;
        return new BaseToDerivedConversion(argType, paramType, distance, context);
    }
    return nullptr;

}

class DynamicPtrCast : public FunctionSymbol
{
public:
    DynamicPtrCast(ClassTypeSymbol* baseClassType_, ClassTypeSymbol* derivedClassType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    ClassTypeSymbol* baseClassType;
    ClassTypeSymbol* derivedClassType;
};

DynamicPtrCast::DynamicPtrCast(ClassTypeSymbol* baseClassType_, ClassTypeSymbol* derivedClassType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), 
    baseClassType(baseClassType_), derivedClassType(derivedClassType_)
{
    SetConversion();
    SetAccess(Access::public_);
    SetConversionKind(ConversionKind::explicitConversion);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(baseClassType->AddPointer(context), context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(derivedClassType->AddPointer(context), context);
    SetNoExcept();
}

void DynamicPtrCast::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    SymbolId derived = derivedClassType->GetBaseType(context)->Id();
    SymbolId base = baseClassType->GetBaseType(context)->Id();
    std::vector<otava::intermediate::Type*> paramTypes;
    paramTypes.push_back(emitter.GetUIntType());
    paramTypes.push_back(emitter.GetUIntType());
    otava::intermediate::FunctionType* fnType = static_cast<otava::intermediate::FunctionType*>(emitter.MakeFunctionType(emitter.GetBoolType(), paramTypes));
    otava::intermediate::Function* fn = emitter.GetOrInsertFunction("ort_is_same_or_has_base", fnType);
    std::vector<otava::intermediate::Value*> arguments;
    arguments.push_back(emitter.EmitUInt(ToUnderlying(derived)));
    arguments.push_back(emitter.EmitUInt(ToUnderlying(base)));
    otava::intermediate::Value* test = emitter.EmitCall(fn, arguments);
    otava::intermediate::BasicBlock* trueBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* falseBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter.CreateBasicBlock();
    emitter.EmitBranch(test, trueBlock, falseBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    std::pair<bool, std::int64_t> p = Delta(baseClassType, derivedClassType, emitter, context);
    bool success = p.first;
    std::int64_t delta = p.second;
    if (!success)
    {
        ThrowException("classes have no inheritance relationship", fullSpan, context);
    }
    otava::intermediate::Value* deltaValue = emitter.EmitLong(delta);
    otava::intermediate::Type* derivedClassIrType = derivedClassType->IrType(emitter, fullSpan, context)->AddPointer(emitter.GetIntermediateContext());
    otava::intermediate::Value* ptr = emitter.EmitClassPtrConversion(value, deltaValue, derivedClassIrType, false);
    otava::intermediate::Value* result = emitter.EmitLocal(derivedClassIrType);
    emitter.EmitStore(ptr, result);
    emitter.EmitJump(nextBlock);
    emitter.SetCurrentBasicBlock(falseBlock);
    otava::intermediate::Value* null = emitter.EmitNull(derivedClassIrType);
    emitter.EmitStore(null, result);
    emitter.EmitJump(nextBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
    otava::intermediate::Value* loadedResult = emitter.EmitLoad(result);
    emitter.Stack().Push(loadedResult);
}

class NullPtrToPtrConversion : public FunctionSymbol
{
public:
    NullPtrToPtrConversion(TypeSymbol* argType_, TypeSymbol* pointerType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return pointerType; }
    TypeSymbol* ConversionArgType() const noexcept override { return argType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::implicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 1; }
private:
    TypeSymbol* argType;
    TypeSymbol* pointerType;
};

NullPtrToPtrConversion::NullPtrToPtrConversion(TypeSymbol* argType_, TypeSymbol* pointerType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), argType(argType_), pointerType(pointerType_)
{
    SetConversion();
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(argType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(pointerType, context);
    SetNoExcept();
}

void NullPtrToPtrConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitBitcast(value, pointerType->IrType(emitter, fullSpan, context)));
}

class NullPtrToPtrArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* NullPtrToPtrArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (argType->IsNullPtrType() && paramType->IsPointerType())
    {
        return new NullPtrToPtrConversion(argType, paramType, context);
    }
    return nullptr;
}

class VoidPtrToPtrConversion : public FunctionSymbol
{
public:
    VoidPtrToPtrConversion(TypeSymbol* voidPtrType_, TypeSymbol* targetPointerType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return targetPointerType; }
    TypeSymbol* ConversionArgType() const noexcept override { return voidPtrType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::explicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 255; }
private:
    TypeSymbol* voidPtrType;
    TypeSymbol* targetPointerType;
};

VoidPtrToPtrConversion::VoidPtrToPtrConversion(TypeSymbol* voidPtrType_, TypeSymbol* targetPointerType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), 
    voidPtrType(voidPtrType_), targetPointerType(targetPointerType_)
{
    SetConversion();
    SetConversionKind(ConversionKind::explicitConversion);
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(voidPtrType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(targetPointerType, context);
    SetNoExcept();
}

void VoidPtrToPtrConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitBitcast(value, targetPointerType->IrType(emitter, fullSpan, context)));
}

class VoidPtrToPtrArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* VoidPtrToPtrArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (argType->IsPointerType() && argType->PointerCount() == 1 && argType->GetBaseType(context)->IsVoidType() && paramType->IsPointerType())
    {
        return new VoidPtrToPtrConversion(argType, paramType, context);
    }
    return nullptr;
}

class PtrToVoidPtrConversion : public FunctionSymbol
{
public:
    PtrToVoidPtrConversion(TypeSymbol* ptrType_, TypeSymbol* voidPtrType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return voidPtrType; }
    TypeSymbol* ConversionArgType() const noexcept override { return ptrType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::implicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 10; }
private:
    TypeSymbol* ptrType;
    TypeSymbol* voidPtrType;
};

PtrToVoidPtrConversion::PtrToVoidPtrConversion(TypeSymbol* ptrType_, TypeSymbol* voidPtrType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), ptrType(ptrType_), voidPtrType(voidPtrType_)
{
    SetConversion();
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(ptrType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(voidPtrType, context);
    SetNoExcept();
}

void PtrToVoidPtrConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitBitcast(value, voidPtrType->IrType(emitter, fullSpan, context)));
}

class PtrToVoidPtrArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* PtrToVoidPtrArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (argType->IsPointerType() && paramType->IsPointerType() && paramType->PointerCount() == 1 && paramType->GetBaseType(context)->IsVoidType())
    {
        return new PtrToVoidPtrConversion(argType, paramType, context);
    }
    return nullptr;
}

class PtrToPtrConversion : public FunctionSymbol
{
public:
    PtrToPtrConversion(TypeSymbol* sourcePtrType_, TypeSymbol* targetPtrType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return targetPtrType; }
    TypeSymbol* ConversionArgType() const noexcept override { return sourcePtrType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::explicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 255; }
private:
    TypeSymbol* sourcePtrType;
    TypeSymbol* targetPtrType;
};

PtrToPtrConversion::PtrToPtrConversion(TypeSymbol* sourcePtrType_, TypeSymbol* targetPtrType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), 
    sourcePtrType(sourcePtrType_), targetPtrType(targetPtrType_)
{
    SetConversion();
    SetConversionKind(ConversionKind::explicitConversion);
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(sourcePtrType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(targetPtrType, context);
    SetNoExcept();
}

void PtrToPtrConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitBitcast(value, targetPtrType->IrType(emitter, fullSpan, context)));
}

class ReinterpretCastArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* ReinterpretCastArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (context->GetFlag(ContextFlags::reinterpretCast))
    {
        if (argType->IsPointerType() && paramType->IsPointerType() && argType->PointerCount() == 1 && paramType->PointerCount() == 1)
        {
            return new PtrToPtrConversion(argType, paramType, context);
        }
    }
    return nullptr;
}

class VoidPtrToUInt64Conversion : public FunctionSymbol
{
public:
    VoidPtrToUInt64Conversion(TypeSymbol* ptrType_, TypeSymbol* uint64Type_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return uint64Type; }
    TypeSymbol* ConversionArgType() const noexcept override { return ptrType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::explicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 255; }
private:
    TypeSymbol* ptrType;
    TypeSymbol* uint64Type;
};

VoidPtrToUInt64Conversion::VoidPtrToUInt64Conversion(TypeSymbol* ptrType_, TypeSymbol* uint64Type_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), ptrType(ptrType_), uint64Type(uint64Type_)
{
    SetConversion();
    SetConversionKind(ConversionKind::explicitConversion);
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(ptrType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(uint64Type, context);
    SetNoExcept();
}

void VoidPtrToUInt64Conversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitPtrToInt(value, uint64Type->IrType(emitter, fullSpan, context)));
}

class VoidPtrToUInt64ArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* VoidPtrToUInt64ArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    TypeSymbol* uint64Type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedLongLongIntType, context);
    if (argType->PlainType(context)->IsVoidPtrType() && TypesEqual(paramType->PlainType(context), uint64Type, context))
    {
        return new VoidPtrToUInt64Conversion(argType, paramType, context);
    }
    return nullptr;
}

class UInt64ToVoidPtrConversion : public FunctionSymbol
{
public:
    UInt64ToVoidPtrConversion(TypeSymbol* uint64Type_, TypeSymbol* ptrType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return ptrType; }
    TypeSymbol* ConversionArgType() const noexcept override { return uint64Type; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::explicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 255; }
private:
    TypeSymbol* uint64Type;
    TypeSymbol* ptrType;
};

UInt64ToVoidPtrConversion::UInt64ToVoidPtrConversion(TypeSymbol* uint64Type_, TypeSymbol* ptrType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), uint64Type(uint64Type), ptrType(ptrType_)
{
    SetConversion();
    SetConversionKind(ConversionKind::explicitConversion);
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(uint64Type, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(ptrType, context);
    SetNoExcept();
}

void UInt64ToVoidPtrConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitIntToPtr(value, ptrType->IrType(emitter, fullSpan, context)));
}

class UInt64ToVoidPtrArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* UInt64ToVoidPtrArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    TypeSymbol* uint64Type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedLongLongIntType, context);
    if (TypesEqual(argType, uint64Type, context) && paramType->IsVoidPtrType())
    {
        return new UInt64ToVoidPtrConversion(argType, paramType, context);
    }
    return nullptr;
}

class PtrToBooleanConversion : public FunctionSymbol
{
public:
    PtrToBooleanConversion(TypeSymbol* ptrType_, TypeSymbol* boolType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return boolType; }
    TypeSymbol* ConversionArgType() const noexcept override { return ptrType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::implicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 1; }
private:
    TypeSymbol* ptrType;
    TypeSymbol* boolType;
};

PtrToBooleanConversion::PtrToBooleanConversion(TypeSymbol* ptrType_, TypeSymbol* boolType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), ptrType(ptrType_), boolType(boolType_)
{
    SetConversion();
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(ptrType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(boolType, context);
    SetNoExcept();
}

void PtrToBooleanConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    otava::intermediate::Type* irType = static_cast<otava::intermediate::Type*>(ptrType->IrType(emitter, fullSpan, context));
    otava::intermediate::Value* defaultValue = irType->DefaultValue();
    otava::intermediate::Value* equal = emitter.EmitEqual(value, defaultValue);
    otava::intermediate::Value* notEqual = emitter.EmitNot(equal);
    emitter.Stack().Push(notEqual);
}

class PtrToBooleanArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* PtrToBooleanArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (context->GetFlag(ContextFlags::skipFirstPtrToBooleanConversion) && context->ArgIndex() == 0) return nullptr;
    TypeSymbol* boolType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context);
    if (argType->IsPointerType() && TypesEqual(paramType, boolType, context))
    {
        return new PtrToBooleanConversion(argType, paramType, context);
    }
    return nullptr;
}

class ArrayToPtrConversion : public FunctionSymbol
{
public:
    ArrayToPtrConversion(ArrayTypeSymbol* arrayType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return elementPtrType; }
    TypeSymbol* ConversionArgType() const noexcept override { return arrayPtrType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::implicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 1; }
private:
    ArrayTypeSymbol* arrayType;
    TypeSymbol* arrayPtrType;
    TypeSymbol* elementPtrType;
};

ArrayToPtrConversion::ArrayToPtrConversion(ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), 
    arrayType(arrayType_), arrayPtrType(arrayType->AddPointer(context)), elementPtrType(arrayType->ElementType(context)->AddPointer(context))
{
    SetConversion();
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(arrayType->AddPointer(context), context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(elementPtrType, context);
    SetNoExcept();
}

void ArrayToPtrConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* arrayPtr = emitter.Stack().Pop();
    otava::intermediate::Value* elemAddr = emitter.EmitElemAddr(arrayPtr, emitter.EmitLong(0));
    emitter.Stack().Push(elemAddr);
}

class ArrayToPtrArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* ArrayToPtrArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    bool addAddr = false;
    if (argType->IsArrayTypeSymbol())
    {
        addAddr = true;
        argType = argType->AddPointer(context);
    }
    TypeSymbol* at = argType;
    if (at->IsPointerType())
    {
        at = at->RemovePointer(context);
    }
    if (at->IsArrayTypeSymbol())
    {
        ArrayTypeSymbol* arrayType = static_cast<ArrayTypeSymbol*>(at);
        TypeSymbol* elementType = arrayType->ElementType(context)->AddPointer(context);
        if (TypesEqual(paramType->RemoveConst(context), elementType, context))
        {
            if (addAddr)
            {
                argumentMatch.preConversionFlags = OperationFlags::addr;
            }
            return new ArrayToPtrConversion(arrayType, context);
        }
        else
        {
            if (arg && functionMatch.function)
            {
                FunctionMatch tempFunctionMatch;
                tempFunctionMatch = functionMatch;
                if (FindTemplateParameterMatch(elementType, paramType, arg, tempFunctionMatch, fullSpan, context))
                {
                    if (addAddr)
                    {
                        argumentMatch.preConversionFlags = OperationFlags::addr;
                    }
                    return new ArrayToPtrConversion(arrayType, context);
                }
                else if (FindClassTemplateMatch(elementType, paramType, arg, tempFunctionMatch, fullSpan, context))
                {
                    if (addAddr)
                    {
                        argumentMatch.preConversionFlags = OperationFlags::addr;
                    }
                    return new ArrayToPtrConversion(arrayType, context);
                }
                else if (FindClassTemplateSpecializationMatch(elementType, paramType, arg, tempFunctionMatch, fullSpan, context))
                {
                    if (addAddr)
                    {
                        argumentMatch.preConversionFlags = OperationFlags::addr;
                    }
                    return new ArrayToPtrConversion(arrayType, context);
                }
            }
        }
    }
    return nullptr;
}

class EnumTypeToUnderlyingTypeConversion : public FunctionSymbol
{
public:
    EnumTypeToUnderlyingTypeConversion(EnumeratedTypeSymbol* enumType_, TypeSymbol* underlyingType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return underlyingType; }
    TypeSymbol* ConversionArgType() const noexcept override { return enumType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::explicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 100; }
private:
    EnumeratedTypeSymbol* enumType;
    TypeSymbol* underlyingType;
};

EnumTypeToUnderlyingTypeConversion::EnumTypeToUnderlyingTypeConversion(EnumeratedTypeSymbol* enumType_, TypeSymbol* underlyingType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), enumType(enumType_), underlyingType(underlyingType_)
{
    SetConversion();
    SetAccess(Access::public_);
    SetConversionKind(ConversionKind::explicitConversion);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(enumType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(underlyingType, context);
    SetNoExcept();
}

void EnumTypeToUnderlyingTypeConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitBitcast(value, underlyingType->IrType(emitter, fullSpan, context)));
}

class EnumTypeToUnderlyingTypeArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* EnumTypeToUnderlyingTypeArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (argType->IsEnumeratedTypeSymbol())
    {
        EnumeratedTypeSymbol* enumType = static_cast<EnumeratedTypeSymbol*>(argType);
        TypeSymbol* underlyingType = enumType->UnderlyingType(context);
        if (!underlyingType)
        {
            underlyingType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context);
        }
        underlyingType = underlyingType->DirectType(context)->FinalType(fullSpan, context);
        if (TypesEqual(paramType, underlyingType, context))
        {
            return new EnumTypeToUnderlyingTypeConversion(enumType, underlyingType, context);
        }
    }
    return nullptr;
}

class UnderlyingTypeToEnumTypeConversion : public FunctionSymbol
{
public:
    UnderlyingTypeToEnumTypeConversion(EnumeratedTypeSymbol* enumType_, TypeSymbol* underlyingType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    TypeSymbol* ConversionParamType() const noexcept override { return enumType; }
    TypeSymbol* ConversionArgType() const noexcept override { return underlyingType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::explicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return 255; }
private:
    EnumeratedTypeSymbol* enumType;
    TypeSymbol* underlyingType;
};

UnderlyingTypeToEnumTypeConversion::UnderlyingTypeToEnumTypeConversion(EnumeratedTypeSymbol* enumType_, TypeSymbol* underlyingType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), enumType(enumType_), underlyingType(underlyingType_)
{
    SetConversion();
    SetAccess(Access::public_);
    SetConversionKind(ConversionKind::explicitConversion);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(underlyingType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(enumType, context);
    SetNoExcept();
}

void UnderlyingTypeToEnumTypeConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitBitcast(value, enumType->IrType(emitter, fullSpan, context)));
}

class UnderlyingTypeEnumTypeToArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

FunctionSymbol* UnderlyingTypeEnumTypeToArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (paramType->IsEnumeratedTypeSymbol())
    {
        EnumeratedTypeSymbol* enumType = static_cast<EnumeratedTypeSymbol*>(paramType);
        TypeSymbol* underlyingType = enumType->UnderlyingType(context);
        if (!underlyingType)
        {
            underlyingType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context);
        }
        underlyingType = underlyingType->DirectType(context)->FinalType(fullSpan, context);
        if (TypesEqual(argType, underlyingType, context))
        {
            return new UnderlyingTypeToEnumTypeConversion(enumType, underlyingType, context);
        }
    }
    return nullptr;
}

class FunctionToFunctionPtrConversion : public FunctionSymbol
{
public:
    FunctionToFunctionPtrConversion(TypeSymbol* functionPtrType_, FunctionSymbol* function_, int32_t distance_, Context* context);
    TypeSymbol* ConversionParamType() const noexcept override { return functionPtrType; }
    TypeSymbol* ConversionArgType() const noexcept override { return functionType; }
    ConversionKind GetConversionKind() const noexcept override { return ConversionKind::implicitConversion; }
    std::int32_t ConversionDistance() const noexcept override { return distance; }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    TypeSymbol* functionPtrType;
    TypeSymbol* functionType;
    FunctionSymbol* function;
    std::int32_t distance;
};

FunctionToFunctionPtrConversion::FunctionToFunctionPtrConversion(TypeSymbol* functionPtrType_, FunctionSymbol* function_, int32_t distance_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), 
    functionPtrType(functionPtrType_), functionType(function_->GetFunctionType(context)), function(function_), distance(distance_)
{
    SetConversion();
    SetAccess(Access::public_);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(functionType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(functionPtrType, context);
    SetNoExcept();
}

void FunctionToFunctionPtrConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.GetOrInsertFunction(function->IrName(context), static_cast<otava::intermediate::FunctionType*>(
        function->IrType(emitter, fullSpan, context))));
};

class FunctionToFunctionPtrArgumentConversion : public ArgumentConversion
{
public:
    FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
};

struct ClassTemplateSpecializationConversionMatch
{
    ClassTemplateSpecializationConversionMatch(Context* context_, bool set_) : context(context_), set(set_)
    {
        if (set)
        {
            context->PushSetFlag(ContextFlags::matchClassTemplateSpecializationConversion);
        }
    }
    ~ClassTemplateSpecializationConversionMatch()
    {
        if (set)
        {
            context->PopFlags();
        }
    }
    Context* context;
    bool set;
};

FunctionSymbol* FunctionToFunctionPtrArgumentConversion::Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, ArgumentMatch& argumentMatch,
    FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (paramType->IsFunctionPtrType(context) && argType->IsFunctionGroupTypeSymbol())
    {
        for (int stage = 0; stage < 2; ++stage)
        {
            ClassTemplateSpecializationConversionMatch match(context, stage == 1);
            FunctionTypeSymbol* functionType = static_cast<FunctionTypeSymbol*>(paramType->GetBaseType(context));
            FunctionGroupTypeSymbol* functionGroupType = static_cast<FunctionGroupTypeSymbol*>(argType);
            FunctionGroupSymbol* functionGroup = functionGroupType->GetFunctionGroup();
            for (FunctionSymbol* functionSymbol : functionGroup->Functions(context))
            {
                Cardinality n = Cardinality(functionType->ParameterTypes(context).size());
                if (Cardinality(functionSymbol->Parameters(context).size()) == n)
                {
                    bool found = true;
                    for (Index i = Index(0); i < Index(n); ++i)
                    {
                        TypeSymbol* leftType = functionType->ParameterTypes(context)[ToUnderlying(i)]->DirectType(context)->FinalType(fullSpan, context);;
                        TypeSymbol* rightType = functionSymbol->Parameters(context)[ToUnderlying(i)]->GetType(context)->DirectType(context)->FinalType(fullSpan, context);
                        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                            leftType, rightType, fullSpan, context);
                        if (!conversion)
                        {
                            found = false;
                            break;
                        }
                    }
                    if (found)
                    {
                        TypeSymbol* leftType = functionType->ReturnType(context)->DirectType(context)->FinalType(fullSpan, context);
                        TypeSymbol* rightType = functionSymbol->ReturnType(context)->DirectType(context)->FinalType(fullSpan, context);
                        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                            leftType, rightType, fullSpan, context);
                        if (conversion)
                        {
                            return new FunctionToFunctionPtrConversion(paramType, functionSymbol, 1 + conversion->ConversionDistance(), context);
                        }
                    }
                }
            }
            for (FunctionSymbol* functionDefinitionSymbol : functionGroup->Definitions(context))
            {
                Cardinality n = Cardinality(functionType->ParameterTypes(context).size());
                if (Cardinality(functionDefinitionSymbol->Parameters(context).size()) == n)
                {
                    bool found = true;
                    for (Index i = Index(0); i < Index(n); ++i)
                    {
                        TypeSymbol* leftType = functionType->ParameterTypes(context)[ToUnderlying(i)]->DirectType(context)->FinalType(fullSpan, context);
                        TypeSymbol* rightType = functionDefinitionSymbol->Parameters(
                            context)[ToUnderlying(i)]->GetType(context)->DirectType(context)->FinalType(fullSpan, context);
                        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                            leftType, rightType, fullSpan, context);
                        if (!conversion)
                        {
                            found = false;
                            break;
                        }
                    }
                    if (found)
                    {
                        TypeSymbol* leftType = functionType->ReturnType(context)->DirectType(context)->FinalType(fullSpan, context);
                        TypeSymbol* rightType = functionDefinitionSymbol->ReturnType(context)->DirectType(context)->FinalType(fullSpan, context);
                        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                            leftType, rightType, fullSpan, context);
                        if (conversion)
                        {
                            if (functionDefinitionSymbol->IsTemplate(context))
                            {
                                functionDefinitionSymbol = InstantiateFunctionTemplate(functionDefinitionSymbol, functionMatch.templateParameterMap, fullSpan, context);
                            }
                            return new FunctionToFunctionPtrConversion(paramType, functionDefinitionSymbol, 1 + conversion->ConversionDistance(), context);
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

class AdjustDeletePtrConversionFn : public FunctionSymbol
{
public:
    AdjustDeletePtrConversionFn(TypeSymbol* thisPtrBaseType_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    TypeSymbol* thisPtrBaseType;
};

AdjustDeletePtrConversionFn::AdjustDeletePtrConversionFn(TypeSymbol* thisPtrBaseType_, Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@conversion"), thisPtrBaseType(thisPtrBaseType_)
{
    SetConversion();
    SetAccess(Access::public_);
    SetConversionKind(ConversionKind::explicitConversion);
    TypeSymbol* voidType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::voidType, context);
    TypeSymbol* voidPtrType = voidType->AddPointer(context);
    ParameterSymbol* arg = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
    arg->SetType(voidPtrType, context);
    AddSymbol(arg, soul::ast::FullSpan(), context);
    SetReturnType(voidPtrType, context);
    SetNoExcept();
}

void AdjustDeletePtrConversionFn::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ClassTypeSymbol* classType = nullptr;
    if (thisPtrBaseType->IsClassTypeSymbol())
    {
        classType = static_cast<ClassTypeSymbol*>(thisPtrBaseType);
    }
    else
    {
        ThrowException("class type expected", fullSpan, context);
    }
    std::vector<ClassTypeSymbol*> vptrHolderClasses = classType->VPtrHolderClasses(context);
    if (vptrHolderClasses.empty())
    {
        ThrowException("no vptr holder classes for the class '" + classType->FullName(context) + "'", fullSpan, context);
    }
    otava::intermediate::Value* thisPtr = emitter.Stack().Pop();
    emitter.Stack().Push(thisPtr);
    ClassTypeSymbol* vptrHolderClass = vptrHolderClasses.front();
    if (classType != vptrHolderClass)
    {
        thisPtr = emitter.EmitBitcast(thisPtr, vptrHolderClass->AddPointer(context)->IrType(emitter, fullSpan, context));
    }
    otava::intermediate::Value* vptrPtr = emitter.EmitElemAddr(thisPtr, emitter.EmitLong(vptrHolderClass->VPtrIndex()));
    otava::intermediate::Value* voidVPtr = emitter.EmitLoad(vptrPtr);
    otava::intermediate::Value* vptr = emitter.EmitBitcast(voidVPtr, classType->VPtrType(emitter));
    otava::intermediate::Value* objectDeltaPtrElem = emitter.EmitElemAddr(thisPtr, emitter.EmitLong(vptrHolderClass->DeltaIndex()));
    otava::intermediate::Type* deltaPtrType = emitter.MakePtrType(emitter.GetLongType());
    otava::intermediate::Value* objectDeltaPtr = emitter.EmitBitcast(objectDeltaPtrElem, deltaPtrType);
    otava::intermediate::Value* objectDelta = emitter.EmitLoad(objectDeltaPtr);
    otava::intermediate::Value* adjustedObjectPtr = emitter.EmitClassPtrConversion(thisPtr, objectDelta, thisPtr->GetType(), false);
    emitter.Stack().Push(adjustedObjectPtr);
}

ArgumentConversionTable::ArgumentConversionTable()
{
    AddArgumentConversion(new IdentityArgumentConversion());
    AddArgumentConversion(new ClassTemplateSpecializationConversion());
    AddArgumentConversion(new DerivedToBaseArgumentConversion());
    AddArgumentConversion(new BaseToDerivedArgumentConversion());
    AddArgumentConversion(new NullPtrToPtrArgumentConversion());
    AddArgumentConversion(new VoidPtrToPtrArgumentConversion());
    AddArgumentConversion(new PtrToVoidPtrArgumentConversion());
    AddArgumentConversion(new ReinterpretCastArgumentConversion());
    AddArgumentConversion(new VoidPtrToUInt64ArgumentConversion());
    AddArgumentConversion(new UInt64ToVoidPtrArgumentConversion());
    AddArgumentConversion(new PtrToBooleanArgumentConversion());
    AddArgumentConversion(new ArrayToPtrArgumentConversion());
    AddArgumentConversion(new EnumTypeToUnderlyingTypeArgumentConversion());
    AddArgumentConversion(new UnderlyingTypeEnumTypeToArgumentConversion());
    AddArgumentConversion(new FunctionToFunctionPtrArgumentConversion());
}

FunctionSymbol* ArgumentConversionTable::GetAdjustDeletePtrConversionFn(TypeSymbol* thisPtrBaseType, Context* context)
{
    auto it = adjustDeletePtrConversionFns.find(thisPtrBaseType);
    if (it != adjustDeletePtrConversionFns.end())
    {
        return it->second;
    }
    FunctionSymbol* adjustDeletePtrConversion = new AdjustDeletePtrConversionFn(thisPtrBaseType, context);
    conversionFunctions.push_back(std::unique_ptr<FunctionSymbol>(adjustDeletePtrConversion));
    adjustDeletePtrConversionFns[thisPtrBaseType] = adjustDeletePtrConversion;
    return adjustDeletePtrConversion;
}

FunctionSymbol* ArgumentConversionTable::GetDynamicPtrCastFn(TypeSymbol* baseClassPtr, TypeSymbol* derivedClassPtr, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (baseClassPtr->PointerCount() != 1 || !baseClassPtr->GetBaseType(context)->IsClassTypeSymbol() ||
        derivedClassPtr->PointerCount() != 1 || !derivedClassPtr->GetBaseType(context)->IsClassTypeSymbol())
    {
        ThrowException("invalid dynamic cast arguments: both arguments should have pointer to class type", fullSpan, context);
    }
    std::pair<TypeSymbol*, TypeSymbol*> p = std::make_pair(baseClassPtr, derivedClassPtr);
    auto it = dynamicPtrCastFns.find(p);
    if (it != dynamicPtrCastFns.end())
    {
        return it->second;
    }
    FunctionSymbol* dynamicPtrCast = new DynamicPtrCast(static_cast<ClassTypeSymbol*>(baseClassPtr->GetBaseType(context)),
        static_cast<ClassTypeSymbol*>(derivedClassPtr->GetBaseType(context)), context);
    conversionFunctions.push_back(std::unique_ptr<FunctionSymbol>(dynamicPtrCast));
    dynamicPtrCastFns[p] = dynamicPtrCast;
    return dynamicPtrCast;
}

void ArgumentConversionTable::AddArgumentConversion(ArgumentConversion* argumentConversion)
{
    argumentConversions.push_back(std::unique_ptr<ArgumentConversion>(argumentConversion));
}

FunctionSymbol* ArgumentConversionTable::GetArgumentConversion(TypeSymbol* paramType, TypeSymbol* argType, const soul::ast::FullSpan& fullSpan,
    Context* context)
{
    ArgumentMatch argumentMatch;
    FunctionMatch functionMatch;
    return GetArgumentConversion(paramType, argType, nullptr, fullSpan, argumentMatch, functionMatch, context);
}

FunctionSymbol* ArgumentConversionTable::GetArgumentConversion(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg,
    const soul::ast::FullSpan& fullSpan, ArgumentMatch& argumentMatch, FunctionMatch& functionMatch, Context* context)
{
    FunctionSymbol* conversion = nullptr;
    if (!TypesEqual(paramType, argType, context))
    {
        int paramRank = paramType->Rank(context);
        int argumentRank = argType->Rank(context);
        if (paramRank != -1 && argumentRank != -1 && !context->GetFlag(ContextFlags::cast))
        {
            if (paramRank < argumentRank)
            {
                argumentMatch.distance += truncateConversionDistance + argumentRank - paramRank;
            }
            else if (argumentRank < paramRank)
            {
                argumentMatch.distance += paramRank - argumentRank;
            }
            if (argType->IsUnsignedIntegerType() && paramType->IsSignedIntegerType())
            {
                argumentMatch.distance += unsignedToSignedConversionDistance;
            }
        }
        TypeSymbol* plainParamType = paramType->PlainType(context);
        TypeSymbol* plainArgType = argType->PlainType(context);
        Module* module = context->GetModule();
        bool setArgTypeModule = false;
        if (plainParamType->IsFundamentalTypeSymbol() && plainArgType->IsFundamentalTypeSymbol())
        {
            module = context->GetStdTypeFundamentalModule();
            conversion = module->GetSymbolTable()->GetConversionTable()->GetConversion(plainParamType, plainArgType, context);
        }
        else
        {
            conversion = module->GetSymbolTable()->GetConversionTable()->GetConversion(plainParamType, plainArgType, context);
        }
        if (!conversion)
        {
            module = paramType->GetBaseType(context)->GetModule();
            setArgTypeModule = true;
            conversion = module->GetSymbolTable()->GetConversionTable()->GetConversion(plainParamType, plainArgType, context);
        }
        if (!conversion && setArgTypeModule)
        {
            module = argType->GetBaseType(context)->GetModule();
            conversion = module->GetSymbolTable()->GetConversionTable()->GetConversion(plainParamType, plainArgType, context);
        }
        if (conversion)
        {
            if (conversion->GetConversionArgType(context)->IsReferenceType() && !argType->IsReferenceType() || 
                conversion->GetConversionArgType(context)->IsClassTypeSymbol())
            {
                argumentMatch.preConversionFlags = OperationFlags::addr;
            }
            else if (!conversion->GetConversionArgType(context)->IsReferenceType() && argType->IsReferenceType())
            {
                argumentMatch.preConversionFlags = OperationFlags::deref;
            }
        }
    }
    if (conversion)
    {
        return conversion;
    }
    if (argType->PlainType(context)->IsClassTypeSymbol())
    {
        ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(argType->GetBaseType(context));
        if (classType->IsClassTemplateSpecializationSymbol())
        {
            ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(classType);
            TypeSymbol* classTemplateSymbol = specialization->ClassTemplate(context);
            if (classTemplateSymbol->IsClassTypeSymbol())
            {
                ClassTypeSymbol* classTemplate = static_cast<ClassTypeSymbol*>(classTemplateSymbol);
                FunctionSymbol* conversionFunction = classTemplate->GetConversionFunction(paramType, context);
                if (conversionFunction)
                {
                    std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual> templateParameterMap;
                    FunctionSymbol* instantiatedConversionFunction = InstantiateMemFnOfClassTemplate(
                        conversionFunction, specialization, templateParameterMap, fullSpan, context);
                    return instantiatedConversionFunction;
                }
            }
        }
        else
        {
            FunctionSymbol* conversionFunction = classType->GetConversionFunction(paramType, context);
            if (conversionFunction)
            {
                return conversionFunction;
            }
        }
    }
    for (const auto& argumentConversion : argumentConversions)
    {
        FunctionSymbol* conversionFunction = argumentConversion->Get(paramType, argType, arg, argumentMatch, functionMatch, fullSpan, context);
        if (conversionFunction)
        {
            conversionFunctions.push_back(std::unique_ptr<FunctionSymbol>(conversionFunction));
            return conversionFunction;
        }
    }
    return nullptr;
}

} // namespace otava::symbols
