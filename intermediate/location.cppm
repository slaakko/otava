// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.location;

import otava.assembly.context;
import otava.assembly.macro;
import otava.assembly.reg;
import soul.ast.span;
import std;

export namespace otava::intermediate {

class IntermediateContext;

enum class Locations : int
{
    none = 0, reg = 1 << 0, frame = 1 << 1
};

constexpr Locations operator|(Locations left, Locations right) noexcept
{
    return Locations(int(left) | int(right));
}

constexpr Locations operator&(Locations left, Locations right) noexcept
{
    return Locations(int(left) & int(right));
}

constexpr Locations operator~(Locations locs) noexcept
{
    return Locations(~int(locs));
}

struct FrameLocation
{
    FrameLocation() noexcept : reg(otava::assembly::RegisterGroupKind::rbp), index(-1), offset(0), size(0), macro(nullptr), level(-1) {}
    FrameLocation(otava::assembly::RegisterGroupKind reg_, int index_, std::int64_t offset_, std::int64_t size_) noexcept :
        reg(reg_), index(index_), offset(offset_), size(size_), macro(nullptr), level(-1) {
    }
    void Dump();
    inline bool Valid() const noexcept { return index != -1; }
    inline bool IsParentFrameLocation() const noexcept { return level >= 0; }
    otava::assembly::RegisterGroupKind reg;
    int index;
    std::int64_t offset;
    std::int64_t size;
    otava::assembly::Macro* macro;
    int level;
};

struct ArgLocation
{
    ArgLocation(int index_, std::int64_t offset_, std::int64_t size_) noexcept :
        reg(otava::assembly::RegisterGroupKind::rsp), index(index_), offset(offset_), size(size_) {
    }
    otava::assembly::RegisterGroupKind reg;
    int index;
    std::int64_t offset;
    std::int64_t size;
};

class Frame
{
public:
    Frame() noexcept;
    FrameLocation AllocateParamLocation(std::int64_t size, otava::assembly::AssemblyContext* assemblyContext);
    FrameLocation AllocateFrameLocation(std::int64_t size);
    FrameLocation GetFrameLocation(int index, const soul::ast::Span& span, IntermediateContext* context) const;
    FrameLocation GetParentFrameLocation(const soul::ast::Span& span, IntermediateContext* context) const;
    inline void SetRbxPushed() noexcept { rbxPushed = true; }
    inline bool RbxPushed() const noexcept { return rbxPushed; }
    std::int64_t Size() const noexcept;
    inline std::int64_t CalleeParamAreaSize() const noexcept { return calleeParamAreaSize; }
    inline std::int64_t XMMSaveRegSize() const noexcept { return 16 * numUsedXMMRegs; }
    inline void SetCalleeParamAreaSize(std::int64_t calleeParamAreaSize_) noexcept { calleeParamAreaSize = calleeParamAreaSize_; }
    void SetMacroValues(otava::assembly::AssemblyContext* assemblyContext);
    inline void SetNumUsedXMMRegs(int numUsedXMMRegs_) noexcept { numUsedXMMRegs = numUsedXMMRegs_; }
    inline int GetNumUsedXMMRegs() const noexcept { return numUsedXMMRegs; }
    void Dump();
    inline bool IsPFrame() const noexcept { return isPFrame; }
    inline void SetPFrame() noexcept { isPFrame = true; }
private:
    std::vector<FrameLocation> paramLocations;
    std::vector<FrameLocation> frameLocations;
    std::int64_t calleeParamAreaSize;
    int numUsedXMMRegs;
    bool rbxPushed;
    bool isPFrame;
};

class CallFrame
{
public:
    CallFrame() noexcept;
    void AllocateArgLocation(std::int64_t size);
    ArgLocation GetArgLocation(int index) noexcept;
private:
    std::vector<ArgLocation> argLocations;
};

} // otava::intermediate
