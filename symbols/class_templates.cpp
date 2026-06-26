// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.class_templates;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.instantiator;
import otava.symbols.modules;
import otava.symbols.statement_binder;
import otava.symbols.templates;
import otava.symbols.type_resolver;
import otava.symbols.writer;
import otava.symbols.reader;
import otava.optimizer;
import otava.ast.classes;
import otava.ast.visitor;
import util.sha1;

namespace otava::symbols {

ClassTemplateSpecializationSymbol::ClassTemplateSpecializationSymbol(Module* module_, SymbolId id_) : 
    ClassTypeSymbol(module_, id_), classTemplateId(zeroSymbolId), templateArgumentsSet(false), instantiated(false), destructor(nullptr), destructorId(zeroSymbolId),
    instantiatingDestructor(false)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

ClassTemplateSpecializationSymbol::ClassTemplateSpecializationSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    ClassTypeSymbol(module_, id_, name_), classTemplateId(zeroSymbolId), templateArgumentsSet(false), instantiated(false), destructor(nullptr), destructorId(zeroSymbolId),
    instantiatingDestructor(false)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

bool ClassTemplateSpecializationSymbol::IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType, Context* context) const 
{
    const TypeSymbol* thisSymbol = this;
    if (visited.find(thisSymbol) != visited.end()) return true;
    visited.insert(thisSymbol);
    for (Symbol* templateArg : templateArguments)
    {
        if (templateArg->IsTypeSymbol())
        {
            TypeSymbol* templateArgType = static_cast<TypeSymbol*>(templateArg);
            if (!templateArgType->IsComplete(visited, incompleteType, context)) return false;
        }
    }
    return true;
}

ClassTypeSymbol* ClassTemplateSpecializationSymbol::ClassTemplate(Context* context) const
{
    if (classTemplate)
    {
        return classTemplate;
    }
    if (IsReadOnly() && classTemplateId != zeroSymbolId)
    {
        classTemplate = GetModule()->GetSymbolTable()->GetClassTypeSymbol(classTemplateId, context);
        if (!classTemplate)
        {
            ThrowException("class template id " + std::to_string(ToUnderlying(classTemplateId)) + " not found", GetFullSpan(), context);
        }
    }
    return classTemplate;
}

FunctionSymbol* ClassTemplateSpecializationSymbol::Destructor(Context* context)
{
    if (destructor)
    {
        return destructor;
    }
    if (IsReadOnly() && destructorId != zeroSymbolId)
    {
        destructor = context->GetSymbolTable()->GetFunctionSymbol(destructorId, context);
    }
    return destructor;
}

TypeSymbol* ClassTemplateSpecializationSymbol::FinalType(const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::vector<Symbol*> templateArgs;
    for (Symbol* templateArg : TemplateArguments(context))
    {
        if (templateArg->IsTypeSymbol())
        {
            TypeSymbol* typeTemplateArg = static_cast<TypeSymbol*>(templateArg);
            typeTemplateArg = typeTemplateArg->DirectType(context)->FinalType(fullSpan, context);
            templateArgs.push_back(typeTemplateArg);
        }
        else
        {
            templateArgs.push_back(templateArg);
        }
    }
    ClassTemplateSpecializationSymbol* specialization = InstantiateClassTemplate(ClassTemplate(context), templateArgs, fullSpan, context);
    return specialization;
}

const std::vector<Symbol*>& ClassTemplateSpecializationSymbol::TemplateArguments(Context* context) const
{
    if (IsReadOnly() && !templateArgumentsSet)
    {
        templateArgumentsSet = true;
        for (SymbolId templateArgumentId : templateArgumentIds)
        {
            Symbol* templateArgument = GetModule()->GetSymbolTable()->GetSymbol(templateArgumentId, context);
            if (!templateArgument)
            {
                ThrowException("template argument id " + std::to_string(ToUnderlying(templateArgumentId)) + " not found", GetFullSpan(), context);
            }
            templateArguments.push_back(templateArgument);
        }
    }
    return templateArguments;
}

void ClassTemplateSpecializationSymbol::AddTemplateArgument(Symbol* templateArgument)
{
    templateArguments.push_back(templateArgument);
}

void ClassTemplateSpecializationSymbol::AddInstantiatedVirtualFunctionSpecialization(FunctionSymbol* specialization)
{
    if (std::find(instantiatedVirtualFunctionSpecializations.begin(), instantiatedVirtualFunctionSpecializations.end(), specialization) ==
        instantiatedVirtualFunctionSpecializations.end())
    {
        instantiatedVirtualFunctionSpecializations.push_back(specialization);
    }
}

TypeSymbol* ClassTemplateSpecializationSymbol::UnifyTemplateArgumentType(
    const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>& templateParameterMap, 
    const soul::ast::FullSpan& fullSpan, Context* context) noexcept
{
    std::vector<Symbol*> targetTemplateArguments;
    for (int i = 0; i < templateArguments.size(); ++i)
    {
        Symbol* sourceTemplateArgumentSymbol = templateArguments[i];
        TypeSymbol* sourceTemplateArgumentType = nullptr;
        if (sourceTemplateArgumentSymbol->IsTypeSymbol())
        {
            sourceTemplateArgumentType = static_cast<TypeSymbol*>(sourceTemplateArgumentSymbol);
        }
        else
        {
            return nullptr;
        }
        TypeSymbol* templateArgumentType = sourceTemplateArgumentType->UnifyTemplateArgumentType(templateParameterMap, fullSpan, context);
        if (templateArgumentType)
        {
            targetTemplateArguments.push_back(templateArgumentType);
        }
        else
        {
            return nullptr;
        }
    }
    return context->GetSymbolTable()->MakeClassTemplateSpecialization(classTemplate, targetTemplateArguments, fullSpan, context);
}

