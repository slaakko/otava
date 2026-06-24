// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module ort.rand;

import util;

std::uint32_t ort_random()
{
    return util::Random();
}

std::uint64_t ort_random64()
{
    return util::Random64();
}

void ort_set_random_seed(std::uint64_t seed)
{
    util::set_rand_seed(seed);
}

void ort_reset_rng()
{
    util::reset_rng();
}
