import std;

enum class SymbolFlags : std::uint8_t
{
    none = 0, project = 1 << 0, readOnly = 1 << 1
};

constexpr std::uint8_t ToUnderlying(SymbolFlags flags) { return std::uint8_t(flags); }

constexpr SymbolFlags operator|(SymbolFlags left, SymbolFlags right) noexcept
{
    return SymbolFlags(ToUnderlying(left) | ToUnderlying(right));
}

constexpr SymbolFlags operator&(SymbolFlags left, SymbolFlags right) noexcept
{
    return SymbolFlags(ToUnderlying(left) & ToUnderlying(right));
}

constexpr SymbolFlags operator~(SymbolFlags flags) noexcept
{
    return SymbolFlags(~ToUnderlying(flags));
}

int main()
{
    std::uint8_t p = ToUnderlying(SymbolFlags::project);
    SymbolFlags f = SymbolFlags(p);
    SymbolFlags s = f & ~SymbolFlags::project;
    std::uint8_t u = ToUnderlying(s);
}
