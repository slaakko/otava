module otava.intermediate.code;

import otava.intermediate.context;
import otava.intermediate.visitor;
import otava.intermediate.error;
import otava.intermediate.util;
import util;

namespace otava::intermediate {

RegValue::RegValue(const soul::ast::Span& span_, Type* type_, std::int32_t reg_) noexcept : Value(span_, ValueKind::regValue, type_), reg(reg_), inst(nullptr)
{
}

RegValue::~RegValue()
{
    if (inst && inst->IsValueInstruction())
    {
        ValueInstruction* valueInst = static_cast<ValueInstruction*>(inst);
        valueInst->ResetResult();
    }
}

std::string RegValue::ToString() const
{
    if (inst)
    {
        return "$" + std::to_string(inst->RegValueIndex());
    }
    else
    {
        return std::to_string(reg) + " is NULL!";
    }
}

Value* RegValue::Clone(CloneContext& cloneContext) const
{
    RegValue* regValue = cloneContext.CurrentFunction()->GetRegValue(reg);
    if (regValue)
    {
        return regValue;
    }
    RegValue* clone = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), reg, cloneContext.GetContext());
    Instruction* mappedInst = cloneContext.GetMappedInstruction(inst);
    if (mappedInst)
    {
        clone->SetInst(mappedInst);
    }
    else
    {
        cloneContext.AddUnmappedInstruction(inst, clone);
    }
    return clone;
}

void AddUser(Instruction* user, Value* value)
{
    if (value)
    {
        if (value->IsRegValue())
        {
            RegValue* regValue = static_cast<RegValue*>(value);
            if (regValue->Inst())
            {
                regValue->Inst()->AddUser(user);
            }
        }
    }
}

void RemoveUser(Instruction* user, Value* value)
{
    if (value)
    {
        if (value->IsRegValue())
        {
            RegValue* regValue = static_cast<RegValue*>(value);
            if (regValue->Inst())
            {
                regValue->Inst()->RemoveUser(user);
            }
        }
    }
}

void AddToUsesVec(std::vector<Instruction*>& uses, Value* value)
{
    if (value)
    {
        if (value->IsRegValue())
        {
            RegValue* regValue = static_cast<RegValue*>(value);
            if (regValue->Inst())
            {
                uses.push_back(regValue->Inst());
            }
        }
    }
}

Instruction::Instruction(const soul::ast::Span& span_, Type* type_, OpCode opCode_) noexcept :
    Value(span_, ValueKind::instruction, type_), opCode(opCode_), metadataRef(nullptr), index(-1), regValueIndex(-1), assemblyInstruction(nullptr)
{
}

void Instruction::Check()
{
    for (Instruction* user : users)
    {
        if (user->opCode < OpCode::store || user->opCode > OpCode::nop)
        {
            SetExceptionThrown();
            throw std::runtime_error("invalid instruction");
        }
    }
}

std::string Instruction::Name() const
{
    switch (opCode)
    {
    case OpCode::store: return "store";
    case OpCode::arg: return "arg";
    case OpCode::jmp: return "jmp";
    case OpCode::branch: return "branch";
    case OpCode::procedure_call: return "call";
    case OpCode::ret: return "ret";
    case OpCode::switch_: return "switch";
    case OpCode::not_: return "not";
    case OpCode::neg: return "neg";
    case OpCode::signextend: return "signextend";
    case OpCode::zeroextend: return "zeroextend";
    case OpCode::truncate: return "truncate";
    case OpCode::fpextend: return "fpextend";
    case OpCode::bitcast: return "bitcast";
    case OpCode::inttofloat: return "inttofloat";
    case OpCode::floattoint: return "floattoint";
    case OpCode::inttoptr: return "inttoptr";
    case OpCode::ptrtoint: return "ptrtoint";
    case OpCode::add: return "add";
    case OpCode::sub: return "sub";
    case OpCode::mul: return "mul";
    case OpCode::div_: return "div";
    case OpCode::mod: return "mod";
    case OpCode::and_: return "and";
    case OpCode::or_: return "or_";
    case OpCode::xor_: return "xor";
    case OpCode::shl: return "shl";
    case OpCode::shr: return "shr";
    case OpCode::equal: return "equal";
    case OpCode::less: return "less";
    case OpCode::param: return "param";
    case OpCode::local: return "local";
    case OpCode::plocal: return "plocal";
    case OpCode::load: return "load";
    case OpCode::elemaddr: return "elemaddr";
    case OpCode::ptroffset: return "ptroffset";
    case OpCode::ptrdiff: return "ptrdiff";
    case OpCode::function_call: return "call";
    case OpCode::nop: return "nop";
    case OpCode::getrbp: return "getrbp";
    }
    return "<inst>";
}

bool Instruction::IsLeader() const noexcept
{
    BasicBlock* basicBlock = Parent();
    return this == basicBlock->Leader();
}

