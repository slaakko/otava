// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.operation_repository;

import otava.symbols.argument_conversion_table;
import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.expression_binder;
import otava.symbols.function_symbol;
import otava.symbols.overload_resolution;
import otava.symbols.type_compare;
import otava.symbols.variable_symbol;
import otava.ast.expression;
import util.sha1;

namespace otava::symbols {

class PointerDefaultCtor : public FunctionSymbol
{
public:
    PointerDefaultCtor(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    inline TypeSymbol* Type() const noexcept { return type; }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
private:
    TypeSymbol* type;
};

PointerDefaultCtor::PointerDefaultCtor(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@constructor"), type(type_)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, fullSpan, context);
    SetNoExcept();
}

void PointerDefaultCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    if ((flags & OperationFlags::defaultInit) != OperationFlags::none)
    {
        emitter.Stack().Push(type->IrType(emitter, fullSpan, context)->DefaultValue());
        OperationFlags storeFlags = OperationFlags::none;
        if ((flags & OperationFlags::storeDeref) != OperationFlags::none)
        {
            storeFlags = storeFlags | OperationFlags::deref;
        }
        args[0]->Store(emitter, storeFlags, fullSpan, context);
    }
}

class PointerDefaultCtorOperation : public Operation
{
public:
    PointerDefaultCtorOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerDefaultCtorOperation::PointerDefaultCtorOperation() : Operation("@constructor", 1)
{
}

FunctionSymbol* PointerDefaultCtorOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    BoundExpressionNode* arg = args[0].get();
    TypeSymbol* type = arg->GetType();
    if (type->PointerCount() <= 1) return nullptr;
    if (type->IsReferenceType()) return nullptr;
    TypeSymbol* pointerType = type->RemovePointer(context);
    auto it = functionMap.find(pointerType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerDefaultCtor* function = new PointerDefaultCtor(pointerType, fullSpan, context);
    functionMap[pointerType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerCopyCtor : public FunctionSymbol
{
public:
    PointerCopyCtor(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    inline TypeSymbol* Type() const noexcept { return type; }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
private:
    TypeSymbol* type;
};

PointerCopyCtor::PointerCopyCtor(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@constructor"), type(type_)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, fullSpan, context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(type, context);
    AddSymbol(thatParam, fullSpan, context);
    SetNoExcept();
}

void PointerCopyCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    if ((flags & OperationFlags::dup) != OperationFlags::none)
    {
        emitter.Stack().Dup();
    }
    OperationFlags storeFlags = OperationFlags::none;
    if ((flags & OperationFlags::storeDeref) != OperationFlags::none)
    {
        storeFlags = storeFlags | OperationFlags::deref;
    }
    args[0]->Store(emitter, storeFlags, fullSpan, context);
}

class PointerCopyCtorOperation : public Operation
{
public:
    PointerCopyCtorOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerCopyCtorOperation::PointerCopyCtorOperation() : Operation("@constructor", 2)
{
}

FunctionSymbol* PointerCopyCtorOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    BoundExpressionNode* arg = args[0].get();
    TypeSymbol* type = arg->GetType();
    if (type->PointerCount() <= 1) return nullptr;
    if (type->IsReferenceType()) return nullptr;
    TypeSymbol* pointerType = type->RemovePointer(context);
    TypeSymbol* rvalueRefType = pointerType->AddRValueRef(context);
    if (TypesEqual(args[1]->GetType(), rvalueRefType, context)) return nullptr;
    auto it = functionMap.find(pointerType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerCopyCtor* function = new PointerCopyCtor(pointerType, fullSpan, context);
    functionMap[pointerType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerMoveCtor : public FunctionSymbol
{
public:
    PointerMoveCtor(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    inline TypeSymbol* Type() const noexcept { return type; }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
private:
    TypeSymbol* type;
};

PointerMoveCtor::PointerMoveCtor(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@constructor"), type(type_)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, fullSpan, context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(type->AddRValueRef(context), context);
    AddSymbol(thatParam, fullSpan, context);
    SetNoExcept();
}

void PointerMoveCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitLoad(rvalueRefValue));
    OperationFlags storeFlags = OperationFlags::none;
    if ((flags & OperationFlags::storeDeref) != OperationFlags::none)
    {
        storeFlags = storeFlags | OperationFlags::deref;
    }
    args[0]->Store(emitter, storeFlags, fullSpan, context);
}

class PointerMoveCtorOperation : public Operation
{
public:
    PointerMoveCtorOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerMoveCtorOperation::PointerMoveCtorOperation() : Operation("@constructor", 2)
{
}

FunctionSymbol* PointerMoveCtorOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    BoundExpressionNode* arg = args[0].get();
    TypeSymbol* type = arg->GetType();
    if (type->PointerCount() <= 1) return nullptr;
    if (type->IsReferenceType()) return nullptr;
    TypeSymbol* pointerType = type->RemovePointer(context);
    TypeSymbol* rvalueRefType = pointerType->AddRValueRef(context);
    if (!TypesEqual(args[1]->GetType(), rvalueRefType, context) && !args[1]->BindToRvalueRef()) return nullptr;
    auto it = functionMap.find(pointerType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerMoveCtor* function = new PointerMoveCtor(pointerType, fullSpan, context);
    functionMap[pointerType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerCopyAssignment : public FunctionSymbol
{
public:
    PointerCopyAssignment(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    inline TypeSymbol* Type() const noexcept { return type; }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    bool IsPointerCopyAssignment() const noexcept override { return true; }
private:
    TypeSymbol* type;
};

PointerCopyAssignment::PointerCopyAssignment(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "operator="), type(type_)
{
    SetFunctionKind(FunctionKind::special);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, fullSpan, context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(type, context);
    AddSymbol(thatParam, fullSpan, context);
    SetNoExcept();
    SetReturnType(type->AddLValueRef(context), context);
    SetNoExcept();
}

void PointerCopyAssignment::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    args[0]->Store(emitter, OperationFlags::setPtr, fullSpan, context);
    emitter.Stack().Push(context->Ptr());
}

class PointerCopyAssignmentOperation : public Operation
{
public:
    PointerCopyAssignmentOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerCopyAssignmentOperation::PointerCopyAssignmentOperation() : Operation("operator=", 2)
{
}

FunctionSymbol* PointerCopyAssignmentOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    BoundExpressionNode* arg = args[0].get();
    TypeSymbol* type = arg->GetType();
    if (type->PointerCount() <= 1) return nullptr;
    TypeSymbol* pointerType = type->RemovePointer(context)->RemoveReference(context);
    if (TypesEqual(args[1]->GetType(), pointerType->AddRValueRef(context), context) || args[1]->BindToRvalueRef()) return nullptr;
    auto it = functionMap.find(pointerType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerCopyAssignment* function = new PointerCopyAssignment(pointerType, fullSpan, context);
    functionMap[pointerType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerMoveAssignment : public FunctionSymbol
{
public:
    PointerMoveAssignment(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    inline TypeSymbol* Type() const noexcept { return type; }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
private:
    TypeSymbol* type;
};

PointerMoveAssignment::PointerMoveAssignment(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "operator="), type(type_)
{
    SetFunctionKind(FunctionKind::special);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, fullSpan, context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(type->AddRValueRef(context), context);
    AddSymbol(thatParam, fullSpan, context);
    SetReturnType(type->AddLValueRef(context), context);
    SetNoExcept();
}

void PointerMoveAssignment::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* refValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitLoad(refValue));
    args[0]->Store(emitter, OperationFlags::setPtr, fullSpan, context);
    emitter.Stack().Push(context->Ptr());
}

class PointerMoveAssignmentOperation : public Operation
{
public:
    PointerMoveAssignmentOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerMoveAssignmentOperation::PointerMoveAssignmentOperation() : Operation("operator=", 2)
{
}

FunctionSymbol* PointerMoveAssignmentOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    BoundExpressionNode* arg = args[0].get();
    TypeSymbol* type = arg->GetType();
    if (type->PointerCount() <= 1) return nullptr;
    TypeSymbol* pointerType = type->RemovePointer(context)->RemoveReference(context);
    if (!TypesEqual(args[1]->GetType(), pointerType->AddRValueRef(context), context) && !args[1]->BindToRvalueRef()) return nullptr;
    auto it = functionMap.find(pointerType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerMoveAssignment* function = new PointerMoveAssignment(pointerType, fullSpan, context);
    functionMap[pointerType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerPlusOffset : public FunctionSymbol
{
public:
    PointerPlusOffset(TypeSymbol* pointerType_, TypeSymbol* longLongIntType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    TypeSymbol* pointerType;
    TypeSymbol* longLongIntType;
};

PointerPlusOffset::PointerPlusOffset(TypeSymbol* pointerType_, TypeSymbol* longLongIntType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "operator+"), pointerType(pointerType_), longLongIntType(longLongIntType_)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
    leftParam->SetType(pointerType, context);
    AddSymbol(leftParam, fullSpan, context);
    ParameterSymbol* rightParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
    rightParam->SetType(longLongIntType, context);
    AddSymbol(rightParam, fullSpan, context);
    SetReturnType(pointerType, context);
    SetNoExcept();
}

void PointerPlusOffset::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* left = emitter.Stack().Pop();
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitPtrOffset(left, right));
}

class PointerPlusOffsetOperation : public Operation
{
public:
    PointerPlusOffsetOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerPlusOffsetOperation::PointerPlusOffsetOperation() : Operation("operator+", 2)
{
}

FunctionSymbol* PointerPlusOffsetOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    if (context->GetFlag(ContextFlags::noPtrOps)) return nullptr;
    TypeSymbol* leftType = args[0]->GetType()->PlainType(context);
    if (!leftType->IsPointerType()) return nullptr;
    TypeSymbol* rightType = args[1]->GetType();
    if (!rightType->PlainType(context)->IsIntegralType())
    {
        if (!context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            rightType, context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longLongIntType, context), 
            fullSpan, context))
        {
            return nullptr;
        }
    }
    auto it = functionMap.find(leftType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerPlusOffset* function = new PointerPlusOffset(leftType, 
        context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longLongIntType, context), fullSpan, context);
    functionMap[leftType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class OffsetPlusPointer : public FunctionSymbol
{
public:
    OffsetPlusPointer(TypeSymbol* pointerType_, TypeSymbol* longLongIntType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    TypeSymbol* pointerType;
    TypeSymbol* longLongIntType;
};

OffsetPlusPointer::OffsetPlusPointer(TypeSymbol* pointerType_, TypeSymbol* longLongIntType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "operator+"), pointerType(pointerType_), longLongIntType(longLongIntType_)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
    leftParam->SetType(longLongIntType, context);
    AddSymbol(leftParam, fullSpan, context);
    ParameterSymbol* rightParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
    rightParam->SetType(pointerType, context);
    AddSymbol(rightParam, fullSpan, context);
    SetReturnType(pointerType, context);
    SetNoExcept();
}

void OffsetPlusPointer::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* left = emitter.Stack().Pop();
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitPtrOffset(right, left));
}

class OffsetPlusPointerOperation : public Operation
{
public:
    OffsetPlusPointerOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

OffsetPlusPointerOperation::OffsetPlusPointerOperation() : Operation("operator+", 2)
{
}

FunctionSymbol* OffsetPlusPointerOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    TypeSymbol* leftType = args[0]->GetType();
    if (!leftType->PlainType(context)->IsIntegralType())
    {
        if (!context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            leftType, context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longLongIntType, context), fullSpan, context))
        {
            return nullptr;
        }
    }
    TypeSymbol* rightType = args[1]->GetType()->PlainType(context);
    if (!rightType->IsPointerType()) return nullptr;
    auto it = functionMap.find(leftType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    OffsetPlusPointer* function = new OffsetPlusPointer(rightType, 
        context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longLongIntType, context), fullSpan, context);
    functionMap[leftType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerMinusOffset : public FunctionSymbol
{
public:
    PointerMinusOffset(TypeSymbol* pointerType_, TypeSymbol* longLongIntType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    TypeSymbol* pointerType;
    TypeSymbol* longLongIntType;
};

PointerMinusOffset::PointerMinusOffset(TypeSymbol* pointerType_, TypeSymbol* longLongIntType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "operator-"), pointerType(pointerType_), longLongIntType(longLongIntType_)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
    leftParam->SetType(pointerType, context);
    AddSymbol(leftParam, fullSpan, context);
    ParameterSymbol* rightParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
    rightParam->SetType(longLongIntType, context);
    AddSymbol(rightParam, fullSpan, context);
    SetReturnType(pointerType, context);
    SetNoExcept();
}

void PointerMinusOffset::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* left = emitter.Stack().Pop();
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* right = emitter.Stack().Pop();
    otava::intermediate::Value* offset = emitter.EmitNeg(right);
    emitter.Stack().Push(emitter.EmitPtrOffset(left, offset));
}

class PointerMinusOffsetOperation : public Operation
{
public:
    PointerMinusOffsetOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerMinusOffsetOperation::PointerMinusOffsetOperation() : Operation("operator-", 2)
{
}

FunctionSymbol* PointerMinusOffsetOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    if (context->GetFlag(ContextFlags::noPtrOps)) return nullptr;
    TypeSymbol* leftType = args[0]->GetType()->PlainType(context);
    if (!leftType->IsPointerType()) return nullptr;
    TypeSymbol* rightType = args[1]->GetType();
    if (!rightType->PlainType(context)->IsIntegralType())
    {
        if (!context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            rightType, context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longLongIntType, context), 
            fullSpan, context))
        {
            return nullptr;
        }
    }
    auto it = functionMap.find(leftType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerMinusOffset* function = new PointerMinusOffset(leftType,
        context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longLongIntType, context), fullSpan, context);
    functionMap[leftType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerMinusPointer : public FunctionSymbol
{
public:
    PointerMinusPointer(TypeSymbol* pointerType_, TypeSymbol* longLongIntType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    TypeSymbol* pointerType;
    TypeSymbol* longLongIntType;
};

PointerMinusPointer::PointerMinusPointer(TypeSymbol* pointerType_, TypeSymbol* longLongIntType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "operator-"), pointerType(pointerType_), longLongIntType(longLongIntType_)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
    leftParam->SetType(pointerType, context);
    AddSymbol(leftParam, fullSpan, context);
    ParameterSymbol* rightParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
    rightParam->SetType(pointerType, context);
    AddSymbol(rightParam, fullSpan, context);
    SetReturnType(longLongIntType, context);
    SetNoExcept();
}

void PointerMinusPointer::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* left = emitter.Stack().Pop();
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitPtrDiff(left, right));
}

class PointerMinusPointerOperation : public Operation
{
public:
    PointerMinusPointerOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerMinusPointerOperation::PointerMinusPointerOperation() : Operation("operator-", 2)
{
}

FunctionSymbol* PointerMinusPointerOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    TypeSymbol* leftType = args[0]->GetType()->PlainType(context);
    if (!leftType->IsPointerType()) return nullptr;
    TypeSymbol* rightType = args[1]->GetType()->PlainType(context);
    if (!rightType->IsPointerType()) return nullptr;
    auto it = functionMap.find(leftType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerMinusPointer* function = new PointerMinusPointer(leftType,
        context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longLongIntType, context), fullSpan, context);
    functionMap[leftType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerEqual : public FunctionSymbol
{
public:
    PointerEqual(TypeSymbol* pointerType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    TypeSymbol* pointerType;
};

PointerEqual::PointerEqual(TypeSymbol* pointerType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "operator=="), pointerType(pointerType_)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
    leftParam->SetType(pointerType, context);
    AddSymbol(leftParam, fullSpan, context);
    ParameterSymbol* rightParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
    rightParam->SetType(pointerType, context);
    AddSymbol(rightParam, fullSpan, context);
    SetReturnType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context), context);
    SetNoExcept();
}

void PointerEqual::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* left = emitter.Stack().Pop();
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitEqual(left, right));
}

class PointerEqualOperation : public Operation
{
public:
    PointerEqualOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerEqualOperation::PointerEqualOperation() : Operation("operator==", 2)
{
}

FunctionSymbol* PointerEqualOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    TypeSymbol* leftType = args[0]->GetType()->PlainType(context);
    if (!leftType->IsPointerType() && !leftType->IsNullPtrType()) return nullptr;
    TypeSymbol* rightType = args[1]->GetType()->PlainType(context);
    if (!rightType->IsPointerType() && !rightType->IsNullPtrType()) return nullptr;
    auto it = functionMap.find(leftType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerEqual* function = new PointerEqual(leftType, fullSpan, context);
    functionMap[leftType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerLess : public FunctionSymbol
{
public:
    PointerLess(TypeSymbol* pointerType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    TypeSymbol* pointerType;
};

PointerLess::PointerLess(TypeSymbol* pointerType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "operator<"), pointerType(pointerType_)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
    leftParam->SetType(pointerType, context);
    AddSymbol(leftParam, fullSpan, context);
    ParameterSymbol* rightParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
    rightParam->SetType(pointerType, context);
    AddSymbol(rightParam, fullSpan, context);
    SetReturnType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context), context);
    SetNoExcept();
}

void PointerLess::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* left = emitter.Stack().Pop();
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitLess(left, right));
}

class PointerLessOperation : public Operation
{
public:
    PointerLessOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerLessOperation::PointerLessOperation() : Operation("operator<", 2)
{
}

FunctionSymbol* PointerLessOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    TypeSymbol* leftType = args[0]->GetType()->PlainType(context);
    if (!leftType->IsPointerType()) return nullptr;
    TypeSymbol* rightType = args[1]->GetType()->PlainType(context);
    if (!rightType->IsPointerType()) return nullptr;
    auto it = functionMap.find(leftType);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerLess* function = new PointerLess(leftType, fullSpan, context);
    functionMap[leftType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class PointerArrow : public FunctionSymbol
{
public:
    PointerArrow(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
private:
    TypeSymbol* type;
};

PointerArrow::PointerArrow(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "operator->"), type(type_)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* operandParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "operand");
    operandParam->SetType(type->AddPointer(context), context);
    AddSymbol(operandParam, fullSpan, context);
    SetReturnType(type, context);
    SetNoExcept();
}

void PointerArrow::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
}

class PointerArrowOperation : public Operation
{
public:
    PointerArrowOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

PointerArrowOperation::PointerArrowOperation() : Operation("operator->", 1)
{
}

FunctionSymbol* PointerArrowOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    TypeSymbol* operandType = args[0]->GetType();
    if (operandType->PointerCount() <= 1) return nullptr;
    TypeSymbol* type = operandType->RemovePointer(context);
    auto it = functionMap.find(type);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    PointerArrow* function = new PointerArrow(type, fullSpan, context);
    functionMap[type] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

class CopyRef : public FunctionSymbol
{
public:
    CopyRef(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
private:
    TypeSymbol* type;
};

CopyRef::CopyRef(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@constructor"), type(type_)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, fullSpan, context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(type, context);
    AddSymbol(thatParam, fullSpan, context);
    SetNoExcept();
}

void CopyRef::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    args[0]->Store(emitter, OperationFlags::none, fullSpan, context);
}

class CopyRefOperation : public Operation
{
public:
    CopyRefOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

CopyRefOperation::CopyRefOperation() : Operation("@constructor", 2)
{
}

FunctionSymbol* CopyRefOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    TypeSymbol* arg0Type = args[0]->GetType();
    if (arg0Type->PointerCount() < 1) return nullptr;
    TypeSymbol* type = arg0Type->RemovePointer(context);
    if (!type->IsReferenceType()) return nullptr;
    TypeSymbol* argType = args[1]->GetType();
    if (!argType->IsReferenceType()) return nullptr;
    auto it = functionMap.find(type);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    CopyRef* copyRef = new CopyRef(type, fullSpan, context);
    functionMap[type] = copyRef;
    functions.push_back(std::unique_ptr<FunctionSymbol>(copyRef));
    return copyRef;
}

class ClassDefaultCtor : public FunctionDefinitionSymbol
{
public:
    ClassDefaultCtor(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    inline ClassTypeSymbol* ClassType() const noexcept { return classType; }
private:
    ClassTypeSymbol* classType;
    std::string irName;
};

ClassDefaultCtor::ClassDefaultCtor(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionDefinitionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionDefinitionSymbol), "@class_default_ctor"), classType(classType_)
{
    SetFunctionKind(FunctionKind::constructor);
    SetGenerated();
    SetParent(classType);
    SetAccess(Access::public_);
    SetReturnType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::voidType, context), context);
    std::string digestSource = classType->FullName(context);
    digestSource.append(1, '.').append(classType->IrName(context));
    digestSource.append(context->GetBoundCompileUnit()->Id());
    irName = "default_ctor_" + classType->GroupName(context) + "_" + util::GetSha1MessageDigest(digestSource);
    SetFixedIrName(irName);
}

class ClassDefaultCtorOperation : public Operation
{
public:
    ClassDefaultCtorOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    void GenerateImplementation(ClassDefaultCtor* classDefaultCtor, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

ClassDefaultCtorOperation::ClassDefaultCtorOperation() : Operation("@constructor", 1)
{
}

FunctionSymbol* ClassDefaultCtorOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    TypeSymbol* type = args[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(context)->PlainType(context)->IsClassTypeSymbol()) return nullptr;
    ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(type->GetBaseType(context));
    if (classType->IsClassTemplateSpecializationSymbol() && context->GetFlag(ContextFlags::ignoreClassTemplateSpecializations)) return nullptr;
    FunctionSymbol* defaultCtor = classType->GetFunctionByIndex(defaultCtorIndex);
    if (defaultCtor)
    {
        return defaultCtor;
    }
    if (classType->HasUserDefinedConstructor()) return nullptr;
    TypeSymbol* ct = classType;
    auto it = functionMap.find(ct);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    ClassDefaultCtor* function = new ClassDefaultCtor(classType, fullSpan, context);
    function->SetParent(classType);
    GenerateImplementation(function, fullSpan, context);
    functionMap[classType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

void ClassDefaultCtorOperation::GenerateImplementation(ClassDefaultCtor* classDefaultCtor, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ClassTypeSymbol* classType = classDefaultCtor->ClassType();
    if (!classType->ObjectLayoutComputed())
    {
        classType->MakeObjectLayout(fullSpan, context);
    }
    std::unique_ptr<BoundFunctionNode> boundFunction(new BoundFunctionNode(classDefaultCtor, fullSpan));
    BoundCompoundStatementNode* body = new BoundCompoundStatementNode(fullSpan);
    boundFunction->SetBody(body);
    context->PushBoundFunction(boundFunction.release());
    bool setNoExcept = true;
    Cardinality nb = Cardinality(classType->BaseClasses(context).size());
    for (Index i = Index(0); i < Index(nb); ++i)
    {
        TypeSymbol* baseClass = classType->BaseClasses(context)[ToUnderlying(i)];
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        BoundExpressionNode* thisPtr = new BoundParameterNode(classDefaultCtor->ThisParam(context), fullSpan,
            classDefaultCtor->ThisParam(context)->GetReferredType(context));
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            baseClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
        if (conversion)
        {
            std::vector<TypeSymbol*> templateArgs;
            TypeSymbol* type = conversion->ReturnType(context);
            args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan, type)));
            std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverloadThrow(
                context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs, args, fullSpan, context);
            if (boundFunctionCall->MayThrow())
            {
                setNoExcept = false;
            }
            BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
            expressionStatement->SetExpr(boundFunctionCall.release(), fullSpan, context);
            context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
        }
        else
        {
            ThrowException("base class conversion not found", fullSpan, context);
        }
    }
    if (classType->IsPolymorphic(context))
    {
        std::vector<ClassTypeSymbol*> vptrHolderClasses = classType->VPtrHolderClasses(context);
        if (vptrHolderClasses.empty())
        {
            ThrowException("no vptr holder classes for the class '" + classType->FullName(context) + "'", fullSpan, context);
        }
        for (ClassTypeSymbol* vptrHolderClass : vptrHolderClasses)
        {
            if (vptrHolderClass != classType)
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(classDefaultCtor->ThisParam(context), fullSpan,
                    classDefaultCtor->ThisParam(context)->GetReferredType(context));
                FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                    vptrHolderClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
                if (conversion)
                {
                    TypeSymbol* type = conversion->ReturnType(context);
                    BoundExpressionNode* thisPtrConverted = new BoundConversionNode(thisPtr, conversion, fullSpan, type);
                    BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtrConverted, classType, vptrHolderClass, fullSpan);
                    context->GetBoundFunction()->Body()->AddStatement(setVPtrStatement);
                }
                else
                {
                    ThrowException("vptr holder class conversion not found", fullSpan, context);
                }
            }
            else
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(classDefaultCtor->ThisParam(context), fullSpan,
                    classDefaultCtor->ThisParam(context)->GetReferredType(context));
                BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtr, classType, classType, fullSpan);
                context->GetBoundFunction()->Body()->AddStatement(setVPtrStatement);
            }
        }
    }
    Cardinality n = Cardinality(classType->MemberVariables(context).size());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        VariableSymbol* memberVariableSymbol = classType->MemberVariables(context)[ToUnderlying(i)];
        BoundVariableNode* boundMemberVariable = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
        boundMemberVariable->SetThisPtr(new BoundParameterNode(classDefaultCtor->ThisParam(context), fullSpan,
            classDefaultCtor->ThisParam(context)->GetReferredType(context)));
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        std::vector<TypeSymbol*> templateArgs;
        args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundMemberVariable, fullSpan,
            boundMemberVariable->GetType()->AddPointer(context))));
        std::unique_ptr<BoundFunctionCallNode> memberConstructorCall = ResolveOverloadThrow(classType->GetScope(), "@constructor", templateArgs,
            args, fullSpan, context);
        if (memberConstructorCall->MayThrow())
        {
            setNoExcept = false;
        }
        BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
        expressionStatement->SetExpr(memberConstructorCall.release(), fullSpan, context);
        context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
    }
    if (!context->GetFlag(ContextFlags::leaveBoundFunction))
    {
        context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(context->ReleaseBoundFunction()), context);
        context->PopBoundFunction();
    }
    if (classType->TotalMemberCount() <= inlineClassOperationsThreshold)
    {
        classDefaultCtor->SetInline();
    }
    if (setNoExcept)
    {
        classDefaultCtor->SetNoExcept();
    }
}

