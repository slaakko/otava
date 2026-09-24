module std.rt;

import std.trace;

namespace std {

rt_init::rt_init() noexcept
{
    stdin = static_cast<void*>(&stdin_);
    stdout = static_cast<void*>(&stdout_);
    stderr = static_cast<void*>(&stderr_);
}

rt_init init_rt;

} // namespace std
