// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.class_group_symbol;

import otava.symbols.symbol;
import std;

export namespace otava::symbols {

class ForwardClassDeclarationSymbol;

class ClassGroupSymbol : public Symbol
{
public:
    ClassGroupSymbol(Module* module_, SymbolId id_);
    ClassGroupSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    Symbol* GetSingleSymbol(Context* contex) noexcept override;
private:
    std::vector<ClassTypeSymbol*> classes;
    std::vector<ForwardClassDeclarationSymbol*> forwardDeclarations;
};

} // namespace otava::symbols
