// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.inline_functions;

import otava.symbols.classes;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.function_symbol;
import otava.symbols.instantiator;
import otava.symbols.statement_binder;
import otava.ast.function;

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
    otava::ast::Node* node = context->GetSymbolTable()->GetNodeNothrow(fn);
    if (!node)
    {
        node = module->GetAstNode(fn->AstNodeId());
    }
    std::unique_ptr<otava::ast::Node> clonedNode;
    if (node)
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
        //instantiationScope.PushParentScope(context->GetSymbolTable()->GetNamespaceScope(U"std", sourcePos, context));
        instantiationScope.PushParentScope(context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context));
        context->GetSymbolTable()->BeginScope(&instantiationScope);
        Instantiator instantiator(context, &instantiationScope);
        FunctionSymbol* inlineFn = nullptr;
        try
        {
            context->PushSetFlag(ContextFlags::instantiateInlineFunction | ContextFlags::saveDeclarations | ContextFlags::dontBind);
            instantiator.SetFunctionNode(functionDefinitionNode);
            functionDefinitionNode->Accept(instantiator);
            inlineFn = instantiator.GetSpecialization();
            context->RemoveSpecialization(functionDefinitionNode);
            if (inlineFn && inlineFn->IsFunctionDefinitionSymbol())
            {
                inlineFn->SetSpecialization();
                FunctionDefinitionSymbol* functionDefinition = static_cast<FunctionDefinitionSymbol*>(inlineFn);
                functionDefinition->SetFlag(FunctionSymbolFlags::fixedIrName);
                functionDefinition->SetCompileUnitId(context->GetBoundCompileUnit()->Id());
                functionDefinition->SetParent(fn->Parent(context));
                std::string irName = functionDefinition->IrName(context);
                inlineFunctionRepository->AddInlineFunction(fn, inlineFn);
                context->PushBoundFunction(new BoundFunctionNode(functionDefinition, fullSpan));
                functionDefinition = BindFunction(functionDefinitionNode, functionDefinition, context);
                inlineFn = functionDefinition;
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
                ThrowException("otava.symbols.inline_functions: function definition symbol expected", node->GetFullSpan(), fullSpan, context);
            }
        }
        catch (const std::exception& ex)
        {
            std::string inlineFunctionFullName;
            if (inlineFn)
            {
                inlineFunctionFullName = inlineFn->FullName(context);
            }
            PrintWarning("failed to instantiating inline function '" + inlineFunctionFullName +
                "': " + std::string(ex.what()), node->GetFullSpan(), fullSpan, context);
            return fn;
        }
        context->GetSymbolTable()->EndScope();
        //instantiationScope.PopParentScope();
        instantiationScope.PopParentScope();
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped); 
        return inlineFn;
    }
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped); 
    return fn;
}

} // namespace otava::symbols