bool Instruction::IsTerminator() const noexcept
{
    switch (opCode)
    {
    case OpCode::jmp:
    case OpCode::branch:
    case OpCode::ret:
    case OpCode::switch_:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

bool Instruction::IsValueInstruction() const noexcept
{
    switch (opCode)
    {
    case OpCode::not_:
    case OpCode::neg:
    case OpCode::signextend:
    case OpCode::zeroextend:
    case OpCode::fpextend:
    case OpCode::truncate:
    case OpCode::bitcast:
    case OpCode::inttofloat:
    case OpCode::floattoint:
    case OpCode::inttoptr:
    case OpCode::ptrtoint:
    case OpCode::add:
    case OpCode::sub:
    case OpCode::mul:
    case OpCode::div_:
    case OpCode::mod:
    case OpCode::and_:
    case OpCode::or_:
    case OpCode::xor_:
    case OpCode::shl:
    case OpCode::shr:
    case OpCode::equal:
    case OpCode::less:
    case OpCode::param:
    case OpCode::local:
    case OpCode::plocal:
    case OpCode::load:
    case OpCode::elemaddr:
    case OpCode::ptroffset:
    case OpCode::ptrdiff:
    case OpCode::function_call:
    case OpCode::getrbp:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

bool Instruction::IsUnaryInstruction() const noexcept
{
    switch (opCode)
    {
    case OpCode::not_:
    case OpCode::neg:
    case OpCode::signextend:
    case OpCode::zeroextend:
    case OpCode::fpextend:
    case OpCode::truncate:
    case OpCode::bitcast:
    case OpCode::inttofloat:
    case OpCode::floattoint:
    case OpCode::inttoptr:
    case OpCode::ptrtoint:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

bool Instruction::IsBinaryInstruction() const noexcept
{
    switch (opCode)
    {
    case OpCode::add:
    case OpCode::sub:
    case OpCode::mul:
    case OpCode::div_:
    case OpCode::mod:
    case OpCode::and_:
    case OpCode::or_:
    case OpCode::xor_:
    case OpCode::shl:
    case OpCode::shr:
    case OpCode::equal:
    case OpCode::less:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

bool Instruction::RequiresLocalRegister() const noexcept
{
    switch (opCode)
    {
    case OpCode::arg:
    case OpCode::procedure_call:
    case OpCode::not_:
    case OpCode::neg:
    case OpCode::signextend:
    case OpCode::zeroextend:
    case OpCode::fpextend:
    case OpCode::truncate:
    case OpCode::bitcast:
    case OpCode::inttofloat:
    case OpCode::floattoint:
    case OpCode::inttoptr:
    case OpCode::ptrtoint:
    case OpCode::add:
    case OpCode::sub:
    case OpCode::mul:
    case OpCode::div_:
    case OpCode::mod:
    case OpCode::and_:
    case OpCode::or_:
    case OpCode::xor_:
    case OpCode::shl:
    case OpCode::shr:
    case OpCode::equal:
    case OpCode::less:
    case OpCode::param:
    case OpCode::load:
    case OpCode::elemaddr:
    case OpCode::ptroffset:
    case OpCode::ptrdiff:
    case OpCode::function_call:
    case OpCode::getrbp:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

std::vector<BasicBlock*> Instruction::Successors() const
{
    std::vector<BasicBlock*> successors;
    switch (opCode)
    {
    case OpCode::jmp:
    {
        const JmpInstruction* jmp = static_cast<const JmpInstruction*>(this);
        AddPtrToSet(jmp->TargetBasicBlock(), successors);
        break;
    }
    case OpCode::branch:
    {
        const BranchInstruction* branch = static_cast<const BranchInstruction*>(this);
        AddPtrToSet(branch->TrueTargetBasicBlock(), successors);
        AddPtrToSet(branch->FalseTargetBasicBlock(), successors);
        break;
    }
    case OpCode::switch_:
    {
        const SwitchInstruction* switch_ = static_cast<const SwitchInstruction*>(this);
        AddPtrToSet(switch_->DefaultTargetBlock(), successors);
        for (const CaseTarget& caseTarget : switch_->CaseTargets())
        {
            AddPtrToSet(caseTarget.targetBlock, successors);
        }
        break;
    }
    }
    return successors;
}

BasicBlock* Instruction::Parent() const noexcept
{
    return static_cast<BasicBlock*>(GetContainer()->Parent());
}

void Instruction::AddUser(Instruction* user)
{
    AddPtrToSet(user, users);
}

void Instruction::RemoveUser(Instruction* user)
{
    RemovePtrFromSet(user, users);
}

void Instruction::AddToUses()
{
}

void Instruction::ReplaceUsesWith(Value* value)
{
    Function* fn = Parent()->Parent();
    RegValue* use = fn->GetRegValue(RegValueIndex());
    if (use)
    {
        std::vector<Instruction*> copiedUsers = users;
        for (auto* user : copiedUsers)
        {
            user->ReplaceValue(use, value);
        }
    }
    else
    {
        Code* code = fn->Parent();
        IntermediateContext* context = code->GetContext();
        Error("reg value " + std::to_string(RegValueIndex()) + "not found", Span(), context);
    }
}

void Instruction::RemoveFromUses()
{
    std::vector<Instruction*> uses = Uses();
    for (Instruction* use : uses)
    {
        use->RemoveUser(this);
    }
}

void Instruction::ReplaceValue(Value* use, Value* value)
{
}

bool Instruction::IsRetVoid() const noexcept
{
    if (IsRetInstruction())
    {
        const RetInstruction* retInst = static_cast<const RetInstruction*>(this);
        return retInst->ReturnValue() == nullptr;
    }
    return false;
}

StoreInstruction::StoreInstruction(const soul::ast::Span& span_, Value* value_, Value* ptr_) noexcept :
    Instruction(span_, nullptr, OpCode::store), value(value_), ptr(ptr_)
{
}

void StoreInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, value);
    otava::intermediate::AddUser(this, ptr);
}

void StoreInstruction::ReplaceValue(Value* use, Value* value)
{
    if (this->value == use)
    {
        otava::intermediate::RemoveUser(this, this->value);
        this->value = value;
        otava::intermediate::AddUser(this, this->value);
    }
    if (ptr == use)
    {
        otava::intermediate::RemoveUser(this, ptr);
        ptr = value;
        otava::intermediate::AddUser(this, ptr);
    }
}

std::vector<Instruction*> StoreInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, value);
    AddToUsesVec(uses, ptr);
    return uses;
}

void StoreInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* StoreInstruction::Clone(CloneContext& cloneContext) const
{
    StoreInstruction* clone = new StoreInstruction(Span(), value->Clone(cloneContext), ptr->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    clone->AddToUses();
    return clone;
}

void StoreInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("store ", 8));
    formatter.Write(value->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(value->ToString());
    formatter.Write(", ");
    formatter.Write(ptr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(ptr->ToString());
}

ArgInstruction::ArgInstruction(const soul::ast::Span& span_, Value* arg_) noexcept : Instruction(span_, nullptr, OpCode::arg), arg(arg_)
{
}

void ArgInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* ArgInstruction::Clone(CloneContext& cloneContext) const
{
    ArgInstruction* clone = new ArgInstruction(Span(), arg->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    clone->AddToUses();
    return clone;
}

void ArgInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, arg);
}

void ArgInstruction::ReplaceValue(Value* use, Value* value)
{
    if (arg == use)
    {
        otava::intermediate::RemoveUser(this, arg);
        arg = value;
        otava::intermediate::AddUser(this, arg);
    }
}

std::vector<Instruction*> ArgInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, arg);
    return uses;
}

void ArgInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("arg ", 8));
    formatter.Write(arg->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(arg->ToString());
}

JmpInstruction::JmpInstruction(const soul::ast::Span& span_, std::int32_t targetLabelId_) noexcept :
    Instruction(span_, nullptr, OpCode::jmp), targetLabelId(targetLabelId_), targetBasicBlock(nullptr)
{
}

void JmpInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* JmpInstruction::Clone(CloneContext& cloneContext) const
{
    JmpInstruction* clone = new JmpInstruction(Span(), targetBasicBlock->Id());
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    BasicBlock* bb = cloneContext.GetMappedBasicBlock(targetBasicBlock);
    if (bb)
    {
        clone->SetTargetBasicBlock(bb);
    }
    else
    {
        Error("mapped basic block not found", Span(), cloneContext.GetContext());
    }
    clone->AddToUses();
    return clone;
}

void JmpInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("jmp ", 8));
    formatter.Write("@" + std::to_string(targetBasicBlock->Id()));
}

BranchInstruction::BranchInstruction(const soul::ast::Span& span_, Value* cond_, std::int32_t trueTargetLabelId_, std::int32_t falseTargetLabelId_) noexcept :
    Instruction(span_, nullptr, OpCode::branch),
    cond(cond_), trueTargetLabelId(trueTargetLabelId_), trueTargetBasicBlock(nullptr), falseTargetLabelId(falseTargetLabelId_), falseTargetBasicBlock(nullptr)
{
}

void BranchInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* BranchInstruction::Clone(CloneContext& cloneContext) const
{
    BranchInstruction* clone = new BranchInstruction(Span(), cond->Clone(cloneContext), trueTargetBasicBlock->Id(), falseTargetBasicBlock->Id());
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    BasicBlock* trueBB = cloneContext.GetMappedBasicBlock(trueTargetBasicBlock);
    if (trueBB)
    {
        clone->SetTrueTargetBasicBlock(trueBB);
    }
    else
    {
        Error("mapped basic block not found", trueTargetBasicBlock->Span(), cloneContext.GetContext());
    }
    BasicBlock* falseBB = cloneContext.GetMappedBasicBlock(falseTargetBasicBlock);
    if (falseBB)
    {
        clone->SetFalseTargetBasicBlock(falseBB);
    }
    else
    {
        Error("mapped basic block not found", falseTargetBasicBlock->Span(), cloneContext.GetContext());
    }
    clone->AddToUses();
    return clone;
}

void BranchInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, cond);
}

void BranchInstruction::ReplaceValue(Value* use, Value* value)
{
    if (cond == use)
    {
        otava::intermediate::RemoveUser(this, cond);
        cond = value;
        otava::intermediate::AddUser(this, cond);
    }
}

std::vector<Instruction*> BranchInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, cond);
    return uses;
}

void BranchInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("branch ", 8));
    formatter.Write(cond->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(cond->ToString());
    formatter.Write(", ");
    formatter.Write("@" + std::to_string(trueTargetBasicBlock->Id()));
    formatter.Write(", ");
    formatter.Write("@" + std::to_string(falseTargetBasicBlock->Id()));
}

ProcedureCallInstruction::ProcedureCallInstruction(const soul::ast::Span& span_, Value* callee_) noexcept :
    Instruction(span_, nullptr, OpCode::procedure_call), callee(callee_)
{
}

void ProcedureCallInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* ProcedureCallInstruction::Clone(CloneContext& cloneContext) const
{
    ProcedureCallInstruction* clone = new ProcedureCallInstruction(Span(), callee->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    std::vector<Value*> clonedArgs;
    for (auto* arg : args)
    {
        clonedArgs.push_back(arg->Clone(cloneContext));
    }
    clone->SetArgs(std::move(clonedArgs));
    clone->AddToUses();
    return clone;
}

void ProcedureCallInstruction::SetArgs(std::vector<Value*>&& args_)
{
    args = std::move(args_);
}

Function* ProcedureCallInstruction::CalleeFn() const noexcept
{
    if (callee->IsSymbolValue())
    {
        SymbolValue* symbolValue = static_cast<SymbolValue*>(callee);
        return symbolValue->GetFunction();
    }
    return nullptr;
}

void ProcedureCallInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, callee);
    for (auto* arg : args)
    {
        otava::intermediate::AddUser(this, arg);
    }
}

void ProcedureCallInstruction::ReplaceValue(Value* use, Value* value)
{
    if (callee == use)
    {
        otava::intermediate::RemoveUser(this, callee);
        callee = value;
        otava::intermediate::AddUser(this, callee);
    }
    for (auto* arg : args)
    {
        if (arg == use)
        {
            otava::intermediate::RemoveUser(this, arg);
            arg = value;
            otava::intermediate::AddUser(this, arg);
        }
    }
}

std::vector<Instruction*> ProcedureCallInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, callee);
    for (auto* arg : args)
    {
        AddToUsesVec(uses, arg);
    }
    return uses;
}

void ProcedureCallInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("call ", 8));
    formatter.Write(callee->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(callee->ToString());
}

RetInstruction::RetInstruction(const soul::ast::Span& span_, Value* returnValue_) noexcept :
    Instruction(span_, nullptr, OpCode::ret), returnValue(returnValue_)
{
}

void RetInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* RetInstruction::Clone(CloneContext& cloneContext) const
{
    Value* clonedReturnValue = nullptr;
    if (returnValue)
    {
        clonedReturnValue = returnValue->Clone(cloneContext);
    }
    RetInstruction* clone = new RetInstruction(Span(), clonedReturnValue);
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    clone->AddToUses();
    return clone;
}

void RetInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("ret ", 8));
    if (returnValue)
    {
        formatter.Write(returnValue->GetType()->Name());
        formatter.Write(" ");
        formatter.Write(returnValue->ToString());
    }
    else
    {
        formatter.Write("void");
    }
}

bool RetInstruction::IsFloatingPointInstruction() const noexcept
{
    if (returnValue)
    {
        return returnValue->GetType()->IsFloatingPointType();
    }
    return false;
}

void RetInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, returnValue);
}

void RetInstruction::ReplaceValue(Value* use, Value* value)
{
    if (returnValue == use)
    {
        otava::intermediate::RemoveUser(this, returnValue);
        returnValue = value;
        otava::intermediate::AddUser(this, returnValue);
    }
}

std::vector<Instruction*> RetInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    if (returnValue)
    {
        AddToUsesVec(uses, returnValue);
    }
    return uses;
}

CaseTarget::CaseTarget() noexcept : caseValue(nullptr), targetLabelId(-1), targetBlock(nullptr)
{
}

SwitchInstruction::SwitchInstruction(const soul::ast::Span& span_, Value* cond_, std::int32_t defaultTargetLabelId_) noexcept :
    Instruction(span_, nullptr, OpCode::switch_), cond(cond_), defaultTargetLabelId(defaultTargetLabelId_), defaultTargetBlock(nullptr)
{
}

void SwitchInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* SwitchInstruction::Clone(CloneContext& cloneContext) const
{
    SwitchInstruction* clone = new SwitchInstruction(Span(), cond->Clone(cloneContext), defaultTargetBlock->Id());
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    BasicBlock* bb = cloneContext.GetMappedBasicBlock(defaultTargetBlock);
    if (bb)
    {
        clone->SetDefaultTargetBlock(bb);
    }
    else
    {
        Error("mapped basic block not found", defaultTargetBlock->Span(), cloneContext.GetContext());
    }
    for (const auto& caseTarget : caseTargets)
    {
        CaseTarget clonedCaseTarget;
        clonedCaseTarget.caseValue = caseTarget.caseValue->Clone(cloneContext);
        BasicBlock* bb = cloneContext.GetMappedBasicBlock(caseTarget.targetBlock);
        if (bb)
        {
            clonedCaseTarget.targetBlock = bb;
        }
        else
        {
            Error("mapped basic block not found", caseTarget.targetBlock->Span(), cloneContext.GetContext());
        }
        clone->AddCaseTarget(clonedCaseTarget);
    }
    clone->AddToUses();
    return clone;
}

void SwitchInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("switch ", 8));
    formatter.Write(cond->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(cond->ToString());
    formatter.Write(" ");
    formatter.Write("@" + std::to_string(defaultTargetBlock->Id()));
    formatter.Write(", [");
    bool first = true;
    for (const auto& p : caseTargets)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(" : ");
        }
        Value* value = p.caseValue;
        BasicBlock* dest = p.targetBlock;
        formatter.Write(value->GetType()->Name());
        formatter.Write(" ");
        formatter.Write(value->ToString());
        formatter.Write(", ");
        formatter.Write("@" + std::to_string(dest->Id()));
    }
    formatter.Write("]");
}

void SwitchInstruction::AddCaseTarget(const CaseTarget& caseTarget)
{
    caseTargets.push_back(caseTarget);
}

void SwitchInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, cond);
    for (auto& caseTarget : caseTargets)
    {
        otava::intermediate::AddUser(this, caseTarget.caseValue);
    }
}

void SwitchInstruction::ReplaceValue(Value* use, Value* value)
{
    if (cond == use)
    {
        otava::intermediate::RemoveUser(this, cond);
        cond = value;
        otava::intermediate::AddUser(this, cond);
    }
    for (auto& caseTarget : caseTargets)
    {
        if (caseTarget.caseValue == use)
        {
            otava::intermediate::RemoveUser(this, caseTarget.caseValue);
            caseTarget.caseValue = value;
            otava::intermediate::AddUser(this, caseTarget.caseValue);
        }
    }
}

std::vector<Instruction*> SwitchInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, cond);
    for (const CaseTarget& caseTarget : caseTargets)
    {
        AddToUsesVec(uses, caseTarget.caseValue);
    }
    return uses;
}

ValueInstruction::ValueInstruction(const soul::ast::Span& span_, RegValue* result_, OpCode opCode_) noexcept :
    Instruction(span_, result_->GetType(), opCode_), result(result_)
{
    result->SetInst(this);
}

ValueInstruction::~ValueInstruction()
{
    if (result)
    {
        result->SetInst(nullptr);
    }
}

void ValueInstruction::WriteResult(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format(result->GetType()->Name(), 7, util::FormatWidth::min));
    formatter.Write(" " + result->ToString());
}

UnaryInstruction::UnaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_, OpCode opCode_) noexcept :
    ValueInstruction(span_, result_, opCode_), operand(operand_)
{
}

void UnaryInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, operand);
}

void UnaryInstruction::ReplaceValue(Value* use, Value* value)
{
    if (operand == use)
    {
        otava::intermediate::RemoveUser(this, operand);
        operand = value;
        otava::intermediate::AddUser(this, operand);
    }
}

std::vector<Instruction*> UnaryInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, operand);
    return uses;
}

void UnaryInstruction::WriteArg(util::CodeFormatter& formatter)
{
    formatter.Write(operand->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(operand->ToString());
}

NotInstruction::NotInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::not_)
{
}

void NotInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* NotInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    NotInstruction* clone = new NotInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void NotInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = not ");
    WriteArg(formatter);
}

NegInstruction::NegInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::neg)
{
}

void NegInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* NegInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    NegInstruction* clone = new NegInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void NegInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = neg ");
    WriteArg(formatter);
}

SignExtendInstruction::SignExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::signextend)
{
}

void SignExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* SignExtendInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    SignExtendInstruction* clone = new SignExtendInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void SignExtendInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = signextend ");
    WriteArg(formatter);
}

ZeroExtendInstruction::ZeroExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::zeroextend)
{
}

void ZeroExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* ZeroExtendInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ZeroExtendInstruction* clone = new ZeroExtendInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ZeroExtendInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = zeroextend ");
    WriteArg(formatter);
}

