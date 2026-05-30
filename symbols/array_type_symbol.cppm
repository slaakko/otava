// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.array_type_symbol;

import std;
import otava.symbols.type_symbol;

export namespace otava::symbols {

class ArrayTypeSymbol : public TypeSymbol
{
public:
    ArrayTypeSymbol(Module* module_, SymbolId id_);
    ArrayTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    inline bool IsBound() const noexcept { return bound; }
    inline void SetBound() noexcept { bound = true; }
    void Bind(const soul::ast::FullSpan& fullSpan, Context* context);
    TypeSymbol* ElementType() const noexcept;
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    bool bound;
};

} // namespace otava::symbols
