// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.string_table;

import otava.symbols.writer;
import util.memory_reader;
import util.binary_stream_writer;
import util.utility;

namespace otava::symbols {

StringTable::StringTable() : start(nullptr), nextOffset(StringOffset(0))
{
}

void StringTable::SetOffsets(const std::uint8_t* start_, Length length_)
{
    start = start_;
    nextOffset = StringOffset(length_);
}

StringOffset StringTable::GetOffset(const std::string& s) const
{
    auto it = stringMap.find(s);
    if (it != stringMap.end())
    {
        return it->second;
    }
    return notFoundOffset;
}

StringOffset StringTable::AddString(const std::string& s)
{
    StringOffset offset = GetOffset(s);
    if (offset != notFoundOffset) return offset;
    offset = nextOffset;
    strings.push_back(s);
    stringMap[s] = offset;
    if (!start)
    {
        offsetMap[offset] = s;
    }
    nextOffset = offset + s.length() + 1;
    return offset;
}

std::string StringTable::GetString(StringOffset offset) const
{
    if (start)
    {
        const std::uint8_t* pos = util::Advance(start, ToUnderlying(offset));
        util::MemoryReader reader(pos, std::uint32_t(start + GetLength() - pos));
        return reader.ReadString();
    }
    else
    {
        auto it = offsetMap.find(offset);
        if (it != offsetMap.end())
        {
            return it->second;
        }
    }
    return std::string();
}

void StringTable::Write(Writer& writer)
{
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    for (const auto& s : strings)
    {
        binaryStreamWriter.Write(s);
    }
}

} // namespace otava::symbols
