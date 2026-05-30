// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.data;

import otava.intermediate.context;
import otava.intermediate.error;
import otava.intermediate.visitor;
import otava.intermediate.code;
import otava.intermediate.util;
import util.text_util;

namespace otava::intermediate {

CloneContext::CloneContext() : context(nullptr), currentFunction(nullptr)
{
}

void CloneContext::MapInstruction(Instruction* inst, Instruction* clone)
{
    instMap[inst] = clone;
}

Instruction* CloneContext::GetMappedInstruction(Instruction* inst) const noexcept
{
    auto it = instMap.find(inst);
    if (it != instMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void CloneContext::AddUnmappedInstruction(Instruction* inst, RegValue* regValue)
{
    unmappedInstructions.insert(std::make_pair(inst, regValue));
}

BasicBlock* CloneContext::GetMappedBasicBlock(BasicBlock* bb) const noexcept
{
    auto it = bbMap.find(bb);
    if (it != bbMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void CloneContext::MapBasicBlock(BasicBlock* bb, BasicBlock* clone)
{
    bbMap[bb] = clone;
}

const char* valueKindStr[]
{
    "boolValue", "sbyteValue", "byteValue", "shortValue", "ushortValue", "intValue", "uintValue", "longValue", "ulongValue", "floatValue", "doubleValue", "nullValue",
    "addressValue",
    "arrayValue", "structureValue", "stringValue", "stringArrayValue", "conversionValue", "clsIdValue", "symbolValue",
    "globalVariable",
    "regValue",
    "instruction"
};

Value::Value(const soul::ast::Span& span_, ValueKind kind_, Type* type_) noexcept : span(span_), kind(kind_), type(type_)
{
}

Value::~Value()
{
}

void Value::Accept(Visitor& visitor)
{
}

bool Value::IsTrue() const noexcept
{
    return kind == ValueKind::boolValue && static_cast<const BoolValue*>(this)->GetValue() == true;
}

bool Value::IsFalse() const noexcept
{
    return kind == ValueKind::boolValue && static_cast<const BoolValue*>(this)->GetValue() == false;
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
    return valueKindStr[static_cast<int>(kind)];
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

AddressValue::AddressValue(const soul::ast::Span& span_, const std::string& id_, Type* type) noexcept :
    Value(span_, ValueKind::addressValue, type), id(id_), globalVariable(nullptr)
{
}

void AddressValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string AddressValue::ToString() const
{
    return globalVariable->Name();
}

ArrayValue::ArrayValue(const soul::ast::Span& span_, const std::vector<Value*>& elements_, ArrayType* type_) : Value(span_, ValueKind::arrayValue, type_), elements(elements_)
{
}

void ArrayValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ArrayValue::ToString() const
{
    std::string s = "[ ";
    bool first = true;
    for (Value* element : elements)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            s.append(", ");
        }
        if (element->IsAggregateValue())
        {
            s.append(element->ToString());
        }
        else
        {
            s.append(element->GetType()->Name()).append(" ").append(element->ToString());
        }
    }
    s.append(" ]");
    return s;
}

StructureValue::StructureValue(const soul::ast::Span& span_, const std::vector<Value*>& fieldValues_, StructureType* type_) :
    Value(span_, ValueKind::structureValue, type_), fieldValues(fieldValues_)
{
}

void StructureValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string StructureValue::ToString() const
{
    std::string s = "{ ";
    bool first = true;
    for (Value* field : fieldValues)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            s.append(", ");
        }
        if (field->IsAggregateValue())
        {
            s.append(field->ToString());
        }
        else
        {
            s.append(field->GetType()->Name()).append(" ").append(field->ToString());
        }
    }
    s.append(" }");
    return s;
}

StringValue::StringValue(const soul::ast::Span& span_, const std::string& value_) : Value(span_, ValueKind::stringValue, nullptr), value(value_)
{
}

void StringValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string StringValue::ToString() const
{
    std::string s("\"");
    for (char c : value)
    {
        if (c == '"')
        {
            std::string hex = util::ToHexString(static_cast<std::uint8_t>(c));
            s.append("\\").append(hex);
        }
        else if (c == '\\')
        {
            std::string hex = util::ToHexString(static_cast<std::uint8_t>(c));
            s.append("\\").append(hex);
        }
        else if (static_cast<std::uint8_t>(c) >= 32u && static_cast<std::uint8_t>(c) < 127u)
        {
            s.append(1, c);
        }
        else
        {
            std::string hex = util::ToHexString(static_cast<std::uint8_t>(c));
            s.append("\\").append(hex);
        }
    }
    std::string hex = util::ToHexString(static_cast<std::uint8_t>(0));
    s.append("\\").append(hex);
    s.append(1, '"');
    return s;
}

StringArrayValue::StringArrayValue(const soul::ast::Span& span_, char prefix_, const std::vector<Value*>& elements_) :
    Value(span_, ValueKind::stringArrayValue, nullptr), prefix(prefix_), elements(elements_)
{
}

void StringArrayValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string StringArrayValue::ToString() const
{
    std::string s = std::string(1, prefix) + "[ ";
    bool first = true;
    for (Value* element : elements)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            s.append(", ");
        }
        s.append(element->GetType()->Name()).append(" ").append(element->ToString());
    }
    s.append(" ]");
    return s;
}

