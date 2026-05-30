
// this file has been automatically generated from 'D:/work/otava/parser/expression.parser' using soul parser generator spg version 5.2.0

export module otava.parser.expression;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::expression {

template<typename LexerT>
struct ExpressionParser
{
    static std::unique_ptr<otava::ast::Node> Parse(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Expression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ConstantExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AssignmentExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AssignmentOp(LexerT& lexer);
    static soul::parser::Match ConditionalExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match YieldExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match LogicalOrExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match LogicalAndExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match InclusiveOrExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ExclusiveOrExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AndExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match EqualityExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match EqualityOp(LexerT& lexer);
    static soul::parser::Match RelationalExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RelationalOp(LexerT& lexer, bool rejectRAngle);
    static soul::parser::Match CompareExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ShiftExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ShiftOp(LexerT& lexer);
    static soul::parser::Match AdditiveExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AdditiveOp(LexerT& lexer);
    static soul::parser::Match MultiplicativeExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MultiplicativeOp(LexerT& lexer);
    static soul::parser::Match PmExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match PmOp(LexerT& lexer);
    static soul::parser::Match CastExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UnaryExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UnaryOp(LexerT& lexer);
    static soul::parser::Match AwaitExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match SizeOfExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AlignOfExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NoexceptEpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NewExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NewPlacement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NewInitializer(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NewTypeId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NewDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NoPtrNewDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DeleteExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match PostfixExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CppCastExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeIdExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match IntrinsicInvokeExpr(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match PrimaryExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match FoldExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match FoldOp(LexerT& lexer);
    static soul::parser::Match ParenthesizedExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DotOp(LexerT& lexer);
    static soul::parser::Match ArrowOp(LexerT& lexer);
    static soul::parser::Match IdExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match IdExpr(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ExpressionList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match ThrowExpression(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::expression
