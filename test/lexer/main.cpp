import std;

class Token
{
public:
    Token() {}
};

class M
{
public:
    M() {}
};

template<typename Machine, typename Char>
class Lexer 
{
public:
    using MachineType = Machine;
    using CharType = Char;
    using Self = Lexer<MachineType, CharType>;
    using TokenType = Token;
    using PPHook = void (*)(Self* lxr, TokenType* token);
    void SetPPHook(PPHook ppHook_) noexcept
    {
        ppHook = ppHook_;
    }
private:
    PPHook ppHook;
};

void foo(Lexer<M, char>* lxr, Token* token)
{
}

int main()
{
    Lexer<M, char> lexer;
    lexer.SetPPHook(foo);
}
