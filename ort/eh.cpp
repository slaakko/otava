// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <csetjmp>

module ort.eh;

import class_info_index;
import ort.trace;

const int numContexts = 16384;

std::jmp_buf jbufs[numContexts];

void* exception = nullptr;
std::uint32_t ext = 0;
void* bad_alloc = nullptr;
std::uint32_t bad_alloc_ext = 0;
int currentContext = 0;
int stackTrace = -1;

void ort_throw(void* ex, std::uint32_t ext_)
{
    if (currentContext > 0)
    {
        exception = ex;
        ext = ext_;
        if (ext != bad_alloc_ext)
        {
            //stackTrace = ort_generate_stack_trace();
        }
        std::longjmp(jbufs[currentContext - 1], 1);
    }
    else
    {
        std::cerr << "throw failed: no exception handler found" << "\n";
        std::exit(1);
    }
}

void ort_rethrow()
{
    ort_resume();
}

void ort_try(TryFn tryFn, HandlerFn handlerFn, void* parentFrame)
{
    if (currentContext >= 0 && currentContext < numContexts)
    {
        if (std::setjmp(jbufs[currentContext++]) == 0)
        {
            tryFn(parentFrame);
            --currentContext;
        }
        else
        {
            --currentContext;
            handlerFn(parentFrame);
        }
    }
    else if (currentContext >= numContexts)
    {
        std::cerr << "try failed: maximum number of contexts (" << numContexts << ") exceeded" << "\n";
        std::exit(1);
    }
    else
    {
        std::cerr << "try failed: invalid context value" << "\n";
        std::exit(1);
    }
}

void ort_invoke(InvokeFn invokeFn, CleanUpFn cleanupFn, void* parentFrame)
{
    if (currentContext >= 0 && currentContext < numContexts)
    {
        if (std::setjmp(jbufs[currentContext++]) == 0)
        {
            invokeFn(parentFrame);
            --currentContext;
        }
        else
        {
            --currentContext;
            cleanupFn(parentFrame);
            ort_resume();
        }
    }
    else if (currentContext >= numContexts)
    {
        std::cerr << "invoke failed: maximum number of contexts (" << numContexts << ") exceeded" << "\n";
        std::exit(1);
    }
    else
    {
        std::cerr << "invoke failed: invalid context value" << "\n";
        std::exit(1);
    }
}

bool ort_begin_catch(std::uint32_t ext_)
{
    if (ext == ext_)
    {
        return true;
    }
    if (ort_is_same_or_has_base(ext, ext_))
    {
        return true;
    }
    return false;
}

void ort_end_catch()
{
    exception = nullptr;
    ext = 0;
    ort_free_stack_trace(stackTrace);
}

void ort_resume()
{
    if (exception)
    {
        if (currentContext > 0)
        {
            std::longjmp(jbufs[currentContext - 1], 1);
        }
        else
        {
            std::cerr << "resume failed: no exception handler found" << "\n";
            std::exit(1);
        }
    }
    else
    {
        std::cerr << "resume failed: not handling exception" << "\n";
        std::exit(1);
    }
}

void* ort_get_exception()
{
    return exception;
}

void ort_print_exception_stack_trace()
{
    if (stackTrace != -1)
    {
        const char* traceStr = ort_get_stack_trace(stackTrace);
        if (traceStr)
        {
            std::cout << traceStr;
        }
    }
}

void ort_set_bad_alloc(void* ex, std::uint32_t ext)
{
    bad_alloc = ex;
    bad_alloc_ext = ext;
}

bool ort_is_bad_alloc(std::uint32_t ext)
{
    return ext == bad_alloc_ext;
}

bool ort_current_ex_is_bad_alloc()
{
    return ext == bad_alloc_ext;
}

void* ort_get_bad_alloc()
{
    return bad_alloc;
}
