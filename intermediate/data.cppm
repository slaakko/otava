// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.data;

import otava.intermediate.code;
import otava.intermediate.types;
import otava.intermediate.value;
import soul.ast.span;
import util.code_formatter;
import std;

export namespace otava::intermediate {

class Data;
class Function;
class BasicBlock;
class RegValue;
class GlobalVariable;
class IntermediateContext;

class CloneContext
{
public:
    CloneContext();
    void SetContext(IntermediateContext* context_) noexcept;
    IntermediateContext* GetContext() const noexcept;
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
    otava::intermediate::Value* Get(const T& value, Data* data, const otava::intermediate::Types& types);
private:
    std::map<T, otava::intermediate::Value*> valueMap;
};

class Data
{
public:
    Data() noexcept;
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    IntermediateContext* GetContext() const noexcept;
    void SetContext(IntermediateContext* context_) noexcept;
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
    GlobalVariable* DoAddGlobalVariable(const soul::ast::Span& span, Type* type, const std::string& variableName, Value* initializer, IntermediateContext* context);
};

template<class T>
ValueMap<T>::ValueMap()
{
}

template<class T>
otava::intermediate::Value* ValueMap<T>::Get(const T& value, Data* data, const otava::intermediate::Types& types)
{
    auto it = valueMap.find(value);
    if (it != valueMap.cend())
    {
        return it->second;
    }
    else
    {
        otava::intermediate::Value* constantValue = data->MakeValue(value, types);
        valueMap[value] = constantValue;
        return constantValue;
    }
}

} // otava::intermediate