FunctionDefinitionSymbol* GenerateClassDefaultCtor(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::unique_ptr<ClassDefaultCtor> defaultCtor(new ClassDefaultCtor(classType, fullSpan, context));
    ClassDefaultCtorOperation operation;
    context->PushSetFlag(ContextFlags::leaveBoundFunction);
    operation.GenerateImplementation(defaultCtor.get(), fullSpan, context);
    context->PopFlags();
    return defaultCtor.release();
}

class ClassCopyCtor : public FunctionDefinitionSymbol
{
public:
    ClassCopyCtor(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    ~ClassCopyCtor();
    inline ClassTypeSymbol* ClassType() const noexcept { return classType; }
private:
    ClassTypeSymbol* classType;
    std::string irName;
};

ClassCopyCtor::ClassCopyCtor(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionDefinitionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionDefinitionSymbol), "@class_copy_ctor"), classType(classType_)
{
    SetFunctionKind(FunctionKind::constructor);
    SetGenerated();
    SetAccess(Access::public_);
    SetParent(classType);
    ParameterSymbol* thisParam = ThisParam(context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(classType->AddConst(context)->AddLValueRef(context), context);
    AddSymbol(thatParam, fullSpan, context);
    SetReturnType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::voidType, context), context);
    std::string digestSource = classType->FullName(context);
    digestSource.append(1, '.').append(classType->IrName(context));
    digestSource.append(context->GetBoundCompileUnit()->Id());
    irName = "copy_ctor_" + classType->GroupName(context) + "_" + util::GetSha1MessageDigest(digestSource);
    SetFixedIrName(irName);
}

