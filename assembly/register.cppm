// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.assembly.reg;

import otava.assembly.value;
import std;

export namespace otava::assembly {

enum class RegisterKind
{
    al, bl, cl, dl, sil, dil, bpl, spl, r8b, r9b, r10b, r11b, r12b, r13b, r14b, r15b,
    ax, bx, cx, dx, si, di, bp, sp, r8w, r9w, r10w, r11w, r12w, r13w, r14w, r15w,
    eax, ebx, ecx, edx, esi, edi, ebp, esp, r8d, r9d, r10d, r11d, r12d, r13d, r14d, r15d,
    rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15,
    ah, bh, ch, dh,
    xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15,
    max
};

enum class RegisterGroupKind
{
    rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15,
    xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15,
    max
};

enum class GlobalReg : std::uint8_t
{
    none = 0, rax = 1 << 0, rbx = 1 << 1, rcx = 1 << 2, rdx = 1 << 3, r8 = 1 << 4, r9 = 1 << 5, r10 = 1 << 6, r11 = 1 << 7
};

constexpr GlobalReg operator|(GlobalReg left, GlobalReg right) noexcept
{
    return GlobalReg(static_cast<std::uint8_t>(left) | static_cast<std::uint8_t>(right));
}

constexpr GlobalReg operator&(GlobalReg left, GlobalReg right) noexcept
{
    return GlobalReg(static_cast<std::uint8_t>(left) & static_cast<std::uint8_t>(right));
}

constexpr GlobalReg operator~(GlobalReg reg) noexcept
{
    return GlobalReg(~std::uint8_t(reg));
}

GlobalReg RegisterGroupKindToGlobalReg(RegisterGroupKind groupKind);

std::string GlobalRegsToString(GlobalReg regs);

class Register : public Value
{
public:
    Register();
    Register(RegisterKind kind_, RegisterGroupKind group_, int size_);
    inline RegisterKind RegKind() const noexcept { return kind; }
    inline RegisterGroupKind Group() const noexcept { return group; }
    inline int Size() const noexcept { return size; }
    inline bool IsFloatingPointReg() const noexcept { return group >= RegisterGroupKind::xmm0 && group <= RegisterGroupKind::xmm15; }
private:
    RegisterKind kind;
    RegisterGroupKind group;
    int size;
};

const int highByteRegSize = 9;

class RegisterGroup
{
public:
    RegisterGroup();
    RegisterGroup(RegisterGroupKind kind_);
    inline RegisterGroupKind Kind() const noexcept { return kind; }
    const Register* GetReg(std::int64_t size) const;
    Register* GetReg(std::int64_t size);
    void SetReg(std::int64_t size, const Register& reg);
    bool IsLocal() const;
    inline bool IsFloatingPointReg() const noexcept { return kind >= RegisterGroupKind::xmm0 && kind <= RegisterGroupKind::xmm15; }
    inline bool IsVolatile() const noexcept { return !nonvolatile; }
    inline bool IsNonvolatile() const noexcept { return nonvolatile; }
    inline void SetNonvolatile(bool nonvolatile_) noexcept { nonvolatile = nonvolatile_; }
private:
    RegisterGroupKind kind;
    Register regs[16];
    bool nonvolatile;
};

class Registers
{
public:
    Registers();
    inline const RegisterGroup* GetRegisterGroup(RegisterGroupKind kind) const noexcept { return regGroups[int(kind)].get(); }
    inline RegisterGroup* GetRegisterGroup(RegisterGroupKind kind) noexcept { return regGroups[int(kind)].get(); }
private:
    std::vector<std::unique_ptr<RegisterGroup>> regGroups;
};

struct RegisterGroupLess
{
    bool operator()(RegisterGroup* left, RegisterGroup* right) const noexcept;
};

class RegisterPool
{
public:
    RegisterPool(Registers& registers_);
    void MakeChildFnRegisterPool();
    void AddLocalRegisterGroup(RegisterGroup* regGroup);
    void AddLocalXMMRegisterGroup(RegisterGroup* regGroup);
    RegisterGroup* GetLocalRegisterGroup();
    RegisterGroup* GetLocalXMMRegisterGroup();
    RegisterGroup* GetRegisterGroup(RegisterGroupKind regGroupKind, bool used);
    inline int LocalRegisterCount() const noexcept { return localRegisterCount; }
    inline int LocalXMMRegisterCount() const noexcept { return localXMMRegisterCount; }
    inline int NumFreeLocalRegisters() const noexcept { return localRegisterPool.size(); }
    inline int NumFreeLocalXMMRegisters() const noexcept { return localXMMRegisterPool.size(); }
    inline const std::set<RegisterGroup*, RegisterGroupLess>& UsedLocalRegs() const noexcept { return usedLocalRegs; }
    inline const std::set<RegisterGroup*, RegisterGroupLess>& UsedNonvolatileRegs() const noexcept { return usedNonvolatileRegs; }
    inline const std::set<RegisterGroup*, RegisterGroupLess>& UsedLocalXMMRegs() const noexcept { return usedLocalXMMRegs; }
    inline const std::set<RegisterGroup*, RegisterGroupLess>& UsedNonvolatileXMMRegs() const noexcept { return usedNonvolatileXMMRegs; }
    inline bool IsChildFnRegisterPool() const noexcept { return isChildFnRegisterPool; }
private:
    Registers& registers;
    int localRegisterCount;
    int localXMMRegisterCount;
    std::set<RegisterGroup*, RegisterGroupLess> localRegisterPool;
    std::set<RegisterGroup*, RegisterGroupLess> localXMMRegisterPool;
    std::map<RegisterGroupKind, RegisterGroup*> globalRegisterMap;
    GlobalReg usedGlobalRegs;
    std::set<RegisterGroup*, RegisterGroupLess> usedLocalRegs;
    std::set<RegisterGroup*, RegisterGroupLess> usedNonvolatileRegs;
    std::set<RegisterGroup*, RegisterGroupLess> usedLocalXMMRegs;
    std::set<RegisterGroup*, RegisterGroupLess> usedNonvolatileXMMRegs;
    bool isChildFnRegisterPool;
};

} // namespace otava::assembly
