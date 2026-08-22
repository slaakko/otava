// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.utility;

import std;

export namespace util {

constexpr const std::uint8_t* Advance(const std::uint8_t* start, std::uint32_t length)
{
    return static_cast<const std::uint8_t*>(start + length);
}

} // namespace util
