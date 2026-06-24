// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.optimizer;

import otava.intermediate.context;
import std;

export namespace otava::optimizer {

enum class Optimizations : std::uint64_t
{
    none = 0,
    xor_ = 1 << 0,
    jump = 1 << 1,
    removeNops = 1 << 2,
    jumpTableSwitch = 1 << 3,
    arithmetic = 1 << 4,
    identity = 1 << 5,
    deadCodeElimination = 1 << 6,
    moveLocalsToEntryBlock = 1 << 7,
    inlining = 1 << 8,
    o0 = none,
    o1 = xor_ | jump | removeNops | jumpTableSwitch | arithmetic | identity | deadCodeElimination | moveLocalsToEntryBlock,
    o2 = xor_ | jump | removeNops | jumpTableSwitch | arithmetic | identity | deadCodeElimination | moveLocalsToEntryBlock | inlining,
    o3 = xor_ | jump | removeNops | jumpTableSwitch | arithmetic | identity | deadCodeElimination | moveLocalsToEntryBlock | inlining
};

constexpr Optimizations operator|(Optimizations left, Optimizations right) noexcept
{
    return Optimizations(static_cast<std::uint64_t>(left) | static_cast<std::uint64_t>(right));
}

constexpr Optimizations operator&(Optimizations left, Optimizations right) noexcept
{
    return Optimizations(static_cast<std::uint64_t>(left) & static_cast<std::uint64_t>(right));
}

constexpr Optimizations operator~(Optimizations operand) noexcept
{
    return Optimizations(~static_cast<std::uint64_t>(operand));
}

Optimizations currentOptimizations = Optimizations::o2;

inline void SetOptimizations(Optimizations optimizations) noexcept
{
    currentOptimizations = optimizations;
}

inline bool HasOptimization(Optimizations optimization) noexcept
{
    return (currentOptimizations & optimization) != Optimizations::none;
}

class Optimizer
{
public:
    static Optimizer& Instance();
    void SetOptimizations(const std::string& optLevel);
    inline const std::string& OptLevel() const noexcept { return optLevel; }
private:
    Optimizer();
    std::map<std::string, Optimizations> optMap;
    std::string optLevel;
};

void Optimize(otava::intermediate::IntermediateContext* context);
void SetOptFn(const std::string& optFn_);

} // otava::optimizer
