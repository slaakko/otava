// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.binary.stream.writer;

import util.unicode;

namespace util {

BinaryStreamWriter::BinaryStreamWriter(Stream& stream_) noexcept : stream(stream_)
{
}

void BinaryStreamWriter::Write(bool x)
{
    Write(std::uint8_t(x));
}

void BinaryStreamWriter::Write(std::uint8_t x)
{
    stream.Write(x);
}

void BinaryStreamWriter::Write(std::int8_t x)
{
    Write(static_cast<std::uint8_t>(x));
}

void BinaryStreamWriter::Write(std::uint16_t x)
{
    std::uint8_t b0 = static_cast<std::uint8_t>(x >> 8);
    std::uint8_t b1 = static_cast<std::uint8_t>(x);
    Write(b0);
    Write(b1);
}

void BinaryStreamWriter::Write(std::int16_t x)
{
    Write(static_cast<std::uint16_t>(x));
}

void BinaryStreamWriter::Write(std::uint32_t x)
{
    std::uint8_t b0 = static_cast<std::uint8_t>(x >> 24);
    std::uint8_t b1 = static_cast<std::uint8_t>(x >> 16);
    std::uint8_t b2 = static_cast<std::uint8_t>(x >> 8);
    std::uint8_t b3 = static_cast<std::uint8_t>(x);
    Write(b0);
    Write(b1);
    Write(b2);
    Write(b3);
}

void BinaryStreamWriter::Write(std::int32_t x)
{
    Write(static_cast<std::uint32_t>(x));
}

void BinaryStreamWriter::Write(std::uint64_t x)
{
    std::uint8_t b0 = static_cast<std::uint8_t>(x >> 56);
    std::uint8_t b1 = static_cast<std::uint8_t>(x >> 48);
    std::uint8_t b2 = static_cast<std::uint8_t>(x >> 40);
    std::uint8_t b3 = static_cast<std::uint8_t>(x >> 32);
    std::uint8_t b4 = static_cast<std::uint8_t>(x >> 24);
    std::uint8_t b5 = static_cast<std::uint8_t>(x >> 16);
    std::uint8_t b6 = static_cast<std::uint8_t>(x >> 8);
    std::uint8_t b7 = static_cast<std::uint8_t>(x);
    Write(b0);
    Write(b1);
    Write(b2);
    Write(b3);
    Write(b4);
    Write(b5);
    Write(b6);
    Write(b7);
}

void BinaryStreamWriter::Write(std::int64_t x)
{
    Write(static_cast<std::uint64_t>(x));
}

void BinaryStreamWriter::Write(float x)
{
    std::uint32_t* u = reinterpret_cast<std::uint32_t*>(&x);
    Write(*u);
}

void BinaryStreamWriter::Write(double x)
{
    std::uint64_t* u = reinterpret_cast<std::uint64_t*>(&x);
    Write(*u);
}

void BinaryStreamWriter::Write(char x)
{
    Write(static_cast<std::uint8_t>(x));
}

void BinaryStreamWriter::Write(char8_t x)
{
    Write(static_cast<std::uint8_t>(x));
}

void BinaryStreamWriter::Write(char16_t x)
{
    Write(static_cast<std::uint16_t>(x));
}

void BinaryStreamWriter::Write(char32_t x)
{
    Write(static_cast<std::uint32_t>(x));
}

void BinaryStreamWriter::Write(const std::string& s)
{
    Write(s, true);
}

void BinaryStreamWriter::Write(const std::string& s, bool writeNull)
{
    for (char c : s)
    {
        std::uint8_t x = static_cast<std::uint8_t>(c);
        Write(x);
    }
    if (writeNull)
    {
        Write(static_cast<std::uint8_t>(0));
    }
}

void BinaryStreamWriter::Write(const std::u16string& s)
{
    std::string utf8_str = ToUtf8(s);
    Write(utf8_str);
}

void BinaryStreamWriter::Write(const std::u32string& s)
{
    std::string utf8_str = ToUtf8(s);
    Write(utf8_str);
}

void BinaryStreamWriter::WriteULEB128UInt(std::uint32_t x)
{
    do
    {
        std::uint8_t b = x & 0x7F;
        x >>= 7u;
        if (x != 0)
        {
            b |= static_cast<std::uint8_t>(0x80);
        }
        Write(b);
    } while (x != 0);
}

void BinaryStreamWriter::WriteULEB128ULong(std::uint64_t x)
{
    do
    {
        std::uint8_t b = x & 0x7F;
        x >>= 7;
        if (x != 0)
        {
            b |= static_cast<std::uint8_t>(0x80);
        }
        Write(b);
    } while (x != 0);
}

void BinaryStreamWriter::WriteSLEB128Int(std::int32_t x)
{
    bool more = true;
    bool negative = x < 0;
    while (more)
    {
        std::uint8_t b = x & 0x7F;
        x >>= 7;
        if (negative)
        {
            x |= ~std::int32_t(0) << (32 - 7);
        }
        if (x == 0 && (b & 0x40) == 0 || x == -1 && (b & 0x40) != 0)
        {
            more = false;
        }
        else
        {
            b |= static_cast<std::uint8_t>(0x80);
        }
        Write(b);
    }
}

void BinaryStreamWriter::WriteSLEB128Long(std::int64_t x)
{
    bool more = true;
    bool negative = x < 0;
    while (more)
    {
        std::uint8_t b = x & 0x7F;
        x >>= 7;
        if (negative)
        {
            x |= ~std::int64_t(0) << (64 - 7);
        }
        if (x == 0 && (b & 0x40) == 0 || x == -1 && (b & 0x40) != 0)
        {
            more = false;
        }
        else
        {
            b |= static_cast<std::uint8_t>(0x80);
        }
        Write(b);
    }
}

void BinaryStreamWriter::Write(const uuid& id)
{
    for (uuid::value_type x : id)
    {
        Write(x);
    }
}

void BinaryStreamWriter::WriteTime(time_t x)
{
    Write(static_cast<std::int64_t>(x));
}

} // namespace util
