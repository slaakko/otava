// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module util.file_mapping;

import util.win_error;

namespace util {

FileMapping::FileMapping(const std::string& fileName_) : fileName(fileName_), fileHandle(nullptr), fileMappingHandle(nullptr), start(nullptr), length(-1)
{
    fileHandle = CreateFileA(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, 
        nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        throw WindowsException(GetLastError());
    }
    fileMappingHandle = CreateFileMappingA(fileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!fileMappingHandle)
    {
        CloseHandle(fileHandle);
        throw WindowsException(GetLastError());
    }
    start = static_cast<std::uint8_t*>(MapViewOfFile(fileMappingHandle, FILE_MAP_READ, 0, 0, 0));
    if (!start)
    {
        CloseHandle(fileMappingHandle);
        CloseHandle(fileHandle);
        throw WindowsException(GetLastError());
    }
    length = GetFileSize(fileHandle, nullptr);
    if (length == INVALID_FILE_SIZE)
    {
        UnmapViewOfFile(start);
        CloseHandle(fileMappingHandle);
        CloseHandle(fileHandle);
        throw WindowsException(GetLastError());
    }
}

FileMapping::~FileMapping()
{
    if (start)
    {
        UnmapViewOfFile(start);
    }
    if (fileMappingHandle)
    {
        CloseHandle(fileMappingHandle);
    }
    if (fileHandle)
    {
        CloseHandle(fileHandle);
    }
}



} // namespace util
