import std;

int main()
{
    std::string a = "abc";
    const char* x = a.c_str();
    std::cout << x << "\n";
}
