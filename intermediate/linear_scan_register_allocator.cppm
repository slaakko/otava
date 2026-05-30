// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.linear_scan_register_allocator;

import otava.intermediate.register_allocator;
import std;

export namespace otava::intermediate {

class IntermediateContext;
class Function;
class Instruction;

struct LiveRange
{
    inline LiveRange() noexcept : start(-1), end(-1) {}
    inline LiveRange(int start_, int end_) noexcept : start(start_), end(end_) {}
    inline bool Valid() const noexcept { return start != -1 && end != -1; }
    int start;
    int end;
};

inline bool operator<(const LiveRange& left, const LiveRange& right) noexcept
{
    if (left.start < right.start) return true;
    if (left.start > right.start) return false;
    return left.end < right.end;
}

struct LiveRangeByStart
{
    inline bool operator()(const LiveRange& left, const LiveRange& right) const noexcept
    {
        if (left.start < right.start) return true;
        if (left.start > right.start) return false;
        return left.end < right.end;
    }
};

struct LiveRangeByEnd
{
    inline bool operator()(const LiveRange& left, const LiveRange& right) const noexcept
    {
        if (left.end < right.end) return true;
        if (left.end > right.end) return false;
        return left.start < right.start;
    }
};

LiveRange GetLiveRange(Instruction* inst);

class LinearScanRegisterAllocator : public RegisterAllocator
{
public:
    LinearScanRegisterAllocator(Function& function, IntermediateContext* context_);
    void AddLiveRange(const LiveRange& liveRange, Instruction* inst);
    void AddFreeRegGroupToPool(Instruction* inst);
    void RemoveFromActive(const LiveRange& range, bool integer);
    bool NoFreeRegs(bool floatingPoint) const;
    inline const std::set<LiveRange, LiveRangeByStart>& LiveRanges() const noexcept { return liveRanges; }
    inline const std::set<LiveRange, LiveRangeByEnd>& ActiveInteger() const noexcept { return activeInteger; }
    inline const std::set<LiveRange, LiveRangeByEnd>& ActiveFP() const noexcept { return activeFP; }
    FrameLocation GetFrameLocation(Instruction* inst) const noexcept override;
    otava::assembly::RegisterGroup* GetRegisterGroup(Instruction* inst) const noexcept override;
    void RemoveRegisterGroup(Instruction* inst);
    void AllocateRegister(Instruction* inst);
    void AllocateFrameLocation(Instruction* inst, bool spill);
    void Spill(Instruction* inst);
    LiveRange GetLiveRange(Instruction* inst) const noexcept;
    Frame& GetFrame() noexcept override { return frame; }
    const std::vector<Instruction*>& GetInstructions(const LiveRange& range) const noexcept;
    RegisterAllocationAction Run(Instruction* inst) override;
    Locations GetLocations(Instruction* inst) const override;
    void AddRegisterLocation(Instruction* inst, otava::assembly::RegisterGroup* regGroup) override;
    int LastActiveLocalRegGroup() const noexcept override;
    const std::vector<SpillData>& GetSpillData() const noexcept override;
    void RemoveFromRegisterGroups(Instruction* inst) override;
private:
    void ComputeLiveRanges(Function& function);
    void ExpireOldRanges(const LiveRange& range);
    Frame& GetPFrame(int level);
    Frame frame;
    std::vector<Frame> pframes;
    std::set<LiveRange, LiveRangeByStart> liveRanges;
    std::set<LiveRange, LiveRangeByEnd> activeInteger;
    std::set<LiveRange, LiveRangeByEnd> activeFP;
    std::map<Instruction*, FrameLocation> frameLocations;
    std::map<Instruction*, otava::assembly::RegisterGroup*> registerGroups;
    std::map<Instruction*, LiveRange> instructionRangeMap;
    std::map<LiveRange, std::vector<Instruction*>> rangeInstructionMap;
    std::map<Instruction*, Locations> locations;
    std::vector<SpillData> spillDataVec;
    IntermediateContext* context;
};

std::unique_ptr<LinearScanRegisterAllocator> CreateLinearScanRegisterAllocator(Function& function, IntermediateContext* context);

} // otava::intermediate
