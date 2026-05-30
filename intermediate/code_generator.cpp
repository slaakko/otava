// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.code_generator;

import otava.intermediate.code;
import otava.intermediate.data;
import otava.intermediate.types;
import otava.intermediate.context;
import otava.intermediate.location;
import otava.intermediate.register_allocator;
import otava.intermediate.linear_scan_register_allocator;
import otava.intermediate.error;
import otava.assembly.context;
import otava.assembly.data;
import otava.assembly.instruction;
import otava.assembly.literal;
import otava.assembly.symbol;
import util;

namespace otava::intermediate {

void EmitIntegerLoad(std::int64_t size, const FrameLocation& frameLocation, otava::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator);
void EmitFloatingPointLoad(std::int64_t size, const FrameLocation& frameLocation, otava::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator);
otava::assembly::Register* MakeFloatingPointRegOperand(Value* value, otava::assembly::Register* reg, CodeGenerator& codeGenerator);
FrameLocation GetFrameLocation(Value* value, CodeGenerator& codeGenerator);
void EmitFrameLocationOperand(std::int64_t size, const FrameLocation& frameLocation, otava::assembly::Instruction* instruction, CodeGenerator& codeGenerator);

void EmitPrologue(CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();

    int numPushes = 0;
    otava::assembly::Instruction* pushRbp = new otava::assembly::Instruction(otava::assembly::OpCode::PUSH);
    pushRbp->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbp));
    codeGenerator.Emit(pushRbp);
    otava::assembly::Instruction* pushRegMacroRbp = new otava::assembly::Instruction(otava::assembly::OpCode::PUSHREG);
    pushRegMacroRbp->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbp));
    codeGenerator.Emit(pushRegMacroRbp);
    ++numPushes;

    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedLocalRegs())
    {
        otava::assembly::Instruction* pushReg = new otava::assembly::Instruction(otava::assembly::OpCode::PUSH);
        pushReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(pushReg);
        if (regGroup->IsVolatile())
        {
            otava::assembly::Instruction* allocStackMacro = new otava::assembly::Instruction(otava::assembly::OpCode::ALLOCSTACK);
            allocStackMacro->AddOperand(assemblyContext->MakeIntegerLiteral(8, 8));
            codeGenerator.Emit(allocStackMacro);
        }
        else
        {
            otava::assembly::Instruction* pushRegMacro = new otava::assembly::Instruction(otava::assembly::OpCode::PUSHREG);
            pushRegMacro->AddOperand(regGroup->GetReg(8));
            codeGenerator.Emit(pushRegMacro);
        }
        ++numPushes;
    }
    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedNonvolatileRegs())
    {
        otava::assembly::Instruction* pushReg = new otava::assembly::Instruction(otava::assembly::OpCode::PUSH);
        pushReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(pushReg);
        otava::assembly::Instruction* pushRegMacro = new otava::assembly::Instruction(otava::assembly::OpCode::PUSHREG);
        pushRegMacro->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(pushRegMacro);
        ++numPushes;
    }

    Frame& frame = codeGenerator.RegAllocator()->GetFrame();

    if ((numPushes % 2) == 0)
    {
        otava::assembly::Instruction* pushReg = new otava::assembly::Instruction(otava::assembly::OpCode::PUSH);
        pushReg->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbx, false));
        codeGenerator.Emit(pushReg);
        otava::assembly::Instruction* pushRegMacro = new otava::assembly::Instruction(otava::assembly::OpCode::PUSHREG);
        pushRegMacro->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbx, false));
        codeGenerator.Emit(pushRegMacro);
        frame.SetRbxPushed();
    }

    int nxmmregs = assemblyContext->GetRegisterPool()->UsedLocalXMMRegs().size() + assemblyContext->GetRegisterPool()->UsedNonvolatileXMMRegs().size();
    frame.SetNumUsedXMMRegs(nxmmregs);
    frame.SetMacroValues(assemblyContext);

    if (frame.Size() > 4096)
    {
        assemblyContext->GetFile()->GetDeclarationSection().AddFunctionDeclaration(new otava::assembly::FunctionDeclaration("__chkstk"));
        otava::assembly::Instruction* movEax = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movEax->AddOperand(assemblyContext->GetGlobalReg(4, otava::assembly::RegisterGroupKind::rax));
        movEax->AddOperand(assemblyContext->MakeIntegerLiteral(frame.Size(), 8));
        codeGenerator.Emit(movEax);
        otava::assembly::Value* chkstk = assemblyContext->MakeSymbol("__chkstk");
        otava::assembly::Instruction* checkStackCall = new otava::assembly::Instruction(otava::assembly::OpCode::CALL);
        checkStackCall->AddOperand(chkstk);
        codeGenerator.Emit(checkStackCall);
    }

    otava::assembly::Instruction* subRsp = new otava::assembly::Instruction(otava::assembly::OpCode::SUB);
    subRsp->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rsp));
    subRsp->AddOperand(assemblyContext->MakeIntegerLiteral(frame.Size(), 8));
    codeGenerator.Emit(subRsp);
    otava::assembly::Instruction* allocStackMacro = new otava::assembly::Instruction(otava::assembly::OpCode::ALLOCSTACK);
    allocStackMacro->AddOperand(assemblyContext->MakeIntegerLiteral(frame.Size(), 8));
    codeGenerator.Emit(allocStackMacro);

    if (nxmmregs > 0)
    {
        int xmmRegIndex = 0;
        for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedLocalXMMRegs())
        {
            otava::assembly::Instruction* movReg = new otava::assembly::Instruction(otava::assembly::OpCode::MOVSD);
            movReg->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(
                assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rsp),
                assemblyContext->MakeIntegerLiteral(xmmRegIndex * 16, 8), otava::assembly::Operator::add))));
            movReg->AddOperand(regGroup->GetReg(16));
            codeGenerator.Emit(movReg);
            ++xmmRegIndex;
        }
        for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedNonvolatileXMMRegs())
        {
            otava::assembly::Instruction* movReg = new otava::assembly::Instruction(otava::assembly::OpCode::MOVSD);
            movReg->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(
                assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rsp),
                assemblyContext->MakeIntegerLiteral(xmmRegIndex * 16, 8), otava::assembly::Operator::add))));
            movReg->AddOperand(regGroup->GetReg(16));
            codeGenerator.Emit(movReg);
            ++xmmRegIndex;
        }
    }

    otava::assembly::Instruction* leaRbp = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
    leaRbp->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbp));
    leaRbp->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
        assemblyContext->MakeBinaryExpr(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rsp),
            assemblyContext->MakeIntegerLiteral(frame.CalleeParamAreaSize() + frame.XMMSaveRegSize(), 8), otava::assembly::Operator::add))));
    codeGenerator.Emit(leaRbp);
    otava::assembly::Instruction* setFrameMacro = new otava::assembly::Instruction(otava::assembly::OpCode::SETFRAME);
    setFrameMacro->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbp));
    setFrameMacro->AddOperand(assemblyContext->MakeIntegerLiteral(frame.CalleeParamAreaSize() + frame.XMMSaveRegSize(), 8));
    codeGenerator.Emit(setFrameMacro);

    otava::assembly::Instruction* endPrologueMacro = new otava::assembly::Instruction(otava::assembly::OpCode::ENDPROLOG);
    codeGenerator.Emit(endPrologueMacro);
}

void EmitEpilogue(CodeGenerator& codeGenerator)
{
    otava::assembly::Instruction* leaderInst = nullptr;
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();

    Frame& frame = codeGenerator.RegAllocator()->GetFrame();

    int nxmmregs = frame.GetNumUsedXMMRegs();

    if (nxmmregs > 0)
    {
        int xmmRegIndex = nxmmregs - 1;

        std::vector<otava::assembly::RegisterGroup*> usedNVXMMRegs;
        for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedNonvolatileXMMRegs())
        {
            usedNVXMMRegs.push_back(regGroup);
        }
        std::reverse(usedNVXMMRegs.begin(), usedNVXMMRegs.end());
        for (const auto& regGroup : usedNVXMMRegs)
        {
            otava::assembly::Instruction* movReg = new otava::assembly::Instruction(otava::assembly::OpCode::MOVSD);
            if (!leaderInst)
            {
                leaderInst = movReg;
                leaderInst->SetLabel("@" + std::to_string(codeGenerator.ExitLabelId()));
            }
            movReg->AddOperand(regGroup->GetReg(16));
            movReg->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(
                assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rsp),
                assemblyContext->MakeIntegerLiteral(xmmRegIndex * 16, 8), otava::assembly::Operator::add))));
            codeGenerator.Emit(movReg);
            --xmmRegIndex;
        }

        std::vector<otava::assembly::RegisterGroup*> usedLocalXMMRegs;
        for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedLocalXMMRegs())
        {
            usedLocalXMMRegs.push_back(regGroup);
        }
        std::reverse(usedLocalXMMRegs.begin(), usedLocalXMMRegs.end());
        for (const auto& regGroup : usedLocalXMMRegs)
        {
            otava::assembly::Instruction* movReg = new otava::assembly::Instruction(otava::assembly::OpCode::MOVSD);
            if (!leaderInst)
            {
                leaderInst = movReg;
                leaderInst->SetLabel("@" + std::to_string(codeGenerator.ExitLabelId()));
            }
            movReg->AddOperand(regGroup->GetReg(16));
            movReg->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(
                assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rsp),
                assemblyContext->MakeIntegerLiteral(xmmRegIndex * 16, 8), otava::assembly::Operator::add))));
            codeGenerator.Emit(movReg);
            --xmmRegIndex;
        }
    }

    otava::assembly::Instruction* leaRsp = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
    if (!leaderInst)
    {
        leaderInst = leaRsp;
        leaderInst->SetLabel("@" + std::to_string(codeGenerator.ExitLabelId()));
    }
    leaRsp->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rsp));
    leaRsp->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
        assemblyContext->MakeBinaryExpr(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbp),
            assemblyContext->MakeIntegerLiteral(frame.Size() - frame.CalleeParamAreaSize() - frame.XMMSaveRegSize(), 8), otava::assembly::Operator::add))));
    codeGenerator.Emit(leaRsp);

    if (frame.RbxPushed())
    {
        otava::assembly::Instruction* popReg = new otava::assembly::Instruction(otava::assembly::OpCode::POP);
        popReg->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbx, false));
        codeGenerator.Emit(popReg);
    }

    std::vector<otava::assembly::RegisterGroup*> usedNVRegs;
    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedNonvolatileRegs())
    {
        usedNVRegs.push_back(regGroup);
    }
    std::reverse(usedNVRegs.begin(), usedNVRegs.end());
    for (const auto& regGroup : usedNVRegs)
    {
        otava::assembly::Instruction* popReg = new otava::assembly::Instruction(otava::assembly::OpCode::POP);
        popReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(popReg);
    }

    std::vector<otava::assembly::RegisterGroup*> usedLocalRegs;
    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedLocalRegs())
    {
        usedLocalRegs.push_back(regGroup);
    }
    std::reverse(usedLocalRegs.begin(), usedLocalRegs.end());
    for (const auto& regGroup : usedLocalRegs)
    {
        otava::assembly::Instruction* popReg = new otava::assembly::Instruction(otava::assembly::OpCode::POP);
        popReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(popReg);
    }

    otava::assembly::Instruction* popRbp = new otava::assembly::Instruction(otava::assembly::OpCode::POP);
    popRbp->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbp));
    codeGenerator.Emit(popRbp);

    otava::assembly::Instruction* ret = new otava::assembly::Instruction(otava::assembly::OpCode::RET);
    ret->AddOperand(assemblyContext->MakeIntegerLiteral(0, 1));
    codeGenerator.Emit(ret);
}

FrameLocation GetFrameLocation(Value* value, CodeGenerator& codeGenerator)
{
    Instruction* inst = nullptr;
    if (value->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        inst = regValue->Inst();
        if (!inst)
        {
            codeGenerator.Error("error getting frame location: instruction for reg value not set");
        }
    }
    else if (value->IsInstruction())
    {
        inst = static_cast<Instruction*>(value);
    }
    if (inst)
    {
        Locations locs = codeGenerator.RegAllocator()->GetLocations(inst);
        if ((locs & Locations::frame) != Locations::none)
        {
            FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(inst);
            if (frameLocation.Valid())
            {
                return frameLocation;
            }
            else
            {
                codeGenerator.Error("error getting frame location: frame location not valid");
            }
        }
        else
        {
            codeGenerator.Error("error getting frame location: value not in frame");
        }
    }
    else
    {
        codeGenerator.Error("error getting frame location: invalid value kind");
    }
    return FrameLocation();
}

