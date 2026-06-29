// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.symbol_table;

import otava.symbols.fundamental_type_symbol;
import otava.symbols.fundamental_type_operation;
import otava.symbols.modules;
import otava.symbols.context;
import otava.ast.error;
import otava.symbols.exception;
import otava.symbols.symbol_factory;
import otava.symbols.variable_group_symbol;
import otava.symbols.intrinsics;
import util.binary_stream_writer;
import util.memory_reader;
import util.rand;
import util.utility;

namespace otava::symbols {

SymbolTable::SymbolTable(Module* module_, bool readOnly_) :
    module(module_), readOnly(readOnly_), globalNs(), currentScope(nullptr), currentAccess(Access::none), currentLinkage(Linkage::cpp_linkage),
    typenameConstraintSymbol(nullptr), classLevel(0), conversionTable(module), symbolIdVectorRead(false), fundamentalTypeMapRead(false), 
    compoundTypeMapRead(false), aliasTypeTemplateMapRead(false), classTemplateSpecializationMapRead(false), functionTypeMapRead(false), 
    explicitInstantiationMapRead(false), arrayTypeMapRead(false)
{
    auto end = ToUnderlying(SectionKind::max);
    for (auto i = ToUnderlying(SectionKind::first); i != end; ++i)
    {
        Section* section = new Section(GetModule(), SectionKind(i));
        sectionMap[section->Kind()] = section;
        sections.push_back(std::unique_ptr<Section>(section));
    }
    if (readOnly)
    {
        SetSectionsReadOnly();
    }
}

void SymbolTable::SetSectionsReadOnly()
{
    for (auto& section : sections)
    {
        section->SetReadOnly();
    }
}

Section* SymbolTable::GetSection(SectionKind sectionKind) const noexcept
{
    auto it = sectionMap.find(sectionKind);
    if (it != sectionMap.end())
    {
        return it->second;
    }
    return nullptr;
}

Section* SymbolTable::GetSection(Symbol* forSymbol) const noexcept
{
    SectionKind sectionKind = otava::symbols::GetSectionKind(forSymbol);
    return GetSection(sectionKind);
}

NamespaceSymbol* SymbolTable::GetGlobalNs(Context* context)
{
    if (IsReadOnly())
    {
        const std::vector<SymbolId>& namespaceIds = GetModule()->NamespaceIds();
        if (!namespaceIds.empty())
        {
            NamespaceSymbol* globalNs = GetNamespaceSymbol(namespaceIds.front(), context);
            if (globalNs)
            {
                return globalNs;
            }
            else
            {
                ThrowException("global namespace for module " + GetModule()->Name() + "' not found");
            }
        }
    }
    else
    {
        return GlobalNs();
    }
    return nullptr;
}

Scope* SymbolTable::GetNamespaceScope(const std::string& nsName, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* ns = Lookup(nsName, SymbolGroupKind::namespaceSymbolGroup, fullSpan, context);
    if (ns)
    {
        return ns->GetScope();
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::AddClass(ClassTypeSymbol* cls)
{
    classes.insert(cls);
}

void SymbolTable::PushScope()
{
    scopeStack.push_back(currentScope);
}

void SymbolTable::PopScope()
{
    if (scopeStack.empty())
    {
        otava::ast::SetExceptionThrown();
        throw std::runtime_error("scope stack is empty");
    }
    currentScope = scopeStack.back();
    scopeStack.pop_back();
}

void SymbolTable::BeginScope(Scope* scope, Context* context)
{
    PushScope();
    currentScope = scope;
}

void SymbolTable::EndScope(Context* context)
{
    PopScope();
}

void SymbolTable::BeginScopeGeneric(Scope* scope, Context* context)
{
    if (context->GetFlag(ContextFlags::instantiateFunctionTemplate |
        ContextFlags::instantiateMemFnOfClassTemplate |
        ContextFlags::instantiateInlineFunction))
    {
        CurrentScope()->PushParentScope(scope);
    }
    else
    {
        BeginScope(scope, context);
    }
}

void SymbolTable::EndScopeGeneric(Context* context)
{
    if (context->GetFlag(ContextFlags::instantiateFunctionTemplate |
        ContextFlags::instantiateMemFnOfClassTemplate |
        ContextFlags::instantiateInlineFunction))
    {
        CurrentScope()->PopParentScope();
    }
    else
    {
        EndScope(context);
    }
}

void SymbolTable::PushTopScopeIndex()
{
    topScopeIndexStack.push(topScopeIndex);
    topScopeIndex = scopeStack.size() - 1;
}

void SymbolTable::PopTopScopeIndex()
{
    topScopeIndex = topScopeIndexStack.top();
    topScopeIndexStack.pop();
}

void SymbolTable::SetCurrentAccess(Access access)
{
    currentAccess = access;
}

void SymbolTable::PushAccess(Access access)
{
    accessStack.push(currentAccess);
    currentAccess = access;
}

void SymbolTable::PopAccess()
{
    currentAccess = accessStack.top();
    accessStack.pop();
}

Symbol* SymbolTable::Lookup(const std::string& name, SymbolGroupKind symbolGroupKind, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* symbol = Lookup(name, symbolGroupKind, fullSpan, context, LookupFlags::none);
    if (!symbol)
    {
        symbol = LookupInScopeStack(name, symbolGroupKind, fullSpan, context, LookupFlags::none);
    }
    if (!symbol && name == "nullptr_t")
    {
        return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::nullPtrType, context);
    }
    return symbol;
}

Symbol* SymbolTable::LookupInScopeStack(const std::string& name, SymbolGroupKind symbolGroupKind, const soul::ast::FullSpan& fullSpan,
    Context* context, LookupFlags flags)
{
    if (topScopeIndex == -1) return nullptr;
    for (int i = scopeStack.size() - 1; i >= topScopeIndex; --i)
    {
        Scope* scope = scopeStack[i];
        Symbol* symbol = scope->Lookup(name, symbolGroupKind, ScopeLookup::allScopes, fullSpan, context, flags);
        if (symbol)
        {
            return symbol;
        }
    }
    return nullptr;
}

Symbol* SymbolTable::Lookup(const std::string& name, SymbolGroupKind symbolGroupKind, const soul::ast::FullSpan& fullSpan, Context* context, LookupFlags flags)
{
    return currentScope->Lookup(name, symbolGroupKind, ScopeLookup::allScopes, fullSpan, context, flags);
}

Symbol* SymbolTable::LookupSymbol(Symbol* symbol, Context* context)
{
    std::vector<Symbol*> components;
    while (symbol)
    {
        if (!symbol->Name().empty())
        {
            components.push_back(symbol);
        }
        symbol = symbol->Parent(context);
    }
    Scope* scope = globalNs->GetScope();
    for (int i = components.size() - 1; i >= 0; --i)
    {
        if (!scope) break;
        Symbol* lookupSymbol = components[i];
        std::vector<Symbol*> symbols;
        std::set<const Scope*> visited;
        scope->Lookup(lookupSymbol->Name(), SymbolGroupKind::classSymbolGroup | SymbolGroupKind::namespaceSymbolGroup, 
            ScopeLookup::thisScope, LookupFlags::none, symbols, visited, nullptr);
        if (symbols.size() == 1)
        {
            Symbol* found = symbols.front();
            if (i == 0)
            {
                return found;
            }
            scope = found->GetScope();
        }
    }
    return nullptr;
}

void SymbolTable::CollectViableFunctions(const std::vector<std::pair<Scope*, ScopeLookup>>& scopeLookups, const std::string& groupName,
    const std::vector<TypeSymbol*>& templateArgs, Cardinality arity, std::vector<FunctionSymbol*>& viableFunctions, Context* context)
{
    std::vector<Symbol*> symbols;
    std::set<const Scope*> visited;
    for (const auto& p : scopeLookups)
    {
        Scope* scope = p.first;
        std::string scopeName = scope->FullName(context);
        ScopeLookup lookup = p.second;
        scope->Lookup(groupName, SymbolGroupKind::functionSymbolGroup, lookup, LookupFlags::dontResolveSingle | LookupFlags::all, symbols, visited, context);
        Scope* scp = scope;
        Scope* classScope = scp->GetClassScope(context);
        if (classScope)
        {
            Symbol* symbol = classScope->GetSymbol();
            if (symbol->IsClassTemplateSpecializationSymbol())
            {
                ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(symbol);
                scp = specialization->ClassTemplate(context)->GetScope();
            }
        }
        if (scp != scope)
        {
            scp->Lookup(groupName, SymbolGroupKind::functionSymbolGroup, lookup, LookupFlags::dontResolveSingle | LookupFlags::all, symbols, visited, context);
        }
    }
    for (Symbol* symbol : symbols)
    {
        if (symbol->IsFunctionGroupSymbol())
        {
            FunctionGroupSymbol* functionGroup = static_cast<FunctionGroupSymbol*>(symbol);
            functionGroup->CollectViableFunctions(arity, templateArgs, viableFunctions, context);
        }
    }
}

void SymbolTable::MapNode(otava::ast::Node* node)
{
    Symbol* symbol = currentScope->GetSymbol();
    MapNode(node, symbol);
}

void SymbolTable::MapNode(otava::ast::Node* node, Symbol* symbol)
{
    MapNode(node, symbol, MapKind::both);
}

void SymbolTable::MapNode(otava::ast::Node* node, Symbol* symbol, MapKind kind)
{
    if (!node) return;
    if (!symbol) return;
    Symbol* prevSymbol = nullptr;
    if (symbol->IsFunctionSymbol() && !symbol->IsFunctionDefinitionSymbol())
    {
        prevSymbol = GetSymbolNothrow(node);
    }
    if ((kind & MapKind::nodeToSymbol) != MapKind::none)
    {
        if (!prevSymbol)
        {
            nodeSymbolMap[node] = symbol;
        }
    }
    if ((kind & MapKind::symbolToNode) != MapKind::none)
    {
        symbolNodeMap[symbol] = node;
    }
/*  TODO
    if (symbol->Parent() && symbol->Parent()->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(symbol->Parent());
        specialization->SetProject();
        AddChangedClassTemplateSpecialization(specialization);
    }
*/
}

otava::ast::Node* SymbolTable::GetNodeNothrow(Symbol* symbol) const noexcept
{
    auto it = symbolNodeMap.find(symbol);
    if (it != symbolNodeMap.end())
    {
        return it->second;
    }
    return nullptr;
}

otava::ast::Node* SymbolTable::GetNode(Symbol* symbol) const
{
    otava::ast::Node* node = GetNodeNothrow(symbol);
    if (node)
    {
        return node;
    }
    else
    {
        otava::ast::SetExceptionThrown();
        throw std::runtime_error("node for symbol '" + symbol->Name() + "' not found");
    }
}

Symbol* SymbolTable::GetSymbolNothrow(otava::ast::Node* node) const noexcept
{
    auto it = nodeSymbolMap.find(node);
    if (it != nodeSymbolMap.end())
    {
        return it->second;
    }
    return nullptr;
}

Symbol* SymbolTable::GetSymbol(otava::ast::Node* node) const
{
    Symbol* symbol = GetSymbolNothrow(node);
    if (symbol)
    {
        return symbol;
    }
    else
    {
        otava::ast::SetExceptionThrown();
        throw std::runtime_error("symbol for node not found");
    }
}

VariableSymbol* SymbolTable::AddVariable(const std::string& name, otava::ast::Node* node, TypeSymbol* declaredType, TypeSymbol* initializerType,
    Value* value, DeclarationFlags flags, Context* context)
{
    VariableGroupSymbol* variableGroup = currentScope->GroupScope(context)->GetOrInsertVariableGroup(name, node->GetFullSpan(), context);
    VariableSymbol* variableSymbol = new VariableSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::variableSymbol), name);
    variableSymbol->SetAccess(CurrentAccess());
    variableSymbol->SetDeclaredType(declaredType, context);
    variableSymbol->SetInitializerType(initializerType, context);
    variableSymbol->SetValue(value);
    variableSymbol->SetDeclarationFlags(flags);
    variableSymbol->SetFullSpan(node->GetFullSpan());
    currentScope->SymbolScope(context)->AddSymbol(variableSymbol, node->GetFullSpan(), context);
    variableGroup->AddVariable(variableSymbol);
    return variableSymbol;
}

