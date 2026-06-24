export module std.utilities.shared_ptr;

import std.type.fundamental;
import std.trace;

export namespace std {

namespace detail {

class counter_base
{
public:
    counter_base();
    counter_base(const counter_base&) = delete;
    counter_base& operator=(const counter_base&) = delete;
    counter_base(counter_base&&) = delete;
    counter_base& operator=(counter_base&&) = delete;
    virtual ~counter_base();
    virtual void dispose() noexcept = 0;
    virtual void destruct() noexcept;
    inline void add_reference() noexcept { ++use_cnt; ++weak_cnt; }
    void release();
    inline void weak_add_reference() noexcept { ++weak_cnt; }
    void weak_release() noexcept;
    inline int use_count() const noexcept { return use_cnt; }
private:
    int use_cnt;
    int weak_cnt;
};

template<class T>
class counter : public counter_base
{
public:
    inline counter(T* ptr_) noexcept : ptr(ptr_) {}
    counter(const counter<T>&) = delete;
    counter(counter<T>&&) = delete;
    counter<T>& operator=(const counter<T>&) = delete;
    counter<T>& operator=(counter<T>&&) = delete;
    void dispose() noexcept override
    {
        delete ptr;
        ptr = nullptr;
    }
private:
    T* ptr;
};

template<class T>
class shared_count
{
public:
    inline shared_count() noexcept : cptr(nullptr) {}
    shared_count(T* ptr_) : cptr(new counter<T>(ptr_)) {}
    inline shared_count(counter<T>* cptr_) noexcept : cptr(cptr_) { if (cptr) cptr->add_reference(); }
    inline shared_count(const shared_count<T>& that) noexcept : cptr(that.cptr) { if (cptr) cptr->add_reference(); }
    inline shared_count(shared_count<T>&& that) noexcept : cptr(std::move(that.cptr)) { that.cptr = nullptr; }
    shared_count<T>& operator=(const shared_count<T>& that)
    {
        counter<T>* other = that.cptr;
        if (other)
        {
            other->add_reference();
        }
        if (cptr)
        {
            cptr->release();
        }
        cptr = other;
        return *this;
    }
    inline shared_count<T>& operator=(shared_count<T>&& that) noexcept
    {
        std::swap(cptr, that.cptr);
        return *this;
    }
    ~shared_count()
    {
        if (cptr)
        {
            cptr->release();
        }
    }
    inline int use_count() const noexcept
    {
        if (cptr)
        {
            return cptr->use_count();
        }
        else
        {
            return 0;
        }
    }
    inline counter<T>* get_counter() const noexcept { return cptr; }
private:
    counter<T>* cptr;
};

template<class T>
class weak_count
{
public:
    inline weak_count() noexcept : cptr(nullptr) {}
    inline weak_count(const weak_count<T>& that) noexcept : cptr(that.cptr) { if (cptr) cptr->weak_add_reference(); }
    inline weak_count(weak_count<T>&& that) noexcept : cptr(std::move(that.cptr)) { that.cptr = nullptr; }
    inline weak_count(shared_count<T>& that) noexcept : cptr(that.cptr) { if (cptr) cptr->weak_add_reference(); }
    ~weak_count()
    {
        if (cptr)
        {
            cptr->weak_release();
        }
    }
    weak_count& operator=(weak_count<T>& that)
    {
        counter<T>* other = that.cptr;
        if (other)
        {
            other->weak_add_reference();
        }
        if (cptr)
        {
            cptr->weak_release();
        }
        cptr = other;
        return *this;
    }
    inline weak_count& operator=(weak_count<T>&& that) noexcept
    {
        std::swap(cptr, that.cptr);
        return *this;
    }
    weak_count& operator=(shared_count<T>& that)
    {
        counter<T>* other = that.cptr;
        if (other)
        {
            other->weak_add_reference();
        }
        if (cptr)
        {
            cptr->weak_release();
        }
        cptr = other;
        return *this;
    }
    inline int use_count() const noexcept
    {
        if (cptr)
        {
            return cptr->use_count();
        }
        else
        {
            return 0;
        }
    }
    inline counter<T>* get_counter() const noexcept { return cptr; }
private:
    counter<T>* cptr;
};

} // namespace detail

template<class T>
class shared_ptr
{
public:
    inline shared_ptr() noexcept : ptr(nullptr), count() {}
    inline shared_ptr(std::nullptr_t) noexcept : shared_ptr() {}
    explicit shared_ptr(T* ptr_) noexcept : ptr(ptr_), count(ptr) {}
    shared_ptr(const shared_ptr<T>& that) noexcept : ptr(that.ptr), count(that.count) {}
    inline shared_ptr(const shared_ptr<T>&& that) noexcept : ptr(std::move(that.ptr)), count(std::move(that.count)) { that.ptr = nullptr; }
    ~shared_ptr() {}
    shared_ptr& operator=(const shared_ptr<T>& that) noexcept { ptr = that.ptr; count = that.count; return *this; }
    shared_ptr& operator=(shared_ptr<T>&& that) noexcept { std::swap(ptr, that.ptr); std::swap(count, that.count); return *this; }
    void swap(shared_ptr<T>& that) noexcept { std::swap(ptr, that.ptr); std::swap(count, that.count); }
    void reset() noexcept { shared_ptr<T>().swap(*this); }
    void reset(T* ptr_) noexcept { shared_ptr<T>(ptr_).swap(*this); }
    inline T* get() const noexcept { return ptr; }
    inline T& operator*() const noexcept { return *ptr; }
    inline T* operator->() const noexcept { return ptr; }
    inline int use_count() const noexcept { return count.use_count(); }
    inline explicit operator bool() const noexcept { return ptr != nullptr; }
private:
    T* ptr;
    detail::shared_count<T> count;
};

template<class T>
inline bool operator==(const shared_ptr<T>& left, const shared_ptr<T>& right) noexcept
{
    return left.get() == right.get();
}

template<class T>
inline bool operator<(const shared_ptr<T>& left, const shared_ptr<T>& right) noexcept
{
    return left.get() < right.get();
}

template<class T>
class weak_ptr
{
public:
    inline weak_ptr() noexcept : ptr(nullptr), count() {}
    inline weak_ptr(const shared_ptr<T>& that) noexcept : ptr(that.ptr), count(that.count) {}
    weak_ptr(const weak_ptr<T>& that) : ptr(nullptr), count(that.count) 
    { 
        ptr = that.lock().get();
    }
    ~weak_ptr() {}
    weak_ptr<T>& operator=(const weak_ptr<T>& that) noexcept
    {
        weak_ptr<T>(that).swap(*this);
        return *this;
    }
    weak_ptr<T>& operator=(weak_ptr<T>&& that) noexcept
    {
        weak_ptr<T>(std::move(that)).swap(*this);
        return *this;
    }
    shared_ptr<T> lock() const noexcept
    {
        if (expired())
        {
            return shared_ptr<T>();
        }
        else
        {
            return shared_ptr<T>(*this);
        }
    }
    inline bool expired() const noexcept
    {
        return count.use_count() == 0;
    }
    void reset() noexcept
    {
        weak_ptr<T>().swap(*this);
    }
    void swap(weak_ptr<T>& that)
    {
        std::swap(ptr, that.ptr);
        std::swap(count, that.count);
    }
private:
    T* ptr;
    detail::weak_count<T> count;
};

} // namespace std
