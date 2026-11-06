// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.value;

import otava.symbols.concrete_value;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.fundamental_type_kind;
import otava.symbols.modules;
import otava.symbols.type_compare;

namespace otava::symbols {

TypeSymbol* GetValueType(ValueKind valueKind, Context* context)
{
    switch (valueKind)
    {
    case ValueKind::boolValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::boolType, context);
    case ValueKind::byteValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::unsignedCharType, context);
    case ValueKind::sbyteValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::signedCharType, context);
    case ValueKind::shortValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::shortIntType, context);
    case ValueKind::ushortValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::unsignedShortIntType, context);
    case ValueKind::intValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::intType, context);
    case ValueKind::uintValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::unsignedIntType, context);
    case ValueKind::longValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::longLongIntType, context);
    case ValueKind::ulongValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::unsignedLongLongIntType, context);
    case ValueKind::floatValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::floatType, context);
    case ValueKind::doubleValue: return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::doubleType, context);
    }
    return nullptr;
}

std::string ValueKindStr(ValueKind kind)
{
    switch (kind)
    {
    case ValueKind::boolValue: return "boolValue";
    case ValueKind::byteValue: return "byteValue";
    case ValueKind::sbyteValue: return "sbyteValue";
    case ValueKind::shortValue: return "shortValue";
    case ValueKind::ushortValue: return "ushortValue";
    case ValueKind::intValue: return "intValue";
    case ValueKind::uintValue: return "uintValue";
    case ValueKind::longValue: return "longValue";
    case ValueKind::ulongValue: return "ulongValue";
    case ValueKind::floatValue: return "floatValue";
    case ValueKind::doubleValue: return "doubleValue";
    case ValueKind::nullPtrValue: return "nullPtrValue";
    case ValueKind::stringValue: return "stringValue";
    case ValueKind::symbolValue: return "symbolValue";
    case ValueKind::invokeValue: return "invokeValue";
    case ValueKind::arrayValue: return "arrayValue";
    case ValueKind::structureValue: return "structureValue";
    }
    return "<value>";
}

