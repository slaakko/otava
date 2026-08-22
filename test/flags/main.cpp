import std;

enum class SymbolFlags : std::uint8_t
{
    none = 0, project = 1 << 0, readOnly = 1 << 1
};

constexpr std::uint8_t ToUnderlying(SymbolFlags flags) { return std::uint8_t(flags); }

constexpr SymbolFlags operator|(SymbolFlags left, SymbolFlags right) noexcept
{
    return SymbolFlags(std::uint8_t(left) | std::uint8_t(right));
}

constexpr SymbolFlags operator&(SymbolFlags left, SymbolFlags right) noexcept
{
    return SymbolFlags(std::uint8_t(left) & std::uint8_t(right));
}

constexpr SymbolFlags operator~(SymbolFlags flags) noexcept
{
    return SymbolFlags(~std::uint8_t(flags));
}

int main()
{
    SymbolFlags flags = SymbolFlags::none;
    std::uint8_t x = ToUnderlying(flags & ~SymbolFlags::project);
}
