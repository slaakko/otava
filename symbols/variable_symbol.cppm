// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.variable_symbol;

import otava.symbols.symbol;
import otava.symbols.value;
import otava.ast.node;

export namespace otava::symbols {

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(Module* module_, SymbolId id_);
    VariableSymbol(Module* module_, SymbolId id_, const std::string& name_);
    inline int Level() const noexcept { return level; }
    inline void SetLevel(int level_) noexcept { level = level_; }
    inline bool FoundFromParent() const noexcept { return foundFromParent; }
    inline void SetFoundFromParent() noexcept { foundFromParent = true; }
    inline std::int64_t NodeId() const noexcept { return nodeId; }
    inline void SetNodeId(std::int64_t nodeId_) noexcept { nodeId = nodeId_; }
    inline bool IsTemporary() const noexcept { return temporary; }
    inline void SetTemporary() noexcept { temporary = true; }
    bool IsLocalVariable() const noexcept;
    bool IsMemberVariable() const noexcept;
    bool IsGlobalVariable(Context* context) const noexcept;
    bool IsStatic() const noexcept;
    inline Value* GetValue() const noexcept { return value; }
    inline void SetValue(Value* value_) noexcept { value = value_; }
    TypeSymbol* GetDeclaredType() const noexcept;
    void SetDeclaredType(TypeSymbol* declaredType_) noexcept;
    TypeSymbol* GetInitializerType() const noexcept;
    void SetInitializerType(TypeSymbol* initializerType_) noexcept;
    TypeSymbol* GetType() const noexcept;
    TypeSymbol* GetReferredType() const noexcept;
private:
    int level;
    bool foundFromParent;
    std::int64_t nodeId;
    bool temporary;
    Value* value;
};

enum class ParameterKind : std::uint8_t
{
    regular, returnValueParam
};

class ParameterSymbol : public Symbol
{
public:
    ParameterSymbol(Module* module_, SymbolId id_);
    ParameterSymbol(Module* module_, SymbolId id_, const std::string& name_);
    inline ParameterKind GetParameterKind() const noexcept { return parameterKind; }
    inline void SetParameterKind(ParameterKind parameterKind_) { parameterKind = parameterKind_; }
    inline otava::ast::Node* DefaultValue() const noexcept { return defaultValue; }
    inline void SetDefaultValue(otava::ast::Node* defaultValue_) noexcept { defaultValue = defaultValue_; }
    inline void SetType(TypeSymbol* type_) noexcept { type = type_; }
private:
    ParameterKind parameterKind;
    otava::ast::Node* defaultValue;
    TypeSymbol* type;
};

} // namespace otava::symbols
