import std;

struct Declaration
{
    Declaration() : s("foo") {}
    std::string s;
};

class TypeSymbol
{
public:
    TypeSymbol() {}
};

Declaration ProcessDeclarator(TypeSymbol* baseType);

void foo()
{
    TypeSymbol* type = nullptr;
    Declaration declaration = ProcessDeclarator(type);
}
