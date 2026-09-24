export module std.thread.mutex;

import std.type.fundamental;

export namespace std {

class mutex
{
public:
    mutex();
    ~mutex();
    void lock();
    bool try_lock();
    void unlock();
private:
    void* handle;
};

class recursive_mutex
{
public:
    recursive_mutex();
    ~recursive_mutex();
    void lock();
    bool try_lock();
    void unlock();
private:
    void* handle;
};

template<class Mutex>
class lock_guard
{
public:
    explicit lock_guard(Mutex& mtx_) : mtx(mtx_) { mtx.lock(); }
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;
    ~lock_guard() { mtx.unlock(); }
private:
    Mutex& mtx;
};

template<class Mutex>
class unique_lock
{
public:
    explicit unique_lock(Mutex& mtx_) : mtx(mtx_) { mtx.lock(); }
    unique_lock(const unique_lock&) = delete;
    unique_lock& operator=(const unique_lock&) = delete;
    ~unique_lock() { mtx.unlock(); }
private:
    Mutex& mtx;
};

} // namespace std
