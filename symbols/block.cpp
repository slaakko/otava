// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.block;

import otava.symbols.context;
import otava.symbols.function_symbol;
import otava.symbols.variable_symbol;

namespace otava::symbols {

BlockSymbol::BlockSymbol(Module* module_, SymbolId id_) : ContainerSymbol(module_, id_), blockId(-1)
{
    GetScope()->SetKind(ScopeKind::blockScope);
}

BlockSymbol::BlockSymbol(Module* module_, SymbolId id_, const std::string& name_) : ContainerSymbol(module_, id_, name_), blockId(-1)
{
    GetScope()->SetKind(ScopeKind::blockScope);
}

void BlockSymbol::AddDestructorCall(int statementIndex, BoundExpressionNode* destructorCall)
{
    destructorCallMap[statementIndex].push_back(destructorCall);
}

bool BlockSymbol::HasDestructorCalls(int statementIndex) const
{
    return destructorCallMap.find(statementIndex) != destructorCallMap.end();
}

void BlockSymbol::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ContainerSymbol::AddSymbol(symbol, fullSpan, context);
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

void RemoveBlock(Context* context)
{
    context->GetSymbolTable()->RemoveBlock();
}

void MapNode(otava::ast::Node* node, Context* context)
{
    context->GetSymbolTable()->MapNode(node);
}


} // namespace otava::symbols
