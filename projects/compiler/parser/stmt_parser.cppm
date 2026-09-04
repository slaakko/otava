// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.parser.stmt;

import std;
import otava.ast.node;
import otava.symbols.context;

export namespace otava::parser {

std::unique_ptr<otava::ast::Node> ParseStatement(const std::string& stmt, otava::symbols::Context* context);

} // otava::parser
