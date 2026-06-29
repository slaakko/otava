// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_symbol;

import otava.symbols.class_group_symbol;
import otava.symbols.classes;
import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.function_group_symbol;
import otava.symbols.modules;
import otava.symbols.project;
import otava.symbols.templates;
import otava.symbols.type_compare;
import otava.symbols.type_symbol;
import otava.symbols.variable_symbol;
import soul.xml.dom;
import util.sha1;

namespace otava::symbols {

class OperatorFunctionMap
{
public:
    OperatorFunctionMap();
    static OperatorFunctionMap& Instance();
    void Init();
    std::string GetPrefix(const std::string& name) const;
private:
    std::map<std::string, std::string> map;
};

OperatorFunctionMap::OperatorFunctionMap()
{
    map["operator new[]"] = "op_new_array";
    map["operator new"] = "op_new";
    map["operator delete[]"] = "op_delete_array";
    map["operator delete"] = "op_delete";
    map["operator co_await"] = "op_co_await";
    map["operator()"] = "op_apply";
    map["operator[]"] = "op_subscript";
    map["operator->"] = "op_arrow";
    map["operator->*"] = "op_arrow_star";
    map["operator~"] = "op_cpl";
    map["operator!"] = "op_not";
    map["operator+"] = "op_plus";
    map["operator-"] = "op_minus";
    map["operator*"] = "op_star";
    map["operator/"] = "op_div";
    map["operator%"] = "op_mod";
    map["operator^"] = "op_xor";
    map["operator&"] = "op_and";
    map["operator|"] = "op_or";
    map["operator="] = "op_assign";
    map["operator+="] = "op_plus_assign";
    map["operator-="] = "op_minus_assign";
    map["operator*="] = "op_mul_assign";
    map["operator/="] = "op_div_assign";
    map["operator%="] = "op_mod_assign";
    map["operator^="] = "op_xor_assign";
    map["operator&="] = "op_and_assign";
    map["operator|="] = "op_or_assign";
    map["operator<<="] = "op_shift_left_assign";
    map["operator>>="] = "op_shift_right_assign";
    map["operator=="] = "op_eq";
    map["operator!="] = "op_neq";
    map["operator<="] = "op_leq";
    map["operator>="] = "op_geq";
    map["operator<=>"] = "op_compare";
    map["operator<"] = "op_less";
    map["operator>"] = "op_greater";
    map["operator&&"] = "op_conjuction";
    map["operator||"] = "op_disjuction";
    map["operator<<"] = "op_shift_left";
    map["operator>>"] = "op_shift_right";
    map["operator++"] = "op_plus_plus";
    map["operator--"] = "op_minus_minus";
    map["operator,"] = "op_comma";
}

OperatorFunctionMap& OperatorFunctionMap::Instance()
{
    static OperatorFunctionMap instance;
    return instance;
}

void OperatorFunctionMap::Init()
{
}

std::string OperatorFunctionMap::GetPrefix(const std::string& name) const
{
    auto it = map.find(name);
    if (it != map.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

std::string GetOperatorFunctionPrefix(const std::string& functionName)
{
    return OperatorFunctionMap::Instance().GetPrefix(functionName);
}

FunctionSymbol::FunctionSymbol(Module* module_, SymbolId id_) :
    ContainerSymbol(module_, id_), flags(FunctionSymbolFlags::none), qualifiers(FunctionQualifiers::none),
    linkage(Linkage::cpp_linkage), group(nullptr), index(0), functionKind(FunctionKind::function), returnType(nullptr), returnTypeId(zeroSymbolId),
    conversionKind(ConversionKind::implicitConversion), conversionParamType(nullptr), conversionArgType(nullptr), conversionDistance(0),
    memFnParamsConstructed(false), vtabIndex(-1), returnValueParam(nullptr), returnValueParamId(zeroSymbolId), 
    conversionParamTypeId(zeroSymbolId), conversionArgTypeId(zeroSymbolId), destructor(nullptr), nextTemporaryId(0), classParsingMap(nullptr), 
    parametersFetched(false), destructing(false)
{
    GetScope()->SetKind(ScopeKind::functionScope);
    GetModule()->AddFunction(this);
}

FunctionSymbol::FunctionSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    ContainerSymbol(module_, id_, name_), flags(FunctionSymbolFlags::none), qualifiers(FunctionQualifiers::none),
    linkage(Linkage::cpp_linkage), group(nullptr), index(0), functionKind(FunctionKind::function), returnType(nullptr), returnTypeId(zeroSymbolId),
    conversionKind(ConversionKind::implicitConversion), conversionParamType(nullptr), conversionArgType(nullptr), conversionDistance(0),
    memFnParamsConstructed(false), vtabIndex(-1), returnValueParam(nullptr), returnValueParamId(zeroSymbolId), conversionParamTypeId(zeroSymbolId),
    conversionArgTypeId(zeroSymbolId), destructor(nullptr), nextTemporaryId(0), classParsingMap(nullptr), parametersFetched(false), destructing(false)
{
    GetScope()->SetKind(ScopeKind::functionScope);
    GetModule()->AddFunction(this);
}

void FunctionSymbol::SetGroup(FunctionGroupSymbol* group_) noexcept
{
    group = group_;
}

FunctionSymbol::~FunctionSymbol()
{
    destructing = true;
    if (GetModule())
    {
        GetModule()->RemoveFunction(this);
    }
    if (group)
    {
        group->Remove(this);
    }
    for (ClassTypeSymbol* cls : classes)
    {
        cls->UnmapFunction(this);
    }
}

FunctionGroupSymbol* FunctionSymbol::Group(Context* context) const
{
    if (group)
    {
        return group;
    }
    if (IsReadOnly() && groupId != zeroSymbolId)
    {
        group = GetModule()->GetSymbolTable()->GetFunctionGroupSymbol(groupId, context);
    }
    return group;
}

std::string FunctionSymbol::GroupName() const
{
    if (functionKind == FunctionKind::constructor)
    {
        return "@constructor";
    }
    else if (functionKind == FunctionKind::destructor)
    {
        return "@destructor";
    }
    else
    {
        return Name();
    }
}

bool FunctionSymbol::IsVirtual() const noexcept
{
    if ((GetDeclarationFlags() & DeclarationFlags::virtualFlag) != DeclarationFlags::none) return true;
    if ((qualifiers & FunctionQualifiers::isOverride) != FunctionQualifiers::none) return true;
    if ((qualifiers & FunctionQualifiers::isFinal) != FunctionQualifiers::none) return true;
    return false;
}

bool FunctionSymbol::IsConst() const noexcept
{
    if ((qualifiers & FunctionQualifiers::isConst) != FunctionQualifiers::none) return true;
    return false;
}

bool FunctionSymbol::IsPure() const noexcept
{
    if ((qualifiers & FunctionQualifiers::isPure) != FunctionQualifiers::none) return true;
    return false;
}

bool FunctionSymbol::IsNoExcept() const noexcept
{
    if ((qualifiers & FunctionQualifiers::isNoexcept) != FunctionQualifiers::none) return true;
    if (GetFunctionKind() == FunctionKind::destructor) return true;
    if (GroupName() == "@destructor") return true;
    if (GetLinkage() == Linkage::c_linkage) return true;
    return false;
}

void FunctionSymbol::SetVirtual() noexcept
{
    SetDeclarationFlags(GetDeclarationFlags() | DeclarationFlags::virtualFlag);
}

bool FunctionSymbol::IsOverride() const noexcept
{
    return (qualifiers & FunctionQualifiers::isOverride) != FunctionQualifiers::none;
}

bool FunctionSymbol::IsFinal() const noexcept
{
    return (qualifiers & FunctionQualifiers::isFinal) != FunctionQualifiers::none;
}

void FunctionSymbol::SetOverride() noexcept
{
    qualifiers = qualifiers | FunctionQualifiers::isOverride;
}

void FunctionSymbol::SetFinal() noexcept
{
    qualifiers = qualifiers | FunctionQualifiers::isFinal;
}

void FunctionSymbol::SetNoExcept() noexcept
{
    qualifiers = qualifiers | FunctionQualifiers::isNoexcept;
}

bool FunctionSymbol::IsStatic() const noexcept
{
    return (GetDeclarationFlags() & DeclarationFlags::staticFlag) != DeclarationFlags::none;
}

bool FunctionSymbol::IsExplicit() const noexcept
{
    return (GetDeclarationFlags() & DeclarationFlags::explicitFlag) != DeclarationFlags::none;
}

bool FunctionSymbol::IsDestructor() const noexcept
{
    return GroupName() == "@destructor";
}

bool FunctionSymbol::HasForwardDeclarationType(Context* context) const 
{
    for (ParameterSymbol* parameter : MemFnParameters(context))
    {
        TypeSymbol* baseType = parameter->GetType(context)->GetBaseType(context);
        if (baseType->HasForwardClassDeclarationSymbol(context)) return true;
    }
    TypeSymbol* returnType = ReturnType(context);
    if (returnType)
    {
        TypeSymbol* baseType = returnType->GetBaseType(context);
        if (baseType->HasForwardClassDeclarationSymbol(context)) return true;
    }
    return false;
}

void FunctionSymbol::SetConversionParamType(TypeSymbol* conversionParamType_) noexcept
{
    conversionParamType = conversionParamType_;
    if (conversionParamType->GetModule() != GetModule())
    {
        GetModule()->GetSymbolTable()->AddImportedSymbol(conversionParamType->Id(), conversionParamType->GetModule()->Id());
    }
}

TypeSymbol* FunctionSymbol::GetConversionParamType(Context* context) const
{
    TypeSymbol* conversionParamType = ConversionParamType();
    if (conversionParamType)
    {
        return conversionParamType;
    }
    if (IsReadOnly() && conversionParamTypeId != zeroSymbolId)
    {
        conversionParamType = GetModule()->GetSymbolTable()->GetTypeSymbol(conversionParamTypeId, context);
        if (!conversionParamType)
        {
            ThrowException("conversion parameter type id " + std::to_string(ToUnderlying(conversionParamTypeId)) + " not found from module '" + GetModule()->Name() + "'");
        }
    }
    return conversionParamType;
}

TypeSymbol* FunctionSymbol::GetConversionArgType(Context* context) const
{
    TypeSymbol* conversionArgType = ConversionArgType();
    if (conversionArgType)
    {
        return conversionArgType;
    }
    if (IsReadOnly() && conversionArgTypeId != zeroSymbolId)
    {
        conversionArgType = GetModule()->GetSymbolTable()->GetTypeSymbol(conversionArgTypeId, context);
        if (!conversionArgType)
        {
            ThrowException("conversion argument type id " + std::to_string(ToUnderlying(conversionArgTypeId)) + " not found from module '" + GetModule()->Name() + "'");
        }
    }
    return conversionArgType;
}

void FunctionSymbol::SetConversionArgType(TypeSymbol* conversionArgType_) noexcept
{
    conversionArgType = conversionArgType_;
    if (conversionArgType->GetModule() != GetModule())
    {
        GetModule()->GetSymbolTable()->AddImportedSymbol(conversionArgType->Id(), conversionArgType->GetModule()->Id());
    }
}

void FunctionSymbol::SetConversionKind(ConversionKind conversionKind_) noexcept
{
    conversionKind = conversionKind_;
}

void FunctionSymbol::SetConversionDistance(std::int32_t conversionDistance_) noexcept
{
    conversionDistance = conversionDistance_;
}

std::string FunctionSymbol::FullName(Context* context) const
{
    ClassTypeSymbol* parentClassType = ParentClassType(context);
    std::string fullName;
    if (parentClassType)
    {
        fullName = parentClassType->FullName(context);
    }
    else if (ParentNamespace(context))
    {
        fullName = ParentNamespace(context)->FullName(context);
    }
    fullName.append("::").append(Name()).append("(");
    bool first = true;
    const std::vector<ParameterSymbol*>& params = Parameters(context);
    for (ParameterSymbol* parameter : params)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            fullName.append(", ");
        }
        fullName.append(parameter->GetType(context)->FullName(context));
    }
    fullName.append(")");
    std::string qualifierStr = MakeFunctionQualifierStr(Qualifiers());
    if (!qualifierStr.empty())
    {
        fullName.append(" ").append(qualifierStr);
    }
    return fullName;
}

void FunctionSymbol::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context) 
{
    ContainerSymbol::AddSymbol(symbol, fullSpan, context);
    if (symbol->IsParameterSymbol())
    {
        ParameterSymbol* parameterSymbol = static_cast<ParameterSymbol*>(symbol);
        if (parameterSymbol->GetParameterKind() == ParameterKind::regular)
        {
            parameters.push_back(parameterSymbol);
        }
    }
    else if (symbol->IsLocalVariableSymbol(context))
    {
        localVariables.push_back(static_cast<VariableSymbol*>(symbol));
    }
}

