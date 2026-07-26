// this file has been automatically generated from 'D:/work/test_soul/tools/otava/projects/soul/xml/xpath/xpath.lexer' using soul lexer generator oslg version 5.0.0

module soul.xml.xpath.lexer;


namespace soul::xml::xpath::lexer {

soul::ast::common::TokenCollection* GetTokens(soul::xml::xpath::lexer::Tag tag)
{
    static soul::ast::common::TokenCollection tokens("soul.xml.xpath.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::OR, "OR", "'or'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::AND, "AND", "'and'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::EQ, "EQ", "'='"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::NEQ, "NEQ", "'!='"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::LEQ, "LEQ", "'<='"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::GEQ, "GEQ", "'>='"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::LESS, "LESS", "'<'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::GREATER, "GREATER", "'>'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::MINUS, "MINUS", "'-'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::STAR, "STAR", "'*'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::DIV, "DIV", "'div'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::MOD, "MOD", "'mod'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::UNION, "UNION", "'|'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::SLASH_SLASH, "SLASH_SLASH", "'//'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::SLASH, "SLASH", "'/'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::DOT_DOT, "DOT_DOT", "'..'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::COLON_COLON, "COLON_COLON", "'::'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::DOLLAR, "DOLLAR", "'$'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::ANCESTOR, "ANCESTOR", "'ancestor'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::ANCESTOR_OR_SELF, "ANCESTOR_OR_SELF", "'ancestor-or-self'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::ATTRIBUTE, "ATTRIBUTE", "'attribte'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::CHILD, "CHILD", "'child'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::DESCENDANT, "DESCENDANT", "'descendant"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::DESCENDANT_OR_SELF, "DESCENDANT_OR_SELF", "'descendant-or-self'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::FOLLOWING, "FOLLOWING", "'following'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::FOLLOWING_SIBLING, "FOLLOWING_SIBLING", "'following-sibling'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::NAMESPACE, "NAMESPACE", "'namespace'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::PARENT, "PARENT", "'parent'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::PRECEDING, "PRECEDING", "'preceding'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::PRECEDING_SIBLING, "PRECEDING_SIBLING", "'preceding-siblibg'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::SELF, "SELF", "'self'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::AT, "AT", "'@'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::PROCESSING_INSTRUCTION, "PROCESSING_INSTRUCTION", "'processing-instructioon'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::COMMENT, "COMMENT", "'comment'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::TEXT, "TEXT", "'text'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::NODE, "NODE", "'node'"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::DQ_STRING, "DQ_STRING", "string"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::SQ_STRING, "SQ_STRING", "string"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::NUMBER, "NUMBER", "number"));
        tokens.AddToken(new soul::ast::common::Token(soul::xml::xpath::token::NAME, "NAME", "name"));
    }
    return &tokens;
}

XPathLexer_Variables::XPathLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>(soul::xml::xpath::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "or", soul::xml::xpath::token::OR },
         { "and", soul::xml::xpath::token::AND },
         { "div", soul::xml::xpath::token::DIV },
         { "mod", soul::xml::xpath::token::MOD },
         { "ancestor", soul::xml::xpath::token::ANCESTOR },
         { "ancestor-or-self", soul::xml::xpath::token::ANCESTOR_OR_SELF },
         { "attribute", soul::xml::xpath::token::ATTRIBUTE },
         { "child", soul::xml::xpath::token::CHILD },
         { "descendant", soul::xml::xpath::token::DESCENDANT },
         { "descendant-or-self", soul::xml::xpath::token::DESCENDANT_OR_SELF },
         { "following", soul::xml::xpath::token::FOLLOWING },
         { "following-sibling", soul::xml::xpath::token::FOLLOWING_SIBLING },
         { "namespace", soul::xml::xpath::token::NAMESPACE },
         { "parent", soul::xml::xpath::token::PARENT },
         { "preceding", soul::xml::xpath::token::PRECEDING },
         { "preceding-sibling", soul::xml::xpath::token::PRECEDING_SIBLING },
         { "self", soul::xml::xpath::token::SELF },
         { "processing-instruction", soul::xml::xpath::token::PROCESSING_INSTRUCTION },
         { "comment", soul::xml::xpath::token::COMMENT },
         { "text", soul::xml::xpath::token::TEXT },
         { "node", soul::xml::xpath::token::NODE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>(soul::xml::xpath::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"or", soul::xml::xpath::token::OR },
         { u8"and", soul::xml::xpath::token::AND },
         { u8"div", soul::xml::xpath::token::DIV },
         { u8"mod", soul::xml::xpath::token::MOD },
         { u8"ancestor", soul::xml::xpath::token::ANCESTOR },
         { u8"ancestor-or-self", soul::xml::xpath::token::ANCESTOR_OR_SELF },
         { u8"attribute", soul::xml::xpath::token::ATTRIBUTE },
         { u8"child", soul::xml::xpath::token::CHILD },
         { u8"descendant", soul::xml::xpath::token::DESCENDANT },
         { u8"descendant-or-self", soul::xml::xpath::token::DESCENDANT_OR_SELF },
         { u8"following", soul::xml::xpath::token::FOLLOWING },
         { u8"following-sibling", soul::xml::xpath::token::FOLLOWING_SIBLING },
         { u8"namespace", soul::xml::xpath::token::NAMESPACE },
         { u8"parent", soul::xml::xpath::token::PARENT },
         { u8"preceding", soul::xml::xpath::token::PRECEDING },
         { u8"preceding-sibling", soul::xml::xpath::token::PRECEDING_SIBLING },
         { u8"self", soul::xml::xpath::token::SELF },
         { u8"processing-instruction", soul::xml::xpath::token::PROCESSING_INSTRUCTION },
         { u8"comment", soul::xml::xpath::token::COMMENT },
         { u8"text", soul::xml::xpath::token::TEXT },
         { u8"node", soul::xml::xpath::token::NODE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>(soul::xml::xpath::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"or", soul::xml::xpath::token::OR },
         { u"and", soul::xml::xpath::token::AND },
         { u"div", soul::xml::xpath::token::DIV },
         { u"mod", soul::xml::xpath::token::MOD },
         { u"ancestor", soul::xml::xpath::token::ANCESTOR },
         { u"ancestor-or-self", soul::xml::xpath::token::ANCESTOR_OR_SELF },
         { u"attribute", soul::xml::xpath::token::ATTRIBUTE },
         { u"child", soul::xml::xpath::token::CHILD },
         { u"descendant", soul::xml::xpath::token::DESCENDANT },
         { u"descendant-or-self", soul::xml::xpath::token::DESCENDANT_OR_SELF },
         { u"following", soul::xml::xpath::token::FOLLOWING },
         { u"following-sibling", soul::xml::xpath::token::FOLLOWING_SIBLING },
         { u"namespace", soul::xml::xpath::token::NAMESPACE },
         { u"parent", soul::xml::xpath::token::PARENT },
         { u"preceding", soul::xml::xpath::token::PRECEDING },
         { u"preceding-sibling", soul::xml::xpath::token::PRECEDING_SIBLING },
         { u"self", soul::xml::xpath::token::SELF },
         { u"processing-instruction", soul::xml::xpath::token::PROCESSING_INSTRUCTION },
         { u"comment", soul::xml::xpath::token::COMMENT },
         { u"text", soul::xml::xpath::token::TEXT },
         { u"node", soul::xml::xpath::token::NODE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>(soul::xml::xpath::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"or", soul::xml::xpath::token::OR },
         { U"and", soul::xml::xpath::token::AND },
         { U"div", soul::xml::xpath::token::DIV },
         { U"mod", soul::xml::xpath::token::MOD },
         { U"ancestor", soul::xml::xpath::token::ANCESTOR },
         { U"ancestor-or-self", soul::xml::xpath::token::ANCESTOR_OR_SELF },
         { U"attribute", soul::xml::xpath::token::ATTRIBUTE },
         { U"child", soul::xml::xpath::token::CHILD },
         { U"descendant", soul::xml::xpath::token::DESCENDANT },
         { U"descendant-or-self", soul::xml::xpath::token::DESCENDANT_OR_SELF },
         { U"following", soul::xml::xpath::token::FOLLOWING },
         { U"following-sibling", soul::xml::xpath::token::FOLLOWING_SIBLING },
         { U"namespace", soul::xml::xpath::token::NAMESPACE },
         { U"parent", soul::xml::xpath::token::PARENT },
         { U"preceding", soul::xml::xpath::token::PRECEDING },
         { U"preceding-sibling", soul::xml::xpath::token::PRECEDING_SIBLING },
         { U"self", soul::xml::xpath::token::SELF },
         { U"processing-instruction", soul::xml::xpath::token::PROCESSING_INSTRUCTION },
         { U"comment", soul::xml::xpath::token::COMMENT },
         { U"text", soul::xml::xpath::token::TEXT },
         { U"node", soul::xml::xpath::token::NODE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace soul::xml::xpath::lexer