void EmitParentFrameAccess(otava::assembly::Register* plocalFrameReg, int level, CodeGenerator& codeGenerator)
{
    otava::assembly::Instruction* instruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
    instruction->AddOperand(plocalFrameReg);
    FrameLocation firstFrameLocation = codeGenerator.RegAllocator()->GetFrame().GetFrameLocation(0, codeGenerator.Span(), codeGenerator.Ctx());
    EmitFrameLocationOperand(8, firstFrameLocation, instruction, codeGenerator);
    codeGenerator.Emit(instruction);
    while (level > 0)
    {
        otava::assembly::Instruction* instruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        instruction->AddOperand(plocalFrameReg);
        FrameLocation nextFrameLocation = codeGenerator.RegAllocator()->GetFrame().GetParentFrameLocation(codeGenerator.Span(), codeGenerator.Ctx());
        EmitFrameLocationOperand(8, nextFrameLocation, instruction, codeGenerator);
        codeGenerator.Emit(instruction);
        --level;
    }
}

void EmitFrameLocationOperand(std::int64_t size, const FrameLocation& frameLocation, otava::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = assemblyContext->GetRegisterPool()->GetRegisterGroup(frameLocation.reg, true);
    otava::assembly::Register* reg = regGroup->GetReg(8);
    if (frameLocation.IsParentFrameLocation())
    {
        EmitParentFrameAccess(reg, frameLocation.level, codeGenerator);
    }
    otava::assembly::Value* frameLoc = assemblyContext->MakeIntegerLiteral(frameLocation.offset, 8);
    if (frameLocation.macro)
    {
        frameLoc = assemblyContext->MakeSymbol(frameLocation.macro->Name());
    }
    instruction->AddOperand(assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(
        assemblyContext->MakeBinaryExpr(reg, frameLoc, otava::assembly::Operator::add))));
}

void EmitIntegerPtrOperand(std::int64_t size, Value* value, otava::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    Instruction* inst = nullptr;
    if (value->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        inst = regValue->Inst();
        if (!inst)
        {
            codeGenerator.Error("error emitting ptr operand: instruction for reg value not set");
        }
    }
    else if (value->IsInstruction())
    {
        inst = static_cast<Instruction*>(value);
    }
    else if (value->IsAddressValue())
    {
        otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
        AddressValue* addressValue = static_cast<AddressValue*>(value);
        GlobalVariable* globalVar = addressValue->GetValue();
        otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
        otava::assembly::Register* rdx = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rdx);
        leaInst->AddOperand(rdx);
        leaInst->AddOperand(assemblyContext->MakeSymbol(globalVar->Name()));
        codeGenerator.Emit(leaInst);
        instruction->AddOperand(assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(rdx)));
        return;
    }
    if (inst)
    {
        if (inst->IsLocalInstruction())
        {
            FrameLocation frameLocation = GetFrameLocation(value, codeGenerator);
            EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
        }
        else if (inst->IsPLocalInstruction())
        {
            FrameLocation frameLocation = GetFrameLocation(value, codeGenerator);
            EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
        }
        else
        {
            otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
            otava::assembly::Register* rax = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rax);
            otava::assembly::Register* reg = MakeIntegerRegOperand(value, rax, codeGenerator);
            otava::assembly::Value* operand = assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(reg));
            instruction->AddOperand(operand);
        }
    }
    else
    {
        codeGenerator.Error("error emitting ptr operand: invalid value kind");
    }
}

void EmitFloatingPointPtrOperand(std::int64_t size, Value* value, otava::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    Instruction* inst = nullptr;
    if (value->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        inst = regValue->Inst();
        if (!inst)
        {
            codeGenerator.Error("error emitting ptr operand: instruction for reg value not set");
        }
    }
    else if (value->IsInstruction())
    {
        inst = static_cast<Instruction*>(value);
    }
    else if (value->IsAddressValue())
    {
        otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
        AddressValue* addressValue = static_cast<AddressValue*>(value);
        GlobalVariable* globalVar = addressValue->GetValue();
        otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
        leaInst->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rdx));
        leaInst->AddOperand(assemblyContext->MakeSymbol(globalVar->Name()));
        codeGenerator.Emit(leaInst);
        instruction->AddOperand(assemblyContext->MakeSizePrefix(
            size, assemblyContext->MakeContent(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::xmm1))));
        return;
    }
    if (inst)
    {
        if (inst->IsLocalInstruction())
        {
            FrameLocation frameLocation = GetFrameLocation(value, codeGenerator);
            EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
        }
        else if (inst->IsPLocalInstruction())
        {
            FrameLocation frameLocation = GetFrameLocation(value, codeGenerator);
            EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
        }
        else
        {
            otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
            otava::assembly::Value* operand = assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(
                MakeFloatingPointRegOperand(value, assemblyContext->GetGlobalReg(16, otava::assembly::RegisterGroupKind::xmm0), codeGenerator)));
            instruction->AddOperand(operand);
        }
    }
}

std::int64_t GetIndex(Value* index, CodeGenerator& codeGen)
{
    if (index->IsIntegerValue())
    {
        return index->GetIntegerValue();
    }
    else
    {
        codeGen.Error("error getting index: invalid index value kind");
    }
    return 0;
}

std::int64_t GetOffset(Type* type, std::int64_t index, CodeGenerator& codeGenerator)
{
    StructureType* structureType = type->GetStructurePointeeType(codeGenerator.Span(), codeGenerator.Ctx());
    return structureType->GetFieldOffset(index);
}

std::int64_t GetElementSize(Type* type, CodeGenerator& codeGenerator)
{
    ArrayType* arrayType = type->GetArrayPointeeType(codeGenerator.Span(), codeGenerator.Ctx());
    return arrayType->ElementType()->Size();
}

std::int64_t GetPointeeSize(Type* type, CodeGenerator& codeGenerator)
{
    if (type->IsPointerType())
    {
        PointerType* pointerType = static_cast<PointerType*>(type);
        Type* pointeeType = pointerType->RemovePointer(codeGenerator.Span(), codeGenerator.Ctx());
        return pointeeType->Size();
    }
    else
    {
        codeGenerator.Error("error getting pointee size: pointer type expected");
    }
    return -1;
}

otava::assembly::Register* MakeIntegerRegOperand(Value* value, otava::assembly::Register* reg, CodeGenerator& codeGenerator)
{
    std::int64_t size = std::min(value->GetType()->Size(), std::int64_t(reg->Size()));
    if (value->Kind() == ValueKind::regValue)
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        Instruction* inst = regValue->Inst();
        if (inst)
        {
            if (inst->IsLocalInstruction())
            {
                otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
                leaInst->AddOperand(reg);
                EmitIntegerPtrOperand(size, value, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                return reg;
            }
            else if (inst->IsPLocalInstruction())
            {
                otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
                leaInst->AddOperand(reg);
                EmitIntegerPtrOperand(size, value, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                return reg;
            }
            Locations locs = codeGenerator.RegAllocator()->GetLocations(inst);
            if ((locs & Locations::reg) != Locations::none)
            {
                otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(inst);
                if (regGroup)
                {
                    return regGroup->GetReg(size);
                }
                else
                {
                    codeGenerator.Error("error making reg operand: reg group not valid");
                }
            }
            else if ((locs & Locations::frame) != Locations::none)
            {
                FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(inst);
                if (frameLocation.Valid())
                {
                    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
                    otava::assembly::RegisterGroup* regGroup = assemblyContext->GetRegisterPool()->GetRegisterGroup(reg->Group(), true);
                    EmitIntegerLoad(size, frameLocation, regGroup, codeGenerator);
                }
                else
                {
                    codeGenerator.Error("error making reg operand: frame location not valid");
                }
            }
            else
            {
                codeGenerator.Error("error making reg operand: no locations for inst");
            }
        }
        else
        {
            codeGenerator.Error("error making reg operand: instruction for reg value not set");
        }
    }
    else if (value->IsAddressValue())
    {
        AddressValue* v = static_cast<AddressValue*>(value);
        otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
        leaInst->AddOperand(reg);
        otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
        leaInst->AddOperand(assemblyContext->MakeSymbol(v->GetValue()->Name()));
        codeGenerator.Emit(leaInst);
    }
    else
    {
        otava::assembly::Instruction* inst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        inst->AddOperand(reg);
        otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
        switch (value->Kind())
        {
        case ValueKind::boolValue:
        {
            BoolValue* v = static_cast<BoolValue*>(value);
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 1));
            break;
        }
        case ValueKind::sbyteValue:
        {
            SByteValue* v = static_cast<SByteValue*>(value);
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 1));
            break;
        }
        case ValueKind::byteValue:
        {
            ByteValue* v = static_cast<ByteValue*>(value);
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 1));
            break;
        }
        case ValueKind::shortValue:
        {
            ShortValue* v = static_cast<ShortValue*>(value);
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 2));
            break;
        }
        case ValueKind::ushortValue:
        {
            UShortValue* v = static_cast<UShortValue*>(value);
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 2));
            break;
        }
        case ValueKind::intValue:
        {
            IntValue* v = static_cast<IntValue*>(value);
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 4));
            break;
        }
        case ValueKind::uintValue:
        {
            UIntValue* v = static_cast<UIntValue*>(value);
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 4));
            break;
        }
        case ValueKind::longValue:
        {
            LongValue* v = static_cast<LongValue*>(value);
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 8));
            break;
        }
        case ValueKind::ulongValue:
        {
            ULongValue* v = static_cast<ULongValue*>(value);
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 8));
            break;
        }
        case ValueKind::nullValue:
        {
            inst->AddOperand(assemblyContext->MakeIntegerLiteral(0, 8));
            break;
        }
        case ValueKind::symbolValue:
        {
            SymbolValue* v = static_cast<SymbolValue*>(value);
            inst->AddOperand(assemblyContext->MakeSymbol(v->Symbol()));
            break;
        }
        default:
        {
            codeGenerator.Error("error making reg operand: not implemented for value kind " + value->KindStr());
            break;
        }
        }
        codeGenerator.Emit(inst);
    }
    return reg;
}

otava::assembly::Register* MakeFloatingPointRegOperand(Value* value, otava::assembly::Register* reg, CodeGenerator& codeGenerator)
{
    std::int64_t size = std::min(value->GetType()->Size(), std::int64_t(reg->Size()));
    if (value->Kind() == ValueKind::regValue)
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        Instruction* inst = regValue->Inst();
        if (inst)
        {
            if (inst->IsLocalInstruction())
            {
                otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
                leaInst->AddOperand(reg);
                EmitFloatingPointPtrOperand(size, value, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                return reg;
            }
            Locations locs = codeGenerator.RegAllocator()->GetLocations(inst);
            if ((locs & Locations::reg) != Locations::none)
            {
                otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(inst);
                if (regGroup)
                {
                    return regGroup->GetReg(size);
                }
                else
                {
                    codeGenerator.Error("error making reg operand: reg group not valid");
                }
            }
            else if ((locs & Locations::frame) != Locations::none)
            {
                FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(inst);
                if (frameLocation.Valid())
                {
                    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
                    otava::assembly::RegisterGroup* regGroup = assemblyContext->GetRegisterPool()->GetRegisterGroup(reg->Group(), true);
                    EmitFloatingPointLoad(size, frameLocation, regGroup, codeGenerator);
                }
                else
                {
                    codeGenerator.Error("error making reg operand: frame location not valid");
                }
            }
            else
            {
                codeGenerator.Error("error making reg operand: no locations for inst");
            }
        }
        else
        {
            codeGenerator.Error("error making reg operand: instruction for reg value not set");
        }
    }
    else if (value->IsAddressValue())
    {
        AddressValue* v = static_cast<AddressValue*>(value);
        otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
        leaInst->AddOperand(reg);
        otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
        leaInst->AddOperand(assemblyContext->MakeSymbol(v->GetValue()->Name()));
        codeGenerator.Emit(leaInst);
    }
    else
    {
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = otava::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = otava::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error making reg operand: invalid value size");
        }
        otava::assembly::Instruction* inst = new otava::assembly::Instruction(opCode);
        inst->AddOperand(reg);
        otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
        switch (value->Kind())
        {
        case ValueKind::floatValue:
        {
            FloatValue* v = static_cast<FloatValue*>(value);
            inst->AddOperand(assemblyContext->MakeFloatLiteralSymbol(v->GetValue()));
            break;
        }
        case ValueKind::doubleValue:
        {
            DoubleValue* v = static_cast<DoubleValue*>(value);
            inst->AddOperand(assemblyContext->MakeDoubleLiteralSymbol(v->GetValue()));
            break;
        }
        default:
        {
            codeGenerator.Error("error making reg operand: invalid value kind " + value->KindStr());
            break;
        }
        }
        codeGenerator.Emit(inst);
    }
    return reg;
}

