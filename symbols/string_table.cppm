// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.string_table;

import otava.symbols.id;
import otava.symbols.writer;
import std;

export namespace otava::symbols {

class StringTable
{
public:
    StringTable();
    void SetOffsets(const std::uint8_t* start_, Length length_);
    inline std::uint32_t GetLength() const noexcept { return ToUnderlying(nextOffset); }
    StringOffset GetOffset(const std::string& s) const;
    StringOffset AddString(const std::string& s);
    std::string GetString(StringOffset offset) const;
    void Write(Writer& writer);
private:
    const std::uint8_t* start;
    StringOffset nextOffset;
    std::vector<std::string> strings;
    std::unordered_map<std::string, StringOffset> stringMap;
    std::unordered_map<StringOffset, std::string> offsetMap;
};

} // namespace otava::symbols