ClassCopyCtor::~ClassCopyCtor()
{
    if (classType && classType->CopyCtor() == this)
    {
        classType->ResetCopyCtor();
    }
}

class ClassCopyCtorOperation : public Operation
{
public:
    ClassCopyCtorOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    void GenerateImplementation(ClassCopyCtor* classCopyCtor, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

ClassCopyCtorOperation::ClassCopyCtorOperation() : Operation("@constructor", 2)
{
}

FunctionSymbol* ClassCopyCtorOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    TypeSymbol* type = args[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(context)->RemoveConst(context)->IsClassTypeSymbol()) return nullptr;
    if (!args[1]->GetType()->PlainType(context)->IsClassTypeSymbol()) return nullptr;
    ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(type->GetBaseType(context));
    if (classType->IsClassTemplateSpecializationSymbol() && context->GetFlag(ContextFlags::ignoreClassTemplateSpecializations)) return nullptr;
    if (TypesEqual(args[1]->GetType(), classType->AddRValueRef(context), context) || args[1]->BindToRvalueRef()) return nullptr;
    int distance = 0;
    if (!TypesEqual(args[1]->GetType()->GetBaseType(context), classType, context) &&
        !args[1]->GetType()->GetBaseType(context)->HasBaseClass(classType, distance, context)) return nullptr;
    FunctionSymbol* copyCtor = classType->GetFunctionByIndex(copyCtorIndex);
    if (copyCtor)
    {
        return copyCtor;
    }
    TypeSymbol* ct = classType;
    auto it = functionMap.find(ct);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    ClassCopyCtor* function = new ClassCopyCtor(classType, fullSpan, context);
    function->SetParent(classType);
    GenerateImplementation(function, fullSpan, context);
    functionMap[classType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

void ClassCopyCtorOperation::GenerateImplementation(ClassCopyCtor* classCopyCtor, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ClassTypeSymbol* classType = classCopyCtor->ClassType();
    if (!classType->ObjectLayoutComputed())
    {
        classType->MakeObjectLayout(fullSpan, context);
    }
    std::unique_ptr<BoundFunctionNode> boundFunction(new BoundFunctionNode(classCopyCtor, fullSpan));
    BoundCompoundStatementNode* body = new BoundCompoundStatementNode(fullSpan);
    boundFunction->SetBody(body);
    context->PushBoundFunction(boundFunction.release());
    bool setNoExcept = true;
    Cardinality nb = Cardinality(classType->BaseClasses(context).size());
    for (Index i = Index(0); i < Index(nb); ++i)
    {
        TypeSymbol* baseClass = classType->BaseClasses(context)[ToUnderlying(i)];
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        BoundExpressionNode* thisPtr = new BoundParameterNode(classCopyCtor->ThisParam(context), fullSpan, classCopyCtor->ThisParam(context)->GetReferredType(context));
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            baseClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
        if (conversion)
        {
            TypeSymbol* type = conversion->ReturnType(context);
            args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan, type)));
            ParameterSymbol* thatParam = classCopyCtor->MemFnParameters(context)[1];
            FunctionSymbol* thatConversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                baseClass->AddConst(context)->AddLValueRef(context), thatParam->GetType(context), fullSpan, context);
            if (thatConversion)
            {
                BoundExpressionNode* thatPtr = new BoundParameterNode(thatParam, fullSpan, thatParam->GetType(context));
                TypeSymbol* type = thatConversion->ReturnType(context);
                args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thatPtr, thatConversion, fullSpan, type)));
                std::vector<TypeSymbol*> templateArgs;
                std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverloadThrow(
                    context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs, args, fullSpan, context);
                if (boundFunctionCall->MayThrow())
                {
                    setNoExcept = false;
                }
                BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
                expressionStatement->SetExpr(boundFunctionCall.release(), fullSpan, context);
                context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
            }
            else
            {
                ThrowException("base class conversion not found", fullSpan, context);
            }
        }
        else
        {
            ThrowException("base class conversion not found", fullSpan, context);
        }
    }
    if (classType->IsPolymorphic(context))
    {
        std::vector<ClassTypeSymbol*> vptrHolderClasses = classType->VPtrHolderClasses(context);
        if (vptrHolderClasses.empty())
        {
            ThrowException("no vptr holder classes for the class '" + classType->FullName(context) + "'", fullSpan, context);
        }
        for (ClassTypeSymbol* vptrHolderClass : vptrHolderClasses)
        {
            if (vptrHolderClass != classType)
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(classCopyCtor->ThisParam(context), fullSpan,
                    classCopyCtor->ThisParam(context)->GetReferredType(context));
                FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                    vptrHolderClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
                if (conversion)
                {
                    TypeSymbol* type = conversion->ReturnType(context);
                    BoundExpressionNode* thisPtrConverted = new BoundConversionNode(thisPtr, conversion, fullSpan, type);
                    BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtrConverted, classType, vptrHolderClass, fullSpan);
                    context->GetBoundFunction()->Body()->AddStatement(setVPtrStatement);
                }
                else
                {
                    ThrowException("vptr holder class conversion not found", fullSpan, context);
                }
            }
            else
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(classCopyCtor->ThisParam(context), fullSpan,
                    classCopyCtor->ThisParam(context)->GetReferredType(context));
                BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtr, classType, classType, fullSpan);
                context->GetBoundFunction()->Body()->AddStatement(setVPtrStatement);
            }
        }
    }
    Cardinality n = Cardinality(classType->MemberVariables(context).size());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        VariableSymbol* memberVariableSymbol = classType->MemberVariables(context)[ToUnderlying(i)];
        BoundVariableNode* boundMemberVariable = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
        boundMemberVariable->SetThisPtr(new BoundParameterNode(classCopyCtor->ThisParam(context), fullSpan, classCopyCtor->ThisParam(context)->GetReferredType(context)));
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundMemberVariable, fullSpan,
            boundMemberVariable->GetType()->AddPointer(context))));
        ParameterSymbol* thatParam = classCopyCtor->MemFnParameters(context)[1];
        BoundVariableNode* thatBoundMemberVariable = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
        thatBoundMemberVariable->SetThisPtr(new BoundRefToPtrNode(
            new BoundParameterNode(thatParam, fullSpan, thatParam->GetReferredType(context)), fullSpan,
            thatParam->GetType(context)->RemoveReference(context)->AddPointer(context)));
        args.push_back(std::unique_ptr<BoundExpressionNode>(thatBoundMemberVariable));
        std::vector<TypeSymbol*> templateArgs;
        std::unique_ptr<BoundFunctionCallNode> memberConstructorCall = ResolveOverloadThrow(classType->GetScope(), "@constructor", templateArgs, args,
            fullSpan, context);
        if (memberConstructorCall->MayThrow())
        {
            setNoExcept = false;
        }
        BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
        expressionStatement->SetExpr(memberConstructorCall.release(), fullSpan, context);
        context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
    }
    if (!context->GetFlag(ContextFlags::leaveBoundFunction))
    {
        context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(context->ReleaseBoundFunction()), context);
        context->PopBoundFunction();
    }
    if (classType->TotalMemberCount() <= inlineClassOperationsThreshold)
    {
        classCopyCtor->SetInline();
    }
    if (setNoExcept)
    {
        classCopyCtor->SetNoExcept();
    }
}

