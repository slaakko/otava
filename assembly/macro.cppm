// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.assembly.macro;

import otava.assembly.value;
import std;

export namespace otava::assembly {

class Macro : public Value
{
public:
    Macro(const std::string& name_);
    void SetValue(Value* value_);
    inline Value* GetValue() const noexcept { return value; }
    std::string ToString() const override;
private:
    Value* value;
};

} // namespace otava::assembly