AliasTypeSymbol* SymbolTable::AddAliasType(otava::ast::Node* idNode, otava::ast::Node* aliasTypeNode, TypeSymbol* type, Context* context)
{
    std::string name = idNode->Str();
    AliasGroupSymbol* aliasGroup = currentScope->GroupScope(context)->GetOrInsertAliasGroup(name, idNode->GetFullSpan(), context);
    AliasTypeSymbol* aliasTypeSymbol = new AliasTypeSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::aliasTypeSymbol), name);
    aliasTypeSymbol->SetFullSpan(idNode->GetFullSpan());
    aliasTypeSymbol->SetReferredType(type);
    aliasTypeSymbol->SetAccess(currentAccess);
    aliasTypeSymbol->SetAstNodeId(aliasTypeNode->Id());
    currentScope->SymbolScope(context)->AddSymbol(aliasTypeSymbol, idNode->GetFullSpan(), context);
    aliasGroup->AddAliasTypeSymbol(aliasTypeSymbol, context);
    MapNode(aliasTypeNode, aliasTypeSymbol);
    return aliasTypeSymbol;
}

void SymbolTable::AddUsingDeclaration(otava::ast::Node* node, Symbol* symbol, Context* context)
{
    currentScope->SymbolScope(context)->AddUsingDeclaration(symbol, node->GetFullSpan(), context);
    MapNode(node, symbol, MapKind::nodeToSymbol);
}

void SymbolTable::AddUsingDirective(NamespaceSymbol* ns, otava::ast::Node* node, Context* context)
{
    currentScope->SymbolScope(context)->AddUsingDirective(ns, node->GetFullSpan(), context);
    MapNode(node, ns, MapKind::nodeToSymbol);
}

void SymbolTable::BeginNamespace(const std::string& name, otava::ast::Node* node, Context* context)
{
    soul::ast::FullSpan fullSpan;
    if (node)
    {
        fullSpan = node->GetFullSpan();
    }
    if (name.empty())
    {
        BeginScope(currentScope, context);
    }
    else
    {
        Symbol* symbol = currentScope->Lookup(name, SymbolGroupKind::namespaceSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
        if (symbol)
        {
            if (symbol->IsNamespaceSymbol())
            {
                NamespaceSymbol* namespaceSymbol = static_cast<NamespaceSymbol*>(symbol);
                if (node)
                {
                    MapNode(node, namespaceSymbol);
                }
                BeginScope(namespaceSymbol->GetScope(), context);
                return;
            }
            else
            {
                ThrowException("name of namespace '" + name + " conflicts with earlier declaration", fullSpan, context);
            }
        }
        NamespaceSymbol* namespaceSymbol = new NamespaceSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::namespaceSymbol), name);
        GetModule()->AddNamespaceId(namespaceSymbol->Id());
        namespaceSymbol->SetFullSpan(fullSpan);
        if (node)
        {
            MapNode(node, namespaceSymbol);
        }
        currentScope->SymbolScope(context)->AddSymbol(namespaceSymbol, fullSpan, context);
        BeginScope(namespaceSymbol->GetScope(), context);
    }
}

void SymbolTable::EndNamespace(Context* context)
{
    EndScope(context);
}

void SymbolTable::BeginNamespace(otava::ast::Node* node, Context* context)
{
    BeginNamespace(node->Str(), node, context);
}

void SymbolTable::EndNamespace(int level, Context* context)
{
    for (int i = 0; i < level; ++i)
    {
        EndNamespace(context);
    }
}

void SymbolTable::BeginClass(const std::string& name, ClassKind classKind, TypeSymbol* specialization, otava::ast::Node* node, Context* context)
{
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    Symbol* symbol = currentScope->Lookup(name, SymbolGroupKind::classSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
    ClassGroupSymbol* classGroup = currentScope->GroupScope(context)->GetOrInsertClassGroup(name, fullSpan, context);
    ClassTypeSymbol* classTypeSymbol = new ClassTypeSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::classTypeSymbol), name);
    classTypeSymbol->SetFullSpan(fullSpan);
    classTypeSymbol->SetLevel(classLevel++);
    AddSymbol(classTypeSymbol);
    AddClass(classTypeSymbol);
    classTypeSymbol->SetAccess(CurrentAccess());
    classTypeSymbol->SetClassKind(classKind);
    classTypeSymbol->SetSpecialization(specialization, context);
    classTypeSymbol->SetAstNodeId(node->Id());
    currentScope->SymbolScope(context)->AddSymbol(classTypeSymbol, fullSpan, context);
    classGroup->AddClass(classTypeSymbol, context);
    MapNode(node, classTypeSymbol);
    SetSpecifierNode(classTypeSymbol, node);
    BeginScope(classTypeSymbol->GetScope(), context);
    switch (classKind)
    {
    case ClassKind::class_:
    {
        PushAccess(Access::private_);
        break;
    }
    case ClassKind::struct_:
    case ClassKind::union_:
    {
        PushAccess(Access::public_);
        break;
    }
    }
}

void SymbolTable::AddBaseClass(ClassTypeSymbol* baseClass, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* symbol = currentScope->GetSymbol();
    if (symbol->IsClassTypeSymbol())
    {
        ClassTypeSymbol* classTypeSymbol = static_cast<ClassTypeSymbol*>(symbol);
        classTypeSymbol->AddBaseClass(baseClass, fullSpan, context);
    }
    else
    {
        ThrowException("class type symbol expected", fullSpan, context);
    }
}

void SymbolTable::EndClass(Context* context)
{
    --classLevel;
    PopAccess();
    EndScope(context);
}

void SymbolTable::AddForwardClassDeclaration(const std::string& name, ClassKind classKind, TypeSymbol* specialization, otava::ast::Node* node, Context* context)
{
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    Symbol* symbol = currentScope->Lookup(name, SymbolGroupKind::classSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
    ClassGroupSymbol* classGroup = currentScope->GroupScope(context)->GetOrInsertClassGroup(name, fullSpan, context);
    std::unique_ptr<ForwardClassDeclarationSymbol> forwardDeclarationSymbol(new ForwardClassDeclarationSymbol(GetModule(), 
        context->GetNextSymbolId(SymbolKind::forwardClassDeclarationSymbol), name));
    forwardDeclarationSymbol->SetAccess(CurrentAccess());
    forwardDeclarationSymbol->SetClassKind(classKind);
    forwardDeclarationSymbol->SetSpecialization(specialization, context);
    forwardDeclarationSymbol->SetParent(currentScope->SymbolScope(context)->GetSymbol());
    forwardDeclarationSymbol->SetFullSpan(fullSpan);
    ForwardClassDeclarationSymbol* fwdDeclaration = classGroup->GetForwardDeclaration(forwardDeclarationSymbol->Arity(context), context);
    if (!fwdDeclaration)
    {
        classGroup->AddForwardDeclaration(forwardDeclarationSymbol.get());
        Symbol* sym = forwardDeclarationSymbol.release();
        MapNode(node, sym);
        currentScope->SymbolScope(context)->AddSymbol(sym, fullSpan, context);
    }
}

void SymbolTable::AddFriend(const std::string& name, otava::ast::Node* node, Context* context)
{
/*
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    FriendSymbol* friendSymbol = new FriendSymbol(name);
    friendSymbol->SetFullSpan(fullSpan);
    currentScope->SymbolScope()->AddSymbol(friendSymbol, fullSpan, context);
    MapNode(node, friendSymbol);
*/
}

void SymbolTable::BeginEnumeratedType(const std::string& name, EnumTypeKind kind, TypeSymbol* underlyingType, otava::ast::Node* node, Context* context)
{
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    EnumGroupSymbol* enumGroup = currentScope->GroupScope(context)->GetOrInsertEnumGroup(name, fullSpan, context);
    EnumeratedTypeSymbol* enumType = enumGroup->GetEnumType(context);
    if (enumType)
    {
        ThrowException("enumerated type '" + name + "' not unique", fullSpan, context);
    }
    EnumeratedTypeSymbol* enumTypeSymbol = new EnumeratedTypeSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::enumTypeSymbol), name);
    enumTypeSymbol->SetAccess(CurrentAccess());
    enumTypeSymbol->SetEnumTypeKind(kind);
    enumTypeSymbol->SetUnderlyingType(underlyingType);
    enumTypeSymbol->SetFullSpan(fullSpan);
    currentScope->SymbolScope(context)->AddSymbol(enumTypeSymbol, fullSpan, context);
    MapNode(node, enumTypeSymbol);
    enumGroup->SetEnumType(enumTypeSymbol);
    BeginScope(enumTypeSymbol->GetScope(), context);
}