FunctionDefinitionSymbol* GenerateClassCopyCtor(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::unique_ptr<ClassCopyCtor> copyCtor(new ClassCopyCtor(classType, fullSpan, context));
    ClassCopyCtorOperation operation;
    context->PushSetFlag(ContextFlags::leaveBoundFunction);
    operation.GenerateImplementation(copyCtor.get(), fullSpan, context);
    context->PopFlags();
    return copyCtor.release();
}

class ClassMoveCtor : public FunctionDefinitionSymbol
{
public:
    ClassMoveCtor(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    inline ClassTypeSymbol* ClassType() const noexcept { return classType; }
private:
    ClassTypeSymbol* classType;
    std::string irName;
};

ClassMoveCtor::ClassMoveCtor(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionDefinitionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionDefinitionSymbol), "@class_move_ctor"), classType(classType_)
{
    SetFunctionKind(FunctionKind::constructor);
    SetGenerated();
    SetAccess(Access::public_);
    SetParent(classType);
    ParameterSymbol* thisParam = ThisParam(context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(classType->AddRValueRef(context), context);
    AddSymbol(thatParam, fullSpan, context);
    SetReturnType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::voidType, context), context);
    std::string digestSource = classType->FullName(context);
    digestSource.append(1, '.').append(classType->IrName(context));
    digestSource.append(context->GetBoundCompileUnit()->Id());
    irName = "move_ctor_" + classType->GroupName(context) + "_" + util::GetSha1MessageDigest(digestSource);
    SetFixedIrName(irName);
    SetNoExcept();
}

