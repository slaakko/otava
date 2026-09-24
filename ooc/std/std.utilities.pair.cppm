export module std.utilities.pair;

import std.type.fundamental;
import std.utilities.utility;

export namespace std {

template<typename T, typename U>
struct pair
{
    using first_type = T;
    using second_type = U;

    T first;
    U second;
    inline pair() : first(), second() {}
    inline pair(const T& first_, const U& second_) : first(first_), second(second_) {}
    inline pair(T&& first_, U&& second_) noexcept : first(std::move(first_)), second(std::move(second_)) {}
    inline pair(const pair& that) : first(that.first), second(that.second) {}
    inline pair(pair&& that) noexcept : first(std::move(that.first)), second(std::move(that.second)) {}
    inline pair& operator=(const pair& p) { first = p.first; second = p.second; return *this; }
    inline pair& operator=(pair&& p) noexcept { std::swap(first, p.first); std::swap(second, p.second); return *this; }
};

template<typename T, typename U>
inline bool operator==(const pair<T, U>& x, const pair<T, U>& y) noexcept
{
    return x.first == y.first && x.second == y.second;
}

template<typename T, typename U>
inline bool operator<(const pair<T, U>& x, const pair<T, U>& y) noexcept
{
    if (x.first < y.first) return true; 
    if (x.first > y.first) return false;
    return x.second < y.second;
}

template<typename T, typename U>
pair<T, U> make_pair(const T& x, const U& y)
{
    pair<T, U> p(x, y);
    return p;
}

template<typename T, typename U>
pair<T, U> make_pair(T&& x, U&& y)
{
    pair<T, U> p(std::move(x), std::move(y));
    return p;
}

template<typename T, typename U>
struct select_first
{
    inline const T& operator()(const pair<T, U>& p) const noexcept { return p.first; }
};

template<typename T, typename U>
struct select_second
{
    inline const U& operator()(const pair<T, U>& p) const noexcept { return p.second; }
};

} // namespace std
