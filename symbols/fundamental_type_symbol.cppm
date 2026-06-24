// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.fundamental_type_symbol;

import std;
import otava.symbols.id;
import otava.symbols.type_symbol;
import otava.ast.node;

export namespace otava::symbols {

class Writer;
class Reader;

enum class FundamentalTypeKind : std::uint8_t
{
    none, boolType, charType, signedCharType, unsignedCharType, char8Type, char16Type,
    shortIntType, unsignedShortIntType, char32Type, wcharType, intType, unsignedIntType, longIntType, unsignedLongIntType,
    longLongIntType, unsignedLongLongIntType, floatType, doubleType, longDoubleType, voidType, autoType, nullPtrType,
    max
};

constexpr std::uint8_t ToUnderlying(FundamentalTypeKind fundamentalTypeKind)
{
    return std::uint8_t(fundamentalTypeKind);
}

std::string MakeFundamentalTypeName(FundamentalTypeKind fundamentalTypeKind);

int Rank(FundamentalTypeKind fundamentalTypeKind) noexcept;
bool IsSignedIntegerType(FundamentalTypeKind fundamentalTypeKind) noexcept;
bool IsUnsignedIntegerType(FundamentalTypeKind fundamentalTypeKind) noexcept;

class FundamentalTypeSymbol : public TypeSymbol
{
public:
    FundamentalTypeSymbol(Module* module_, SymbolId symbolId_);
    FundamentalTypeSymbol(Module* module_, SymbolId symbolId_, FundamentalTypeKind fundamentalTypeKind_);
    inline FundamentalTypeKind GetFundamentalTypeKind() const noexcept { return fundamentalTypeKind; }
    inline bool IsAutoTypeSymbol() const noexcept { return fundamentalTypeKind == FundamentalTypeKind::autoType; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    bool IsBoolType() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::boolType; }
    bool IsIntType() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::intType; }
    bool IsUnsignedShortType() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::unsignedShortIntType; }
    bool IsVoidType() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::voidType; }
    bool IsNullPtrType() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::nullPtrType; }
    bool IsDoubleType() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::doubleType; }
    bool IsFloatType() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::floatType; }
    bool IsCharTypeSymbol() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::charType; }
    bool IsChar8TypeSymbol() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::char8Type; }
    bool IsChar16TypeSymbol() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::char16Type; }
    bool IsChar32TypeSymbol() const noexcept override { return fundamentalTypeKind == FundamentalTypeKind::char32Type; }
    int Rank(Context* context) override { return otava::symbols::Rank(fundamentalTypeKind); }
    bool IsSignedIntegerType() const noexcept override { return otava::symbols::IsSignedIntegerType(fundamentalTypeKind);; }
    bool IsUnsignedIntegerType() const noexcept override { return otava::symbols::IsUnsignedIntegerType(fundamentalTypeKind);; }
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    FundamentalTypeKind fundamentalTypeKind;
};

TypeSymbol* GetFundamentalType(DeclarationFlags fundamentalTypeFlags, const soul::ast::FullSpan& fullSpan, Context* context);

void MakeFundamentaTypeSequence(FundamentalTypeSymbol* fundamentalType, const soul::ast::FullSpan& fullSpan, otava::ast::SequenceNode* sequence);

} // namespace otava::symbols
