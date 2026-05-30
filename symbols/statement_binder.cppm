// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.statement_binder;

import std;
import otava.ast.node;

export namespace otava::symbols {

class FunctionDefinitionSymbol;
class BoundStatementNode;
class Context;

FunctionDefinitionSymbol* BindFunction(otava::ast::Node* functionDefinitionNode, FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context);
std::unique_ptr<BoundStatementNode> BindStatement(otava::ast::Node* statementNode, FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context);

} // namespace otava::symbols
