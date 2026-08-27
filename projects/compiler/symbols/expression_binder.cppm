// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.expression_binder;

import std;
import otava.symbols.bound_tree;
import otava.ast.node;
import otava.symbols.scope;
import otava.symbols.symbol;
import soul.ast.span;

export namespace otava::symbols {

class Context;
class Scope;
class StatementBinder;
class VariableSymbol;

class OperatorGroupNameMap
{
public:
    static OperatorGroupNameMap& Instance();
    std::string GetGroupName(otava::ast::NodeKind nodeKind, const soul::ast::FullSpan& fullSpan, Context* context);
private:
    OperatorGroupNameMap();
    std::map<otava::ast::NodeKind, std::string> operatorGroupNameMap;
};

otava::ast::Node* MakeTypeNameNodes(const soul::ast::FullSpan& fullSpan, const std::string& fullTypeName);
std::pair<VariableSymbol*, int> GetParentTemporary(std::int64_t nodeId, Context* context);

std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context);
std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context, bool booleanChild);
std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context, SymbolGroupKind symbolGroups, Scope*& scope);
void InitExpressionBinder();

otava::ast::Node* MakeTypeNameNodes(const soul::ast::FullSpan& fullSpan, const std::string& fullTypeName);

} // namespace otava::symbols
