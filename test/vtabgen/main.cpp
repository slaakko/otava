import std;

class BoundCompileUnitNode
{
public:
    BoundCompileUnitNode() {}
};

class BoundTreeVisitor
{
public:
    BoundTreeVisitor() {}
    virtual ~BoundTreeVisitor() {}
    virtual void Visit(BoundCompileUnitNode& node) {}
};

class DefaultBoundTreeVisitor : public BoundTreeVisitor
{
public:
    DefaultBoundTreeVisitor() {}
    void Visit(BoundCompileUnitNode& node) override
    {
        std::cout << "default visit" << "\n";
    }
};

class ClassTypeSymbol;

class VTabGenerator
{
public:
    VTabGenerator() {}
    virtual ~VTabGenerator() {}
    virtual void GenerateVTab(ClassTypeSymbol* cls) = 0;
};

class Emitter
{
public:
    Emitter() : vtabGen(nullptr) {}
    inline void SetVTabGenerator(VTabGenerator* vtabGenerator) noexcept { vtabGen = vtabGenerator; }
    inline VTabGenerator* GetVTabGenerator() const noexcept { return vtabGen; }
private:
    VTabGenerator* vtabGen;
};

class ClassTypeSymbol
{
public:
    ClassTypeSymbol() {}
    void foo(Emitter& emitter)
    {
        VTabGenerator* vtabGen = emitter.GetVTabGenerator();
        vtabGen->GenerateVTab(this);
    }
};

class CodeGenerator : public DefaultBoundTreeVisitor, public VTabGenerator
{
public:
    CodeGenerator(Emitter* emitter_) : emitter(emitter_)
    {
        emitter->SetVTabGenerator(this);
    }
    void Visit(BoundCompileUnitNode& node) override
    {
        std::cout << "codegen visit" << "\n";
    }
    void GenerateVTab(ClassTypeSymbol* cls) override
    {
        std::cout << "codegen generate vtab" << "\n";
    }
private:
    Emitter* emitter;
};

int main()
{
    Emitter emitter;
    CodeGenerator codegen(&emitter);
    ClassTypeSymbol cls;
    cls.foo(emitter);
}
