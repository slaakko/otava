module std.new_delete_op;

import std.trace;
import std.exception;
import std.rt;

void* operator new(size_t size)
{
    void* ptr = std::malloc(size);
    if (!ptr)
    {
        throw std::bad_alloc();
    }
    return ptr;
}

void* operator new(size_t size, void* placement)
{
    return placement;
}

void operator delete(void* object)
{
    std::free(object);
}
