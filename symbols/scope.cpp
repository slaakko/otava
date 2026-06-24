// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.scope;

import otava.symbols.alias_group_symbol;
import otava.symbols.class_templates;
import otava.symbols.context;
import otava.symbols.enum_group_symbol;
import otava.symbols.exception;
import otava.symbols.namespaces;
import otava.symbols.function_group_symbol;
import otava.symbols.modules;
import otava.symbols.symbol;
import otava.symbols.templates;
import otava.symbols.variable_group_symbol;
import otava.ast.error;
import otava.assembly.error;
import otava.intermediate.error;
import otava.optimizer.error;
import util.utility;

namespace otava::symbols {

bool compileEnded = false;

void SetCompileEnded()
{
    compileEnded = true;
}

void ResetCompileEnded()
{
    compileEnded = false;
}

bool CompileEnded()
{
    return compileEnded || otava::ast::ExceptionThrown() || otava::assembly::ExceptionThrown() || otava::intermediate::ExceptionThrown() ||
        otava::optimizer::ExceptionThrown() || otava::symbols::ExceptionThrown();
}

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

std::string SymbolGroupStr(SymbolGroupKind group)
{
    std::string groupStr;
    if ((group & SymbolGroupKind::functionSymbolGroup) != SymbolGroupKind::none)
    {
        if (!groupStr.empty())
        {
            groupStr.append(" | ");
        }
        groupStr.append("functionSymbolGroup");
    }
    if ((group & SymbolGroupKind::aliasSymbolGroup) != SymbolGroupKind::none)
    {
        if (!groupStr.empty())
        {
            groupStr.append(" | ");
        }
        groupStr.append("aliasSymbolGroup");
    }
    if ((group & SymbolGroupKind::classSymbolGroup) != SymbolGroupKind::none)
    {
        if (!groupStr.empty())
        {
            groupStr.append(" | ");
        }
        groupStr.append("classSymbolGroup");
    }
    if ((group & SymbolGroupKind::enumSymbolGroup) != SymbolGroupKind::none)
    {
        if (!groupStr.empty())
        {
            groupStr.append(" | ");
        }
        groupStr.append("enumSymbolGroup");
    }
    if ((group & SymbolGroupKind::variableSymbolGroup) != SymbolGroupKind::none)
    {
        if (!groupStr.empty())
        {
            groupStr.append(" | ");
        }
        groupStr.append("variableSymbolGroup");
    }
    if ((group & SymbolGroupKind::namespaceSymbolGroup) != SymbolGroupKind::none)
    {
        if (!groupStr.empty())
        {
            groupStr.append(" | ");
        }
        groupStr.append("namespaceSymbolGroup");
    }
    return groupStr;
}

std::vector<SymbolGroupKind> SymbolGroupKindstoSymbolGroupKindVec(SymbolGroupKind symbolGroupKinds)
{
    std::vector<SymbolGroupKind> symbolGroupKindVec;
    if ((symbolGroupKinds & SymbolGroupKind::functionSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::functionSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::aliasSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::aliasSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::classSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::classSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::enumSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::enumSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::templateParamSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::templateParamSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::variableSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::variableSymbolGroup);
    }
    if ((symbolGroupKinds & SymbolGroupKind::namespaceSymbolGroup) != SymbolGroupKind::none)
    {
        symbolGroupKindVec.push_back(SymbolGroupKind::namespaceSymbolGroup);
    }
    return symbolGroupKindVec;
}

Scope::Scope(Module* module_) noexcept : module(module_), readOnly(module->IsReadOnly()), kind(ScopeKind::none), read(false), global(false), imported(false), 
    destructing(false)
{
    module->AddScope(this);
}

Scope::~Scope()
{ 
    destructing = true;
    if (module)
    {
        module->RemoveScope(this);
    }
}

Symbol* Scope::Lookup(const std::string& name, SymbolGroupKind symbolGroupKind, ScopeLookup scopeLookup, const soul::ast::FullSpan& fullSpan, Context* context,
    LookupFlags flags) 
{
    if (!module) return nullptr;
    if (IsReadOnly())
    {
        Read();
    }
    std::vector<Symbol*> symbols;
    std::set<const Scope*> visited;
    Lookup(name, symbolGroupKind, scopeLookup, flags, symbols, visited, context);
    if (symbols.empty() && !IsReadOnly() && (flags & LookupFlags::dontImport) == LookupFlags::none)
    {
        if (IsGlobal())
        {
            ImportModuleScopes(context);
            Lookup(name, symbolGroupKind, scopeLookup, flags, symbols, visited, context);
        }
    }
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
        std::string errorMessage = "ambiguous reference to symbol '" + name + "': could be:\n";
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

void Scope::Lookup(const std::string& name, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
    std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) 
{
    if (!module) return;
    if (IsReadOnly())
    {
        Read();
    }
    StringOffset stringOffset = module->GetStringTable()->GetOffset(name);
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
                    if (it != symbolIdMap.end())
                    {
                        SymbolId symbolId = it->second;
                        if ((flags & LookupFlags::noFwdDeclarationSymbol) != LookupFlags::none && IsForwardDeclarationSymbol(symbolId))
                        {
                            continue;
                        }
                        Symbol* symbol = GetModule()->GetSymbolTable()->GetSymbol(symbolId, context);
                        if (symbol)
                        {
                            if (std::find(symbols.begin(), symbols.end(), symbol) == symbols.end())
                            {
                                symbols.push_back(symbol);
                            }
                        }
                    }
                    auto mit = moduleSymbolIdMap.find(symbolOffset);
                    if (mit != moduleSymbolIdMap.end())
                    {
                        ModuleSymbolId moduleSymbolId = mit->second;
                        if ((flags & LookupFlags::noFwdDeclarationSymbol) != LookupFlags::none && IsForwardDeclarationSymbol(moduleSymbolId.symbolId))
                        {
                            continue;
                        }
                        Module* module = context->GetModuleMapper()->GetModule(moduleSymbolId.moduleId);
                        if (module)
                        {
                            Symbol* symbol = module->GetSymbolTable()->GetSymbol(moduleSymbolId.symbolId, context);
                            if (symbol)
                            {
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
    }
}

void Scope::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot declare symbol '" + symbol->Name() + "' in " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
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

void Scope::RemoveParentScope(Scope* parentScope)
{
    SetExceptionThrown();
    throw std::runtime_error("could not remove parent scope");
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
    ThrowException("cannot add base class scope to " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
}

void Scope::AddUsingDeclaration(Symbol* usingDeclaration, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add using declaration '" + usingDeclaration->FullName(context) + "' to " + ScopeKindStr(kind) + " '" +
        FullName(context) + "'", fullSpan, context);
}

void Scope::AddUsingDirective(NamespaceSymbol* ns, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add using directive '" + ns->FullName(context) + "' to " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
}

NamespaceSymbol* Scope::GetOrInsertNamespace(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add class namespace '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
    return nullptr;
}

ClassGroupSymbol* Scope::GetOrInsertClassGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add class group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
    return nullptr;
}

FunctionGroupSymbol* Scope::GetOrInsertFunctionGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add function group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
    return nullptr;
}

VariableGroupSymbol* Scope::GetOrInsertVariableGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add variable group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
    return nullptr;
}

AliasGroupSymbol* Scope::GetOrInsertAliasGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add alias group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
    return nullptr;
}

