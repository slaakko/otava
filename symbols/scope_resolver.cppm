// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.scope_resolver;

import std;
import soul.ast.span;
import otava.ast.node;
import otava.symbols.scope;

export namespace otava::symbols {

class Scope;
class Context;

Scope* ResolveScope(otava::ast::Node* nnsNode, Context* context);

void AddParentScope(otava::ast::Node* node, Context* context);
Scope* GetScope(otava::ast::Node* nnsNode, Context* context);
Scopes GetScopes(otava::ast::Node* nnsNode, Context* context);
Scope* EnterScope(Scope* scope, const std::vector<std::string>& containerNames, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
