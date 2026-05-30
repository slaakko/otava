module otava.assembly.instruction;

import otava.assembly.context;
import otava.assembly.value;
import otava.assembly.reg;
import otava.assembly.literal;
import util.text_util;

namespace otava::assembly {

// .allocstack STACK_SIZE
// .pushreg REG
// .savereg REG, OFFSET
// .savexmm128 XMMREG, OFFSET
// .setframe REG, OFFSET
// .endprolog

std::string OpCodeStr(OpCode opCode)
{
    switch (opCode)
    {
    case OpCode::ADC: return "adc";
    case OpCode::ADCX: return "adcx";
    case OpCode::ADD: return "add";
    case OpCode::ADOX: return "adox";
    case OpCode::AND: return "and";
    case OpCode::ANDN: return "andn";
    case OpCode::BEXTR: return "bextr";
    case OpCode::BLCFILL: return "blcfill";
    case OpCode::BLCI: return "blci";
    case OpCode::BLCIC: return "blcic";
    case OpCode::BLCMSK: return "blcmsk";
    case OpCode::BLCS: return "blcs";
    case OpCode::BLSFILL: return "blsfill";
    case OpCode::BLSI: return "blsi";
    case OpCode::BLSIC: return "blsic";
    case OpCode::BLSMSK: return "blsmsk";
    case OpCode::BLSR: return "blsr";
    case OpCode::BOUND: return "bound";
    case OpCode::BSF: return "bsf";
    case OpCode::BSR: return "bsr";
    case OpCode::BSWAP: return "bswap";
    case OpCode::BT: return "bt";
    case OpCode::BTC: return "btc";
    case OpCode::BTR: return "btr";
    case OpCode::BTS: return "bts";
    case OpCode::BZHI: return "bzhi";
    case OpCode::CALL: return "call";
    case OpCode::CBW: return "cbw";
    case OpCode::CWDE: return "cwde";
    case OpCode::CDQE: return "cdqe";
    case OpCode::CWD: return "cwd";
    case OpCode::CDQ: return "cdq";
    case OpCode::CQO: return "cqo";
    case OpCode::CLC: return "clc";
    case OpCode::CLD: return "cld";
    case OpCode::CLFLUSH: return "clflush";
    case OpCode::CLFLUSHOPT: return "clflushopt";
    case OpCode::CLZERO: return "clzero";
    case OpCode::CMC: return "cmc";
    case OpCode::CMOVO: return "cmovo";
    case OpCode::CMOVNO: return "cmovno";
    case OpCode::CMOVB: return "cmovb";
    case OpCode::CMOVC: return "cmovc";
    case OpCode::CMOVNAE: return "cmovnae";
    case OpCode::CMOVNB: return "cmovnb";
    case OpCode::CMOVNC: return "cmovnc";
    case OpCode::CMOVAE: return "cmovae";
    case OpCode::CMOVZ: return "cmovz";
    case OpCode::CMOVE: return "cmove";
    case OpCode::CMOVNZ: return "cmovnz";
    case OpCode::CMOVNE: return "cmovne";
    case OpCode::CMOVBE: return "cmovbe";
    case OpCode::CMOVNA: return "cmovna";
    case OpCode::CMOVNBE: return "cmovnbe";
    case OpCode::CMOVA: return "cmova";
    case OpCode::CMOVS: return "cmovs";
    case OpCode::CMOVNS: return "cmovns";
    case OpCode::CMOVP: return "cmovp";
    case OpCode::CMOVPE: return "cmovpe";
    case OpCode::CMOVNP: return "cmovnp";
    case OpCode::CMOVPO: return "cmovpo";
    case OpCode::CMOVL: return "cmovl";
    case OpCode::CMOVNGE: return "cmovnge";
    case OpCode::CMOVNL: return "cmovnl";
    case OpCode::CMOVGE: return "cmovge";
    case OpCode::CMOVLE: return "cmovle";
    case OpCode::CMOVNG: return "cmovng";
    case OpCode::CMOVNLE: return "cmovnle";
    case OpCode::CMOVG: return "cmovg";
    case OpCode::CMP: return "cmp";
    case OpCode::CMPS: return "cmps";
    case OpCode::CMPSB: return "cmpsb";
    case OpCode::CMPSW: return "cmpsw";
    case OpCode::CMPSD: return "cmpsd";
    case OpCode::CMPSQ: return "cmpsq";
    case OpCode::CMPXCHG: return "cmpxchg";
    case OpCode::CMPXCHG8B: return "cmpxchgb";
    case OpCode::CMPXCHG16B: return "cmpxchg16b";
    case OpCode::CPUID: return "cpuid";
    case OpCode::CRC32: return "crc32";
    case OpCode::DEC: return "dec";
    case OpCode::DIV: return "div";
    case OpCode::ENTER: return "enter";
    case OpCode::IDIV: return "idiv";
    case OpCode::IMUL: return "imul";
    case OpCode::IN: return "in";
    case OpCode::INC: return "inc";
    case OpCode::INS: return "ins";
    case OpCode::INSB: return "insb";
    case OpCode::INSW: return "insw";
    case OpCode::INSD: return "insd";
    case OpCode::INT: return "int";
    case OpCode::INTO: return "into";
    case OpCode::JO: return "jo";
    case OpCode::JNO: return "jno";
    case OpCode::JB: return "jb";
    case OpCode::JC: return "jc";
    case OpCode::JNAE: return "jnae";
    case OpCode::JNB: return "jnb";
    case OpCode::JNC: return "jnc";
    case OpCode::JAE: return "jae";
    case OpCode::JZ: return "jz";
    case OpCode::JE: return "je";
    case OpCode::JNZ: return "jnz";
    case OpCode::JNE: return "jne";
    case OpCode::JBE: return "jbe";
    case OpCode::JNA: return "jna";
    case OpCode::JNBE: return "jnbe";
    case OpCode::JA: return "ja";
    case OpCode::JS: return "js";
    case OpCode::JNS: return "jns";
    case OpCode::JP: return "jp";
    case OpCode::JPE: return "jpe";
    case OpCode::JNP: return "jnp";
    case OpCode::JPO: return "jpo";
    case OpCode::JL: return "jl";
    case OpCode::JNGE: return "jnge";
    case OpCode::JNL: return "jnl";
    case OpCode::JGE: return "jge";
    case OpCode::JLE: return "jle";
    case OpCode::JNG: return "jng";
    case OpCode::JNLE: return "jnle";
    case OpCode::JG: return "jg";
    case OpCode::JCXZ: return "jcxz";
    case OpCode::JECXZ: return "jecxz";
    case OpCode::JRCXZ: return "jrcxz";
    case OpCode::JMP: return "jmp";
    case OpCode::LAHF: return "lahf";
    case OpCode::LDS: return "lds";
    case OpCode::LES: return "les";
    case OpCode::LFS: return "lfs";
    case OpCode::LGS: return "lgs";
    case OpCode::LSS: return "lss";
    case OpCode::LEA: return "lea";
    case OpCode::LEAVE: return "leave";
    case OpCode::LFENCE: return "lfence";
    case OpCode::LLWPCB: return "llwpcb";
    case OpCode::LODS: return "lods";
    case OpCode::LODSB: return "lodsb";
    case OpCode::LODSW: return "lodsw";
    case OpCode::LODSD: return "lodsd";
    case OpCode::LODSQ: return "lodsq";
    case OpCode::LOOP: return "loop";
    case OpCode::LOOPE: return "loope";
    case OpCode::LOOPNE: return "loopne";
    case OpCode::LOOPNZ: return "loopnz";
    case OpCode::LOOPZ: return "loopz";
    case OpCode::LWPINS: return "lwpins";
    case OpCode::LWPVAL: return "lwpval";
    case OpCode::LZCNT: return "lzcnt";
    case OpCode::MCOMMIT: return "mcommit";
    case OpCode::MFENCE: return "mfence";
    case OpCode::MONITORX: return "monitorx";
    case OpCode::MOV: return "mov";
    case OpCode::MOVBE: return "movbe";
    case OpCode::MOVD: return "movd";
    case OpCode::MOVMSKPD: return "movmskpd";
    case OpCode::MOVMSKPS: return "movmskps";
    case OpCode::MOVNTI: return "movnti";
    case OpCode::MOVS: return "movs";
    case OpCode::MOVSB: return "movsb";
    case OpCode::MOVSW: return "movsw";
    case OpCode::MOVSD: return "movsd";
    case OpCode::MOVSQ: return "movsq";
    case OpCode::MOVSX: return "movsx";
    case OpCode::MOVSXD: return "movsxd";
    case OpCode::MOVZX: return "movzx";
    case OpCode::MUL: return "mul";
    case OpCode::MULX: return "mulx";
    case OpCode::MWAITX: return "mwaitx";
    case OpCode::NEG: return "neg";
    case OpCode::NOP: return "nop";
    case OpCode::NOT: return "not";
    case OpCode::OR: return "or";
    case OpCode::OUT: return "out";
    case OpCode::OUTS: return "outs";
    case OpCode::OUTSB: return "outsb";
    case OpCode::OUTSW: return "outsw";
    case OpCode::OUTSD: return "outsd";
    case OpCode::PAUSE: return "pause";
    case OpCode::PDEP: return "pdep";
    case OpCode::PEXT: return "pext";
    case OpCode::POP: return "pop";
    case OpCode::POPA: return "popa";
    case OpCode::POPAD: return "popad";
    case OpCode::POPCNT: return "popcnt";
    case OpCode::POPF: return "popf";
    case OpCode::POPFD: return "popfd";
    case OpCode::POPFQ: return "popfq";
    case OpCode::PREFETCH: return "prefetch";
    case OpCode::PREFETCHW: return "prefetchw";
    case OpCode::PUSH: return "push";
    case OpCode::PUSHA: return "pusha";
    case OpCode::PUSHAD: return "pushad";
    case OpCode::PUSHF: return "pushf";
    case OpCode::PUSHFD: return "pushfd";
    case OpCode::RCL: return "rcl";
    case OpCode::RCR: return "rcr";
    case OpCode::RDFSBASE: return "rdfsbase";
    case OpCode::RDGSBASE: return "rdgsbase";
    case OpCode::RDPID: return "rdpid";
    case OpCode::RDPRU: return "rdpru";
    case OpCode::RDRAND: return "rdrand";
    case OpCode::RDSEED: return "rdseed";
    case OpCode::RET: return "ret";
    case OpCode::RETF: return "retf";
    case OpCode::ROL: return "rol";
    case OpCode::ROR: return "ror";
    case OpCode::RORX: return "rorx";
    case OpCode::SAHF: return "sahf";
    case OpCode::SAL: return "sal";
    case OpCode::SHL: return "shl";
    case OpCode::SAR: return "sar";
    case OpCode::SARX: return "sarx";
    case OpCode::SBB: return "sbb";
    case OpCode::SCAS: return "scas";
    case OpCode::SCASB: return "scasb";
    case OpCode::SCASW: return "scasw";
    case OpCode::SCASD: return "scasd";
    case OpCode::SCASQ: return "scasq";
    case OpCode::SETO: return "seto";
    case OpCode::SETNO: return "setno";
    case OpCode::SETB: return "setb";
    case OpCode::SETC: return "setc";
    case OpCode::SETNAE: return "setnae";
    case OpCode::SETNB: return "setnb";
    case OpCode::SETNC: return "setnc";
    case OpCode::SETAE: return "setae";
    case OpCode::SETZ: return "setz";
    case OpCode::SETE: return "sete";
    case OpCode::SETNZ: return "setnz";
    case OpCode::SETNE: return "setne";
    case OpCode::SETBE: return "setbe";
    case OpCode::SETNA: return "setna";
    case OpCode::SETNBE: return "setnbe";
    case OpCode::SETA: return "seta";
    case OpCode::SETS: return "sets";
    case OpCode::SETNS: return "setns";
    case OpCode::SETP: return "setp";
    case OpCode::SETPE: return "setpe";
    case OpCode::SETNP: return "setnp";
    case OpCode::SETPO: return "setpo";
    case OpCode::SETL: return "setl";
    case OpCode::SETNGE: return "setnge";
    case OpCode::SETNL: return "setnl";
    case OpCode::SETGE: return "setge";
    case OpCode::SETLE: return "setle";
    case OpCode::SETNG: return "setng";
    case OpCode::SETNLE: return "setnle";
    case OpCode::SETG: return "setg";
    case OpCode::SFENCE: return "sfence";
    case OpCode::SHLD: return "shld";
    case OpCode::SHLX: return "shlx";
    case OpCode::SHR: return "shr";
    case OpCode::SHRD: return "shrd";
    case OpCode::SHRX: return "shrx";
    case OpCode::SLWPCB: return "slwpcb";
    case OpCode::STC: return "stc";
    case OpCode::STD: return "std";
    case OpCode::STOS: return "stos";
    case OpCode::STOSB: return "stosb";
    case OpCode::STOSW: return "stosw";
    case OpCode::STOSD: return "stosd";
    case OpCode::STOSQ: return "stosq";
    case OpCode::SUB: return "sub";
    case OpCode::T1MSKC: return "t1mskc";
    case OpCode::TEST: return "test";
    case OpCode::TZCNT: return "tzcnt";
    case OpCode::TZMSK: return "tzmsk";
    case OpCode::XADD: return "xadd";
    case OpCode::XCHG: return "xchg";
    case OpCode::XLAT: return "xlat";
    case OpCode::XLATB: return "xlatb";
    case OpCode::XOR: return "xor";
    case OpCode::MOVDQA: return "movdqa";
    case OpCode::MOVQ: return "movq";
    case OpCode::MOVSS: return "movss";
    case OpCode::ADDSD: return "addsd";
    case OpCode::ADDSS: return "addss";
    case OpCode::SUBSD: return "subsd";
    case OpCode::SUBSS: return "subss";
    case OpCode::MULSD: return "mulsd";
    case OpCode::MULSS: return "mulss";
    case OpCode::DIVSD: return "divsd";
    case OpCode::DIVSS: return "divss";
    case OpCode::UCOMISD: return "ucomisd";
    case OpCode::UCOMISS: return "ucomiss";
    case OpCode::COMISD: return "comisd";
    case OpCode::COMISS: return "comiss";
    case OpCode::XORPD: return "xorpd";
    case OpCode::XORPS: return "xorps";
    case OpCode::CVTSI2SD: return "cvtsi2sd";
    case OpCode::CVTSI2SS: return "cvtsi2ss";
    case OpCode::CVTTSS2SI: return "cvttss2si";
    case OpCode::CVTSS2SD: return "cvtss2sd";
    case OpCode::CVTSD2SS: return "cvtsd2ss";
    case OpCode::CVTTSD2SI: return "cvttsd2si";
    case OpCode::DB: return "DB";
    case OpCode::DW: return "DW";
    case OpCode::DD: return "DD";
    case OpCode::DQ: return "DQ";
    case OpCode::REAL4: return "REAL4";
    case OpCode::REAL8: return "REAL8";
    case OpCode::ALLOCSTACK: return ".allocstack";
    case OpCode::PUSHREG: return ".pushreg";
    case OpCode::SAVEREG: return ".savereg";
    case OpCode::SAVEXMM128: return ".savexmm128";
    case OpCode::SETFRAME: return ".setframe";
    case OpCode::ENDPROLOG: return ".endprolog";
    }
    return "<inst>";
}

Instruction::Instruction(OpCode opCode_) : opCode(opCode_), nocolon(false), writeln(false)
{
}

Instruction* Instruction::Clone() const
{
    Instruction* clone = new Instruction(opCode);
    for (auto operand : operands)
    {
        clone->AddOperand(operand);
    }
    return clone;
}

void Instruction::SetLabel(const std::string& label_)
{
    label = label_;
}

void Instruction::AddOperand(Value* operand)
{
    if (!operand->IsEmpty())
    {
        operands.push_back(operand);
    }
}

void Instruction::Write(util::CodeFormatter& formatter)
{
    if (!label.empty())
    {
        formatter.DecIndent();
        std::string c(":");
        if (nocolon)
        {
            c = " ";
        }
        std::string lbl = util::Format(label + c, 8, util::FormatWidth::min, util::FormatJustify::left);
        formatter.Write(lbl);
        if (writeln)
        {
            formatter.WriteLine();
            formatter.IncIndent();
        }
    }
    formatter.Write(util::Format(OpCodeStr(opCode), 16));
    bool first = true;
    for (const auto* operand : operands)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        formatter.Write(operand->ToString());
    }
    formatter.WriteLine();
    if (!label.empty() && !writeln)
    {
        formatter.IncIndent();
    }
}

int Instruction::Length() const noexcept
{
    int length = 0;
    if (!label.empty())
    {
        std::string c = ":";
        std::string f = util::Format(label + c, 8, util::FormatWidth::min, util::FormatJustify::left);
        length += f.length();
    }
    else
    {
        length += 8;
    }
    std::string f = util::Format(OpCodeStr(opCode), 16);
    length += f.length();
    bool first = true;
    for (const auto* operand : operands)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            length += 2;
        }
        length += operand->Length();
    }
    return length;
}

Instruction* MakeInst(OpCode opCode, Value* operand)
{
    Instruction* inst = new Instruction(opCode);
    inst->AddOperand(operand);
    return inst;
}

Instruction* MakeInst(OpCode opCode, Value* left, Value* right)
{
    Instruction* inst = new Instruction(opCode);
    inst->AddOperand(left);
    inst->AddOperand(right);
    return inst;
}

} // namespace otava::assembly