void SymbolTable::EndEnumeratedType(Context* context)
{
    EndScope(context);
}

void SymbolTable::AddForwardEnumDeclaration(const std::string& name, EnumTypeKind enumTypeKind, TypeSymbol* underlyingType, otava::ast::Node* node, Context* context)
{
/*
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    EnumGroupSymbol* enumGroup = currentScope->GroupScope()->GetOrInsertEnumGroup(name, fullSpan, context);
    if (enumGroup->GetForwardDeclaration())
    {
        return;
    }
    ForwardEnumDeclarationSymbol* forwardDeclarationSymbol = new ForwardEnumDeclarationSymbol(name);
    forwardDeclarationSymbol->SetAccess(CurrentAccess());
    forwardDeclarationSymbol->SetEnumTypeKind(enumTypeKind);
    forwardDeclarationSymbol->SetUnderlyingType(underlyingType);
    forwardDeclarationSymbol->SetFullSpan(fullSpan);
    Symbol* sym = forwardDeclarationSymbol;
    currentScope->SymbolScope()->AddSymbol(sym, fullSpan, context);
    enumGroup->SetForwardDeclaration(forwardDeclarationSymbol);
    MapNode(node, sym);
    forwardDeclarations.insert(sym);
    allForwardDeclarations.insert(sym);
*/
}

void SymbolTable::AddEnumerator(const std::string& name, Value* value, otava::ast::Node* node, Context* context)
{
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    EnumeratedTypeSymbol* enumTypeSymbol = nullptr;
    Scope* scope = currentScope->SymbolScope(context);
    if (scope->IsContainerScope())
    {
        ContainerScope* containerScope = static_cast<ContainerScope*>(scope);
        if (containerScope)
        {
            ContainerSymbol* containerSymbol = containerScope->GetContainerSymbol();
            if (containerSymbol && containerSymbol->IsEnumeratedTypeSymbol())
            {
                enumTypeSymbol = static_cast<EnumeratedTypeSymbol*>(containerSymbol);
                EnumTypeKind kind = enumTypeSymbol->GetEnumTypeKind();
                if (kind == EnumTypeKind::enum_)
                {
                    scope = enumTypeSymbol->Parent(context)->GetScope();
                }
            }
        }
    }
    if (!enumTypeSymbol)
    {
        ThrowException("could not add enumerator to symbol table: enumerated type not found", fullSpan, context);
    }
    EnumConstantSymbol* enumConstantSymbol = new EnumConstantSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::enumConstantSymbol), name);
    enumConstantSymbol->SetEnumType(enumTypeSymbol);
    enumConstantSymbol->SetValue(value);
    enumConstantSymbol->SetFullSpan(fullSpan);
    scope->AddSymbol(enumConstantSymbol, fullSpan, context);
    MapNode(node, enumConstantSymbol);
}

BlockSymbol* SymbolTable::BeginBlock(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BlockSymbol* blockSymbol = new BlockSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::blockSymbol), std::string());
    blockSymbol->SetFullSpan(fullSpan);
    currentScope->SymbolScope(context)->AddSymbol(blockSymbol, fullSpan, context);
    BeginScopeGeneric(blockSymbol->GetScope(), context);
    return blockSymbol;
}

void SymbolTable::EndBlock(Context* context)
{
    EndScopeGeneric(context);
}

void SymbolTable::RemoveBlock(Context* context)
{
    Symbol* symbol = currentScope->GetSymbol();
    if (symbol && symbol->IsBlockSymbol())
    {
        EndScope(context);
        currentScope->RemoveSymbol(symbol);
    }
    else
    {
        otava::ast::SetExceptionThrown();
        throw std::runtime_error("otava.symbols.symbol_table: block expected");
    }
}

void SymbolTable::BeginTemplateDeclaration(otava::ast::Node* node, Context* context)
{
    TemplateDeclarationSymbol* templateDeclarationSymbol = new TemplateDeclarationSymbol(GetModule(), 
        context->GetNextSymbolId(SymbolKind::templateDeclarationSymbol), std::string());
    templateDeclarationSymbol->SetFullSpan(node->GetFullSpan());
    currentScope->SymbolScope(context)->AddSymbol(templateDeclarationSymbol, node->GetFullSpan(), context);
    BeginScope(templateDeclarationSymbol->GetScope(), context);
}

void SymbolTable::EndTemplateDeclaration(Context* context)
{
    EndScope(context);
}

void SymbolTable::RemoveTemplateDeclaration(Context* context)
{
    Symbol* symbol = currentScope->GetSymbol();
    if (symbol && symbol->IsTemplateDeclarationSymbol())
    {
        EndScope(context);
        currentScope->RemoveSymbol(symbol);
    }
}

void SymbolTable::AddTemplateParameter(const std::string& name, otava::ast::Node* node, Symbol* constraint, int index,
    ParameterSymbol* parameter, otava::ast::Node* defaultTemplateArgNode, Context* context)
{
    soul::ast::FullSpan fullSpan;
    if (node)
    {
        fullSpan = node->GetFullSpan();
    }
    TemplateParamGroupSymbol* templateParamGroup = currentScope->GetOrInsertTemplateParamGroup(name, fullSpan, context);
    TemplateParameterSymbol* templateParamSymbol = templateParamGroup->GetTemplateParam(context);
    if (templateParamSymbol)
    {
        ThrowException("template parameter '" + name + "' not unique", fullSpan, context);
    }
    TemplateParameterSymbol* templateParameterSymbol = new TemplateParameterSymbol(
        context->GetModule(), context->GetNextSymbolId(SymbolKind::templateParameterSymbol), name);
    templateParameterSymbol->SetIndex(index);
    templateParameterSymbol->SetDefaultTemplateArg(defaultTemplateArgNode);
    templateParameterSymbol->SetFullSpan(node->GetFullSpan());
    if (parameter)
    {
        templateParameterSymbol->AddSymbol(parameter, node->GetFullSpan(), context);
    }
    currentScope->SymbolScope(context)->AddSymbol(templateParameterSymbol, node->GetFullSpan(), context);
    MapNode(node, templateParameterSymbol);
    templateParamGroup->SetTemplateParam(templateParameterSymbol);
}

FunctionSymbol* SymbolTable::AddFunction(const std::string& name, const std::vector<TypeSymbol*>& specialization, otava::ast::Node* node, FunctionKind kind,
    FunctionQualifiers qualifiers, DeclarationFlags flags, Context* context)
{
    std::string groupName = name;
    if (kind == FunctionKind::constructor)
    {
        groupName = "@constructor";
    }
    else if (kind == FunctionKind::destructor)
    {
        groupName = "@destructor";
    }
    soul::ast::FullSpan fullSpan;
    if (node)
    {
        fullSpan = node->GetFullSpan();
    }
    std::int64_t astNodeId = -1;
    if (node)
    {
        astNodeId = node->Id();
    }
    FunctionGroupSymbol* functionGroup = currentScope->GroupScope(context)->GetOrInsertFunctionGroup(groupName, fullSpan, context);
    FunctionSymbol* functionSymbol = new FunctionSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), name);
    functionSymbol->SetAstNodeId(astNodeId);
    functionSymbol->SetAccess(CurrentAccess());
    functionSymbol->SetFunctionKind(kind);
    functionSymbol->SetFunctionQualifiers(qualifiers);
    functionSymbol->SetLinkage(currentLinkage);
    functionSymbol->SetDeclarationFlags(flags);
    functionSymbol->SetFullSpan(fullSpan);
    if ((flags & (DeclarationFlags::inlineFlag | DeclarationFlags::constExprFlag)) != DeclarationFlags::none)
    {
        functionSymbol->SetInline();
    }
    functionSymbol->SetSpecialization(specialization);
    currentScope->SymbolScope(context)->AddSymbol(functionSymbol, fullSpan, context);
    functionGroup->AddFunction(functionSymbol);
    MapNode(node, functionSymbol);
    return functionSymbol;
}

void SymbolTable::AddFunctionSymbol(Scope* scope, FunctionSymbol* functionSymbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    FunctionGroupSymbol* functionGroup = currentScope->GroupScope(context)->GetOrInsertFunctionGroup(functionSymbol->GroupName(), fullSpan, context);
    functionSymbol->SetLinkage(currentLinkage);
    scope->SymbolScope(context)->AddSymbol(functionSymbol, fullSpan, context);
    functionGroup->AddFunction(functionSymbol);
}

