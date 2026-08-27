// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_completion;

import otava.symbols.bound_tree;
import otava.symbols.context;
import otava.symbols.scope;
import otava.symbols.scope_ptr;
import otava.symbols.exception;
import otava.symbols.function_kind;
import otava.symbols.function_symbol;
import otava.symbols.instantiator;
import otava.symbols.modules;
import otava.symbols.statement_binder;
import otava.ast.function;
import otava.ast.node;

namespace otava::symbols {

FunctionCompletionRepository::FunctionCompletionRepository()
{
}

FunctionSymbol* FunctionCompletionRepository::GetCompletedFunction(FunctionSymbol* fn) const noexcept
{
    auto it = completedFunctionMap.find(fn);
    if (it != completedFunctionMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void FunctionCompletionRepository::AddCompletedFunction(FunctionSymbol* fn, FunctionSymbol* completedFunction)
{
    completedFunctionMap[fn] = completedFunction;
}

FunctionSymbol* CompleteIncompleteFunction(FunctionSymbol* fn, const soul::ast::FullSpan& fullSpan, Context* context)
{
    FunctionCompletionRepository* functionCompletionRepository = context->GetBoundCompileUnit()->GetFunctionCompletionRepository();
    FunctionSymbol* completedFn = functionCompletionRepository->GetCompletedFunction(fn);
    if (completedFn)
    {
        return completedFn;
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
            std::cout << "warning: function '" << fn->FullName(context) + "' not completed because node not found" << "\n";
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
        FunctionSymbol* completedFn = nullptr;
        try
        {
            FlagSetter flagSetter(context, ContextFlags::instantiateInlineFunction | ContextFlags::saveDeclarations | ContextFlags::dontBind);
            instantiator.SetFunctionNode(functionDefinitionNode);
            functionDefinitionNode->Accept(instantiator);
            completedFn = instantiator.GetSpecialization();
            context->RemoveSpecialization(functionDefinitionNode);
            if (completedFn && completedFn->IsFunctionDefinitionSymbol())
            {
                completedFn->SetSpecialization();
                FunctionDefinitionSymbol* functionDefinition = static_cast<FunctionDefinitionSymbol*>(completedFn);
                ParentScopeCleaner parentScopeClearn(functionDefinition->GetScope());
                functionDefinition->SetFlag(FunctionSymbolFlags::fixedIrName);
                functionDefinition->SetCompileUnitId(context->GetBoundCompileUnit()->Id());
                functionDefinition->SetParent(fn->Parent(context));
                std::string irName = functionDefinition->IrName(context);
                functionCompletionRepository->AddCompletedFunction(fn, completedFn);
                context->PushBoundFunction(new BoundFunctionNode(functionDefinition, fullSpan));
                functionDefinition = BindFunction(functionDefinitionNode, functionDefinition, context);
                completedFn = functionDefinition;
                completedFn->SetSkip();
                completedFn->SetVTabIndex(fn->VTabIndex());
                if (functionDefinition->IsBound())
                {
                    std::unique_ptr<BoundNode> boundNode(context->ReleaseBoundFunction());
                    context->GetBoundCompileUnit()->AddBoundNode(std::move(boundNode), context);
                }
                context->PopBoundFunction();
            }
            else
            {
                ThrowException("otava.symbols.function_completion: function definition symbol expected", node->GetFullSpan(), fullSpan, context);
            }
        }
        catch (const std::exception& ex)
        {
            std::string completedFunctionFullName;
            if (completedFn)
            {
                completedFunctionFullName = completedFn->FullName(context);
            }
            PrintWarning("failed to complete function '" + completedFunctionFullName +
                "': " + std::string(ex.what()), node->GetFullSpan(), fullSpan, context);
            context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
            return fn;
        }
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
        if (!completedFn->RemoveForwardDeclarationTypes(context, true))
        {
            otava::symbols::PrintWarning("not all forward references could not be removed for function '" +
                completedFn->FullName(context) + "'", completedFn->GetFullSpan(), fullSpan, context);
        }
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
        return completedFn;
    }
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
    return fn;
}

} // namespace otava::symbols
