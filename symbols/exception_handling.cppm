// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.exception_handling;

import otava.ast.statement;
import soul.ast.span;
import std;

export namespace otava::symbols {

class BoundExpressionNode;
class BoundFunctionNode;
class BoundStatementNode;
class Context;
class FunctionDefinitionSymbol;

class Cleanup;

class CleanupBlock
{
public:
    CleanupBlock(Cleanup* cleanup_);
    inline bool IsEmpty() const noexcept { return destructorCalls.empty(); }
    void Add(BoundExpressionNode* destructorCall, Context* context);
    void Make(otava::ast::CompoundStatementNode* compoundStatement);
    soul::ast::FullSpan GetFullSpan() const;
private:
    Cleanup* cleanup;
    std::vector<std::unique_ptr<BoundExpressionNode>> destructorCalls;
};

class Cleanup
{
public:
    Cleanup();
    bool IsEmpty() const noexcept;
    void PushCleanupBlock();
    void PopCleanupBlock();
    CleanupBlock* CurrentCleanupBlock() const noexcept { return cleanupBlocks.back().get(); }
    inline bool Changed() const noexcept { return changed; }
    inline void SetChanged() noexcept { changed = true; }
    inline void ResetChanged() noexcept { changed = false; }
    void Make(otava::ast::CompoundStatementNode* compoundStatement);
    soul::ast::FullSpan GetFullSpan() const;
private:
    std::vector<std::unique_ptr<CleanupBlock>> cleanupBlocks;
    bool changed;
};

void MakeInvokesAndCleanups(BoundFunctionNode* boundFunction, Context* context);

std::unique_ptr<BoundStatementNode> ConvertReturnStatement(otava::ast::ReturnStatementNode* returnStatement,
    FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context);

} // namespace otava::symbols
