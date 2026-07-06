export module soul.lexer.keyword;

import std;
import soul.lexer.token;
import soul.lexer.lexeme;

export namespace soul::lexer {

template<typename Char>
constexpr const Char* StrEnd(const Char* s) noexcept
{
    while (*s)
    {
        ++s;
    }
    return s;
}

template<typename Char>
struct Keyword
{
    inline Keyword() noexcept : str(nullptr), tokenID(INVALID_TOKEN) {}
    inline Keyword(const Char* str_, std::int64_t tokenID_) noexcept : str(str_), tokenID(tokenID_) {}
    const Char* str;
    std::int64_t tokenID;
};

template<typename Char>
class KeywordMap
{
public:
    constexpr KeywordMap(const soul::lexer::Keyword<Char>* keywords_) noexcept : keywords(keywords_)
    {
        const soul::lexer::Keyword<Char>* kw = keywords;
        while (kw->str)
        {
            Lexeme<Char> lexeme(kw->str, StrEnd(kw->str));
            keywordMap[lexeme] = kw->tokenID;
            ++kw;
        }
    }
    std::int64_t GetKeywordToken(const Lexeme<Char>& lexeme) const noexcept
    {
        auto it = keywordMap.find(lexeme);
        if (it != keywordMap.cend())
        {
            return it->second;
        }
        else
        {
            return INVALID_TOKEN;
        }
    }
private:
    const soul::lexer::Keyword<Char>* keywords;
    std::map<Lexeme<Char>, std::int64_t, LexemeCompare<Char>> keywordMap;
};

} // namespace soul::lexer
