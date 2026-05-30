// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.fundamental_type_conversion_init;

import otava.symbols.context;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.fundamental_type_conversion;
import soul.ast.span;

namespace otava::symbols {

void AddFundamentalTypeConversionsToSymboTable(
    TypeSymbol* charType, TypeSymbol* signedCharType, TypeSymbol* unsignedCharType, TypeSymbol* char8Type, TypeSymbol* char16Type, TypeSymbol* char32Type, 
    TypeSymbol* wcharType, TypeSymbol* shortIntType, TypeSymbol* unsignedShortIntType, TypeSymbol* intType, TypeSymbol* unsignedIntType, TypeSymbol* longIntType, 
    TypeSymbol* unsignedLongIntType, TypeSymbol* longLongIntType, TypeSymbol* unsignedLongLongIntType, TypeSymbol* floatType, TypeSymbol* doubleType, Context* context)
{
    soul::ast::FullSpan fullSpan;
    TypeSymbol* boolType = context->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(), 
        new FundamentalTypeBooleanConversion(context->GetModule(), 
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), charType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(), 
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), signedCharType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), char8Type, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), char32Type, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), wcharType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), unsignedShortIntType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), intType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), unsignedIntType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), longIntType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), unsignedLongIntType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), longLongIntType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), unsignedLongLongIntType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), floatType, boolType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBooleanConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolean), doubleType, boolType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(), 
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(), 
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, signedCharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, unsignedCharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, shortIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, unsignedShortIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, unsignedIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, longIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, unsignedLongIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, longLongIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBoolToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBoolToInt), boolType, unsignedLongLongIntType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, charType, signedCharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, charType, unsignedCharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, charType, char8Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, charType, char16Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, charType, char32Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, charType, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, charType, shortIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, charType, unsignedShortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, charType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, charType, unsignedIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, charType, longIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, charType, unsignedLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, charType, longLongIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, charType, unsignedLongLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, charType, floatType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, charType, doubleType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, signedCharType, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, signedCharType, unsignedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, signedCharType, char8Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, signedCharType, char16Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, signedCharType, char32Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, signedCharType, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, signedCharType, shortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, signedCharType, unsignedShortIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, signedCharType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, signedCharType, unsignedIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, signedCharType, longIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, signedCharType, unsignedLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, signedCharType, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, signedCharType, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, signedCharType, floatType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, signedCharType, doubleType, context), 
        fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedCharType, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedCharType, signedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedCharType, char8Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedCharType, char16Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, unsignedCharType, char32Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, unsignedCharType, wcharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedCharType, shortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedCharType, unsignedShortIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, unsignedCharType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, unsignedCharType, unsignedIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, unsignedCharType, longIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, unsignedCharType, unsignedLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, unsignedCharType, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, unsignedCharType, unsignedLongLongIntType, context), 
            fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, unsignedCharType, floatType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, unsignedCharType, doubleType, context), 
        fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char8Type, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char8Type, signedCharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char8Type, unsignedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, char8Type, char16Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char8Type, char32Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char8Type, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, char8Type, shortIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, char8Type, unsignedShortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char8Type, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char8Type, unsignedIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char8Type, longIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char8Type, unsignedLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, char8Type, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, char8Type, unsignedLongLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, char8Type, floatType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, char8Type, doubleType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char16Type, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char16Type, signedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char16Type, unsignedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char16Type, char8Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, char16Type, char32Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, char16Type, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char16Type, shortIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char16Type, unsignedShortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char16Type, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char16Type, unsignedIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char16Type, longIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 3, ConversionKind::explicitConversion, char16Type, unsignedLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, char16Type, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, char16Type, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, char16Type, floatType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, char16Type, doubleType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char32Type, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char32Type, signedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char32Type, unsignedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char32Type, char8Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char32Type, char16Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char32Type, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char32Type, shortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, char32Type, unsignedShortIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char32Type, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char32Type, unsignedIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char32Type, longIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, char32Type, unsignedLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, char32Type, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, char32Type, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, char32Type, floatType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, char32Type, doubleType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, wcharType, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, wcharType, signedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, wcharType, unsignedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, wcharType, char8Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, wcharType, char16Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, wcharType, char32Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, wcharType, shortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, wcharType, unsignedShortIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, wcharType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, wcharType, unsignedIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, wcharType, longIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, wcharType, unsignedLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, wcharType, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, wcharType, unsignedLongLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, wcharType, floatType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, wcharType, doubleType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, shortIntType, charType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, shortIntType, signedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, shortIntType, unsignedCharType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, shortIntType, char8Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, shortIntType, char16Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, shortIntType, char32Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, shortIntType, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, shortIntType, unsignedShortIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, shortIntType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, shortIntType, unsignedIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, shortIntType, longIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, shortIntType, unsignedLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, shortIntType, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, shortIntType, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, shortIntType, floatType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, shortIntType, doubleType, context), 
        fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedShortIntType, charType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedShortIntType, signedCharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedShortIntType, char8Type, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedShortIntType, char16Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedShortIntType, char32Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedShortIntType, wcharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedShortIntType, shortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedShortIntType, intType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedShortIntType, unsignedIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedShortIntType, longIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedShortIntType, unsignedLongIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, unsignedShortIntType, longLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 4, ConversionKind::explicitConversion, unsignedShortIntType, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, unsignedShortIntType, floatType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, unsignedShortIntType, doubleType, context), 
        fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, intType, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, intType, signedCharType, context),
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, intType, char8Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, intType, char16Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, intType, char32Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, intType, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, intType, shortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, intType, unsignedShortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, intType, unsignedIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, intType, longIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, intType, unsignedLongIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, intType, longLongIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, intType, unsignedLongLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, intType, floatType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, intType, doubleType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedIntType, charType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedIntType, signedCharType, context),
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedIntType, char8Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedIntType, char16Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedIntType, char32Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedIntType, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedIntType, shortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedIntType, unsignedShortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedIntType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedIntType, longIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedIntType, unsignedLongIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedIntType, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedIntType, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, unsignedIntType, floatType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, unsignedIntType, doubleType, context), 
        fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longIntType, charType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, longIntType, signedCharType, context),
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longIntType, char8Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longIntType, char16Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, longIntType, char32Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, longIntType, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, longIntType, shortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longIntType, unsignedShortIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, longIntType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, longIntType, unsignedIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, longIntType, unsignedLongIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, longIntType, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, longIntType, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, longIntType, floatType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, longIntType, doubleType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongIntType, charType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongIntType, signedCharType, context),
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongIntType, char8Type, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongIntType, char16Type, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedLongIntType, char32Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedLongIntType, wcharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongIntType, shortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongIntType, unsignedShortIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedLongIntType, intType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedLongIntType, unsignedIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedLongIntType, longIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedLongIntType, longLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 2, ConversionKind::explicitConversion, unsignedLongIntType, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, unsignedLongIntType, floatType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, unsignedLongIntType, doubleType, context), 
        fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longLongIntType, charType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, longLongIntType, signedCharType, context),
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longLongIntType, char8Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longLongIntType, char16Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longLongIntType, char32Type, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longLongIntType, wcharType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, longLongIntType, shortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longLongIntType, unsignedShortIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, longLongIntType, intType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longLongIntType, unsignedIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeSignExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeSignExtension), 1, ConversionKind::implicitConversion, longLongIntType, longIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, longLongIntType, unsignedLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, longLongIntType, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, longLongIntType, floatType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, longLongIntType, doubleType, context), 
        fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, charType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, signedCharType, context),
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, char8Type, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, char16Type, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, char32Type, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, wcharType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, shortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, 
            unsignedShortIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, intType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, unsignedIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, longIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeZeroExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeZeroExtension), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, 
            unsignedLongIntType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeBitcastConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeBitcast), 1, ConversionKind::implicitConversion, unsignedLongLongIntType, longLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 5, ConversionKind::explicitConversion, unsignedLongLongIntType, floatType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatToIntConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatToInt), 6, ConversionKind::explicitConversion, unsignedLongLongIntType, doubleType, 
            context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, signedCharType, context),
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, char8Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, char16Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, char32Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, shortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, unsignedShortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, unsignedIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, longIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, floatType, unsignedLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 2, ConversionKind::explicitConversion, floatType, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 2, ConversionKind::explicitConversion, floatType, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeTruncateConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeTruncate), 6, ConversionKind::explicitConversion, floatType, doubleType, context), fullSpan, context);

    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, charType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, signedCharType, context),
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, char8Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, char16Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, char32Type, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, wcharType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, shortIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, unsignedShortIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, intType, context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, unsignedIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, longIntType,
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, unsignedLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, longLongIntType, context), 
        fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeIntToFloatConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeIntToFloat), 1, ConversionKind::implicitConversion, doubleType, unsignedLongLongIntType, 
            context), fullSpan, context);
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(),
        new FundamentalTypeFloatingPointExtendConversion(context->GetModule(),
            context->GetNextSymbolId(SymbolKind::fundamentalTypeFloatingPointExtension), 1, ConversionKind::implicitConversion, doubleType, floatType, 
            context), fullSpan, context);
}

} // namespace otava::symbols
