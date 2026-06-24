// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.alias_group_symbol;

import otava.symbols.alias_type_symbol;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.type_compare;
import otava.symbols.writer;
import otava.symbols.reader;

namespace otava::symbols {

struct ReferredTypeEqual
{
    ReferredTypeEqual(AliasTypeSymbol* aliasTypeSymbol_, Context* context_) : aliasTypeSymbol(aliasTypeSymbol_), context(context_) {}
    inline bool operator()(AliasTypeSymbol* symbol) const noexcept
    {
        return TypesEqual(symbol->ReferredType(context), aliasTypeSymbol->ReferredType(context), context);
    }
    AliasTypeSymbol* aliasTypeSymbol;
    Context* context;
};

AliasGroupSymbol::AliasGroupSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), aliasTypeSymbolsFetched(false), expanded(false), readOnlyAliasGroup(nullptr)
{
}

AliasGroupSymbol::AliasGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), aliasTypeSymbolsFetched(false), expanded(false), readOnlyAliasGroup(nullptr)
{
}

void AliasGroupSymbol::GetAliasTypeSymbols(Context* context)
{
    if (aliasTypeSymbolsFetched) return;
    aliasTypeSymbolsFetched = true;
    for (SymbolId aliasTypeSymbolId : aliasTypeSymbolIds)
    {
        AliasTypeSymbol* aliasTypeSymbol = GetModule()->GetSymbolTable()->GetAliasTypeSymbol(aliasTypeSymbolId, context);
        if (aliasTypeSymbol)
        {
            aliasTypeSymbols.push_back(aliasTypeSymbol);
            aliasTypeSymbol->SetGroup(this);
        }
        else
        {
            ThrowException("alias type symbol '" + Name() + "' id " + std::to_string(ToUnderlying(aliasTypeSymbolId)) + " not found from module '" + 
                GetModule()->Name() + "'");
        }
    }
}

bool AliasGroupSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::templateDeclarationScope:
    case ScopeKind::classScope:
    case ScopeKind::blockScope:
    {
        return true;
    }
    }
    return false;
}

Symbol* AliasGroupSymbol::GetSingleSymbol(Context* context) 
{
    if (IsReadOnly())
    {
        GetAliasTypeSymbols(context);
    }
    else
    {
        Expand(context);
    }
    if (readOnlyAliasGroup)
    {
        return readOnlyAliasGroup->GetSingleSymbol(context);
    }
    if (aliasTypeSymbols.size() == 1)
    {
        Symbol* front = aliasTypeSymbols.front();
        return front;
    }
    else
    {
        return this;
    }
}

void AliasGroupSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    Cardinality count = Cardinality(aliasTypeSymbols.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (AliasTypeSymbol* aliasTypeSymbol : aliasTypeSymbols)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(aliasTypeSymbol->Id()));
    }
}

void AliasGroupSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId aliasTypeSymbolId = SymbolId(reader.CurrentReader().ReadUInt());
        aliasTypeSymbolIds.push_back(aliasTypeSymbolId);
    }
}

void AliasGroupSymbol::AddAliasTypeSymbol(AliasTypeSymbol* aliasTypeSymbol, Context* context)
{
    ReferredTypeEqual referredTypeEqual(aliasTypeSymbol, context);
    if (std::find(aliasTypeSymbols.begin(), aliasTypeSymbols.end(), aliasTypeSymbol) == aliasTypeSymbols.end() &&
        std::find_if(aliasTypeSymbols.begin(), aliasTypeSymbols.end(), referredTypeEqual) == aliasTypeSymbols.end())
    {
        aliasTypeSymbol->SetGroup(this);
        aliasTypeSymbols.push_back(aliasTypeSymbol);
    }
}

AliasTypeSymbol* AliasGroupSymbol::GetAliasTypeSymbol(Cardinality arity, Context* context) 
{
    if (IsReadOnly())
    {
        GetAliasTypeSymbols(context);
    }
    else
    {
        Expand(context);
    }
    if (readOnlyAliasGroup)
    {
        return readOnlyAliasGroup->GetAliasTypeSymbol(arity, context);
    }
    for (AliasTypeSymbol* aliasTypeSymbol : aliasTypeSymbols)
    {
        if (aliasTypeSymbol->Arity(context) == arity)
        {
            return aliasTypeSymbol;
        }
    }
    return nullptr;
}

