export module util.sha1;

import std;

export namespace util {

class Sha1
{
public:
    Sha1() noexcept;
    void Reset() noexcept;
    void Process(std::uint8_t x) noexcept;
    void Process(void* begin, void* end) noexcept;
    void Process(void* buf, int count) noexcept;
    std::string GetDigest();
private:
    void ProcessByte(std::uint8_t x) noexcept;
    void ProcessBlock() noexcept;
    std::uint32_t digest[5];
    std::uint8_t block[64];
    std::uint8_t byteIndex;
    std::uint64_t bitCount;
};

std::string GetSha1MessageDigest(const std::string& message);

} // namespace util
