export module std.container.vector;

import std.type.fundamental;
import std.utilities.utility;
import std.type_traits;
import std.rt;
import std.algorithm;
import std.new_delete_op;

export namespace std {

template<typename T>
void constructive_copy(T* to, T* from, ssize_t count)
{
    for (ssize_t i = 0; i < count; ++i)
    {
        new (to) T(*from);
        ++to;
        ++from;
    }
}

template<typename T>
void constructive_move(T* to, T* from, ssize_t count)
{
    for (ssize_t i = 0; i < count; ++i)
    {
        new (to) T(std::move(*from));
        ++to;
        ++from;
    }
}

template<typename T>
void constructive_move_backward(T* to, T* from, ssize_t count)
{
    for (ssize_t i = 0; i < count; ++i)
    {
        --to;
        --from;
        new (to) T(std::move(*from));
    }
}

template<typename T>
void destruct(T* elements, ssize_t count)
{
    for (ssize_t i = 0; i < count; ++i)
    {
        elements->T::~T();
        ++elements;
    }
}

template<typename T>
class vector
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = int64_t;
    using difference_type = int64_t;
    using iterator = value_type*;
    using const_iterator = const value_type*;

    inline vector() noexcept : elements(nullptr), sz(0), res(0) {}
    explicit vector(size_type n) : elements(nullptr), sz(n), res(0)
    {
        reserve(sz);
        for (size_type i = 0; i < sz; ++i)
        {
            new (elements + i) T();
        }
    }
    vector(size_type n, const value_type& value) : elements(nullptr), sz(n), res(0)
    {
        reserve(sz);
        for (size_type i = 0; i < sz; ++i)
        {
            new (elements + i) T(value);
        }
    }
    vector(const vector& that) : elements(nullptr), sz(that.sz), res(0)
    {
        if (sz > 0)
        {
            reserve(sz);
            constructive_copy(elements, that.elements, sz);
        }
    }
    vector(vector&& that) noexcept : elements(that.elements), sz(that.sz), res(that.res)
    {
        that.elements = static_cast<T*>(nullptr);
        that.sz = 0;
        that.res = 0;
    }
    ~vector()
    {
        destruct();
    }
    vector& operator=(const vector& that)
    {
        clear();
        sz = that.sz;
        reserve(sz);
        constructive_copy(elements, that.elements, sz);
        return *this;
    }
    vector& operator=(vector&& that) noexcept
    {
        std::swap(elements, that.elements);
        std::swap(sz, that.sz);
        std::swap(res, that.res);
        return *this;
    }
    void assign(size_type n, const T& value)
    {
        clear();
        reserve(n);
        for (size_type i = 0; i < n; ++i)
        {
            push_back(value);
        }
    }
    
    inline iterator begin() noexcept { return elements; }
    inline const_iterator begin() const noexcept { return elements; }
    iterator end() noexcept
    {
        if (elements)
        {
            return elements + sz;
        }
        else
        {
            return nullptr;
        }
    }
    const_iterator end() const noexcept
    {
        if (elements)
        {
            return elements + sz;
        }
        else
        {
            return nullptr;
        }
    }
    inline const_iterator cbegin() const noexcept { return elements; }
    const_iterator cend() const noexcept
    {
        if (elements)
        {
            return elements + sz;
        }
        else
        {
            return nullptr;
        }
    }

    inline bool empty() const noexcept { return sz == 0; }
    inline size_type size() const noexcept { return sz; }
    inline size_type capacity() const noexcept { return res; }
    void resize(size_type new_sz)
    {
        if (new_sz != sz)
        {
            if (new_sz < sz)
            {
                for (size_type i = new_sz; i < sz; ++i)
                {
                    T* p = elements + i;
                    p->T::~T();
                }
            }
            else if (new_sz > sz)
            {
                reserve(new_sz);
                for (size_type i = sz; i < new_sz; ++i)
                {
                    new (elements + i) T();
                }
            }
            sz = new_sz;
        }
    }
    void resize(size_type new_sz, const T& c)
    {
        if (new_sz != sz)
        {
            if (new_sz < sz)
            {
                for (size_type i = new_sz; i < sz; ++i)
                {
                    T* p = elements + i;
                    p->T::~T();
                }
            }
            else if (new_sz > sz)
            {
                reserve(new_sz);
                for (size_type i = sz; i < new_sz; ++i)
                {
                    new (elements + i) T(c);
                }
            }
            sz = new_sz;
        }
    }
    void reserve(size_type n)
    {
        if (n > res)
        {
            grow(n);
        }
    }
   