class ClassMoveCtorOperation : public Operation
{
public:
    ClassMoveCtorOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    void GenerateImplementation(ClassMoveCtor* classMoveCtor, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

ClassMoveCtorOperation::ClassMoveCtorOperation() : Operation("@constructor", 2)
{
}

FunctionSymbol* ClassMoveCtorOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    TypeSymbol* type = args[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(context)->RemoveConst(context)->IsClassTypeSymbol()) return nullptr;
    if (!args[1]->GetType()->PlainType(context)->IsClassTypeSymbol()) return nullptr;
    ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(type->GetBaseType(context));
    if (classType->IsClassTemplateSpecializationSymbol() && context->GetFlag(ContextFlags::ignoreClassTemplateSpecializations)) return nullptr;
    if (!TypesEqual(args[1]->GetType(), classType->AddRValueRef(context), context) && !args[1]->BindToRvalueRef()) return nullptr;
    int distance = 0;
    if (!TypesEqual(args[1]->GetType()->GetBaseType(context), classType, context) &&
        !args[1]->GetType()->GetBaseType(context)->HasBaseClass(classType, distance, context)) return nullptr;
    FunctionSymbol* moveCtor = classType->GetFunctionByIndex(moveCtorIndex);
    if (moveCtor)
    {
        return moveCtor;
    }
    TypeSymbol* ct = classType;
    auto it = functionMap.find(ct);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    ClassMoveCtor* function = new ClassMoveCtor(classType, fullSpan, context);
    function->SetParent(classType);
    GenerateImplementation(function, fullSpan, context);
    functionMap[classType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

void ClassMoveCtorOperation::GenerateImplementation(ClassMoveCtor* classMoveCtor, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ClassTypeSymbol* classType = classMoveCtor->ClassType();
    if (!classType->ObjectLayoutComputed())
    {
        classType->MakeObjectLayout(fullSpan, context);
    }
    std::unique_ptr<BoundFunctionNode> boundFunction(new BoundFunctionNode(classMoveCtor, fullSpan));
    BoundCompoundStatementNode* body = new BoundCompoundStatementNode(fullSpan);
    boundFunction->SetBody(body);
    context->PushBoundFunction(boundFunction.release());
    Cardinality nb = Cardinality(classType->BaseClasses(context).size());
    for (Index i = Index(0); i < Index(nb); ++i)
    {
        TypeSymbol* baseClass = classType->BaseClasses(context)[ToUnderlying(i)];
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        BoundExpressionNode* thisPtr = new BoundParameterNode(classMoveCtor->ThisParam(context), fullSpan, classMoveCtor->ThisParam(context)->GetReferredType(context));
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            baseClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
        if (conversion)
        {
            TypeSymbol* type = conversion->ReturnType(context);
            args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan, type)));
            ParameterSymbol* thatParam = classMoveCtor->MemFnParameters(context)[1];
            FunctionSymbol* thatParamConversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                baseClass->AddRValueRef(context), thatParam->GetType(context), fullSpan, context);
            if (thatParamConversion)
            {
                BoundExpressionNode* thatPtr = new BoundParameterNode(thatParam, fullSpan, thatParam->GetType(context));
                TypeSymbol* type = thatParamConversion->ReturnType(context);
                args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thatPtr, thatParamConversion, fullSpan, type)));
                std::vector<TypeSymbol*> templateArgs;
                std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverloadThrow(
                    context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs, args, fullSpan, context);
                BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
                expressionStatement->SetExpr(boundFunctionCall.release(), fullSpan, context);
                context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
            }
            else
            {
                ThrowException("base class conversion not found", fullSpan, context);
            }
        }
        else
        {
            ThrowException("base class conversion not found", fullSpan, context);
        }
    }
    if (classType->IsPolymorphic(context))
    {
        std::vector<ClassTypeSymbol*> vptrHolderClasses = classType->VPtrHolderClasses(context);
        if (vptrHolderClasses.empty())
        {
            ThrowException("no vptr holder classes for the class '" + classType->FullName(context) + "'", fullSpan, context);
        }
        for (ClassTypeSymbol* vptrHolderClass : vptrHolderClasses)
        {
            if (vptrHolderClass != classType)
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(classMoveCtor->ThisParam(context), fullSpan,
                    classMoveCtor->ThisParam(context)->GetReferredType(context));
                FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                    vptrHolderClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
                if (conversion)
                {
                    TypeSymbol* type = conversion->ReturnType(context);
                    BoundExpressionNode* thisPtrConverted = new BoundConversionNode(thisPtr, conversion, fullSpan, type);
                    BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtrConverted, classType, vptrHolderClass, fullSpan);
                    context->GetBoundFunction()->Body()->AddStatement(setVPtrStatement);
                }
                else
                {
                    ThrowException("vptr holder class conversion not found", fullSpan, context);
                }
            }
            else
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(classMoveCtor->ThisParam(context), fullSpan,
                    classMoveCtor->ThisParam(context)->GetReferredType(context));
                BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtr, classType, classType, fullSpan);
                context->GetBoundFunction()->Body()->AddStatement(setVPtrStatement);
            }
        }
    }
    Cardinality n = Cardinality(classType->MemberVariables(context).size());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        VariableSymbol* memberVariableSymbol = classType->MemberVariables(context)[ToUnderlying(i)];
        BoundVariableNode* boundMemberVariable = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
        boundMemberVariable->SetThisPtr(new BoundParameterNode(classMoveCtor->ThisParam(context), fullSpan, classMoveCtor->ThisParam(context)->GetReferredType(context)));
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        std::vector<TypeSymbol*> templateArgs;
        args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundMemberVariable, fullSpan,
            boundMemberVariable->GetType()->AddPointer(context))));
        ParameterSymbol* thatParam = classMoveCtor->MemFnParameters(context)[1];
        BoundVariableNode* thatBoundMemberVariable = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
        thatBoundMemberVariable->SetThisPtr(new BoundRefToPtrNode(
            new BoundParameterNode(thatParam, fullSpan, thatParam->GetReferredType(context)), fullSpan,
            thatParam->GetType(context)->RemoveReference(context)->AddPointer(context)));
        if (thatBoundMemberVariable->GetType()->IsFunctionPtrType(context))
        {
            args.push_back(std::unique_ptr<BoundExpressionNode>(thatBoundMemberVariable));
        }
        else
        {
            std::vector<std::unique_ptr<BoundExpressionNode>> moveArgs;
            moveArgs.push_back(std::unique_ptr<BoundExpressionNode>(thatBoundMemberVariable));
            Scope* stdScope = context->GetSymbolTable()->GetNamespaceScope("std", fullSpan, context);
            std::unique_ptr<BoundFunctionCallNode> moveThat(ResolveOverloadThrow(
                stdScope, "move", templateArgs, moveArgs, fullSpan, context));
            args.push_back(std::unique_ptr<BoundExpressionNode>(moveThat.release()));
        }
        std::unique_ptr<BoundFunctionCallNode> memberConstructorCall = ResolveOverloadThrow(classType->GetScope(), "@constructor",
            templateArgs, args, fullSpan, context);
        BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
        expressionStatement->SetExpr(memberConstructorCall.release(), fullSpan, context);
        context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
    }
    if (!context->GetFlag(ContextFlags::leaveBoundFunction))
    {
        context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(context->ReleaseBoundFunction()), context);
        context->PopBoundFunction();
    }
    if (classType->TotalMemberCount() <= inlineClassOperationsThreshold)
    {
        classMoveCtor->SetInline();
    }
}

