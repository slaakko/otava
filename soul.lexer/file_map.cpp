// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.lexer.file.map;

import util;

namespace soul::lexer {

FileMap::FileMap() noexcept : nextFileId(0)
{
}

std::int32_t FileMap::MapFile(const std::string& filePath)
{
    std::int32_t fileId = nextFileId++;
    MapFile(filePath, fileId);
    return fileId;
}

void FileMap::MapFile(const std::string& filePath, std::int32_t fileId)
{
    filePathMap[fileId] = filePath;
}

const std::string& FileMap::GetFilePath(std::int32_t fileId) noexcept
{
    auto it = filePathMap.find(fileId);
    if (it != filePathMap.end())
    {
        return it->second;
    }
    else
    {
        static std::string empty;
        return empty;
    }
}

void FileMap::AddFileContent(std::int32_t fileId, std::u32string&& fileContent, std::vector<int>&& lineStartIndeces)
{
    fileContentsMap[fileId] = std::make_pair(std::move(fileContent), std::move(lineStartIndeces));
}

bool FileMap::HasFileContent(std::int32_t fileId)
{
    return fileContentsMap.find(fileId) != fileContentsMap.end();
}

const std::u32string& FileMap::GetFileContent(std::int32_t fileId)
{
    auto it = fileContentsMap.find(fileId);
    if (it != fileContentsMap.end())
    {
        return it->second.first;
    }
    else
    {
        throw std::runtime_error("file contents for file id " + std::to_string(fileId) + " not found from file map");
    }
}

std::vector<int> ComputeLineStartIndeces(const std::u32string& content)
{
    std::vector<int> indeces;
    indeces.push_back(0);
    const char32_t* start = content.c_str();
    const char32_t* end = content.c_str() + content.size();
    const char32_t* pos = start;
    bool startOfLine = true;
    while (pos != end)
    {
        if (startOfLine)
        {
            indeces.push_back(static_cast<int>(pos - start));
        }
        startOfLine = *pos == '\n';
        ++pos;
    }
    indeces.push_back(static_cast<int>(end - start));
    return indeces;
}

void FileMap::ReadFile(std::int32_t fileId)
{
    std::string filePath = GetFilePath(fileId);
    if (filePath.empty()) return;
    std::string fileContent = util::ReadFile(filePath);
    std::u32string ucontent;
    try
    {
        ucontent = util::ToUtf32(fileContent);
    }
    catch (const util::UnicodeException& ex)
    {
        util::ThrowUnicodeException(std::string(ex.what()) + ", file=" + filePath);
    }
    std::vector<int> lineStartIndeces = ComputeLineStartIndeces(ucontent);
    AddFileContent(fileId, std::move(ucontent), std::move(lineStartIndeces));
}

std::u32string FileMap::GetFileLine(std::int32_t fileId, int line)
{
    if (fileId == -1) return std::u32string();
    if (!HasFileContent(fileId))
    {
        ReadFile(fileId);
    }
    const std::vector<int>* indexes = LineStartIndeces(fileId);
    if (!indexes) return std::u32string();
    if (line < 1 || line >= indexes->size()) return std::u32string();
    std::u32string::size_type lineStart = (*indexes)[line];
    std::u32string::size_type lineLength = std::u32string::npos;
    if (line < indexes->size() - 1)
    {
        lineLength = (*indexes)[line + 1] - lineStart;
    }
    std::u32string lineStr = GetFileContent(fileId).substr(lineStart, lineLength);
    int n = lineStr.length();
    int i = n - 1;
    while (i >= 0)
    {
        if (lineStr[i] != '\r' && lineStr[i] != '\n')
        {
            break;
        }
        --i;
    }
    std::u32string trimmedLine = lineStr.substr(0, i + 1);
    return trimmedLine;
}

const std::vector<int>* FileMap::LineStartIndeces(std::int32_t fileId)
{
    auto it = fileContentsMap.find(fileId);
    if (it != fileContentsMap.end())
    {
        return &it->second.second;
    }
    else
    {
        return nullptr;
    }
}

FileMap* globalFileMap = nullptr;

FileMap* GetGlobalFileMap()
{
    return globalFileMap;
}

void SetGlobalFileMap(FileMap* fileMap)
{
    globalFileMap = fileMap;
}

} // namespace soul::lexer
