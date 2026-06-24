// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.array_type_symbol;

import std;
import otava.symbols.function_symbol;
import otava.symbols.id;
import otava.symbols.type_symbol;

export namespace otava::symbols {

struct ArrayTypeKey
{
    ArrayTypeKey(TypeSymbol* elementType, std::int64_t size_);
    SymbolId elementTypeId;
    std::int64_t size;
};

struct ArrayTypeKeyHash
{
    size_t operator()(const ArrayTypeKey& key) const noexcept;
};

struct ArrayTypeKeyEqual
{
    inline bool operator()(const ArrayTypeKey& left, const ArrayTypeKey& right) const noexcept
    {
        return left.elementTypeId == right.elementTypeId && left.size == right.size;
    }
};

class Context;

std::string MakeArrayTypeName(TypeSymbol* elementType, std::int64_t size, Context* context);

class ArrayTypeSymbol : public TypeSymbol
{
public:
    ArrayTypeSymbol(Module* module_, SymbolId id_);
    ArrayTypeSymbol(Module* module_, SymbolId id_, TypeSymbol* elementType_, std::int64_t size_, Context* context);
    inline bool IsBound() const noexcept { return bound; }
    inline void SetBound() noexcept { bound = true; }
    void Bind(const soul::ast::FullSpan& fullSpan, Context* context);
    TypeSymbol* ElementType(Context* context) const noexcept;
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
    inline std::int64_t Size() const noexcept { return size; }
    inline void SetSize(std::int64_t size_) noexcept { size = size_; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    bool bound;
    mutable TypeSymbol* elementType;
    SymbolId elementTypeId;
    std::int64_t size;
};

class ArrayTypeDefaultCtor : public FunctionSymbol
{
public:
    ArrayTypeDefaultCtor(Module* module_, SymbolId id_);
    ArrayTypeDefaultCtor(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    ArrayTypeSymbol* arrayType;
    SymbolId arrayTypeId;
};

class ArrayTypeCopyCtor : public FunctionSymbol
{
public:
    ArrayTypeCopyCtor(Module* module_, SymbolId id_);
    ArrayTypeCopyCtor(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    ArrayTypeSymbol* arrayType;
    SymbolId arrayTypeId;
};

class ArrayTypeMoveCtor : public FunctionSymbol
{
public:
    ArrayTypeMoveCtor(Module* module_, SymbolId id_);
    ArrayTypeMoveCtor(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    ArrayTypeSymbol* arrayType;
    SymbolId arrayTypeId;
};

class ArrayTypeCopyAssignment : public FunctionSymbol
{
public:
    ArrayTypeCopyAssignment(Module* module_, SymbolId id_);
    ArrayTypeCopyAssignment(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    ArrayTypeSymbol* arrayType;
    SymbolId arrayTypeId;
};

class ArrayTypeMoveAssignment : public FunctionSymbol
{
public:
    ArrayTypeMoveAssignment(Module* module_, SymbolId id_);
    ArrayTypeMoveAssignment(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    ArrayTypeSymbol* arrayType;
    SymbolId arrayTypeId;
};

class ArrayTypeBegin : public FunctionSymbol
{
public:
    ArrayTypeBegin(Module* module_, SymbolId id_);
    ArrayTypeBegin(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    ArrayTypeSymbol* arrayType;
    SymbolId arrayTypeId;
};

class ArrayTypeEnd : public FunctionSymbol
{
public:
    ArrayTypeEnd(Module* module_, SymbolId id_);
    ArrayTypeEnd(Module* module_, SymbolId id_, ArrayTypeSymbol* arrayType_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    ArrayTypeSymbol* arrayType;
    SymbolId arrayTypeId;
};

} // namespace otava::symbols