bool ClassTemplateSpecializationSymbol::IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const
{
    const Symbol* thisSymbol = this;
    if (visited.find(thisSymbol) == visited.end())
    {
        if (ClassTypeSymbol::IsTemplateParameterInstantiation(context, visited)) return true;
        for (Symbol* templateArg : TemplateArguments(context))
        {
            if (templateArg->IsTemplateParameterInstantiation(context, visited)) return true;
        }
        visited.insert(thisSymbol);
    }
    return false;
}

FunctionSymbol* ClassTemplateSpecializationSymbol::GetMatchingVirtualFunctionSpecialization(FunctionSymbol* newcomer, Context* context) const noexcept
{
    bool found = false;
    bool def = newcomer->IsFunctionDefinitionSymbol();
    for (auto prev : instantiatedVirtualFunctionSpecializations)
    {
        bool prevDef = prev->IsFunctionDefinitionSymbol();
        if (def == prevDef && FunctionMatches(newcomer, prev, context))
        {
            return prev;
        }
    }
    return nullptr;
}

bool ClassTemplateSpecializationSymbol::ContainsVirtualFunctionSpecialization(FunctionSymbol* specialization) const noexcept
{
    for (FunctionSymbol* instance : instantiatedVirtualFunctionSpecializations)
    {
        if (instance == specialization) return true;
    }
    return false;
}

std::string ClassTemplateSpecializationSymbol::GroupName(Context* context) 
{
    ClassTypeSymbol* classTemplate = ClassTemplate(context);
    if (classTemplate)
    {
        return classTemplate->GroupName(context);
    }
    else
    {
        ThrowException("group name of class '" + FullName(context) + "' not resolved", GetFullSpan(), context);
    }
}

std::string ClassTemplateSpecializationSymbol::FullName(Context* context) const
{
    std::string specializationName;
    ClassTypeSymbol* classTemplate = ClassTemplate(context);
    specializationName.append(classTemplate->FullName(context));
    specializationName.append(1, '<');
    const std::vector<Symbol*>& templateArgs = TemplateArguments(context);
    bool first = true;
    for (Symbol* templateArg : templateArgs)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            specializationName.append(", ");
        }
        specializationName.append(templateArg->FullName(context));
    }
    specializationName.append(1, '>');
    return specializationName;
}

std::string ClassTemplateSpecializationSymbol::IrName(Context* context) const
{
    std::string fullIrName = ClassTemplate(context)->IrName(context);
    std::string shaMaterial;
    shaMaterial.append(std::to_string(ToUnderlying(Id())));
    for (Symbol* templateArg : TemplateArguments(context))
    {
        shaMaterial.append(1, '.').append(templateArg->IrName(context));
    }
    shaMaterial.append(".").append(context->GetBoundCompileUnit()->Id());
    fullIrName.append(1, '_').append(util::GetSha1MessageDigest(shaMaterial));
    return fullIrName;
}

void ClassTemplateSpecializationSymbol::SetClassTemplate(ClassTypeSymbol* classTemplate_) noexcept
{
    classTemplate = classTemplate_;
    if (classTemplate->GetModule() != GetModule())
    {
        GetModule()->GetSymbolTable()->AddImportedSymbol(classTemplate->Id(), classTemplate->GetModule()->Id());
    }
}

void ClassTemplateSpecializationSymbol::Write(Writer& writer)
{
    ClassTypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(instantiated);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(classTemplate->Id()));
    Cardinality count = Cardinality(templateArguments.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (Symbol* templateArgument : templateArguments)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(templateArgument->Id()));
    }
}

void ClassTemplateSpecializationSymbol::Read(Reader& reader)
{
    ClassTypeSymbol::Read(reader);
    instantiated = reader.CurrentReader().ReadBool();
    classTemplateId = SymbolId(reader.CurrentReader().ReadUInt());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId templateArgumentId = SymbolId(reader.CurrentReader().ReadUInt());
        templateArgumentIds.push_back(templateArgumentId);
    }
}

MemFnKey::MemFnKey() : memFnId(zeroSymbolId)
{
}

MemFnKey::MemFnKey(FunctionSymbol* memFn, const std::vector<TypeSymbol*> templateArguments) : memFnId(memFn->Id())
{
    for (const auto* templateArgumentType : templateArguments)
    {
        templateArgumentIds.push_back(templateArgumentType->Id());
    }
}

bool MemFnKeyEqual::operator()(const MemFnKey& left, const MemFnKey& right) const noexcept
{
    if (left.memFnId != right.memFnId) return false;
    if (left.templateArgumentIds.size() != right.templateArgumentIds.size()) return false;
    for (Index i = Index(0); i < Index(left.templateArgumentIds.size()); ++i)
    {
        SymbolId leftSymbolId = left.templateArgumentIds[ToUnderlying(i)];
        SymbolId rightSymbolId = right.templateArgumentIds[ToUnderlying(i)];
        if (leftSymbolId != rightSymbolId) return false;
    }
    return true;
}

size_t MemFnKeyHash::operator()(const MemFnKey& key) const noexcept
{
    size_t hashCode = std::hash<std::uint32_t>()(ToUnderlying(key.memFnId));
    Cardinality count = Cardinality(key.templateArgumentIds.size());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId argId = key.templateArgumentIds[ToUnderlying(i)];
        size_t argHashCode = std::hash<std::uint32_t>()(ToUnderlying(argId));
        hashCode ^= (argHashCode << ToUnderlying(i + Index(1))) | (argHashCode >> ToUnderlying(Index(count) - i + Index(1)));
    }
    return hashCode;
}

