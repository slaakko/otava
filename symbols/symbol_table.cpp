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
import util.binary_stream_writer;
import util.memory_reader;
import util.utility;

namespace otava::symbols {

SymbolTable::SymbolTable(Module* module_, bool readOnly_) :
    module(module_), readOnly(readOnly_), globalNs(), currentScope(nullptr), currentAccess(Access::none), currentLinkage(Linkage::cpp_linkage),
    typenameConstraintSymbol(nullptr), classLevel(0), conversionTable(module), symbolIdVectorRead(false)
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

Scope* SymbolTable::GetNamespaceScope(const std::string& nsName, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* ns = Lookup(nsName, SymbolGroupKind::typeSymbolGroup, fullSpan, context);
    if (ns)
    {
        return ns->GetScope();
    }
    else
    {
        return nullptr;
    }
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

void SymbolTable::BeginScope(Scope* scope)
{
    PushScope();
    currentScope = scope;
}

void SymbolTable::EndScope()
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
        BeginScope(scope);
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
        EndScope();
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
        scope->Lookup(lookupSymbol->Name(), SymbolGroupKind::typeSymbolGroup, ScopeLookup::thisScope, LookupFlags::none, symbols, visited, nullptr);
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
/*
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
            allNodeSymbolMap[node] = symbol;
        }
    }
    if ((kind & MapKind::symbolToNode) != MapKind::none)
    {
        symbolNodeMap[symbol] = node;
        allSymbolNodeMap[symbol] = node;
    }
    if (symbol->Parent() && symbol->Parent()->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(symbol->Parent());
        specialization->SetProject();
        AddChangedClassTemplateSpecialization(specialization);
    }
*/
}

VariableSymbol* SymbolTable::AddVariable(const std::string& name, otava::ast::Node* node, TypeSymbol* declaredType, TypeSymbol* initializerType,
    Value* value, DeclarationFlags flags, Context* context)
{
/*
    VariableGroupSymbol* variableGroup = currentScope->GroupScope()->GetOrInsertVariableGroup(name, node->GetFullSpan(), context);
    VariableSymbol* variableSymbol = new VariableSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::variableSymbol), name);
    variableSymbol->SetAccess(CurrentAccess());
    variableSymbol->SetDeclaredType(declaredType);
    variableSymbol->SetInitializerType(initializerType);
    variableSymbol->SetValue(value);
    variableSymbol->SetDeclarationFlags(flags);
    currentScope->SymbolScope()->AddSymbol(variableSymbol, node->GetFullSpan(), context);
    variableGroup->AddVariable(variableSymbol);
    return variableSymbol;
*/
    return nullptr;
}

AliasTypeSymbol* SymbolTable::AddAliasType(otava::ast::Node* idNode, otava::ast::Node* aliasTypeNode, TypeSymbol* type, Context* context)
{
/*
    std::string id = idNode->Str();
    AliasGroupSymbol* aliasGroup = currentScope->GroupScope()->GetOrInsertAliasGroup(id, idNode->GetFullSpan(), context);
    AliasTypeSymbol* aliasTypeSymbol = new AliasTypeSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::aliasTypeSymbol), id);
    aliasTypeSymbol->SetReferredType(type);
    aliasTypeSymbol->SetAccess(currentAccess);
    currentScope->SymbolScope()->AddSymbol(aliasTypeSymbol, idNode->GetFullSpan(), context);
    aliasGroup->AddAliasTypeSymbol(aliasTypeSymbol, context);
    MapNode(aliasTypeNode, aliasTypeSymbol);
    return aliasTypeSymbol;
*/
    return nullptr;
}

void SymbolTable::AddUsingDeclaration(otava::ast::Node* node, Symbol* symbol, Context* context)
{
    currentScope->SymbolScope()->AddUsingDeclaration(symbol, node->GetFullSpan(), context);
    MapNode(node, symbol, MapKind::nodeToSymbol);
}

void SymbolTable::AddUsingDirective(NamespaceSymbol* ns, otava::ast::Node* node, Context* context)
{
    currentScope->SymbolScope()->AddUsingDirective(ns, node->GetFullSpan(), context);
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
        BeginScope(currentScope);
    }
    else
    {
        Symbol* symbol = currentScope->Lookup(name, SymbolGroupKind::typeSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
        if (symbol)
        {
            if (symbol->IsNamespaceSymbol())
            {
                NamespaceSymbol* namespaceSymbol = static_cast<NamespaceSymbol*>(symbol);
                if (node)
                {
                    MapNode(node, namespaceSymbol);
                }
                BeginScope(namespaceSymbol->GetScope());
                return;
            }
            else
            {
                ThrowException("name of namespace '" + name + " conflicts with earlier declaration", fullSpan, context);
            }
        }
        NamespaceSymbol* namespaceSymbol = new NamespaceSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::namespaceSymbol), name);
        if (node)
        {
            MapNode(node, namespaceSymbol);
        }
        currentScope->SymbolScope()->AddSymbol(namespaceSymbol, fullSpan, context);
        BeginScope(namespaceSymbol->GetScope());
    }
}

