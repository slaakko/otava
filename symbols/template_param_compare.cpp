// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.template_param_compare;

import otava.symbols.templates;

namespace otava::symbols {

bool TemplateParamLess::operator()(TemplateParameterSymbol* left, TemplateParameterSymbol* right) const noexcept
{
    return left->GetIndex() < right->GetIndex();
}

bool TemplateParamEqual::operator()(TemplateParameterSymbol* left, TemplateParameterSymbol* right) const noexcept
{
    return left->GetIndex() == right->GetIndex();
}

} // namespace otava::symbols
