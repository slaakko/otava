// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.value;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.writer;
import otava.symbols.reader;

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

Value::Value(Module* module_, SymbolId id_) : Symbol(module_, id_), type(nullptr), typeId(zeroSymbolId)
{
}

Value::Value(Module* module_, SymbolId id_, const std::string& rep_) : Symbol(module_, id_, rep_), type(nullptr), typeId(zeroSymbolId)
{
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

TypeSymbol* Value::GetType(Context* context)
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
    if (type->GetModule() != GetModule())
    {
        GetModule()->GetSymbolTable()->AddImportedSymbol(type->Id(), type->GetModule()->Id());
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
    typeId = SymbolId(reader.CurrentReader().ReadUInt());
}

BoolValue::BoolValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

BoolValue::BoolValue(Module* module_, bool value_, const std::string& rep_, Context* context) : 
    Value(module_, context->GetNextSymbolId(SymbolKind::boolValueSymbol), rep_), value(value_)
{
}

Value* BoolValue::Convert(ValueKind kind, Context* context)
{
    switch (kind)
    {
    case ValueKind::boolValue: return this;
    case ValueKind::integerValue:
    {
        return context->GetEvaluationContext()->GetIntegerValue(static_cast<std::int64_t>(value), std::to_string(static_cast<int>(value)),
            context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context), context);
    }
    case ValueKind::floatingValue:
    {
        return context->GetEvaluationContext()->GetFloatingValue(static_cast<double>(static_cast<int>(value)), std::to_string(static_cast<int>(value)),
            context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::doubleType, context), context);
    }
    }
    return this;
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

otava::intermediate::Value* BoolValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    return emitter.EmitBool(value);
}

IntegerValue::IntegerValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

IntegerValue::IntegerValue(Module* module_, std::int64_t value_, const std::string& rep_, Context* context) : 
    Value(module_, context->GetNextSymbolId(SymbolKind::integerValueSymbol), rep_), value(value_)
{
}

Value* IntegerValue::Convert(ValueKind kind, Context* context)
{
    switch (kind)
    {
    case ValueKind::boolValue: return ToBoolValue(context);
    case ValueKind::integerValue: return this;
    case ValueKind::floatingValue:
    {
        return context->GetEvaluationContext()->GetFloatingValue(static_cast<double>(value), std::to_string(value),
            context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::doubleType, context), context);
    }
    }
    return this;
}

BoolValue* IntegerValue::ToBoolValue(Context* context)
{
    if (value) return context->GetEvaluationContext()->GetBoolValue(true); else return context->GetEvaluationContext()->GetBoolValue(false);
}

void IntegerValue::Write(Writer& writer)
{
    Value::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void IntegerValue::Read(Reader& reader)
{
    Value::Read(reader);
    value = reader.CurrentReader().ReadLong();
}

otava::intermediate::Value* IntegerValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    otava::intermediate::Type* irType = GetType(context)->IrType(emitter, fullSpan, context);
    return emitter.EmitIntegerValue(irType, value);
}

FloatingValue::FloatingValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

FloatingValue::FloatingValue(Module* module_, double value_, const std::string& rep_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::floatingValueSymbol), rep_), value(value_)
{
}

BoolValue* FloatingValue::ToBoolValue(Context* context)
{
    if (value) return context->GetEvaluationContext()->GetBoolValue(true); else return context->GetEvaluationContext()->GetBoolValue(false);
}

Value* FloatingValue::Convert(ValueKind kind, Context* context)
{
    switch (kind)
    {
    case ValueKind::boolValue: return ToBoolValue(context);
    case ValueKind::integerValue:
    {
        return context->GetEvaluationContext()->GetIntegerValue(static_cast<std::int64_t>(value), std::to_string(value),
            context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context), context);
    }
    case ValueKind::floatingValue: return this;
    }
    return this;
}

