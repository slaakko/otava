// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.fundamental_type_operation;

import otava.symbols.fundamental_type_conversion_init;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.bound_tree;
import otava.symbols.exception;
import otava.symbols.symbol_table;
import otava.symbols.context;
import otava.symbols.namespaces;
import otava.symbols.reader;
import otava.symbols.writer;
import otava.symbols.type_symbol;
import otava.symbols.modules;
import otava.symbols.context;
import otava.symbols.emitter;

namespace otava::symbols {

const char*FundamentalTypeNot::GroupName()
{
    return "operator!";
}

otava::intermediate::Value* FundamentalTypeNot::Generate(Emitter& emitter, otava::intermediate::Value* arg)
{
    return emitter.EmitNot(arg);
}

const char*FundamentalTypeUnaryPlus::GroupName()
{
    return "operator+";
}

otava::intermediate::Value* FundamentalTypeUnaryPlus::Generate(Emitter& emitter, otava::intermediate::Value* arg)
{
    return arg;
}

const char*FundamentalTypeUnaryMinus::GroupName()
{
    return "operator-";
}

otava::intermediate::Value* FundamentalTypeUnaryMinus::Generate(Emitter& emitter, otava::intermediate::Value* arg)
{
    return emitter.EmitNeg(arg);
}

const char*FundamentalTypeComplement::GroupName()
{
    return "operator~";
}

otava::intermediate::Value* FundamentalTypeComplement::Generate(Emitter& emitter, otava::intermediate::Value* arg)
{
    return emitter.EmitNot(arg);
}

const char*FundamentalTypeAdd::GroupName()
{
    return "operator+";
}

const char*FundamentalTypeAdd::AssignmentOpGroupName()
{
    return "operator+=";
}

otava::intermediate::Value* FundamentalTypeAdd::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitAdd(left, right);
}

const char*FundamentalTypeSub::GroupName()
{
    return "operator-";
}

const char*FundamentalTypeSub::AssignmentOpGroupName()
{
    return "operator-=";
}

otava::intermediate::Value* FundamentalTypeSub::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitSub(left, right);
}

const char*FundamentalTypeMul::GroupName()
{
    return "operator*";
}

const char*FundamentalTypeMul::AssignmentOpGroupName()
{
    return "operator*=";
}

otava::intermediate::Value* FundamentalTypeMul::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitMul(left, right);
}

const char*FundamentalTypeDiv::GroupName()
{
    return "operator/";
}

const char*FundamentalTypeDiv::AssignmentOpGroupName()
{
    return "operator/=";
}

otava::intermediate::Value* FundamentalTypeDiv::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitDiv(left, right);
}

const char*FundamentalTypeMod::GroupName()
{
    return "operator%";
}

const char*FundamentalTypeMod::AssignmentOpGroupName()
{
    return "operator%=";
}

otava::intermediate::Value* FundamentalTypeMod::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitMod(left, right);
}

const char*FundamentalTypeAnd::GroupName()
{
    return "operator&";
}

const char*FundamentalTypeAnd::AssignmentOpGroupName()
{
    return "operator&=";
}

otava::intermediate::Value* FundamentalTypeAnd::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitAnd(left, right);
}

const char*FundamentalTypeOr::GroupName()
{
    return "operator|";
}

const char*FundamentalTypeOr::AssignmentOpGroupName()
{
    return "operator|=";
}

otava::intermediate::Value* FundamentalTypeOr::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitOr(left, right);
}

const char*FundamentalTypeXor::GroupName()
{
    return "operator^";
}

const char*FundamentalTypeXor::AssignmentOpGroupName()
{
    return "operator^=";
}

otava::intermediate::Value* FundamentalTypeXor::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitXor(left, right);
}

const char*FundamentalTypeShl::GroupName()
{
    return "operator<<";
}

const char*FundamentalTypeShl::AssignmentOpGroupName()
{
    return "operator<<=";
}

otava::intermediate::Value* FundamentalTypeShl::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitShl(left, right);
}

const char*FundamentalTypeShr::GroupName()
{
    return "operator>>";
}

const char*FundamentalTypeShr::AssignmentOpGroupName()
{
    return "operator>>=";
}

otava::intermediate::Value* FundamentalTypeShr::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitShr(left, right);
}

const char*FundamentalTypeEqual::GroupName()
{
    return "operator==";
}

otava::intermediate::Value* FundamentalTypeEqual::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitEqual(left, right);
}

const char*FundamentalTypeLess::GroupName()
{
    return "operator<";
}

