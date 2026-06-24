// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.optimizer.inliner;

import otava.intermediate.error;

namespace otava::optimizer {

void ReplaceParamsWithArgs(otava::intermediate::Instruction* callInst, otava::intermediate::ArgInstruction* firstArg, otava::intermediate::Function* callee,
    otava::intermediate::Function* originalCallee, otava::intermediate::IntermediateContext* context)
{
    otava::intermediate::BasicBlock* bb = callee->FirstBasicBlock();
    while (bb)
    {
        otava::intermediate::Instruction* inst = bb->FirstInstruction();
        while (inst)
        {
            otava::intermediate::Instruction* next = inst->Next();
            if (inst->IsParamInstruction())
            {
                if (firstArg)
                {
                    inst->ReplaceUsesWith(firstArg->Arg());
                    otava::intermediate::Instruction* nextArg = firstArg->Next();
                    firstArg->RemoveFromUses();
                    firstArg->Parent()->RemoveInstruction(firstArg);
                    if (nextArg && nextArg->IsArgInstruction())
                    {
                        firstArg = static_cast<otava::intermediate::ArgInstruction*>(nextArg);
                    }
                    else
                    {
                        firstArg = nullptr;
                    }
                }
                else
                {
                    otava::intermediate::Error("number of arguments and parameters do not match", callInst->Span(), context, originalCallee->Span());
                }
                inst->RemoveFromUses();
                inst->Parent()->RemoveInstruction(inst);
            }
            else
            {
                break;
            }
            inst = next;
        }
        bb = bb->Next();
    }
}

void MoveInstructions(otava::intermediate::BasicBlock* fromBasicBlock, otava::intermediate::Instruction* afterCallInst,
    std::vector<otava::intermediate::RetInstruction*>& rets)
{
    otava::intermediate::Instruction* inst = fromBasicBlock->FirstInstruction();
    while (inst)
    {
        otava::intermediate::Instruction* next = inst->Next();
        std::unique_ptr<otava::intermediate::Instruction> instToMove(inst->Parent()->RemoveInstruction(inst));
        if (instToMove->IsRetInstruction())
        {
            rets.push_back(static_cast<otava::intermediate::RetInstruction*>(instToMove.get()));
        }
        afterCallInst->Parent()->InsertInstructionAfter(instToMove.release(), afterCallInst);
        afterCallInst = afterCallInst->Next();
        inst = next;
    }
}

void MergeSingleBasicBlock(otava::intermediate::Function* fn, otava::intermediate::BasicBlock* basicBlock, otava::intermediate::Instruction* callInst,
    otava::intermediate::IntermediateContext* context)
{
    std::vector<otava::intermediate::RetInstruction*> rets;
    MoveInstructions(basicBlock, callInst, rets);
    otava::intermediate::Function* callee = basicBlock->Parent();
    callee->MoveRegValues(fn);
    fn->SetNumbers();
    if (callInst->IsFunctionCallInstruction())
    {
        if (rets.size() == 1)
        {
            otava::intermediate::RetInstruction* ret = rets.front();
            if (ret->ReturnValue())
            {
                callInst->ReplaceUsesWith(ret->ReturnValue());
            }
            else
            {
                otava::intermediate::Error("return value expected", ret->Span(), context);
            }
        }
        else
        {
            otava::intermediate::Error("single ret instruction expected", callee->Span(), context);
        }
    }
    for (otava::intermediate::RetInstruction* ret : rets)
    {
        ret->RemoveFromUses();
        ret->Parent()->RemoveInstruction(ret);
    }
    callInst->RemoveFromUses();
    callInst->Parent()->RemoveInstruction(callInst);
}

void MergeManyBasicBlocks(otava::intermediate::Function* fn, otava::intermediate::Function* callee, otava::intermediate::Instruction* callInst,
    otava::intermediate::IntermediateContext* context)
{
    otava::intermediate::Instruction* lastInst = callInst;
    otava::intermediate::LocalInstruction* localInst = nullptr;
    otava::intermediate::Type* returnType = nullptr;
    otava::intermediate::BasicBlock* targetBB = callInst->Parent()->SplitAfter(callInst);
    if (callInst->IsFunctionCallInstruction())
    {
        returnType = callee->GetType()->ReturnType();
        localInst = static_cast<otava::intermediate::LocalInstruction*>(context->CreateLocal(returnType));
        callInst->Parent()->InsertInstructionAfter(localInst, callInst);
        lastInst = localInst;
    }
    otava::intermediate::BasicBlock* firstMergedBasicBlock = callee->FirstBasicBlock();
    otava::intermediate::BasicBlock* toMerge = firstMergedBasicBlock;
    while (toMerge)
    {
        otava::intermediate::BasicBlock* next = toMerge->Next();
        std::unique_ptr<otava::intermediate::BasicBlock> removedBasicBlock = callee->RemoveBasicBlock(toMerge);
        removedBasicBlock->SetId(fn->GetNextBasicBlockNumber());
        removedBasicBlock->SetContainer(fn->BasicBlocks());
        otava::intermediate::Instruction* last = removedBasicBlock->LastInstruction();
        if (last->IsRetInstruction())
        {
            otava::intermediate::RetInstruction* ret = static_cast<otava::intermediate::RetInstruction*>(last);
            if (callInst->IsFunctionCallInstruction())
            {
                if (ret->ReturnValue())
                {
                    otava::intermediate::StoreInstruction* store = static_cast<otava::intermediate::StoreInstruction*>(
                        context->CreateStore(ret->ReturnValue(), localInst->Result()));
                    store->AddToUses();
                    ret->Parent()->InsertInstructionAfter(store, ret);
                    last = store;
                }
                else
                {
                    otava::intermediate::Error("return value expected", ret->Span(), context);
                }
            }
            otava::intermediate::JmpInstruction* jmp = static_cast<otava::intermediate::JmpInstruction*>(context->CreateJump(targetBB));
            last->Parent()->InsertInstructionAfter(jmp, last);
            ret->RemoveFromUses();
            removedBasicBlock->RemoveInstruction(ret);
        }
        removedBasicBlock->SetContainer(nullptr);
        fn->InsertBasicBlockBefore(removedBasicBlock.release(), targetBB);
        toMerge = next;
    }
    if (callInst->IsFunctionCallInstruction())
    {
        otava::intermediate::LoadInstruction* load = static_cast<otava::intermediate::LoadInstruction*>(context->CreateLoad(localInst->Result()));
        targetBB->InsertFront(load);
        load->AddToUses();
        callInst->ReplaceUsesWith(load->Result());
    }
    otava::intermediate::JmpInstruction* jmp = static_cast<otava::intermediate::JmpInstruction*>(context->CreateJump(firstMergedBasicBlock));
    callInst->Parent()->InsertInstructionAfter(jmp, lastInst);
    callInst->RemoveFromUses();
    callInst->Parent()->RemoveInstruction(callInst);
    callee->MoveRegValues(fn);
}

void MergeBasicBlocks(otava::intermediate::Function* fn, otava::intermediate::Function* callee, otava::intermediate::Instruction* callInst,
    otava::intermediate::IntermediateContext* context)
{
    if (callee->NumBasicBlocks() == 1)
    {
        MergeSingleBasicBlock(fn, callee->FirstBasicBlock(), callInst, context);
    }
    else if (callee->NumBasicBlocks() > 1)
    {
        MergeManyBasicBlocks(fn, callee, callInst, context);
    }
    else
    {
        otava::intermediate::Error("callee has no basic blocks", callee->Span(), context);
    }
}

void Inline(otava::intermediate::Function* fn, otava::intermediate::Instruction* callInst, otava::intermediate::ArgInstruction* firstArg,
    otava::intermediate::Function* callee, otava::intermediate::IntermediateContext* context)
{
    std::unique_ptr<otava::intermediate::Function> clonedCallee(callee->Clone());
    ReplaceParamsWithArgs(callInst, firstArg, clonedCallee.get(), callee, context);
    MergeBasicBlocks(fn, clonedCallee.get(), callInst, context);
    fn->SetNumbers();
}

void Inline(otava::intermediate::Function* fn, otava::intermediate::IntermediateContext* context)
{
    bool fnInlined = false;
    bool inlined = false;
    int inlineDepth = context->InlineDepth();
    int inlineCount = 0;
    do
    {
        inlined = false;
        otava::intermediate::ArgInstruction* firstArg = nullptr;
        otava::intermediate::BasicBlock* bb = fn->FirstBasicBlock();
        while (bb)
        {
            otava::intermediate::Instruction* inst = bb->FirstInstruction();
            while (inst)
            {
                otava::intermediate::Instruction* next = inst->Next();
                if (inst->IsArgInstruction())
                {
                    if (!firstArg)
                    {
                        firstArg = static_cast<otava::intermediate::ArgInstruction*>(inst);
                    }
                }
                otava::intermediate::Function* callee = nullptr;
                bool isCallInst = false;
                if (inst->IsProcedureCallInstruction())
                {
                    otava::intermediate::ProcedureCallInstruction* procedureCall = static_cast<otava::intermediate::ProcedureCallInstruction*>(inst);
                    callee = procedureCall->CalleeFn();
                    isCallInst = true;
                }
                else if (inst->IsFunctionCallInstruction())
                {
                    otava::intermediate::FunctionCallInstruction* functionCall = static_cast<otava::intermediate::FunctionCallInstruction*>(inst);
                    callee = functionCall->CalleeFn();
                    isCallInst = true;
                }
                if (callee)
                {
                    if (callee != fn)
                    {
                        if (callee->IsInline())
                        {
                            Inline(fn, inst, firstArg, callee, context);
                            context->IncInlinedFunctionCalls();
                            fnInlined = true;
                            inlined = true;
                        }
                    }
                }
                if (isCallInst)
                {
                    firstArg = nullptr;
                }
                inst = next;
            }
            bb = bb->Next();
        }
    } while (inlined && ++inlineCount < inlineDepth);
    if (fnInlined)
    {
        context->IncFunctionsInlined();
    }
}

} // namespace otava::optimizer
