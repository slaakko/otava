// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.assembly.instruction;

import util.code_formatter;
import util.component;
import std;

export namespace otava::assembly {

class Value;
class AssemblyContext;

enum class OpCode
{
    ADC, ADCX, ADD, ADOX, AND, ANDN,
    BEXTR, BLCFILL, BLCI, BLCIC, BLCMSK, BLCS, BLSFILL, BLSI, BLSIC, BLSMSK, BLSR, BOUND, BSF, BSR, BSWAP, BT, BTC, BTR, BTS, BZHI,
    CALL, CBW, CWDE, CDQE, CWD, CDQ, CQO, CLC, CLD, CLFLUSH, CLFLUSHOPT, CLZERO, CMC,
    CMOVO, CMOVNO, CMOVB, CMOVC, CMOVNAE, CMOVNB, CMOVNC, CMOVAE, CMOVZ, CMOVE, CMOVNZ, CMOVNE, CMOVBE, CMOVNA, CMOVNBE, CMOVA, CMOVS, CMOVNS, CMOVP, CMOVPE, CMOVNP,
    CMOVPO, CMOVL, CMOVNGE, CMOVNL, CMOVGE, CMOVLE, CMOVNG, CMOVNLE, CMOVG,
    CMP, CMPS, CMPSB, CMPSW, CMPSD, CMPSQ, CMPXCHG, CMPXCHG8B, CMPXCHG16B, CPUID, CRC32,
    DEC, DIV,
    ENTER,
    IDIV, IMUL, IN, INC, INS, INSB, INSW, INSD, INT, INTO,
    JO, JNO, JB, JC, JNAE, JNB, JNC, JAE, JZ, JE, JNZ, JNE, JBE, JNA, JNBE, JA, JS, JNS, JP, JPE, JNP, JPO, JL, JNGE, JNL, JGE, JLE, JNG, JNLE, JG,
    JCXZ, JECXZ, JRCXZ, JMP,
    LAHF, LDS, LES, LFS, LGS, LSS, LEA, LEAVE, LFENCE, LLWPCB, LODS, LODSB, LODSW, LODSD, LODSQ, LOOP, LOOPE, LOOPNE, LOOPNZ, LOOPZ, LWPINS, LWPVAL, LZCNT,
    MCOMMIT, MFENCE, MONITORX, MOV, MOVBE, MOVD, MOVMSKPD, MOVMSKPS, MOVNTI, MOVS, MOVSB, MOVSW, MOVSD, MOVSQ, MOVSX, MOVSXD, MOVZX, MUL, MULX, MWAITX,
    NEG, NOP, NOT, OR, OUT, OUTS, OUTSB, OUTSW, OUTSD,
    PAUSE, PDEP, PEXT, POP, POPA, POPAD, POPCNT, POPF, POPFD, POPFQ, PREFETCH, PREFETCHW, PUSH, PUSHA, PUSHAD, PUSHF, PUSHFD, PUSHFQ,
    RCL, RCR, RDFSBASE, RDGSBASE, RDPID, RDPRU, RDRAND, RDSEED, RET, RETF, ROL, ROR, RORX,
    SAHF, SAL, SHL, SAR, SARX, SBB, SCAS, SCASB, SCASW, SCASD, SCASQ,
    SETO, SETNO, SETB, SETC, SETNAE, SETNB, SETNC, SETAE, SETZ, SETE, SETNZ, SETNE, SETBE, SETNA, SETNBE, SETA, SETS, SETNS, SETP, SETPE, SETNP, SETPO, SETL, SETNGE, SETNL, SETGE,
    SETLE, SETNG, SETNLE, SETG,
    SFENCE, SHLD, SHLX, SHR, SHRD, SHRX, SLWPCB, STC, STD, STOS, STOSB, STOSW, STOSD, STOSQ, SUB,
    T1MSKC, TEST, TZCNT, TZMSK,
    XADD, XCHG, XLAT, XLATB, XOR,
    // floating-point support:
    MOVDQA, MOVQ, MOVSS, ADDSD, ADDSS, SUBSD, SUBSS, MULSD, MULSS, DIVSD, DIVSS, UCOMISD, UCOMISS, COMISD, COMISS,
    XORPD, XORPS, CVTSI2SD, CVTSI2SS, CVTTSS2SI, CVTSS2SD, CVTSD2SS, CVTTSD2SI,
    // data definition support:
    DB, DW, DD, DQ, REAL4, REAL8,
    ALLOCSTACK, PUSHREG, SAVEREG, SAVEXMM128, SETFRAME, ENDPROLOG
};

std::string OpCodeStr(OpCode opCode);

class Instruction : public util::Component
{
public:
    Instruction(OpCode opCode_);
    Instruction* Clone() const;
    void SetLabel(const std::string& label_);
    inline const std::string& Label() const noexcept { return label; }
    inline void SetNoColon() noexcept { nocolon = true; }
    void AddOperand(Value* operand);
    inline bool HasOperands() const noexcept { return !operands.empty(); }
    inline OpCode GetOpCode() const noexcept { return opCode; }
    inline const std::vector<Value*>& Operands() const noexcept { return operands; }
    void Write(util::CodeFormatter& formatter);
    int Length() const noexcept;
    inline void SetWriteln() noexcept { writeln = true; }
    inline Instruction* Next() const noexcept { return static_cast<Instruction*>(NextSibling()); }
    inline Instruction* Prev() const noexcept { return static_cast<Instruction*>(PrevSibling()); }
private:
    std::string label;
    bool nocolon;
    OpCode opCode;
    std::vector<Value*> operands;
    bool writeln;
};

Instruction* MakeInst(OpCode opCode, Value* operand);
Instruction* MakeInst(OpCode opCode, Value* left, Value* right);

void FreeGlobalRegs(AssemblyContext* context, Instruction* inst);

} // namespace otava::assembly
