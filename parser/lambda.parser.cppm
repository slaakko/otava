
// this file has been automatically generated from 'D:/work/otava/parser/lambda.parser' using soul parser generator spg version 5.2.0

export module otava.parser.lambda;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::lambda {

template<typename LexerT>
struct LambdaParser
{
    static soul::parser::Match LambdaExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match LambdaIntroducer(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match LambdaCapture(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CaptureDefault(LexerT& lexer);
    static soul::parser::Match CaptureList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match Capture(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match InitCapture(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match SimpleCapture(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match LambdaDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match LambdaSpecifiers(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match LambdaTemplateParams(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::lambda
