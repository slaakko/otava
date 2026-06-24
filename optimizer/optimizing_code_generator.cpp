// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.optimizer.optimizing_code_generator;

import otava.optimizer;
import otava.optimizer.jump_table_switch;
import otava.intermediate.code;
import otava.intermediate.context;
import otava.assembly.literal;
import otava.assembly.instruction;
import otava.assembly.symbol;
import otava.assembly.value;

namespace otava::optimizer {

OptimizingCodeGenerator::OptimizingCodeGenerator(otava::intermediate::IntermediateContext* context_, const std::string& assemblyFilePath_) :
    otava::intermediate::CodeGenerator(context_, assemblyFilePath_)
{
}

void OptimizingCodeGenerator::Emit(otava::assembly::Instruction* assemblyInstruction)
{
    std::unique_ptr<otava::assembly::Instruction> inst(assemblyInstruction);
    switch (inst->GetOpCode())
    {
    case otava::assembly::OpCode::MOV:
    {
        if (inst->Operands().size() == 2)
        {
            otava::assembly::Value* operand1 = inst->Operands()[0];
            otava::assembly::Value* operand2 = inst->Operands()[1];
            if (operand2->IsIntegerLiteral())
            {
                otava::assembly::IntegerLiteral* integerLiteral = static_cast<otava::assembly::IntegerLiteral*>(operand2);
                if (integerLiteral->GetValue() == 0)
                {
                    if (operand1->IsRegister())
                    {
                        otava::assembly::Register* reg = static_cast<otava::assembly::Register*>(operand1);
                        switch (reg->RegKind())
                        {
                        case otava::assembly::RegisterKind::al:
                        case otava::assembly::RegisterKind::bl:
                        case otava::assembly::RegisterKind::cl:
                        case otava::assembly::RegisterKind::dl:
                        case otava::assembly::RegisterKind::sil:
                        case otava::assembly::RegisterKind::dil:
                        case otava::assembly::RegisterKind::bpl:
                        case otava::assembly::RegisterKind::spl:
                        case otava::assembly::RegisterKind::r8b:
                        case otava::assembly::RegisterKind::r9b:
                        case otava::assembly::RegisterKind::r10b:
                        case otava::assembly::RegisterKind::r11b:
                        case otava::assembly::RegisterKind::r12b:
                        case otava::assembly::RegisterKind::r13b:
                        case otava::assembly::RegisterKind::r14b:
                        case otava::assembly::RegisterKind::r15b:
                        case otava::assembly::RegisterKind::ax:
                        case otava::assembly::RegisterKind::bx:
                        case otava::assembly::RegisterKind::cx:
                        case otava::assembly::RegisterKind::dx:
                        case otava::assembly::RegisterKind::si:
                        case otava::assembly::RegisterKind::di:
                        case otava::assembly::RegisterKind::bp:
                        case otava::assembly::RegisterKind::sp:
                        case otava::assembly::RegisterKind::r8w:
                        case otava::assembly::RegisterKind::r9w:
                        case otava::assembly::RegisterKind::r10w:
                        case otava::assembly::RegisterKind::r11w:
                        case otava::assembly::RegisterKind::r12w:
                        case otava::assembly::RegisterKind::r13w:
                        case otava::assembly::RegisterKind::r14w:
                        case otava::assembly::RegisterKind::r15w:
                        case otava::assembly::RegisterKind::eax:
                        case otava::assembly::RegisterKind::ebx:
                        case otava::assembly::RegisterKind::ecx:
                        case otava::assembly::RegisterKind::edx:
                        case otava::assembly::RegisterKind::esi:
                        case otava::assembly::RegisterKind::edi:
                        case otava::assembly::RegisterKind::ebp:
                        case otava::assembly::RegisterKind::esp:
                        case otava::assembly::RegisterKind::r8d:
                        case otava::assembly::RegisterKind::r9d:
                        case otava::assembly::RegisterKind::r10d:
                        case otava::assembly::RegisterKind::r11d:
                        case otava::assembly::RegisterKind::r12d:
                        case otava::assembly::RegisterKind::r13d:
                        case otava::assembly::RegisterKind::r14d:
                        case otava::assembly::RegisterKind::r15d:
                        case otava::assembly::RegisterKind::rax:
                        case otava::assembly::RegisterKind::rbx:
                        case otava::assembly::RegisterKind::rcx:
                        case otava::assembly::RegisterKind::rdx:
                        case otava::assembly::RegisterKind::rsi:
                        case otava::assembly::RegisterKind::rdi:
                        case otava::assembly::RegisterKind::rbp:
                        case otava::assembly::RegisterKind::rsp:
                        case otava::assembly::RegisterKind::r8:
                        case otava::assembly::RegisterKind::r9:
                        case otava::assembly::RegisterKind::r10:
                        case otava::assembly::RegisterKind::r11:
                        case otava::assembly::RegisterKind::r12:
                        case otava::assembly::RegisterKind::r13:
                        case otava::assembly::RegisterKind::r14:
                        case otava::assembly::RegisterKind::r15:
                        case otava::assembly::RegisterKind::ah:
                        case otava::assembly::RegisterKind::bh:
                        case otava::assembly::RegisterKind::ch:
                        case otava::assembly::RegisterKind::dh:
                        {
                            EmitXorInst(inst->Label(), reg);
                            return;
                        }
                        }
                    }
                }
            }
        }
        break;
    }
    }
    otava::intermediate::CodeGenerator::Emit(inst.release());
}

void OptimizingCodeGenerator::Visit(otava::intermediate::SwitchInstruction& inst)
{
    if (HasOptimization(Optimizations::jumpTableSwitch))
    {
        if (inst.CaseTargets().size() <= jumpTableSwitchThreshold)
        {
            otava::intermediate::EmitSwitch(inst, *this);
        }
        else
        {
            EmitJumpTableSwitch(inst, *this);
        }
    }
    else
    {
        otava::intermediate::EmitSwitch(inst, *this);
    }
}

void OptimizingCodeGenerator::Visit(otava::intermediate::JmpInstruction& inst)
{
    if (HasOptimization(Optimizations::jump))
    {
        otava::intermediate::BasicBlock* bb = inst.Parent();
        otava::intermediate::BasicBlock* next = bb->Next();
        if (next && !bb->ContainsOnlyNops())
        {
            if (inst.TargetBasicBlock() == next)
            {
                return;
            }
        }
    }
    CodeGenerator::Visit(inst);
}

void OptimizingCodeGenerator::Visit(otava::intermediate::RetInstruction& inst)
{
    if (HasOptimization(Optimizations::jump))
    {
        otava::intermediate::BasicBlock* bb = inst.Parent();
        if (bb->IsLast() && inst.IsLeader() && inst.ReturnValue() == nullptr)
        {
            ResetLeader();
            return;
        }
    }
    CodeGenerator::Visit(inst);
}

void OptimizingCodeGenerator::Visit(otava::intermediate::NoOperationInstruction& inst)
{
    if (HasOptimization(Optimizations::removeNops))
    {
        return;
    }
    CodeGenerator::Visit(inst);
}

int OptimizingCodeGenerator::ExitLabelId() const noexcept
{
    if (HasOptimization(Optimizations::jump))
    {
        otava::intermediate::BasicBlock* lastBB = CurrentFunction()->LastBasicBlock();
        if (lastBB->Leader()->IsRetVoid())
        {
            return lastBB->Id();
        }
    }
    return CodeGenerator::ExitLabelId();
}

void OptimizingCodeGenerator::EmitJumpToExit(otava::intermediate::RetInstruction& retInst)
{
    if (HasOptimization(Optimizations::jump))
    {
        otava::intermediate::BasicBlock* bb = retInst.Parent();
        if (bb->IsLast())
        {
            return;
        }
    }
    CodeGenerator::EmitJumpToExit(retInst);
}

void OptimizingCodeGenerator::EmitBranchJumps(otava::intermediate::BranchInstruction& branchInst)
{
    if (HasOptimization(Optimizations::jump))
    {
        otava::intermediate::BasicBlock* bb = branchInst.Parent();
        if (branchInst.TrueTargetBasicBlock() == bb->Next())
        {
            otava::assembly::Instruction* jzInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::JZ);
            jzInstruction->AddOperand(Ctx()->GetAssemblyContext()->MakeSymbol("@" + std::to_string(branchInst.FalseTargetBasicBlock()->Id())));
            Emit(jzInstruction);
            return;
        }
    }
    CodeGenerator::EmitBranchJumps(branchInst);
}

void OptimizingCodeGenerator::EmitXorInst(const std::string& label, otava::assembly::Register* reg)
{
    otava::assembly::Instruction* xorInst = new otava::assembly::Instruction(otava::assembly::OpCode::XOR);
    xorInst->SetLabel(label);
    xorInst->AddOperand(reg);
    xorInst->AddOperand(reg);
    Emit(xorInst);
}

} // otava::optimizer