FunctionDefinitionSymbol* SymbolTable::AddOrGetFunctionDefinition(Scope* scope, const std::string& name, const std::vector<TypeSymbol*>& specialization,
    const std::vector<TypeSymbol*>& parameterTypes, FunctionQualifiers qualifiers, FunctionKind kind, DeclarationFlags declarationFlags, otava::ast::Node* node,
    otava::ast::Node* functionNode, bool& get, Context* context)
{
    get = false;
    std::string groupName = name;
    Symbol* containerSymbol = scope->SymbolScope(context)->GetSymbol();
    if (containerSymbol && containerSymbol->SimpleName(context) == name)
    {
        kind = FunctionKind::constructor;
    }
    if (kind == FunctionKind::constructor)
    {
        groupName = "@constructor";
    }
    else if (kind == FunctionKind::destructor)
    {
        groupName = "@destructor";
    }
    FunctionGroupSymbol* functionGroup = scope->GroupScope(context)->GetOrInsertFunctionGroup(groupName, node->GetFullSpan(), context);
    std::unique_ptr<FunctionDefinitionSymbol> functionDefinition(new FunctionDefinitionSymbol(context->GetModule(),
        context->GetNextSymbolId(SymbolKind::functionDefinitionSymbol), name));
    functionDefinition->SetGroup(functionGroup);
    functionDefinition->SetDeclarationFlags(declarationFlags);
    if (context->GetFlag(ContextFlags::instantiateFunctionTemplate |
        ContextFlags::instantiateMemFnOfClassTemplate |
        ContextFlags::instantiateInlineFunction))
    {
        functionDefinition->SetSpecialization();
    }
    if ((declarationFlags & (DeclarationFlags::inlineFlag | DeclarationFlags::constExprFlag)) != DeclarationFlags::none)
    {
        functionDefinition->SetInline();
    }
    functionDefinition->SetLinkage(currentLinkage);
    functionDefinition->SetFunctionKind(kind);
    functionDefinition->SetFunctionQualifiers(qualifiers);
    functionDefinition->SetSpecialization(specialization);
    functionDefinition->SetParent(currentScope->SymbolScope(context)->GetSymbol());
    if (node)
    {
        functionDefinition->SetFullSpan(node->GetFullSpan());
    }
    int index = 0;
    for (auto* parameterType : parameterTypes)
    {
        functionDefinition->AddTemporaryParameter(parameterType, index++, context);
    }
    if (functionDefinition->IsVirtual())
    {
        ClassTemplateSpecializationSymbol* csp = context->GetClassTemplateSpecialization(functionNode);
        if (csp)
        {
            FunctionSymbol* prev = csp->GetMatchingVirtualFunctionSpecialization(functionDefinition.get(), context);
            if (prev && prev->IsFunctionDefinitionSymbol())
            {
                get = true;
                return static_cast<FunctionDefinitionSymbol*>(prev);
            }
        }
    }
    functionDefinition->ClearTemporaryParameters(); 
    MapNode(node, functionDefinition.get());
    if (context->MemFunDefSymbolIndex() != -1)
    {
        functionDefinition->SetDefIndex(context->MemFunDefSymbolIndex());
        context->SetMemFunDefSymbolIndex(-1);
    }
    FunctionDefinitionSymbol* definition = functionDefinition.get();
    currentScope->SymbolScope(context)->AddSymbol(functionDefinition.release(), node->GetFullSpan(), context);
    FunctionSymbol* declaration = functionGroup->ResolveFunction(parameterTypes, qualifiers, specialization, definition->ParentTemplateDeclaration(context),
        definition->IsSpecialization(), context);
    if (declaration)
    {
        definition->SetDeclaration(declaration, context);
        definition->SetAccess(declaration->GetAccess());
    }
    return definition;
}

ParameterSymbol* SymbolTable::CreateParameter(const std::string& name, otava::ast::Node* node, TypeSymbol* type, Context* context)
{
    ParameterSymbol* parameterSymbol = new ParameterSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), name);
    parameterSymbol->SetType(type, context);
    if (node)
    {
        MapNode(node, parameterSymbol);
        parameterSymbol->SetFullSpan(node->GetFullSpan());
    }
    return parameterSymbol;
}

otava::ast::Node* SymbolTable::GetSpecifierNode(Symbol* symbol) const noexcept
{
    auto it = specifierNodeMap.find(symbol);
    if (it != specifierNodeMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::SetSpecifierNode(Symbol* symbol, otava::ast::Node* node)
{
    specifierNodeMap[symbol] = node;
}

TypeSymbol* SymbolTable::GetCompoundType(TypeSymbol* baseType, Derivations derivations, Context* context)
{
    ReadCompoundTypeMaps();
    auto it = compoundTypeMap.find(CompoundTypeKey(baseType->Id(), derivations));
    if (it != compoundTypeMap.end())
    {
        SymbolId symbolId = it->second;
        return GetTypeSymbol(symbolId, context);
    }
    return nullptr;
}

void SymbolTable::SetIrId(CompoundTypeSymbol* compoundTypeSymbol, Context* context)
{
    CompoundTypeKey irKey = CompoundTypeKey(compoundTypeSymbol->GetBaseType(context)->IrId(), compoundTypeSymbol->GetDerivations());
    for (Module* importedModule : GetModule()->ImportExportModules(context))
    {
        auto it = importedModule->GetSymbolTable()->irCompoundTypeMap.find(irKey);
        if (it != importedModule->GetSymbolTable()->irCompoundTypeMap.end())
        {
            SymbolId irId = it->second;
            compoundTypeSymbol->SetIrId(irId);
            return;
        }
    }
    auto it = irCompoundTypeMap.find(irKey);
    if (it != irCompoundTypeMap.end())
    {
        SymbolId irId = it->second;
        compoundTypeSymbol->SetIrId(irId);
    }
    else
    {
        SymbolId irId = context->GetNextSymbolId(SymbolKind::compoundTypeSymbol);
        irCompoundTypeMap[irKey] = irId;
        compoundTypeSymbol->SetIrId(irId);
    }
}

TypeSymbol* SymbolTable::MakeCompoundType(TypeSymbol* baseType, Derivations derivations, Context* context)
{
    Derivations drv = derivations;
    if (baseType->IsCompoundTypeSymbol())
    {
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(baseType);
        drv = Merge(drv, compoundTypeSymbol->GetDerivations());
        baseType = compoundTypeSymbol->GetBaseType(context);
    }
    if (drv == Derivations::none)
    {
        return baseType;
    }
    TypeSymbol* compoundType = GetCompoundType(baseType, drv, context);
    if (compoundType)
    {
        return compoundType;
    }
    Module* baseTypeModule = baseType->GetModule();
    if (baseTypeModule != GetModule())
    {
        compoundType = baseTypeModule->GetSymbolTable()->GetCompoundType(baseType, drv, context);
        if (compoundType)
        {
            return compoundType;
        }
    }
    CompoundTypeKey key = CompoundTypeKey(baseType->Id(), drv);
    SymbolId symbolId = context->GetNextSymbolId(SymbolKind::compoundTypeSymbol);
    compoundTypeMap[key] = symbolId;
    CompoundTypeSymbol* compoundTypeSymbol = new CompoundTypeSymbol(module, symbolId, MakeCompoundTypeName(baseType, drv));
    compoundTypeSymbol->SetBaseType(baseType);
    compoundTypeSymbol->SetDerivations(drv);
    SetIrId(compoundTypeSymbol, context);
    GlobalNs()->GetScope()->AddSymbol(compoundTypeSymbol, soul::ast::FullSpan(), context);
    return compoundTypeSymbol;
}

AliasTypeTemplateSpecializationSymbol* SymbolTable::GetAliasTypeTemplateSpecialization(TypeSymbol* aliasTypeTemplate, const std::vector<Symbol*>& templateArguments,
    Context* context)
{
    ReadAliasTypeTemplateMap();
    SpecializationKey key;
    key.typeSymbolId = aliasTypeTemplate->Id();
    for (Symbol* templateArg : templateArguments)
    {
        key.templateArgumentIds.push_back(templateArg->Id());
    }
    auto it = aliasTypeTemplateMap.find(key);
    if (it != aliasTypeTemplateMap.end())
    {
        SymbolId symbolId = it->second;
        AliasTypeTemplateSpecializationSymbol* aliasTypeTemplateSpecializationSymbol = GetAliasTypeTemplateSpecializationSymbol(symbolId, context);
        if (!aliasTypeTemplateSpecializationSymbol)
        {
            ThrowException("alias type template specialization symbol id " + std::to_string(ToUnderlying(symbolId)) + " not found from module '" + 
                GetModule()->Name() + "'");
        }
        return aliasTypeTemplateSpecializationSymbol;
    }
    return nullptr;
}

AliasTypeTemplateSpecializationSymbol* SymbolTable::MakeAliasTypeTemplateSpecialization(TypeSymbol* aliasTypeTemplate, const std::vector<Symbol*>& templateArguments, 
    Context* context)
{
    AliasTypeTemplateSpecializationSymbol* aliasTypeTemplateSpecialization = GetAliasTypeTemplateSpecialization(aliasTypeTemplate, templateArguments, context);
    if (aliasTypeTemplateSpecialization)
    {
        return aliasTypeTemplateSpecialization;
    }
    Module* aliasTemplateModule = aliasTypeTemplate->GetModule();
    if (aliasTemplateModule != GetModule())
    {
        aliasTypeTemplateSpecialization = aliasTemplateModule->GetSymbolTable()->GetAliasTypeTemplateSpecialization(aliasTypeTemplate, templateArguments, context);
        if (aliasTypeTemplateSpecialization)
        {
            return aliasTypeTemplateSpecialization;
        }
    }
    for (Module* importedModule : GetModule()->ImportExportModules(context))
    {
        aliasTypeTemplateSpecialization = importedModule->GetSymbolTable()->GetAliasTypeTemplateSpecialization(aliasTypeTemplate, templateArguments, context);
        if (aliasTypeTemplateSpecialization)
        {
            return aliasTypeTemplateSpecialization;
        }
    }
    SpecializationKey key;
    key.typeSymbolId = aliasTypeTemplate->Id();
    for (Symbol* templateArg : templateArguments)
    {
        key.templateArgumentIds.push_back(templateArg->Id());
    }
    SymbolId symbolId = context->GetNextSymbolId(SymbolKind::aliasTypeTemplateSpecializationSymbol);
    aliasTypeTemplateMap[key] = symbolId;
    aliasTypeTemplateSpecialization = new AliasTypeTemplateSpecializationSymbol(module, symbolId, MakeSpecializationName(aliasTypeTemplate, templateArguments));
    aliasTypeTemplateSpecialization->SetAliasTypeTemplate(aliasTypeTemplate);
    for (Symbol* templateArgument : templateArguments)
    {
        aliasTypeTemplateSpecialization->AddTemplateArgument(templateArgument);
    }
    GlobalNs()->GetScope()->AddSymbol(aliasTypeTemplateSpecialization, soul::ast::FullSpan(), context);
    return aliasTypeTemplateSpecialization;
}

ClassTemplateSpecializationSymbol* SymbolTable::GetClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArguments,
    Context* context)
{
    ReadClassTemplateSpecializationMaps();
    SpecializationKey key;
    key.typeSymbolId = classTemplate->Id();
    for (Symbol* templateArg : templateArguments)
    {
        key.templateArgumentIds.push_back(templateArg->Id());
    }
    auto it = classTemplateSpecializationMap.find(key);
    if (it != classTemplateSpecializationMap.end())
    {
        SymbolId symbolId = it->second;
        ClassTemplateSpecializationSymbol* classtemplateSpecializationSymbol = GetClassTemplateSpecializationSymbol(symbolId, context);
        if (!classtemplateSpecializationSymbol)
        {
            ThrowException("class template specialization symbol id " + std::to_string(ToUnderlying(symbolId)) + " not found from module '" + GetModule()->Name() + "'");
        }
        return classtemplateSpecializationSymbol;
    }
    return nullptr;
}

void SymbolTable::SetIrId(ClassTemplateSpecializationSymbol* specialization, Context* context)
{
    SpecializationKey irKey;
    irKey.typeSymbolId = specialization->ClassTemplate(context)->IrId();
    for (Symbol* templateArg : specialization->TemplateArguments(context))
    {
        irKey.templateArgumentIds.push_back(templateArg->IrId());
    }
    for (Module* importedModule : GetModule()->ImportExportModules(context))
    {
        auto it = importedModule->GetSymbolTable()->irClassTemplateSpecializationMap.find(irKey);
        if (it != importedModule->GetSymbolTable()->irClassTemplateSpecializationMap.end())
        {
            SymbolId irId = it->second;
            specialization->SetIrId(irId);
            return;
        }
    }
    auto it = irClassTemplateSpecializationMap.find(irKey);
    if (it != irClassTemplateSpecializationMap.end())
    {
        SymbolId irId = it->second;
        specialization->SetIrId(irId);
    }
    else
    {
        SymbolId irId = context->GetNextSymbolId(SymbolKind::classTemplateSpecializationSymbol);
        irClassTemplateSpecializationMap[irKey] = irId;
        specialization->SetIrId(irId);
    }
}

ClassTemplateSpecializationSymbol* SymbolTable::MakeClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArguments,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    return MakeClassTemplateSpecialization(classTemplate, templateArguments, fullSpan, context, false);
}

