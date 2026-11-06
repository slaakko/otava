// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.stmt_parser;

import std;
import otava.ast.node;
import otava.symbols.context;

export namespace otava::symbols {

using StmtParser = std::unique_ptr<otava::ast::Node>(*)(const std::string&, Context*);

void SetStmtParser(StmtParser stmtParser_) noexcept;

std::unique_ptr<otava::ast::Node> ParseStatement(const std::string& stmt, Context* context);

} // namespace otava::symbols
