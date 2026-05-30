// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.conversion_table;

import otava.symbols.id;
import std;

export namespace otava::symbols {

class Context;
class FunctionSymbol;
class Module;
class TypeSymbol;
class Writer;
class Reader;

struct ConversionTableEntry
{
    ConversionTableEntry() noexcept : paramTypeId(ToUnderlying(zeroSymbolId)), argTypeId(ToUnderlying(zeroSymbolId)) {}
    ConversionTableEntry(std::uint32_t paramTypeId_, std::uint32_t argTypeId_) noexcept : paramTypeId(paramTypeId_), argTypeId(argTypeId_) {}
    std::uint32_t paramTypeId;
    std::uint32_t argTypeId;
};

struct ConversionTableEntryHash
{
    inline size_t operator()(const ConversionTableEntry& entry) const noexcept
    {
        size_t hashCode = std::hash<std::uint32_t>()(entry.paramTypeId) ^ std::hash<std::uint32_t>()(entry.argTypeId);
        return hashCode;
    }
};

struct ConversionTableEntryEqual
{
    inline bool operator()(const ConversionTableEntry& left, const ConversionTableEntry& right) const noexcept
    {
        return left.paramTypeId == right.paramTypeId && left.argTypeId == right.argTypeId;
    }
};

class ConversionTable
{
public:    
    ConversionTable(Module* module_);
    void AddConversion(FunctionSymbol* conversion);
    FunctionSymbol* GetConversion(TypeSymbol* paramType, TypeSymbol* argType, Context* context);
    void Write(Writer& writer);
    void Read(Reader& reader);
private:
    Module* module;
    std::unordered_map<ConversionTableEntry, std::uint32_t, ConversionTableEntryHash, ConversionTableEntryEqual> conversionMap;
    bool read;
    void Read();
};

} // otava::symbols
