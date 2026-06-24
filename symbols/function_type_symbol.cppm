// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.function_type_symbol;

import otava.symbols.id;
import otava.symbols.type_symbol;
import std;

export namespace otava::symbols {

class Writer;
class Reader;

struct FunctionTypeSymbolKey
{
    FunctionTypeSymbolKey();
    void Write(Writer& writer);
    void Read(Reader& reader);
    SymbolId returnTypeId;
    std::vector<SymbolId> parameterTypeIds;
};

struct FunctionTypeSymbolKeyHash
{
    size_t operator()(const FunctionTypeSymbolKey& key) const noexcept;
};

struct FunctionTypeSymbolKeyEqual
{
    bool operator()(const FunctionTypeSymbolKey& left, const FunctionTypeSymbolKey& right) const noexcept;
};

std::string MakeFunctionTypeName(TypeSymbol* returnType, const std::vector<TypeSymbol*>& parameterTypes, int& ptrIndex, bool makePtrType);

class FunctionTypeSymbol : public TypeSymbol
{
public:
    FunctionTypeSymbol(Module* module_, SymbolId id_);
    FunctionTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    inline void SetPtrIndex(int ptrIndex_) noexcept { ptrIndex = ptrIndex_; }
    int PtrIndex() const noexcept override { return ptrIndex; }
    inline void SetReturnType(TypeSymbol* returnType_) noexcept { returnType = returnType_; }
    TypeSymbol* ReturnType(Context* context);
    void AddParameterType(TypeSymbol* parameterType);
    const std::vector<TypeSymbol*>& ParameterTypes(Context* context);
    bool IsFunctionType() const noexcept override { return true; }
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    void Write(Writer& writer);
    void Read(Reader& reader);
private:
    TypeSymbol* returnType;
    SymbolId returnTypeId;
    std::vector<TypeSymbol*> parameterTypes;
    std::vector<SymbolId> parameterTypeIds;
    int ptrIndex;
    bool contentFetched;
    void GetContent(Context* context);
};

bool FunctionTypesEqual(FunctionTypeSymbol* left, FunctionTypeSymbol* right, Context* context) noexcept;

} // namespace otava::symbols
