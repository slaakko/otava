// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.evaluation_context;

import otava.symbols.concrete_value;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.fundamental_type_kind;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.modules;

namespace otava::symbols {

EvaluationStack::EvaluationStack()
{
}

Value* EvaluationStack::Pop()
{
    if (stack.empty())
    {
        ThrowException("evaluation stack is empty");
    }
    Value* top = stack.top();
    stack.pop();
    return top;
}

EvaluationMap::EvaluationMap(EvaluationMap* parentMap_) : parentMap(parentMap_)
{
}

Value* EvaluationMap::GetValue(const std::string& symbol) const noexcept
{
    auto it = map.find(symbol);
    if (it != map.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void EvaluationMap::SetValue(const std::string& symbol, Value* value)
{
    map[symbol] = value;
}

EvaluationContext::EvaluationContext(Module* module_, bool readOnly_) : 
    module(module_), initialized(false), readOnly(readOnly_), currentEvaluationMap(nullptr), nullPtrValue(nullptr)
{
}

EvaluationContext::~EvaluationContext()
{
    NullPtrValue* nv = nullPtrValue;
    if (nv)
    {
        delete nv;
    }
}

void EvaluationContext::ResetEvaluationStack()
{
    evaluationStack.reset(new EvaluationStack());
}

void EvaluationContext::ResetEvaluationMaps()
{
    evaluationMapStack.clear();
    currentEvaluationMap = nullptr;
}

void EvaluationContext::PushEvaluationMap()
{
    EvaluationMap* evaluationMap = new EvaluationMap(currentEvaluationMap.get());
    evaluationMapStack.push_back(std::unique_ptr<EvaluationMap>(currentEvaluationMap.release()));
    currentEvaluationMap.reset(evaluationMap);
}

void EvaluationContext::PopEvaluationMap()
{
    if (evaluationMapStack.empty())
    {
        ThrowException("evaluation map stack is empty");
    }
    currentEvaluationMap.reset(evaluationMapStack.back().release());
    evaluationMapStack.pop_back();
}

void EvaluationContext::Init(Context* context)
{
    if (initialized) return;
    initialized = true;
    trueValue.reset(new BoolValue(module, true, context));
    TypeSymbol* boolType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context);
    trueValue->SetType(boolType, context);
    falseValue.reset(new BoolValue(module, false, context));
    falseValue->SetType(boolType, context);
    nullPtrValue = new NullPtrValue(module, context);
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
        if (value->IsSerializableValue())
        {
            writer.Write(value.get());
        }
    }
}

void EvaluationContext::Read(Reader& reader, Context* context)
{
    trueValue.reset(new BoolValue(module, true, context));
    trueValue->Read(reader);
    MapValue(trueValue.get());
    falseValue.reset(new BoolValue(module, false, context));
    falseValue->Read(reader);
    MapValue(falseValue.get());
    nullPtrValue = new NullPtrValue(module, context);
    nullPtrValue->Read(reader);
    MapValue(nullPtrValue);
}

BoolValue* EvaluationContext::GetBoolValue(bool value)
{
    if (value) return trueValue.get(); else return falseValue.get();
}

Value* EvaluationContext::GetIntegerValue(std::uint64_t value, TypeSymbol* type, Context* context)
{
    auto it = integerValueMap.find(std::make_pair(value, type));
    if (it != integerValueMap.cend())
    {
        return it->second;
    }
    else
    {
        if (type->IsFundamentalTypeSymbol())
        {
            FundamentalTypeSymbol* fundamentalType = static_cast<FundamentalTypeSymbol*>(type);
            FundamentalTypeKind fundamentalTypeKind = fundamentalType->GetFundamentalTypeKind();
            Value* v = nullptr;
            bool unsupported = false;
            switch (fundamentalTypeKind)
            {
            case FundamentalTypeKind::charType:
            case FundamentalTypeKind::char8Type:
            case FundamentalTypeKind::unsignedCharType:
            {
                v = new FundamentalTypeValue<std::uint8_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::byteValueSymbol), fundamentalTypeKind,
                    static_cast<std::uint8_t>(value), context);
                break;
            }
            case FundamentalTypeKind::signedCharType:
            {
                v = new FundamentalTypeValue<std::int8_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::sbyteValueSymbol), fundamentalTypeKind,
                    static_cast<std::int8_t>(value), context);
                break;
            }
            case FundamentalTypeKind::char16Type:
            case FundamentalTypeKind::unsignedShortIntType:
            case FundamentalTypeKind::wcharType:
            {
                v = new FundamentalTypeValue<std::uint16_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::ushortValueSymbol), fundamentalTypeKind,
                    static_cast<std::uint16_t>(value), context);
                break;
            }
            case FundamentalTypeKind::shortIntType:
            {
                v = new FundamentalTypeValue<std::int16_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::shortValueSymbol), fundamentalTypeKind,
                    static_cast<std::int16_t>(value), context);
                break;
            }
            case FundamentalTypeKind::char32Type:
            case FundamentalTypeKind::unsignedIntType:
            case FundamentalTypeKind::unsignedLongIntType:
            {
                v = new FundamentalTypeValue<std::uint32_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::uintValueSymbol), fundamentalTypeKind,
                    static_cast<std::uint32_t>(value), context);
                break;
            }
            case FundamentalTypeKind::intType:
            case FundamentalTypeKind::longIntType:
            {
                v = new FundamentalTypeValue<std::int32_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::intValueSymbol), fundamentalTypeKind,
                    static_cast<std::int32_t>(value), context);
                break;
            }
            case FundamentalTypeKind::longLongIntType:
            {
                v = new FundamentalTypeValue<std::int64_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::longValueSymbol), fundamentalTypeKind,
                    static_cast<std::int64_t>(value), context);
                break;
            }
            case FundamentalTypeKind::unsignedLongLongIntType:
            {
                v = new FundamentalTypeValue<std::uint64_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::ulongValueSymbol), fundamentalTypeKind,
                    static_cast<std::uint64_t>(value), context);
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
                ThrowException("otava.symbols.evaluation_context: GetIntegerValue(): unsupported fundamental type");
            }
            integerValueMap[std::make_pair(value, type)] = v;
            values.push_back(std::unique_ptr<Value>(v));
            MapValue(v);
            return v;
        }
        else
        {
            ThrowException("otava.symbols.evaluation_context: GetIntegerValue(): fundamental type expected");
        }
    }
    return nullptr;
}

