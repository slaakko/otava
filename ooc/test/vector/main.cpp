import std;

void foo(int n)
{
    std::vector<std::unique_ptr<int>> v;
    for (int i = 0; i < n; ++i)
    {
        v.push_back(std::unique_ptr<int>(new int(i)));
    }
}

int main()
{
    for (int i = 0; i < 512; ++i)
    {
        int n = 8 * i;
        foo(n);
    }
}
