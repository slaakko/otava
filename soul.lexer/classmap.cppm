// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.classmap;

import std;
import util;

export namespace soul::lexer {

template<typename Char>
class ClassMap
{
public:
    ClassMap(const std::int32_t* data_, std::int32_t upperBound_) noexcept : data(data_), upperBound(upperBound_) { }
    inline std::int32_t GetClass(Char c) const noexcept
    {
        std::int32_t i = static_cast<std::int32_t>(c);
        if (i < upperBound)
        {
            return data[i];
        }
        else
        {
            return -1;
        }
    }
private:
    const std::int32_t* data;
    std::int32_t upperBound;
};

template<typename Char>
ClassMap<Char>* MakeClassMap(const std::string& classMapName)
{
    std::string currentExecutableName = util::Path::GetFileName(util::GetFullPath(util::GetPathToExecutable()));
    util::BinaryResourcePtr resource(currentExecutableName, classMapName);
    util::MemoryStream memoryStream(resource.Data(), resource.Size());
    util::BinaryStreamReader rawReader(memoryStream);
    std::int32_t size = rawReader.ReadInt();
    std::int32_t* data = static_cast<std::int32_t*>(operator new(sizeof(std::int32_t) * size));
    util::DeflateStream compressedStream(util::CompressionMode::decompress, memoryStream);
    util::BinaryStreamReader reader(compressedStream);
    for (std::int64_t i = 0; i < size; ++i)
    {
        std::int32_t x = reader.ReadInt();
        data[i] = x;
    }
    ClassMap<Char>* classMap = new ClassMap<Char>(data, size);
    return classMap;
}

template<typename Char>
ClassMap<Char>* MakeClassMap(const std::string& moduleFileName, const std::string& classMapName, util::ResourceFlags resourceFlags)
{
    util::BinaryResourcePtr resource(moduleFileName, classMapName, resourceFlags);
    util::MemoryStream memoryStream(resource.Data(), resource.Size());
    util::BinaryStreamReader rawReader(memoryStream);
    std::int32_t size = rawReader.ReadInt();
    std::int32_t* data = static_cast<std::int32_t*>(operator new(sizeof(std::int32_t) * size));
    util::DeflateStream compressedStream(util::CompressionMode::decompress, memoryStream);
    util::BinaryStreamReader reader(compressedStream);
    for (std::int64_t i = 0; i < size; ++i)
    {
        std::int32_t x = reader.ReadInt();
        data[i] = x;
    }
    ClassMap<Char>* classMap = new ClassMap<Char>(data, size);
    return classMap;
}

} // soul::lexer
