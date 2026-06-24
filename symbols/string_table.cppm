// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.string_table;

import otava.symbols.id;
import otava.symbols.writer;
import otava.symbols.reader;
import std;

export namespace otava::symbols {

class Module;

struct StringTableHeader
{
    StringTableHeader();
    void Write(Writer& writer);
    void Read(Reader& reader);
    FileOffset start;
    Length length;
    Cardinality count;
};

class StringTable
{
public:
    StringTable(Module* module_);
    Length GetLength() const noexcept { return length; }
    StringOffset GetOffset(const std::string& s);
    StringOffset AddString(const std::string& s);
    std::string GetString(StringOffset offset);
    const char* CharPtr(StringOffset offset);
    void Write(Writer& writer);
    void Read(Reader& reader);
private:
    Module* module;
    Length length;
    std::vector<std::string> strings;
    std::unordered_map<std::string, StringOffset> stringMap;
    std::unordered_map<StringOffset, std::string> offsetMap;
    StringTableHeader header;
    bool headerRead;
    void ReadHeader();
    bool stringsRead;
    void ReadStrings();
};

} // namespace otava::symbols
