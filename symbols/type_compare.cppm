// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.type_compare;

import std;

export namespace otava::symbols {

class TypeSymbol;
class Context;

struct TypeIdLess
{
    bool operator()(TypeSymbol* left, TypeSymbol* right) const noexcept;
};

bool TypesEqual(TypeSymbol* left, TypeSymbol* right, Context* context) noexcept;

} // namespace otava::symbols
