// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.value;

import otava.intermediate.code;
import otava.intermediate.context;
import otava.intermediate.util;
import otava.intermediate.visitor;

namespace otava::intermediate {

std::string GetValueKindStr(ValueKind kind)
{
    switch (kind)
    {
    case ValueKind::boolValue: return "boolValue";
    case ValueKind::sbyteValue: return "sbyteValue";
    case ValueKind::byteValue: return "byteValue";
    case ValueKind::shortValue: return "shortValue";
    case ValueKind::ushortValue: return "ushortValue";
    case ValueKind::intValue: return "intValue";
    case ValueKind::uintValue: return "uintValue";
    case ValueKind::longValue: return "longValue";
    case ValueKind::ulongValue: return "ulongValue";
    case ValueKind::floatValue: return "floatValue";
    case ValueKind::doubleValue: return "doubleValue";
    case ValueKind::nullValue: return "nullValue";
    case ValueKind::addressValue: return "addressValue";
    case ValueKind::arrayValue: return "arrayValue";
    case ValueKind::structureValue: return "structureValue";
    case ValueKind::stringValue: return "stringValue";
    case ValueKind::stringArrayValue: return "stringArrayValue";
    case ValueKind::conversionValue: return "conversionValue";
    case ValueKind::clsIdValue: return "clsIdValue";
    case ValueKind::symbolValue: return "symbolValue";
    case ValueKind::globalVariable: return "globalVariable";
    case ValueKind::regValue: return "regValue";
    case ValueKind::instruction: return "instruction";
    }
    return "<valueKind>";
}

Value::Value(const soul::ast::Span& span_, ValueKind kind_, Type* type_) noexcept : span(span_), kind(kind_), type(type_)
{
}

Value::~Value()
{
}

void Value::Accept(Visitor& visitor)
{
}

Value* Value::Clone(CloneContext& cloneContext) const
{
    return const_cast<Value*>(this);
}

bool Value::IsTrue() const noexcept
{
    if (kind == ValueKind::boolValue)
    {
        const BoolValue* boolValue = static_cast<const BoolValue*>(this);
        return boolValue->GetValue() == true;
    }
    return false;
}

bool Value::IsFalse() const noexcept
{
    if (kind == ValueKind::boolValue)
    {
        const BoolValue* boolValue = static_cast<const BoolValue*>(this);
        return boolValue->GetValue() == false;
    }
    return false;
}

bool Value::IsIntegerValue() const noexcept
{
    switch (kind)
    {
    case ValueKind::sbyteValue:
    case ValueKind::byteValue:
    case ValueKind::shortValue:
    case ValueKind::ushortValue:
    case ValueKind::intValue:
    case ValueKind::uintValue:
    case ValueKind::longValue:
    case ValueKind::ulongValue:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

bool Value::IsFloatingPointValue() const noexcept
{
    return kind == ValueKind::floatValue || kind == ValueKind::doubleValue;
}

std::int64_t Value::GetIntegerValue() const noexcept
{
    switch (kind)
    {
    case ValueKind::sbyteValue:
    {
        const SByteValue* sbyteValue = static_cast<const SByteValue*>(this);
        return sbyteValue->GetValue();
    }
    case ValueKind::byteValue:
    {
        const ByteValue* byteValue = static_cast<const ByteValue*>(this);
        return byteValue->GetValue();
    }
    case ValueKind::shortValue:
    {
        const ShortValue* shortValue = static_cast<const ShortValue*>(this);
        return shortValue->GetValue();
    }
    case ValueKind::ushortValue:
    {
        const UShortValue* ushortValue = static_cast<const UShortValue*>(this);
        return ushortValue->GetValue();
    }
    case ValueKind::intValue:
    {
        const IntValue* intValue = static_cast<const IntValue*>(this);
        return intValue->GetValue();
    }
    case ValueKind::uintValue:
    {
        const UIntValue* uintValue = static_cast<const UIntValue*>(this);
        return uintValue->GetValue();
    }
    case ValueKind::longValue:
    {
        const LongValue* longValue = static_cast<const LongValue*>(this);
        return longValue->GetValue();
    }
    case ValueKind::ulongValue:
    {
        const ULongValue* ulongValue = static_cast<const ULongValue*>(this);
        return static_cast<std::int64_t>(ulongValue->GetValue());
    }
    default:
    {
        return -1;
    }
    }
}

std::string Value::KindStr() const
{
    return GetValueKindStr(kind);
}

Instruction* Value::GetInstruction() const noexcept
{
    if (IsRegValue())
    {
        const RegValue* regValue = static_cast<const RegValue*>(this);
        return regValue->Inst();
    }
    return nullptr;
}

Value* Value::Log2(IntermediateContext* context) const 
{ 
    return nullptr; 
}

Value* Value::ModPowerOfTwo(IntermediateContext* context) const 
{ 
    return nullptr; 
}

BoolValue::BoolValue(bool value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::boolValue, type_), value(value_)
{
}

void BoolValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string BoolValue::ToString() const
{
    return value ? "true" : "false";
}

SByteValue::SByteValue(std::int8_t value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::sbyteValue, type_), value(value_)
{
}

void SByteValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string SByteValue::ToString() const
{
    return std::to_string(value);
}

Value* SByteValue::Log2(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::int8_t>(shift), context->GetTypes());
        }
    }
    return nullptr;
}

