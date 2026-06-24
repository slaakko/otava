export module std.atexit;

import std.type.fundamental;

export namespace std {

void at_exit(void* fn, void* arg);
void run_at_exits();

} // namespace std
