// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.fundamental_type_conversion;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.function_symbol;
import otava.symbols.variable_symbol;
import otava.symbols.writer;
import otava.symbols.reader;
import otava.intermediate.data;
import otava.intermediate.types;

export namespace otava::symbols {

class Context;

struct FundamentalTypeIdentity
{
    inline static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context)
    {
        return value;
    }
};

struct FundamentalTypeSignExtension
{
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context);
};

struct FundamentalTypeZeroExtension
{
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context);
};

struct FundamentalTypeFloatingPointExtension
{
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context);
};

struct FundamentalTypeTruncate
{
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context);
};

struct FundamentalTypeFloatingPointTruncate
{
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context);
};

struct FundamentalTypeBitcast
{
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context);
};

struct FundamentalTypeIntToFloat
{
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context);
};

struct FundamentalTypeFloatToInt
{
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context);
};

struct FundamentalTypeBoolToInt
{
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* value, otava::intermediate::Type* destType, Context* context);
};

template<class Op>
struct FundamentalTypeConversion : public FunctionSymbol
{
    FundamentalTypeConversion(Module* module_, SymbolId id_) :
        FunctionSymbol(module_, id_, "@conversion"), distance(0), conversionKind(ConversionKind::implicitConversion), paramType(nullptr), argType(nullptr)
    {
    }
    FundamentalTypeConversion(Module* module_, SymbolId id_, std::int32_t distance_, ConversionKind conversionKind_, TypeSymbol* paramType_, 
        TypeSymbol* argType_, Context* context) :
        FunctionSymbol(module_, id_, "@conversion"), distance(distance_), conversionKind(conversionKind_), paramType(paramType_), argType(argType_)
    {
        SetConversion();
        SetAccess(Access::public_);
        SetConversionParamType(paramType_);
        SetConversionArgType(argType_);
        ParameterSymbol* arg = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "arg");
        arg->SetType(argType);
        AddSymbol(arg, soul::ast::FullSpan(), context);
        SetReturnType(paramType_, soul::ast::FullSpan(), context);
        SetNoExcept();
    }
    TypeSymbol* ConversionParamType() const noexcept override
    {
        return paramType;
    }
    TypeSymbol* ConversionArgType() const noexcept override
    {
        return argType;
    }
    ConversionKind GetConversionKind() const noexcept override
    {
        return conversionKind;
    }
    std::int32_t ConversionDistance() const noexcept override
    {
        return distance;
    }
    void  Write(Writer& writer) override
    {
        FunctionSymbol::Write(writer);
        writer.GetBinaryStreamWriter().Write(distance);
        writer.GetBinaryStreamWriter().Write(static_cast<std::uint8_t>(conversionKind));
        writer.GetBinaryStreamWriter().Write(ToUnderlying(paramType->Id()));
        writer.GetBinaryStreamWriter().Write(ToUnderlying(argType->Id()));
    }
    void Read(Reader& reader) override
    {
/*
        FunctionSymbol::Read(reader);
        distance = reader.GetMemoryReader().ReadInt();
        conversionKind = static_cast<ConversionKind>(reader.GetMemoryReader().ReadByte());
        paramTypeId = reader.ReadTypeSymbolId();
        argTypeId = reader.ReadTypeSymbolId();
*/
    }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override
    {
        context->SetArgType(argType);
        context->SetParamType(paramType);
        otava::intermediate::Value* value = emitter.Stack().Pop();
        emitter.Stack().Push(Op::Generate(emitter, value, static_cast<otava::intermediate::Type*>(paramType->IrType(emitter, fullSpan, context)), context));
    }
    std::int32_t distance;
    ConversionKind conversionKind;
    TypeSymbol* paramType;
    TypeSymbol* argType;
};

class FundamentalTypeSignExtendConversion : public FundamentalTypeConversion<FundamentalTypeSignExtension>
{
public:
    FundamentalTypeSignExtendConversion(Module* module_, SymbolId id_);
    FundamentalTypeSignExtendConversion(Module* module_, SymbolId id_, std::int32_t distance_, ConversionKind conversionKind_, 
        TypeSymbol* paramType_, TypeSymbol* argType_, Context* context);
};

