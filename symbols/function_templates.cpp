// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_templates;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.function_symbol;
import otava.symbols.instantiator;
import otava.symbols.instantiation_queue;
import otava.symbols.statement_binder;
import otava.symbols.templates;
import otava.symbols.type_resolver;
import otava.ast.function;

namespace otava::symbols {


FunctionTemplateKey::FunctionTemplateKey(SymbolId functionTemplateId_, const std::vector<SymbolId>& templateArgumentTypeIds_) :
    functionTemplateId(functionTemplateId_), templateArgumentTypeIds(templateArgumentTypeIds_)
{
}

size_t FunctionTemplateKeyHash::operator()(const FunctionTemplateKey& key) const noexcept
{
    size_t hashCode = std::hash<std::uint32_t>()(ToUnderlying(key.functionTemplateId));
    Cardinality count = Cardinality(key.templateArgumentTypeIds.size());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId argId = key.templateArgumentTypeIds[ToUnderlying(i)];
        size_t argHashCode = std::hash<std::uint32_t>()(ToUnderlying(argId));
        hashCode ^= (argHashCode << ToUnderlying(i + Index(1))) | (argHashCode >> ToUnderlying(Index(count) - i + Index(1)));
    }
    return hashCode;
}

bool FunctionTemplateKeyEqual::operator()(const FunctionTemplateKey& left, const FunctionTemplateKey& right) const noexcept
{
    if (left.functionTemplateId != right.functionTemplateId) return false;
    if (left.templateArgumentTypeIds.size() != right.templateArgumentTypeIds.size()) return false;
    Cardinality n = Cardinality(left.templateArgumentTypeIds.size());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        if (left.templateArgumentTypeIds[ToUnderlying(i)] != right.templateArgumentTypeIds[ToUnderlying(i)]) return false;
    }
    return true;
}

FunctionTemplateRepository::FunctionTemplateRepository()
{
}

FunctionDefinitionSymbol* FunctionTemplateRepository::GetFunctionDefinition(const FunctionTemplateKey& key, Context* context)
{
    auto it = functionTemplateMap.find(key);
    if (it != functionTemplateMap.end())
    {
        SymbolId id = it->second;
        FunctionDefinitionSymbol* definition = context->GetSymbolTable()->GetFunctionDefinitionSymbol(id, context);
        if (!definition)
        {
            ThrowException("function definition id " + std::to_string(ToUnderlying(id)) + " not found from module '" + context->GetModule()->Name() + "'");
        }
        return definition;
    }
    return nullptr;
}

void FunctionTemplateRepository::AddFunctionDefinition(const FunctionTemplateKey& key,
    FunctionDefinitionSymbol* functionDefinitionSymbol, otava::ast::Node* functionDefinitionNode)
{
    functionTemplateMap[key] = functionDefinitionSymbol->Id();
    functionDefinitionNodes.push_back(std::unique_ptr<otava::ast::Node>(functionDefinitionNode));
}

