// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.statement_binder;

import otava.symbols.bound_tree;

namespace otava::symbols {

FunctionDefinitionSymbol* BindFunction(otava::ast::Node* functionDefinitionNode, FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context)
{
    // TODO
    return nullptr;
}

std::unique_ptr<BoundStatementNode> BindStatement(otava::ast::Node* statementNode, FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context)
{
    // TODO
    return std::unique_ptr<BoundStatementNode>();
}

} // namespace otava::symbols