FunctionDefinitionSymbol* GenerateClassMoveCtor(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::unique_ptr<ClassMoveCtor> classMoveCtor(new ClassMoveCtor(classType, fullSpan, context));;
    ClassMoveCtorOperation operation;
    context->PushSetFlag(ContextFlags::leaveBoundFunction);
    operation.GenerateImplementation(classMoveCtor.get(), fullSpan, context);
    context->PopFlags();
    return classMoveCtor.release();
}

class ClassCopyAssignment : public FunctionDefinitionSymbol
{
public:
    ClassCopyAssignment(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    ClassTypeSymbol* ClassType() const { return classType; }
private:
    ClassTypeSymbol* classType;
    std::string irName;
};

ClassCopyAssignment::ClassCopyAssignment(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionDefinitionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionDefinitionSymbol), "operator="), classType(classType_)
{
    SetFunctionKind(FunctionKind::special);
    SetGenerated();
    SetAccess(Access::public_);
    SetParent(classType);
    ParameterSymbol* thisParam = ThisParam(context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(classType->AddConst(context)->AddLValueRef(context), context);
    AddSymbol(thatParam, fullSpan, context);
    SetReturnType(classType->AddLValueRef(context), context);
    std::string digestSource = classType->FullName(context);
    digestSource.append(1, '.').append(classType->IrName(context));
    digestSource.append(context->GetBoundCompileUnit()->Id());
    irName = "copy_assignment_" + classType->GroupName(context) + "_" + util::GetSha1MessageDigest(digestSource);
    SetFixedIrName(irName);
}

class ClassCopyAssignmentOperation : public Operation
{
public:
    ClassCopyAssignmentOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    void GenerateImplementation(ClassCopyAssignment* classCopyAssignment, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

ClassCopyAssignmentOperation::ClassCopyAssignmentOperation() : Operation("operator=", 2)
{
}

FunctionSymbol* ClassCopyAssignmentOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    TypeSymbol* type = args[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(context)->PlainType(context)->IsClassTypeSymbol()) return nullptr;
    if (!args[1]->GetType()->PlainType(context)->IsClassTypeSymbol()) return nullptr;
    ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(type->GetBaseType(context));
    if (classType->IsClassTemplateSpecializationSymbol() && context->GetFlag(ContextFlags::ignoreClassTemplateSpecializations)) return nullptr;
    if (TypesEqual(args[1]->GetType(), classType->AddRValueRef(context), context) || args[1]->BindToRvalueRef()) return nullptr;
    FunctionSymbol* copyAssignment = classType->GetFunctionByIndex(copyAssignmentIndex);
    if (copyAssignment)
    {
        return copyAssignment;
    }
    TypeSymbol* ct = classType;
    auto it = functionMap.find(ct);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    ClassCopyAssignment* function = new ClassCopyAssignment(classType, fullSpan, context);
    function->SetParent(classType);
    GenerateImplementation(function, fullSpan, context);
    functionMap[classType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

void ClassCopyAssignmentOperation::GenerateImplementation(ClassCopyAssignment* classCopyAssignment, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ClassTypeSymbol* classType = classCopyAssignment->ClassType();
    std::unique_ptr<BoundFunctionNode> boundFunction(new BoundFunctionNode(classCopyAssignment, fullSpan));
    BoundCompoundStatementNode* body = new BoundCompoundStatementNode(fullSpan);
    boundFunction->SetBody(body);
    context->PushBoundFunction(boundFunction.release());
    bool setNoExcept = true;
    Cardinality nb = Cardinality(classType->BaseClasses(context).size());
    for (Index i = Index(0); i < Index(nb); ++i)
    {
        TypeSymbol* baseClass = classType->BaseClasses(context)[ToUnderlying(i)];
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        BoundExpressionNode* thisPtr = new BoundParameterNode(classCopyAssignment->ThisParam(context), fullSpan,
            classCopyAssignment->ThisParam(context)->GetReferredType(context));
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            baseClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
        if (conversion)
        {
            TypeSymbol* type = conversion->ReturnType(context);
            args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan, type)));
            ParameterSymbol* thatParam = classCopyAssignment->MemFnParameters(context)[1];
            BoundExpressionNode* thatPtr = new BoundParameterNode(thatParam, fullSpan, thatParam->GetType(context));
            FunctionSymbol* thatConversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                baseClass->AddConst(context)->AddLValueRef(context), thatParam->GetType(context), fullSpan, context);
            if (thatConversion)
            {
                TypeSymbol* type = thatConversion->ReturnType(context);
                args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thatPtr, thatConversion, fullSpan, type)));
                std::vector<TypeSymbol*> templateArgs;
                std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverloadThrow(
                    context->GetSymbolTable()->CurrentScope(), "operator=", templateArgs, args, fullSpan, context);
                if (boundFunctionCall->MayThrow())
                {
                    setNoExcept = false;
                }
                BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
                expressionStatement->SetExpr(boundFunctionCall.release(), fullSpan, context);
                context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
            }
            else
            {
                ThrowException("base class conversion not found", fullSpan, context);
            }
        }
        else
        {
            ThrowException("base class conversion not found", fullSpan, context);
        }
    }
    Cardinality n = Cardinality(classType->MemberVariables(context).size());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        VariableSymbol* memberVariableSymbol = classType->MemberVariables(context)[ToUnderlying(i)];
        BoundVariableNode* boundMemberVariable = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
        boundMemberVariable->SetThisPtr(new BoundParameterNode(classCopyAssignment->ThisParam(context), fullSpan,
            classCopyAssignment->ThisParam(context)->GetReferredType(context)));
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundMemberVariable, fullSpan,
            boundMemberVariable->GetType()->AddPointer(context))));
        ParameterSymbol* thatParam = classCopyAssignment->MemFnParameters(context)[1];
        BoundVariableNode* thatBoundMemberVariable = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
        thatBoundMemberVariable->SetThisPtr(new BoundRefToPtrNode(
            new BoundParameterNode(thatParam, fullSpan, thatParam->GetReferredType(context)), fullSpan,
            thatParam->GetType(context)->RemoveReference(context)->AddPointer(context)));
        args.push_back(std::unique_ptr<BoundExpressionNode>(thatBoundMemberVariable));
        std::vector<TypeSymbol*> templateArgs;
        std::unique_ptr<BoundFunctionCallNode> memberAssignmentrCall = ResolveOverloadThrow(classType->GetScope(), "operator=", templateArgs, args, fullSpan, context);
        if (memberAssignmentrCall->MayThrow())
        {
            setNoExcept = false;
        }
        BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
        expressionStatement->SetExpr(memberAssignmentrCall.release(), fullSpan, context);
        context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
    }
    BoundReturnStatementNode* returnStatement = new BoundReturnStatementNode(fullSpan);
    otava::ast::ThisNode* thisNode = new otava::ast::ThisNode(fullSpan.span, fullSpan.fileIndex); 
    otava::ast::UnaryExprNode derefNode(fullSpan.span, fullSpan.fileIndex, new otava::ast::DerefNode(fullSpan.span, fullSpan.fileIndex), thisNode);
    std::unique_ptr<BoundExpressionNode> derefThisExpr = BindExpression(&derefNode, context); 
    returnStatement->SetExpr(derefThisExpr.release(), fullSpan, context);
    context->GetBoundFunction()->Body()->AddStatement(returnStatement);
    if (!context->GetFlag(ContextFlags::leaveBoundFunction))
    {
        context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(context->ReleaseBoundFunction()), context);
        context->PopBoundFunction();
    }
    if (classType->TotalMemberCount() <= inlineClassOperationsThreshold)
    {
        classCopyAssignment->SetInline();
    }
    if (setNoExcept)
    {
        classCopyAssignment->SetNoExcept();
    }
}