ValueKind CommonValueKind(ValueKind left, ValueKind right) noexcept
{
    switch (left)
    {
    case ValueKind::boolValue:
    {
        return ValueKind::boolValue;
    }
    case ValueKind::byteValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::byteValue: return ValueKind::byteValue;
        case ValueKind::sbyteValue: return ValueKind::shortValue;
        case ValueKind::shortValue: return ValueKind::shortValue;
        case ValueKind::intValue: return ValueKind::intValue;
        case ValueKind::uintValue: return ValueKind::uintValue;
        case ValueKind::longValue: return ValueKind::longValue;
        case ValueKind::ulongValue: return ValueKind::ulongValue;
        case ValueKind::floatValue: return ValueKind::floatValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    case ValueKind::sbyteValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::sbyteValue: return ValueKind::sbyteValue;
        case ValueKind::byteValue: return ValueKind::shortValue;
        case ValueKind::shortValue: return ValueKind::shortValue;
        case ValueKind::intValue: return ValueKind::intValue;
        case ValueKind::uintValue: return ValueKind::longValue;
        case ValueKind::longValue: return ValueKind::longValue;
        case ValueKind::ulongValue: return ValueKind::ulongValue;
        case ValueKind::floatValue: return ValueKind::floatValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    case ValueKind::shortValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::byteValue: return ValueKind::shortValue;
        case ValueKind::sbyteValue: return ValueKind::shortValue;
        case ValueKind::shortValue: return ValueKind::shortValue;
        case ValueKind::ushortValue: return ValueKind::intValue;
        case ValueKind::intValue: return ValueKind::intValue;
        case ValueKind::uintValue: return ValueKind::longValue;
        case ValueKind::longValue: return ValueKind::longValue;
        case ValueKind::ulongValue: return ValueKind::ulongValue;
        case ValueKind::floatValue: return ValueKind::floatValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    case ValueKind::ushortValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::byteValue: return ValueKind::ushortValue;
        case ValueKind::sbyteValue: return ValueKind::intValue;
        case ValueKind::shortValue: return ValueKind::intValue;
        case ValueKind::ushortValue: return ValueKind::ushortValue;
        case ValueKind::intValue: return ValueKind::intValue;
        case ValueKind::uintValue: return ValueKind::uintValue;
        case ValueKind::longValue: return ValueKind::longValue;
        case ValueKind::ulongValue: return ValueKind::ulongValue;
        case ValueKind::floatValue: return ValueKind::floatValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    case ValueKind::intValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::byteValue: return ValueKind::intValue;
        case ValueKind::sbyteValue: return ValueKind::intValue;
        case ValueKind::shortValue: return ValueKind::intValue;
        case ValueKind::ushortValue: return ValueKind::intValue;
        case ValueKind::intValue: return ValueKind::intValue;
        case ValueKind::uintValue: return ValueKind::longValue;
        case ValueKind::longValue: return ValueKind::longValue;
        case ValueKind::ulongValue: return ValueKind::ulongValue;
        case ValueKind::floatValue: return ValueKind::floatValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    case ValueKind::uintValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::byteValue: return ValueKind::uintValue;
        case ValueKind::sbyteValue: return ValueKind::longValue;
        case ValueKind::shortValue: return ValueKind::longValue;
        case ValueKind::ushortValue: return ValueKind::uintValue;
        case ValueKind::intValue: return ValueKind::longValue;
        case ValueKind::uintValue: return ValueKind::uintValue;
        case ValueKind::longValue: return ValueKind::longValue;
        case ValueKind::ulongValue: return ValueKind::ulongValue;
        case ValueKind::floatValue: return ValueKind::floatValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    case ValueKind::longValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::byteValue: return ValueKind::longValue;
        case ValueKind::sbyteValue: return ValueKind::longValue;
        case ValueKind::shortValue: return ValueKind::longValue;
        case ValueKind::ushortValue: return ValueKind::longValue;
        case ValueKind::intValue: return ValueKind::longValue;
        case ValueKind::uintValue: return ValueKind::longValue;
        case ValueKind::longValue: return ValueKind::longValue;
        case ValueKind::ulongValue: return ValueKind::ulongValue;
        case ValueKind::floatValue: return ValueKind::floatValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    case ValueKind::ulongValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::byteValue: return ValueKind::ulongValue;
        case ValueKind::sbyteValue: return ValueKind::longValue;
        case ValueKind::shortValue: return ValueKind::longValue;
        case ValueKind::ushortValue: return ValueKind::ulongValue;
        case ValueKind::intValue: return ValueKind::longValue;
        case ValueKind::uintValue: return ValueKind::ulongValue;
        case ValueKind::longValue: return ValueKind::longValue;
        case ValueKind::ulongValue: return ValueKind::ulongValue;
        case ValueKind::floatValue: return ValueKind::floatValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    case ValueKind::floatValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::byteValue: return ValueKind::floatValue;
        case ValueKind::sbyteValue: return ValueKind::floatValue;
        case ValueKind::shortValue: return ValueKind::floatValue;
        case ValueKind::ushortValue: return ValueKind::floatValue;
        case ValueKind::intValue: return ValueKind::floatValue;
        case ValueKind::uintValue: return ValueKind::floatValue;
        case ValueKind::longValue: return ValueKind::floatValue;
        case ValueKind::ulongValue: return ValueKind::floatValue;
        case ValueKind::floatValue: return ValueKind::floatValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    case ValueKind::doubleValue:
    {
        switch (right)
        {
        case ValueKind::boolValue: return ValueKind::boolValue;
        case ValueKind::byteValue: return ValueKind::doubleValue;
        case ValueKind::sbyteValue: return ValueKind::doubleValue;
        case ValueKind::shortValue: return ValueKind::doubleValue;
        case ValueKind::ushortValue: return ValueKind::doubleValue;
        case ValueKind::intValue: return ValueKind::doubleValue;
        case ValueKind::uintValue: return ValueKind::doubleValue;
        case ValueKind::longValue: return ValueKind::doubleValue;
        case ValueKind::ulongValue: return ValueKind::doubleValue;
        case ValueKind::floatValue: return ValueKind::doubleValue;
        case ValueKind::doubleValue: return ValueKind::doubleValue;
        }
        break;
    }
    }
    return ValueKind::none;
}

