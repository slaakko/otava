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

NamespaceSymbol::NamespaceSymbol(Module* module_, SymbolId id_) : ContainerSymbol(module_, id_), expanded(false)
{
    GetScope()->SetKind(ScopeKind::namespaceScope);
}

NamespaceSymbol::NamespaceSymbol(Module* module_, SymbolId id_, const std::string& name_) :
    ContainerSymbol(module_, id_, name_), expanded(false)
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

void NamespaceSymbol::Expand(Context* context)
{
    if (expanded) return;
    expanded = true;
    for (const auto& moduleSymboId : ModuleSymbolIds())
    {
        ModuleId moduleId = moduleSymboId.moduleId;
        Module* module = context->GetModuleMapper()->GetModule(moduleId);
        if (module)
        {
            SymbolId symbolId = moduleSymboId.symbolId;
            NamespaceSymbol* ns = module->GetSymbolTable()->GetNamespaceSymbol(symbolId, context);
            if (ns)
            {
                GetScope()->Import(ns->GetScope(), context);
            }
            else
            {
                ThrowException("namespace symbol " + std::to_string(ToUnderlying(symbolId)) + " not found from module " + module->Name());
            }
        }
        else
        {
            ThrowException("import module " + std::to_string(ToUnderlying(moduleId)) + " not found from namespace '" + FullName(context) + 
                "' of module " + GetModule()->Name());
        }
    }
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
