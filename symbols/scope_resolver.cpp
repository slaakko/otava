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
    Scope* GetScope() const noexcept;
    void Visit(otava::ast::NestedNameSpecifierNode& node) override;
    void Visit(otava::ast::ColonColonNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::TemplateIdNode& node) override;
private:
    Context* context;
    bool first;
    Scope* currentScope;
    bool failed;
};

ScopeResolver::ScopeResolver(Context* context_) : context(context_), first(false), currentScope(nullptr), failed(false)
{
}

Scope* ScopeResolver::GetScope() const noexcept
{
    if (failed) return nullptr;
    return currentScope;
}

void ScopeResolver::Visit(otava::ast::NestedNameSpecifierNode& node)
{
    if (failed) return;
    first = true;
    currentScope = context->GetSymbolTable()->CurrentScope();
    VisitSequence(node);
}

void ScopeResolver::Visit(otava::ast::ColonColonNode& node)
{
    if (failed) return;
    if (first)
    {
        currentScope = context->GetSymbolTable()->GetGlobalNs(context)->GetScope();
        first = false;
    }
}

void ScopeResolver::Visit(otava::ast::IdentifierNode& node)
{
    if (failed) return;
    first = false;
    if (!currentScope) return;
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
                if (context->GetFlag(ContextFlags::dontThrow))
                {
                    failed = true;
                    if (!context->HasException())
                    {
                        context->SetException(MakeException("symbol '" + symbol->FullName(context) + "' does not have a scope", node.GetFullSpan(), context));
                    }
                    return;
                }
                ThrowException("symbol '" + symbol->FullName(context) + "' does not have a scope", node.GetFullSpan(), context);
            }
        }
        else
        {
            if (context->GetFlag(ContextFlags::dontThrow))
            {
                failed = true;
                if (!context->HasException())
                {
                    context->SetException(MakeException("symbol '" + node.Str() + "' not found from " + ScopeKindStr(currentScope->Kind()) + " '" +
                        currentScope->FullName(context) + "'", node.GetFullSpan(), context));
                }
                return;
            }
            ThrowException("symbol '" + node.Str() + "' not found from " + ScopeKindStr(currentScope->Kind()) + " '" +
                currentScope->FullName(context) + "'", node.GetFullSpan(), context);
        }
    }
    else
    {
        if (context->GetFlag(ContextFlags::dontThrow))
        {
            failed = true;
            if (!context->HasException())
            {
                context->SetException(MakeException("symbol '" + node.Str() + "' not found from " + ScopeKindStr(currentScope->Kind()) + " '" + 
                    currentScope->FullName(context) + "'", node.GetFullSpan(), context));
            }
            return;
        }
        ThrowException("symbol '" + node.Str() + "' not found from " + ScopeKindStr(currentScope->Kind()) + " '" + currentScope->FullName(context) + "'",
            node.GetFullSpan(), context);
    }
}

void ScopeResolver::Visit(otava::ast::TemplateIdNode& node)
{
    if (failed) return;
    first = false;
    ScopePtr scopePtr(currentScope, context);
    TypeResolverFlags flags = TypeResolverFlags::none;
    if (context->GetFlag(ContextFlags::dontThrow))
    {
        flags = flags | TypeResolverFlags::dontThrow;
    }
    TypeSymbol* type = ResolveType(&node, DeclarationFlags::none, context, flags);
    if (type && type->GetScope())
    {
        currentScope = type->GetScope();
    }
    else if (context->GetFlag(ContextFlags::dontThrow))
    {
        failed = true;
        if (!context->HasException())
        {
            context->SetException(MakeException("could not resolve template id", node.GetFullSpan(), context));
        }
        return;
    }
    scopePtr.Reset();
}

Scope* ResolveScope(otava::ast::Node* nnsNode, Context* context)
{
    ScopeResolver resolver(context);
    nnsNode->Accept(resolver);
    return resolver.GetScope();
}

Scope* GetScope(otava::ast::Node* nnsNode, Context* context)
{
    Scope* scope = ResolveScope(nnsNode, context);
    return scope;
}

Scope* EnterScope(Scope* scope, const std::vector<std::string>& containerNames, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Scope* lastScope = scope;
    Symbol* lastSymbol = nullptr;
    int n = int(containerNames.size());
    for (int i = n - 1; i >= 0; --i)
    {
        const std::string& name = containerNames[i];
        Symbol* symbol = scope->Lookup(name, SymbolGroupKind::all, ScopeLookup::thisScope, fullSpan, context, LookupFlags::none);
        if (symbol)
        {
            lastSymbol = symbol;
            scope = symbol->GetScope();
            if (scope)
            {
                lastScope = scope;
            }
        }
    }
    return lastScope;
}

Scopes GetScopes(otava::ast::Node* nnsNode, Context* context)
{
    context->PushSetFlag(ContextFlags::dontThrow);
    Scopes scopes;
    Scope* scope = GetScope(nnsNode, context);
    if (scope)
    {
        scopes.AddScope(scope);
        if (scope->IsClassScope())
        {
            context->PopFlags();
            return scopes;
        }
    }
    std::vector<std::string> containerNames;
    Scope* currentScope = context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context);
    if (currentScope)
    {
        containerNames = GetContainerNames(currentScope->GetSymbol(), context);
    }
    Scope* templateNsScope = context->GetTemplateNsScope();
    if (templateNsScope)
    {
        Symbol* symbol = templateNsScope->GetSymbol();
        containerNames = GetContainerNames(symbol, context);
    }
    std::vector<Module*> importedModules = context->GetModule()->ImportExportModules(context);
    Module* templateModule = context->GetTemplateModule();
    if (templateModule)
    {
        std::vector<Module*> templateModules = templateModule->ImportExportModules(context);
        for (Module* importedModule : templateModules)
        {
            if (std::find(importedModules.begin(), importedModules.end(), importedModule) == importedModules.end())
            {
                importedModules.push_back(importedModule);
            }
        }
    }
    for (Module* importedModule : importedModules)
    {
        ModulePtr modulePtr(importedModule, context);
        Scope* containerScope = EnterScope(importedModule->GetSymbolTable()->CurrentScope(), containerNames, nnsNode->GetFullSpan(), context);
        ScopePtr scopePtr(containerScope, context);
        Scope* scope = GetScope(nnsNode, context);
        if (scope)
        {
            scopes.AddScope(scope);
            if (scope->IsClassScope())
            {
                break;
            }
        }
    }
    context->PopFlags();
    return scopes;
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