Value* EvaluationContext::GetFloatingValue(double value, TypeSymbol* type, Context* context)
{
    auto it = floatingValueMap.find(std::make_pair(value, type));
    if (it != floatingValueMap.cend())
    {
        return it->second;
    }
    else
    {
        if (type->IsFundamentalTypeSymbol())
        {
            FundamentalTypeSymbol* fundamentalType = static_cast<FundamentalTypeSymbol*>(type);
            FundamentalTypeKind fundamentalTypeKind = fundamentalType->GetFundamentalTypeKind();
            Value* v = nullptr;
            bool unsupported = false;
            switch (fundamentalTypeKind)
            {
            case FundamentalTypeKind::floatType:
            {
                v = new FundamentalTypeValue<float>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::floatValueSymbol), fundamentalTypeKind,
                    static_cast<float>(value), context);
                break;
            }
            case FundamentalTypeKind::doubleType:
            {
                v = new FundamentalTypeValue<double>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::doubleValueSymbol), fundamentalTypeKind,
                    static_cast<double>(value), context);
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
                ThrowException("otava.symbols.evaluation_context: GetFloatingValue(): unsupported fundamental type");
            }
            floatingValueMap[std::make_pair(value, type)] = v;
            values.push_back(std::unique_ptr<Value>(v));
            MapValue(v);
            return v;
        }
        else
        {
            ThrowException("otava.symbols.evaluation_context: GetFloatingValue(): fundamental type expected");
        }
    }
    return nullptr;
}

NullPtrValue* EvaluationContext::GetNullPtrValue()
{
    return nullPtrValue;
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
        std::unique_ptr<Value> svalue(stringValue);
        values.push_back(std::move(svalue));
        MapValue(stringValue);
        return stringValue;
    }
}

