// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.exception_handling;

import otava.ast.statement;
import std;

export namespace otava::symbols {

class BoundStatementNode;
class Context;
class FunctionDefinitionSymbol;

std::unique_ptr<BoundStatementNode> ConvertReturnStatement(otava::ast::ReturnStatementNode* returnStatement,
    FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context);

} // namespace otava::symbols
