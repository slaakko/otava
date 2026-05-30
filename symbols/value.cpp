// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.value;

import otava.symbols.context;

namespace otava::symbols {

std::string ValueKindStr(ValueKind kind)
{
    switch (kind)
    {
    case ValueKind::boolValue: return "boolValue";
    case ValueKind::integerValue: return "integerValue";
    case ValueKind::floatingValue: return "floatingValue";
    case ValueKind::nullPtrValue: return "nullPtrValue";
    case ValueKind::stringValue: return "stringValue";
    case ValueKind::charValue: return "charValue";
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
    case ValueKind::integerValue:
    {
        switch (right)
        {
        case ValueKind::integerValue:
        {
            return ValueKind::integerValue;
        }
        case ValueKind::floatingValue:
        {
            return ValueKind::floatingValue;
        }
        case ValueKind::boolValue:
        {
            return ValueKind::boolValue;
        }
        default:
        {
            return ValueKind::none;
        }
        }
        break;
    }
    case ValueKind::floatingValue:
    {
        switch (right)
        {
        case ValueKind::integerValue:
        case ValueKind::floatingValue:
        {
            return ValueKind::floatingValue;
        }
        case ValueKind::boolValue:
        {
            return ValueKind::boolValue;
        }
        }
    }
    }
    return ValueKind::none;
}

Value::Value(Module* module_, SymbolId id_) : Symbol(module_, id_)
{
}

Value::Value(Module* module_, SymbolId id_, const std::string& rep_) : Symbol(module_, id_, rep_)
{
}

ValueKind Value::GetValueKind() const noexcept
{
    switch (Kind())
    {
    case SymbolKind::boolValueSymbol:
    {
        return ValueKind::boolValue;
    }
    case SymbolKind::integerValueSymbol:
    {
        return ValueKind::integerValue;
    }
    case SymbolKind::floatingValueSymbol:
    {
        return ValueKind::floatingValue;
    }
    case SymbolKind::nullPtrValueSymbol:
    {
        return ValueKind::nullPtrValue;
    }
    case SymbolKind::stringValueSymbol:
    {
        return ValueKind::stringValue;
    }
    case SymbolKind::charValueSymbol:
    {
        return ValueKind::charValue;
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

TypeSymbol* Value::GetType() const
{
    // TODO
    return nullptr;
}

BoolValue::BoolValue(Module* module_, bool value_, const std::string& rep_, Context* context) : 
    Value(module_, context->GetNextSymbolId(SymbolKind::boolValueSymbol), rep_), value(value_)
{
}

IntegerValue::IntegerValue(Module* module_, std::int64_t value_, const std::string& rep_, Context* context) : 
    Value(module_, context->GetNextSymbolId(SymbolKind::integerValueSymbol), rep_), value(value_)
{
}

BoolValue* IntegerValue::ToBoolValue(EvaluationContext& context)
{
    // TODO
    return nullptr;
}

FloatingValue::FloatingValue(Module* module_, double value_, const std::string& rep_, Context* context) : 
    Value(module_, context->GetNextSymbolId(SymbolKind::floatingValueSymbol), rep_), value(value_)
{
}

BoolValue* FloatingValue::ToBoolValue(EvaluationContext& context)
{
    // TODO
    return nullptr;
}

NullPtrValue::NullPtrValue(Module* module_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::nullPtrValueSymbol), "nullptr")
{
}

BoolValue* NullPtrValue::ToBoolValue(EvaluationContext& context)
{
    // TODO
    return nullptr;
}

StringValue::StringValue(Module* module_, const std::string& value_, Context* context) : 
    Value(module_, context->GetNextSymbolId(SymbolKind::stringValueSymbol), value_), value(value_)
{
}

BoolValue* StringValue::ToBoolValue(EvaluationContext& context)
{
    // TODO
    return nullptr;
}

CharValue::CharValue(Module* module_, char32_t value_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::charValueSymbol), util::ToUtf8(std::u32string(1, value_))), value(value_)
{
}

BoolValue* CharValue::ToBoolValue(EvaluationContext& context)
{
    // TODO
    return nullptr;
}

SymbolValue::SymbolValue(Module* module_, Symbol* symbol_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::symbolValueSymbol), symbol_->Name()), symbol(symbol_)
{
}

BoolValue* SymbolValue::ToBoolValue(EvaluationContext& context)
{
    // TODO
    return nullptr;
}

InvokeValue::InvokeValue(Module* module_, Value* subject_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::invokeValueSymbol), subject->Name()), subject(subject_)
{
}

ArrayValue::ArrayValue(Module* module_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::arrayValueSymbol), "<array>")
{
}

BoolValue* ArrayValue::ToBoolValue(EvaluationContext& context)
{
    // TODO
    return nullptr;
}

StructureValue::StructureValue(Module* module_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::structureValueSymbol), "<structure>")
{
}

void EvaluationContext::Init()
{
    // TODO
}

void EvaluationContext::Write(Writer& writer, Context* context)
{
    // TODO
}

void EvaluationContext::Read(Reader& reader)
{
    // TODO
}

BoolValue* EvaluationContext::GetBoolValue(bool value)
{
    // TODO
    return nullptr;
}

IntegerValue* EvaluationContext::GetIntegerValue(std::int64_t value, const std::string& rep, TypeSymbol* type)
{
    // TODO
    return nullptr;
}

FloatingValue* EvaluationContext::GetFloatingValue(double value, const std::string& rep, TypeSymbol* type)
{
    // TODO
    return nullptr;
}

NullPtrValue* EvaluationContext::GetNullPtrValue()
{
    // TODO
    return nullptr;
}

StringValue* EvaluationContext::GetStringValue(const std::string& value, TypeSymbol* type)
{
    // TODO
    return nullptr;
}

CharValue* EvaluationContext::GetCharValue(char32_t value, TypeSymbol* type)
{
    // TODO
    return nullptr;
}

SymbolValue* EvaluationContext::GetSymbolValue(Symbol* symbol)
{
    // TODO
    return nullptr;
}

InvokeValue* EvaluationContext::GetInvokeValue(Value* subject)
{
    // TODO
    return nullptr;
}

ArrayValue* EvaluationContext::GetArrayValue()
{
    // TODO
    return nullptr;
}

StructureValue* EvaluationContext::GetStructureValue()
{
    // TODO
    return nullptr;
}

void EvaluationContext::AddValue(Value* value)
{
    // TODO
}

} // namespace otava::symbols
