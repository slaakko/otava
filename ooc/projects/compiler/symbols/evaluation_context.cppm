// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.evaluation_context;

import otava.symbols.id;
import otava.symbols.symbol;
import otava.symbols.value;
import util.unicode;
import soul.ast.span;
import std;

export namespace otava::symbols {

class Context;
class Module;
class Writer;
class Reader;
class NullPtrValue;
class StringValue;
class SymbolValue;
class InvokeValue;
class ArrayValue;
class StructureValue;
class FunctionGroupValue;
class TypeValue;

class EvaluationStack
{
public:
    EvaluationStack();
    inline void Push(Value* value) { stack.push(value); }
    Value* Pop();
private:
    std::stack<Value*> stack;
};

class EvaluationMap
{
public:
    EvaluationMap(EvaluationMap* parentMap_);
    Value* GetValue(const std::string& symbol) const noexcept;
    void SetValue(const std::string& symbol, Value* value);
    inline EvaluationMap* ParentMap() const noexcept { return parentMap; }
private:
    std::unordered_map<std::string, Value*> map;
    EvaluationMap* parentMap;
};

class EvaluationContext
{
public:
    EvaluationContext(Module* module_, bool readOnly_);
    ~EvaluationContext();
    inline Module* GetModule() const noexcept { return module; }
    void Init(Context* context);
    void Write(Writer& writer, Context* context);
    void Read(Reader& reader, Context* context);
    inline bool IsReadOnly() const noexcept { return readOnly; }
    BoolValue* GetBoolValue(bool value);
    Value* GetIntegerValue(std::uint64_t value, TypeSymbol* type, Context* context);
    Value* GetFloatingValue(double value, TypeSymbol* type, Context* context);
    NullPtrValue* GetNullPtrValue();
    StringValue* GetStringValue(const std::string& value, TypeSymbol* type, Context* context);
    Value* GetCharValue(char32_t value, TypeSymbol* type, Context* context);
    SymbolValue* GetSymbolValue(Symbol* symbol, Context* context);
    InvokeValue* GetInvokeValue(Value* subject, const std::vector<Value*>& arguments, Context* context);
    ArrayValue* GetArrayValue(TypeSymbol* type, Context* context);
    StructureValue* GetStructureValue(TypeSymbol* type, Context* context);
    FunctionGroupValue* GetFunctionGroupValue(Module* module, SymbolId symbolId, FunctionGroupSymbol* functionGroup, FunctionSymbol* fn, Context* context);
    TypeValue* GetTypeValue(Module* module, SymbolId symbolId, TypeSymbol* type, Context* context);
    void AddValue(Value* value);
    Value* GetValue(SymbolId valueId);
    void ResetEvaluationStack();
    inline EvaluationStack* GetEvaluationStack() const { return evaluationStack.get(); }
    void ResetEvaluationMaps();
    inline EvaluationMap* CurrentEvaluationMap() const { return currentEvaluationMap.get(); }
    void PushEvaluationMap();
    void PopEvaluationMap();
private:
    void MapValue(Value* value);
    bool initialized;
    bool readOnly;
    Module* module;
    std::unique_ptr<BoolValue> trueValue;
    std::unique_ptr<BoolValue> falseValue;
    NullPtrValue* nullPtrValue;
    std::map<std::pair<std::uint64_t, TypeSymbol*>, Value*> integerValueMap;
    std::map<std::pair<double, TypeSymbol*>, Value*> floatingValueMap;
    std::map<std::pair<std::string, TypeSymbol*>, StringValue*> stringValueMap;
    std::map<std::pair<char32_t, TypeSymbol*>, Value*> charValueMap;
    std::map<Symbol*, SymbolValue*> symbolValueMap;
    std::map<Value*, InvokeValue*> invokeMap;
    std::vector<std::unique_ptr<Value>> values;
    std::unordered_map<SymbolId, Value*> valueMap;
    std::unique_ptr<EvaluationStack> evaluationStack;
    std::vector<std::unique_ptr<EvaluationMap>> evaluationMapStack;
    std::unique_ptr<EvaluationMap> currentEvaluationMap;
};

} // namespace otava::symbols
