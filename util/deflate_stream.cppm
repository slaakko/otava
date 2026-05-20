// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.deflate.stream;

import std;
import util.compression;
import util.stream;

export namespace util {

#ifndef SOUL_CPP20

const int defaultDeflateCompressionLevel = -1;
const int noDeflateCompression = 0;
const int fastestDeflateCompression = 1;
const int optimalDeflateCompression = 9;
const int Z_FINISH = 4;
const int Z_STREAM_END = 1;
const int Z_NO_FLUSH = 0;

class DeflateStream : public Stream
{
public:
    DeflateStream(CompressionMode mode_, Stream& underlyingStream_);
    DeflateStream(CompressionMode mode_, Stream& underlyingStream_, std::int64_t bufferSize_);
    DeflateStream(CompressionMode mode_, Stream& underlyingStream_, std::int64_t bufferSize_, int compressionLevel_);
    ~DeflateStream() override;
    int ReadByte() override;
    std::int64_t Read(std::uint8_t* buf, std::int64_t count) override;
    void Write(std::uint8_t x) override;
    void Write(std::uint8_t* buf, std::int64_t count) override;
private:
    void Finish();
    CompressionMode mode;
    Stream& underlyingStream;
    std::int64_t bufferSize;
    int compressionLevel;
    std::uint32_t inAvail;
    std::unique_ptr<std::uint8_t[]> in;
    std::uint32_t outAvail;
    std::int64_t outPos;
    std::uint32_t outHave;
    bool endOfInput;
    bool endOfStream;
    std::unique_ptr<std::uint8_t[]> out;
    void* handle;
};

#endif

} // namespace util
