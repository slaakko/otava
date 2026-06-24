// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.variable_group_symbol;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.variable_symbol;

namespace otava::symbols {

VariableGroupSymbol::VariableGroupSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), variablesFetched(false), expanded(false), readOnlyVariableGroup(nullptr)
{
}

VariableGroupSymbol::VariableGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), variablesFetched(false), expanded(false), readOnlyVariableGroup(nullptr)
{
}

bool VariableGroupSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
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

Symbol* VariableGroupSymbol::GetSingleSymbol(Context* context) 
{
    if (IsReadOnly())
    {
        GetVariables(context);
    }
    else
    {
        Expand(context);
    }
    if (readOnlyVariableGroup)
    {
        return readOnlyVariableGroup->GetSingleSymbol(context);
    }
    if (variables.size() == 1)
    {
        Symbol* front = variables.front();
        return front;
    }
    else
    {
        return this;
    }
}

void VariableGroupSymbol::AddVariable(VariableSymbol* variableSymbol)
{
    if (std::find(variables.begin(), variables.end(), variableSymbol) == variables.end())
    {
        variables.push_back(variableSymbol);
    }
}

void VariableGroupSymbol::GetVariables(Context* context)
{
    if (variablesFetched) return;
    variablesFetched = true;
    for (SymbolId variableId : variableIds)
    {
        VariableSymbol* variableSymbol = GetModule()->GetSymbolTable()->GetVariableSymbol(variableId, context);
        if (variableSymbol)
        {
            variables.push_back(variableSymbol);
        }
        else
        {
            ThrowException("variable id " + std::to_string(ToUnderlying(variableId)) + " not found");
        }
    }
}

bool VariableGroupSymbol::IsEmpty() const noexcept
{
    return variables.empty() && variableIds.empty();
}

void VariableGroupSymbol::Expand(Context* context)
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
            VariableGroupSymbol* variableGroup = module->GetSymbolTable()->GetVariableGroupSymbol(symbolId, context);
            if (variableGroup)
            {
                if (!readOnlyVariableGroup || readOnlyVariableGroup->IsEmpty())
                {
                    readOnlyVariableGroup = variableGroup;
                }
                else if (!variableGroup->IsEmpty())
                {
                    Symbol* symbol = readOnlyVariableGroup->GetSingleSymbol(context);
                    Symbol* prev = variableGroup->GetSingleSymbol(context);
                    if (symbol != prev)
                    {
                        symbol = readOnlyVariableGroup->GetSingleSymbol(context);
                        prev = variableGroup->GetSingleSymbol(context);
                        ThrowException("variable type '" + symbol->Name() + "' not unique", symbol->GetFullSpan(), prev->GetFullSpan(), context);
                    }
                }
            }
            else
            {
                ThrowException("variable group symbol " + std::to_string(ToUnderlying(symbolId)) + " not found from module " + module->Name());
            }
        }
        else
        {
            ThrowException("import module " + std::to_string(ToUnderlying(moduleId)) + " not found from variable group '" + FullName(context) +
                "' of module " + GetModule()->Name());
        }
    }
}

const std::vector<VariableSymbol*>& VariableGroupSymbol::Variables(Context* context)
{
    if (IsReadOnly())
    {
        GetVariables(context);
    }
    else
    {
        Expand(context);
    }
    if (readOnlyVariableGroup)
    {
        return readOnlyVariableGroup->Variables(context);
    }
    return variables;
}

VariableSymbol* VariableGroupSymbol::GetVariable(int arity, Context* context)
{
    if (IsReadOnly())
    {
        GetVariables(context);
    }
    else
    {
        Expand(context);
    }
    if (readOnlyVariableGroup)
    {
        return readOnlyVariableGroup->GetVariable(arity, context);
    }
    for (VariableSymbol* variable : variables)
    {
        if (variable->Arity() == arity)
        {
            return variable;
        }
    }
    return nullptr;
}

void VariableGroupSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    Cardinality count = Cardinality(variables.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (VariableSymbol* variable : variables)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(variable->Id()));
    }
}

void VariableGroupSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId variableId = SymbolId(reader.CurrentReader().ReadUInt());
        variableIds.push_back(variableId);
    }
}

} // namespace otava::symbols
