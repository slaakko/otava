// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module ort.threading;

import std;

export extern "C" void* ort_create_mutex();
export extern "C" void ort_destroy_mutex(void* mtx);
export extern "C" void ort_lock_mutex(void* mtx);
export extern "C" void ort_unlock_mutex(void* mtx);
export extern "C" bool ort_trylock_mutex(void* mtx);
export extern "C" void* ort_create_recursive_mutex();
export extern "C" void ort_destroy_recursive_mutex(void* mtx);
export extern "C" void ort_lock_recursive_mutex(void* mtx);
export extern "C" void ort_unlock_recursive_mutex(void* mtx);
export extern "C" bool ort_trylock_recursive_mutex(void* mtx);
export extern "C" void* ort_create_atomic_bool();
export extern "C" void ort_destroy_atomic_bool(void* handle);
export extern "C" bool ort_load_atomic_bool(void* handle);
export extern "C" void ort_store_atomic_bool(void* handle, bool value);
