import std;

void test()
{
    int n = std::random() % 4096;
    void* p = std::malloc(n);
    std::uint8_t* m = static_cast<std::uint8_t*>(p);
    for (int i = 0; i < n; ++i)
    {
        std::uint8_t* q = m + i;
        *q = std::uint8_t(i % 256);
    }
    for (int i = 0; i < n; ++i)
    {
        std::uint8_t* q = m + i;
        std::uint8_t x = *q;
        if (x != std::uint8_t(i % 256))
        {
            throw std::runtime_error("invalid byte");
        }
    }
    std::free(p);
}

int main()
{
    try
    {
        int n = 1000;
        for (int i = 0; i < n; ++i)
        {
            test();
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    return 0;
}
