export module std.algorithm;

import std.utilities.utility;
import std.type_traits;
import std.utilities.pair;
import std.iterator;
import std.rt;

export namespace std {

template<typename T>
constexpr const T& min(const T& left, const T& right) noexcept
{
    if (left <= right) return left;
    else return right;
}

template<typename T>
constexpr const T& max(const T& left, const T& right) noexcept
{
    if (left >= right) return left;
    else return right;
}

template<typename T>
constexpr void swap(T& left, T& right) noexcept
{
    T temp(move(left));
    left = move(right);
    right = move(temp);
}

namespace detail {

template<typename I>
void reverse_iter(I begin, I end, std::bidirectional_iterator_tag) noexcept
{
    while (true)
    {
        if (begin == end) return;
        --end;
        if (begin == end) return;
        swap(*begin, *end);
        ++begin;
    }
}

template<typename I>
void reverse_iter(I begin, I end, std::random_access_iterator_tag) noexcept
{
    while (begin < end)
    {
        --end;
        swap(*begin, *end);
        ++begin;
    }
}

} // namespace detail

template<typename I> 
constexpr void reverse(I begin, I end) noexcept
{
    detail::reverse_iter(begin, end, typename std::iterator_traits<I>::iterator_category());
}

template<typename I>
pair<I, I> reverse_until(I first, I middle, I last) noexcept
{
    while (first != middle && middle != last)
    {
        --last;
        swap(*first, *last);
        ++first;
    }
    return make_pair(first, last);
}

template<typename I>
I rotate(I first, I middle, I last) noexcept
{
    reverse(first, middle);
    reverse(middle, last);
    pair<I, I> p = reverse_until(first, middle, last);
    if (middle == p.first) return p.second;
    return p.first;
}

template<typename I, typename O>
O copy(I begin, I end, O to)
{
    while (begin != end)
    {
        *to = *begin;
        ++begin;
        ++to;
    }
    return to;
}

template<typename I, typename O>
O copy_backward(I begin, I end, O to)
{
    while (begin != end)
    {
        --to;
        --end;
        *to = *end;
    }
    return to;
}

template<typename I, typename T>
I find(I begin, I end, const T& value) noexcept
{
    while (begin != end)
    {
        if (*begin == value)
        {
            return begin;
        }
        ++begin;
    }
    return end;
}

template<typename I, typename P>
I find_if(I begin, I end, P p) noexcept
{
    while (begin != end)
    {
        if (p(*begin))
        {
            return begin;
        }
        ++begin;
    }
    return end;
}

template<typename I, typename P>
I find_if_not(I begin, I end, P p) noexcept
{
    while (begin != end)
    {
        if (!p(*begin))
        {
            return begin;
        }
        ++begin;
    }
    return end;
}

template<typename I, typename O, typename T>
O remove_copy(I begin, I end, O result, const T& value)
{
    while (begin != end)
    {
        if (*begin == value)
        {
            *result = *begin;
            ++result;
        }
        ++begin;
    }
    return result;
}

template<typename I, typename T>
I remove(I begin, I end, const T& value)
{
    begin = find(begin, end, value);
    if (begin == end)
    {
        return begin;
    }
    else
    {
        I i = begin;
        ++i;
        return remove_copy(i, end, begin, value);
    }
}

template<typename I, typename P>
I remove_if(I begin, I end, P p)
{
    begin = find_if(begin, end, p);
    if (begin != end)
    {
        for (I it = begin; it != end; ++it)
        {
            if (!p(*it))
            {
                *begin++ = std::move(*it);
            }
        }
    }
    return begin;
}

template<typename I, typename R>
void insertion_sort(I begin, I end, R r)
{
    I i = begin;
    while (i < end)
    {
        I j = i;
        while (j > begin && r(*j, *(j - 1)))
        {
            swap(*j, *(j - 1));
            --j;
        }
        ++i;
    }
}

template<typename I>
constexpr void insertion_sort(I begin, I end)
{
    insertion_sort(begin, end, less<iterator_traits<I>::value_type>());
}

template<typename I, typename P>
I partition(I begin, I end, P p)
{
    begin = find_if_not(begin, end, p);
    if (begin == end)
    {
        return begin;
    }
    for (auto i = next(begin); i != end; ++i)
    {
        if (p(*i))
        {
            swap(*i, *begin);
            ++begin;
        }
    }
    return begin;
}

template<typename T, typename R>
class operand_rel_x
{
public:
    operand_rel_x(const T& x_, R r_) : x(x_), r(r_) {}
    inline bool operator()(const T& a) const noexcept
    {
        return r(a, x);
    }
private:
    T x;
    R r;
};

template<typename T, typename R>
class not_x_rel_operand
{
public:
    not_x_rel_operand(const T& x_, R r_) : x(x_), r(r_) {}
    inline bool operator()(const T& a) const noexcept
    {
        return !r(x, a);
    }
private:
    T x;
    R r;
};

template<typename I, typename R>
void quick_sort(I begin, I end, R r)
{
    if (begin == end) return;
    auto pivot = *next(begin, distance(begin, end) / 2);
    auto mid1 = partition(begin, end, operand_rel_x<iterator_traits<I>::value_type, R>(pivot, r));
    auto mid2 = partition(mid1, end, not_x_rel_operand<iterator_traits<I>::value_type, R>(pivot, r));
    quick_sort(begin, mid1, r);
    quick_sort(mid2, end, r);
}

constexpr ssize_t insertion_sort_threshold = 16;

template<typename I, typename R>
void sort(I begin, I end, R r)
{
    if (begin == end) return;
    if (end - begin <= insertion_sort_threshold)
    {
        insertion_sort(begin, end, r);
    }
    else
    {
        quick_sort(begin, end, r);
    }
}

template<typename I>
inline void sort(I begin, I end)
{
    sort(begin, end, less<iterator_traits<I>::value_type>());
}

template<typename I, typename R>
I adjacent_find(I begin, I end, R r)
{
    if (begin != end)
    {
        I i = begin;
        ++i;
        while (i != end)
        {
            if (r(*begin, *i))
            {
                return begin;
            }
            begin = i;
            ++i;
        }
    }
    return end;
}

template<typename I>
I adjacent_find(I begin, I end)
{
    return adjacent_find(begin, end, equal_to<iterator_traits<I>::value_type>());
}

template<typename I, typename R>
I unique(I begin, I end, R r)
{
    begin = adjacent_find(begin, end, r);
    if (begin != end)
    {
        I i = begin;
        ++i;
        ++i;
        while (i != end)
        {
            if (!r(*begin, *i))
            {
                *++begin = std::move(*i);
            }
            ++i;
        }
        ++begin;
    }
    return begin;
}

template<typename I>
I unique(I begin, I end)
{
    return unique(begin, end, equal_to<iterator_traits<I>::value_type>());
}

template<typename I, typename T>
I lower_bound(I first, I last, const T& value) noexcept
{
    typename std::iterator_traits<I>::difference_type len = distance(first, last);
    while (len > 0)
    {
        typename std::iterator_traits<I>::difference_type half = len >> 1;
        I middle = next(first, half);
        if (value > *middle)
        {
            first = middle;
            ++first;
            len -= half + 1;
        }
        else
        {
            len = half;
        }
    }
    return first;
}

template<typename I, typename T, typename R>
I lower_bound(I first, I last, const T& value, R r) noexcept
{
    typename std::iterator_traits<I>::difference_type len = distance(first, last);
    while (len > 0)
    {
        typename std::iterator_traits<I>::difference_type half = len >> 1;
        I middle = next(first, half);
        if (r(*middle, value))
        {
            first = middle;
            ++first;
            len -= half + 1;
        }
        else
        {
            len = half;
        }
    }
    return first;
}

template<typename I, typename T>
I upper_bound(I first, I last, const T& value) noexcept
{
    typename std::iterator_traits<I>::difference_type len = distance(first, last);
    while (len > 0)
    {
        typename std::iterator_traits<I>::difference_type half = len >> 1;
        I middle = next(first, half);
        if (value < *middle)
        {
            len = half;
        }
        else
        {
            first = middle;
            ++first;
            len -= half + 1;
        }
    }
    return first;
}

template<typename I, typename T, typename R>
I upper_bound(I first, I last, const T& value, R r) noexcept
{
    typename std::iterator_traits<I>::difference_type len = distance(first, last);
    while (len > 0)
    {
        typename std::iterator_traits<I>::difference_type half = len >> 1;
        I middle = next(first, half);
        if (r(value, *middle))
        {
            len = half;
        }
        else
        {
            first = middle;
            ++first;
            len -= half + 1;
        }
    }
    return first;
}

template<typename I, typename T>
pair<I, I> equal_range(I first, I last, const T& value) noexcept
{
    typename std::iterator_traits<I>::difference_type len = distance(first, last);
    while (len > 0)
    {
        typename std::iterator_traits<I>::difference_type half = len >> 1;
        I middle = iter_next(first, half);
        if (*middle < value)
        {
            first = middle;
            ++first;
            len -= half + 1;
        }
        else if (value < *middle)
        {
            len = half;
        }
        else
        {
            I left = lower_bound(first, middle, value);
            I end = iter_next(first, len);
            ++middle;
            I right = upper_bound(middle, end, value);
            return pair<I, I>(left, right);
        }
    }
    return pair<I, I>(first, first);
}

template<typename I, typename T, typename R>
pair<I, I> equal_range(I first, I last, const T& value, R r) noexcept
{
    typename std::iterator_traits<I>::difference_type len = distance(first, last);
    while (len > 0)
    {
        typename std::iterator_traits<I>::difference_type half = len >> 1;
        I middle = next(first, half);
        if (r(*middle, value))
        {
            first = middle;
            ++first;
            len -= half + 1;
        }
        else if (r(value, *middle))
        {
            len = half;
        }
        else
        {
            I left = lower_bound(first, middle, value, r);
            I end = next(first, len);
            ++middle;
            I right = upper_bound(middle, end, value, r);
            return pair<I, I>(left, right);
        }
    }
    return pair<I, I>(first, first);
}

template<typename I1, typename I2>
bool equal(I1 first1, I1 last1, I2 first2) noexcept
{
    while (first1 != last1)
    {
        if (*first1 != *first2)
        {
            return false;
        }
        ++first1;
        ++first2;
    }
    return true;
}

template<typename I1, typename I2, typename R>
bool equal(I1 first1, I1 last1, I2 first2, R r) noexcept
{
    while (first1 != last1)
    {
        if (!r(*first1, *first2))
        {
            return false;
        }
        ++first1;
        ++first2;
    }
    return true;
}

template<typename I1, typename I2>
bool lexicographical_compare(I1 first1, I1 last1, I2 first2, I2 last2) noexcept
{
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 < *first2)
        {
            return true;
        }
        if (*first1 > *first2)
        {
            return false;
        }
        ++first1;
        ++first2;
    }
    return first1 == last1 && first2 != last2;
}

template<typename I1, typename I2, typename Compare>
bool lexicographical_compare(I1 first1, I1 last1, I2 first2, I2 last2, Compare comp) noexcept
{
    while (first1 != last1 && first2 != last2)
    {
        if (comp(*first1, *first2))
        {
            return true;
        }
        if (comp(*first2, *first1))
        {
            return false;
        }
        ++first1;
        ++first2;
    }
    return first1 == last1 && first2 != last2;
}

template<typename I>
bool next_permutation(I first, I last)
{
    if (first == last)
    {
        return false;
    }
    I i = first;
    ++i;
    if (i == last)
    {
        return false;
    }
    i = last;
    --i;
    while (true)
    {
        I ii = i;
        --i;
        if (*i < *ii)
        {
            I j = last;
            --j;
            while (*i >= *j)
            {
                --j;
            }
            swap(*i, *j);
            reverse(ii, last);
            return true;
        }
        if (i == first)
        {
            reverse(first, last);
            return false;
        }
    }
}

std::uint32_t random()
{
    return ort_random();
}

std::uint64_t random64()
{
    return ort_random64();
}

void reset_rng()
{
    ort_reset_rng();
}

void set_random_seed(std::uint64_t seed)
{
    ort_set_random_seed(seed);
}

} // std
