
// this file has been automatically generated from 'D:/work/otava/parser/punctuation.parser' using soul parser generator spg version 5.2.0

export module otava.parser.punctuation;

import std;
import soul.lexer;
import soul.parser;

export namespace otava::parser::punctuation {

template<typename LexerT>
struct PunctuationParser
{
    static soul::parser::Match Semicolon(LexerT& lexer);
    static soul::parser::Match Comma(LexerT& lexer);
    static soul::parser::Match Quest(LexerT& lexer);
    static soul::parser::Match Colon(LexerT& lexer);
    static soul::parser::Match ColonColon(LexerT& lexer);
    static soul::parser::Match Ellipsis(LexerT& lexer);
    static soul::parser::Match LParen(LexerT& lexer);
    static soul::parser::Match RParen(LexerT& lexer);
    static soul::parser::Match LBracket(LexerT& lexer);
    static soul::parser::Match RBracket(LexerT& lexer);
    static soul::parser::Match LBrace(LexerT& lexer);
    static soul::parser::Match RBrace(LexerT& lexer);
    static soul::parser::Match Assign(LexerT& lexer);
};

} // namespace otava::parser::punctuation