otava::intermediate::Value* FundamentalTypeLess::Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right)
{
    return emitter.EmitLess(left, right);
}

FundamentalTypeNotOperation::FundamentalTypeNotOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeUnaryOperation<FundamentalTypeNot>(module_, id_)
{
}

FundamentalTypeNotOperation::FundamentalTypeNotOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeUnaryOperation<FundamentalTypeNot>(module_, id_, type_, context)
{
}

FundamentalTypeUnaryPlusOperation::FundamentalTypeUnaryPlusOperation(Module* module_, SymbolId id_) :
    FundamentalTypeUnaryOperation<FundamentalTypeUnaryPlus>(module_, id_)
{
}

FundamentalTypeUnaryPlusOperation::FundamentalTypeUnaryPlusOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeUnaryOperation<FundamentalTypeUnaryPlus>(module_, id_, type_, context)
{
}

FundamentalTypeUnaryMinusOperation::FundamentalTypeUnaryMinusOperation(Module* module_, SymbolId id_) :
    FundamentalTypeUnaryOperation<FundamentalTypeUnaryMinus>(module_, id_)
{
}

FundamentalTypeUnaryMinusOperation::FundamentalTypeUnaryMinusOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeUnaryOperation<FundamentalTypeUnaryMinus>(module_, id_, type_, context)
{
}

FundamentalTypeComplementOperation::FundamentalTypeComplementOperation(Module* module_, SymbolId id_) :
    FundamentalTypeUnaryOperation<FundamentalTypeComplement>(module_, id_)
{
}

FundamentalTypeComplementOperation::FundamentalTypeComplementOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeUnaryOperation<FundamentalTypeComplement>(module_, id_, type_, context)
{
}

FundamentalTypeAddOperation::FundamentalTypeAddOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeAdd>(module_, id_)
{
}

FundamentalTypeAddOperation::FundamentalTypeAddOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeBinaryOperation<FundamentalTypeAdd>(module_, id_, type_, context)
{
}

FundamentalTypeSubOperation::FundamentalTypeSubOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeSub>(module_, id_)
{
}

FundamentalTypeSubOperation::FundamentalTypeSubOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeBinaryOperation<FundamentalTypeSub>(module_, id_, type_, context)
{
}

FundamentalTypeMulOperation::FundamentalTypeMulOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeMul>(module_, id_)
{
}

FundamentalTypeMulOperation::FundamentalTypeMulOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeBinaryOperation<FundamentalTypeMul>(module_, id_, type_, context)
{
}

FundamentalTypeDivOperation::FundamentalTypeDivOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeDiv>(module_, id_)
{
}

FundamentalTypeDivOperation::FundamentalTypeDivOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeBinaryOperation<FundamentalTypeDiv>(module_, id_, type_, context)
{
}

FundamentalTypeModOperation::FundamentalTypeModOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeMod>(module_, id_)
{
}

FundamentalTypeModOperation::FundamentalTypeModOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeBinaryOperation<FundamentalTypeMod>(module_, id_, type_, context)
{
}

FundamentalTypeAndOperation::FundamentalTypeAndOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeAnd>(module_, id_)
{
}

FundamentalTypeAndOperation::FundamentalTypeAndOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context)
    : FundamentalTypeBinaryOperation<FundamentalTypeAnd>(module_, id_, type_, context)
{
}

FundamentalTypeOrOperation::FundamentalTypeOrOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeOr>(module_, id_)
{
}

FundamentalTypeOrOperation::FundamentalTypeOrOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeBinaryOperation<FundamentalTypeOr>(module_, id_, type_, context)
{
}

FundamentalTypeXorOperation::FundamentalTypeXorOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeXor>(module_, id_)
{
}

FundamentalTypeXorOperation::FundamentalTypeXorOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeBinaryOperation<FundamentalTypeXor>(module_, id_, type_, context)
{
}

FundamentalTypeShlOperation::FundamentalTypeShlOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeShl>(module_, id_)
{
}

FundamentalTypeShlOperation::FundamentalTypeShlOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeBinaryOperation<FundamentalTypeShl>(module_, id_, type_, context)
{
}

FundamentalTypeShrOperation::FundamentalTypeShrOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeBinaryOperation<FundamentalTypeShr>(module_, id_)
{
}

FundamentalTypeShrOperation::FundamentalTypeShrOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeBinaryOperation<FundamentalTypeShr>(module_, id_, type_, context)
{
}

FundamentalTypePlusAssignOperation::FundamentalTypePlusAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeAdd>(module_, id_)
{
}

