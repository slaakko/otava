export module std.type_traits;

import std.type.fundamental;

export namespace std {

template<typename T>
struct remove_reference
{
    using type = T;
};

template<typename T>
struct remove_reference<T&&>
{
    using type = T;
};

template<typename T>
struct remove_reference<T&>
{
    using type = T;
};

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

template<typename T>
struct iter_value_type
{
    using type = typename T::value_type;
};

template<typename T>
struct iter_value_type<T*>
{
    using type = T;
};

template<typename T>
using iter_value_t = typename iter_value_type<T>::type;

} // namespace std
