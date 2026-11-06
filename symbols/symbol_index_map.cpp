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
import util.file_stream;
import util.binary_stream_writer;
import util.binary_stream_reader;

namespace otava::symbols {

SymbolIndexMap::SymbolIndexMap() 
{
    auto start = ToUnderlying(SymbolKind::null);
    auto end = ToUnderlying(SymbolKind::max);
#ifdef OTAVA
    indexMap.resize(end - start);
#endif
    for (auto i = start; i < end; ++i)
    {
        indexMap[ToUnderlying(SymbolKind(i))] = Index(1);
    }
}

Index SymbolIndexMap::GetNextIndex(SymbolKind symbolKind)
{
    Index index = GetIndex(symbolKind);
    Index next = index + Index(1);
    indexMap[ToUnderlying(symbolKind)] = next;
    return index;
}

void SymbolIndexMap::Save(const std::string& symbolIndexFilePath)
{
    util::FileStream fs(symbolIndexFilePath, util::OpenMode::binary | util::OpenMode::write);
    util::BinaryStreamWriter writer(fs);
    auto start = ToUnderlying(SymbolKind::null);
    auto end = ToUnderlying(SymbolKind::max);
    for (auto i = start; i < end; ++i)
    {
        Index index = GetIndex(SymbolKind(i));
        writer.Write(ToUnderlying(index));
    }
}

void SymbolIndexMap::Load(const std::string& symbolIndexFilePath)
{
    util::FileStream fs(symbolIndexFilePath, util::OpenMode::binary | util::OpenMode::read);
    util::BinaryStreamReader reader(fs);
    auto start = ToUnderlying(SymbolKind::null);
    auto end = ToUnderlying(SymbolKind::max);
    for (auto i = start; i < end; ++i)
    {
        Index index = Index(reader.ReadInt());
        SetIndex(SymbolKind(i), index);
    }
}

void SymbolIndexMap::AddCounts(const SymbolCounts& counts)
{
    auto start = ToUnderlying(SymbolKind::null);
    auto end = ToUnderlying(SymbolKind::max);
    for (auto i = start; i < end; ++i)
    {
        Cardinality count = counts.GetCount(SymbolKind(i));
        Index index = GetIndex(SymbolKind(i));
        Index sum = index + ToIndex(count);
        indexMap[ToUnderlying(SymbolKind(i))] = sum;
    }
}

SymbolCounts::SymbolCounts()
{
    auto start = ToUnderlying(SymbolKind::null);
    auto end = ToUnderlying(SymbolKind::max);
#ifdef OTAVA
    counts.resize(end - start);
#endif
    for (auto i = start; i < end; ++i)
    {
        counts[ToUnderlying(SymbolKind(i))] = Cardinality(0);
    }
}

void SymbolCounts::IncCount(SymbolKind symbolKind)
{
    Cardinality count = GetCount(symbolKind);
    ++count;
    counts[ToUnderlying(symbolKind)] = count;
}

void SymbolCounts::Save(const std::string& scountFilePath)
{
    util::FileStream fs(scountFilePath, util::OpenMode::binary | util::OpenMode::write);
    util::BinaryStreamWriter writer(fs);
    auto start = ToUnderlying(SymbolKind::null);
    auto end = ToUnderlying(SymbolKind::max);
    for (auto i = start; i < end; ++i)
    {
        Cardinality count = GetCount(SymbolKind(i));
        writer.Write(ToUnderlying(count));
    }
}

void SymbolCounts::Load(const std::string& scountFilePath)
{
    util::FileStream fs(scountFilePath, util::OpenMode::binary | util::OpenMode::read);
    util::BinaryStreamReader reader(fs);
    auto start = ToUnderlying(SymbolKind::null);
    auto end = ToUnderlying(SymbolKind::max);
    for (auto i = start; i < end; ++i)
    {
        Cardinality count = Cardinality(reader.ReadUInt());
        counts[i] = count;
    }
}

} // namespace otava::symbols
