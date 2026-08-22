// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.inline_functions;

import soul.ast.span;
import std;

export namespace otava::symbols {

class Context;
class FunctionSymbol;

class InlineFunctionRepository
{
public:
    InlineFunctionRepository();
    FunctionSymbol* GetInlineFunction(FunctionSymbol* fn) const noexcept;
    void AddInlineFunction(FunctionSymbol* fn, FunctionSymbol* inlineFn);
private:
    std::unordered_map<FunctionSymbol*, FunctionSymbol*> inlineFunctionMap;
};

FunctionSymbol* InstantiateInlineFunction(FunctionSymbol* fn, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
