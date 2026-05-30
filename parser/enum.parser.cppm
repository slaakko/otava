
// this file has been automatically generated from 'D:/work/otava/parser/enum.parser' using soul parser generator spg version 5.2.0

export module otava.parser.enums;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::enums {

template<typename LexerT>
struct EnumParser
{
    static soul::parser::Match EnumName(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match EnumSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match EnumHead(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match EnumKey(LexerT& lexer);
    static soul::parser::Match EnumHeadName(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match EnumBase(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match EnumeratorList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match EnumeratorDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Enumerator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match OpaqueEnumDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ElaboratedEnumSpecifier(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::enums