ClassTemplateRepository::ClassTemplateRepository()
{
}

FunctionDefinitionSymbol* ClassTemplateRepository::GetFunctionDefinition(const MemFnKey& key, Context* context) const noexcept
{
    auto it = memFnKeyMap.find(key);
    if (it != memFnKeyMap.end())
    {
        SymbolId functionDefinitionId = it->second;
        FunctionDefinitionSymbol* def = context->GetSymbolTable()->GetFunctionDefinitionSymbol(functionDefinitionId, context);
        if (!def)
        {
            ThrowException("function definition id " + std::to_string(ToUnderlying(functionDefinitionId)) + " not found from module '" + 
                context->GetModule()->Name() + "'");
        }
        return def;
    }
    else
    {
        return nullptr;
    }
}

void ClassTemplateRepository::AddFunctionDefinition(const MemFnKey& key, FunctionDefinitionSymbol* functionDefinitionSymbol)
{
    memFnKeyMap[key] = functionDefinitionSymbol->Id();
}

class VirtualFunctionNodeClassifierVisitor : public otava::ast::DefaultVisitor
{
public:
    VirtualFunctionNodeClassifierVisitor();
    void Visit(otava::ast::VirtualNode& node) override;
    void Visit(otava::ast::OverrideNode& node) override;
    void Visit(otava::ast::FinalNode& node) override;
    bool GetValue() const { return value; }
private:
    bool value;
};

VirtualFunctionNodeClassifierVisitor::VirtualFunctionNodeClassifierVisitor() : value(false)
{
}

void VirtualFunctionNodeClassifierVisitor::Visit(otava::ast::VirtualNode& node)
{
    value = true;
}

void VirtualFunctionNodeClassifierVisitor::Visit(otava::ast::OverrideNode& node)
{
    value = true;
}

void VirtualFunctionNodeClassifierVisitor::Visit(otava::ast::FinalNode& node)
{
    value = true;
}

bool IsVirtualFunctionNode(otava::ast::Node* node)
{
    VirtualFunctionNodeClassifierVisitor visitor;
    node->Accept(visitor);
    return visitor.GetValue();
}

void InstantiateVirtualFunctions(ClassTemplateSpecializationSymbol* specialization, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::string fname = specialization->FullName(context);
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped(); 
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true); 
    std::set<const Symbol*> visited;
    std::vector<FunctionSymbol*> virtualFunctions;
    if (!specialization->IsTemplateParameterInstantiation(context, visited))
    {
        ClassTypeSymbol* classTemplate = specialization->ClassTemplate(context);
        classTemplate->GetModule()->ReadAstNode();
        for (FunctionSymbol* memFn : classTemplate->MemberFunctions(context))
        {
            otava::ast::Node* node = context->GetSymbolTable()->GetNodeNothrow(memFn);
            if (!node)
            {
                std::int64_t astNodeId = memFn->AstNodeId();
                memFn->GetModule()->ReadAstNode();
                node = memFn->GetModule()->GetAstNode(astNodeId);
            }
            if (node && IsVirtualFunctionNode(node))
            {
                virtualFunctions.push_back(memFn);
            }
        }
    }
    for (FunctionSymbol* virtualMemFn : virtualFunctions)
    {
        std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual> templateParameterMap;
        FunctionSymbol* instance = InstantiateMemFnOfClassTemplate(virtualMemFn, specialization, templateParameterMap, fullSpan, context);
        specialization->AddInstantiatedVirtualFunctionSpecialization(instance);
    }
    if (!virtualFunctions.empty())
    {
        specialization->MakeVTab(context, fullSpan);
    }
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped); 
}

void InstantiateDestructor(ClassTemplateSpecializationSymbol* specialization, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::set<const Symbol*> tpi_visited;
    if (specialization->IsTemplateParameterInstantiation(context, tpi_visited)) return;
    if (specialization->Destructor(context)) return;
    std::set<const TypeSymbol*> visited;
    const TypeSymbol* incompleteType = nullptr;
    if (!specialization->IsComplete(visited, incompleteType, context)) return;
    if (specialization->InstantiatingDestructor()) return;
    specialization->SetInstantiatingDestructor(true);
    context->PushResetFlag(ContextFlags::skipFunctionDefinitions);
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    ClassTypeSymbol* classTemplate = specialization->ClassTemplate(context);
    Symbol* destructor = classTemplate->GetScope()->Lookup("@destructor", SymbolGroupKind::functionSymbolGroup, ScopeLookup::thisScope, fullSpan, context,
        LookupFlags::dontResolveSingle);
    if (destructor && destructor->IsFunctionGroupSymbol())
    {
        FunctionGroupSymbol* destructorGroup = static_cast<FunctionGroupSymbol*>(destructor);
        FunctionDefinitionSymbol* destructorFn = destructorGroup->GetSingleDefinition(context);
        if (destructorFn)
        {
            destructorFn->SetNoExcept();
            std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual> templateParameterMap;
            FunctionSymbol* instantiatedDestructor = InstantiateMemFnOfClassTemplate(destructorFn, specialization,
                templateParameterMap, fullSpan, context);
            instantiatedDestructor->SetNoExcept();
            instantiatedDestructor->SetFlag(FunctionSymbolFlags::fixedIrName);
            std::string irName = instantiatedDestructor->IrName(context);
            specialization->SetDestructor(instantiatedDestructor);
        }
    }
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
    context->PopFlags();
    specialization->SetInstantiatingDestructor(false);
}

ClassTemplateSpecializationSymbol* InstantiateClassTemplate(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArgs,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    return InstantiateClassTemplate(classTemplate, templateArgs, fullSpan, context, false);
}