void SymbolTable::EndNamespace()
{
    EndScope();
}

void SymbolTable::BeginNamespace(otava::ast::Node* node, Context* context)
{
    BeginNamespace(node->Str(), node, context);
}

void SymbolTable::EndNamespace(int level)
{
    for (int i = 0; i < level; ++i)
    {
        EndNamespace();
    }
}

void SymbolTable::BeginClass(const std::string& name, ClassKind classKind, TypeSymbol* specialization, otava::ast::Node* node, Context* context)
{
/*
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    Symbol* symbol = currentScope->Lookup(name, SymbolGroupKind::typeSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
    ClassGroupSymbol* classGroup = currentScope->GroupScope()->GetOrInsertClassGroup(name, fullSpan, context);
    ClassTypeSymbol* classTypeSymbol = new ClassTypeSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::classTypeSymbol), name);
    classTypeSymbol->SetFullSpan(fullSpan);
    classTypeSymbol->SetLevel(classLevel++);
    AddClass(classTypeSymbol);
    classTypeSymbol->SetAccess(CurrentAccess());
    classTypeSymbol->SetClassKind(classKind);
    classTypeSymbol->SetSpecialization(specialization, context);
    currentScope->SymbolScope()->AddSymbol(classTypeSymbol, fullSpan, context);
    classGroup->AddClass(classTypeSymbol);
    MapNode(node, classTypeSymbol);
    SetSpecifierNode(classTypeSymbol, node);
    BeginScope(classTypeSymbol->GetScope());
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
*/
}

void SymbolTable::AddBaseClass(ClassTypeSymbol* baseClass, const soul::ast::FullSpan& fullSpan, Context* context)
{
/*
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
*/
}

void SymbolTable::EndClass()
{
    --classLevel;
    PopAccess();
    EndScope();
}

