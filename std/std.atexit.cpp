module std.atexit;

import std.new_delete_op;
import std.trace;
import std.eh;

namespace std {

using at_exit_fn = void (*)(void* arg);

struct at_exit_entry
{
    at_exit_entry(void* fn_, void* arg_, at_exit_entry* next_);
    void* fn;
    void* arg;
    at_exit_entry* next;
};

at_exit_entry* at_exit_list = nullptr;

at_exit_entry::at_exit_entry(void* fn_, void* arg_, at_exit_entry* next_) : fn(fn_), arg(arg_), next(next_)
{
}

void at_exit(void* fn, void* arg)
{
    at_exit_list = new at_exit_entry(fn, arg, at_exit_list);
}

void run_at_exits()
{
    at_exit_entry* entry = at_exit_list;
    while (entry)
    {
        at_exit_list = entry->next;
        void* fn = entry->fn;
        at_exit_fn invoke_fn = static_cast<at_exit_fn>(fn);
        void* arg = entry->arg;
        invoke_fn(arg);
        delete entry;
        entry = at_exit_list;
    }
}

} // namespace std