const std::vector<ParameterSymbol*>& FunctionSymbol::Parameters(Context* context) const
{
    if (IsReadOnly() && !parametersFetched)
    {
        parametersFetched = true;
        for (auto parameterId : parameterIds)
        {
            ParameterSymbol* parameterSymbol = GetModule()->GetSymbolTable()->GetParameterSymbol(parameterId, context);
            if (parameterSymbol)
            {
                parameters.push_back(parameterSymbol);
            }
            else
            {
                ThrowException("parameter symbol for id " + std::to_string(ToUnderlying(parameterId)) + " not found");
            }
        }
    }
    return parameters;
}

void FunctionSymbol::AddTemporaryParameter(TypeSymbol* paramType, int index, Context* context)
{
    std::string paramName = "p" + std::to_string(index);
    std::unique_ptr<ParameterSymbol> temporaryParam(new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), paramName));
    temporaryParam->SetType(paramType, context);
    parameters.push_back(temporaryParam.get());
    temporaryParams.push_back(std::move(temporaryParam));
}

void FunctionSymbol::ClearTemporaryParameters()
{
    thisParam.reset();
    memFnParamsConstructed = false;
    memFnParameters.clear();
    parameters.clear();
    temporaryParams.clear();
    SetParent(nullptr);
}

ParameterSymbol* FunctionSymbol::ThisParam(Context* context) const
{
    if (IsStatic())
    {
        return nullptr;
    }
    if (!thisParam)
    {
        ClassTypeSymbol* classType = ParentClassType(context);
        if (classType)
        {
            if ((Qualifiers() & FunctionQualifiers::isConst) != FunctionQualifiers::none)
            {
                ParameterSymbol* tp = new ParameterSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
                tp->SetType(classType->AddConst(context)->AddPointer(context), context);
                thisParam.reset(tp);
            }
            else
            {
                ParameterSymbol* tp = new ParameterSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
                tp->SetType(classType->AddPointer(context), context);
                thisParam.reset(tp);
            }
        }
    }
    return thisParam.get();
}