ClassTemplateSpecializationSymbol* InstantiateClassTemplate(ClassTypeSymbol* classTemplate, const std::vector<Symbol*>& templateArgs,
    const soul::ast::FullSpan& fullSpan, Context* context, bool createNew)
{
    if (classTemplate->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(classTemplate);
        classTemplate = specialization->ClassTemplate(context);
    }
    TemplateDeclarationSymbol* templateDeclaration = classTemplate->ParentTemplateDeclaration(context);
    if (!templateDeclaration)
    {
        ThrowException("otava.symbols.class_templates: template declaration not found from class template '" + classTemplate->Name() + "'",
            fullSpan, context);
    }
    Cardinality arity = templateDeclaration->Arity();
    ClassTemplateSpecializationSymbol* specialization = context->GetSymbolTable()->MakeClassTemplateSpecialization(
        classTemplate, templateArgs, fullSpan, context, createNew);
    std::string fname = specialization->FullName(context);
    specialization->IrName(context);
    Cardinality m = Cardinality(templateArgs.size());
    bool wasInstantiated = specialization->Instantiated();
    if (wasInstantiated && arity == m)
    {
        return specialization;
    }
    SpecializationKey key;
    key.typeSymbolId = specialization->ClassTemplate(context)->Id();
    for (Symbol* templateArgument : specialization->TemplateArguments(context))
    {
        key.templateArgumentIds.push_back(templateArgument->Id());
    }
    ExplicitInstantiationSymbol* explicitInstantiation = context->GetSymbolTable()->GetExplicitInstantiation(key, context);
    if (explicitInstantiation)
    {
        return explicitInstantiation->Specialization(context);
    }
    otava::ast::Node* classNode = context->GetSymbolTable()->GetSpecifierNode(classTemplate);
    if (!classNode)
    {
        classTemplate->GetModule()->ReadAstNode();
        std::int64_t astNodeId = classTemplate->AstNodeId();
        classNode = classTemplate->GetModule()->GetAstNode(astNodeId);
        if (!classNode)
        {
            return specialization;
        }
    }
    if (classNode->IsClassSpecifierNode())
    {
        otava::ast::ClassSpecifierNode* classSpecifierNode = static_cast<otava::ast::ClassSpecifierNode*>(classNode);
        if (!classSpecifierNode->Complete())
        {
            return specialization;
        }
    }
    specialization->SetInstantiated();
    context->GetBoundCompileUnit()->AddBoundNodeForClass(specialization, fullSpan, context); 
    Cardinality argCount = Cardinality(templateArgs.size());
    if (argCount > arity)
    {
        ThrowException("otava.symbols.class_templates: wrong number of template args for instantiating class template '" + classTemplate->Name() + "'", fullSpan, context);
    }
    specialization->GetScope()->AddParentScope(context->GetSymbolTable()->CurrentScope());
    specialization->GetScope()->AddParentScope(specialization->ClassTemplate(context)->GetScope()->GetNamespaceScope(context));
    context->GetSymbolTable()->BeginScope(specialization->GetScope());
    InstantiationScope instantiationScope(context->GetModule(), specialization->GetScope());
    std::vector<std::unique_ptr<BoundTemplateParameterSymbol>> boundTemplateParameters;
    for (Index i = Index(0); i < Index(arity); ++i)
    {
        TemplateParameterSymbol* templateParameter = templateDeclaration->TemplateParameters(context)[ToUnderlying(i)];
        Symbol* templateArg = nullptr;
        if (i >= Index(argCount))
        {
            otava::ast::Node* defaultTemplateArgNode = templateParameter->DefaultTemplateArg();
            if (defaultTemplateArgNode)
            {
                context->GetSymbolTable()->BeginScope(&instantiationScope);
                templateArg = ResolveType(defaultTemplateArgNode, DeclarationFlags::none, context);
                context->GetSymbolTable()->EndScope();
                if (Cardinality(specialization->TemplateArguments(context).size()) < arity)
                {
                    std::vector<Symbol*> newTemplateArgs(templateArgs);
                    newTemplateArgs.push_back(templateArg);
                    return InstantiateClassTemplate(classTemplate, newTemplateArgs, fullSpan, context);
                }
            }
            else
            {
                ThrowException("otava.symbols.templates: template parameter " + std::to_string(ToUnderlying(i)) + " has no default type argument", fullSpan, context);
            }
        }
        else
        {
            templateArg = templateArgs[ToUnderlying(i)];
        }
        if (templateArg->IsTypeSymbol())
        {
            TypeSymbol* templateArgType = static_cast<TypeSymbol*>(templateArg);
            TypeSymbol* specialization = classTemplate->Specialization(context);
            if (specialization)
            {
                CompoundTypeSymbol* specializationArgType = GetCompoundSpecializationArgType(specialization, i, context);
                if (specializationArgType)
                {
                    templateArg = templateArgType->RemoveDerivations(specializationArgType->GetDerivations(), context);
                }
                else
                {
                    ClassTemplateSpecializationSymbol* specializationArgType = GetClassTemplateSpecializationArgType(specialization, i, context);
                    if (!specializationArgType)
                    {
                        ThrowException("otava.symbols.templates: specialization argument type not resolved", fullSpan, context);
                    }
                }
            }
        }
        BoundTemplateParameterSymbol* boundTemplateParameter = new BoundTemplateParameterSymbol(context->GetModule(), 
            context->GetNextSymbolId(SymbolKind::boundTemplateParameterSymbol), templateParameter->Name());
        boundTemplateParameter->SetTemplateParameterSymbol(templateParameter);
        boundTemplateParameter->SetBoundSymbol(templateArg);
        boundTemplateParameters.push_back(std::unique_ptr<BoundTemplateParameterSymbol>(boundTemplateParameter));
        instantiationScope.Install(boundTemplateParameter, context);
        context->GetSymbolTable()->MapSymbol(boundTemplateParameter);
    }
    if (wasInstantiated)
    {
        context->GetSymbolTable()->EndScope();
        specialization->GetScope()->ClearParentScopes();
        for (const auto& boundTemplateParameter : boundTemplateParameters)
        {
            //context->GetSymbolTable()->UnmapType(boundTemplateParameter.get());
        }
        return specialization;
    }
    specialization->SetNextMemFnDefIndex(classTemplate->NextMemFnDefIndex());
    context->GetSymbolTable()->BeginScope(&instantiationScope);
    Instantiator instantiator(context, &instantiationScope);
    try
    {
        context->PushSetFlag(ContextFlags::dontBind | ContextFlags::skipFunctionDefinitions);
        classNode->Accept(instantiator);
        std::vector<ClassTypeSymbol*> baseClasses = instantiator.GetBaseClasses();
        for (ClassTypeSymbol* baseClass : baseClasses)
        {
            specialization->AddBaseClass(baseClass, fullSpan, context);
        }
        context->PopFlags();
        std::set<const Symbol*> visited;
        if (!specialization->IsTemplateParameterInstantiation(context, visited))
        {
            InstantiateDestructor(specialization, fullSpan, context);
            InstantiateVirtualFunctions(specialization, fullSpan, context);
            context->GetSymbolTable()->AddClass(specialization); 
        }
    }
    catch (const std::exception& ex)
    {
        ThrowException("otava.symbols.templates: error instantiating specialization '" +
            specialization->FullName(context) + "': " + std::string(ex.what()), fullSpan, context);
    }
    context->GetSymbolTable()->EndScope();
    context->GetSymbolTable()->EndScope();
    specialization->GetScope()->ClearParentScopes();
    for (const auto& boundTemplateParameter : boundTemplateParameters)
    {
        //context->GetSymbolTable()->UnmapType(boundTemplateParameter.get()); TODO
    }
/*
    if (!specialization->IsProject())
    {
        specialization->SetProject();
        context->GetSymbolTable()->AddChangedClassTemplateSpecialization(specialization);
    }
*/
    return specialization;
}

