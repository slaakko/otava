// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.class_templates;

namespace otava::symbols {

ClassTemplateSpecializationSymbol::ClassTemplateSpecializationSymbol(Module* module_, SymbolId id_) : ClassTypeSymbol(module_, id_)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

ClassTemplateSpecializationSymbol::ClassTemplateSpecializationSymbol(Module* module_, SymbolId id_, const std::string& name_) : ClassTypeSymbol(module_, id_, name_)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

bool ClassTemplateSpecializationSymbol::IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType) const noexcept
{
    // TODO
    return false;
}

ClassTypeSymbol* ClassTemplateSpecializationSymbol::ClassTemplate() const noexcept
{
    // TODO
    return nullptr;
}

void ClassTemplateSpecializationSymbol::SetClassTemplate(ClassTypeSymbol* classTemplate_) noexcept
{
    // TODO
}

} // namespace otava::symbol
