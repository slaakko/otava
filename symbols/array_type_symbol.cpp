// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.array_type_symbol;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.function_group_symbol;
import otava.symbols.modules;
import otava.symbols.writer;
import otava.symbols.reader;

namespace otava::symbols {

ArrayTypeKey::ArrayTypeKey(TypeSymbol* elementType, std::int64_t size_) : elementTypeId(elementType->Id()), size(size_)
{
}

size_t ArrayTypeKeyHash::operator()(const ArrayTypeKey& key) const noexcept
{
    size_t elementTypeHash = std::hash<std::uint32_t>()(ToUnderlying(key.elementTypeId));
    size_t sizeHash = std::hash<std::int64_t>()(key.size);
    return elementTypeHash ^ sizeHash;
}

std::string MakeArrayTypeName(TypeSymbol* elementType, std::int64_t size, Context* context)
{
    std::string fullName = elementType->FullName(context);
    fullName.append(1, '[');
    if (size != -1)
    {
        fullName.append(std::to_string(size));
    }
    fullName.append(1, ']');
    return fullName;
}

ArrayTypeSymbol::ArrayTypeSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), bound(false), elementType(nullptr), elementTypeId(zeroSymbolId), size(-1)
{
    GetScope()->SetKind(ScopeKind::arrayScope);
}

ArrayTypeSymbol::ArrayTypeSymbol(Module* module_, SymbolId id_, TypeSymbol* elementType_, std::int64_t size_, Context* context) : 
    TypeSymbol(module_, id_, MakeArrayTypeName(elementType_, size_, context)), bound(false), elementType(elementType_), elementTypeId(zeroSymbolId), size(size_)
{
    GetScope()->SetKind(ScopeKind::arrayScope);
    if (elementType->GetModule() != GetModule())
    {
        GetModule()->GetSymbolTable()->AddImportedSymbol(elementType->Id(), elementType->GetModule()->Id());
    }
}

void ArrayTypeSymbol::Write(Writer& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(bound);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(elementType->Id()));
    writer.GetBinaryStreamWriter().Write(size);
}

void ArrayTypeSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    bound = reader.CurrentReader().ReadBool();
    elementTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    size = reader.CurrentReader().ReadLong();
}

void ArrayTypeSymbol::Bind(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (IsBound()) return;
    SetBound();
    FunctionGroupSymbol* constructorGroup = GetScope()->GetOrInsertFunctionGroup("@constructor", fullSpan, context);

    ArrayTypeDefaultCtor* arrayTypeDefaultCtor = new ArrayTypeDefaultCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::arrayTypeDefaultCtor), this, context);
    GetScope()->AddSymbol(arrayTypeDefaultCtor, fullSpan, context);
    constructorGroup->AddFunction(arrayTypeDefaultCtor);

    ArrayTypeCopyCtor* arrayTypeCopyCtor = new ArrayTypeCopyCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::arrayTypeCopyCtor), this, context);
    GetScope()->AddSymbol(arrayTypeCopyCtor, fullSpan, context);
    constructorGroup->AddFunction(arrayTypeCopyCtor);

    ArrayTypeMoveCtor* arrayTypeMoveCtor = new ArrayTypeMoveCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::arrayTypeMoveCtor), this, context);
    GetScope()->AddSymbol(arrayTypeMoveCtor, fullSpan, context);
    constructorGroup->AddFunction(arrayTypeMoveCtor);

    FunctionGroupSymbol* assignmentGroup = GetScope()->GetOrInsertFunctionGroup("operator=", fullSpan, context);

    ArrayTypeCopyAssignment* arrayTypeCopyAssignment = new ArrayTypeCopyAssignment(context->GetModule(), 
        context->GetNextSymbolId(SymbolKind::arrayTypeCopyAssignment), this, context);
    GetScope()->AddSymbol(arrayTypeCopyAssignment, fullSpan, context);
    assignmentGroup->AddFunction(arrayTypeCopyAssignment);

    ArrayTypeMoveAssignment* arrayTypeMoveAssignment = new ArrayTypeMoveAssignment(context->GetModule(), 
        context->GetNextSymbolId(SymbolKind::arrayTypeMoveAssignment), this, context);
    GetScope()->AddSymbol(arrayTypeMoveAssignment, fullSpan, context);
    assignmentGroup->AddFunction(arrayTypeMoveAssignment);

    FunctionGroupSymbol* beginGroup = GetScope()->GetOrInsertFunctionGroup("begin", fullSpan, context);

    ArrayTypeBegin* arrayTypeBegin = new ArrayTypeBegin(context->GetModule(), context->GetNextSymbolId(SymbolKind::arrayTypeBegin), this, context);
    GetScope()->AddSymbol(arrayTypeBegin, fullSpan, context);
    beginGroup->AddFunction(arrayTypeBegin);

    FunctionGroupSymbol* endGroup = GetScope()->GetOrInsertFunctionGroup("end", fullSpan, context);

    ArrayTypeEnd* arrayTypeEnd = new ArrayTypeEnd(context->GetModule(), context->GetNextSymbolId(SymbolKind::arrayTypeEnd), this, context);
    GetScope()->AddSymbol(arrayTypeEnd, fullSpan, context);
    endGroup->AddFunction(arrayTypeEnd);
}

