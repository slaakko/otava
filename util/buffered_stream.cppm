// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.buffered.stream;

import std;
import util.stream;

export namespace util {

class BufferedStream : public Stream
{
public:
    BufferedStream(Stream& baseStream_);
    BufferedStream(Stream& baseStream_, std::int64_t bufferSize_);
    ~BufferedStream() override;
    int ReadByte() override;
    std::int64_t Read(std::uint8_t* buf, std::int64_t count) override;
    void Write(std::uint8_t x) override;
    void Write(std::uint8_t* buf, std::int64_t count) override;
    void Flush() override;
    void Seek(std::int64_t pos, Origin origin) override;
    std::int64_t Tell() override;
    inline Stream& BaseStream() noexcept { return baseStream; }
private:
    void FillBuf();
    Stream& baseStream;
    std::int64_t bufferSize;
    std::unique_ptr<std::uint8_t> buffer;
    std::int64_t pos;
    std::int64_t bytesAvailable;
    std::int64_t end;
};

} // namespace util