Value* EvaluationContext::GetCharValue(char32_t value, TypeSymbol* type, Context* context)
{
    auto it = charValueMap.find(std::make_pair(value, type));
    if (it != charValueMap.cend())
    {
        return it->second;
    }
    else
    {
        if (type->IsFundamentalTypeSymbol())
        {
            FundamentalTypeSymbol* fundamentalType = static_cast<FundamentalTypeSymbol*>(type);
            FundamentalTypeKind fundamentalTypeKind = fundamentalType->GetFundamentalTypeKind();
            Value* v = nullptr;
            bool unsupported = false;
            switch (fundamentalTypeKind)
            {
            case FundamentalTypeKind::charType:
            case FundamentalTypeKind::char8Type:
            case FundamentalTypeKind::unsignedCharType:
            {
                v = new FundamentalTypeValue<std::uint8_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::byteValueSymbol), fundamentalTypeKind,
                    static_cast<std::uint8_t>(value), context);
                break;
            }
            case FundamentalTypeKind::signedCharType:
            {
                v = new FundamentalTypeValue<std::int8_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::sbyteValueSymbol), fundamentalTypeKind,
                    static_cast<std::int8_t>(value), context);
                break;
            }
            case FundamentalTypeKind::char16Type:
            case FundamentalTypeKind::wcharType:
            {
                v = new FundamentalTypeValue<std::uint16_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::ushortValueSymbol), fundamentalTypeKind,
                    static_cast<std::uint16_t>(value), context);
                break;
            }
            case FundamentalTypeKind::char32Type:
            {
                v = new FundamentalTypeValue<std::uint32_t>(GetModule(),
                    context->GetNextSymbolId(SymbolKind::uintValueSymbol), fundamentalTypeKind,
                    static_cast<std::uint32_t>(value), context);
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
                ThrowException("otava.symbols.evaluation_context: GetCharValue(): unsupported fundamental type");
            }
            charValueMap[std::make_pair(value, type)] = v;
            values.push_back(std::unique_ptr<Value>(v));
            MapValue(v);
            return v;
        }
        else
        {
            ThrowException("otava.symbols.evaluation_context: GetCharValue(): fundamental type expected");
        }
    }
    return nullptr;
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

InvokeValue* EvaluationContext::GetInvokeValue(Value* subject, const std::vector<Value*>& arguments, Context* context)
{
    auto it = invokeMap.find(subject);
    if (it != invokeMap.cend())
    {
        return it->second;
    }
    else
    {
        InvokeValue* invokeValue = new InvokeValue(GetModule(), subject, arguments, context);
        invokeMap[subject] = invokeValue;
        values.push_back(std::unique_ptr<Value>(invokeValue));
        MapValue(invokeValue);
        return invokeValue;
    }
}

ArrayValue* EvaluationContext::GetArrayValue(TypeSymbol* type, Context* context)
{
    ArrayValue* arrayValue = new ArrayValue(GetModule(), context);
    arrayValue->SetType(type->PlainType(context), context);
    values.push_back(std::unique_ptr<Value>(arrayValue));
    return arrayValue;
}

StructureValue* EvaluationContext::GetStructureValue(TypeSymbol* type, Context* context)
{
    StructureValue* structureValue = new StructureValue(GetModule(), context);
    structureValue->SetType(type->PlainType(context), context);
    values.push_back(std::unique_ptr<Value>(structureValue));
    return structureValue;
}

FunctionGroupValue* EvaluationContext::GetFunctionGroupValue(Module* module, SymbolId symbolId, FunctionGroupSymbol* functionGroup, FunctionSymbol* fn, Context* context)
{
    FunctionGroupValue* functionGroupValue = new FunctionGroupValue(module, symbolId, functionGroup, fn);
    values.push_back(std::unique_ptr<Value>(functionGroupValue));
    return functionGroupValue;
}

TypeValue* EvaluationContext::GetTypeValue(Module* module, SymbolId symbolId, TypeSymbol* type, Context* context)
{
    TypeValue* typeValue = new TypeValue(module, symbolId, type);
    typeValue->SetType(type, context);
    values.push_back(std::unique_ptr<Value>(typeValue));
    return typeValue;
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
