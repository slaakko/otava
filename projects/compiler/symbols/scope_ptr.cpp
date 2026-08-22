// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.scope_ptr;

namespace otava::symbols {

ScopesPtr::ScopesPtr(Scopes&& scopes, Context* context_) : context(context_)
{
    context->PushScopes(std::move(scopes));
}

void ScopesPtr::Reset(Scopes&& scopes, Context* context_) noexcept
{
    if (context)
    {
        context->PopScopes();
        context = nullptr;
    }
    context = context_;
    context->PushScopes(std::move(scopes));
}

void ScopesPtr::Reset()
{
    if (context)
    {
        context->PopScopes();
        context = nullptr;
    }
}

ScopesPtr::~ScopesPtr()
{
    if (context)
    {
        context->PopScopes();
    }
}

} // namespace otava::symbols
