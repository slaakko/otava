// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.util;

namespace otava::intermediate {

void ReplaceInstructionWithValue(Instruction* inst, Value* value)
{
    inst->ReplaceUsesWith(value);
    inst->Parent()->RemoveInstruction(inst);
}

void ReplaceInstructionWithInstruction(Instruction* oldInst, Instruction* newInst)
{
    if (oldInst->IsValueInstruction() && newInst->IsValueInstruction())
    {
        ValueInstruction* valueInst = static_cast<ValueInstruction*>(newInst);
        oldInst->ReplaceUsesWith(valueInst->Result());
    }
    BasicBlock* bb = oldInst->Parent();
    bb->InsertInstructionAfter(newInst, oldInst);
    bb->RemoveInstruction(oldInst);
}

bool IsPowerOfTwo(std::uint64_t n, int& shift)
{
    std::uint64_t p = 2;
    shift = 1;
    while (p != 0 && p < n)
    {
        p <<= 1;
        ++shift;
    }
    if (n == p)
    {
        return true;
    }
    else
    {
        return false;
    }
}

} // otava::intermediate
