// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.specialization;

import otava.symbols.id;
import std;

export namespace otava::symbols {

class Writer;
class Reader;
class Symbol;
class TypeSymbol;

struct SpecializationKey
{
    SpecializationKey();
    void Write(Writer& writer);
    void Read(Reader& reader);
    SymbolId typeSymbolId;
    std::vector<SymbolId> templateArgumentIds;
};

struct SpecializationKeyHash
{
    size_t operator()(const SpecializationKey& key) const noexcept;
};

struct SpecializationKeyEqual
{
    bool operator()(const SpecializationKey& left, const SpecializationKey& right) const noexcept;
};

std::string MakeSpecializationName(TypeSymbol* templateTypeSymbol, const std::vector<Symbol*>& templateArguments);

} // namespace otava::symbols
