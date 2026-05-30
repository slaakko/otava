
// this file has been automatically generated from 'D:/work/otava/parser/attribute.parser' using soul parser generator spg version 5.2.0

export module otava.parser.attribute;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::attribute {

template<typename LexerT>
struct AttributeParser
{
    static soul::parser::Match AttributeSpecifierSeq(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AttributeSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AttributeList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match AttributeUsingPrefix(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AttributeNamespace(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Attribute(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AttributeToken(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AttributeScopedToken(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AttributeArgumentClause(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match BalancedTokenSeq(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match BalancedToken(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AlignmentSpecifier(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::attribute