FloatingPointExtendInstruction::FloatingPointExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::fpextend)
{
}

void FloatingPointExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* FloatingPointExtendInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    FloatingPointExtendInstruction* clone = new FloatingPointExtendInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void FloatingPointExtendInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = fpextend ");
    WriteArg(formatter);
}

TruncateInstruction::TruncateInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::truncate)
{
}

void TruncateInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* TruncateInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    TruncateInstruction* clone = new TruncateInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void TruncateInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = truncate ");
    WriteArg(formatter);
}

BitcastInstruction::BitcastInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::bitcast)
{
}

void BitcastInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* BitcastInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    BitcastInstruction* clone = new BitcastInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void BitcastInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = bitcast ");
    WriteArg(formatter);
}

IntToFloatInstruction::IntToFloatInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::inttofloat)
{
}

void IntToFloatInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* IntToFloatInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    IntToFloatInstruction* clone = new IntToFloatInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void IntToFloatInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = inttofloat ");
    WriteArg(formatter);
}

FloatToIntInstruction::FloatToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::floattoint)
{
}

void FloatToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* FloatToIntInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    FloatToIntInstruction* clone = new FloatToIntInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void FloatToIntInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = floattoint ");
    WriteArg(formatter);
}

IntToPtrInstruction::IntToPtrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::inttoptr)
{
}

void IntToPtrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* IntToPtrInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    IntToPtrInstruction* clone = new IntToPtrInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void IntToPtrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = inttoptr ");
    WriteArg(formatter);
}

PtrToIntInstruction::PtrToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept :
    UnaryInstruction(span_, result_, operand_, OpCode::ptrtoint)
{
}

void PtrToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* PtrToIntInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    PtrToIntInstruction* clone = new PtrToIntInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void PtrToIntInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = ptrtoint ");
    WriteArg(formatter);
}

BinaryInstruction::BinaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_, OpCode opCode_) noexcept :
    ValueInstruction(span_, result_, opCode_), left(left_), right(right_)
{
}

void BinaryInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, left);
    otava::intermediate::AddUser(this, right);
}

void BinaryInstruction::ReplaceValue(Value* use, Value* value)
{
    if (left == use)
    {
        otava::intermediate::RemoveUser(this, left);
        left = value;
        otava::intermediate::AddUser(this, left);
    }
    if (right == use)
    {
        otava::intermediate::RemoveUser(this, right);
        right = value;
        otava::intermediate::AddUser(this, right);
    }
}

std::vector<Instruction*> BinaryInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, left);
    AddToUsesVec(uses, right);
    return uses;
}

void BinaryInstruction::WriteArgs(util::CodeFormatter& formatter)
{
    formatter.Write(left->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(left->ToString());
    formatter.Write(", ");
    formatter.Write(right->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(right->ToString());
}


AddInstruction::AddInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::add)
{
}

void AddInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* AddInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    AddInstruction* clone = new AddInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void AddInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = add ");
    WriteArgs(formatter);
}

SubInstruction::SubInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::sub)
{
}

void SubInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* SubInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    SubInstruction* clone = new SubInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void SubInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = sub ");
    WriteArgs(formatter);
}

MulInstruction::MulInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::mul)
{
}

void MulInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* MulInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    MulInstruction* clone = new MulInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void MulInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = mul ");
    WriteArgs(formatter);
}

DivInstruction::DivInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::div_)
{
}

void DivInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* DivInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    DivInstruction* clone = new DivInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void DivInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = div ");
    WriteArgs(formatter);
}

ModInstruction::ModInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::mod)
{
}

void ModInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* ModInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ModInstruction* clone = new ModInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ModInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = mod ");
    WriteArgs(formatter);
}

AndInstruction::AndInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::and_)
{
}

void AndInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* AndInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    AndInstruction* clone = new AndInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void AndInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = and ");
    WriteArgs(formatter);
}

OrInstruction::OrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::or_)
{
}

void OrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* OrInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    OrInstruction* clone = new OrInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void OrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = or ");
    WriteArgs(formatter);
}

XorInstruction::XorInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::xor_)
{
}

void XorInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* XorInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    XorInstruction* clone = new XorInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void XorInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = xor ");
    WriteArgs(formatter);
}

ShlInstruction::ShlInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::shl)
{
}

void ShlInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* ShlInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ShlInstruction* clone = new ShlInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ShlInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = shl ");
    WriteArgs(formatter);
}

ShrInstruction::ShrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::shr)
{
}

void ShrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* ShrInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ShrInstruction* clone = new ShrInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ShrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = shr ");
    WriteArgs(formatter);
}

EqualInstruction::EqualInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::equal)
{
}

void EqualInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* EqualInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    EqualInstruction* clone = new EqualInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void EqualInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = equal ");
    WriteArgs(formatter);
}

LessInstruction::LessInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept :
    BinaryInstruction(span_, result_, left_, right_, OpCode::less)
{
}

void LessInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* LessInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    LessInstruction* clone = new LessInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void LessInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = less ");
    WriteArgs(formatter);
}

ParamInstruction::ParamInstruction(const soul::ast::Span& span_, RegValue* result_) noexcept :
    ValueInstruction(span_, result_, OpCode::param)
{
}

void ParamInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* ParamInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ParamInstruction* clone = new ParamInstruction(Span(), result);
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ParamInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = param");
}

LocalInstruction::LocalInstruction(const soul::ast::Span& span_, RegValue* result_, Type* localType_) noexcept :
    ValueInstruction(span_, result_, OpCode::local), localType(localType_)
{
}

void LocalInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* LocalInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    LocalInstruction* clone = new LocalInstruction(Span(), result, localType);
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void LocalInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = local ");
    formatter.Write(localType->Name());
}

PLocalInstruction::PLocalInstruction(const soul::ast::Span& span_, RegValue* result_, Type* localType_, int level_) noexcept :
    ValueInstruction(span_, result_, OpCode::plocal), localType(localType_), level(level_)
{
}

void PLocalInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* PLocalInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    PLocalInstruction* clone = new PLocalInstruction(Span(), result, localType, level);
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void PLocalInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = plocal ");
    formatter.Write(localType->Name());
    formatter.Write(", " + std::to_string(level));
}

LoadInstruction::LoadInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_) noexcept :
    ValueInstruction(span_, result_, OpCode::load), ptr(ptr_)
{
}

void LoadInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* LoadInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    LoadInstruction* clone = new LoadInstruction(Span(), result, ptr->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void LoadInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = load ");
    formatter.Write(ptr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(ptr->ToString());
}

void LoadInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, ptr);
}

void LoadInstruction::ReplaceValue(Value* use, Value* value)
{
    if (ptr == use)
    {
        otava::intermediate::RemoveUser(this, ptr);
        ptr = value;
        otava::intermediate::AddUser(this, ptr);
    }
}

std::vector<Instruction*> LoadInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, ptr);
    return uses;
}

ElemAddrInstruction::ElemAddrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* indexValue_) noexcept :
    ValueInstruction(span_, result_, OpCode::elemaddr), ptr(ptr_), indexValue(indexValue_)
{
}

void ElemAddrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* ElemAddrInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ElemAddrInstruction* clone = new ElemAddrInstruction(Span(), result, Ptr()->Clone(cloneContext), IndexValue()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ElemAddrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = elemaddr ");
    formatter.Write(ptr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(ptr->ToString());
    formatter.Write(", ");
    formatter.Write(indexValue->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(indexValue->ToString());
}

ElemAddrKind ElemAddrInstruction::GetElemAddrKind(IntermediateContext* context) const
{
    if (ptr->GetType()->IsPointerType())
    {
        PointerType* ptrType = static_cast<PointerType*>(ptr->GetType());
        Type* pointeeType = ptrType->RemovePointer(Span(), context);
        if (pointeeType->IsArrayType())
        {
            return ElemAddrKind::array;
        }
        else if (pointeeType->IsStructureType())
        {
            return ElemAddrKind::structure;
        }
    }
    Error("invalid elem addr kind", Span(), context);
    return ElemAddrKind::none;
}

void ElemAddrInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, ptr);
    otava::intermediate::AddUser(this, indexValue);
}

