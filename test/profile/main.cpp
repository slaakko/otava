import std;

void foo()
{
    std::profile::fn_profiler profiler(123);
}

int main()
{
    foo();
}

