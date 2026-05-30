// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.concepts;

import std;
import otava.symbols.symbol;
import otava.ast.node;

export namespace otava::symbols {

class Context;

class ConceptSymbol : public Symbol
{
public:
    ConceptSymbol(Module* module_, SymbolId id_);
    ConceptSymbol(Module* module_, SymbolId id_, const std::string& name_);
};

bool IsConceptName(otava::ast::Node* node, Context* context) noexcept;
void ProcessConcept(otava::ast::Node* node, Context* context);

} // namespace otava::symbols