Value* SByteValue::ModPowerOfTwo(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::int8_t>(value - 1), context->GetTypes());
        }
    }
    return nullptr;
}

ByteValue::ByteValue(std::uint8_t value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::byteValue, type_), value(value_)
{
}

void ByteValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ByteValue::ToString() const
{
    return std::to_string(value);
}

Value* ByteValue::Log2(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::uint8_t>(shift), context->GetTypes());
        }
    }
    return nullptr;
}

Value* ByteValue::ModPowerOfTwo(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::uint8_t>(value - 1), context->GetTypes());
        }
    }
    return nullptr;
}

ShortValue::ShortValue(std::int16_t value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::shortValue, type_), value(value_)
{
}

void ShortValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ShortValue::ToString() const
{
    return std::to_string(value);
}

Value* ShortValue::Log2(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::int16_t>(shift), context->GetTypes());
        }
    }
    return nullptr;
}

Value* ShortValue::ModPowerOfTwo(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::int16_t>(value - 1), context->GetTypes());
        }
    }
    return nullptr;
}

UShortValue::UShortValue(std::uint16_t value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::ushortValue, type_), value(value_)
{
}

void UShortValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string UShortValue::ToString() const
{
    return std::to_string(value);
}

Value* UShortValue::Log2(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::uint16_t>(shift), context->GetTypes());
        }
    }
    return nullptr;
}

Value* UShortValue::ModPowerOfTwo(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::uint16_t>(value - 1), context->GetTypes());
        }
    }
    return nullptr;
}

IntValue::IntValue(std::int32_t value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::intValue, type_), value(value_)
{
}

void IntValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string IntValue::ToString() const
{
    return std::to_string(value);
}

Value* IntValue::Log2(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::int32_t>(shift), context->GetTypes());
        }
    }
    return nullptr;
}

Value* IntValue::ModPowerOfTwo(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::int32_t>(value - 1), context->GetTypes());
        }
    }
    return nullptr;
}

UIntValue::UIntValue(std::uint32_t value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::uintValue, type_), value(value_)
{
}

void UIntValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string UIntValue::ToString() const
{
    return std::to_string(value);
}

Value* UIntValue::Log2(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::uint32_t>(shift), context->GetTypes());
        }
    }
    return nullptr;
}

Value* UIntValue::ModPowerOfTwo(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::uint32_t>(value - 1), context->GetTypes());
        }
    }
    return nullptr;
}

LongValue::LongValue(std::int64_t value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::longValue, type_), value(value_)
{
}

void LongValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string LongValue::ToString() const
{
    return std::to_string(value);
}

Value* LongValue::Log2(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::int64_t>(shift), context->GetTypes());
        }
    }
    return nullptr;
}

Value* LongValue::ModPowerOfTwo(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::int64_t>(value - 1), context->GetTypes());
        }
    }
    return nullptr;
}

ULongValue::ULongValue(std::uint64_t value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::ulongValue, type_), value(value_)
{
}

void ULongValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ULongValue::ToString() const
{
    return std::to_string(value);
}

Value* ULongValue::Log2(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::uint64_t>(shift), context->GetTypes());
        }
    }
    return nullptr;
}

Value* ULongValue::ModPowerOfTwo(IntermediateContext* context) const
{
    if (value > 0)
    {
        int shift = 0;
        if (IsPowerOfTwo(static_cast<std::uint64_t>(value), shift))
        {
            return context->GetData().MakeValue(static_cast<std::uint64_t>(value - 1), context->GetTypes());
        }
    }
    return nullptr;
}

FloatValue::FloatValue(float value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::floatValue, type_), value(value_)
{
}

void FloatValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string FloatValue::ToString() const
{
    return std::to_string(value);
}

DoubleValue::DoubleValue(double value_, Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::doubleValue, type_), value(value_)
{
}

void DoubleValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string DoubleValue::ToString() const
{
    return std::to_string(value);
}

NullValue::NullValue(Type* type_) noexcept : Value(soul::ast::Span(), ValueKind::nullValue, type_)
{
}

void NullValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string NullValue::ToString() const
{
    return "null";
}

} // otava::intermediate
