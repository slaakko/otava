module std.thread.mutex;

import std.rt;
import std.trace;

namespace std {

mutex::mutex() : handle(ort_create_mutex())
{
}

mutex::~mutex()
{
    ort_destroy_mutex(handle);
}

void mutex::lock()
{
    ort_lock_mutex(handle);
}

bool mutex::try_lock()
{
    return ort_trylock_mutex(handle);
}

void mutex::unlock()
{
    ort_unlock_mutex(handle);
}

recursive_mutex::recursive_mutex() : handle(ort_create_recursive_mutex())
{
}

recursive_mutex::~recursive_mutex()
{
    ort_destroy_recursive_mutex(handle);
}

void recursive_mutex::lock()
{
    ort_lock_recursive_mutex(handle);
}

bool recursive_mutex::try_lock()
{
    return ort_trylock_recursive_mutex(handle);
}

void recursive_mutex::unlock()
{
    ort_unlock_recursive_mutex(handle);
}

} // namespace std
