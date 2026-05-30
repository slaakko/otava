// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.block;

import std;
import otava.symbols.container_symbol;
import otava.ast.node;

export namespace otava::symbols {

class BlockSymbol : public ContainerSymbol
{
public:
    BlockSymbol(Module* module_, SymbolId id_);
    BlockSymbol(Module* module_, SymbolId id_, const std::string& name_);
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    int blockId;
};

class Context;

BlockSymbol* BeginBlock(const soul::ast::FullSpan& fullSpan, int blockId, Context* context);
void EndBlock(Context* context);
void RemoveBlock(Context* context);
void MapNode(otava::ast::Node* node, Context* context);

} // namespace otava::symbols
