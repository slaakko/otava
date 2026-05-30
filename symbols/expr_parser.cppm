// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.expr_parser;

import std;
import otava.ast.node;

export namespace otava::symbols {

class Context;

using ExprParser = std::unique_ptr<otava::ast::Node>(*)(const std::string&, Context* context);

void SetExprParser(ExprParser exprParser_) noexcept;

std::unique_ptr<otava::ast::Node> ParseExpression(const std::string& expr, Context* context);

} // namespace otava::symbols
