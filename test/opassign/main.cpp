import std;

class Foo
{
public:
    Foo() : x(1) {}
    Foo& operator=(const Foo& that)
    {
        x = that.x + 1;
        return *this;
    }
    void Print()
    {
        std::cout << x << "\n";
    }
private:
    int x;
    std::unique_ptr<int> y;
};

void foo(Foo& f)
{
    Foo another;
    another = f;
    another.Print();
}

int main()
{
    Foo f;
    foo(f);
}