TypeSymbol* ArrayTypeSymbol::ElementType(Context* context) const 
{
    if (elementType)
    {
        return elementType;
    }
    if (IsReadOnly() && elementTypeId != zeroSymbolId)
    {
        elementType = GetModule()->GetSymbolTable()->GetTypeSymbol(elementTypeId, context);
        if (!elementType)
        {
            ThrowException("element type id " + std::to_string(ToUnderlying(elementTypeId)) + " not found");
        }
    }
    return elementType;
}

bool ArrayTypeSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::classScope:
    case ScopeKind::functionScope:
    {
        return true;
    }
    }
    return false;
}

otava::intermediate::Type* ArrayTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    SymbolId id = Id();
    otava::intermediate::Type* type = emitter.GetType(id);
    if (!type)
    {
        type = emitter.MakeArrayType(size, ElementType(context)->IrType(emitter, fullSpan, context));
        emitter.SetType(id, type);
    }
    return type;
}

ArrayTypeDefaultCtor::ArrayTypeDefaultCtor(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), arrayType(nullptr), arrayTypeId(zeroSymbolId)
{
}

ArrayTypeDefaultCtor::ArrayTypeDefaultCtor(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context) : 
    FunctionSymbol(module_, id_, "@array_default_ctor"), arrayType(arrayType_), arrayTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(arrayType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    SetNoExcept();
}

void ArrayTypeDefaultCtor::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(arrayType->Id()));
}

void ArrayTypeDefaultCtor::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    arrayTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void ArrayTypeDefaultCtor::Resolve(Context* context)
{
    if (IsReadOnly() && !arrayType)
    {
        arrayType = GetModule()->GetSymbolTable()->GetArrayTypeSymbol(arrayTypeId, context);
        if (!arrayType)
        {
            ThrowException("array type id " + std::to_string(ToUnderlying(arrayTypeId)) + " not found");
        }
    }
}

void ArrayTypeDefaultCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    Resolve(context);
    if ((flags & OperationFlags::defaultInit) == OperationFlags::none) return;
    otava::intermediate::Value* loopIndexVar = emitter.EmitLocal(emitter.GetLongType());
    emitter.EmitStore(emitter.EmitLong(0), loopIndexVar);
    otava::intermediate::BasicBlock* condBlock = emitter.CreateBasicBlock();
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(condBlock);
    otava::intermediate::Value* loopIndexValue = emitter.EmitLoad(loopIndexVar);
    otava::intermediate::Value* size = emitter.EmitLong(arrayType->Size());
    otava::intermediate::Value* less = emitter.EmitLess(loopIndexValue, size);
    otava::intermediate::BasicBlock* trueBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter.CreateBasicBlock();
    emitter.EmitBranch(less, trueBlock, nextBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    otava::intermediate::Value* currentLoopIndexValue = emitter.EmitLoad(loopIndexVar);
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* array = emitter.Stack().Pop();
    otava::intermediate::Value* elemAddr = emitter.EmitElemAddr(array, currentLoopIndexValue);
    otava::intermediate::Value* defaultValue = arrayType->ElementType(context)->IrType(
        emitter, fullSpan, context)->MakeDefaultValue(*emitter.GetIntermediateContext());
    emitter.EmitStore(defaultValue, elemAddr);
    otava::intermediate::Value* nextLoopIndexValue = emitter.EmitAdd(currentLoopIndexValue, emitter.EmitLong(1));
    emitter.EmitStore(nextLoopIndexValue, loopIndexVar);
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
}