ConversionValue::ConversionValue(const soul::ast::Span& span_, Type* type_, Value* from_) noexcept :
    Value(span_, ValueKind::conversionValue, type_), from(from_)
{
}

void ConversionValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ConversionValue::ToString() const
{
    std::string s = "conv(";
    s.append(from->GetType()->Name()).append(1, ' ').append(from->ToString()).append(1, ')');
    return s;
}

ClsIdValue::ClsIdValue(const soul::ast::Span& span_, Type* type_, const std::string& typeId_) :
    Value(span_, ValueKind::clsIdValue, type_), typeId(typeId_)
{
}

void ClsIdValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ClsIdValue::ToString() const
{
    std::string s = "clsid(" + typeId + ")";
    return s;
}

SymbolValue::SymbolValue(const soul::ast::Span& span_, Type* type_, const std::string& symbol_) :
    Value(span_, ValueKind::symbolValue, type_), symbol(symbol_), function(nullptr), globalVariable(nullptr)
{
}

void SymbolValue::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string SymbolValue::ToString() const
{
    return "@" + symbol;
}

GlobalVariable::GlobalVariable(const soul::ast::Span& span_, Type* type_, const std::string& name_, Value* initializer_) :
    Value(span_, ValueKind::globalVariable, type_), name(name_), initializer(initializer_)
{
}

void GlobalVariable::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void GlobalVariable::Write(util::CodeFormatter& formatter)
{
    formatter.Write(GetType()->BaseType()->Name());
    formatter.Write(" ");
    formatter.Write(name);
    if (initializer)
    {
        formatter.Write(" = ");
        if (initializer->IsAggregateValue() || initializer->IsStringValue() || initializer->IsStringArrayValue())
        {
            formatter.Write(initializer->ToString());
        }
        else
        {
            formatter.Write(initializer->GetType()->Name());
            formatter.Write(" ");
            formatter.Write(initializer->ToString());
        }
    }
    else
    {
        formatter.Write(";");
    }
}

std::string GlobalVariable::ToString() const
{
    return name;
}

Data::Data() noexcept : context(nullptr), nextStringValueId(0)
{
}

GlobalVariable* Data::AddGlobalVariable(const soul::ast::Span& span, Type* type, const std::string& variableName, Value* initializer, IntermediateContext* context)
{
    try
    {
        GlobalVariable* globalVariable = new GlobalVariable(span, type, variableName, initializer);
        values.push_back(std::unique_ptr<Value>(globalVariable));
        globalVariableMap[variableName] = globalVariable;
        globalVariables.push_back(globalVariable);
        return globalVariable;
    }
    catch (const std::exception& ex)
    {
        Error("error adding global variable: " + std::string(ex.what()), span, context);
        return nullptr;
    }
}

GlobalVariable* Data::GetGlobalVariableForString(Value* stringValue, Type* charType)
{
    std::string strValue = stringValue->ToString();
    auto it = globalStringVariableMap.find(strValue);
    if (it != globalStringVariableMap.cend())
    {
        return it->second;
    }
    GlobalVariable* globalVariable = context->AddGlobalVariable(soul::ast::Span(),
        context->MakePtrType(charType), context->GetNextStringValueId(), stringValue);
    globalStringVariableMap[strValue] = globalVariable;
    return globalVariable;
}

