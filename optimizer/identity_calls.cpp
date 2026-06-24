// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.optimizer.identity_calls;

import otava.intermediate.error;
import otava.intermediate.types;

namespace otava::optimizer {

bool IsIdentityFunction(otava::intermediate::Function* fn)
{
    otava::intermediate::FunctionType* fnType = fn->GetType();
    if (fnType->IsUnaryOperationType())
    {
        if (fn->NumBasicBlocks() == 1)
        {
            otava::intermediate::BasicBlock* bb = fn->FirstBasicBlock();
            otava::intermediate::Instruction* inst0 = bb->FirstInstruction();
            if (inst0 && inst0->IsParamInstruction())
            {
                otava::intermediate::Instruction* inst1 = inst0->Next();
                if (inst1 && inst1->IsLocalInstruction())
                {
                    otava::intermediate::Instruction* inst2 = inst1->Next();
                    if (inst2 && inst2->IsStoreInstruction())
                    {
                        otava::intermediate::Instruction* inst3 = inst2->Next();
                        if (inst3 && inst3->IsLoadInstruction())
                        {
                            otava::intermediate::Instruction* inst4 = inst3->Next();
                            if (inst4 && inst4->IsRetInstruction())
                            {
                                if (!inst4->Next())
                                {
                                    otava::intermediate::Type* paramType = fnType->ParamType(0);
                                    otava::intermediate::ParamInstruction* paramInst = static_cast<otava::intermediate::ParamInstruction*>(inst0);
                                    if (paramInst->Result()->GetType() == paramType)
                                    {
                                        otava::intermediate::LocalInstruction* localInst = static_cast<otava::intermediate::LocalInstruction*>(inst1);
                                        if (localInst->LocalType() == paramType)
                                        {
                                            otava::intermediate::StoreInstruction* storeInst = static_cast<otava::intermediate::StoreInstruction*>(inst2);
                                            if (storeInst->GetValue()->GetInstruction() == paramInst && storeInst->GetPtr()->GetInstruction() == localInst)
                                            {
                                                otava::intermediate::LoadInstruction* loadInst = static_cast<otava::intermediate::LoadInstruction*>(inst3);
                                                if (loadInst->Ptr()->GetInstruction() == localInst)
                                                {
                                                    otava::intermediate::Value* loadedValue = loadInst->Result();
                                                    otava::intermediate::RetInstruction* retInst = static_cast<otava::intermediate::RetInstruction*>(inst4);
                                                    if (retInst->ReturnValue() == loadedValue)
                                                    {
                                                        return true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

void RemoveIdentityFunctionCall(otava::intermediate::FunctionCallInstruction* identityFnCallInst, otava::intermediate::IntermediateContext* context)
{
    otava::intermediate::Instruction* prev = identityFnCallInst->Prev();
    if (!prev || !prev->IsArgInstruction())
    {
        otava::intermediate::Error("arg prev instruction expected", identityFnCallInst->Span(), context);
        return;
    }
    otava::intermediate::ArgInstruction* argInst = static_cast<otava::intermediate::ArgInstruction*>(prev);
    identityFnCallInst->ReplaceUsesWith(argInst->Arg());
    otava::intermediate::Instruction* inst = argInst->Arg()->GetInstruction();
    if (inst)
    {
        inst->RemoveUser(argInst);
        inst->RemoveUser(identityFnCallInst);
    }
    argInst->Parent()->RemoveInstruction(argInst);
    identityFnCallInst->Parent()->RemoveInstruction(identityFnCallInst);
}

void OptimizeIdentityCalls(otava::intermediate::Function* fn, otava::intermediate::IntermediateContext* context)
{
    bool optimized = false;
    otava::intermediate::BasicBlock* bb = fn->FirstBasicBlock();
    while (bb)
    {
        otava::intermediate::Instruction* inst = bb->FirstInstruction();
        while (inst)
        {
            otava::intermediate::Instruction* next = inst->Next();
            if (inst->IsFunctionCallInstruction())
            {
                otava::intermediate::FunctionCallInstruction* fnCall = static_cast<otava::intermediate::FunctionCallInstruction*>(inst);
                otava::intermediate::Function* callee = fnCall->CalleeFn();
                if (callee && IsIdentityFunction(callee))
                {
                    RemoveIdentityFunctionCall(fnCall, context);
                    optimized = true;
                }
            }
            inst = next;
        }
        bb = bb->Next();
    }
    if (optimized)
    {
        fn->SetNumbers();
    }
}

} // otava::optimizer