class FundamentalTypeZeroExtendConversion : public FundamentalTypeConversion<FundamentalTypeZeroExtension>
{
public:
    FundamentalTypeZeroExtendConversion(Module* module_, SymbolId id_);
    FundamentalTypeZeroExtendConversion(Module* module_, SymbolId id_, std::int32_t distance_, ConversionKind conversionKind_, 
        TypeSymbol* paramType_, TypeSymbol* argType_, Context* context);
};

class FundamentalTypeFloatingPointExtendConversion : public FundamentalTypeConversion<FundamentalTypeFloatingPointExtension>
{
public:
    FundamentalTypeFloatingPointExtendConversion(Module* module_, SymbolId id_);
    FundamentalTypeFloatingPointExtendConversion(Module* module_, SymbolId id_, std::int32_t distance_, ConversionKind conversionKind_, 
        TypeSymbol* paramType_, TypeSymbol* argType_, Context* context);
};

class FundamentalTypeTruncateConversion : public FundamentalTypeConversion<FundamentalTypeTruncate>
{
public:
    FundamentalTypeTruncateConversion(Module* module_, SymbolId id_);
    FundamentalTypeTruncateConversion(Module* module_, SymbolId id_, std::int32_t distance_, ConversionKind conversionKind_, 
        TypeSymbol* paramType_, TypeSymbol* argType_, Context* context);
};

class FundamentalTypeBitcastConversion : public FundamentalTypeConversion<FundamentalTypeBitcast>
{
public:
    FundamentalTypeBitcastConversion(Module* module_, SymbolId id_);
    FundamentalTypeBitcastConversion(Module* module_, SymbolId id_, std::int32_t distance_, ConversionKind conversionKind_, 
        TypeSymbol* paramType_, TypeSymbol* argType_, Context* context);
};

class FundamentalTypeIntToFloatConversion : public FundamentalTypeConversion<FundamentalTypeIntToFloat>
{
public:
    FundamentalTypeIntToFloatConversion(Module* module_, SymbolId id_);
    FundamentalTypeIntToFloatConversion(Module* module_, SymbolId id_, std::int32_t distance_, ConversionKind conversionKind_, 
        TypeSymbol* paramType_, TypeSymbol* argType_, Context* context);
};

class FundamentalTypeFloatToIntConversion : public FundamentalTypeConversion<FundamentalTypeFloatToInt>
{
public:
    FundamentalTypeFloatToIntConversion(Module* module_, SymbolId id_);
    FundamentalTypeFloatToIntConversion(Module* module_, SymbolId id_, std::int32_t distance_, ConversionKind conversionKind_, 
        TypeSymbol* paramType_, TypeSymbol* argType_, Context* context);
};

class FundamentalTypeBoolToIntConversion : public FundamentalTypeConversion<FundamentalTypeBoolToInt>
{
public:
    FundamentalTypeBoolToIntConversion(Module* module_, SymbolId id_);
    FundamentalTypeBoolToIntConversion(Module* module_, SymbolId id_, TypeSymbol* boolType, TypeSymbol* destType, Context* context);
};

class FundamentalTypeBooleanConversion : public FunctionSymbol
{
public:
    FundamentalTypeBooleanConversion(Module* module_, SymbolId id_);
    FundamentalTypeBooleanConversion(Module* module_, SymbolId id_, TypeSymbol* type_, TypeSymbol* boolType, Context* context);
    TypeSymbol* ConversionParamType() const noexcept override;
    TypeSymbol* ConversionArgType() const noexcept override;
    ConversionKind GetConversionKind() const noexcept override;
    std::int32_t ConversionDistance() const noexcept override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
private:
    TypeSymbol* paramType;
    TypeSymbol* argType;
};

} // namespace otava::symbols
