// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.enums;

import otava.symbols.function_symbol;
import otava.symbols.type_symbol;
import otava.symbols.variable_symbol;
import otava.ast.node;
import std;

export namespace otava::symbols {

class Context;
class TypeSymbol;
class Value;

enum class EnumTypeKind : std::uint8_t
{
    enum_, enumClass, enumStruct
};

constexpr std::uint8_t ToUnderlying(EnumTypeKind kind)
{
    return std::uint8_t(kind);
}

class EnumeratedTypeSymbol : public TypeSymbol
{
public:
    EnumeratedTypeSymbol(Module* module_, SymbolId id_);
    EnumeratedTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    inline void SetUnderlyingType(TypeSymbol* underlyingType_) { underlyingType = underlyingType_; }
    TypeSymbol* UnderlyingType(Context* context);
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline EnumTypeKind GetEnumTypeKind() const noexcept { return enumTypeKind; }
    inline void SetEnumTypeKind(EnumTypeKind enumTypeKind_) { enumTypeKind = enumTypeKind_; }
    inline bool IsBound() const noexcept { return bound; }
    inline void SetBound() noexcept { bound = true; }
    int Rank(Context* context) override;
private:
    TypeSymbol* underlyingType;
    SymbolId underlyingTypeId;
    EnumTypeKind enumTypeKind;
    bool bound;
};

class ForwardEnumDeclarationSymbol : public TypeSymbol
{
public:
    ForwardEnumDeclarationSymbol(Module* module_, SymbolId id_);
    ForwardEnumDeclarationSymbol(Module* module_, SymbolId id_, const std::string& name_);
    inline void SetEnumeratedTypeSymbol(EnumeratedTypeSymbol* enumTypeSymbol_) noexcept { enumTypeSymbol = enumTypeSymbol_; }
    inline EnumeratedTypeSymbol* GetEnumeratedTypeSymbol() const noexcept { return enumTypeSymbol; }
private:
    EnumeratedTypeSymbol* enumTypeSymbol;
};

class EnumConstantSymbol : public Symbol
{
public:
    EnumConstantSymbol(Module* module_, SymbolId id_);
    EnumConstantSymbol(Module* module_, SymbolId id_, const std::string& name_);
    EnumeratedTypeSymbol* GetEnumType(Context* context);
    void SetEnumType(EnumeratedTypeSymbol* enumType_) noexcept;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    Value* GetValue(Context* context);
    void SetValue(Value* value_) noexcept;
private:
    Value* value;
    SymbolId valueId;
    EnumeratedTypeSymbol* enumType;
    SymbolId enumTypeId;
};

struct EnumTypeLessFunctor
{
    bool operator()(EnumeratedTypeSymbol* left, EnumeratedTypeSymbol* right) const noexcept;
};

class EnumTypeDefaultCtor : public FunctionSymbol
{
public:
    EnumTypeDefaultCtor(Module* module_, SymbolId id_);
    EnumTypeDefaultCtor(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    EnumeratedTypeSymbol* enumType;
    SymbolId enumTypeId;
};

class EnumTypeCopyCtor : public FunctionSymbol
{
public:
    EnumTypeCopyCtor(Module* module_, SymbolId id_);
    EnumTypeCopyCtor(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    EnumeratedTypeSymbol* enumType;
    SymbolId enumTypeId;
};

class EnumTypeMoveCtor : public FunctionSymbol
{
public:
    EnumTypeMoveCtor(Module* module_, SymbolId id_);
    EnumTypeMoveCtor(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    EnumeratedTypeSymbol* enumType;
    SymbolId enumTypeId;
};

class EnumTypeCopyAssignment : public FunctionSymbol
{
public:
    EnumTypeCopyAssignment(Module* module_, SymbolId id_);
    EnumTypeCopyAssignment(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    EnumeratedTypeSymbol* enumType;
    SymbolId enumTypeId;
};

class EnumTypeMoveAssignment : public FunctionSymbol
{
public:
    EnumTypeMoveAssignment(Module* module_, SymbolId id_);
    EnumTypeMoveAssignment(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    EnumeratedTypeSymbol* enumType;
    SymbolId enumTypeId;
};

class EnumTypeEqual : public FunctionSymbol
{
public:
    EnumTypeEqual(Module* module_, SymbolId id_);
    EnumTypeEqual(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    EnumeratedTypeSymbol* enumType;
    SymbolId enumTypeId;
};

class EnumTypeLess : public FunctionSymbol
{
public:
    EnumTypeLess(Module* module_, SymbolId id_);
    EnumTypeLess(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    EnumeratedTypeSymbol* enumType;
    SymbolId enumTypeId;
};

void BeginEnumType(otava::ast::Node* node, Context* context);
void AddEnumerators(otava::ast::Node* node, Context* context);
void EndEnumType(otava::ast::Node* node, Context* context);
void ProcessEnumForwardDeclaration(otava::ast::Node* node, Context* context);
void BindEnumType(EnumeratedTypeSymbol* enumType, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
