// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.overload_resolution;

import otava.symbols.argument_conversion_table;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.expression_binder;
import otava.symbols.function_templates;
import otava.symbols.function_symbol;
import otava.symbols.inline_functions;
import otava.symbols.operation_repository;
import otava.symbols.templates;
import otava.symbols.type_symbol;
import otava.symbols.type_compare;
import otava.symbols.variable_symbol;
import otava.optimizer;
import otava.ast.literal;

namespace otava::symbols {

ArgumentMatch::ArgumentMatch() noexcept :
    conversionFun(nullptr),
    conversionKind(ConversionKind::implicitConversion),
    distance(0),
    integerRank(-1),
    fundamentalTypeDistance(0),
    preConversionFlags(OperationFlags::none),
    postConversionFlags(OperationFlags::none)
{
}

std::int32_t ArgumentConversionValue(const ArgumentMatch& match) noexcept
{
    std::int32_t argumentConversionValue = 0;
    if (match.conversionFun)
    {
        argumentConversionValue = 100;
        if (match.conversionKind == ConversionKind::explicitConversion)
        {
            argumentConversionValue += 10;
        }
        argumentConversionValue += match.distance;
    }
    return argumentConversionValue;
}

struct BetterArgumentMatch
{
    bool operator()(const ArgumentMatch& left, const ArgumentMatch& right) const noexcept;
};

bool BetterArgumentMatch::operator()(const ArgumentMatch& left, const ArgumentMatch& right) const noexcept
{
    if (left.conversionFun == nullptr && right.conversionFun != nullptr) return true;
    if (right.conversionFun == nullptr && left.conversionFun != nullptr) return false;
    if (left.conversionKind == ConversionKind::implicitConversion && right.conversionKind == ConversionKind::explicitConversion) return true;
    if (left.conversionKind == ConversionKind::explicitConversion && right.conversionKind == ConversionKind::implicitConversion) return false;
    if (left.distance < right.distance) return true;
    if (left.distance > right.distance) return false;
    if (left.integerRank != -1 && right.integerRank != -1)
    {
        if (left.integerRank < right.integerRank) return true;
        if (left.integerRank > right.integerRank) return false;
    }
    if (left.fundamentalTypeDistance < right.fundamentalTypeDistance) return true;
    if (left.fundamentalTypeDistance > right.fundamentalTypeDistance) return false;
    return false;
}

FunctionMatch::FunctionMatch() noexcept : function(nullptr), numConversions(0), numQualifyingConversions(0), specialization(nullptr), context(nullptr)
{
}

FunctionMatch::FunctionMatch(FunctionSymbol* function_, Context* context_) noexcept :
    function(function_), context(context_), numConversions(0), numQualifyingConversions(0), specialization(nullptr)
{
}

FunctionMatch& FunctionMatch::operator=(const FunctionMatch& that)
{
    function = that.function;
    context = that.context;
    argumentMatches = that.argumentMatches;
    numConversions = that.numConversions;
    numQualifyingConversions = that.numQualifyingConversions;
    templateParameterMap = that.templateParameterMap;
    specialization = that.specialization;
    return *this;
}

BetterFunctionMatch::BetterFunctionMatch(Context* context_) : context(context_)
{
}

bool BetterFunctionMatch::operator()(const FunctionMatch& left, const FunctionMatch& right) const noexcept
{
    if (left.function->IsTemplate(context) && !right.function->IsTemplate(context))
    {
        return true;
    }
    if (!left.function->IsTemplate(context) && right.function->IsTemplate(context))
    {
        return false;
    }
    BetterArgumentMatch betterArgumentMatch;
    int leftBetterArgumentMatches = 0;
    int rightBetterArgumentMatches = 0;
    int n = std::max(int(left.argumentMatches.size()), int(right.argumentMatches.size()));
    for (int i = 0; i < n; ++i)
    {
        ArgumentMatch leftMatch;
        if (i < int(left.argumentMatches.size()))
        {
            leftMatch = left.argumentMatches[i];
        }
        ArgumentMatch rightMatch;
        if (i < int(right.argumentMatches.size()))
        {
            rightMatch = right.argumentMatches[i];
        }
        if (betterArgumentMatch(leftMatch, rightMatch))
        {
            ++leftBetterArgumentMatches;
        }
        else if (betterArgumentMatch(rightMatch, leftMatch))
        {
            ++rightBetterArgumentMatches;
        }
    }
    if (leftBetterArgumentMatches > rightBetterArgumentMatches)
    {
        return true;
    }
    if (rightBetterArgumentMatches > leftBetterArgumentMatches)
    {
        return false;
    }
    int leftValue = 0;
    int rightValue = 0;
    for (int i = 0; i < n; ++i)
    {
        ArgumentMatch leftMatch;
        if (i < int(left.argumentMatches.size()))
        {
            leftMatch = left.argumentMatches[i];
        }
        ArgumentMatch rightMatch;
        if (i < int(right.argumentMatches.size()))
        {
            rightMatch = right.argumentMatches[i];
        }
        leftValue += ArgumentConversionValue(leftMatch);
        rightValue += ArgumentConversionValue(rightMatch);
    }
    if (leftValue < rightValue)
    {
        return true;
    }
    if (leftValue > rightValue)
    {
        return false;
    }
    if (left.numConversions < right.numConversions)
    {
        return true;
    }
    if (left.numConversions > right.numConversions)
    {
        return false;
    }
    if (left.numQualifyingConversions < right.numQualifyingConversions)
    {
        return true;
    }
    if (left.numQualifyingConversions > right.numQualifyingConversions)
    {
        return false;
    }
    if (left.function->IsFunctionDefinitionSymbol() && !right.function->IsFunctionDefinitionSymbol())
    {
        return true;
    }
    if (!left.function->IsFunctionDefinitionSymbol() && right.function->IsFunctionDefinitionSymbol())
    {
        return false;
    }
    if (!left.function->IsSpecialization() && right.function->IsSpecialization())
    {
        return true;
    }
    if (left.function->IsSpecialization() && !right.function->IsSpecialization())
    {
        return false;
    }
    if ((left.function->IsTemplate(context) || left.function->IsMemFnOfClassTemplate(context)) && 
        !(right.function->IsTemplate(context) || right.function->IsMemFnOfClassTemplate(context)))
    {
        return true;
    }
    if (!(left.function->IsTemplate(context) || left.function->IsMemFnOfClassTemplate(context)) && 
        (right.function->IsTemplate(context) || right.function->IsMemFnOfClassTemplate(context)))
    {
        return false;
    }
    if (left.function->IsConst() && !right.function->IsConst())
    {
        return true;
    }
    if (!left.function->IsConst() && right.function->IsConst())
    {
        return false;
    }
    if (left.function->Arity() > Cardinality(0) && right.function->Arity() > Cardinality(0))
    {
        TypeSymbol* leftType = left.function->Parameters(context)[0]->GetType(context);
        TypeSymbol* rightType = right.function->Parameters(context)[0]->GetType(context);
        if (leftType->IsUnsignedIntegerType() && rightType->IsSignedIntegerType())
        {
            return true;
        }
        else if (leftType->IsSignedIntegerType() && rightType->IsUnsignedIntegerType())
        {
            return false;
        }
        else if (leftType->IsChar32TypeSymbol() && rightType->IsIntType())
        {
            return true;
        }
        else if (leftType->IsIntType() && rightType->IsChar32TypeSymbol())
        {
            return false;
        }
        else if (leftType->IsChar16TypeSymbol() && rightType->IsUnsignedShortType())
        {
            return true;
        }
        else if (leftType->IsUnsignedShortType() && rightType->IsChar16TypeSymbol())
        {
            return false;
        }
    }
    return false;
}

BoundExpressionNode* MakeLvalueExpression(BoundExpressionNode* arg, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (arg->IsLvalueExpression(context)) return arg;
    BoundVariableNode* backingStore = nullptr;
    if (context->GetBoundFunction())
    {
        std::int64_t nodeId = context->NodeId();
        VariableSymbol* temporary = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(arg->GetType(), nodeId, context);
        backingStore = new BoundVariableNode(temporary, fullSpan, temporary->GetReferredType(context));
    }
    return new BoundTemporaryNode(arg, backingStore, fullSpan);
}

std::unique_ptr<BoundFunctionCallNode> CreateBoundFunctionCall(FunctionMatch& functionMatch, std::vector<std::unique_ptr<BoundExpressionNode>>& args,
    const soul::ast::FullSpan& fullSpan, Exception& ex, Context* context)
{
    TypeSymbol* type = functionMatch.function->ReturnType(context);
    if (type)
    {
        type = type->DirectType(context)->FinalType(fullSpan, context);
    }
    std::unique_ptr<BoundFunctionCallNode> boundFunctionCall(new BoundFunctionCallNode(functionMatch.function, fullSpan, type));
    int n = args.size();
    int m = functionMatch.defaultArgs.size();
    int count = n + m;
    for (int i = 0; i < count; ++i)
    {
        std::unique_ptr<BoundExpressionNode> arg;
        if (i >= n)
        {
            arg.reset(functionMatch.defaultArgs[i - n].release());
        }
        else
        {
            arg.reset(args[i].release());
        }
        if (i == 0 && !functionMatch.function->IsMemberFunction(context) && functionMatch.function->IsCtorAssignmentOrArrow())
        {
            if (arg->IsBoundAddressOfNode())
            {
                BoundAddressOfNode* addrOfNode = static_cast<BoundAddressOfNode*>(arg.release());
                BoundExpressionNode* subject = addrOfNode->ReleaseSubject();
                args[i].reset(subject);
                arg.reset(args[i].release());
            }
        }
        ArgumentMatch& argumentMatch = functionMatch.argumentMatches[i];
        if (argumentMatch.preConversionFlags == OperationFlags::addr)
        {
            arg.reset(MakeLvalueExpression(arg.release(), fullSpan, context));
            TypeSymbol* type = nullptr;
            if (arg->GetType()->IsClassTypeSymbol() && arg->GetFlag(BoundExpressionFlags::bindToRvalueRef))
            {
                type = arg->GetType()->AddRValueRef(context);
            }
            else
            {
                type = arg->GetType()->AddLValueRef(context);
            }
            arg.reset(new BoundAddressOfNode(arg.release(), fullSpan, type));
        }
        else if (argumentMatch.preConversionFlags == OperationFlags::deref)
        {
            TypeSymbol* type = arg->GetType()->RemoveReference(context);
            arg.reset(new BoundDereferenceNode(arg.release(), fullSpan, type));
        }
        if (argumentMatch.conversionFun)
        {
            TypeSymbol* argType = arg->GetType();
            FunctionSymbol* conversionFun = argumentMatch.conversionFun;
            if (conversionFun->GetConversionKind() == ConversionKind::explicitConversion && !context->GetFlag(ContextFlags::suppress_warning))
            {
                ex = Exception("warning: ", "implicit conversion from '" + conversionFun->GetConversionArgType(context)->FullName(context) + "' to '" +
                    conversionFun->GetConversionParamType(context)->FullName(context) + "'", fullSpan, context);
                ex.SetWarning();
            }
            if (conversionFun->GetFunctionKind() == FunctionKind::conversionMemFn && argType->PlainType(context)->IsClassTypeSymbol())
            {
                if (argType->IsReferenceType())
                {
                    TypeSymbol* type = argType->RemoveReference(context)->AddPointer(context);
                    arg.reset(new BoundRefToPtrNode(arg.release(), fullSpan, type));
                }
                else
                {
                    TypeSymbol* type = argType->GetBaseType(context)->AddPointer(context);
                    arg.reset(new BoundAddressOfNode(arg.release(), fullSpan, type));
                }
                BoundFunctionCallNode* functionCall = new BoundFunctionCallNode(conversionFun, fullSpan, conversionFun->ReturnType(context));
                functionCall->AddArgument(arg.release());
                arg.reset(functionCall);
            }
            else if (conversionFun->GetFunctionKind() == FunctionKind::constructor)
            {
                VariableSymbol* temporary = nullptr;
                BoundExpressionNode* temporaryDestructorCallArg = nullptr;
                BoundFunctionCallNode* constructorCall = new BoundFunctionCallNode(conversionFun, fullSpan, nullptr);
                BoundAddressOfNode* temporaryArg = nullptr;
                BoundExpressionNode* boundTemporary2 = nullptr;
                int level = 0;
                bool invoke = context->GetFlag(ContextFlags::invoke);
                if (invoke)
                {
                    std::int64_t nodeId = context->NodeId() ^ context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetArgumentId(i);
                    std::pair<VariableSymbol*, int> temporaryLevel = GetParentTemporary(nodeId, context);
                    temporary = temporaryLevel.first;
                    level = temporaryLevel.second;
                    TypeSymbol* type = temporary->GetReferredType(context);
                    BoundParentVariableNode* boundParentVariable = new BoundParentVariableNode(temporary, fullSpan, type);
                    boundParentVariable->SetLevel(level);
                    temporaryArg = new BoundAddressOfNode(boundParentVariable, fullSpan, temporary->GetType(context)->AddPointer(context));
                    temporaryDestructorCallArg = temporaryArg;
                    boundTemporary2 = new BoundParentVariableNode(temporary, fullSpan, type);
                }
                else
                {
                    std::int64_t nodeId = context->NodeId() ^ context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetArgumentId(i);
                    temporary = context->GetBoundFunction()->GetFunctionDefinitionSymbol()->CreateTemporary(
                        conversionFun->GetConversionParamType(context), nodeId, context);
                    TypeSymbol* type = temporary->GetReferredType(context);
                    BoundVariableNode* boundTemporary = new BoundVariableNode(temporary, fullSpan, type);
                    temporaryArg = new BoundAddressOfNode(boundTemporary, fullSpan, temporary->GetType(context)->AddPointer(context));
                    temporaryDestructorCallArg = temporaryArg;
                    boundTemporary2 = new BoundVariableNode(temporary, fullSpan, type);
                }
                constructorCall->AddArgument(temporaryArg);
                if (argumentMatch.preConversionFlags == OperationFlags::addr)
                {
                    TypeSymbol* type = arg->GetType()->AddPointer(context);
                    arg.reset(new BoundAddressOfNode(MakeLvalueExpression(arg.release(), fullSpan, context), fullSpan, type));
                }
                else if (argumentMatch.preConversionFlags == OperationFlags::deref)
                {
                    TypeSymbol* type = arg->GetType()->RemoveReference(context);
                    arg.reset(new BoundDereferenceNode(arg.release(), fullSpan, type));
                }
                constructorCall->AddArgument(arg.release());
                if (argumentMatch.postConversionFlags == OperationFlags::addr)
                {
                    boundTemporary2 = new BoundAddressOfNode(MakeLvalueExpression(boundTemporary2, fullSpan, context), fullSpan,
                        boundTemporary2->GetType()->AddPointer(context));
                }
                else if (argumentMatch.postConversionFlags == OperationFlags::deref)
                {
                    boundTemporary2 = new BoundDereferenceNode(boundTemporary2, fullSpan, boundTemporary2->GetType()->RemoveReference(context));
                }
                argumentMatch.postConversionFlags = OperationFlags::none;
                BoundConstructTemporaryNode* constructTemporary = new BoundConstructTemporaryNode(constructorCall, boundTemporary2, fullSpan);
                CheckGenerateTemporaryDestructorCall(constructTemporary, temporaryDestructorCallArg, context);
                arg.reset(constructTemporary);
            }
            else
            {
                TypeSymbol* type = conversionFun->ReturnType(context);
                arg.reset(new BoundConversionNode(arg.release(), conversionFun, fullSpan, type));
            }
        }
        if (argumentMatch.postConversionFlags == OperationFlags::addr)
        {
            TypeSymbol* type = arg->GetType()->AddPointer(context);
            arg.reset(new BoundAddressOfNode(MakeLvalueExpression(arg.release(), fullSpan, context), fullSpan, type));
        }
        else if (argumentMatch.postConversionFlags == OperationFlags::deref)
        {
            TypeSymbol* type = arg->GetType()->RemoveReference(context);
            arg.reset(new BoundDereferenceNode(arg.release(), fullSpan, type));
        }
        boundFunctionCall->AddArgument(arg.release());
    }
    if (context->GetFlag(ContextFlags::debugMemory))
    {
        std::string fullName = boundFunctionCall->GetFunctionSymbol()->FullName(context);
        if (fullName == "::operator new(unsigned long long int)")
        {
            std::vector<TypeSymbol*> templateArgs;
            std::vector<std::unique_ptr<BoundExpressionNode>> debugOpNewArgs;
            debugOpNewArgs.push_back(std::unique_ptr<BoundExpressionNode>(boundFunctionCall.release()));
            otava::ast::StringLiteralNode fnAst(fullSpan.span, fullSpan.fileIndex, context->Function(), otava::ast::EncodingPrefix::none, std::string());
            std::unique_ptr<BoundExpressionNode> fn = BindExpression(&fnAst, context);
            debugOpNewArgs.push_back(std::move(fn));
            otava::ast::StringLiteralNode sfpAst(fullSpan.span, fullSpan.fileIndex, context->FileName(), otava::ast::EncodingPrefix::none, std::string());
            std::unique_ptr<BoundExpressionNode> sfp = BindExpression(&sfpAst, context);
            debugOpNewArgs.push_back(std::move(sfp));
            otava::ast::IntegerLiteralNode lnAst(fullSpan.span, fullSpan.fileIndex, context->Line(), otava::ast::Suffix::none, otava::ast::Base::decimal, std::string());
            std::unique_ptr<BoundExpressionNode> ln = BindExpression(&lnAst, context);
            debugOpNewArgs.push_back(std::move(ln));
            std::unique_ptr<BoundFunctionCallNode> debugOperatorNewCall = ResolveOverloadThrow(context->GetSymbolTable()->GlobalNs()->GetScope(),
                "ort_debug_operator_new", templateArgs, debugOpNewArgs, fullSpan, context);
            return debugOperatorNewCall;
        }
    }
    return boundFunctionCall;
}

std::unique_ptr<BoundFunctionCallNode> CreateBoundConversionFunctionCall(FunctionSymbol* conversionFunction, BoundExpressionNode* arg,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    Exception ex;
    FunctionMatch functionMatch(conversionFunction, context);
    ArgumentMatch argumentMatch;
    functionMatch.argumentMatches.push_back(argumentMatch);
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    if (arg->GetType()->IsClassTypeSymbol())
    {
        arg = new BoundAddressOfNode(arg, fullSpan, arg->GetType()->AddPointer(context));
    }
    args.push_back(std::unique_ptr<BoundExpressionNode>(arg));
    return CreateBoundFunctionCall(functionMatch, args, fullSpan, ex, context);
}

bool FindQualificationConversion(TypeSymbol* argType, TypeSymbol* paramType, BoundExpressionNode* arg, FunctionMatch& functionMatch, ArgumentMatch& argumentMatch) noexcept
{
    int distance = 0;
    int fundamentalTypeDistance = 0;
    if (paramType->IsFundamentalTypeSymbol())
    {
        FundamentalTypeSymbol* fundamentalTypeSymbol = static_cast<FundamentalTypeSymbol*>(paramType);
        fundamentalTypeDistance = static_cast<std::uint8_t>(fundamentalTypeSymbol->GetFundamentalTypeKind());
        argumentMatch.fundamentalTypeDistance = fundamentalTypeDistance;
    }
    if (argumentMatch.conversionFun)
    {
        argumentMatch.conversionKind = argumentMatch.conversionFun->GetConversionKind();
        distance = argumentMatch.conversionFun->ConversionDistance();
    }
    if (paramType->IsRValueRefType())
    {
        if (!argType->IsRValueRefType())
        {
            ++functionMatch.numQualifyingConversions;
            distance += 5;
        }
    }
    if (argType->IsConstType())
    {
        if (paramType->IsConstType() || !paramType->IsReferenceType())
        {
            ++distance;
        }
        else
        {
            distance = 255;
        }
    }
    else
    {
        if (paramType->IsConstType())
        {
            distance += 2;
        }
        else if (!paramType->IsRValueRefType())
        {
            distance += 3;
        }
    }
    if (argType->IsReferenceType() && !paramType->IsReferenceType())
    {
        argumentMatch.postConversionFlags = OperationFlags::deref;
        argumentMatch.distance = distance;
        ++functionMatch.numQualifyingConversions;
        return true;
    }
    else if (!argType->IsReferenceType() && paramType->IsReferenceType())
    {
        argumentMatch.postConversionFlags = OperationFlags::addr;
        if (paramType->IsRValueRefType() && arg->BindToRvalueRef())
        {
            argumentMatch.distance = 0;
            return true;
        }
        argumentMatch.distance = distance;
        ++functionMatch.numQualifyingConversions;
        return true;
    }
    else if (argType->IsConstType() && !paramType->IsConstType())
    {
        ++functionMatch.numQualifyingConversions;
        ++distance;
        if (argType->IsLValueRefType() && paramType->IsRValueRefType())
        {
            ++distance;
            ++functionMatch.numQualifyingConversions;
        }
        argumentMatch.distance = distance;
        return true;
    }
    else if (!argType->IsConstType() && paramType->IsConstType())
    {
        ++functionMatch.numQualifyingConversions;
        ++distance;
        if (argType->IsLValueRefType() && paramType->IsRValueRefType())
        {
            ++distance;
            ++functionMatch.numQualifyingConversions;
        }
        argumentMatch.distance = distance;
        return true;
    }
    else if (argType->IsLValueRefType() && paramType->IsRValueRefType())
    {
        ++distance;
        ++functionMatch.numQualifyingConversions;
        argumentMatch.distance = distance;
        return true;
    }
    else if (argType->IsRValueRefType() && paramType->IsLValueRefType())
    {
        ++distance;
        ++functionMatch.numQualifyingConversions;
        argumentMatch.distance = distance;
        return true;
    }
    else if (argumentMatch.conversionFun)
    {
        argumentMatch.distance = distance;
        return true;
    }
    return false;
}

bool FindTemplateParameterMatch(TypeSymbol* argType, TypeSymbol* paramType, BoundExpressionNode* arg, FunctionMatch& functionMatch, const soul::ast::FullSpan& fullSpan,
    Context* context)
{
    if (!paramType->GetBaseType(context)->IsTemplateParameterSymbol()) return false;
    TemplateParameterSymbol* templateParameter = static_cast<TemplateParameterSymbol*>(paramType->GetBaseType(context));
    TypeSymbol* templateArgumentType = nullptr;
    auto it = functionMatch.templateParameterMap.find(templateParameter);
    if (it == functionMatch.templateParameterMap.cend())
    {
        templateArgumentType = argType->RemoveDerivations(paramType->GetDerivations(), context);
        if (templateArgumentType)
        {
            functionMatch.templateParameterMap[templateParameter] = templateArgumentType;
        }
        else
        {
            return false;
        }
    }
    else
    {
        templateArgumentType = it->second;
    }
    paramType = paramType->Unify(templateArgumentType, context);
    if (!paramType)
    {
        return false;
    }
    if (TypesEqual(argType, paramType, context))
    {
        ArgumentMatch argumentMatch;
        if (argType->IsClassTypeSymbol() && paramType->IsClassTypeSymbol())
        {
            argumentMatch.postConversionFlags = argumentMatch.postConversionFlags | OperationFlags::addr;
        }
        functionMatch.argumentMatches.push_back(argumentMatch);
        return true;
    }
    else
    {
        bool qualificationConversionMatch = false;
        ArgumentMatch argumentMatch;
        if (TypesEqual(argType->PlainType(context), paramType->PlainType(context), context))
        {
            qualificationConversionMatch = FindQualificationConversion(argType, paramType, arg, functionMatch, argumentMatch);
            if (qualificationConversionMatch)
            {
                functionMatch.argumentMatches.push_back(argumentMatch);
                return true;
            }
        }
        else
        {
            FunctionSymbol* conversionFun = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                paramType, argType, arg, fullSpan, argumentMatch, functionMatch, context);
            if (conversionFun)
            {
                ++functionMatch.numConversions;
                argumentMatch.conversionFun = conversionFun;
                if (argumentMatch.preConversionFlags == OperationFlags::none)
                {
                    if (FindQualificationConversion(argType, paramType, arg, functionMatch, argumentMatch))
                    {
                        functionMatch.argumentMatches.push_back(argumentMatch);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    if (FindQualificationConversion(conversionFun->GetConversionParamType(context), paramType, arg, functionMatch, argumentMatch))
                    {
                        functionMatch.argumentMatches.push_back(argumentMatch);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

bool FindClassTemplateMatch(TypeSymbol* argType, TypeSymbol* paramType, BoundExpressionNode* arg, FunctionMatch& functionMatch,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!paramType->GetBaseType(context)->IsClassTypeSymbol()) return false;
    ClassTypeSymbol* paramClassType = static_cast<ClassTypeSymbol*>(paramType->GetBaseType(context));
    if (!paramClassType->IsTemplate(context)) return false;
    TemplateDeclarationSymbol* paramTemplateDeclaration = paramClassType->ParentTemplateDeclaration(context);
    Cardinality n = Cardinality(0);
    Cardinality arity = Cardinality(paramTemplateDeclaration->Arity());
    Cardinality numArgumentMatches = Cardinality(functionMatch.argumentMatches.size());
    if (argType->GetBaseType(context)->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* sourceClassTemplateSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(argType->GetBaseType(context));
        Cardinality m = Cardinality(sourceClassTemplateSpecialization->TemplateArguments(context).size());
        n = std::min(arity, m);
        for (Index i = Index(0); i < Index(n); ++i)
        {
            TypeSymbol* sourceArgumentType = nullptr;
            Symbol* sourceArgumentSymbol = sourceClassTemplateSpecialization->TemplateArguments(context)[ToUnderlying(i)];
            if (sourceArgumentSymbol->IsTypeSymbol())
            {
                sourceArgumentType = static_cast<TypeSymbol*>(sourceArgumentSymbol);
            }
            else
            {
                return false;
            }
            Symbol* targetArgumentSymbol = paramTemplateDeclaration->TemplateParameters(context)[ToUnderlying(i)];
            TypeSymbol* targetArgumentType = nullptr;
            if (targetArgumentSymbol->IsTypeSymbol())
            {
                targetArgumentType = static_cast<TypeSymbol*>(targetArgumentSymbol);
            }
            else
            {
                return false;
            }
            if (FindTemplateParameterMatch(sourceArgumentType, targetArgumentType, arg, functionMatch, fullSpan, context))
            {
                continue;
            }
            else if (FindClassTemplateMatch(sourceArgumentType, targetArgumentType, arg, functionMatch, fullSpan, context))
            {
                continue;
            }
            else if (FindClassTemplateSpecializationMatch(sourceArgumentType, targetArgumentType, arg, functionMatch, fullSpan, context))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
    }
    if (n == Cardinality(0))
    {
        return false;
    }
    functionMatch.argumentMatches.resize(ToUnderlying(numArgumentMatches));
    std::vector<Symbol*> targetTemplateArguments;
    for (Index i = Index(0); i < Index(n); ++i)
    {
        Symbol* targetArgumentSymbol = paramTemplateDeclaration->TemplateParameters(context)[ToUnderlying(i)];
        TypeSymbol* targetArgumentType = nullptr;
        if (targetArgumentSymbol->IsTypeSymbol())
        {
            targetArgumentType = static_cast<TypeSymbol*>(targetArgumentSymbol);
        }
        else
        {
            return false;
        }
        TypeSymbol* templateArgumentType = targetArgumentType->UnifyTemplateArgumentType(functionMatch.templateParameterMap, fullSpan, context);
        if (templateArgumentType)
        {
            targetTemplateArguments.push_back(templateArgumentType);
        }
        else
        {
            return false;
        }
    }
    TypeSymbol* plainTargetType = context->GetSymbolTable()->MakeClassTemplateSpecialization(paramClassType, targetTemplateArguments, fullSpan, context);
    paramType = context->GetSymbolTable()->MakeCompoundType(plainTargetType, paramType->GetDerivations(), context);
    if (TypesEqual(argType, paramType, context))
    {
        ArgumentMatch argumentMatch;
        if (argType->IsClassTypeSymbol() && paramType->IsClassTypeSymbol())
        {
            argumentMatch.postConversionFlags = argumentMatch.postConversionFlags | OperationFlags::addr;
        }
        functionMatch.argumentMatches.push_back(argumentMatch);
        return true;
    }
    else
    {
        bool qualificationConversionMatch = false;
        ArgumentMatch argumentMatch;
        if (TypesEqual(argType->PlainType(context), paramType->PlainType(context), context))
        {
            qualificationConversionMatch = FindQualificationConversion(argType, paramType, arg, functionMatch, argumentMatch);
        }
        if (qualificationConversionMatch)
        {
            functionMatch.argumentMatches.push_back(argumentMatch);
            return true;
        }
        else
        {
            FunctionSymbol* conversionFun = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                paramType, argType, arg, fullSpan, argumentMatch, functionMatch, context);
            if (conversionFun)
            {
                ++functionMatch.numConversions;
                argumentMatch.conversionFun = conversionFun;
                if (argumentMatch.preConversionFlags == OperationFlags::none)
                {
                    if (FindQualificationConversion(argType, paramType, arg, functionMatch, argumentMatch))
                    {
                        functionMatch.argumentMatches.push_back(argumentMatch);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    if (FindQualificationConversion(conversionFun->GetConversionParamType(context), paramType, arg, functionMatch, argumentMatch))
                    {
                        functionMatch.argumentMatches.push_back(argumentMatch);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

bool PlainTemplateArgsEqual(const std::vector<Symbol*>& sourceTemplateArguments, const std::vector<Symbol*>& targetTemplateArguments, Context* context)
{
    int n = sourceTemplateArguments.size();
    int m = targetTemplateArguments.size();
    if (n != m) return false;
    for (int i = 0; i < n; ++i)
    {
        Symbol* sourceTemplateArg = sourceTemplateArguments[i];
        Symbol* targetTemplateArg = targetTemplateArguments[i];
        if (!sourceTemplateArg->IsTypeSymbol() || !targetTemplateArg->IsTypeSymbol()) return false;
        TypeSymbol* sourceType = static_cast<TypeSymbol*>(sourceTemplateArg);
        TypeSymbol* targetType = static_cast<TypeSymbol*>(targetTemplateArg);
        if (!TypesEqual(sourceType->PlainType(context), targetType->PlainType(context), context))
        {
            return false;
        }
    }
    return true;
}

bool FindClassTemplateSpecializationMatch(TypeSymbol* argType, TypeSymbol* paramType, BoundExpressionNode* arg, FunctionMatch& functionMatch,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (!paramType->GetBaseType(context)->IsClassTypeSymbol()) return false;
    ClassTypeSymbol* paramClassType = static_cast<ClassTypeSymbol*>(paramType->GetBaseType(context));
    ClassTemplateSpecializationSymbol* paramSpecializationType = nullptr;
    if (paramClassType->IsClassTemplateSpecializationSymbol())
    {
        paramSpecializationType = static_cast<ClassTemplateSpecializationSymbol*>(paramClassType);
    }
    else
    {
        return false;
    }
    Cardinality n = Cardinality(paramSpecializationType->TemplateArguments(context).size());
    Cardinality numArgumentMatches = Cardinality(functionMatch.argumentMatches.size());
    std::vector<Symbol*> sourceTemplateArguments;
    if (argType->GetBaseType(context)->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* sourceClassTemplateSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(argType->GetBaseType(context));
        Cardinality m = Cardinality(sourceClassTemplateSpecialization->TemplateArguments(context).size());
        if (n != m) return false;
        sourceTemplateArguments = sourceClassTemplateSpecialization->TemplateArguments(context);
        for (Index i = Index(0); i < Index(n); ++i)
        {
            Symbol* sourceArgumentSymbol = sourceClassTemplateSpecialization->TemplateArguments(context)[ToUnderlying(i)];
            TypeSymbol* sourceArgumentType = nullptr;
            if (sourceArgumentSymbol->IsTypeSymbol())
            {
                sourceArgumentType = static_cast<TypeSymbol*>(sourceArgumentSymbol);
            }
            else
            {
                return false;
            }
            Symbol* targetArgumentSymbol = paramSpecializationType->TemplateArguments(context)[ToUnderlying(i)];
            TypeSymbol* targetArgumentType = nullptr;
            if (targetArgumentSymbol->IsTypeSymbol())
            {
                targetArgumentType = static_cast<TypeSymbol*>(targetArgumentSymbol);
            }
            else
            {
                return false;
            }
            if (FindTemplateParameterMatch(sourceArgumentType, targetArgumentType, arg, functionMatch, fullSpan, context))
            {
                continue;
            }
            else if (FindClassTemplateMatch(sourceArgumentType, targetArgumentType, arg, functionMatch, fullSpan, context))
            {
                continue;
            }
            else if (FindClassTemplateSpecializationMatch(sourceArgumentType, targetArgumentType, arg, functionMatch, fullSpan, context))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
        functionMatch.argumentMatches.resize(ToUnderlying(numArgumentMatches));
        std::vector<Symbol*> targetTemplateArguments;
        for (Index i = Index(0); i < Index(n); ++i)
        {
            Symbol* templateArgumentSymbol = paramSpecializationType->TemplateArguments(context)[ToUnderlying(i)];
            TypeSymbol* templateArgumentType = nullptr;
            if (templateArgumentSymbol->IsTypeSymbol())
            {
                templateArgumentType = static_cast<TypeSymbol*>(templateArgumentSymbol);
            }
            templateArgumentType = templateArgumentType->UnifyTemplateArgumentType(functionMatch.templateParameterMap, fullSpan, context);
            if (templateArgumentType)
            {
                targetTemplateArguments.push_back(templateArgumentType);
            }
            else
            {
                return false;
            }
        }
        TypeSymbol* plainTargetType = context->GetSymbolTable()->MakeClassTemplateSpecialization(
            paramSpecializationType->ClassTemplate(context), targetTemplateArguments, fullSpan, context);
        TypeSymbol* compoundParamType = context->GetSymbolTable()->MakeCompoundType(plainTargetType, paramType->GetDerivations(), context);
        if (TypesEqual(argType, compoundParamType, context))
        {
            ArgumentMatch argumentMatch;
            if (argType->IsClassTypeSymbol() && paramType->IsClassTypeSymbol())
            {
                argumentMatch.postConversionFlags = argumentMatch.postConversionFlags | OperationFlags::addr;
            }
            functionMatch.argumentMatches.push_back(argumentMatch);
            return true;
        }
        else
        {
            ArgumentMatch argumentMatch;
            bool qualificationConversionMatch = false;
            if (TypesEqual(argType->PlainType(context), compoundParamType->PlainType(context), context))
            {
                qualificationConversionMatch = FindQualificationConversion(argType, compoundParamType, arg, functionMatch, argumentMatch);
            }
            if (qualificationConversionMatch)
            {
                functionMatch.argumentMatches.push_back(argumentMatch);
                return true;
            }
            else
            {
                FunctionSymbol* conversionFun = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                    compoundParamType, argType, arg, fullSpan, argumentMatch, functionMatch, context);
                if (conversionFun)
                {
                    ++functionMatch.numConversions;
                    argumentMatch.conversionFun = conversionFun;
                    if (argumentMatch.preConversionFlags == OperationFlags::none)
                    {
                        if (FindQualificationConversion(argType, compoundParamType, arg, functionMatch, argumentMatch))
                        {
                            functionMatch.argumentMatches.push_back(argumentMatch);
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (FindQualificationConversion(conversionFun->GetConversionParamType(context), compoundParamType, arg, functionMatch, argumentMatch))
                        {
                            functionMatch.argumentMatches.push_back(argumentMatch);
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    bool plainTemplateArgsEqual = PlainTemplateArgsEqual(sourceTemplateArguments, targetTemplateArguments, context);
                    if (plainTemplateArgsEqual)
                    {
                        ++functionMatch.numConversions;
                        plainTargetType = context->GetSymbolTable()->MakeClassTemplateSpecialization(paramSpecializationType->ClassTemplate(context),
                            sourceTemplateArguments, fullSpan, context);
                        compoundParamType = context->GetSymbolTable()->MakeCompoundType(plainTargetType, paramType->GetDerivations(), context);
                        if (TypesEqual(argType, compoundParamType, context))
                        {
                            functionMatch.argumentMatches.push_back(ArgumentMatch());
                            return true;
                        }
                        else
                        {
                            ArgumentMatch argumentMatch;
                            if (TypesEqual(argType->PlainType(context), compoundParamType->PlainType(context), context))
                            {
                                qualificationConversionMatch = FindQualificationConversion(argType, compoundParamType, arg, functionMatch, argumentMatch);
                            }
                            if (qualificationConversionMatch)
                            {
                                functionMatch.argumentMatches.push_back(argumentMatch);
                                return true;
                            }
                            else
                            {
                                FunctionSymbol* conversionFun = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                                    compoundParamType, argType, arg, fullSpan, argumentMatch, functionMatch, context);
                                if (conversionFun)
                                {
                                    ++functionMatch.numConversions;
                                    argumentMatch.conversionFun = conversionFun;
                                    if (argumentMatch.preConversionFlags == OperationFlags::none)
                                    {
                                        if (FindQualificationConversion(argType, compoundParamType, arg, functionMatch, argumentMatch))
                                        {
                                            functionMatch.argumentMatches.push_back(argumentMatch);
                                            return true;
                                        }
                                        else
                                        {
                                            return false;
                                        }
                                    }
                                    else
                                    {
                                        if (FindQualificationConversion(conversionFun->GetConversionParamType(context), compoundParamType, arg,
                                            functionMatch, argumentMatch))
                                        {
                                            functionMatch.argumentMatches.push_back(argumentMatch);
                                            return true;
                                        }
                                        else
                                        {
                                            return false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

bool FindConversions(FunctionMatch& functionMatch, const std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context)
{
    Cardinality arity = Cardinality(args.size());
    Cardinality n = functionMatch.function->MemFnArity(context);
    for (Index i = Index(0); i < Index(n); ++i)
    {
        context->SetArgIndex(ToUnderlying(i));
        BoundExpressionNode* arg = nullptr;
        TypeSymbol* argType = nullptr;
        std::unique_ptr<BoundExpressionNode> defaultArg;
        if (i >= Index(arity))
        {
            ParameterSymbol* parameter = functionMatch.function->MemFnParameters(context)[ToUnderlying(i)];
            if (parameter->DefaultValue())
            {
                context->GetSymbolTable()->CurrentScope()->PushParentScope(functionMatch.function->GetScope());
                defaultArg = BindExpression(parameter->DefaultValue(), context);
                context->GetSymbolTable()->CurrentScope()->PopParentScope();
                arg = defaultArg.get();
                argType = arg->GetType()->DirectType(context)->FinalType(fullSpan, context);
                functionMatch.defaultArgs.push_back(std::move(defaultArg));
            }
            else
            {
                return false;
            }
        }
        else
        {
            arg = args[ToUnderlying(i)].get();
            argType = arg->GetType();
        }
        ParameterSymbol* parameter = functionMatch.function->MemFnParameters(context)[ToUnderlying(i)];
        context->PushSetFlag(ContextFlags::resolveDependentTypes | ContextFlags::resolveNestedTypes);
        context->PushTemplateParameterMap(&functionMatch.templateParameterMap);
        TypeSymbol* paramType = parameter->GetReferredType(context)->FinalType(fullSpan, context);
        context->PopTemplateParameterMap();
        context->PopFlags();
        if (TypesEqual(argType, paramType, context))
        {
            ArgumentMatch argumentMatch;
            if (argType->IsClassTypeSymbol() && paramType->IsClassTypeSymbol())
            {
                argumentMatch.postConversionFlags = argumentMatch.postConversionFlags | OperationFlags::addr;
            }
            functionMatch.argumentMatches.push_back(argumentMatch);
        }
        else
        {
            bool qualificationConversionMatch = false;
            ArgumentMatch argumentMatch;
            argumentMatch.integerRank = paramType->PlainType(context)->Rank(context);
            if (TypesEqual(argType->PlainType(context), paramType->PlainType(context), context))
            {
                qualificationConversionMatch = FindQualificationConversion(argType, paramType, arg, functionMatch, argumentMatch);
                if (qualificationConversionMatch)
                {
                    functionMatch.argumentMatches.push_back(argumentMatch);
                }
            }
            if (!qualificationConversionMatch)
            {
                FunctionSymbol* conversionFun = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                    paramType, argType, arg, fullSpan, argumentMatch, functionMatch, context);
                if (conversionFun)
                {
                    argumentMatch.conversionFun = conversionFun;
                    ++functionMatch.numConversions;
                    if (argumentMatch.preConversionFlags == OperationFlags::none)
                    {
                        if (FindQualificationConversion(argType, paramType, arg, functionMatch, argumentMatch))
                        {
                            functionMatch.argumentMatches.push_back(argumentMatch);
                            continue;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        TypeSymbol* conversionParamType = conversionFun->GetConversionParamType(context);
                        if (FindQualificationConversion(conversionParamType, paramType, arg, functionMatch, argumentMatch))
                        {
                            functionMatch.argumentMatches.push_back(argumentMatch);
                            continue;
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    if (functionMatch.function->IsTemplate(context) || functionMatch.function->IsMemFnOfClassTemplate(context))
                    {
                        if (FindTemplateParameterMatch(argType, paramType, arg, functionMatch, fullSpan, context))
                        {
                            continue;
                        }
                        if (FindClassTemplateMatch(argType, paramType, arg, functionMatch, fullSpan, context))
                        {
                            continue;
                        }
                        if (FindClassTemplateSpecializationMatch(argType, paramType, arg, functionMatch, fullSpan, context))
                        {
                            continue;
                        }
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void SetTemplateArgs(FunctionSymbol* viableFunction, std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>& 
    templateParameterMap, const std::vector<TypeSymbol*>& templateArgs, Context* context)
{
    if (viableFunction->IsTemplate(context))
    {
        TemplateDeclarationSymbol* templateDeclaration = viableFunction->ParentTemplateDeclaration(context);
        if (templateDeclaration)
        {
            Cardinality n = Cardinality(templateArgs.size());
            if (templateDeclaration->Arity() >= n)
            {
                for (Index i = Index(0); i < Index(n); ++i)
                {
                    TypeSymbol* templateArg = templateArgs[ToUnderlying(i)];
                    TemplateParameterSymbol* templateParam = templateDeclaration->TemplateParameters(context)[ToUnderlying(i)];
                    templateParameterMap[templateParam] = templateArg;
                }
            }
        }
    }
}

std::unique_ptr<FunctionMatch> SelectBestMatchingFunction(const std::vector<FunctionSymbol*>& viableFunctions, const std::vector<TypeSymbol*>& templateArgs,
    const std::vector<std::unique_ptr<BoundExpressionNode>>& args, const std::string& groupName, const soul::ast::FullSpan& fullSpan, Context* context, Exception& ex)
{
    std::vector<std::unique_ptr<FunctionMatch>> functionMatches;
    std::set<FunctionSymbol*> viableFunctionSet;
    std::set<std::string> viableFunctionFullNameSet;
    std::set<std::string> viableFunctionDeclarationFullNameSet;
    Cardinality n = Cardinality(viableFunctions.size());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        FunctionSymbol* viableFunction = viableFunctions[ToUnderlying(i)];
        if (viableFunctionSet.find(viableFunction) != viableFunctionSet.end())
        {
            continue;
        }
        std::string viableFunctionFullName = viableFunction->FullName(context);
        if (viableFunction->IsFunctionDefinitionSymbol() &&
            viableFunctionFullNameSet.find(viableFunctionFullName) != viableFunctionFullNameSet.end())
        {
            continue;
        }
        else if (!viableFunction->IsFunctionDefinitionSymbol() &&
            viableFunctionDeclarationFullNameSet.find(viableFunctionFullName) != viableFunctionDeclarationFullNameSet.end())
        {
            continue;
        }
        viableFunctionSet.insert(viableFunction);
        if (viableFunction->IsFunctionDefinitionSymbol())
        {
            viableFunctionFullNameSet.insert(viableFunctionFullName);
        }
        else
        {
            viableFunctionDeclarationFullNameSet.insert(viableFunctionFullName);
        }
        std::unique_ptr<FunctionMatch> functionMatch(new FunctionMatch(viableFunction, context));
        SetTemplateArgs(viableFunction, functionMatch->templateParameterMap, templateArgs, context);
        if (FindConversions(*functionMatch, args, fullSpan, context))
        {
            functionMatches.push_back(std::move(functionMatch));
        }
    }
    if (functionMatches.size() == 1)
    {
        return std::move(functionMatches.front());
    }
    else if (functionMatches.size() > 1)
    {
        std::sort(functionMatches.begin(), functionMatches.end(), BetterFunctionMatch(context));
        if (BetterFunctionMatch(context)(functionMatches[0], functionMatches[1]))
        {
            return std::move(functionMatches.front());
        }
        else
        {
            std::string message = "ambiguous function call, " + std::to_string(viableFunctions.size()) + " viable functions examined:";
            bool first = true;
            for (const auto& functionMatch : functionMatches)
            {
                if (BetterFunctionMatch(context)(functionMatches[0], functionMatch)) break;
                if (first)
                {
                    first = false;
                }
                else
                {
                    message.append(" or \n");
                }
                message.append(functionMatch->function->FullName(context));
            }
            ex = Exception(message);
            return std::unique_ptr<FunctionMatch>(nullptr);
        }
    }
    else
    {
        ex = Exception("overload resolution failed: overload in function group '" + groupName + "' not found, " +
            "or there are no acceptable conversions for all argument types. " + std::to_string(viableFunctions.size()) + " viable functions examined.", fullSpan, context);
        return std::unique_ptr<FunctionMatch>(nullptr);
    }
}

std::vector<Scope*> GetArgumentScopes(BoundExpressionNode* arg, Context* context)
{
    std::vector<Scope*> scopes;
    TypeSymbol* type = arg->GetType();
    if (type)
    {
        Scope* first = type->GetBaseType(context)->GetScope();
        scopes.push_back(first);
        Scope* second = first->GetNamespaceScope(context);
        if (second)
        {
            scopes.push_back(second);
        }
        if (type->GetBaseType(context)->IsClassTemplateSpecializationSymbol())
        {
            ClassTemplateSpecializationSymbol* sp = static_cast<ClassTemplateSpecializationSymbol*>(type->GetBaseType(context));
            ClassTypeSymbol* classTemplate = sp->ClassTemplate(context);
            Scope* third = classTemplate->GetScope()->GetNamespaceScope(context);
            scopes.push_back(third);
            Module* classTemplateModule = classTemplate->GetModule();
            std::vector<Module*> importedModules = classTemplateModule->ImportedModules(context);
            for (Module* importedModule : importedModules)
            {
                std::vector<NamespaceSymbol*> namespaces;
                const std::vector<SymbolId>& namespaceIds = importedModule->NamespaceIds();
                for (SymbolId nsId : namespaceIds)
                {
                    NamespaceSymbol* ns = importedModule->GetSymbolTable()->GetNamespaceSymbol(nsId, context);
                    if (!ns)
                    {
                        ThrowException("namespace symbol " + std::to_string(ToUnderlying(nsId)) + " not found from module '" + importedModule->Name() + "'");
                    }
                    namespaces.push_back(ns);
                }
                for (NamespaceSymbol* ns : namespaces)
                {
                    Scope* fourth = ns->GetScope();
                    scopes.push_back(fourth);
                }
            }
        }
    }
    return scopes;
}

void AddArgumentScopes(std::vector<std::pair<Scope*, ScopeLookup>>& scopeLookups, const std::vector<std::unique_ptr<BoundExpressionNode>>& args, Context* context)
{
    for (const auto& arg : args)
    {
        std::vector<Scope*> scopes = GetArgumentScopes(arg.get(), context);
        for (auto scope : scopes)
        {
            std::pair<Scope*, ScopeLookup> scopeLookup = std::make_pair(scope, ScopeLookup::thisAndBaseScopes);
            if (std::find(scopeLookups.begin(), scopeLookups.end(), scopeLookup) == scopeLookups.end())
            {
                scopeLookups.push_back(scopeLookup);
            }
        }
    }
}

void MakeFinalDirectArgs(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context)
{
    for (auto& arg : args)
    {
        arg->ModifyTypes(fullSpan, context);
    }
}

std::unique_ptr<BoundFunctionCallNode> ResolveOverload(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, Exception& ex,
    FunctionMatch& functionMatch, OverloadResolutionFlags flags)
{
    MakeFinalDirectArgs(args, fullSpan, context);
    std::vector<FunctionSymbol*> viableFunctions;
    if (groupName == "@destructor")
    {
        if (args.size() == 1)
        {
            BoundExpressionNode* arg = args[0].get();
            if (arg->GetType()->GetBaseType(context)->IsClassTemplateSpecializationSymbol())
            {
                ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(arg->GetType()->GetBaseType(context));
                Symbol* destructor = GenerateDestructor(specialization, fullSpan, context);
                viableFunctions.push_back(static_cast<FunctionSymbol*>(destructor));
            }
        }
    }
    context->PushSetFlag(ContextFlags::ignoreClassTemplateSpecializations);
    FunctionSymbol* operation = context->GetOperationRepository()->GetOperation(groupName, args, fullSpan, context);
    context->PopFlags();
    if (operation)
    {
        viableFunctions.push_back(operation);
    }
    else
    {
        std::vector<std::pair<Scope*, ScopeLookup>> scopeLookups;
        if ((flags & OverloadResolutionFlags::dontSearchArgumentScopes) != OverloadResolutionFlags::none)
        {
            scopeLookups.push_back(std::make_pair(scope, ScopeLookup::thisAndBaseScopes));
        }
        else
        {
            scopeLookups.push_back(std::make_pair(scope, ScopeLookup::allScopes));
            AddArgumentScopes(scopeLookups, args, context);
        }
        for (Module* importedModule : context->GetModule()->ImportedModules(context))
        {
            std::vector<NamespaceSymbol*> namespaces;
            const std::vector<SymbolId>& namespaceIds = importedModule->NamespaceIds();
            for (SymbolId nsId : namespaceIds)
            {
                NamespaceSymbol* ns = importedModule->GetSymbolTable()->GetNamespaceSymbol(nsId, context);
                if (!ns)
                {
                    ThrowException("namespace symbol " + std::to_string(ToUnderlying(nsId)) + " not found from module '" + importedModule->Name() + "'");
                }
                namespaces.push_back(ns);
            }
            for (NamespaceSymbol* ns : namespaces)
            {
                Scope* scope = ns->GetScope();
                scopeLookups.push_back(std::make_pair(scope, ScopeLookup::allScopes));
            }
        }
        bool flagsPushed = false;
        if ((flags & OverloadResolutionFlags::noMemberFunctions) != OverloadResolutionFlags::none)
        {
            context->PushSetFlag(ContextFlags::skipNonstaticMemberFunctions);
            flagsPushed = true;
        }
        context->GetSymbolTable()->CollectViableFunctions(scopeLookups, groupName, templateArgs, Cardinality(args.size()), viableFunctions, context);
        if (flagsPushed)
        {
            context->PopFlags();
        }
    }
    std::unique_ptr<FunctionMatch> bestMatch = SelectBestMatchingFunction(viableFunctions, templateArgs, args, groupName, fullSpan, context, ex);
    if (!bestMatch)
    {
        context->ResetFlag(ContextFlags::ignoreClassTemplateSpecializations);
        FunctionSymbol* operation = context->GetOperationRepository()->GetOperation(groupName, args, fullSpan, context);
        if (operation)
        {
            viableFunctions.clear();
            viableFunctions.push_back(operation);
            bestMatch = SelectBestMatchingFunction(viableFunctions, templateArgs, args, groupName, fullSpan, context, ex);
        }
        if (!bestMatch)
        {
            return std::unique_ptr<BoundFunctionCallNode>();
        }
    }
    if ((bestMatch->function->Qualifiers() & FunctionQualifiers::isDeleted) != FunctionQualifiers::none)
    {
        ex = Exception("attempt to call a deleted function", fullSpan, context);
        return std::unique_ptr<BoundFunctionCallNode>();
    }
    context->ResetFlag(ContextFlags::noPtrOps);
    context->ResetFlag(ContextFlags::skipFirstPtrToBooleanConversion);
    functionMatch = *bestMatch;
    bool instantiate = (flags & OverloadResolutionFlags::dontInstantiate) == OverloadResolutionFlags::none;
    if (instantiate)
    {
        context->PushResetFlag(ContextFlags::makeChildFn | ContextFlags::invoke | ContextFlags::tryCatch);
        ParseInlineMemberFunction(context, bestMatch->function);
        if (bestMatch->function->IsTemplate(context))
        {
            bestMatch->function = InstantiateFunctionTemplate(bestMatch->function, bestMatch->templateParameterMap, fullSpan, context);
        }
        else if (bestMatch->function->IsMemFnOfClassTemplate(context))
        {
            ClassTemplateSpecializationSymbol* classTemplateSpecialization = nullptr;
            if (bestMatch->specialization)
            {
                classTemplateSpecialization = bestMatch->specialization;
            }
            else
            {
                if (!bestMatch->function->IsStatic())
                {
                    TypeSymbol* arg0Type = args[0]->GetType()->GetBaseType(context);
                    if (arg0Type->IsClassTemplateSpecializationSymbol())
                    {
                        classTemplateSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(arg0Type);
                    }
                }
            }
            bestMatch->function = InstantiateMemFnOfClassTemplate(bestMatch->function, classTemplateSpecialization, bestMatch->templateParameterMap, fullSpan, context);
        }
        else if (bestMatch->function->IsInline() && context->ReleaseConfig() && otava::optimizer::HasOptimization(otava::optimizer::Optimizations::inlining))
        {
            bestMatch->function = InstantiateInlineFunction(bestMatch->function, fullSpan, context);
        }
        context->PopFlags();
    }
    std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = CreateBoundFunctionCall(*bestMatch, args, fullSpan, ex, context);
    return boundFunctionCall;
}

std::unique_ptr<BoundFunctionCallNode> ResolveOverload(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, Exception& ex, OverloadResolutionFlags flags)
{
    FunctionMatch functionMatch;
    return ResolveOverload(scope, groupName, templateArgs, args, fullSpan, context, ex, functionMatch, flags);
}

std::unique_ptr<BoundFunctionCallNode> ResolveOverload(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, Exception& ex, FunctionMatch& functionMatch)
{
    return ResolveOverload(scope, groupName, templateArgs, args, fullSpan, context, ex, functionMatch, OverloadResolutionFlags::none);
}

std::unique_ptr<BoundFunctionCallNode> ResolveOverload(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, Exception& ex)
{
    FunctionMatch functionMatch;
    return ResolveOverload(scope, groupName, templateArgs, args, fullSpan, context, ex, functionMatch, OverloadResolutionFlags::none);
}

std::unique_ptr<BoundFunctionCallNode> ResolveOverloadThrow(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context, OverloadResolutionFlags flags)
{
    Exception ex;
    std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverload(scope, groupName, templateArgs, args, fullSpan, context, ex, flags);
    if (!boundFunctionCall)
    {
        ThrowException(ex);
    }
    return boundFunctionCall;
}

std::unique_ptr<BoundFunctionCallNode> ResolveOverloadThrow(Scope* scope, const std::string& groupName, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, Context* context)
{
    return ResolveOverloadThrow(scope, groupName, templateArgs, args, fullSpan, context, OverloadResolutionFlags::none);
}

} // namespace otava::symbols