EnumGroupSymbol* Scope::GetOrInsertEnumGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add enum group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
    return nullptr;
}

TemplateParamGroupSymbol* Scope::GetOrInsertTemplateParamGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("cannot add template parameter group '" + name + "' to " + ScopeKindStr(kind) + " '" + FullName(context) + "'", fullSpan, context);
    return nullptr;
}

void Scope::Write(Writer& writer)
{
    std::vector<std::pair<SymbolOffset, SymbolId>> symbolIdVec;
    for (const auto& s : symbolIdMap)
    {
        SymbolOffset symbolOffset = s.first;
        SymbolGroupKind groupKind = GetSymbolGroupKind(symbolOffset);
        symbolIdVec.push_back(s);
    }
    Cardinality count = Cardinality(symbolIdVec.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (const auto& s : symbolIdVec)
    {
        SymbolOffset symbolOffset = s.first;
        SymbolId symbolId = s.second;
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolOffset));
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
    }
    writer.GetBinaryStreamWriter().Write(global);
}

void Scope::Read()
{
    if (read || !IsReadOnly()) return;
    read = true;
    Symbol* symbol = GetSymbol();
    if (symbol && symbol->IsContainerSymbol())
    {
        ContainerSymbol* containerSymbol = static_cast<ContainerSymbol*>(symbol);
        Reader reader(GetModule()->GetFileMapping());
        reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(containerSymbol->ScopeOffset())), containerSymbol->ScopeLength());
        Read(reader);
        reader.PopCurrentReader();
    }
}

