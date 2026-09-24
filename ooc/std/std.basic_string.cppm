export module std.basic_string;

import std.type.fundamental;
import std.utilities.utility;
import std.type_traits;
import std.rt;
import std.algorithm;
import std.new_delete_op;
import std.eh;

export namespace std {

char32_t nul = 0;

template<typename charT>
class basic_string
{
public:
    using value_type = charT;
    using size_type = int64_t;
    using difference_type = int64_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = pointer;
    using const_iterator = const_pointer;
    
    static const size_type npos = -1;
    
    inline basic_string() noexcept : chars(nullptr), len(0), res(0)
    {
    }
    basic_string(const basic_string& that) : chars(nullptr), len(that.len), res(0)
    {
        if (len > 0)
        {
            reserve(len);
            scpy(chars, that.chars);
        }
    }
    basic_string(const basic_string& that, size_type pos) : chars(nullptr), len(slen(that.chars + pos)), res(0)
    {
        if (len > 0)
        {
            reserve(len);
            scpy(chars, that.chars + pos);
        }
    }
    basic_string(const basic_string& that, size_type pos, size_type n) : chars(nullptr), len(0), res(0)
    {
        size_type ln = min(slen(that.chars + pos), n);
        if (ln > 0)
        {
            reserve(ln);
            len = slencpy(chars, that.chars + pos, n);
        }
    }
    basic_string(const charT* s, size_type n) : chars(nullptr), len(0), res(0)
    {
        if (n > 0)
        {
            reserve(n);
            len = slencpy(chars, s, n);
        }
    }
    basic_string(const charT* s) : chars(nullptr), len(slen(s)), res(0)
    {
        if (len > 0)
        {
            reserve(len);
            scpy(chars, s);
        }
    }
    basic_string(const charT* begin, const charT* end) : basic_string<charT>(begin, end - begin)
    {
    }
    basic_string(size_type n, charT c) : chars(nullptr), len(n), res(0)
    {
        if (n > 0)
        {
            reserve(n);
            for (size_type i = 0; i < n; ++i)
            {
                chars[i] = c;
            }
            chars[n] = '\0';
        }
    }
    inline basic_string(basic_string&& that) noexcept : chars(that.chars), len(that.len), res(that.res)
    {
        that.chars = nullptr;
        that.len = 0;
        that.res = 0;
    }
    ~basic_string() 
    {
        deallocate();
    }
    basic_string& operator=(const basic_string& that)
    {
        deallocate();
        reserve(that.len);
        len = that.len;
        if (len > 0)
        {
            scpy(chars, that.chars);
        }
        return *this;
    }
    basic_string& operator=(basic_string&& that) noexcept
    {
        swap(chars, that.chars);
        swap(len, that.len);
        swap(res, that.res);
        return *this;
    }
    basic_string& operator=(const charT* s)
    {
        deallocate();
        len = slen(s);
        reserve(len);
        scpy(chars, s);
        return *this;
    }
    basic_string& operator=(charT c)
    {
        deallocate();
        reserve(1);
        chars[0] = c;
        chars[1] = '\0';
        return *this;
    }
    inline iterator begin() noexcept
    { 
        return chars; 
    }
    inline const_iterator begin() const noexcept
    { 
        return chars; 
    }
    iterator end() noexcept
    {
        if (chars)
        {
            return chars + len;
        }
        else
        {
            return nullptr;
        }
    }
    const_iterator end() const noexcept
    {
        if (chars)
        {
            return chars + len;
        }
        else
        {
            return nullptr;
        }
    }
    inline const_iterator cbegin() const noexcept
    { 
        return chars; 
    }
    const_iterator cend() const noexcept
    {
        if (chars)
        {
            return chars + len;
        }
        else
        {
            return nullptr;
        }
    }
    inline size_type size() const noexcept
    { 
        return len; 
    }
    inline size_type length() const noexcept
    { 
        return len; 
    }
    inline size_type capacity() const noexcept
    { 
        return res; 
    }
    void reserve(size_type n)
    {
        if (n > 0)
        {
            size_type min_res = n + 1;
            if (min_res > res)
            {
                grow(min_res);
            }
        }
    }
    inline void clear() noexcept
    { 
        deallocate(); 
    }
    inline bool empty() const noexcept
    { 
        return len == 0; 
    }
    inline const_reference operator[](size_type pos) const noexcept
    {
        return chars[pos];
    }
    inline reference operator[](size_type pos) noexcept
    {
        return chars[pos];
    }
    inline const charT& front() const noexcept
    {
        return chars[0];
    }
    inline charT& front() noexcept
    {
        return chars[0];
    }
    inline const charT& back() const noexcept
    {
        return chars[len - 1];
    }
    inline charT& back() noexcept
    {
        return chars[len - 1];
    }
    inline basic_string& operator+=(const basic_string& str)
    {
        return append(str);
    }
    inline basic_string& operator+=(const charT* s)
    {
        return append(s);
    }
    inline basic_string& operator+=(charT c)
    {
        return append(1, c);
    }
    inline basic_string& append(const basic_string& str)
    {
        append_from(str.c_str(), str.length());
        return *this;
    }
    inline basic_string& append(const basic_string& str, size_type pos)
    {
        append_from(str.c_str() + pos, str.length() - pos);
        return *this;
    }
    inline basic_string& append(const basic_string& str, size_type pos, size_type n)
    {
        append_from(str.c_str() + pos, n);
        return *this;
    }
    inline basic_string& append(const charT* s, size_type n)
    {
        append_from(s, n);
        return *this;
    }
    inline basic_string& append(const charT* begin, const charT* end)
    {
        return append(begin, end - begin);
    }
    inline basic_string& append(const charT* s)
    {
        append_from(s, slen(s));
        return *this;
    }
    basic_string& append(size_type n, charT c)
    {
        if (n > 0)
        {
            reserve(len + n);
            for (size_type i = 0; i < n; ++i)
            {
                chars[len] = c;
                ++len;
            }
            chars[len] = '\0';
        }
        return *this;
    }
    inline void push_back(charT c)
    {
        append(1, c);
    }
    basic_string& assign(const basic_string& str)
    {
        clear();
        return append(str);
    }
    basic_string& assign(basic_string&& that)
    {
        std::swap(chars, that.chars);
        std::swap(len, that.len);
        std::swap(res, that.res);
        return *this;
    }
    basic_string& assign(const basic_string& str, size_type pos)
    {
        clear();
        return append(str, pos);
    }
    basic_string& assign(const basic_string& str, size_type pos, size_type n)
    {
        clear();
        return append(str, pos, n);
    }
    basic_string& assign(const charT* s, size_type n)
    {
        clear();
        append_from(s, n);
        return *this;
    }
    basic_string& assign(const charT* s)
    {
        clear();
        append_from(s, slen(s));
        return *this;
    }
    basic_string& assign(size_type n, charT c)
    {
        clear();
        return append(n, c);
    }
    basic_string& insert(size_type index, const basic_string& str)
    {
        insert_from(str.c_str(), str.length(), index);
        return *this;
    }
    basic_string& insert(size_type index1, const basic_string& str, size_type index2, size_type n = npos)
    {
        if (n == npos)
        {
            n = str.length() - index2;
        }
        insert_from(str.c_str() + index2, n, index1);
        return *this;
    }
    basic_string& insert(size_type index, const charT* s, size_type n)
    {
        insert_from(s, n, index);
        return *this;
    }
    basic_string& insert(size_type index, const charT* s)
    {
        insert_from(s, slen(s), index);
        return *this;
    }
    basic_string& insert(size_type index, size_type n, charT c)
    {
        basic_string that(n, c);
        insert_from(that.c_str(), n, index);
        return *this;
    }
    iterator insert(const_iterator p, size_type n, charT c)
    {
        basic_string that(n, c);
        size_type index = p - begin();
        insert_from(that.c_str(), n, index);
        return begin() + index;
    }
    inline iterator insert(const_iterator p, charT c)
    {
        return insert(p, 1, c);
    }

