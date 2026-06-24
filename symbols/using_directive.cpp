// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.using_directive;

import otava.ast.declaration;
import otava.ast.identifier;
import otava.ast.visitor;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.scope;
import otava.symbols.scope_resolver;

namespace otava::symbols {

class UsingDirectiveProcessor : public otava::ast::DefaultVisitor
{
public:
    UsingDirectiveProcessor(Context* context_);
    void Visit(otava::ast::UsingDirectiveNode& node) override;
    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
private:
    Context* context;
    Scope* scope;
};

UsingDirectiveProcessor::UsingDirectiveProcessor(Context* context_) : context(context_), scope(context->GetSymbolTable()->CurrentScope())
{
}

void UsingDirectiveProcessor::Visit(otava::ast::UsingDirectiveNode& node)
{
    node.GetId()->Accept(*this);
}

void UsingDirectiveProcessor::Visit(otava::ast::QualifiedIdNode& node)
{
    scope = ResolveScope(node.Left(), context);
    node.Right()->Accept(*this);
}

void UsingDirectiveProcessor::Visit(otava::ast::IdentifierNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    Symbol* symbol = scope->Lookup(node.Str(), SymbolGroupKind::namespaceSymbolGroup, ScopeLookup::allScopes, fullSpan, context, LookupFlags::none);
    if (symbol)
    {
        if (symbol->IsNamespaceSymbol())
        {
            NamespaceSymbol* ns = static_cast<NamespaceSymbol*>(symbol);
            context->GetSymbolTable()->AddUsingDirective(ns, &node, context);
        }
        else
        {
            ThrowException("symbol '" + symbol->FullName(context) + "' does not denote a namespace", fullSpan, context);
        }
    }
    else
    {
        ThrowException("symbol '" + node.Str() + "' not found", fullSpan, context);
    }
}

void AddUsingDirective(otava::ast::Node* node, Context* context)
{
    UsingDirectiveProcessor processor(context);
    node->Accept(processor);
}

} // namespace otava::symbols