ClassTemplateSpecializationSymbol* SymbolTable::MakeClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArguments,
    const soul::ast::FullSpan& fullSpan, Context* context, bool createNew)
{
    ClassTemplateSpecializationSymbol* classTemplateSpecialization = GetClassTemplateSpecialization(classTemplate, templateArguments, context);
    if (classTemplateSpecialization)
    {
        if (!classTemplateSpecialization->IsReadOnly() || !createNew)
        {
            return classTemplateSpecialization;
        }
    }
    Module* classTemplateModule = classTemplate->GetModule();
    if (classTemplateModule != GetModule())
    {
        classTemplateSpecialization = classTemplateModule->GetSymbolTable()->GetClassTemplateSpecialization(classTemplate, templateArguments, context);
        if (classTemplateSpecialization)
        {
            if (!classTemplateSpecialization->IsReadOnly() || !createNew)
            {
                return classTemplateSpecialization;
            }
        }
    }
    for (Module* importedModule : GetModule()->ImportExportModules(context))
    {
        classTemplateSpecialization = importedModule->GetSymbolTable()->GetClassTemplateSpecialization(classTemplate, templateArguments, context);
        if (classTemplateSpecialization)
        {
            if (!classTemplateSpecialization->IsReadOnly() || !createNew)
            {
                return classTemplateSpecialization;
            }
        }
    }
    SpecializationKey key;
    key.typeSymbolId = classTemplate->Id();
    for (Symbol* templateArg : templateArguments)
    {
        key.templateArgumentIds.push_back(templateArg->Id());
    }
    SymbolId symbolId = context->GetNextSymbolId(SymbolKind::classTemplateSpecializationSymbol);
    classTemplateSpecializationMap[key] = symbolId;
    classTemplateSpecialization = new ClassTemplateSpecializationSymbol(module, symbolId, MakeSpecializationName(classTemplate, templateArguments));
    classTemplateSpecialization->SetClassTemplate(classTemplate, context);
    for (Symbol* templateArgument : templateArguments)
    {
        classTemplateSpecialization->AddTemplateArgument(templateArgument, context);
    }
    SetIrId(classTemplateSpecialization, context);
    GlobalNs()->GetScope()->AddSymbol(classTemplateSpecialization, fullSpan, context);
    return classTemplateSpecialization;
}

ExplicitInstantiationSymbol* SymbolTable::GetExplicitInstantiation(const SpecializationKey& key, Context* context)
{
    return GetExplicitInstantiation(key, context, 1);
}

ExplicitInstantiationSymbol* SymbolTable::GetExplicitInstantiation(const SpecializationKey& key, Context* context, int level)
{
    ReadExplicitInstantiationMap();
    auto it = explicitInstantiationMap.find(key);
    if (it != explicitInstantiationMap.end())
    {
        SymbolId symbolId = it->second;
        ExplicitInstantiationSymbol* explicitInstantiationSymbol = GetExplicitInstantiationSymbol(symbolId, context);
        if (!explicitInstantiationSymbol)
        {
            ThrowException("explicit instantiation symbol id " + std::to_string(ToUnderlying(symbolId)) + " not found from module '" + GetModule()->Name() + "'");
        }
        return explicitInstantiationSymbol;
    }
    if (level > 0)
    {
        for (Module* importedModule : GetModule()->ImportExportModules(context))
        {
            ExplicitInstantiationSymbol* explicitInstantiationSymbol = importedModule->GetSymbolTable()->GetExplicitInstantiation(key, context, level - 1);
            if (explicitInstantiationSymbol)
            {
                return explicitInstantiationSymbol;
            }
        }
    }
    return nullptr;
}

void SymbolTable::AddExplicitInstantiation(ExplicitInstantiationSymbol* explicitInstantiationSymbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    SymbolId symbolId = explicitInstantiationSymbol->Id();
    SpecializationKey key;
    key.typeSymbolId = explicitInstantiationSymbol->Specialization(context)->ClassTemplate(context)->Id();
    for (Symbol* templateArg : explicitInstantiationSymbol->Specialization(context)->TemplateArguments(context))
    {
        key.templateArgumentIds.push_back(templateArg->Id());
    }
    explicitInstantiationMap[key] = symbolId;
    GlobalNs()->GetScope()->AddSymbol(explicitInstantiationSymbol, fullSpan, context);
}

FunctionTypeSymbol* SymbolTable::GetFunctionTypeSymbol(const FunctionTypeSymbolKey& key, Context* context)
{
    ReadFunctionTypeMap();
    auto it = functionTypeMap.find(key);
    if (it != functionTypeMap.end())
    {
        SymbolId symbolId = it->second;
        FunctionTypeSymbol* functionTypeSymbol = GetFunctionTypeSymbol(symbolId, context);
        if (!functionTypeSymbol)
        {
            ThrowException("function type symbol id " + std::to_string(ToUnderlying(symbolId)) + " not found from module '" + GetModule()->Name() + "'");
        }
        return functionTypeSymbol;
    }
    return nullptr;
}

