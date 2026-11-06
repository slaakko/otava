// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_kind;

namespace otava::symbols {

std::string FunctionKindStr(FunctionKind functionKind)
{
    switch (functionKind)
    {
    case FunctionKind::function: return "fn";
    case FunctionKind::constructor: return "ctor";
    case FunctionKind::destructor: return "dtor";
    case FunctionKind::special: return "special";
    case FunctionKind::conversionMemFn: return "conversionMemFn";
    }
    return "fn";
}

std::string SpecialFunctionKindPrefix(SpecialFunctionKind specialFunctionKind)
{
    switch (specialFunctionKind)
    {
    case SpecialFunctionKind::defaultCtor: return "default_ctor";
    case SpecialFunctionKind::copyCtor: return "copy_ctor";
    case SpecialFunctionKind::moveCtor: return "move_ctor";
    case SpecialFunctionKind::copyAssignment: return "copy_assignment";
    case SpecialFunctionKind::moveAssignment: return "move_assignment";
    case SpecialFunctionKind::dtor: return "dtor";
    }
    return std::string();
}

std::string MakeFunctionQualifierStr(FunctionQualifiers qualifiers)
{
    std::string s;
    if ((qualifiers & FunctionQualifiers::isVolatile) != FunctionQualifiers::none)
    {
        s.append("volatile");
    }
    if ((qualifiers & FunctionQualifiers::isConst) != FunctionQualifiers::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("const");
    }
    if ((qualifiers & FunctionQualifiers::isOverride) != FunctionQualifiers::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("override");
    }
    if ((qualifiers & FunctionQualifiers::isFinal) != FunctionQualifiers::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("final");
    }
    if ((qualifiers & FunctionQualifiers::isDefault) != FunctionQualifiers::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("= default");
    }
    if ((qualifiers & FunctionQualifiers::isDeleted) != FunctionQualifiers::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("= delete");
    }
    return s;
}

} // namespace otava::symbols
