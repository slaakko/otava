// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.concrete_value;

import otava.symbols.id;
import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.evaluation_context;
import otava.symbols.exception;
import otava.symbols.fundamental_type_kind;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.modules;
import otava.symbols.value;
import otava.symbols.writer;
import otava.symbols.reader;
import otava.intermediate.types;
import otava.intermediate.value;
import util.unicode;
import soul.ast.span;
import std;

export namespace otava::symbols {

template<typename T>
class FundamentalTypeValue : public Value
{
public:
    using ValueType = T;
    FundamentalTypeValue(Module* module_, SymbolId id_) : Value(module_, id_), fundamentalTypeKind(FundamentalTypeKind::none), value() {}
    FundamentalTypeValue(Module* module_, SymbolId id_, FundamentalTypeKind fundamentalTypeKind_, T value_, Context* context) :
        Value(module_, id_, std::to_string(value_)), fundamentalTypeKind(fundamentalTypeKind_), value(value_)
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(fundamentalTypeKind, context);
        SetType(type, context);
    }
    inline T GetValue() const noexcept { return value; }
    inline void SetValue(T value_) noexcept { value = value_; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override
    {
        switch (fundamentalTypeKind)
        {
        case FundamentalTypeKind::boolType:
        {
            return emitter.EmitBool(value);
        }
        case FundamentalTypeKind::charType:
        case FundamentalTypeKind::unsignedCharType:
        case FundamentalTypeKind::char8Type:
        {
            return emitter.EmitByte(static_cast<std::uint8_t>(value));
        }
        case FundamentalTypeKind::signedCharType:
        {
            return emitter.EmitSByte(static_cast<std::int8_t>(value));
        }
        case FundamentalTypeKind::char16Type:
        case FundamentalTypeKind::wcharType:
        case FundamentalTypeKind::unsignedShortIntType:
        {
            return emitter.EmitUShort(static_cast<std::uint16_t>(value));
        }
        case FundamentalTypeKind::shortIntType:
        {
            return emitter.EmitShort(static_cast<std::int16_t>(value));
        }
        case FundamentalTypeKind::char32Type:
        case FundamentalTypeKind::unsignedIntType:
        case FundamentalTypeKind::unsignedLongIntType:
        {
            return emitter.EmitUInt(static_cast<std::uint32_t>(value));
        }
        case FundamentalTypeKind::intType:
        case FundamentalTypeKind::longIntType:
        {
            return emitter.EmitInt(static_cast<std::int32_t>(value));
        }
        case FundamentalTypeKind::longLongIntType:
        {
            return emitter.EmitLong(static_cast<std::int64_t>(value));
        }
        case FundamentalTypeKind::unsignedLongLongIntType:
        {
            return emitter.EmitULong(static_cast<std::uint64_t>(value));
        }
        case FundamentalTypeKind::floatType:
        {
            return emitter.EmitFloat(static_cast<float>(value));
        }
        case FundamentalTypeKind::doubleType:
        {
            return emitter.EmitDouble(static_cast<double>(value));
        }
        }
        ThrowException("unsupported fundamental type", fullSpan, context);
    }
    void Write(Writer& writer) override
    {
        Value::Write(writer);
        writer.GetBinaryStreamWriter().Write(ToUnderlying(fundamentalTypeKind));
        writer.GetBinaryStreamWriter().Write(value);
    }
    void Read(Reader& reader) override
    {
        Value::Read(reader);
        fundamentalTypeKind = FundamentalTypeKind(reader.CurrentReader().ReadByte());
        bool unsupported = false;
        switch (fundamentalTypeKind)
        {
        case FundamentalTypeKind::boolType:
        {
            value = reader.CurrentReader().ReadBool();
            break;
        }
        case FundamentalTypeKind::charType:
        case FundamentalTypeKind::char8Type:
        case FundamentalTypeKind::signedCharType:
        case FundamentalTypeKind::unsignedCharType:
        {
            value = static_cast<T>(reader.CurrentReader().ReadByte());
            break;
        }
        case FundamentalTypeKind::char16Type:
        case FundamentalTypeKind::unsignedShortIntType:
        case FundamentalTypeKind::wcharType:
        {
            value = static_cast<T>(reader.CurrentReader().ReadUShort());
            break;
        }
        case FundamentalTypeKind::shortIntType:
        {
            value = static_cast<T>(reader.CurrentReader().ReadShort());
            break;
        }
        case FundamentalTypeKind::char32Type:
        case FundamentalTypeKind::unsignedIntType:
        case FundamentalTypeKind::unsignedLongIntType:
        {
            value = static_cast<T>(reader.CurrentReader().ReadUInt());
            break;
        }
        case FundamentalTypeKind::intType:
        case FundamentalTypeKind::longIntType:
        {
            value = static_cast<T>(reader.CurrentReader().ReadInt());
            break;
        }
        case FundamentalTypeKind::longLongIntType:
        {
            value = static_cast<T>(reader.CurrentReader().ReadLong());
            break;
        }
        case FundamentalTypeKind::unsignedLongLongIntType:
        {
            value = static_cast<T>(reader.CurrentReader().ReadULong());
            break;
        }
        case FundamentalTypeKind::floatType:
        {
            value = static_cast<T>(reader.CurrentReader().ReadFloat());
            break;
        }
        case FundamentalTypeKind::doubleType:
        {
            value = static_cast<T>(reader.CurrentReader().ReadDouble());
            break;
        }
        default:
        {
            unsupported = true;
            break;
        }
        }
        if (unsupported)
        {
            ThrowException("read of unsupported fundamental type");
        }
    }
    Value* Convert(ValueKind kind, Context* context) override
    {
        switch (fundamentalTypeKind)
        {
        case FundamentalTypeKind::charType:
        case FundamentalTypeKind::signedCharType:
        case FundamentalTypeKind::unsignedCharType:
        case FundamentalTypeKind::char8Type:
        case FundamentalTypeKind::char16Type:
        case FundamentalTypeKind::shortIntType:
        case FundamentalTypeKind::unsignedShortIntType:
        case FundamentalTypeKind::char32Type:
        case FundamentalTypeKind::wcharType:
        case FundamentalTypeKind::intType:
        case FundamentalTypeKind::unsignedIntType:
        case FundamentalTypeKind::longIntType:
        case FundamentalTypeKind::unsignedLongIntType:
        case FundamentalTypeKind::longLongIntType:
        case FundamentalTypeKind::unsignedLongLongIntType:
        case FundamentalTypeKind::floatType:
        case FundamentalTypeKind::doubleType:
        {
            switch (kind)
            {
            case ValueKind::boolValue:
            {
                return value ? context->GetEvaluationContext()->GetBoolValue(true) : context->GetEvaluationContext()->GetBoolValue(false);
            }
            case ValueKind::byteValue:
            case ValueKind::sbyteValue:
            case ValueKind::shortValue:
            case ValueKind::ushortValue:
            case ValueKind::intValue:
            case ValueKind::uintValue:
            case ValueKind::longValue:
            case ValueKind::ulongValue:
            {
                return context->GetEvaluationContext()->GetIntegerValue(static_cast<std::uint64_t>(value), GetValueType(kind, context), context);
            }
            case ValueKind::floatValue:
            case ValueKind::doubleValue:
            {
                return context->GetEvaluationContext()->GetFloatingValue(static_cast<double>(value), GetValueType(kind, context), context);
            }
            }
            break;
        }
        }
        return this;
    }
    BoolValue* ToBoolValue(Context* context) override
    {
        switch (fundamentalTypeKind)
        {
        case FundamentalTypeKind::boolType:
        case FundamentalTypeKind::charType:
        case FundamentalTypeKind::signedCharType:
        case FundamentalTypeKind::unsignedCharType:
        case FundamentalTypeKind::char8Type:
        case FundamentalTypeKind::char16Type:
        case FundamentalTypeKind::shortIntType:
        case FundamentalTypeKind::unsignedShortIntType:
        case FundamentalTypeKind::char32Type:
        case FundamentalTypeKind::wcharType:
        case FundamentalTypeKind::intType:
        case FundamentalTypeKind::unsignedIntType:
        case FundamentalTypeKind::longIntType:
        case FundamentalTypeKind::unsignedLongIntType:
        case FundamentalTypeKind::longLongIntType:
        case FundamentalTypeKind::unsignedLongLongIntType:
        case FundamentalTypeKind::floatType:
        case FundamentalTypeKind::doubleType:
        case FundamentalTypeKind::longDoubleType:
        {
            return value ? context->GetEvaluationContext()->GetBoolValue(true) : context->GetEvaluationContext()->GetBoolValue(false);
        }
        }
        return context->GetEvaluationContext()->GetBoolValue(false);
    }

    std::string Val() const override
    {
        return std::to_string(value);
    }
    std::uint64_t GetIntegerValue() const noexcept override
    {
        return static_cast<std::uint64_t>(value);
    }
    double GetFloatingValue() const noexcept override
    {
        return static_cast<double>(value);
    }
    Value* Clone(Context* context) const override
    {
        Value* clone = new FundamentalTypeValue<T>(GetModule(), context->GetNextSymbolId(Kind()), fundamentalTypeKind, value, context);
        context->GetEvaluationContext()->AddValue(clone);
        return clone;
    }
private:
    FundamentalTypeKind fundamentalTypeKind;
    T value;
};

class NullPtrValue : public Value
{
public:
    NullPtrValue(Module* module_, SymbolId id_);
    NullPtrValue(Module* module_, Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return "nullptr"; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    Value* Clone(Context* context) const override;
};

class StringValue : public Value
{
public:
    StringValue(Module* module_, SymbolId id_);
    StringValue(Module* module_, const std::string& value_, Context* context);
    inline const std::string& GetValue() const { return value; }
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return value; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    Value* Clone(Context* context) const override;
private:
    std::string value;
};

class SymbolValue : public Value
{
public:
    SymbolValue(Module* module_, SymbolId id_);
    SymbolValue(Module* module_, Symbol* symbol_, Context* context);
    Symbol* GetSymbol(Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return symbol->Name(); }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    bool IsComplete() const noexcept override { return false; }
private:
    Symbol* symbol;
    SymbolId symbolId;
};

class InvokeValue : public Value
{
public:
    InvokeValue(Module* module_, SymbolId id_);
    InvokeValue(Module* module_, Value* subject_, const std::vector<Value*>& arguments_, Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    Value* Subject(Context* context);
    const std::vector<Value*>& Arguments() const { return arguments; }
    std::string Val() const override { return subject->Val(); }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    bool IsComplete() const noexcept override { return false; }
private:
    Value* subject;
    std::vector<Value*> arguments;
    SymbolId subjectId;
};

struct ArrayValueHeader
{
    ArrayValueHeader();
    void Write(Writer& writer);
    void Read(Reader& reader);
    FileOffset bodyOffset;
    Length bodyLength;
};

class ArrayValue : public Value
{
public:
    ArrayValue(Module* module_, SymbolId id_);
    ArrayValue(Module* module_, Context* context);
    void AddElementValue(Value* elementValue);
    Cardinality Count();
    Value* GetElementValue(Index index, Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return "<array>"; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    ArrayValueHeader header;
    std::vector<Value*> elementValues;
    std::vector<SymbolId> elementValueIds;
};

struct StructureValueHeader
{
    StructureValueHeader();
    void Write(Writer& writer);
    void Read(Reader& reader);
    FileOffset bodyOffset;
    Length bodyLength;
};

class StructureValue : public Value
{
public:
    StructureValue(Module* module_, SymbolId id_);
    StructureValue(Module* module_, Context* context);
    void AddFieldValue(Value* fieldValue);
    Cardinality Count();
    Value* GetFieldValue(Index index, Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return "<structure>"; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    StructureValueHeader header;
    std::vector<Value*> fieldValues;
    std::vector<SymbolId> fieldValueIds;
};

class FunctionGroupValue : public Value
{
public:
    FunctionGroupValue(Module* module_, SymbolId symbolId_, FunctionGroupSymbol* functionGroup_, FunctionSymbol* fn_);
    Value* Convert(ValueKind kind, Context* context) override { return nullptr; }
    BoolValue* ToBoolValue(Context* context) override { return nullptr; }
    std::string Val() const override { return std::string(); }
    inline FunctionGroupSymbol* GetFunctionGroup() const noexcept { return functionGroup; }
    inline FunctionSymbol* Fn() const noexcept { return fn; }
    bool IsSerializableValue() const noexcept override { return false; }
    bool IsComplete() const noexcept override { return false; }
private:
    FunctionGroupSymbol* functionGroup;
    FunctionSymbol* fn;
};

class TypeValue : public Value
{
public:
    TypeValue(Module* module_, SymbolId symbolId_, TypeSymbol* type_);
    Value* Convert(ValueKind kind, Context* context) override { return nullptr; }
    BoolValue* ToBoolValue(Context* context) override { return nullptr; }
    std::string Val() const override { return std::string(); }
    inline TypeSymbol* GetType() const noexcept { return type; }
    bool IsSerializableValue() const noexcept override { return false; }
    bool IsComplete() const noexcept override { return false; }
private:
    TypeSymbol* type;
};

} // namespace otava::symbols