Value::Value(Module* module_, SymbolId id_) : Symbol(module_, id_), type(nullptr), typeId(zeroSymbolId), interfaceType(nullptr)
{
}

Value::Value(Module* module_, SymbolId id_, const std::string& rep_) : Symbol(module_, id_, rep_), type(nullptr), typeId(zeroSymbolId), interfaceType(nullptr)
{
}

TypeSymbol* Value::GetInterfaceType(Context* context) const
{
    if (interfaceType)
    {
        return interfaceType;
    }
    return GetType(context);
}

otava::intermediate::Value* Value::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot evaluate statically", fullSpan, context);
    return nullptr;
}

ValueKind Value::GetValueKind() const noexcept
{
    switch (Kind())
    {
    case SymbolKind::boolValueSymbol:
    {
        return ValueKind::boolValue;
    }
    case SymbolKind::byteValueSymbol:
    {
        return ValueKind::byteValue;
    }
    case SymbolKind::sbyteValueSymbol:
    {
        return ValueKind::sbyteValue;
    }
    case SymbolKind::shortValueSymbol:
    {
        return ValueKind::shortValue;
    }
    case SymbolKind::ushortValueSymbol:
    {
        return ValueKind::ushortValue;
    }
    case SymbolKind::intValueSymbol:
    {
        return ValueKind::intValue;
    }
    case SymbolKind::uintValueSymbol:
    {
        return ValueKind::uintValue;
    }
    case SymbolKind::longValueSymbol:
    {
        return ValueKind::longValue;
    }
    case SymbolKind::ulongValueSymbol:
    {
        return ValueKind::ulongValue;
    }
    case SymbolKind::floatValueSymbol:
    {
        return ValueKind::floatValue;
    }
    case SymbolKind::doubleValueSymbol:
    {
        return ValueKind::doubleValue;
    }
    case SymbolKind::nullPtrValueSymbol:
    {
        return ValueKind::nullPtrValue;
    }
    case SymbolKind::stringValueSymbol:
    {
        return ValueKind::stringValue;
    }
    case SymbolKind::symbolValueSymbol:
    {
        return ValueKind::symbolValue;
    }
    case SymbolKind::invokeValueSymbol:
    {
        return ValueKind::invokeValue;
    }
    }
    return ValueKind::none;
}

bool Value::IsIntegerValue() const noexcept
{
    switch (GetValueKind())
    {
    case ValueKind::byteValue:
    case ValueKind::sbyteValue:
    case ValueKind::shortValue:
    case ValueKind::ushortValue:
    case ValueKind::intValue:
    case ValueKind::uintValue:
    case ValueKind::longValue:
    case ValueKind::ulongValue:
    {
        return true;
    }
    }
    return false;
}

bool Value::IsFloatingValue() const noexcept
{
    switch (GetValueKind())
    {
    case ValueKind::floatValue:
    case ValueKind::doubleValue:
    {
        return true;
    }
    }
    return false;
}

TypeSymbol* Value::GetType(Context* context) const
{
    if (type)
    {
        return type;
    }
    if (IsReadOnly() && typeId != zeroSymbolId)
    {
        type = GetModule()->GetSymbolTable()->GetTypeSymbol(typeId, context);
        if (!type)
        {
            ThrowException("value type id " + std::to_string(ToUnderlying(typeId)) + " not found");
        }
    }
    return type;
}

