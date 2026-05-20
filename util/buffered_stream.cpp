// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.buffered.stream;

import util.stream;

namespace util {

BufferedStream::BufferedStream(Stream& baseStream_) : BufferedStream(baseStream_, 4096)
{
}

BufferedStream::BufferedStream(Stream& baseStream_, std::int64_t bufferSize_) :
    Stream(), baseStream(baseStream_), bufferSize(bufferSize_), buffer(static_cast<std::uint8_t*>(operator new(bufferSize))), pos(bufferSize), bytesAvailable(0), end(0)
{
}

BufferedStream::~BufferedStream()
{
    try
    {
        Flush();
    }
    catch (...)
    {
    }
}

int BufferedStream::ReadByte()
{
    Flush();
    if (bytesAvailable == 0)
    {
        FillBuf();
        if (bytesAvailable == 0)
        {
            return -1;
        }
    }
    std::uint8_t* b = buffer.get();
    std::uint8_t value = b[pos++];
    --bytesAvailable;
    SetPosition(Position() + 1);
    return value;
}

std::int64_t BufferedStream::Read(std::uint8_t* buf, std::int64_t count)
{
    Flush();
    if (bytesAvailable == 0)
    {
        FillBuf();
    }
    std::int64_t bytesRead = 0;
    std::int64_t n = std::min(bytesAvailable, count);
    for (std::int64_t i = 0; i < n; ++i)
    {
        std::uint8_t* b = buffer.get();
        buf[i] = b[pos++];
        ++bytesRead;
        --bytesAvailable;
    }
    SetPosition(Position() + bytesRead);
    return bytesRead;
}

void BufferedStream::Write(std::uint8_t x)
{
    if (end >= bufferSize)
    {
        Flush();
    }
    std::uint8_t* b = buffer.get();
    b[end++] = x;
    SetPosition(Position() + 1);
}

void BufferedStream::Write(std::uint8_t* buf, std::int64_t count)
{
    for (std::int64_t i = 0; i < count; ++i)
    {
        Write(buf[i]);
    }
}

void BufferedStream::Flush()
{
    if (end != 0)
    {
        baseStream.Write(buffer.get(), end);
        baseStream.Flush();
        end = 0;
    }
}

void BufferedStream::Seek(std::int64_t pos, Origin origin)
{
    Flush();
    bytesAvailable = 0;
    baseStream.Seek(pos, origin);
}

std::int64_t BufferedStream::Tell()
{
    Flush();
    return baseStream.Tell() - bytesAvailable;
}

void BufferedStream::FillBuf()
{
    bytesAvailable = baseStream.Read(buffer.get(), bufferSize);
    pos = 0;
}

} // namespace util
