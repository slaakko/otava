import std;

class msg
{
public:
    msg() {}
    virtual ~msg() {}
    virtual void print()
    {
        std::cout << "msg" << "\n";
    }
};

class foo : public msg
{
public:
    foo() : msg() {}
    void print() override
    {
        std::cout << "foo" << "\n";
    }
};

int main()
{
    msg* m = new foo();
    m->print();
}
