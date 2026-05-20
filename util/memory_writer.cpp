// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.memory.writer;

namespace util {

MemoryWriter::MemoryWriter(std::uint8_t* ptr_, std::int64_t count_) noexcept : ptr(ptr_), pos(ptr), count(count_)
{
}

void MemoryWriter::Write(std::uint8_t x)
{
    if (pos - ptr >= count)
    {
        throw std::runtime_error("memory writer: end of space");
    }
    *pos++ = x;
}

void MemoryWriter::Write(std::int8_t x)
{
    Write(static_cast<std::uint8_t>(x));
}

void MemoryWriter::Write(std::uint16_t x)
{
    std::uint8_t b0 = static_cast<std::uint8_t>(x >> 8u);
    std::uint8_t b1 = static_cast<std::uint8_t>(x);
    Write(b0);
    Write(b1);
}

void MemoryWriter::Write(std::int16_t x)
{
    Write(static_cast<std::uint16_t>(x));
}

void MemoryWriter::Write(std::uint32_t x)
{
    std::uint8_t b0 = static_cast<std::uint8_t>(x >> 24u);
    std::uint8_t b1 = static_cast<std::uint8_t>(x >> 16u);
    std::uint8_t b2 = static_cast<std::uint8_t>(x >> 8u);
    std::uint8_t b3 = static_cast<std::uint8_t>(x);
    Write(b0);
    Write(b1);
    Write(b2);
    Write(b3);
}

void MemoryWriter::Write(std::int32_t x)
{
    Write(static_cast<std::uint32_t>(x));
}

void MemoryWriter::Write(std::uint64_t x)
{
    std::uint8_t b0 = static_cast<std::uint8_t>(x >> 56u);
    std::uint8_t b1 = static_cast<std::uint8_t>(x >> 48u);
    std::uint8_t b2 = static_cast<std::uint8_t>(x >> 40u);
    std::uint8_t b3 = static_cast<std::uint8_t>(x >> 32u);
    std::uint8_t b4 = static_cast<std::uint8_t>(x >> 24u);
    std::uint8_t b5 = static_cast<std::uint8_t>(x >> 16u);
    std::uint8_t b6 = static_cast<std::uint8_t>(x >> 8u);
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

void MemoryWriter::Write(std::int64_t x)
{
    Write(static_cast<std::uint64_t>(x));
}

void MemoryWriter::Write(const DateTime& dt)
{
    Date date = dt.GetDate();
    Write(date.Year());
    Write(static_cast<std::int8_t>(date.GetMonth()));
    Write(date.Day());
    Write(dt.Seconds());
}

void MemoryWriter::Write(const std::string& str)
{
    for (char c : str)
    {
        Write(static_cast<std::uint8_t>(c));
    }
    Write(static_cast<std::uint8_t>(0u));
}

} // namespace util
