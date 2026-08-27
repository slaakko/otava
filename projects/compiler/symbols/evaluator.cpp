// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.evaluator;

import otava.symbols.array_type_symbol;
import otava.symbols.bound_tree;
import otava.symbols.context;
import otava.symbols.evaluation_context;
import otava.symbols.exception;
import otava.symbols.expression_binder;
import otava.symbols.function_symbol;
import otava.symbols.function_group_symbol;
import otava.symbols.fundamental_type_kind;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.id;
import otava.symbols.modules;
import otava.symbols.overload_resolution;
import otava.symbols.scope;
import otava.symbols.scope_resolver;
import otava.symbols.scope_ptr;
import otava.symbols.symbol;
import otava.symbols.type_resolver;
import otava.symbols.concrete_value;
import otava.symbols.value;
import otava.intermediate.types;
import otava.ast.identifier;
import otava.ast.declaration;
import otava.ast.expression;
import otava.ast.node_list;
import otava.ast.simple_type;
import otava.ast.statement;
import otava.ast.visitor;

namespace otava::symbols {

template<typename T, typename Op>
Value* EvaluateIntegerUnaryOperator(FundamentalTypeValue<T>* arg, Op op, Context* context)
{
    if (!arg) return nullptr;
    ValueKind valueKind = arg->GetValueKind();
    Value* result = context->GetEvaluationContext()->GetIntegerValue(static_cast<std::uint64_t>(op(arg->GetValue())), GetValueType(valueKind, context), context);
    return result;
}

template<typename T, typename Op>
Value* EvaluateFloatingUnaryOperator(FundamentalTypeValue<T>* arg, Op op, Context* context)
{
    if (!arg) return nullptr;
    ValueKind valueKind = arg->GetValueKind();
    Value* result = context->GetEvaluationContext()->GetFloatingValue(static_cast<double>(op(arg->GetValue())), GetValueType(valueKind, context), context);
    return result;
}

template<typename T, typename Op>
Value* EvaluateIntegerBinaryOperator(ValueKind commonValueKind, FundamentalTypeValue<T>* left, FundamentalTypeValue<T>* right, Op op, Context* context)
{
    if (!left || !right) return nullptr;
    Value* result = context->GetEvaluationContext()->GetIntegerValue(static_cast<std::uint64_t>(op(left->GetValue(), 
        right->GetValue())), GetValueType(commonValueKind, context), context);
    return result;
}

template<typename T, typename Op>
Value* EvaluateFloatingBinaryOperator(ValueKind commonValueKind, FundamentalTypeValue<T>* left, FundamentalTypeValue<T>* right, Op op, Context* context)
{
    if (!left || !right) return nullptr;
    Value* result = context->GetEvaluationContext()->GetFloatingValue(static_cast<double>(op(left->GetValue(),
        right->GetValue())), GetValueType(commonValueKind, context), context);
    return result;
}

template<typename T, typename Op>
Value* EvaluateComparisonOperator(FundamentalTypeValue<T>* left, FundamentalTypeValue<T>* right, Op op, Context* context)
{
    if (!left || !right) return nullptr;
    Value* result = context->GetEvaluationContext()->GetBoolValue(op(left->GetValue(), right->GetValue()));
    return result;
}

Value* EvaluateUnaryMinus(Value* arg, Context* context)
{
    if (!arg) return nullptr;
    ValueKind valueKind = arg->GetValueKind();
    switch (valueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::uint8_t>*>(arg), std::negate<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::int8_t>*>(arg), std::negate<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::int16_t>*>(arg), std::negate<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::uint16_t>*>(arg), std::negate<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::int32_t>*>(arg), std::negate<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::uint32_t>*>(arg), std::negate<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::int64_t>*>(arg), std::negate<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::uint64_t>*>(arg), std::negate<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateFloatingUnaryOperator(static_cast<FundamentalTypeValue<float>*>(arg), std::negate<float>(), context);
    case ValueKind::doubleValue: return EvaluateFloatingUnaryOperator(static_cast<FundamentalTypeValue<double>*>(arg), std::negate<double>(), context);
    }
    return nullptr;
}

Value* EvaluateComplement(Value* arg, Context* context)
{
    if (!arg) return nullptr;
    ValueKind valueKind = arg->GetValueKind();
    switch (valueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::uint8_t>*>(arg), std::bit_not<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::int8_t>*>(arg), std::bit_not<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::int16_t>*>(arg), std::bit_not<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::uint16_t>*>(arg), std::bit_not<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::int32_t>*>(arg), std::bit_not<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::uint32_t>*>(arg), std::bit_not<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::int64_t>*>(arg), std::bit_not<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerUnaryOperator(static_cast<FundamentalTypeValue<std::uint64_t>*>(arg), std::bit_not<std::uint64_t>(), context);
    }
    return nullptr;
}

Value* EvaluateAdd(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind, 
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon), std::plus<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon), std::plus<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon), std::plus<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon), std::plus<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon), std::plus<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon), std::plus<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon), std::plus<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon), std::plus<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateFloatingBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon), std::plus<float>(), context);
    case ValueKind::doubleValue: return EvaluateFloatingBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon), std::plus<double>(), context);
    }
    return nullptr;
}

Value* EvaluateSub(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon), 
        std::minus<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon), 
        std::minus<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon), 
        std::minus<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon), 
        std::minus<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon), 
        std::minus<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon), 
        std::minus<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon), 
        std::minus<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon), 
        std::minus<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateFloatingBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon), 
        std::minus<float>(), context);
    case ValueKind::doubleValue: return EvaluateFloatingBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon), 
        std::minus<double>(), context);
    }
    return nullptr;
}

Value* EvaluateMul(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon), 
        std::multiplies<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon), 
        std::multiplies<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon), 
        std::multiplies<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon), 
        std::multiplies<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon), 
        std::multiplies<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon), 
        std::multiplies<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon), 
        std::multiplies<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon), 
        std::multiplies<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateFloatingBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon), 
        std::multiplies<float>(), context);
    case ValueKind::doubleValue: return EvaluateFloatingBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon), 
        std::multiplies<double>(), context);
    }
    return nullptr;
}

Value* EvaluateDiv(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::divides<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::divides<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::divides<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::divides<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::divides<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::divides<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::divides<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::divides<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateFloatingBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon),
        std::divides<float>(), context);
    case ValueKind::doubleValue: return EvaluateFloatingBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon),
        std::divides<double>(), context);
    }
    return nullptr;
}

template<typename T>
struct shift_left
{
    constexpr T operator()(const T& left, const T& right) const
    {
        return left << right;
    }
};

template<typename T>
struct shift_right
{
    constexpr T operator()(const T& left, const T& right) const
    {
        return left >> right;
    }
};

Value* EvaluateMod(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::modulus<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::modulus<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::modulus<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::modulus<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::modulus<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::modulus<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::modulus<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::modulus<std::uint64_t>(), context);
    }
    return nullptr;
}

Value* EvaluateShiftLeft(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        shift_left<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        shift_left<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        shift_left<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        shift_left<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        shift_left<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        shift_left<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        shift_left<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        shift_left<std::uint64_t>(), context);
    }
    return nullptr;
}

Value* EvaluateShiftRight(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        shift_right<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        shift_right<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        shift_right<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        shift_right<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        shift_right<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        shift_right<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        shift_right<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        shift_right<std::uint64_t>(), context);
    }
    return nullptr;
}

