import std;

constexpr std::uint8_t to_byte(int x)
{
    return std::uint8_t(x);
}

int main()
{
    std::uint8_t b = to_byte(1);
    std::cout << int(b) << "\n";
}
