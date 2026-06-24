// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.optimizer.dead_code_elimination;

namespace otava::optimizer {

void Walk(intermediate::BasicBlock* bb, std::set<intermediate::BasicBlock*>& reachable)
{
    if (reachable.find(bb) != reachable.end()) return;
    reachable.insert(bb);
    intermediate::Instruction* terminator = bb->LastInstruction();
    switch (terminator->GetOpCode())
    {
    case intermediate::OpCode::jmp:
    {
        intermediate::JmpInstruction* jmp = static_cast<intermediate::JmpInstruction*>(terminator);
        intermediate::BasicBlock* target = jmp->TargetBasicBlock();
        Walk(target, reachable);
        break;
    }
    case intermediate::OpCode::branch:
    {
        intermediate::BranchInstruction* br = static_cast<intermediate::BranchInstruction*>(terminator);
        intermediate::BasicBlock* trueTarget = br->TrueTargetBasicBlock();
        Walk(trueTarget, reachable);
        intermediate::BasicBlock* falseTarget = br->FalseTargetBasicBlock();
        Walk(falseTarget, reachable);
        break;
    }
    case intermediate::OpCode::switch_:
    {
        intermediate::SwitchInstruction* sw = static_cast<intermediate::SwitchInstruction*>(terminator);
        intermediate::BasicBlock* defaultBlock = sw->DefaultTargetBlock();
        Walk(defaultBlock, reachable);
        for (const intermediate::CaseTarget& caseTarget : sw->CaseTargets())
        {
            intermediate::BasicBlock* caseBlock = caseTarget.targetBlock;
            Walk(caseBlock, reachable);
        }
        break;
    }
    }
}

void DeadCodeElimination(otava::intermediate::Function* fn)
{
    bool optimized = false;
    std::set<intermediate::BasicBlock*> reachable;
    intermediate::BasicBlock* entry = fn->FirstBasicBlock();
    Walk(entry, reachable);
    intermediate::BasicBlock* bb = fn->FirstBasicBlock();
    while (bb)
    {
        intermediate::BasicBlock* next = bb->Next();
        if (reachable.find(bb) == reachable.end())
        {
            fn->RemoveBasicBlock(bb);
            optimized = true;
        }
        bb = next;
    }
    if (optimized)
    {
        fn->SetNumbers();
    }
}

} // otava::optimizer