void FloatingValue::Write(Writer& writer)
{
    Value::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void FloatingValue::Read(Reader& reader)
{
    Value::Read(reader);
    value = reader.CurrentReader().ReadDouble();
}

otava::intermediate::Value* FloatingValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    otava::intermediate::Type* irType = GetType(context)->IrType(emitter, fullSpan, context);
    return emitter.EmitFloatingValue(irType, value);
}

NullPtrValue::NullPtrValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

NullPtrValue::NullPtrValue(Module* module_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::nullPtrValueSymbol), "nullptr")
{
}

BoolValue* NullPtrValue::ToBoolValue(Context* context)
{
    return context->GetEvaluationContext()->GetBoolValue(false);
}

Value* NullPtrValue::Convert(ValueKind kind, Context* context)
{
    switch (kind)
    {
    case ValueKind::boolValue: return ToBoolValue(context);
    }
    return this;
}

otava::intermediate::Value* NullPtrValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    return GetType(context)->IrType(emitter, fullSpan, context)->DefaultValue();
}

StringValue::StringValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

StringValue::StringValue(Module* module_, const std::string& value_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::stringValueSymbol), value_), value(value_)
{
}

BoolValue* StringValue::ToBoolValue(Context* context)
{
    return context->GetEvaluationContext()->GetBoolValue(false);
}

Value* StringValue::Convert(ValueKind kind, Context* context)
{
    return this;
}

void StringValue::Write(Writer& writer)
{
    Value::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void StringValue::Read(Reader& reader)
{
    Value::Read(reader);
    value = reader.CurrentReader().ReadString();
}

otava::intermediate::Value* StringValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    TypeSymbol* type = GetType(context)->DirectType(context)->FinalType(fullSpan, context);
    if (type->IsConstCharPtrType(context) || type->IsBasicStringCharType(context))
    {
        return emitter.EmitStringValue(value);
    }
    else if (type->IsConstChar16PtrType(context) || type->IsBasicStringChar16Type(context))
    {
        return emitter.EmitString16Value(value);
    }
    else if (type->IsConstChar32PtrType(context) || type->IsBasicStringChar32Type(context))
    {
        return emitter.EmitString32Value(value);
    }
    else
    {
        ThrowException("unknown base type for string type '" + type->FullName(context) + "'");
    }
    return nullptr;
}

CharValue::CharValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

CharValue::CharValue(Module* module_, char32_t value_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::charValueSymbol), util::ToUtf8(std::u32string(1, value_))), value(value_)
{
}

BoolValue* CharValue::ToBoolValue(Context* context)
{
    return context->GetEvaluationContext()->GetBoolValue(false);
}

Value* CharValue::Convert(ValueKind kind, Context* context)
{
    return this;
}

void CharValue::Write(Writer& writer)
{
    Value::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
}

void CharValue::Read(Reader& reader)
{
    Value::Read(reader);
    value = reader.CurrentReader().ReadUChar();
}

otava::intermediate::Value* CharValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    otava::intermediate::Type* irType = GetType(context)->IrType(emitter, fullSpan, context);
    return emitter.EmitIntegerValue(irType, value);
}

SymbolValue::SymbolValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

SymbolValue::SymbolValue(Module* module_, Symbol* symbol_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::symbolValueSymbol), symbol_->Name()), symbol(symbol_), symbolId(zeroSymbolId)
{
}

Symbol* SymbolValue::GetSymbol(Context* context)
{
    if (symbol)
    {
        return symbol;
    }
    if (IsReadOnly() && symbolId != zeroSymbolId)
    {
        symbol = context->GetSymbolTable()->GetSymbol(symbolId, context);
    }
    return symbol;
}

BoolValue* SymbolValue::ToBoolValue(Context* context)
{
    return context->GetEvaluationContext()->GetBoolValue(false);
}

