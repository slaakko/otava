
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/cpp/expression.parser' using soul parser generator ospg version 5.0.0

export module soul.cpp.expression.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.cpp;
import soul.ast.common;

export namespace soul::cpp::expression::parser {

template<typename LexerT>
struct ExpressionParser
{
    static soul::parser::Match Expression(LexerT& lexer);
    static soul::parser::Match ConstantExpression(LexerT& lexer);
    static soul::parser::Match AssignmentExpression(LexerT& lexer);
    static soul::parser::Match AssignmentOp(LexerT& lexer);
    static soul::parser::Match ConditionalExpression(LexerT& lexer);
    static soul::parser::Match LogicalOrExpression(LexerT& lexer);
    static soul::parser::Match LogicalAndExpression(LexerT& lexer);
    static soul::parser::Match InclusiveOrExpression(LexerT& lexer);
    static soul::parser::Match ExclusiveOrExpression(LexerT& lexer);
    static soul::parser::Match AndExpression(LexerT& lexer);
    static soul::parser::Match EqualityExpression(LexerT& lexer);
    static soul::parser::Match EqOp(LexerT& lexer);
    static soul::parser::Match RelationalExpression(LexerT& lexer);
    static soul::parser::Match RelOp(LexerT& lexer);
    static soul::parser::Match ShiftExpression(LexerT& lexer);
    static soul::parser::Match ShiftOp(LexerT& lexer);
    static soul::parser::Match AdditiveExpression(LexerT& lexer);
    static soul::parser::Match AddOp(LexerT& lexer);
    static soul::parser::Match MultiplicativeExpression(LexerT& lexer);
    static soul::parser::Match MulOp(LexerT& lexer);
    static soul::parser::Match PmExpression(LexerT& lexer);
    static soul::parser::Match PmOp(LexerT& lexer);
    static soul::parser::Match CastExpression(LexerT& lexer);
    static soul::parser::Match UnaryExpression(LexerT& lexer);
    static soul::parser::Match UnaryOperator(LexerT& lexer);
    static soul::parser::Match PostfixExpression(LexerT& lexer);
    static soul::parser::Match PostCastExpression(LexerT& lexer);
    static soul::parser::Match ExpressionList(LexerT& lexer, soul::ast::cpp::Node* owner);
    static soul::parser::Match PrimaryExpression(LexerT& lexer);
    static soul::parser::Match IdExpression(LexerT& lexer);
    static soul::parser::Match TypeSpecifierOrTypeName(LexerT& lexer);
    static soul::parser::Match NewExpression(LexerT& lexer);
    static soul::parser::Match NewPlacement(LexerT& lexer, soul::ast::cpp::Node* owner);
    static soul::parser::Match NewTypeId(LexerT& lexer);
    static soul::parser::Match NewDeclarator(LexerT& lexer);
    static soul::parser::Match NewInitializer(LexerT& lexer, soul::ast::cpp::Node* owner);
    static soul::parser::Match DirectNewDeclarator(LexerT& lexer);
    static soul::parser::Match DeleteExpression(LexerT& lexer);
    static soul::parser::Match OperatorFunctionId(LexerT& lexer);
    static soul::parser::Match Operator(LexerT& lexer);
};

} // namespace soul::cpp::expression::parser