FunctionDefinitionSymbol* GenerateClassCopyAssignment(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::unique_ptr<ClassCopyAssignment> copyAssignment(new ClassCopyAssignment(classType, fullSpan, context));
    ClassCopyAssignmentOperation operation;
    context->PushSetFlag(ContextFlags::leaveBoundFunction);
    operation.GenerateImplementation(copyAssignment.get(), fullSpan, context);
    context->PopFlags();
    return copyAssignment.release();
}

class ClassMoveAssignment : public FunctionDefinitionSymbol
{
public:
    ClassMoveAssignment(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    inline ClassTypeSymbol* ClassType() const noexcept { return classType; }
private:
    ClassTypeSymbol* classType;
    std::string irName;
};

ClassMoveAssignment::ClassMoveAssignment(ClassTypeSymbol* classType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionDefinitionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionDefinitionSymbol), "operator="), classType(classType_)
{
    SetFunctionKind(FunctionKind::constructor);
    SetGenerated();
    SetAccess(Access::public_);
    SetParent(classType);
    ParameterSymbol* thisParam = ThisParam(context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(classType->AddRValueRef(context), context);
    AddSymbol(thatParam, fullSpan, context);
    SetReturnType(classType->AddLValueRef(context), context);
    std::string digestSource = classType->FullName(context);
    digestSource.append(1, '.').append(classType->IrName(context));
    digestSource.append(context->GetBoundCompileUnit()->Id());
    irName = "move_assignment_" + classType->GroupName(context) + "_" + util::GetSha1MessageDigest(digestSource);
    SetFixedIrName(irName);
    SetNoExcept();
}

class ClassMoveAssignmentOperation : public Operation
{
public:
    ClassMoveAssignmentOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    void GenerateImplementation(ClassMoveAssignment* classMoveAssignment, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
private:
    std::map<TypeSymbol*, FunctionSymbol*> functionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

ClassMoveAssignmentOperation::ClassMoveAssignmentOperation() : Operation("operator=", 2)
{
}

FunctionSymbol* ClassMoveAssignmentOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    TypeSymbol* type = args[0]->GetType();
    if (type->PointerCount() != 1 || !type->RemovePointer(context)->PlainType(context)->IsClassTypeSymbol()) return nullptr;
    if (!args[1]->GetType()->PlainType(context)->IsClassTypeSymbol()) return nullptr;
    ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(type->GetBaseType(context));
    if (classType->IsClassTemplateSpecializationSymbol() && context->GetFlag(ContextFlags::ignoreClassTemplateSpecializations)) return nullptr;
    if (!TypesEqual(args[1]->GetType(), classType->AddRValueRef(context), context) && !args[1]->BindToRvalueRef()) return nullptr;
    FunctionSymbol* moveAssignment = classType->GetFunctionByIndex(moveAssignmentIndex);
    if (moveAssignment)
    {
        return moveAssignment;
    }
    TypeSymbol* ct = classType;
    auto it = functionMap.find(ct);
    if (it != functionMap.cend())
    {
        FunctionSymbol* function = it->second;
        return function;
    }
    ClassMoveAssignment* function = new ClassMoveAssignment(classType, fullSpan, context);
    function->SetParent(classType);
    GenerateImplementation(function, fullSpan, context);
    functionMap[classType] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
    return function;
}

void ClassMoveAssignmentOperation::GenerateImplementation(ClassMoveAssignment* classMoveAssignment, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    ClassTypeSymbol* classType = classMoveAssignment->ClassType();
    std::unique_ptr<BoundFunctionNode> boundFunction(new BoundFunctionNode(classMoveAssignment, fullSpan));
    BoundCompoundStatementNode* body = new BoundCompoundStatementNode(fullSpan);
    boundFunction->SetBody(body);
    context->PushBoundFunction(boundFunction.release());
    Cardinality nb = Cardinality(classType->BaseClasses(context).size());
    for (Index i = Index(0); i < Index(nb); ++i)
    {
        TypeSymbol* baseClass = classType->BaseClasses(context)[ToUnderlying(i)];
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        BoundExpressionNode* thisPtr = new BoundParameterNode(classMoveAssignment->ThisParam(context), fullSpan,
            classMoveAssignment->ThisParam(context)->GetReferredType(context));
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            baseClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
        if (conversion)
        {
            TypeSymbol* type = conversion->ReturnType(context);
            args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan, type)));
            ParameterSymbol* thatParam = classMoveAssignment->MemFnParameters(context)[1];
            BoundExpressionNode* thatPtr = new BoundParameterNode(thatParam, fullSpan, thatParam->GetType(context));
            FunctionSymbol* thatConversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                baseClass->AddRValueRef(context), thatParam->GetType(context), fullSpan, context);
            std::vector<std::unique_ptr<BoundExpressionNode>> moveArgs;
            TypeSymbol* thatType = thatConversion->ReturnType(context);
            moveArgs.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thatPtr, thatConversion, fullSpan, thatType)));
            Scope* stdScope = context->GetSymbolTable()->GetNamespaceScope("std", fullSpan, context);
            std::vector<TypeSymbol*> templateArgs;
            std::unique_ptr<BoundFunctionCallNode> moveThat(ResolveOverloadThrow(
                stdScope, "move", templateArgs, moveArgs, fullSpan, context));
            args.push_back(std::unique_ptr<BoundExpressionNode>(moveThat.release()));
            std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverloadThrow(
                context->GetSymbolTable()->CurrentScope(), "operator=", templateArgs, args, fullSpan, context);
            BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
            expressionStatement->SetExpr(boundFunctionCall.release(), fullSpan, context);
            context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
        }
        else
        {
            ThrowException("base class conversion not found", fullSpan, context);
        }
    }
    Cardinality n = Cardinality(classType->MemberVariables(context).size());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        VariableSymbol* memberVariableSymbol = classType->MemberVariables(context)[ToUnderlying(i)];
        BoundVariableNode* boundMemberVariable = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
        boundMemberVariable->SetThisPtr(new BoundParameterNode(classMoveAssignment->ThisParam(context), fullSpan,
            classMoveAssignment->ThisParam(context)->GetReferredType(context)));
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        args.push_back(std::unique_ptr<BoundExpressionNode>(boundMemberVariable));
        ParameterSymbol* thatParam = classMoveAssignment->MemFnParameters(context)[1];
        BoundVariableNode* thatBoundMemberVariable = new BoundVariableNode(memberVariableSymbol, fullSpan, memberVariableSymbol->GetReferredType(context));
        thatBoundMemberVariable->SetThisPtr(new BoundRefToPtrNode(
            new BoundParameterNode(thatParam, fullSpan, thatParam->GetReferredType(context)), fullSpan,
            thatParam->GetType(context)->RemoveReference(context)->AddPointer(context)));
        args.push_back(std::unique_ptr<BoundExpressionNode>(thatBoundMemberVariable));
        Scope* stdScope = context->GetSymbolTable()->GetNamespaceScope("std", fullSpan, context);
        std::vector<TypeSymbol*> templateArgs;
        std::unique_ptr<BoundFunctionCallNode> memberConstructorCall = ResolveOverloadThrow(stdScope, "swap", templateArgs, args, fullSpan, context);
        BoundExpressionStatementNode* expressionStatement = new BoundExpressionStatementNode(fullSpan);
        expressionStatement->SetExpr(memberConstructorCall.release(), fullSpan, context);
        context->GetBoundFunction()->Body()->AddStatement(expressionStatement);
    }
    BoundReturnStatementNode* returnStatement = new BoundReturnStatementNode(fullSpan);
    otava::ast::ThisNode* thisNode = new otava::ast::ThisNode(fullSpan.span, fullSpan.fileIndex);
    otava::ast::UnaryExprNode derefNode(fullSpan.span, fullSpan.fileIndex, new otava::ast::DerefNode(fullSpan.span, fullSpan.fileIndex), thisNode);
    std::unique_ptr<BoundExpressionNode> derefThisExpr = BindExpression(&derefNode, context);
    returnStatement->SetExpr(derefThisExpr.release(), fullSpan, context);
    context->GetBoundFunction()->Body()->AddStatement(returnStatement);
    if (!context->GetFlag(ContextFlags::leaveBoundFunction))
    {
        context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(context->ReleaseBoundFunction()), context);
        context->PopBoundFunction();
    }
    if (classType->TotalMemberCount() <= inlineClassOperationsThreshold)
    {
        classMoveAssignment->SetInline();
    }
}