Value* SymbolValue::Convert(ValueKind kind, Context* context)
{
    switch (kind)
    {
    case ValueKind::integerValue:
    {
        switch (symbol->Kind())
        {
        case SymbolKind::enumConstantSymbol:
        {
            EnumConstantSymbol* enumConstantSymbol = static_cast<EnumConstantSymbol*>(symbol);
            Value* value = enumConstantSymbol->GetValue(context);
            switch (value->GetValueKind())
            {
            case ValueKind::integerValue:
            {
                IntegerValue* integerValue = static_cast<IntegerValue*>(value);
                return context->GetEvaluationContext()->GetIntegerValue(integerValue->GetValue(), enumConstantSymbol->Name(), value->GetType(context), context);
            }
            case ValueKind::boolValue:
            {
                BoolValue* boolValue = static_cast<BoolValue*>(value);
                return context->GetEvaluationContext()->GetIntegerValue(static_cast<std::int64_t>(boolValue->GetValue()), enumConstantSymbol->Name(), 
                    value->GetType(context), context);
            }
            }
            break;
        }
        }
        break;
    }
    }
    return this;
}

void SymbolValue::Write(Writer& writer)
{
    Value::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(symbol->Id()));
}

void SymbolValue::Read(Reader& reader)
{
    Value::Read(reader);
    symbolId = SymbolId(reader.CurrentReader().ReadUInt());
}

otava::intermediate::Value* SymbolValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (symbol->IsVariableSymbol())
    {
        VariableSymbol* variableSymbol = static_cast<VariableSymbol*>(symbol);
        Value* value = variableSymbol->GetValue(context);
        if (value)
        {
            return value->IrValue(emitter, fullSpan, context);
        }
    }
    ThrowException("cannot evaluate statically", fullSpan, context);
    return nullptr;
}

InvokeValue::InvokeValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

InvokeValue::InvokeValue(Module* module_, Value* subject_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::invokeValueSymbol), subject_->Name()), subject(subject_), subjectId(zeroSymbolId)
{
}

Value* InvokeValue::Subject(Context* context)
{
    if (subject)
    {
        return subject;
    }
    if (IsReadOnly() && subjectId != zeroSymbolId)
    {
        subject = context->GetEvaluationContext()->GetValue(subjectId);
    }
    return subject;
}

Value* InvokeValue::Convert(ValueKind kind, Context* context)
{
    return this;
}

BoolValue* InvokeValue::ToBoolValue(Context* context)
{
    return context->GetEvaluationContext()->GetBoolValue(false);
}

void InvokeValue::Write(Writer& writer)
{
    Value::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(subject->Id()));
}

void InvokeValue::Read(Reader& reader)
{
    Value::Read(reader);
    subjectId = SymbolId(reader.CurrentReader().ReadUInt());
}

ArrayValueHeader::ArrayValueHeader() : bodyOffset(FileOffset(0)), bodyLength(Length(0))
{
}

void ArrayValueHeader::Write(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(bodyOffset));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(bodyLength));
}

void ArrayValueHeader::Read(Reader& reader)
{
    bodyOffset = FileOffset(reader.CurrentReader().ReadUInt());
    bodyLength = Length(reader.CurrentReader().ReadUInt());
}

ArrayValue::ArrayValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

ArrayValue::ArrayValue(Module* module_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::arrayValueSymbol), "<array>")
{
}

Cardinality ArrayValue::Count()
{
    if (IsReadOnly())
    {
        return Cardinality(elementValueIds.size());
    }
    else
    {
        return Cardinality(elementValues.size());
    }
}

void ArrayValue::AddElementValue(Value* elementValue)
{
    elementValues.push_back(elementValue);
}

Value* ArrayValue::GetElementValue(Index index, Context* context)
{
    if (IsReadOnly())
    {
        SymbolId valueId = elementValueIds[ToUnderlying(index)];
        Symbol* symbol = context->GetSymbolTable()->GetSymbol(valueId, context);
        if (symbol)
        {
            if (symbol->IsValueSymbol())
            {
                Value* value = static_cast<Value*>(symbol);
                return value;
            }
            else
            {
                ThrowException("ArrayValue::GetElementValue: value symbol expected");
            }
        }
        else
        {
            ThrowException("ArrayValue::GetElementValue: symbol " + std::to_string(ToUnderlying(valueId)) + " not found");
        }
    }
    else
    {
        return elementValues[ToUnderlying(index)];
    }
}

