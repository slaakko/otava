// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.vtabgen;

import soul.ast.span;
import std;

export namespace otava::symbols {

class ClassTypeSymbol;

class VTabGenerator
{
public:
    virtual ~VTabGenerator();
    virtual void GenerateVTab(ClassTypeSymbol* cls, const soul::ast::FullSpan& fullSpan) = 0;
};

} // namespace otava::symbol