ArrayTypeCopyCtor::ArrayTypeCopyCtor(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), arrayType(nullptr), arrayTypeId(zeroSymbolId)
{
}

ArrayTypeCopyCtor::ArrayTypeCopyCtor(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(module_, id_, "@array_copy_ctor"), arrayType(arrayType_), arrayTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(arrayType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(arrayType->AddConst(context)->AddLValueRef(context), context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetNoExcept();
}

void ArrayTypeCopyCtor::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(arrayType->Id()));
}

void ArrayTypeCopyCtor::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    arrayTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void ArrayTypeCopyCtor::Resolve(Context* context)
{
    if (IsReadOnly() && !arrayType)
    {
        arrayType = GetModule()->GetSymbolTable()->GetArrayTypeSymbol(arrayTypeId, context);
        if (!arrayType)
        {
            ThrowException("array type id " + std::to_string(ToUnderlying(arrayTypeId)) + " not found");
        }
    }
}

void ArrayTypeCopyCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    Resolve(context);
    otava::intermediate::Value* loopIndexVar = emitter.EmitLocal(emitter.GetLongType());
    emitter.EmitStore(emitter.EmitLong(0), loopIndexVar);
    otava::intermediate::BasicBlock* condBlock = emitter.CreateBasicBlock();
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(condBlock);
    otava::intermediate::Value* loopIndexValue = emitter.EmitLoad(loopIndexVar);
    otava::intermediate::Value* size = emitter.EmitLong(arrayType->Size());
    otava::intermediate::Value* less = emitter.EmitLess(loopIndexValue, size);
    otava::intermediate::BasicBlock* trueBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter.CreateBasicBlock();
    emitter.EmitBranch(less, trueBlock, nextBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    otava::intermediate::Value* currentLoopIndexValue = emitter.EmitLoad(loopIndexVar);
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* sourceArray = emitter.Stack().Pop();
    otava::intermediate::Value* sourceElemAddr = emitter.EmitElemAddr(sourceArray, currentLoopIndexValue);
    otava::intermediate::Value* sourceValue = emitter.EmitLoad(sourceElemAddr);
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* targetArray = emitter.Stack().Pop();
    otava::intermediate::Value* targetElemAddr = emitter.EmitElemAddr(targetArray, currentLoopIndexValue);
    emitter.EmitStore(sourceValue, targetElemAddr);
    otava::intermediate::Value* nextLoopIndexValue = emitter.EmitAdd(currentLoopIndexValue, emitter.EmitLong(1));
    emitter.EmitStore(nextLoopIndexValue, loopIndexVar);
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
}

ArrayTypeMoveCtor::ArrayTypeMoveCtor(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), arrayType(nullptr), arrayTypeId(zeroSymbolId)
{
}

ArrayTypeMoveCtor::ArrayTypeMoveCtor(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(module_, id_, "@array_move_ctor"), arrayType(arrayType_), arrayTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(arrayType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(arrayType->AddRValueRef(context), context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetNoExcept();
}

void ArrayTypeMoveCtor::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(arrayType->Id()));
}

void ArrayTypeMoveCtor::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    arrayTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void ArrayTypeMoveCtor::Resolve(Context* context)
{
    if (IsReadOnly() && !arrayType)
    {
        arrayType = GetModule()->GetSymbolTable()->GetArrayTypeSymbol(arrayTypeId, context);
        if (!arrayType)
        {
            ThrowException("array type id " + std::to_string(ToUnderlying(arrayTypeId)) + " not found");
        }
    }
}

void ArrayTypeMoveCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* loopIndexVar = emitter.EmitLocal(emitter.GetLongType());
    emitter.EmitStore(emitter.EmitLong(0), loopIndexVar);
    otava::intermediate::BasicBlock* condBlock = emitter.CreateBasicBlock();
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(condBlock);
    otava::intermediate::Value* loopIndexValue = emitter.EmitLoad(loopIndexVar);
    otava::intermediate::Value* size = emitter.EmitLong(arrayType->Size());
    otava::intermediate::Value* less = emitter.EmitLess(loopIndexValue, size);
    otava::intermediate::BasicBlock* trueBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter.CreateBasicBlock();
    emitter.EmitBranch(less, trueBlock, nextBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    otava::intermediate::Value* currentLoopIndexValue = emitter.EmitLoad(loopIndexVar);
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* sourceArray = emitter.Stack().Pop();
    otava::intermediate::Value* sourceElemAddr = emitter.EmitElemAddr(sourceArray, currentLoopIndexValue);
    otava::intermediate::Value* sourceValue = emitter.EmitLoad(sourceElemAddr);
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* targetArray = emitter.Stack().Pop();
    otava::intermediate::Value* targetElemAddr = emitter.EmitElemAddr(targetArray, currentLoopIndexValue);
    emitter.EmitStore(sourceValue, targetElemAddr);
    otava::intermediate::Value* nextLoopIndexValue = emitter.EmitAdd(currentLoopIndexValue, emitter.EmitLong(1));
    emitter.EmitStore(nextLoopIndexValue, loopIndexVar);
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
}

