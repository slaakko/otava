// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.namespaces;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.symbol_table;
import otava.ast.visitor;
import otava.ast.identifier;

namespace otava::symbols {

NamespaceSymbol::NamespaceSymbol(Module* module_, SymbolId id_) : ContainerSymbol(module_, id_)
{
    GetScope()->SetKind(ScopeKind::namespaceScope);
}

NamespaceSymbol::NamespaceSymbol(Module* module_, SymbolId id_, const std::string& name_) :
    ContainerSymbol(module_, id_, name_)
{
    GetScope()->SetKind(ScopeKind::namespaceScope);
}

bool NamespaceSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope: return true;
    }
    return false;
}

class NamespaceCreator : public otava::ast::DefaultVisitor
{
public:
    NamespaceCreator(Context* context_);
    void Visit(otava::ast::ColonColonNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
private:
    Context* context;
    Scope* currentScope;
    bool first;
};

NamespaceCreator::NamespaceCreator(Context* context_) : context(context_), currentScope(context->GetSymbolTable()->CurrentScope()), first(true)
{
}

void NamespaceCreator::Visit(otava::ast::ColonColonNode& node)
{
    if (first)
    {
        currentScope = context->GetSymbolTable()->GlobalNs()->GetScope();
        first = false;
    }
}

void NamespaceCreator::Visit(otava::ast::IdentifierNode& node)
{
    first = false;
    context->GetSymbolTable()->BeginNamespace(&node, context);
    currentScope = context->GetSymbolTable()->CurrentScope();
}

void BeginNamespace(otava::ast::Node* node, Context* context)
{
    NamespaceCreator creator(context);
    node->Accept(creator);
}

void EndNamespace(otava::ast::Node* node, int level, Context* context)
{
    if (!context->GetSymbolTable()->CurrentScope()->GetSymbol()->IsNamespaceSymbol())
    {
        ThrowException("otava.symbols.namespace: EndNamespace(): namespace scope expected", node->GetFullSpan(), context);
    }
    context->GetSymbolTable()->EndNamespace(level);
}

} // namespace otava::symbols
