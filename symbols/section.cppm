// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.section;

import otava.symbols.id;
import otava.symbols.symbol;
import std;

export namespace otava::symbols {

class Module;
class Reader;
class Writer;
class Symbol;

enum class SectionKind : std::uint8_t
{
    none, namespaceSection, typeSection, functionSection, variableSection, max, first = namespaceSection
};

constexpr std::uint8_t ToUnderlying(SectionKind sectionKind) { return std::uint8_t(sectionKind); }

constexpr std::uint8_t operator-(SectionKind x, SectionKind y)
{
    return ToUnderlying(x) - ToUnderlying(y);
}

SectionKind ToSectionKind(SymbolKind symbolKind);
SectionKind GetSectionKind(Symbol* symbol);

struct SectionHeader
{
    SectionHeader();
    void Write(Writer& writer);
    void Read(Reader& reader);
    FileOffset entryMapOffset;
    Length entryMapLength;
};

struct SectionEntry
{
    SectionEntry();
    void Write(Writer& writer);
    void Read(Reader& reader);
    FileOffset fileOffset;
    Length length;
};

class Section
{
public:
    Section(Module* module_, SectionKind kind_);
    inline void SetReadOnly() noexcept { readOnly = true; }
    inline Module* GetModule() const noexcept { return module; }
    inline SectionKind Kind() const noexcept { return kind; }
    Symbol* GetSymbol(SymbolId symbolId, Context* context);
    void MapSymbol(Symbol* symbol);
    void AddEntry(Symbol* symbol, const SectionEntry& entry);
    void Write(Writer& writer);
private:
    Module* module;
    SectionKind kind;
    std::unordered_map<SymbolId, SectionEntry> entryMap;
    std::unordered_map<SymbolId, Symbol*> symbolMap;
    std::vector<std::unique_ptr<Symbol>> symbols;
    bool headerRead;
    bool entriesRead;
    bool readOnly;
    void ReadHeader();
    void ReadEntries();
};

} // namespace otava::symbols
