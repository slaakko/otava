// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.optimizer.jump_table_switch;

import otava.optimizer.optimizing_code_generator;
import otava.intermediate.context;
import otava.assembly.declaration;
import otava.assembly.data;
import otava.assembly.literal;
import otava.assembly.symbol;
import otava.assembly.value;

namespace otava::optimizer {

struct CaseLess
{
    bool operator()(const otava::intermediate::CaseTarget& left, otava::intermediate::CaseTarget& right) const noexcept
    {
        return left.caseValue->GetIntegerValue() < right.caseValue->GetIntegerValue();
    }
};

void EmitJumpTableSwitch(otava::intermediate::SwitchInstruction& inst, otava::intermediate::CodeGenerator& codeGenerator)
{
    for (const otava::intermediate::CaseTarget& caseTarget : inst.CaseTargets())
    {
        if (!caseTarget.caseValue->IsIntegerValue())
        {
            otava::intermediate::EmitSwitch(inst, codeGenerator);
            return;
        }
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    int size = static_cast<int>(inst.Cond()->GetType()->Size());
    std::vector<otava::intermediate::CaseTarget> caseTargets = inst.CaseTargets();
    std::sort(caseTargets.begin(), caseTargets.end(), CaseLess());
    otava::assembly::Register* condReg = otava::intermediate::MakeIntegerRegOperand(
        inst.Cond(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rcx), codeGenerator);
    std::int64_t low = caseTargets.front().caseValue->GetIntegerValue();
    std::int64_t count = caseTargets.back().caseValue->GetIntegerValue() - low + 1;
    if (count > maxJumpTableEntries)
    {
        otava::intermediate::EmitSwitch(inst, codeGenerator);
        return;
    }
    if (low != 0)
    {
        otava::assembly::Instruction* subInst = new otava::assembly::Instruction(otava::assembly::OpCode::SUB);
        subInst->AddOperand(condReg);
        if (low > 0xFFFFFFFF)
        {
            otava::assembly::Instruction* movRaxInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
            otava::assembly::Register* rax = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rax);
            movRaxInst->AddOperand(rax);
            movRaxInst->AddOperand(assemblyContext->MakeIntegerLiteral(low, 8));
            codeGenerator.Emit(movRaxInst);
            subInst->AddOperand(rax);
        }
        else
        {
            subInst->AddOperand(assemblyContext->MakeIntegerLiteral(low, size));
        }
        codeGenerator.Emit(subInst);
    }
    std::map<std::int64_t, std::int32_t> caseMap;
    for (const otava::intermediate::CaseTarget& caseTarget : inst.CaseTargets())
    {
        std::int64_t value = caseTarget.caseValue->GetIntegerValue() - low;
        std::int32_t targetLabelId = caseTarget.targetLabelId;
        caseMap[value] = targetLabelId;
    }
    otava::assembly::Instruction* cmpInst = new otava::assembly::Instruction(otava::assembly::OpCode::CMP);
    cmpInst->AddOperand(condReg);
    cmpInst->AddOperand(assemblyContext->MakeIntegerLiteral(count, size));
    codeGenerator.Emit(cmpInst);
    otava::assembly::Instruction* jaeIst = new otava::assembly::Instruction(otava::assembly::OpCode::JAE);
    jaeIst->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(inst.DefaultTargetBlock()->Id())));
    codeGenerator.Emit(jaeIst);
    otava::assembly::Register* condReg8 = assemblyContext->GetRegisterPool()->GetRegisterGroup(condReg->Group(), true)->GetReg(8);
    if (condReg->Size() == 4)
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(condReg);
        movInst->AddOperand(condReg);
        codeGenerator.Emit(movInst);
    }
    else if (condReg->Size() != 8)
    {
        otava::assembly::Instruction* movzxInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOVZX);
        movzxInst->AddOperand(condReg8);
        movzxInst->AddOperand(condReg);
        codeGenerator.Emit(movzxInst);
    }
    std::string jumpTabLabel = "@jmptab" + std::to_string(assemblyContext->GetNextJumpTabLabelId());
    otava::assembly::Instruction* setJumpTabAddrInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
    otava::assembly::Register* jumpTabAddrReg = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbx);
    setJumpTabAddrInst->AddOperand(jumpTabAddrReg);
    setJumpTabAddrInst->AddOperand(assemblyContext->MakeSymbol(jumpTabLabel));
    codeGenerator.Emit(setJumpTabAddrInst);
    otava::assembly::Instruction* movSxdInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOVSXD);
    otava::assembly::Register* jumpReg = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rax);
    movSxdInst->AddOperand(jumpReg);
    movSxdInst->AddOperand(assemblyContext->MakeSizePrefix(4, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(jumpTabAddrReg,
        assemblyContext->MakeBinaryExpr(condReg8, assemblyContext->MakeIntegerLiteral(4, 4), otava::assembly::Operator::mul),
        otava::assembly::Operator::add))));
    codeGenerator.Emit(movSxdInst);
    otava::assembly::Instruction* addInst = new otava::assembly::Instruction(otava::assembly::OpCode::ADD);
    addInst->AddOperand(jumpReg);
    addInst->AddOperand(jumpTabAddrReg);
    codeGenerator.Emit(addInst);
    otava::assembly::Instruction* jmpInst = new otava::assembly::Instruction(otava::assembly::OpCode::JMP);
    jmpInst->AddOperand(jumpReg);
    codeGenerator.Emit(jmpInst);
    for (std::int64_t i = 0; i < count; ++i)
    {
        otava::assembly::Instruction* offsetInst = new otava::assembly::Instruction(otava::assembly::OpCode::DD);
        if (i == 0)
        {
            offsetInst->SetLabel(jumpTabLabel);
            offsetInst->SetWriteln();
        }
        std::int32_t targetLabelId = inst.DefaultTargetLabelId();
        auto it = caseMap.find(i);
        if (it != caseMap.end())
        {
            targetLabelId = it->second;
        }
        offsetInst->AddOperand(assemblyContext->MakeBinaryExpr(assemblyContext->MakeSymbol("@" + std::to_string(targetLabelId)),
            assemblyContext->MakeSymbol(jumpTabLabel), otava::assembly::Operator::sub));
        codeGenerator.Emit(offsetInst);
    }
}

} // otava::optimizer
