// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.using_declaration;

import otava.ast.declaration;
import otava.ast.identifier;
import otava.ast.visitor;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.scope;
import otava.symbols.scope_resolver;

namespace otava::symbols {

class UsingDeclarationProcessor : public otava::ast::DefaultVisitor
{
public:
    UsingDeclarationProcessor(Context* context_);
    void Visit(otava::ast::UsingDeclarationNode& node) override;
    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    inline void SetScope(Scope* scope_) noexcept { scope = scope_; }
    inline bool Succeeded() const noexcept { return succeeded; }
private:
    Context* context;
    Scope* scope;
    bool succeeded;
};

UsingDeclarationProcessor::UsingDeclarationProcessor(Context* context_) : context(context_), scope(context->GetSymbolTable()->CurrentScope()), succeeded(false)
{
}

void UsingDeclarationProcessor::Visit(otava::ast::UsingDeclarationNode& node)
{
    node.Declarators()->Accept(*this);
}

void UsingDeclarationProcessor::Visit(otava::ast::QualifiedIdNode& node)
{
    Scopes scopes = GetScopes(node.Left(), context);
    for (Scope* scope : scopes.GetScopes())
    {
        context->PushScope(scope);
        FlagSetter flagSetter(context, ContextFlags::dontThrow);
        bool succeeded = AddUsingDeclaration(node.Right(), context);
        context->PopScope();
        if (succeeded)
        {
            return;
        }
    }
    if (context->GetFlag(ContextFlags::dontThrow))
    {
        if (!context->HasException())
        {
            context->SetException(MakeException("error adding using declaration: scope not found", node.GetFullSpan(), context));
        }
        return;
    }
    ThrowException("error adding using declaration: scope not found", node.GetFullSpan(), context);
}

void UsingDeclarationProcessor::Visit(otava::ast::IdentifierNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::vector<Symbol*> symbols;
    std::set<const Scope*> visited;
    scope->Lookup(node.Str(), SymbolGroupKind::all, ScopeLookup::thisScope, LookupFlags::none, symbols, visited, context);
    if (symbols.empty())
    {
        if (context->GetFlag(ContextFlags::dontThrow))
        {
            if (!context->HasException())
            {
                context->SetException(MakeException("symbol '" + node.Str() + "' not found", fullSpan, context));
            }
            return;
        }
        ThrowException("symbol '" + node.Str() + "' not found", fullSpan, context);
    }
    bool failed = false;
    for (Symbol* symbol : symbols)
    {
        if (symbol->IsNamespaceSymbol())
        {
            if (context->GetFlag(ContextFlags::dontThrow))
            {
                if (!context->HasException())
                {
                    context->SetException(MakeException("symbol '" + symbol->FullName(context) + "' denotes a namespace", fullSpan, context));
                }
                failed = true;
                break;
            }
            ThrowException("symbol '" + symbol->FullName(context) + "' denotes a namespace", fullSpan, context);
        }
        else
        {
            context->GetSymbolTable()->AddUsingDeclaration(&node, symbol, context);
        }
    }
    succeeded = !failed;
}

bool AddUsingDeclaration(otava::ast::Node* node, Context* context)
{
    UsingDeclarationProcessor processor(context);
    if (context->GetScope())
    {
        processor.SetScope(context->GetScope());
    }
    node->Accept(processor);
    return processor.Succeeded();
}

} // namespace otava::symbols
