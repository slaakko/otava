// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.using_declaration;

import std;
import otava.ast.node;

export namespace otava::symbols {

class Context;

void AddUsingDeclaration(otava::ast::Node* node, Context* context);

} // namespace otava::symbols
