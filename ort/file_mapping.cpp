// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module ort.file_mapping;

bool ort_create_file_mapping(const char* filePath, void*& fileHandle, void*& fileMappingHandle, std::uint8_t*& start, std::uint32_t& length)
{
    fileHandle = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ,
        nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    fileMappingHandle = CreateFileMappingA(fileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!fileMappingHandle)
    {
        CloseHandle(fileHandle);
        return false;
    }
    start = static_cast<std::uint8_t*>(MapViewOfFile(fileMappingHandle, FILE_MAP_READ, 0, 0, 0));
    if (!start)
    {
        CloseHandle(fileMappingHandle);
        CloseHandle(fileHandle);
        return false;
    }
    length = GetFileSize(fileHandle, nullptr);
    if (length == INVALID_FILE_SIZE)
    {
        UnmapViewOfFile(start);
        CloseHandle(fileMappingHandle);
        CloseHandle(fileHandle);
        return false;
    }
    return true;
}

void ort_destruct_file_mapping(void* fileHandle, void* fileMappingHandle, std::uint8_t* start)
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
