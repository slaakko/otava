export module std.limits;

export namespace std {

template<typename T>
class numeric_limits
{
public:
    static constexpr T min();
    static constexpr T max();
};

template<> 
class numeric_limits<bool>
{
public:
    static constexpr bool min()
    {
        return false;
    }
    static constexpr bool max()
    {
        return true;
    }
};

template<>
class numeric_limits<char>
{
public:    
    static constexpr char min()
    {
        return char(0);
    }
    static constexpr char max()
    {
        return char(255);
    }
};

template<>
class numeric_limits<signed char>
{
public:
    static constexpr signed char min()
    {
        return static_cast<signed char>(-128);
    }
    static constexpr signed char max()
    {
        return static_cast<signed char>(127);
    }
};

template<>
class numeric_limits<unsigned char>
{
public:
    static constexpr unsigned char min()
    {
        return static_cast<unsigned char>(0);
    }
    static constexpr unsigned char max()
    {
        return static_cast<unsigned char>(255);
    }
};

template<>
class numeric_limits<wchar_t>
{
public:
    static constexpr wchar_t min()
    {
        return wchar_t(0);
    }
    static constexpr wchar_t max()
    {
        return wchar_t(65535);
    }
};

template<>
class numeric_limits<char8_t>
{
public:
    static constexpr char8_t min()
    {
        return char8_t(0);
    }
    static constexpr char8_t max()
    {
        return char8_t(255);
    }
};

template<>
class numeric_limits<char16_t>
{
public:
    static constexpr char16_t min()
    {
        return char16_t(0);
    }
    static constexpr char16_t max()
    {
        return char16_t(65535);
    }
};

template<>
class numeric_limits<char32_t>
{
public:
    static constexpr char32_t min()
    {
        return char32_t(0);
    }
    static constexpr char32_t max()
    {
        return char32_t(4294967295u);
    }
};

template<>
class numeric_limits<short>
{
public:
    static constexpr short min()
    {
        return short(-32768);
    }
    static constexpr short max()
    {
        return short(32767);
    }
};

template<>
class numeric_limits<unsigned short>
{
public:
    static constexpr unsigned short min()
    {
        return static_cast<unsigned short>(0);
    }
    static constexpr unsigned short max()
    {
        return static_cast<unsigned short>(65535);
    }
};

template<>
class numeric_limits<int>
{
public:
    static constexpr int min()
    {
        return int(-2147483648);
    }
    static constexpr int max()
    {
        return int(2147483647);
    }
};

template<>
class numeric_limits<unsigned int>
{
public:
    static constexpr unsigned int min()
    {
        return static_cast<unsigned int>(0);
    }
    static constexpr unsigned int max()
    {
        return static_cast<unsigned int>(4294967295u);
    }
};

template<>
class numeric_limits<long>
{
public:
    static constexpr long min()
    {
        return long(-2147483648);
    }
    static constexpr long max()
    {
        return long(2147483647);
    }
};

template<>
class numeric_limits<unsigned long>
{
public:
    static constexpr unsigned long min()
    {
        return static_cast<unsigned long>(0);
    }
    static constexpr unsigned long max()
    {
        return static_cast<unsigned long>(4294967295u);
    }
};

template<>
class numeric_limits<long long>
{
public:
    static constexpr long long min()
    {
        return static_cast<long long>(-9223372036854775808ll);
    }
    static constexpr long long max()
    {
        return static_cast<long long>(9223372036854775807ll);
    }
};

template<>
class numeric_limits<unsigned long long>
{
public:
    static constexpr unsigned long long min()
    {
        return static_cast<unsigned long long>(0);
    }
    static constexpr unsigned long long max()
    {
        return static_cast<unsigned long long>(18446744073709551615u);
    }
};

} // std
