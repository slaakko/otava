// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.function_kind;

import std;

export namespace otava::symbols {

enum class FunctionKind : std::uint8_t
{
    function, constructor, destructor, special, conversionMemFn
};

constexpr std::uint8_t ToUnderlying(FunctionKind functionKind)
{
    return std::uint8_t(functionKind);
}

std::string FunctionKindStr(FunctionKind functionKind);

enum class SpecialFunctionKind : std::uint8_t
{
    none, defaultCtor, copyCtor, moveCtor, copyAssignment, moveAssignment, dtor
};

constexpr std::uint8_t ToUnderlying(SpecialFunctionKind specialFunctionKind)
{
    return std::uint8_t(specialFunctionKind);
}

std::string SpecialFunctionKindPrefix(SpecialFunctionKind specialFunctionKind);

enum class Linkage : std::uint8_t
{
    c_linkage, cpp_linkage
};

constexpr std::uint8_t ToUnderlying(Linkage linkage)
{
    return std::uint8_t(linkage);
}

enum class FunctionQualifiers : std::uint16_t
{
    none = 0, isConst = 1 << 0, isVolatile = 1 << 1, isOverride = 1 << 2, isFinal = 1 << 3, isPure = 1 << 4, isDefault = 1 << 5, isDeleted = 1 << 6,
    noreturn = 1 << 7, isNoexcept = 1 << 8
};

constexpr FunctionQualifiers operator|(FunctionQualifiers left, FunctionQualifiers right) noexcept
{
    return FunctionQualifiers(std::uint16_t(left) | std::uint16_t(right));
}

constexpr FunctionQualifiers operator&(FunctionQualifiers left, FunctionQualifiers right) noexcept
{
    return FunctionQualifiers(std::uint16_t(left) & std::uint16_t(right));
}

constexpr FunctionQualifiers operator~(FunctionQualifiers qualifiers) noexcept
{
    return FunctionQualifiers(~std::uint16_t(qualifiers));
}

constexpr std::uint16_t ToUnderlying(FunctionQualifiers qualifiers)
{
    return std::uint16_t(qualifiers);
}

enum class ConversionKind : std::uint8_t
{
    implicitConversion, explicitConversion
};

constexpr std::uint8_t ToUnderlying(ConversionKind conversionKind)
{
    return std::uint8_t(conversionKind);
}

std::string MakeFunctionQualifierStr(FunctionQualifiers qualifiers);

enum class FunctionSymbolFlags : std::uint16_t
{
    none = 0, bound = 1 << 0, specialization = 1 << 1, trivialDestructor = 1 << 2, returnsClass = 1 << 3, conversion = 1 << 4, fixedIrName = 1 << 5, inline_ = 1 << 6,
    generated = 1 << 7, skip = 1 << 8, containsReturnStatement = 1 << 9, containsStatics = 1 << 10, containsNodeWithNoSource = 1 << 11, skipInvokeChecking = 1 << 12,
    containsLocalVariableWithDestructor = 1 << 13, unparsed = 1 << 14, parsing = 1 << 15
};

constexpr std::uint16_t ToUnderlying(FunctionSymbolFlags flags)
{
    return std::uint16_t(flags);
}

constexpr FunctionSymbolFlags operator|(FunctionSymbolFlags left, FunctionSymbolFlags right) noexcept
{
    return FunctionSymbolFlags(ToUnderlying(left) | ToUnderlying(right));
}

constexpr FunctionSymbolFlags operator&(FunctionSymbolFlags left, FunctionSymbolFlags right) noexcept
{
    return FunctionSymbolFlags(ToUnderlying(left) & ToUnderlying(right));
}

constexpr FunctionSymbolFlags operator~(FunctionSymbolFlags flags) noexcept
{
    return FunctionSymbolFlags(~ToUnderlying(flags));
}

} // namespace otava::symbols
