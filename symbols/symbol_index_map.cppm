// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.symbol_index_map;

import std;
import otava.symbols.id;
import otava.symbols.symbol;

export namespace otava::symbols {

class SymbolIndexMap
{
public:
    SymbolIndexMap();
    SymbolIndexMap(Module* module_);
    void Write(Writer& writer);
    void Read(Reader& reader);
    Index GetNextIndex(SymbolKind symbolKind);
    inline Index GetIndex(SymbolKind symbolKind) const noexcept
    {
        return indexMap[ToUnderlying(symbolKind)];
    }
    void Import(const SymbolIndexMap& that);
private:
    Module* module;
    Index indexMap[ToUnderlying(SymbolKind::max)];
};

} // namespace otava::symbols
