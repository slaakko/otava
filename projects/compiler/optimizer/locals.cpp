// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.optimizer.locals;

namespace otava::optimizer {

void MoveLocalsToEntryBlock(otava::intermediate::Function* fn)
{
    otava::intermediate::BasicBlock* bb = fn->FirstBasicBlock();
    otava::intermediate::BasicBlock* entryBlock = bb;
    otava::intermediate::Instruction* lastEntryBlockLocal = nullptr;
    while (bb)
    {
        otava::intermediate::Instruction* inst = bb->FirstInstruction();
        while (inst)
        {
            otava::intermediate::Instruction* next = inst->Next();
            if (inst->IsLocalInstruction())
            {
                if (bb == entryBlock)
                {
                    lastEntryBlockLocal = inst;
                }
                else
                {
                    std::unique_ptr<otava::intermediate::Instruction> removedLocal = inst->Parent()->RemoveInstruction(inst);
                    entryBlock->Instructions()->InsertAfter(removedLocal.release(), lastEntryBlockLocal);
                    lastEntryBlockLocal = inst;
                }
            }
            inst = next;
        }
        bb = bb->Next();
    }
}

} // namespace otava::optimizer