void SymbolTable::AddForwardClassDeclaration(const std::string& name, ClassKind classKind, TypeSymbol* specialization, otava::ast::Node* node, Context* context)
{
/*
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    Symbol* symbol = currentScope->Lookup(name, SymbolGroupKind::typeSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::dontResolveSingle);
    ClassGroupSymbol* classGroup = currentScope->GroupScope()->GetOrInsertClassGroup(name, fullSpan, context);
    std::unique_ptr<ForwardClassDeclarationSymbol> forwardDeclarationSymbol(new ForwardClassDeclarationSymbol(GetModule(), 
        context->GetNextSymbolId(SymbolKind::forwardClassDeclarationSymbol), name));
    forwardDeclarationSymbol->SetAccess(CurrentAccess());
    forwardDeclarationSymbol->SetClassKind(classKind);
    forwardDeclarationSymbol->SetSpecialization(specialization);
    forwardDeclarationSymbol->SetParent(currentScope->SymbolScope()->GetSymbol());
    forwardDeclarationSymbol->SetFullSpan(fullSpan);
    ForwardClassDeclarationSymbol* fwdDeclaration = classGroup->GetForwardDeclaration(forwardDeclarationSymbol->Arity());
    if (!fwdDeclaration)
    {
        classGroup->AddForwardDeclaration(forwardDeclarationSymbol.get());
        Symbol* sym = forwardDeclarationSymbol.release();
        MapNode(node, sym);
        forwardDeclarations.insert(sym);
        allForwardDeclarations.insert(sym);
        currentScope->SymbolScope()->AddSymbol(sym, fullSpan, context);
    }
*/
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
/*
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    EnumGroupSymbol* enumGroup = currentScope->GroupScope()->GetOrInsertEnumGroup(name, fullSpan, context);
    EnumeratedTypeSymbol* enumType = enumGroup->GetEnumType();
    if (enumType)
    {
        ThrowException("enumerated type '" + util::ToUtf8(name) + "' not unique", fullSpan, context);
    }
    EnumeratedTypeSymbol* enumTypeSymbol = new EnumeratedTypeSymbol(name);
    enumTypeSymbol->SetAccess(CurrentAccess());
    enumTypeSymbol->SetEnumTypeKind(kind);
    enumTypeSymbol->SetUnderlyingType(underlyingType);
    enumTypeSymbol->SetFullSpan(fullSpan);
    currentScope->SymbolScope()->AddSymbol(enumTypeSymbol, fullSpan, context);
    MapNode(node, enumTypeSymbol);
    enumGroup->SetEnumType(enumTypeSymbol);
    BeginScope(enumTypeSymbol->GetScope());
*/
}

void SymbolTable::EndEnumeratedType()
{
    EndScope();
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
/*
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    EnumeratedTypeSymbol* enumTypeSymbol = nullptr;
    Scope* scope = currentScope->SymbolScope();
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
                    scope = enumTypeSymbol->Parent()->GetScope();
                }
            }
        }
    }
    if (!enumTypeSymbol)
    {
        ThrowException("could not add enumerator to symbol table: enumerated type not found", fullSpan, context);
    }
    EnumConstantSymbol* enumConstantSymbol = new EnumConstantSymbol(name);
    enumConstantSymbol->SetEnumType(enumTypeSymbol);
    enumConstantSymbol->SetValue(value);
    enumConstantSymbol->SetFullSpan(fullSpan);
    scope->AddSymbol(enumConstantSymbol, fullSpan, context);
    MapNode(node, enumConstantSymbol);
*/
}

BlockSymbol* SymbolTable::BeginBlock(const soul::ast::FullSpan& fullSpan, Context* context)
{
    BlockSymbol* blockSymbol = new BlockSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::blockSymbol), std::string());
    blockSymbol->SetFullSpan(fullSpan);
    currentScope->SymbolScope()->AddSymbol(blockSymbol, fullSpan, context);
    BeginScopeGeneric(blockSymbol->GetScope(), context);
    return blockSymbol;
}

void SymbolTable::EndBlock(Context* context)
{
    EndScopeGeneric(context);
}

