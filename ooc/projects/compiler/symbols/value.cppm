// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.value;

import otava.symbols.id;
import otava.symbols.symbol;
import otava.intermediate.value;
import util.unicode;
import soul.ast.span;
import std;

export namespace otava::symbols {

class BoolValue;
class Emitter;
class EvaluationContext;
class Context;
class TypeSymbol;
class FunctionGroupSymbol;

enum class ValueKind : std::uint8_t
{
    none, boolValue, byteValue, sbyteValue, shortValue, ushortValue, intValue, uintValue, longValue, ulongValue, 
    floatValue, doubleValue, nullPtrValue, stringValue, symbolValue, invokeValue, arrayValue, structureValue
};

TypeSymbol* GetValueType(ValueKind valueKind, Context* context); 

constexpr std::uint8_t ToUnderlying(ValueKind valueKind) { return std::uint8_t(valueKind); }

std::string ValueKindStr(ValueKind kind);

ValueKind CommonValueKind(ValueKind left, ValueKind right) noexcept;

bool readingEvaluationContext = false;
bool cloning = false;

class Value : public Symbol
{
public:
    Value(Module* module_, SymbolId id_);
    Value(Module* module_, SymbolId id_, const std::string& rep_);
    inline bool IsBoolValue() const noexcept { return GetValueKind() == ValueKind::boolValue; }
    bool IsIntegerValue() const noexcept;
    bool IsFloatingValue() const noexcept;
    virtual std::uint64_t GetIntegerValue() const noexcept { return 0; }
    virtual double GetFloatingValue() const noexcept { return 0.0; }
    inline bool IsStringValue() const noexcept { return GetValueKind() == ValueKind::stringValue; }
    inline bool IsNullPtrValue() const noexcept { return GetValueKind() == ValueKind::nullPtrValue; }
    inline bool IsSymbolValue() const noexcept { return GetValueKind() == ValueKind::symbolValue; }
    inline bool IsInvokeValue() const noexcept { return GetValueKind() == ValueKind::invokeValue; }
    inline bool IsArrayValue() const noexcept { return GetValueKind() == ValueKind::arrayValue; }
    virtual bool IsSerializableValue() const noexcept { return true; }
    virtual bool IsComplete() const noexcept { return true; }
    virtual Value* Convert(ValueKind kind, Context* context) = 0;
    virtual BoolValue* ToBoolValue(Context* context) = 0;
    virtual otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    ValueKind GetValueKind() const noexcept;
    std::string Rep() const { return Name(); }
    virtual std::string ToString() const { return Rep(); }
    virtual std::string Val() const = 0;
    TypeSymbol* GetType(Context* context) const;
    void SetType(TypeSymbol* type_, Context* context) noexcept;
    TypeSymbol* GetInterfaceType(Context* context) const;
    inline void SetInterfaceType(TypeSymbol* interfaceType_) noexcept { interfaceType = interfaceType_; }
    virtual Value* Clone(Context* context) const;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    friend class EvaluationContext;
    mutable TypeSymbol* type;
    SymbolId typeId;
    TypeSymbol* interfaceType;
};

bool ValuesEqual(Value* left, Value* right, Context* context);
bool ValueLess(Value* left, Value* right, Context* context);

class BoolValue : public Value
{
public:
    BoolValue(Module* module_, SymbolId id_);
    BoolValue(Module* module_, bool value_, Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override;
    Value* Clone(Context* context) const override;
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    inline bool GetValue() const noexcept { return value; }
    inline void SetValue(bool value_) noexcept { value = value_; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    bool value;
};

} // namespace otava::symbols
