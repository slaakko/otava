import std;

class Foo
{
public:
    Foo() {}
};

void foo()
{
    std::vector<std::unique_ptr<Foo>> args;
    args.push_back(std::unique_ptr<Foo>(new Foo()));
}

int main()
{
    foo();
}
