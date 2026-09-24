import std;

export namespace soul::lexer {

template<class Char>
class LexerBase
{
public:
    LexerBase() {}
};

template<typename Char, typename LexerBaseT>
struct Token
{
    Token() {}
};

template<typename Machine, typename Char>
class Lexer : public LexerBase<Char>
{
public:
    using MachineType = Machine;
    using CharType = Char;
    using Self = Lexer<MachineType, CharType>;
    using TokenType = soul::lexer::Token<CharType, LexerBase<CharType>>;
    using PPHook = void (*)(Self* lxr, TokenType* token);
    Lexer() : ppHook(nullptr) {}
    void SetPPHook(PPHook ppHook_) noexcept
    {
        ppHook = ppHook_;
    }
private:
    PPHook ppHook;
};

} // soul::lexer

class M
{
public:
    M() {}
};

void PreprocessPPLine(soul::lexer::Lexer<M, char>* lexer, soul::lexer::Token<char, soul::lexer::LexerBase<char>>* token)
{
}

int main() 
{
    soul::lexer::Lexer<M, char> lxr;
    lxr.SetPPHook(PreprocessPPLine);
}
