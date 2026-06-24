// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module ort.memory;

import std;

export namespace ort::memory {

void allocate(void* block, std::int64_t size);
bool free(void* block);
void set_info(void* block, const char* function, const char* sourceFilePath, int line);
void set_allocation_serial(int serial);
void print_leaks(std::ostream* s);

} // namespace ort::memory
