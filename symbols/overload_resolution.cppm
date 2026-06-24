// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.overload_resolution;

import otava.symbols.function_kind;
import otava.symbols.bound_tree;
import otava.symbols.template_param_compare;
import soul.ast.span;
import std;

export namespace otava::symbols {

class BoundFunctionCallNode;
class BoundExpressionNode;
class Context;
class Exception;
class FunctionSymbol;
class ClassTemplateSpecializationSymbol;
class TemplateParameterSymbol;

enum class OverloadResolutionFlags
{
    none = 0, dontInstantiate = 1 << 0, dontSearchArgumentScopes = 1 << 1, noMemberFunctions = 1 << 2
};

constexpr OverloadResolutionFlags operator|(OverloadResolutionFlags left, OverloadResolutionFlags right) noexcept
{
    return OverloadResolutionFlags(int(left) | int(right));
}

constexpr OverloadResolutionFlags operator&(OverloadResolutionFlags left, OverloadResolutionFlags right) noexcept
{
    return OverloadResolutionFlags(int(left) & int(right));
}

constexpr OverloadResolutionFlags operator~(OverloadResolutionFlags flags) noexcept
{
    return OverloadResolutionFlags(~int(flags));
}

const std::int32_t truncateConversionDistance = 10;
const std::int32_t unsignedToSignedConversionDistance = 10;

struct ArgumentMatch
{
    ArgumentMatch() noexcept;
    FunctionSymbol* conversionFun;
    ConversionKind conversionKind;
    std::int32_t distance;
    std::int32_t integerRank;
    std::int32_t fundamentalTypeDistance;
    OperationFlags preConversionFlags;
    OperationFlags postConversionFlags;
};

struct FunctionMatch
{
    FunctionMatch() noexcept;
    FunctionMatch(FunctionSymbol* function_, Context* context_) noexcept;
    FunctionMatch& operator=(const FunctionMatch& that);
    FunctionSymbol* function;
    Context* context;
    std::vector<ArgumentMatch> argumentMatches;
    int numConversions;
    int numQualifyingConversions;
    std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual> templateParameterMap;
    ClassTemplateSpecializationSymbol* specialization;
    std::vector<std::unique_ptr<BoundExpressionNode>> defaultArgs;
};

struct BetterFunctionMatch
{
    BetterFunctionMatch(Context* context_);
    inline bool operator()(const std::unique_ptr<FunctionMatch>& left, const std::unique_ptr<FunctionMatch>& right) const noexcept
    {
        return BetterFunctionMatch(context)(*left, *right);
    }
    bool operator()(const FunctionMatch& left, const FunctionMatch& right) const noexcept;
    Context* context;
};

bool FindTemplateParameterMatch(TypeSymbol* argType, TypeSymbol* paramType, BoundExpressionNode* arg, FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan,
    Context* context);

bool FindClassTemplateMatch(TypeSymbol* argType, TypeSymbol* paramType, BoundExpressionNode* arg, FunctionMatch& functionMatch,
    const soul::ast::FullSpan& fullSpan, Context* context);

bool FindClassTemplateSpecializationMatch(TypeSymbol* argType, TypeSymbol* paramType, BoundExpressionNode* arg, FunctionMatch& functionMatch,
    const soul::ast::FullSpan& fullSpan, Context* context);

BoundExpressionNode* MakeLvalueExpression(BoundExpressionNode* arg, const soul::ast::FullSpan& fullSpan, Context* context);

std::unique_ptr<BoundFunctionCallNode> CreateBoundConversionFunctionCall(FunctionSymbol* conversionFunction, BoundExpressionNode* arg,
    const soul::ast::FullSpan& fullSpan, Context* context);

std::unique_ptr<BoundFunctionCallNode> ResolveOverload(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, Exception& ex, FunctionMatch& functionMatch,
    OverloadResolutionFlags flags);

std::unique_ptr<BoundFunctionCallNode> ResolveOverload(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, Exception& ex, OverloadResolutionFlags flags);

std::unique_ptr<BoundFunctionCallNode> ResolveOverload(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, Exception& ex, FunctionMatch& functionMatch);

std::unique_ptr<BoundFunctionCallNode> ResolveOverload(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, Exception& ex);

std::unique_ptr<BoundFunctionCallNode> ResolveOverloadThrow(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, OverloadResolutionFlags flags);

std::unique_ptr<BoundFunctionCallNode> ResolveOverloadThrow(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