FundamentalTypePlusAssignOperation::FundamentalTypePlusAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeAdd>(module_, id_, type_, context)
{
}

FundamentalTypeMinusAssignOperation::FundamentalTypeMinusAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeSub>(module_, id_)
{
}

FundamentalTypeMinusAssignOperation::FundamentalTypeMinusAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeSub>(module_, id_, type_, context)
{
}

FundamentalTypeMulAssignOperation::FundamentalTypeMulAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeMul>(module_, id_)
{
}

FundamentalTypeMulAssignOperation::FundamentalTypeMulAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeMul>(module_, id_, type_, context)
{
}

FundamentalTypeDivAssignOperation::FundamentalTypeDivAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeDiv>(module_, id_)
{
}

FundamentalTypeDivAssignOperation::FundamentalTypeDivAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeDiv>(module_, id_, type_, context)
{
}

FundamentalTypeModAssignOperation::FundamentalTypeModAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeMod>(module_, id_)
{
}

FundamentalTypeModAssignOperation::FundamentalTypeModAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeMod>(module_, id_, type_, context)
{
}

FundamentalTypeAndAssignOperation::FundamentalTypeAndAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeAnd>(module_, id_)
{
}

FundamentalTypeAndAssignOperation::FundamentalTypeAndAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeAnd>(module_, id_, type_, context)
{
}

FundamentalTypeOrAssignOperation::FundamentalTypeOrAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeOr>(module_, id_)
{
}

FundamentalTypeOrAssignOperation::FundamentalTypeOrAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeOr>(module_, id_, type_, context)
{
}

FundamentalTypeXorAssignOperation::FundamentalTypeXorAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeXor>(module_, id_)
{
}

FundamentalTypeXorAssignOperation::FundamentalTypeXorAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeXor>(module_, id_, type_, context)
{
}

FundamentalTypeShlAssignOperation::FundamentalTypeShlAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeShl>(module_, id_)
{
}

FundamentalTypeShlAssignOperation::FundamentalTypeShlAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeShl>(module_, id_, type_, context)
{
}

FundamentalTypeShrAssignOperation::FundamentalTypeShrAssignOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeAssignmentOperation<FundamentalTypeShr>(module_, id_)
{
}

FundamentalTypeShrAssignOperation::FundamentalTypeShrAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FundamentalTypeAssignmentOperation<FundamentalTypeShr>(module_, id_, type_, context)
{
}

FundamentalTypeEqualOperation::FundamentalTypeEqualOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeComparisonOperation<FundamentalTypeEqual>(module_, id_)
{
}

FundamentalTypeEqualOperation::FundamentalTypeEqualOperation(Module* module_, SymbolId id_, TypeSymbol* type_, TypeSymbol* boolType_, Context* context) :
    FundamentalTypeComparisonOperation<FundamentalTypeEqual>(module_, id_, type_, boolType_, context)
{
}

FundamentalTypeLessOperation::FundamentalTypeLessOperation(Module* module_, SymbolId id_) : 
    FundamentalTypeComparisonOperation<FundamentalTypeLess>(module_, id_)
{
}

FundamentalTypeLessOperation::FundamentalTypeLessOperation(Module* module_, SymbolId id_, TypeSymbol* type_, TypeSymbol* boolType_, Context* context) :
    FundamentalTypeComparisonOperation<FundamentalTypeLess>(module_, id_, type_, boolType_, context)
{
}

FundamentalTypeDefaultCtor::FundamentalTypeDefaultCtor(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), type(nullptr), typeId(zeroSymbolId)
{
}

FundamentalTypeDefaultCtor::FundamentalTypeDefaultCtor(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) :
    FunctionSymbol(module_, id_, "@constructor"), type(type_), typeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    SetNoExcept();
}

void FundamentalTypeDefaultCtor::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(type->Id()));
}

void FundamentalTypeDefaultCtor::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    typeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void FundamentalTypeDefaultCtor::Resolve(Context* context)
{
    if (IsReadOnly() && typeId != zeroSymbolId)
    {
        type = GetModule()->GetSymbolTable()->GetTypeSymbol(typeId, context);
        if (!type)
        {
            ThrowException("type id " + std::to_string(ToUnderlying(typeId)) + " not found from module '" + GetModule()->Name() + "'");
        }
    }
}

void FundamentalTypeDefaultCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    if ((flags & OperationFlags::defaultInit) != OperationFlags::none)
    {
        Resolve(context);
        emitter.Stack().Push(type->IrType(emitter, fullSpan, context)->DefaultValue());
        OperationFlags storeFlags = OperationFlags::none;
        if ((flags & OperationFlags::storeDeref) != OperationFlags::none)
        {
            storeFlags = storeFlags | OperationFlags::deref;
        }
        args[0]->Store(emitter, storeFlags, fullSpan, context);
    }
}

FundamentalTypeCopyCtor::FundamentalTypeCopyCtor(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_)
{
}

FundamentalTypeCopyCtor::FundamentalTypeCopyCtor(Module* module_, SymbolId id_, TypeSymbol* type, Context* context) :
    FunctionSymbol(module_, id_, "@constructor")
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(type, context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetNoExcept();
}

void FundamentalTypeCopyCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    OperationFlags storeFlags = OperationFlags::none;
    if ((flags & OperationFlags::storeDeref) != OperationFlags::none)
    {
        storeFlags = storeFlags | OperationFlags::deref;
    }
    args[0]->Store(emitter, storeFlags, fullSpan, context);
}

FundamentalTypeMoveCtor::FundamentalTypeMoveCtor(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_)
{
}

FundamentalTypeMoveCtor::FundamentalTypeMoveCtor(Module* module_, SymbolId id_, TypeSymbol* type, Context* context) :
    FunctionSymbol(module_, id_, "@constructor")
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(type->AddRValueRef(context), context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetNoExcept();
}

void FundamentalTypeMoveCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
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

FundamentalTypeCopyAssignment::FundamentalTypeCopyAssignment(Module* module_, SymbolId id_) : 
    FunctionSymbol(module_, id_)
{
}

FundamentalTypeCopyAssignment::FundamentalTypeCopyAssignment(Module* module_, SymbolId id_, TypeSymbol* type, Context* context)
    : FunctionSymbol(module_, id_, "operator=")
{
    SetFunctionKind(FunctionKind::special);
    SetAccess(Access::public_);

    ParameterSymbol* thisParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(type, context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetReturnType(type->AddLValueRef(context), context);
    SetNoExcept();
}

void FundamentalTypeCopyAssignment::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    args[0]->Store(emitter, OperationFlags::setPtr, fullSpan, context);
    emitter.Stack().Push(context->Ptr());
}

FundamentalTypeMoveAssignment::FundamentalTypeMoveAssignment(Module* module_, SymbolId id_) : 
    FunctionSymbol(module_, id_)
{
}

FundamentalTypeMoveAssignment::FundamentalTypeMoveAssignment(Module* module_, SymbolId id_, TypeSymbol* type, Context* context) :
    FunctionSymbol(module_, id_, "operator=")
{
    SetFunctionKind(FunctionKind::special);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(type->AddRValueRef(context), context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetReturnType(type->AddLValueRef(context), context);
    SetNoExcept();
}

void FundamentalTypeMoveAssignment::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* refValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitLoad(refValue));
    args[0]->Store(emitter, OperationFlags::setPtr, fullSpan, context);
    emitter.Stack().Push(context->Ptr());
}

TrivialDestructor::TrivialDestructor(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_)
{
}

TrivialDestructor::TrivialDestructor(Module* module_, SymbolId id_, TypeSymbol* type, Context* context) :
    FunctionSymbol(module_, id_, "@destructor")
{
    SetFunctionKind(FunctionKind::destructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(type->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    SetFlag(FunctionSymbolFlags::trivialDestructor);
    SetNoExcept();
}

void TrivialDestructor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
}
void AddFundamentalIntegerOperationsToSymbolTable(TypeSymbol* type, Context* context)
{
    soul::ast::FullSpan fullSpan;
    TypeSymbol* boolType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType,  context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(), 
        new FundamentalTypeUnaryPlusOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeUnaryPlus), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeUnaryMinusOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeUnaryMinus), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeComplementOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeComplement), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeAddOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeAdd), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSubOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeSub), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMulOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMul), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeDivOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeDiv), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeModOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMod), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeAndOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeAnd), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeOrOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeOr), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeXorOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeXor), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeShlOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeShl), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeShrOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeShr), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypePlusAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypePlusAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMinusAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMinusAssign), type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMulAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMulAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeDivAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeDivAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeModAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeModAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeAndAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeAndAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeOrAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeOrAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeXorAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeXorAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeShlAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeShlAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeShrAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeShrAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeEqualOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeEqual), type, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeLessOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeLess), type, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeDefaultCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeDefaultCtor), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeCopyCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeCopyCtor), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMoveCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMoveCtor), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeCopyAssignment(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeCopyAssignment), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMoveAssignment(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMoveAssignment), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new TrivialDestructor(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), type, context), fullSpan, context);
}

