// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.symbol_factory;

import otava.symbols.exception;
import otava.symbols.namespaces;
import otava.symbols.fundamental_type_symbol;

namespace otava::symbols {

Symbol* MakeSymbol(Module* module, SymbolId symbolId)
{
    SymbolKind kind = GetSymbolKind(symbolId);
    switch (kind)
    {
    case SymbolKind::namespaceSymbol:
    {
        return new NamespaceSymbol(module, symbolId);
    }
    case SymbolKind::fundamentalTypeSymbol:
    {
        return new FundamentalTypeSymbol(module, symbolId);
    }
    }
    ThrowException("symbol factory: could not make symbol for symbol kind '" + SymbolKindStr(kind) + "': invalid kind");
}

} // namespace otava::symbols
