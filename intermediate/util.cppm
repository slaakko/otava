// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.util;

import otava.intermediate.code;
import otava.intermediate.data;
import std;

export namespace otava::intermediate {

class Function;

template<typename PtrT>
inline void AddPtrToSet(PtrT* ptr, std::vector<PtrT*>& set)
{
    if (std::find(set.cbegin(), set.cend(), ptr) == set.cend())
    {
        set.push_back(ptr);
    }
}

template<typename PtrT>
bool RemovePtrFromSet(PtrT* ptr, std::vector<PtrT*>& set)
{
    auto it = std::remove(set.begin(), set.end(), ptr);
    if (it == set.cend())
    {
        return false;
    }
    else
    {
        set.erase(it, set.end());
        return true;
    }
}

template<typename PtrT>
inline bool PtrInSet(PtrT* ptr, const std::vector<PtrT*>& set) noexcept
{
    return std::find(set.cbegin(), set.cend(), ptr) != set.cend();
}

void ReplaceInstructionWithValue(Instruction* inst, Value* value);

void ReplaceInstructionWithInstruction(Instruction* oldInst, Instruction* newInst);

bool IsPowerOfTwo(std::uint64_t n, int& shift);

} // otava::intermediate
