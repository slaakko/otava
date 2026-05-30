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
import otava.symbols.modules;
import otava.symbols.templates;
import otava.symbols.type_symbol;
import otava.symbols.variable_symbol;
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
    conversionParamTypeId(zeroSymbolId), conversionArgTypeId(zeroSymbolId), fixedIrNameOffset(notFoundOffset)
{
    GetScope()->SetKind(ScopeKind::functionScope);
}

FunctionSymbol::FunctionSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    ContainerSymbol(module_, id_, name_), flags(FunctionSymbolFlags::none), qualifiers(FunctionQualifiers::none),
    linkage(Linkage::cpp_linkage), group(nullptr), index(0), functionKind(FunctionKind::function), returnType(nullptr), returnTypeId(zeroSymbolId),
    conversionKind(ConversionKind::implicitConversion), conversionParamType(nullptr), conversionArgType(nullptr), conversionDistance(0),
    memFnParamsConstructed(false), vtabIndex(-1), returnValueParam(nullptr), returnValueParamId(zeroSymbolId), conversionParamTypeId(zeroSymbolId),
    conversionArgTypeId(zeroSymbolId), fixedIrNameOffset(notFoundOffset)
{
    GetScope()->SetKind(ScopeKind::functionScope);
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

void FunctionSymbol::SetConversionParamType(TypeSymbol* conversionParamType_) noexcept
{
    conversionParamType = conversionParamType_;
}

void FunctionSymbol::SetConversionArgType(TypeSymbol* conversionArgType_) noexcept
{
    conversionArgType = conversionArgType_;
}

void FunctionSymbol::SetConversionKind(ConversionKind conversionKind_) noexcept
{
    conversionKind = conversionKind_;
}

void FunctionSymbol::SetConversionDistance(std::int32_t conversionDistance_) noexcept
{
    conversionDistance = conversionDistance_;
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
}

const std::vector<ParameterSymbol*>& FunctionSymbol::Parameters(const soul::ast::FullSpan& fullSpan, Context* context) const
{
    if (IsReadOnly())
    {
        for (auto parameterId : parameterIds)
        {
            ParameterSymbol* parameterSymbol = GetModule()->GetSymbolTable()->GetParameterSymbol(parameterId, context);
            if (parameterSymbol)
            {
                parameters.push_back(parameterSymbol);
            }
            else
            {
                ThrowException("parameter symbol for id " + std::to_string(ToUnderlying(parameterId)) + " not found", fullSpan, context);
            }
        }
    }
    return parameters;
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
                tp->SetType(classType->AddConst(context)->AddPointer(context));
                thisParam.reset(tp);
            }
            else
            {
                ParameterSymbol* tp = new ParameterSymbol(GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
                tp->SetType(classType->AddPointer(context));
                thisParam.reset(tp);
            }
        }
    }
    return thisParam.get();
}

const std::vector<ParameterSymbol*>& FunctionSymbol::MemFnParameters(const soul::ast::FullSpan& fullSpan, Context* context) const
{
    if (memFnParamsConstructed) return memFnParameters;
    memFnParamsConstructed = true;
    ParameterSymbol* thisParam = ThisParam(context);
    if (thisParam)
    {
        memFnParameters.push_back(thisParam);
    }
    const std::vector<ParameterSymbol*>& params = Parameters(fullSpan, context);
    for (ParameterSymbol* parameter : params)
    {
        memFnParameters.push_back(parameter);
    }
    return memFnParameters;
}

SpecialFunctionKind FunctionSymbol::GetSpecialFunctionKind(Context* context) const noexcept
{
    // TODO
    return SpecialFunctionKind(SpecialFunctionKind::none);
}

TemplateDeclarationSymbol* FunctionSymbol::ParentTemplateDeclaration() const noexcept
{
    // TODO
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
        returnValueParam = context->GetSymbolTable()->GetParameterSymbol(returnValueParamId, context);
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
        returnType = context->GetSymbolTable()->GetTypeSymbol(returnTypeId, context);
    }
    return returnType;
}

void FunctionSymbol::SetReturnType(TypeSymbol* returnType_, const soul::ast::FullSpan& fullSpan, Context* context)
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
            AddSymbol(returnValueParam, fullSpan, context);
            returnValueParam->SetType(returnValueType);
            SetReturnValueParam(returnValueParam);
        }
    }
}

