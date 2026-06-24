// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.argument_conversion_table;

import soul.ast.span;
import std;

export namespace otava::symbols {

class BoundExpressionNode;
class FunctionSymbol;
class Context;
class TypeSymbol;
class ClassTypeSymbol;
struct ArgumentMatch;
struct FunctionMatch;

class ArgumentConversion
{
public:
    virtual FunctionSymbol* Get(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg,
        ArgumentMatch& argumentMatch, FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan, Context* context) = 0;
};

class ArgumentConversionTable
{
public:
    ArgumentConversionTable();
    void AddArgumentConversion(ArgumentConversion* argumentConversion);
    FunctionSymbol* GetArgumentConversion(TypeSymbol* paramType, TypeSymbol* argType, const soul::ast::FullSpan& fullSpan, Context* context);
    FunctionSymbol* GetArgumentConversion(TypeSymbol* paramType, TypeSymbol* argType, BoundExpressionNode* arg, const soul::ast::FullSpan& fullSpan,
        ArgumentMatch& argumentMatch, FunctionMatch& functionMatch, Context* context);
    FunctionSymbol* GetAdjustDeletePtrConversionFn(TypeSymbol* thisPtrBaseType, Context* context);
    FunctionSymbol* GetDynamicPtrCastFn(TypeSymbol* baseClassPtr, TypeSymbol* derivedClassPtr, const soul::ast::FullSpan& fullSpan, Context* context);
private:
    std::vector<std::unique_ptr<ArgumentConversion>> argumentConversions;
    std::vector<std::unique_ptr<FunctionSymbol>> conversionFunctions;
    std::map<TypeSymbol*, FunctionSymbol*> adjustDeletePtrConversionFns;
    std::map<std::pair<TypeSymbol*, TypeSymbol*>, FunctionSymbol*> dynamicPtrCastFns;
};

} // namespace otava::symbols
