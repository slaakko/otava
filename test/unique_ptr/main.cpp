import std;

std::unique_ptr<int> foo()
{
    return std::unique_ptr<int>();
}

int main()
{
    std::unique_ptr<int> f = foo();
}