void EmitIntegerRet(RetInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    if (inst.ReturnValue())
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        std::int64_t size = inst.ReturnValue()->GetType()->Size();
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax));
        otava::assembly::Register* reg = MakeIntegerRegOperand(inst.ReturnValue(),
            assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
        movInst->AddOperand(reg);
        codeGenerator.Emit(movInst);
    }
    codeGenerator.EmitJumpToExit(inst);
}

void EmitFloatingPointRet(RetInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    if (inst.ReturnValue())
    {
        std::int64_t size = inst.ReturnValue()->GetType()->Size();
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = otava::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = otava::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error making floating point ret: invalid value size");
        }
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(opCode);
        movInst->AddOperand(assemblyContext->GetGlobalReg(16, otava::assembly::RegisterGroupKind::xmm0));
        movInst->AddOperand(MakeFloatingPointRegOperand(
            inst.ReturnValue(), assemblyContext->GetGlobalReg(16, otava::assembly::RegisterGroupKind::xmm1), codeGenerator));
        codeGenerator.Emit(movInst);
    }
    codeGenerator.EmitJumpToExit(inst);
}

void EmitSignExtend(SignExtendInstruction& inst, CodeGenerator& codeGenerator)
{
    std::int64_t operandSize = inst.Operand()->GetType()->Size();
    std::int64_t resultSize = inst.Result()->GetType()->Size();
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::OpCode opCode = otava::assembly::OpCode::MOVSX;
    if (operandSize == 4)
    {
        opCode = otava::assembly::OpCode::MOVSXD;
    }
    otava::assembly::Instruction* movsxInst = new otava::assembly::Instruction(opCode);
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        otava::assembly::Register* reg = regGroup->GetReg(resultSize);
        movsxInst->AddOperand(reg);
        otava::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        movsxInst->AddOperand(sourceReg);
        codeGenerator.Emit(movsxInst);
    }
    else
    {
        codeGenerator.Error("error emitting sign extend instruction: reg group for inst not found");
    }
}

void EmitZeroExtend(ZeroExtendInstruction& inst, CodeGenerator& codeGenerator)
{
    std::int64_t operandSize = inst.Operand()->GetType()->Size();
    std::int64_t resultSize = inst.Result()->GetType()->Size();
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::OpCode opCode = otava::assembly::OpCode::MOV;
    if (operandSize <= 2 && operandSize != resultSize)
    {
        opCode = otava::assembly::OpCode::MOVZX;
    }
    else if (resultSize == 8)
    {
        resultSize = 4;
    }
    otava::assembly::Instruction* movInst = new otava::assembly::Instruction(opCode);
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        otava::assembly::Register* reg = regGroup->GetReg(resultSize);
        movInst->AddOperand(reg);
        otava::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting zero extend instruction: reg group for inst not found");
    }
}

void EmitFloatingPointExtend(FloatingPointExtendInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        std::int64_t resultSize = inst.Result()->GetType()->Size();
        std::int64_t operandSize = inst.Operand()->GetType()->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        otava::assembly::Register* sourceReg = MakeFloatingPointRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::xmm0), codeGenerator);
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (resultSize == 8 && operandSize == 4)
        {
            opCode = otava::assembly::OpCode::CVTSS2SD;
        }
        else
        {
            codeGenerator.Error("error emitting fpextend instruction: invalid floating-point types");
        }
        otava::assembly::Instruction* convertInst = new otava::assembly::Instruction(opCode);
        convertInst->AddOperand(resultReg);
        convertInst->AddOperand(sourceReg);
        codeGenerator.Emit(convertInst);
    }
    else
    {
        codeGenerator.Error("error emitting fpextend instruction: reg group for inst not found");
    }
}

void EmitIntegerTruncate(TruncateInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        std::int64_t resultSize = inst.Result()->GetType()->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        otava::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(resultSize, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting truncate instruction: reg group for inst not found");
    }
}

void EmitFloatingPointTruncate(TruncateInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        std::int64_t resultSize = inst.Result()->GetType()->Size();
        std::int64_t operandSize = inst.Operand()->GetType()->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        otava::assembly::Register* sourceReg = MakeFloatingPointRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::xmm0), codeGenerator);
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (resultSize == 4 && operandSize == 8)
        {
            opCode = otava::assembly::OpCode::CVTSD2SS;
        }
        else
        {
            codeGenerator.Error("error emitting truncate instruction: invalid floating-point truncation types");
        }
        otava::assembly::Instruction* convertInst = new otava::assembly::Instruction(opCode);
        convertInst->AddOperand(resultReg);
        convertInst->AddOperand(sourceReg);
        codeGenerator.Emit(convertInst);
    }
    else
    {
        codeGenerator.Error("error emitting truncate instruction: reg group for inst not found");
    }
}

void EmitIntegerBinOpInst(BinaryInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    std::int64_t size = inst.GetType()->Size();
    otava::assembly::OpCode opCode = otava::assembly::OpCode::ADD;
    bool smallMod = false;
    bool smallDiv = false;
    switch (inst.GetOpCode())
    {
    case OpCode::add:
    {
        opCode = otava::assembly::OpCode::ADD;
        break;
    }
    case OpCode::sub:
    {
        opCode = otava::assembly::OpCode::SUB;
        break;
    }
    case OpCode::mul:
    {
        if (inst.GetType()->IsUnsignedType())
        {
            opCode = otava::assembly::OpCode::MUL;
        }
        else
        {
            opCode = otava::assembly::OpCode::IMUL;
        }
        break;
    }
    case OpCode::div_:
    {
        if (inst.GetType()->IsUnsignedType())
        {
            opCode = otava::assembly::OpCode::DIV;
        }
        else
        {
            opCode = otava::assembly::OpCode::IDIV;
        }
        break;
    }
    case OpCode::mod:
    {
        if (inst.GetType()->IsUnsignedType())
        {
            opCode = otava::assembly::OpCode::DIV;
        }
        else
        {
            opCode = otava::assembly::OpCode::IDIV;
        }
        break;
    }
    case OpCode::and_:
    {
        opCode = otava::assembly::OpCode::AND;
        break;
    }
    case OpCode::or_:
    {
        opCode = otava::assembly::OpCode::OR;
        break;
    }
    case OpCode::xor_:
    {
        opCode = otava::assembly::OpCode::XOR;
        break;
    }
    case OpCode::shl:
    {
        opCode = otava::assembly::OpCode::SHL;
        break;
    }
    case OpCode::shr:
    {
        if (inst.GetType()->IsUnsignedType())
        {
            opCode = otava::assembly::OpCode::SHR;
        }
        else
        {
            opCode = otava::assembly::OpCode::SAR;
        }
        break;
    }
    }
    otava::assembly::Instruction* instruction = new otava::assembly::Instruction(opCode);
    otava::assembly::Register* leftOperandReg = nullptr;
    otava::assembly::Register* rightOperandReg = nullptr;
    otava::assembly::Register* resultOperandReg = nullptr;
    switch (inst.GetOpCode())
    {
    case OpCode::add:
    case OpCode::sub:
    case OpCode::and_:
    case OpCode::or_:
    case OpCode::xor_:
    {
        leftOperandReg = MakeIntegerRegOperand(inst.Left(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        instruction->AddOperand(leftOperandReg);
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        resultOperandReg = leftOperandReg;
        break;
    }
    case OpCode::div_:
    {
        otava::assembly::Register* rdx = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rdx);
        otava::assembly::Instruction* xorInst = new otava::assembly::Instruction(otava::assembly::OpCode::XOR);
        xorInst->AddOperand(rdx);
        xorInst->AddOperand(rdx);
        codeGenerator.Emit(xorInst);
        otava::assembly::Register* rax = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax);
        leftOperandReg = MakeIntegerRegOperand(inst.Left(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        if (leftOperandReg->Group() != otava::assembly::RegisterGroupKind::rax)
        {
            otava::assembly::Instruction* movInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
            movInstruction->AddOperand(rax);
            movInstruction->AddOperand(leftOperandReg);
            codeGenerator.Emit(movInstruction);
        }
        resultOperandReg = rax;
        if (size < 2)
        {
            smallDiv = true;
        }
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        break;
    }
    case OpCode::mul:
    {
        otava::assembly::Register* rax = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax);
        leftOperandReg = MakeIntegerRegOperand(inst.Left(),
            assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        if (leftOperandReg->Group() != otava::assembly::RegisterGroupKind::rax)
        {
            otava::assembly::Instruction* movInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
            movInstruction->AddOperand(rax);
            movInstruction->AddOperand(leftOperandReg);
            codeGenerator.Emit(movInstruction);
        }
        resultOperandReg = rax;
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        break;
    }
    case OpCode::mod:
    {
        otava::assembly::Register* rdx = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rdx);
        otava::assembly::Instruction* xorInst = new otava::assembly::Instruction(otava::assembly::OpCode::XOR);
        xorInst->AddOperand(rdx);
        xorInst->AddOperand(rdx);
        codeGenerator.Emit(xorInst);
        otava::assembly::Register* rax = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax);
        leftOperandReg = MakeIntegerRegOperand(inst.Left(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        if (leftOperandReg->Group() != otava::assembly::RegisterGroupKind::rax)
        {
            otava::assembly::Instruction* movInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
            movInstruction->AddOperand(rax);
            movInstruction->AddOperand(leftOperandReg);
            codeGenerator.Emit(movInstruction);
        }
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        if (size > 1)
        {
            resultOperandReg = rdx;
        }
        else
        {
            resultOperandReg = rax;
            smallMod = true;
        }
        break;
    }
    case OpCode::shl:
    case OpCode::shr:
    {
        leftOperandReg = MakeIntegerRegOperand(inst.Left(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        otava::assembly::Register* cl = assemblyContext->GetGlobalReg(1, otava::assembly::RegisterGroupKind::rcx);
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(1, otava::assembly::RegisterGroupKind::rcx), codeGenerator);
        if (rightOperandReg->Group() != otava::assembly::RegisterGroupKind::rcx)
        {
            otava::assembly::Instruction* movInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
            movInstruction->AddOperand(cl);
            movInstruction->AddOperand(rightOperandReg);
            codeGenerator.Emit(movInstruction);
        }
        instruction->AddOperand(leftOperandReg);
        instruction->AddOperand(cl);
        resultOperandReg = leftOperandReg;
        break;
    }
    }
    if (size == 1 && (smallMod || smallDiv) && inst.GetType()->IsSignedType())
    {
        otava::assembly::Instruction* cbwInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::CBW);
        codeGenerator.Emit(cbwInstruction);
    }
    else if (size == 2 && (inst.GetOpCode() == OpCode::div_ || inst.GetOpCode() == OpCode::mod) && inst.GetType()->IsSignedType())
    {
        otava::assembly::Instruction* cwdInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::CWD);
        codeGenerator.Emit(cwdInstruction);
    }
    else if (size == 4 && (inst.GetOpCode() == OpCode::div_ || inst.GetOpCode() == OpCode::mod) && inst.GetType()->IsSignedType())
    {
        otava::assembly::Instruction* cdqInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::CDQ);
        codeGenerator.Emit(cdqInstruction);
    }
    else if (size == 8 && (inst.GetOpCode() == OpCode::div_ || inst.GetOpCode() == OpCode::mod) && inst.GetType()->IsSignedType())
    {
        otava::assembly::Instruction* cqoInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::CQO);
        codeGenerator.Emit(cqoInstruction);
    }
    codeGenerator.Emit(instruction);
    if (smallMod)
    {
        otava::assembly::Instruction* shrInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::SHR);
        shrInstruction->AddOperand(assemblyContext->GetGlobalReg(2, otava::assembly::RegisterGroupKind::rax));
        shrInstruction->AddOperand(assemblyContext->MakeIntegerLiteral(8, 1));
        codeGenerator.Emit(shrInstruction);
    }
    otava::assembly::Instruction* movInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        if (smallDiv)
        {
            otava::assembly::Instruction* xorInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::XOR);
            otava::assembly::Register* ah = assemblyContext->GetGlobalReg(otava::assembly::highByteRegSize, otava::assembly::RegisterGroupKind::rax);
            xorInstruction->AddOperand(ah);
            xorInstruction->AddOperand(ah);
            codeGenerator.Emit(xorInstruction);
        }
        movInstruction->AddOperand(regGroup->GetReg(size));
        movInstruction->AddOperand(resultOperandReg);
        codeGenerator.Emit(movInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting integer binary operation instruction: reg group for inst not found");
    }
}

