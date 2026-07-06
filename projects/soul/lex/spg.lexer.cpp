// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/lex/spg.lexer' using soul lexer generator oslg version 5.0.0

module soul.lex.spg;


namespace soul::lex::spg {

soul::ast::common::TokenCollection* GetTokens()
{
    static soul::ast::common::TokenCollection tokens("soul.lex.spg.tokens");
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

SpgLexer_Variables::SpgLexer_Variables() :
    leftAngleCount(), matchFilePath()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "auto", soul::cpp::token::AUTO },
         { "register", soul::cpp::token::REGISTER },
         { "static", soul::cpp::token::STATIC },
         { "extern", soul::cpp::token::EXTERN },
         { "mutable", soul::cpp::token::MUTABLE },
         { "char", soul::cpp::token::CHAR },
         { "char8_t", soul::cpp::token::CHAR8T },
         { "char16_t", soul::cpp::token::CHAR16T },
         { "char32_t", soul::cpp::token::CHAR32T },
         { "wchar_t", soul::cpp::token::WCHART },
         { "bool", soul::cpp::token::BOOL },
         { "short", soul::cpp::token::SHORT },
         { "int", soul::cpp::token::INT },
         { "long", soul::cpp::token::LONG },
         { "signed", soul::cpp::token::SIGNED },
         { "unsigned", soul::cpp::token::UNSIGNED },
         { "float", soul::cpp::token::FLOAT },
         { "double", soul::cpp::token::DOUBLE },
         { "void", soul::cpp::token::VOID },
         { "typedef", soul::cpp::token::TYPEDEF },
         { "const", soul::cpp::token::CONST },
         { "volatile", soul::cpp::token::VOLATILE },
         { "using", soul::cpp::token::USING },
         { "namespace", soul::cpp::token::NAMESPACE },
         { "throw", soul::cpp::token::THROW },
         { "sizeof", soul::cpp::token::SIZEOF },
         { "operator", soul::cpp::token::OPERATOR },
         { "dynamic_cast", soul::cpp::token::DYNAMIC_CAST },
         { "static_cast", soul::cpp::token::STATIC_CAST },
         { "reinterpret_cast", soul::cpp::token::REINTERPRET_CAST },
         { "const_cast", soul::cpp::token::CONST_CAST },
         { "typeid", soul::cpp::token::TYPEID },
         { "this", soul::cpp::token::THIS },
         { "new", soul::cpp::token::NEW },
         { "delete", soul::cpp::token::DELETE },
         { "true", soul::cpp::token::TRUE },
         { "false", soul::cpp::token::FALSE },
         { "nullptr", soul::cpp::token::NULLPTR },
         { "case", soul::cpp::token::CASE },
         { "default", soul::cpp::token::DEFAULT },
         { "if", soul::cpp::token::IF },
         { "else", soul::cpp::token::ELSE },
         { "switch", soul::cpp::token::SWITCH },
         { "while", soul::cpp::token::WHILE },
         { "do", soul::cpp::token::DO },
         { "for", soul::cpp::token::FOR },
         { "break", soul::cpp::token::BREAK },
         { "continue", soul::cpp::token::CONTINUE },
         { "return", soul::cpp::token::RETURN },
         { "goto", soul::cpp::token::GOTO },
         { "try", soul::cpp::token::TRY },
         { "catch", soul::cpp::token::CATCH },
         { "main", soul::tool::token::MAIN },
         { "start", soul::tool::token::START },
         { "empty", soul::tool::token::EMPTY },
         { "any", soul::tool::token::ANY },
         { "var", soul::tool::token::VAR },
         { "project", soul::tool::token::PROJECT },
         { "tokens", soul::tool::token::TOKENS },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"auto", soul::cpp::token::AUTO },
         { u8"register", soul::cpp::token::REGISTER },
         { u8"static", soul::cpp::token::STATIC },
         { u8"extern", soul::cpp::token::EXTERN },
         { u8"mutable", soul::cpp::token::MUTABLE },
         { u8"char", soul::cpp::token::CHAR },
         { u8"char8_t", soul::cpp::token::CHAR8T },
         { u8"char16_t", soul::cpp::token::CHAR16T },
         { u8"char32_t", soul::cpp::token::CHAR32T },
         { u8"wchar_t", soul::cpp::token::WCHART },
         { u8"bool", soul::cpp::token::BOOL },
         { u8"short", soul::cpp::token::SHORT },
         { u8"int", soul::cpp::token::INT },
         { u8"long", soul::cpp::token::LONG },
         { u8"signed", soul::cpp::token::SIGNED },
         { u8"unsigned", soul::cpp::token::UNSIGNED },
         { u8"float", soul::cpp::token::FLOAT },
         { u8"double", soul::cpp::token::DOUBLE },
         { u8"void", soul::cpp::token::VOID },
         { u8"typedef", soul::cpp::token::TYPEDEF },
         { u8"const", soul::cpp::token::CONST },
         { u8"volatile", soul::cpp::token::VOLATILE },
         { u8"using", soul::cpp::token::USING },
         { u8"namespace", soul::cpp::token::NAMESPACE },
         { u8"throw", soul::cpp::token::THROW },
         { u8"sizeof", soul::cpp::token::SIZEOF },
         { u8"operator", soul::cpp::token::OPERATOR },
         { u8"dynamic_cast", soul::cpp::token::DYNAMIC_CAST },
         { u8"static_cast", soul::cpp::token::STATIC_CAST },
         { u8"reinterpret_cast", soul::cpp::token::REINTERPRET_CAST },
         { u8"const_cast", soul::cpp::token::CONST_CAST },
         { u8"typeid", soul::cpp::token::TYPEID },
         { u8"this", soul::cpp::token::THIS },
         { u8"new", soul::cpp::token::NEW },
         { u8"delete", soul::cpp::token::DELETE },
         { u8"true", soul::cpp::token::TRUE },
         { u8"false", soul::cpp::token::FALSE },
         { u8"nullptr", soul::cpp::token::NULLPTR },
         { u8"case", soul::cpp::token::CASE },
         { u8"default", soul::cpp::token::DEFAULT },
         { u8"if", soul::cpp::token::IF },
         { u8"else", soul::cpp::token::ELSE },
         { u8"switch", soul::cpp::token::SWITCH },
         { u8"while", soul::cpp::token::WHILE },
         { u8"do", soul::cpp::token::DO },
         { u8"for", soul::cpp::token::FOR },
         { u8"break", soul::cpp::token::BREAK },
         { u8"continue", soul::cpp::token::CONTINUE },
         { u8"return", soul::cpp::token::RETURN },
         { u8"goto", soul::cpp::token::GOTO },
         { u8"try", soul::cpp::token::TRY },
         { u8"catch", soul::cpp::token::CATCH },
         { u8"main", soul::tool::token::MAIN },
         { u8"start", soul::tool::token::START },
         { u8"empty", soul::tool::token::EMPTY },
         { u8"any", soul::tool::token::ANY },
         { u8"var", soul::tool::token::VAR },
         { u8"project", soul::tool::token::PROJECT },
         { u8"tokens", soul::tool::token::TOKENS },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"auto", soul::cpp::token::AUTO },
         { u"register", soul::cpp::token::REGISTER },
         { u"static", soul::cpp::token::STATIC },
         { u"extern", soul::cpp::token::EXTERN },
         { u"mutable", soul::cpp::token::MUTABLE },
         { u"char", soul::cpp::token::CHAR },
         { u"char8_t", soul::cpp::token::CHAR8T },
         { u"char16_t", soul::cpp::token::CHAR16T },
         { u"char32_t", soul::cpp::token::CHAR32T },
         { u"wchar_t", soul::cpp::token::WCHART },
         { u"bool", soul::cpp::token::BOOL },
         { u"short", soul::cpp::token::SHORT },
         { u"int", soul::cpp::token::INT },
         { u"long", soul::cpp::token::LONG },
         { u"signed", soul::cpp::token::SIGNED },
         { u"unsigned", soul::cpp::token::UNSIGNED },
         { u"float", soul::cpp::token::FLOAT },
         { u"double", soul::cpp::token::DOUBLE },
         { u"void", soul::cpp::token::VOID },
         { u"typedef", soul::cpp::token::TYPEDEF },
         { u"const", soul::cpp::token::CONST },
         { u"volatile", soul::cpp::token::VOLATILE },
         { u"using", soul::cpp::token::USING },
         { u"namespace", soul::cpp::token::NAMESPACE },
         { u"throw", soul::cpp::token::THROW },
         { u"sizeof", soul::cpp::token::SIZEOF },
         { u"operator", soul::cpp::token::OPERATOR },
         { u"dynamic_cast", soul::cpp::token::DYNAMIC_CAST },
         { u"static_cast", soul::cpp::token::STATIC_CAST },
         { u"reinterpret_cast", soul::cpp::token::REINTERPRET_CAST },
         { u"const_cast", soul::cpp::token::CONST_CAST },
         { u"typeid", soul::cpp::token::TYPEID },
         { u"this", soul::cpp::token::THIS },
         { u"new", soul::cpp::token::NEW },
         { u"delete", soul::cpp::token::DELETE },
         { u"true", soul::cpp::token::TRUE },
         { u"false", soul::cpp::token::FALSE },
         { u"nullptr", soul::cpp::token::NULLPTR },
         { u"case", soul::cpp::token::CASE },
         { u"default", soul::cpp::token::DEFAULT },
         { u"if", soul::cpp::token::IF },
         { u"else", soul::cpp::token::ELSE },
         { u"switch", soul::cpp::token::SWITCH },
         { u"while", soul::cpp::token::WHILE },
         { u"do", soul::cpp::token::DO },
         { u"for", soul::cpp::token::FOR },
         { u"break", soul::cpp::token::BREAK },
         { u"continue", soul::cpp::token::CONTINUE },
         { u"return", soul::cpp::token::RETURN },
         { u"goto", soul::cpp::token::GOTO },
         { u"try", soul::cpp::token::TRY },
         { u"catch", soul::cpp::token::CATCH },
         { u"main", soul::tool::token::MAIN },
         { u"start", soul::tool::token::START },
         { u"empty", soul::tool::token::EMPTY },
         { u"any", soul::tool::token::ANY },
         { u"var", soul::tool::token::VAR },
         { u"project", soul::tool::token::PROJECT },
         { u"tokens", soul::tool::token::TOKENS },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"auto", soul::cpp::token::AUTO },
         { U"register", soul::cpp::token::REGISTER },
         { U"static", soul::cpp::token::STATIC },
         { U"extern", soul::cpp::token::EXTERN },
         { U"mutable", soul::cpp::token::MUTABLE },
         { U"char", soul::cpp::token::CHAR },
         { U"char8_t", soul::cpp::token::CHAR8T },
         { U"char16_t", soul::cpp::token::CHAR16T },
         { U"char32_t", soul::cpp::token::CHAR32T },
         { U"wchar_t", soul::cpp::token::WCHART },
         { U"bool", soul::cpp::token::BOOL },
         { U"short", soul::cpp::token::SHORT },
         { U"int", soul::cpp::token::INT },
         { U"long", soul::cpp::token::LONG },
         { U"signed", soul::cpp::token::SIGNED },
         { U"unsigned", soul::cpp::token::UNSIGNED },
         { U"float", soul::cpp::token::FLOAT },
         { U"double", soul::cpp::token::DOUBLE },
         { U"void", soul::cpp::token::VOID },
         { U"typedef", soul::cpp::token::TYPEDEF },
         { U"const", soul::cpp::token::CONST },
         { U"volatile", soul::cpp::token::VOLATILE },
         { U"using", soul::cpp::token::USING },
         { U"namespace", soul::cpp::token::NAMESPACE },
         { U"throw", soul::cpp::token::THROW },
         { U"sizeof", soul::cpp::token::SIZEOF },
         { U"operator", soul::cpp::token::OPERATOR },
         { U"dynamic_cast", soul::cpp::token::DYNAMIC_CAST },
         { U"static_cast", soul::cpp::token::STATIC_CAST },
         { U"reinterpret_cast", soul::cpp::token::REINTERPRET_CAST },
         { U"const_cast", soul::cpp::token::CONST_CAST },
         { U"typeid", soul::cpp::token::TYPEID },
         { U"this", soul::cpp::token::THIS },
         { U"new", soul::cpp::token::NEW },
         { U"delete", soul::cpp::token::DELETE },
         { U"true", soul::cpp::token::TRUE },
         { U"false", soul::cpp::token::FALSE },
         { U"nullptr", soul::cpp::token::NULLPTR },
         { U"case", soul::cpp::token::CASE },
         { U"default", soul::cpp::token::DEFAULT },
         { U"if", soul::cpp::token::IF },
         { U"else", soul::cpp::token::ELSE },
         { U"switch", soul::cpp::token::SWITCH },
         { U"while", soul::cpp::token::WHILE },
         { U"do", soul::cpp::token::DO },
         { U"for", soul::cpp::token::FOR },
         { U"break", soul::cpp::token::BREAK },
         { U"continue", soul::cpp::token::CONTINUE },
         { U"return", soul::cpp::token::RETURN },
         { U"goto", soul::cpp::token::GOTO },
         { U"try", soul::cpp::token::TRY },
         { U"catch", soul::cpp::token::CATCH },
         { U"main", soul::tool::token::MAIN },
         { U"start", soul::tool::token::START },
         { U"empty", soul::tool::token::EMPTY },
         { U"any", soul::tool::token::ANY },
         { U"var", soul::tool::token::VAR },
         { U"project", soul::tool::token::PROJECT },
         { U"tokens", soul::tool::token::TOKENS },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace soul::lex::spg