const std::vector<ParameterSymbol*>& FunctionSymbol::MemFnParameters(Context* context) const
{
    if (memFnParamsConstructed) return memFnParameters;
    memFnParamsConstructed = true;
    ParameterSymbol* thisParam = ThisParam(context);
    if (thisParam)
    {
        memFnParameters.push_back(thisParam);
    }
    const std::vector<ParameterSymbol*>& params = Parameters(context);
    for (ParameterSymbol* parameter : params)
    {
        memFnParameters.push_back(parameter);
    }
    return memFnParameters;
}

SpecialFunctionKind FunctionSymbol::GetSpecialFunctionKind(Context* context) const noexcept
{
    ClassTypeSymbol* classType = ParentClassType(context);
    TypeSymbol* classTemplate = nullptr;
    std::string className;
    if (classType)
    {
        className = classType->Name();
    }
    if (classType && classType->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(classType);
        classTemplate = specialization->ClassTemplate(context);
        className = classTemplate->Name();
    }
    if (classType)
    {
        const std::vector<ParameterSymbol*>& memFnParams = MemFnParameters(context);
        TypeSymbol* pointerType = classType->AddPointer(context);
        if (memFnParams.size() == 1 && TypesEqual(memFnParams[0]->GetType(context), pointerType, context) && Name() == className) return SpecialFunctionKind::defaultCtor;
        if (memFnParams.size() == 1 && TypesEqual(memFnParams[0]->GetType(context), pointerType, context) && Name() == "~" + className) return SpecialFunctionKind::dtor;
        if (classTemplate)
        {
            if (memFnParams.size() == 1 && TypesEqual(memFnParams[0]->GetType(context), pointerType, context) && Name() == "~" + classTemplate->Name())
            {
                return SpecialFunctionKind::dtor;
            }
        }
        TypeSymbol* constRefType = classType->AddConst(context)->AddLValueRef(context);
        if (memFnParams.size() == 2 && TypesEqual(memFnParams[0]->GetType(context), pointerType, context) && Name() == className &&
            TypesEqual(memFnParams[1]->GetType(context), constRefType, context)) return SpecialFunctionKind::copyCtor;
        TypeSymbol* rvalueRefType = classType->AddRValueRef(context);
        if (memFnParams.size() == 2 && TypesEqual(memFnParams[0]->GetType(context), pointerType, context) && Name() == className &&
            TypesEqual(memFnParams[1]->GetType(context), rvalueRefType, context)) return SpecialFunctionKind::moveCtor;
        if (memFnParams.size() == 2 && TypesEqual(memFnParams[0]->GetType(context), pointerType, context) && Name() == "operator=" &&
            TypesEqual(memFnParams[1]->GetType(context), constRefType, context)) return SpecialFunctionKind::copyAssignment;
        if (memFnParams.size() == 2 && TypesEqual(memFnParams[0]->GetType(context), pointerType, context) && Name() == "operator=" &&
            TypesEqual(memFnParams[1]->GetType(context), rvalueRefType, context)) return SpecialFunctionKind::moveAssignment;
    }
    return SpecialFunctionKind::none;
}

TemplateDeclarationSymbol* FunctionSymbol::ParentTemplateDeclaration(Context* context) const noexcept
{
    Symbol* parentSymbol = const_cast<FunctionSymbol*>(this)->Parent(context);
    if (parentSymbol && parentSymbol->IsTemplateDeclarationSymbol())
    {
        return static_cast<TemplateDeclarationSymbol*>(parentSymbol);
    }
    return nullptr;
}

ParameterSymbol* FunctionSymbol::ReturnValueParam(Context* context) const
{
    if (returnValueParam)
    {
        return returnValueParam;
    }
    if (IsReadOnly() && returnValueParamId != zeroSymbolId)
    {
        returnValueParam = GetModule()->GetSymbolTable()->GetParameterSymbol(returnValueParamId, context);
        if (!returnValueParam)
        {
            ThrowException("return value paramter id " + std::to_string(ToUnderlying(returnValueParamId)) + " not found from function '" + FullName(context) + "'");
        }
    }
    return returnValueParam;
}

void FunctionSymbol::SetReturnValueParam(ParameterSymbol* returnValueParam_) noexcept
{
    returnValueParam = returnValueParam_;
}

TypeSymbol* FunctionSymbol::ReturnType(Context* context) const
{
    if (returnType)
    {
        return returnType;
    }
    if (IsReadOnly() && returnTypeId != zeroSymbolId)
    {
        returnType = GetModule()->GetSymbolTable()->GetTypeSymbol(returnTypeId, context);
        if (!returnType)
        {
            ThrowException("return type of function '" + FullName(context) + "' id " + std::to_string(ToUnderlying(returnTypeId)) + " not resolved", 
                GetFullSpan(), context);
        }
    }
    return returnType;
}

void FunctionSymbol::SetReturnType(TypeSymbol* returnType_, Context* context)
{
    returnType = returnType_;
    if (returnType)
    {
        if (returnType->IsClassTypeSymbol() || returnType->IsForwardClassDeclarationSymbol())
        {
            SetReturnsClass();
            TypeSymbol* returnValueType = returnType->AddPointer(context);
            ParameterSymbol* returnValueParam = new ParameterSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "@return_value");
            returnValueParam->SetParameterKind(ParameterKind::returnValueParam);
            AddSymbol(returnValueParam, soul::ast::FullSpan(), context);
            returnValueParam->SetType(returnValueType, context);
            SetReturnValueParam(returnValueParam);
        }
    }
    if (returnType && returnType->GetModule() != context->GetModule())
    {
        context->GetModule()->GetSymbolTable()->AddImportedSymbol(returnType->Id(), returnType->GetModule()->Id());
    }
}

void FunctionSymbol::AddDefinitionToGroup(Context* context)
{
}

ClassTypeSymbol* FunctionSymbol::ParentClassType(Context* context) const noexcept
{
    Symbol* parent = const_cast<FunctionSymbol*>(this)->Parent(context);
    if (parent && parent->IsClassTypeSymbol())
    {
        return static_cast<ClassTypeSymbol*>(parent);
    }
    else
    {
        Scope* scope = const_cast<FunctionSymbol*>(this)->GetScope();
        Scope* classScope = scope->GetClassScope(context);
        if (classScope)
        {
            Symbol* symbol = classScope->GetSymbol();
            if (symbol && symbol->IsClassTypeSymbol())
            {
                return static_cast<ClassTypeSymbol*>(symbol);
            }
        }
    }
    return nullptr;
}