void EmitFloatingPointBinOpInst(BinaryInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    std::int64_t size = inst.GetType()->Size();
    if (size != 4 && size != 8)
    {
        codeGenerator.Error("error emitting integer binary operation instruction: invalid size");
    }
    otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
    switch (inst.GetOpCode())
    {
    case OpCode::add:
    {
        if (size == 4)
        {
            opCode = otava::assembly::OpCode::ADDSS;
        }
        else if (size == 8)
        {
            opCode = otava::assembly::OpCode::ADDSD;
        }
        break;
    }
    case OpCode::sub:
    {
        if (size == 4)
        {
            opCode = otava::assembly::OpCode::SUBSS;
        }
        else if (size == 8)
        {
            opCode = otava::assembly::OpCode::SUBSD;
        }
        break;
    }
    case OpCode::mul:
    {
        if (size == 4)
        {
            opCode = otava::assembly::OpCode::MULSS;
        }
        else if (size == 8)
        {
            opCode = otava::assembly::OpCode::MULSD;
        }
        break;
    }
    case OpCode::div_:
    {
        if (size == 4)
        {
            opCode = otava::assembly::OpCode::DIVSS;
        }
        else if (size == 8)
        {
            opCode = otava::assembly::OpCode::DIVSD;
        }
        break;
    }
    }
    otava::assembly::Instruction* instruction = new otava::assembly::Instruction(opCode);
    otava::assembly::Register* leftOperandReg = MakeFloatingPointRegOperand(
        inst.Left(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm0), codeGenerator);
    instruction->AddOperand(leftOperandReg);
    otava::assembly::Register* rightOperandReg = MakeFloatingPointRegOperand(
        inst.Right(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm1), codeGenerator);
    instruction->AddOperand(rightOperandReg);
    otava::assembly::Register* resultOperandReg = leftOperandReg;
    codeGenerator.Emit(instruction);
    otava::assembly::OpCode movOpCode = otava::assembly::OpCode::NOP;
    if (size == 4)
    {
        movOpCode = otava::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        movOpCode = otava::assembly::OpCode::MOVSD;
    }
    otava::assembly::Instruction* movInstruction = new otava::assembly::Instruction(movOpCode);
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        movInstruction->AddOperand(regGroup->GetReg(size));
        movInstruction->AddOperand(resultOperandReg);
        codeGenerator.Emit(movInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting floating-point binary operation instruction: reg group for inst not found");
    }
}

void EmitBitcast(BitcastInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        std::int64_t size = type->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(size);
        otava::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting bitcast instruction: reg group for inst not found");
    }
}

void EmitIntToFloat(IntToFloatInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    Value* operand = inst.Operand();
    Type* operandType = operand->GetType();
    std::int64_t operandSize = operandType->Size();
    Type* resultType = inst.Result()->GetType();
    std::int64_t resultSize = resultType->Size();
    otava::assembly::Register* sourceReg = MakeIntegerRegOperand(
        inst.Operand(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
    if (operandSize < 4)
    {
        otava::assembly::OpCode opCode = otava::assembly::OpCode::MOVSX;
        if (operandType->IsUnsignedType())
        {
            opCode = otava::assembly::OpCode::MOVZX;
        }
        otava::assembly::Instruction* extendInst = new otava::assembly::Instruction(opCode);
        otava::assembly::Register* rax = assemblyContext->GetGlobalReg(4, otava::assembly::RegisterGroupKind::rax);
        extendInst->AddOperand(rax);
        extendInst->AddOperand(sourceReg);
        codeGenerator.Emit(extendInst);
        sourceReg = rax;
    }
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        otava::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (resultSize == 4)
        {
            opCode = otava::assembly::OpCode::CVTSI2SS;
        }
        else if (resultSize == 8)
        {
            opCode = otava::assembly::OpCode::CVTSI2SD;
        }
        else
        {
            codeGenerator.Error("error emitting inttofloat instruction: invalid result size");
        }
        otava::assembly::Instruction* convertInstruction = new otava::assembly::Instruction(opCode);
        convertInstruction->AddOperand(resultReg);
        convertInstruction->AddOperand(sourceReg);
        codeGenerator.Emit(convertInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting inttofloat instruction: reg group for inst not found");
    }
}

void EmitFloatToInt(FloatToIntInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    Value* operand = inst.Operand();
    Type* operandType = operand->GetType();
    std::int64_t operandSize = operandType->Size();
    Type* resultType = inst.Result()->GetType();
    std::int64_t resultSize = resultType->Size();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        otava::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        otava::assembly::Register* sourceReg = MakeFloatingPointRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::xmm0), codeGenerator);
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (operandSize == 4)
        {
            opCode = otava::assembly::OpCode::CVTTSS2SI;
        }
        else if (operandSize == 8)
        {
            opCode = otava::assembly::OpCode::CVTTSD2SI;
        }
        else
        {
            codeGenerator.Error("error emitting floattoint instruction: invalid operand size");
        }
        otava::assembly::Instruction* convertInstruction = new otava::assembly::Instruction(opCode);
        convertInstruction->AddOperand(resultReg);
        convertInstruction->AddOperand(sourceReg);
        codeGenerator.Emit(convertInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting floattoint instruction: reg group for inst not found");
    }
}

void EmitIntToPtr(IntToPtrInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        std::int64_t size = type->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(size);
        otava::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting inttoptr instruction: reg group for inst not found");
    }
}

void EmitPtrToInt(PtrToIntInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        std::int64_t size = type->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(size);
        otava::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting ptrtoint instruction: reg group for inst not found");
    }
}

void EmitIntegerEqual(EqualInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        std::int64_t resultSize = 1;
        std::int64_t size = inst.Left()->GetType()->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        otava::assembly::Instruction* cmpInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::CMP);
        otava::assembly::Register* leftOperandReg = MakeIntegerRegOperand(
            inst.Left(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rcx), codeGenerator);
        cmpInstruction->AddOperand(leftOperandReg);
        otava::assembly::Register* rightOperandReg = MakeIntegerRegOperand(
            inst.Right(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rdx), codeGenerator);
        cmpInstruction->AddOperand(rightOperandReg);
        codeGenerator.Emit(cmpInstruction);
        otava::assembly::Instruction* setInst = new otava::assembly::Instruction(otava::assembly::OpCode::SETE);
        setInst->AddOperand(resultReg);
        codeGenerator.Emit(setInst);
    }
    else
    {
        codeGenerator.Error("error emitting equal instruction: reg group for inst not found");
    }
}

void EmitFloatingPointEqual(EqualInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    Value* leftOperand = inst.Left();
    Type* leftOperandType = leftOperand->GetType();
    std::int64_t operandSize = leftOperandType->Size();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        std::int64_t resultSize = 1;
        otava::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (operandSize == 4)
        {
            opCode = otava::assembly::OpCode::UCOMISS;
        }
        else if (operandSize == 8)
        {
            opCode = otava::assembly::OpCode::UCOMISD;
        }
        else
        {
            codeGenerator.Error("error emitting equal instruction: invalid operand size");
        }
        otava::assembly::Instruction* instruction = new otava::assembly::Instruction(opCode);
        otava::assembly::Register* leftOperandReg = MakeFloatingPointRegOperand(
            inst.Left(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::xmm1), codeGenerator);
        instruction->AddOperand(leftOperandReg);
        otava::assembly::Register* rightOperandReg = MakeFloatingPointRegOperand(
            inst.Right(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::xmm2), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        codeGenerator.Emit(instruction);
        otava::assembly::Instruction* setInst = new otava::assembly::Instruction(otava::assembly::OpCode::SETE);
        setInst->AddOperand(resultReg);
        codeGenerator.Emit(setInst);
    }
    else
    {
        codeGenerator.Error("error emitting equal instruction: reg group for inst not found");
    }
}

void EmitIntegerLess(LessInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        std::int64_t resultSize = 1;
        std::int64_t size = inst.Left()->GetType()->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        otava::assembly::Instruction* cmpInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::CMP);
        otava::assembly::Register* leftOperandReg = MakeIntegerRegOperand(
            inst.Left(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rcx), codeGenerator);
        cmpInstruction->AddOperand(leftOperandReg);
        otava::assembly::Register* rightOperandReg = MakeIntegerRegOperand(
            inst.Right(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rdx), codeGenerator);
        cmpInstruction->AddOperand(rightOperandReg);
        codeGenerator.Emit(cmpInstruction);
        otava::assembly::OpCode opCode = otava::assembly::OpCode::SETL;
        if (inst.Left()->GetType()->IsUnsignedType())
        {
            opCode = otava::assembly::OpCode::SETC;
        }
        otava::assembly::Instruction* setInst = new otava::assembly::Instruction(opCode);
        setInst->AddOperand(resultReg);
        codeGenerator.Emit(setInst);
    }
    else
    {
        codeGenerator.Error("error emitting less instruction: reg group for inst not found");
    }
}

void EmitFloatingPointLess(LessInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    Value* leftOperand = inst.Left();
    Type* leftOperandType = leftOperand->GetType();
    std::int64_t operandSize = leftOperandType->Size();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        std::int64_t resultSize = 1;
        otava::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (operandSize == 4)
        {
            opCode = otava::assembly::OpCode::COMISS;
        }
        else if (operandSize == 8)
        {
            opCode = otava::assembly::OpCode::COMISD;
        }
        else
        {
            codeGenerator.Error("error emitting less instruction: invalid operand size");
        }
        otava::assembly::Instruction* instruction = new otava::assembly::Instruction(opCode);
        otava::assembly::Register* leftOperandReg = MakeFloatingPointRegOperand(
            inst.Left(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::xmm1), codeGenerator);
        instruction->AddOperand(leftOperandReg);
        otava::assembly::Register* rightOperandReg = MakeFloatingPointRegOperand(
            inst.Right(), assemblyContext->GetGlobalReg(operandSize, otava::assembly::RegisterGroupKind::xmm2), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        codeGenerator.Emit(instruction);
        otava::assembly::Instruction* setInst = new otava::assembly::Instruction(otava::assembly::OpCode::SETC);
        setInst->AddOperand(resultReg);
        codeGenerator.Emit(setInst);
    }
    else
    {
        codeGenerator.Error("error emitting less instruction: reg group for inst not found");
    }
}

void EmitJmp(JmpInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::Instruction* jmpInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::JMP);
    jmpInstruction->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(inst.TargetBasicBlock()->Id())));
    codeGenerator.Emit(jmpInstruction);
}

void EmitBranch(BranchInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    int size = 4;
    otava::assembly::Register* operandReg = MakeIntegerRegOperand(
        inst.Cond(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
    otava::assembly::Instruction* orInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::OR);
    orInstruction->AddOperand(operandReg);
    orInstruction->AddOperand(operandReg);
    codeGenerator.Emit(orInstruction);
    codeGenerator.EmitBranchJumps(inst);
}

void EmitNot(NotInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        std::int64_t size = type->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(size);
        otava::assembly::Register* operandReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(operandReg);
        codeGenerator.Emit(movInst);
        if (type->IsBooleanType())
        {
            otava::assembly::Instruction* xorInst = new otava::assembly::Instruction(otava::assembly::OpCode::XOR);
            xorInst->AddOperand(resultReg);
            xorInst->AddOperand(assemblyContext->MakeIntegerLiteral(1, int(size)));
            codeGenerator.Emit(xorInst);
        }
        else if (type->IsIntegerType())
        {
            otava::assembly::Instruction* notInst = new otava::assembly::Instruction(otava::assembly::OpCode::NOT);
            notInst->AddOperand(resultReg);
            codeGenerator.Emit(notInst);
        }
        else
        {
            codeGenerator.Error("error emitting not instruction: integer or boolean type expected");
        }
    }
    else
    {
        codeGenerator.Error("error emitting not instruction: reg group for inst not found");
    }
}

