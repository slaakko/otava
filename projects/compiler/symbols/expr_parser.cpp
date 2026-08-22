// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.expr_parser;

namespace otava::symbols {

ExprParser exprParser;

void SetExprParser(ExprParser exprParser_) noexcept
{
    exprParser = exprParser_;
}

std::unique_ptr<otava::ast::Node> ParseExpression(const std::string& expr, Context* context)
{
    return exprParser(expr, context);
}

} // namespace otava::symbols
