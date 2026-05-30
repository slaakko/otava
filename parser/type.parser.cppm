
// this file has been automatically generated from 'D:/work/otava/parser/type.parser' using soul parser generator spg version 5.2.0

export module otava.parser.type;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;

export namespace otava::parser::type {

template<typename LexerT>
struct TypeParser
{
    static soul::parser::Match TypeSpecifierSeq(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeSpecifierSeqReset(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DefiningTypeId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DefiningTypeSpecifierSeq(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DefiningTypeSpecifierReset(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DefiningTypeSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ElaboratedTypeSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match SimpleTypeSpecifierReset(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match SimpleTypeSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeName(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypenameSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DeclTypeSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match PlaceholderTypeSpecifier(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::type