void EmitIntegerNeg(NegInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        std::int64_t size = type->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(size);
        otava::assembly::Register* operandReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(operandReg);
        codeGenerator.Emit(movInst);
        otava::assembly::Instruction* negInst = new otava::assembly::Instruction(otava::assembly::OpCode::NEG);
        negInst->AddOperand(resultReg);
        codeGenerator.Emit(negInst);
    }
    else
    {
        codeGenerator.Error("error emitting neg instruction: reg group for inst not found");
    }
}

void EmitFloatingPointNeg(NegInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        std::int64_t size = type->Size();
        otava::assembly::Register* resultReg = regGroup->GetReg(size);
        otava::assembly::Register* operandReg = MakeFloatingPointRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm0), codeGenerator);
        otava::assembly::OpCode mulOpCode = otava::assembly::OpCode::NOP;
        std::unique_ptr<Value> value(nullptr);
        if (size == 4)
        {
            value.reset(new FloatValue(-1.0f, codeGenerator.Ctx()->GetTypes().GetFloatType()));
            mulOpCode = otava::assembly::OpCode::MULSS;
        }
        else if (size == 8)
        {
            value.reset(new DoubleValue(-1.0, codeGenerator.Ctx()->GetTypes().GetDoubleType()));
            mulOpCode = otava::assembly::OpCode::MULSD;
        }
        else
        {
            codeGenerator.Error("error emitting neg instruction: invalid operand size");
        }
        otava::assembly::Register* minusOneReg = MakeFloatingPointRegOperand(
            value.get(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm1), codeGenerator);
        otava::assembly::Instruction* instruction = new otava::assembly::Instruction(mulOpCode);
        instruction->AddOperand(operandReg);
        instruction->AddOperand(minusOneReg);
        codeGenerator.Emit(instruction);
        otava::assembly::OpCode movOpCode = otava::assembly::OpCode::NOP;
        if (size == 4)
        {
            movOpCode = otava::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            movOpCode = otava::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error emitting neg instruction: invalid operand size");
        }
        otava::assembly::Instruction* movInstruction = new otava::assembly::Instruction(movOpCode);
        movInstruction->AddOperand(resultReg);
        movInstruction->AddOperand(operandReg);
        codeGenerator.Emit(movInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting neg instruction: reg group for inst not found");
    }
}

void EmitNop(NoOperationInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::Instruction* nopInst = new otava::assembly::Instruction(otava::assembly::OpCode::NOP);
    codeGenerator.Emit(nopInst);
}

void EmitGetRbp(GetRbpInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        otava::assembly::Register* resultReg = regGroup->GetReg(8);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbp));
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting getrbp instruction: reg group for inst not found");
    }
}

void EmitSwitch(SwitchInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    std::int64_t size = inst.Cond()->GetType()->Size();
    otava::assembly::Register* condReg = MakeIntegerRegOperand(
        inst.Cond(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
    for (const auto& caseTarget : inst.CaseTargets())
    {
        std::int64_t size = caseTarget.caseValue->GetType()->Size();
        otava::assembly::Register* caseReg = MakeIntegerRegOperand(
            caseTarget.caseValue, assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        otava::assembly::Instruction* cmpInst = new otava::assembly::Instruction(otava::assembly::OpCode::CMP);
        cmpInst->AddOperand(caseReg);
        cmpInst->AddOperand(condReg);
        codeGenerator.Emit(cmpInst);
        otava::assembly::Instruction* jeInst = new otava::assembly::Instruction(otava::assembly::OpCode::JE);
        jeInst->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(caseTarget.targetBlock->Id())));
        codeGenerator.Emit(jeInst);
    }
    otava::assembly::Instruction* jmpInst = new otava::assembly::Instruction(otava::assembly::OpCode::JMP);
    jmpInst->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(inst.DefaultTargetBlock()->Id())));
    codeGenerator.Emit(jmpInst);
}

void EmitIntegerLoad(std::int64_t size, const FrameLocation& frameLocation, otava::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator)
{
    if (!frameLocation.Valid())
    {
        codeGenerator.Error("error emitting load: frame location not valid");
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::Instruction* instruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
    instruction->AddOperand(regGroup->GetReg(size));
    EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
    codeGenerator.Emit(instruction);
}

otava::assembly::Instruction* EmitIntegerStore(std::int64_t size, const FrameLocation& frameLocation, otava::assembly::RegisterGroup* regGroup,
    CodeGenerator& codeGenerator)
{
    if (!frameLocation.Valid())
    {
        codeGenerator.Error("error emitting store: frame location not valid");
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::Instruction* instruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
    EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
    instruction->AddOperand(regGroup->GetReg(size));
    codeGenerator.Emit(instruction);
    return instruction;
}

void EmitFloatingPointLoad(std::int64_t size, const FrameLocation& frameLocation, otava::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator)
{
    if (!frameLocation.Valid())
    {
        codeGenerator.Error("error emitting load: frame location not valid");
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
    if (size == 4)
    {
        opCode = otava::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        opCode = otava::assembly::OpCode::MOVSD;
    }
    else
    {
        codeGenerator.Error("error emitting load: invalid size");
    }
    otava::assembly::Instruction* instruction = new otava::assembly::Instruction(opCode);
    instruction->AddOperand(regGroup->GetReg(size));
    EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
    codeGenerator.Emit(instruction);
}

otava::assembly::Instruction* EmitFloatingPointStore(std::int64_t size, const FrameLocation& frameLocation, otava::assembly::RegisterGroup* regGroup,
    CodeGenerator& codeGenerator)
{
    if (!frameLocation.Valid())
    {
        codeGenerator.Error("error emitting store: frame location not valid");
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
    if (size == 4)
    {
        opCode = otava::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        opCode = otava::assembly::OpCode::MOVSD;
    }
    else
    {
        codeGenerator.Error("error emitting load: invalid size");
    }
    otava::assembly::Instruction* instruction = new otava::assembly::Instruction(opCode);
    EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
    instruction->AddOperand(regGroup->GetReg(size));
    codeGenerator.Emit(instruction);
    return instruction;
}

void EmitIntegerLoad(LoadInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (!regGroup)
    {
        codeGenerator.Error("error emitting load: reg group not valid");
    }
    Type* type = inst.Result()->GetType();
    if (type->IsFundamentalType() || type->IsPointerType())
    {
        std::int64_t size = type->Size();
        otava::assembly::Instruction* instruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        instruction->AddOperand(regGroup->GetReg(size));
        EmitIntegerPtrOperand(size, inst.Ptr(), instruction, codeGenerator);
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting load: invalid type kind");
    }
}

void EmitIntegerStore(StoreInstruction& inst, CodeGenerator& codeGenerator)
{
    Type* type = inst.GetValue()->GetType();
    if (type->IsFundamentalType() || type->IsPointerType())
    {
        std::int64_t size = type->Size();
        otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
        otava::assembly::Instruction* instruction = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        EmitIntegerPtrOperand(size, inst.GetPtr(), instruction, codeGenerator);
        otava::assembly::Register* reg = MakeIntegerRegOperand(
            inst.GetValue(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        instruction->AddOperand(reg);
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting store: invalid type kind");
    }
}

void EmitFloatingPointLoad(LoadInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (!regGroup)
    {
        codeGenerator.Error("error emitting load: reg group not valid");
    }
    Type* type = inst.Result()->GetType();
    if (type->IsFloatingPointType())
    {
        std::int64_t size = type->Size();
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = otava::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = otava::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error emitting load: invalid size");
        }
        otava::assembly::Instruction* instruction = new otava::assembly::Instruction(opCode);
        instruction->AddOperand(regGroup->GetReg(size));
        EmitFloatingPointPtrOperand(size, inst.Ptr(), instruction, codeGenerator);
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting load: invalid type kind");
    }
}

void EmitFloatingPointStore(StoreInstruction& inst, CodeGenerator& codeGenerator)
{
    Type* type = inst.GetValue()->GetType();
    if (type->IsFloatingPointType())
    {
        std::int64_t size = type->Size();
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = otava::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = otava::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error emitting store: invalid size");
        }
        otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
        otava::assembly::Instruction* instruction = new otava::assembly::Instruction(opCode);
        EmitFloatingPointPtrOperand(size, inst.GetPtr(), instruction, codeGenerator);
        instruction->AddOperand(MakeFloatingPointRegOperand(
            inst.GetValue(), assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm0), codeGenerator));
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting store: invalid type kind");
    }
}

void EmitIntegerParam(ParamInstruction& inst, CodeGenerator& codeGenerator)
{
    Type* type = inst.GetType();
    std::int64_t size = type->Size();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    if (!regGroup)
    {
        codeGenerator.Error("error emitting param: reg group not valid");
    }
    switch (inst.Index())
    {
    case 0:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        otava::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rcx);
        movInst->AddOperand(regGroup->GetReg(size));
        movInst->AddOperand(paramReg);
        codeGenerator.Emit(movInst);
        break;
    }
    case 1:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        otava::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rdx);
        movInst->AddOperand(regGroup->GetReg(size));
        movInst->AddOperand(paramReg);
        codeGenerator.Emit(movInst);
        break;
    }
    case 2:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        otava::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::r8);
        movInst->AddOperand(regGroup->GetReg(size));
        movInst->AddOperand(paramReg);
        codeGenerator.Emit(movInst);
        break;
    }
    case 3:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        otava::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::r9);
        movInst->AddOperand(regGroup->GetReg(size));
        movInst->AddOperand(paramReg);
        codeGenerator.Emit(movInst);
        break;
    }
    default:
    {
        Locations locs = codeGenerator.RegAllocator()->GetLocations(&inst);
        if ((locs & Locations::frame) != Locations::none)
        {
            FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(&inst);
            EmitIntegerLoad(size, frameLocation, regGroup, codeGenerator);
        }
        else
        {
            codeGenerator.Error("error emitting param: no frame location");
        }
    }
    }
}

void EmitFloatingPointParam(ParamInstruction& inst, CodeGenerator& codeGenerator)
{
    Type* type = inst.GetType();
    std::int64_t size = type->Size();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    if (!regGroup)
    {
        codeGenerator.Error("error emitting param: reg group not valid");
    }
    otava::assembly::OpCode movOpCode = otava::assembly::OpCode::NOP;
    if (size == 4)
    {
        movOpCode = otava::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        movOpCode = otava::assembly::OpCode::MOVSD;
    }
    else
    {
        codeGenerator.Error("error emitting param: invalid operand size");
    }
    switch (inst.Index())
    {
    case 0:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(movOpCode);
        otava::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm0);
        movInst->AddOperand(regGroup->GetReg(size));
        movInst->AddOperand(paramReg);
        codeGenerator.Emit(movInst);
        break;
    }
    case 1:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(movOpCode);
        otava::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm1);
        movInst->AddOperand(regGroup->GetReg(size));
        movInst->AddOperand(paramReg);
        codeGenerator.Emit(movInst);
        break;
    }
    case 2:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(movOpCode);
        otava::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm2);
        movInst->AddOperand(regGroup->GetReg(size));
        movInst->AddOperand(paramReg);
        codeGenerator.Emit(movInst);
        break;
    }
    case 3:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(movOpCode);
        otava::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm3);
        movInst->AddOperand(regGroup->GetReg(size));
        movInst->AddOperand(paramReg);
        codeGenerator.Emit(movInst);
        break;
    }
    default:
    {
        Locations locs = codeGenerator.RegAllocator()->GetLocations(&inst);
        if ((locs & Locations::frame) != Locations::none)
        {
            FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(&inst);
            EmitFloatingPointLoad(size, frameLocation, regGroup, codeGenerator);
        }
        else
        {
            codeGenerator.Error("error emitting param: no frame location");
        }
    }
    }
}

