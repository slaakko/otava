// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.scope;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.namespaces;
import otava.symbols.function_group_symbol;
import otava.symbols.symbol;

namespace otava::symbols {

std::string ScopeKindStr(ScopeKind kind)
{
    switch (kind)
    {
    case ScopeKind::templateDeclarationScope: return "template declaration scope";
    case ScopeKind::namespaceScope: return "namespace scope";
    case ScopeKind::classScope: return "class scope";
    case ScopeKind::enumerationScope: return "enumeration scope";
    case ScopeKind::functionScope: return "function scope";
    case ScopeKind::arrayScope: return "array scope";
    case ScopeKind::blockScope: return "block scope";
    case ScopeKind::usingDeclarationScope: return "using declaration scope";
    case ScopeKind::usingDirectiveScope: return "using directive scope";
    case ScopeKind::instantiationScope: return "instantiation scope";
    }
    return "<unknown scope>";
}

std::vector<SymbolGroupKind> SymbolGroupKindstoSymbolGroupKindVec(SymbolGroupKind symbolGroupKinds)
{
    std::vector<SymbolGroupKind> symbolGroupKindVec;
    if ((symbolGroupKinds & SymbolGroupKind::functionSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::functionSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::typeSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::typeSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::variableSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::variableSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::enumConstantSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::enumConstantSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::conceptSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::conceptSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::blockSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::blockSymbolGroup);
    }
    return symbolGroupKindVec;
}

Scope::Scope(Module* module_) noexcept : module(module_)
{
}

Scope::~Scope()
{ 
}

Symbol* Scope::Lookup(const std::string& id, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, const soul::ast::FullSpan& fullSpan, Context* context,
    LookupFlags flags) const
{
    std::vector<Symbol*> symbols;
    std::set<const Scope*> visited;
    Lookup(id, symbolGroupKind, scopeLookup, flags, symbols, visited, context);
    if (symbols.empty())
    {
        return nullptr;
    }
    else if (symbols.size() == 1)
    {
        if ((flags & LookupFlags::dontResolveSingle) == LookupFlags::none)
        {
            Symbol* front = symbols.front();
            return front->GetSingleSymbol(context);
        }
        else
        {
            Symbol* front = symbols.front();
            return front;
        }
    }
    else
    {
        std::string errorMessage = "ambiguous reference to symbol '" + id + "': could be:\n";
        bool first = true;
        for (Symbol* symbol : symbols)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                errorMessage.append(" or\n");
            }
            errorMessage.append(symbol->FullName(context));
        }
        ThrowException(errorMessage, fullSpan, context);
    }
    return nullptr;
}

void Scope::Lookup(const std::string& id, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
    std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) const
{
    StringOffset stringOffset = module->GetStringTable()->GetOffset(id);
    if (stringOffset == notFoundOffset) return;
    if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
    {
        std::vector<SymbolGroupKind> symbolGroupKindVec = SymbolGroupKindstoSymbolGroupKindVec(symbolGroupKinds);
        for (SymbolGroupKind symbolGroupKind : symbolGroupKindVec)
        {
            if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
            {
                if ((scopeLookup & ScopeLookup::thisAndBaseScopes) != ScopeLookup::none)
                {
                    SymbolOffset symbolOffset = MakeSymbolOffset(symbolGroupKind, stringOffset);
                    auto it = symbolIdMap.find(symbolOffset);
                    if (it != symbolIdMap.cend())
                    {
                        SymbolId symbolId = it->second;
                        if ((flags & LookupFlags::noFwdDeclarationSymbol) != LookupFlags::none && IsForwardDeclarationSymbol(symbolId))
                        {
                            continue;
                        }
                        Symbol* symbol = context->GetSymbolTable()->GetSymbol(symbolId, context);
                        if (std::find(symbols.begin(), symbols.end(), symbol) == symbols.end())
                        {
                            symbols.push_back(symbol);
                        }
                    }
                }
            }
        }
    }
}

void Scope::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot declare symbol '" + symbol->Name() + "' in " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
}

std::unique_ptr<Symbol> Scope::RemoveSymbol(Symbol* symbol)
{
    SetExceptionThrown();
    throw std::runtime_error("could not remove symbol");
}

void Scope::AddParentScope(Scope* parentScope_)
{
    SetExceptionThrown();
    throw std::runtime_error("could not add parent scope");
}

void Scope::PushParentScope(Scope* parentScope)
{
    SetExceptionThrown();
    throw std::runtime_error("could not push parent scope");
}

void Scope::PopParentScope()
{
    SetExceptionThrown();
    throw std::runtime_error("could not pop parent scope");
}

void Scope::AddBaseScope(Scope* baseScope, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add base class scope to " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
}

void Scope::AddUsingDeclaration(Symbol* usingDeclaration, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add using declaration '" + usingDeclaration->FullName(context) + "' to " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
}

void Scope::AddUsingDirective(NamespaceSymbol* ns, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add using directive '" + ns->FullName(context) + "' to " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
}

ClassGroupSymbol* Scope::GetOrInsertClassGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add class group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
    return nullptr;
}

FunctionGroupSymbol* Scope::GetOrInsertFunctionGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add function group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
    return nullptr;
}

ConceptGroupSymbol* Scope::GetOrInsertConceptGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add concept group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
    return nullptr;
}

