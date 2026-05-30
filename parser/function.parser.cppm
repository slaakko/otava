
// this file has been automatically generated from 'D:/work/otava/parser/function.parser' using soul parser generator spg version 5.2.0

export module otava.parser.function;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::function {

template<typename LexerT>
struct FunctionParser
{
    static soul::parser::Match FunctionDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NoDeclSpecFunctionDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match FunctionBody(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Default(LexerT& lexer);
    static soul::parser::Match Delete(LexerT& lexer);
    static soul::parser::Match ParameterDeclarationClause(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match ParameterDeclarationList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match ParameterDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match OperatorFunctionId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ConversionFunctionId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DestructorId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ConversionTypeId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ConversionDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match OperatorKw(LexerT& lexer);
    static soul::parser::Match Operator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Noexcept(LexerT& lexer);
};

} // namespace otava::parser::function