BoolValue* ArrayValue::ToBoolValue(Context* context)
{
    return context->GetEvaluationContext()->GetBoolValue(false);
}

Value* ArrayValue::Convert(ValueKind kind, Context* context)
{
    return this;
}

void ArrayValue::Write(Writer& writer)
{
    FileOffset start = FileOffset(writer.Position());
    header.Write(writer);
    Value::Write(writer);
    Cardinality count = Cardinality(elementValues.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (Value* elementValue : elementValues)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(elementValue->Id()));
    }
    header.bodyOffset = FileOffset(writer.Position());
    for (Value* elementValue : elementValues)
    {
        writer.Write(elementValue);
    }
    header.bodyLength = Length(writer.Position() - ToUnderlying(header.bodyOffset));
    FileOffset end = FileOffset(writer.Position());
    writer.Seek(ToUnderlying(start));
    header.Write(writer);
    writer.Seek(ToUnderlying(end));
}

void ArrayValue::Read(Reader& reader)
{
    header.Read(reader);
    Value::Read(reader);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId elementValueId = SymbolId(reader.CurrentReader().ReadUInt());
        elementValueIds.push_back(elementValueId);
    }
    reader.CurrentReader().Skip(ToUnderlying(header.bodyLength));
}

otava::intermediate::Value* ArrayValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    otava::intermediate::ArrayType* arrayType = static_cast<otava::intermediate::ArrayType*>(GetType(context)->IrType(emitter, fullSpan, context));
    std::vector<otava::intermediate::Value*> elements;
    for (Value* elementValue : elementValues)
    {
        elements.push_back(elementValue->IrValue(emitter, fullSpan, context));
    }
    return emitter.EmitArrayValue(elements, arrayType);
}

StructureValueHeader::StructureValueHeader() : bodyOffset(FileOffset(0)), bodyLength(Length(0))
{
}

void StructureValueHeader::Write(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(bodyOffset));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(bodyLength));
}

void StructureValueHeader::Read(Reader& reader)
{
    bodyOffset = FileOffset(reader.CurrentReader().ReadUInt());
    bodyLength = Length(reader.CurrentReader().ReadUInt());
}

StructureValue::StructureValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

StructureValue::StructureValue(Module* module_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::structureValueSymbol), "<structure>")
{
}

Cardinality StructureValue::Count()
{
    if (IsReadOnly())
    {
        return Cardinality(fieldValueIds.size());
    }
    else
    {
        return Cardinality(fieldValues.size());
    }
}

void StructureValue::AddFieldValue(Value* fieldValue)
{
    fieldValues.push_back(fieldValue);
}

Value* StructureValue::GetFieldValue(Index index, Context* context)
{
    if (IsReadOnly())
    {
        SymbolId valueId = fieldValueIds[ToUnderlying(index)];
        Symbol* symbol = context->GetSymbolTable()->GetSymbol(valueId, context);
        if (symbol)
        {
            if (symbol->IsValueSymbol())
            {
                Value* value = static_cast<Value*>(symbol);
                return value;
            }
            else
            {
                ThrowException("StructureValue::GetFieldValue: value symbol expected");
            }
        }
        else
        {
            ThrowException("StructureValue::GetFieldValue: symbol " + std::to_string(ToUnderlying(valueId)) + " not found");
        }
    }
    else
    {
        return fieldValues[ToUnderlying(index)];
    }
}

BoolValue* StructureValue::ToBoolValue(Context* context)
{
    return context->GetEvaluationContext()->GetBoolValue(false);
}

Value* StructureValue::Convert(ValueKind kind, Context* context)
{
    return this;
}

