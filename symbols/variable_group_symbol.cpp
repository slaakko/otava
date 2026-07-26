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
    Symbol(module_, id_), variablesFetched(false)
{
}

VariableGroupSymbol::VariableGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), variablesFetched(false)
{
}

VariableGroupSymbol::~VariableGroupSymbol()
{
    for (VariableSymbol* variable : variables)
    {
        variable->ResetGroup();
    }
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
        variableSymbol->SetGroup(this);
        variables.push_back(variableSymbol);
    }
}

void VariableGroupSymbol::ResetVariables()
{
    variables.clear();
    variablesFetched = false;
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

const std::vector<VariableSymbol*>& VariableGroupSymbol::Variables(Context* context)
{
    if (IsReadOnly())
    {
        GetVariables(context);
    }
    return variables;
}

VariableSymbol* VariableGroupSymbol::GetVariable(int arity, Context* context)
{
    if (IsReadOnly())
    {
        GetVariables(context);
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
        SymbolId variableId = SymbolId(reader.CurrentReader().ReadULong());
        variableIds.push_back(variableId);
    }
}

} // namespace otava::symbols
