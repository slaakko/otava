// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.class_templates;

import otava.symbols.classes;
import std;

export namespace otava::symbols {

class ClassTemplateSpecializationSymbol : public ClassTypeSymbol
{
public:
    ClassTemplateSpecializationSymbol(Module* module__, SymbolId id_);
    ClassTemplateSpecializationSymbol(Module* module__, SymbolId id_, const std::string& name_);
    bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType) const noexcept override;
    ClassTypeSymbol* ClassTemplate() const noexcept;
    void SetClassTemplate(ClassTypeSymbol* classTemplate_) noexcept;
};

} // namespace otava::symbol
