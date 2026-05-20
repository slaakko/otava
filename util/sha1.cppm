// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.sha1;

import std;

export namespace util {

class Sha1
{
public:
    Sha1() noexcept;
    void Reset() noexcept;
    inline void Process(std::uint8_t x) noexcept
    {
        ProcessByte(x);
        bitCount = bitCount + 8u;
    }
    void Process(void* begin, void* end) noexcept;
    inline void Process(void* buf, int count) noexcept
    {
        std::uint8_t* b = static_cast<std::uint8_t*>(buf);
        Process(b, b + count);
    }
    std::string GetDigest();
private:
    inline void ProcessByte(std::uint8_t x) noexcept
    {
        block[byteIndex++] = x;
        if (byteIndex == 64u)
        {
            byteIndex = static_cast<std::uint8_t>(0u);
            ProcessBlock();
        }
    }
    void ProcessBlock() noexcept;
    std::uint32_t digest[5];
    std::uint8_t block[64];
    std::uint8_t byteIndex;
    std::uint64_t bitCount;
};

std::string GetSha1MessageDigest(const std::string& message);

} // namespace util
