// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.ir_value_stack;

import otava.intermediate.data;
import std;

export namespace otava::symbols {

class IrValueStack
{
public:
    inline void Push(otava::intermediate::Value* value)
    {
        s.push_back(value);
    }
    inline otava::intermediate::Value* Pop()
    {
        otava::intermediate::Value* top = s.back();
        s.pop_back();
        return top;
    }
    inline void Dup()
    {
        s.push_back(s.back());
    }
    inline void Swap()
    {
        std::swap(s.back(), s[s.size() - 2]);
    }
    inline void Rotate()
    {
        std::swap(s[s.size() - 3], s[s.size() - 2]);
        std::swap(s.back(), s[s.size() - 2]);
    }
    inline bool IsEmpty() const noexcept
    {
        return s.empty();
    }
private:
    std::vector<otava::intermediate::Value*> s;
};

} // namespace otava::symbols
