export module std.expected;

import std.type.fundamental;
import std.algorithm;
import std.utilities.utility;
import std.functional;

export namespace std {

template<typename E>
class unexpected
{
public:
    unexpected(const unexpected&) = default;
    unexpected(unexpected&&) = default;
    explicit unexpected(const E& e_) : e(e_) {}
    explicit unexpected(E&& e_) : e(move(e_)) {}
    const E& error() const noexcept { return e; }
    E& error() noexcept { return e; }
    void swap(unexpected<E>& other) noexcept
    {
        std::swap(e, other.e);
    }
private:
    E e;
};

template<typename E>
constexpr bool operator==(const unexpected<E>& lhs, const unexpected<E>& rhs) noexcept
{
    return lhs.error() == rhs.error();
}

template<typename E>
constexpr void swap(unexpected<E>& lhs, unexpected<E>& rhs) noexcept
{
    lhs.swap(rhs);
}

template<typename T, typename E>
class expected
{
public:
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;

    constexpr expected() : has_val(true), val(), unex() {}
    constexpr expected(const T& val_) : has_val(true), val(val_), unex() {}
    constexpr expected(T&& val_) noexcept : has_val(true), val(std::move(val_)), unex() {}
    constexpr expected(const unexpected<E>& e): has_val(false), unex(e.error()) {}
    constexpr expected(unexpected<E>&& e) : has_val(false), unex(e.error()) {}
    constexpr expected& operator=(const T& val_) { has_val = true; val = val_; return *this; }
    constexpr expected& operator=(T&& val_) noexcept { has_val = true; val = std::move(val_); return *this; }
    constexpr expected& operator=(const unexpected<E>& e) { has_val = false; unex = e.error(); return *this; }
    constexpr expected& operator=(unexpected<E>&& e) { has_val = false; unex = e.error(); return *this; }
    constexpr const T* operator->() const noexcept { return &val; }
    constexpr T* operator->() noexcept { return &val; }
    constexpr const T& operator*() const noexcept  { return val; }
    constexpr T& operator*() noexcept  { return val; }
    constexpr bool has_value() const noexcept { return has_val; }
    constexpr explicit operator bool() const noexcept { return has_value(); }
    constexpr T& value() noexcept { return val; }
    constexpr const T& value() const noexcept { return val; }
    constexpr const E& error() const noexcept { return unex; }
    constexpr E& error() noexcept { return unex; }
    constexpr void swap(expected& other) noexcept
    {
        std::swap(has_val, other.has_val);
        std::swap(val, other.val);
        std::swap(unex, other.unex);
    }
private:
    bool has_val;
    T val;
    E unex;
};

template<typename T, typename E>
constexpr bool operator==(const expected<T, E>& lhs, const expected<T, E>& rhs) noexcept
{
    return lhs.has_value() == rhs.has_value() && (lhs.has_value() && lhs.value() == rhs.value()) || (!lhs.has_value() && lhs.error() == rhs.error());
}

template<typename T, typename E>
constexpr void swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept
{
    lhs.swap(rhs);
}

} // namespace std

template class std::unexpected<int>;
