// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.using_directive;

import std;
import otava.ast.node;

export namespace otava::symbols {

class Context;

void AddUsingDirective(otava::ast::Node* node, Context* context);

} // namespace otava::symbols
