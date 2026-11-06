module std.profile;

namespace std::profile {

fn_profiler* profiler = nullptr;

fn_profiler* get_profiler() noexcept
{
    return profiler;
}

void set_profiler(fn_profiler* p) noexcept
{
    profiler = p;
}

fn_profiler::fn_profiler(std::uint64_t fn_) noexcept : parent(get_profiler()), fn(fn_), start(ort_now()), ctime(0) 
{ 
    set_profiler(this); 
}

void fn_profiler::add_ctime(std::uint64_t t) noexcept
{
    ctime += t;
}

fn_profiler::~fn_profiler()
{
    if (parent != this)
    {
        std::uint64_t elapsed = ort_now() - start;
        if (parent)
        {
            parent->add_ctime(elapsed);
        }
        ort_add_to_profile(fn, elapsed, ctime);
        set_profiler(parent);
    }
}

} // std::profile
