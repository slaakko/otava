// this file has been automatically generated from 'D:/work/otava/soul.rex/rex.lexer' using soul lexer generator slg version 5.2.0

module soul.rex.lexer;


namespace soul::rex::lexer {

soul::ast::common::TokenCollection* GetTokens(soul::rex::lexer::Tag tag)
{
    static soul::ast::common::TokenCollection tokens("soul.rex.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::ALT, "ALT", "'|'"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::STAR, "STAR", "'*'"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::QUEST, "QUEST", "'?'"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::ESCAPE, "ESCAPE", "escape"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::INVERSE, "INVERSE", "'^'"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::MINUS, "MINUS", "'-'"));
        tokens.AddToken(new soul::ast::common::Token(soul::rex::token::CHAR, "CHAR", "character"));
    }
    return &tokens;
}

RexLexer_Variables::RexLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>(soul::rex::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>(soul::rex::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>(soul::rex::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>(soul::rex::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace soul::rex::lexer
