// this file has been automatically generated from 'D:/work/otava/build/project.lexer' using soul lexer generator slg version 5.2.0

module otava.build.project_lexer;


namespace otava::build::project_lexer {

soul::ast::common::TokenCollection* GetTokens(otava::build::project_lexer::Tag tag)
{
    static soul::ast::common::TokenCollection tokens("otava.build.project_lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::PROJECT, "PROJECT", "'project'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::ASSIGN, "ASSIGN", "'='"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::INTERFACE, "INTERFACE", "'interface'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::SOURCE, "SOURCE", "'source'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::RESOURCE, "RESOURCE", "'resource'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::FILEPATH, "FILEPATH", "file path"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::REFERENCE, "REFERENCE", "'reference'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::DEFINE, "DEFINE", "'define'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::VALUE, "VALUE", "value"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::TARGET, "TARGET", "'target'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::PROGRAM, "PROGRAM", "'program'"));
        tokens.AddToken(new soul::ast::common::Token(otava::build::project_token::LIBRARY, "LIBRARY", "'library"));
    }
    return &tokens;
}

ProjectLexer_Variables::ProjectLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>(otava::build::project_lexer::Tag tag)
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "project", otava::build::project_token::PROJECT },
         { "interface", otava::build::project_token::INTERFACE },
         { "source", otava::build::project_token::SOURCE },
         { "reference", otava::build::project_token::REFERENCE },
         { "resource", otava::build::project_token::RESOURCE },
         { "define", otava::build::project_token::DEFINE },
         { "target", otava::build::project_token::TARGET },
         { "program", otava::build::project_token::PROGRAM },
         { "library", otava::build::project_token::LIBRARY },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>(otava::build::project_lexer::Tag tag)
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"project", otava::build::project_token::PROJECT },
         { u8"interface", otava::build::project_token::INTERFACE },
         { u8"source", otava::build::project_token::SOURCE },
         { u8"reference", otava::build::project_token::REFERENCE },
         { u8"resource", otava::build::project_token::RESOURCE },
         { u8"define", otava::build::project_token::DEFINE },
         { u8"target", otava::build::project_token::TARGET },
         { u8"program", otava::build::project_token::PROGRAM },
         { u8"library", otava::build::project_token::LIBRARY },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>(otava::build::project_lexer::Tag tag)
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"project", otava::build::project_token::PROJECT },
         { u"interface", otava::build::project_token::INTERFACE },
         { u"source", otava::build::project_token::SOURCE },
         { u"reference", otava::build::project_token::REFERENCE },
         { u"resource", otava::build::project_token::RESOURCE },
         { u"define", otava::build::project_token::DEFINE },
         { u"target", otava::build::project_token::TARGET },
         { u"program", otava::build::project_token::PROGRAM },
         { u"library", otava::build::project_token::LIBRARY },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>(otava::build::project_lexer::Tag tag)
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"project", otava::build::project_token::PROJECT },
         { U"interface", otava::build::project_token::INTERFACE },
         { U"source", otava::build::project_token::SOURCE },
         { U"reference", otava::build::project_token::REFERENCE },
         { U"resource", otava::build::project_token::RESOURCE },
         { U"define", otava::build::project_token::DEFINE },
         { U"target", otava::build::project_token::TARGET },
         { U"program", otava::build::project_token::PROGRAM },
         { U"library", otava::build::project_token::LIBRARY },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace otava::build::project_lexer
