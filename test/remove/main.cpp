import std;

void print(const std::vector<int>& x)
{
    bool first = true;
    for (int y : x)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            std::cout << ", ";
        }
        std::cout << y;
    }
    std::cout << "\n";
}

int main()
{
    std::vector<int> x;
    x.push_back(1);
    x.push_back(2);
    print(x);
    x.erase(std::remove(x.begin(), x.end(), 1), x.end());
    print(x);
}
