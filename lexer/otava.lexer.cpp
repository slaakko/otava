// this file has been automatically generated from 'D:/work/otava/lexer/otava.lexer' using soul lexer generator slg version 5.2.0

module otava.lexer;


namespace otava::lexer {

soul::ast::common::TokenCollection* GetTokens(otava::lexer::Tag tag)
{
    static soul::ast::common::TokenCollection tokens("otava.lexer.tokens");
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

OtavaLexer_Variables::OtavaLexer_Variables() :
    langleCount()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>(otava::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "alignas", otava::token::ALIGNAS },
         { "alignof", otava::token::ALIGNOF },
         { "asm", otava::token::ASM },
         { "auto", otava::token::AUTO },
         { "bool", otava::token::BOOL },
         { "break", otava::token::BREAK },
         { "case", otava::token::CASE },
         { "catch", otava::token::CATCH },
         { "char", otava::token::CHAR },
         { "char8_t", otava::token::CHAR8_T },
         { "char16_t", otava::token::CHAR16_T },
         { "char32_t", otava::token::CHAR32_T },
         { "class", otava::token::CLASS },
         { "concept", otava::token::CONCEPT },
         { "const", otava::token::CONST },
         { "consteval", otava::token::CONSTEVAL },
         { "constexpr", otava::token::CONSTEXPR },
         { "constinit", otava::token::CONSTINIT },
         { "const_cast", otava::token::CONST_CAST },
         { "continue", otava::token::CONTINUE },
         { "co_await", otava::token::CO_AWAIT },
         { "co_return", otava::token::CO_RETURN },
         { "co_yield", otava::token::CO_YIELD },
         { "decltype", otava::token::DECLTYPE },
         { "default", otava::token::DEFAULT },
         { "delete", otava::token::DELETE },
         { "do", otava::token::DO },
         { "double", otava::token::DOUBLE },
         { "dynamic_cast", otava::token::DYNAMIC_CAST },
         { "else", otava::token::ELSE },
         { "enum", otava::token::ENUM },
         { "explicit", otava::token::EXPLICIT },
         { "extern", otava::token::EXTERN },
         { "false", otava::token::FALSE },
         { "float", otava::token::FLOAT },
         { "for", otava::token::FOR },
         { "friend", otava::token::FRIEND },
         { "goto", otava::token::GOTO },
         { "if", otava::token::IF },
         { "inline", otava::token::INLINE },
         { "int", otava::token::INT },
         { "long", otava::token::LONG },
         { "mutable", otava::token::MUTABLE },
         { "namespace", otava::token::NAMESPACE },
         { "new", otava::token::NEW },
         { "noexcept", otava::token::NOEXCEPT },
         { "nullptr", otava::token::NULLPTR },
         { "operator", otava::token::OPERATOR },
         { "private", otava::token::PRIVATE },
         { "protected", otava::token::PROTECTED },
         { "public", otava::token::PUBLIC },
         { "register", otava::token::REGISTER },
         { "reinterpret_cast", otava::token::REINTERPRET_CAST },
         { "requires", otava::token::REQUIRES },
         { "return", otava::token::RETURN },
         { "short", otava::token::SHORT },
         { "signed", otava::token::SIGNED },
         { "sizeof", otava::token::SIZEOF },
         { "static", otava::token::STATIC },
         { "static_assert", otava::token::STATIC_ASSERT },
         { "static_cast", otava::token::STATIC_CAST },
         { "struct", otava::token::STRUCT },
         { "switch", otava::token::SWITCH },
         { "template", otava::token::TEMPLATE },
         { "this", otava::token::THIS },
         { "thread_local", otava::token::THREAD_LOCAL },
         { "throw", otava::token::THROW },
         { "true", otava::token::TRUE },
         { "try", otava::token::TRY },
         { "typedef", otava::token::TYPEDEF },
         { "typeid", otava::token::TYPEID },
         { "typename", otava::token::TYPENAME },
         { "union", otava::token::UNION },
         { "unsigned", otava::token::UNSIGNED },
         { "using", otava::token::USING },
         { "virtual", otava::token::VIRTUAL },
         { "void", otava::token::VOID },
         { "volatile", otava::token::VOLATILE },
         { "wchar_t", otava::token::WCHAR_T },
         { "while", otava::token::WHILE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>(otava::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"alignas", otava::token::ALIGNAS },
         { u8"alignof", otava::token::ALIGNOF },
         { u8"asm", otava::token::ASM },
         { u8"auto", otava::token::AUTO },
         { u8"bool", otava::token::BOOL },
         { u8"break", otava::token::BREAK },
         { u8"case", otava::token::CASE },
         { u8"catch", otava::token::CATCH },
         { u8"char", otava::token::CHAR },
         { u8"char8_t", otava::token::CHAR8_T },
         { u8"char16_t", otava::token::CHAR16_T },
         { u8"char32_t", otava::token::CHAR32_T },
         { u8"class", otava::token::CLASS },
         { u8"concept", otava::token::CONCEPT },
         { u8"const", otava::token::CONST },
         { u8"consteval", otava::token::CONSTEVAL },
         { u8"constexpr", otava::token::CONSTEXPR },
         { u8"constinit", otava::token::CONSTINIT },
         { u8"const_cast", otava::token::CONST_CAST },
         { u8"continue", otava::token::CONTINUE },
         { u8"co_await", otava::token::CO_AWAIT },
         { u8"co_return", otava::token::CO_RETURN },
         { u8"co_yield", otava::token::CO_YIELD },
         { u8"decltype", otava::token::DECLTYPE },
         { u8"default", otava::token::DEFAULT },
         { u8"delete", otava::token::DELETE },
         { u8"do", otava::token::DO },
         { u8"double", otava::token::DOUBLE },
         { u8"dynamic_cast", otava::token::DYNAMIC_CAST },
         { u8"else", otava::token::ELSE },
         { u8"enum", otava::token::ENUM },
         { u8"explicit", otava::token::EXPLICIT },
         { u8"extern", otava::token::EXTERN },
         { u8"false", otava::token::FALSE },
         { u8"float", otava::token::FLOAT },
         { u8"for", otava::token::FOR },
         { u8"friend", otava::token::FRIEND },
         { u8"goto", otava::token::GOTO },
         { u8"if", otava::token::IF },
         { u8"inline", otava::token::INLINE },
         { u8"int", otava::token::INT },
         { u8"long", otava::token::LONG },
         { u8"mutable", otava::token::MUTABLE },
         { u8"namespace", otava::token::NAMESPACE },
         { u8"new", otava::token::NEW },
         { u8"noexcept", otava::token::NOEXCEPT },
         { u8"nullptr", otava::token::NULLPTR },
         { u8"operator", otava::token::OPERATOR },
         { u8"private", otava::token::PRIVATE },
         { u8"protected", otava::token::PROTECTED },
         { u8"public", otava::token::PUBLIC },
         { u8"register", otava::token::REGISTER },
         { u8"reinterpret_cast", otava::token::REINTERPRET_CAST },
         { u8"requires", otava::token::REQUIRES },
         { u8"return", otava::token::RETURN },
         { u8"short", otava::token::SHORT },
         { u8"signed", otava::token::SIGNED },
         { u8"sizeof", otava::token::SIZEOF },
         { u8"static", otava::token::STATIC },
         { u8"static_assert", otava::token::STATIC_ASSERT },
         { u8"static_cast", otava::token::STATIC_CAST },
         { u8"struct", otava::token::STRUCT },
         { u8"switch", otava::token::SWITCH },
         { u8"template", otava::token::TEMPLATE },
         { u8"this", otava::token::THIS },
         { u8"thread_local", otava::token::THREAD_LOCAL },
         { u8"throw", otava::token::THROW },
         { u8"true", otava::token::TRUE },
         { u8"try", otava::token::TRY },
         { u8"typedef", otava::token::TYPEDEF },
         { u8"typeid", otava::token::TYPEID },
         { u8"typename", otava::token::TYPENAME },
         { u8"union", otava::token::UNION },
         { u8"unsigned", otava::token::UNSIGNED },
         { u8"using", otava::token::USING },
         { u8"virtual", otava::token::VIRTUAL },
         { u8"void", otava::token::VOID },
         { u8"volatile", otava::token::VOLATILE },
         { u8"wchar_t", otava::token::WCHAR_T },
         { u8"while", otava::token::WHILE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>(otava::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"alignas", otava::token::ALIGNAS },
         { u"alignof", otava::token::ALIGNOF },
         { u"asm", otava::token::ASM },
         { u"auto", otava::token::AUTO },
         { u"bool", otava::token::BOOL },
         { u"break", otava::token::BREAK },
         { u"case", otava::token::CASE },
         { u"catch", otava::token::CATCH },
         { u"char", otava::token::CHAR },
         { u"char8_t", otava::token::CHAR8_T },
         { u"char16_t", otava::token::CHAR16_T },
         { u"char32_t", otava::token::CHAR32_T },
         { u"class", otava::token::CLASS },
         { u"concept", otava::token::CONCEPT },
         { u"const", otava::token::CONST },
         { u"consteval", otava::token::CONSTEVAL },
         { u"constexpr", otava::token::CONSTEXPR },
         { u"constinit", otava::token::CONSTINIT },
         { u"const_cast", otava::token::CONST_CAST },
         { u"continue", otava::token::CONTINUE },
         { u"co_await", otava::token::CO_AWAIT },
         { u"co_return", otava::token::CO_RETURN },
         { u"co_yield", otava::token::CO_YIELD },
         { u"decltype", otava::token::DECLTYPE },
         { u"default", otava::token::DEFAULT },
         { u"delete", otava::token::DELETE },
         { u"do", otava::token::DO },
         { u"double", otava::token::DOUBLE },
         { u"dynamic_cast", otava::token::DYNAMIC_CAST },
         { u"else", otava::token::ELSE },
         { u"enum", otava::token::ENUM },
         { u"explicit", otava::token::EXPLICIT },
         { u"extern", otava::token::EXTERN },
         { u"false", otava::token::FALSE },
         { u"float", otava::token::FLOAT },
         { u"for", otava::token::FOR },
         { u"friend", otava::token::FRIEND },
         { u"goto", otava::token::GOTO },
         { u"if", otava::token::IF },
         { u"inline", otava::token::INLINE },
         { u"int", otava::token::INT },
         { u"long", otava::token::LONG },
         { u"mutable", otava::token::MUTABLE },
         { u"namespace", otava::token::NAMESPACE },
         { u"new", otava::token::NEW },
         { u"noexcept", otava::token::NOEXCEPT },
         { u"nullptr", otava::token::NULLPTR },
         { u"operator", otava::token::OPERATOR },
         { u"private", otava::token::PRIVATE },
         { u"protected", otava::token::PROTECTED },
         { u"public", otava::token::PUBLIC },
         { u"register", otava::token::REGISTER },
         { u"reinterpret_cast", otava::token::REINTERPRET_CAST },
         { u"requires", otava::token::REQUIRES },
         { u"return", otava::token::RETURN },
         { u"short", otava::token::SHORT },
         { u"signed", otava::token::SIGNED },
         { u"sizeof", otava::token::SIZEOF },
         { u"static", otava::token::STATIC },
         { u"static_assert", otava::token::STATIC_ASSERT },
         { u"static_cast", otava::token::STATIC_CAST },
         { u"struct", otava::token::STRUCT },
         { u"switch", otava::token::SWITCH },
         { u"template", otava::token::TEMPLATE },
         { u"this", otava::token::THIS },
         { u"thread_local", otava::token::THREAD_LOCAL },
         { u"throw", otava::token::THROW },
         { u"true", otava::token::TRUE },
         { u"try", otava::token::TRY },
         { u"typedef", otava::token::TYPEDEF },
         { u"typeid", otava::token::TYPEID },
         { u"typename", otava::token::TYPENAME },
         { u"union", otava::token::UNION },
         { u"unsigned", otava::token::UNSIGNED },
         { u"using", otava::token::USING },
         { u"virtual", otava::token::VIRTUAL },
         { u"void", otava::token::VOID },
         { u"volatile", otava::token::VOLATILE },
         { u"wchar_t", otava::token::WCHAR_T },
         { u"while", otava::token::WHILE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>(otava::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"alignas", otava::token::ALIGNAS },
         { U"alignof", otava::token::ALIGNOF },
         { U"asm", otava::token::ASM },
         { U"auto", otava::token::AUTO },
         { U"bool", otava::token::BOOL },
         { U"break", otava::token::BREAK },
         { U"case", otava::token::CASE },
         { U"catch", otava::token::CATCH },
         { U"char", otava::token::CHAR },
         { U"char8_t", otava::token::CHAR8_T },
         { U"char16_t", otava::token::CHAR16_T },
         { U"char32_t", otava::token::CHAR32_T },
         { U"class", otava::token::CLASS },
         { U"concept", otava::token::CONCEPT },
         { U"const", otava::token::CONST },
         { U"consteval", otava::token::CONSTEVAL },
         { U"constexpr", otava::token::CONSTEXPR },
         { U"constinit", otava::token::CONSTINIT },
         { U"const_cast", otava::token::CONST_CAST },
         { U"continue", otava::token::CONTINUE },
         { U"co_await", otava::token::CO_AWAIT },
         { U"co_return", otava::token::CO_RETURN },
         { U"co_yield", otava::token::CO_YIELD },
         { U"decltype", otava::token::DECLTYPE },
         { U"default", otava::token::DEFAULT },
         { U"delete", otava::token::DELETE },
         { U"do", otava::token::DO },
         { U"double", otava::token::DOUBLE },
         { U"dynamic_cast", otava::token::DYNAMIC_CAST },
         { U"else", otava::token::ELSE },
         { U"enum", otava::token::ENUM },
         { U"explicit", otava::token::EXPLICIT },
         { U"extern", otava::token::EXTERN },
         { U"false", otava::token::FALSE },
         { U"float", otava::token::FLOAT },
         { U"for", otava::token::FOR },
         { U"friend", otava::token::FRIEND },
         { U"goto", otava::token::GOTO },
         { U"if", otava::token::IF },
         { U"inline", otava::token::INLINE },
         { U"int", otava::token::INT },
         { U"long", otava::token::LONG },
         { U"mutable", otava::token::MUTABLE },
         { U"namespace", otava::token::NAMESPACE },
         { U"new", otava::token::NEW },
         { U"noexcept", otava::token::NOEXCEPT },
         { U"nullptr", otava::token::NULLPTR },
         { U"operator", otava::token::OPERATOR },
         { U"private", otava::token::PRIVATE },
         { U"protected", otava::token::PROTECTED },
         { U"public", otava::token::PUBLIC },
         { U"register", otava::token::REGISTER },
         { U"reinterpret_cast", otava::token::REINTERPRET_CAST },
         { U"requires", otava::token::REQUIRES },
         { U"return", otava::token::RETURN },
         { U"short", otava::token::SHORT },
         { U"signed", otava::token::SIGNED },
         { U"sizeof", otava::token::SIZEOF },
         { U"static", otava::token::STATIC },
         { U"static_assert", otava::token::STATIC_ASSERT },
         { U"static_cast", otava::token::STATIC_CAST },
         { U"struct", otava::token::STRUCT },
         { U"switch", otava::token::SWITCH },
         { U"template", otava::token::TEMPLATE },
         { U"this", otava::token::THIS },
         { U"thread_local", otava::token::THREAD_LOCAL },
         { U"throw", otava::token::THROW },
         { U"true", otava::token::TRUE },
         { U"try", otava::token::TRY },
         { U"typedef", otava::token::TYPEDEF },
         { U"typeid", otava::token::TYPEID },
         { U"typename", otava::token::TYPENAME },
         { U"union", otava::token::UNION },
         { U"unsigned", otava::token::UNSIGNED },
         { U"using", otava::token::USING },
         { U"virtual", otava::token::VIRTUAL },
         { U"void", otava::token::VOID },
         { U"volatile", otava::token::VOLATILE },
         { U"wchar_t", otava::token::WCHAR_T },
         { U"while", otava::token::WHILE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace otava::lexer
