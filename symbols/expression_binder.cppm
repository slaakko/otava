// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.expression_binder;

import std;
import otava.ast.node;
import otava.symbols.scope;
import otava.symbols.symbol;
import soul.ast.span;

export namespace otava::symbols {

class BoundExpressionNode;
class Context;
class Scope;
class StatementBinder;
class VariableSymbol;

otava::ast::Node* MakeTypeNameNodes(const soul::ast::FullSpan& fullSpan, const std::u32string& fullTypeName);
std::pair<VariableSymbol*, int> GetParentTemporary(std::int64_t nodeId, Context* context);

std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context);
std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context, bool booleanChild);
std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context, SymbolGroupKind symbolGroups, Scope*& scope);
void InitExpressionBinder();

bool MultiplicativeRightIdOperandNotFound(otava::ast::Node* op, otava::ast::Node* rightOperand, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
