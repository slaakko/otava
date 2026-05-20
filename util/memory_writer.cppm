// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.memory.writer;

import std;
import util.time;

export namespace util {

class MemoryWriter
{
public:
    MemoryWriter(std::uint8_t* ptr_, std::int64_t count_) noexcept;
    void Write(std::uint8_t x);
    void Write(std::int8_t x);
    void Write(std::uint16_t x);
    void Write(std::int16_t x);
    void Write(std::uint32_t x);
    void Write(std::int32_t x);
    void Write(std::uint64_t x);
    void Write(std::int64_t x);
    void Write(const DateTime& dt);
    void Write(const std::string& str);
private:
    std::uint8_t* ptr;
    std::uint8_t* pos;
    std::int64_t count;
};

} // namespace util
