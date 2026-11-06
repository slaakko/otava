// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.register_allocator;

import otava.intermediate.location;
import otava.assembly.reg;
import std;

export namespace otava::intermediate {

class Instruction;

struct SpillData
{
    SpillData() noexcept;
    otava::assembly::RegisterGroup* registerGroupToSpill;
    FrameLocation spillToFrameLocation;
    Instruction* instToSpill;
};

enum class RegisterAllocationAction
{
    none, spill, registerAllocated
};

class RegisterAllocator
{
public:
    RegisterAllocator();
    virtual ~RegisterAllocator();
    virtual RegisterAllocationAction Run(Instruction* inst) = 0;
    virtual const std::vector<SpillData>& GetSpillData() const noexcept = 0;
    virtual Locations GetLocations(Instruction* inst) const = 0;
    virtual otava::assembly::RegisterGroup* GetRegisterGroup(Instruction* inst) const noexcept = 0;
    virtual void AddRegisterLocation(Instruction* inst, otava::assembly::RegisterGroup* regGroup) = 0;
    virtual Frame& GetFrame() noexcept = 0;
    virtual FrameLocation GetFrameLocation(Instruction* inst) const noexcept = 0;
    virtual int LastActiveLocalRegGroup() const noexcept = 0;
    virtual void RemoveFromRegisterGroups(Instruction* inst) = 0;
};

} // otava::intermediate
