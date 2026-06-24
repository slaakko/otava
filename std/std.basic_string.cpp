module std.basic_string;

import std.stream;
import std.new_delete_op;
import std.atomic;
import std.thread.mutex;
import std.init;

namespace std {

template<typename T>
string to_string_signed(T x)
{
    string s;
    std::uint64_t u = static_cast<std::uint64_t>(0);
    bool neg = static_cast<std::int64_t>(x) < static_cast<std::int64_t>(0);
    if (neg)
    {
        u = -static_cast<std::int64_t>(x);
    }
    else
    {
        u = x;
    }
    do
    {
        s.append(1, static_cast<char>(static_cast<uint8_t>('0') + static_cast<uint8_t>(u % static_cast<std::uint64_t>(10))));
        u /= static_cast<std::uint64_t>(10);
    } while (u != static_cast<std::uint64_t>(0));
    if (neg)
    {
        s.append(1, '-');
    }
    reverse(s.begin(), s.end());
    return s;
}

template<typename T>
string to_string_unsigned(T x)
{
    string s;
    do
    {
        s.append(1, static_cast<char>(static_cast<uint8_t>('0') + static_cast<uint8_t>(x % static_cast<T>(static_cast<uint8_t>(10)))));
        x /= static_cast<T>(static_cast<uint8_t>(10));
    } while (x != static_cast<T>(static_cast<uint8_t>(0)));
    reverse(s.begin(), s.end());
    return s;
}

string to_string(int val)
{
    return to_string_signed(val);
}

string to_string(unsigned val)
{
    return to_string_unsigned(val);
}

string to_string(long val)
{
    return to_string_signed(val);
}

string to_string(unsigned long val)
{
    return to_string_unsigned(val);
}

string to_string(long long val)
{
    return to_string_signed(val);
}

string to_string(unsigned long long val)
{
    return to_string_unsigned(val);
}

string to_string(float val)
{
    return to_string(static_cast<double>(val));
}

string to_string(double x)
{
    const int minNumDecimals = 0;
    const int maxNumDecimals = 15;
    string result;
    if (x < 0)
    {
        x = -x;
        result.append(1, '-');
    }
    result.append(to_string(static_cast<int>(x)));
    double d = x - static_cast<int>(x);
    if (d > 0)
    {
        result.append(1, '.');
        for (int i = 0; (d > 0 || i < minNumDecimals) && i < maxNumDecimals; ++i)
        {
            d = 10 * d;
            int digit = static_cast<int>(d) % 10;
            result.append(1, static_cast<char>(static_cast<int>('0') + digit));
            d = d - static_cast<int>(d);
        }
    }
    else
    {
        result.append(".0");
    }
    return result;
}

char hex_char(int nibble)
{
    const char* hex_chars = "0123456789ABCDEF";
    return hex_chars[nibble];
}

string to_hexstring(uint8_t x)
{
    string s;
    s.append(1, hex_char(x >> 4));
    s.append(1, hex_char(x & 0x0F));
    return s;
}

string to_hexstring(uint16_t x)
{
    return to_hex(x);
}

string to_hexstring(uint32_t x)
{
    return to_hex(x);
}

string to_hexstring(uint64_t x)
{
    return to_hex(x);
}

string to_octstring(uint8_t x)
{
    return to_oct(x);
}

string to_octstring(uint16_t x)
{
    return to_oct(x);
}

string to_octstring(uint32_t x)
{
    return to_oct(x);
}

string to_octstring(uint64_t x)
{
    return to_oct(x);
}

template<typename T>
void parse_decimal(T& x, const string& s, size_t* idx)
{
    x = 0;
    bool negative = false;
    int state = 0;
    size_t index = 0;
    for (char c : s)
    {
        switch (state)
        {
            case 0:
            {
                if (!std::isspace(c))
                {
                    if (c == '+')
                    {
                        state = 1;
                    }
                    else if (c == '-')
                    {
                        negative = true;
                        state = 1;
                    }
                    else if (c >= '0' && c <= '9')
                    {
                        x = c - '0';
                        state = 1;
                    }
                    else
                    {
                        if (idx)
                        {
                            *idx = index;
                        }
                        return;
                    }
                }
                break;
            }
            case 1:
            {
                if (c >= '0' && c <= '9')
                {
                    x = 10 * x + (c - '0');
                }
                else
                {
                    if (idx)
                    {
                        *idx = index;
                    }
                    if (negative)
                    {
                        x = -x;
                    }
                    return;
                }
                break;
            }
        }
        ++index;
    }
    if (negative)
    {
        x = -x;
    }
    if (idx)
    {
        *idx = index;
    }
}

template<typename T>
void parse_hexadecimal(T& x, const string& s, size_t* idx)
{
    x = 0;
    bool negative = false;
    int state = 0;
    size_t index = 0;
    for (char c : s)
    {
        switch (state)
        {
        case 0:
        {
            if (!std::isspace(c))
            {
                if (c == '+')
                {
                    state = 1;
                }
                else if (c == '-')
                {
                    negative = true;
                    state = 1;
                }
                else if (c >= '0' && c <= '9')
                {
                    x = c - '0';
                    state = 1;
                }
                else if (c >= 'a' && c <= 'f')
                {
                    x = 10 + c - 'a';
                }
                else if (c >= 'A' && c <= 'F')
                {
                    x = 10 + c - 'A';
                }
                else
                {
                    if (idx)
                    {
                        *idx = index;
                    }
                    return;
                }
            }
            break;
        }
        case 1:
        {
            if (c >= '0' && c <= '9')
            {
                x = 16 * x + c - '0';
            }
            else if (c >= 'a' && c <= 'f')
            {
                x = 16 * x + 10 + c - 'a';
            }
            else if (c >= 'A' && c <= 'F')
            {
                x = 16 * x + 10 + c - 'A';
            }
            else
            {
                if (idx)
                {
                    *idx = index;
                }
                if (negative)
                {
                    x = -x;
                }
                return;
            }
            break;
        }
        }
        ++index;
    }
    if (negative)
    {
        x = -x;
    }
    if (idx)
    {
        *idx = index;
    }
}

template<typename T>
void parse_octal(T& x, const string& s, size_t* idx)
{
    x = 0;
    bool negative = false;
    int state = 0;
    size_t index = 0;
    for (char c : s)
    {
        switch (state)
        {
            case 0:
            {
                if (!std::isspace(c))
                {
                    if (c == '+')
                    {
                        state = 1;
                    }
                    else if (c == '-')
                    {
                        negative = true;
                        state = 1;
                    }
                    else if (c >= '0' && c <= '7')
                    {
                        x = c - '0';
                        state = 1;
                    }
                    else
                    {
                        if (idx)
                        {
                            *idx = index;
                        }
                        return;
                    }
                }
                break;
            }
            case 1:
            {
                if (c >= '0' && c <= '7')
                {
                    x = 8 * x + c - '0';
                }
                else
                {
                    if (idx)
                    {
                        *idx = index;
                    }
                    if (negative)
                    {
                        x = -x;
                    }
                    return;
                }
                break;
            }
        }
        ++index;
    }
    if (negative)
    {
        x = -x;
    }
    if (idx)
    {
        *idx = index;
    }
}

int stoi(const string& str, size_t* idx, int base)
{
    switch (base)
    {
        case 10:
        {
            int x = 0;
            parse_decimal(x, str, idx);
            return x;
        }
        case 16:
        {
            int x = 0;
            parse_hexadecimal(x, str, idx);
            return x;
        }
        case 8:
        {
            int x = 0;
            parse_octal(x, str, idx);
            return x;
        }
        default:
        {
            cerr << "stoi: unsupported base" << "\n";
            return 0;
        }
    }
    if (idx)
    {
        *idx = 0;
    }
    return 0;
}

long stol(const string& str, size_t* idx, int base)
{
    switch (base)
    {
        case 10:
        {
            long x = 0;
            parse_decimal(x, str, idx);
            return x;
        }
        case 16:
        {
            long x = 0;
            parse_hexadecimal(x, str, idx);
            return x;
        }
        case 8:
        {
            long x = 0;
            parse_octal(x, str, idx);
            return x;
        }
        default:
        {
            cerr << "stol: unsupported base" << "\n";
            return 0;
        }
    }
    if (idx)
    {
        *idx = 0;
    }
    return 0;
}

unsigned long stoul(const string& str, size_t* idx, int base)
{
    switch (base)
    {
        case 10:
        {
            unsigned long x = 0;
            parse_decimal(x, str, idx);
            return x;
        }
        case 16:
        {
            unsigned long x = 0;
            parse_hexadecimal(x, str, idx);
            return x;
        }
        case 8:
        {
            unsigned long x = 0;
            parse_octal(x, str, idx);
            return x;
        }
        default:
        {
            cerr << "stoul: unsupported base" << "\n";
            return 0;
        }
    }
    if (idx)
    {
        *idx = 0;
    }
    return 0;
}

long long stoll(const string& str, size_t* idx, int base)
{
    switch (base)
    {
        case 10:
        {
            long long x = 0;
            parse_decimal(x, str, idx);
            return x;
        }
        case 16:
        {
            long long x = 0;
            parse_hexadecimal(x, str, idx);
            return x;
        }
        case 8:
        {
            long long x = 0;
            parse_octal(x, str, idx);
            return x;
        }
        default:
        {
            cerr << "stoll: unsupported base" << "\n";
            return 0;
        }
    }
    if (idx)
    {
        *idx = 0;
    }
    return 0;
}

unsigned long long stoull(const string& str, size_t* idx, int base)
{
    switch (base)
    {
        case 10:
        {
            unsigned long long x = 0;
            parse_decimal(x, str, idx);
            return x;
        }
        case 16:
        {
            unsigned long long x = 0;
            parse_hexadecimal(x, str, idx);
            return x;
        }
        case 8:
        {
            unsigned long long x = 0;
            parse_octal(x, str, idx);
            return x;
        }
        default:
        {
            cerr << "stoull: unsupported base" << "\n";
            return 0;
        }
    }
    if (idx)
    {
        *idx = 0;
    }
    return 0;
}

float stof(const string& str, size_t* idx = nullptr)
{
    return ort_stof(str.c_str(), idx);
}

double stod(const string& str, size_t* idx = nullptr)
{
    return ort_stod(str.c_str(), idx);
}

double strtod(const char* str, char** str_end)
{
    return ort_strtod(str, str_end);
}

} // namespace std
