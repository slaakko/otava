// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module ort.io;

import std;

export extern "C" void ort_io_init();
export extern "C" void ort_io_done();
export extern "C" void ort_io_write(int handle, const char* s);