CompoundTypeSymbol* GetCompoundSpecializationArgType(TypeSymbol* specialization, Index index, Context* context) noexcept
{
    if (specialization->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* specializationSymbol = static_cast<ClassTemplateSpecializationSymbol*>(specialization);
        if (index >= Index(0) && index < Index(specializationSymbol->TemplateArguments(context).size()))
        {
            Symbol* specializationArg = specializationSymbol->TemplateArguments(context)[ToUnderlying(index)];
            if (specializationArg->IsCompoundTypeSymbol())
            {
                CompoundTypeSymbol* specializationArgType = static_cast<CompoundTypeSymbol*>(specializationArg);
                return specializationArgType;
            }
        }
    }
    return nullptr;
}

ClassTemplateSpecializationSymbol* GetClassTemplateSpecializationArgType(TypeSymbol* specialization, Index index, Context* context) noexcept
{
    if (specialization->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* specializationSymbol = static_cast<ClassTemplateSpecializationSymbol*>(specialization);
        if (index >= Index(0) && index < Index(specializationSymbol->TemplateArguments(context).size()))
        {
            Symbol* specializationArg = specializationSymbol->TemplateArguments(context)[ToUnderlying(index)];
            if (specializationArg->IsClassTemplateSpecializationSymbol())
            {
                ClassTemplateSpecializationSymbol* specializationArgType = static_cast<ClassTemplateSpecializationSymbol*>(specializationArg);
                return specializationArgType;
            }
        }
    }
    return nullptr;
}