FunctionSymbol* InstantiateFunctionTemplate(FunctionSymbol* functionTemplate,
    const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>& templateParameterMap,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::string fname = functionTemplate->FullName(context);
    FunctionTemplateRepository* functionTemplateRepository = context->GetBoundCompileUnit()->GetFunctionTemplateRepository();
    std::vector<TypeSymbol*> templateArgumentTypes;
    TemplateDeclarationSymbol* templateDeclaration = functionTemplate->ParentTemplateDeclaration(context);
    for (TemplateParameterSymbol* templateParameter : templateDeclaration->TemplateParameters(context))
    {
        auto it = templateParameterMap.find(templateParameter);
        if (it != templateParameterMap.cend())
        {
            TypeSymbol* templateArgumentType = it->second;
            templateArgumentTypes.push_back(templateArgumentType);
        }
        else
        {
            ThrowException("template parameter type not found", fullSpan, context);
        }
    }
    std::vector<SymbolId> templateArgumentIds;
    for (TypeSymbol* templateArgument : templateArgumentTypes)
    {
        templateArgumentIds.push_back(templateArgument->Id());
    }
    FunctionTemplateKey key(functionTemplate->Id(), templateArgumentIds);
    FunctionDefinitionSymbol* functionDefinitionSymbol = functionTemplateRepository->GetFunctionDefinition(key, context);
    if (functionDefinitionSymbol)
    {
        return functionDefinitionSymbol;
    }
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped(); 
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    otava::ast::Node* node = context->GetSymbolTable()->GetNodeNothrow(functionTemplate);
    std::unique_ptr<otava::ast::Node> clonedNode;
    if (node)
    {
        clonedNode.reset(node->Clone());
        node = clonedNode.get();
    }
    else
    {
        Module* module = functionTemplate->GetModule();
        module->ReadAstNode();
        std::int64_t astNodeId = functionTemplate->AstNodeId();
        node = module->GetAstNode(astNodeId);
        if (node)
        {
            clonedNode.reset(node->Clone());
            node = clonedNode.get();
        }
        else
        {
            ThrowException("node for symbol '" + functionTemplate->FullName(context) + "' not found", fullSpan, context);
        }
    }
    if (node->IsFunctionDefinitionNode())
    {
        otava::ast::FunctionDefinitionNode* functionDefinitionNode = static_cast<otava::ast::FunctionDefinitionNode*>(node);
        Cardinality arity = templateDeclaration->Arity();
        Cardinality argCount = Cardinality(templateArgumentTypes.size());
        if (argCount > arity)
        {
            ThrowException("otava.symbols.function_templates: wrong number of template args for instantiating function template '" +
                functionTemplate->Name() + "'", node->GetFullSpan(), fullSpan, context);
        }
        InstantiationScope instantiationScope(context->GetModule(), functionTemplate->Parent(context)->GetScope());
        instantiationScope.PushParentScope(context->GetSymbolTable()->GetNamespaceScope("std", fullSpan, context));
        instantiationScope.PushParentScope(context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context));
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
                }
                else
                {
                    ThrowException("otava.symbols.function_templates: template parameter " +
                        std::to_string(ToUnderlying(i)) + " has no default type argument", node->GetFullSpan(), fullSpan, context);
                }
            }
            else
            {
                templateArg = templateArgumentTypes[ToUnderlying(i)];
            }
            BoundTemplateParameterSymbol* boundTemplateParameter = new BoundTemplateParameterSymbol(context->GetModule(), 
                context->GetNextSymbolId(SymbolKind::boundTemplateParameterSymbol), templateParameter->Name());
            boundTemplateParameter->SetTemplateParameterSymbol(templateParameter);
            boundTemplateParameter->SetBoundSymbol(templateArg);
            boundTemplateParameters.push_back(std::unique_ptr<BoundTemplateParameterSymbol>(boundTemplateParameter));
            instantiationScope.Install(boundTemplateParameter, context);
            context->GetSymbolTable()->MapSymbol(boundTemplateParameter);
        }
        context->GetSymbolTable()->BeginScope(&instantiationScope);
        Instantiator instantiator(context, &instantiationScope);
        FunctionSymbol* specialization = nullptr;
        try
        {
            context->PushSetFlag(ContextFlags::instantiateFunctionTemplate | ContextFlags::saveDeclarations | ContextFlags::dontBind);
            instantiator.SetFunctionNode(functionDefinitionNode);
            functionDefinitionNode->Accept(instantiator);
            specialization = instantiator.GetSpecialization();
            context->RemoveSpecialization(functionDefinitionNode);
            if (specialization && specialization->IsFunctionDefinitionSymbol())
            {
                specialization->SetSpecialization();
                FunctionDefinitionSymbol* functionDefinition = static_cast<FunctionDefinitionSymbol*>(specialization);
                functionDefinition->SetFlag(FunctionSymbolFlags::fixedIrName);
                if (!context->InstantiationIrName().empty())
                {
                    functionDefinition->SetFixedIrName(context->InstantiationIrName());
                    context->SetInstantiationIrName(std::string());
                }
                else
                {
                    functionDefinition->SetCompileUnitId(context->GetBoundCompileUnit()->Id());
                }
                std::string irName = functionDefinition->IrName(context);
                functionDefinition->SetParent(functionTemplate->GetScope()->GetNamespaceScope(context)->GetSymbol());
                functionTemplateRepository->AddFunctionDefinition(key, functionDefinition, clonedNode.release());
                context->PushBoundFunction(new BoundFunctionNode(functionDefinition, fullSpan));
                functionDefinition = BindFunction(functionDefinitionNode, functionDefinition, context);
                specialization = functionDefinition;
                context->PopFlags();
                if (functionDefinition->IsBound())
                {
                    context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(context->ReleaseBoundFunction()), context);
                }
                context->PopBoundFunction();
                functionDefinition->GetScope()->ClearParentScopes();
            }
            else
            {
                ThrowException("otava.symbols.function_templates: function definition symbol expected", node->GetFullSpan(), fullSpan, context);
            }
        }
        catch (const std::exception& ex)
        {
            std::string specializationFullName;
            if (specialization)
            {
                specializationFullName = specialization->FullName(context);
            }
            ThrowException("otava.symbols.function_templates: error instantiating specialization '" + specializationFullName +
                "': " + std::string(ex.what()), node->GetFullSpan(), fullSpan, context);
        }
        context->GetSymbolTable()->EndScope();
        instantiationScope.PopParentScope();
        instantiationScope.PopParentScope();
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
        for (const auto& boundTemplateParameter : boundTemplateParameters)
        {
            //context->GetSymbolTable()->UnmapType(boundTemplateParameter.get()); TODO
        }
        specialization->SetSkip();
        return specialization;
    }
    else
    {
        context->GetInstantiationQueue()->EnqueueInstantiationRequest(functionTemplate, templateParameterMap);
        Cardinality arity = templateDeclaration->Arity();
        Cardinality argCount = Cardinality(templateArgumentTypes.size());
        if (argCount > arity)
        {
            ThrowException("otava.symbols.function_templates: wrong number of template args for instantiating function template '" +
                functionTemplate->Name() + "'", node->GetFullSpan(), fullSpan, context);
        }
        InstantiationScope instantiationScope(context->GetModule(), functionTemplate->Parent(context)->GetScope());
        instantiationScope.PushParentScope(context->GetSymbolTable()->GetNamespaceScope("std", fullSpan, context));
        instantiationScope.PushParentScope(context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context));
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
                }
                else
                {
                    ThrowException("otava.symbols.function_templates: template parameter " +
                        std::to_string(ToUnderlying(i)) + " has no default type argument", node->GetFullSpan(), fullSpan, context);
                }
            }
            else
            {
                templateArg = templateArgumentTypes[ToUnderlying(i)];
            }
            BoundTemplateParameterSymbol* boundTemplateParameter = new BoundTemplateParameterSymbol(
                context->GetModule(), context->GetNextSymbolId(SymbolKind::boundTemplateParameterSymbol), templateParameter->Name());
            boundTemplateParameter->SetTemplateParameterSymbol(templateParameter);
            boundTemplateParameter->SetBoundSymbol(templateArg);
            boundTemplateParameters.push_back(std::unique_ptr<BoundTemplateParameterSymbol>(boundTemplateParameter));
            instantiationScope.Install(boundTemplateParameter, context);
            context->GetSymbolTable()->MapSymbol(boundTemplateParameter);
        }
        context->GetSymbolTable()->BeginScope(&instantiationScope);
        Instantiator instantiator(context, &instantiationScope);
        FunctionSymbol* specialization = nullptr;
        try
        {
            context->PushSetFlag(ContextFlags::instantiateFunctionTemplate | ContextFlags::saveDeclarations | ContextFlags::dontBind);
            instantiator.SetFunctionNode(node);
            node->Accept(instantiator);
            specialization = instantiator.GetSpecialization();
            specialization->SetParent(functionTemplate->GetScope()->GetNamespaceScope(context)->GetSymbol());
            context->RemoveSpecialization(node);
            if (specialization)
            {
                specialization->SetSpecialization();
                specialization->SetFlag(FunctionSymbolFlags::fixedIrName);
                if (functionTemplate->IsExplicitSpecializationDeclaration(context))
                {
                    Symbol* symbol = functionTemplate->Parent(context)->GetScope()->GetNamespaceScope(context)->Lookup(specialization->GroupName(),
                        SymbolGroupKind::functionSymbolGroup, ScopeLookup::thisScope, fullSpan, context, LookupFlags::none);
                    if (symbol && symbol->IsFunctionGroupSymbol())
                    {
                        FunctionGroupSymbol* functionGroup = static_cast<FunctionGroupSymbol*>(symbol);
                        FunctionSymbol* functionSymbol = functionGroup->GetMatchingSpecialization(specialization, context);
                        if (functionSymbol)
                        {
                            specialization->SetFixedIrName(functionSymbol->IrName(context));
                        }
                    }
                }
                std::string irName = specialization->IrName(context);
                specialization->GetScope()->ClearParentScopes();
            }
            else
            {
                ThrowException("otava.symbols.function_templates: function symbol expected", node->GetFullSpan(), fullSpan, context);
            }
            context->PopFlags();
        }
        catch (const std::exception& ex)
        {
            std::string specializationName;
            if (specialization)
            {
                specializationName = specialization->Name();
            }
            ThrowException("otava.symbols.function_templates: error instantiating specialization '" + specializationName +
                "': " + std::string(ex.what()), node->GetFullSpan(), fullSpan, context);
        }
        context->GetSymbolTable()->EndScope();
        instantiationScope.PopParentScope();
        instantiationScope.PopParentScope();
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped); 
        for (const auto& boundTemplateParameter : boundTemplateParameters)
        {
            //context->GetSymbolTable()->UnmapType(boundTemplateParameter.get()); TODO
        }
        specialization->SetSkip();
        return specialization;
    }
    return nullptr;
}

} // namespace otava::symbols