Value* EvaluateBitOr(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::bit_or<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::bit_or<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::bit_or<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::bit_or<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::bit_or<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::bit_or<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::bit_or<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::bit_or<std::uint64_t>(), context);
    }
    return nullptr;
}

Value* EvaluateBitXor(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::bit_xor<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::bit_xor<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::bit_xor<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::bit_xor<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::bit_xor<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::bit_xor<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::bit_xor<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::bit_xor<std::uint64_t>(), context);
    }
    return nullptr;
}

Value* EvaluateBitAnd(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::bit_and<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::bit_and<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::bit_and<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::bit_and<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::bit_and<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::bit_and<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::bit_and<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateIntegerBinaryOperator(commonValueKind,
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::bit_and<std::uint64_t>(), context);
    }
    return nullptr;
}

Value* EvaluateEqual(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::boolValue:
    {
        BoolValue* leftBool = static_cast<BoolValue*>(leftCommon);
        BoolValue* rightBool = static_cast<BoolValue*>(rightCommon);
        return context->GetEvaluationContext()->GetBoolValue(leftBool->GetValue() == rightBool->GetValue());
    }
    case ValueKind::byteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon), 
        std::equal_to<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon), 
        std::equal_to<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon), 
        std::equal_to<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon), 
        std::equal_to<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon), 
        std::equal_to<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon), 
        std::equal_to<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon), 
        std::equal_to<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon), 
        std::equal_to<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon), 
        std::equal_to<float>(), context);
    case ValueKind::doubleValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon), 
        std::equal_to<double>(), context);
    }
    return nullptr;
}

Value* EvaluateNotEqual(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::boolValue:
    {
        BoolValue* leftBool = static_cast<BoolValue*>(leftCommon);
        BoolValue* rightBool = static_cast<BoolValue*>(rightCommon);
        return context->GetEvaluationContext()->GetBoolValue(leftBool->GetValue() != rightBool->GetValue());
    }
    case ValueKind::byteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::not_equal_to<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::not_equal_to<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::not_equal_to<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::not_equal_to<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::not_equal_to<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::not_equal_to<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::not_equal_to<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::not_equal_to<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon),
        std::not_equal_to<float>(), context);
    case ValueKind::doubleValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon),
        std::not_equal_to<double>(), context);
    }
    return nullptr;
}

Value* EvaluateLess(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::less<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::less<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::less<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::less<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::less<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::less<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::less<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::less<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon),
        std::less<float>(), context);
    case ValueKind::doubleValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon),
        std::less<double>(), context);
    }
    return nullptr;
}

Value* EvaluateGreater(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::greater<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::greater<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::greater<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::greater<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::greater<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::greater<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::greater<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::greater<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon),
        std::greater<float>(), context);
    case ValueKind::doubleValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon),
        std::greater<double>(), context);
    }
    return nullptr;
}

Value* EvaluateLessOrEqual(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::less_equal<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::less_equal<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::less_equal<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::less_equal<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::less_equal<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::less_equal<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::less_equal<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::less_equal<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon),
        std::less_equal<float>(), context);
    case ValueKind::doubleValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon),
        std::less_equal<double>(), context);
    }
    return nullptr;
}

Value* EvaluateGreaterOrEqual(Value* left, Value* right, Context* context)
{
    if (!left || !right) return nullptr;
    ValueKind commonValueKind = CommonValueKind(left->GetValueKind(), right->GetValueKind());
    Value* leftCommon = left->Convert(commonValueKind, context);
    Value* rightCommon = right->Convert(commonValueKind, context);
    switch (commonValueKind)
    {
    case ValueKind::byteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint8_t>*>(rightCommon),
        std::greater_equal<std::uint8_t>(), context);
    case ValueKind::sbyteValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int8_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int8_t>*>(rightCommon),
        std::greater_equal<std::int8_t>(), context);
    case ValueKind::shortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int16_t>*>(rightCommon),
        std::greater_equal<std::int16_t>(), context);
    case ValueKind::ushortValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint16_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint16_t>*>(rightCommon),
        std::greater_equal<std::uint16_t>(), context);
    case ValueKind::intValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int32_t>*>(rightCommon),
        std::greater_equal<std::int32_t>(), context);
    case ValueKind::uintValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint32_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint32_t>*>(rightCommon),
        std::greater_equal<std::uint32_t>(), context);
    case ValueKind::longValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::int64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::int64_t>*>(rightCommon),
        std::greater_equal<std::int64_t>(), context);
    case ValueKind::ulongValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<std::uint64_t>*>(leftCommon), static_cast<FundamentalTypeValue<std::uint64_t>*>(rightCommon),
        std::greater_equal<std::uint64_t>(), context);
    case ValueKind::floatValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<float>*>(leftCommon), static_cast<FundamentalTypeValue<float>*>(rightCommon),
        std::greater_equal<float>(), context);
    case ValueKind::doubleValue: return EvaluateComparisonOperator(
        static_cast<FundamentalTypeValue<double>*>(leftCommon), static_cast<FundamentalTypeValue<double>*>(rightCommon),
        std::greater_equal<double>(), context);
    }
    return nullptr;
}

otava::ast::FunctionDefinitionNode* GetFunctionDefinitionNode(FunctionSymbol* fn, Context* context)
{
    if (fn->IsFunctionDefinitionSymbol())
    {
        FunctionDefinitionSymbol* fnDef = static_cast<FunctionDefinitionSymbol*>(fn);
        Module* module = fn->GetModule();
        module->ReadAstNode();
        otava::ast::Node* node = module->GetAstNode(fnDef->AstNodeId());
        if (node && node->IsFunctionDefinitionNode())
        {
            return static_cast<otava::ast::FunctionDefinitionNode*>(node);
        }
        else
        {
            otava::ast::Node* node = context->GetSymbolTable()->GetNodeNothrow(fnDef, context);
            if (node && node->IsFunctionDefinitionNode())
            {
                return static_cast<otava::ast::FunctionDefinitionNode*>(node);
            }
        }
    }
    else
    {
        Module* module = fn->GetModule();
        module->ReadAstNode();
        otava::ast::Node* node = module->GetAstNode(fn->AstNodeId());
        if (node && node->IsFunctionDefinitionNode())
        {
            return static_cast<otava::ast::FunctionDefinitionNode*>(node);
        }
    }
    return nullptr;
}

std::vector<std::unique_ptr<BoundExpressionNode>> ValuesToLiterals(const std::vector<Value*>& values, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::vector<std::unique_ptr<BoundExpressionNode>> literals;
    for (Value* value : values)
    {
        BoundLiteralNode* literal = new BoundLiteralNode(value, fullSpan, value->GetType(context));
        if (value->GetInterfaceType(context)->IsEnumeratedTypeSymbol())
        {
            literal->SetInterfaceType(value->GetInterfaceType(context));
        }
        std::unique_ptr<BoundExpressionNode> lit(literal);
        literals.push_back(std::move(lit));
    }
    return literals;
}

