// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.fundamental_type_kind;

import std;

export namespace otava::symbols {

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

} // namespace otava::symbols