void StructureValue::Write(Writer& writer)
{
    FileOffset start = FileOffset(writer.Position());
    header.Write(writer);
    Value::Write(writer);
    Cardinality count = Cardinality(fieldValues.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (Value* fieldValue : fieldValues)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(fieldValue->Id()));
    }
    header.bodyOffset = FileOffset(writer.Position());
    for (Value* fieldValue : fieldValues)
    {
        writer.Write(fieldValue);
    }
    header.bodyLength = Length(writer.Position() - ToUnderlying(header.bodyOffset));
    FileOffset end = FileOffset(writer.Position());
    writer.Seek(ToUnderlying(start));
    header.Write(writer);
    writer.Seek(ToUnderlying(end));
}

void StructureValue::Read(Reader& reader)
{
    header.Read(reader);
    Value::Read(reader);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId fieldValueId = SymbolId(reader.CurrentReader().ReadUInt());
        fieldValueIds.push_back(fieldValueId);
    }
    reader.CurrentReader().Skip(ToUnderlying(header.bodyLength));
}

otava::intermediate::Value* StructureValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    otava::intermediate::StructureType* structureType = static_cast<otava::intermediate::StructureType*>(GetType(context)->IrType(emitter, fullSpan, context));
    std::vector<otava::intermediate::Value*> fields;
    for (Value* fieldValue : fieldValues)
    {
        fields.push_back(fieldValue->IrValue(emitter, fullSpan, context));
    }
    return emitter.EmitStructureValue(fields, structureType);
}

EvaluationContext::EvaluationContext(Module* module_, bool readOnly_) : module(module_), initialized(false), readOnly(readOnly_)
{
}

void EvaluationContext::Init(Context* context)
{
    if (initialized) return;
    initialized = true;
    trueValue.reset(new BoolValue(module, true, "true", context));
    TypeSymbol* boolType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context);
    trueValue->SetType(boolType, context);
    falseValue.reset(new BoolValue(module, false, "false", context));
    falseValue->SetType(boolType, context);
    nullPtrValue.reset(new NullPtrValue(module, context));
    TypeSymbol* nullPtrType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::nullPtrType, context);
    nullPtrValue->SetType(nullPtrType, context);
}

void EvaluationContext::Write(Writer& writer, Context* context)
{
    trueValue->Write(writer);
    falseValue->Write(writer);
    nullPtrValue->Write(writer);
    for (const auto& value : values)
    {
        writer.Write(value.get());
    }
}

void EvaluationContext::Read(Reader& reader, Context* context)
{
    trueValue.reset(new BoolValue(module, true, "true", context));
    trueValue->Read(reader);
    MapValue(trueValue.get());
    falseValue.reset(new BoolValue(module, false, "false", context));
    falseValue->Read(reader);
    MapValue(falseValue.get());
    nullPtrValue.reset(new NullPtrValue(module, context));
    nullPtrValue->Read(reader);
    MapValue(nullPtrValue.get());
}

BoolValue* EvaluationContext::GetBoolValue(bool value)
{
    if (value) return trueValue.get(); else return falseValue.get();
}

IntegerValue* EvaluationContext::GetIntegerValue(std::int64_t value, const std::string& rep, TypeSymbol* type, Context* context)
{
    auto it = integerValueMap.find(std::make_pair(std::make_pair(value, rep), type));
    if (it != integerValueMap.cend())
    {
        return it->second;
    }
    else
    {
        IntegerValue* integerValue = new IntegerValue(GetModule(), value, rep, context);
        integerValue->SetType(type, context);
        integerValueMap[std::make_pair(std::make_pair(value, rep), type)] = integerValue;
        values.push_back(std::unique_ptr<Value>(integerValue));
        MapValue(integerValue);
        return integerValue;
    }
}