FunctionTypeSymbol* SymbolTable::MakeFunctionTypeSymbol(TypeSymbol* returnType, const std::vector<TypeSymbol*>& parameterTypes, Module* functionModule, 
    bool makePtrType, Context* context)
{
    FunctionTypeSymbolKey key;
    if (returnType)
    {
        key.returnTypeId = returnType->Id();
    }
    else
    {
        ThrowException("return type expected");
    }
    for (TypeSymbol* parameterType : parameterTypes)
    {
        key.parameterTypeIds.push_back(parameterType->Id());
    }
    FunctionTypeSymbol* functionTypeSymbol = GetFunctionTypeSymbol(key, context);
    if (functionTypeSymbol)
    {
        return functionTypeSymbol;
    }
    if (functionModule && functionModule != GetModule())
    {
        functionTypeSymbol = functionModule->GetSymbolTable()->GetFunctionTypeSymbol(key, context);
        if (functionTypeSymbol)
        {
            return functionTypeSymbol;
        }
    }
    for (Module* importedModule : GetModule()->ImportExportModules(context))
    {
        functionTypeSymbol = importedModule->GetSymbolTable()->GetFunctionTypeSymbol(key, context);
        if (functionTypeSymbol)
        {
            return functionTypeSymbol;
        }
    }
    SymbolId symbolId = context->GetNextSymbolId(SymbolKind::functionTypeSymbol);
    functionTypeMap[key] = symbolId;
    int ptrIndex = -1;
    functionTypeSymbol = new FunctionTypeSymbol(module, symbolId, MakeFunctionTypeName(returnType, parameterTypes, ptrIndex, makePtrType));
    functionTypeSymbol->SetPtrIndex(ptrIndex);
    functionTypeSymbol->SetReturnType(returnType);
    for (TypeSymbol* parameterType : parameterTypes)
    {
        functionTypeSymbol->AddParameterType(parameterType);
    }
    GlobalNs()->GetScope()->AddSymbol(functionTypeSymbol, soul::ast::FullSpan(), context);
    return functionTypeSymbol;
}

FunctionTypeSymbol* SymbolTable::MakeFunctionTypeSymbol(FunctionSymbol* functionSymbol, Context* context)
{
    TypeSymbol* returnType = functionSymbol->ReturnType(context);
    std::vector<TypeSymbol*> parameterTypes;
    Cardinality n = Cardinality(functionSymbol->Parameters(context).size());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        TypeSymbol* parameterType = functionSymbol->Parameters(context)[ToUnderlying(i)]->GetType(context);
        if (parameterType)
        {
            parameterTypes.push_back(parameterType);
        }
        else
        {
            ThrowException("parameter type expected");
        }
    }
    return MakeFunctionTypeSymbol(returnType, parameterTypes, functionSymbol->GetModule(), false, context);
}

TypeSymbol* SymbolTable::MakeConstCharPtrType(Context* context)
{
    Derivations derivations = Derivations::constDerivation;
    derivations = SetPointerCount(derivations, 1);
    return MakeCompoundType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::charType, context), derivations, context);
}

TypeSymbol* SymbolTable::MakeConstChar8PtrType(Context* context)
{
    Derivations derivations = Derivations::constDerivation;
    derivations = SetPointerCount(derivations, 1);
    return MakeCompoundType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::char8Type, context), derivations, context);
}

TypeSymbol* SymbolTable::MakeConstChar16PtrType(Context* context)
{
    Derivations derivations = Derivations::constDerivation;
    derivations = SetPointerCount(derivations, 1);
    return MakeCompoundType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::char16Type, context), derivations, context);
}

TypeSymbol* SymbolTable::MakeConstChar32PtrType(Context* context)
{
    Derivations derivations = Derivations::constDerivation;
    derivations = SetPointerCount(derivations, 1);
    return MakeCompoundType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::char32Type, context), derivations, context);
}

TypeSymbol* SymbolTable::MakeConstWCharPtrType(Context* context)
{
    Derivations derivations = Derivations::constDerivation;
    derivations = SetPointerCount(derivations, 1);
    return MakeCompoundType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        FundamentalTypeKind::wcharType, context), derivations, context);
}

ArrayTypeSymbol* SymbolTable::GetArrayType(TypeSymbol* elementType, std::int64_t size, Context* context)
{
    ArrayTypeKey key(elementType, size);
    auto it = arrayTypeMap.find(key);
    if (it != arrayTypeMap.end())
    {
        SymbolId symbolId = it->second;
        ArrayTypeSymbol* arrayTypeSymbol = GetArrayTypeSymbol(symbolId, context);
        if (!arrayTypeSymbol)
        {
            ThrowException("array type symbol id " + std::to_string(ToUnderlying(symbolId)) + " not found from module '" + GetModule()->Name() + "'");
        }
        return arrayTypeSymbol;
    }
    return nullptr;
}

ArrayTypeSymbol* SymbolTable::MakeArrayType(TypeSymbol* elementType, std::int64_t size, Context* context)
{
    ArrayTypeSymbol* arrayTypeSymbol = GetArrayType(elementType, size, context);
    if (arrayTypeSymbol)
    {
        return arrayTypeSymbol;
    }
    for (Module* importedModule : GetModule()->ImportExportModules(context))
    {
        arrayTypeSymbol = importedModule->GetSymbolTable()->GetArrayType(elementType, size, context);
        if (arrayTypeSymbol)
        {
            return arrayTypeSymbol;
        }
    }
    arrayTypeSymbol = new ArrayTypeSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::arrayTypeSymbol), elementType, size, context);
    GlobalNs()->GetScope()->AddSymbol(arrayTypeSymbol, soul::ast::FullSpan(), context);
    return arrayTypeSymbol;
}

DependentTypeSymbol* SymbolTable::MakeDependentTypeSymbol(otava::ast::Node* node, Context* context)
{
    DependentTypeSymbol* dependentTypeSymbol = new DependentTypeSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::dependentTypeSymbol), std::string());
    dependentTypeSymbol->ResetNode(node);
    dependentTypeSymbol->SetFullSpan(node->GetFullSpan());
    GlobalNs()->GetScope()->AddSymbol(dependentTypeSymbol, node->GetFullSpan(), context);
    return dependentTypeSymbol;
}

ClassGroupTypeSymbol* SymbolTable::MakeClassGroupTypeSymbol(ClassGroupSymbol* classGroup, Context* context)
{
    ClassGroupTypeSymbol* classGroupTypeSymbol = new ClassGroupTypeSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::classGroupTypeSymbol), classGroup->Name());
    classGroupTypeSymbol->SetClassGroup(classGroup);
    GlobalNs()->GetScope()->AddSymbol(classGroupTypeSymbol, classGroup->GetFullSpan(), context);
    return classGroupTypeSymbol;
}

AliasGroupTypeSymbol* SymbolTable::MakeAliasGroupTypeSymbol(AliasGroupSymbol* aliasGroup, Context* context)
{
    AliasGroupTypeSymbol* aliasGroupTypeSymbol = new AliasGroupTypeSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::aliasGroupTypeSymbol), aliasGroup->Name());
    aliasGroupTypeSymbol->SetAliasGroup(aliasGroup);
    GlobalNs()->GetScope()->AddSymbol(aliasGroupTypeSymbol, aliasGroup->GetFullSpan(), context);
    return aliasGroupTypeSymbol;
}

FunctionGroupTypeSymbol* SymbolTable::MakeFunctionGroupTypeSymbol(FunctionGroupSymbol* functionGroup, Context* context)
{
    FunctionGroupTypeSymbol* functionGroupTypeSymbol = new FunctionGroupTypeSymbol(
        GetModule(), context->GetNextSymbolId(SymbolKind::functionGroupTypeSymbol), functionGroup->Name());
    functionGroupTypeSymbol->SetFunctionGroup(functionGroup);
    GlobalNs()->GetScope()->AddSymbol(functionGroupTypeSymbol, functionGroup->GetFullSpan(), context);
    return functionGroupTypeSymbol;
}

void SymbolTable::PushLinkage(Linkage linkage_)
{
    linkageStack.push(currentLinkage);
    currentLinkage = linkage_;
}

void SymbolTable::PopLinkage()
{
    currentLinkage = linkageStack.top();
    linkageStack.pop();
}

void SymbolTable::Init(Context* context)
{
    if (!readOnly)
    {
        globalNs.reset(new NamespaceSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::namespaceSymbol), std::string()));
        GetModule()->AddNamespaceId(globalNs->Id());
        currentScope = globalNs->GetScope();
        currentScope->SetGlobal();
        Section* section = GetSection(globalNs.get());
        section->MapSymbol(globalNs.get());
        if (GetModule()->Name() == "std.type.fundamental")
        {
            MakeFundamentalTypes(context);
            MakeFundamentalTypeOperations(context);
            MakeArgumentIds();
            AddIntrinsics(context);
        }
    }
}

void SymbolTable::MakeFundamentalTypes(Context* context)
{
    AddFundamentalType(FundamentalTypeKind::charType, context);
    AddFundamentalType(FundamentalTypeKind::char8Type, context);
    AddFundamentalType(FundamentalTypeKind::char16Type, context);
    AddFundamentalType(FundamentalTypeKind::char32Type, context);
    AddFundamentalType(FundamentalTypeKind::wcharType, context);
    AddFundamentalType(FundamentalTypeKind::boolType, context);
    AddFundamentalType(FundamentalTypeKind::shortIntType, context);
    AddFundamentalType(FundamentalTypeKind::intType, context);
    AddFundamentalType(FundamentalTypeKind::longIntType, context);
    AddFundamentalType(FundamentalTypeKind::longLongIntType, context);
    AddFundamentalType(FundamentalTypeKind::floatType, context);
    AddFundamentalType(FundamentalTypeKind::doubleType, context);
    AddFundamentalType(FundamentalTypeKind::voidType, context);
    AddFundamentalType(FundamentalTypeKind::signedCharType, context);
    AddFundamentalType(FundamentalTypeKind::unsignedCharType, context);
    AddFundamentalType(FundamentalTypeKind::unsignedShortIntType, context);
    AddFundamentalType(FundamentalTypeKind::unsignedIntType, context);
    AddFundamentalType(FundamentalTypeKind::unsignedLongIntType, context);
    AddFundamentalType(FundamentalTypeKind::unsignedLongLongIntType, context);
    AddFundamentalType(FundamentalTypeKind::longDoubleType, context);
    AddFundamentalType(FundamentalTypeKind::autoType, context);
    AddFundamentalType(FundamentalTypeKind::nullPtrType, context);
}

