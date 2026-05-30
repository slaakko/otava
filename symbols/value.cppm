// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.value;

import std;
import otava.symbols.symbol;
import util;

export namespace otava::symbols {

class BoolValue;
class Emitter;
class EvaluationContext;
class Context;
class TypeSymbol;

enum class ValueKind : std::uint8_t
{
    none, boolValue, integerValue, floatingValue, nullPtrValue, stringValue, charValue, symbolValue, invokeValue, arrayValue, structureValue
};

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
    inline bool IsIntegerValue() const noexcept { return GetValueKind() == ValueKind::integerValue; }
    inline bool IsFloatingValue() const noexcept { return GetValueKind() == ValueKind::floatingValue; }
    inline bool IsStringValue() const noexcept { return GetValueKind() == ValueKind::stringValue; }
    inline bool IsCharValue() const noexcept { return GetValueKind() == ValueKind::charValue; }
    inline bool IsNullPtrValue() const noexcept { return GetValueKind() == ValueKind::nullPtrValue; }
    inline bool IsSymbolValue() const noexcept { return GetValueKind() == ValueKind::symbolValue; }
    inline bool IsInvokeValue() const noexcept { return GetValueKind() == ValueKind::invokeValue; }
    inline bool IsArrayValue() const noexcept { return GetValueKind() == ValueKind::arrayValue; }
    virtual BoolValue* ToBoolValue(EvaluationContext& context) = 0;
    ValueKind GetValueKind() const noexcept;
    std::string Rep() const { return Name(); }
    virtual std::string ToString() const { return Rep(); }
    virtual std::string Val() const = 0;
    TypeSymbol* GetType() const;
private:
    friend class EvaluationContext;
};

class BoolValue : public Value
{
public:
    BoolValue(Module* module_, bool value_, const std::string& rep_, Context* context);
    inline bool GetValue() const noexcept { return value; }
    BoolValue* ToBoolValue(EvaluationContext& context) override { return this; }
    std::string Val() const override { return value ? "true" : "false"; }
private:
    bool value;
};

class IntegerValue : public Value
{
public:
    IntegerValue(Module* module_, std::int64_t value_, const std::string& rep_, Context* context);
    inline std::int64_t GetValue() const noexcept { return value; }
    BoolValue* ToBoolValue(EvaluationContext& context) override;
    std::string Val() const override { return std::to_string(value); }
private:
    std::int64_t value;
};

class FloatingValue : public Value
{
public:
    FloatingValue(Module* module_, double value_, const std::string& rep_, Context* context);
    inline double GetValue() const noexcept { return value; }
    BoolValue* ToBoolValue(EvaluationContext& context) override;
    std::string Val() const override { return std::to_string(value); }
private:
    double value;
};

class NullPtrValue : public Value
{
public:
    NullPtrValue(Module* module_, Context* context);
    BoolValue* ToBoolValue(EvaluationContext& context) override;
    std::string Val() const override { return "nullptr"; }
};

class StringValue : public Value
{
public:
    StringValue(Module* module_, const std::string& value_, Context* context);
    inline const std::string& GetValue() const { return value; }
    BoolValue* ToBoolValue(EvaluationContext& context) override;
    std::string Val() const override { return value; }
private:
    std::string value;
};

class CharValue : public Value
{
public:
    CharValue(Module* module_, char32_t value_, Context* context);
    inline char32_t GetValue() const { return value; }
    BoolValue* ToBoolValue(EvaluationContext& context) override;
    std::string Val() const override { return util::ToUtf8(std::u32string(1, value)); }
private:
    char32_t value;
};

class SymbolValue : public Value
{
public:
    SymbolValue(Module* module_, Symbol* symbol_, Context* context);
    inline Symbol* GetSymbol() const { return symbol; }
    BoolValue* ToBoolValue(EvaluationContext& context) override;
    std::string Val() const override { return symbol->Name(); }
private:
    Symbol* symbol;
};

class InvokeValue : public Value
{
public:
    InvokeValue(Module* module_, Value* subject_, Context* context);
    inline Value* Subject() const { return subject; }
    std::string Val() const override { return subject->Val(); }
private:
    Value* subject;
};

class ArrayValue : public Value
{
public:
    ArrayValue(Module* module_, Context* context);
    void AddElementValue(Value* elementValue);
    BoolValue* ToBoolValue(EvaluationContext& context) override;
    std::string Val() const override { return "<array>"; }
private:
    std::vector<Value*> elementValues;
};

class StructureValue : public Value
{
public:
    StructureValue(Module* module_, Context* context);
    void AddFieldValue(Value* fieldValue);
    std::string Val() const override { return "<structure>"; }
private:
    std::vector<Value*> fieldValues;
};

class EvaluationContext
{
public:
    EvaluationContext(Module* module_);
    void Init();
    void Write(Writer& writer, Context* context);
    void Read(Reader& reader);
    BoolValue* GetBoolValue(bool value);
    IntegerValue* GetIntegerValue(std::int64_t value, const std::string& rep, TypeSymbol* type);
    FloatingValue* GetFloatingValue(double value, const std::string& rep, TypeSymbol* type);
    NullPtrValue* GetNullPtrValue();
    StringValue* GetStringValue(const std::string& value, TypeSymbol* type);
    CharValue* GetCharValue(char32_t value, TypeSymbol* type);
    SymbolValue* GetSymbolValue(Symbol* symbol);
    InvokeValue* GetInvokeValue(Value* subject);
    ArrayValue* GetArrayValue();
    StructureValue* GetStructureValue();
    void AddValue(Value* value);
private:
    void MapValue(Value* value);
    bool initialized;
    Module* module;
    BoolValue trueValue;
    BoolValue falseValue;
    NullPtrValue nullPtrValue;
    std::map<std::pair<std::pair<std::int64_t, std::string>, TypeSymbol*>, IntegerValue*> integerValueMap;
    std::map<std::pair<std::pair<double, std::string>, TypeSymbol*>, FloatingValue*> floatingValueMap;
    std::map<std::pair<std::string, TypeSymbol*>, StringValue*> stringValueMap;
    std::map<std::pair<char32_t, TypeSymbol*>, CharValue*> charValueMap;
    std::map<Symbol*, SymbolValue*> symbolValueMap;
    std::map<Value*, InvokeValue*> invokeMap;
    std::vector<std::unique_ptr<Value>> values;
    std::map<util::uuid, Value*> valueMap;
};

} // namespace otava::symbols