void EmitIntegerArgOperand(Instruction* argInst, std::int64_t size, Value* arg, otava::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    if (arg->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(arg);
        Instruction* inst = regValue->Inst();
        if (inst->IsLocalInstruction())
        {
            otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(argInst);
            if (regGroup)
            {
                otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
                leaInst->AddOperand(regGroup->GetReg(8));
                FrameLocation frameLocation = GetFrameLocation(arg, codeGenerator);
                EmitFrameLocationOperand(1, frameLocation, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                instruction->AddOperand(regGroup->GetReg(8));
                return;
            }
            else
            {
                codeGenerator.Error("error emitting arg: reg group not valid");
            }
        }
        else if (inst->IsPLocalInstruction())
        {
            otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(argInst);
            if (regGroup)
            {
                otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
                leaInst->AddOperand(regGroup->GetReg(8));
                FrameLocation frameLocation = GetFrameLocation(arg, codeGenerator);
                EmitFrameLocationOperand(1, frameLocation, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                instruction->AddOperand(regGroup->GetReg(8));
                return;
            }
        }
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::Register* reg = MakeIntegerRegOperand(arg, assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rax), codeGenerator);
    instruction->AddOperand(reg);
}

void EmitFloatingPointArgOperand(Instruction* argInst, std::int64_t size, Value* arg, otava::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    if (arg->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(arg);
        Instruction* inst = regValue->Inst();
        if (inst->IsLocalInstruction())
        {
            otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(argInst);
            if (regGroup)
            {
                otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
                leaInst->AddOperand(regGroup->GetReg(8));
                FrameLocation frameLocation = GetFrameLocation(arg, codeGenerator);
                EmitFrameLocationOperand(1, frameLocation, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                instruction->AddOperand(regGroup->GetReg(8));
                return;
            }
            else
            {
                codeGenerator.Error("error emitting arg: reg group not valid");
            }
        }
        else if (inst->IsPLocalInstruction())
        {
            otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(argInst);
            if (regGroup)
            {
                otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
                leaInst->AddOperand(regGroup->GetReg(8));
                FrameLocation frameLocation = GetFrameLocation(arg, codeGenerator);
                EmitFrameLocationOperand(1, frameLocation, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                instruction->AddOperand(regGroup->GetReg(8));
                return;
            }
            else
            {
                codeGenerator.Error("error emitting arg: reg group not valid");
            }
        }
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    instruction->AddOperand(MakeFloatingPointRegOperand(arg, assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm0), codeGenerator));
}

void EmitArgLocationOperand(std::int64_t size, const ArgLocation& argLocation, otava::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::Register* reg = assemblyContext->GetGlobalReg(8, argLocation.reg);
    instruction->AddOperand(assemblyContext->MakeSizePrefix(size,
        assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(reg, assemblyContext->MakeIntegerLiteral(argLocation.offset, 8),
            otava::assembly::Operator::add))));
}

void EmitIntegerArg(ArgInstruction& inst, CallFrame* callFrame, std::int32_t index, CodeGenerator& codeGenerator)
{
    RegisterAllocationAction action = codeGenerator.RegAllocator()->Run(&inst);
    if (action == RegisterAllocationAction::spill)
    {
        for (const SpillData& spillData : codeGenerator.RegAllocator()->GetSpillData())
        {
            otava::assembly::Instruction* storeInst = EmitIntegerStore(
                8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
            Instruction* instToSpill = spillData.instToSpill;
            otava::assembly::Instruction* assemblyInstruction = instToSpill->AssemblyInstruction();
            if (assemblyInstruction)
            {
                assemblyInstruction->LinkAfter(storeInst->Clone());
            }
            else
            {
                codeGenerator.AssemblyFunction()->AddInstruction(storeInst->Clone());
            }
            codeGenerator.RegAllocator()->RemoveFromRegisterGroups(instToSpill);
        }
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    std::int64_t size = inst.Arg()->GetType()->Size();
    switch (index)
    {
    case 0:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rcx));
        EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    case 1:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::rdx));
        EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    case 2:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::r8));
        EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    case 3:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::r9));
        EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    default:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
        if (regGroup)
        {
            otava::assembly::Register* reg = regGroup->GetReg(size);
            movInst->AddOperand(reg);
            EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            otava::assembly::Instruction* pushToFrameInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
            ArgLocation argLocation = callFrame->GetArgLocation(index);
            EmitArgLocationOperand(size, argLocation, pushToFrameInst, codeGenerator);
            pushToFrameInst->AddOperand(reg);
            codeGenerator.Emit(pushToFrameInst);
        }
        else
        {
            codeGenerator.Error("error emitting arg instruction: reg group for inst not found");
        }
        break;
    }
    }
}

void EmitFloatingPointArg(ArgInstruction& inst, CallFrame* callFrame, std::int32_t index, CodeGenerator& codeGenerator)
{
    RegisterAllocationAction action = codeGenerator.RegAllocator()->Run(&inst);
    if (action == RegisterAllocationAction::spill)
    {
        for (const SpillData& spillData : codeGenerator.RegAllocator()->GetSpillData())
        {
            otava::assembly::Instruction* storeInst = EmitFloatingPointStore(
                8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
            Instruction* instToSpill = spillData.instToSpill;
            otava::assembly::Instruction* assemblyInstruction = instToSpill->AssemblyInstruction();
            if (assemblyInstruction)
            {
                assemblyInstruction->LinkAfter(storeInst->Clone());
            }
            else
            {
                codeGenerator.AssemblyFunction()->AddInstruction(storeInst->Clone());
            }
            codeGenerator.RegAllocator()->RemoveFromRegisterGroups(instToSpill);
        }
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    std::int64_t size = inst.Arg()->GetType()->Size();
    otava::assembly::OpCode movOpCode = otava::assembly::OpCode::NOP;
    if (size == 4)
    {
        movOpCode = otava::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        movOpCode = otava::assembly::OpCode::MOVSD;
    }
    else
    {
        codeGenerator.Error("error emitting arg instruction: invalid operand size");
    }
    switch (index)
    {
    case 0:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(movOpCode);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm0));
        EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    case 1:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(movOpCode);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm1));
        EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    case 2:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(movOpCode);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm2));
        EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    case 3:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(movOpCode);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm3));
        EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    default:
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(movOpCode);
        otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
        if (regGroup)
        {
            otava::assembly::Register* reg = regGroup->GetReg(size);
            movInst->AddOperand(reg);
            EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            otava::assembly::Instruction* pushToFrameInst = new otava::assembly::Instruction(movOpCode);
            ArgLocation argLocation = callFrame->GetArgLocation(index);
            EmitArgLocationOperand(size, argLocation, pushToFrameInst, codeGenerator);
            pushToFrameInst->AddOperand(reg);
            codeGenerator.Emit(pushToFrameInst);
        }
        else
        {
            codeGenerator.Error("error emitting arg instruction: reg group for inst not found");
        }
        break;
    }
    }
}

void EmitArgs(const std::vector<ArgInstruction*>& args, CodeGenerator& codeGenerator)
{
    CallFrame callFrame;
    int n = args.size();
    std::int64_t calleeParamAreaSize = util::Align(n * 8, 16);
    Frame& frame = codeGenerator.RegAllocator()->GetFrame();
    frame.SetCalleeParamAreaSize(std::max(frame.CalleeParamAreaSize(), calleeParamAreaSize));
    for (int i = 0; i < n; ++i)
    {
        std::int64_t size = args[i]->Arg()->GetType()->Size();
        callFrame.AllocateArgLocation(size);
    }
    for (int i = n - 1; i >= 0; --i)
    {
        ArgInstruction* arg = args[i];
        if (arg->IsFloatingPointInstruction())
        {
            EmitFloatingPointArg(*arg, &callFrame, i, codeGenerator);
        }
        else
        {
            EmitIntegerArg(*arg, &callFrame, i, codeGenerator);
        }
    }
}

otava::assembly::Value* MakeCalleeOperand(Value* value, otava::assembly::Register* reg, CodeGenerator& codeGenerator)
{
    if (value->Kind() == ValueKind::regValue)
    {
        otava::assembly::Register* r = MakeIntegerRegOperand(value, reg, codeGenerator);
        return r;
    }
    else
    {
        if (value->IsSymbolValue())
        {
            SymbolValue* symbolValue = static_cast<SymbolValue*>(value);
            otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
            return assemblyContext->MakeSymbol(symbolValue->Symbol());
        }
        else
        {
            codeGenerator.Error("error emitting callee operand: invalid value kind");
        }
    }
    return nullptr;
}

void EmitProcedureCall(ProcedureCallInstruction& inst, const std::vector<ArgInstruction*>& args, CodeGenerator& codeGenerator)
{
    EmitArgs(args, codeGenerator);
    RegisterAllocationAction action = codeGenerator.RegAllocator()->Run(&inst);
    if (action == RegisterAllocationAction::spill)
    {
        for (const SpillData& spillData : codeGenerator.RegAllocator()->GetSpillData())
        {
            otava::assembly::Instruction* storeInst = nullptr;
            if (spillData.registerGroupToSpill->IsFloatingPointReg())
            {
                storeInst = EmitFloatingPointStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
            }
            else
            {
                storeInst = EmitIntegerStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
            }
            Instruction* instToSpill = spillData.instToSpill;
            otava::assembly::Instruction* assemblyInstruction = instToSpill->AssemblyInstruction();
            if (assemblyInstruction)
            {
                assemblyInstruction->LinkAfter(storeInst->Clone());
            }
            else
            {
                codeGenerator.AssemblyFunction()->AddInstruction(storeInst->Clone());
            }
            codeGenerator.RegAllocator()->RemoveFromRegisterGroups(instToSpill);
        }
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    otava::assembly::Register* reg = regGroup->GetReg(8);
    otava::assembly::Instruction* callInst = new otava::assembly::Instruction(otava::assembly::OpCode::CALL);
    callInst->AddOperand(MakeCalleeOperand(inst.Callee(), reg, codeGenerator));
    codeGenerator.Emit(callInst);
}

void EmitFunctionCall(FunctionCallInstruction& inst, const std::vector<ArgInstruction*>& args, CodeGenerator& codeGenerator)
{
    EmitArgs(args, codeGenerator);
    RegisterAllocationAction action = codeGenerator.RegAllocator()->Run(&inst);
    if (action == RegisterAllocationAction::spill)
    {
        for (const SpillData& spillData : codeGenerator.RegAllocator()->GetSpillData())
        {
            otava::assembly::Instruction* storeInst = nullptr;
            if (spillData.registerGroupToSpill->IsFloatingPointReg())
            {
                storeInst = EmitFloatingPointStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
            }
            else
            {
                storeInst = EmitIntegerStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
            }
            Instruction* instToSpill = spillData.instToSpill;
            otava::assembly::Instruction* assemblyInstruction = instToSpill->AssemblyInstruction();
            if (assemblyInstruction)
            {
                assemblyInstruction->LinkAfter(storeInst->Clone());
            }
            else
            {
                codeGenerator.AssemblyFunction()->AddInstruction(storeInst->Clone());
            }
            codeGenerator.RegAllocator()->RemoveFromRegisterGroups(instToSpill);
        }
    }
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    otava::assembly::Instruction* callInst = new otava::assembly::Instruction(otava::assembly::OpCode::CALL);
    Type* type = inst.Result()->GetType();
    int size = static_cast<int>(type->Size());
    if (!type->IsFloatingPointType())
    {
        size = 8;
    }
    otava::assembly::Register* reg = regGroup->GetReg(size);
    otava::assembly::Register* callReg = regGroup->GetReg(8);
    callInst->AddOperand(MakeCalleeOperand(inst.Callee(), callReg, codeGenerator));
    codeGenerator.Emit(callInst);
    if (type->IsFloatingPointType())
    {
        otava::assembly::OpCode opCode = otava::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = otava::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = otava::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error emitting function call: invalid result size");
        }
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(opCode);
        movInst->AddOperand(reg);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, otava::assembly::RegisterGroupKind::xmm0));
        codeGenerator.Emit(movInst);
    }
    else
    {
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(reg);
        movInst->AddOperand(assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rax));
        codeGenerator.Emit(movInst);
    }
}