bool FunctionSymbol::IsTemplate(Context* context) const noexcept
{
    return ParentTemplateDeclaration(context) != nullptr && !IsSpecialization();
}

void FunctionSymbol::SetSpecialization(const std::vector<TypeSymbol*>& specialization_)
{
    specialization = specialization_;
}

const std::vector<TypeSymbol*>& FunctionSymbol::Specialization()
{
    // todo specialization ids
    return specialization;
}

bool FunctionSymbol::IsMemberFunction(Context* context) const noexcept
{
    ClassTypeSymbol* classType = ParentClassType(context);
    if (classType && Parent(context) == classType)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Cardinality FunctionSymbol::TemplateArity(Context* context) const noexcept
{
    TemplateDeclarationSymbol* templateDecl = ParentTemplateDeclaration(context);
    return templateDecl->Arity();
}

bool FunctionSymbol::IsMemFnOfClassTemplate(Context* context) const noexcept
{
    if (IsSpecialization()) return false;
    ClassTypeSymbol* parentClassType = ParentClassType(context);
    if (parentClassType && parentClassType->IsTemplate(context))
    {
        return true;
    }
    return false;
}

bool FunctionSymbol::IsExplicitSpecializationDefinitionSymbol(Context* context) const noexcept
{
    if (!IsFunctionDefinitionSymbol()) return false;
    TemplateDeclarationSymbol* templateDeclaration = ParentTemplateDeclaration(context);
    if (!templateDeclaration) return false;
    if (templateDeclaration->Arity() != Cardinality(0)) return false;
    return true;
}

bool FunctionSymbol::IsExplicitSpecializationDeclaration(Context* context) const noexcept
{
    if (IsFunctionDefinitionSymbol()) return false;
    TemplateDeclarationSymbol* templateDeclaration = ParentTemplateDeclaration(context);
    if (!templateDeclaration) return false;
    if (templateDeclaration->Arity() != Cardinality(0)) return false;
    return true;
}

Cardinality FunctionSymbol::Arity() const noexcept
{
    if (IsReadOnly())
    {
        return Cardinality(parameterIds.size());
    }
    return Cardinality(parameters.size());
}

Cardinality FunctionSymbol::MemFnArity(Context* context) const
{
    return Cardinality(MemFnParameters(context).size());
}

Cardinality FunctionSymbol::MinArity(Context* context) const
{
    Cardinality minArity = Arity();
    const std::vector<ParameterSymbol*>& params = Parameters(context);
    for (Index i = Index(params.size()) - Index(1); i >= Index(0); --i)
    {
        ParameterSymbol* parameter = params[ToUnderlying(i)];
        if (parameter->DefaultValue())
        {
            --minArity;
        }
        else
        {
            break;
        }
    }
    return minArity;
}

Cardinality FunctionSymbol::MinMemFnArity(Context* context) const
{
    Cardinality minMemFnArity = MemFnArity(context);
    const std::vector<ParameterSymbol*>& params = Parameters(context);
    for (Index i = Index(params.size()) - Index(1); i >= Index(0); --i)
    {
        ParameterSymbol* parameter = params[ToUnderlying(i)];
        if (parameter->DefaultValue())
        {
            --minMemFnArity;
        }
        else
        {
            break;
        }
    }
    return minMemFnArity;
}

void FunctionSymbol::Write(Writer& writer)
{
    ContainerSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(flags));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(qualifiers));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(linkage));
    writer.GetBinaryStreamWriter().Write(index);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(functionKind));
    Cardinality parameterCount = Cardinality(parameters.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(parameterCount));
    for (const auto* parameter : parameters)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(parameter->Id()));
    }
    if (returnType)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(returnType->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    if (ReturnsClass())
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(returnValueParam->Id()));
    }
    if (GetFlag(FunctionSymbolFlags::fixedIrName))
    {
        writer.GetBinaryStreamWriter().Write(fixedIrName);
    }
    if (IsSpecialization())
    {
        Cardinality specializationCount = Cardinality(specialization.size());
        writer.GetBinaryStreamWriter().Write(ToUnderlying(specializationCount));
        for (const auto* specializationType : specialization)
        {
            writer.GetBinaryStreamWriter().Write(ToUnderlying(specializationType->Id()));
        }
    }
    if (IsConversion())
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(ConversionKind()));
        writer.GetBinaryStreamWriter().Write(ToUnderlying(ConversionParamType()->Id()));
        writer.GetBinaryStreamWriter().Write(ToUnderlying(ConversionArgType()->Id()));
        writer.GetBinaryStreamWriter().Write(ConversionDistance());
    }
    writer.GetBinaryStreamWriter().Write(vtabIndex);
    if (group)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(group->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
}

void FunctionSymbol::Read(Reader& reader)
{
    ContainerSymbol::Read(reader);
    flags = FunctionSymbolFlags(reader.CurrentReader().ReadUShort());
    qualifiers = FunctionQualifiers(reader.CurrentReader().ReadUShort());
    linkage = Linkage(reader.CurrentReader().ReadByte());
    index = reader.CurrentReader().ReadInt();
    functionKind = FunctionKind(reader.CurrentReader().ReadByte());
    Cardinality parameterCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(parameterCount); ++i)
    {
        SymbolId parameterId = SymbolId(reader.CurrentReader().ReadUInt());
        parameterIds.push_back(parameterId);
    }
    returnTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    if (ReturnsClass())
    {
        returnValueParamId = SymbolId(reader.CurrentReader().ReadUInt());
    }
    if (GetFlag(FunctionSymbolFlags::fixedIrName))
    {
        fixedIrName = reader.CurrentReader().ReadString();
    }
    if (IsSpecialization())
    {
        Cardinality specializationCount = Cardinality(reader.CurrentReader().ReadUInt());
        for (Index i = Index(0); i < Index(specializationCount); ++i)
        {
            SymbolId specilizationId = SymbolId(reader.CurrentReader().ReadUInt());
            specializationIds.push_back(specilizationId);
        }
    }
    if (IsConversion())
    {
        conversionKind = ConversionKind(reader.CurrentReader().ReadByte());
        conversionParamTypeId = SymbolId(reader.CurrentReader().ReadUInt());
        conversionArgTypeId = SymbolId(reader.CurrentReader().ReadUInt());
        conversionDistance = reader.CurrentReader().ReadInt();
    }
    vtabIndex = reader.CurrentReader().ReadInt();
    groupId = SymbolId(reader.CurrentReader().ReadUInt());
}

void FunctionSymbol::SetFixedIrName(const std::string& fixedIrName_)
{
    fixedIrName = fixedIrName_;
    SetFlag(FunctionSymbolFlags::fixedIrName);
}

std::string FunctionSymbol::FixedIrName() const
{
    return fixedIrName;
}