void AddFundamentalFloatingPointOperationsToSymbolTable(TypeSymbol* type, Context* context)
{
    soul::ast::FullSpan fullSpan;
    TypeSymbol* boolType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeUnaryPlusOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeUnaryPlus), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeUnaryMinusOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeUnaryMinus), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeAddOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeAdd), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSubOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeSub), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMulOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMul), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeDivOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeDiv), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypePlusAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypePlusAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMinusAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMinusAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMulAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMulAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeDivAssignOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeDivAssign), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeEqualOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeEqual), type, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeLessOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeLess), type, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeDefaultCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeDefaultCtor), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeCopyCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeCopyCtor), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMoveCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMoveCtor), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeCopyAssignment(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeCopyAssignment), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMoveAssignment(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMoveAssignment), type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new TrivialDestructor(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), type, context), fullSpan, context);
}

void AddFundamentalTypeOperationsToSymbolTable(Context* context)
{
    soul::ast::FullSpan fullSpan;
    TypeSymbol* boolType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeDefaultCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeDefaultCtor), boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeCopyCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeCopyCtor), boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMoveCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMoveCtor), boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeCopyAssignment(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeCopyAssignment), boolType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeMoveAssignment(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeMoveAssignment), boolType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeEqualOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeEqual), boolType, boolType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new TrivialDestructor(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeNotOperation(context->GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeNot), boolType, context), fullSpan, context);
    TypeSymbol* charType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::charType, context);
    TypeSymbol* signedCharType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::signedCharType, context);
    TypeSymbol* unsignedCharType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedCharType, context);
    TypeSymbol* char8Type = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::char8Type, context);
    TypeSymbol* char16Type = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::char16Type, context);
    TypeSymbol* char32Type = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::char32Type, context);
    TypeSymbol* wcharType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::wcharType, context);
    TypeSymbol* shortIntType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::shortIntType, context);
    TypeSymbol* unsignedShortIntType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedShortIntType, context);
    TypeSymbol* intType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context);
    TypeSymbol* unsignedIntType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedIntType, context);
    TypeSymbol* longIntType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longIntType, context);
    TypeSymbol* unsignedLongIntType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedLongIntType, context);
    TypeSymbol* longLongIntType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longLongIntType, context);
    TypeSymbol* unsignedLongLongIntType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedLongLongIntType, context);
    TypeSymbol* floatType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::floatType, context);
    TypeSymbol* doubleType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::doubleType, context);
    TypeSymbol* longDoubleType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longDoubleType, context);
    AddFundamentalIntegerOperationsToSymbolTable(charType, context);
    AddFundamentalIntegerOperationsToSymbolTable(signedCharType, context);
    AddFundamentalIntegerOperationsToSymbolTable(unsignedCharType, context);
    AddFundamentalIntegerOperationsToSymbolTable(char8Type, context);
    AddFundamentalIntegerOperationsToSymbolTable(char16Type, context);
    AddFundamentalIntegerOperationsToSymbolTable(char32Type, context);
    AddFundamentalIntegerOperationsToSymbolTable(wcharType, context);
    AddFundamentalIntegerOperationsToSymbolTable(shortIntType, context);
    AddFundamentalIntegerOperationsToSymbolTable(unsignedShortIntType, context);
    AddFundamentalIntegerOperationsToSymbolTable(intType, context);
    AddFundamentalIntegerOperationsToSymbolTable(unsignedIntType, context);
    AddFundamentalIntegerOperationsToSymbolTable(longIntType, context);
    AddFundamentalIntegerOperationsToSymbolTable(unsignedLongIntType, context);
    AddFundamentalIntegerOperationsToSymbolTable(longLongIntType, context);
    AddFundamentalIntegerOperationsToSymbolTable(unsignedLongLongIntType, context);
    AddFundamentalFloatingPointOperationsToSymbolTable(floatType, context);
    AddFundamentalFloatingPointOperationsToSymbolTable(doubleType, context);
    AddFundamentalTypeConversionsToSymboTable(charType, signedCharType, unsignedCharType, char8Type, char16Type, char32Type, wcharType,
        shortIntType, unsignedShortIntType, intType, unsignedIntType, longIntType, unsignedLongIntType, longLongIntType, unsignedLongLongIntType,
        floatType, doubleType, context);
}

} // namespace otava::symbols