ArrayTypeCopyAssignment::ArrayTypeCopyAssignment(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), arrayType(nullptr), arrayTypeId(zeroSymbolId)
{
}

ArrayTypeCopyAssignment::ArrayTypeCopyAssignment(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(module_, id_, "@array_copy_assignment"), arrayType(arrayType_), arrayTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::special);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(arrayType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(arrayType->AddConst(context)->AddLValueRef(context), context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetReturnType(arrayType->AddLValueRef(context), context);
    SetNoExcept();
}

void ArrayTypeCopyAssignment::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(arrayType->Id()));
}

void ArrayTypeCopyAssignment::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    arrayTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void ArrayTypeCopyAssignment::Resolve(Context* context)
{
    if (IsReadOnly() && !arrayType)
    {
        arrayType = GetModule()->GetSymbolTable()->GetArrayTypeSymbol(arrayTypeId, context);
        if (!arrayType)
        {
            ThrowException("array type id " + std::to_string(ToUnderlying(arrayTypeId)) + " not found");
        }
    }
}

void ArrayTypeCopyAssignment::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    Resolve(context);
    otava::intermediate::Value* loopIndexVar = emitter.EmitLocal(emitter.GetLongType());
    emitter.EmitStore(emitter.EmitLong(0), loopIndexVar);
    otava::intermediate::BasicBlock* condBlock = emitter.CreateBasicBlock();
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(condBlock);
    otava::intermediate::Value* loopIndexValue = emitter.EmitLoad(loopIndexVar);
    otava::intermediate::Value* size = emitter.EmitLong(arrayType->Size());
    otava::intermediate::Value* less = emitter.EmitLess(loopIndexValue, size);
    otava::intermediate::BasicBlock* trueBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter.CreateBasicBlock();
    emitter.EmitBranch(less, trueBlock, nextBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    otava::intermediate::Value* currentLoopIndexValue = emitter.EmitLoad(loopIndexVar);
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* sourceArray = emitter.Stack().Pop();
    otava::intermediate::Value* sourceElemAddr = emitter.EmitElemAddr(sourceArray, currentLoopIndexValue);
    otava::intermediate::Value* sourceValue = emitter.EmitLoad(sourceElemAddr);
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* targetArray = emitter.Stack().Pop();
    otava::intermediate::Value* targetElemAddr = emitter.EmitElemAddr(targetArray, currentLoopIndexValue);
    emitter.EmitStore(sourceValue, targetElemAddr);
    otava::intermediate::Value* nextLoopIndexValue = emitter.EmitAdd(currentLoopIndexValue, emitter.EmitLong(1));
    emitter.EmitStore(nextLoopIndexValue, loopIndexVar);
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
}

ArrayTypeMoveAssignment::ArrayTypeMoveAssignment(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), arrayType(nullptr), arrayTypeId(zeroSymbolId)
{
}

ArrayTypeMoveAssignment::ArrayTypeMoveAssignment(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(module_, id_, "@array_move_assignment"), arrayType(arrayType_), arrayTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::special);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(arrayType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(arrayType->AddConst(context)->AddRValueRef(context), context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetReturnType(arrayType->AddLValueRef(context), context);
    SetNoExcept();
}

void ArrayTypeMoveAssignment::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(arrayType->Id()));
}

void ArrayTypeMoveAssignment::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    arrayTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void ArrayTypeMoveAssignment::Resolve(Context* context)
{
    if (IsReadOnly() && !arrayType)
    {
        arrayType = GetModule()->GetSymbolTable()->GetArrayTypeSymbol(arrayTypeId, context);
        if (!arrayType)
        {
            ThrowException("array type id " + std::to_string(ToUnderlying(arrayTypeId)) + " not found");
        }
    }
}

