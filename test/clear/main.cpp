import std;

int main()
{
    std::map<std::int32_t, std::set<std::int32_t>> m;
    m[0].insert(1);
}
