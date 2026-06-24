export module std.init;

import std.thread.mutex;

export namespace std {

std::recursive_mutex& get_init_lock();

} // namespace std