std::vector<Value*> ExpressionsToValues(const std::vector<std::unique_ptr<BoundExpressionNode>>& exprs, const soul::ast::FullSpan& fullSpan, Context* context, 
    bool& failed)
{
    std::vector<Value*> values;
    for (const auto& expr : exprs)
    {
        Value* value = expr->ToValue(context);
        if (value)
        {
            if (value->GetInterfaceType(context)->IsEnumeratedTypeSymbol())
            {
                value->SetInterfaceType(expr->GetInterfaceType(context));
            }
            values.push_back(value);
        }
        else
        {
            failed = true;
            return std::vector<Value*>();
        }
    }
    failed = false;
    return values;
}

EvaluationMapSetter::EvaluationMapSetter(Context* context_) : context(context_) 
{ 
    context->GetEvaluationContext()->PushEvaluationMap(); 
}

EvaluationMapSetter::~EvaluationMapSetter() 
{ 
    context->GetEvaluationContext()->PopEvaluationMap(); 
}

void PopulateEvaluationMap(FunctionSymbol* fn, const std::vector<Value*>& args, Context* context)
{
    EvaluationMap* evaluationMap = context->GetEvaluationContext()->CurrentEvaluationMap();
    Cardinality n = fn->Arity();
    if (n != Cardinality(args.size()))
    {
        ThrowException("invalid argument count");
    }
    for (Index i = Index(0); i < ToIndex(n); ++i)
    {
        ParameterSymbol* parameter = fn->Parameters(context)[ToUnderlying(i)];
        evaluationMap->SetValue(parameter->Name(), args[ToUnderlying(i)]);
    }
}

TypeSymbol* GetIntegerType(otava::ast::Suffix suffix, const soul::ast::FullSpan& fullSpan, Context* context)
{
    DeclarationFlags flags = DeclarationFlags::intFlag;
    if ((suffix & otava::ast::Suffix::u) != otava::ast::Suffix::none)
    {
        flags = flags | DeclarationFlags::unsignedFlag;
    }
    if ((suffix & otava::ast::Suffix::l) != otava::ast::Suffix::none)
    {
        flags = flags | DeclarationFlags::longFlag;
    }
    if ((suffix & otava::ast::Suffix::ll) != otava::ast::Suffix::none)
    {
        flags = flags | DeclarationFlags::longLongFlag;
    }
    return GetFundamentalType(flags, fullSpan, context);
}

TypeSymbol* GetFloatingPointType(otava::ast::Suffix suffix, const soul::ast::FullSpan& fullSpan, Context* context)
{
    DeclarationFlags flags = DeclarationFlags::doubleFlag;
    if ((suffix & otava::ast::Suffix::l) != otava::ast::Suffix::none)
    {
        flags = flags | DeclarationFlags::longFlag;
    }
    if ((suffix & otava::ast::Suffix::f) != otava::ast::Suffix::none)
    {
        flags = (flags & ~DeclarationFlags::doubleFlag) | DeclarationFlags::floatFlag;
    }
    return GetFundamentalType(flags, fullSpan, context);
}

TypeSymbol* GetStringType(otava::ast::EncodingPrefix encodingPrefix, const soul::ast::FullSpan& fullSpan, Context* context)
{
    switch (encodingPrefix)
    {
    case otava::ast::EncodingPrefix::u8:
    {
        return context->GetSymbolTable()->MakeConstChar8PtrType(context);
    }
    case otava::ast::EncodingPrefix::u:
    {
        return context->GetSymbolTable()->MakeConstChar16PtrType(context);
    }
    case otava::ast::EncodingPrefix::U:
    {
        return context->GetSymbolTable()->MakeConstChar32PtrType(context);
    }
    case otava::ast::EncodingPrefix::L:
    {
        return context->GetSymbolTable()->MakeConstWCharPtrType(context);
    }
    default:
    {
        return context->GetSymbolTable()->MakeConstCharPtrType(context);
    }
    }
}

TypeSymbol* GetCharacterType(otava::ast::EncodingPrefix encodingPrefix, const soul::ast::FullSpan& fullSpan, Context* context)
{
    switch (encodingPrefix)
    {
    case otava::ast::EncodingPrefix::u8:
    {
        return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::char8Type, context);
    }
    case otava::ast::EncodingPrefix::u:
    {
        return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::char16Type, context);
    }
    case otava::ast::EncodingPrefix::U:
    {
        return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::char32Type, context);
    }
    case otava::ast::EncodingPrefix::L:
    {
        return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::wcharType, context);
    }
    default:
    {
        return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::charType, context);
    }
    }
}

class Evaluator : public otava::ast::DefaultVisitor
{
public:
    Evaluator(Context* context_);
    Value* GetValue() { if (error) return nullptr; else return value; }
    void Visit(otava::ast::CharNode& node) override;
    void Visit(otava::ast::Char8Node& node) override;
    void Visit(otava::ast::Char16Node& node) override;
    void Visit(otava::ast::Char32Node& node) override;
    void Visit(otava::ast::WCharNode& node) override;
    void Visit(otava::ast::BoolNode& node) override;
    void Visit(otava::ast::ShortNode& node) override;
    void Visit(otava::ast::IntNode& node) override;
    void Visit(otava::ast::LongNode& node) override;
    void Visit(otava::ast::SignedNode& node) override;
    void Visit(otava::ast::UnsignedNode& node) override;
    void Visit(otava::ast::FloatNode& node) override;
    void Visit(otava::ast::DoubleNode& node) override;
    void Visit(otava::ast::IntegerLiteralNode& node) override;
    void Visit(otava::ast::FloatingLiteralNode& node) override;
    void Visit(otava::ast::CharacterLiteralNode& node) override;
    void Visit(otava::ast::StringLiteralNode& node) override;
    void Visit(otava::ast::BooleanLiteralNode& node) override;
    void Visit(otava::ast::NullPtrLiteralNode& node) override;
    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::AssignmentInitNode& node) override;
    void Visit(otava::ast::UnaryExprNode& node) override;
    void Visit(otava::ast::BinaryExprNode& node) override;
    void Visit(otava::ast::PostfixIncExprNode& node) override;
    void Visit(otava::ast::PostfixDecExprNode& node) override;
    void Visit(otava::ast::InvokeExprNode& node) override;
    void Visit(otava::ast::SizeOfTypeExprNode& node) override;
    void Visit(otava::ast::BracedInitListNode& node) override;
    void Visit(otava::ast::CppCastExprNode& node) override;
    void Visit(otava::ast::FunctionDefinitionNode& node) override;
    void Visit(otava::ast::CompoundStatementNode& node) override;
    void Visit(otava::ast::DeclarationStatementNode& node) override;
    void Visit(otava::ast::SimpleDeclarationNode& node) override;
    void Visit(otava::ast::DeclSpecifierSequenceNode& node) override;
    void Visit(otava::ast::InitDeclaratorListNode& node) override;
    void Visit(otava::ast::InitDeclaratorNode& node) override;
    void Visit(otava::ast::ExpressionStatementNode& node) override;
    void Visit(otava::ast::ReturnStatementNode& node) override;
    void Visit(otava::ast::IfStatementNode& node) override;
    void Visit(otava::ast::SwitchStatementNode& node) override;
    void Visit(otava::ast::WhileStatementNode& node) override;
    void Visit(otava::ast::DoStatementNode& node) override;
    void Visit(otava::ast::RangeForStatementNode& node) override;
    void Visit(otava::ast::ForStatementNode& node) override;
    void Visit(otava::ast::BreakStatementNode& node) override;
    void Visit(otava::ast::ContinueStatementNode& node) override;
    void Visit(otava::ast::GotoStatementNode& node) override;
    void Visit(otava::ast::TryStatementNode& node) override;
