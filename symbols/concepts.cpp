// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.concepts;

import otava.symbols.context;

namespace otava::symbols {

ConceptSymbol::ConceptSymbol(Module* module_, SymbolId id_) : Symbol(module_, id_)
{
}

ConceptSymbol::ConceptSymbol(Module* module_, SymbolId id_, const std::string& name_) : Symbol(module_, id_, name_)
{
}

bool IsConceptName(otava::ast::Node* node, Context* context) noexcept
{
    std::string name = node->Str();
    Symbol* symbol = context->GetSymbolTable()->Lookup(name, SymbolGroupKind::conceptSymbolGroup, node->GetFullSpan(), context);
    if (symbol)
    {
        return symbol->IsConceptSymbol();
    }
    return false;
}

void ProcessConcept(otava::ast::Node* node, Context* context)
{
/*
    ConceptProcessor processor(context);
    node->Accept(processor);
*/
}

} // namespace otava::symbols
