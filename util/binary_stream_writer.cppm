// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.binary.stream.writer;
import std;
import util.stream;
import util.uuid;

export namespace util {

class BinaryStreamWriter
{
public:
    BinaryStreamWriter(Stream& stream_) noexcept;
    inline Stream& GetStream() noexcept { return stream; }
    void Write(bool x);
    void Write(std::uint8_t x);
    void Write(std::int8_t x);
    void Write(std::uint16_t x);
    void Write(std::int16_t x);
    void Write(std::uint32_t x);
    void Write(std::int32_t x);
    void Write(std::uint64_t x);
    void Write(std::int64_t x);
    void Write(float x);
    void Write(double x);
    void Write(char x);
    void Write(char8_t x);
    void Write(char16_t x);
    void Write(char32_t x);
    void Write(const std::string& s);
    void Write(const std::string& s, bool writeNull);
    void Write(const std::u16string& s);
    void Write(const std::u32string& s);
    void WriteULEB128UInt(std::uint32_t x);
    void WriteULEB128ULong(std::uint64_t x);
    void WriteSLEB128Int(std::int32_t x);
    void WriteSLEB128Long(std::int64_t x);
    void Write(const uuid& uuid);
    void WriteTime(std::time_t x);
    inline std::int64_t Position() const noexcept { return stream.Position(); }
private:
    Stream& stream;
};

} // util
