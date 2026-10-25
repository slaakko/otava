import std;

void foo()
{
    throw std::runtime_error("foo");
}

int main()
{
    try
    {
        foo();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    return 0;
}
