// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module ort.rand;

import std;

export extern "C" std::uint32_t ort_random();
export extern "C" std::uint64_t ort_random64();
export extern "C" void ort_set_random_seed(std::uint64_t seed);
export extern "C" void ort_reset_rng();
