export module util.memory_reader;

import std;
import util.time;

export namespace util {

class MemoryReader
{
public:
    MemoryReader(const std::uint8_t* ptr_, std::int64_t count_) noexcept;
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
private:
    const std::uint8_t* ptr;
    const std::uint8_t* pos;
    std::int64_t count;
};

} // namespace util