void Scope::Read(Reader& reader)
{
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolOffset symbolOffset = SymbolOffset(reader.CurrentReader().ReadUInt());
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        symbolIdMap[symbolOffset] = symbolId;
    }
    global = reader.CurrentReader().ReadBool();
}

void Scope::Install(Symbol* symbol, Context* context)
{
    StringOffset stringOffset = GetModule()->GetStringTable()->AddString(symbol->Name());
    SymbolGroupKind symbolGroupKind = symbol->GetSymbolGroupKind();
    SymbolOffset symbolOffset = MakeSymbolOffset(symbolGroupKind, stringOffset);
    if (symbol->GetModule() == GetModule())
    {
        symbolIdMap[symbolOffset] = symbol->Id();
    }
    else
    {
        moduleSymbolIdMap[symbolOffset] = ModuleSymbolId(symbol->GetModule()->Id(), symbol->Id());
    }
}

void Scope::Uninstall(Symbol* symbol)
{
    // TODO
}

void Scope::ImportModuleScopes(Context* context)
{
    if (imported) return;
    imported = true;
    const std::vector<Module*>& importedModules = GetModule()->AllImportedModules(context);
    for (Module* m : importedModules)
    {
        Scope* thatScope = m->GetSymbolTable()->GetGlobalNs(context)->GetScope();
        Import(thatScope, context);
    }
}

bool Scope::Imported(Scope* scope) noexcept
{ 
    return importSet.find(scope) != importSet.end();
}

void Scope::AddImported(Scope* scope)
{
    importSet.insert(scope);
}

void Scope::Import(Scope* that, Context* context)
{
    if (Imported(that)) return;
    that->Read();
    for (const auto& symbolOffsetId : that->symbolIdMap)
    {
        SymbolOffset symbolOffset = symbolOffsetId.first;
        SymbolId symbolId = symbolOffsetId.second;
        SymbolGroupKind symbolGroupKind = GetSymbolGroupKind(symbolOffset);
        switch (symbolGroupKind)
        {
        case SymbolGroupKind::namespaceSymbolGroup:
        {
            StringOffset stringOffset = GetStringOffset(symbolOffset);
            std::string name = that->GetModule()->GetStringTable()->GetString(stringOffset);
            NamespaceSymbol* ns = GetOrInsertNamespace(name, soul::ast::FullSpan(), context);
            ns->AddModuleSymbolId(ModuleSymbolId(that->GetModule()->Id(), symbolId));
            break;
        }
        case SymbolGroupKind::aliasSymbolGroup:
        {
            StringOffset stringOffset = GetStringOffset(symbolOffset);
            std::string name = that->GetModule()->GetStringTable()->GetString(stringOffset);
            AliasGroupSymbol* aliasGroup = GetOrInsertAliasGroup(name, soul::ast::FullSpan(), context);
            aliasGroup->AddModuleSymbolId(ModuleSymbolId(that->GetModule()->Id(), symbolId));
            break;
        }
        case SymbolGroupKind::functionSymbolGroup:
        {
            StringOffset stringOffset = GetStringOffset(symbolOffset);
            std::string name = that->GetModule()->GetStringTable()->GetString(stringOffset);
            FunctionGroupSymbol* functionGroup = GetOrInsertFunctionGroup(name, soul::ast::FullSpan(), context);
            functionGroup->AddModuleSymbolId(ModuleSymbolId(that->GetModule()->Id(), symbolId));
            break;
        }
        case SymbolGroupKind::classSymbolGroup:
        {
            StringOffset stringOffset = GetStringOffset(symbolOffset);
            std::string name = that->GetModule()->GetStringTable()->GetString(stringOffset);
            ClassGroupSymbol* classGroup = GetOrInsertClassGroup(name, soul::ast::FullSpan(), context);
            classGroup->AddModuleSymbolId(ModuleSymbolId(that->GetModule()->Id(), symbolId));
            break;
        }
        case SymbolGroupKind::enumSymbolGroup:
        {
            StringOffset stringOffset = GetStringOffset(symbolOffset);
            std::string name = that->GetModule()->GetStringTable()->GetString(stringOffset);
            EnumGroupSymbol* enumGroup = GetOrInsertEnumGroup(name, soul::ast::FullSpan(), context);
            enumGroup->AddModuleSymbolId(ModuleSymbolId(that->GetModule()->Id(), symbolId));
            break;
        }
        case SymbolGroupKind::templateParamSymbolGroup:
        {
            StringOffset stringOffset = GetStringOffset(symbolOffset);
            std::string name = that->GetModule()->GetStringTable()->GetString(stringOffset);
            TemplateParamGroupSymbol* templateParamGroup = GetOrInsertTemplateParamGroup(name, soul::ast::FullSpan(), context);
            templateParamGroup->AddModuleSymbolId(ModuleSymbolId(that->GetModule()->Id(), symbolId));
            break;
        }
        case SymbolGroupKind::variableSymbolGroup:
        {
            StringOffset stringOffset = GetStringOffset(symbolOffset);
            std::string name = that->GetModule()->GetStringTable()->GetString(stringOffset);
            VariableGroupSymbol* variableGroup = GetOrInsertVariableGroup(name, soul::ast::FullSpan(), context);
            variableGroup->AddModuleSymbolId(ModuleSymbolId(that->GetModule()->Id(), symbolId));
            break;
        }
        }
    }
    if (!IsContainerScope())
    {
        AddImported(that);
    }
}