void EmitElemAddr(ElemAddrInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    ElemAddrKind elemAddrKind = inst.GetElemAddrKind(codeGenerator.Ctx());
    if (elemAddrKind == ElemAddrKind::array)
    {
        otava::assembly::Register* indexReg = MakeIntegerRegOperand(
            inst.IndexValue(), assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
        std::int64_t indexFactor = GetElementSize(inst.Ptr()->GetType(), codeGenerator);
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        otava::assembly::Register* rax = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rax);
        movInst->AddOperand(rax);
        movInst->AddOperand(assemblyContext->MakeIntegerLiteral(indexFactor, 8));
        codeGenerator.Emit(movInst);
        otava::assembly::Instruction* mulInst = new otava::assembly::Instruction(otava::assembly::OpCode::MUL);
        mulInst->AddOperand(indexReg);
        codeGenerator.Emit(mulInst);
        otava::assembly::Instruction* leaPtrInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
        otava::assembly::Register* rcx = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rcx);
        leaPtrInst->AddOperand(rcx);
        EmitIntegerPtrOperand(8, inst.Ptr(), leaPtrInst, codeGenerator);
        codeGenerator.Emit(leaPtrInst);
        otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
        if (regGroup)
        {
            otava::assembly::Register* resultReg = regGroup->GetReg(8);
            otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
            leaInst->AddOperand(resultReg);
            leaInst->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
                assemblyContext->MakeBinaryExpr(rax, rcx, otava::assembly::Operator::add))));
            codeGenerator.Emit(leaInst);
        }
        else
        {
            codeGenerator.Error("error emitting elemaddr instruction: reg group for inst not found");
        }
    }
    else if (elemAddrKind == ElemAddrKind::structure)
    {
        otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
        if (regGroup)
        {
            otava::assembly::Register* resultReg = regGroup->GetReg(8);
            std::int64_t index = GetIndex(inst.IndexValue(), codeGenerator);
            std::int64_t offset = GetOffset(inst.Ptr()->GetType(), index, codeGenerator);
            otava::assembly::Instruction* movOffsetInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
            otava::assembly::Register* rbx = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbx);
            movOffsetInst->AddOperand(rbx);
            movOffsetInst->AddOperand(assemblyContext->MakeIntegerLiteral(offset, 8));
            codeGenerator.Emit(movOffsetInst);
            otava::assembly::Instruction* leaPtrInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
            otava::assembly::Register* rcx = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rcx);
            leaPtrInst->AddOperand(rcx);
            EmitIntegerPtrOperand(8, inst.Ptr(), leaPtrInst, codeGenerator);
            codeGenerator.Emit(leaPtrInst);
            otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
            leaInst->AddOperand(resultReg);
            leaInst->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
                assemblyContext->MakeBinaryExpr(rbx, rcx, otava::assembly::Operator::add))));
            codeGenerator.Emit(leaInst);
        }
        else
        {
            codeGenerator.Error("error emitting elemaddr instruction: reg group for inst not found");
        }
    }
    else
    {
        codeGenerator.Error("error emitting elemaddr: invalid elemaddr kind");
    }
}

void EmitPtrOffset(PtrOffsetInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    otava::assembly::Register* offsetReg = MakeIntegerRegOperand(
        inst.Offset(), assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
    std::int64_t offsetFactor = GetPointeeSize(inst.Ptr()->GetType(), codeGenerator);
    otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
    otava::assembly::Register* rax = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rax);
    movInst->AddOperand(rax);
    movInst->AddOperand(assemblyContext->MakeIntegerLiteral(offsetFactor, 8));
    codeGenerator.Emit(movInst);
    otava::assembly::Instruction* mulInst = new otava::assembly::Instruction(otava::assembly::OpCode::MUL);
    mulInst->AddOperand(offsetReg);
    codeGenerator.Emit(mulInst);
    otava::assembly::Register* ptrReg = MakeIntegerRegOperand(
        inst.Ptr(), assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rcx), codeGenerator);
    otava::assembly::Instruction* leaInst = new otava::assembly::Instruction(otava::assembly::OpCode::LEA);
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        otava::assembly::Register* resultReg = regGroup->GetReg(8);
        leaInst->AddOperand(resultReg);
        otava::assembly::Value* operand = assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
            assemblyContext->MakeBinaryExpr(rax, ptrReg, otava::assembly::Operator::add)));
        leaInst->AddOperand(operand);
        codeGenerator.Emit(leaInst);
    }
    else
    {
        codeGenerator.Error("error emitting ptroffset instruction: reg group for inst not found");
    }
}

void EmitPtrDiff(PtrDiffInstruction& inst, CodeGenerator& codeGenerator)
{
    otava::assembly::AssemblyContext* assemblyContext = codeGenerator.Ctx()->GetAssemblyContext();
    std::int64_t scaleFactor = GetPointeeSize(inst.LeftPtr()->GetType(), codeGenerator);
    otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
    otava::assembly::Register* rcx = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rcx);
    movInst->AddOperand(rcx);
    movInst->AddOperand(assemblyContext->MakeIntegerLiteral(scaleFactor, 8));
    codeGenerator.Emit(movInst);
    otava::assembly::Instruction* subInst = new otava::assembly::Instruction(otava::assembly::OpCode::SUB);
    otava::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        otava::assembly::Register* resultReg = regGroup->GetReg(8);
        otava::assembly::Register* leftReg = MakeIntegerRegOperand(
            inst.LeftPtr(), assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rax), codeGenerator);
        subInst->AddOperand(leftReg);
        otava::assembly::Register* rightReg = MakeIntegerRegOperand(
            inst.RightPtr(), assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rbx), codeGenerator);
        subInst->AddOperand(rightReg);
        codeGenerator.Emit(subInst);
        otava::assembly::Register* rdx = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rdx);
        otava::assembly::Instruction* xorInst = new otava::assembly::Instruction(otava::assembly::OpCode::XOR);
        xorInst->AddOperand(rdx);
        xorInst->AddOperand(rdx);
        codeGenerator.Emit(xorInst);
        otava::assembly::Register* rax = assemblyContext->GetGlobalReg(8, otava::assembly::RegisterGroupKind::rax);
        otava::assembly::Instruction* movInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movInst->AddOperand(rax);
        movInst->AddOperand(leftReg);
        codeGenerator.Emit(movInst);
        otava::assembly::Instruction* divInst = new otava::assembly::Instruction(otava::assembly::OpCode::DIV);
        divInst->AddOperand(rcx);
        codeGenerator.Emit(divInst);
        otava::assembly::Instruction* movResultInst = new otava::assembly::Instruction(otava::assembly::OpCode::MOV);
        movResultInst->AddOperand(resultReg);
        movResultInst->AddOperand(rax);
        codeGenerator.Emit(movResultInst);
    }
    else
    {
        codeGenerator.Error("error emitting ptrdiff instruction: reg group for inst not found");
    }
}

CodeGenerator::CodeGenerator(IntermediateContext* context_, const std::string& assemblyFilePath_) :
    Visitor(context_), context(context_), file(assemblyFilePath_), currentFunction(nullptr), currentInst(nullptr), assemblyFunction(nullptr), leader(false),
    registerAllocator(nullptr), data(nullptr), label(), dataInstruction(nullptr), prevDataOpCode(otava::assembly::OpCode::DB), currentOffset(0)
{
    context->GetAssemblyContext()->SetFile(&file);
}

const soul::ast::Span& CodeGenerator::Span() const noexcept
{
    return currentInst->Span();
}

int CodeGenerator::ExitLabelId() const noexcept
{
    return currentInst->Parent()->Parent()->LastBasicBlock()->Id() + 1;
}

void CodeGenerator::Emit(otava::assembly::Instruction* assemblyInstruction)
{
    if (leader)
    {
        leader = false;
        assemblyInstruction->SetLabel("@" + std::to_string(currentInst->Parent()->Id()));
    }
    currentInst->SetAssemblyInstruction(assemblyInstruction);
    assemblyFunction->AddInstruction(assemblyInstruction);
}

void CodeGenerator::EmitDataValue(std::unique_ptr<otava::assembly::Value>&& dataValue, otava::assembly::OpCode dataOpCode)
{
    std::unique_ptr<otava::assembly::Value> dv = std::move(dataValue);
    if (prevDataOpCode != dataOpCode)
    {
        if (dataInstruction)
        {
            data->AddInstruction(std::move(dataInstruction));
            dataInstruction.reset();
        }
    }
    if (!dataInstruction)
    {
        dataInstruction.reset(new otava::assembly::Instruction(dataOpCode));
        dataInstruction->SetNoColon();
    }
    if (leader)
    {
        leader = false;
        if (!dataInstruction->Label().empty())
        {
            data->AddInstruction(std::move(dataInstruction));
            dataInstruction.reset(new otava::assembly::Instruction(dataOpCode));
            dataInstruction->SetNoColon();
        }
        dataInstruction->SetLabel(label);
        label.clear();
    }
    int splitCount = 0;
    while (splitCount < otava::assembly::maxSplitCount && dataInstruction->Length() + dv->Length() > otava::assembly::maxAssemblyLineLength && dv->CanSplit())
    {
        std::unique_ptr<otava::assembly::Value> next(dv->Split(otava::assembly::maxAssemblyLineLength - dataInstruction->Length()));
        dataInstruction->AddOperand(dv.release());
        data->AddInstruction(std::move(dataInstruction));
        dataInstruction.reset(new otava::assembly::Instruction(dataOpCode));
        dataInstruction->SetNoColon();
        dv.reset(next.release());
        ++splitCount;
    }
    dataInstruction->AddOperand(dv.release());
    data->AddInstruction(std::move(dataInstruction));
    dataInstruction.reset(new otava::assembly::Instruction(dataOpCode));
    dataInstruction->SetNoColon();
    prevDataOpCode = dataOpCode;
}

void CodeGenerator::Visit(GlobalVariable& globalVariable)
{
    if (data && dataInstruction)
    {
        data->AddInstruction(std::move(dataInstruction));
        dataInstruction.reset();
    }
    if (globalVariable.Initializer())
    {
        file.GetDeclarationSection().AddPublicDeclaration(new otava::assembly::PublicDeclaration(globalVariable.Name()));
        label = globalVariable.Name();
        data = new otava::assembly::Data();
        file.GetDataSection().AddData(data);
        leader = true;
        currentOffset = 0;
        globalVariable.Initializer()->Accept(*this);
        ByteType* byteType = context->GetTypes().GetByteType();
        while ((currentOffset & 7) != 0)
        {
            ByteValue value(0u, byteType);
            Visit(value);
        }
    }
    else
    {
        file.GetDeclarationSection().AddExternalDataDeclaration(new otava::assembly::ExternalDataDeclaration(globalVariable.Name()));
    }
}

void CodeGenerator::Visit(Function& function)
{
    context->IncTotalFunctions();
    if (!function.IsDefined())
    {
        file.GetDeclarationSection().AddFunctionDeclaration(new otava::assembly::FunctionDeclaration(function.Name()));
    }
    else
    {
        std::string fullFunctionName = function.ResolveFullName();
        if (function.IsLinkOnce())
        {
            file.GetDeclarationSection().AddLinkOnceDeclaration(new otava::assembly::LinkOnceDeclaration(function.Name()));
        }
        else
        {
            file.GetDeclarationSection().AddPublicDeclaration(new otava::assembly::PublicDeclaration(function.Name()));
        }
        currentFunction = &function;
        context->GetAssemblyContext()->ResetRegisterPool();
        if (currentFunction->IsChildFn())
        {
            context->GetAssemblyContext()->GetRegisterPool()->MakeChildFnRegisterPool();
        }
        assemblyFunction = file.GetCodeSection().CreateFunction(function.Name());
        if (!fullFunctionName.empty())
        {
            assemblyFunction->SetComment(fullFunctionName);
        }
        context->GetAssemblyContext()->SetCurrentFunction(assemblyFunction);
        std::unique_ptr<RegisterAllocator> linearScanRregisterAllocator = CreateLinearScanRegisterAllocator(function, Ctx());
        registerAllocator = linearScanRregisterAllocator.get();
        function.VisitBasicBlocks(*this);
        assemblyFunction->SetActiveFunctionPart(otava::assembly::FunctionPart::prologue);
        EmitPrologue(*this);
        assemblyFunction->SetActiveFunctionPart(otava::assembly::FunctionPart::epilogue);
        EmitEpilogue(*this);
    }
}

void CodeGenerator::Visit(BasicBlock& basicBlock)
{
    Instruction* inst = basicBlock.FirstInstruction();
    leader = true;
    while (inst)
    {
        currentInst = inst;
        if (inst->IsArgInstruction() || inst->IsProcedureCallInstruction() || inst->IsFunctionCallInstruction())
        {
            inst->Accept(*this);
        }
        else
        {
            RegisterAllocationAction action = registerAllocator->Run(inst);
            if (action == RegisterAllocationAction::spill)
            {
                for (const SpillData& spillData : registerAllocator->GetSpillData())
                {
                    otava::assembly::Instruction* storeInst = nullptr;
                    if (spillData.registerGroupToSpill->IsFloatingPointReg())
                    {
                        storeInst = EmitFloatingPointStore(
                            8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, *this);
                    }
                    else
                    {
                        storeInst = EmitIntegerStore(
                            8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, *this);
                    }
                    Instruction* instToSpill = spillData.instToSpill;
                    otava::assembly::Instruction* assemblyInstruction = instToSpill->AssemblyInstruction();
                    if (assemblyInstruction)
                    {
                        assemblyInstruction->LinkAfter(storeInst->Clone());
                    }
                    else
                    {
                        assemblyFunction->AddInstruction(storeInst->Clone());
                    }
                    registerAllocator->RemoveFromRegisterGroups(instToSpill);
                }
            }
            inst->Accept(*this);
        }
        inst = inst->Next();
    }
}

