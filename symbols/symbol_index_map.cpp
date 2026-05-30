// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.symbol_index_map;

import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.reader;
import otava.symbols.writer;
import util.utility;

namespace otava::symbols {

SymbolIndexMap::SymbolIndexMap() : SymbolIndexMap(nullptr)
{
}

SymbolIndexMap::SymbolIndexMap(Module* module_) : module(module_)
{
    auto start = ToUnderlying(SymbolKind::null);
    auto end = ToUnderlying(SymbolKind::max);
    for (auto i = start; i < end; ++i)
    {
        indexMap[ToUnderlying(SymbolKind(i))] = Index(1);
    }
}

void SymbolIndexMap::Write(Writer& writer)
{
    Cardinality count = Cardinality(ToUnderlying(SymbolKind::max));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (const auto& index : indexMap)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(index));
    }
}

void SymbolIndexMap::Read(Reader& reader)
{
    if (!module)
    {
        ThrowException("cannot read symbol index map because module not set");
    }
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(module->GetSymbolIndexMapOffset())), module->GetSymbolIndexMapLength());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        Index index = Index(reader.CurrentReader().ReadUInt());
        indexMap[ToUnderlying(i)] = index;
    }
    reader.PopCurrentReader();
}

Index SymbolIndexMap::GetNextIndex(SymbolKind symbolKind)
{
    Index index = GetIndex(symbolKind);
    Index next = index + Index(1);
    indexMap[ToUnderlying(symbolKind)] = next;
    return index;
}

void SymbolIndexMap::Import(const SymbolIndexMap& that)
{
    auto start = ToUnderlying(SymbolKind::null);
    auto end = ToUnderlying(SymbolKind::max);
    for (auto i = start; i < end; ++i)
    {
        indexMap[ToUnderlying(SymbolKind(i))] = GetIndex(SymbolKind(i)) + that.GetIndex(SymbolKind(i)) - Index(1);
    }
}

} // namespace otava::symbols
