// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.template_param_group_symbol;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.templates;

namespace otava::symbols {

TemplateParamGroupSymbol::TemplateParamGroupSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), templateParameterSymbol(nullptr), templateParamId(zeroSymbolId)
{
}

TemplateParamGroupSymbol::TemplateParamGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), templateParameterSymbol(nullptr), templateParamId(zeroSymbolId)
{
}

bool TemplateParamGroupSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::templateDeclarationScope:
    {
        return true;
    }
    }
    return false;
}

Symbol* TemplateParamGroupSymbol::GetSingleSymbol(Context* context)
{
    TemplateParameterSymbol* t = GetTemplateParam(context);
    if (t)
    {
        return t;
    }
    else
    {
        return this;
    }
}

TemplateParameterSymbol* TemplateParamGroupSymbol::GetTemplateParam(Context* context)
{
    if (templateParameterSymbol)
    {
        return templateParameterSymbol;
    }
    if (IsReadOnly() && templateParamId != zeroSymbolId)
    {
        templateParameterSymbol = context->GetSymbolTable()->GetTemplateParameterSymbol(templateParamId, context);
        if (!templateParameterSymbol)
        {
            ThrowException("templte parameter symbol " + std::to_string(ToUnderlying(templateParamId)) + " not found");
        }
    }
    return templateParameterSymbol;
}

void TemplateParamGroupSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(templateParameterSymbol->Id()));
}

void TemplateParamGroupSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    templateParamId = SymbolId(reader.CurrentReader().ReadULong());
}

} // namespace otava::symbols
