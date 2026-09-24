export module std.utilities.unique_ptr;

import std.type.fundamental;
import std.basic_string;

export namespace std {

template<class T>
class unique_ptr
{
public:
    using pointer = T*;
    using element_type = T;

    inline unique_ptr() noexcept : ptr(nullptr) {}
    inline explicit unique_ptr(pointer p) noexcept : ptr(p) {}
    inline unique_ptr(unique_ptr&& u) noexcept : ptr(u.ptr) { u.ptr = nullptr; }
    inline unique_ptr(nullptr_t) noexcept : ptr(nullptr) {}
    ~unique_ptr() { if (ptr) delete ptr; }
    inline unique_ptr& operator=(unique_ptr&& u) noexcept { std::swap(ptr, u.ptr); return *this; }
    inline unique_ptr& operator=(nullptr_t) noexcept { if (ptr) delete ptr; ptr = nullptr; return *this; }
    inline T& operator*() const noexcept { return *ptr; }
    inline pointer operator->() const noexcept { return ptr; }
    inline pointer get() const noexcept { return ptr; }
    inline explicit operator bool() const noexcept { return ptr != nullptr; }
    inline pointer release() noexcept { pointer p = ptr; ptr = nullptr; return p; }
    void reset(pointer p) noexcept { if (ptr) delete ptr; ptr = p; }
    inline void reset() noexcept { if (ptr) delete ptr; ptr = nullptr; }
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
private:
    pointer ptr;
};

} // namespace std
