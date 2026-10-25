import std;

void print(const std::vector<int>& v)
{
    bool first = true;
    for (const auto& x : v)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            std::cout << ", ";
        }
        std::cout << x;
    }
    std::cout << "\n";
}

void erase(int n)
{
    std::vector<int> v;
    for (int i = 0; i < n; ++i)
    {
        v.push_back(i);
    }
    for (int i = 0; i < n; ++i)
    {
        std::vector<int> w = v;
        print(w);
        w.erase(w.begin() + i);
        print(w);
    }
}

int main()
{
    for (int n = 0; n < 8; ++n)
    {
        erase(n);
    }
}