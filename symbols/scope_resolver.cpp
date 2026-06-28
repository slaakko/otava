// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.scope_resolver;

import otava.ast.identifier;
import otava.ast.visitor;
import otava.ast.templates;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.scope_ptr;
import otava.symbols.type_resolver;

namespace otava::symbols {

class ScopeResolver : public otava::ast::DefaultVisitor
{
public:
    ScopeResolver(Context* context_);
    inline Scope* GetScope() const { return currentScope; }
    void Visit(otava::ast::NestedNameSpecifierNode& node) override;
    void Visit(otava::ast::ColonColonNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::TemplateIdNode& node) override;
private:
    Context* context;
    bool first;
    Scope* currentScope;
};

ScopeResolver::ScopeResolver(Context* context_) : context(context_), first(false), currentScope(nullptr)
{
}

void ScopeResolver::Visit(otava::ast::NestedNameSpecifierNode& node)
{
    first = true;
    currentScope = context->GetSymbolTable()->CurrentScope();
    VisitSequence(node);
}

void ScopeResolver::Visit(otava::ast::ColonColonNode& node)
{
    if (first)
    {
        currentScope = context->GetSymbolTable()->GlobalNs()->GetScope();
        first = false;
    }
}

void ScopeResolver::Visit(otava::ast::IdentifierNode& node)
{
    first = false;
    Symbol* symbol = currentScope->Lookup(node.Str(), 
        SymbolGroupKind::templateParamSymbolGroup | SymbolGroupKind::classSymbolGroup | SymbolGroupKind::enumSymbolGroup | SymbolGroupKind::aliasSymbolGroup |
        SymbolGroupKind::namespaceSymbolGroup, ScopeLookup::allScopes, node.GetFullSpan(), context, LookupFlags::none);
    if (!symbol)
    {
        symbol = context->GetSymbolTable()->LookupInScopeStack(node.Str(), SymbolGroupKind::templateParamSymbolGroup | SymbolGroupKind::classSymbolGroup |
            SymbolGroupKind::aliasSymbolGroup | SymbolGroupKind::enumSymbolGroup | SymbolGroupKind::namespaceSymbolGroup, node.GetFullSpan(), context, 
            LookupFlags::none);
    }
    if (symbol)
    {
        while (symbol && symbol->IsAliasTypeSymbol())
        {
            AliasTypeSymbol* aliasTypeSymbol = static_cast<AliasTypeSymbol*>(symbol);
            symbol = aliasTypeSymbol->ReferredType(context);
        }
        if (symbol)
        {
            Scope* scope = symbol->GetScope();
            if (scope)
            {
                currentScope = scope;
            }
            else
            {
                ThrowException("symbol '" + symbol->FullName(context) + "' does not have a scope", node.GetFullSpan(), context);
            }
        }
        else
        {
            ThrowException("symbol '" + node.Str() + "' not found from " + ScopeKindStr(currentScope->Kind()) + " '" + 
                currentScope->FullName(context) + "'", node.GetFullSpan(), context);
        }
    }
    else
    {
        ThrowException("symbol '" + node.Str() + "' not found from " + ScopeKindStr(currentScope->Kind()) + " '" + currentScope->FullName(context) + "'", 
            node.GetFullSpan(), context);
    }
}

void ScopeResolver::Visit(otava::ast::TemplateIdNode& node)
{
    first = false;
    ScopePtr scopePtr(currentScope, context);
    TypeSymbol* type = ResolveType(&node, DeclarationFlags::none, context);
    currentScope = type->GetScope();
    scopePtr.Reset();
}

Scope* ResolveScope(otava::ast::Node* nnsNode, Context* context)
{
    ScopeResolver resolver(context);
    nnsNode->Accept(resolver);
    return resolver.GetScope();
}

otava::symbols::Scope* GetScope(otava::ast::Node* nnsNode, Context* context)
{
    Scope* scope = ResolveScope(nnsNode, context);
    return scope;
}

void AddParentScope(otava::ast::Node* node, Context* context)
{
    if (context->GetFlag(ContextFlags::addClassScope))
    {
        if (node->IsQualifiedIdNode())
        {
            if (context->GetSymbolTable()->CurrentScope()->IsNamespaceScope())
            {
                otava::ast::QualifiedIdNode* qualifiedIdNode = static_cast<otava::ast::QualifiedIdNode*>(node);
                Scope* scope = ResolveScope(qualifiedIdNode->Left(), context);
                if (scope->IsClassScope())
                {
                    context->GetSymbolTable()->CurrentScope()->PushParentScope(scope);
                }
            }
        }
    }
}

} // namespace otava::symbols