std::string FunctionSymbol::IrName(Context* context) const
{
    if (GetFlag(FunctionSymbolFlags::fixedIrName) && !fixedIrName.empty())
    {
        return FixedIrName();
    }
    if (linkage == Linkage::cpp_linkage)
    {
        std::string irName;
        SpecialFunctionKind specialFunctionKind = GetSpecialFunctionKind(context);
        std::string digestSource;
        if (specialFunctionKind != SpecialFunctionKind::none)
        {
            irName.append(SpecialFunctionKindPrefix(specialFunctionKind));
            ClassTypeSymbol* classType = ParentClassType(context);
            digestSource.append(classType->IrName(context));
        }
        else
        {
            std::string operatorFunctionPrefix = GetOperatorFunctionPrefix(Name());
            if (!operatorFunctionPrefix.empty())
            {
                irName.append(operatorFunctionPrefix);
                ClassTypeSymbol* classType = ParentClassType(context);
                if (classType)
                {
                    digestSource.append(classType->IrName(context));
                    irName.append("_").append(classType->GroupName(context));
                }
            }
            else
            {
                ClassTypeSymbol* classType = ParentClassType(context);
                if (functionKind == FunctionKind::constructor && classType)
                {
                    irName.append("ctor_").append(classType->GroupName(context));
                    digestSource.append(classType->IrName(context));
                }
                else if (functionKind == FunctionKind::destructor && classType)
                {
                    irName.append("dtor_").append(classType->GroupName(context));
                    digestSource.append(classType->IrName(context));
                }
                else if (classType)
                {
                    irName.append("mfn_").append(classType->GroupName(context)).append("_").append(Name());
                }
                else
                {
                    irName.append("fn_" + Name());
                }
            }
        }
        std::string fullName = FullName(context);
        if (!specialization.empty())
        {
            int n = specialization.size();
            for (int i = 0; i < n; ++i)
            {
                digestSource.append(".").append(specialization[i]->FullName(context));
            }
        }
        digestSource.append(fullName);
        digestSource.append(compileUnitId);
        if (returnType)
        {
            digestSource.append(".").append(returnType->FullName(context));
        }
        irName.append("_").append(util::GetSha1MessageDigest(digestSource));
        if (GetFlag(FunctionSymbolFlags::fixedIrName))
        {
            fixedIrName = irName;
        }
        return irName;
    }
    else if (linkage == Linkage::c_linkage)
    {
        return Name();
    }
    else
    {
        return Name();
    }
}

void FunctionSymbol::SetCompileUnitId(const std::string& compileUnitId_)
{
    compileUnitId = compileUnitId_;
}

otava::intermediate::Type* FunctionSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) const
{
    SymbolId irId = IrId();
    std::string fname = FullName(context);
    otava::intermediate::Type* type = emitter.GetType(irId);
    if (!type)
    {
        otava::intermediate::Type* returnIrType = nullptr;
        if (returnType && !ReturnsClass())
        {
            returnIrType = returnType->DirectType(context)->FinalType(fullSpan, context)->IrType(emitter, fullSpan, context);
        }
        else
        {
            returnIrType = emitter.GetVoidType();
        }
        std::vector<otava::intermediate::Type*> paramIrTypes;
        for (ParameterSymbol* param : MemFnParameters(context))
        {
            otava::intermediate::Type* paramIrType = nullptr;
            TypeSymbol* paramType = param->GetReferredType(context);
            if (paramType->IsClassTypeSymbol())
            {
                paramIrType = paramType->AddConst(context)->AddLValueRef(context)->IrType(emitter, fullSpan, context);
            }
            else
            {
                paramIrType = paramType->IrType(emitter, fullSpan, context);
            }
            paramIrTypes.push_back(paramIrType);
        }
        if (ReturnsClass())
        {
            paramIrTypes.push_back(ReturnValueParam(context)->GetReferredType(context)->IrType(emitter, fullSpan, context));
        }
        type = emitter.MakeFunctionType(returnIrType, paramIrTypes);
        emitter.SetType(irId, type);
    }
    return type;
}

FunctionTypeSymbol* FunctionSymbol::GetFunctionType(otava::symbols::Context* context)
{
    FunctionTypeSymbol* functionType = context->GetSymbolTable()->MakeFunctionTypeSymbol(this, context);
    return functionType;
}

void FunctionSymbol::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    if (IsTrivialDestructor()) return;
    if ((flags & OperationFlags::virtualCall) != OperationFlags::none)
    {
        GenerateVirtualFunctionCall(emitter, args, fullSpan, context);
        return;
    }
    int n = args.size();
    for (int i = 0; i < n; ++i)
    {
        BoundExpressionNode* arg = args[i];
        arg->Load(emitter, OperationFlags::none, fullSpan, context);
    }
    std::vector<otava::intermediate::Value*> arguments;
    arguments.resize(n);
    for (int i = 0; i < n; ++i)
    {
        otava::intermediate::Value* arg = emitter.Stack().Pop();
        arguments[n - i - 1] = arg;
    }
    otava::intermediate::Type* type = IrType(emitter, fullSpan, context);
    if (type->IsFunctionType())
    {
        otava::intermediate::FunctionType* functionType = static_cast<otava::intermediate::FunctionType*>(type);
        std::string irName = IrName(context);
        otava::intermediate::Function* function = emitter.GetOrInsertFunction(irName, functionType);
        if (!functionType->ReturnType() || functionType->ReturnType()->IsVoidType())
        {
            emitter.EmitCall(function, arguments);
        }
        else
        {
            emitter.Stack().Push(emitter.EmitCall(function, arguments));
        }
    }
    else
    {
        ThrowException("function type expected", fullSpan, context);
    }
}

