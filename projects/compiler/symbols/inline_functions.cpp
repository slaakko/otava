// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.inline_functions;

import otava.symbols.bound_tree;
import otava.symbols.classes;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.function_kind;
import otava.symbols.instantiator;
import otava.symbols.modules;
import otava.symbols.scope;
import otava.symbols.scope_ptr;
import otava.symbols.statement_binder;
import otava.ast.function;
import otava.ast.node;

namespace otava::symbols {

InlineFunctionRepository::InlineFunctionRepository()
{
}

void InlineFunctionRepository::AddInlineFunction(FunctionSymbol* fn, FunctionSymbol* inlineFn)
{
    inlineFunctionMap[fn] = inlineFn;
}

FunctionSymbol* InlineFunctionRepository::GetInlineFunction(FunctionSymbol* fn) const noexcept
{
    auto it = inlineFunctionMap.find(fn);
    if (it != inlineFunctionMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

class InlineScopePtr
{
public:
    InlineScopePtr(Context* context_, Scope* scope) noexcept;
    ~InlineScopePtr();
private:
    Context* context;
};

InlineScopePtr::InlineScopePtr(Context* context_, Scope* scope) noexcept : context(context_)
{
    context->PushInlineScope(scope);
}

InlineScopePtr::~InlineScopePtr()
{
    context->PopInlineScope();
}

class InlineModulePtr
{
public:
    InlineModulePtr(Context* context_, Module* module_) noexcept;
    ~InlineModulePtr();
private:
    Context* context;
    Module* module;
};

InlineModulePtr::InlineModulePtr(Context* context_, Module* module_) noexcept : context(context_), module(module_)
{
    context->PushInlineModule(module);
}

InlineModulePtr::~InlineModulePtr()
{
    context->PopInlineModule();
}

FunctionSymbol* TryInstantiateInlineFunction(Context* context, Instantiator& instantiator, otava::ast::FunctionDefinitionNode* functionDefinitionNode, 
    FunctionSymbol* inlineFn, FunctionSymbol* fn, InlineFunctionRepository* inlineFunctionRepository, const soul::ast::FullSpan& fullSpan, otava::ast::Node* node)
{
    FlagSetter flagSetter(context, ContextFlags::instantiateInlineFunction | ContextFlags::saveDeclarations | ContextFlags::dontBind);
    instantiator.SetFunctionNode(functionDefinitionNode);
    Scope* inlineNsScope = fn->GetScope()->GetNamespaceScope(context);
    InlineScopePtr inlineScopePtr(context, inlineNsScope);
    InlineModulePtr inlineModulePtr(context, fn->GetModule());
    functionDefinitionNode->Accept(instantiator);
    inlineFn = instantiator.GetSpecialization();
    context->RemoveSpecialization(functionDefinitionNode);
    if (inlineFn && inlineFn->IsFunctionDefinitionSymbol())
    {
        inlineFn->SetSpecialization();
        FunctionDefinitionSymbol* functionDefinition = static_cast<FunctionDefinitionSymbol*>(inlineFn);
        ParentScopeCleaner parentScopeCleaner(functionDefinition->GetScope());
        functionDefinition->SetFlag(FunctionSymbolFlags::fixedIrName);
        functionDefinition->SetCompileUnitId(context->GetBoundCompileUnit()->Id());
        functionDefinition->SetParent(fn->Parent(context));
        std::string irName = functionDefinition->IrName(context);
        inlineFunctionRepository->AddInlineFunction(fn, inlineFn);
        context->PushBoundFunction(new BoundFunctionNode(functionDefinition, fullSpan));
        BoundFunctionGuard boundFunctionGuard(context);
        functionDefinition = BindFunction(functionDefinitionNode, functionDefinition, context);
        inlineFn = functionDefinition;
        inlineFn->SetSkip();
        if (functionDefinition->IsBound())
        {
            std::unique_ptr<BoundNode> boundNode(context->ReleaseBoundFunction());
            context->GetBoundCompileUnit()->AddBoundNode(std::move(boundNode), context);
        }
    }
    else
    {
        ThrowException("otava.symbols.inline_functions: function definition symbol expected", node->GetFullSpan(), fullSpan, context);
    }
    return inlineFn;
}

FunctionSymbol* InstantiateInlineFunction(FunctionSymbol* fn, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (fn->IsGenerated()) return fn;
    InlineFunctionRepository* inlineFunctionRepository = context->GetBoundCompileUnit()->GetInlineFunctionRepository();
    FunctionSymbol* inlineFn = inlineFunctionRepository->GetInlineFunction(fn);
    if (inlineFn)
    {
        return inlineFn;
    }
    Module* module = fn->GetModule();
    module->ReadAstNode();
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped(); 
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    otava::ast::Node* node = module->GetAstNode(fn->AstNodeId());
    if (!node)
    {
        node = context->GetSymbolTable()->GetNodeNothrow(fn, context);
    }
    std::unique_ptr<otava::ast::Node> clonedNode;
    if (node && node->IsFunctionDefinitionNode())
    {
        clonedNode.reset(node->Clone());
        node = clonedNode.get();
    }
    else
    {
        if (!context->GetFlag(ContextFlags::noWarnings))
        {
            std::cout << "warning: function '" << fn->FullName(context) + "' not inlined because node not found" << "\n";
        }
        return fn;
    }
    if (node->IsFunctionDefinitionNode())
    {
        otava::ast::FunctionDefinitionNode* functionDefinitionNode = static_cast<otava::ast::FunctionDefinitionNode*>(node);
        InstantiationScope instantiationScope(context->GetModule(), fn->Parent(context)->GetScope());
        ParentScopeAdder parentScopeAdder(&instantiationScope, context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context));
        ScopePtr instantiationScopePtr(&instantiationScope, context);
        Instantiator instantiator(context, &instantiationScope);
        FunctionSymbol* inlineFn = nullptr;
        try
        {
            inlineFn = TryInstantiateInlineFunction(context, instantiator, functionDefinitionNode, inlineFn, fn, inlineFunctionRepository, fullSpan, node);
        }
        catch (const std::exception& ex)
        {
            std::string inlineFunctionFullName;
            if (inlineFn)
            {
                inlineFunctionFullName = inlineFn->FullName(context);
            }
            else
            {
                inlineFunctionFullName = fn->FullName(context);
            }
            PrintWarning("failed to instantiate inline function '" + inlineFunctionFullName +
                "': " + std::string(ex.what()), node->GetFullSpan(), fullSpan, context);
            return fn;
        }
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped); 
        return inlineFn;
    }
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped); 
    return fn;
}

} // namespace otava::symbols
