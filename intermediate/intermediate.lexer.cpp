// this file has been automatically generated from 'D:/work/otava/intermediate/intermediate.lexer' using soul lexer generator slg version 5.2.0

module otava.intermediate.lexer;


namespace otava::intermediate::lexer {

soul::ast::common::TokenCollection* GetTokens(otava::intermediate::lexer::Tag tag)
{
    static soul::ast::common::TokenCollection tokens("otava.intermediate.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::TYPES, "TYPES", "'types'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::DATA, "DATA", "'data'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::FUNCTION, "FUNCTION", "'function'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::EXTERN, "EXTERN", "'extern'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::INLINE, "INLINE", "'inline'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::LINK_ONCE, "LINK_ONCE", "'link_once'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::CHILD, "CHILD", "'child'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::METADATA, "METADATA", "'metadata'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::STORE, "STORE", "'store'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::ARG, "ARG", "'arg'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::JMP, "JMP", "'jmp'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::BRANCH, "BRANCH", "'branch'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::CALL, "CALL", "'call'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::RET, "RET", "'ret'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::SWITCH, "SWITCH", "'switch'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::NOP, "NOP", "'nop'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::NOT, "NOT", "'not'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::NEG, "NEG", "'neg'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::SIGNEXTEND, "SIGNEXTEND", "'signextend'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::ZEROEXTEND, "ZEROEXTEND", "'zeroextend'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::FPEXTEND, "FPEXTEND", "'fpextend'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::TRUNCATE, "TRUNCATE", "'truncate'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::BITCAST, "BITCAST", "'bitcast'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::INTTOFLOAT, "INTTOFLOAT", "'inttofloat'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::FLOATTOINT, "FLOATTOINT", "'floattoint'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::INTTOPTR, "INTTOPTR", "'inttoptr'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::PTRTOINT, "PTRTOINT", "'ptrtoint'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::ADD, "ADD", "'add'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::SUB, "SUB", "'sub'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::MUL, "MUL", "'mul'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::DIV, "DIV", "'div'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::MOD, "MOD", "'mod'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::AND, "AND", "'and'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::OR, "OR", "'or'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::XOR, "XOR", "'xor'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::SHL, "SHL", "'shl'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::SHR, "SHR", "'shr'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::EQUAL, "EQUAL", "'equal'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::LESS, "LESS", "'less'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::PARAM, "PARAM", "'param'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::LOCAL, "LOCAL", "'local'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::PLOCAL, "PLOCAL", "'plocal'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::LOAD, "LOAD", "'load'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::ELEMADDR, "ELEMADDR", "'elemaddr'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::PTROFFSET, "PTROFFSET", "'ptroffset'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::PTRDIFF, "PTRDIFF", "'ptrdiff'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::GETRBP, "GETRBP", "'getrbp'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::TYPE, "TYPE", "'type'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::OFFSET, "OFFSET", "'offset'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::SIZE, "SIZE", "'size'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::ALIGNMENT, "ALIGNMENT", "'alignment'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::VOID, "VOID", "'void'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::BOOL, "BOOL", "'bool'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::SBYTE, "SBYTE", "'sbyte'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::BYTE, "BYTE", "'byte'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::SHORT, "SHORT", "'short'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::USHORT, "USHORT", "'ushort'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::INT, "INT", "'int'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::UINT, "UINT", "'uint'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::LONG, "LONG", "'long'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::ULONG, "ULONG", "'ulong'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::FLOAT, "FLOAT", "'float'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::DOUBLE, "DOUBLE", "'double'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::TRUE, "TRUE", "'true'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::FALSE, "FALSE", "'false'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::NULL_TOKEN, "NULL_TOKEN", "'null'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::CONV, "CONV", "'conv'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::LBRACE, "LBRACE", "'{'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::RBRACE, "RBRACE", "'}'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::AST, "AST", "'*'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::ASSIGN, "ASSIGN", "'='"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::AT, "AT", "'@'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::DOLLAR, "DOLLAR", "'$'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::EXCLAMATION, "EXCLAMATION", "'!'"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::TYPEID, "TYPEID", "type identifier"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::CLSID, "CLSID", "class identifier"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::STRING, "STRING", "string"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::NUMBER, "NUMBER", "number"));
        tokens.AddToken(new soul::ast::common::Token(otava::intermediate::token::HEXNUM, "HEXNUM", "hex number"));
    }
    return &tokens;
}

IntermediateLexer_Variables::IntermediateLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>(otava::intermediate::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "types", otava::intermediate::token::TYPES },
         { "data", otava::intermediate::token::DATA },
         { "function", otava::intermediate::token::FUNCTION },
         { "extern", otava::intermediate::token::EXTERN },
         { "inline", otava::intermediate::token::INLINE },
         { "link_once", otava::intermediate::token::LINK_ONCE },
         { "child", otava::intermediate::token::CHILD },
         { "metadata", otava::intermediate::token::METADATA },
         { "store", otava::intermediate::token::STORE },
         { "arg", otava::intermediate::token::ARG },
         { "jmp", otava::intermediate::token::JMP },
         { "branch", otava::intermediate::token::BRANCH },
         { "call", otava::intermediate::token::CALL },
         { "ret", otava::intermediate::token::RET },
         { "switch", otava::intermediate::token::SWITCH },
         { "nop", otava::intermediate::token::NOP },
         { "not", otava::intermediate::token::NOT },
         { "neg", otava::intermediate::token::NEG },
         { "signextend", otava::intermediate::token::SIGNEXTEND },
         { "zeroextend", otava::intermediate::token::ZEROEXTEND },
         { "fpextend", otava::intermediate::token::FPEXTEND },
         { "truncate", otava::intermediate::token::TRUNCATE },
         { "bitcast", otava::intermediate::token::BITCAST },
         { "inttofloat", otava::intermediate::token::INTTOFLOAT },
         { "floattoint", otava::intermediate::token::FLOATTOINT },
         { "inttoptr", otava::intermediate::token::INTTOPTR },
         { "ptrtoint", otava::intermediate::token::PTRTOINT },
         { "add", otava::intermediate::token::ADD },
         { "sub", otava::intermediate::token::SUB },
         { "mul", otava::intermediate::token::MUL },
         { "div", otava::intermediate::token::DIV },
         { "mod", otava::intermediate::token::MOD },
         { "and", otava::intermediate::token::AND },
         { "or", otava::intermediate::token::OR },
         { "xor", otava::intermediate::token::XOR },
         { "shl", otava::intermediate::token::SHL },
         { "shr", otava::intermediate::token::SHR },
         { "equal", otava::intermediate::token::EQUAL },
         { "less", otava::intermediate::token::LESS },
         { "param", otava::intermediate::token::PARAM },
         { "local", otava::intermediate::token::LOCAL },
         { "plocal", otava::intermediate::token::PLOCAL },
         { "load", otava::intermediate::token::LOAD },
         { "elemaddr", otava::intermediate::token::ELEMADDR },
         { "ptroffset", otava::intermediate::token::PTROFFSET },
         { "ptrdiff", otava::intermediate::token::PTRDIFF },
         { "getrbp", otava::intermediate::token::GETRBP },
         { "type", otava::intermediate::token::TYPE },
         { "offset", otava::intermediate::token::OFFSET },
         { "size", otava::intermediate::token::SIZE },
         { "alignment", otava::intermediate::token::ALIGNMENT },
         { "void", otava::intermediate::token::VOID },
         { "bool", otava::intermediate::token::BOOL },
         { "sbyte", otava::intermediate::token::SBYTE },
         { "byte", otava::intermediate::token::BYTE },
         { "short", otava::intermediate::token::SHORT },
         { "ushort", otava::intermediate::token::USHORT },
         { "int", otava::intermediate::token::INT },
         { "uint", otava::intermediate::token::UINT },
         { "long", otava::intermediate::token::LONG },
         { "ulong", otava::intermediate::token::ULONG },
         { "float", otava::intermediate::token::FLOAT },
         { "double", otava::intermediate::token::DOUBLE },
         { "true", otava::intermediate::token::TRUE },
         { "false", otava::intermediate::token::FALSE },
         { "null", otava::intermediate::token::NULL_TOKEN },
         { "conv", otava::intermediate::token::CONV },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>(otava::intermediate::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"types", otava::intermediate::token::TYPES },
         { u8"data", otava::intermediate::token::DATA },
         { u8"function", otava::intermediate::token::FUNCTION },
         { u8"extern", otava::intermediate::token::EXTERN },
         { u8"inline", otava::intermediate::token::INLINE },
         { u8"link_once", otava::intermediate::token::LINK_ONCE },
         { u8"child", otava::intermediate::token::CHILD },
         { u8"metadata", otava::intermediate::token::METADATA },
         { u8"store", otava::intermediate::token::STORE },
         { u8"arg", otava::intermediate::token::ARG },
         { u8"jmp", otava::intermediate::token::JMP },
         { u8"branch", otava::intermediate::token::BRANCH },
         { u8"call", otava::intermediate::token::CALL },
         { u8"ret", otava::intermediate::token::RET },
         { u8"switch", otava::intermediate::token::SWITCH },
         { u8"nop", otava::intermediate::token::NOP },
         { u8"not", otava::intermediate::token::NOT },
         { u8"neg", otava::intermediate::token::NEG },
         { u8"signextend", otava::intermediate::token::SIGNEXTEND },
         { u8"zeroextend", otava::intermediate::token::ZEROEXTEND },
         { u8"fpextend", otava::intermediate::token::FPEXTEND },
         { u8"truncate", otava::intermediate::token::TRUNCATE },
         { u8"bitcast", otava::intermediate::token::BITCAST },
         { u8"inttofloat", otava::intermediate::token::INTTOFLOAT },
         { u8"floattoint", otava::intermediate::token::FLOATTOINT },
         { u8"inttoptr", otava::intermediate::token::INTTOPTR },
         { u8"ptrtoint", otava::intermediate::token::PTRTOINT },
         { u8"add", otava::intermediate::token::ADD },
         { u8"sub", otava::intermediate::token::SUB },
         { u8"mul", otava::intermediate::token::MUL },
         { u8"div", otava::intermediate::token::DIV },
         { u8"mod", otava::intermediate::token::MOD },
         { u8"and", otava::intermediate::token::AND },
         { u8"or", otava::intermediate::token::OR },
         { u8"xor", otava::intermediate::token::XOR },
         { u8"shl", otava::intermediate::token::SHL },
         { u8"shr", otava::intermediate::token::SHR },
         { u8"equal", otava::intermediate::token::EQUAL },
         { u8"less", otava::intermediate::token::LESS },
         { u8"param", otava::intermediate::token::PARAM },
         { u8"local", otava::intermediate::token::LOCAL },
         { u8"plocal", otava::intermediate::token::PLOCAL },
         { u8"load", otava::intermediate::token::LOAD },
         { u8"elemaddr", otava::intermediate::token::ELEMADDR },
         { u8"ptroffset", otava::intermediate::token::PTROFFSET },
         { u8"ptrdiff", otava::intermediate::token::PTRDIFF },
         { u8"getrbp", otava::intermediate::token::GETRBP },
         { u8"type", otava::intermediate::token::TYPE },
         { u8"offset", otava::intermediate::token::OFFSET },
         { u8"size", otava::intermediate::token::SIZE },
         { u8"alignment", otava::intermediate::token::ALIGNMENT },
         { u8"void", otava::intermediate::token::VOID },
         { u8"bool", otava::intermediate::token::BOOL },
         { u8"sbyte", otava::intermediate::token::SBYTE },
         { u8"byte", otava::intermediate::token::BYTE },
         { u8"short", otava::intermediate::token::SHORT },
         { u8"ushort", otava::intermediate::token::USHORT },
         { u8"int", otava::intermediate::token::INT },
         { u8"uint", otava::intermediate::token::UINT },
         { u8"long", otava::intermediate::token::LONG },
         { u8"ulong", otava::intermediate::token::ULONG },
         { u8"float", otava::intermediate::token::FLOAT },
         { u8"double", otava::intermediate::token::DOUBLE },
         { u8"true", otava::intermediate::token::TRUE },
         { u8"false", otava::intermediate::token::FALSE },
         { u8"null", otava::intermediate::token::NULL_TOKEN },
         { u8"conv", otava::intermediate::token::CONV },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>(otava::intermediate::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"types", otava::intermediate::token::TYPES },
         { u"data", otava::intermediate::token::DATA },
         { u"function", otava::intermediate::token::FUNCTION },
         { u"extern", otava::intermediate::token::EXTERN },
         { u"inline", otava::intermediate::token::INLINE },
         { u"link_once", otava::intermediate::token::LINK_ONCE },
         { u"child", otava::intermediate::token::CHILD },
         { u"metadata", otava::intermediate::token::METADATA },
         { u"store", otava::intermediate::token::STORE },
         { u"arg", otava::intermediate::token::ARG },
         { u"jmp", otava::intermediate::token::JMP },
         { u"branch", otava::intermediate::token::BRANCH },
         { u"call", otava::intermediate::token::CALL },
         { u"ret", otava::intermediate::token::RET },
         { u"switch", otava::intermediate::token::SWITCH },
         { u"nop", otava::intermediate::token::NOP },
         { u"not", otava::intermediate::token::NOT },
         { u"neg", otava::intermediate::token::NEG },
         { u"signextend", otava::intermediate::token::SIGNEXTEND },
         { u"zeroextend", otava::intermediate::token::ZEROEXTEND },
         { u"fpextend", otava::intermediate::token::FPEXTEND },
         { u"truncate", otava::intermediate::token::TRUNCATE },
         { u"bitcast", otava::intermediate::token::BITCAST },
         { u"inttofloat", otava::intermediate::token::INTTOFLOAT },
         { u"floattoint", otava::intermediate::token::FLOATTOINT },
         { u"inttoptr", otava::intermediate::token::INTTOPTR },
         { u"ptrtoint", otava::intermediate::token::PTRTOINT },
         { u"add", otava::intermediate::token::ADD },
         { u"sub", otava::intermediate::token::SUB },
         { u"mul", otava::intermediate::token::MUL },
         { u"div", otava::intermediate::token::DIV },
         { u"mod", otava::intermediate::token::MOD },
         { u"and", otava::intermediate::token::AND },
         { u"or", otava::intermediate::token::OR },
         { u"xor", otava::intermediate::token::XOR },
         { u"shl", otava::intermediate::token::SHL },
         { u"shr", otava::intermediate::token::SHR },
         { u"equal", otava::intermediate::token::EQUAL },
         { u"less", otava::intermediate::token::LESS },
         { u"param", otava::intermediate::token::PARAM },
         { u"local", otava::intermediate::token::LOCAL },
         { u"plocal", otava::intermediate::token::PLOCAL },
         { u"load", otava::intermediate::token::LOAD },
         { u"elemaddr", otava::intermediate::token::ELEMADDR },
         { u"ptroffset", otava::intermediate::token::PTROFFSET },
         { u"ptrdiff", otava::intermediate::token::PTRDIFF },
         { u"getrbp", otava::intermediate::token::GETRBP },
         { u"type", otava::intermediate::token::TYPE },
         { u"offset", otava::intermediate::token::OFFSET },
         { u"size", otava::intermediate::token::SIZE },
         { u"alignment", otava::intermediate::token::ALIGNMENT },
         { u"void", otava::intermediate::token::VOID },
         { u"bool", otava::intermediate::token::BOOL },
         { u"sbyte", otava::intermediate::token::SBYTE },
         { u"byte", otava::intermediate::token::BYTE },
         { u"short", otava::intermediate::token::SHORT },
         { u"ushort", otava::intermediate::token::USHORT },
         { u"int", otava::intermediate::token::INT },
         { u"uint", otava::intermediate::token::UINT },
         { u"long", otava::intermediate::token::LONG },
         { u"ulong", otava::intermediate::token::ULONG },
         { u"float", otava::intermediate::token::FLOAT },
         { u"double", otava::intermediate::token::DOUBLE },
         { u"true", otava::intermediate::token::TRUE },
         { u"false", otava::intermediate::token::FALSE },
         { u"null", otava::intermediate::token::NULL_TOKEN },
         { u"conv", otava::intermediate::token::CONV },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>(otava::intermediate::lexer::Tag tag)
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"types", otava::intermediate::token::TYPES },
         { U"data", otava::intermediate::token::DATA },
         { U"function", otava::intermediate::token::FUNCTION },
         { U"extern", otava::intermediate::token::EXTERN },
         { U"inline", otava::intermediate::token::INLINE },
         { U"link_once", otava::intermediate::token::LINK_ONCE },
         { U"child", otava::intermediate::token::CHILD },
         { U"metadata", otava::intermediate::token::METADATA },
         { U"store", otava::intermediate::token::STORE },
         { U"arg", otava::intermediate::token::ARG },
         { U"jmp", otava::intermediate::token::JMP },
         { U"branch", otava::intermediate::token::BRANCH },
         { U"call", otava::intermediate::token::CALL },
         { U"ret", otava::intermediate::token::RET },
         { U"switch", otava::intermediate::token::SWITCH },
         { U"nop", otava::intermediate::token::NOP },
         { U"not", otava::intermediate::token::NOT },
         { U"neg", otava::intermediate::token::NEG },
         { U"signextend", otava::intermediate::token::SIGNEXTEND },
         { U"zeroextend", otava::intermediate::token::ZEROEXTEND },
         { U"fpextend", otava::intermediate::token::FPEXTEND },
         { U"truncate", otava::intermediate::token::TRUNCATE },
         { U"bitcast", otava::intermediate::token::BITCAST },
         { U"inttofloat", otava::intermediate::token::INTTOFLOAT },
         { U"floattoint", otava::intermediate::token::FLOATTOINT },
         { U"inttoptr", otava::intermediate::token::INTTOPTR },
         { U"ptrtoint", otava::intermediate::token::PTRTOINT },
         { U"add", otava::intermediate::token::ADD },
         { U"sub", otava::intermediate::token::SUB },
         { U"mul", otava::intermediate::token::MUL },
         { U"div", otava::intermediate::token::DIV },
         { U"mod", otava::intermediate::token::MOD },
         { U"and", otava::intermediate::token::AND },
         { U"or", otava::intermediate::token::OR },
         { U"xor", otava::intermediate::token::XOR },
         { U"shl", otava::intermediate::token::SHL },
         { U"shr", otava::intermediate::token::SHR },
         { U"equal", otava::intermediate::token::EQUAL },
         { U"less", otava::intermediate::token::LESS },
         { U"param", otava::intermediate::token::PARAM },
         { U"local", otava::intermediate::token::LOCAL },
         { U"plocal", otava::intermediate::token::PLOCAL },
         { U"load", otava::intermediate::token::LOAD },
         { U"elemaddr", otava::intermediate::token::ELEMADDR },
         { U"ptroffset", otava::intermediate::token::PTROFFSET },
         { U"ptrdiff", otava::intermediate::token::PTRDIFF },
         { U"getrbp", otava::intermediate::token::GETRBP },
         { U"type", otava::intermediate::token::TYPE },
         { U"offset", otava::intermediate::token::OFFSET },
         { U"size", otava::intermediate::token::SIZE },
         { U"alignment", otava::intermediate::token::ALIGNMENT },
         { U"void", otava::intermediate::token::VOID },
         { U"bool", otava::intermediate::token::BOOL },
         { U"sbyte", otava::intermediate::token::SBYTE },
         { U"byte", otava::intermediate::token::BYTE },
         { U"short", otava::intermediate::token::SHORT },
         { U"ushort", otava::intermediate::token::USHORT },
         { U"int", otava::intermediate::token::INT },
         { U"uint", otava::intermediate::token::UINT },
         { U"long", otava::intermediate::token::LONG },
         { U"ulong", otava::intermediate::token::ULONG },
         { U"float", otava::intermediate::token::FLOAT },
         { U"double", otava::intermediate::token::DOUBLE },
         { U"true", otava::intermediate::token::TRUE },
         { U"false", otava::intermediate::token::FALSE },
         { U"null", otava::intermediate::token::NULL_TOKEN },
         { U"conv", otava::intermediate::token::CONV },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace otava::intermediate::lexer