void SymbolTable::RemoveBlock()
{
    Symbol* symbol = currentScope->GetSymbol();
    if (symbol && symbol->IsBlockSymbol())
    {
        EndScope();
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
    currentScope->SymbolScope()->AddSymbol(templateDeclarationSymbol, node->GetFullSpan(), context);
    BeginScope(templateDeclarationSymbol->GetScope());
}

void SymbolTable::EndTemplateDeclaration()
{
    EndScope();
}

void SymbolTable::RemoveTemplateDeclaration()
{
    Symbol* symbol = currentScope->GetSymbol();
    if (symbol && symbol->IsTemplateDeclarationSymbol())
    {
        EndScope();
        currentScope->RemoveSymbol(symbol);
    }
}

void SymbolTable::AddTemplateParameter(const std::string& name, otava::ast::Node* node, Symbol* constraint, int index,
    ParameterSymbol* parameter, otava::ast::Node* defaultTemplateArgNode, Context* context)
{
/*
    TemplateParameterSymbol* templateParameterSymbol = new TemplateParameterSymbol(constraint, name, GetTemplateParameterId(index), index, defaultTemplateArgNode);
    templateParameterSymbol->SetFullSpan(node->GetFullSpan());
    if (parameter)
    {
        templateParameterSymbol->AddSymbol(parameter, node->GetFullSpan(), context);
    }
    currentScope->SymbolScope()->AddSymbol(templateParameterSymbol, node->GetFullSpan(), context);
    MapNode(node, templateParameterSymbol);
*/
}

FunctionSymbol* SymbolTable::AddFunction(const std::string& name, const std::vector<TypeSymbol*>& specialization, otava::ast::Node* node, FunctionKind kind,
    FunctionQualifiers qualifiers, DeclarationFlags flags, Context* context)
{
/*
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
    FunctionGroupSymbol* functionGroup = currentScope->GroupScope()->GetOrInsertFunctionGroup(groupName, fullSpan, context);
    FunctionSymbol* functionSymbol = new FunctionSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), name);
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
    currentScope->SymbolScope()->AddSymbol(functionSymbol, fullSpan, context);
    functionGroup->AddFunction(functionSymbol);
    MapNode(node, functionSymbol);
    return functionSymbol;
*/
    return nullptr;
}

void SymbolTable::AddFunctionSymbol(Scope* scope, FunctionSymbol* functionSymbol, soul::ast::FullSpan& fullSpan, Context* context)
{
    FunctionGroupSymbol* functionGroup = currentScope->GroupScope()->GetOrInsertFunctionGroup(functionSymbol->GroupName(), fullSpan, context);
    functionSymbol->SetLinkage(currentLinkage);
    scope->SymbolScope()->AddSymbol(functionSymbol, fullSpan, context);
    functionGroup->AddFunction(functionSymbol);
}

FunctionDefinitionSymbol* SymbolTable::AddOrGetFunctionDefinition(Scope* scope, const std::string& name, const std::vector<TypeSymbol*>& specialization,
    const std::vector<TypeSymbol*>& parameterTypes, FunctionQualifiers qualifiers, FunctionKind kind, DeclarationFlags declarationFlags, otava::ast::Node* node,
    otava::ast::Node* functionNode, bool& get, Context* context)
{
/*
    get = false;
    std::string groupName = name;
    Symbol* containerSymbol = scope->SymbolScope()->GetSymbol();
    if (containerSymbol && containerSymbol->SimpleName() == name)
    {
        kind = FunctionKind::constructor;
    }
    if (kind == FunctionKind::constructor)
    {
        groupName = U"@constructor";
    }
    else if (kind == FunctionKind::destructor)
    {
        groupName = U"@destructor";
    }
    FunctionGroupSymbol* functionGroup = scope->GroupScope()->GetOrInsertFunctionGroup(groupName, node->GetFullSpan(), context);
    std::unique_ptr<FunctionDefinitionSymbol> functionDefinition(new FunctionDefinitionSymbol(name));
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
    functionDefinition->SetParent(currentScope->SymbolScope()->GetSymbol());
    if (node)
    {
        functionDefinition->SetFullSpan(node->GetFullSpan());
    }
    int index = 0;
    for (auto* parameterType : parameterTypes)
    {
        functionDefinition->AddTemporaryParameter(parameterType, index++);
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
    currentScope->SymbolScope()->AddSymbol(functionDefinition.release(), node->GetFullSpan(), context);
    FunctionSymbol* declaration = functionGroup->ResolveFunction(parameterTypes, qualifiers, specialization, definition->ParentTemplateDeclaration(),
        definition->IsSpecialization(), context);
    if (declaration)
    {
        definition->SetDeclaration(declaration);
        definition->SetAccess(declaration->GetAccess());
    }
    return definition;
*/
    return nullptr;
}

ParameterSymbol* SymbolTable::CreateParameter(const std::string& name, otava::ast::Node* node, TypeSymbol* type, Context* context)
{
    ParameterSymbol* parameterSymbol = new ParameterSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), name);
    parameterSymbol->SetType(type);
    if (node)
    {
        MapNode(node, parameterSymbol);
        parameterSymbol->SetFullSpan(node->GetFullSpan());
    }
    return parameterSymbol;
}

