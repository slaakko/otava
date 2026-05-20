// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.rand;

import std;

export namespace util {

void set_rand_seed(std::uint64_t seed) noexcept;
void reset_rng() noexcept;
std::uint8_t get_random_byte() noexcept;
std::uint32_t Random() noexcept;
std::uint64_t Random64() noexcept;

} // namespace util
