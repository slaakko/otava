
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/cpp/statement.parser' using soul parser generator ospg version 5.0.0

export module soul.cpp.statement.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.cpp;
import soul.ast.common;

export namespace soul::cpp::statement::parser {

template<typename LexerT>
struct StatementParser
{
    static soul::parser::Match Statement(LexerT& lexer);
    static soul::parser::Match LabeledStatement(LexerT& lexer);
    static soul::parser::Match EmptyStatement(LexerT& lexer);
    static soul::parser::Match CompoundStatement(LexerT& lexer);
    static soul::parser::Match SelectionStatement(LexerT& lexer);
    static soul::parser::Match IfStatement(LexerT& lexer);
    static soul::parser::Match SwitchStatement(LexerT& lexer);
    static soul::parser::Match IterationStatement(LexerT& lexer);
    static soul::parser::Match WhileStatement(LexerT& lexer);
    static soul::parser::Match DoStatement(LexerT& lexer);
    static soul::parser::Match RangeForStatement(LexerT& lexer);
    static soul::parser::Match ForRangeDeclaration(LexerT& lexer);
    static soul::parser::Match ForStatement(LexerT& lexer);
    static soul::parser::Match ForInitStatement(LexerT& lexer);
    static soul::parser::Match JumpStatement(LexerT& lexer);
    static soul::parser::Match BreakStatement(LexerT& lexer);
    static soul::parser::Match ContinueStatement(LexerT& lexer);
    static soul::parser::Match ReturnStatement(LexerT& lexer);
    static soul::parser::Match GotoStatement(LexerT& lexer);
    static soul::parser::Match DeclarationStatement(LexerT& lexer);
    static soul::parser::Match Condition(LexerT& lexer);
    static soul::parser::Match ExpressionStatement(LexerT& lexer);
};

} // namespace soul::cpp::statement::parser