void ElemAddrInstruction::ReplaceValue(Value* use, Value* value)
{
    if (ptr == use)
    {
        otava::intermediate::RemoveUser(this, ptr);
        ptr = value;
        otava::intermediate::AddUser(this, ptr);
    }
    if (indexValue == use)
    {
        otava::intermediate::RemoveUser(this, indexValue);
        indexValue = value;
        otava::intermediate::AddUser(this, indexValue);
    }
}

std::vector<Instruction*> ElemAddrInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, ptr);
    AddToUsesVec(uses, indexValue);
    return uses;
}

PtrOffsetInstruction::PtrOffsetInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* offset_) noexcept :
    ValueInstruction(span_, result_, OpCode::ptroffset), ptr(ptr_), offset(offset_)
{
}

void PtrOffsetInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* PtrOffsetInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    PtrOffsetInstruction* clone = new PtrOffsetInstruction(Span(), result, Ptr()->Clone(cloneContext), offset->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void PtrOffsetInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = ptroffset ");
    formatter.Write(ptr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(ptr->ToString());
    formatter.Write(", ");
    formatter.Write(offset->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(offset->ToString());
}

void PtrOffsetInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, ptr);
    otava::intermediate::AddUser(this, offset);
}

void PtrOffsetInstruction::ReplaceValue(Value* use, Value* value)
{
    if (ptr == use)
    {
        otava::intermediate::RemoveUser(this, ptr);
        ptr = value;
        otava::intermediate::AddUser(this, ptr);
    }
    if (offset == use)
    {
        otava::intermediate::RemoveUser(this, offset);
        offset = value;
        otava::intermediate::AddUser(this, offset);
    }
}

std::vector<Instruction*> PtrOffsetInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, ptr);
    AddToUsesVec(uses, offset);
    return uses;
}

PtrDiffInstruction::PtrDiffInstruction(const soul::ast::Span& span_, RegValue* result_, Value* leftPtr_, Value* rightPtr_) noexcept :
    ValueInstruction(span_, result_, OpCode::ptrdiff), leftPtr(leftPtr_), rightPtr(rightPtr_)
{
}

void PtrDiffInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* PtrDiffInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    PtrDiffInstruction* clone = new PtrDiffInstruction(Span(), result, leftPtr->Clone(cloneContext), rightPtr->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void PtrDiffInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = ptrdiff ");
    formatter.Write(leftPtr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(leftPtr->ToString());
    formatter.Write(", ");
    formatter.Write(rightPtr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(rightPtr->ToString());
}

void PtrDiffInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, leftPtr);
    otava::intermediate::AddUser(this, rightPtr);
}

void PtrDiffInstruction::ReplaceValue(Value* use, Value* value)
{
    if (leftPtr == use)
    {
        otava::intermediate::RemoveUser(this, leftPtr);
        leftPtr = value;
        otava::intermediate::AddUser(this, leftPtr);
    }
    if (rightPtr == use)
    {
        otava::intermediate::RemoveUser(this, rightPtr);
        rightPtr = value;
        otava::intermediate::AddUser(this, rightPtr);
    }
}

std::vector<Instruction*> PtrDiffInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, leftPtr);
    AddToUsesVec(uses, rightPtr);
    return uses;
}

FunctionCallInstruction::FunctionCallInstruction(const soul::ast::Span& span_, RegValue* result_, Value* callee_) noexcept :
    ValueInstruction(span_, result_, OpCode::function_call), callee(callee_)
{
}

void FunctionCallInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* FunctionCallInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    FunctionCallInstruction* clone = new FunctionCallInstruction(Span(), result, callee->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    std::vector<Value*> clonedArgs;
    for (auto* arg : args)
    {
        clonedArgs.push_back(arg->Clone(cloneContext));
    }
    clone->SetArgs(std::move(clonedArgs));
    clone->AddToUses();
    return clone;
}

void FunctionCallInstruction::SetArgs(std::vector<Value*>&& args_)
{
    args = std::move(args_);
}

Function* FunctionCallInstruction::CalleeFn() const noexcept
{
    if (callee->IsSymbolValue())
    {
        SymbolValue* symbolValue = static_cast<SymbolValue*>(callee);
        return symbolValue->GetFunction();
    }
    return nullptr;
}

void FunctionCallInstruction::AddToUses()
{
    otava::intermediate::AddUser(this, callee);
    for (auto* arg : args)
    {
        otava::intermediate::AddUser(this, arg);
    }
}

void FunctionCallInstruction::ReplaceValue(Value* use, Value* value)
{
    if (callee == use)
    {
        otava::intermediate::RemoveUser(this, callee);
        callee = value;
        otava::intermediate::AddUser(this, callee);
    }
    for (auto* arg : args)
    {
        if (arg == use)
        {
            otava::intermediate::RemoveUser(this, arg);
            arg = value;
            otava::intermediate::AddUser(this, arg);
        }
    }
}

void FunctionCallInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = call ");
    formatter.Write(callee->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(callee->ToString());
}

std::vector<Instruction*> FunctionCallInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, callee);
    for (auto* arg : args)
    {
        AddToUsesVec(uses, arg);
    }
    return uses;
}

BlockValue::BlockValue(Value* value_, BasicBlock* block_) noexcept : value(value_), blockId(block_->Id()), block(block_)
{
}

NoOperationInstruction::NoOperationInstruction(const soul::ast::Span& span_) noexcept :
    Instruction(span_, nullptr, OpCode::nop)
{
}

void NoOperationInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* NoOperationInstruction::Clone(CloneContext& cloneContext) const
{
    NoOperationInstruction* clone = new NoOperationInstruction(Span());
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    clone->AddToUses();
    return clone;
}

void NoOperationInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write("nop");
}

GetRbpInstruction::GetRbpInstruction(const soul::ast::Span& span_, RegValue* result_) noexcept :
    ValueInstruction(span_, result_, OpCode::getrbp)
{
}

void GetRbpInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Value* GetRbpInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    GetRbpInstruction* clone = new GetRbpInstruction(Span(), result);
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    clone->AddToUses();
    return clone;
}

void GetRbpInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = getrbp");
}

BasicBlock::BasicBlock(const soul::ast::Span& span_, std::int32_t id_) noexcept : span(span_), id(id_), instructions(this)
{
}

void BasicBlock::Check()
{
    Instruction* inst = FirstInstruction();
    while (inst)
    {
        inst->Check();
        inst = inst->Next();
    }
}

void BasicBlock::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BasicBlock* BasicBlock::Clone(CloneContext& cloneContext) const
{
    BasicBlock* clone = new BasicBlock(Span(), id);
    cloneContext.MapBasicBlock(const_cast<BasicBlock*>(this), clone);
    return clone;
}

void BasicBlock::CloneInstructions(CloneContext& cloneContext, BasicBlock* to)
{
    Instruction* inst = FirstInstruction();
    while (inst)
    {
        Value* clonedValue = inst->Clone(cloneContext);
        Instruction* clonedInst = static_cast<Instruction*>(clonedValue);
        cloneContext.MapInstruction(inst, clonedInst);
        to->AddInstruction(clonedInst);
        inst = inst->Next();
    }
}

void BasicBlock::VisitInstructions(Visitor& visitor)
{
    Instruction* inst = FirstInstruction();
    while (inst != nullptr)
    {
        inst->Accept(visitor);
        inst = inst->Next();
    }
}

std::string BasicBlock::Name() const
{
    if (id == entryBlockId)
    {
        return "entry";
    }
    else if (id == exitBlockId)
    {
        return "exit";
    }
    else
    {
        return std::to_string(id);
    }
}

bool BasicBlock::IsLast() const noexcept
{
    Function* fn = Parent();
    return this == fn->LastBasicBlock();
}