VariableGroupSymbol* Scope::GetOrInsertVariableGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add variable group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
    return nullptr;
}

AliasGroupSymbol* Scope::GetOrInsertAliasGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add alias group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
    return nullptr;
}

EnumGroupSymbol* Scope::GetOrInsertEnumGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add enum group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName() + "'", fullSpan, context);
    return nullptr;
}

void Scope::Install(Symbol* symbol)
{
    StringOffset stringOffset = GetModule()->GetStringTable()->AddString(symbol->Name());
    SymbolGroupKind symbolGroupKind = symbol->GetSymbolGroupKind();
    SymbolOffset symbolOffset = MakeSymbolOffset(symbolGroupKind, stringOffset);
    symbolIdMap[symbolOffset] = symbol->Id();
}

void Scope::Uninstall(Symbol* symbol)
{
    // TODO
}

Scope* Scope::GroupScope() noexcept
{
    if (IsTemplateDeclarationScope())
    {
        std::vector<Scope*> pscopes = ParentScopes();
        return pscopes.front();
    }
    else
    {
        return this;
    }
}

Scope* Scope::SymbolScope() noexcept
{
    return this;
}

ContainerScope::ContainerScope(Module* module_) noexcept : Scope(module_)
{
}

std::vector<Scope*> ContainerScope::ParentScopes() const 
{
    // TODO
    return std::vector<Scope*>();
}

void ContainerScope::AddParentScope(Scope* parentScope) 
{
    // TODO
}

void ContainerScope::PushParentScope(Scope* parentScope)
{
    // TODO
}

void ContainerScope::PopParentScope()
{
    // TODO
}

void ContainerScope::ClearParentScopes()
{
    // TODO
}

Scope* ContainerScope::GetClassScope() const noexcept
{
    // TODO
    return nullptr;
}

Scope* ContainerScope::GetNamespaceScope() const noexcept
{
    // TODO
    return nullptr;
}

void ContainerScope::AddBaseScope(Scope* baseScope, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
}

Symbol* ContainerScope::GetSymbol() noexcept
{
    // TODO
    return nullptr;
}

ClassTemplateSpecializationSymbol* ContainerScope::GetClassTemplateSpecialization(std::set<Scope*>& visited) const
{
    // TODO
    return nullptr;
}

void ContainerScope::AddUsingDeclaration(Symbol* usingDeclaration, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
}

void ContainerScope::AddUsingDirective(NamespaceSymbol* ns, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
}

std::string ContainerScope::FullName() const
{
    // TODO
    return std::string();
}

void ContainerScope::Lookup(const std::string& id, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
    std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) const
{
    Scope::Lookup(id, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
    if ((scopeLookup & ScopeLookup::parentScope) != ScopeLookup::none)
    {
        for (Scope* parentScope : parentScopes)
        {
            if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
            {
                parentScope->Lookup(id, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
            }
        }
    }
    if ((scopeLookup & ScopeLookup::baseScope) != ScopeLookup::none)
    {
        if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
        {
            for (Scope* baseScope : baseScopes)
            {
                baseScope->Lookup(id, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
            }
        }
    }
    if ((scopeLookup & ScopeLookup::usingScope) != ScopeLookup::none)
    {
        if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
        {
            if (usingDeclarationScope)
            {
                if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
                {
                    usingDeclarationScope->Lookup(id, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
                }
            }
            for (Scope* usingDirectiveScope : usingDirectiveScopes)
            {
                if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
                {
                    usingDirectiveScope->Lookup(id, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
                }
            }
        }
    }
}

void ContainerScope::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!symbol->IsValidDeclarationScope(Kind()))
    {
        ThrowException("cannot declare symbol '" + symbol->Name() + "' in " + ScopeKindStr(Kind()) + " '" + FullName() + "'", fullSpan, context);
    }
    containerSymbol->AddSymbol(symbol, fullSpan, context);
}

std::unique_ptr<Symbol> ContainerScope::RemoveSymbol(Symbol* symbol)
{
    // TODO
    return std::unique_ptr<Symbol>();
}

ClassGroupSymbol* ContainerScope::GetOrInsertClassGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

FunctionGroupSymbol* ContainerScope::GetOrInsertFunctionGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* symbol = Scope::Lookup(name, SymbolGroupKind::functionSymbolGroup, ScopeLookup::thisScope, 
        fullSpan, context, LookupFlags::dontResolveSingle);
    if (symbol)
    {
        if (symbol->Kind() == SymbolKind::functionGroupSymbol)
        {
            FunctionGroupSymbol* functionGroupSymbol = static_cast<FunctionGroupSymbol*>(symbol);
            return functionGroupSymbol;
        }
    }
    FunctionGroupSymbol* functionGroupSymbol = new FunctionGroupSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::functionGroupSymbol), name);
    AddSymbol(functionGroupSymbol, fullSpan, context);
    return functionGroupSymbol;
}

ConceptGroupSymbol* ContainerScope::GetOrInsertConceptGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

VariableGroupSymbol* ContainerScope::GetOrInsertVariableGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

AliasGroupSymbol* ContainerScope::GetOrInsertAliasGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

EnumGroupSymbol* ContainerScope::GetOrInsertEnumGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

bool ContainerScope::HasParentScope(const Scope* parentScope) const noexcept
{
    // TODO
    return false;
}

} // namespace otava::symbols