Scope* Scope::GroupScope(Context* context) noexcept
{
    if (IsTemplateDeclarationScope())
    {
        std::vector<Scope*> pscopes = ParentScopes(context);
        return pscopes.front();
    }
    else
    {
        return this;
    }
}

Scope* Scope::SymbolScope(Context* context) noexcept
{
    return this;
}

void Scope::AddContainerScope(Scope* containerScope)
{
    if (std::find(containerScopes.begin(), containerScopes.end(), containerScope) == containerScopes.end())
    {
        containerScopes.push_back(containerScope);
    }
}

void Scope::RemoveContainerScope(Scope* containerScope)
{
    if (destructing) return;
    containerScopes.erase(std::remove(containerScopes.begin(), containerScopes.end(), containerScope), containerScopes.end());
}

ContainerScope::ContainerScope(Module* module_) noexcept : 
    Scope(module_), parentScopes(), usingDeclarationScope(nullptr), containerSymbol(nullptr), parentScopePushed(false), destructing(false)
{
}

ContainerScope::~ContainerScope()
{
    destructing = true;
    if (CompileEnded()) return;
    for (Scope* parentScope : pscopes)
    {
        parentScope->RemoveContainerScope(this);
    }
}

std::vector<Scope*> ContainerScope::ParentScopes(Context* context) 
{
    Symbol* parent = containerSymbol->Parent(context);
    if (parent)
    {
        Scope* pscope = parent->GetScope();
        if (std::find(parentScopes.begin(), parentScopes.end(), pscope) == parentScopes.end())
        {
            parentScopes.push_back(pscope);
        }
        if (std::find(pscopes.begin(), pscopes.end(), pscope) == pscopes.end())
        {
            pscopes.push_back(pscope);
        }
    }
    return parentScopes;
}

std::vector<Scope*> ContainerScope::BaseScopes(Context* context)
{
    if (containerSymbol->IsClassTypeSymbol())
    {
        ClassTypeSymbol* cls = static_cast<ClassTypeSymbol*>(containerSymbol);
        for (ClassTypeSymbol* baseClass : cls->BaseClasses(context))
        {
            Scope* baseClassScope = baseClass->GetScope();
            if (std::find(baseScopes.begin(), baseScopes.end(), baseClassScope) == baseScopes.end())
            {
                baseScopes.push_back(baseClassScope);
            }
        }
    }
    return baseScopes;
}

void ContainerScope::AddParentScope(Scope* parentScope) 
{
    if (parentScope)
    {
        if (std::find(parentScopes.begin(), parentScopes.end(), parentScope) == parentScopes.end())
        {
            parentScopes.push_back(parentScope);
        }
    }
}

void ContainerScope::RemoveParentScope(Scope* parentScope)
{
    parentScopes.erase(std::remove(parentScopes.begin(), parentScopes.end(), parentScope), parentScopes.end());
}

void ContainerScope::PushParentScope(Scope* parentScope)
{
    if (!parentScopePushed)
    {
        parentScopePushed = true;
        parentScopes.push_back(parentScope);
    }
}

void ContainerScope::PopParentScope()
{
    if (parentScopePushed)
    {
        parentScopePushed = false;
        parentScopes.pop_back();
    }
}

void ContainerScope::ClearParentScopes()
{
    parentScopes.clear();
}