bool BasicBlock::ContainsOnlyNops() noexcept
{
    Instruction* inst = FirstInstruction();
    while (inst && !inst->IsTerminator())
    {
        if (!inst->IsNopInstruction())
        {
            return false;
        }
        inst = inst->Next();
    }
    return true;
}

void BasicBlock::AddInstruction(Instruction* instruction, bool mapInstruction)
{
    instructions.AddChild(instruction);
    Function* fn = Parent();
    int nextRegNumber = fn->NextRegNumber();
    if (mapInstruction && instruction->IsValueInstruction())
    {
        ValueInstruction* valueInstruction = static_cast<ValueInstruction*>(instruction);
        IntermediateContext* context = fn->Parent()->GetContext();
        fn->MapInstruction(valueInstruction->Result()->Reg(), valueInstruction, context);
        nextRegNumber = std::max(valueInstruction->Result()->Reg() + 1, nextRegNumber);
    }
    fn->SetNextRegNumber(nextRegNumber);
}

void BasicBlock::AddInstruction(Instruction* instruction)
{
    AddInstruction(instruction, true);
}

std::unique_ptr<Instruction> BasicBlock::RemoveInstruction(Instruction* instruction)
{
    return std::unique_ptr<Instruction>(static_cast<Instruction*>(instructions.RemoveChild(instruction).release()));
}

void BasicBlock::InsertInstructionAfter(Instruction* instruction, Instruction* after)
{
    instructions.InsertAfter(instruction, after);
    Function* fn = Parent();
    int nextRegNumber = fn->NextRegNumber();
    if (instruction->IsValueInstruction())
    {
        ValueInstruction* valueInstruction = static_cast<ValueInstruction*>(instruction);
        IntermediateContext* context = fn->Parent()->GetContext();
        fn->MapInstruction(valueInstruction->Result()->Reg(), valueInstruction, context);
        nextRegNumber = std::max(valueInstruction->Result()->Reg() + 1, nextRegNumber);
    }
    fn->SetNextRegNumber(nextRegNumber);
}

void BasicBlock::InsertFront(Instruction* instruction)
{
    Function* fn = Parent();
    int nextRegNumber = fn->NextRegNumber();
    if (instructions.IsEmpty())
    {
        instructions.AddChild(instruction);
    }
    else
    {
        instructions.InsertBefore(instruction, instructions.FirstChild());
    }
    if (instruction->IsValueInstruction())
    {
        ValueInstruction* valueInstruction = static_cast<ValueInstruction*>(instruction);
        IntermediateContext* context = fn->Parent()->GetContext();
        fn->MapInstruction(valueInstruction->Result()->Reg(), valueInstruction, context);
        nextRegNumber = std::max(valueInstruction->Result()->Reg() + 1, nextRegNumber);
    }
    fn->SetNextRegNumber(nextRegNumber);
}

BasicBlock* BasicBlock::SplitAfter(Instruction* instruction)
{
    Function* fn = Parent();
    IntermediateContext* context = fn->Parent()->GetContext();
    std::unique_ptr<BasicBlock> newBB(new BasicBlock(Span(), fn->GetNextBasicBlockNumber()));
    BasicBlock* nbb = newBB.get();
    fn->InsertBasicBlockAfter(newBB.release(), instruction->Parent());
    instruction = instruction->Next();
    while (instruction)
    {
        Instruction* next = instruction->Next();
        std::unique_ptr<Instruction> removedInst = instruction->Parent()->RemoveInstruction(instruction);
        if (removedInst->IsValueInstruction())
        {
            ValueInstruction* valueInst = static_cast<ValueInstruction*>(removedInst.get());
            valueInst->SetRegValueIndex(fn->GetNextRegNumber());
            valueInst->Result()->SetReg(valueInst->RegValueIndex());
            fn->MapRegValue(valueInst->Result());
        }
        nbb->AddInstruction(removedInst.release(), false);
        instruction = next;
    }
    /*
        if (!LastInstruction()->IsTerminator())
        {
            DoAddInstruction(context->CreateJump(nbb));
        }
    */
    return nbb;
}

void BasicBlock::AddSuccessor(BasicBlock* successor)
{
    AddPtrToSet(successor, successors);
}

bool BasicBlock::RemoveSuccessor(BasicBlock* successor)
{
    return RemovePtrFromSet(successor, successors);
}

void BasicBlock::AddPredecessor(BasicBlock* predecessor)
{
    AddPtrToSet(predecessor, predecessors);
}

bool BasicBlock::RemovePredecessor(BasicBlock* predecessor)
{
    return RemovePtrFromSet(predecessor, predecessors);
}

void BasicBlock::ClearSuccessorsAndPredecessors()
{
    successors.clear();
    predecessors.clear();
}

int BasicBlock::IndexOf(Instruction* x) noexcept
{
    int index = 0;
    Instruction* inst = FirstInstruction();
    while (inst)
    {
        if (inst == x)
        {
            return index;
        }
        ++index;
        inst = inst->Next();
    }
    return -1;
}

Function* BasicBlock::Parent() const noexcept
{
    return static_cast<Function*>(GetContainer()->Parent());
}

Instruction* BasicBlock::Leader() const noexcept
{
    Instruction* leader = const_cast<BasicBlock*>(this)->FirstInstruction();
    while (leader && leader->IsNopInstruction())
    {
        leader = leader->Next();
    }
    return leader;
}

void BasicBlock::Write(util::CodeFormatter& formatter)
{
    int indent = formatter.IndentSize();
    bool indentDecremented = false;
    if (formatter.CurrentIndent() > 0)
    {
        formatter.DecIndent();
        indentDecremented = true;
    }
    formatter.Write(util::Format("@" + std::to_string(id), indent, util::FormatWidth::min));
    bool first = true;
    Instruction* inst = FirstInstruction();
    while (inst)
    {
        inst->Write(formatter);
        formatter.WriteLine();
        if (first)
        {
            if (indentDecremented)
            {
                formatter.IncIndent();
            }
            first = false;
        }
        inst = inst->Next();
    }
}

Function::Function(const soul::ast::Span& span_, FunctionType* type_, const std::string& name_, bool once_, bool definition_, bool createEntry,
    MetadataRef* metadataRef_, IntermediateContext* context) :
    Value(span_, ValueKind::function, type_),
    flags(FunctionFlags::none), span(span_), type(type_), name(name_), metadataRef(metadataRef_), basicBlocks(this), nextRegNumber(0), nextBBNumber(0), mdId(-1)
{
    SetContainer(context->GetCode().Functions());
    if (createEntry)
    {
        CreateEntry(context);
    }
    if (once_)
    {
        SetFlag(FunctionFlags::linkOnce);
    }
    if (definition_)
    {
        SetFlag(FunctionFlags::defined);
    }
}

void Function::CreateEntry(IntermediateContext* context)
{
    if (entryBlock) return;
    entryBlock.reset(new BasicBlock(Span(), nextBBNumber++));
    entryBlock->SetContainer(&basicBlocks);
    std::int32_t n = type->ParamTypeRefs().size();
    for (std::int32_t index = 0; index < n; ++index)
    {
        const TypeRef& paramTypeRef = type->ParamTypeRefs()[index];
        RegValue* regValue = new RegValue(Span(), paramTypeRef.GetType(), nextRegNumber++);
        Instruction* paramInst = new ParamInstruction(Span(), regValue);
        context->AddLineInfo(paramInst);
        entryBlock->AddInstruction(paramInst);
        params.push_back(paramInst);
    }
}

void Function::Check()
{
    BasicBlock* bb = FirstBasicBlock();
    while (bb)
    {
        bb->Check();
        bb = bb->Next();
    }
}

