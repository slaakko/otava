// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.stmt_parser;

namespace otava::symbols {

StmtParser stmtParser;

void SetStmtParser(StmtParser stmtParser_) noexcept
{
    stmtParser = stmtParser_;
}

std::unique_ptr<otava::ast::Node> ParseStatement(const std::string& stmt, Context* context)
{
    return stmtParser(stmt, context);
}

} // namespace otava::symbols
