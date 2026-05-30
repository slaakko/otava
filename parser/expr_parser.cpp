// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.parser.expr;

import otava.lexer;
import otava.parser.expression;
import otava.symbols.context;
import util.unicode;

namespace otava::parser {

std::unique_ptr<otava::ast::Node> ParseExpression(const std::string& expr, otava::symbols::Context* context)
{
    std::u32string e = util::ToUtf32(expr);
    auto lexer = otava::lexer::MakeLexer(e.c_str(), e.c_str() + e.length(), "<expr>");
    using LexerType = decltype(lexer);
    otava::symbols::Lexer* prevLexer = context->GetLexer();
    context->SetLexer(&lexer);
    std::unique_ptr<otava::ast::Node> node = otava::parser::expression::ExpressionParser<LexerType>::Parse(lexer, context);
    context->SetLexer(prevLexer);
    return node;
}

} // otava::parser
