// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.compound_type_symbol;

import otava.symbols.derivations;
import otava.symbols.type_symbol;
import std;

export namespace otava::symbols {

class CompoundTypeSymbol : public TypeSymbol
{
public:
    CompoundTypeSymbol(Module* module_, SymbolId id_);
    CompoundTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType) const noexcept override;
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
};

} // namespace otava::symbols
