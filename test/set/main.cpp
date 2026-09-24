import std;
import ctx;

void foo()
{
    std::set<std::string> s;
    if (s.find("foo") == s.end())
    {
        s.insert("foo");
    }
}

int main()
{
    foo();
}
