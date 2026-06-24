// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.optimizer.jumps;

import otava.intermediate.util;

namespace otava::optimizer {

void OptimizeJumps(otava::intermediate::Function* fn)
{
    bool optimized = false;
    otava::intermediate::BasicBlock* bb = fn->FirstBasicBlock();
    while (bb)
    {
        otava::intermediate::Instruction* terminator = bb->LastInstruction();
        if (terminator->IsJumpInstruction())
        {
            otava::intermediate::JmpInstruction* jmp = static_cast<otava::intermediate::JmpInstruction*>(terminator);
            otava::intermediate::BasicBlock* target = jmp->TargetBasicBlock();
            otava::intermediate::BasicBlock* firstTarget = target;
            otava::intermediate::Instruction* leader = target->Leader();
            while (leader->IsJumpInstruction())
            {
                otava::intermediate::JmpInstruction* jmpLeader = static_cast<otava::intermediate::JmpInstruction*>(leader);
                target = jmpLeader->TargetBasicBlock();
                leader = target->Leader();
            }
            if (target != firstTarget)
            {
                jmp->SetTargetBasicBlock(target);
                optimized = true;
            }
            else if (terminator->IsBranchInstruction())
            {
                otava::intermediate::BranchInstruction* branch = static_cast<otava::intermediate::BranchInstruction*>(terminator);
                otava::intermediate::BasicBlock* trueTarget = branch->TrueTargetBasicBlock();
                otava::intermediate::BasicBlock* firstTrueTarget = trueTarget;
                otava::intermediate::Instruction* trueLeader = trueTarget->Leader();
                while (trueLeader->IsJumpInstruction())
                {
                    otava::intermediate::JmpInstruction* jmpLeader = static_cast<otava::intermediate::JmpInstruction*>(trueLeader);
                    trueTarget = jmpLeader->TargetBasicBlock();
                    trueLeader = trueTarget->Leader();
                }
                if (trueTarget != firstTrueTarget)
                {
                    branch->SetTrueTargetBasicBlock(trueTarget);
                    optimized = true;
                }
                otava::intermediate::BasicBlock* falseTarget = branch->FalseTargetBasicBlock();
                otava::intermediate::BasicBlock* firstFalseTarget = falseTarget;
                otava::intermediate::Instruction* falseLeader = falseTarget->Leader();
                while (falseLeader->IsJumpInstruction())
                {
                    otava::intermediate::JmpInstruction* jmpLeader = static_cast<otava::intermediate::JmpInstruction*>(falseLeader);
                    falseTarget = jmpLeader->TargetBasicBlock();
                    falseLeader = falseTarget->Leader();
                }
                if (falseTarget != firstFalseTarget)
                {
                    branch->SetFalseTargetBasicBlock(falseTarget);
                    optimized = true;
                }
                if (branch->Cond()->IsTrue())
                {
                    otava::intermediate::JmpInstruction* jmp = new otava::intermediate::JmpInstruction(soul::ast::Span(), trueTarget->Id());
                    jmp->SetTargetBasicBlock(trueTarget);
                    otava::intermediate::ReplaceInstructionWithInstruction(branch, jmp);
                    optimized = true;
                }
                else if (branch->Cond()->IsFalse())
                {
                    otava::intermediate::JmpInstruction* jmp = new otava::intermediate::JmpInstruction(soul::ast::Span(), falseTarget->Id());
                    jmp->SetTargetBasicBlock(falseTarget);
                    otava::intermediate::ReplaceInstructionWithInstruction(branch, jmp);
                    optimized = true;
                }
            }
        }
        bb = bb->Next();
    }
    if (optimized)
    {
        fn->SetNumbers();
    }
}

} // otava::optimizer
