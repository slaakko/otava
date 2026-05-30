
// this file has been automatically generated from 'D:/work/otava/parser/identifier.parser' using soul parser generator spg version 5.2.0

export module otava.parser.identifier;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;

export namespace otava::parser::identifier {

template<typename LexerT>
struct IdentifierParser
{
    static soul::parser::Match Identifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UnqualifiedId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match QualifiedId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Template(LexerT& lexer);
    static soul::parser::Match NestedNameSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match IdentifierList(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeIdentifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeIdentifierUnchecked(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeIdentifierChecked(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Unnamed(LexerT& lexer);
};

} // namespace otava::parser::identifier
