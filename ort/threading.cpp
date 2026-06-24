// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module ort.threading;

void* ort_create_mutex()
{
    return new std::mutex();
}

void ort_destroy_mutex(void* mtx)
{
    std::mutex* m = static_cast<std::mutex*>(mtx);
    delete m;
}

void ort_lock_mutex(void* mtx)
{
    std::mutex* m = static_cast<std::mutex*>(mtx);
    return m->lock();
}

void ort_unlock_mutex(void* mtx)
{
    std::mutex* m = static_cast<std::mutex*>(mtx);
    return m->unlock();
}

bool ort_trylock_mutex(void* mtx)
{
    std::mutex* m = static_cast<std::mutex*>(mtx);
    return m->try_lock();
}

void* ort_create_recursive_mutex()
{
    return new std::recursive_mutex();
}

void ort_destroy_recursive_mutex(void* mtx)
{
    std::recursive_mutex* m = static_cast<std::recursive_mutex*>(mtx);
    delete m;
}

void ort_lock_recursive_mutex(void* mtx)
{
    std::recursive_mutex* m = static_cast<std::recursive_mutex*>(mtx);
    m->lock();
}

void ort_unlock_recursive_mutex(void* mtx)
{
    std::recursive_mutex* m = static_cast<std::recursive_mutex*>(mtx);
    m->unlock();
}

bool ort_trylock_recursive_mutex(void* mtx)
{
    std::recursive_mutex* m = static_cast<std::recursive_mutex*>(mtx);
    return m->try_lock();
}

void* ort_create_atomic_bool()
{
    return new std::atomic_bool();
}

void ort_destroy_atomic_bool(void* handle)
{
    std::atomic_bool* a = static_cast<std::atomic_bool*>(handle);
    delete a;
}

bool ort_load_atomic_bool(void* handle)
{
    std::atomic_bool* a = static_cast<std::atomic_bool*>(handle);
    return a->load();
}

void ort_store_atomic_bool(void* handle, bool value)
{
    std::atomic_bool* a = static_cast<std::atomic_bool*>(handle);
    a->store(value);
}