otava::ast::Node* SymbolTable::GetSpecifierNode(Symbol* symbol) const noexcept
{
/*
    auto it = allSpecifierNodeMap.find(symbol);
    if (it != allSpecifierNodeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
*/
    return nullptr;
}

void SymbolTable::SetSpecifierNode(Symbol* symbol, otava::ast::Node* node)
{
/*
    specifierNodeMap[symbol] = node;
    allSpecifierNodeMap[symbol] = node;
*/
}

TypeSymbol* SymbolTable::MakeCompoundType(TypeSymbol* baseType, Derivations derivations, Context* context)
{
    // TODO
    return nullptr;
}

ArrayTypeSymbol* SymbolTable::MakeArrayType(TypeSymbol* elementType, std::int64_t size, otava::ast::Node* node)
{
    // TODO
    return nullptr;
}

void SymbolTable::MapFunction(FunctionSymbol* function)
{
    // TODO
}
void SymbolTable::MapFunctionDefinition(FunctionDefinitionSymbol* functionDefinition)
{
    // TODO
}

void SymbolTable::MapVariable(VariableSymbol* variable)
{
    // TODO
}

void SymbolTable::MapConstraint(Symbol* constraint)
{
    // TODO
}

void SymbolTable::MapFunctionGroup(FunctionGroupSymbol* functionGroup)
{
    // TODO
}
void SymbolTable::MapClassGroup(ClassGroupSymbol* classGroup)
{
    // TODO
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

void SymbolTable::MapAliasGroup(AliasGroupSymbol* aliasGroup)
{
    // TODO
}

void SymbolTable::Init(Context* context)
{
    if (!readOnly)
    {
        globalNs.reset(new NamespaceSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::namespaceSymbol), std::string()));
        currentScope = globalNs->GetScope();
        Section* section = GetSection(globalNs.get());
        section->MapSymbol(globalNs.get());
        if (GetModule()->Name() == "std.type.fundamental")
        {
            MakeFundamentalTypes(context);
            MakeFundamentalTypeOperations(context);
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

void SymbolTable::AddFundamentalType(FundamentalTypeKind fundamentalTypeKind, Context* context)
{
    FundamentalTypeSymbol* fundamentalTypeSymbol = new FundamentalTypeSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::fundamentalTypeSymbol), fundamentalTypeKind);
    globalNs->AddSymbol(fundamentalTypeSymbol, soul::ast::FullSpan(), context);
}

void SymbolTable::MakeFundamentalTypeOperations(Context* context)
{
    AddFundamentalTypeOperationsToSymbolTable(context);
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
    reader.PopCurrentReader();
}

void SymbolTable::MapExportedSymbolId(SymbolId symbolId, ModuleId moduleId)
{
    exportedSymbolMap[symbolId] = moduleId;
}

ModuleId SymbolTable::GetModuleIdOfExportedSymbol(SymbolId symbolId) const
{
    GetModule()->ReadExportedSymbols();
    auto it = exportedSymbolMap.find(symbolId);
    if (it != exportedSymbolMap.end())
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

const std::vector<SymbolId>& SymbolTable::SymbolIds() 
{
    ReadSymbolIdVector();
    return symbolIds;
}

void SymbolTable::ReadSymbolIdVector()
{
    if (symbolIdVectorRead) return;
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
