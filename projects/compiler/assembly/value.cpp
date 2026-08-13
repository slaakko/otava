// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.assembly.value;

namespace otava::assembly {

Value::Value(ValueKind kind_, const std::string& name_) : kind(kind_), name(name_)
{
}

Value::~Value()
{
}

void Value::SetName(const std::string& name_)
{
    name = name_;
}

int Value::Length() const noexcept
{
    std::string s = ToString();
    return s.length();
}

} // otava::assembly
