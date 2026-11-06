// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.symbol_index_map;

import std;
import otava.symbols.id;
import otava.symbols.symbol;

export namespace otava::symbols {

class SymbolCounts;

class SymbolIndexMap
{
public:
    SymbolIndexMap();
    Index GetNextIndex(SymbolKind symbolKind);
    inline Index GetIndex(SymbolKind symbolKind) const noexcept
    {
        return indexMap[ToUnderlying(symbolKind)];
    }
    inline void SetIndex(SymbolKind symbolKind, Index index)
    {
        indexMap[ToUnderlying(symbolKind)] = index;
    }
    void AddCounts(const SymbolCounts& counts);
    void Save(const std::string& symbolIndexFilePath);
    void Load(const std::string& symbolIndexFilePath);
private:
#ifdef OTAVA
    std::vector<Index> indexMap;
#else
    Index indexMap[ToUnderlying(SymbolKind::max)];
#endif
};

class SymbolCounts
{
public:
    SymbolCounts();
    inline Cardinality GetCount(SymbolKind symbolKind) const noexcept
    {
        return counts[ToUnderlying(symbolKind)];
    }
    void IncCount(SymbolKind symbolKind);
    void Save(const std::string& scountFilePath);
    void Load(const std::string& scountFilePath);
private:
#ifdef OTAVA
    std::vector<Cardinality> counts;
#else
    Cardinality counts[ToUnderlying(SymbolKind::max)];
#endif
};

} // namespace otava::symbols
