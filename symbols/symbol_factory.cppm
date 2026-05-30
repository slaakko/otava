// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.symbol_factory;

import std;
import otava.symbols.id;
import otava.symbols.symbol;

export namespace otava::symbols {

Symbol* MakeSymbol(Module* module, SymbolId symbolId);

} // namespace otava::symbols
