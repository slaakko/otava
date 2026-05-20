// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.file.stream;

import std;
import util.stream;

export namespace util {

enum class OpenMode : int
{
    none = 0,
    read = 1 << 0,
    write = 1 << 1,
    append = 1 << 2,
    binary = 1 << 3
};

inline constexpr OpenMode operator|(OpenMode left, OpenMode right) noexcept
{
    return OpenMode(int(left) | int(right));
}

inline constexpr OpenMode operator&(OpenMode left, OpenMode right) noexcept
{
    return OpenMode(int(left) & int(right));
}

inline constexpr OpenMode operator~(OpenMode mode) noexcept
{
    return OpenMode(~int(mode));
}

class FileStream : public Stream
{
public:
    explicit FileStream(int handle);
    FileStream(const std::string& filePath_, OpenMode openMode);
    ~FileStream() override;
    inline const std::string& FilePath() const noexcept { return filePath; }
    int ReadByte() override;
    std::int64_t Read(std::uint8_t* buf, std::int64_t count) override;
    void Write(std::uint8_t x) override;
    void Write(std::uint8_t* buf, std::int64_t count) override;
    void Flush() override;
    void Seek(std::int64_t pos, Origin origin) override;
    std::int64_t Tell() override;
    std::int64_t Size() const;
private:
    std::string filePath;
    std::FILE* file;
    bool needToClose;
};

std::string ReadFile(const std::string& filePath);
std::string ReadFile(const std::string& filePath, bool doNotSkipBOM);

} // util
