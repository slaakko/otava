import std;

class Foo
{
public:
    Foo() : s("foo") {}
    void Print()
    {
        std::cout << s << "\n";
    }
private:
    std::string s;
};

std::unique_ptr<Foo> bar()
{
    return new Foo();
}

std::unique_ptr<Foo> foo()
{
    int x = 0;
    switch (x)
    {
    case 0:
    {
        return bar();
    }
    }
    //return bar();
}

int main()
{
    std::unique_ptr<Foo> f = foo();
    f->Print();
}
