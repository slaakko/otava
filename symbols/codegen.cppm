// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.codegen;

import std;
import soul.ast.span;
import otava.symbols.bound_tree_visitor;

export namespace otava::symbols {

class ClassTypeSymbol;

class CodeGen : public DefaultBoundTreeVisitor
{
public:
    virtual void GenerateVTab(ClassTypeSymbol* cls, const soul::ast::FullSpan& fullSpan) = 0;
};

} // namespace otava::symbol
