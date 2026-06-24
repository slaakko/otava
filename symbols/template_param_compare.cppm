// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.template_param_compare;

import std;

export namespace otava::symbols {

class TemplateParameterSymbol;

struct TemplateParamLess
{
    bool operator()(TemplateParameterSymbol* left, TemplateParameterSymbol* right) const noexcept;
};

struct TemplateParamEqual
{
    bool operator()(TemplateParameterSymbol* left, TemplateParameterSymbol* right) const noexcept;
};

} // namespace otava::symbols
