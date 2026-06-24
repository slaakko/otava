// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.string_table;

import otava.symbols.modules;
import otava.symbols.writer;
import util.memory_reader;
import util.binary_stream_writer;
import util.utility;

namespace otava::symbols {

StringTableHeader::StringTableHeader() : start(FileOffset(0)), length(Length(0)), count(Cardinality(0))
{
}

void StringTableHeader::Write(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(start));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(length));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
}

void StringTableHeader::Read(Reader& reader)
{
    start = FileOffset(reader.CurrentReader().ReadInt());
    length = Length(reader.CurrentReader().ReadInt());
    count = Cardinality(reader.CurrentReader().ReadInt());
}

StringTable::StringTable(Module* module_) : module(module_), length(Length(0)), headerRead(false), stringsRead(false)
{
}

StringOffset StringTable::GetOffset(const std::string& s) 
{
    if (module->IsReadOnly())
    {
        ReadStrings();
    }
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
    offset = StringOffset(length);
    strings.push_back(s);
    stringMap[s] = offset;
    offsetMap[offset] = s;
    length = Length(offset + s.length() + 1);
    return offset;
}

std::string StringTable::GetString(StringOffset offset)
{
    if (module->IsReadOnly())
    {
        ReadHeader();
        const std::uint8_t* pos = util::Advance(module->GetFileMapping()->Start(), ToUnderlying(header.start) + ToUnderlying(offset));
        util::MemoryReader reader(pos, std::int32_t(util::Advance(module->GetFileMapping()->Start(), 
            ToUnderlying(header.start) + ToUnderlying(header.length)) - pos));
        std::string str = reader.ReadString();
        return str;
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

const char* StringTable::CharPtr(StringOffset offset)
{
    if (module->IsReadOnly())
    {
        ReadHeader();
        const std::uint8_t* pos = util::Advance(module->GetFileMapping()->Start(), ToUnderlying(header.start) + ToUnderlying(offset));
        return reinterpret_cast<const char*>(pos);
    }
    else
    {
        auto it = offsetMap.find(offset);
        if (it != offsetMap.end())
        {
            return it->second.c_str();
        }
    }
    return "";
}

void StringTable::Write(Writer& writer)
{
    FileOffset start = FileOffset(writer.Position());
    header.Write(writer);
    FileOffset contentStart = FileOffset(writer.Position());
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    for (const auto& s : strings)
    {
        binaryStreamWriter.Write(s);
    }
    FileOffset end = FileOffset(writer.Position());
    Length length = Length(end - contentStart);
    header.start = contentStart;
    header.length = length;
    header.count = Cardinality(strings.size());
    writer.Seek(ToUnderlying(start));
    header.Write(writer);
    writer.Seek(ToUnderlying(end));
}

void StringTable::ReadHeader()
{
    if (headerRead) return;
    headerRead = true;
    if (!module->IsReadOnly()) return;
    Reader reader(module->GetFileMapping());
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(module->GetStringTableOffset())), module->GetStringTableLength());
    Read(reader);
    reader.PopCurrentReader();
}

void StringTable::Read(Reader& reader)
{
    header.Read(reader);
}

void StringTable::ReadStrings()
{
    if (stringsRead) return;
    stringsRead = true;
    Cardinality count = header.count;
    StringOffset offset = StringOffset(0);
    for (Index i = Index(0); i < Index(count); ++i)
    {
        std::string str = GetString(offset);
        stringMap[str] = offset;
        offset += str.length() + 1;
    }
}

} // namespace otava::symbols
