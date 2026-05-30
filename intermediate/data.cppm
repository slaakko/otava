// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.data;

import otava.intermediate.types;
import soul.ast.span;
import util.code_formatter;
import std;

export namespace otava::intermediate {

class Data;
class Function;
class BasicBlock;
class Instruction;
class RegValue;
class GlobalVariable;

class CloneContext
{
public:
    CloneContext();
    inline void SetContext(IntermediateContext* context_) noexcept { context = context_; }
    inline IntermediateContext* GetContext() const noexcept { return context; }
    inline void SetCurrentFunction(Function* currentFunction_) noexcept { currentFunction = currentFunction_; }
    inline Function* CurrentFunction() const noexcept { return currentFunction; }
    void MapInstruction(Instruction* inst, Instruction* clone);
    Instruction* GetMappedInstruction(Instruction* inst) const noexcept;
    void AddUnmappedInstruction(Instruction* inst, RegValue* regValue);
    inline const std::set<std::pair<Instruction*, RegValue*>>& UnmappedInstructions() const noexcept { return unmappedInstructions; }
    BasicBlock* GetMappedBasicBlock(BasicBlock* bb) const noexcept;
    void MapBasicBlock(BasicBlock* bb, BasicBlock* clone);
private:
    IntermediateContext* context;
    Function* currentFunction;
    std::map<Instruction*, Instruction*> instMap;
    std::set<std::pair<Instruction*, RegValue*>> unmappedInstructions;
    std::map<BasicBlock*, BasicBlock*> bbMap;
};

enum class ValueKind
{
    boolValue, sbyteValue, byteValue, shortValue, ushortValue, intValue, uintValue, longValue, ulongValue, floatValue, doubleValue, nullValue, addressValue,
    arrayValue, structureValue, stringValue, stringArrayValue, conversionValue, clsIdValue, symbolValue, globalVariable, regValue, instruction, function
};

class Value
{
public:
    Value(const soul::ast::Span& span_, ValueKind kind_, Type* type_) noexcept;
    virtual ~Value();
    virtual Value* Clone(CloneContext& cloneContext) const { return const_cast<Value*>(this); }
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
    virtual Value* Log2(IntermediateContext* context) const { return nullptr; }
    virtual Value* ModPowerOfTwo(IntermediateContext* context) const { return nullptr; }
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

class AddressValue : public Value
{
public:
    AddressValue(const soul::ast::Span& span_, const std::string& id_, Type* type) noexcept;
    inline const std::string& Id() const noexcept { return id; }
    inline GlobalVariable* GetValue() const noexcept { return globalVariable; }
    inline void SetValue(GlobalVariable* globalVariable_) noexcept { globalVariable = globalVariable_; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string id;
    GlobalVariable* globalVariable;
};

class ArrayValue : public Value
{
public:
    ArrayValue(const soul::ast::Span& span_, const std::vector<Value*>& elements_, ArrayType* type_);
    inline const std::vector<Value*>& Elements() const noexcept { return elements; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::vector<Value*> elements;
};

class StructureValue : public Value
{
public:
    StructureValue(const soul::ast::Span& span_, const std::vector<Value*>& fieldValues_, StructureType* type_);
    inline const std::vector<Value*>& FieldValues() const noexcept { return fieldValues; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::vector<Value*> fieldValues;
};

class StringValue : public Value
{
public:
    StringValue(const soul::ast::Span& span_, const std::string& value_);
    inline const std::string& GetValue() const noexcept { return value; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string value;
};

class StringArrayValue : public Value
{
public:
    StringArrayValue(const soul::ast::Span& span_, char prefix_, const std::vector<Value*>& elements_);
    inline char Prefix() const noexcept { return prefix; }
    inline const std::vector<Value*>& Elements() const noexcept { return elements; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    char prefix;
    std::vector<Value*> elements;
};

class ConversionValue : public Value
{
public:
    ConversionValue(const soul::ast::Span& span_, Type* type_, Value* from_) noexcept;
    inline Value* From() const noexcept { return from; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    Value* from;
};

class ClsIdValue : public Value
{
public:
    ClsIdValue(const soul::ast::Span& span_, Type* type_, const std::string& typeId_);
    inline const std::string& TypeId() const noexcept { return typeId; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string typeId;
};

class SymbolValue : public Value
{
public:
    SymbolValue(const soul::ast::Span& span_, Type* type_, const std::string& symbol_);
    const std::string& Symbol() const noexcept { return symbol; }
    inline Function* GetFunction() const noexcept { return function; }
    inline void SetFunction(Function* function_) noexcept { function = function_; }
    inline GlobalVariable* GetGlobalVariable() const noexcept { return globalVariable; }
    inline void SetGlobalVariable(GlobalVariable* globalVariable_) noexcept { globalVariable = globalVariable_; }
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
private:
    std::string symbol;
    Function* function;
    GlobalVariable* globalVariable;
};

class GlobalVariable : public Value
{
public:
    GlobalVariable(const soul::ast::Span& span_, Type* type_, const std::string& name_, Value* initializer_);
    void Accept(Visitor& visitor) override;
    inline const std::string& Name() const noexcept { return name; }
    inline Value* Initializer() const noexcept { return initializer; }
    void Write(util::CodeFormatter& formatter);
    std::string ToString() const override;
private:
    std::string name;
    Value* initializer;
};

template<class T>
class ValueMap
{
public:
    ValueMap();
    Value* Get(const T& value, Data* data, const Types& types);
private:
    std::map<T, Value*> valueMap;
};

class Data
{
public:
    Data() noexcept;
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    inline IntermediateContext* GetContext() const noexcept { return context; }
    inline void SetContext(IntermediateContext* context_) noexcept { context = context_; }
    GlobalVariable* AddGlobalVariable(const soul::ast::Span& span, Type* type, const std::string& variableName, Value* initializer, IntermediateContext* context);
    GlobalVariable* GetGlobalVariableForString(Value* stringValue, Type* charType);
    Value* GetBoolValue(bool value, const Types& types);
    Value* GetTrueValue(const Types& types);
    Value* GetFalseValue(const Types& types);
    Value* GetSByteValue(std::int8_t value, const Types& types);
    Value* GetByteValue(std::uint8_t value, const Types& types);
    Value* GetShortValue(std::int16_t value, const Types& types);
    Value* GetUShortValue(std::uint16_t value, const Types& types);
    Value* GetIntValue(std::int32_t value, const Types& types);
    Value* GetUIntValue(std::uint32_t value, const Types& types);
    Value* GetLongValue(std::int64_t value, const Types& types);
    Value* GetULongValue(std::uint64_t value, const Types& types);
    Value* GetIntegerValue(Type* type, std::int64_t value, const Types& types);
    Value* GetFloatValue(float value, const Types& types);
    Value* GetDoubleValue(double value, const Types& types);
    Value* GetFloatingValue(Type* type, double value, const Types& types);
    Value* GetNullValue(Type* type);
    Value* MakeValue(std::int8_t value, const Types& types);
    Value* MakeValue(std::uint8_t value, const Types& types);
    Value* MakeValue(std::int16_t value, const Types& types);
    Value* MakeValue(std::uint16_t value, const Types& types);
    Value* MakeValue(std::int32_t value, const Types& types);
    Value* MakeValue(std::uint32_t value, const Types& types);
    Value* MakeValue(std::int64_t value, const Types& types);
    Value* MakeValue(std::uint64_t value, const Types& types);
    Value* MakeValue(float value, const Types& types);
    Value* MakeValue(double value, const Types& types);
    Value* MakeArrayValue(const soul::ast::Span& span, const std::vector<Value*>& elements, ArrayType* arrayType);
    Value* MakeStructureValue(const soul::ast::Span& span, const std::vector<Value*>& fieldValues, StructureType* structureType);
    Value* MakeStringValue(const soul::ast::Span& span, const std::string& value, bool crop);
    Value* MakeStringArrayValue(const soul::ast::Span& span, char prefix, const std::vector<Value*>& elements);
    Value* MakeConversionValue(const soul::ast::Span& span, Type* type, Value* from);
    Value* MakeClsIdValue(const soul::ast::Span& span, Type* type, const std::string& clsIdStr);
    Value* MakeSymbolValue(const soul::ast::Span& span, Type* type, const std::string& symbol);
    Value* MakeIntegerLiteral(const soul::ast::Span& span, Type* type, const std::string& strValue, const Types& types);
    Value* MakeAddressLiteral(const soul::ast::Span& span, Type* type, const std::string& id, bool resolve);
    void ResolveAddressValue(AddressValue* addressValue);
    void ResolveAddressValues();
    void VisitGlobalVariables(Visitor& visitor);
    std::string GetNextStringValueId();
    void Write(util::CodeFormatter& formatter);
private:
    IntermediateContext* context;
    std::vector<std::unique_ptr<Value>> values;
    std::vector<GlobalVariable*> globalVariables;
    std::unique_ptr<BoolValue> trueValue;
    std::unique_ptr<BoolValue> falseValue;
    ValueMap<std::int8_t> sbyteValueMap;
    ValueMap<std::uint8_t> byteValueMap;
    ValueMap<std::int16_t> shortValueMap;
    ValueMap<std::uint16_t> ushortValueMap;
    ValueMap<std::int32_t> intValueMap;
    ValueMap<std::uint32_t> uintValueMap;
    ValueMap<std::int64_t> longValueMap;
    ValueMap<std::uint64_t> ulongValueMap;
    ValueMap<float> floatValueMap;
    ValueMap<double> doubleValueMap;
    std::map<Type*, NullValue*> nullValueMap;
    std::vector<AddressValue*> addressValues;
    std::map<std::string, GlobalVariable*> globalVariableMap;
    std::map<std::string, GlobalVariable*> globalStringVariableMap;
    std::int32_t nextStringValueId;
};

template<class T>
ValueMap<T>::ValueMap()
{
}

template<class T>
Value* ValueMap<T>::Get(const T& value, Data* data, const Types& types)
{
    auto it = valueMap.find(value);
    if (it != valueMap.cend())
    {
        return it->second;
    }
    else
    {
        Value* constantValue = data->MakeValue(value, types);
        valueMap[value] = constantValue;
        return constantValue;
    }
}

} // otava::intermediate
