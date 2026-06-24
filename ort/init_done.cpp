// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module ort.init_done;

import ort.io;

void ort_init()
{
    ort_io_init();
}

void ort_done()
{
    ort_io_done();
}