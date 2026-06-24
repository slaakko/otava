
// this file has been automatically generated from 'D:/work/otava/pp/pp.parser' using soul parser generator spg version 5.2.0

export module otava.pp.parser;

import std;
import soul.lexer;
import soul.parser;
import otava.pp.state;

export namespace otava::pp::parser {

template<typename LexerT>
struct PPParser
{
    static void Parse(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match PPLIne(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match PPCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match IfCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match ElifCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match IfdefCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match IfndefCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match ElseCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match EndifCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match DefineCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match UndefCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match IncludeCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match PragmaCommand(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match Header(LexerT& lexer);
    static soul::parser::Match Expression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match LogicalOrExpr(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match LogicalAndExpr(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match InclusiveOrExpression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match ExclusiveOrExpression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match AndExpression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match EqualityExpression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match RelationalExpression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match ShiftExpression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match AdditiveExpression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match MultiplicativeExpression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match UnaryExpression(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match PrimaryExpr(LexerT& lexer, otava::pp::state::State* state);
    static soul::parser::Match DefinedExpr(LexerT& lexer, otava::pp::state::State* state);
};

} // namespace otava::pp::parser
