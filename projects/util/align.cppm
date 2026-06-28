export module util.align;

import std;

export namespace util {

inline std::uint64_t Align(std::uint64_t n, std::uint64_t alignment) noexcept
{
    return (n + alignment - 1u) & -alignment;
}

} // namespace util
