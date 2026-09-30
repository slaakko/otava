import std;

std::vector<int> Container()
{
    std::vector<int> c;
    c.push_back(1);
    c.push_back(2);
    c.push_back(3);
    return c;
}

void foo()
{
    for (const int x : Container())
    {
        std::cout << x << "\n";
    }
}

int main()
{
    foo();
}
