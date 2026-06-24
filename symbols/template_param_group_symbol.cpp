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
    Symbol(module_, id_), readOnlyTemplateParamGroup(nullptr), templateParameterSymbol(nullptr), templateParamId(zeroSymbolId), expanded(false)
{
}

TemplateParamGroupSymbol::TemplateParamGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), readOnlyTemplateParamGroup(nullptr), templateParameterSymbol(nullptr), templateParamId(zeroSymbolId), expanded(false)
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
    if (!IsReadOnly())
    {
        Expand(context);
    }
    if (readOnlyTemplateParamGroup)
    {
        return readOnlyTemplateParamGroup->GetSingleSymbol(context);
    }
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
    templateParamId = SymbolId(reader.CurrentReader().ReadUInt());
}

void TemplateParamGroupSymbol::Expand(Context* context)
{
    if (expanded) return;
    expanded = true;
    for (const auto& moduleSymbolId : ModuleSymbolIds())
    {
        ModuleId moduleId = moduleSymbolId.moduleId;
        Module* module = context->GetModuleMapper()->GetModule(moduleId);
        if (module)
        {
            SymbolId symbolId = moduleSymbolId.symbolId;
            TemplateParamGroupSymbol* templateParamGroup = module->GetSymbolTable()->GetTemplateParamGroupSymbol(symbolId, context);
            if (templateParamGroup)
            {
                if (readOnlyTemplateParamGroup)
                {
                    ThrowException("template parameter group symbol '" + Name() + "' not unique");
                }
                readOnlyTemplateParamGroup = templateParamGroup;
            }
            else
            {
                ThrowException("template parameter group symbol " + std::to_string(ToUnderlying(symbolId)) + " not found from module " + module->Name());
            }
        }
        else
        {
            ThrowException("import module " + std::to_string(ToUnderlying(moduleId)) + " not found from template parameter group '" + FullName(context) +
                "' of module " + GetModule()->Name());
        }
    }
}

} // namespace otava::symbols