void SymbolTable::AddIntrinsics(Context* context)
{
    otava::symbols::MakeIntrinsics(context);
}

void SymbolTable::AddFundamentalType(FundamentalTypeKind fundamentalTypeKind, Context* context)
{
    FundamentalTypeSymbol* fundamentalTypeSymbol = new FundamentalTypeSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeSymbol), fundamentalTypeKind);
    if (fundamentalTypeSymbol->Name() == "nullptr_t")
    {
        NamespaceSymbol* stdNs = globalNs->GetScope()->GetOrInsertNamespace("std", soul::ast::FullSpan(), context);
        stdNs->AddSymbol(fundamentalTypeSymbol, soul::ast::FullSpan(), context);
    }
    else
    {
        globalNs->AddSymbol(fundamentalTypeSymbol, soul::ast::FullSpan(), context);
    }
}

void SymbolTable::MapFundamentalType(FundamentalTypeSymbol* fundamentalTypeSymbol)
{
    FundamentalTypeKind kind = fundamentalTypeSymbol->GetFundamentalTypeKind();
    MapFundamentalTypeId(kind, fundamentalTypeSymbol->Id());
}

void SymbolTable::MapFundamentalTypeId(FundamentalTypeKind kind, SymbolId fundamentalTypeId)
{
    fundamentalTypeMap[static_cast<std::uint8_t>(kind)] = fundamentalTypeId;
}

TypeSymbol* SymbolTable::GetFundamentalTypeSymbol(FundamentalTypeKind kind, Context* context)
{
    ReadFundamentalTypeMap();
    auto it = fundamentalTypeMap.find(static_cast<std::uint8_t>(kind));
    if (it != fundamentalTypeMap.cend())
    {
        SymbolId symbolId = it->second;
        return GetTypeSymbol(symbolId, context);
    }
    else
    {
        SetExceptionThrown();
        throw std::runtime_error("fundamental type '" + MakeFundamentalTypeName(kind) + "' not found");
    }
}

void SymbolTable::MakeFundamentalTypeOperations(Context* context)
{
    AddFundamentalTypeOperationsToSymbolTable(context);
}

void SymbolTable::MakeArgumentIds()
{
    for (int i = 0; i < maxArguments; ++i)
    {
        argumentIds.push_back(static_cast<std::int64_t>(util::Random64()));
    }
}

void SymbolTable::WriteFundamentalTypeMap(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(fundamentalTypeMap.size())));
    for (const auto& s : fundamentalTypeMap)
    {
        std::uint8_t kindByte = s.first;
        SymbolId id = s.second;
        writer.GetBinaryStreamWriter().Write(kindByte);
        writer.GetBinaryStreamWriter().Write(ToUnderlying(id));
    }
    Cardinality argumentIdCount = Cardinality(argumentIds.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(argumentIdCount));
    for (int64_t argumentId : argumentIds)
    {
        writer.GetBinaryStreamWriter().Write(argumentId);
    }
}

void SymbolTable::ReadFundamentalTypeMap()
{
    if (fundamentalTypeMapRead || !IsReadOnly()) return;
    fundamentalTypeMapRead = true;
    Reader reader(GetModule()->GetFileMapping());
    ReadFundamentalTypeMap(reader);
}

void SymbolTable::ReadFundamentalTypeMap(Reader& reader)
{
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(module->GetFundamentalTypeTableOffset())), module->GetFundamentalTypeTableLength());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        std::uint8_t kindByte = reader.CurrentReader().ReadByte();
        SymbolId id = SymbolId(reader.CurrentReader().ReadUInt());
        MapFundamentalTypeId(FundamentalTypeKind(kindByte), id);
    }
    Cardinality argumentIdCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(argumentIdCount); ++i)
    {
        std::int64_t argumentId = reader.CurrentReader().ReadLong();
        argumentIds.push_back(argumentId);
    }
    reader.PopCurrentReader();
}

void SymbolTable::WriteCompoundTypeMaps(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(compoundTypeMap.size())));
    for (const auto& c : compoundTypeMap)
    {
        CompoundTypeKey key = c.first;
        SymbolId symbolId = c.second;
        key.Write(writer);
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
    }
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(irCompoundTypeMap.size())));
    for (const auto& c : irCompoundTypeMap)
    {
        CompoundTypeKey key = c.first;
        SymbolId symbolId = c.second;
        key.Write(writer);
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
    }
}

void SymbolTable::ReadCompoundTypeMaps()
{
    if (compoundTypeMapRead || !IsReadOnly()) return;
    compoundTypeMapRead = true;
    Reader reader(GetModule()->GetFileMapping());
    ReadCompoundTypeMaps(reader);
}

void SymbolTable::ReadCompoundTypeMaps(Reader& reader)
{
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(module->GetCompoundTypeMapOffset())), module->GetCompoundTypeMapLength());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        CompoundTypeKey key;
        key.Read(reader);
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        compoundTypeMap[key] = symbolId;
    }
    Cardinality irCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(irCount); ++i)
    {
        CompoundTypeKey key;
        key.Read(reader);
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        irCompoundTypeMap[key] = symbolId;
    }
    reader.PopCurrentReader();
}

void SymbolTable::WriteAliasTypeTemplateMap(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(aliasTypeTemplateMap.size())));
    for (const auto& a : aliasTypeTemplateMap)
    {
        SpecializationKey key = a.first;
        SymbolId symbolId = a.second;
        key.Write(writer);
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
    }
}

void SymbolTable::ReadAliasTypeTemplateMap()
{
    if (aliasTypeTemplateMapRead || !IsReadOnly()) return;
    aliasTypeTemplateMapRead = true;
    Reader reader(GetModule()->GetFileMapping());
    ReadAliasTypeTemplateMap(reader);
}

void SymbolTable::ReadAliasTypeTemplateMap(Reader& reader)
{
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(module->GetAliasTypeTemplateMapOffset())), module->GetAliasTypeTemplateMapLength());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SpecializationKey key;
        key.Read(reader);
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        aliasTypeTemplateMap[key] = symbolId;
    }
    reader.PopCurrentReader();
}

void SymbolTable::WriteClassTemplateSpecializationMaps(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(classTemplateSpecializationMap.size())));
    for (const auto& a : classTemplateSpecializationMap)
    {
        SpecializationKey key = a.first;
        SymbolId symbolId = a.second;
        key.Write(writer);
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
    }
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(irClassTemplateSpecializationMap.size())));
    for (const auto& a : irClassTemplateSpecializationMap)
    {
        SpecializationKey key = a.first;
        SymbolId symbolId = a.second;
        key.Write(writer);
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
    }
}

void SymbolTable::ReadClassTemplateSpecializationMaps()
{
    if (classTemplateSpecializationMapRead || !IsReadOnly()) return;
    classTemplateSpecializationMapRead = true;
    Reader reader(GetModule()->GetFileMapping());
    ReadClassTemplateSpecializationMaps(reader);
}

void SymbolTable::ReadClassTemplateSpecializationMaps(Reader& reader)
{
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(module->GetClassTemplateSpecializationMapOffset())), 
        module->GetClassTemplateSpecializationMapLength());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SpecializationKey key;
        key.Read(reader);
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        classTemplateSpecializationMap[key] = symbolId;
    }
    Cardinality irCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(irCount); ++i)
    {
        SpecializationKey key;
        key.Read(reader);
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        irClassTemplateSpecializationMap[key] = symbolId;
    }
    reader.PopCurrentReader();
}

void SymbolTable::WriteExplicitInstantiationMap(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(explicitInstantiationMap.size())));
    for (const auto& a : explicitInstantiationMap)
    {
        SpecializationKey key = a.first;
        SymbolId symbolId = a.second;
        key.Write(writer);
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
    }
}

void SymbolTable::ReadExplicitInstantiationMap()
{
    if (explicitInstantiationMapRead || !IsReadOnly()) return;
    explicitInstantiationMapRead = true;
    Reader reader(GetModule()->GetFileMapping());
    ReadExplicitInstantiationMap(reader);
}

void SymbolTable::ReadExplicitInstantiationMap(Reader& reader)
{
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(module->GetExplicitInstantiationMapOffset())),
        module->GetExplicitInstantiationMapLength());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SpecializationKey key;
        key.Read(reader);
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        explicitInstantiationMap[key] = symbolId;
    }
    reader.PopCurrentReader();
}

void SymbolTable::WriteFunctionTypeMap(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(functionTypeMap.size())));
    for (const auto& f : functionTypeMap)
    {
        FunctionTypeSymbolKey key = f.first;
        SymbolId symbolId = f.second;
        key.Write(writer);
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
    }
}

void SymbolTable::ReadFunctionTypeMap()
{
    if (functionTypeMapRead || !IsReadOnly()) return;
    functionTypeMapRead = true;
    Reader reader(GetModule()->GetFileMapping());
    ReadFunctionTypeMap(reader);
}

void SymbolTable::ReadFunctionTypeMap(Reader& reader)
{
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(module->GetFunctionTypeMapOffset())), module->GetFunctionTypeMapLength());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        FunctionTypeSymbolKey key;
        key.Read(reader);
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        functionTypeMap[key] = symbolId;
    }
    reader.PopCurrentReader();
}

void SymbolTable::MapImportedSymbolId(SymbolId symbolId, ModuleId moduleId)
{
    importedSymbolMap[symbolId] = moduleId;
}

ModuleId SymbolTable::GetModuleIdOfImportedSymbol(SymbolId symbolId) const
{
    GetModule()->ReadImportedSymbols();
    auto it = importedSymbolMap.find(symbolId);
    if (it != importedSymbolMap.end())
    {
        return it->second;
    }
    return zeroModuleId;
}

