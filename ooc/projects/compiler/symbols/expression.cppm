// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.expression;

import std;
import otava.ast.node;
import soul.ast.span;

export namespace otava::symbols {

class Context;

bool MultiplicativeRightIdOperandNotFound(otava::ast::Node* op, otava::ast::Node* rightOperand, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
