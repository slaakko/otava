// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.assembly.symbol;

import otava.assembly.value;
import std;

export namespace otava::assembly {

class Symbol : public Value
{
public:
    Symbol(const std::string& name_);
};

} // otava::assembly
