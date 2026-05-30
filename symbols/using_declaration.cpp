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
private:
    Context* context;
    Scope* scope;
};

UsingDeclarationProcessor::UsingDeclarationProcessor(Context* context_) : context(context_), scope(context->GetSymbolTable()->CurrentScope())
{
}

void UsingDeclarationProcessor::Visit(otava::ast::UsingDeclarationNode& node)
{
    node.Declarators()->Accept(*this);
}

void UsingDeclarationProcessor::Visit(otava::ast::QualifiedIdNode& node)
{
    scope = ResolveScope(node.Left(), context);
    node.Right()->Accept(*this);
}

void UsingDeclarationProcessor::Visit(otava::ast::IdentifierNode& node)
{
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    std::vector<Symbol*> symbols;
    std::set<const Scope*> visited;
    scope->Lookup(node.Str(), SymbolGroupKind::all, ScopeLookup::thisScope, LookupFlags::none, symbols, visited, context);
    if (symbols.empty())
    {
        ThrowException("symbol '" + node.Str() + "' not found", fullSpan, context);
    }
    for (Symbol* symbol : symbols)
    {
        if (symbol->IsNamespaceSymbol())
        {
            ThrowException("symbol '" + symbol->FullName(context) + "' denotes a namespace", fullSpan, context);
        }
        else
        {
            context->GetSymbolTable()->AddUsingDeclaration(&node, symbol, context);
        }
    }
}

void AddUsingDeclaration(otava::ast::Node* node, Context* context)
{
    UsingDeclarationProcessor processor(context);
    node->Accept(processor);
}

} // namespace otava::symbols
