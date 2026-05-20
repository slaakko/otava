// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.binary.stream.reader;

import util.unicode;

namespace util {

BinaryStreamReader::BinaryStreamReader(Stream& stream_) noexcept : stream(stream_)
{
}

bool BinaryStreamReader::ReadBool()
{
    return static_cast<bool>(ReadByte());
}

std::uint8_t BinaryStreamReader::ReadByte()
{
    int x = stream.ReadByte();
    if (x == -1)
    {
        throw std::runtime_error("unexpected end of stream");
    }
    return static_cast<std::uint8_t>(x);
}

std::int8_t BinaryStreamReader::ReadSByte()
{
    return static_cast<std::int8_t>(ReadByte());
}

std::uint16_t BinaryStreamReader::ReadUShort()
{
    std::uint8_t h = ReadByte();
    std::uint8_t l = ReadByte();
    return (static_cast<std::uint16_t>(h) << 8) | static_cast<std::uint16_t>(l);
}

std::int16_t BinaryStreamReader::ReadShort()
{
    std::uint16_t x = ReadUShort();
    return static_cast<std::int16_t>(x);
}

std::uint32_t BinaryStreamReader::ReadUInt()
{
    std::uint8_t b0 = ReadByte();
    std::uint8_t b1 = ReadByte();
    std::uint8_t b2 = ReadByte();
    std::uint8_t b3 = ReadByte();
    return (static_cast<std::uint32_t>(b0) << 24) | (static_cast<std::uint32_t>(b1) << 16) | (static_cast<std::uint32_t>(b2) << 8) | static_cast<std::uint32_t>(b3);
}

std::int32_t BinaryStreamReader::ReadInt()
{
    std::uint32_t x = ReadUInt();
    return static_cast<std::int32_t>(x);
}

std::uint64_t BinaryStreamReader::ReadULong()
{
    std::uint8_t b0 = ReadByte();
    std::uint8_t b1 = ReadByte();
    std::uint8_t b2 = ReadByte();
    std::uint8_t b3 = ReadByte();
    std::uint8_t b4 = ReadByte();
    std::uint8_t b5 = ReadByte();
    std::uint8_t b6 = ReadByte();
    std::uint8_t b7 = ReadByte();
    return (static_cast<std::uint64_t>(b0) << 56) | (static_cast<std::uint64_t>(b1) << 48) | (static_cast<std::uint64_t>(b2) << 40) | (static_cast<std::uint64_t>(b3) << 32) | (static_cast<std::uint64_t>(b4) << 24) |
        (static_cast<std::uint64_t>(b5) << 16) | (static_cast<std::uint64_t>(b6) << 8) | static_cast<std::uint64_t>(b7);
}

std::int64_t BinaryStreamReader::ReadLong()
{
    std::uint64_t x = ReadULong();
    return static_cast<std::int64_t>(x);
}

float BinaryStreamReader::ReadFloat()
{
    std::uint32_t x = ReadUInt();
    return *reinterpret_cast<float*>(&x);
}

double BinaryStreamReader::ReadDouble()
{
    std::uint64_t x = ReadULong();
    return *reinterpret_cast<double*>(&x);
}

char BinaryStreamReader::ReadChar()
{
    std::uint8_t x = ReadByte();
    return static_cast<char>(x);
}

char8_t BinaryStreamReader::ReadChar8()
{
    std::uint8_t x = ReadByte();
    return static_cast<char8_t>(x);
}

char16_t BinaryStreamReader::ReadWChar()
{
    std::uint16_t x = ReadUShort();
    return static_cast<char16_t>(x);
}

char32_t BinaryStreamReader::ReadUChar()
{
    std::uint32_t x = ReadUInt();
    return static_cast<char32_t>(x);
}

std::string BinaryStreamReader::ReadUtf8String()
{
    std::string s;
    std::uint8_t x = ReadByte();
    while (x != 0)
    {
        s.append(1, static_cast<char>(x));
        x = ReadByte();
    }
    return s;
}

std::u16string BinaryStreamReader::ReadUtf16String()
{
    std::string s = ReadUtf8String();
    return ToUtf16(s);
}

std::u32string BinaryStreamReader::ReadUtf32String()
{
    std::string s = ReadUtf8String();
    return ToUtf32(s);
}

std::uint32_t BinaryStreamReader::ReadULEB128UInt()
{
    std::uint32_t result = 0;
    std::uint32_t shift = 0;
    while (true)
    {
        std::uint8_t b = ReadByte();
        result |= static_cast<std::uint32_t>(b & 0x7F) << shift;
        if ((b & 0x80) == 0) break;
        shift += static_cast<std::uint32_t>(7);
    }
    return result;
}

std::uint64_t BinaryStreamReader::ReadULEB128ULong()
{
    std::uint64_t result = 0;
    std::uint64_t shift = 0;
    while (true)
    {
        std::uint8_t b = ReadByte();
        result |= static_cast<std::uint64_t>(b & 0x7F) << shift;
        if ((b & 0x80) == 0) break;
        shift += 7;
    }
    return result;
}

std::int32_t BinaryStreamReader::ReadSLEB128Int()
{
    std::int32_t result = 0;
    std::int32_t shift = 0;
    std::uint8_t b = 0;
    do
    {
        b = ReadByte();
        result |= static_cast<int32_t>(b & 0x7F) << shift;
        shift += 7;
    } while ((b & 0x80) != 0);
    if ((shift < 32) && (b & 0x40) != 0)
    {
        result |= ~std::int32_t(0) << shift;
    }
    return result;
}

std::int64_t BinaryStreamReader::ReadSLEB128Long()
{
    std::int64_t result = 0;
    std::int64_t shift = 0;
    std::uint8_t b = 0;
    do
    {
        b = ReadByte();
        result |= static_cast<std::int64_t>((b & 0x7F) << shift);
        shift += 7;
    } while ((b & 0x80) != 0);
    if ((shift < 64) && (b & 0x40) != 0)
    {
        result |= ~std::int64_t(0) << shift;
    }
    return result;
}

void BinaryStreamReader::ReadUuid(uuid& id)
{
    for (uuid::value_type& x : id)
    {
        x = ReadByte();
    }
}

time_t BinaryStreamReader::ReadTime()
{
    return static_cast<time_t>(ReadLong());
}

} // namespace util
