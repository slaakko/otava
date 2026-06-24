// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module ort.eh;

import std;

using TryFn = void(*)(void*);
using HandlerFn = void(*)(void*);
using InvokeFn = void(*)(void*);
using CleanUpFn = void(*)(void*);

export extern "C" void ort_throw(void* ex, std::uint32_t ext);
export extern "C" void ort_rethrow();
export extern "C" void ort_try(TryFn tryFn, HandlerFn handlerFn, void* parentFrame);
export extern "C" void ort_invoke(InvokeFn invokeFn, CleanUpFn cleanupFn, void* parentFrame);
export extern "C" bool ort_begin_catch(std::uint32_t ext);
export extern "C" void ort_end_catch();
export extern "C" void ort_resume();
export extern "C" void* ort_get_exception();
export extern "C" void ort_print_exception_stack_trace();
export extern "C" void ort_set_bad_alloc(void* ex, std::uint32_t ext);
export extern "C" bool ort_is_bad_alloc(std::uint32_t ext);
export extern "C" bool ort_current_ex_is_bad_alloc();
export extern "C" void* ort_get_bad_alloc();
