// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.file.mapping;

import std;

export namespace util {

class FileMapping
{
public:
    FileMapping(const std::string& fileName_);
    ~FileMapping();
    inline const std::string& FileName() const noexcept { return fileName; }
    inline const std::uint8_t* Start() const noexcept { return start; }
    inline std::uint32_t Length() const noexcept { return length; }
private:
    std::string fileName;
    void* fileHandle;
    void* fileMappingHandle;
    const std::uint8_t* start;
    std::uint32_t length;
};

constexpr const std::uint8_t* Advance(const std::uint8_t* start, std::uint32_t length)
{
    return static_cast<const std::uint8_t*>(start + length);
}

} // namespace util
