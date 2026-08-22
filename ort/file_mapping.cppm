// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module ort.file_mapping;

import std;

export extern "C" bool ort_create_file_mapping(const char* filePath, void*& fileHandle, void*& fileMappingHandle, std::uint8_t*& start, std::uint32_t& length);
export extern "C" void ort_destruct_file_mapping(void* fileHandle, void* fileMappingHandle, std::uint8_t* start);
