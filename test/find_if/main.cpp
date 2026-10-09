import std;

class TypeSymbol
{
public:
    TypeSymbol() {}
};

class Context
{
public:
    Context() {}
};

bool TypesEqual(TypeSymbol* left, TypeSymbol* right, Context* context)
{
    return false;
}

class AliasGroupSymbol;

class AliasTypeSymbol
{
public:
    AliasTypeSymbol() : referredType(nullptr) {}
    TypeSymbol* ReferredType(Context* context)
    {
        return referredType;
    }
    void SetGroup(AliasGroupSymbol* group) {}
private:
    TypeSymbol* referredType;
};

class AliasGroupSymbol
{
public:
    AliasGroupSymbol() {}
    void AddAliasTypeSymbol(AliasTypeSymbol* aliasTypeSymbol, Context* context);
private:
    std::vector<AliasTypeSymbol*> aliasTypeSymbols;
};

struct ReferredTypeEqual
{
    ReferredTypeEqual(AliasTypeSymbol* aliasTypeSymbol_, Context* context_) : aliasTypeSymbol(aliasTypeSymbol_), context(context_) {}
    inline bool operator()(AliasTypeSymbol* symbol) const noexcept
    {
        return TypesEqual(symbol->ReferredType(context), aliasTypeSymbol->ReferredType(context), context);
    }
    AliasTypeSymbol* aliasTypeSymbol;
    Context* context;
};

void AliasGroupSymbol::AddAliasTypeSymbol(AliasTypeSymbol* aliasTypeSymbol, Context* context)
{
    ReferredTypeEqual referredTypeEqual(aliasTypeSymbol, context);
    if (std::find(aliasTypeSymbols.begin(), aliasTypeSymbols.end(), aliasTypeSymbol) == aliasTypeSymbols.end() &&
        std::find_if(aliasTypeSymbols.begin(), aliasTypeSymbols.end(), referredTypeEqual) == aliasTypeSymbols.end())
    {
        aliasTypeSymbol->SetGroup(this);
        aliasTypeSymbols.push_back(aliasTypeSymbol);
    }
}

void foo()
{
    AliasGroupSymbol group;
    AliasTypeSymbol* aliasType = nullptr;
    Context* context = nullptr;
    group.AddAliasTypeSymbol(aliasType, context);
}

int main()
{
    foo();
}