    basic_string& erase(size_type index = 0, size_type count = npos)
    {
        if (index == 0 && count == npos)
        {
            clear();
        }
        else
        {
            if (count == npos)
            {
                count = length() - index;
            }
            iterator p = begin() + index;
            iterator e = end();
            copy(p + count, e, p);
            iterator new_end = e - count;
            len -= count;
            *new_end = '\0';
        }
        return *this;
    }
    iterator erase(iterator position)
    {
        size_type index = position - begin();
        erase(index, 1);
        iterator p = begin() + index;
        iterator e = end();
        if (p < e)
        {
            return p;
        }
        else
        {
            return e;
        }
    }
    iterator erase(const_iterator position)
    {
        size_type index = position - begin();
        erase(index, 1);
        iterator p = begin() + index;
        iterator e = end();
        if (p < e)
        {
            return p;
        }
        else
        {
            return e;
        }
    }
    iterator erase(iterator first, iterator last)
    {
        size_type index = first - begin();
        size_type count = last - first;
        size_type last_index = last - begin();
        erase(index, count);
        iterator p = begin() + last_index;
        iterator e = end();
        if (p < e)
        {
            return p;
        }
        else
        {
            return e;
        }
    }

    basic_string& replace(size_type pos, size_type count, const basic_string& str)
    {
        erase(pos, count);
        return insert(pos, str);
    }
    inline basic_string& replace(const_iterator first, const_iterator last, const basic_string& str)
    {
        return replace(first - begin(), last - first, str);
    }
    basic_string& replace(size_type pos, size_type count, const basic_string& str, size_type pos2, size_type count2 = npos)
    {
        basic_string s = str.substr(pos2, count2);
        return replace(pos, count, s);
    }
    basic_string& replace(size_type pos, size_type count, const charT* cstr, size_type count2)
    {
        erase(pos, count);
        return insert(pos, cstr, count2);
    }
    basic_string& replace(const_iterator first, const_iterator last, const charT* cstr, size_type count2)
    {
        size_type index = first - begin();
        erase(first, last);
        return insert(index, cstr, count2);
    }
    inline basic_string& replace(size_type pos, size_type count, const charT* cstr)
    {
        return replace(pos, count, cstr, slen(cstr));
    }
    inline basic_string& replace(const_iterator first, const_iterator last, const charT* cstr)
    {
        return replace(first, last, cstr, slen(cstr));
    }
    basic_string& replace(size_type pos, size_type count, size_type count2, charT c)
    {
        basic_string str(count2, c);
        return replace(pos, count, str);
    }

