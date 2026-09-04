
// this file has been automatically generated from 'D:/work/otava/projects/compiler/parser/simple_type.parser' using soul parser generator ospg version 0.1.0

export module otava.parser.simple.type;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;

export namespace otava::parser::simple::type {

template<typename LexerT>
struct SimpleTypeParser
{
    static soul::parser::Match SimpleType(LexerT& lexer);
};

} // namespace otava::parser::simple::type
