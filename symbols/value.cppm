// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.value;

import otava.symbols.symbol;
import otava.intermediate.data;
import util.unicode;
import std;

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
    virtual Value* Convert(ValueKind kind, Context* context) = 0;
    virtual BoolValue* ToBoolValue(Context* context) = 0;
    virtual otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    ValueKind GetValueKind() const noexcept;
    std::string Rep() const { return Name(); }
    virtual std::string ToString() const { return Rep(); }
    virtual std::string Val() const = 0;
    TypeSymbol* GetType(Context* context);
    void SetType(TypeSymbol* type_, Context* context) noexcept;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    friend class EvaluationContext;
    TypeSymbol* type;
    SymbolId typeId;
};

class BoolValue : public Value
{
public:
    BoolValue(Module* module_, SymbolId id_);
    BoolValue(Module* module_, bool value_, const std::string& rep_, Context* context);
    inline bool GetValue() const noexcept { return value; }
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override { return this; }
    std::string Val() const override { return value ? "true" : "false"; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    bool value;
};

class IntegerValue : public Value
{
public:
    IntegerValue(Module* module_, SymbolId id_);
    IntegerValue(Module* module_, std::int64_t value_, const std::string& rep_, Context* context);
    inline std::int64_t GetValue() const noexcept { return value; }
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return std::to_string(value); }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    std::int64_t value;
};

class FloatingValue : public Value
{
public:
    FloatingValue(Module* module_, SymbolId id_);
    FloatingValue(Module* module_, double value_, const std::string& rep_, Context* context);
    inline double GetValue() const noexcept { return value; }
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return std::to_string(value); }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    double value;
};

class NullPtrValue : public Value
{
public:
    NullPtrValue(Module* module_, SymbolId id_);
    NullPtrValue(Module* module_, Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return "nullptr"; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
};

class StringValue : public Value
{
public:
    StringValue(Module* module_, SymbolId id_);
    StringValue(Module* module_, const std::string& value_, Context* context);
    inline const std::string& GetValue() const { return value; }
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return value; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    std::string value;
};

class CharValue : public Value
{
public:
    CharValue(Module* module_, SymbolId id_);
    CharValue(Module* module_, char32_t value_, Context* context);
    inline char32_t GetValue() const { return value; }
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return util::ToUtf8(std::u32string(1, value)); }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    char32_t value;
};

class SymbolValue : public Value
{
public:
    SymbolValue(Module* module_, SymbolId id_);
    SymbolValue(Module* module_, Symbol* symbol_, Context* context);
    Symbol* GetSymbol(Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return symbol->Name(); }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    Symbol* symbol;
    SymbolId symbolId;
};

class InvokeValue : public Value
{
public:
    InvokeValue(Module* module_, SymbolId id_);
    InvokeValue(Module* module_, Value* subject_, Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    Value* Subject(Context* context);
    std::string Val() const override { return subject->Val(); }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    Value* subject;
    SymbolId subjectId;
};

struct ArrayValueHeader
{
    ArrayValueHeader();
    void Write(Writer& writer);
    void Read(Reader& reader);
    FileOffset bodyOffset;
    Length bodyLength;
};

class ArrayValue : public Value
{
public:
    ArrayValue(Module* module_, SymbolId id_);
    ArrayValue(Module* module_, Context* context);
    void AddElementValue(Value* elementValue);
    Cardinality Count();
    Value* GetElementValue(Index index, Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return "<array>"; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    ArrayValueHeader header;
    std::vector<Value*> elementValues;
    std::vector<SymbolId> elementValueIds;
};

struct StructureValueHeader
{
    StructureValueHeader();
    void Write(Writer& writer);
    void Read(Reader& reader);
    FileOffset bodyOffset;
    Length bodyLength;
};

class StructureValue : public Value
{
public:
    StructureValue(Module* module_, SymbolId id_);
    StructureValue(Module* module_, Context* context);
    void AddFieldValue(Value* fieldValue);
    Cardinality Count();
    Value* GetFieldValue(Index index, Context* context);
    Value* Convert(ValueKind kind, Context* context) override;
    BoolValue* ToBoolValue(Context* context) override;
    std::string Val() const override { return "<structure>"; }
    otava::intermediate::Value* IrValue(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    StructureValueHeader header;
    std::vector<Value*> fieldValues;
    std::vector<SymbolId> fieldValueIds;
};

class EvaluationContext
{
public:
    EvaluationContext(Module* module_, bool readOnly_);
    inline Module* GetModule() const noexcept { return module; }
    void Init(Context* context);
    void Write(Writer& writer, Context* context);
    void Read(Reader& reader, Context* context);
    inline bool IsReadOnly() const noexcept { return readOnly; }
    BoolValue* GetBoolValue(bool value);
    IntegerValue* GetIntegerValue(std::int64_t value, const std::string& rep, TypeSymbol* type, Context* context);
    FloatingValue* GetFloatingValue(double value, const std::string& rep, TypeSymbol* type, Context* context);
    NullPtrValue* GetNullPtrValue();
    StringValue* GetStringValue(const std::string& value, TypeSymbol* type, Context* context);
    CharValue* GetCharValue(char32_t value, TypeSymbol* type, Context* context);
    SymbolValue* GetSymbolValue(Symbol* symbol, Context* context);
    InvokeValue* GetInvokeValue(Value* subject, Context* context);
    ArrayValue* GetArrayValue(TypeSymbol* type, Context* context);
    StructureValue* GetStructureValue(TypeSymbol* type, Context* context);
    void AddValue(Value* value);
    Value* GetValue(SymbolId valueId);
private:
    void MapValue(Value* value);
    bool initialized;
    bool readOnly;
    Module* module;
    std::unique_ptr<BoolValue> trueValue;
    std::unique_ptr<BoolValue> falseValue;
    std::unique_ptr<NullPtrValue> nullPtrValue;
    std::map<std::pair<std::pair<std::int64_t, std::string>, TypeSymbol*>, IntegerValue*> integerValueMap;
    std::map<std::pair<std::pair<double, std::string>, TypeSymbol*>, FloatingValue*> floatingValueMap;
    std::map<std::pair<std::string, TypeSymbol*>, StringValue*> stringValueMap;
    std::map<std::pair<char32_t, TypeSymbol*>, CharValue*> charValueMap;
    std::map<Symbol*, SymbolValue*> symbolValueMap;
    std::map<Value*, InvokeValue*> invokeMap;
    std::vector<std::unique_ptr<Value>> values;
    std::unordered_map<SymbolId, Value*> valueMap;
};

} // namespace otava::symbols

