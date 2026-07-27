import std;

class foo
{
public:
    foo() {}
    virtual ~foo() {}
};

class bar : public foo
{
public:
    bar() : foo() {}
};

int main()
{
    foo* f = new bar();
    if (bar* b = dynamic_cast<bar*>(f))
    {
        std::cout << "yep" << "\n";
    }
    else
    {
        std::cout << "nope" << "\n";
    }
}
