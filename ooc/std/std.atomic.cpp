module std.atomic;

import std.rt;
import std.atexit;
import std.trace;

namespace std {

atomic_bool::atomic_bool() : handle(ort_create_atomic_bool())
{
}

atomic_bool::~atomic_bool()
{
    ort_destroy_atomic_bool(handle);
}

atomic_bool::operator bool()
{
    return ort_load_atomic_bool(handle);
}

atomic_bool& atomic_bool::operator=(bool value)
{
    ort_store_atomic_bool(handle, value);
    return *this;
}

} // namespace std