Scope* ContainerScope::GetClassScope(Context* context) const noexcept
{
    if (Kind() == ScopeKind::classScope)
    {
        return const_cast<Scope*>(static_cast<const Scope*>(this));
    }
    for (Scope* parentScope : parentScopes)
    {
        Scope* classScope = parentScope->GetClassScope(context);
        if (classScope)
        {
            return classScope;
        }
    }
    Symbol* p = containerSymbol;
    while (p)
    {
        if (p->IsClassTypeSymbol())
        {
            return p->GetScope();
        }
        p = p->Parent(context);
    }
    return nullptr;
}

Scope* ContainerScope::GetNamespaceScope(Context* context) const noexcept
{
    if (Kind() == ScopeKind::namespaceScope)
    {
        return const_cast<Scope*>(static_cast<const Scope*>(this));
    }
    for (Scope* parentScope : parentScopes)
    {
        Scope* namespaceScope = parentScope->GetNamespaceScope(context);
        if (namespaceScope)
        {
            return namespaceScope;
        }
    }
    Symbol* p = containerSymbol;
    while (p)
    {
        if (p->IsNamespaceSymbol())
        {
            return p->GetScope();
        }
        p = p->Parent(context);
    }
    return nullptr;
}

void ContainerScope::AddBaseScope(Scope* baseScope, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (baseScope->IsContainerScope())
    {
        ContainerScope* containerScope = static_cast<ContainerScope*>(baseScope);
        ContainerSymbol* baseClassSymbol = containerScope->GetContainerSymbol();
        Install(baseClassSymbol, context);
    }
    baseScopes.push_back(baseScope);
}

Symbol* ContainerScope::GetSymbol() noexcept
{
    return containerSymbol;
}

ClassTemplateSpecializationSymbol* ContainerScope::GetClassTemplateSpecialization(std::set<Scope*>& visited) const
{
    if (containerSymbol->IsClassTemplateSpecializationSymbol())
    {
        return static_cast<ClassTemplateSpecializationSymbol*>(containerSymbol);
    }
    else
    {
        for (Scope* parentScope : parentScopes)
        {
            if (visited.find(parentScope) == visited.end())
            {
                visited.insert(parentScope);
                ClassTemplateSpecializationSymbol* sp = parentScope->GetClassTemplateSpecialization(visited);
                if (sp)
                {
                    return sp;
                }
            }
        }
    }
    return nullptr;
}

void ContainerScope::AddUsingDeclaration(Symbol* usingDeclaration, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!usingDeclarationScope)
    {
        usingDeclarationScope = new UsingDeclarationScope(GetModule(), this);
        scopes.push_back(std::unique_ptr<Scope>(usingDeclarationScope));
    }
    usingDeclarationScope->Install(usingDeclaration, context);
}

void ContainerScope::AddUsingDirective(NamespaceSymbol* ns, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (Kind() == ScopeKind::namespaceScope || Kind() == ScopeKind::blockScope)
    {
        for (UsingDirectiveScope* scope : usingDirectiveScopes)
        {
            if (scope->Ns() == ns) return;
        }
        UsingDirectiveScope* usingDirectiveScope = new UsingDirectiveScope(GetModule(), ns);
        scopes.push_back(std::unique_ptr<Scope>(usingDirectiveScope));
        usingDirectiveScopes.push_back(usingDirectiveScope);
    }
    else
    {
        ThrowException("cannot add using directive to " + ScopeKindStr(Kind()), fullSpan, context);
    }
}

std::string ContainerScope::FullName(Context* context) const
{
    if (Kind() == ScopeKind::blockScope)
    {
        return std::string();
    }
    return containerSymbol->FullName(context);
}

void ContainerScope::Import(Scope* that, Context* context)
{
    if (Imported(that)) return;
    Scope::Import(that, context);
    if (that->IsContainerScope())
    {
        ContainerScope* thatContainerScope = static_cast<ContainerScope*>(that);
        if (thatContainerScope->usingDeclarationScope)
        {
            const std::unordered_map<SymbolOffset, SymbolId>& symbolIdMap = thatContainerScope->usingDeclarationScope->SymbolIdMap();
            for (const auto& s : symbolIdMap)
            {
                SymbolId symbolId = s.second;
                Symbol* usingDeclaration = that->GetModule()->GetSymbolTable()->GetSymbol(symbolId, context);
                AddUsingDeclaration(usingDeclaration, soul::ast::FullSpan(), context);
            }
        }
    }
    AddImported(that);
}

