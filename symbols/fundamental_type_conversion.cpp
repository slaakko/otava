// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.fundamental_type_conversion;

namespace otava::symbols {

otava::intermediate::Value* FundamentalTypeSignExtension::Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, 
    Context* context)
{
    return emitter.EmitSignExtend(value, destType);
}

otava::intermediate::Value* FundamentalTypeZeroExtension::Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, 
    Context* context)
{
    return emitter.EmitZeroExtend(value, destType);
}

otava::intermediate::Value* FundamentalTypeFloatingPointExtension::Generate(Emitter& emitter, otava::intermediate::Value* value, 
    otava::intermediate::Type* destType, Context* context)
{
    return emitter.EmitFPExtend(value, destType);
}

otava::intermediate::Value* FundamentalTypeTruncate::Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context)
{
    return emitter.EmitTruncate(value, destType);
}

otava::intermediate::Value* FundamentalTypeBitcast::Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context)
{
    return emitter.EmitBitcast(value, destType);
}

otava::intermediate::Value* FundamentalTypeIntToFloat::Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context)
{
/*
    if (destType->Size() == 4)
    {
        if (value->GetType()->Size() < 4)
        {
            ConversionTable& conversionTable = context->GetSymbolTable()->GetConversionTable();
            TypeSymbol* argType = context->ArgType();
            TypeSymbol* paramType = nullptr;
            if (value->GetType()->IsSignedType())
            {
                paramType = context->GetSymbolTable()->GetFundamentalType(FundamentalTypeKind::intType);
            }
            else if (value->GetType()->IsUnsignedType())
            {
                paramType = context->GetSymbolTable()->GetFundamentalType(FundamentalTypeKind::unsignedIntType);
            }
            FunctionSymbol* conversion = conversionTable.GetConversion(paramType, argType, context);
            BoundValueExpressionNode* arg = new BoundValueExpressionNode(value, argType);
            BoundConversionNode boundConversionNode(arg, conversion, soul::ast::FullSpan());
            boundConversionNode.Load(emitter, OperationFlags::none, soul::ast::FullSpan(), context);
            value = emitter.Stack().Pop();
        }
    }
    else if (destType->Size() == 8)
    {
        if (value->GetType()->Size() < 8)
        {
            ConversionTable& conversionTable = context->GetSymbolTable()->GetConversionTable();
            TypeSymbol* argType = context->ArgType();
            TypeSymbol* paramType = nullptr;
            if (value->GetType()->IsSignedType())
            {
                paramType = context->GetSymbolTable()->GetFundamentalType(FundamentalTypeKind::longLongIntType);
            }
            else if (value->GetType()->IsUnsignedType())
            {
                paramType = context->GetSymbolTable()->GetFundamentalType(FundamentalTypeKind::unsignedLongLongIntType);
            }
            FunctionSymbol* conversion = conversionTable.GetConversion(paramType, argType, context);
            BoundValueExpressionNode* arg = new BoundValueExpressionNode(value, argType);
            BoundConversionNode boundConversionNode(arg, conversion, soul::ast::FullSpan());
            boundConversionNode.Load(emitter, OperationFlags::none, soul::ast::FullSpan(), context);
            value = emitter.Stack().Pop();
        }
    }
    return emitter.EmitIntToFloat(value, destType);
*/
    return nullptr;
}

otava::intermediate::Value* FundamentalTypeFloatToInt::Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context)
{
/*
    if (value->GetType()->Size() == 4)
    {
        if (destType->Size() < 4)
        {
            ConversionTable& conversionTable = context->GetSymbolTable()->GetConversionTable();
            TypeSymbol* argType = context->ArgType();
            TypeSymbol* finalParamType = context->ParamType();
            TypeSymbol* paramType = nullptr;
            if (destType->IsSignedType())
            {
                paramType = context->GetSymbolTable()->GetFundamentalType(FundamentalTypeKind::intType);
            }
            else if (destType->IsUnsignedType())
            {
                paramType = context->GetSymbolTable()->GetFundamentalType(FundamentalTypeKind::unsignedIntType);
            }
            FunctionSymbol* firstConversion = conversionTable.GetConversion(paramType, argType, context);
            BoundValueExpressionNode* firstArg = new BoundValueExpressionNode(value, argType);
            BoundConversionNode firstConversionNode(firstArg, firstConversion, soul::ast::FullSpan());
            firstConversionNode.Load(emitter, OperationFlags::none, soul::ast::FullSpan(), context);
            value = emitter.Stack().Pop();
            BoundValueExpressionNode* arg = new BoundValueExpressionNode(value, paramType);
            FunctionSymbol* conversion = conversionTable.GetConversion(finalParamType, paramType, context);
            BoundConversionNode boundConversionNode(arg, conversion, soul::ast::FullSpan());
            boundConversionNode.Load(emitter, OperationFlags::none, soul::ast::FullSpan(), context);
            value = emitter.Stack().Pop();
            return value;
        }
    }
    else if (value->GetType()->Size() == 8)
    {
        if (destType->Size() < 8)
        {
            ConversionTable& conversionTable = context->GetSymbolTable()->GetConversionTable();
            TypeSymbol* argType = context->ArgType();
            TypeSymbol* finalParamType = context->ParamType();
            TypeSymbol* paramType = nullptr;
            if (destType->IsSignedType())
            {
                paramType = context->GetSymbolTable()->GetFundamentalType(FundamentalTypeKind::longLongIntType);
            }
            else if (destType->IsUnsignedType())
            {
                paramType = context->GetSymbolTable()->GetFundamentalType(FundamentalTypeKind::unsignedLongLongIntType);
            }
            FunctionSymbol* firstConversion = conversionTable.GetConversion(paramType, argType, context);
            BoundValueExpressionNode* firstArg = new BoundValueExpressionNode(value, argType);
            BoundConversionNode firstConversionNode(firstArg, firstConversion, soul::ast::FullSpan());
            firstConversionNode.Load(emitter, OperationFlags::none, soul::ast::FullSpan(), context);
            value = emitter.Stack().Pop();
            BoundValueExpressionNode* arg = new BoundValueExpressionNode(value, paramType);
            FunctionSymbol* conversion = conversionTable.GetConversion(finalParamType, paramType, context);
            BoundConversionNode boundConversionNode(arg, conversion, soul::ast::FullSpan());
            boundConversionNode.Load(emitter, OperationFlags::none, soul::ast::FullSpan(), context);
            value = emitter.Stack().Pop();
            return value;
        }
    }
    return emitter.EmitFloatToInt(value, destType);
*/
    return nullptr;
}

