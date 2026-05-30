// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.assembly.value;

import std;

export namespace otava::assembly {

enum class ValueKind : std::int32_t
{
    integerLiteral, floatLiteral, doubleLiteral, stringLiteral, symbol, macro, reg, binaryExpr, content, sizePrefix
};

const int maxSplitCount = 16;

class Value
{
public:
    Value(ValueKind kind_, const std::string& name_);
    virtual ~Value();
    inline ValueKind Kind() const { return kind; }
    inline bool IsIntegerLiteral() const noexcept { return kind == ValueKind::integerLiteral; }
    inline bool IsFloatLiteral() const noexcept { return kind == ValueKind::floatLiteral; }
    inline bool IsDoubletLiteral() const noexcept { return kind == ValueKind::doubleLiteral; }
    inline bool IsStringLiteral() const noexcept { return kind == ValueKind::stringLiteral; }
    inline bool IsSymbol() const noexcept { return kind == ValueKind::symbol; }
    inline bool IsMacro() const noexcept { return kind == ValueKind::macro; }
    inline bool IsRegister() const noexcept { return kind == ValueKind::reg; }
    inline const std::string& Name() const noexcept { return name; }
    void SetName(const std::string& name_);
    virtual std::string ToString() const { return Name(); }
    int Length() const noexcept;
    virtual bool CanSplit() const noexcept { return false; }
    virtual Value* Split(int length) { return nullptr; }
    virtual bool IsEmpty() const noexcept { return false; }
private:
    ValueKind kind;
    std::string name;
};

} // namespace otava::assembly
