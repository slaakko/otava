// this file has been automatically generated from 'D:/src/otava-0.2.3/projects/soul/lex/re.lexer' using soul lexer generator oslg version 0.2.4

module soul.lex.re;


namespace soul::lex::re {

soul::ast::common::TokenCollection* GetTokens()
{
    static soul::ast::common::TokenCollection tokens("soul.lex.re.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::AUTO, "AUTO", "'auto'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::REGISTER, "REGISTER", "'register'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::STATIC, "STATIC", "'static'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::EXTERN, "EXTERN", "'extern'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::MUTABLE, "MUTABLE", "'mutable'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::EXPORT, "EXPORT", "'export'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::IMPORT, "IMPORT", "'import'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::MODULE, "MODULE", "'module"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CHAR, "CHAR", "'char'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CHAR8T, "CHAR8T", "'char8_t'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CHAR16T, "CHAR16T", "'char16_t'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CHAR32T, "CHAR32T", "'char32_t'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::WCHART, "WCHART", "'wchar_t'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::BOOL, "BOOL", "'bool'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::SHORT, "SHORT", "'short'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::INT, "INT", "'int'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::LONG, "LONG", "'long'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::SIGNED, "SIGNED", "'signed'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::UNSIGNED, "UNSIGNED", "'unsigned'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::FLOAT, "FLOAT", "'float'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::DOUBLE, "DOUBLE", "'double'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::VOID, "VOID", "'void'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::TYPEDEF, "TYPEDEF", "'typedef'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CONST, "CONST", "'const'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::VOLATILE, "VOLATILE", "'volatile'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::USING, "USING", "'using'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::NAMESPACE, "NAMESPACE", "'namespace'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::THROW, "THROW", "'throw'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::SIZEOF, "SIZEOF", "'sizeof'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::OPERATOR, "OPERATOR", "'operator'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::DYNAMIC_CAST, "DYNAMIC_CAST", "'dynamic_cast'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::STATIC_CAST, "STATIC_CAST", "'static_cast'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::REINTERPRET_CAST, "REINTERPRET_CAST", "'reinterpret_cast'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CONST_CAST, "CONST_CAST", "'const_cast'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::TYPEID, "TYPEID", "'typeid'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::THIS, "THIS", "'this'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::NEW, "NEW", "'new'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::DELETE, "DELETE", "'delete'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::TRUE, "TRUE", "'true'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::FALSE, "FALSE", "'false'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::NULLPTR, "NULLPTR", "'nullptr'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CASE, "CASE", "'case'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::DEFAULT, "DEFAULT", "'default'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::IF, "IF", "'if'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::ELSE, "ELSE", "'else'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::SWITCH, "SWITCH", "'switch'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::WHILE, "WHILE", "'while'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::DO, "DO", "'do'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::FOR, "FOR", "'for'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::BREAK, "BREAK", "'break'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CONTINUE, "CONTINUE", "'continue'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::RETURN, "RETURN", "'return'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::GOTO, "GOTO", "'goto'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::TRY, "TRY", "'try'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CATCH, "CATCH", "'catch'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::INTEGER_LITERAL, "INTEGER_LITERAL", "integer literal"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::FLOATING_LITERAL, "FLOATING_LITERAL", "floating literal"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::CHAR_LITERAL, "CHAR_LITERAL", "character literal"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::token::STRING_LITERAL, "STRING_LITERAL", "string literal"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::DISJUNCTION, "DISJUNCTION", "'||'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::AMP_AMP, "AMP_AMP", "'&&'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::DOT_STAR, "DOT_STAR", "'.*'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::ARROW_STAR, "ARROW_STAR", "'->*'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::EQ, "EQ", "'=='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::NEQ, "NEQ", "'!='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::LEQ, "LEQ", "'<='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::GEQ, "GEQ", "'>='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::SHIFT_LEFT, "SHIFT_LEFT", "'<<'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::SHIFT_RIGHT, "SHIFT_RIGHT", "'>>'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::MINUS, "MINUS", "'-'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::STAR, "STAR", "'*'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::DIV, "DIV", "'/'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::REM, "REM", "'%'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::PLUS_PLUS, "PLUS_PLUS", "'++'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::MINUS_MINUS, "MINUS_MINUS", "'--'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::ELLIPSIS, "ELLIPSIS", "'...'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::COLON_COLON, "COLON_COLON", "'::'"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::ASSIGN, "ASSIGN", "'='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::MUL_ASSIGN, "MUL_ASSIGN", "'*='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::DIV_ASSIGN, "DIV_ASSIGN", "'/='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::REM_ASSIGN, "REM_ASSIGN", "'%='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::PLUS_ASSIGN, "PLUS_ASSIGN", "'+='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::MINUS_ASSIGN, "MINUS_ASSIGN", "'-='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::SHIFT_LEFT_ASSIGN, "SHIFT_LEFT_ASSIGN", "'<<='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::SHIFT_RIGHT_ASSIGN, "SHIFT_RIGHT_ASSIGN", "'>>='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::AND_ASSIGN, "AND_ASSIGN", "'&='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::XOR_ASSIGN, "XOR_ASSIGN", "'^='"));
        tokens.AddToken(new soul::ast::common::Token(soul::cpp::op::token::OR_ASSIGN, "OR_ASSIGN", "'|='"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::LBRACE, "LBRACE", "'{'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::RBRACE, "RBRACE", "'}'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::LANGLE, "LANGLE", "'<'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::RANGLE, "RANGLE", "'>'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::HASH, "HASH", "'#'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::ARROW, "ARROW", "'->'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::QUEST, "QUEST", "'?'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::AMP, "AMP", "'&'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::DOLLAR, "DOLLAR", "'$'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::EXCLAMATION, "EXCLAMATION", "'!'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::TILDE, "TILDE", "'~'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::CARET, "CARET", "'^'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::PIPE, "PIPE", "'|'"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::PRODUCES, "PRODUCES", "'::='"));
        tokens.AddToken(new soul::ast::common::Token(soul::punctuation::token::BACKSLASH, "BACKSLASH", "'\'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::TOKENS, "TOKENS", "'tokens'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::KEYWORDS, "KEYWORDS", "'keywords'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::EXPRESSIONS, "EXPRESSIONS", "'expressions'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::RULES, "RULES", "'rules'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::VARIABLES, "VARIABLES", "''variables'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::ACTIONS, "ACTIONS", "'actions'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::MAIN, "MAIN", "'main'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::START, "START", "'start'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::EMPTY, "EMPTY", "'empty'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::ANY, "ANY", "'any'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::VAR, "VAR", "'var'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::FILEPATH, "FILEPATH", "file path"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::PROJECT, "PROJECT", "'project'"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::CHARACTER, "CHARACTER", "character"));
        tokens.AddToken(new soul::ast::common::Token(soul::tool::token::ESCAPE, "ESCAPE", "escape"));
    }
    return &tokens;
}

RegExLexer_Variables::RegExLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace soul::lex::re