void Function::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Function* Function::Clone() const
{
    Code* code = Parent();
    Function* clone = new Function(Span(), type, name, IsLinkOnce(), IsDefined(), false, metadataRef, Parent()->GetContext());
    clone->SetMdId(mdId);
    std::string fullName = ResolveFullName();
    if (!fullName.empty())
    {
        clone->SetComment(fullName);
    }
    clone->SetContainer(code->Functions());
    CloneContext cloneContext;
    cloneContext.SetContext(code->GetContext());
    cloneContext.SetCurrentFunction(clone);
    BasicBlock* bb = const_cast<Function*>(this)->FirstBasicBlock();
    while (bb)
    {
        BasicBlock* clonedBB = bb->Clone(cloneContext);
        clone->AddBasicBlock(clonedBB);
        bb = bb->Next();
    }
    bb = const_cast<Function*>(this)->FirstBasicBlock();
    while (bb)
    {
        BasicBlock* clonedBB = cloneContext.GetMappedBasicBlock(bb);
        if (clonedBB)
        {
            bb->CloneInstructions(cloneContext, clonedBB);
        }
        else
        {
            Error("mapped basic block not found", bb->Span(), code->GetContext());
        }
        bb = bb->Next();
    }
    for (const auto& instRegValuePair : cloneContext.UnmappedInstructions())
    {
        Instruction* inst = instRegValuePair.first;
        RegValue* regValue = instRegValuePair.second;
        Instruction* mappedInst = cloneContext.GetMappedInstruction(inst);
        if (mappedInst)
        {
            regValue->SetInst(mappedInst);
        }
        else
        {
            Error("mapped instruction not found", inst->Span(), code->GetContext());
        }
    }
    return clone;
}

void Function::VisitBasicBlocks(Visitor& visitor)
{
    BasicBlock* basicBlock = FirstBasicBlock();
    while (basicBlock)
    {
        basicBlock->Accept(visitor);
        basicBlock = basicBlock->Next();
    }
}

Code* Function::Parent() const noexcept
{
    return static_cast<Code*>(GetContainer()->Parent());
}

BasicBlock* Function::GetBasicBlock(std::int32_t id) const noexcept
{
    auto it = basicBlockMap.find(id);
    if (it != basicBlockMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

BasicBlock* Function::CreateBasicBlock()
{
    if (!FirstBasicBlock())
    {
        BasicBlock* eb = entryBlock.release();
        AddBasicBlock(eb);
        basicBlockMap[eb->Id()] = eb;
        return eb;
    }
    BasicBlock* bb = new BasicBlock(soul::ast::Span(), nextBBNumber++);
    AddBasicBlock(bb);
    basicBlockMap[bb->Id()] = bb;
    return bb;
}

BasicBlock* Function::AddBasicBlock(const soul::ast::Span& span, std::int32_t id, IntermediateContext* context)
{
    BasicBlock* prev = GetBasicBlock(id);
    if (prev)
    {
        Error("error adding basic block: basic block id not unique", span, context, prev->Span());
    }
    BasicBlock* basicBlock = new BasicBlock(span, id);
    basicBlocks.AddChild(basicBlock);
    basicBlockMap[id] = basicBlock;
    nextBBNumber = std::max(id + 1, nextBBNumber);
    return basicBlock;
}

void Function::AddBasicBlock(BasicBlock* basicBlock)
{
    basicBlocks.AddChild(basicBlock);
    basicBlockMap[basicBlock->Id()] = basicBlock;
    nextBBNumber = std::max(basicBlock->Id() + 1, nextBBNumber);
}

void Function::InsertBasicBlockBefore(BasicBlock* basicBlockToInsert, BasicBlock* before)
{
    basicBlocks.InsertBefore(basicBlockToInsert, before);
    basicBlockMap[basicBlockToInsert->Id()] = basicBlockToInsert;
    nextBBNumber = std::max(basicBlockToInsert->Id() + 1, nextBBNumber);
}

void Function::InsertBasicBlockAfter(BasicBlock* basicBlockToInsert, BasicBlock* after)
{
    basicBlocks.InsertAfter(basicBlockToInsert, after);
    basicBlockMap[basicBlockToInsert->Id()] = basicBlockToInsert;
    nextBBNumber = std::max(basicBlockToInsert->Id() + 1, nextBBNumber);
}

std::unique_ptr<BasicBlock> Function::RemoveBasicBlock(BasicBlock* block)
{
    RemovePtrFromSet(block, retBlocks);
    basicBlockMap.erase(block->Id());
    for (BasicBlock* successor : block->Successors())
    {
        successor->RemovePredecessor(block);
    }
    for (BasicBlock* predecessor : block->Predecessors())
    {
        predecessor->RemoveSuccessor(block);
    }
    return std::unique_ptr<BasicBlock>(static_cast<BasicBlock*>(basicBlocks.RemoveChild(block).release()));
}

RegValue* Function::GetRegValue(std::int32_t reg) const noexcept
{
    auto it = regValueMap.find(reg);
    if (it != regValueMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

RegValue* Function::GetRegRef(const soul::ast::Span& span, Type* type, std::int32_t reg, IntermediateContext* context) const
{
    RegValue* regValue = GetRegValue(reg);
    if (regValue)
    {
        if (regValue->GetType() == type)
        {
            return regValue;
        }
        else
        {
            Error("error referencing register value " + std::to_string(reg) + " in function " + Name() + ": type conflict", span, context, regValue->Span());
        }
    }
    else
    {
        Error("error referencing register in function " + Name() + ": register " + std::to_string(reg) + " not found", span, context);
    }
    return nullptr;
}

RegValue* Function::MakeRegValue(const soul::ast::Span& span, Type* type, std::int32_t reg, IntermediateContext* context)
{
    RegValue* regValue = new RegValue(span, type, reg);
    regValues.push_back(std::unique_ptr<RegValue>(regValue));
    regValueMap[reg] = regValue;
    return regValue;
}

RegValue* Function::MakeNextRegValue(const soul::ast::Span& span, Type* type, IntermediateContext* context)
{
    return MakeRegValue(span, type, GetNextRegNumber(), context);
}

void Function::MapRegValue(RegValue* regValue)
{
    regValueMap[regValue->Reg()] = regValue;
}

void Function::RemoveRegValue(std::int32_t reg)
{
    regValueMap.erase(reg);
}

Instruction* Function::GetInstruction(std::int32_t reg) const noexcept
{
    auto it = instructionMap.find(reg);
    if (it != instructionMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void Function::MapInstruction(std::int32_t reg, Instruction* inst, IntermediateContext* context)
{
    instructionMap[reg] = inst;
}

int Function::NumBasicBlocks() const noexcept
{
    return basicBlockMap.size();
}

void Function::AddRetBlock(BasicBlock* retBlock)
{
    AddPtrToSet(retBlock, retBlocks);
}

void Function::AddEntryAndExitBlocks()
{
    if (basicBlocks.IsEmpty())
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has no basic blocks", Span(), Parent()->GetContext());
    }
    BasicBlock* prevEntryBlock = GetBasicBlock(entryBlockId);
    if (prevEntryBlock)
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has already an entry block", Span(), Parent()->GetContext());
    }
    BasicBlock* prevExitBlock = GetBasicBlock(exitBlockId);
    if (prevExitBlock)
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has already an exit block", Span(), Parent()->GetContext());
    }
    if (RetBlocks().empty())
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has no ret blocks", Span(), Parent()->GetContext());
    }
    BasicBlock* firstBasicBlock = FirstBasicBlock();
    BasicBlock* entryBlock = new BasicBlock(Span(), entryBlockId);
    basicBlockMap[entryBlock->Id()] = entryBlock;
    entryBlock->AddSuccessor(firstBasicBlock);
    firstBasicBlock->AddPredecessor(entryBlock);
    basicBlocks.InsertBefore(entryBlock, firstBasicBlock);
    BasicBlock* exitBlock = new BasicBlock(Span(), exitBlockId);
    for (BasicBlock* retBlock : RetBlocks())
    {
        retBlock->AddSuccessor(exitBlock);
        exitBlock->AddPredecessor(retBlock);
    }
    basicBlocks.AddChild(exitBlock);
}

void Function::RemoveEntryAndExitBlocks()
{
    BasicBlock* entryBlock = GetBasicBlock(entryBlockId);
    if (!entryBlock)
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' has no entry block", Span(), Parent()->GetContext());
    }
    BasicBlock* exitBlock = GetBasicBlock(exitBlockId);
    if (!exitBlock)
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' has no exit block", Span(), Parent()->GetContext());
    }
    BasicBlock* firstBasicBlock = entryBlock->Next();
    if (!firstBasicBlock)
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' has no basic blocks other than entry and exit blocks", Span(), Parent()->GetContext());
    }
    if (!firstBasicBlock->RemovePredecessor(entryBlock))
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' old first block does not have the entry block as a predecessor", Span(), Parent()->GetContext());
    }
    basicBlocks.RemoveChild(entryBlock);
    basicBlockMap.erase(entryBlockId);
    for (BasicBlock* retBlock : RetBlocks())
    {
        if (!retBlock->RemoveSuccessor(exitBlock))
        {
            Error("error removing entry and exit blocks: function '" + Name() + "' ret block does not have the exit block as a successor", Span(), Parent()->GetContext());
        }
    }
    basicBlocks.RemoveChild(exitBlock);
    basicBlockMap.erase(exitBlockId);
}