void FunctionSymbol::AddDefinitionToGroup(Context* context)
{
    // TODO
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
        Scope* classScope = scope->GetClassScope();
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

bool FunctionSymbol::IsTemplate() const noexcept
{
    return ParentTemplateDeclaration() != nullptr && !IsSpecialization();
}

int FunctionSymbol::TemplateArity() const noexcept
{
    TemplateDeclarationSymbol* templateDecl = ParentTemplateDeclaration();
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

bool FunctionSymbol::IsExplicitSpecializationDefinitionSymbol() const noexcept
{
    if (!IsFunctionDefinitionSymbol()) return false;
    TemplateDeclarationSymbol* templateDeclaration = ParentTemplateDeclaration();
    if (!templateDeclaration) return false;
    if (templateDeclaration->Arity() != 0) return false;
    return true;
}

int FunctionSymbol::Arity() const noexcept
{
    if (IsReadOnly())
    {
        return parameterIds.size();
    }
    return parameters.size();
}

int FunctionSymbol::MemFnArity(const soul::ast::FullSpan& fullSpan, Context* context) const
{
    return int(MemFnParameters(fullSpan, context).size());
}

int FunctionSymbol::MinArity(const soul::ast::FullSpan& fullSpan, Context* context) const 
{
    int minArity = Arity();
    const std::vector<ParameterSymbol*>& params = Parameters(fullSpan, context);
    for (int i = params.size() - 1; i >= 0; --i)
    {
        ParameterSymbol* parameter = params[i];
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

int FunctionSymbol::MinMemFnArity(const soul::ast::FullSpan& fullSpan, Context* context) const
{
    int minMemFnArity = MemFnArity(fullSpan, context);
    const std::vector<ParameterSymbol*>& params = Parameters(fullSpan, context);
    for (int i = params.size() - 1; i >= 0; --i)
    {
        ParameterSymbol* parameter = params[i];
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
        writer.GetBinaryStreamWriter().Write(ToUnderlying(fixedIrNameOffset));
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
    writer.GetBinaryStreamWriter().Write(ToUnderlying(group->Id()));
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
        fixedIrNameOffset = StringOffset(reader.CurrentReader().ReadUInt());
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
    fixedIrNameOffset = GetModule()->GetStringTable()->AddString(fixedIrName_);
    SetFlag(FunctionSymbolFlags::fixedIrName);
}

std::string FunctionSymbol::FixedIrName() const 
{
    return GetModule()->GetStringTable()->GetString(fixedIrNameOffset);
}

std::string FunctionSymbol::IrName(Context* context) const
{
    if (GetFlag(FunctionSymbolFlags::fixedIrName) && fixedIrNameOffset != notFoundOffset)
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
                    irName.append("_").append(classType->Group(context)->Name());
                }
            }
            else
            {
                ClassTypeSymbol* classType = ParentClassType(context);
                if (functionKind == FunctionKind::constructor && classType)
                {
                    irName.append("ctor_").append(classType->Group(context)->Name());
                    digestSource.append(classType->IrName(context));
                }
                else if (functionKind == FunctionKind::destructor && classType)
                {
                    irName.append("dtor_").append(classType->Group(context)->Name());
                    digestSource.append(classType->IrName(context));
                }
                else if (classType)
                {
                    irName.append("mfn_").append(classType->Group(context)->Name()).append("_").append(Name());
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
            fixedIrNameOffset = GetModule()->GetStringTable()->AddString(irName);
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

otava::intermediate::Type* FunctionSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) const
{
/*
    util::uuid irId = IrId(fullSpan, context);
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
        for (ParameterSymbol* param : MemFunParameters(context))
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
            paramIrTypes.push_back(ReturnValueParam()->GetReferredType(context)->IrType(emitter, fullSpan, context));
        }
        type = emitter.MakeFunctionType(returnIrType, paramIrTypes);
        emitter.SetType(irId, type);
    }
    return type;
*/
    return nullptr;
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
    TypeSymbol* thisPtrBaseType = args[0]->GetType()->GetBaseType()->DirectType(context)->FinalType(fullSpan, context);
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
    std::vector<ClassTypeSymbol*> vptrHolderClasses = classType->VPtrHolderClasses();
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
    // context->GetBoundCompileUnit()->AddBoundNodeForClass(classType, fullSpan, context); TODO
}

FunctionDefinitionSymbol::FunctionDefinitionSymbol(Module* module_, SymbolId id_) : 
    FunctionSymbol(module_, id_), declaration(nullptr), parentFn(nullptr), parentFnScope(nullptr), defIndex(-1)
{
}

FunctionDefinitionSymbol::FunctionDefinitionSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    FunctionSymbol(module_, id_, name_), declaration(nullptr), parentFn(nullptr), parentFnScope(nullptr), defIndex(-1)
{
}

void FunctionDefinitionSymbol::MapBlock(int blockId, Symbol* block)
{
    // TODO
    //blockMap[blockId] = block;

}

Symbol* FunctionDefinitionSymbol::GetBlock(int blockId) const noexcept
{
/*
    auto it = blockMap.find(blockId);
    if (it != blockMap.end())
    {
        return it->second;
    }
*/
    return nullptr;
}

} // namespace otava::symbols