void ArrayTypeMoveAssignment::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    Resolve(context);
    otava::intermediate::Value* loopIndexVar = emitter.EmitLocal(emitter.GetLongType());
    emitter.EmitStore(emitter.EmitLong(0), loopIndexVar);
    otava::intermediate::BasicBlock* condBlock = emitter.CreateBasicBlock();
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(condBlock);
    otava::intermediate::Value* loopIndexValue = emitter.EmitLoad(loopIndexVar);
    otava::intermediate::Value* size = emitter.EmitLong(arrayType->Size());
    otava::intermediate::Value* less = emitter.EmitLess(loopIndexValue, size);
    otava::intermediate::BasicBlock* trueBlock = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter.CreateBasicBlock();
    emitter.EmitBranch(less, trueBlock, nextBlock);
    emitter.SetCurrentBasicBlock(trueBlock);
    otava::intermediate::Value* currentLoopIndexValue = emitter.EmitLoad(loopIndexVar);
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* sourceArray = emitter.Stack().Pop();
    otava::intermediate::Value* sourceElemAddr = emitter.EmitElemAddr(sourceArray, currentLoopIndexValue);
    otava::intermediate::Value* sourceValue = emitter.EmitLoad(sourceElemAddr);
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* targetArray = emitter.Stack().Pop();
    otava::intermediate::Value* targetElemAddr = emitter.EmitElemAddr(targetArray, currentLoopIndexValue);
    emitter.EmitStore(sourceValue, targetElemAddr);
    otava::intermediate::Value* nextLoopIndexValue = emitter.EmitAdd(currentLoopIndexValue, emitter.EmitLong(1));
    emitter.EmitStore(nextLoopIndexValue, loopIndexVar);
    emitter.EmitJump(condBlock);
    emitter.SetCurrentBasicBlock(nextBlock);
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
}

ArrayTypeBegin::ArrayTypeBegin(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), arrayType(nullptr), arrayTypeId(zeroSymbolId)
{
}

ArrayTypeBegin::ArrayTypeBegin(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(module_, id_, "@array_begin"), arrayType(arrayType_), arrayTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::function);;
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(arrayType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    SetReturnType(arrayType->ElementType(context)->AddPointer(context), context);
    SetNoExcept();
}

void ArrayTypeBegin::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(arrayType->Id()));
}

void ArrayTypeBegin::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    arrayTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void ArrayTypeBegin::Resolve(Context* context)
{
    if (IsReadOnly() && !arrayType)
    {
        arrayType = GetModule()->GetSymbolTable()->GetArrayTypeSymbol(arrayTypeId, context);
        if (!arrayType)
        {
            ThrowException("array type id " + std::to_string(ToUnderlying(arrayTypeId)) + " not found");
        }
    }
}

void ArrayTypeBegin::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    Resolve(context);
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* array = emitter.Stack().Pop();
    otava::intermediate::Value* elemAddr = emitter.EmitElemAddr(array, emitter.EmitLong(0));
    emitter.Stack().Push(elemAddr);
}

ArrayTypeEnd::ArrayTypeEnd(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), arrayType(nullptr), arrayTypeId(zeroSymbolId)
{
}

ArrayTypeEnd::ArrayTypeEnd(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context) :
    FunctionSymbol(module_, id_, "@array_end"), arrayType(arrayType_), arrayTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::function);;
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(arrayType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    SetReturnType(arrayType->ElementType(context)->AddPointer(context), context);
    SetNoExcept();
}

void ArrayTypeEnd::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(arrayType->Id()));
}

void ArrayTypeEnd::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    arrayTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void ArrayTypeEnd::Resolve(Context* context)
{
    if (IsReadOnly() && !arrayType)
    {
        arrayType = GetModule()->GetSymbolTable()->GetArrayTypeSymbol(arrayTypeId, context);
        if (!arrayType)
        {
            ThrowException("array type id " + std::to_string(ToUnderlying(arrayTypeId)) + " not found");
        }
    }
}

void ArrayTypeEnd::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    Resolve(context);
    args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* array = emitter.Stack().Pop();
    otava::intermediate::Value* elemAddr = emitter.EmitElemAddr(array, emitter.EmitLong(arrayType->Size()));
    emitter.Stack().Push(elemAddr);
}

} // namespace otava::symbols