Value* Function::GetParam(int index) const
{
    if (index >= 0 && index < params.size())
    {
        return params[index];
    }
    else
    {
        Error("invalid param index " + std::to_string(index) + " for function '" + Name() + "'", Span(), Parent()->GetContext());
        return nullptr;
    }
}

void Function::SetNumbers()
{
    IntermediateContext* context = Parent()->GetContext();
    basicBlockMap.clear();
    regValueMap.clear();
    instructionMap.clear();
    nextBBNumber = 0;
    BasicBlock* bb = FirstBasicBlock();
    while (bb)
    {
        bb->SetId(nextBBNumber++);
        basicBlockMap[bb->Id()] = bb;
        bb = bb->Next();
    }
    int instructionIndex = 0;
    nextRegNumber = 0;
    bb = FirstBasicBlock();
    while (bb)
    {
        Instruction* inst = bb->FirstInstruction();
        while (inst)
        {
            inst->SetIndex(instructionIndex++);
            if (inst->IsValueInstruction())
            {
                ValueInstruction* valueInst = static_cast<ValueInstruction*>(inst);
                valueInst->SetRegValueIndex(nextRegNumber++);
                valueInst->Result()->SetReg(valueInst->RegValueIndex());
                MapRegValue(valueInst->Result());
                MapInstruction(valueInst->RegValueIndex(), valueInst, context);
            }
            inst = inst->Next();
        }
        bb = bb->Next();
    }
}

void Function::MoveRegValues(Function* toFunction)
{
    for (auto& regValue : regValues)
    {
        if (!regValue->Inst()) continue;
        regValue->SetReg(toFunction->GetNextRegNumber());
        toFunction->MapRegValue(regValue.get());
        toFunction->regValues.push_back(std::move(regValue));
    }
}

void Function::SetComment(const std::string& comment_)
{
    comment = comment_;
}

std::string Function::ResolveFullName() const
{
    std::string fullName;
    if (metadataRef)
    {
        MetadataStruct* metadataStruct = metadataRef->GetMetadataStruct();
        if (metadataStruct)
        {
            MetadataItem* metadataItem = metadataStruct->GetItem("fullName");
            if (metadataItem)
            {
                if (metadataItem->IsMetadataString())
                {
                    MetadataString* metadataString = static_cast<MetadataString*>(metadataItem);
                    fullName = metadataString->Value();
                }
            }
            MetadataItem* parentNameItem = metadataStruct->GetItem("parentName");
            if (parentNameItem)
            {
                if (parentNameItem->IsMetadataString())
                {
                    MetadataString* metadataString = static_cast<MetadataString*>(parentNameItem);
                    fullName.append(", parentFn=").append(metadataString->Value());
                }
            }
        }
    }
    return fullName;
}

void Function::Write(util::CodeFormatter& formatter)
{
    if (metadataRef)
    {
        SetComment(ResolveFullName());
    }
    if (!comment.empty())
    {
        formatter.WriteLine("// " + comment);
        formatter.WriteLine();
    }
#ifdef DEBUG_FUNCTIONS
    std::cout << "fn>" << Name() << ":" << comment << "\n";
#endif
    if (basicBlocks.IsEmpty())
    {
        formatter.Write("extern ");
    }
    else if (IsInline())
    {
        formatter.Write("inline ");
    }
    if (IsLinkOnce())
    {
        formatter.Write("link_once ");
    }
    if (IsChildFn())
    {
        formatter.Write("child ");
    }
    IntermediateContext* context = Parent()->GetContext();
    Type* ptrType = type->AddPointer(context);
    if (metadataRef)
    {
        mdId = metadataRef->NodeId();
    }
    std::string mdIdStr;
    if (mdId != -1)
    {
        mdIdStr.append(" !").append(std::to_string(mdId));
    }
    formatter.WriteLine("function " + ptrType->Name() + " " + name + mdIdStr);
    if (basicBlocks.IsEmpty())
    {
        return;
    }
    SetNumbers();
    formatter.WriteLine("{");
    formatter.IncIndent();
    bool first = true;
    BasicBlock* bb = FirstBasicBlock();
    while (bb)
    {
        if (bb->IsEmpty())
        {
            bb = bb->Next();
            continue;
        }
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.WriteLine();
        }
        bb->Write(formatter);
        bb = bb->Next();
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
#ifdef DEBUG_FUNCTIONS
    std::cout << "fn<" << Name() << "\n";
#endif
}

Code::Code() noexcept : context(nullptr), currentFunction(nullptr), functions(this), totalFunctions(0)
{
}

void Code::SetCurrentFunction(Function* function) noexcept
{
    currentFunction = function;
}

Function* Code::GetOrInsertFunction(const std::string& functionId, FunctionType* functionType)
{
    Function* function = GetFunction(functionId);
    if (function)
    {
        if (function->GetType() != functionType)
        {
            Error("type of function '" + function->Name() + "' (" + function->GetType()->Name() + ") conflicts with earlier declaration (" +
                functionType->Name() + ")", soul::ast::Span(), context);
            return nullptr;
        }
        else
        {
            return function;
        }
    }
    else
    {
        return AddFunctionDeclaration(soul::ast::Span(), functionType, functionId);
    }
}

Function* Code::GetFunction(const std::string& functionId) const noexcept
{
    auto it = functionMap.find(functionId);
    if (it != functionMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Function* Code::AddFunctionDefinition(const soul::ast::Span& span, FunctionType* functionType, const std::string& functionId, bool inline_, bool linkOnce, bool child,
    bool createEntry, MetadataRef* metadataRef)
{
    Function* prev = GetFunction(functionId);
    if (prev)
    {
        if (prev->IsDefined())
        {
            Error("error adding function '" + functionId + "': function id not unique", span, context, prev->Span());
        }
        else
        {
            if (prev->GetType() != functionType)
            {
                Error("error adding function '" + functionId + "': type '" + functionType->Name() +
                    "' conflicts with earlier declaration (" + prev->GetType()->Name() + ")", span, context, prev->Span());
            }
            prev->SetDefined();
            prev->CreateEntry(context);
            return prev;
        }
    }
    Function* function = new Function(span, functionType, functionId, linkOnce, true, createEntry, metadataRef, GetContext());
    if (inline_)
    {
        function->SetInline();
    }
    if (linkOnce)
    {
        function->SetLinkOnce();
    }
    if (child)
    {
        function->SetAsChildFn();
    }
    functions.AddChild(function);
    functionMap[function->Name()] = function;
    return function;
}

Function* Code::AddFunctionDeclaration(const soul::ast::Span& span, FunctionType* functionType, const std::string& functionId)
{
    Function* prev = GetFunction(functionId);
    if (prev)
    {
        if (prev->GetType() != functionType)
        {
            Error("error adding function declaration '" + functionId + "': type '" + functionType->Name() + "' conflicts with earlier declaration",
                span, context, prev->Span());
            return nullptr;
        }
        return prev;
    }
    Function* function = new Function(span, functionType, functionId, false, false, false, nullptr, GetContext());
    functions.AddChild(function);
    functionMap[function->Name()] = function;
    return function;
}

void Code::VisitFunctions(Visitor& visitor)
{
    totalFunctions = 0;
    Function* function = FirstFunction();
    while (function)
    {
        function->Accept(visitor);
        ++totalFunctions;
        function = function->Next();
    }
}

} // otava::intermediate
