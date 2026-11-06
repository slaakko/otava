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

} // namespace otava::symbols