void FunctionSymbol::GenerateVirtualFunctionCall(Emitter& emitter, std::vector<BoundExpressionNode*>& args, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context)
{
    TypeSymbol* thisPtrBaseType = args[0]->GetType()->GetBaseType(context)->DirectType(context)->FinalType(fullSpan, context);
    otava::intermediate::Type* irType = IrType(emitter, fullSpan, context);
    otava::intermediate::FunctionType* functionType = nullptr;
    if (irType->IsFunctionType())
    {
        functionType = static_cast<otava::intermediate::FunctionType*>(irType);
    }
    else
    {
        ThrowException("function type expected", fullSpan, context);
    }
    ClassTypeSymbol* classType = nullptr;
    if (thisPtrBaseType->IsClassTypeSymbol())
    {
        classType = static_cast<ClassTypeSymbol*>(thisPtrBaseType);
    }
    else
    {
        ThrowException("class type expected", fullSpan, context);
    }
    classType->MakeVTab(context, fullSpan);
    std::vector<ClassTypeSymbol*> vptrHolderClasses = classType->VPtrHolderClasses(context);
    if (vptrHolderClasses.empty())
    {
        ThrowException("no vptr holder classes for the class '" + classType->FullName(context) + "'", fullSpan, context);
    }
    ClassTypeSymbol* vptrHolderClass = vptrHolderClasses.front();
    int na = args.size();
    otava::intermediate::Value* callee = nullptr;
    for (int i = 0; i < na; ++i)
    {
        args[i]->Load(emitter, OperationFlags::none, fullSpan, context);
        if (i == 0)
        {
            otava::intermediate::Value* thisPtr = emitter.Stack().Pop();
            if (classType != vptrHolderClass)
            {
                thisPtr = emitter.EmitBitcast(thisPtr, vptrHolderClass->AddPointer(context)->IrType(emitter, fullSpan, context));
            }
            otava::intermediate::Value* vptrPtr = emitter.EmitElemAddr(thisPtr, emitter.EmitLong(vptrHolderClass->VPtrIndex()));
            otava::intermediate::Value* voidVPtr = emitter.EmitLoad(vptrPtr);
            otava::intermediate::Value* vptr = emitter.EmitBitcast(voidVPtr, classType->VPtrType(emitter));
            otava::intermediate::Value* objectDeltaPtrElem = emitter.EmitElemAddr(thisPtr, emitter.EmitLong(vptrHolderClass->DeltaIndex()));
            otava::intermediate::Type* deltaPtrType = emitter.MakePtrType(emitter.GetLongType());
            otava::intermediate::Value* objectDeltaPtr = emitter.EmitBitcast(objectDeltaPtrElem, deltaPtrType);
            otava::intermediate::Value* objectDelta = emitter.EmitLoad(objectDeltaPtr);
            otava::intermediate::Value* adjustedObjectPtr = emitter.EmitClassPtrConversion(thisPtr, objectDelta, thisPtr->GetType(), false);
            if (VTabIndex() == -1)
            {
                ThrowException("invalid vtab index", fullSpan, context);
            }
            otava::intermediate::Value* functionPtrPtr = emitter.EmitElemAddr(vptr, emitter.EmitLong(vtabClassIdElementCount + 2 * VTabIndex()));
            otava::intermediate::Value* voidFunctionPtr = emitter.EmitLoad(functionPtrPtr);
            callee = emitter.EmitBitcast(voidFunctionPtr, emitter.MakePtrType(functionType));
            otava::intermediate::Value* deltaPtrElem = emitter.EmitElemAddr(vptr, emitter.EmitLong(vtabClassIdElementCount + 2 * VTabIndex() + 1));
            otava::intermediate::Value* deltaPtr = emitter.EmitBitcast(deltaPtrElem, deltaPtrType);
            otava::intermediate::Value* delta = emitter.EmitLoad(deltaPtr);
            otava::intermediate::Value* adjustedThisPtr = emitter.EmitClassPtrConversion(adjustedObjectPtr, delta, thisPtr->GetType(), false);
            emitter.Stack().Push(adjustedThisPtr);
        }
    }
    std::vector<otava::intermediate::Value*> arguments;
    arguments.resize(na);
    for (int i = 0; i < na; ++i)
    {
        otava::intermediate::Value* arg = emitter.Stack().Pop();
        arguments[na - i - 1] = arg;
    }
    if (!functionType->ReturnType() || functionType->ReturnType()->IsVoidType())
    {
        emitter.EmitCall(callee, arguments);
    }
    else
    {
        emitter.Stack().Push(emitter.EmitCall(callee, arguments));
    }
    context->GetBoundCompileUnit()->AddBoundNodeForClass(classType, fullSpan, context); 
}

std::string FunctionSymbol::NextTemporaryName()
{
    return "@t" + std::to_string(nextTemporaryId++);
}

VariableSymbol* FunctionSymbol::CreateTemporary(TypeSymbol* type, std::int64_t nodeId, Context* context)
{
    VariableSymbol* temporary = new VariableSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::variableSymbol), NextTemporaryName());
    temporary->SetTemporary();
    temporary->SetDeclaredType(type, context);
    AddLocalVariable(temporary, context);
    if (nodeId != -1)
    {
        temporaryMap[nodeId] = temporary;
        temporary->SetNodeId(nodeId);
    }
    if (context->CurrentProject()->HasDefine("PRINT_TEMPORARIES"))
    {
        std::cout << "CREATE TEMPORARY:" << temporary->Name() << ":" << type->FullName(context) << ":" << nodeId << "\n";
    }
    return temporary;
}

