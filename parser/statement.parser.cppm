
// this file has been automatically generated from 'D:/work/otava/parser/statement.parser' using soul parser generator spg version 5.2.0

export module otava.parser.statement;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::statement {

template<typename LexerT>
struct StatementParser
{
    static std::unique_ptr<otava::ast::Node> Parse(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Statement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match LabeledStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CompoundStatementUnguarded(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RBraceNext(LexerT& lexer);
    static soul::parser::Match CompoundStatementGuarded(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CompoundStatementSaved(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CompoundStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match SelectionStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match IfStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match SwitchStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match IterationStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match WhileStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DoStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RangeForStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ForRangeDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ForRangeInitializer(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ForStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match JumpStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match BreakStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ContinueStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ReturnStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CoroutineReturnStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match GotoStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ExpressionStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DeclarationStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TryStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match HandlerSequence(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Handler(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ExceptionDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match InitStatement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Condition(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::statement
