
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/common/token_file.parser' using soul parser generator ospg version 5.0.0

export module soul.common.token.file.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.common;

export namespace soul::common::token::file::parser {

template<typename LexerT>
struct TokenFileParser
{
    static std::unique_ptr<soul::ast::common::TokenFile> Parse(LexerT& lexer);
    static soul::parser::Match TokenFile(LexerT& lexer);
    static soul::parser::Match Token(LexerT& lexer);
};

} // namespace soul::common::token::file::parser
