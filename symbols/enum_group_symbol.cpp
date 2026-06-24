// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.enum_group_symbol;

import otava.symbols.enums;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.writer;
import otava.symbols.reader;

namespace otava::symbols {

EnumGroupSymbol::EnumGroupSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), enumType(nullptr), enumTypeId(zeroSymbolId), forwardDeclaration(nullptr), readOnlyEnumGroup(nullptr), expanded(false)
{
}

EnumGroupSymbol::EnumGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), enumType(nullptr), enumTypeId(zeroSymbolId), forwardDeclaration(nullptr), readOnlyEnumGroup(nullptr), expanded(false)
{
}

bool EnumGroupSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::classScope:
    {
        return true;
    }
    }
    return false;
}

Symbol* EnumGroupSymbol::GetSingleSymbol(Context* context) 
{
    if (!IsReadOnly())
    {
        Expand(context);
    }
    if (readOnlyEnumGroup)
    {
        return readOnlyEnumGroup->GetSingleSymbol(context);
    }
    EnumeratedTypeSymbol* e = GetEnumType(context);
    if (e)
    {
        return e;
    }
    else if (forwardDeclaration)
    {
        return forwardDeclaration;
    }
    else
    {
        return this;
    }
}

void EnumGroupSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumType->Id()));
}

void EnumGroupSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    enumTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

bool EnumGroupSymbol::IsExportSymbol(Context* context) const noexcept
{
    return Symbol::IsExportSymbol(context) && enumType != nullptr && enumType->IsExportSymbol(context);
}

void EnumGroupSymbol::Expand(Context* context)
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
            EnumGroupSymbol* enumGroup = module->GetSymbolTable()->GetEnumGroupSymbol(symbolId, context);
            if (enumGroup)
            {
                if (readOnlyEnumGroup)
                {
                    ThrowException("enum group symbol '" + Name() + "' not unique");
                }
                readOnlyEnumGroup = enumGroup;
            }
            else
            {
                //ThrowException("enum group symbol " + std::to_string(ToUnderlying(symbolId)) + " not found from module " + module->Name());
            }
        }
        else
        {
            ThrowException("import module " + std::to_string(ToUnderlying(moduleId)) + " not found from enum group '" + FullName(context) +
                "' of module " + GetModule()->Name());
        }
    }
}

EnumeratedTypeSymbol* EnumGroupSymbol::GetEnumType(Context* context)
{
    if (enumType)
    {
        return enumType;
    }
    if (IsReadOnly() && enumTypeId != zeroSymbolId)
    {
        enumType = GetModule()->GetSymbolTable()->GetEnumeratedTypeSymbol(enumTypeId, context);
        if (!enumType)
        {
            ThrowException("enumerated type symbol " + std::to_string(ToUnderlying(enumTypeId)) + " not found");
        }
    }
    return enumType;
}

} // namespace otava::symbols
