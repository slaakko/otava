// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.variable_symbol;

import otava.symbols.ast_node_io;
import otava.symbols.symbol;
import otava.symbols.value;
import otava.ast.node;

export namespace otava::symbols {

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(Module* module_, SymbolId id_);
    VariableSymbol(Module* module_, SymbolId id_, const std::string& name_);
    inline int Arity() const noexcept { return 0; }
    inline int Level() const noexcept { return level; }
    inline void SetLevel(int level_) noexcept { level = level_; }
    inline bool FoundFromParent() const noexcept { return foundFromParent; }
    inline void SetFoundFromParent() noexcept { foundFromParent = true; }
    inline std::int64_t NodeId() const noexcept { return nodeId; }
    inline void SetNodeId(std::int64_t nodeId_) noexcept { nodeId = nodeId_; }
    inline bool IsTemporary() const noexcept { return temporary; }
    inline void SetTemporary() noexcept { temporary = true; }
    bool IsLocalVariable(Context* context);
    bool IsMemberVariable(Context* context);
    bool IsGlobalVariable(Context* context);
    bool IsStatic() const noexcept;
    inline VariableSymbol* Final() noexcept { if (global) return global; else return this; }
    inline void SetGlobal(VariableSymbol* global_) noexcept { global = global_; }
    inline std::int32_t GetIndex() const noexcept { return index; }
    inline void SetIndex(std::int32_t index_) noexcept { index = index_; }
    Value* GetValue(Context* context);
    inline void SetValue(Value* value_) noexcept { value = value_; }
    inline std::int32_t LayoutIndex() const noexcept { return layoutIndex; }
    inline void SetLayoutIndex(std::int32_t layoutIndex_) noexcept { layoutIndex = layoutIndex_; }
    std::string IrName(Context* context) const override;
    TypeSymbol* GetDeclaredType(Context* context);
    void SetDeclaredType(TypeSymbol* declaredType_, Context* context) noexcept;
    TypeSymbol* GetInitializerType(Context* context);
    void SetInitializerType(TypeSymbol* initializerType_, Context* context) noexcept;
    TypeSymbol* GetType(Context* context);
    TypeSymbol* GetReferredType(Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    TypeSymbol* declaredType;
    SymbolId declaredTypeId;
    TypeSymbol* initializerType;
    SymbolId initializerTypeId;
    Value* value;
    SymbolId valueId;
    std::int32_t index;
    int level;
    bool foundFromParent;
    std::int64_t nodeId;
    bool temporary;
    std::int32_t layoutIndex;
    VariableSymbol* global;
    bool contentFetched;
    void GetContent(Context* context);
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
    inline otava::ast::Node* DefaultValue() const noexcept { return defaultValue.get(); }
    void SetDefaultValue(otava::ast::Node* defaultValue_) noexcept;
    void SetType(TypeSymbol* type_, Context* context) noexcept;
    TypeSymbol* GetReferredType(Context* context);
    TypeSymbol* GetType(Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    AstNodeHeader astNodeHeader;
    ParameterKind parameterKind;
    std::unique_ptr<otava::ast::Node> defaultValue;
    SymbolId typeId;
    TypeSymbol* type;
};

} // namespace otava::symbols
