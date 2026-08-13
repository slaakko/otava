import std;

int main()
{
    std::cout << "bool: " << std::numeric_limits<bool>::min() << " : " << std::numeric_limits<bool>::max() << "\n";
    std::cout << "char: " << int(std::numeric_limits<char>::min()) << " : " << int(std::numeric_limits<char>::max()) << "\n";
    std::cout << "signed char: " << int(std::numeric_limits<signed char>::min()) << " : " << int(std::numeric_limits<signed char>::max()) << "\n";
    std::cout << "unsigned char: " << int(std::numeric_limits<unsigned char>::min()) << " : " << int(std::numeric_limits<unsigned char>::max()) << "\n";
    std::cout << "char8_t: " << int(std::numeric_limits<char8_t>::min()) << " : " << int(std::numeric_limits<char8_t>::max()) << "\n";
    std::cout << "char16_t: " << int(std::numeric_limits<char16_t>::min()) << " : " << int(std::numeric_limits<char16_t>::max()) << "\n";
    std::cout << "char32_t: " << int(std::numeric_limits<char32_t>::min()) << " : " << static_cast<unsigned int>(std::numeric_limits<char32_t>::max()) << "\n";
    std::cout << "wchar_t: " << int(std::numeric_limits<wchar_t>::min()) << " : " << int(std::numeric_limits<wchar_t>::max()) << "\n";
    std::cout << "short: " << int(std::numeric_limits<short>::min()) << " : " << int(std::numeric_limits<short>::max()) << "\n";
    std::cout << "unsigned short: " << std::numeric_limits<unsigned short>::min() << " : " << std::numeric_limits<unsigned short>::max() << "\n";
    std::cout << "int: " << std::numeric_limits<int>::min() << " : " << std::numeric_limits<int>::max() << "\n";
    std::cout << "unsigned int: " << std::numeric_limits<unsigned int>::min() << " : " << std::numeric_limits<unsigned int>::max() << "\n";
    std::cout << "long: " << std::numeric_limits<long>::min() << " : " << std::numeric_limits<long>::max() << "\n";
    std::cout << "unsigned long: " << std::numeric_limits<unsigned long>::min() << " : " << std::numeric_limits<unsigned long>::max() << "\n";
    std::cout << "long long: " << std::numeric_limits<long long>::min() << " : " << std::numeric_limits<long long>::max() << "\n";
    std::cout << "unsigned long long: " << std::numeric_limits<unsigned long long>::min() << " : " << std::numeric_limits<unsigned long long>::max() << "\n";
}
