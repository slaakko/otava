// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.variable_symbol;

namespace otava::symbols {

VariableSymbol::VariableSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), level(0), foundFromParent(false), nodeId(-1), temporary(false), value(nullptr)

{
}

VariableSymbol::VariableSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), level(0), foundFromParent(false), nodeId(-1), temporary(false), value(nullptr)
{
}

bool VariableSymbol::IsGlobalVariable(Context* context) const noexcept
{
    return Parent(context)->IsNamespaceSymbol();
}

TypeSymbol* VariableSymbol::GetDeclaredType() const noexcept
{
    // TODO
    return nullptr;
}

void VariableSymbol::SetDeclaredType(TypeSymbol* declaredType_) noexcept
{
    // TODO
}

TypeSymbol* VariableSymbol::GetInitializerType() const noexcept
{
    // TODO
    return nullptr;
}

void VariableSymbol::SetInitializerType(TypeSymbol* initializerType_) noexcept
{
    // TODO
}

TypeSymbol* VariableSymbol::GetType() const noexcept
{
    // TODO
    return nullptr;
}

TypeSymbol* VariableSymbol::GetReferredType() const noexcept
{
    // TODO
    return nullptr;
}

ParameterSymbol::ParameterSymbol(Module* module_, SymbolId id_) :
    Symbol(module_, id_), defaultValue(nullptr), type(nullptr), parameterKind(ParameterKind::regular)
{
}

ParameterSymbol::ParameterSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), defaultValue(nullptr), type(nullptr), parameterKind(ParameterKind::regular)
{
}

} // namespace otava::symbols