    size_type copy(charT* dest, size_type count, size_type pos = 0) const
    {
        if (count == npos || pos + count >= length())
        {
            count = length() - pos;
        }
        if (chars)
        {
            for (size_type i = 0; i < count; ++i)
            {
                size_type p = i + pos;
                *dest++ = chars[p];
            }
            return count;
        }
        else
        { 
            return 0;
        }
    }
    inline void swap(basic_string& that) noexcept
    {
        std::swap(chars, that.chars);
        std::swap(len, that.len);
        std::swap(res, that.res);
    }
    const charT* c_str() const noexcept
    {
        if (chars)
        {
            return chars;
        }
        else
        {
            return static_cast<const charT*>(static_cast<void*>(&nul));
        }
    }
    inline const charT* data() const noexcept
    { 
        return chars; 
    }
    inline charT* data() noexcept
    { 
        return chars; 
    }
    inline size_type find(const basic_string& str) const noexcept
    {
        return find(str, 0);
    }
    size_type find(const basic_string& str, size_type pos) const noexcept
    {
        if (str.empty()) return pos;
        size_type p = find(str[0], pos);
        while (p != npos)
        {
            bool found = true;
            for (size_type i = 1; i < str.length(); ++i)
            {
                if (chars[i + p] != str[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                return p;
            }
            p = find(str[0], p + 1);
        }
        return npos;
    }
    inline size_type find(const charT* s) const noexcept
    {
        return find(s, 0);
    }
    inline size_type find(const charT* s, size_type pos) const noexcept
    {
        return find(s, slen(s), pos);
    }
    size_type find(const charT* s, size_type n, size_type pos) const noexcept
    {
        if (n == 0) return pos;
        size_type p = find(s[0], pos);
        while (p != npos)
        {
            bool found = true;
            for (size_type i = 1; i < n; ++i)
            {
                if (chars[i + p] != s[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                return p;
            }
            p = find(s[0], p + 1);
        }
        return npos;
    }
    inline size_type find(charT c) const noexcept
    {
        return find(c, 0);
    }
    size_type find(charT c, size_type pos) const noexcept
    {
        if (!chars) return npos;
        for (size_type i = pos; i < len; ++i)
        {
            if (chars[i] == c)
            {
                return i;
            }
        }
        return npos;
    }
    inline size_type rfind(const basic_string& str) const noexcept
    {
        return rfind(str, npos);
    }
    size_type rfind(const basic_string& str, size_type pos) const noexcept
    {
        if (str.empty()) return pos;
        size_type p = rfind(str[0], pos);
        while (p != npos)
        {
            bool found = true;
            for (size_type i = 1; i < str.length(); ++i)
            {
                if (chars[i + p] != str[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                return p;
            }
            p = rfind(str[0], p - 1);
        }
        return npos;
    }
    inline size_type rfind(const charT* s) const noexcept
    {
        return rfind(s, slen(s));
    }
    size_type rfind(const charT* s, size_type n, size_type pos = npos) const noexcept
    {
        if (n == 0) return pos;
        size_type p = rfind(s[0], pos);
        while (p != npos)
        {
            bool found = true;
            for (size_type i = 1; i < n; ++i)
            {
                if (chars[i + p] != s[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                return p;
            }
            p = rfind(s[0], p - 1);
        }
        return npos;
    }
    inline size_type rfind(charT c) const noexcept
    {
        return rfind(c, npos);
    }
    size_type rfind(charT c, size_type pos) const noexcept
    {
        if (!chars) return npos;
        if (pos == npos)
        {
            pos = len - 1;
        }
        for (size_type i = pos; i >= 0; --i)
        {
            if (chars[i] == c)
            {
                return i;
            }
        }
        return npos;
    }

    //size_type find_first_of(const basic_string& str, size_type pos = 0) const;
    //size_type find_first_of(const charT* s, size_type pos, size_type n) const;
    //size_type find_first_of(const charT* s, size_type pos = 0) const;
    //size_type find_first_of(charT c, size_type pos = 0) const;
    //
    //size_type find_last_of(const basic_string& str, size_type pos = npos) const;
    //size_type find_last_of(const charT* s, size_type pos, size_type n) const;
    //size_type find_last_of(const charT* s, size_type pos = npos) const;
    //size_type find_last_of(charT c, size_type pos = npos) const;
    //
    //size_type find_first_not_of(const basic_string& str, size_type pos = 0) const;
    //size_type find_first_not_of(const charT* s, size_type pos, size_type n) const;
    //size_type find_first_not_of(const charT* s, size_type pos = 0) const;
    //size_type find_first_not_of(charT c, size_type pos = 0) const;

    //size_type find_last_not_of(const basic_string& str, size_type pos = npos) const;
    //size_type find_last_not_of(const charT* s, size_type pos, size_type n) const;
    //size_type find_last_not_of(const charT* s, size_type pos = npos) const;
    //size_type find_last_not_of(charT c, size_type pos = npos) const;
    
    basic_string substr(size_type pos = 0, size_type count = npos) const
    {
        if (pos >= 0 && pos < len)
        {
            if (count == npos)
            {
                count = len - pos;
            }
            return basic_string<charT>(chars + pos, count);
        }
        else
        {
            return basic_string<charT>();
        }
    }
   
    //int compare(const basic_string& str) const;
    //int compare(size_type pos1, size_type n1, const basic_string& str) const;
    //int compare(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2 = npos) const;
    //int compare(const charT* s) const;
    //int compare(size_type pos1, size_type n1, const charT* s) const;
    //int compare(size_type pos1, size_type n1, const charT* s, size_type n2) const;
    
    inline bool starts_with(charT x) const noexcept
    {
        if (empty()) return false;
        return chars[0] == x;
    }
    bool starts_with(const charT* s) const noexcept
    {
        size_type n = slen(s);
        if (len < n) return false;
        for (size_type i = 0; i < n; ++i)
        {
            if (chars[i] != s[i]) return false;
        }
        return true;
    }
    inline bool starts_with(const basic_string<charT>& s) const noexcept
    {
        return starts_with(s.c_str());
    }
    inline bool ends_with(charT x) const noexcept
    {
        if (empty()) return false;
        return chars[len - 1] == x;
    }
    bool ends_with(const charT* s) const noexcept
    {
        size_type n = len;
        size_type m = slen(s);
        if (n < m) return false;
        for (size_type i = 0; i < m; ++i)
        {
            if (chars[i + n - m] != s[i]) return false;
        }
        return true;
    }
    inline bool ends_with(const basic_string<charT>& s) const noexcept
    {
        return ends_with(s.c_str());
    }
    inline bool contains(charT x) const noexcept
    {
        return find(x) != npos;
    }
    inline bool contains(const charT* s) const noexcept
    {
        return find(s) != npos;
    }
private:
    void append_from(const charT* s, size_type n)
    {
        size_type newlen = len + n;
        if (newlen > 0)
        {
            reserve(newlen);
            newlen = len + slencpy(chars + len, s, n);
        }
        len = newlen;
    }
    void insert_from(const charT* s, size_type n, size_type index)
    {
        size_type new_len = len + n;
        if (new_len > 0)
        {
            reserve(new_len);
            iterator b = begin();
            iterator e = end();
            iterator new_end = e + n;
            iterator p = b + index;
            copy_backward(p, e, new_end);
            copy(s, s + n, p);
            *new_end = '\0';
        }
        len = new_len;
    }
    void grow(size_type min_res)
    {
        min_res = std::grow_size(min_res);
        void* ptr = operator new(min_res * sizeof(charT));
        charT* new_chars = static_cast<charT*>(ptr);
        if (chars)
        {
            scpy(new_chars, chars);
            std::free(chars);
        }
        chars = new_chars;
        res = min_res;
    }
    void deallocate() noexcept
    {
        len = 0;
        if (res != 0)
        {
            std::free(chars);
            res = 0;
        }
        chars = nullptr;
    }
    charT* chars;
    size_type len;
    size_type res;
};

template<typename charT>
basic_string<charT> operator+(const basic_string<charT>& lhs, const basic_string<charT>& rhs)
{
    basic_string<charT> result(lhs);
    result.append(rhs);
    return result;
}

template<typename charT>
basic_string<charT> operator+(const charT* lhs, const basic_string<charT>& rhs)
{
    basic_string<charT> result(lhs);
    result.append(rhs);
    return result;
}

template<typename charT>
basic_string<charT> operator+(const basic_string<charT>& lhs, const charT* rhs)
{
    basic_string<charT> result(lhs);
    result.append(rhs);
    return result;
}

template<typename charT>
bool operator==(const basic_string<charT>& lhs, const basic_string<charT>& rhs) noexcept
{
    basic_string<charT>::size_type len = lhs.length();
    if (len != rhs.length()) return false;
    for (basic_string<charT>::size_type i = 0; i < len; ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }
    return true;
}

template<typename charT>
bool operator==(const basic_string<charT>& lhs, const charT* rhs) noexcept
{
    basic_string<charT>::size_type len = lhs.length();
    if (len != slen(rhs)) return false;
    for (basic_string<charT>::size_type i = 0; i < len; ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }
    return true;
}

template<typename charT>
bool operator==(const charT* lhs, const basic_string<charT>& rhs) noexcept
{
    basic_string<charT>::size_type len = slen(lhs);
    if (len != rhs.length()) return false;
    for (basic_string<charT>::size_type i = 0; i < len; ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }
    return true;
}

template<typename charT>
bool operator<(const basic_string<charT>& lhs, const basic_string<charT>& rhs) noexcept
{
    basic_string<charT>::size_type leftLen = lhs.length();
    basic_string<charT>::size_type rightLen = rhs.length();
    if (leftLen == 0 && rightLen > 0) return true;
    if (leftLen > 0 && rightLen == 0) return false;
    basic_string<charT>::size_type n = min(leftLen, rightLen);
    for (basic_string<charT>::size_type i = 0; i < n; ++i)
    {
        charT l = lhs[i];
        charT r = rhs[i];
        if (l < r) return true;
        if (l > r) return false;
    }
    if (leftLen < rightLen) return true;
    return false;
}

using string = basic_string<char>;
using u8string = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;
using wstring = basic_string<wchar_t>;

string to_string(int val);
string to_string(unsigned val);
string to_string(long val);
string to_string(unsigned long val);
string to_string(long long val);
string to_string(unsigned long long val);
string to_string(float val);
string to_string(double val);
// string to_string(long double val);

char hex_char(int nibble);

template<typename T>
string to_hex(const T& x)
{
    string s;
    int n = static_cast<int>(sizeof(x));
    for (int i = 0; i < n; ++i)
    {
        uint8_t b = static_cast<uint8_t>(x & 0xFF);
        s.append(1, hex_char(b & 0x0F));
        s.append(1, hex_char(b >> 4));
        x = static_cast<T>(x >> 8);
    }
    std::reverse(s.begin(), s.end());
    return s;
}

string to_hexstring(uint8_t x);
string to_hexstring(uint16_t x);
string to_hexstring(uint32_t x);
string to_hexstring(uint64_t x);

template<typename T>
string to_oct(T x)
{
    string s;
    do
    {
        uint8_t b = static_cast<uint8_t>(x);
        s.append(1, static_cast<char>(static_cast<int>(b & 7) + static_cast<int>('0')));
        x = static_cast<T>(x >> static_cast<T>(3));
    } while (x != 0);
    if (s[s.length() - 1] != '0')
    {
        s.append(1, '0');
    }
    std::reverse(s.begin(), s.end());
    return s;
}

string to_octstring(uint8_t x);
string to_octstring(uint16_t x);
string to_octstring(uint32_t x);
string to_octstring(uint64_t x);

/*
wstring to_wstring(int val);
wstring to_wstring(unsigned val);
wstring to_wstring(long val);
wstring to_wstring(unsigned long val);
wstring to_wstring(long long val);
wstring to_wstring(unsigned long long val);
wstring to_wstring(float val);
wstring to_wstring(double val);
wstring to_wstring(long double val);
*/

int stoi(const string& str, size_t* idx = nullptr, int base = 10);
long stol(const string& str, size_t* idx = nullptr, int base = 10);
unsigned long stoul(const string& str, size_t* idx = nullptr, int base = 10);
long long stoll(const string& str, size_t* idx = nullptr, int base = 10);
unsigned long long stoull(const string& str, size_t* idx = nullptr, int base = 10);
float stof(const string& str, size_t* idx = nullptr);
double stod(const string& str, size_t* idx = nullptr);
double strtod(const char* str, char** str_end);

int stoi(const wstring& str, size_t* idx = nullptr, int base = 10);
long stol(const wstring& str, size_t* idx = nullptr, int base = 10);
unsigned long stoul(const wstring& str, size_t* idx = nullptr, int base = 10);
long long stoll(const wstring& str, size_t* idx = nullptr, int base = 10);
unsigned long long stoull(const wstring& str, size_t* idx = nullptr, int base = 10);
float stof(const wstring& str, size_t* idx = nullptr);
double stod(const wstring& str, size_t* idx = nullptr);
long double stold(const wstring& str, size_t* idx = nullptr);

template class basic_string<char>; // explicit instantiation for char type
template class basic_string<char16_t>; // explicit instantiation for char16_t type
template class basic_string<char32_t>; // explicit instantiation for char32_t type

} // namespace std
