import std;

namespace foo::fuz {

class bar
{
public:
    bar(int x_) : x(x_)
    {
    }
private:
    int x;
};

}

void f()
{
}

foo::fuz::bar* fn(int x)
{
    return new foo::bar(x);
}

int main()
{
    foo::bar* b = fn(1);
}