void ContainerScope::Lookup(const std::string& name, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
    std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) 
{
    if (!GetModule()) return;
    if (IsReadOnly())
    {
        Scope::Read();
    }
    else
    {
        containerSymbol->Expand(context);
    }
    Scope::Lookup(name, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
    if ((scopeLookup & ScopeLookup::parentScope) != ScopeLookup::none)
    {
        for (Scope* parentScope : ParentScopes(context))
        {
            if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
            {
                parentScope->Lookup(name, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
            }
        }
    }
    if ((scopeLookup & ScopeLookup::baseScope) != ScopeLookup::none)
    {
        if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
        {
            for (Scope* baseScope : BaseScopes(context))
            {
                baseScope->Lookup(name, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
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
                    usingDeclarationScope->Lookup(name, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
                }
            }
            for (Scope* usingDirectiveScope : usingDirectiveScopes)
            {
                if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
                {
                    usingDirectiveScope->Lookup(name, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
                }
            }
        }
    }
}

void ContainerScope::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!symbol->IsValidDeclarationScope(Kind()))
    {
        ThrowException("cannot declare symbol '" + symbol->Name() + "' in " + ScopeKindStr(Kind()) + " '" + FullName(context) + "'", fullSpan, context);
    }
    if (IsReadOnly())
    {
        context->GetModule()->GetSymbolTable()->GlobalNs()->AddSymbol(symbol, fullSpan, context);
        symbol->SetParent(containerSymbol);
    }
    else
    {
        containerSymbol->AddSymbol(symbol, fullSpan, context);
    }
}

std::unique_ptr<Symbol> ContainerScope::RemoveSymbol(Symbol* symbol)
{
    // TODO
    return std::unique_ptr<Symbol>();
}

NamespaceSymbol* ContainerScope::GetOrInsertNamespace(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* symbol = Scope::Lookup(name, SymbolGroupKind::namespaceSymbolGroup, ScopeLookup::thisScope, fullSpan, context, 
        LookupFlags::dontResolveSingle | LookupFlags::dontImport);
    if (symbol)
    {
        if (symbol->Kind() == SymbolKind::namespaceSymbol)
        {
            NamespaceSymbol* ns = static_cast<NamespaceSymbol*>(symbol);
            return ns;
        }
    }
    NamespaceSymbol* ns = new NamespaceSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::namespaceSymbol), name);
    context->GetModule()->AddNamespaceId(ns->Id());
    AddSymbol(ns, fullSpan, context);
    return ns;
}

ClassGroupSymbol* ContainerScope::GetOrInsertClassGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* symbol = Scope::Lookup(name, 
        SymbolGroupKind::classSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
    if (symbol)
    {
        if (symbol->Kind() == SymbolKind::classGroupSymbol)
        {
            ClassGroupSymbol* classGroupSymbol = static_cast<ClassGroupSymbol*>(symbol);
            return classGroupSymbol;
        }
    }
    ClassGroupSymbol* classGroupSymbol = new ClassGroupSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::classGroupSymbol), name);
    AddSymbol(classGroupSymbol, fullSpan, context);
    return classGroupSymbol;
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

VariableGroupSymbol* ContainerScope::GetOrInsertVariableGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* symbol = Scope::Lookup(name, SymbolGroupKind::variableSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
    if (symbol)
    {
        if (symbol->Kind() == SymbolKind::variableGroupSymbol)
        {
            VariableGroupSymbol* variableGroupSymbol = static_cast<VariableGroupSymbol*>(symbol);
            return variableGroupSymbol;
        }
    }
    VariableGroupSymbol* variableGroupSymbol = new VariableGroupSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::variableGroupSymbol), name);
    AddSymbol(variableGroupSymbol, fullSpan, context);
    return variableGroupSymbol;
}

AliasGroupSymbol* ContainerScope::GetOrInsertAliasGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* symbol = Scope::Lookup(name, SymbolGroupKind::aliasSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
    if (symbol)
    {
        if (symbol->Kind() == SymbolKind::aliasGroupSymbol)
        {
            AliasGroupSymbol* aliasGroupSymbol = static_cast<AliasGroupSymbol*>(symbol);
            return aliasGroupSymbol;
        }
    }
    AliasGroupSymbol* aliasGroupSymbol = new AliasGroupSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::aliasGroupSymbol), name);
    AddSymbol(aliasGroupSymbol, fullSpan, context);
    return aliasGroupSymbol;
}

EnumGroupSymbol* ContainerScope::GetOrInsertEnumGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* symbol = Scope::Lookup(name, 
        SymbolGroupKind::enumSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
    if (symbol)
    {
        if (symbol->Kind() == SymbolKind::enumGroupSymbol)
        {
            EnumGroupSymbol* enumGroupSymbol = static_cast<EnumGroupSymbol*>(symbol);
            return enumGroupSymbol;
        }
    }
    EnumGroupSymbol* enumGroupSymbol = new EnumGroupSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::enumGroupSymbol), name);
    AddSymbol(enumGroupSymbol, fullSpan, context);
    return enumGroupSymbol;
}

