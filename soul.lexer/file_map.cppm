// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.file.map;

import soul.lexer.base;
import soul.lexer.token;
import util;
import std;

export namespace soul::lexer {

class FileMap
{
public:
    FileMap() noexcept;
    std::int32_t MapFile(const std::string& filePath);
    void MapFile(const std::string& filePath, std::int32_t fileId);
    const std::string& GetFilePath(std::int32_t fileId) noexcept;
    void AddFileContent(std::int32_t fileId, std::u32string&& fileContent, std::vector<int>&& lineStartIndeces);
    bool HasFileContent(std::int32_t fileId);
    void ReadFile(std::int32_t fileId);
    const std::u32string& GetFileContent(std::int32_t file);
    std::u32string GetFileLine(std::int32_t fileId, int line);
    inline std::int32_t NextFileId() const { return nextFileId; }
    const std::vector<int>* LineStartIndeces(std::int32_t fileId);
private:
    std::int32_t nextFileId;
    std::map<std::int32_t, std::string> filePathMap;
    std::map<std::int32_t, std::pair<std::u32string, std::vector<int>>> fileContentsMap;
};

FileMap* GetGlobalFileMap();
void SetGlobalFileMap(soul::lexer::FileMap* fileMap_);

} // namespace soul::lexer
