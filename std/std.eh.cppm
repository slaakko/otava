export module std.eh;

import std.type.fundamental;

export namespace std {

enum class child_control_result : int
{
    none = 0, ret = 1, brk = 2, cnt = 3
};

enum class eh_return_from : int
{
    try_block = 0, handler_block = 1
};

} // namespace std
