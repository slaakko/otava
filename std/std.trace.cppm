export module std.trace;

import std.type.fundamental;

export namespace std {

struct trace_entry
{
    trace_entry(std::int64_t fn_) noexcept : next(nullptr), fn(fn_), line(0) {}
    trace_entry* next;
    std::int64_t fn;
    std::int32_t line;
};

struct trace_guard
{
    trace_guard(trace_entry* entry_) noexcept;
    ~trace_guard();
    trace_entry* entry;
};

} // namespace std
