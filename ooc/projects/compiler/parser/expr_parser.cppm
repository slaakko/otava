// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.parser.expr;

import std;
import otava.ast.node;
import otava.symbols.context;

export namespace otava::parser {

std::unique_ptr<otava::ast::Node> ParseExpression(const std::string& expr, otava::symbols::Context* contex);

} // otava::parser
