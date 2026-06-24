module std.trace;

import std.rt;

namespace std {

trace_guard::trace_guard(trace_entry* entry_) : entry(entry_)
{
    ort_push_function(static_cast<void*>(entry));
}

trace_guard::~trace_guard()
{
    ort_pop_function(static_cast<void*>(entry));
}

} // namespace std