private:
    Context* context;
    Value* value;
    Value* initializer;
    Scope* scope;
    bool returned;
    bool breakSet;
    bool continueSet;
    bool error;
    bool setValue;
    bool composeType;
    DeclarationFlags declarationFlags;
    void EvaluateUnaryOp(otava::ast::NodeKind op, otava::ast::Node* argument);
    void EvaluateBinOp(otava::ast::NodeKind op, otava::ast::Node* left, otava::ast::Node* right);
    void EvaluatePrefixInc(otava::ast::Node* argument);
    void EvaluatePrefixDec(otava::ast::Node* argument);
};

Evaluator::Evaluator(Context* context_) : 
    context(context_), value(nullptr), initializer(nullptr), 
    scope(context->GetSymbolTable()->CurrentScope()), returned(false), breakSet(false), continueSet(false),
    error(false), setValue(false), composeType(false), declarationFlags(DeclarationFlags::none)
{
    if (context->GetFlag(ContextFlags::setValue))
    {
        setValue = true;
        initializer = context->Initializer();
    }
}

void Evaluator::Visit(otava::ast::CharNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::charFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::charType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::Char8Node& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::char8Flag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::char8Type, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::Char16Node& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::char16Flag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::char16Type, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::Char32Node& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::char32Flag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::char32Type, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::WCharNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::wcharFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::wcharType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::BoolNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::boolFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::ShortNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::shortFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::shortIntType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::IntNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::intFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::LongNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::longFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longIntType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::SignedNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::signedFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::UnsignedNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::unsignedFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedIntType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::FloatNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::floatFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::floatType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::DoubleNode& node)
{
    if (composeType)
    {
        declarationFlags = declarationFlags | DeclarationFlags::doubleFlag;
    }
    else
    {
        TypeSymbol* type = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::doubleType, context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
    }
}

void Evaluator::Visit(otava::ast::IntegerLiteralNode& node)
{
    TypeSymbol* type = GetIntegerType(node.GetSuffix(), node.GetFullSpan(), context);
    value = context->GetEvaluationContext()->GetIntegerValue(node.GetValue(), type, context);
}

void Evaluator::Visit(otava::ast::FloatingLiteralNode& node)
{
    TypeSymbol* type = GetFloatingPointType(node.GetSuffix(), node.GetFullSpan(), context);
    value = context->GetEvaluationContext()->GetFloatingValue(node.GetValue(), type, context);
}

void Evaluator::Visit(otava::ast::CharacterLiteralNode& node)
{
    TypeSymbol* type = GetCharacterType(node.GetEncodingPrefix(), node.GetFullSpan(), context);
    value = context->GetEvaluationContext()->GetCharValue(node.GetValue(), type, context);
}

void Evaluator::Visit(otava::ast::StringLiteralNode& node)
{
    TypeSymbol* type = GetStringType(node.GetEncodingPrefix(), node.GetFullSpan(), context);
    value = context->GetEvaluationContext()->GetStringValue(node.GetValue(), type, context);
}

void Evaluator::Visit(otava::ast::BooleanLiteralNode& node)
{
    value = context->GetEvaluationContext()->GetBoolValue(node.GetValue());
}

void Evaluator::Visit(otava::ast::NullPtrLiteralNode& node)
{
    value = context->GetEvaluationContext()->GetNullPtrValue();
}

void Evaluator::Visit(otava::ast::QualifiedIdNode& node)
{
    Scopes scopes = GetScopes(node.Left(), context);
    for (Scope* scope : scopes.GetScopes())
    {
        ModulePtr modulePtr(scope->GetModule(), context);
        ScopePtr scopePtr(scope, context);
        Value* value = Evaluate(node.Right(), context);
        if (value)
        {
            this->value = value;
            return;
        }
    }
    error = true;
}

void Evaluator::Visit(otava::ast::IdentifierNode& node)
{
    if (setValue)
    {
        EvaluationMap* evalutionMap = context->GetEvaluationContext()->CurrentEvaluationMap();
        evalutionMap->SetValue(node.Str(), initializer);
        value = initializer;
        return;
    }
    EvaluationMap* evalutionMap = context->GetEvaluationContext()->CurrentEvaluationMap();
    while (evalutionMap)
    {
        value = evalutionMap->GetValue(node.Str());
        if (value)
        {
            return;
        }
        evalutionMap = evalutionMap->ParentMap();
    }
    scope = scope->SpecializationScope(context);
    Symbol* symbol = scope->Lookup(node.Str(),
        SymbolGroupKind::functionSymbolGroup | SymbolGroupKind::variableSymbolGroup | SymbolGroupKind::classSymbolGroup | 
        SymbolGroupKind::enumSymbolGroup | SymbolGroupKind::aliasSymbolGroup, ScopeLookup::allScopes, node.GetFullSpan(), context, LookupFlags::none);
    if (symbol)
    {
        if (symbol->IsTypeSymbol())
        {
            TypeSymbol* type = static_cast<TypeSymbol*>(symbol);
            value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), type, context);
        }
        else
        {
            switch (symbol->Kind())
            {
            case SymbolKind::variableSymbol:
            {
                VariableSymbol* variableSymbol = static_cast<VariableSymbol*>(symbol);
                if (variableSymbol->GetValue(context))
                {
                    value = variableSymbol->GetValue(context);
                }
                else
                {
                    error = true;
                    return;
                }
                break;
            }
            case SymbolKind::enumConstantSymbol:
            {
                EnumConstantSymbol* enumConstant = static_cast<EnumConstantSymbol*>(symbol);
                value = enumConstant->GetValue(context);
                break;
            }
            case SymbolKind::functionGroupSymbol:
            {
                FunctionGroupSymbol* functionGroup = static_cast<FunctionGroupSymbol*>(symbol);
                value = context->GetEvaluationContext()->GetFunctionGroupValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionGroupValueSymbol),
                    functionGroup, nullptr, context);
                break;
            }
            case SymbolKind::functionSymbol: 
            case SymbolKind::functionDefinitionSymbol:
            {
                FunctionSymbol* fn = static_cast<FunctionSymbol*>(symbol);
                FunctionGroupSymbol* fnGroup = fn->Group(context);
                value = context->GetEvaluationContext()->GetFunctionGroupValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionGroupValueSymbol),
                    fnGroup, fn, context);
                break;
            }
            case SymbolKind::aliasTypeSymbol:
            {
                AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(symbol);
                TypeSymbol* referredType = aliasType->ReferredType(context);
                while (referredType->IsAliasTypeSymbol())
                {
                    aliasType = static_cast<AliasTypeSymbol*>(referredType);
                    referredType = aliasType->ReferredType(context);
                }
                value = context->GetEvaluationContext()->GetTypeValue(
                    context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), referredType, context);
                break;
            }
            default:
            {
                error = true;
                return;
            }
            }
        }
    }
    else
    {
        error = true;
        return;
    }
}

void Evaluator::Visit(otava::ast::AssignmentInitNode& node)
{
    if (error) return;
    node.Child()->Accept(*this);
}