void Value::SetType(TypeSymbol* type_, Context* context) noexcept
{
    type = type_;
    if (type && type->GetModule() != GetModule())
    {
        GetModule()->GetSymbolTable()->AddImportedSymbol(type->Id(), type->GetModule());
    }
}

void Value::Write(Writer& writer)
{
    Symbol::Write(writer);
    if (type)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(type->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
}

void Value::Read(Reader& reader)
{
    Symbol::Read(reader);
    typeId = SymbolId(reader.CurrentReader().ReadULong());
}

Value* Value::Clone(Context* context) const
{
    ThrowException("cannot clone this kind of value");
    return nullptr;
}

bool ValuesEqual(Value* left, Value* right, Context* context)
{
    if (!TypesEqual(left->GetType(context), right->GetType(context), context)) return false;
    ValueKind valueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    switch (valueKind)
    {
        case ValueKind::boolValue: 
        {
            BoolValue* leftBool = static_cast<BoolValue*>(left);
            BoolValue* rightBool = static_cast<BoolValue*>(right);
            return leftBool->GetValue() == rightBool->GetValue();
        }
        case ValueKind::byteValue:
        {
            FundamentalTypeValue<std::uint8_t>* leftByte = static_cast<FundamentalTypeValue<std::uint8_t>*>(left);
            FundamentalTypeValue<std::uint8_t>* rightByte = static_cast<FundamentalTypeValue<std::uint8_t>*>(right);
            return leftByte->GetValue() == rightByte->GetValue();
        }
        case ValueKind::sbyteValue:
        {
            FundamentalTypeValue<std::int8_t>* leftSByte = static_cast<FundamentalTypeValue<std::int8_t>*>(left);
            FundamentalTypeValue<std::int8_t>* rightSByte = static_cast<FundamentalTypeValue<std::int8_t>*>(right);
            return leftSByte->GetValue() == rightSByte->GetValue();
        }
        case ValueKind::shortValue:
        {
            FundamentalTypeValue<std::int16_t>* lefShort = static_cast<FundamentalTypeValue<std::int16_t>*>(left);
            FundamentalTypeValue<std::int16_t>* rightShort = static_cast<FundamentalTypeValue<std::int16_t>*>(right);
            return lefShort->GetValue() == rightShort->GetValue();
        }
        case ValueKind::ushortValue:
        {
            FundamentalTypeValue<std::uint16_t>* leftUShort = static_cast<FundamentalTypeValue<std::uint16_t>*>(left);
            FundamentalTypeValue<std::uint16_t>* rightUShort = static_cast<FundamentalTypeValue<std::uint16_t>*>(right);
            return leftUShort->GetValue() == rightUShort->GetValue();
        }
        case ValueKind::intValue:
        {
            FundamentalTypeValue<std::int32_t>* leftInt = static_cast<FundamentalTypeValue<std::int32_t>*>(left);
            FundamentalTypeValue<std::int32_t>* rightInt = static_cast<FundamentalTypeValue<std::int32_t>*>(right);
            return leftInt->GetValue() == rightInt->GetValue();
        }
        case ValueKind::uintValue:
        {
            FundamentalTypeValue<std::uint32_t>* leftUInt = static_cast<FundamentalTypeValue<std::uint32_t>*>(left);
            FundamentalTypeValue<std::uint32_t>* rightUInt = static_cast<FundamentalTypeValue<std::uint32_t>*>(right);
            return leftUInt->GetValue() == rightUInt->GetValue();
        }
        case ValueKind::longValue:
        {
            FundamentalTypeValue<std::int64_t>* leftLong = static_cast<FundamentalTypeValue<std::int64_t>*>(left);
            FundamentalTypeValue<std::int64_t>* rightLong = static_cast<FundamentalTypeValue<std::int64_t>*>(right);
            return leftLong->GetValue() == rightLong->GetValue();
        }
        case ValueKind::ulongValue:
        {
            FundamentalTypeValue<std::uint64_t>* leftULong = static_cast<FundamentalTypeValue<std::uint64_t>*>(left);
            FundamentalTypeValue<std::uint64_t>* rightULong = static_cast<FundamentalTypeValue<std::uint64_t>*>(right);
            return leftULong->GetValue() == rightULong->GetValue();
        }
        case ValueKind::floatValue:
        {
            FundamentalTypeValue<float>* leftFloat = static_cast<FundamentalTypeValue<float>*>(left);
            FundamentalTypeValue<float>* rightFloat = static_cast<FundamentalTypeValue<float>*>(right);
            return leftFloat->GetValue() == rightFloat->GetValue();
        }
        case ValueKind::doubleValue:
        {
            FundamentalTypeValue<double>* leftDouble = static_cast<FundamentalTypeValue<double>*>(left);
            FundamentalTypeValue<double>* rightDouble = static_cast<FundamentalTypeValue<double>*>(right);
            return leftDouble->GetValue() == rightDouble->GetValue();
        }
        case ValueKind::nullPtrValue:
        {
            return true;
        }
        case ValueKind::stringValue:
        {
            StringValue* leftString = static_cast<StringValue*>(left);
            StringValue* rightString = static_cast<StringValue*>(right);
            return leftString->GetValue() == rightString->GetValue();
        }
    }
    return false;
}

bool ValueLess(Value* left, Value* right, Context* context)
{
    if (!TypesEqual(left->GetType(context), right->GetType(context), context)) return false;
    ValueKind valueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    switch (valueKind)
    {
    case ValueKind::boolValue:
    {
        BoolValue* leftBool = static_cast<BoolValue*>(left);
        BoolValue* rightBool = static_cast<BoolValue*>(right);
        return std::int8_t(leftBool->GetValue()) < std::int8_t(rightBool->GetValue());
    }
    case ValueKind::byteValue:
    {
        FundamentalTypeValue<std::uint8_t>* leftByte = static_cast<FundamentalTypeValue<std::uint8_t>*>(left);
        FundamentalTypeValue<std::uint8_t>* rightByte = static_cast<FundamentalTypeValue<std::uint8_t>*>(right);
        return leftByte->GetValue() < rightByte->GetValue();
    }
    case ValueKind::sbyteValue:
    {
        FundamentalTypeValue<std::int8_t>* leftSByte = static_cast<FundamentalTypeValue<std::int8_t>*>(left);
        FundamentalTypeValue<std::int8_t>* rightSByte = static_cast<FundamentalTypeValue<std::int8_t>*>(right);
        return leftSByte->GetValue() < rightSByte->GetValue();
    }
    case ValueKind::shortValue:
    {
        FundamentalTypeValue<std::int16_t>* lefShort = static_cast<FundamentalTypeValue<std::int16_t>*>(left);
        FundamentalTypeValue<std::int16_t>* rightShort = static_cast<FundamentalTypeValue<std::int16_t>*>(right);
        return lefShort->GetValue() < rightShort->GetValue();
    }
    case ValueKind::ushortValue:
    {
        FundamentalTypeValue<std::uint16_t>* leftUShort = static_cast<FundamentalTypeValue<std::uint16_t>*>(left);
        FundamentalTypeValue<std::uint16_t>* rightUShort = static_cast<FundamentalTypeValue<std::uint16_t>*>(right);
        return leftUShort->GetValue() < rightUShort->GetValue();
    }
    case ValueKind::intValue:
    {
        FundamentalTypeValue<std::int32_t>* leftInt = static_cast<FundamentalTypeValue<std::int32_t>*>(left);
        FundamentalTypeValue<std::int32_t>* rightInt = static_cast<FundamentalTypeValue<std::int32_t>*>(right);
        return leftInt->GetValue() < rightInt->GetValue();
    }
    case ValueKind::uintValue:
    {
        FundamentalTypeValue<std::uint32_t>* leftUInt = static_cast<FundamentalTypeValue<std::uint32_t>*>(left);
        FundamentalTypeValue<std::uint32_t>* rightUInt = static_cast<FundamentalTypeValue<std::uint32_t>*>(right);
        return leftUInt->GetValue() < rightUInt->GetValue();
    }
    case ValueKind::longValue:
    {
        FundamentalTypeValue<std::int64_t>* leftLong = static_cast<FundamentalTypeValue<std::int64_t>*>(left);
        FundamentalTypeValue<std::int64_t>* rightLong = static_cast<FundamentalTypeValue<std::int64_t>*>(right);
        return leftLong->GetValue() < rightLong->GetValue();
    }
    case ValueKind::ulongValue:
    {
        FundamentalTypeValue<std::uint64_t>* leftULong = static_cast<FundamentalTypeValue<std::uint64_t>*>(left);
        FundamentalTypeValue<std::uint64_t>* rightULong = static_cast<FundamentalTypeValue<std::uint64_t>*>(right);
        return leftULong->GetValue() < rightULong->GetValue();
    }
    case ValueKind::floatValue:
    {
        FundamentalTypeValue<float>* leftFloat = static_cast<FundamentalTypeValue<float>*>(left);
        FundamentalTypeValue<float>* rightFloat = static_cast<FundamentalTypeValue<float>*>(right);
        return leftFloat->GetValue() < rightFloat->GetValue();
    }
    case ValueKind::doubleValue:
    {
        FundamentalTypeValue<double>* leftDouble = static_cast<FundamentalTypeValue<double>*>(left);
        FundamentalTypeValue<double>* rightDouble = static_cast<FundamentalTypeValue<double>*>(right);
        return leftDouble->GetValue() < rightDouble->GetValue();
    }
    case ValueKind::nullPtrValue:
    {
        return false;
    }
    case ValueKind::stringValue:
    {
        StringValue* leftString = static_cast<StringValue*>(left);
        StringValue* rightString = static_cast<StringValue*>(right);
        return leftString->GetValue() < rightString->GetValue();
    }
    }
    return false;
}

BoolValue::BoolValue(Module* module_, SymbolId id_) : Value(module_, id_), value(false)
{
}

BoolValue::BoolValue(Module* module_, bool value_, Context* context) : Value(module_, context->GetNextSymbolId(SymbolKind::boolValueSymbol)), value(value_)
{
}

Value* BoolValue::Convert(ValueKind kind, Context* context)
{
    switch (kind)
    {
    case ValueKind::boolValue: return this;
    case ValueKind::byteValue:
    case ValueKind::sbyteValue:
    case ValueKind::shortValue:
    case ValueKind::ushortValue:
    case ValueKind::intValue:
    case ValueKind::uintValue:
    case ValueKind::longValue:
    case ValueKind::ulongValue:
    {
        if (GetValue())
        {
            return context->GetEvaluationContext()->GetIntegerValue(1, GetValueType(kind, context), context);
        }
        else
        {
            return context->GetEvaluationContext()->GetIntegerValue(0, GetValueType(kind, context), context);
        }
    }
    case ValueKind::floatValue:
    case ValueKind::doubleValue:
    {
        if (GetValue())
        {
            return context->GetEvaluationContext()->GetFloatingValue(1.0, GetValueType(kind, context), context);
        }
        else
        {
            return context->GetEvaluationContext()->GetFloatingValue(0.0, GetValueType(kind, context), context);
        }
    }
    }
    return this;
}

Value* BoolValue::Clone(Context* context) const
{
    Value* clone = new BoolValue(GetModule(), GetValue(), context);
    context->GetEvaluationContext()->AddValue(clone);
    return clone;
}

BoolValue* BoolValue::ToBoolValue(Context* context)
{
    return this;
}

std::string BoolValue::Val() const
{
    return GetValue() ? "true" : "false";
}

otava::intermediate::Value* BoolValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    return emitter.EmitBool(value);
}

void BoolValue::Write(Writer& writer)
{
    Value::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void BoolValue::Read(Reader& reader)
{
    Value::Read(reader);
    value = reader.CurrentReader().ReadBool();
}

} // namespace otava::symbols
