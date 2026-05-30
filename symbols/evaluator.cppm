// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.evaluator;

import std;
import otava.ast.node;
import otava.ast.literal;
import soul.ast.span;

export namespace otava::symbols {

class Value;
class Context;
class TypeSymbol;

TypeSymbol* GetIntegerType(otava::ast::Suffix suffix, const soul::ast::FullSpan& fullSpan, Context* context);
TypeSymbol* GetFloatingPointType(otava::ast::Suffix suffix, const soul::ast::FullSpan& fullSpan, Context* context);
TypeSymbol* GetStringType(otava::ast::EncodingPrefix encodingPrefix, const soul::ast::FullSpan& fullSpan, Context* context);
TypeSymbol* GetCharacterType(otava::ast::EncodingPrefix encodingPrefix, const soul::ast::FullSpan& fullSpan, Context* context);
Value* Evaluate(otava::ast::Node* node, Context* context);

} // namespace otava::symbols