struct ViableAliasTypeGreater
{
    bool operator()(const std::pair<AliasTypeSymbol*, int>& left, const std::pair<AliasTypeSymbol*, int>& right) noexcept
    {
        return left.second > right.second;
    }
};

AliasTypeSymbol* AliasGroupSymbol::GetBestMatchingAliasType(const std::vector<Symbol*>& templateArgs, Context* context) noexcept
{
    if (IsReadOnly())
    {
        GetAliasTypeSymbols(context);
    }
    else
    {
        Expand(context);
    }
    if (readOnlyAliasGroup)
    {
        return readOnlyAliasGroup->GetBestMatchingAliasType(templateArgs, context);
    }
    std::vector<std::pair<AliasTypeSymbol*, int>> viableAliasTypes;
    Cardinality arity = Cardinality(templateArgs.size());
    for (AliasTypeSymbol* alias : aliasTypeSymbols)
    {
        if (alias->Arity(context) == arity)
        {
            bool added = false;
            if (arity == Cardinality(1))
            {
                if (templateArgs[0]->IsTypeSymbol())
                {
                    TypeSymbol* templateArgType = static_cast<TypeSymbol*>(templateArgs[0]);
                    if (TypesEqual(alias->ReferredType(context), templateArgType, context))
                    {
                        viableAliasTypes.push_back(std::make_pair(alias, 1));
                        added = true;
                    }
                }
            }
            if (!added)
            {
                viableAliasTypes.push_back(std::make_pair(alias, 0));
            }
        }
    }
    std::sort(viableAliasTypes.begin(), viableAliasTypes.end(), ViableAliasTypeGreater());
    if (!viableAliasTypes.empty())
    {
        return viableAliasTypes[0].first;
    }
    return nullptr;
}

void AliasGroupSymbol::RemoveAliasType(AliasTypeSymbol* aliasType)
{
    aliasTypeSymbols.erase(std::remove(aliasTypeSymbols.begin(), aliasTypeSymbols.end(), aliasType), aliasTypeSymbols.end());
}

bool AliasGroupSymbol::IsEmpty() const
{
    return aliasTypeSymbols.empty() && aliasTypeSymbolIds.empty();
}

void AliasGroupSymbol::Expand(Context* context)
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
            AliasGroupSymbol* aliasGroup = module->GetSymbolTable()->GetAliasGroupSymbol(symbolId, context);
            if (aliasGroup)
            {
                if (!readOnlyAliasGroup || readOnlyAliasGroup->IsEmpty())
                {
                    readOnlyAliasGroup = aliasGroup;
                }
                else if (!aliasGroup->IsEmpty())
                {
                    Symbol* symbol = readOnlyAliasGroup->GetSingleSymbol(context);
                    Symbol* prev = aliasGroup->GetSingleSymbol(context);
                    if (symbol != prev)
                    {
                        symbol = readOnlyAliasGroup->GetSingleSymbol(context);
                        prev = aliasGroup->GetSingleSymbol(context);
                        ThrowException("alias type '" + symbol->Name() + "' not unique", symbol->GetFullSpan(), prev->GetFullSpan(), context);
                    }
                }
            }
            else
            {
                //ThrowException("alias group symbol " + std::to_string(ToUnderlying(symbolId)) + " not found from module " + module->Name());
            }
        }
        else
        {
            ThrowException("import module " + std::to_string(ToUnderlying(moduleId)) + " not found from alias group '" + FullName(context) +
                "' of module " + GetModule()->Name());
        }
    }
}

bool AliasGroupSymbol::IsExportSymbol(Context* context) const noexcept
{
    return Symbol::IsExportSymbol(context) && ContainsExportAliasType(context);
}

bool AliasGroupSymbol::ContainsExportAliasType(Context* context) const noexcept
{
    for (AliasTypeSymbol* aliasTypeSymbol : aliasTypeSymbols)
    {
        if (aliasTypeSymbol->IsExportSymbol(context)) return true;
    }
    return false;
}

} // namespace otava::symbols
