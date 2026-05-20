// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.binary.stream.reader;

import std;
import util.stream;
import util.uuid;

export namespace util {

class BinaryStreamReader
{
public:
    BinaryStreamReader(Stream& stream_) noexcept;
    inline Stream& GetStream() noexcept { return stream; }
    bool ReadBool();
    std::uint8_t ReadByte();
    std::int8_t ReadSByte();
    std::uint16_t ReadUShort();
    std::int16_t ReadShort();
    std::uint32_t ReadUInt();
    std::int32_t ReadInt();
    std::uint64_t ReadULong();
    std::int64_t ReadLong();
    float ReadFloat();
    double ReadDouble();
    char ReadChar();
    char8_t ReadChar8();
    char16_t ReadWChar();
    char32_t ReadUChar();
    std::string ReadUtf8String();
    std::u16string ReadUtf16String();
    std::u32string ReadUtf32String();
    std::uint32_t ReadULEB128UInt();
    std::uint64_t ReadULEB128ULong();
    std::int32_t ReadSLEB128Int();
    std::int64_t ReadSLEB128Long();
    void ReadUuid(uuid& uuid);
    std::time_t ReadTime();
    inline std::int64_t Position() const noexcept { return stream.Position(); }
private:
    Stream& stream;
};

} // util