void CodeGenerator::Visit(RetInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointRet(inst, *this);
    }
    else
    {
        EmitIntegerRet(inst, *this);
    }
}

void CodeGenerator::Visit(SignExtendInstruction& inst)
{
    EmitSignExtend(inst, *this);
}

void CodeGenerator::Visit(ZeroExtendInstruction& inst)
{
    EmitZeroExtend(inst, *this);
}

void CodeGenerator::Visit(FloatingPointExtendInstruction& inst)
{
    EmitFloatingPointExtend(inst, *this);
}

void CodeGenerator::Visit(ParamInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointParam(inst, *this);
    }
    else
    {
        EmitIntegerParam(inst, *this);
    }
}

void CodeGenerator::Visit(LoadInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointLoad(inst, *this);
    }
    else
    {
        EmitIntegerLoad(inst, *this);
    }
}

void CodeGenerator::Visit(StoreInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointStore(inst, *this);
    }
    else
    {
        EmitIntegerStore(inst, *this);
    }
}

void CodeGenerator::Visit(ArgInstruction& inst)
{
    args.push_back(&inst);
}

void CodeGenerator::Visit(ProcedureCallInstruction& inst)
{
    EmitProcedureCall(inst, args, *this);
    args.clear();
}

void CodeGenerator::Visit(FunctionCallInstruction& inst)
{
    EmitFunctionCall(inst, args, *this);
    args.clear();
}

void CodeGenerator::Visit(ElemAddrInstruction& inst)
{
    EmitElemAddr(inst, *this);
}

void CodeGenerator::Visit(PtrOffsetInstruction& inst)
{
    EmitPtrOffset(inst, *this);
}

void CodeGenerator::Visit(PtrDiffInstruction& inst)
{
    EmitPtrDiff(inst, *this);
}

void CodeGenerator::Visit(TruncateInstruction& inst)
{
    if (inst.GetType()->IsIntegerType() || inst.GetType()->IsBooleanType())
    {
        EmitIntegerTruncate(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointTruncate(inst, *this);
    }
}

void CodeGenerator::Visit(AddInstruction& inst)
{
    if (inst.GetType()->IsIntegerType())
    {
        EmitIntegerBinOpInst(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointBinOpInst(inst, *this);
    }
}

void CodeGenerator::Visit(SubInstruction& inst)
{
    if (inst.GetType()->IsIntegerType())
    {
        EmitIntegerBinOpInst(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointBinOpInst(inst, *this);
    }
}

void CodeGenerator::Visit(MulInstruction& inst)
{
    if (inst.GetType()->IsIntegerType())
    {
        EmitIntegerBinOpInst(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointBinOpInst(inst, *this);
    }
}

void CodeGenerator::Visit(DivInstruction& inst)
{
    if (inst.GetType()->IsIntegerType())
    {
        EmitIntegerBinOpInst(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointBinOpInst(inst, *this);
    }
}

void CodeGenerator::Visit(ModInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(AndInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(OrInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(XorInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(ShlInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(ShrInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(BitcastInstruction& inst)
{
    EmitBitcast(inst, *this);
}

void CodeGenerator::Visit(IntToFloatInstruction& inst)
{
    EmitIntToFloat(inst, *this);
}

void CodeGenerator::Visit(FloatToIntInstruction& inst)
{
    EmitFloatToInt(inst, *this);
}

void CodeGenerator::Visit(IntToPtrInstruction& inst)
{
    EmitIntToPtr(inst, *this);
}

void CodeGenerator::Visit(PtrToIntInstruction& inst)
{
    EmitPtrToInt(inst, *this);
}

void CodeGenerator::Visit(EqualInstruction& inst)
{
    if (inst.Left()->GetType()->IsFloatingPointType())
    {
        EmitFloatingPointEqual(inst, *this);
    }
    else
    {
        EmitIntegerEqual(inst, *this);
    }
}

void CodeGenerator::Visit(LessInstruction& inst)
{
    if (inst.Left()->GetType()->IsFloatingPointType())
    {
        EmitFloatingPointLess(inst, *this);
    }
    else
    {
        EmitIntegerLess(inst, *this);
    }
}

void CodeGenerator::Visit(JmpInstruction& inst)
{
    EmitJmp(inst, *this);
}

void CodeGenerator::Visit(BranchInstruction& inst)
{
    EmitBranch(inst, *this);
}

void CodeGenerator::Visit(NotInstruction& inst)
{
    EmitNot(inst, *this);
}

void CodeGenerator::Visit(NegInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointNeg(inst, *this);
    }
    else
    {
        EmitIntegerNeg(inst, *this);
    }
}

void CodeGenerator::Visit(NoOperationInstruction& inst)
{
    EmitNop(inst, *this);
}

void CodeGenerator::Visit(GetRbpInstruction& inst)
{
    EmitGetRbp(inst, *this);
}

void CodeGenerator::Visit(SwitchInstruction& inst)
{
    EmitSwitch(inst, *this);
}

void CodeGenerator::Visit(BoolValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value.ToInteger(), 1)), otava::assembly::OpCode::DB);
    currentOffset += 1;
}

void CodeGenerator::Visit(SByteValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value.GetValue(), 1)), otava::assembly::OpCode::DB);
    currentOffset += 1;
}

void CodeGenerator::Visit(ByteValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value.GetValue(), 1)), otava::assembly::OpCode::DB);
    currentOffset += 1;
}

void CodeGenerator::Visit(ShortValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value.GetValue(), 2)), otava::assembly::OpCode::DW);
    currentOffset += 2;
}

void CodeGenerator::Visit(UShortValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value.GetValue(), 2)), otava::assembly::OpCode::DW);
    currentOffset += 2;
}

void CodeGenerator::Visit(IntValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value.GetValue(), 4)), otava::assembly::OpCode::DD);
    currentOffset += 4;
}

void CodeGenerator::Visit(UIntValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value.GetValue(), 4)), otava::assembly::OpCode::DD);
    currentOffset += 4;
}

void CodeGenerator::Visit(LongValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value.GetValue(), 8)), otava::assembly::OpCode::DQ);
    currentOffset += 8;
}

void CodeGenerator::Visit(ULongValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value.GetValue(), 8)), otava::assembly::OpCode::DQ);
    currentOffset += 8;
}

void CodeGenerator::Visit(FloatValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::FloatLiteral(value.GetValue())), otava::assembly::OpCode::REAL4);
    currentOffset += 4;
}

void CodeGenerator::Visit(DoubleValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::DoubleLiteral(value.GetValue())), otava::assembly::OpCode::REAL8);
    currentOffset += 8;
}

void CodeGenerator::Visit(NullValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(0, 8)), otava::assembly::OpCode::DQ);
    currentOffset += 8;
}

void CodeGenerator::Visit(AddressValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::Symbol(value.GetValue()->Name())), otava::assembly::OpCode::DQ);
    currentOffset += 8;
}

void CodeGenerator::Visit(ArrayValue& value)
{
    Type* type = value.GetType();
    std::int64_t start = currentOffset;
    Value* zero = context->GetByteValue(0u);
    for (const auto& element : value.Elements())
    {
        Type* elementType = element->GetType();
        element->Accept(*this);
        if (currentOffset > 0)
        {
            std::int64_t alignment = elementType->Alignment();
            std::int64_t elementOffset = alignment * ((currentOffset - 1) / alignment + 1);
            while (currentOffset < elementOffset)
            {
                zero->Accept(*this);
            }
        }
    }
    while (currentOffset - start < type->Size())
    {
        zero->Accept(*this);
    }
}

void CodeGenerator::Visit(StructureValue& value)
{
    StructureType* structureType = nullptr;
    Type* type = value.GetType();
    if (type->IsStructureType())
    {
        structureType = static_cast<StructureType*>(type);
    }
    else
    {
        Error("structure type expected");
    }
    std::int64_t start = currentOffset;
    std::int64_t n = value.FieldValues().size();
    Value* zero = context->GetByteValue(0u);
    for (std::int64_t i = 0; i < n; ++i)
    {
        std::int64_t fieldOffset = structureType->GetFieldOffset(i);
        while (currentOffset - start < fieldOffset)
        {
            zero->Accept(*this);
        }
        Value* fieldValue = value.FieldValues()[i];
        fieldValue->Accept(*this);
    }
    while (currentOffset - start < structureType->Size())
    {
        zero->Accept(*this);
    }
}

void CodeGenerator::Visit(StringValue& value)
{
    std::string hexByteStr;
    std::string stringValue;
    int state = 0;
    std::string val = value.ToString();
    val = val.substr(1, val.length() - 2);
    for (char c : val)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '\\')
            {
                if (!stringValue.empty())
                {
                    EmitDataValue(std::unique_ptr<otava::assembly::Value>(
                        new otava::assembly::StringLiteral(stringValue)), otava::assembly::OpCode::DB);
                    stringValue.clear();
                }
                state = 1;
            }
            else
            {
                stringValue.append(1, c);
                currentOffset += 1;
            }
            break;
        }
        case 1:
        {
            hexByteStr.append(1, c);
            state = 2;
            break;
        }
        case 2:
        {
            hexByteStr.append(1, c);
            std::uint8_t value = util::ParseHexByte(hexByteStr);
            EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::IntegerLiteral(value, 1)), otava::assembly::OpCode::DB);
            currentOffset += 1;
            hexByteStr.clear();
            state = 0;
            break;
        }
        }
    }
    if (!stringValue.empty())
    {
        EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::StringLiteral(stringValue)), otava::assembly::OpCode::DB);
        stringValue.clear();
    }
}

void CodeGenerator::Visit(StringArrayValue& value)
{
    for (auto elementValue : value.Elements())
    {
        elementValue->Accept(*this);
    }
}

void CodeGenerator::Visit(ConversionValue& value)
{
    value.From()->Accept(*this);
}

void CodeGenerator::Visit(SymbolValue& value)
{
    EmitDataValue(std::unique_ptr<otava::assembly::Value>(new otava::assembly::Symbol(value.Symbol())), otava::assembly::OpCode::DQ);
    currentOffset += 8;
}

void CodeGenerator::Error(const std::string& message)
{
    otava::intermediate::Error(message, currentInst->Span(), context);
}

void CodeGenerator::WriteOutputFile(bool verbose)
{
    if (data && dataInstruction)
    {
        data->AddInstruction(std::move(dataInstruction));
    }
    file.Write();
    if (verbose)
    {
        std::cout << "==> " << file.FilePath() << "\n";
    }
}

void CodeGenerator::EmitJumpToExit(RetInstruction& retInst)
{
    int targetLabelId = ExitLabelId();
    otava::assembly::Instruction* jmpInst = new otava::assembly::Instruction(otava::assembly::OpCode::JMP);
    jmpInst->AddOperand(context->GetAssemblyContext()->MakeSymbol("@" + std::to_string(targetLabelId)));
    Emit(jmpInst);
}

void CodeGenerator::EmitBranchJumps(BranchInstruction& branchInst)
{
    otava::assembly::AssemblyContext* assemblyContext = context->GetAssemblyContext();
    otava::assembly::Instruction* jnzInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::JNZ);
    jnzInstruction->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(branchInst.TrueTargetBasicBlock()->Id())));
    Emit(jnzInstruction);
    otava::assembly::Instruction* jmpInstruction = new otava::assembly::Instruction(otava::assembly::OpCode::JMP);
    jmpInstruction->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(branchInst.FalseTargetBasicBlock()->Id())));
    Emit(jmpInstruction);
}

void GenerateCode(IntermediateContext& context, CodeGenerator& codeGenerator, bool verbose)
{
    codeGenerator.Ctx()->GetData().VisitGlobalVariables(codeGenerator);
    codeGenerator.Ctx()->GetCode().VisitFunctions(codeGenerator);
    codeGenerator.WriteOutputFile(verbose);
}

} // otava::intermediate
