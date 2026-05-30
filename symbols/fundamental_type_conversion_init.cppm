// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.fundamental_type_conversion_init;

export namespace otava::symbols {

class Context;
class TypeSymbol;

void AddFundamentalTypeConversionsToSymboTable(TypeSymbol* charType,
    TypeSymbol* signedCharType, TypeSymbol* unsignedCharType, TypeSymbol* char8Type, TypeSymbol* char16Type, TypeSymbol* char32Type, TypeSymbol* wcharType,
    TypeSymbol* shortIntType, TypeSymbol* unsignedShortIntType, TypeSymbol* intType, TypeSymbol* unsignedIntType, TypeSymbol* longIntType, TypeSymbol* unsignedLongIntType,
    TypeSymbol* longLongIntType, TypeSymbol* unsignedLongLongIntType, TypeSymbol* floatType, TypeSymbol* doubleType, Context* context);

} // namespace otava::symbols
