// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.namespaces;

import std;
import otava.symbols.id;
import otava.symbols.container_symbol;
import otava.ast.node;

export namespace otava::symbols {

class NamespaceSymbol : public ContainerSymbol
{
public:
    NamespaceSymbol(Module* module_, SymbolId id_);
    NamespaceSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
};

void BeginNamespace(otava::ast::Node* node, Context* context);
void EndNamespace(otava::ast::Node* node, int level, Context* context);

} // namespace otava::symbols