otava::intermediate::Value* FundamentalTypeBoolToInt::Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context)
{
    otava::intermediate::Value* resultVar = emitter.EmitLocal(destType);
    otava::intermediate::BasicBlock* trueDest = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* falseDest = emitter.CreateBasicBlock();
    otava::intermediate::BasicBlock* next = emitter.CreateBasicBlock();
    emitter.EmitBranch(value, trueDest, falseDest);
    emitter.SetCurrentBasicBlock(falseDest);
    otava::intermediate::Value* zero = emitter.EmitIntegerValue(destType, 0);
    emitter.EmitStore(zero, resultVar);
    emitter.EmitJump(next);
    emitter.SetCurrentBasicBlock(trueDest);
    otava::intermediate::Value* one = emitter.EmitIntegerValue(destType, 1);
    emitter.EmitStore(one, resultVar);
    emitter.EmitJump(next);
    emitter.SetCurrentBasicBlock(next);
    otava::intermediate::Value* result = emitter.EmitLoad(resultVar);
    return result;
}

FundamentalTypeSignExtendConversion::FundamentalTypeSignExtendConversion(Module* module_, SymbolId id_) : 
    FundamentalTypeConversion<FundamentalTypeSignExtension>(module_, id_)
{
}

FundamentalTypeSignExtendConversion::FundamentalTypeSignExtendConversion(Module* module_, SymbolId id_, std::int32_t distance_, 
    ConversionKind conversionKind_, TypeSymbol* paramType_, TypeSymbol* argType_, Context* context) :
    FundamentalTypeConversion<FundamentalTypeSignExtension>(module_, id_, distance_, conversionKind_, paramType_, argType_, context)
{
}

FundamentalTypeZeroExtendConversion::FundamentalTypeZeroExtendConversion(Module* module_, SymbolId id_) 
    : FundamentalTypeConversion<FundamentalTypeZeroExtension>(module_, id_)
{
}

FundamentalTypeZeroExtendConversion::FundamentalTypeZeroExtendConversion(Module* module_, SymbolId id_, std::int32_t distance_, 
    ConversionKind conversionKind_, TypeSymbol* paramType_, TypeSymbol* argType_, Context* context) :
    FundamentalTypeConversion<FundamentalTypeZeroExtension>(module_, id_, distance_, conversionKind_, paramType_, argType_, context)
{
}

FundamentalTypeFloatingPointExtendConversion::FundamentalTypeFloatingPointExtendConversion(Module* module_, SymbolId id_) :
    FundamentalTypeConversion<FundamentalTypeFloatingPointExtension>(module_, id_)
{
}

FundamentalTypeFloatingPointExtendConversion::FundamentalTypeFloatingPointExtendConversion(Module* module_, SymbolId id_, 
    std::int32_t distance_, ConversionKind conversionKind_, TypeSymbol* paramType_, TypeSymbol* argType_, Context* context) :
    FundamentalTypeConversion<FundamentalTypeFloatingPointExtension>(module_, id_, distance_, conversionKind_, paramType_, argType_,
        context)
{
}

FundamentalTypeTruncateConversion::FundamentalTypeTruncateConversion(Module* module_, SymbolId id_) : 
    FundamentalTypeConversion<FundamentalTypeTruncate>(module_, id_)
{
}

FundamentalTypeTruncateConversion::FundamentalTypeTruncateConversion(Module* module_, SymbolId id_, 
    std::int32_t distance_, ConversionKind conversionKind_, TypeSymbol* paramType_, TypeSymbol* argType_, Context* context) :
    FundamentalTypeConversion<FundamentalTypeTruncate>(module_, id_, distance_, conversionKind_, paramType_, argType_, context)
{
}

