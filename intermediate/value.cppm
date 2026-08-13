// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.value;

import otava.intermediate.types;
import soul.ast.span;
import std;

export namespace otava::intermediate {

enum class ValueKind
{
    boolValue, sbyteValue, byteValue, shortValue, ushortValue, intValue, uintValue, longValue, ulongValue, floatValue, doubleValue, nullValue, addressValue,
    arrayValue, structureValue, stringValue, stringArrayValue, conversionValue, clsIdValue, symbolValue, globalVariable, regValue, instruction, function
};

class CloneContext;
class IntermediateContext;
class Instruction;
class Visitor;

class Value
{
public:
    Value(const soul::ast::Span& span_, ValueKind kind_, Type* type_) noexcept;
    virtual ~Value();
    virtual Value* Clone(CloneContext& cloneContext) const;
    virtual void Accept(Visitor& visitor);
    inline bool IsRegValue() const noexcept { return kind == ValueKind::regValue; }
    inline bool IsInstruction() const noexcept { return kind == ValueKind::instruction; }
    inline bool IsSymbolValue() const noexcept { return kind == ValueKind::symbolValue; }
    inline bool IsAddressValue() const noexcept { return kind == ValueKind::addressValue; }
    inline bool IsGlobalVariable() const noexcept { return kind == ValueKind::globalVariable; }
    bool IsIntegerValue() const noexcept;
    bool IsFloatingPointValue() const noexcept;
    inline bool IsBoolValue() const noexcept { return kind == ValueKind::boolValue; }
    inline bool IsSByteValue() const noexcept { return kind == ValueKind::sbyteValue; }
    inline bool IsByteValue() const noexcept { return kind == ValueKind::byteValue; }
    inline bool IsShortValue() const noexcept { return kind == ValueKind::shortValue; }
    inline bool IsUShortValue() const noexcept { return kind == ValueKind::ushortValue; }
    inline bool IsIntValue() const noexcept { return kind == ValueKind::intValue; }
    inline bool IsUIntValue() const noexcept { return kind == ValueKind::uintValue; }
    inline bool IsLongValue() const noexcept { return kind == ValueKind::longValue; }
    inline bool IsULongValue() const noexcept { return kind == ValueKind::ulongValue; }
    inline bool IsFloatValue() const noexcept { return kind == ValueKind::floatValue; }
    inline bool IsDoubleValue() const noexcept { return kind == ValueKind::doubleValue; }
    inline bool IsArrayValue() const noexcept { return kind == ValueKind::arrayValue; }
    inline bool IsStructureValue() const noexcept { return kind == ValueKind::structureValue; }
    inline bool IsAggregateValue() const noexcept { return IsArrayValue() || IsStructureValue(); }
    inline bool IsStringValue() const noexcept { return kind == ValueKind::stringValue; }
    inline bool IsStringArrayValue() const noexcept { return kind == ValueKind::stringArrayValue; }
    bool IsTrue() const noexcept;
    bool IsFalse() const noexcept;
    std::int64_t GetIntegerValue() const noexcept;
    inline const soul::ast::Span& Span() const noexcept { return span; }
    inline ValueKind Kind() const noexcept { return kind; }
    std::string KindStr() const;
    inline Type* GetType() const noexcept { return type; }
    inline void SetType(Type* type_) noexcept { type = type_; }
    virtual std::string ToString() const { return std::string(); }
    Instruction* GetInstruction() const noexcept;
    virtual bool IsZero() const noexcept { return false; }
    virtual bool IsOne() const noexcept { return false; }
    virtual bool IsTwo() const noexcept { return false; }
    virtual Value* Log2(IntermediateContext* context) const;
    virtual Value* ModPowerOfTwo(IntermediateContext* context) const;
private:
    soul::ast::Span span;
    ValueKind kind;
    Type* type;
};

class BoolValue : public Value
{
public:
    BoolValue(bool value_, Type* type_) noexcept;
    inline bool GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    inline std::int64_t ToInteger() const noexcept { return value ? 1 : 0; }
    std::string ToString() const override;
private:
    bool value;
};

class SByteValue : public Value
{
public:
    SByteValue(std::int8_t value_, Type* type_) noexcept;
    inline std::int8_t GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
    bool IsZero() const noexcept override { return value == 0; }
    bool IsOne() const noexcept override { return value == 1; }
    bool IsTwo() const noexcept override { return value == 2; }
    Value* Log2(IntermediateContext* context) const override;
    Value* ModPowerOfTwo(IntermediateContext* context) const override;
private:
    std::int8_t value;
};

class ByteValue : public Value
{
public:
    ByteValue(std::uint8_t value_, Type* type_) noexcept;
    inline std::uint8_t GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
    bool IsZero() const noexcept override { return value == 0; }
    bool IsOne() const noexcept override { return value == 1; }
    bool IsTwo() const noexcept override { return value == 2; }
    Value* Log2(IntermediateContext* context) const override;
    Value* ModPowerOfTwo(IntermediateContext* context) const override;
private:
    std::uint8_t value;
};

class ShortValue : public Value
{
public:
    ShortValue(std::int16_t value_, Type* type_) noexcept;
    inline std::int16_t GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
    bool IsZero() const noexcept override { return value == 0; }
    bool IsOne() const noexcept override { return value == 1; }
    bool IsTwo() const noexcept override { return value == 2; }
    Value* Log2(IntermediateContext* context) const override;
    Value* ModPowerOfTwo(IntermediateContext* context) const override;
private:
    std::int16_t value;
};

class UShortValue : public Value
{
public:
    UShortValue(std::uint16_t value_, Type* type_) noexcept;
    inline std::uint16_t GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
    bool IsZero() const noexcept override { return value == 0; }
    bool IsOne() const noexcept override { return value == 1; }
    bool IsTwo() const noexcept override { return value == 2; }
    Value* Log2(IntermediateContext* context) const override;
    Value* ModPowerOfTwo(IntermediateContext* context) const override;
private:
    std::uint16_t value;
};

class IntValue : public Value
{
public:
    IntValue(std::int32_t value_, Type* type_) noexcept;
    inline std::int32_t GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
    bool IsZero() const noexcept override { return value == 0; }
    bool IsOne() const noexcept override { return value == 1; }
    bool IsTwo() const noexcept override { return value == 2; }
    Value* Log2(IntermediateContext* context) const override;
    Value* ModPowerOfTwo(IntermediateContext* context) const override;
private:
    std::int32_t value;
};

class UIntValue : public Value
{
public:
    UIntValue(std::uint32_t value_, Type* type_) noexcept;
    inline std::uint32_t GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
    bool IsZero() const noexcept override { return value == 0; }
    bool IsOne() const noexcept override { return value == 1; }
    bool IsTwo() const noexcept override { return value == 2; }
    Value* Log2(IntermediateContext* context) const override;
    Value* ModPowerOfTwo(IntermediateContext* context) const override;
private:
    std::uint32_t value;
};

class LongValue : public Value
{
public:
    LongValue(std::int64_t value_, Type* type_) noexcept;
    inline std::int64_t GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
    bool IsZero() const noexcept override { return value == 0; }
    bool IsOne() const noexcept override { return value == 1; }
    bool IsTwo() const noexcept override { return value == 2; }
    Value* Log2(IntermediateContext* context) const override;
    Value* ModPowerOfTwo(IntermediateContext* context) const override;
private:
    std::int64_t value;
};

class ULongValue : public Value
{
public:
    ULongValue(std::uint64_t value_, Type* type_) noexcept;
    inline std::uint64_t GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
    bool IsZero() const noexcept override { return value == 0; }
    bool IsOne() const noexcept override { return value == 1; }
    bool IsTwo() const noexcept override { return value == 2; }
    Value* Log2(IntermediateContext* context) const override;
    Value* ModPowerOfTwo(IntermediateContext* context) const override;
private:
    std::uint64_t value;
};

class FloatValue : public Value
{
public:
    FloatValue(float value_, Type* type_) noexcept;
    inline float GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    float value;
};

class DoubleValue : public Value
{
public:
    DoubleValue(double value_, Type* type_) noexcept;
    inline double GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    double value;
};

class NullValue : public Value
{
public:
    NullValue(Type* type_) noexcept;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

} // otava::intermediate
