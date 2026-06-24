// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.conversion_table;

import otava.symbols.function_symbol;
import otava.symbols.modules;
import otava.symbols.type_symbol;
import otava.symbols.variable_symbol;
import otava.symbols.writer;
import otava.symbols.reader;
import util.utility;

namespace otava::symbols {

ConversionTable::ConversionTable(Module* module_) : module(module_), read(false)
{
}

void ConversionTable::AddConversion(FunctionSymbol* conversion, Context* context)
{
    TypeSymbol* paramType = conversion->GetConversionParamType(context);
    TypeSymbol* argType = conversion->GetConversionArgType(context);
    ConversionTableEntry entry(ToUnderlying(paramType->Id()), ToUnderlying(argType->Id()));
    conversionMap[entry] = ToUnderlying(conversion->Id());
}

FunctionSymbol* ConversionTable::GetConversion(TypeSymbol* paramType, TypeSymbol* argType, Context* context) 
{
    if (module->IsReadOnly())
    {
        Read();
    }
    ConversionTableEntry entry(ToUnderlying(paramType->Id()), ToUnderlying(argType->Id()));
    auto it = conversionMap.find(entry);
    if (it != conversionMap.end())
    {
        SymbolId functionSymbolId = SymbolId(it->second);
        return module->GetSymbolTable()->GetFunctionSymbol(functionSymbolId, context);
    }
    return nullptr;
}

void ConversionTable::Write(Writer& writer)
{
    Cardinality count = Cardinality(conversionMap.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (const auto& e : conversionMap)
    {
        const ConversionTableEntry& entry = e.first;
        std::uint32_t functionId = e.second;
        writer.GetBinaryStreamWriter().Write(entry.paramTypeId);
        writer.GetBinaryStreamWriter().Write(entry.argTypeId);
        writer.GetBinaryStreamWriter().Write(functionId);
    }
}

void ConversionTable::Read()
{
    if (read) return;
    read = true;
    Reader reader(module->GetFileMapping());
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(module->GetConversionTableOffset())), module->GetConversionTableLength());
    Read(reader);
    reader.PopCurrentReader();
}

void ConversionTable::Read(Reader& reader)
{
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        ConversionTableEntry entry;
        entry.paramTypeId = reader.CurrentReader().ReadUInt();
        entry.argTypeId = reader.CurrentReader().ReadUInt();
        std::uint32_t functionId = reader.CurrentReader().ReadUInt();
        conversionMap[entry] = functionId;
    }
}

} // otava::symbols
