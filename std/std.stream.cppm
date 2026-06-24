export module std.stream;

import std.type.fundamental;
import std.basic_string;
import std.utf;
import std.atexit;

export namespace std {

class ios_base
{
public:
    ios_base();
    virtual ~ios_base();
    enum fmtflags
    {
        dec = 1 << 0, oct = 1 << 1, hex = 1 << 2, basefield = dec | oct | hex
    };
    enum openmode
    {
        app, ate, binary, in, out, trunc
    };
    fmtflags flags() const noexcept;
    void flags(fmtflags f) noexcept;
    void setf(fmtflags f) noexcept;
    void unsetf(fmtflags f) noexcept;
    void init() noexcept;
private:
    fmtflags fl;
};

template<typename T>
string to_base_string(const T& x, ios_base::fmtflags f)
{
    if ((int(f) & int(ios_base::dec)) != 0)
    {
        return to_string(x);
    }
    else if ((int(f) & int(ios_base::oct)) != 0)
    {
        return to_octstring(x);
    }
    else if ((int(f) & int(ios_base::hex)) != 0)
    {
        return to_hexstring(x);
    }
    else
    {
        return to_string(x);
    }
}

template<typename CharT>
class basic_ios : public ios_base
{
public:
};

template<typename CharT>
class basic_istream : public basic_ios<CharT>
{
public:
};

template<typename CharT>
class basic_ostream : public basic_ios<CharT>
{
public:
    basic_ostream() : handle(1)
    {
    }
    explicit basic_ostream(int handle_) : handle(handle_)
    {
    }
    basic_ostream<CharT>& flush()
    {
        ort_flush_handle(handle);
        return *this;
    }
    virtual void write(const char* s) 
    {
        ort_io_write(handle, s);
    }
    void write(const string& s) 
    {
        write(s.c_str());
    }
private:
    int handle;
};

template<typename CharT>
class basic_iostream : public basic_istream<CharT>, public basic_ostream<CharT>
{
public:
    basic_iostream() : basic_istream<CharT>(), basic_ostream<CharT>()
    {
    }
};

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, char c)
{
    basic_string<CharT> str(1, c);
    s.write(to_utf8(str));
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, int x)
{
    string str = to_base_string(x, s.flags());
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, long long x)
{
    string str = to_base_string(x, s.flags());
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, const char* str)
{
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, const string& str)
{
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, bool x)
{
    if (x) return s << "true"; else return s << "false";
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, long x)
{
    string str = to_base_string(x, s.flags());
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, unsigned long x)
{
    string str = to_base_string(x, s.flags());
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, unsigned long long x)
{
    string str = to_base_string(x, s.flags());
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, double x)
{
    string str(to_string(x));
    return s << str;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, const void* x)
{
    unsigned long long value = static_cast<unsigned long long>(x);
    return s << value;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, nullptr_t)
{
    return s << "nullptr";
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, short x)
{
    string str = to_base_string(x, s.flags());
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>&s, unsigned short x)
{
    string str = to_base_string(x, s.flags());
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, unsigned int x)
{
    string str = to_base_string(x, s.flags());
    s.write(str);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, float x)
{
    string str(to_string(x));
    return s << str;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, ios_base& (*f)(ios_base&))
{
    f(s);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, basic_ios<CharT>& (*f)(basic_ios<CharT>&))
{
    f(s);
    return s;
}

template<typename CharT>
basic_ostream<CharT>& operator<<(basic_ostream<CharT>& s, basic_ostream<CharT>& (*f)(basic_ostream<CharT>&))
{
    f(s);
    return s;
}

ios_base& dec(ios_base& strm);
ios_base& hex(ios_base& strm);
ios_base& oct(ios_base& strm);

template<typename CharT>
basic_ostream<CharT>& endl(basic_ostream<CharT>& strm)
{
    strm << "\n";
    return strm.flush();
}

template<typename CharT>
class basic_ofstream : public basic_ostream<CharT>
{
public:
    basic_ofstream(const char* filename) : file(nullptr)
    {
        open(filename, std::ios_base::out);
    }
    basic_ofstream(const char* filename, std::ios_base::openmode mode) : file(nullptr)
    {
        open(filename, mode);
    }
    basic_ofstream(const std::string& filename) : file(nullptr)
    {
        open(filename.c_str(), std::ios_base::out);
    }
    basic_ofstream(const std::string& filename, std::ios_base::openmode mode) : file(nullptr)
    {
        open(filename.c_str(), mode);
    }
    ~basic_ofstream()
    {
        if (file)
        {
            std::fclose(file);
        }
    }
    void write(const char* s) override
    {
        std::fputs(s, file);
    }
    void open(const char* filename, std::ios_base::openmode mode = std::ios_base::out)
    {
        if (mode == std::ios_base::out)
        {
            file = std::fopen(filename, "w");
        }
        else if (mode == std::ios_base::app)
        {
            file = std::fopen(filename, "a");
        }
    }
    inline explicit operator bool() const noexcept { return file != nullptr; }
private:
    void* file;
};

template<typename CharT>
class basic_istringstream;;
template<typename CharT>
class basic_ostringstream;
template<typename CharT>
class basic_stringstream;
template<typename CharT>
class basic_ifstream;
template<typename CharT>
class basic_fstream;
using ios = basic_ios<char>;
using wios = basic_ios<wchar_t>;
using istream = basic_istream<char>;
using ostream = basic_ostream<char>;
using iostream = basic_iostream<char>;
using ifstream = basic_ifstream<char>;
using ofstream = basic_ofstream<char>;
using fstream = basic_fstream<char>;
using wistream = basic_istream<wchar_t>;
using wostream = basic_ostream<wchar_t>;
using wistringstream = basic_istringstream<wchar_t>;
using wostringstream = basic_ostringstream<wchar_t>;
using wstringstream = basic_stringstream<wchar_t>;
using wifstream = basic_ifstream<wchar_t>;
using wofstream = basic_ofstream<wchar_t>;
using wfstream = basic_fstream<wchar_t>;

extern ostream cout;
extern ostream cerr;

template class basic_ostream<char>;

/*
template class basic_stringstream<char>;
*/

} // namespace std