void Evaluator::EvaluateUnaryOp(otava::ast::NodeKind op, otava::ast::Node* argument)
{
    if (error) return;
    value = Evaluate(argument, context);
    if (value)
    {
        switch (op)
        {
        case otava::ast::NodeKind::plusNode:
        {
            break;
        }
        case otava::ast::NodeKind::minusNode:
        {
            value = otava::symbols::EvaluateUnaryMinus(value, context);
            break;
        }
        case otava::ast::NodeKind::complementNode:
        {
            value = otava::symbols::EvaluateComplement(value, context);
            break;
        }
        case otava::ast::NodeKind::notNode:
        {
            if (value->IsBoolValue())
            {
                BoolValue* boolValue = static_cast<BoolValue*>(value);
                value = context->GetEvaluationContext()->GetBoolValue(!boolValue->GetValue());
            }
            break;
        }
        case otava::ast::NodeKind::prefixIncNode:
        {
            EvaluatePrefixInc(argument);
            break;
        }
        case otava::ast::NodeKind::prefixDecNode:
        {
            EvaluatePrefixDec(argument);
            break;
        }
        default:
        {
            error = true;
            return;
        }
        }
    }
    else
    {
        error = true;
    }
}

void Evaluator::Visit(otava::ast::UnaryExprNode& node)
{
    if (error) return;
    otava::ast::NodeKind op = node.Op()->Kind();
    EvaluateUnaryOp(op, node.Child());
    if (value)
    {
        return;
    }
    FlagSetter dontThrowFlagSetter(context, ContextFlags::dontThrow);
    std::string groupName = OperatorGroupNameMap::Instance().GetGroupName(op, node.GetFullSpan(), context);
    if (groupName.empty())
    {
        error = true;
        return;
    }
    dontThrowFlagSetter.Reset();
    Exception exception;
    std::vector<TypeSymbol*> templateArguments;
    std::vector<Value*> argumentValues;
    Value* argumentValue = Evaluate(node.Child(), context);
    if (!argumentValue)
    {
        error = true;
        return;
    }
    argumentValues.push_back(argumentValue);
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments = ValuesToLiterals(argumentValues, node.GetFullSpan(), context);
    std::unique_ptr<BoundFunctionCallNode> fnCall = ResolveOverload(scope, groupName, templateArguments, arguments,
        node.GetFullSpan(), context, exception, OverloadResolutionFlags::dontInstantiate);
    if (fnCall)
    {
        FunctionSymbol* fn = fnCall->GetFunctionSymbol();
        if ((fn->GetDeclarationFlags() & DeclarationFlags::constExprFlag) != DeclarationFlags::none)
        {
            otava::ast::FunctionDefinitionNode* fnDefNode = GetFunctionDefinitionNode(fn, context);
            if (fnDefNode)
            {
                EvaluationMapSetter evaluationMapSetter(context);
                PopulateEvaluationMap(fn, argumentValues, context);
                value = Evaluate(fnDefNode, context);
                if (!value)
                {
                    error = true;
                    return;
                }
            }
            else
            {
                error = true;
                return;
            }
        }
        else
        {
            if (fn->IsCompileTimeFn())
            {
                context->GetEvaluationContext()->ResetEvaluationStack();
                for (Value* argumentValue : argumentValues)
                {
                    context->GetEvaluationContext()->GetEvaluationStack()->Push(argumentValue);
                }
                fn->Evaluate(context);
                value = context->GetEvaluationContext()->GetEvaluationStack()->Pop();
            }
            else
            {
                error = true;
                return;
            }
        }
    }
    else
    {
        error = true;
        return;
    }
}