FloatingValue* EvaluationContext::GetFloatingValue(double value, const std::string& rep, TypeSymbol* type, Context* context)
{
    auto it = floatingValueMap.find(std::make_pair(std::make_pair(value, rep), type));
    if (it != floatingValueMap.cend())
    {
        return it->second;
    }
    else
    {
        FloatingValue* floatingValue = new FloatingValue(GetModule(), value, rep, context);
        floatingValue->SetType(type, context);
        floatingValueMap[std::make_pair(std::make_pair(value, rep), type)] = floatingValue;
        values.push_back(std::unique_ptr<Value>(floatingValue));
        MapValue(floatingValue);
        return floatingValue;
    }
}

NullPtrValue* EvaluationContext::GetNullPtrValue()
{
    return nullPtrValue.get();
}

StringValue* EvaluationContext::GetStringValue(const std::string& value, TypeSymbol* type, Context* context)
{
    auto it = stringValueMap.find(std::make_pair(value, type));
    if (it != stringValueMap.cend())
    {
        return it->second;
    }
    else
    {
        std::string escapedValue;
        for (char c : value)
        {
            if (c == '\\')
            {
                escapedValue.append(2, '\\');
            }
            else
            {
                escapedValue.append(1, c);
            }
        }
        StringValue* stringValue = new StringValue(GetModule(), escapedValue, context);
        stringValue->SetType(type, context);
        stringValueMap[std::make_pair(value, type)] = stringValue;
        values.push_back(std::unique_ptr<Value>(stringValue));
        MapValue(stringValue);
        return stringValue;
    }
}

CharValue* EvaluationContext::GetCharValue(char32_t value, TypeSymbol* type, Context* context)
{
    auto it = charValueMap.find(std::make_pair(value, type));
    if (it != charValueMap.cend())
    {
        return it->second;
    }
    else
    {
        CharValue* charValue = new CharValue(GetModule(), value, context);
        charValue->SetType(type, context);
        charValueMap[std::make_pair(value, type)] = charValue;
        values.push_back(std::unique_ptr<Value>(charValue));
        MapValue(charValue);
        return charValue;
    }
}

SymbolValue* EvaluationContext::GetSymbolValue(Symbol* symbol, Context* context)
{
    auto it = symbolValueMap.find(symbol);
    if (it != symbolValueMap.cend())
    {
        return it->second;
    }
    else
    {
        SymbolValue* symbolValue = new SymbolValue(GetModule(), symbol, context);
        symbolValueMap[symbol] = symbolValue;
        values.push_back(std::unique_ptr<Value>(symbolValue));
        MapValue(symbolValue);
        return symbolValue;
    }
}

InvokeValue* EvaluationContext::GetInvokeValue(Value* subject, Context* context)
{
    auto it = invokeMap.find(subject);
    if (it != invokeMap.cend())
    {
        return it->second;
    }
    else
    {
        InvokeValue* invokeValue = new InvokeValue(GetModule(), subject, context);
        invokeMap[subject] = invokeValue;
        values.push_back(std::unique_ptr<Value>(invokeValue));
        MapValue(invokeValue);
        return invokeValue;
    }
}

ArrayValue* EvaluationContext::GetArrayValue(TypeSymbol* type, Context* context)
{
    ArrayValue* arrayValue = new ArrayValue(GetModule(), context);
    arrayValue->SetType(type, context);
    values.push_back(std::unique_ptr<Value>(arrayValue));
    return arrayValue;
}

StructureValue* EvaluationContext::GetStructureValue(TypeSymbol* type, Context* context)
{
    StructureValue* structureValue = new StructureValue(GetModule(), context);
    structureValue->SetType(type, context);
    values.push_back(std::unique_ptr<Value>(structureValue));
    return structureValue;
}

void EvaluationContext::MapValue(Value* value)
{
    valueMap[value->Id()] = value;
}

void EvaluationContext::AddValue(Value* value)
{
    values.push_back(std::unique_ptr<Value>(value));
    MapValue(value);
}

Value* EvaluationContext::GetValue(SymbolId valueId)
{
    auto it = valueMap.find(valueId);
    if (it != valueMap.end())
    {
        return it->second;
    }
    return nullptr;
}

} // namespace otava::symbols
