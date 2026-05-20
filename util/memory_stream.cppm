// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.memory.stream;

import std;
import util.stream;

export namespace util {

class MemoryStream : public Stream
{
public:
    MemoryStream() noexcept;
    MemoryStream(std::uint8_t* data_, std::int64_t size_) noexcept;
    int ReadByte() override;
    std::int64_t Read(std::uint8_t* buf, std::int64_t count) override;
    void Write(std::uint8_t x) override;
    void Write(std::uint8_t* buf, std::int64_t count) override;
    void Seek(std::int64_t pos, Origin origin) override;
    std::int64_t Tell() override;
    inline std::uint8_t* Data() noexcept { return data; }
    inline std::int64_t Size() const noexcept { return size; }
    inline std::int64_t ReadPos() const noexcept { return readPos; }
    inline const std::vector<std::uint8_t>& Content() const noexcept { return content; }
    void SetFromContent();
private:
    std::uint8_t* data;
    std::int64_t size;
    std::int64_t readPos;
    std::vector<std::uint8_t> content;
};

} // namespace util

