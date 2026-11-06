export module std.profile;

import std.type.fundamental;
import std.rt;

export namespace std::profile {

class fn_profiler;

fn_profiler* get_profiler() noexcept;
void set_profiler(fn_profiler* p) noexcept;

class fn_profiler
{
public:
    fn_profiler(std::uint64_t fn_) noexcept;
    void add_ctime(std::uint64_t t) noexcept;
    ~fn_profiler();
private:
    fn_profiler* parent;
    std::uint64_t fn;
    std::uint64_t start;
    std::uint64_t ctime;
};

} // std::profile