TemplateParamGroupSymbol* ContainerScope::GetOrInsertTemplateParamGroup(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* symbol = Scope::Lookup(name, 
        SymbolGroupKind::templateParamSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
    if (symbol)
    {
        if (symbol->Kind() == SymbolKind::templateParamGroupSymbol)
        {
            TemplateParamGroupSymbol* templateParamGroupSymbol = static_cast<TemplateParamGroupSymbol*>(symbol);
            return templateParamGroupSymbol;
        }
    }
    TemplateParamGroupSymbol* templateParamGroupSymbol = new TemplateParamGroupSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::templateParamGroupSymbol), name);
    AddSymbol(templateParamGroupSymbol, fullSpan, context);
    return templateParamGroupSymbol;
}

bool ContainerScope::HasParentScope(const Scope* parentScope) const noexcept
{
    if (parentScope == this)
    {
        return true;
    }
    for (Scope* scope : parentScopes)
    {
        if (scope->HasParentScope(parentScope)) return true;
    }
    return false;
}

void ContainerScope::Write(Writer& writer)
{
    Scope::Write(writer);
    Cardinality count = Cardinality(scopes.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (const auto& scope : scopes)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(scope->Kind()));
        scope->Write(writer);
    }
}

void ContainerScope::Read(Reader& reader)
{
    Scope::Read(reader);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        ScopeKind scopeKind = ScopeKind(reader.CurrentReader().ReadByte());
        Scope* scope = MakeScope(GetModule(), scopeKind, this);
        if (scope)
        {
            scopes.push_back(std::unique_ptr<Scope>(scope));
            scope->Read(reader);
            switch (scopeKind)
            {
                case ScopeKind::usingDeclarationScope:
                {
                    usingDeclarationScope = static_cast<UsingDeclarationScope*>(scope);
                    break;
                }
                case ScopeKind::usingDirectiveScope:
                {
                    usingDirectiveScopes.push_back(static_cast<UsingDirectiveScope*>(scope));
                    break;
                }
                default:
                {
                    ThrowException("ContainerScope::Read: invalid scope kind " + ScopeKindStr(scopeKind) + " read");
                    break;
                }
            }
        }
        else
        {
            ThrowException("ContainerScope::Read: invalid scope kind " + ScopeKindStr(scopeKind) + " read");
        }
    }
}

UsingDeclarationScope::UsingDeclarationScope(Module* module_, ContainerScope* parentScope_) noexcept : Scope(module_), parentScope(parentScope_)
{
    SetKind(ScopeKind::usingDeclarationScope);
}

std::string UsingDeclarationScope::FullName(Context* context) const
{
    return parentScope->FullName(context);
}

void UsingDeclarationScope::Lookup(const std::string& name, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
    std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) 
{
    if (!GetModule()) return;
    if (IsReadOnly())
    {
        Scope::Read();
    }
    Scope::Lookup(name, symbolGroupKinds, ScopeLookup::thisScope, flags, symbols, visited, context);
}

UsingDirectiveScope::UsingDirectiveScope(Module* module_) noexcept : Scope(module_), ns(nullptr), nsId(zeroSymbolId)
{
}

UsingDirectiveScope::UsingDirectiveScope(Module* module_, NamespaceSymbol* ns_) noexcept : Scope(module_), ns(ns_), nsId(zeroSymbolId)
{
    SetKind(ScopeKind::usingDirectiveScope);
}

void UsingDirectiveScope::Write(Writer& writer)
{
    Scope::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(ns->Id()));
}

void UsingDirectiveScope::Read(Reader& reader)
{
    Scope::Read(reader);
    nsId = SymbolId(reader.CurrentReader().ReadUInt());
}

