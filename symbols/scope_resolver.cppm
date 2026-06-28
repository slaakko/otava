// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.scope_resolver;

import std;
import otava.ast.node;

export namespace otava::symbols {

class Scope;
class Context;

Scope* ResolveScope(otava::ast::Node* nnsNode, Context* context);

void AddParentScope(otava::ast::Node* node, Context* context);
otava::symbols::Scope* GetScope(otava::ast::Node* nnsNode, Context* context);

} // namespace otava::symbols