    inline reference operator[](size_type n) noexcept
    {
        return elements[n];
    }
    inline const_reference operator[](size_type n) const noexcept
    {
        return elements[n];
    }
    inline reference front() noexcept
    {
        return elements[0];
    }
    inline const_reference front() const noexcept
    {
        return elements[0];
    }
    inline reference back() noexcept
    {
        return elements[sz - 1];
    }
    inline const_reference back() const noexcept
    {
        return elements[sz - 1];
    }
    inline pointer data() noexcept { return elements; }
    inline const_pointer data() const noexcept { return elements; }

    void push_back(const T& x)
    {
        reserve(sz + 1);
        new (elements + sz) T(x);
        ++sz;
    }
    void push_back(T&& x)
    {
        reserve(sz + 1);
        new (elements + sz) T(std::move(x));
        ++sz;
    }
    void pop_back()
    {
        --sz;
        T* p = elements + sz;
        p->T::~T();
    }

    iterator insert(const_iterator position, const T& x)
    {
        size_type index = position - begin();
        reserve(sz + 1);
        position = begin() + index;
        iterator e = end();
        constructive_move_backward(e + 1, e, e - position);
        new (position) T(x);
        ++sz;
        return begin() + index;
    }
    iterator insert(const_iterator position, T&& x)
    {
        size_type index = position - begin();
        reserve(sz + 1);
        position = begin() + index;
        iterator e = end();
        constructive_move_backward(e + 1, e, e - position);
        new (position) T(std::move(x));
        ++sz;
        return begin() + index;
    }
    iterator insert(const_iterator position, size_type n, const T& x)
    {
        size_type index = position - cbegin();
        iterator first = end();
        for (size_type i = 0; i < n; ++i)
        {
            const_iterator p = cbegin() + index;
            iterator it = insert(p, x);
            if (i == 0)
            {
                first = it;
            }
            ++index;
        }
        return first;
    }
    iterator erase(iterator pos)
    {
        size_type index = pos - begin();
        pos->T::~T();
        constructive_move(pos, pos + 1, end() - pos);
        --sz;
        if (sz > 0)
        {
            return begin() + index;
        }
        else
        {
            return end();
        }
    }
    iterator erase(const_iterator pos)
    {
        size_type index = pos - begin();
        pos->T::~T();
        constructive_move(pos, pos + 1, end() - pos);
        --sz;
        if (sz > 0)
        {
            return begin() + index;
        }
        else
        {
            return end();
        }
    }
    iterator erase(iterator first, iterator last)
    {
        if (first == last) return last;
        size_type index = first - begin();
        size_type n = last - first;
        for (iterator it = first; it != last; ++it)
        { 
            it->T::~T();
        }
        size_type m = end() - last;
        constructive_move(first, last, m);
        sz -= n;
        if (sz > 0)
        {
            return begin() + index;
        }
        else
        {
            return end();
        }
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        if (first == last) return last;
        size_type index = first - begin();
        size_type n = last - first;
        for (iterator it = first; it != last; ++it)
        {
            it->T::~T();
        }
        size_type m = end() - last;
        constructive_move(first, last, m);
        sz -= n;
        if (sz > 0)
        {
            return begin() + index;
        }
        else
        {
            return end();
        }
    }
    inline void clear() noexcept
    {
        destruct();
    }
    void print()
    {
        std::cout << "sz=" << sz << ", res=" << res << "\n";
    }
private:
    void destruct() noexcept
    {
        if (sz > 0)
        {
            destruct(elements, sz);
            sz = 0;
        }
        if (res > 0)
        {
            std::free(elements);
            elements = static_cast<T*>(nullptr);
            res = 0;
        }
    }
    void grow(size_type min_res)
    {
        min_res = std::grow_size(min_res);
        void* ptr = operator new(min_res * sizeof(value_type));
        value_type* new_elements = static_cast<value_type*>(ptr);
        if (elements)
        {
            constructive_move(new_elements, elements, sz);
            size_type save_sz = sz;
            destruct();
            sz = save_sz;
        }
        elements = new_elements;
        res = min_res;
    }
    T* elements;
    size_type sz;
    size_type res;
};

template<typename T>
bool operator==(const vector<T>& x, const vector<T>& y) noexcept
{
    if (x.size() != y.size()) return false;
    typename vector<T>::size_type n = x.size();
    for (typename vector<T>::size_type i = 0; i < n; ++i)
    {
        if (x[i] != y[i]) return false;
    }
    return true;
}

template<typename T>
inline bool operator<(const vector<T>& x, const vector<T>& y) noexcept
{
    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

} // namespace std