void Evaluator::EvaluateBinOp(otava::ast::NodeKind op, otava::ast::Node* left, otava::ast::Node* right)
{
    switch (op)
    {
    case otava::ast::NodeKind::assignNode:
    {
        Value* rightValue = Evaluate(right, context);
        context->SetInitializer(rightValue);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::plusAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::plusNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::minusAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::minusNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::mulAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::mulNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::divAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::divNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::modAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::modNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::andAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::andNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::orAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::inclusiveOrNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::xorAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::exclusiveOrNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::shiftLeftAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::shiftLeftNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    case otava::ast::NodeKind::shiftRightAssignNode:
    {
        EvaluateBinOp(otava::ast::NodeKind::shiftRightNode, left, right);
        context->SetInitializer(value);
        FlagSetter setValueFlagSetter(context, ContextFlags::setValue);
        Value* leftValue = Evaluate(left, context);
        value = leftValue;
        return;
    }
    }
    Value* leftValue = Evaluate(left, context);
    if (!leftValue)
    {
        error = true;
        return;
    }
    Value* rightValue = Evaluate(right, context);
    if (!rightValue)
    {
        error = true;
        return;
    }
    switch (op)
    {
    case otava::ast::NodeKind::plusNode:
    {
        value = EvaluateAdd(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::minusNode:
    {
        value = EvaluateSub(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::mulNode:
    {
        value = EvaluateMul(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::divNode:
    {
        value = EvaluateDiv(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::modNode:
    {
        value = EvaluateMod(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::shiftLeftNode:
    {
        value = EvaluateShiftLeft(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::shiftRightNode:
    {
        value = EvaluateShiftRight(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::inclusiveOrNode:
    {
        value = EvaluateBitOr(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::exclusiveOrNode:
    {
        value = EvaluateBitXor(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::andNode:
    {
        value = EvaluateBitAnd(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::equalNode:
    {
        value = EvaluateEqual(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::notEqualNode:
    {
        value = EvaluateNotEqual(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::lessNode:
    {
        value = EvaluateLess(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::greaterNode:
    {
        value = EvaluateGreater(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::lessOrEqualNode:
    {
        value = EvaluateLessOrEqual(leftValue, rightValue, context);
        break;
    }
    case otava::ast::NodeKind::greaterOrEqualNode:
    {
        value = EvaluateGreaterOrEqual(leftValue, rightValue, context);
        break;
    }
    default:
    {
        error = true;
        return;
    }
    }
}

void Evaluator::EvaluatePrefixInc(otava::ast::Node* argument)
{
    soul::ast::FullSpan fs = argument->GetFullSpan();
    otava::ast::BinaryExprNode assignmentExpr(fs.span, fs.fileIndex,
        new otava::ast::AssignNode(fs.span, fs.fileIndex),
        argument->Clone(),
        new otava::ast::BinaryExprNode(fs.span, fs.fileIndex, 
            new otava::ast::PlusNode(fs.span, fs.fileIndex), argument->Clone(),
            new otava::ast::IntegerLiteralNode(fs.span, fs.fileIndex, 
                1, otava::ast::Suffix::none, otava::ast::Base::decimal, std::string())));
    value = Evaluate(&assignmentExpr, context);
}

void Evaluator::EvaluatePrefixDec(otava::ast::Node* argument)
{
    soul::ast::FullSpan fs = argument->GetFullSpan();
    otava::ast::BinaryExprNode assignmentExpr(fs.span, fs.fileIndex,
        new otava::ast::AssignNode(fs.span, fs.fileIndex),
        argument->Clone(),
        new otava::ast::BinaryExprNode(fs.span, fs.fileIndex,
            new otava::ast::MinusNode(fs.span, fs.fileIndex), argument->Clone(),
            new otava::ast::IntegerLiteralNode(fs.span, fs.fileIndex,
                1, otava::ast::Suffix::none, otava::ast::Base::decimal, std::string())));
    value = Evaluate(&assignmentExpr, context);
}

void Evaluator::Visit(otava::ast::BinaryExprNode& node)
{
    if (error) return;
    otava::ast::NodeKind op = node.Op()->Kind();
    EvaluateBinOp(op, node.Left(), node.Right());
    if (value)
    {
        return;
    }
    FlagSetter dontThrowFlagSetter(context, ContextFlags::dontThrow);
    std::string groupName = OperatorGroupNameMap::Instance().GetGroupName(op, node.GetFullSpan(), context);
    if (groupName.empty())
    {
        error = true;
        return;
    }
    dontThrowFlagSetter.Reset();
    Exception exception;
    std::vector<TypeSymbol*> templateArguments;
    std::vector<Value*> argumentValues;
    Value* leftValue = Evaluate(node.Left(), context);
    if (!leftValue)
    {
        error = true;
        return;
    }
    argumentValues.push_back(leftValue);
    Value* rightValue = Evaluate(node.Right(), context);
    if (!rightValue)
    {
        error = true;
        return;
    }
    argumentValues.push_back(rightValue);
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments = ValuesToLiterals(argumentValues, node.GetFullSpan(), context);
    std::unique_ptr<BoundFunctionCallNode> fnCall = ResolveOverload(scope, groupName, templateArguments, arguments,
        node.GetFullSpan(), context, exception, OverloadResolutionFlags::dontInstantiate);
    if (fnCall)
    {
        FunctionSymbol* fn = fnCall->GetFunctionSymbol();
        if ((fn->GetDeclarationFlags() & DeclarationFlags::constExprFlag) != DeclarationFlags::none)
        {
            context->GetEvaluationContext()->ResetEvaluationMaps();
            otava::ast::FunctionDefinitionNode* fnDefNode = GetFunctionDefinitionNode(fn, context);
            if (fnDefNode)
            {
                EvaluationMapSetter evaluationMapSetter(context);
                PopulateEvaluationMap(fn, argumentValues, context);
                value = Evaluate(fnDefNode, context);
                if (!value)
                {
                    error = true;
                    return;
                }
            }
            else
            {
                error = true;
                return;
            }
        }
        else
        {
            if (fn->IsCompileTimeFn())
            {
                context->GetEvaluationContext()->ResetEvaluationStack();
                for (Value* argumentValue : argumentValues)
                {
                    context->GetEvaluationContext()->GetEvaluationStack()->Push(argumentValue);
                }
                fn->Evaluate(context);
                value = context->GetEvaluationContext()->GetEvaluationStack()->Pop();
            }
            else
            {
                error = true;
                return;
            }
        }
    }
    else
    {
        error = true;
        return;
    }
}

void Evaluator::Visit(otava::ast::PostfixIncExprNode& node)
{
    Value* result = Evaluate(node.Child(), context);
    EvaluatePrefixInc(node.Child());
    if (value && result)
    {
        value = result;
        return;
    }
    FlagSetter dontThrowFlagSetter(context, ContextFlags::dontThrow);
    std::string groupName = OperatorGroupNameMap::Instance().GetGroupName(node.Kind(), node.GetFullSpan(), context);
    if (groupName.empty())
    {
        error = true;
        return;
    }
    dontThrowFlagSetter.Reset();
    Exception exception;
    std::vector<TypeSymbol*> templateArguments;
    std::vector<Value*> argumentValues;
    Value* argumentValue = Evaluate(node.Child(), context);
    if (!argumentValue)
    {
        error = true;
        return;
    }
    argumentValues.push_back(argumentValue);
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments = ValuesToLiterals(argumentValues, node.GetFullSpan(), context);
    std::unique_ptr<BoundFunctionCallNode> fnCall = ResolveOverload(scope, groupName, templateArguments, arguments,
        node.GetFullSpan(), context, exception, OverloadResolutionFlags::dontInstantiate);
    if (fnCall)
    {
        FunctionSymbol* fn = fnCall->GetFunctionSymbol();
        if ((fn->GetDeclarationFlags() & DeclarationFlags::constExprFlag) != DeclarationFlags::none)
        {
            otava::ast::FunctionDefinitionNode* fnDefNode = GetFunctionDefinitionNode(fn, context);
            if (fnDefNode)
            {
                EvaluationMapSetter evaluationMapSetter(context);
                PopulateEvaluationMap(fn, argumentValues, context);
                value = Evaluate(fnDefNode, context);
                if (!value)
                {
                    error = true;
                    return;
                }
            }
            else
            {
                error = true;
                return;
            }
        }
        else
        {
            if (fn->IsCompileTimeFn())
            {
                context->GetEvaluationContext()->ResetEvaluationStack();
                for (Value* argumentValue : argumentValues)
                {
                    context->GetEvaluationContext()->GetEvaluationStack()->Push(argumentValue);
                }
                fn->Evaluate(context);
                value = context->GetEvaluationContext()->GetEvaluationStack()->Pop();
            }
            else
            {
                error = true;
                return;
            }
        }
    }
    else
    {
        error = true;
        return;
    }
}

void Evaluator::Visit(otava::ast::PostfixDecExprNode& node)
{
    Value* result = Evaluate(node.Child(), context);
    EvaluatePrefixDec(node.Child());
    if (value && result)
    {
        value = result;
        return;
    }
    FlagSetter dontThrowFlagSetter(context, ContextFlags::dontThrow);
    std::string groupName = OperatorGroupNameMap::Instance().GetGroupName(node.Kind(), node.GetFullSpan(), context);
    if (groupName.empty())
    {
        error = true;
        return;
    }
    dontThrowFlagSetter.Reset();
    Exception exception;
    std::vector<TypeSymbol*> templateArguments;
    std::vector<Value*> argumentValues;
    Value* argumentValue = Evaluate(node.Child(), context);
    if (!argumentValue)
    {
        error = true;
        return;
    }
    argumentValues.push_back(argumentValue);
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments = ValuesToLiterals(argumentValues, node.GetFullSpan(), context);
    std::unique_ptr<BoundFunctionCallNode> fnCall = ResolveOverload(scope, groupName, templateArguments, arguments,
        node.GetFullSpan(), context, exception, OverloadResolutionFlags::dontInstantiate);
    if (fnCall)
    {
        FunctionSymbol* fn = fnCall->GetFunctionSymbol();
        if ((fn->GetDeclarationFlags() & DeclarationFlags::constExprFlag) != DeclarationFlags::none)
        {
            otava::ast::FunctionDefinitionNode* fnDefNode = GetFunctionDefinitionNode(fn, context);
            if (fnDefNode)
            {
                EvaluationMapSetter evaluationMapSetter(context);
                PopulateEvaluationMap(fn, argumentValues, context);
                value = Evaluate(fnDefNode, context);
                if (!value)
                {
                    error = true;
                    return;
                }
            }
            else
            {
                error = true;
                return;
            }
        }
        else
        {
            if (fn->IsCompileTimeFn())
            {
                context->GetEvaluationContext()->ResetEvaluationStack();
                for (Value* argumentValue : argumentValues)
                {
                    context->GetEvaluationContext()->GetEvaluationStack()->Push(argumentValue);
                }
                fn->Evaluate(context);
                value = context->GetEvaluationContext()->GetEvaluationStack()->Pop();
            }
            else
            {
                error = true;
                return;
            }
        }
    }
    else
    {
        error = true;
        return;
    }
}

void Evaluator::Visit(otava::ast::InvokeExprNode& node)
{
    if (error) return;
    if (context->GetFlag(ContextFlags::invoke))
    {
        error = true;
        return;
    }
    value = Evaluate(node.Subject(), context);
    std::string groupName;
    std::vector<Value*> argumentValues;
    BoundTypeNode* boundTypeNode = nullptr;
    FunctionSymbol* fn = nullptr;
    if (value)
    {
        if (value->IsFunctionGroupValue())
        {
            FunctionGroupValue* functionGroupValue = static_cast<FunctionGroupValue*>(value);
            FunctionGroupSymbol* functionGroup = functionGroupValue->GetFunctionGroup();
            fn = functionGroupValue->Fn();
            groupName = functionGroup->Name();
        }
        else if (value->IsTypeValue())
        {
            groupName = "@constructor";
            TypeValue* typeValue = static_cast<TypeValue*>(value);
            boundTypeNode = new BoundTypeNode(typeValue->GetType()->AddPointer(context), node.GetFullSpan());
        }
    }
    else
    {
        error = true;
        return;
    }
    if (!groupName.empty())
    {
        for (const auto& arg : node.Nodes())
        {
            Value* argValue = Evaluate(arg.get(), context);
            if (argValue)
            {
                argumentValues.push_back(argValue);
            }
            else
            {
                error = true;
                return;
            }
        }
        std::vector<std::unique_ptr<BoundExpressionNode>> arguments = ValuesToLiterals(argumentValues, node.GetFullSpan(), context);
        if (groupName == "@constructor")
        {
            arguments.insert(arguments.begin(), std::unique_ptr<BoundExpressionNode>(boundTypeNode));
        }
        Exception exception;
        std::vector<TypeSymbol*> templateArguments;
        if (fn)
        {
            scope = fn->GetScope()->GroupScope(context);
        }
        std::unique_ptr<BoundFunctionCallNode> fnCall = ResolveOverload(scope, groupName, templateArguments, arguments,
            node.GetFullSpan(), context, exception, OverloadResolutionFlags::dontInstantiate);
        if (fnCall)
        {
            FunctionSymbol* fn = fnCall->GetFunctionSymbol();
            if ((fn->GetDeclarationFlags() & DeclarationFlags::constExprFlag) != DeclarationFlags::none)
            {
                otava::ast::FunctionDefinitionNode* fnDefNode = GetFunctionDefinitionNode(fn, context);
                if (fnDefNode)
                {
                    EvaluationMapSetter evaluationMapSetter(context);
                    PopulateEvaluationMap(fn, argumentValues, context);
                    value = Evaluate(fnDefNode, context);
                    if (!value)
                    {
                        error = true;
                        return;
                    }
                }
                else
                {
                    error = true;
                    return;
                }
            }
            else
            {
                if (fn->IsCompileTimeFn())
                {
                    context->GetEvaluationContext()->ResetEvaluationStack();
                    for (Value* argumentValue : argumentValues)
                    {
                        context->GetEvaluationContext()->GetEvaluationStack()->Push(argumentValue);
                    }
                    fn->Evaluate(context);
                    value = context->GetEvaluationContext()->GetEvaluationStack()->Pop();
                }
                else
                {
                    error = true;
                    return;
                }
            }
        }
        else
        {
            error = true;
            return;
        }
    }
    else
    {
        error = true;
    }
}

void Evaluator::Visit(otava::ast::SizeOfTypeExprNode& node)
{
    TypeSymbol* type = ResolveType(node.Child(), DeclarationFlags::none, context);
    type = type->DirectType(context)->FinalType(node.GetFullSpan(), context);
    otava::intermediate::Type* irType = type->IrType(*context->GetEmitter(), node.GetFullSpan(), context);
    std::int64_t size = irType->Size();
    value = context->GetEvaluationContext()->GetIntegerValue(size, 
        context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedLongLongIntType, context), context);
}

void Evaluator::Visit(otava::ast::BracedInitListNode& node)
{
    TypeSymbol* type = context->DeclaredInitializerType();
    if (type)
    {
        if (type->IsArrayTypeSymbol())
        {
            ArrayTypeSymbol* arrayTypeSymbol = static_cast<ArrayTypeSymbol*>(type);
            ArrayValue* arrayValue = context->GetSymbolTable()->GetModule()->GetEvaluationContext()->GetArrayValue(type, context);
            std::int64_t count = 0;
            for (otava::ast::Node* element : node.Items())
            {
                if (element->IsLBraceNode() || element->IsRBraceNode()) continue;
                context->SetDeclaredInitializerType(arrayTypeSymbol->ElementType(context));
                Value* elementValue = Evaluate(element, context);
                arrayValue->AddElementValue(elementValue);
                context->SetDeclaredInitializerType(type);
                ++count;
            }
            if (arrayTypeSymbol->Size() == -1)
            {
                arrayTypeSymbol->SetSize(count);
            }
            else if (arrayTypeSymbol->Size() != count)
            {
                ThrowException("conflicting array size: size=" + std::to_string(arrayTypeSymbol->Size()) + ", number of elements in initializer=" + std::to_string(count),
                    node.GetFullSpan(), context);
            }
            value = arrayValue;
        }
        else if (type->PlainType(context)->IsClassTypeSymbol())
        {
            ClassTypeSymbol* classTypeSymbol = static_cast<ClassTypeSymbol*>(type->PlainType(context));
            StructureValue* structureValue = context->GetSymbolTable()->GetModule()->GetEvaluationContext()->GetStructureValue(type, context);
            int index = 0;
            for (otava::ast::Node* field : node.Items())
            {
                if (field->IsLBraceNode() || field->IsRBraceNode()) continue;
                context->SetDeclaredInitializerType(nullptr);
                Value* fieldValue = Evaluate(field, context);
                structureValue->AddFieldValue(fieldValue);
                context->SetDeclaredInitializerType(type);
                ++index;
            }
            value = structureValue;
        }
    }
    else
    {
        value = nullptr;
    }
}

void Evaluator::Visit(otava::ast::CppCastExprNode& node)
{
    TypeSymbol* type = ResolveType(node.TypeId(), DeclarationFlags(), context, TypeResolverFlags::dontThrow);
    if (type && type->IsFundamentalTypeSymbol())
    {
        node.Child()->Accept(*this);
        if (value)
        {
            if (value->IsIntegerValue())
            {
                value = context->GetEvaluationContext()->GetIntegerValue(value->GetIntegerValue(), type, context);
            }
            else if (value->IsFloatingValue())
            {
                value = context->GetEvaluationContext()->GetFloatingValue(value->GetFloatingValue(), type, context);
            }
        }
    }
    else
    {
        value = nullptr;
    }
}

void Evaluator::Visit(otava::ast::FunctionDefinitionNode& node)
{
    node.FunctionBody()->Accept(*this);
}

void Evaluator::Visit(otava::ast::CompoundStatementNode& node)
{
    if (error || breakSet || continueSet) return;
    EvaluationMapSetter evaluationMapSetter(context);
    for (const auto& stmt : node.Nodes())
    {
        stmt->Accept(*this);
        if (returned || error)
        {
            break;
        }
    }
}

void Evaluator::Visit(otava::ast::DeclarationStatementNode& node)
{
    if (error || breakSet || continueSet) return;
    node.Declaration()->Accept(*this);
}

void Evaluator::Visit(otava::ast::SimpleDeclarationNode& node)
{
    node.DeclarationSpecifiers()->Accept(*this);
    Value* v = value;
    if (v && v->IsTypeValue())
    {
        TypeValue* typeValue = static_cast<TypeValue*>(v);
        TypeSymbol* type = typeValue->GetType();
        node.InitDeclaratorList()->Accept(*this);
    }
}

void Evaluator::Visit(otava::ast::DeclSpecifierSequenceNode& node)
{
    bool prevComposeType = composeType;
    composeType = true;
    declarationFlags = DeclarationFlags::none;
    otava::ast::NodeList<otava::ast::Node>& nodes = node.Nodes();
    for (int index = 0; index < nodes.Count(); ++index)
    {
        otava::ast::Node* n = nodes[index];
        n->Accept(*this);
    }
    if (declarationFlags != DeclarationFlags::none)
    {
        TypeSymbol* fundamentalType = GetFundamentalType(declarationFlags, node.GetFullSpan(), context);
        value = context->GetEvaluationContext()->GetTypeValue(context->GetModule(), context->GetNextSymbolId(SymbolKind::typeValueSymbol), fundamentalType, context);
    }
    else if (!value)
    {
        error = true;
        return;
    }
    composeType = prevComposeType;
}

void Evaluator::Visit(otava::ast::InitDeclaratorListNode& node)
{
    for (int index = 0; index < node.Count(); ++index)
    {
        otava::ast::Node* n = node.Items()[index];
        n->Accept(*this);
    }
}

void Evaluator::Visit(otava::ast::InitDeclaratorNode& node)
{
    node.Right()->Accept(*this);
    if (value)
    {
        initializer = value;
        bool prevSetValue = setValue;
        setValue = true;
        node.Left()->Accept(*this);
        setValue = prevSetValue;
    }
}

void Evaluator::Visit(otava::ast::ExpressionStatementNode& node)
{
    if (error || breakSet || continueSet) return;
    node.Expression()->Accept(*this);
}

void Evaluator::Visit(otava::ast::ReturnStatementNode& node)
{
    if (error || breakSet || continueSet) return;
    if (node.ReturnValue())
    {
        node.ReturnValue()->Accept(*this);
    }
    returned = true;
}

void Evaluator::Visit(otava::ast::IfStatementNode& node)
{
    if (error || breakSet || continueSet) return;
    EvaluationMapSetter evaluationMapSetter(context);
    Value* value = Evaluate(node.Condition(), context);
    if (value)
    {
        BoolValue* boolValue = value->ToBoolValue(context);
        if (boolValue)
        {
            if (boolValue->GetValue())
            {
                node.ThenStatement()->Accept(*this);
                if (error) return;
            }
            else if (node.ElseStatement() && !boolValue->GetValue())
            {
                node.ElseStatement()->Accept(*this);
                if (error) return;
            }
        }
        else
        {
            error = true;
            return;
        }
    }
    else
    {
        error = true;
        return;
    }
}

void Evaluator::Visit(otava::ast::SwitchStatementNode& node)
{
    error = true;
}

void Evaluator::Visit(otava::ast::WhileStatementNode& node)
{
    if (error || breakSet || continueSet) return;
    EvaluationMapSetter evaluationMapSetter(context);
    Value* value = Evaluate(node.Condition(), context);
    if (value)
    {
        BoolValue* boolValue = value->ToBoolValue(context);
        while (boolValue && boolValue->GetValue())
        {
            node.Statement()->Accept(*this);
            if (error || breakSet)
            {
                if (breakSet)
                {
                    breakSet = false;
                }
                return;
            }
            if (continueSet)
            {
                continueSet = false;
            }
            value = Evaluate(node.Condition(), context);
            if (value)
            {
                boolValue = value->ToBoolValue(context);
            }
            else
            {
                error = true;
                return;
            }
        }
        if (!boolValue)
        {
            error = true;
            return;
        }
    }
    else
    {
        error = true;
        return;
    }
}

void Evaluator::Visit(otava::ast::DoStatementNode& node)
{
    if (error || breakSet || continueSet) return;
    BoolValue* boolValue = nullptr;
    do
    {
        node.Statement()->Accept(*this);
        if (error || breakSet)
        {
            if (breakSet)
            {
                breakSet = false;
            }
            return;
        }
        if (continueSet)
        {
            continueSet = false;
        }
        Value* value = Evaluate(node.Expression(), context);
        if (value)
        {
            boolValue = value->ToBoolValue(context);
        }
        else
        {
            error = true;
            return;
        }
    } 
    while (boolValue && boolValue->GetValue());
    if (!boolValue)
    {
        error = true;
        return;
    }
}

void Evaluator::Visit(otava::ast::RangeForStatementNode& node)
{
    error = true;
}

void Evaluator::Visit(otava::ast::ForStatementNode& node)
{
    if (error || breakSet) return;
    EvaluationMapSetter evaluationMapSetter(context);
    if (node.InitStatement())
    {
        node.InitStatement()->Accept(*this);
        if (error) return;
        bool condition = true;
        if (node.Condition())
        {
            Value* value = Evaluate(node.Condition(), context);
            if (value)
            {
                BoolValue* boolValue = value->ToBoolValue(context);
                if (!boolValue)
                {
                    error = true;
                    return;
                }
                condition = boolValue->GetValue();
            }
            else
            {
                error = true;
                return;
            }
        }
        while (condition)
        {
            if (node.Statement())
            {
                node.Statement()->Accept(*this);
                if (error || breakSet)
                {
                    if (breakSet)
                    {
                        breakSet = false;
                    }
                    return;
                }
            }
            if (continueSet)
            {
                continueSet = false;
            }
            if (node.LoopExpr())
            {
                node.LoopExpr()->Accept(*this);
                if (error) return;
            }
            if (node.Condition())
            {
                Value* value = Evaluate(node.Condition(), context);
                if (value)
                {
                    BoolValue* boolValue = value->ToBoolValue(context);
                    if (!boolValue)
                    {
                        error = true;
                        return;
                    }
                    condition = boolValue->GetValue();
                }
                else
                {
                    error = true;
                    return;
                }
            }
        }
    }
}

void Evaluator::Visit(otava::ast::BreakStatementNode& node)
{
    if (error) return;
    breakSet = true;
}

void Evaluator::Visit(otava::ast::ContinueStatementNode& node)
{
    if (error) return;
    continueSet = true;
}

void Evaluator::Visit(otava::ast::GotoStatementNode& node)
{
    error = true;
}

void Evaluator::Visit(otava::ast::TryStatementNode& node)
{
    error = true;
}

Value* TryEvaluate(otava::ast::Node* node, Context* context)
{
    Evaluator evaluator(context);
    node->Accept(evaluator);
    return evaluator.GetValue();
}

Value* Evaluate(otava::ast::Node* node, Context* context)
{
    try
    {
        return TryEvaluate(node, context);
    }
    catch (...)
    {
    }
    return nullptr;
}

} // namespace otava::symbols