void UsingDirectiveScope::Lookup(const std::string& name, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
    std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) 
{
    if (!GetModule()) return;
    if (IsReadOnly())
    {
        Scope::Read();
    }
    if (IsReadOnly() && !ns)
    {
        if (nsId != zeroSymbolId)
        {
            ns = context->GetSymbolTable()->GetNamespaceSymbol(nsId, context);
        }
        if (!ns)
        {
            ThrowException("UsingDirectiveScope::Lookup: namespace symbol for id " + std::to_string(ToUnderlying(nsId)) + " not found");
        }
    }
    if (visited.find(ns->GetScope()) == visited.end())
    {
        visited.insert(ns->GetScope());
        ns->GetScope()->Lookup(name, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
    }
}

std::string UsingDirectiveScope::FullName(Context* context) const
{
    return ns->FullName(context);
}

InstantiationScope::InstantiationScope(Module* module_, Scope* parentScope_) noexcept : Scope(module_)
{
    SetKind(ScopeKind::instantiationScope);
    parentScopes.push_back(parentScope_);
}

std::string InstantiationScope::FullName(Context* context) const
{
    Scope* first = *parentScopes.begin();
    return first->FullName(context);
}

Scope* InstantiationScope::GroupScope(Context* context) noexcept
{
    Scope* first = *parentScopes.begin();
    return first->GroupScope(context);
}

Scope* InstantiationScope::SymbolScope(Context* context) noexcept
{
    Scope* first = *parentScopes.begin();
    return first->SymbolScope(context);
}

void InstantiationScope::PushParentScope(Scope* parentScope_)
{
    parentScopes.insert(parentScopes.begin(), parentScope_);
}

void InstantiationScope::PopParentScope()
{
    parentScopes.erase(parentScopes.begin());
}

bool InstantiationScope::HasParentScope(const Scope* parentScope) const noexcept
{
    if (parentScope == this)
    {
        return true;
    }
    for (Scope* scope : parentScopes)
    {
        if (scope->HasParentScope(parentScope)) return true;
    }
    return false;
}

ClassTemplateSpecializationSymbol* InstantiationScope::GetClassTemplateSpecialization(std::set<Scope*>& visited) const
{
    for (Scope* parentScope : parentScopes)
    {
        if (visited.find(parentScope) == visited.end())
        {
            visited.insert(parentScope);
            ClassTemplateSpecializationSymbol* sp = parentScope->GetClassTemplateSpecialization(visited);
            if (sp)
            {
                return sp;
            }
        }
    }
    return nullptr;
}

Scope* InstantiationScope::GetClassScope(Context* context) const noexcept
{
    for (Scope* parentScope : parentScopes)
    {
        Scope* classScope = parentScope->GetClassScope(context);
        if (classScope)
        {
            return classScope;
        }
    }
    return nullptr;
}

Scope* InstantiationScope::GetNamespaceScope(Context* context) const noexcept
{
    for (Scope* parentScope : parentScopes)
    {
        Scope* namespaceScope = parentScope->GetNamespaceScope(context);
        if (namespaceScope)
        {
            return namespaceScope;
        }
    }
    return nullptr;
}

void InstantiationScope::Lookup(const std::string& id, SymbolGroupKind symbolGroupKinds, ScopeLookup scopeLookup, LookupFlags flags,
    std::vector<Symbol*>& symbols, std::set<const Scope*>& visited, Context* context) 
{
    if (!GetModule()) return;
    std::vector<Symbol*> foundSymbols;
    Scope::Lookup(id, symbolGroupKinds, ScopeLookup::thisScope, flags, foundSymbols, visited, context);
    for (Symbol* symbol : foundSymbols)
    {
        if (symbol->IsBoundTemplateParameterSymbol())
        {
            BoundTemplateParameterSymbol* boundTemplateParameterSymbol = static_cast<BoundTemplateParameterSymbol*>(symbol);
            symbols.push_back(boundTemplateParameterSymbol->BoundSymbol());
        }
        else
        {
            symbols.push_back(symbol);
        }
    }
    if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
    {
        if ((scopeLookup & ScopeLookup::parentScope) != ScopeLookup::none)
        {
            for (Scope* parentScope : parentScopes)
            {
                if (symbols.empty() || (flags & LookupFlags::all) != LookupFlags::none)
                {
                    if (visited.find(parentScope) == visited.end())
                    {
                        visited.insert(parentScope);
                        parentScope->Lookup(id, symbolGroupKinds, scopeLookup, flags, symbols, visited, context);
                    }
                }
            }
        }
    }
}

Scope* MakeScope(Module* module, ScopeKind scopeKind, ContainerScope* parentScope)
{
    switch (scopeKind)
    {
        case ScopeKind::usingDeclarationScope:
        {
            return new UsingDeclarationScope(module, parentScope);
        }
        case ScopeKind::usingDirectiveScope:
        {
            return new UsingDirectiveScope(module);
        }
    }
    return nullptr;
}

} // namespace otava::symbols