FunctionDefinitionSymbol* GenerateClassMoveAssignment(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::unique_ptr<ClassMoveAssignment> moveAssignment(new ClassMoveAssignment(classType, fullSpan, context));
    ClassMoveAssignmentOperation operation;
    context->PushSetFlag(ContextFlags::leaveBoundFunction);
    operation.GenerateImplementation(moveAssignment.get(), fullSpan, context);
    context->PopFlags();
    return moveAssignment.release();
}

class FunctionPtrApply : public FunctionSymbol
{
public:
    FunctionPtrApply(FunctionTypeSymbol* type_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    FunctionTypeSymbol* functionType;
};

FunctionPtrApply::FunctionPtrApply(FunctionTypeSymbol* functionType_, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) :
    FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionDefinitionSymbol), "operator()"), functionType(functionType_)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* parameter = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "fn");
    parameter->SetType(functionType->AddPointer(context), context);
    AddSymbol(parameter, fullSpan, context);
    for (TypeSymbol* parameterType : functionType->ParameterTypes(context))
    {
        ParameterSymbol* parameter = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "@param");
        parameter->SetType(parameterType, context);
        AddSymbol(parameter, fullSpan, context);
    }
    SetReturnType(functionType->ReturnType(context), context);
}

void FunctionPtrApply::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* callee = emitter.Stack().Pop();
    int na = args.size() - 1;
    for (int i = 0; i < na; ++i)
    {
        args[i + 1]->Load(emitter, OperationFlags::none, fullSpan, context);
    }
    std::vector<otava::intermediate::Value*> arguments;
    arguments.resize(na);
    for (int i = 0; i < na; ++i)
    {
        otava::intermediate::Value* arg = emitter.Stack().Pop();
        arguments[na - i - 1] = arg;
    }
    emitter.EmitCall(callee, arguments);
}

class FunctionPtrApplyOperation : public Operation
{
public:
    FunctionPtrApplyOperation();
    FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

FunctionPtrApplyOperation::FunctionPtrApplyOperation() : Operation("operator()", -1)
{
}

FunctionSymbol* FunctionPtrApplyOperation::Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    if (args.size() < 1) return nullptr;
    TypeSymbol* type = args[0]->GetType();
    if (type->PointerCount() != 1) return nullptr;
    TypeSymbol* pointeeType = type->RemovePointer(context);
    if (!pointeeType->IsFunctionTypeSymbol()) return nullptr;
    FunctionTypeSymbol* functionType = static_cast<FunctionTypeSymbol*>(pointeeType);
    FunctionPtrApply* apply = new FunctionPtrApply(functionType, fullSpan, context);
    functions.push_back(std::unique_ptr<FunctionSymbol>(apply));
    return apply;
}

Operation::Operation(const std::string& groupName_, int arity_) : groupName(groupName_), arity(arity_)
{
}

Operation::~Operation()
{
}

OperationGroup::OperationGroup(const std::string& name_) : name(name_)
{
}

void OperationGroup::AddOperation(Operation* operation)
{
    if (operation->Arity() == -1)
    {
        anyArityOperations.push_back(operation);
    }
    else
    {
        arityOperationsMap[operation->Arity()].push_back(operation);
    }
}

FunctionSymbol* OperationGroup::GetOperation(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    auto it = arityOperationsMap.find(args.size());
    if (it != arityOperationsMap.cend())
    {
        for (Operation* operation : it->second)
        {
            FunctionSymbol* op = operation->Get(args, fullSpan, context);
            if (op)
            {
                return op;
            }
        }
    }
    for (Operation* operation : anyArityOperations)
    {
        FunctionSymbol* op = operation->Get(args, fullSpan, context);
        if (op)
        {
            return op;
        }
    }
    return nullptr;
}

OperationRepository::OperationRepository()
{
    AddOperation(new PointerDefaultCtorOperation());
    AddOperation(new PointerCopyCtorOperation());
    AddOperation(new PointerMoveCtorOperation());
    AddOperation(new PointerCopyAssignmentOperation());
    AddOperation(new PointerMoveAssignmentOperation());
    AddOperation(new PointerPlusOffsetOperation());
    AddOperation(new OffsetPlusPointerOperation());
    AddOperation(new PointerMinusOffsetOperation());
    AddOperation(new PointerMinusPointerOperation());
    AddOperation(new PointerEqualOperation());
    AddOperation(new PointerLessOperation());
    AddOperation(new PointerArrowOperation());
    AddOperation(new ClassDefaultCtorOperation());
    AddOperation(new ClassCopyCtorOperation());
    AddOperation(new ClassMoveCtorOperation());
    AddOperation(new ClassCopyAssignmentOperation());
    AddOperation(new ClassMoveAssignmentOperation());
    AddOperation(new CopyRefOperation());
    AddOperation(new FunctionPtrApplyOperation());
}

OperationGroup* OperationRepository::GetOrInsertOperationGroup(const std::string& operationGroupName)
{
    auto it = groupMap.find(operationGroupName);
    if (it != groupMap.cend())
    {
        return it->second;
    }
    OperationGroup* group = new OperationGroup(operationGroupName);
    groupMap[operationGroupName] = group;
    groups.push_back(std::unique_ptr<OperationGroup>(group));
    return group;
}

void OperationRepository::AddOperation(Operation* operation)
{
    OperationGroup* group = GetOrInsertOperationGroup(operation->GroupName());
    group->AddOperation(operation);
    operations.push_back(std::unique_ptr<Operation>(operation));
}

FunctionSymbol* OperationRepository::GetOperation(const std::string& groupName, std::vector<std::unique_ptr<BoundExpressionNode>>& args,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    auto it = groupMap.find(groupName);
    if (it != groupMap.cend())
    {
        OperationGroup* group = it->second;
        return group->GetOperation(args, fullSpan, context);
    }
    else
    {
        return nullptr;
    }
}

} // namespace otava::symbols
