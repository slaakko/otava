// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.code;

import otava.intermediate.data;
import otava.assembly.instruction;
import util;
import std;

export namespace otava::intermediate {

class Instruction;
class BasicBlock;
class MetadataRef;
class Function;
class Code;

class RegValue : public Value
{
public:
    RegValue(const soul::ast::Span& span_, Type* type_, std::int32_t reg_) noexcept;
    ~RegValue();
    Value* Clone(CloneContext& cloneContext) const override;
    inline std::int32_t Reg() const noexcept { return reg; }
    inline void SetReg(std::int32_t reg_) noexcept { reg = reg_; }
    inline void SetInst(Instruction* inst_) noexcept { inst = inst_; }
    inline Instruction* Inst() const noexcept { return inst; }
    std::string ToString() const override;
private:
    std::int32_t reg;
    Instruction* inst;
};

enum class OpCode : int
{
    store, arg, jmp, branch, procedure_call, ret, switch_,
    not_, neg, signextend, zeroextend, truncate, fpextend, bitcast, inttofloat, floattoint, inttoptr, ptrtoint,
    add, sub, mul, div_, mod, and_, or_, xor_, shl, shr, equal, less,
    param, local, plocal, load, elemaddr, ptroffset, ptrdiff, function_call,
    nop, getrbp
};

void AddUser(Instruction* user, Value* value);
void RemoveUser(Instruction* user, Value* value);
void AddToUsesVec(std::vector<Instruction*>& uses, Value* value);

class Instruction : public Value, public util::Component
{
public:
    Instruction(const soul::ast::Span& span_, Type* type_, OpCode opCode_) noexcept;
    void Check();
    std::string Name() const;
    BasicBlock* Parent() const noexcept;
    inline Instruction* Next() noexcept { return static_cast<Instruction*>(NextSibling()); }
    inline Instruction* Prev() noexcept { return static_cast<Instruction*>(PrevSibling()); }
    inline OpCode GetOpCode() const noexcept { return opCode; }
    inline void SetMetadataRef(MetadataRef* metadataRef_) noexcept { metadataRef = metadataRef_; }
    inline MetadataRef* GetMetadataRef() const noexcept { return metadataRef; }
    bool IsLeader() const noexcept;
    bool IsTerminator() const noexcept;
    bool IsValueInstruction() const noexcept;
    bool IsUnaryInstruction() const noexcept;
    bool IsBinaryInstruction() const noexcept;
    inline bool IsSwitchInstruction() const noexcept { return opCode == OpCode::switch_; }
    inline bool IsParamInstruction() const noexcept { return opCode == OpCode::param; }
    inline bool IsLocalInstruction() const noexcept { return opCode == OpCode::local; }
    inline bool IsPLocalInstruction() const noexcept { return opCode == OpCode::plocal; }
    inline bool IsLoadInstruction() const noexcept { return opCode == OpCode::load; }
    inline bool IsStoreInstruction() const noexcept { return opCode == OpCode::store; }
    inline bool IsElemAddrInstruction() const noexcept { return opCode == OpCode::elemaddr; }
    inline bool IsArgInstruction() const noexcept { return opCode == OpCode::arg; }
    inline bool IsFunctionCallInstruction() const noexcept { return opCode == OpCode::function_call; }
    inline bool IsProcedureCallInstruction() const noexcept { return opCode == OpCode::procedure_call; }
    inline bool IsRetInstruction() const noexcept { return opCode == OpCode::ret; }
    bool IsRetVoid() const noexcept;
    inline bool IsJumpInstruction() const noexcept { return opCode == OpCode::jmp; }
    inline bool IsBranchInstruction() const noexcept { return opCode == OpCode::branch; }
    inline bool IsNopInstruction() const noexcept { return opCode == OpCode::nop; }
    bool RequiresLocalRegister() const noexcept;
    virtual bool IsFloatingPointInstruction() const noexcept { return false; }
    std::vector<BasicBlock*> Successors() const;
    inline int Index() const noexcept { return index; }
    inline void SetIndex(int index_) noexcept { index = index_; }
    inline int RegValueIndex() const noexcept { return regValueIndex; }
    inline void SetRegValueIndex(int regValueIndex_) noexcept { regValueIndex = regValueIndex_; }
    inline const std::vector<Instruction*>& Users() const noexcept { return users; }
    void AddUser(Instruction* user);
    void RemoveUser(Instruction* user);
    virtual std::vector<Instruction*> Uses() const { return std::vector<Instruction*>(); }
    virtual void AddToUses();
    void ReplaceUsesWith(Value* value);
    void RemoveFromUses();
    virtual void ReplaceValue(Value* use, Value* value);
    virtual void Write(util::CodeFormatter& formatter) = 0;
    inline otava::assembly::Instruction* AssemblyInstruction() const noexcept { return assemblyInstruction; }
    inline void SetAssemblyInstruction(otava::assembly::Instruction* assemblyInstruction_) noexcept { assemblyInstruction = assemblyInstruction_; }
private:
    OpCode opCode;
    MetadataRef* metadataRef;
    int index;
    int regValueIndex;
    std::vector<Instruction*> users;
    otava::assembly::Instruction* assemblyInstruction;
};

class StoreInstruction : public Instruction
{
public:
    StoreInstruction(const soul::ast::Span& span_, Value* value_, Value* ptr_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* GetValue() const noexcept { return value; }
    inline Value* GetPtr() const noexcept { return ptr; }
    bool IsFloatingPointInstruction() const noexcept override { return value->GetType()->IsFloatingPointType(); }
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* value;
    Value* ptr;
};

class ArgInstruction : public Instruction
{
public:
    ArgInstruction(const soul::ast::Span& span_, Value* arg_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* Arg() const noexcept { return arg; }
    bool IsFloatingPointInstruction() const noexcept override { return arg->GetType()->IsFloatingPointType(); }
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* arg;
};

class JmpInstruction : public Instruction
{
public:
    JmpInstruction(const soul::ast::Span& span_, std::int32_t targetLabelId_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline std::int32_t TargetLabelId() const noexcept { return targetLabelId; }
    inline BasicBlock* TargetBasicBlock() const noexcept { return targetBasicBlock; }
    inline void SetTargetBasicBlock(BasicBlock* targetBasicBlock_) noexcept { targetBasicBlock = targetBasicBlock_; }
private:
    std::int32_t targetLabelId;
    BasicBlock* targetBasicBlock;
};

class BranchInstruction : public Instruction
{
public:
    BranchInstruction(const soul::ast::Span& span_, Value* cond_, std::int32_t trueTargetLabelId_, std::int32_t falseTargetLabelId_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    Value* Cond() const noexcept { return cond; }
    inline std::int32_t TrueTargetLabelId() const noexcept { return trueTargetLabelId; }
    inline BasicBlock* TrueTargetBasicBlock() const noexcept { return trueTargetBasicBlock; }
    inline void SetTrueTargetBasicBlock(BasicBlock* trueTargetBasicBlock_) noexcept { trueTargetBasicBlock = trueTargetBasicBlock_; }
    inline std::int32_t FalseTargetLabelId() const noexcept { return falseTargetLabelId; }
    inline BasicBlock* FalseTargetBasicBlock() const noexcept { return falseTargetBasicBlock; }
    inline void SetFalseTargetBasicBlock(BasicBlock* falseTargetBasicBlock_) noexcept { falseTargetBasicBlock = falseTargetBasicBlock_; }
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* cond;
    std::int32_t trueTargetLabelId;
    BasicBlock* trueTargetBasicBlock;
    std::int32_t falseTargetLabelId;
    BasicBlock* falseTargetBasicBlock;
};

class ProcedureCallInstruction : public Instruction
{
public:
    ProcedureCallInstruction(const soul::ast::Span& span_, Value* callee_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* Callee() const noexcept { return callee; }
    Function* CalleeFn() const noexcept;
    inline const std::vector<Value*>& Args() const noexcept { return args; }
    void SetArgs(std::vector<Value*>&& args_);
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* callee;
    std::vector<Value*> args;
};

class RetInstruction : public Instruction
{
public:
    RetInstruction(const soul::ast::Span& span_, Value* returnValue_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* ReturnValue() const noexcept { return returnValue; }
    bool IsFloatingPointInstruction() const noexcept override;
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* returnValue;
};

struct CaseTarget
{
    CaseTarget() noexcept;
    inline CaseTarget(Value* caseValue_, std::int32_t targetLabelId_) noexcept : caseValue(caseValue_), targetLabelId(targetLabelId_), targetBlock(nullptr) {}
    Value* caseValue;
    std::int32_t targetLabelId;
    BasicBlock* targetBlock;
};

class SwitchInstruction : public Instruction
{
public:
    SwitchInstruction(const soul::ast::Span& span_, Value* cond_, std::int32_t defaultTargetLabelId_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* Cond() const noexcept { return cond; }
    inline std::int32_t DefaultTargetLabelId() const noexcept { return defaultTargetLabelId; }
    void AddCaseTarget(const CaseTarget& caseTarget);
    inline const std::vector<CaseTarget>& CaseTargets() const noexcept { return caseTargets; }
    inline std::vector<CaseTarget>& CaseTargets() noexcept { return caseTargets; }
    inline BasicBlock* DefaultTargetBlock() const noexcept { return defaultTargetBlock; }
    inline void SetDefaultTargetBlock(BasicBlock* defaultTargetBlock_) noexcept { defaultTargetBlock = defaultTargetBlock_; }
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* cond;
    std::int32_t defaultTargetLabelId;
    BasicBlock* defaultTargetBlock;
    std::vector<CaseTarget> caseTargets;
};

class ValueInstruction : public Instruction
{
public:
    ValueInstruction(const soul::ast::Span& span_, RegValue* result_, OpCode opCode_) noexcept;
    ~ValueInstruction();
    inline RegValue* Result() const noexcept { return result; }
    inline void ResetResult() noexcept { result = nullptr; }
    void WriteResult(util::CodeFormatter& formatter);
    bool IsFloatingPointInstruction() const noexcept override { return result->GetType()->IsFloatingPointType(); }
    std::string ToString() const override { return result->ToString(); }
private:
    RegValue* result;
};

class UnaryInstruction : public ValueInstruction
{
public:
    UnaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_, OpCode opCode_) noexcept;
    void WriteArg(util::CodeFormatter& formatter);
    inline Value* Operand() const noexcept { return operand; }
    inline void SetOperand(Value* operand_) noexcept { operand = operand_; }
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* operand;
};

class NotInstruction : public UnaryInstruction
{
public:
    NotInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class NegInstruction : public UnaryInstruction
{
public:
    NegInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    bool IsFloatingPointInstruction() const noexcept override { return Operand()->GetType()->IsFloatingPointType(); }
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class SignExtendInstruction : public UnaryInstruction
{
public:
    SignExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class ZeroExtendInstruction : public UnaryInstruction
{
public:
    ZeroExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class FloatingPointExtendInstruction : public UnaryInstruction
{
public:
    FloatingPointExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class TruncateInstruction : public UnaryInstruction
{
public:
    TruncateInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class BitcastInstruction : public UnaryInstruction
{
public:
    BitcastInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class IntToFloatInstruction : public UnaryInstruction
{
public:
    IntToFloatInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    bool IsFloatingPointInstruction() const noexcept override { return true; }
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class FloatToIntInstruction : public UnaryInstruction
{
public:
    FloatToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    bool IsFloatingPointInstruction() const noexcept override { return false; }
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class IntToPtrInstruction : public UnaryInstruction
{
public:
    IntToPtrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class PtrToIntInstruction : public UnaryInstruction
{
public:
    PtrToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class BinaryInstruction : public ValueInstruction
{
public:
    BinaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_, OpCode opCode_) noexcept;
    inline Value* Left() const noexcept { return left; }
    inline Value* Right() const noexcept { return right; }
    bool IsFloatingPointInstruction() const noexcept override { return Left()->GetType()->IsFloatingPointType(); }
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    void WriteArgs(util::CodeFormatter& formatter);
    std::vector<Instruction*> Uses() const override;
private:
    Value* left;
    Value* right;
};

class AddInstruction : public BinaryInstruction
{
public:
    AddInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class SubInstruction : public BinaryInstruction
{
public:
    SubInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class MulInstruction : public BinaryInstruction
{
public:
    MulInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class DivInstruction : public BinaryInstruction
{
public:
    DivInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class ModInstruction : public BinaryInstruction
{
public:
    ModInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class AndInstruction : public BinaryInstruction
{
public:
    AndInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class OrInstruction : public BinaryInstruction
{
public:
    OrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class XorInstruction : public BinaryInstruction
{
public:
    XorInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class ShlInstruction : public BinaryInstruction
{
public:
    ShlInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class ShrInstruction : public BinaryInstruction
{
public:
    ShrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class EqualInstruction : public BinaryInstruction
{
public:
    EqualInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    bool IsFloatingPointInstruction() const noexcept override { return false; }
    void Write(util::CodeFormatter& formatter) override;
};

class LessInstruction : public BinaryInstruction
{
public:
    LessInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    bool IsFloatingPointInstruction() const noexcept override { return false; }
    void Write(util::CodeFormatter& formatter) override;
};

class ParamInstruction : public ValueInstruction
{
public:
    ParamInstruction(const soul::ast::Span& span_, RegValue* result_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class LocalInstruction : public ValueInstruction
{
public:
    LocalInstruction(const soul::ast::Span& span_, RegValue* result_, Type* localType_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Type* LocalType() const { return localType; }
private:
    Type* localType;
};

class PLocalInstruction : public ValueInstruction
{
public:
    PLocalInstruction(const soul::ast::Span& span_, RegValue* result_, Type* localType_, int level_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Type* LocalType() const { return localType; }
    inline int Level() const { return level; }
private:
    Type* localType;
    int level;
};

class LoadInstruction : public ValueInstruction
{
public:
    LoadInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* Ptr() const noexcept { return ptr; }
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* ptr;
};

enum class ElemAddrKind
{
    none, array, structure
};

class ElemAddrInstruction : public ValueInstruction
{
public:
    ElemAddrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* indexValue_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* Ptr() const noexcept { return ptr; }
    inline Value* IndexValue() const noexcept { return indexValue; }
    ElemAddrKind GetElemAddrKind(IntermediateContext* context) const;
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* ptr;
    Value* indexValue;
};

class PtrOffsetInstruction : public ValueInstruction
{
public:
    PtrOffsetInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* offset_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* Ptr() const noexcept { return ptr; }
    inline Value* Offset() const noexcept { return offset; }
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* ptr;
    Value* offset;
};

class PtrDiffInstruction : public ValueInstruction
{
public:
    PtrDiffInstruction(const soul::ast::Span& span_, RegValue* result_, Value* leftPtr_, Value* rightPtr_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* LeftPtr() const noexcept { return leftPtr; }
    inline Value* RightPtr() const noexcept { return rightPtr; }
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* leftPtr;
    Value* rightPtr;
};

class FunctionCallInstruction : public ValueInstruction
{
public:
    FunctionCallInstruction(const soul::ast::Span& span_, RegValue* result_, Value* callee_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
    inline Value* Callee() const noexcept { return callee; }
    Function* CalleeFn() const noexcept;
    inline const std::vector<Value*>& Args() const noexcept { return args; }
    void SetArgs(std::vector<Value*>&& args_);
    void AddToUses() override;
    void ReplaceValue(Value* use, Value* value) override;
    std::vector<Instruction*> Uses() const override;
private:
    Value* callee;
    std::vector<Value*> args;
};

struct BlockValue
{
    inline BlockValue(Value* value_, std::int32_t blockId_) noexcept : value(value_), blockId(blockId_), block(nullptr) {}
    BlockValue(Value* value_, BasicBlock* block_) noexcept;
    Value* value;
    std::int32_t blockId;
    BasicBlock* block;
};

class NoOperationInstruction : public Instruction
{
public:
    NoOperationInstruction(const soul::ast::Span& span_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

class GetRbpInstruction : public ValueInstruction
{
public:
    GetRbpInstruction(const soul::ast::Span& span_, RegValue* result_) noexcept;
    void Accept(Visitor& visitor) override;
    Value* Clone(CloneContext& cloneContext) const override;
    void Write(util::CodeFormatter& formatter) override;
};

const std::int32_t entryBlockId = -1;
const std::int32_t exitBlockId = -2;

class BasicBlock : public util::Component
{
public:
    BasicBlock(const soul::ast::Span& span_, std::int32_t id_) noexcept;
    void Check();
    void Accept(Visitor& visitor);
    BasicBlock* Clone(CloneContext& cloneContext) const;
    void CloneInstructions(CloneContext& cloneContext, BasicBlock* to);
    void VisitInstructions(Visitor& visitor);
    inline const soul::ast::Span& Span() const noexcept { return span; }
    std::string Name() const;
    inline std::int32_t Id() const noexcept { return id; }
    inline void SetId(std::int32_t id_) noexcept { id = id_; }
    Function* Parent() const noexcept;
    inline BasicBlock* Next() noexcept { return static_cast<BasicBlock*>(NextSibling()); }
    inline BasicBlock* Prev() noexcept { return static_cast<BasicBlock*>(PrevSibling()); }
    inline Instruction* FirstInstruction() noexcept { return static_cast<Instruction*>(instructions.FirstChild()); }
    inline Instruction* LastInstruction() noexcept { return static_cast<Instruction*>(instructions.LastChild()); }
    inline util::Container* Instructions() noexcept { return &instructions; }
    Instruction* Leader() const noexcept;
    bool IsLast() const noexcept;
    bool ContainsOnlyNops() noexcept;
    void AddInstruction(Instruction* instruction);
    void AddInstruction(Instruction* instruction, bool mapInstruction);
    std::unique_ptr<Instruction> RemoveInstruction(Instruction* instruction);
    void InsertInstructionAfter(Instruction* instruction, Instruction* after);
    void InsertFront(Instruction* instruction);
    BasicBlock* SplitAfter(Instruction* instruction);
    inline bool IsEmpty() const noexcept { return instructions.IsEmpty(); }
    inline bool IsEntryBlock() const noexcept { return id == entryBlockId; }
    inline bool IsExitBlock() const noexcept { return id == exitBlockId; }
    inline const std::vector<BasicBlock*>& Successors() const noexcept { return successors; }
    void AddSuccessor(BasicBlock* successor);
    bool RemoveSuccessor(BasicBlock* successor);
    inline const std::vector<BasicBlock*>& Predecessors() const noexcept { return predecessors; }
    void AddPredecessor(BasicBlock* predecessor);
    bool RemovePredecessor(BasicBlock* predecessor);
    void ClearSuccessorsAndPredecessors();
    int IndexOf(Instruction* x) noexcept;
    void Write(util::CodeFormatter& formatter);
private:
    soul::ast::Span span;
    std::int32_t id;
    util::Container instructions;
    std::vector<BasicBlock*> successors;
    std::vector<BasicBlock*> predecessors;
};

enum class FunctionFlags : int
{
    none = 0, defined = 1 << 0, inline_ = 1 << 1, linkOnce = 1 << 2, child = 1 << 3
};

inline FunctionFlags operator|(FunctionFlags left, FunctionFlags right) noexcept
{
    return FunctionFlags(static_cast<int>(left) | static_cast<int>(right));
}

inline FunctionFlags operator&(FunctionFlags left, FunctionFlags right) noexcept
{
    return FunctionFlags(static_cast<int>(left) & static_cast<int>(right));
}

inline FunctionFlags operator~(FunctionFlags flags) noexcept
{
    return FunctionFlags(~static_cast<int>(flags));
}

class Function : public Value, public util::Component
{
public:
    Function(const soul::ast::Span& span_, FunctionType* functionType_, const std::string& name_, bool once_, bool definition_, bool createEntry,
        MetadataRef* metadataRef_, IntermediateContext* context);
    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;
    void CreateEntry(IntermediateContext* context);
    void Check();
    inline bool GetFlag(FunctionFlags flag) const noexcept { return (flags & flag) != FunctionFlags::none; }
    inline void SetFlag(FunctionFlags flag) noexcept { flags = flags | flag; }
    inline void ResetFlag(FunctionFlags flag) noexcept { flags = flags & ~flag; }
    inline bool IsDefined() const noexcept { return GetFlag(FunctionFlags::defined); }
    inline bool IsExternal() const noexcept { return !IsDefined(); }
    inline void SetDefined() noexcept { SetFlag(FunctionFlags::defined); }
    inline bool IsInline() const noexcept { return GetFlag(FunctionFlags::inline_); }
    inline void SetInline() noexcept { SetFlag(FunctionFlags::inline_); }
    inline bool IsLinkOnce() const noexcept { return GetFlag(FunctionFlags::linkOnce); }
    inline void SetLinkOnce() noexcept { SetFlag(FunctionFlags::linkOnce); }
    inline bool IsChildFn() const noexcept { return GetFlag(FunctionFlags::child); }
    void SetAsChildFn() noexcept { SetFlag(FunctionFlags::child); }
    inline int Arity() const noexcept { return type->Arity(); }
    void Accept(Visitor& visitor) override;
    Function* Clone() const;
    void VisitBasicBlocks(Visitor& visitor);
    Code* Parent() const noexcept;
    inline Function* Next() noexcept { return static_cast<Function*>(NextSibling()); }
    inline Function* Prev() noexcept { return static_cast<Function*>(PrevSibling()); }
    inline bool IsEmpty() const noexcept { return basicBlocks.IsEmpty(); }
    BasicBlock* GetBasicBlock(std::int32_t id) const noexcept;
    BasicBlock* CreateBasicBlock();
    BasicBlock* AddBasicBlock(const soul::ast::Span& span, std::int32_t id, IntermediateContext* context);
    void AddBasicBlock(BasicBlock* basicBlock);
    inline util::Container* BasicBlocks() noexcept { return &basicBlocks; }
    void InsertBasicBlockBefore(BasicBlock* basicBlockToInsert, BasicBlock* before);
    void InsertBasicBlockAfter(BasicBlock* basicBlockToInsert, BasicBlock* after);
    std::unique_ptr<BasicBlock> RemoveBasicBlock(BasicBlock* block);
    inline BasicBlock* FirstBasicBlock() noexcept { return static_cast<BasicBlock*>(basicBlocks.FirstChild()); }
    inline BasicBlock* LastBasicBlock() noexcept { return static_cast<BasicBlock*>(basicBlocks.LastChild()); }
    const soul::ast::Span& Span() const noexcept { return span; }
    inline FunctionType* GetType() const noexcept { return type; }
    inline const std::string& Name() const noexcept { return name; }
    std::string ToString() const override { return "@" + name; }
    RegValue* GetRegValue(std::int32_t reg) const noexcept;
    RegValue* GetRegRef(const soul::ast::Span& span, Type* type, std::int32_t reg, IntermediateContext* context) const;
    RegValue* MakeRegValue(const soul::ast::Span& span, Type* type, std::int32_t reg, IntermediateContext* context);
    RegValue* MakeNextRegValue(const soul::ast::Span& span, Type* type, IntermediateContext* context);
    void MapRegValue(RegValue* regValue);
    void RemoveRegValue(std::int32_t reg);
    Instruction* GetInstruction(std::int32_t reg) const noexcept;
    void MapInstruction(std::int32_t reg, Instruction* inst, IntermediateContext* context);
    int NumBasicBlocks() const noexcept;
    inline const std::vector<BasicBlock*>& RetBlocks() const noexcept { return retBlocks; }
    void AddRetBlock(BasicBlock* retBlock);
    void AddEntryAndExitBlocks();
    void RemoveEntryAndExitBlocks();
    inline void SetNextRegNumber(std::int32_t nextRegNumber_) noexcept { nextRegNumber = nextRegNumber_; }
    inline std::int32_t NextRegNumber() const noexcept { return nextRegNumber; }
    inline std::int32_t GetNextRegNumber() noexcept { return nextRegNumber++; }
    inline std::int32_t GetNextBasicBlockNumber() noexcept { return nextBBNumber++; }
    Value* GetParam(int index) const;
    void SetNumbers();
    void MoveRegValues(Function* toFunction);
    inline void SetMdId(int mdId_) noexcept { mdId = mdId_; }
    inline int MdId() const noexcept { return mdId; }
    void SetMetadataRef(MetadataRef* metadataRef_) noexcept { metadataRef = metadataRef_; }
    inline MetadataRef* GetMetadataRef() const noexcept { return metadataRef; }
    void SetComment(const std::string& comment_);
    inline const std::string& Comment() const noexcept { return comment; }
    std::string ResolveFullName() const;
    void Write(util::CodeFormatter& formatter);
private:
    FunctionFlags flags;
    soul::ast::Span span;
    FunctionType* type;
    std::unique_ptr<BasicBlock> entryBlock;
    std::string name;
    std::vector<Instruction*> params;
    MetadataRef* metadataRef;
    std::string comment;
    util::Container basicBlocks;
    std::map<std::int32_t, BasicBlock*> basicBlockMap;
    std::map<std::int32_t, RegValue*> regValueMap;
    std::map<std::int32_t, Instruction*> instructionMap;
    std::vector<std::unique_ptr<RegValue>> regValues;
    std::vector<BasicBlock*> retBlocks;
    std::int32_t nextRegNumber;
    std::int32_t nextBBNumber;
    int mdId;
};

class Code : public util::Component
{
public:
    Code() noexcept;
    Code(const Code&) = delete;
    Code& operator=(const Code&) = delete;
    inline IntermediateContext* GetContext() const noexcept { return context; }
    inline void SetContext(IntermediateContext* context_) noexcept { context = context_; }
    inline util::Container* Functions() const noexcept { return const_cast<util::Container*>(&functions); }
    inline Function* CurrentFunction() const noexcept { return currentFunction; }
    void SetCurrentFunction(Function* function) noexcept;
    Function* GetOrInsertFunction(const std::string& functionId, FunctionType* functionType);
    Function* GetFunction(const std::string& functionId) const noexcept;
    Function* AddFunctionDefinition(const soul::ast::Span& span, FunctionType* functionType, const std::string& functionId, bool inline_, bool linkOnce, bool chid,
        bool createEntry, MetadataRef* metadataRef);
    Function* AddFunctionDeclaration(const soul::ast::Span& span, FunctionType* functionType, const std::string& functionId);
    inline Function* FirstFunction() noexcept { return static_cast<Function*>(functions.FirstChild()); }
    inline Function* LastFunction() noexcept { return static_cast<Function*>(functions.LastChild()); }
    void VisitFunctions(Visitor& visitor);
    inline int TotalFunctions() const noexcept { return totalFunctions; }
private:
    IntermediateContext* context;
    Function* currentFunction;
    util::Container functions;
    std::map<std::string, Function*> functionMap;
    int totalFunctions;
};

} // otava::intermediate