void SymbolTable::WriteSymbolIdVector(Writer& writer)
{
    Cardinality count = Cardinality(symbols.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (const auto* symbol : symbols)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbol->Id()));
    }
}

const std::vector<SymbolId>& SymbolTable::GetSymbolIds() 
{
    ReadSymbolIdVector();
    return symbolIds;
}

std::int64_t SymbolTable::GetArgumentId(int index) 
{
    ReadFundamentalTypeMap();
    if (argumentIds.empty())
    {
        ThrowException("argument ids not set");
    }
    return argumentIds[index % maxArguments];
}

void SymbolTable::AddImportedSymbol(SymbolId symbolId, ModuleId moduleId)
{
    addedImportedSymbolMap[symbolId] = moduleId;
}

void SymbolTable::ReadSymbolIdVector()
{
    if (symbolIdVectorRead || !IsReadOnly()) return;
    symbolIdVectorRead = true;
    Module* module = GetModule();
    Reader reader(module->GetFileMapping());
    FileOffset fileOffset = module->GetSymbolIdVectorOffset();
    Length length = module->GetSymbolIdVectorLength();
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(fileOffset)), length);
    ReadSymbolIdVector(reader);
    reader.PopCurrentReader();
}

void SymbolTable::ReadSymbolIdVector(Reader& reader)
{
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        symbolIds.push_back(symbolId);
    }
}

Symbol* SymbolTable::GetSymbol(SymbolId id, Context* context)
{
    SectionKind sectionKind = ToSectionKind(GetSymbolKind(id));
    Section* section = GetSection(sectionKind);
    if (section)
    {
        return section->GetSymbol(id, context);
    }
    return nullptr;
}

Value* SymbolTable::GetValue(SymbolId valueId, Context* context)
{
    Symbol* symbol = GetSymbol(valueId, context);
    if (symbol)
    {
        if (symbol->IsValueSymbol())
        {
            return static_cast<Value*>(symbol);
        }
        else
        {
            ThrowException("value symbol expected");
        }
    }
    return nullptr;
}

TypeSymbol* SymbolTable::GetTypeSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsTypeSymbol())
        {
            return static_cast<TypeSymbol*>(symbol);
        }
        else
        {
            ThrowException("type symbol expected");
        }
    }
    return nullptr;
}

ClassTypeSymbol* SymbolTable::GetClassTypeSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsClassTypeSymbol())
        {
            return static_cast<ClassTypeSymbol*>(symbol);
        }
        else
        {
            ThrowException("class type symbol expected");
        }
    }
    return nullptr;
}

ArrayTypeSymbol* SymbolTable::GetArrayTypeSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsArrayTypeSymbol())
        {
            return static_cast<ArrayTypeSymbol*>(symbol);
        }
        else
        {
            ThrowException("array type symbol expected");
        }
    }
    return nullptr;
}

ParameterSymbol* SymbolTable::GetParameterSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsParameterSymbol())
        {
            return static_cast<ParameterSymbol*>(symbol);
        }
        else
        {
            ThrowException("parameter symbol expected");
        }
    }
    return nullptr;
}

EnumeratedTypeSymbol* SymbolTable::GetEnumeratedTypeSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsEnumeratedTypeSymbol())
        {
            return static_cast<EnumeratedTypeSymbol*>(symbol);
        }
        else
        {
            ThrowException("enumerated type symbol expected");
        }
    }
    return nullptr;
}

VariableSymbol* SymbolTable::GetVariableSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsVariableSymbol())
        {
            return static_cast<VariableSymbol*>(symbol);
        }
        else
        {
            ThrowException("parameter symbol expected");
        }
    }
    return nullptr;
}

FunctionSymbol* SymbolTable::GetFunctionSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsFunctionSymbol())
        {
            return static_cast<FunctionSymbol*>(symbol);
        }
        else
        {
            ThrowException("function symbol expected");
        }
    }
    return nullptr;
}

FunctionDefinitionSymbol* SymbolTable::GetFunctionDefinitionSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsFunctionDefinitionSymbol())
        {
            return static_cast<FunctionDefinitionSymbol*>(symbol);
        }
        else
        {
            ThrowException("function definition symbol expected");
        }
    }
    return nullptr;
}

ExplicitlyInstantiatedFunctionDefinitionSymbol* SymbolTable::GetExplicitlyInstantiatedFunctionDefinitionSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsExplicitlyInstantiatedFunctionDefinitionSymbol())
        {
            return static_cast<ExplicitlyInstantiatedFunctionDefinitionSymbol*>(symbol);
        }
        else
        {
            ThrowException("explicitly instantiate4d function definition symbol expected");
        }
    }
    return nullptr;
}

FunctionGroupSymbol* SymbolTable::GetFunctionGroupSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsFunctionGroupSymbol())
        {
            return static_cast<FunctionGroupSymbol*>(symbol);
        }
        else
        {
            ThrowException("function group symbol expected");
        }
    }
    return nullptr;
}

ClassGroupSymbol* SymbolTable::GetClassGroupSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsClassGroupSymbol())
        {
            return static_cast<ClassGroupSymbol*>(symbol);
        }
        else
        {
            ThrowException("class group symbol expected");
        }
    }
    return nullptr;
}

NamespaceSymbol* SymbolTable::GetNamespaceSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsNamespaceSymbol())
        {
            return static_cast<NamespaceSymbol*>(symbol);
        }
        else
        {
            ThrowException("namespace symbol expected");
        }
    }
    return nullptr;
}

AliasTypeTemplateSpecializationSymbol* SymbolTable::GetAliasTypeTemplateSpecializationSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsAliasTypeTemplateSpecializationSymbol())
        {
            return static_cast<AliasTypeTemplateSpecializationSymbol*>(symbol);
        }
        else
        {
            ThrowException("alias type template specialization symbol expected");
        }
    }
    return nullptr;
}

ClassTemplateSpecializationSymbol* SymbolTable::GetClassTemplateSpecializationSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsClassTemplateSpecializationSymbol())
        {
            return static_cast<ClassTemplateSpecializationSymbol*>(symbol);
        }
        else
        {
            ThrowException("class template specialization symbol expected");
        }
    }
    return nullptr;
}

ExplicitInstantiationSymbol* SymbolTable::GetExplicitInstantiationSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsExplicitInstantiationSymbol())
        {
            return static_cast<ExplicitInstantiationSymbol*>(symbol);
        }
        else
        {
            ThrowException("explicit instantiation symbol expected");
        }
    }
    return nullptr;
}

AliasTypeSymbol* SymbolTable::GetAliasTypeSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsAliasTypeSymbol())
        {
            return static_cast<AliasTypeSymbol*>(symbol);
        }
        else
        {
            ThrowException("alias type symbol expected");
        }
    }
    return nullptr;
}

FunctionTypeSymbol* SymbolTable::GetFunctionTypeSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsFunctionTypeSymbol())
        {
            return static_cast<FunctionTypeSymbol*>(symbol);
        }
        else
        {
            ThrowException("function type symbol expected");
        }
    }
    return nullptr;
}

AliasGroupSymbol* SymbolTable::GetAliasGroupSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsAliasGroupSymbol())
        {
            return static_cast<AliasGroupSymbol*>(symbol);
        }
        else
        {
            ThrowException("alias group symbol expected");
        }
    }
    return nullptr;
}

EnumGroupSymbol* SymbolTable::GetEnumGroupSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsEnumGroupSymbol())
        {
            return static_cast<EnumGroupSymbol*>(symbol);
        }
        else
        {
            ThrowException("enum group symbol expected");
        }
    }
    return nullptr;
}

VariableGroupSymbol* SymbolTable::GetVariableGroupSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsVariableGroupSymbol())
        {
            return static_cast<VariableGroupSymbol*>(symbol);
        }
        else
        {
            ThrowException("variable group symbol expected");
        }
    }
    return nullptr;
}

TemplateParameterSymbol* SymbolTable::GetTemplateParameterSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsTemplateParameterSymbol())
        {
            return static_cast<TemplateParameterSymbol*>(symbol);
        }
        else
        {
            ThrowException("template parameter symbol expected");
        }
    }
    return nullptr;
}

TemplateParamGroupSymbol* SymbolTable::GetTemplateParamGroupSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsTemplateParamGroupSymbol())
        {
            return static_cast<TemplateParamGroupSymbol*>(symbol);
        }
        else
        {
            ThrowException("template parameter group symbol expected");
        }
    }
    return nullptr;
}

ForwardClassDeclarationSymbol* SymbolTable::GetForwardClassDeclarationSymbol(SymbolId id, Context* context)
{
    Symbol* symbol = GetSymbol(id, context);
    if (symbol)
    {
        if (symbol->IsForwardClassDeclarationSymbol())
        {
            return static_cast<ForwardClassDeclarationSymbol*>(symbol);
        }
        else
        {
            ThrowException("forward class declaration symbol expected");
        }
    }
    return nullptr;
}

void SymbolTable::AddSymbol(Symbol* symbol)
{
    symbols.push_back(symbol);
}

void SymbolTable::MapSymbol(Symbol* symbol)
{
    Section* section = GetSection(symbol);
    if (section)
    {
        section->MapSymbol(symbol);
    }
    else
    {
        ThrowException("symbol table: section for symbol kind " + SymbolKindStr(symbol->Kind()) + " not found");
    }
}

void SymbolTable::Write(Writer& writer)
{
    writer.Write(globalNs.get());
    writer.GetBinaryStreamWriter().WriteULEB128UInt(ToUnderlying(SectionKind::max));
    auto end = ToUnderlying(SectionKind::max);
    for (auto i = ToUnderlying(SectionKind::first); i != end; ++i)
    {
        Section* section = GetSection(SectionKind(i));
        section->Write(writer);
    }
}

} // namespace otava::symbols
