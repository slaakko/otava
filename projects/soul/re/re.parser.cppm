
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/re/re.parser' using soul parser generator ospg version 5.0.0

export module soul.re.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.re;

export namespace soul::re::parser {

template<typename LexerT>
struct RegExParser
{
    static soul::ast::re::Nfa Parse(LexerT& lexer, soul::ast::re::LexerContext* lexerContext);
    static soul::parser::Match RegularExpression(LexerT& lexer, soul::ast::re::LexerContext* lexerContext);
    static soul::parser::Match Alternative(LexerT& lexer, soul::ast::re::LexerContext* lexerContext);
    static soul::parser::Match Catenation(LexerT& lexer, soul::ast::re::LexerContext* lexerContext);
    static soul::parser::Match Repetition(LexerT& lexer, soul::ast::re::LexerContext* lexerContext);
    static soul::parser::Match Primary(LexerT& lexer, soul::ast::re::LexerContext* lexerContext);
    static soul::parser::Match Class(LexerT& lexer, soul::ast::re::LexerContext* lexerContext);
    static soul::parser::Match Range(LexerT& lexer, soul::ast::re::LexerContext* lexerContext);
    static soul::parser::Match Char(LexerT& lexer);
    static soul::parser::Match ExpressionReference(LexerT& lexer);
};

} // namespace soul::re::parser
