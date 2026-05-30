// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.memory_reader;

import std;
import util.time;
import util.uuid;

export namespace util {

class MemoryReader
{
public:
    MemoryReader(const std::uint8_t* ptr_, std::int32_t count_) noexcept;
    bool ReadBool();
    std::uint8_t ReadByte();
    std::int8_t ReadSByte();
    std::uint16_t ReadUShort();
    std::int16_t ReadShort();
    std::uint32_t ReadUInt();
    std::int32_t ReadInt();
    std::uint64_t ReadULong();
    std::int64_t ReadLong();
    DateTime ReadDateTime();
    std::string ReadString();
    util::uuid ReadUuid();
    std::uint32_t ReadULEB128UInt();
    inline std::uint32_t CurrentOffset() const noexcept { return static_cast<std::uint32_t>(pos - ptr); }
    inline void Skip(std::uint32_t amount) noexcept { pos += amount; }
private:
    const std::uint8_t* ptr;
    const std::uint8_t* pos;
    std::uint32_t count;
};

} // namespace util
