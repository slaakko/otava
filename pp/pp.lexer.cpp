// this file has been automatically generated from 'D:/work/otava/pp/pp.lexer' using soul lexer generator slg version 5.2.0

module otava.pp.lexer;


namespace otava::pp::lexer {

soul::ast::common::TokenCollection* GetTokens(otava::pp::lexer::Tag tag)
{
    static soul::ast::common::TokenCollection tokens("otava.pp.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::common::Token(otava::token::ALIGNAS, "ALIGNAS", "'alignas'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ALIGNOF, "ALIGNOF", "'alignof'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ASM, "ASM", "'asm'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::AUTO, "AUTO", "'auto'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::BOOL, "BOOL", "'bool'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::BREAK, "BREAK", "'break'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CASE, "CASE", "'case'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CATCH, "CATCH", "'catch'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CHAR, "CHAR", "'char'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CHAR8_T, "CHAR8_T", "'char8_t'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CHAR16_T, "CHAR16_T", "'char16_t'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CHAR32_T, "CHAR32_T", "'char32_t'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CLASS, "CLASS", "'class'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CONCEPT, "CONCEPT", "'concept'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CONST, "CONST", "'const'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CONSTEVAL, "CONSTEVAL", "'consteval'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CONSTEXPR, "CONSTEXPR", "'constexpr'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CONSTINIT, "CONSTINIT", "'constinit'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CONST_CAST, "CONST_CAST", "'const_cast'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CONTINUE, "CONTINUE", "'continue'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CO_AWAIT, "CO_AWAIT", "'co_await'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CO_RETURN, "CO_RETURN", "'co_return'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CO_YIELD, "CO_YIELD", "'co_yield'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DECLTYPE, "DECLTYPE", "'decltype'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DEFAULT, "DEFAULT", "'default'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DELETE, "DELETE", "'delete'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DO, "DO", "'do'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DOUBLE, "DOUBLE", "'double'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DYNAMIC_CAST, "DYNAMIC_CAST", "'dynamic_cast'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ELSE, "ELSE", "'else'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ENUM, "ENUM", "'enum'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::EXPLICIT, "EXPLICIT", "'explicit'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::EXPORT, "EXPORT", "'export'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::EXTERN, "EXTERN", "'extern'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::FALSE, "FALSE", "'false'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::FLOAT, "FLOAT", "'float'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::FOR, "FOR", "'for'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::FRIEND, "FRIEND", "'friend'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::GOTO, "GOTO", "'goto'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::IF, "IF", "'if'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::INLINE, "INLINE", "'inline'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::INT, "INT", "'int'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::LONG, "LONG", "'long'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::MUTABLE, "MUTABLE", "'mutable'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::NAMESPACE, "NAMESPACE", "'namespace'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::NEW, "NEW", "'new'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::NOEXCEPT, "NOEXCEPT", "'noexcept'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::NULLPTR, "NULLPTR", "'nullptr'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::OPERATOR, "OPERATOR", "'operator'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::PRIVATE, "PRIVATE", "'private'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::PROTECTED, "PROTECTED", "'protected'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::PUBLIC, "PUBLIC", "'public'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::REGISTER, "REGISTER", "'register'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::REINTERPRET_CAST, "REINTERPRET_CAST", "'reinterpret_cast'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::REQUIRES, "REQUIRES", "'requires'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::RETURN, "RETURN", "'return'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SHORT, "SHORT", "'short'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SIGNED, "SIGNED", "'signed'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SIZEOF, "SIZEOF", "'sizeof'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::STATIC, "STATIC", "'static'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::STATIC_ASSERT, "STATIC_ASSERT", "'static_assert'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::STATIC_CAST, "STATIC_CAST", "'static_cast'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::STRUCT, "STRUCT", "'struct'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SWITCH, "SWITCH", "'switch'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::TEMPLATE, "TEMPLATE", "'template'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::THIS, "THIS", "'this'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::THREAD_LOCAL, "THREAD_LOCAL", "'thread_local'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::THROW, "THROW", "'throw'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::TRUE, "TRUE", "'true'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::TRY, "TRY", "'try'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::TYPEDEF, "TYPEDEF", "'typedef'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::TYPEID, "TYPEID", "'typeid'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::TYPENAME, "TYPENAME", "'typename'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::UNION, "UNION", "'union'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::UNSIGNED, "UNSIGNED", "'unsigned'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::USING, "USING", "'using'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::VIRTUAL, "VIRTUAL", "'virtual'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::VOID, "VOID", "'void'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::VOLATILE, "VOLATILE", "'volatile'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::WCHAR_T, "WCHAR_T", "'wchar_t'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::WHILE, "WHILE", "'while'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::FLOATING_LITERAL, "FLOATING_LITERAL", "floating literal"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::INTEGER_LITERAL, "INTEGER_LITERAL", "integer literal"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::CHARACTER_LITERAL, "CHARACTER_LITERAL", "character literal"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::STRING_LITERAL, "STRING_LITERAL", "string literal"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::BEGIN_RAW_STRING_LITERAL, "BEGIN_RAW_STRING_LITERAL", "'begin raw string literal'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::COLON_COLON, "COLON_COLON", "'::'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ASSIGN, "ASSIGN", "'='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::MUL_ASSIGN, "MUL_ASSIGN", "'*='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DIV_ASSIGN, "DIV_ASSIGN", "'/='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::REM_ASSIGN, "REM_ASSIGN", "'%='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ADD_ASSIGN, "ADD_ASSIGN", "'+='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SUB_ASSIGN, "SUB_ASSIGN", "'-='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SHIFT_RIGHT_ASSIGN, "SHIFT_RIGHT_ASSIGN", "'>>='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SHIFT_LEFT_ASSIGN, "SHIFT_LEFT_ASSIGN", "'<<='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::AND_ASSIGN, "AND_ASSIGN", "'&='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::XOR_ASSIGN, "XOR_ASSIGN", "'^='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::OR_ASSIGN, "OR_ASSIGN", "'|='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::QUEST, "QUEST", "'?'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::OR_OR, "OR_OR", "'||'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::AMP_AMP, "AMP_AMP", "'&&'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::OR, "OR", "'|'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::XOR, "XOR", "'^'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::AMP, "AMP", "'&'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::EQ, "EQ", "'=='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::NEQ, "NEQ", "'!='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::LEQ, "LEQ", "'<='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::GEQ, "GEQ", "'>='"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SPACE_SHIP, "SPACE_SHIP", "<=>"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::LANGLE, "LANGLE", "'<'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::RANGLE, "RANGLE", "'>'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SHIFT_LEFT, "SHIFT_LEFT", "'<<'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SHIFT_RIGHT, "SHIFT_RIGHT", "'>>'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::MINUS, "MINUS", "'-'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::STAR, "STAR", "'*'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DIV, "DIV", "'/'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::MOD, "MOD", "'%'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DOT_STAR, "DOT_STAR", "'.*'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ARROW_STAR, "ARROW_STAR", "'->*'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::PLUS_PLUS, "PLUS_PLUS", "'++'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::MINUS_MINUS, "MINUS_MINUS", "'--'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::EXCLAMATION, "EXCLAMATION", "'!'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::TILDE, "TILDE", "'~'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::LBRACE, "LBRACE", "'{'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::RBRACE, "RBRACE", "'}'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ARROW, "ARROW", "'->'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ELLIPSIS, "ELLIPSIS", "'...'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ANGLE_HEADER_NAME, "ANGLE_HEADER_NAME", "'angle header name'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::QUOTE_HEADER_NAME, "QUOTE_HEADER_NAME", "'quote header name'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::KEYWORD, "KEYWORD", "keyword"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::WS, "WS", "white space"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::NEWLINE, "NEWLINE", "new line"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::LINE_COMMENT, "LINE_COMMENT", "line comment"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::BLOCK_COMMENT_START, "BLOCK_COMMENT_START", "block comment start"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::BLOCK_COMMENT_END, "BLOCK_COMMENT_END", "block comment end"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::OTHER, "OTHER", "other"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::PP_LINE, "PP_LINE", "preprocessing line"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::HASH, "HASH", "'#'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DEFINE, "DEFINE", "'define'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::INCLUDE, "INCLUDE", "'include'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::IFDEF, "IFDEF", "'ifdef'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::IFNDEF, "IFNDEF", "'ifndef'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ELIF, "ELIF", "'elif'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::ENDIF, "ENDIF", "'endif'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::UNDEF, "UNDEF", "'undef'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::DEFINED, "DEFINED", "'defined'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::PRAGMA, "PRAGMA", "'pragma'"));
        tokens.AddToken(new soul::ast::common::Token(otava::token::MAX, "MAX", "max"));
    }
    return &tokens;
}

PPLexer_Variables::PPLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>(otava::pp::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "include", otava::token::INCLUDE },
         { "if", otava::token::IF },
         { "elif", otava::token::ELIF },
         { "else", otava::token::ELSE },
         { "ifdef", otava::token::IFDEF },
         { "ifndef", otava::token::IFNDEF },
         { "endif", otava::token::ENDIF },
         { "undef", otava::token::UNDEF },
         { "defined", otava::token::DEFINED },
         { "pragma", otava::token::PRAGMA },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>(otava::pp::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"include", otava::token::INCLUDE },
         { u8"if", otava::token::IF },
         { u8"elif", otava::token::ELIF },
         { u8"else", otava::token::ELSE },
         { u8"ifdef", otava::token::IFDEF },
         { u8"ifndef", otava::token::IFNDEF },
         { u8"endif", otava::token::ENDIF },
         { u8"undef", otava::token::UNDEF },
         { u8"defined", otava::token::DEFINED },
         { u8"pragma", otava::token::PRAGMA },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>(otava::pp::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"include", otava::token::INCLUDE },
         { u"if", otava::token::IF },
         { u"elif", otava::token::ELIF },
         { u"else", otava::token::ELSE },
         { u"ifdef", otava::token::IFDEF },
         { u"ifndef", otava::token::IFNDEF },
         { u"endif", otava::token::ENDIF },
         { u"undef", otava::token::UNDEF },
         { u"defined", otava::token::DEFINED },
         { u"pragma", otava::token::PRAGMA },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>(otava::pp::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"include", otava::token::INCLUDE },
         { U"if", otava::token::IF },
         { U"elif", otava::token::ELIF },
         { U"else", otava::token::ELSE },
         { U"ifdef", otava::token::IFDEF },
         { U"ifndef", otava::token::IFNDEF },
         { U"endif", otava::token::ENDIF },
         { U"undef", otava::token::UNDEF },
         { U"defined", otava::token::DEFINED },
         { U"pragma", otava::token::PRAGMA },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace otava::pp::lexer
