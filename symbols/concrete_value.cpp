// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.concrete_value;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.enums;
import otava.symbols.evaluation_context;
import otava.symbols.exception;
import otava.symbols.function_group_symbol;
import otava.symbols.function_symbol;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.modules;
import otava.symbols.writer;
import otava.symbols.reader;
import otava.symbols.symbol;
import otava.symbols.type_compare;
import otava.symbols.type_symbol;
import otava.symbols.variable_symbol;
import util.text_util;

namespace otava::symbols {

NullPtrValue::NullPtrValue(Module* module_, SymbolId id_) : Value(module_, id_)
{
}

NullPtrValue::NullPtrValue(Module* module_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::nullPtrValueSymbol), "nullptr")
{
    TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::nullPtrType, context);
    SetType(type, context);
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

Value* NullPtrValue::Clone(Context* context) const
{
    return static_cast<Value*>(const_cast<NullPtrValue*>(this));
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

Value* StringValue::Clone(Context* context) const
{
    Value* clone = new StringValue(GetModule(), GetValue(), context);
    context->GetEvaluationContext()->AddValue(clone);
    return clone;
}

SymbolValue::SymbolValue(Module* module_, SymbolId id_) : Value(module_, id_), symbol(nullptr), symbolId(zeroSymbolId)
{
}

SymbolValue::SymbolValue(Module* module_, Symbol* symbol_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::symbolValueSymbol), symbol_->Name()), symbol(symbol_), symbolId(zeroSymbolId)
{
    if (symbol->IsTypeSymbol())
    {
        TypeSymbol* type = static_cast<TypeSymbol*>(symbol);
        SetType(type, context);
    }
    else if (symbol->IsVariableSymbol())
    {
        VariableSymbol* vs = static_cast<VariableSymbol*>(symbol);
        SetType(vs->GetType(context), context);
    }
    else if (symbol->IsParameterSymbol())
    {
        ParameterSymbol* ps = static_cast<ParameterSymbol*>(symbol);
        SetType(ps->GetType(context), context);
    }
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
    symbolId = SymbolId(reader.CurrentReader().ReadULong());
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

InvokeValue::InvokeValue(Module* module_, SymbolId id_) : Value(module_, id_), subject(nullptr), subjectId(zeroSymbolId)
{
}

InvokeValue::InvokeValue(Module* module_, Value* subject_, const std::vector<Value*>& arguments_, Context* context) :
    Value(module_, context->GetNextSymbolId(SymbolKind::invokeValueSymbol), subject_->Name()), subject(subject_), subjectId(zeroSymbolId), arguments(arguments_)
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

otava::intermediate::Value* InvokeValue::IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (subject->IsSymbolValue() && arguments.size() == 1)
    {
        otava::intermediate::Value* conversionValue = emitter.EmitConversionValue(subject->GetType(context)->IrType(emitter, fullSpan, context),
            arguments.front()->IrValue(emitter, fullSpan, context));
        return conversionValue;
    }
    else
    {
        return Value::IrValue(emitter, fullSpan, context);
    }
}

void InvokeValue::Write(Writer& writer)
{
    Value::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(subject->Id()));
}

void InvokeValue::Read(Reader& reader)
{
    Value::Read(reader);
    subjectId = SymbolId(reader.CurrentReader().ReadULong());
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
    for (Index i = Index(0); i < ToIndex(count); ++i)
    {
        SymbolId elementValueId = SymbolId(reader.CurrentReader().ReadULong());
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
    for (Index i = Index(0); i < ToIndex(count); ++i)
    {
        SymbolId fieldValueId = SymbolId(reader.CurrentReader().ReadULong());
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

FunctionGroupValue::FunctionGroupValue(Module* module_, SymbolId symbolId_, FunctionGroupSymbol* functionGroup_, FunctionSymbol* fn_) :
    Value(module_, symbolId_), functionGroup(functionGroup_), fn(fn_)
{
}

TypeValue::TypeValue(Module* module_, SymbolId symbolId_, TypeSymbol* type_) : Value(module_, symbolId_), type(type_)
{
}

} // namespace otava::symbols
