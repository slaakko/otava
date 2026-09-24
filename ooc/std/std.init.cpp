module std.init;

import std.thread.mutex;
import std.atexit;
import std.trace;

namespace std {

std::recursive_mutex init_lock;

std::recursive_mutex& get_init_lock()
{
    return init_lock;
}

} // namespace std
