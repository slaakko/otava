// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.function_completion;

import soul.ast.span;
import std;

export namespace otava::symbols {

class Context;
class FunctionSymbol;

class FunctionCompletionRepository
{
public:
    FunctionCompletionRepository();
    FunctionSymbol* GetCompletedFunction(FunctionSymbol* fn) const noexcept;
    void AddCompletedFunction(FunctionSymbol* fn, FunctionSymbol* completedFn);
private:
    std::unordered_map<FunctionSymbol*, FunctionSymbol*> completedFunctionMap;
};

FunctionSymbol* CompleteIncompleteFunction(FunctionSymbol* fn, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