FundamentalTypeBitcastConversion::FundamentalTypeBitcastConversion(Module* module_, SymbolId id_) : 
    FundamentalTypeConversion<FundamentalTypeBitcast>(module_, id_)
{
}

FundamentalTypeBitcastConversion::FundamentalTypeBitcastConversion(Module* module_, SymbolId id_, 
    std::int32_t distance_, ConversionKind conversionKind_, TypeSymbol* paramType_, TypeSymbol* argType_, Context* context) :
    FundamentalTypeConversion<FundamentalTypeBitcast>(module_, id_, distance_, conversionKind_, paramType_, argType_, context)
{
}

FundamentalTypeIntToFloatConversion::FundamentalTypeIntToFloatConversion(Module* module_, SymbolId id_) : 
    FundamentalTypeConversion<FundamentalTypeIntToFloat>(module_, id_)
{
}

FundamentalTypeIntToFloatConversion::FundamentalTypeIntToFloatConversion(Module* module_, SymbolId id_, 
    std::int32_t distance_, ConversionKind conversionKind_, TypeSymbol* paramType_, TypeSymbol* argType_, Context* context) :
    FundamentalTypeConversion<FundamentalTypeIntToFloat>(module_, id_, distance_, conversionKind_, paramType_, argType_, context)
{
}

FundamentalTypeFloatToIntConversion::FundamentalTypeFloatToIntConversion(Module* module_, SymbolId id_) : 
    FundamentalTypeConversion<FundamentalTypeFloatToInt>(module_, id_)
{
}

FundamentalTypeFloatToIntConversion::FundamentalTypeFloatToIntConversion(Module* module_, SymbolId id_,
    std::int32_t distance_, ConversionKind conversionKind_, TypeSymbol* paramType_, TypeSymbol* argType_, Context* context) :
    FundamentalTypeConversion<FundamentalTypeFloatToInt>(module_, id_, distance_, conversionKind_, paramType_, argType_, context)
{
}

FundamentalTypeBoolToIntConversion::FundamentalTypeBoolToIntConversion(Module* module_, SymbolId id_) : 
    FundamentalTypeConversion<FundamentalTypeBoolToInt>(module_, id_)
{
}

FundamentalTypeBoolToIntConversion::FundamentalTypeBoolToIntConversion(Module* module_, SymbolId id_, TypeSymbol* boolType, TypeSymbol* destType, Context* context) :
    FundamentalTypeConversion<FundamentalTypeBoolToInt>(module_, id_, 255, ConversionKind::explicitConversion, destType, boolType, context)
{
}

FundamentalTypeBooleanConversion::FundamentalTypeBooleanConversion(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), argType(nullptr), paramType(nullptr)
{
}

FundamentalTypeBooleanConversion::FundamentalTypeBooleanConversion(Module* module_, SymbolId id_, TypeSymbol* type_, TypeSymbol* boolType, Context* context) :
    FunctionSymbol(module_, id_, "@conversion"), paramType(boolType), argType(type_)
{
    SetConversion();
    SetAccess(Access::public_);
    ParameterSymbol* param = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "param");
    param->SetType(argType);
    AddSymbol(param, soul::ast::FullSpan(), context);
    SetReturnType(boolType, soul::ast::FullSpan(), context);
    SetNoExcept();
}

TypeSymbol* FundamentalTypeBooleanConversion::ConversionParamType() const noexcept
{
    return paramType;
}

TypeSymbol* FundamentalTypeBooleanConversion::ConversionArgType() const noexcept
{
    return argType;
}

ConversionKind FundamentalTypeBooleanConversion::GetConversionKind() const noexcept
{
    return ConversionKind::implicitConversion;
}

std::int32_t FundamentalTypeBooleanConversion::ConversionDistance() const noexcept
{
    return 1;
}

void FundamentalTypeBooleanConversion::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(paramType->Id()));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(argType->Id()));
}

void FundamentalTypeBooleanConversion::Read(Reader& reader)
{
/*
    FunctionSymbol::Read(reader);
    reader.GetBinaryStreamReader().ReadUuid(paramTypeId);
    reader.GetBinaryStreamReader().ReadUuid(argTypeId);
*/
}

/*
void FundamentalTypeBooleanConversion::Resolve(SymbolTable& symbolTable, Context* context)
{
    FunctionSymbol::Resolve(symbolTable, context);
    paramType = symbolTable.GetType(paramTypeId);
    argType = symbolTable.GetType(argTypeId);
}
*/

void FundamentalTypeBooleanConversion::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::Value* value = emitter.Stack().Pop();
    otava::intermediate::Type* irType = static_cast<otava::intermediate::Type*>(argType->IrType(emitter, fullSpan, context));
    otava::intermediate::Value* defaultValue = irType->DefaultValue();
    otava::intermediate::Value* equal = emitter.EmitEqual(value, defaultValue);
    otava::intermediate::Value* notEqual = emitter.EmitNot(equal);
    emitter.Stack().Push(notEqual);
}

} // namespace otava::symbols
