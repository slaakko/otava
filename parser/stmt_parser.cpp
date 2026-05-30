// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.parser.stmt;

import otava.lexer;
import otava.parser.statement;
import otava.symbols.context;
import util.unicode;

namespace otava::parser {

std::unique_ptr<otava::ast::Node> ParseStatement(const std::string& stmt, otava::symbols::Context* context)
{
    std::u32string s = util::ToUtf32(stmt);
    auto lexer = otava::lexer::MakeLexer(s.c_str(), s.c_str() + s.length(), "<stmt>");
    using LexerType = decltype(lexer);
    otava::symbols::Lexer* prevLexer = context->GetLexer();
    context->SetLexer(&lexer);
    std::unique_ptr<otava::ast::Node> node = otava::parser::statement::StatementParser<LexerType>::Parse(lexer, context);
    context->SetLexer(prevLexer);
    return node;
}

} // otava::parser