FunctionSymbol* InstantiateMemFnOfClassTemplate(FunctionSymbol* memFn, ClassTemplateSpecializationSymbol* classTemplateSpecialization,
    const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>& 
    templateParameterMap, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::string specializationName = memFn->Name();
    if (!classTemplateSpecialization)
    {
        ClassTypeSymbol* classTemplate = memFn->ParentClassType(context);
        std::vector<Symbol*> templateArguments;
        TemplateDeclarationSymbol* templateDeclaration = classTemplate->ParentTemplateDeclaration(context);
        for (TemplateParameterSymbol* templateParameter : templateDeclaration->TemplateParameters(context))
        {
            auto it = templateParameterMap.find(templateParameter);
            if (it != templateParameterMap.cend())
            {
                TypeSymbol* templateArgumentType = it->second;
                templateArguments.push_back(templateArgumentType);
            }
            else
            {
                ThrowException("template parameter type not found", fullSpan, context);
            }
        }
        classTemplateSpecialization = InstantiateClassTemplate(classTemplate, templateArguments, fullSpan, context, true);
    }
    else
    {
        classTemplateSpecialization = InstantiateClassTemplate(classTemplateSpecialization->ClassTemplate(context),
            classTemplateSpecialization->TemplateArguments(context), fullSpan, context, true);
    }
    classTemplateSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(classTemplateSpecialization->FinalType(fullSpan, context));
    context->GetBoundCompileUnit()->AddBoundNodeForClass(classTemplateSpecialization, fullSpan, context);
    ClassTemplateRepository* classTemplateRepository = context->GetBoundCompileUnit()->GetClassTemplateRepository();
    std::vector<TypeSymbol*> templateArgumentTypes;
    int n = classTemplateSpecialization->TemplateArguments(context).size();
    for (int i = 0; i < n; ++i)
    {
        Symbol* templateArg = classTemplateSpecialization->TemplateArguments(context)[i];
        if (templateArg->IsTypeSymbol())
        {
            TypeSymbol* templateArgumentType = static_cast<TypeSymbol*>(templateArg);
            templateArgumentTypes.push_back(templateArgumentType);
        }
    }
    MemFnKey key(memFn, templateArgumentTypes);
    FunctionDefinitionSymbol* functionDefinitionSymbol = classTemplateRepository->GetFunctionDefinition(key, context);
    if (functionDefinitionSymbol)
    {
        return functionDefinitionSymbol;
    }
    SpecializationKey instantiationKey;
    instantiationKey.typeSymbolId = classTemplateSpecialization->ClassTemplate(context)->Id();
    for (Symbol* templateArgument : classTemplateSpecialization->TemplateArguments(context))
    {
        instantiationKey.templateArgumentIds.push_back(templateArgument->Id());
    }
    ExplicitInstantiationSymbol* explicitInstantiation = context->GetSymbolTable()->GetExplicitInstantiation(instantiationKey, context);
    if (explicitInstantiation)
    {
        bool isInline = memFn->IsInline() && context->ReleaseConfig() && otava::optimizer::HasOptimization(otava::optimizer::Optimizations::inlining);
        bool hasDefaultParams = memFn->HasDefaultParams(context);
        if (!isInline && !hasDefaultParams)
        {
            if (memFn->IsFunctionDefinitionSymbol())
            {
                FunctionDefinitionSymbol* memFnDefSymbol = static_cast<FunctionDefinitionSymbol*>(memFn);
                FunctionDefinitionSymbol* functionDefinitionSymbol = explicitInstantiation->GetFunctionDefinitionSymbol(memFnDefSymbol->DefIndex(), context);
                functionDefinitionSymbol->SetDestructor(explicitInstantiation->Destructor());
                return functionDefinitionSymbol;
            }
            else
            {
                ThrowException(memFn->Name() + ": otava.symbols.class_templates: function definition symbol expected", fullSpan, context);
            }
        }
    }
    if (memFn->IsFunctionDefinitionSymbol())
    {
        FunctionDefinitionSymbol* memFnDefSymbol = static_cast<FunctionDefinitionSymbol*>(memFn);
        std::int32_t memFnDefIndex = memFnDefSymbol->DefIndex();
        FunctionDefinitionSymbol* memFnSpecialization = classTemplateSpecialization->GetMemFnDefSymbol(memFnDefIndex);
        if (memFnSpecialization)
        {
            return memFnSpecialization;
        }
    }
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    bool prevParseMemberFunction = context->GetFlag(ContextFlags::parseMemberFunction);
    context->ResetFlag(ContextFlags::parseMemberFunction);
    otava::ast::Node* node = context->GetSymbolTable()->GetNodeNothrow(memFn);
    std::unique_ptr<otava::ast::Node> clonedNode;
    if (!node)
    {
        Module* module = memFn->GetModule();
        module->ReadAstNode();
        node = module->GetAstNode(memFn->AstNodeId());
    }
    if (node)
    {
        clonedNode.reset(node->Clone());
        node = clonedNode.get();
    }
    else
    {
        if (memFn->IsDestructor())
        {
            InstantiateDestructor(classTemplateSpecialization, fullSpan, context);
            return memFn;
        }
        else
        {
            ThrowException("node for symbol '" + memFn->FullName(context) + "' not found", fullSpan, context);
        }
    }
    if (node->IsFunctionDefinitionNode())
    {
        otava::ast::FunctionDefinitionNode* functionDefinitionNode = static_cast<otava::ast::FunctionDefinitionNode*>(node);
        if (functionDefinitionNode->FunctionBody()->IsDefaultedOrDeletedFunctionNode())
        {
            otava::ast::DefaultedOrDeletedFunctionNode* bodyNode = static_cast<otava::ast::DefaultedOrDeletedFunctionNode*>(functionDefinitionNode->FunctionBody());
            if (bodyNode->DefaultOrDelete()->Kind() == otava::ast::NodeKind::deleteNode)
            {
                ThrowException("attempt to instantiate a deleted function", fullSpan, context);
            }
        }
        ClassTypeSymbol* parentClass = memFn->ParentClassType(context);
        if (parentClass)
        {
            if (parentClass->IsClassTemplateSpecializationSymbol())
            {
                ClassTemplateSpecializationSymbol* pc = static_cast<ClassTemplateSpecializationSymbol*>(parentClass);
                parentClass = pc->ClassTemplate(context);
            }
            TemplateDeclarationSymbol* templateDeclaration = parentClass->ParentTemplateDeclaration(context);
            if (templateDeclaration)
            {
                Cardinality arity = templateDeclaration->Arity();
                Cardinality argCount = Cardinality(templateArgumentTypes.size());
                if (argCount > arity)
                {
                    ThrowException("otava.symbols.class_templates: wrong number of template args for instantiating class template member function '" +
                        memFn->Name() + "'", fullSpan, node->GetFullSpan(), context);
                }
                classTemplateSpecialization->GetScope()->AddParentScope(context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context));
                classTemplateSpecialization->GetScope()->AddParentScope(classTemplateSpecialization->ClassTemplate(context)->GetScope()->GetNamespaceScope(context));
                InstantiationScope instantiationScope(context->GetModule(), classTemplateSpecialization->GetScope());
                std::vector<std::unique_ptr<BoundTemplateParameterSymbol>> boundTemplateParameters;
                for (Index i = Index(0); i < Index(arity); ++i)
                {
                    Symbol* templateArg = templateArgumentTypes[ToUnderlying(i)];
                    TemplateParameterSymbol* templateParameter = templateDeclaration->TemplateParameters(context)[ToUnderlying(i)];
                    BoundTemplateParameterSymbol* boundTemplateParameter = new BoundTemplateParameterSymbol(
                        context->GetModule(), context->GetNextSymbolId(SymbolKind::boundTemplateParameterSymbol), templateParameter->Name());
                    boundTemplateParameter->SetTemplateParameterSymbol(templateParameter);
                    boundTemplateParameter->SetBoundSymbol(templateArg);
                    boundTemplateParameters.push_back(std::unique_ptr<BoundTemplateParameterSymbol>(boundTemplateParameter));
                    instantiationScope.Install(boundTemplateParameter, context);
                    context->GetSymbolTable()->MapSymbol(boundTemplateParameter);
                }
                BoundTemplateParameterSymbol* templateNameParameter = new BoundTemplateParameterSymbol(
                    context->GetModule(), context->GetNextSymbolId(SymbolKind::boundTemplateParameterSymbol), classTemplateSpecialization->ClassTemplate(context)->Name());
                templateNameParameter->SetBoundSymbol(classTemplateSpecialization);
                boundTemplateParameters.push_back(std::unique_ptr<BoundTemplateParameterSymbol>(templateNameParameter));
                instantiationScope.Install(templateNameParameter, context);
                context->GetSymbolTable()->MapSymbol(templateNameParameter);
                context->GetSymbolTable()->BeginScope(&instantiationScope);
                Instantiator instantiator(context, &instantiationScope);
                FunctionSymbol* specialization = nullptr;
                try
                {
                    context->PushSetFlag(ContextFlags::instantiateMemFnOfClassTemplate | ContextFlags::saveDeclarations | ContextFlags::dontBind);
                    if (memFn->IsFunctionDefinitionSymbol())
                    {
                        FunctionDefinitionSymbol* memFnDefSymbol = static_cast<FunctionDefinitionSymbol*>(memFn);
                        context->SetMemFunDefSymbolIndex(memFnDefSymbol->DefIndex());
                    }
                    instantiator.SetFunctionNode(functionDefinitionNode);
                    context->SetClassTemplateSpecialization(functionDefinitionNode, classTemplateSpecialization);
                    context->PushResetFlag(ContextFlags::skipFunctionDefinitions);
                    functionDefinitionNode->Accept(instantiator);
                    context->PopFlags();
                    context->SetMemFunDefSymbolIndex(-1);
                    specialization = instantiator.GetSpecialization();
                    context->RemoveSpecialization(functionDefinitionNode);
                    context->RemoveClassTemplateSpecialization(functionDefinitionNode);
                    if (classTemplateSpecialization->ContainsVirtualFunctionSpecialization(specialization))
                    {
                        context->PopFlags();
                        context->GetSymbolTable()->EndScope();
                        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
                        if (prevParseMemberFunction)
                        {
                            context->SetFlag(ContextFlags::parseMemberFunction);
                        }
                        return specialization;
                    }
                    if (specialization && specialization->IsFunctionDefinitionSymbol())
                    {
                        if (specialization->IsVirtual())
                        {
                            classTemplateSpecialization->AddInstantiatedVirtualFunctionSpecialization(specialization);
                        }
                        specializationName = specialization->Name();
                        FunctionDefinitionSymbol* functionDefinition = static_cast<FunctionDefinitionSymbol*>(specialization);
                        functionDefinition->SetFlag(FunctionSymbolFlags::fixedIrName);
                        functionDefinition->SetCompileUnitId(context->GetBoundCompileUnit()->Id());
                        std::string irName = functionDefinition->IrName(context);
                        classTemplateRepository->AddFunctionDefinition(key, functionDefinition);
                        context->PushBoundFunction(new BoundFunctionNode(functionDefinition, fullSpan));
                        Scope* nsScope = classTemplateSpecialization->ClassTemplate(context)->GetScope()->GetNamespaceScope(context);                        
                        instantiationScope.PushParentScope(nsScope);
                        functionDefinition = BindFunction(functionDefinitionNode, functionDefinition, context);
                        specialization = functionDefinition;
                        context->PopFlags();
                        if (specialization->IsBound())
                        {
                            context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(context->ReleaseBoundFunction()), context);
                        }
                        context->PopBoundFunction();
                        instantiationScope.PopParentScope();
                        functionDefinition->GetScope()->ClearParentScopes();
                        //context->GetSymbolTable()->MapClassTemplateSpecialization(classTemplateSpecialization); TODO
                    }
                    else
                    {
                        ThrowException("otava.symbols.class_templates: function definition symbol expected", node->GetFullSpan(), fullSpan, context);
                    }
                }
                catch (const std::exception& ex)
                {
                    ThrowException("otava.symbols.class_templates: error instantiating specialization '" + specializationName +
                        "': " + std::string(ex.what()), node->GetFullSpan(), fullSpan, context);
                }
                context->GetSymbolTable()->EndScope();
                context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
                if (prevParseMemberFunction)
                {
                    context->SetFlag(ContextFlags::parseMemberFunction);
                }
                if (specialization->GroupName() != "@destructor")
                {
                    InstantiateDestructor(classTemplateSpecialization, fullSpan, context);
                }
                for (const auto& boundTemplateParameter : boundTemplateParameters)
                {
                    //context->GetSymbolTable()->UnmapType(boundTemplateParameter.get());
                }
                return specialization;
            }
            else
            {
                ThrowException("otava.symbols.class_templates: parent class template declaration not found", node->GetFullSpan(), fullSpan, context);
            }
        }
        else
        {
            ThrowException("otava.symbols.class_templates: parent class template not found", node->GetFullSpan(), fullSpan, context);
        }
    }
    else
    {
        ClassTypeSymbol* parentClass = memFn->ParentClassType(context);
        if (parentClass)
        {
            if (parentClass->IsClassTemplateSpecializationSymbol())
            {
                ClassTemplateSpecializationSymbol* pc = static_cast<ClassTemplateSpecializationSymbol*>(parentClass);
                parentClass = pc->ClassTemplate(context);
            }
            TemplateDeclarationSymbol* templateDeclaration = parentClass->ParentTemplateDeclaration(context);
            if (templateDeclaration)
            {
                Cardinality arity = templateDeclaration->Arity();
                Cardinality argCount = Cardinality(templateArgumentTypes.size());
                if (argCount > arity)
                {
                    ThrowException("otava.symbols.class_templates: wrong number of template args for instantiating class template member function '" +
                        memFn->Name() + "'", fullSpan, node->GetFullSpan(), context);
                }
                classTemplateSpecialization->GetScope()->AddParentScope(context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context));
                classTemplateSpecialization->GetScope()->AddParentScope(classTemplateSpecialization->ClassTemplate(context)->GetScope()->GetNamespaceScope(context));
                InstantiationScope instantiationScope(context->GetModule(), classTemplateSpecialization->GetScope());
                std::vector<std::unique_ptr<BoundTemplateParameterSymbol>> boundTemplateParameters;
                for (Index i = Index(0); i < Index(arity); ++i)
                {
                    Symbol* templateArg = templateArgumentTypes[ToUnderlying(i)];
                    TemplateParameterSymbol* templateParameter = templateDeclaration->TemplateParameters(context)[ToUnderlying(i)];
                    BoundTemplateParameterSymbol* boundTemplateParameter = new BoundTemplateParameterSymbol(
                        context->GetModule(), context->GetNextSymbolId(SymbolKind::boundTemplateParameterSymbol), templateParameter->Name());
                    boundTemplateParameter->SetTemplateParameterSymbol(templateParameter);
                    boundTemplateParameter->SetBoundSymbol(templateArg);
                    boundTemplateParameters.push_back(std::unique_ptr<BoundTemplateParameterSymbol>(boundTemplateParameter));
                    instantiationScope.Install(boundTemplateParameter, context);
                    context->GetSymbolTable()->MapSymbol(boundTemplateParameter);
                }
                BoundTemplateParameterSymbol* templateNameParameter = new BoundTemplateParameterSymbol(
                    context->GetModule(), context->GetNextSymbolId(SymbolKind::boundTemplateParameterSymbol), classTemplateSpecialization->ClassTemplate(context)->Name());
                templateNameParameter->SetBoundSymbol(classTemplateSpecialization);
                boundTemplateParameters.push_back(std::unique_ptr<BoundTemplateParameterSymbol>(templateNameParameter));
                instantiationScope.Install(templateNameParameter, context);
                context->GetSymbolTable()->MapSymbol(templateNameParameter);
                context->GetSymbolTable()->BeginScope(&instantiationScope);
                Instantiator instantiator(context, &instantiationScope);
                FunctionSymbol* specialization = nullptr;
                try
                {
                    context->PushSetFlag(ContextFlags::instantiateMemFnOfClassTemplate | ContextFlags::saveDeclarations | ContextFlags::dontBind);
                    if (memFn->IsFunctionDefinitionSymbol())
                    {
                        FunctionDefinitionSymbol* memFnDefSymbol = static_cast<FunctionDefinitionSymbol*>(memFn);
                        context->SetMemFunDefSymbolIndex(memFnDefSymbol->DefIndex());
                    }
                    instantiator.SetFunctionNode(node);
                    context->SetClassTemplateSpecialization(node, classTemplateSpecialization);
                    node->Accept(instantiator);
                    context->PopFlags();
                    context->SetMemFunDefSymbolIndex(-1);
                    specialization = instantiator.GetSpecialization();
                    context->RemoveSpecialization(node);
                    context->RemoveClassTemplateSpecialization(node);
                    if (specialization)
                    {
                        specializationName = specialization->FullName(context);
                        if (classTemplateSpecialization->ContainsVirtualFunctionSpecialization(specialization))
                        {
                            context->PopFlags();
                            context->GetSymbolTable()->EndScope();
                            context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
                            if (prevParseMemberFunction)
                            {
                                context->SetFlag(ContextFlags::parseMemberFunction);
                            }
                            return specialization;
                        }
                        if (specialization->IsVirtual())
                        {
                            classTemplateSpecialization->AddInstantiatedVirtualFunctionSpecialization(specialization);
                        }
                    }
                    else
                    {
                        ThrowException("otava.symbols.class_templates: function symbol expected", node->GetFullSpan(), fullSpan, context);
                    }
                    instantiationScope.PopParentScope();
                }
                catch (const std::exception& ex)
                {
                    ThrowException("otava.symbols.class_templates: error instantiating specialization '" + specializationName +
                        "': " + std::string(ex.what()), node->GetFullSpan(), fullSpan, context);
                }
                context->GetSymbolTable()->EndScope();
                context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
                if (prevParseMemberFunction)
                {
                    context->SetFlag(ContextFlags::parseMemberFunction);
                }
                for (const auto& boundTemplateParameter : boundTemplateParameters)
                {
                    //context->GetSymbolTable()->UnmapType(boundTemplateParameter.get());
                }
                return specialization;
            }
            else
            {
                ThrowException("otava.symbols.class_templates: parent class template declaration not found", node->GetFullSpan(), fullSpan, context);
            }
        }
        else
        {
            ThrowException("otava.symbols.class_templates: parent class template not found", node->GetFullSpan(), fullSpan, context);
        }
    }
    return nullptr;
}

} // namespace otava::symbol