VariableSymbol* FunctionSymbol::GetTemporary(std::int64_t nodeId) const noexcept
{
    auto it = temporaryMap.find(nodeId);
    if (it != temporaryMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void FunctionSymbol::AddLocalVariable(VariableSymbol* localVariable, Context* context)
{
    if (std::find(localVariables.begin(), localVariables.end(), localVariable) != localVariables.end()) return;
    if (!localVariable->Parent(context))
    {
        localVariable->SetParent(this);
    }
    localVariables.push_back(localVariable);
}

void FunctionSymbol::CheckGenerateClassCopyCtor(const soul::ast::FullSpan& fullSpan, Context* context)
{
    for (ParameterSymbol* parameter : MemFnParameters(context))
    {
        TypeSymbol* paramType = parameter->GetReferredType(context);
        if (paramType->IsClassTypeSymbol())
        {
            ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(paramType);
            classType->GenerateCopyCtor(fullSpan, context);
        }
    }
}

ClassParsingMap* FunctionSymbol::GetClassParsingMap() const noexcept
{
    return classParsingMap;
}

void FunctionSymbol::SetClassParsingMap(ClassParsingMap* classParsingMap_) noexcept
{
    classParsingMap = classParsingMap_;
}

void FunctionSymbol::AddClass(ClassTypeSymbol* cls)
{
    if (std::find(classes.begin(), classes.end(), cls) == classes.end())
    {
        classes.push_back(cls);
    }
}

void FunctionSymbol::RemoveClass(ClassTypeSymbol* cls)
{
    if (!destructing)
    {
        classes.erase(std::remove(classes.begin(), classes.end(), cls), classes.end());
    }
}

FunctionDefinitionSymbol::FunctionDefinitionSymbol(Module* module_, SymbolId id_) : 
    FunctionSymbol(module_, id_), declaration(nullptr), parentFn(nullptr), parentFnScope(nullptr), defIndex(-1), containsGotosOrLabels(false),
    declarationId(zeroSymbolId), parentFnId(zeroSymbolId)
{
}

FunctionDefinitionSymbol::FunctionDefinitionSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    FunctionSymbol(module_, id_, name_), declaration(nullptr), parentFn(nullptr), parentFnScope(nullptr), defIndex(-1), containsGotosOrLabels(false),
    declarationId(zeroSymbolId), parentFnId(zeroSymbolId)
{
}

FunctionDefinitionSymbol::~FunctionDefinitionSymbol()
{
    for (ClassTypeSymbol* cls : Classes())
    {
        cls->ResetMemFnDefSymbol(this);
    }
}

bool FunctionDefinitionSymbol::IsMemberFunction(Context* context) const noexcept
{
    if (declaration)
    {
        return declaration->IsMemberFunction(context);
    }
    return false;
}

FunctionKind FunctionDefinitionSymbol::GetFunctionKind() const noexcept
{
    if (declaration)
    {
        return declaration->GetFunctionKind();
    }
    else
    {
        return FunctionSymbol::GetFunctionKind();
    }
}

bool FunctionDefinitionSymbol::IsVirtual() const noexcept
{
    if (declaration)
    {
        return declaration->IsVirtual();
    }
    else
    {
        return FunctionSymbol::IsVirtual();
    }
}

bool FunctionDefinitionSymbol::IsConst() const noexcept
{
    if (declaration)
    {
        return declaration->IsConst();
    }
    else
    {
        return FunctionSymbol::IsConst();
    }
}

bool FunctionDefinitionSymbol::IsPure() const noexcept
{
    if (declaration)
    {
        return declaration->IsPure();
    }
    else
    {
        return FunctionSymbol::IsPure();
    }
}

bool FunctionDefinitionSymbol::IsOverride() const noexcept
{
    if (declaration)
    {
        return declaration->IsOverride();
    }
    else
    {
        return FunctionSymbol::IsOverride();
    }
}

bool FunctionDefinitionSymbol::IsFinal() const noexcept
{
    if (declaration)
    {
        return declaration->IsFinal();
    }
    else
    {
        return FunctionSymbol::IsFinal();
    }
}

bool FunctionDefinitionSymbol::IsNoExcept() const noexcept
{
    if (declaration)
    {
        return declaration->IsNoExcept();
    }
    else
    {
        return FunctionSymbol::IsNoExcept();
    }
}

void FunctionDefinitionSymbol::SetNoExcept() noexcept
{
    if (declaration)
    {
        declaration->SetNoExcept();
    }
    FunctionSymbol::SetNoExcept();
}

void FunctionDefinitionSymbol::SetOverride() noexcept
{
    if (declaration)
    {
        declaration->SetOverride();
    }
    FunctionSymbol::SetOverride();
}

void FunctionDefinitionSymbol::SetFinal() noexcept
{
    if (declaration)
    {
        declaration->SetFinal();
    }
    FunctionSymbol::SetFinal();
}

bool FunctionDefinitionSymbol::IsInline() const noexcept
{
    if (declaration)
    {
        return declaration->IsInline();
    }
    return FunctionSymbol::IsInline();
}
void FunctionDefinitionSymbol::SetInline() noexcept
{
    if (declaration)
    {
        declaration->SetInline();
    }
    FunctionSymbol::SetInline();
}

bool FunctionDefinitionSymbol::IsUnparsed() const noexcept
{
    if (declaration)
    {
        return declaration->IsUnparsed();
    }
    return FunctionSymbol::IsUnparsed();
}

void FunctionDefinitionSymbol::SetUnparsed() noexcept
{
    if (declaration)
    {
        declaration->SetUnparsed();
    }
    FunctionSymbol::SetUnparsed();
}

void FunctionDefinitionSymbol::ResetUnparsed() noexcept
{
    if (declaration)
    {
        declaration->ResetUnparsed();
    }
    FunctionSymbol::ResetUnparsed();
}

bool FunctionDefinitionSymbol::Parsing() const noexcept
{
    if (declaration)
    {
        return declaration->Parsing();
    }
    return FunctionSymbol::Parsing();
}

void FunctionDefinitionSymbol::SetParsing() noexcept
{
    if (declaration)
    {
        declaration->SetParsing();
    }
    FunctionSymbol::SetParsing();
}

void FunctionDefinitionSymbol::ResetParsing() noexcept
{
    if (declaration)
    {
        declaration->ResetParsing();
    }
    FunctionSymbol::ResetParsing();
}

std::int32_t FunctionDefinitionSymbol::VTabIndex() const noexcept
{
    if (declaration)
    {
        return declaration->VTabIndex();
    }
    else
    {
        return FunctionSymbol::VTabIndex();
    }
}

bool FunctionDefinitionSymbol::IsStatic() const noexcept
{
    if (declaration)
    {
        return declaration->IsStatic();
    }
    else
    {
        return FunctionSymbol::IsStatic();
    }
}

bool FunctionDefinitionSymbol::IsExplicit() const noexcept
{
    if (declaration)
    {
        return declaration->IsExplicit();
    }
    else
    {
        return FunctionSymbol::IsExplicit();
    }
}

TypeSymbol* FunctionDefinitionSymbol::ConversionParamType() const noexcept
{
    if (declaration)
    {
        return declaration->ConversionParamType();
    }
    else
    {
        return FunctionSymbol::ConversionParamType();
    }
}

TypeSymbol* FunctionDefinitionSymbol::GetConversionParamType(Context* context) const 
{
    if (declaration)
    {
        return declaration->GetConversionParamType(context);
    }
    else
    {
        return FunctionSymbol::GetConversionParamType(context);
    }
}

TypeSymbol* FunctionDefinitionSymbol::ConversionArgType() const noexcept
{
    if (declaration)
    {
        return declaration->ConversionArgType();
    }
    else
    {
        return FunctionSymbol::ConversionArgType();
    }
}

TypeSymbol* FunctionDefinitionSymbol::GetConversionArgType(Context* context) const
{
    if (declaration)
    {
        return declaration->GetConversionArgType(context);
    }
    else
    {
        return FunctionSymbol::GetConversionArgType(context);
    }
}

std::int32_t FunctionDefinitionSymbol::ConversionDistance() const noexcept
{
    if (declaration)
    {
        return declaration->ConversionDistance();
    }
    else
    {
        return FunctionSymbol::ConversionDistance();
    }
}

void FunctionDefinitionSymbol::AddDefinitionToGroup(Context* context)
{
    Group(context)->AddFunctionDefinition(this, context);
}

void FunctionDefinitionSymbol::MapBlock(int blockId, Symbol* block)
{
    blockMap[blockId] = block;
}

Symbol* FunctionDefinitionSymbol::GetBlock(int blockId) const noexcept
{
    auto it = blockMap.find(blockId);
    if (it != blockMap.end())
    {
        return it->second;
    }
    return nullptr;
}

TypeSymbol* FunctionDefinitionSymbol::NonChildFunctionResultType(Context* context) const noexcept
{
    if (ParentFn())
    {
        return ParentFn()->NonChildFunctionResultType(context);
    }
    else
    {
        TypeSymbol* voidType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::voidType, context);
        if (ReturnType(context) && !TypesEqual(ReturnType(context), voidType, context))
        {
            return ReturnType(context);
        }
        else
        {
            return nullptr;
        }
    }
}

std::string FunctionDefinitionSymbol::ResultVarExprStr(TypeSymbol* resultType) const
{
    if (resultType->IsReferenceType())
    {
        return "*" + resultVarName;
    }
    return resultVarName;
}

ClassParsingMap* FunctionDefinitionSymbol::GetClassParsingMap() const noexcept
{
    if (declaration)
    {
        return declaration->GetClassParsingMap();
    }
    return FunctionSymbol::GetClassParsingMap();
}

void FunctionDefinitionSymbol::SetClassParsingMap(ClassParsingMap* classParsingMap_) noexcept
{
    if (declaration)
    {
        declaration->SetClassParsingMap(classParsingMap_);
    }
    FunctionSymbol::SetClassParsingMap(classParsingMap_);
}

void FunctionDefinitionSymbol::SetResultVarName(const std::string& resultVarName_)
{
    resultVarName = resultVarName_;
}

void FunctionDefinitionSymbol::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    if (declaration)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(declaration->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    if (parentFn)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(parentFn->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    writer.GetBinaryStreamWriter().Write(defIndex);
}

void FunctionDefinitionSymbol::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    declarationId = SymbolId(reader.CurrentReader().ReadUInt()); 
    parentFnId = SymbolId(reader.CurrentReader().ReadUInt());
    defIndex = reader.CurrentReader().ReadInt();
}

void FunctionDefinitionSymbol::SetDeclaration(FunctionSymbol* declaration_, Context* context) noexcept
{
    declaration = declaration_;
    if (declaration->GetModule() != GetModule())
    {
        context->GetModule()->GetSymbolTable()->AddImportedSymbol(declaration->Id(), declaration->GetModule()->Id());
    }
}

std::string FunctionDefinitionSymbol::IrName(Context* context) const
{
    if (declaration)
    {
        if (irName.empty())
        {
            irName = declaration->IrName(context);
        }
        return irName;
    }
    else
    {
        if (irName.empty())
        {
            irName = FunctionSymbol::IrName(context);
        }
        return irName;
    }
}

otava::intermediate::Type* FunctionDefinitionSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) const
{
    if (declaration)
    {
        return declaration->IrType(emitter, fullSpan, context);
    }
    else
    {
        return FunctionSymbol::IrType(emitter, fullSpan, context);
    }
}

