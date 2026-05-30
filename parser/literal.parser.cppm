
// this file has been automatically generated from 'D:/work/otava/parser/literal.parser' using soul parser generator spg version 5.2.0

export module otava.parser.literal;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;

export namespace otava::parser::literal {

template<typename LexerT>
struct LiteralParser
{
    static soul::parser::Match Literal(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UserDefinedLiteral(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UdSuffix(LexerT& lexer);
    static soul::parser::Match LiteralOperatorId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match StringLiteral(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::literal