Value* Data::GetBoolValue(bool value, const Types& types)
{
    if (value)
    {
        return GetTrueValue(types);
    }
    else
    {
        return GetFalseValue(types);
    }
}

Value* Data::GetTrueValue(const Types& types)
{
    if (!trueValue)
    {
        trueValue.reset(new BoolValue(true, types.Get(boolTypeId)));
    }
    return trueValue.get();
}

Value* Data::GetFalseValue(const Types& types)
{
    if (!falseValue)
    {
        falseValue.reset(new BoolValue(false, types.Get(boolTypeId)));
    }
    return falseValue.get();
}

Value* Data::GetSByteValue(std::int8_t value, const Types& types)
{
    return sbyteValueMap.Get(value, this, types);
}

Value* Data::GetByteValue(std::uint8_t value, const Types& types)
{
    return byteValueMap.Get(value, this, types);
}

Value* Data::GetShortValue(std::int16_t value, const Types& types)
{
    return shortValueMap.Get(value, this, types);
}

Value* Data::GetUShortValue(std::uint16_t value, const Types& types)
{
    return ushortValueMap.Get(value, this, types);
}

Value* Data::GetIntValue(std::int32_t value, const Types& types)
{
    return intValueMap.Get(value, this, types);
}

Value* Data::GetUIntValue(std::uint32_t value, const Types& types)
{
    return uintValueMap.Get(value, this, types);
}

Value* Data::GetLongValue(std::int64_t value, const Types& types)
{
    return longValueMap.Get(value, this, types);
}

Value* Data::GetULongValue(std::uint64_t value, const Types& types)
{
    return ulongValueMap.Get(value, this, types);
}

Value* Data::GetIntegerValue(Type* type, std::int64_t value, const Types& types)
{
    switch (type->Id())
    {
    case sbyteTypeId:
    {
        return GetSByteValue(static_cast<std::int8_t>(value), types);
    }
    case byteTypeId:
    {
        return GetByteValue(static_cast<std::uint8_t>(value), types);
    }
    case shortTypeId:
    {
        return GetShortValue(static_cast<std::int16_t>(value), types);
    }
    case ushortTypeId:
    {
        return GetUShortValue(static_cast<std::uint16_t>(value), types);
    }
    case intTypeId:
    {
        return GetIntValue(static_cast<std::int32_t>(value), types);
    }
    case uintTypeId:
    {
        return GetUIntValue(static_cast<std::uint32_t>(value), types);
    }
    case longTypeId:
    {
        return GetLongValue(static_cast<std::int64_t>(value), types);
    }
    case ulongTypeId:
    {
        return GetULongValue(static_cast<std::uint64_t>(value), types);
    }
    }
    Error("integer type expected", soul::ast::Span(), context);
    return nullptr;
}

Value* Data::GetFloatValue(float value, const Types& types)
{
    return floatValueMap.Get(value, this, types);
}

Value* Data::GetDoubleValue(double value, const Types& types)
{
    return doubleValueMap.Get(value, this, types);
}

Value* Data::GetFloatingValue(Type* type, double value, const Types& types)
{
    switch (type->Id())
    {
    case floatTypeId:
    {
        return GetFloatValue(static_cast<float>(value), types);
    }
    case doubleTypeId:
    {
        return GetDoubleValue(static_cast<double>(value), types);
    }
    }
    Error("floating point type expected", soul::ast::Span(), context);
    return nullptr;
}

Value* Data::GetNullValue(Type* type)
{
    auto it = nullValueMap.find(type);
    if (it != nullValueMap.cend())
    {
        return it->second;
    }
    else
    {
        NullValue* nullValue = new NullValue(type);
        nullValueMap[type] = nullValue;
        values.push_back(std::unique_ptr<Value>(nullValue));
        return nullValue;
    }
}

