// this file has been automatically generated from 'D:/work/otava/build/solution.lexer' using soul lexer generator slg version 5.2.0

module otava.build.solution_lexer;


namespace otava::build::solution_lexer {

soul::ast::common::TokenCollection* GetTokens(otava::build::solution_lexer::Tag tag)
{
    static soul::ast::common::TokenCollection tokens("otava.build.solution_lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::common::Token(otava::build::solution_token::SOLUTION, "SOLUTION", "'solution'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::solution_token::ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::solution_token::DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::solution_token::SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::solution_token::PROJECT, "PROJECT", "'project'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::solution_token::FILEPATH, "FILEPATH", "file path"));
    }
    return &tokens;
}

SolutionLexer_Variables::SolutionLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>(otava::build::solution_lexer::Tag tag)
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "solution", otava::build::solution_token::SOLUTION },
         { "project", otava::build::solution_token::PROJECT },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>(otava::build::solution_lexer::Tag tag)
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"solution", otava::build::solution_token::SOLUTION },
         { u8"project", otava::build::solution_token::PROJECT },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>(otava::build::solution_lexer::Tag tag)
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"solution", otava::build::solution_token::SOLUTION },
         { u"project", otava::build::solution_token::PROJECT },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>(otava::build::solution_lexer::Tag tag)
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"solution", otava::build::solution_token::SOLUTION },
         { U"project", otava::build::solution_token::PROJECT },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace otava::build::solution_lexer
