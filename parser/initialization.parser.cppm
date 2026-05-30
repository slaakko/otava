
// this file has been automatically generated from 'D:/work/otava/parser/initialization.parser' using soul parser generator spg version 5.2.0

export module otava.parser.initialization;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::initialization {

template<typename LexerT>
struct InitializationParser
{
    static soul::parser::Match Initializer(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match InitializerClause(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match InitializerList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match DesignatedInitializerList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match DesignatedInitializerClause(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Designator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ExprOrBracedInitList(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match BraceOrEqualInitializer(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match BracedInitList(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::initialization
