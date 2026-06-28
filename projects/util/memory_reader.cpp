module util.memory_reader;

namespace util {

MemoryReader::MemoryReader(const std::uint8_t* ptr_, std::int64_t count_) noexcept : ptr(ptr_), pos(ptr), count(count_)
{
}

std::uint8_t MemoryReader::ReadByte()
{
    if (pos - ptr >= count)
    {
        throw std::runtime_error("memory reader: unexpected end of data");
    }
    return *pos++;
}

std::int8_t MemoryReader::ReadSByte()
{
    return static_cast<std::int8_t>(ReadByte());
}

std::uint16_t MemoryReader::ReadUShort()
{
    std::uint8_t b0 = ReadByte();
    std::uint8_t b1 = ReadByte();
    return (static_cast<std::uint16_t>(b0) << 8u) | static_cast<std::uint16_t>(b1);
}

std::int16_t MemoryReader::ReadShort()
{
    return static_cast<std::int16_t>(ReadUShort());
}

std::uint32_t MemoryReader::ReadUInt()
{
    std::uint8_t b0 = ReadByte();
    std::uint8_t b1 = ReadByte();
    std::uint8_t b2 = ReadByte();
    std::uint8_t b3 = ReadByte();
    return (static_cast<std::uint32_t>(b0) << 24u) | (static_cast<std::uint32_t>(b1) << 16u) | (static_cast<std::uint32_t>(b2) << 8u) | static_cast<std::uint32_t>(b3);
}

std::int32_t MemoryReader::ReadInt()
{
    return static_cast<std::int32_t>(ReadUInt());
}

std::uint64_t MemoryReader::ReadULong()
{
    std::uint8_t b0 = ReadByte();
    std::uint8_t b1 = ReadByte();
    std::uint8_t b2 = ReadByte();
    std::uint8_t b3 = ReadByte();
    std::uint8_t b4 = ReadByte();
    std::uint8_t b5 = ReadByte();
    std::uint8_t b6 = ReadByte();
    std::uint8_t b7 = ReadByte();
    return (static_cast<std::uint64_t>(b0) << 56u) | (static_cast<std::uint64_t>(b1) << 48u) | (static_cast<std::uint64_t>(b2) << 40u) | (static_cast<std::uint64_t>(b3) << 32u) |
        (static_cast<std::uint64_t>(b4) << 24u) | (static_cast<std::uint64_t>(b5) << 16u) | (static_cast<std::uint64_t>(b6) << 8u) | static_cast<std::uint64_t>(b7);
}

std::int64_t MemoryReader::ReadLong()
{
    return static_cast<std::int64_t>(ReadULong());
}

DateTime MemoryReader::ReadDateTime()
{
    std::int16_t year = ReadShort();
    Month month = static_cast<Month>(ReadSByte());
    std::int8_t day = ReadSByte();
    Date date(year, month, day);
    std::int32_t secs = ReadInt();
    DateTime dt(date, secs);
    return dt;
}

std::string MemoryReader::ReadString()
{
    std::string result;
    std::uint8_t b = ReadByte();
    while (b != 0u)
    {
        result.append(1, static_cast<char>(b));
        b = ReadByte();
    }
    return result;
}

} // namespace util