void FunctionDefinitionSymbol::SetReturnType(TypeSymbol* returnType_, Context* context)
{
    if (declaration && !declaration->IsReadOnly())
    {
        declaration->SetReturnType(returnType_, context);
    }
    FunctionSymbol::SetReturnType(returnType_, context);
}

ExplicitlyInstantiatedFunctionDefinitionSymbol::ExplicitlyInstantiatedFunctionDefinitionSymbol(Module* module_, SymbolId id_) :
    FunctionDefinitionSymbol(module_, id_)
{
}

ExplicitlyInstantiatedFunctionDefinitionSymbol::ExplicitlyInstantiatedFunctionDefinitionSymbol(Module* module_, SymbolId id_, 
    FunctionDefinitionSymbol* functionDefinitionSymbol_,
    const soul::ast::FullSpan& fullSpan, Context* context) :
    FunctionDefinitionSymbol(module_, id_, functionDefinitionSymbol_->Name()),
    functionDefinitionSymbol(functionDefinitionSymbol_), irName(functionDefinitionSymbol->IrName(context))
{
    SetDeclarationFlags(functionDefinitionSymbol->GetDeclarationFlags());
    SetVTabIndex(functionDefinitionSymbol->VTabIndex());
    SetDefIndex(functionDefinitionSymbol->DefIndex());
    SetFunctionQualifiers(functionDefinitionSymbol->Qualifiers());
    for (ParameterSymbol* parameter : functionDefinitionSymbol->Parameters(context))
    {
        ParameterSymbol* p = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), parameter->Name());
        p->SetType(parameter->GetType(context), context);
        AddSymbol(p, fullSpan, context);
    }
    if (functionDefinitionSymbol->ReturnType(context))
    {
        SetReturnType(functionDefinitionSymbol->ReturnType(context), context);
    }
}

void ExplicitlyInstantiatedFunctionDefinitionSymbol::Write(Writer& writer)
{
    FunctionDefinitionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(irName);
    if (!functionDefinitionSymbol->IsGenerated())
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(functionDefinitionSymbol->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
}

void ExplicitlyInstantiatedFunctionDefinitionSymbol::Read(Reader& reader)
{
    FunctionDefinitionSymbol::Read(reader);
    irName = reader.CurrentReader().ReadString();
    functionDefinitionId = SymbolId(reader.CurrentReader().ReadUInt());
}

otava::intermediate::Type* ExplicitlyInstantiatedFunctionDefinitionSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan,
    otava::symbols::Context* context) const
{
    if (functionDefinitionSymbol)
    {
        return functionDefinitionSymbol->IrType(emitter, fullSpan, context);
    }
    else
    {
        return FunctionDefinitionSymbol::IrType(emitter, fullSpan, context);
    }
}

CompileUnitInitFn::CompileUnitInitFn(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_)
{
}

CompileUnitInitFn::CompileUnitInitFn(Module* module_, SymbolId id_, const std::string& name_) : FunctionSymbol(module_, id_, name_)
{
}

void CompileUnitInitFn::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    otava::intermediate::FunctionType* initFunctionType = static_cast<otava::intermediate::FunctionType*>(emitter.MakeFunctionType(emitter.GetVoidType(),
        std::vector<otava::intermediate::Type*>()));
    otava::intermediate::Function* initFn = emitter.GetOrInsertFunction(Name(), initFunctionType);
    emitter.EmitCall(initFn, std::vector<otava::intermediate::Value*>());
}

bool FunctionMatches(FunctionSymbol* left, FunctionSymbol* right, Context* context) noexcept
{
    if (left->GroupName() != right->GroupName()) return false;
    if (left->IsConst() != right->IsConst()) return false;
    if (left->IsTemplate(context) != right->IsTemplate(context)) return false;
    if (left->IsTemplate(context))
    {
        TemplateDeclarationSymbol* leftTemplateDeclaration = left->ParentTemplateDeclaration(context);
        TemplateDeclarationSymbol* rightTemplateDeclaration = right->ParentTemplateDeclaration(context);
        if (leftTemplateDeclaration->Arity() != rightTemplateDeclaration->Arity()) return false;
        for (Index i = Index(0); i < Index(leftTemplateDeclaration->Arity()); ++i)
        {
            if (!TypesEqual(leftTemplateDeclaration->TemplateParameters(context)[ToUnderlying(i)], 
                rightTemplateDeclaration->TemplateParameters(context)[ToUnderlying(i)], context)) return false;
        }
    }
    int leftsn = left->Specialization().size();
    int rightsn = right->Specialization().size();
    if (leftsn != rightsn) return false;
    for (int i = 0; i < leftsn; ++i)
    {
        if (!TypesEqual(left->Specialization()[i], right->Specialization()[i], context)) return false;
    }
    if (left->MemFnArity(context) != right->MemFnArity(context)) return false;
    Cardinality n = left->MemFnArity(context);
    for (Index i = Index(0); i < Index(n); ++i)
    {
        if (!TypesEqual(left->MemFnParameters(context)[ToUnderlying(i)]->GetType(context), 
            right->MemFnParameters(context)[ToUnderlying(i)]->GetType(context), context)) return false;
    }
    return true;
}

void PrintXml(FunctionSymbol* function, Context* context)
{
    std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement("function"));
    element->SetAttribute("kind", SymbolKindStr(function->Kind()));
    element->SetAttribute("name", function->FullName(context));
    element->SetAttribute("id", std::to_string(ToUnderlying(function->Id())));
    element->SetAttribute("qualifiers", MakeFunctionQualifierStr(function->Qualifiers()));
    util::CodeFormatter formatter(std::cout);
    element->Write(formatter);
}

} // namespace otava::symbols
