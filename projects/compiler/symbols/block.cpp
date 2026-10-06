// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.block;

import otava.symbols.bound_tree;
import otava.symbols.context;
import otava.symbols.function_symbol;
import otava.symbols.variable_symbol;
import otava.symbols.symbol_table;
import otava.symbols.scope;

namespace otava::symbols {

BlockSymbol::BlockSymbol(Module* module_, SymbolId id_) : ContainerSymbol(module_, id_), blockId(-1), index(-1)
{
    GetScope()->SetKind(ScopeKind::blockScope);
}

BlockSymbol::BlockSymbol(Module* module_, SymbolId id_, const std::string& name_) : ContainerSymbol(module_, id_, name_), blockId(-1), index(-1)
{
    GetScope()->SetKind(ScopeKind::blockScope);
}

BlockSymbol::~BlockSymbol()
{
    //std::cout << "~block:" << ToUnderlying(Id()) << " : " << blockId << " : " << index << "\n";
}

void BlockSymbol::AddDestructorCall(int statementIndex, BoundExpressionNode* destructorCall)
{
    destructorCallMap[statementIndex].push_back(destructorCall);
}

bool BlockSymbol::HasDestructorCalls(int statementIndex) const
{
    return destructorCallMap.find(statementIndex) != destructorCallMap.end();
}

int BlockSymbol::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    int index = ContainerSymbol::AddSymbol(symbol, fullSpan, context);
    if (symbol->IsVariableSymbol())
    {
        VariableSymbol* variable = static_cast<VariableSymbol*>(symbol);
        if (variable->IsLocalVariable(context))
        {
            localVariables.push_back(variable);
            FunctionSymbol* function = ParentFunction(context);
            if (function)
            {
                function->AddLocalVariable(variable, context);
            }
        }
    }
    return index;
}

void BlockSymbol::RemoveLocalVariables(Context* context)
{
    FunctionSymbol* functionParent = ParentFunction(context);
    if (functionParent)
    {
        for (VariableSymbol* localVariable : localVariables)
        {
            functionParent->RemoveLocalVariable(localVariable->FunctionIndex());
        }
    }
}

std::vector<BoundExpressionNode*> BlockSymbol::GetDestructorCalls(int statementIndex)
{
    std::vector<BoundExpressionNode*> v = destructorCallMap[statementIndex];
    return v;
}

BlockSymbol* BeginBlock(const soul::ast::FullSpan& fullSpan, int blockId, Context* context)
{
    BlockSymbol* block = context->GetSymbolTable()->BeginBlock(fullSpan, context);
    block->SetBlockId(blockId);
    return block;
}

void EndBlock(Context* context)
{
    context->GetSymbolTable()->EndBlock(context);
}

void RemoveBlock(Context* context, int index)
{
    context->GetSymbolTable()->RemoveBlock(context, index);
}

void MapNode(otava::ast::Node* node, Context* context)
{
    context->GetSymbolTable()->MapNode(node, context);
}

} // namespace otava::symbols
