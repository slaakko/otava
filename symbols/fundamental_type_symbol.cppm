// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.fundamental_type_symbol;

import std;
import otava.symbols.id;
import otava.symbols.type_symbol;

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

class FundamentalTypeSymbol : public TypeSymbol
{
public:
    FundamentalTypeSymbol(Module* module_, SymbolId symbolId_);
    FundamentalTypeSymbol(Module* module_, SymbolId symbolId_, FundamentalTypeKind fundamentalTypeKind_);
    inline FundamentalTypeKind GetFundamentalTypeKind() const noexcept { return fundamentalTypeKind; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    FundamentalTypeKind fundamentalTypeKind;
};

TypeSymbol* GetFundamentalType(DeclarationFlags fundamentalTypeFlags, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
