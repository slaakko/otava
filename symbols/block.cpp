// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.block;

import otava.symbols.context;

namespace otava::symbols {

BlockSymbol::BlockSymbol(Module* module_, SymbolId id_) : ContainerSymbol(module_, id_), blockId(-1)
{
    GetScope()->SetKind(ScopeKind::blockScope);
}

BlockSymbol::BlockSymbol(Module* module_, SymbolId id_, const std::string& name_) : ContainerSymbol(module_, id_, name_), blockId(-1)
{
    GetScope()->SetKind(ScopeKind::blockScope);
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
