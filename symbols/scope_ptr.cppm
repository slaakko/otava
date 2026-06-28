// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.scope_ptr;

import std;
import otava.symbols.scope;
import otava.symbols.context;

export namespace otava::symbols {

class ScopePtr
{
public:
    inline ScopePtr() noexcept : context(nullptr) {}
    inline ScopePtr(Scope* scope, Context* context_) noexcept : context(context_) { context->GetSymbolTable()->BeginScope(scope, context); }
    inline void Reset(Scope* scope, Context* context_) noexcept
    {
        if (context)
        {
            context->GetSymbolTable()->EndScope(context);
        }
        context = context_;
        context->GetSymbolTable()->BeginScope(scope, context);
    }
    inline void Reset()
    {
        if (context)
        {
            context->GetSymbolTable()->EndScope(context);
            context = nullptr;
        }
    }
    inline ~ScopePtr()
    {
        if (context)
        {
            context->GetSymbolTable()->EndScope(context);
        }
    }
private:
    Context* context;
};

} // namespace otava::symbols