Value* Data::MakeValue(std::int8_t value, const Types& types)
{
    SByteValue* constantValue = new SByteValue(value, types.Get(sbyteTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(std::uint8_t value, const Types& types)
{
    ByteValue* constantValue = new ByteValue(value, types.Get(byteTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(std::int16_t value, const Types& types)
{
    ShortValue* constantValue = new ShortValue(value, types.Get(shortTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(std::uint16_t value, const Types& types)
{
    UShortValue* constantValue = new UShortValue(value, types.Get(ushortTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(std::int32_t value, const Types& types)
{
    IntValue* constantValue = new IntValue(value, types.Get(intTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(std::uint32_t value, const Types& types)
{
    UIntValue* constantValue = new UIntValue(value, types.Get(uintTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(std::int64_t value, const Types& types)
{
    LongValue* constantValue = new LongValue(value, types.Get(longTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(std::uint64_t value, const Types& types)
{
    ULongValue* constantValue = new ULongValue(value, types.Get(ulongTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(float value, const Types& types)
{
    FloatValue* constantValue = new FloatValue(value, types.Get(floatTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeValue(double value, const Types& types)
{
    DoubleValue* constantValue = new DoubleValue(value, types.Get(doubleTypeId));
    values.push_back(std::unique_ptr<Value>(constantValue));
    return constantValue;
}

Value* Data::MakeArrayValue(const soul::ast::Span& span, const std::vector<Value*>& elements, ArrayType* arrayType)
{
    ArrayValue* arrayValue = new ArrayValue(span, elements, arrayType);
    values.push_back(std::unique_ptr<Value>(arrayValue));
    return arrayValue;
}

Value* Data::MakeStructureValue(const soul::ast::Span& span, const std::vector<Value*>& fieldValues, StructureType* structureType)
{
    StructureValue* structureValue = new StructureValue(span, fieldValues, structureType);
    values.push_back(std::unique_ptr<Value>(structureValue));
    return structureValue;
}

Value* Data::MakeStringValue(const soul::ast::Span& span, const std::string& value, bool crop)
{
    std::string val = value;
    if (crop)
    {
        val = val.substr(1, val.length() - 2);
    }
    std::string s;
    int state = 0;
    std::string hex;
    for (char c : val)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '\\')
            {
                hex.clear();
                state = 1;
            }
            else
            {
                s.append(1, c);
            }
            break;
        }
        case 1:
        {
            if (c == '\\')
            {
                s.append(1, c);
                state = 0;
            }
            else
            {
                hex.append(1, c);
                state = 2;
            }
            break;
        }
        case 2:
        {
            hex.append(1, c);
            std::uint8_t x = util::ParseHexByte(hex);
            char ch = static_cast<char>(x);
            s.append(1, ch);
            state = 0;
            break;
        }
        }
    }
    StringValue* stringValue = new StringValue(span, s);
    values.push_back(std::unique_ptr<Value>(stringValue));
    return stringValue;
}

Value* Data::MakeStringArrayValue(const soul::ast::Span& span, char prefix, const std::vector<Value*>& elements)
{
    StringArrayValue* stringArrayValue = new StringArrayValue(span, prefix, elements);
    values.push_back(std::unique_ptr<Value>(stringArrayValue));
    return stringArrayValue;
}

Value* Data::MakeConversionValue(const soul::ast::Span& span, Type* type, Value* from)
{
    ConversionValue* conversionValue = new ConversionValue(span, type, from);
    values.push_back(std::unique_ptr<Value>(conversionValue));
    return conversionValue;
}

Value* Data::MakeClsIdValue(const soul::ast::Span& span, Type* type, const std::string& clsIdStr)
{
    std::string typeId = clsIdStr.substr(6, clsIdStr.length() - 6 - 1);
    ClsIdValue* clsIdValue = new ClsIdValue(span, type, typeId);
    values.push_back(std::unique_ptr<Value>(clsIdValue));
    return clsIdValue;
}

Value* Data::MakeSymbolValue(const soul::ast::Span& span, Type* type, const std::string& symbol)
{
    SymbolValue* symbolValue = new SymbolValue(span, type, symbol);
    values.push_back(std::unique_ptr<Value>(symbolValue));
    return symbolValue;
}

Value* Data::MakeIntegerLiteral(const soul::ast::Span& span, Type* type, const std::string& strValue, const Types& types)
{
    switch (type->Id())
    {
    case boolTypeId:
    {
        if (strValue == "true")
        {
            return GetTrueValue(types);
        }
        else if (strValue == "false")
        {
            return GetFalseValue(types);
        }
        else
        {
            Error("error making literal: Boolean value expected", span, context);
        }
        break;
    }
    case sbyteTypeId:
    {
        std::int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<std::int8_t>::min() || value > std::numeric_limits<std::int8_t>::max())
        {
            Error("error making literal: range error: sbyte value expected", span, context);
        }
        return GetSByteValue(static_cast<std::int8_t>(value), types);
    }
    case byteTypeId:
    {
        std::int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<std::uint8_t>::min() || value > std::numeric_limits<std::uint8_t>::max())
        {
            Error("error making literal: range error: byte value expected", span, context);
        }
        return GetByteValue(static_cast<std::uint8_t>(value), types);
    }
    case shortTypeId:
    {
        std::int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<std::int16_t>::min() || value > std::numeric_limits<std::int16_t>::max())
        {
            Error("error making literal: range error: short value expected", span, context);
        }
        return GetShortValue(static_cast<std::int16_t>(value), types);
    }
    case ushortTypeId:
    {
        std::int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<std::uint16_t>::min() || value > std::numeric_limits<std::uint16_t>::max())
        {
            Error("error making literal: range error: ushort value expected", span, context);
        }
        return GetUShortValue(static_cast<std::uint16_t>(value), types);
    }
    case intTypeId:
    {
        std::int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<std::int32_t>::min() || value > std::numeric_limits<std::int32_t>::max())
        {
            Error("error making literal: range error: int value expected", span, context);
        }
        return GetIntValue(static_cast<std::int32_t>(value), types);
    }
    case uintTypeId:
    {
        std::int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<std::uint32_t>::min() || value > std::numeric_limits<std::uint32_t>::max())
        {
            Error("error making literal: range error: uint value expected", span, context);
        }
        return GetUIntValue(static_cast<std::uint32_t>(value), types);
    }
    case longTypeId:
    {
        std::int64_t value = std::stoll(strValue);
        if (value < std::numeric_limits<std::int64_t>::min() || value > std::numeric_limits<std::int64_t>::max())
        {
            Error("error making literal: range error: long value expected", span, context);
        }
        return GetLongValue(static_cast<std::int64_t>(value), types);
    }
    case ulongTypeId:
    {
        std::uint64_t value = std::stoull(strValue);
        if (value < std::numeric_limits<std::uint64_t>::min() || value > std::numeric_limits<std::uint64_t>::max())
        {
            Error("error making literal: range error: ulong value expected", span, context);
        }
        return GetULongValue(static_cast<std::int64_t>(value), types);
    }
    case floatTypeId:
    {
        float value = std::stof(strValue);
        return GetFloatValue(value, types);
    }
    case doubleTypeId:
    {
        double value = std::stod(strValue);
        return GetDoubleValue(value, types);
    }
    default:
    {
        Error("error making literal: invalid numeric value", span, context);
    }
    }
    return nullptr;
}

Value* Data::MakeAddressLiteral(const soul::ast::Span& span, Type* type, const std::string& id, bool resolve)
{
    AddressValue* addressValue = new AddressValue(span, id, type);
    if (resolve)
    {
        ResolveAddressValue(addressValue);
    }
    values.push_back(std::unique_ptr<Value>(addressValue));
    addressValues.push_back(addressValue);
    return addressValue;
}

void Data::ResolveAddressValue(AddressValue* addressValue)
{
    auto it = globalVariableMap.find(addressValue->Id());
    if (it != globalVariableMap.cend())
    {
        GlobalVariable* globalVariable = it->second;
        addressValue->SetValue(globalVariable);
    }
    else
    {
        Error("error resolving address literal: global variable id '" + addressValue->Id() + "' not found", addressValue->Span(), context);
    }
}

void Data::ResolveAddressValues()
{
    for (AddressValue* addressValue : addressValues)
    {
        ResolveAddressValue(addressValue);
    }
}

void Data::VisitGlobalVariables(Visitor& visitor)
{
    for (GlobalVariable* globalVariable : globalVariables)
    {
        globalVariable->Accept(visitor);
    }
}

std::string Data::GetNextStringValueId()
{
    std::int32_t id = nextStringValueId++;
    return "string_" + std::to_string(id) + "_" + context->GetCompileUnit().Id();
}

void Data::Write(util::CodeFormatter& formatter)
{
    if (globalVariables.empty()) return;
    formatter.WriteLine("data");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& globalVariable : globalVariables)
    {
        globalVariable->Write(formatter);
        formatter.WriteLine();
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

} // cmajor::otava::intermediate
