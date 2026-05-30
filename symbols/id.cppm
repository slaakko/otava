// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.id;

import std;

export namespace otava::symbols {

enum class StringOffset : std::uint32_t {};

constexpr std::uint32_t ToUnderlying(StringOffset offset) noexcept { return std::uint32_t(offset); }

constexpr StringOffset operator+(StringOffset offset, std::uint32_t operand) noexcept
{
    return StringOffset(ToUnderlying(offset) + operand);
}

constexpr StringOffset notFoundOffset = StringOffset(0xFFFFFFFF);

enum class SymbolId : std::uint32_t {};

constexpr SymbolId zeroSymbolId = SymbolId(0);

constexpr std::uint32_t ToUnderlying(SymbolId id) noexcept { return std::uint32_t(id); }

enum class Length : std::uint32_t {};

constexpr std::uint32_t ToUnderlying(Length length)  noexcept { return std::uint32_t(length); }

constexpr Length operator+(Length x, Length y) noexcept
{
    return Length(ToUnderlying(x) + ToUnderlying(y));
}

constexpr Length& operator+=(Length& x, Length y) noexcept
{
    x = x + y;
    return x;
}

constexpr Length operator*(Length x, Length y) noexcept
{
    return Length(ToUnderlying(x) * ToUnderlying(y));
}

enum class FileOffset : std::uint32_t {};

constexpr std::uint32_t ToUnderlying(FileOffset fileOffset) noexcept { return std::uint32_t(fileOffset); }

constexpr Length operator-(FileOffset x, FileOffset y) noexcept
{
    return Length(ToUnderlying(x) - ToUnderlying(y));
}

enum class Cardinality : std::uint32_t {};

constexpr std::uint32_t ToUnderlying(Cardinality cardinality) noexcept { return std::uint32_t(cardinality); }

constexpr Cardinality operator+(Cardinality x, Cardinality y) noexcept
{
    return Cardinality(ToUnderlying(x) + ToUnderlying(y));
}

constexpr Cardinality& operator++(Cardinality& c) noexcept
{
    c = c + Cardinality(1);
    return c;
}

constexpr Cardinality operator++(Cardinality& c, int) noexcept
{
    Cardinality result(c);
    c = c + Cardinality(1);
    return result;
}

enum class Index : std::uint32_t {};

constexpr std::uint32_t ToUnderlying(Index index) noexcept { return std::uint32_t(index); }

constexpr Index operator+(Index x, Index y) noexcept
{
    return Index(ToUnderlying(x) + ToUnderlying(y));
}

constexpr Index operator-(Index x, Index y) noexcept
{
    return Index(ToUnderlying(x) - ToUnderlying(y));
}

constexpr Index& operator++(Index& x) noexcept
{
    x = x + Index(1);
    return x;
}

constexpr Index operator++(Index& x, int) noexcept
{
    Index result(x);
    x = x + Index(1);
    return result;
}

enum class ModuleId : std::uint32_t {};

constexpr std::uint32_t ToUnderlying(ModuleId moduleId) noexcept { return std::uint32_t(moduleId); }

constexpr ModuleId zeroModuleId = ModuleId(0);

constexpr ModuleId operator+(ModuleId x, ModuleId y) noexcept
{
    return ModuleId(ToUnderlying(x) + ToUnderlying(y));
}

constexpr ModuleId operator+(ModuleId x, Cardinality y) noexcept
{
    return ModuleId(ToUnderlying(x) + ToUnderlying(y));
}

constexpr ModuleId& operator++(ModuleId& x) noexcept
{
    x = x + ModuleId(1);
    return x;
}

constexpr ModuleId operator++(ModuleId & x, int) noexcept
{
    ModuleId result(x);
    x = x + ModuleId(1);
    return result;
}

} // namespace otava::symbols
