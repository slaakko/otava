export module std.iterator;

import std.type.fundamental;
import std.trace;

export namespace std {

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
struct contiguous_iterator_tag : public random_access_iterator_tag { };

template<typename IterT>
struct iterator_traits
{
    using difference_type = typename IterT::difference_type;
    using value_type = typename IterT::value_type;
    using pointer = typename IterT::pointer;
    using reference = typename IterT::reference;
    using iterator_category = typename IterT::iterator_category;
};

template<typename T>
struct iterator_traits<T*>
{
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = random_access_iterator_tag;
};

template<typename T>
struct iterator_traits<const T*>
{
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = random_access_iterator_tag;
};

namespace detail {

template<typename IterT>
void advance_iter(IterT& it, typename std::iterator_traits<IterT>::difference_type n, std::input_iterator_tag) noexcept
{
    while (n > 0)
    {
        --n;
        ++it;
    }
}

template<typename IterT>
void advance_iter(IterT& it, typename std::iterator_traits<IterT>::difference_type n, std::bidirectional_iterator_tag) noexcept
{
    while (n > 0)
    {
        --n;
        ++it;
    }
    while (n < 0)
    {
        ++n;
        --it;
    }
}

template<typename IterT>
constexpr void advance_iter(IterT& it, typename std::iterator_traits<IterT>::difference_type n, std::random_access_iterator_tag) noexcept
{
    it += n;
}

} // namespace detail

template<typename IterT, typename Distance>
constexpr void advance(IterT& it, Distance n) noexcept
{
    detail::advance_iter(it, typename std::iterator_traits<IterT>::difference_type(n), typename std::iterator_traits<IterT>::iterator_category());
}

namespace detail {

template<typename IterT>
typename std::iterator_traits<IterT>::difference_type distance_iter(IterT first, IterT last, std::input_iterator_tag) noexcept
{
    typename std::iterator_traits<IterT>::difference_type result = 0;
    while (first != last)
    {
        ++first;
        ++result;
    }
    return result;
}

template<typename IterT>
constexpr typename std::iterator_traits<IterT>::difference_type distance_iter(IterT first, IterT last, std::random_access_iterator_tag) noexcept
{
    return last - first;
}

} // namespace detail

template<typename IterT>
constexpr typename std::iterator_traits<IterT>::difference_type distance(IterT first, IterT last) noexcept
{
    return detail::distance_iter(first, last, typename std::iterator_traits<IterT>::iterator_category());
}

template<typename IterT>
constexpr IterT next(IterT it, typename std::iterator_traits<IterT>::difference_type n = 1) noexcept
{
    std::advance(it, n);
    return it;
}

template<typename IterT>
constexpr IterT prev(IterT it, typename std::iterator_traits<IterT>::difference_type n = 1) noexcept
{
    std::advance(it, -n);
    return it;
}

} // namespace std
