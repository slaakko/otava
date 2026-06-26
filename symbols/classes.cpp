// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.classes;

import otava.symbols.argument_conversion_table;
import otava.symbols.class_group_symbol;
import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.function_symbol;
import otava.symbols.modules;
import otava.symbols.overload_resolution;
import otava.symbols.project;
import otava.symbols.statement_binder;
import otava.symbols.templates;
import otava.symbols.type_compare;
import otava.symbols.type_resolver;
import otava.symbols.variable_symbol;
import otava.ast.visitor;
import otava.ast.identifier;
import otava.ast.templates;
import otava.ast.type;
import util.sha1;

namespace otava::symbols {

Symbol* GenerateDestructor(ClassTypeSymbol* classTypeSymbol, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);

std::int32_t GetSpecialFunctionIndex(SpecialFunctionKind specialFunctionKind) noexcept
{
    switch (specialFunctionKind)
    {
    case SpecialFunctionKind::defaultCtor:
    {
        return defaultCtorIndex;
    }
    case SpecialFunctionKind::copyCtor:
    {
        return copyCtorIndex;
    }
    case SpecialFunctionKind::moveCtor:
    {
        return moveCtorIndex;
    }
    case SpecialFunctionKind::copyAssignment:
    {
        return copyAssignmentIndex;
    }
    case SpecialFunctionKind::moveAssignment:
    {
        return moveAssignmentIndex;
    }
    case SpecialFunctionKind::dtor:
    {
        return destructorIndex;
    }
    }
    return 0;
}

RecordedParseCompoundStatementFn recordedParseCompoundStatementFn = nullptr;

void SetRecordedParseCompoundStatementFn(RecordedParseCompoundStatementFn fn) noexcept
{
    recordedParseCompoundStatementFn = fn;
}

void RecordedParseCompoundStatement(otava::ast::CompoundStatementNode* compoundStatementNode, Context* context)
{
    if (recordedParseCompoundStatementFn)
    {
        recordedParseCompoundStatementFn(compoundStatementNode, context);
    }
}

RecordedParseCtorInitializerFn recordedParseInitializerFn = nullptr;

void SetRecordedParseCtorInitializerFn(RecordedParseCtorInitializerFn fn) noexcept
{
    recordedParseInitializerFn = fn;
}

void RecordedParseCtorInitializer(otava::ast::ConstructorInitializerNode* ctorInitializerNode, Context* context)
{
    recordedParseInitializerFn(ctorInitializerNode, context);
}

ClassTypeSymbol::ClassTypeSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), flags(ClassTypeSymbolFlags::none), classKind(ClassKind::class_), level(0), group(nullptr), 
    groupId(zeroSymbolId), vptrIndex(-1), deltaIndex(-1), currentFunctionIndex(1), specialization(nullptr), specializationId(zeroSymbolId),
    nextMemFnDefIndex(0), copyCtor(nullptr), contentFetched(false), destructing(false), vtabNameOffset(notFoundOffset)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

ClassTypeSymbol::ClassTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), flags(ClassTypeSymbolFlags::none), classKind(ClassKind::class_), level(0), group(nullptr), 
    groupId(zeroSymbolId), vptrIndex(-1), deltaIndex(-1), currentFunctionIndex(1), specialization(nullptr), specializationId(zeroSymbolId),
    nextMemFnDefIndex(0), copyCtor(nullptr), contentFetched(false), destructing(false), vtabNameOffset(notFoundOffset)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

ClassTypeSymbol::~ClassTypeSymbol()
{
    destructing = true;
    std::vector<FunctionSymbol*> fns;
    for (const auto& f : functionIndexMap)
    {
        FunctionSymbol* fn = f.second;
        if (fn)
        {
            fns.push_back(fn);
        }
    }
    functionIndexMap.clear();
    for (FunctionSymbol* fn : fns)
    {
        fn->SetIndex(undefinedIndex);
        fn->RemoveClass(this);
    }
}

bool ClassTypeSymbol::IsTemplate(Context* context) const noexcept
{
    return ParentTemplateDeclaration(context) != nullptr;
}

bool ClassTypeSymbol::HasPolymorphicBaseClass(Context* context) const noexcept
{
    for (ClassTypeSymbol* baseCls : BaseClasses(context))
    {
        if (baseCls->IsPolymorphic(context))
        {
            return true;
        }
    }
    return false;
}

bool ClassTypeSymbol::IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const
{
    const Symbol* thisSymbol = this;
    if (visited.find(thisSymbol) == visited.end())
    {
        visited.insert(thisSymbol);
        for (VariableSymbol* memberVariable : MemberVariables(context))
        {
            if (memberVariable->IsTemplateParameterInstantiation(context, visited)) return true;
        }
        for (VariableSymbol* staticMemberVar : StaticMemberVariables(context))
        {
            if (staticMemberVar->IsTemplateParameterInstantiation(context, visited)) return true;
        }
    }
    return false;
}

bool ClassTypeSymbol::HasBaseClass(TypeSymbol* baseClass, int& distance, Context* context) const noexcept
{
    for (ClassTypeSymbol* baseCls : BaseClasses(context))
    {
        if (TypesEqual(baseCls, baseClass, context))
        {
            ++distance;
            return true;
        }
        else
        {
            ++distance;
            if (baseCls->HasBaseClass(baseClass, distance, context))
            {
                return true;
            }
            --distance;
        }
    }
    return false;
}

ClassGroupSymbol* ClassTypeSymbol::Group(Context* context) const
{
    if (group)
    {
        return group;
    }
    if (IsReadOnly() && groupId != zeroSymbolId)
    {
        group = GetModule()->GetSymbolTable()->GetClassGroupSymbol(groupId, context);
    }
    return group;
}

Cardinality ClassTypeSymbol::Arity(Context* context) noexcept
{
    TemplateDeclarationSymbol* templateDeclaration = ParentTemplateDeclaration(context);
    if (templateDeclaration)
    {
        return templateDeclaration->Arity();
    }
    else
    {
        return Cardinality(0);
    }
}

bool ClassTypeSymbol::IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType, Context* context) const 
{
    const TypeSymbol* thisSymbol = this;
    if (visited.find(thisSymbol) != visited.end()) return true;
    visited.insert(thisSymbol);
    for (ClassTypeSymbol* baseClass : baseClasses)
    {
        if (!baseClass->IsComplete(visited, incompleteType, context)) return false;
    }
    for (VariableSymbol* memberVariable : memberVariables)
    {
        if (!memberVariable->GetType(context)->IsComplete(visited, incompleteType, context)) return false;
    }
    return true;
}

TypeSymbol* ClassTypeSymbol::Specialization(Context* context) 
{
    if (specialization)
    {
        return specialization;
    }
    if (IsReadOnly() && specializationId != zeroSymbolId)
    {
        specialization = GetModule()->GetSymbolTable()->GetTypeSymbol(specializationId, context);
        if (!specialization)
        {
            ThrowException("specialization id " + std::to_string(ToUnderlying(specializationId)) + " not found from class '" + FullName(context) + "'");
        }
    }
    return specialization;
}

void ClassTypeSymbol::SetSpecialization(TypeSymbol* specialization_, Context* context) noexcept
{
    specialization = specialization_;
/*
    if (specialization)
    {
        context->GetSymbolTable()->MapType(specialization);
        if (specialization->IsClassTemplateSpecializationSymbol())
        {
            context->GetSymbolTable()->MapClassTemplateSpecialization(static_cast<ClassTemplateSpecializationSymbol*>(specialization));
        }
    }
*/
}

bool ClassTypeSymbol::IsPolymorphic(Context* context) const noexcept
{
    for (ClassTypeSymbol* baseClass : BaseClasses(context))
    {
        if (baseClass->IsPolymorphic(context)) return true;
    }
    for (FunctionSymbol* memberFunction : MemberFunctions(context))
    {
        if (memberFunction->IsVirtual()) return true;
    }
    return false;
}

void ClassTypeSymbol::MakeObjectLayout(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (ObjectLayoutComputed())
    {
        for (VariableSymbol* memberVar : MemberVariables(context))
        {
            if (memberVar->LayoutIndex() == -1)
            {
                ResetObjectLayoutComputed();
                break;
            }
        }
    }
    if (ObjectLayoutComputed()) return;
    SetObjectLayoutComputed();
    objectLayout.clear();
    for (ClassTypeSymbol* baseClass : BaseClasses(context))
    {
        baseClass->MakeObjectLayout(fullSpan, context);
        objectLayout.push_back(baseClass);
    }
    if (BaseClasses(context).empty())
    {
        if (IsPolymorphic(context))
        {
            SetVPtrIndex(objectLayout.size());
            objectLayout.push_back(
                context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::voidType, context)->AddPointer(context));
            SetDeltaIndex(objectLayout.size());
            objectLayout.push_back(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::longLongIntType, context));
        }
        else if (memberVariables.empty())
        {
            objectLayout.push_back(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::unsignedCharType, context));
        }
    }
    for (VariableSymbol* memberVar : MemberVariables(context))
    {
        std::int32_t layoutIndex = objectLayout.size();
        memberVar->SetLayoutIndex(layoutIndex);
        TypeSymbol* memberVarType = memberVar->GetType(context)->FinalType(fullSpan, context);
        memberVar->SetDeclaredType(memberVarType, context);
        if (memberVarType->IsForwardClassDeclarationSymbol())
        {
            ThrowException("could not make object layout: incomplete types not allowed", fullSpan, context);
        }
        objectLayout.push_back(memberVarType);
    }
}

class TrivialClassDtor : public FunctionSymbol
{
public:
    TrivialClassDtor(Module* module_, SymbolId id_);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
};

TrivialClassDtor::TrivialClassDtor(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_, "@destructor")
{
    SetFunctionKind(FunctionKind::destructor);
    SetAccess(Access::public_);
    SetFlag(FunctionSymbolFlags::trivialDestructor);
    SetNoExcept();
}

void TrivialClassDtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
}

Cardinality ClassTypeSymbol::TotalMemberCount() const noexcept
{
    Cardinality totalMemberCount = Cardinality(0);
    for (ClassTypeSymbol* baseClass : baseClasses)
    {
        totalMemberCount += baseClass->TotalMemberCount();
    }
    totalMemberCount += Cardinality(memberVariables.size());
    return totalMemberCount;
}

TemplateDeclarationSymbol* ClassTypeSymbol::ParentTemplateDeclaration(Context* context) const noexcept
{
    Symbol* parentSymbol = Parent(context);
    if (parentSymbol && parentSymbol->IsTemplateDeclarationSymbol())
    {
        return static_cast<TemplateDeclarationSymbol*>(parentSymbol);
    }
    return nullptr;
}

std::int32_t ClassTypeSymbol::NextFunctionIndex() noexcept
{
    return currentFunctionIndex++;
}

void ClassTypeSymbol::MapFunction(FunctionSymbol* function)
{
    functionIndexMap[function->GetIndex()] = function;
    function->AddClass(this);
}

void ClassTypeSymbol::UnmapFunction(FunctionSymbol* function)
{
    if (destructing) return;
    std::int32_t index = function->GetIndex();
    if (index != undefinedIndex)
    {
        functionIndexMap.erase(index);
        function->RemoveClass(this);
    }
}

void ClassTypeSymbol::SetMemFnDefSymbol(FunctionDefinitionSymbol* memFnDefSymbol)
{
    if (memFnDefSymbol->DefIndex() == -1)
    {
        memFnDefSymbol->SetDefIndex(nextMemFnDefIndex++);
    }
    memFnDefSymbolMap[memFnDefSymbol->DefIndex()] = memFnDefSymbol;
    nextMemFnDefIndex = std::max(nextMemFnDefIndex, memFnDefSymbol->DefIndex() + 1);
}

FunctionDefinitionSymbol* ClassTypeSymbol::GetMemFnDefSymbol(int32_t defIndex) const noexcept
{
    auto it = memFnDefSymbolMap.find(defIndex);
    if (it != memFnDefSymbolMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

FunctionSymbol* ClassTypeSymbol::GetFunctionByIndex(std::int32_t functionIndex) const noexcept
{
    auto it = functionIndexMap.find(functionIndex);
    if (it != functionIndexMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

otava::intermediate::Type* ClassTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    SymbolId id = Id();
    otava::intermediate::Type* irType = emitter.GetType(id);
    if (!irType)
    {
        irType = emitter.GetOrInsertFwdDeclaredStructureType(id, FullName(context));
        emitter.SetType(id, irType);
        MakeObjectLayout(fullSpan, context);
        int n = objectLayout.size();
        std::vector<otava::intermediate::Type*> elementTypes;
        for (int i = 0; i < n; ++i)
        {
            TypeSymbol* type = objectLayout[i];
            elementTypes.push_back(type->IrType(emitter, fullSpan, context));
        }
        otava::intermediate::MetadataStruct* metadataStruct = emitter.CreateMetadataStruct();
        metadataStruct->AddItem("fullName", emitter.CreateMetadataString(FullName(context)));
        otava::intermediate::MetadataRef* metadataRef = emitter.CreateMetadataRef(metadataStruct->Id());
        otava::intermediate::Type* type = emitter.MakeStructureType(elementTypes, FullName(context));
        otava::intermediate::StructureType* structureType = static_cast<otava::intermediate::StructureType*>(type);
        structureType->SetMetadataRef(metadataRef);
        irType = type;
        emitter.SetType(id, irType);
        emitter.ResolveForwardReferences(id, structureType);
    }
    return irType;
}

void ClassTypeSymbol::MakeVTab(Context* context, const soul::ast::FullSpan& fullSpan)
{
    if (!IsClassTemplateSpecializationSymbol())
    {
        if (VTabInitialized()) return;
        SetVTabInitialized();
    }
    ComputeVTabName(context);
    InitVTab(vtab, context, fullSpan, IsClassTemplateSpecializationSymbol());
}

bool Overrides(FunctionSymbol* f, FunctionSymbol* g, Context* context) noexcept
{
    if (f->GroupName() == g->GroupName())
    {
        int n = f->Parameters(context).size();
        if (n == g->Parameters(context).size())
        {
            for (int i = 0; i < n; ++i)
            {
                ParameterSymbol* p = f->Parameters(context)[i];
                ParameterSymbol* q = g->Parameters(context)[i];
                context->PushSetFlag(ContextFlags::matchClassGroup);
                if (!TypesEqual(p->GetType(context), q->GetType(context), context))
                {
                    context->PopFlags();
                    return false;
                }
                context->PopFlags();
            }
            if (f->IsConst() != g->IsConst()) return false;
            return true;
        }
    }
    return false;
}

void ClassTypeSymbol::InitVTab(std::vector<FunctionSymbol*>& vtab, Context* context, const soul::ast::FullSpan& fullSpan, bool clear)
{
    if (!IsPolymorphic(context)) return;
    if (clear)
    {
        vtab.clear();
    }
    std::string fname = FullName(context);
    const std::vector<ClassTypeSymbol*> baseClasses = BaseClasses(context);
    if (!baseClasses.empty())
    {
        for (ClassTypeSymbol* baseClass : baseClasses)
        {
            std::string bname = baseClass->FullName(context);
            baseClass->InitVTab(vtab, context, fullSpan, false);
        }
    }
    std::vector<FunctionSymbol*> fns;
    for (FunctionSymbol* function : MemberFunctions(context))
    {
        FunctionSymbol* fn = function;
        if (function->IsFunctionDefinitionSymbol())
        {
            FunctionDefinitionSymbol* definition = static_cast<FunctionDefinitionSymbol*>(function);
            FunctionSymbol* declaration = definition->Declaration();
            if (declaration)
            {
                fn = declaration;
            }
        }
        if (!fn->IsTrivialDestructor())
        {
            bool found = false;
            for (FunctionSymbol* existing : fns)
            {
                if (FunctionMatches(fn, existing, context))
                {
                    if (existing->VTabIndex() != -1)
                    {
                        fn->SetVTabIndex(existing->VTabIndex());
                    }
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                fns.push_back(fn);
            }
        }
    }
    int n = fns.size();
    for (int i = 0; i < n; ++i)
    {
        FunctionSymbol* f = fns[i];
        bool found = false;
        int m = vtab.size();
        for (int j = 0; j < m; ++j)
        {
            FunctionSymbol* v = vtab[j];
            if (Overrides(f, v, context))
            {
                if (v->IsFinal())
                {
                    ThrowException("function (" + f->FullName(context) + ") cannot override a final function (" +
                        v->FullName(context) + ")", fullSpan, context);
                }
                if (!f->IsOverride() && !f->IsFinal() && !f->IsDestructor())
                {
                    ThrowException("overriding function should be declared with override or final specifier: (" +
                        f->FullName(context) + " overrides " + v->FullName(context) + ")", fullSpan, context);
                }
                TypeSymbol* fr = nullptr;
                if (f->ReturnType(context))
                {
                    fr = f->ReturnType(context)->DirectType(context)->FinalType(fullSpan, context);
                }
                TypeSymbol* vr = nullptr;
                if (v->ReturnType(context))
                {
                    vr = v->ReturnType(context)->DirectType(context)->FinalType(fullSpan, context);
                }
                if (fr && vr && !TypesEqual(fr, vr, context))
                {
                    ThrowException("the return type of the overriding function differs from the return type of base class function",
                        f->GetFullSpan(), v->GetFullSpan(), context);
                }
                vtab[j] = f;
                f->SetVTabIndex(j);
                if (f->Group(context))
                {
                    f->Group(context)->SetVTabIndex(f, j, context);
                }
                found = true;
                break;
            }
        }
        if (!found)
        {
            if (f->IsOverride() || f->IsFinal())
            {
                ThrowException("no suitable function to override ('" + f->FullName(context) + "')", fullSpan, context);
            }
            else if (f->IsVirtual() || f->IsPure())
            {
                f->SetVTabIndex(m);
                if (f->Group(context))
                {
                    f->Group(context)->SetVTabIndex(f, m, context);
                }
                vtab.push_back(f);
            }
        }
    }
}

FunctionSymbol* ClassTypeSymbol::GetConversionFunction(TypeSymbol* type, Context* context) 
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    for (FunctionSymbol* conversionFunction : conversionFunctions)
    {
        if (TypesEqual(conversionFunction->ReturnType(context), type, context))
        {
            return conversionFunction;
        }
    }
    for (ClassTypeSymbol* baseClass : baseClasses)
    {
        FunctionSymbol* baseClassConversionFn = baseClass->GetConversionFunction(type, context);
        if (baseClassConversionFn)
        {
            return baseClassConversionFn;
        }
    }
    return nullptr;
}

bool ClassTypeSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::templateDeclarationScope:
    case ScopeKind::classScope:
    case ScopeKind::blockScope:
    {
        return true;
    }
    }
    return false;
}

std::string ClassTypeSymbol::IrName(Context* context) const
{
    std::string irName;
    irName.append("class_").append(Name());
    std::string shaMaterial = FullName(context);
    irName.append("_").append(util::GetSha1MessageDigest(shaMaterial));
    return irName;
}

void ClassTypeSymbol::ComputeVTabName(Context* context)
{
    if (vtabNameOffset != notFoundOffset) return;
    std::string vtabName = "vtab_";
    vtabName.append(IrName(context));
    vtabNameOffset = GetModule()->GetStringTable()->AddString(vtabName);
}

std::string ClassTypeSymbol::GroupName(Context* context) 
{
    ClassGroupSymbol* group = Group(context);
    if (group)
    {
        return group->Name();
    }
    else
    {
        ThrowException("group name of class '" + FullName(context) + "' not resolved", GetFullSpan(), context);
    }
}

otava::intermediate::Value* ClassTypeSymbol::GetVTabVariable(Emitter& emitter, Context* context)
{
    otava::intermediate::Value* vtabVariable = emitter.GetVTabVariable(FullName(context));
    if (!vtabVariable)
    {
        otava::intermediate::Type* voidPtrIrType = emitter.MakePtrType(emitter.GetVoidType());
        otava::intermediate::Type* arrayType = emitter.MakeArrayType(vtab.size() * 2 + otava::symbols::vtabClassIdElementCount, voidPtrIrType);
        vtabVariable = emitter.EmitGlobalVariable(arrayType, VTabName(context), nullptr);
        emitter.SetVTabVariable(FullName(context), vtabVariable);
    }
    return vtabVariable;
}

std::string ClassTypeSymbol::VTabName(Context* context) const
{
    return GetModule()->GetStringTable()->GetString(vtabNameOffset);
}

std::vector<ClassTypeSymbol*> ClassTypeSymbol::VPtrHolderClasses(Context* context) const
{
    std::vector<ClassTypeSymbol*> vptrHolderClasses;
    if (vptrIndex != -1)
    {
        vptrHolderClasses.push_back(const_cast<ClassTypeSymbol*>(this));
    }
    else
    {
        for (ClassTypeSymbol* baseClass : BaseClasses(context))
        {
            std::vector<ClassTypeSymbol*> vptrHolderBaseClasses = baseClass->VPtrHolderClasses(context);
            for (ClassTypeSymbol* vptrHolderClass : vptrHolderBaseClasses)
            {
                if (std::find(vptrHolderClasses.begin(), vptrHolderClasses.end(), vptrHolderClass) == vptrHolderClasses.end())
                {
                    vptrHolderClasses.push_back(vptrHolderClass);
                }
            }
        }
    }
    return vptrHolderClasses;
}

otava::intermediate::Type* ClassTypeSymbol::VPtrType(Emitter& emitter) const noexcept
{
    otava::intermediate::Type* voidPtrIrType = emitter.MakePtrType(emitter.GetVoidType());
    Cardinality vtabSize = Cardinality(vtab.size());
    otava::intermediate::Type* vptrType = emitter.MakePtrType(
        emitter.MakeArrayType(ToUnderlying(vtabSize) * 2 + otava::symbols::vtabClassIdElementCount, voidPtrIrType));
    return vptrType;
}

void ClassTypeSymbol::AddBaseClass(ClassTypeSymbol* baseClass, const soul::ast::FullSpan& fullSpan, Context* context)
{
    baseClasses.push_back(baseClass);
    GetScope()->AddBaseScope(baseClass->GetScope(), fullSpan, context);
}

void ClassTypeSymbol::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    TypeSymbol::AddSymbol(symbol, fullSpan, context);
    if (symbol->IsVariableSymbol())
    {
        VariableSymbol* memberVariable = static_cast<VariableSymbol*>(symbol);
        if ((symbol->GetDeclarationFlags() & DeclarationFlags::staticFlag) != DeclarationFlags::none)
        {
            staticMemberVariables.push_back(memberVariable);
        }
        else
        {
            memberVariable->SetIndex(memberVariables.size());
            memberVariables.push_back(memberVariable);
        }
    }
    else if (symbol->IsFunctionSymbol())
    {
        FunctionSymbol* functionSymbol = static_cast<FunctionSymbol*>(symbol);
        memberFunctions.push_back(functionSymbol);
        if (functionSymbol->IsConversionMemFn())
        {
            conversionFunctions.push_back(functionSymbol);
        }
        MapFunction(functionSymbol);
    }
    if (symbol->IsFunctionDefinitionSymbol())
    {
        FunctionDefinitionSymbol* memFnDefSymbol = static_cast<FunctionDefinitionSymbol*>(symbol);
        SetMemFnDefSymbol(memFnDefSymbol);
    }
}

const std::vector<ClassTypeSymbol*>& ClassTypeSymbol::BaseClasses(Context* context) const
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    return baseClasses;
}

const std::vector<VariableSymbol*>& ClassTypeSymbol::MemberVariables(Context* context) const
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    return memberVariables;
}

const std::vector<VariableSymbol*>& ClassTypeSymbol::StaticMemberVariables(Context* context) const
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    return staticMemberVariables;
}

const std::vector<FunctionSymbol*>& ClassTypeSymbol::MemberFunctions(Context* context) const
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    return memberFunctions;
}

void ClassTypeSymbol::Write(Writer& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(flags));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(classKind));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(baseClasses.size())));
    for (ClassTypeSymbol* baseClass : baseClasses)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(baseClass->Id()));
    }
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(memberVariables.size())));
    for (VariableSymbol* memberVariable : memberVariables)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(memberVariable->Id()));
    }
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(staticMemberVariables.size())));
    for (VariableSymbol* staticMemberVariable : staticMemberVariables)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(staticMemberVariable->Id()));
    }
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(memberFunctions.size())));
    for (FunctionSymbol* memberFunction : memberFunctions)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(memberFunction->Id()));
    }
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(objectLayout.size())));
    for (TypeSymbol* type : objectLayout)
    {
        if (type->GetModule() != GetModule())
        {
            GetModule()->GetSymbolTable()->AddImportedSymbol(type->Id(), type->GetModule()->Id());
        }
        writer.GetBinaryStreamWriter().Write(ToUnderlying(type->Id()));
    }
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(conversionFunctions.size())));
    for (FunctionSymbol* conversionFunction : conversionFunctions)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(conversionFunction->Id()));
    }
    writer.GetBinaryStreamWriter().Write(level);
    if (group)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(group->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(vtab.size())));
    for (FunctionSymbol* vfn : vtab)
    {
        if (vfn->GetModule() != GetModule())
        {
            GetModule()->GetSymbolTable()->AddImportedSymbol(vfn->Id(), vfn->GetModule()->Id());
        }
        writer.GetBinaryStreamWriter().Write(ToUnderlying(vfn->Id()));
    }
    writer.GetBinaryStreamWriter().Write(vptrIndex);
    writer.GetBinaryStreamWriter().Write(deltaIndex);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(vtabNameOffset));
    if (specialization)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(specialization->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    writer.GetBinaryStreamWriter().Write(nextMemFnDefIndex);
}

void ClassTypeSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    flags = ClassTypeSymbolFlags(reader.CurrentReader().ReadByte());
    classKind = ClassKind(reader.CurrentReader().ReadByte());
    Cardinality baseClassCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(baseClassCount); ++i)
    {
        baseClassIds.push_back(SymbolId(reader.CurrentReader().ReadUInt()));
    }
    Cardinality memberVariableCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(memberVariableCount); ++i)
    {
        memberVariableIds.push_back(SymbolId(reader.CurrentReader().ReadUInt()));
    }
    Cardinality staticMemberVariableCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(staticMemberVariableCount); ++i)
    {
        staticMemberVariableIds.push_back(SymbolId(reader.CurrentReader().ReadUInt()));
    }
    Cardinality memberFunctionCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(memberFunctionCount); ++i)
    {
        memberFunctionIds.push_back(SymbolId(reader.CurrentReader().ReadUInt()));
    }
    Cardinality objectLayoutCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(objectLayoutCount); ++i)
    {
        objectLayoutIds.push_back(SymbolId(reader.CurrentReader().ReadUInt()));
    }
    Cardinality conversionFunctionCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(conversionFunctionCount); ++i)
    {
        conversionFunctionIds.push_back(SymbolId(reader.CurrentReader().ReadUInt()));
    }
    level = reader.CurrentReader().ReadInt();
    groupId = SymbolId(reader.CurrentReader().ReadUInt());
    Cardinality vtabSize = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(vtabSize); ++i)
    {
        vtabIds.push_back(SymbolId(reader.CurrentReader().ReadUInt()));
    }
    vptrIndex = reader.CurrentReader().ReadInt();
    deltaIndex = reader.CurrentReader().ReadInt();
    vtabNameOffset = StringOffset(reader.CurrentReader().ReadUInt());
    specializationId = SymbolId(reader.CurrentReader().ReadUInt());
    nextMemFnDefIndex = reader.CurrentReader().ReadInt();
}

void ClassTypeSymbol::GetContent(Context* context) const
{
    if (contentFetched) return;
    contentFetched = true;
    for (SymbolId baseClassId : baseClassIds)
    {
        ClassTypeSymbol* baseClass = GetModule()->GetSymbolTable()->GetClassTypeSymbol(baseClassId, context);
        if (baseClass)
        {
            baseClasses.push_back(baseClass);
        }
        else
        {
            ThrowException("base class id " + std::to_string(ToUnderlying(baseClassId)) + " for class '" + FullName(context) + "' not found");
        }
    }
    for (SymbolId memberVariableId : memberVariableIds)
    {
        VariableSymbol* memberVariable = GetModule()->GetSymbolTable()->GetVariableSymbol(memberVariableId, context);
        if (memberVariable)
        {
            memberVariables.push_back(memberVariable);
        }
        else
        {
            ThrowException("member variable id " + std::to_string(ToUnderlying(memberVariableId)) + " for class '" + FullName(context) + "' not found");
        }
    }
    for (SymbolId staticMemberVariableId : staticMemberVariableIds)
    {
        VariableSymbol* staticMemberVariable = GetModule()->GetSymbolTable()->GetVariableSymbol(staticMemberVariableId, context);
        if (staticMemberVariable)
        {
            staticMemberVariables.push_back(staticMemberVariable);
        }
        else
        {
            ThrowException("static member variable id " + std::to_string(ToUnderlying(staticMemberVariableId)) + " for class '" + FullName(context) + "' not found");
        }
    }
    for (SymbolId memberFunctionId : memberFunctionIds)
    {
        FunctionSymbol* memberFunction = GetModule()->GetSymbolTable()->GetFunctionSymbol(memberFunctionId, context);
        if (memberFunction)
        {
            memberFunctions.push_back(memberFunction);
        }
        else
        {
            ThrowException("member function id " + std::to_string(ToUnderlying(memberFunctionId)) + " for class '" + FullName(context) + "' not found");
        }
    }
    for (SymbolId objectLayoutId : objectLayoutIds)
    {
        TypeSymbol* layoutType = GetModule()->GetSymbolTable()->GetTypeSymbol(objectLayoutId, context);
        if (layoutType)
        {
            objectLayout.push_back(layoutType);
        }
        else
        {
            ThrowException("object layout type id " + std::to_string(ToUnderlying(objectLayoutId)) + " for class '" + FullName(context) + "' not found");
        }
    }
    for (SymbolId conversionFunctionId : conversionFunctionIds)
    {
        FunctionSymbol* conversionFunction = GetModule()->GetSymbolTable()->GetFunctionSymbol(conversionFunctionId, context);
        if (conversionFunction)
        {
            conversionFunctions.push_back(conversionFunction);
        }
        else
        {
            ThrowException("conversion function id " + std::to_string(ToUnderlying(conversionFunctionId)) + " for class '" + FullName(context) + "' not found");
        }
    }
    group = context->GetSymbolTable()->GetClassGroupSymbol(groupId, context);
    for (SymbolId vtabId : vtabIds)
    {
        FunctionSymbol* vfn = GetModule()->GetSymbolTable()->GetFunctionSymbol(vtabId, context);
        if (vfn)
        {
            vtab.push_back(vfn);
        }
        else
        {
            ThrowException("vtable function id " + std::to_string(ToUnderlying(vtabId)) + " for class '" + FullName(context) + "' not found");
        }
    }
    if (specializationId != zeroSymbolId)
    {
        specialization = GetModule()->GetSymbolTable()->GetTypeSymbol(specializationId, context);
    }
}

ForwardClassDeclarationSymbol::ForwardClassDeclarationSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), classTypeSymbol(nullptr), classKind(ClassKind::class_), specialization(nullptr), group(nullptr), groupId(zeroSymbolId),
    classTypeSymbolId(zeroSymbolId), specializationId(zeroSymbolId)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

ForwardClassDeclarationSymbol::ForwardClassDeclarationSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), classTypeSymbol(nullptr), classKind(ClassKind::class_), specialization(nullptr), group(nullptr), groupId(zeroSymbolId),
    classTypeSymbolId(zeroSymbolId), specializationId(zeroSymbolId)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

bool ForwardClassDeclarationSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::templateDeclarationScope:
    case ScopeKind::classScope:
    case ScopeKind::blockScope:
    {
        return true;
    }
    }
    return false;
}

bool ForwardClassDeclarationSymbol::IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType, Context* context) const 
{
    const TypeSymbol* thisSymbol = this;
    if (visited.find(thisSymbol) != visited.end()) return true;
    visited.insert(thisSymbol);
    if (classTypeSymbol)
    {
        return classTypeSymbol->IsComplete(visited, incompleteType, context);
    }
    else
    {
        const TypeSymbol* ict = incompleteType;
        if (!ict)
        {
            incompleteType = this;
        }
        return false;
    }
}

TemplateDeclarationSymbol* ForwardClassDeclarationSymbol::ParentTemplateDeclaration(Context* context) const noexcept
{
    Symbol* parentSymbol = Parent(context);
    if (parentSymbol && parentSymbol->IsTemplateDeclarationSymbol())
    {
        return static_cast<TemplateDeclarationSymbol*>(parentSymbol);
    }
    return nullptr;
}

ClassGroupSymbol* ForwardClassDeclarationSymbol::Group(Context* context) const
{
    if (group)
    {
        return group;
    }
    if (IsReadOnly() && groupId != zeroSymbolId)
    {
        group = GetModule()->GetSymbolTable()->GetClassGroupSymbol(groupId, context);
    }
    return group;
}

Cardinality ForwardClassDeclarationSymbol::Arity(Context* context) noexcept
{
    TemplateDeclarationSymbol* templateDeclaration = ParentTemplateDeclaration(context);
    if (templateDeclaration)
    {
        return templateDeclaration->Arity();
    }
    else
    {
        return Cardinality(0);
    }
}

TypeSymbol* ForwardClassDeclarationSymbol::FinalType(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (classTypeSymbol)
    {
        return classTypeSymbol->FinalType(fullSpan, context);
    }
    else
    {
        return this;
    }
}

ClassTypeSymbol* ForwardClassDeclarationSymbol::GetClassTypeSymbol(Context* context) const
{
    if (classTypeSymbol)
    {
        return classTypeSymbol;
    }
    if (IsReadOnly() && classTypeSymbolId != zeroSymbolId)
    {
        classTypeSymbol = GetModule()->GetSymbolTable()->GetClassTypeSymbol(classTypeSymbolId, context);
    }
    return classTypeSymbol;
}

TypeSymbol* ForwardClassDeclarationSymbol::Specialization(Context* context)
{
    if (specialization)
    {
        return specialization;
    }
    if (IsReadOnly() && specializationId != zeroSymbolId)
    {
        specialization = context->GetSymbolTable()->GetTypeSymbol(specializationId, context);
    }
    return specialization;
}

void ForwardClassDeclarationSymbol::SetSpecialization(TypeSymbol* specialization_, Context* context) noexcept
{
    specialization = specialization_;
}

otava::intermediate::Type* ForwardClassDeclarationSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    TypeSymbol* finalType = FinalType(fullSpan, context);
    if (finalType->IsForwardClassDeclarationSymbol())
    {
        return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
            FundamentalTypeKind::voidType, context)->IrType(emitter, fullSpan, context);
    }
    else
    {
        return finalType->IrType(emitter, fullSpan, context);
    }
}

void ForwardClassDeclarationSymbol::Write(Writer& writer)
{
    TypeSymbol::Write(writer);
    if (classTypeSymbol)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(classTypeSymbol->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    writer.GetBinaryStreamWriter().Write(ToUnderlying(classKind));
    if (specialization)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(specialization->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    if (group)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(group->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
}

void ForwardClassDeclarationSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    classTypeSymbolId = SymbolId(reader.CurrentReader().ReadUInt());
    classKind = ClassKind(reader.CurrentReader().ReadByte());
    specializationId = SymbolId(reader.CurrentReader().ReadUInt());
    groupId = SymbolId(reader.CurrentReader().ReadUInt());
}

void ThrowMemberDeclarationParsingError(const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ThrowException("class member declaration parsing error", fullSpan, context);
}

void ThrowStatementParsingError(const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ThrowException("statement parsing error", fullSpan, context);
}

class ClassResolver : public otava::ast::DefaultVisitor
{
public:
    ClassResolver(Context* context_);
    std::string GetName() const { return name; }
    otava::symbols::ClassKind GetClassKind() const noexcept { return classKind; }
    TypeSymbol* Specialization() const noexcept { return specialization; }
    void Visit(otava::ast::ClassSpecifierNode& node) override;
    void Visit(otava::ast::ClassHeadNode& node) override;
    void Visit(otava::ast::ElaboratedTypeSpecifierNode& node) override;
    void Visit(otava::ast::ClassNode& node) override;
    void Visit(otava::ast::StructNode& node) override;
    void Visit(otava::ast::UnionNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::TemplateIdNode& node) override;
private:
    Context* context;
    std::string name;
    otava::symbols::ClassKind classKind;
    TypeSymbol* specialization;
};

ClassResolver::ClassResolver(Context* context_) : context(context_), classKind(otava::symbols::ClassKind::class_), specialization(nullptr)
{
}

void ClassResolver::Visit(otava::ast::ClassSpecifierNode& node)
{
    node.ClassHead()->Accept(*this);
}

void ClassResolver::Visit(otava::ast::ClassHeadNode& node)
{
    node.ClassKey()->Accept(*this);
    node.ClassHeadName()->Accept(*this);
}

void ClassResolver::Visit(otava::ast::IdentifierNode& node)
{
    name = node.Str();
}

void ClassResolver::Visit(otava::ast::TemplateIdNode& node)
{
    node.TemplateName()->Accept(*this);
    specialization = ResolveType(&node, DeclarationFlags::none, context, TypeResolverFlags::dontInstantiate);
}

void ClassResolver::Visit(otava::ast::ElaboratedTypeSpecifierNode& node)
{
    node.ClassKey()->Accept(*this);
    name = node.GetId()->Str();
}

void ClassResolver::Visit(otava::ast::ClassNode& node)
{
    classKind = otava::symbols::ClassKind::class_;
}

void ClassResolver::Visit(otava::ast::StructNode& node)
{
    classKind = otava::symbols::ClassKind::struct_;
}

void ClassResolver::Visit(otava::ast::UnionNode& node)
{
    classKind = otava::symbols::ClassKind::union_;
}

class BaseClassResolver : public otava::ast::DefaultVisitor
{
public:
    BaseClassResolver(Context* context_);
    void Visit(otava::ast::BaseSpecifierNode& node) override;
    std::vector<ClassTypeSymbol*> BaseClasses() const { return std::move(baseClasses); }
private:
    Context* context;
    std::vector<ClassTypeSymbol*> baseClasses;
};

BaseClassResolver::BaseClassResolver(Context* context_) : context(context_)
{
}

void BaseClassResolver::Visit(otava::ast::BaseSpecifierNode& node)
{
    TypeSymbol* baseClassType = ResolveType(node.ClassOrDeclType(), DeclarationFlags::none, context);
    if (baseClassType->IsClassTypeSymbol())
    {
        ClassTypeSymbol* baseClass = static_cast<ClassTypeSymbol*>(baseClassType);
        baseClasses.push_back(baseClass);
    }
    else if (baseClassType->IsClassGroupTypeSymbol())
    {
        ClassGroupTypeSymbol* classGroupType = static_cast<ClassGroupTypeSymbol*>(baseClassType);
        ClassGroupSymbol* classGroup = classGroupType->GetClassGroup();
        for (ClassTypeSymbol* cls : classGroup->Classes(context))
        {
            std::cout << cls->FullName(context) << "\n";
        }
    }
    else
    {
        ThrowException("class type symbol expected", node.GetFullSpan(), context);
    }
}

std::vector<ClassTypeSymbol*> ResolveBaseClasses(otava::ast::Node* node, Context* context)
{
    BaseClassResolver resolver(context);
    node->Accept(resolver);
    return resolver.BaseClasses();
}

ClassParsingMap::ClassParsingMap() : map()
{
}

otava::ast::FunctionDefinitionNode* ClassParsingMap::GetFunctionDefnitionNode(FunctionSymbol* fn) const noexcept
{
    auto it = map.find(fn);
    if (it != map.end())
    {
        return it->second;
    }
    return nullptr;
}

void ClassParsingMap::MapFunctionDefinitionNode(FunctionSymbol* fn, otava::ast::FunctionDefinitionNode* node)
{
    fns.push_back(fn);
    map[fn] = node;
}

void GetClassAttributes(otava::ast::Node* node, std::string& name, otava::symbols::ClassKind& kind, TypeSymbol*& specialization, Context* context)
{
    ClassResolver resolver(context);
    node->Accept(resolver);
    name = resolver.GetName();
    kind = resolver.GetClassKind();
    specialization = resolver.Specialization();
}

void SetCurrentAccess(otava::ast::Node* node, otava::symbols::Context* context)
{
    switch (node->Kind())
    {
    case otava::ast::NodeKind::publicNode:
    {
        context->GetSymbolTable()->SetCurrentAccess(Access::public_);
        break;
    }
    case otava::ast::NodeKind::protectedNode:
    {
        context->GetSymbolTable()->SetCurrentAccess(Access::protected_);
        break;
    }
    case otava::ast::NodeKind::privateNode:
    {
        context->GetSymbolTable()->SetCurrentAccess(Access::private_);
        break;
    }
    }
}

void BeginClass(otava::ast::Node* node, Context* context)
{
    std::string name;
    otava::symbols::ClassKind kind;
    TypeSymbol* specialization = nullptr;
    GetClassAttributes(node, name, kind, specialization, context);
    context->GetSymbolTable()->BeginClass(name, kind, specialization, node, context);
    std::vector<ClassTypeSymbol*> baseClasses = ResolveBaseClasses(node, context);
    for (ClassTypeSymbol* baseClass : baseClasses)
    {
        context->GetSymbolTable()->AddBaseClass(baseClass, node->GetFullSpan(), context);
    }
    context->PushSetFlag(ContextFlags::parseMemberFunction);
}

void EndClass(otava::ast::Node* node, Context* context)
{
    Symbol* symbol = context->GetSymbolTable()->CurrentScope()->GetSymbol();
    if (!symbol->IsClassTypeSymbol())
    {
        ThrowException("otava.symbols.classes: EndClass(): class scope expected", node->GetFullSpan(), context);
    }
    ClassTypeSymbol* classTypeSymbol = static_cast<ClassTypeSymbol*>(symbol);
    otava::ast::Node* specNode = context->GetSymbolTable()->GetSpecifierNode(classTypeSymbol);
    if (specNode && specNode->IsClassSpecifierNode())
    {
        otava::ast::ClassSpecifierNode* specifierNode = static_cast<otava::ast::ClassSpecifierNode*>(specNode);
        specifierNode->SetComplete();
    }
    else
    {
        ThrowException("otava.symbols.classes: EndClass(): class specifier node not found", node->GetFullSpan(), context);
    }
    if (!classTypeSymbol->IsTemplate(context) && !classTypeSymbol->HasUserDefinedDestructor())
    {
        std::set<const TypeSymbol*> visited;
        const TypeSymbol* incompleteType = nullptr;
        if (classTypeSymbol->IsComplete(visited, incompleteType, context))
        {
            GenerateDestructor(classTypeSymbol, node->GetFullSpan(), context);
        }
        else
        {
            context->GetBoundCompileUnit()->AddClassToGenerateDestructorList(classTypeSymbol);
        }
    }
    context->PopFlags();
    context->GetSymbolTable()->EndClass();
    if (classTypeSymbol->Level() == 0)
    {
        ParseInlineMemberFunctions(specNode, classTypeSymbol, context);
    }
    std::set<const Symbol*> visited;
    if (!classTypeSymbol->IsTemplate(context) && !classTypeSymbol->IsTemplateParameterInstantiation(context, visited))
    {
        context->GetBoundCompileUnit()->AddBoundNodeForClass(classTypeSymbol, node->GetFullSpan(), context);
    }
}

void ClassTypeSymbol::GenerateCopyCtor(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (copyCtor) return;
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    VariableSymbol* classTempVar = new VariableSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::variableSymbol), "@class_temp");
    classTempVar->SetDeclaredType(this, context);
    tempVars.push_back(std::unique_ptr<Symbol>(classTempVar));
    args.push_back(std::unique_ptr<BoundExpressionNode>(
        new BoundAddressOfNode(new BoundVariableNode(classTempVar, fullSpan, classTempVar->GetReferredType(context)), 
            fullSpan, classTempVar->GetType(context)->AddPointer(context))));
    VariableSymbol* constLvalueRefTempVar = new VariableSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::variableSymbol), "@const_lvalue_ref_temp");
    constLvalueRefTempVar->SetDeclaredType(this->AddConst(context)->AddLValueRef(context), context);
    tempVars.push_back(std::unique_ptr<Symbol>(constLvalueRefTempVar));
    args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundVariableNode(constLvalueRefTempVar, fullSpan, constLvalueRefTempVar->GetReferredType(context))));
    std::vector<TypeSymbol*> templateArgs;
    std::unique_ptr<BoundFunctionCallNode> functionCall = ResolveOverloadThrow(
        context->GetSymbolTable()->CurrentScope(), "@constructor", templateArgs, args, fullSpan, context);
    copyCtor = functionCall->GetFunctionSymbol();
}

void ProcessElaboratedClassDeclaration(otava::ast::Node* node, otava::symbols::Context* context)
{
    std::string name;
    otava::symbols::ClassKind kind;
    TypeSymbol* specialization = nullptr;
    GetClassAttributes(node, name, kind, specialization, context);
    if (context->GetFlag(ContextFlags::friendSpecifier))
    {
        context->GetSymbolTable()->AddFriend(name, node, context);
    }
    else
    {
        context->GetSymbolTable()->AddForwardClassDeclaration(name, kind, specialization, node, context);
    }
}

class FunctionDefinitionMapBuilderVisitor : public otava::ast::DefaultVisitor
{
public:
    FunctionDefinitionMapBuilderVisitor(Context* context_);
    void Visit(otava::ast::FunctionDefinitionNode& node) override;
    ClassParsingMap* GetClassParsingMap() const { return classParsingMap.get(); }
private:
    Context* context;
    std::unique_ptr<ClassParsingMap> classParsingMap;
};

FunctionDefinitionMapBuilderVisitor::FunctionDefinitionMapBuilderVisitor(Context* context_) : context(context_), classParsingMap(new ClassParsingMap())
{
}

void FunctionDefinitionMapBuilderVisitor::Visit(otava::ast::FunctionDefinitionNode& node)
{
    Symbol* symbol = context->GetSymbolTable()->GetSymbol(&node);
    FunctionSymbol* functionSymbol = nullptr;
    if (symbol->IsFunctionSymbol())
    {
        functionSymbol = static_cast<FunctionSymbol*>(symbol);
    }
    else
    {
        ThrowException("function symbol expected", node.GetFullSpan(), context);
    }
    functionSymbol->SetClassParsingMap(classParsingMap.get());
    functionSymbol->SetUnparsed();
    classParsingMap->MapFunctionDefinitionNode(functionSymbol, &node);
}


void ParseInlineMemberFunctions(otava::ast::Node* classSpecifierNode, ClassTypeSymbol* classTypeSymbol, Context* context)
{
    context->GetSymbolTable()->BeginScope(classTypeSymbol->GetScope());
    FunctionDefinitionMapBuilderVisitor visitor(context);
    classSpecifierNode->Accept(visitor);
    ClassParsingMap* classParsingMap = visitor.GetClassParsingMap();
    for (auto* fn : classParsingMap->Functions())
    {
        ParseInlineMemberFunction(context, fn);
    }
    context->GetSymbolTable()->EndScope();
}

void ParseInlineMemberFunction(Context* context, FunctionSymbol* memfn)
{
    ClassParsingMap* classParsingMap = memfn->GetClassParsingMap();
    if (!classParsingMap) return;
    if (!memfn->IsUnparsed()) return;
    if (memfn->Parsing() && memfn->IsInline())
    {
        ThrowException("inline member function cannot be recursive", memfn->GetFullSpan(), context);
    }
    memfn->SetParsing();
    otava::ast::FunctionDefinitionNode* node = classParsingMap->GetFunctionDefnitionNode(memfn);
    if (!node)
    {
        ThrowException("error parsing inline member function: function definition node not found", memfn->GetFullSpan(), context);
    }
    try
    {
        otava::ast::Node* fnBody = node->FunctionBody();
        otava::ast::ConstructorInitializerNode* ctorInitializerNode = nullptr;
        otava::ast::CompoundStatementNode* compoundStatementNode = nullptr;
        otava::ast::ConstructorNode* constructorNode = nullptr;
        otava::ast::FunctionBodyNode* functionBodyNode = nullptr;
        if (fnBody->IsConstructorNode())
        {
            constructorNode = static_cast<otava::ast::ConstructorNode*>(fnBody);
            ctorInitializerNode = static_cast<otava::ast::ConstructorInitializerNode*>(constructorNode->Left());
            compoundStatementNode = static_cast<otava::ast::CompoundStatementNode*>(constructorNode->Right());
        }
        else if (fnBody->IsFunctionBodyNode())
        {
            functionBodyNode = static_cast<otava::ast::FunctionBodyNode*>(node->FunctionBody());
            compoundStatementNode = static_cast<otava::ast::CompoundStatementNode*>(functionBodyNode->Child());
        }
        if (ctorInitializerNode)
        {
            if (ctorInitializerNode->GetLexerPosPair().IsValid())
            {
                RecordedParseCtorInitializer(ctorInitializerNode, context);
            }
        }
        if (compoundStatementNode)
        {
            if (compoundStatementNode->GetLexerPosPair().IsValid())
            {
                RecordedParseCompoundStatement(compoundStatementNode, context);
            }
        }
        if (!context->GetFlag(ContextFlags::parsingTemplateDeclaration) && memfn->IsFunctionDefinitionSymbol())
        {
            FunctionDefinitionSymbol* functionDefinitionSymbol = static_cast<FunctionDefinitionSymbol*>(memfn);
            context->PushBoundFunction(new BoundFunctionNode(functionDefinitionSymbol, node->GetFullSpan()));
            functionDefinitionSymbol = BindFunction(node, functionDefinitionSymbol, context);
            std::unique_ptr<BoundNode> boundFunctionNode(context->ReleaseBoundFunction());
            if (functionDefinitionSymbol->IsBound())
            {
                context->GetBoundCompileUnit()->AddBoundNode(std::move(boundFunctionNode), context);
            }
            context->PopBoundFunction();
        }
        context->GetLexer()->SetLog(nullptr);
    }
    catch (const std::exception& ex)
    {
        ThrowException("error parsing inline member function body: " + std::string(ex.what()), node->GetFullSpan(), context);
    }
    memfn->ResetUnparsed();
    memfn->ResetParsing();
    memfn->SetClassParsingMap(nullptr);
}

void MakeObjectLayouts(ClassTypeSymbol* classTypeSymbol, Context* context, const soul::ast::FullSpan& fullSpan)
{
    for (auto* baseClass : classTypeSymbol->BaseClasses(context))
    {
        MakeObjectLayouts(baseClass, context, fullSpan);
    }
    if (!classTypeSymbol->ObjectLayoutComputed())
    {
        classTypeSymbol->MakeObjectLayout(fullSpan, context);
    }
}

void InitVTabs(ClassTypeSymbol* classTypeSymbol, Context* context, const soul::ast::FullSpan& fullSpan)
{
    for (auto* baseClass : classTypeSymbol->BaseClasses(context))
    {
        InitVTabs(baseClass, context, fullSpan);
    }
    if (!classTypeSymbol->VTabInitialized())
    {
        classTypeSymbol->MakeVTab(context, fullSpan);
    }
}

Symbol* GenerateDestructor(ClassTypeSymbol* classTypeSymbol, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    Symbol* dtorFunctionGroupSymbol = classTypeSymbol->GetScope()->Lookup("@destructor", SymbolGroupKind::functionSymbolGroup, ScopeLookup::thisScope,
        fullSpan, context, LookupFlags::dontResolveSingle);
    Symbol* destructorFn = nullptr;
    if (dtorFunctionGroupSymbol && dtorFunctionGroupSymbol->IsFunctionGroupSymbol())
    {
        FunctionGroupSymbol* destructorGroup = static_cast<FunctionGroupSymbol*>(dtorFunctionGroupSymbol);
        destructorFn = destructorGroup->GetSingleDefinition(context);
        if (destructorFn)
        {
            if (destructorFn->IsFunctionSymbol())
            {
                FunctionSymbol* dtor = static_cast<FunctionSymbol*>(destructorFn);
                if (classTypeSymbol->IsPolymorphic(context))
                {
                    if (classTypeSymbol->HasPolymorphicBaseClass(context))
                    {
                        dtor->SetOverride();
                    }
                    else
                    {
                        dtor->SetVirtual();
                    }
                }
                dtor->SetNoExcept();
            }
            return destructorFn;
        }
        destructorFn = destructorGroup->GetSingleSymbol(context);
        if (destructorFn && destructorFn != destructorGroup)
        {
            if (destructorFn->IsFunctionSymbol())
            {
                FunctionSymbol* dtor = static_cast<FunctionSymbol*>(destructorFn);
                if (classTypeSymbol->IsPolymorphic(context))
                {
                    if (classTypeSymbol->HasPolymorphicBaseClass(context))
                    {
                        dtor->SetOverride();
                    }
                    else
                    {
                        dtor->SetVirtual();
                    }
                }
                dtor->SetNoExcept();
            }
            return destructorFn;
        }
    }
    if (classTypeSymbol->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* sp = static_cast<ClassTemplateSpecializationSymbol*>(classTypeSymbol);
        std::set<const Symbol*> visited;
        if (!sp->IsTemplateParameterInstantiation(context, visited))
        {
            if (!sp->Destructor(context))
            {
                std::set<const TypeSymbol*> visited;
                const TypeSymbol* incompleteType = nullptr;
                if (sp->IsComplete(visited, incompleteType, context))
                {
                    InstantiateDestructor(sp, fullSpan, context);
                }
                else
                {
                    std::string note;
                    if (incompleteType)
                    {
                        note.append(": note incomplete type is '" + incompleteType->FullName(context) + "'");
                    }
                    ThrowException("cannot create destructor for class template specialization '" +
                        sp->FullName(context) + "' because it is incomplete at this point" +
                        note, fullSpan, context);
                }
            }
            destructorFn = sp->Destructor(context);
            if (destructorFn)
            {
                if (destructorFn->IsFunctionSymbol())
                {
                    FunctionSymbol* dtor = static_cast<FunctionSymbol*>(destructorFn);
                    if (classTypeSymbol->IsPolymorphic(context))
                    {
                        if (classTypeSymbol->HasPolymorphicBaseClass(context))
                        {
                            dtor->SetOverride();
                        }
                        else
                        {
                            dtor->SetVirtual();
                        }
                    }
                    dtor->SetNoExcept();
                }
                return destructorFn;
            }
        }
    }
    std::unique_ptr<TrivialClassDtor> trivialClassDestructor(new TrivialClassDtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol)));
    Cardinality nm = Cardinality(classTypeSymbol->MemberVariables(context).size());
    Cardinality nb = Cardinality(classTypeSymbol->BaseClasses(context).size());
    if (nm == Cardinality(0) && nb == Cardinality(0))
    {
        FunctionGroupSymbol* functionGroup = classTypeSymbol->GetScope()->GroupScope(context)->GetOrInsertFunctionGroup("@destructor", fullSpan, context);
        Symbol* destructorSymbol = trivialClassDestructor.get();
        functionGroup->AddFunction(trivialClassDestructor.get());
        classTypeSymbol->AddSymbol(trivialClassDestructor.release(), fullSpan, context);
        return destructorSymbol;
    }
    bool hasNonTrivialDestructor = false;
    std::unique_ptr<FunctionSymbol> destructorSymbol(new FunctionSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), "@destructor"));
    destructorSymbol->SetParent(classTypeSymbol);
    destructorSymbol->SetFunctionKind(FunctionKind::destructor);
    destructorSymbol->SetAccess(Access::public_);
    destructorSymbol->SetCompileUnitId(context->GetBoundCompileUnit()->Id());
    destructorSymbol->SetFixedIrName(destructorSymbol->IrName(context));
    destructorSymbol->SetNoExcept();
    std::unique_ptr<FunctionDefinitionSymbol> destructorDefinitionSymbol(new FunctionDefinitionSymbol(
        context->GetModule(), context->GetNextSymbolId(SymbolKind::functionDefinitionSymbol), "@destructor"));
    destructorDefinitionSymbol->SetParent(classTypeSymbol);
    destructorDefinitionSymbol->SetFunctionKind(FunctionKind::destructor);
    destructorDefinitionSymbol->SetAccess(Access::public_);
    destructorDefinitionSymbol->SetDeclaration(destructorSymbol.get());
    destructorDefinitionSymbol->SetCompileUnitId(context->GetBoundCompileUnit()->Id());
    destructorDefinitionSymbol->SetFixedIrName(destructorSymbol->IrName(context));
    destructorDefinitionSymbol->SetNoExcept();
    std::unique_ptr<BoundDtorTerminatorNode> terminator(new BoundDtorTerminatorNode(fullSpan));
    for (Index i = Index(nm) - Index(1); i >= Index(0); --i)
    {
        VariableSymbol* memberVar = classTypeSymbol->MemberVariables(context)[ToUnderlying(i)];
        if (memberVar->GetType(context)->IsPointerType() || memberVar->GetType(context)->IsReferenceType()) continue;
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        if (memberVar->GetType(context)->IsClassTypeSymbol())
        {
            ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(memberVar->GetType(context));
            Symbol* destructorFn = GenerateDestructor(classType, fullSpan, context);
            if (destructorFn->IsFunctionSymbol())
            {
                FunctionSymbol* dtor = static_cast<FunctionSymbol*>(destructorFn);
                if (classType->IsPolymorphic(context))
                {
                    if (classType->HasPolymorphicBaseClass(context))
                    {
                        dtor->SetOverride();
                    }
                    else
                    {
                        dtor->SetVirtual();
                    }
                }
                dtor->SetNoExcept();
            }
        }
        BoundVariableNode* boundVariableNode = new BoundVariableNode(memberVar, fullSpan, memberVar->GetReferredType(context));
        ParameterSymbol* thisParam = destructorDefinitionSymbol->ThisParam(context);
        BoundExpressionNode* thisPtr = new BoundParameterNode(thisParam, fullSpan, thisParam->GetReferredType(context));
        boundVariableNode->SetThisPtr(thisPtr);
        args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundVariableNode, fullSpan, boundVariableNode->GetType()->AddPointer(context))));
        Exception ex;
        std::vector<TypeSymbol*> templateArgs;
        std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverload(
            context->GetSymbolTable()->CurrentScope(), "@destructor", templateArgs, args, fullSpan, context, ex);
        if (boundFunctionCall)
        {
            if (boundFunctionCall->GetFunctionSymbol()->IsVirtual() || boundFunctionCall->GetFunctionSymbol()->IsOverride() ||
                boundFunctionCall->GetFunctionSymbol()->IsFinal())
            {
                boundFunctionCall->SetFlag(BoundExpressionFlags::virtualCall);
            }
            if (!boundFunctionCall->GetFunctionSymbol()->GetFlag(FunctionSymbolFlags::trivialDestructor))
            {
                hasNonTrivialDestructor = true;
                terminator->AddMemberTerminator(boundFunctionCall.release());
            }
        }
    }
    for (Index i = Index(nb) - Index(1); i >= Index(0); --i)
    {
        ClassTypeSymbol* baseClass = classTypeSymbol->BaseClasses(context)[ToUnderlying(i)];
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        ParameterSymbol* thisParam = destructorDefinitionSymbol->ThisParam(context);
        BoundExpressionNode* thisPtr = new BoundParameterNode(thisParam, fullSpan, thisParam->GetType(context));
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            baseClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
        if (conversion)
        {
            Symbol* destructorFn = GenerateDestructor(baseClass, fullSpan, context);
            args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan, conversion->ReturnType(context))));
            Exception ex;
            std::vector<TypeSymbol*> templateArgs;
            std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverload(
                context->GetSymbolTable()->CurrentScope(), "@destructor", templateArgs, args, fullSpan, context, ex);
            if (boundFunctionCall)
            {
                if (!boundFunctionCall->GetFunctionSymbol()->GetFlag(FunctionSymbolFlags::trivialDestructor))
                {
                    hasNonTrivialDestructor = true;
                    terminator->AddMemberTerminator(boundFunctionCall.release());
                }
            }
        }
        else
        {
            ThrowException("base class conversion not found", fullSpan, context);
        }
    }
    if (!hasNonTrivialDestructor)
    {
        FunctionGroupSymbol* functionGroup = classTypeSymbol->GetScope()->GroupScope(context)->GetOrInsertFunctionGroup("@destructor", fullSpan, context);
        FunctionSymbol* trivialDestructor = trivialClassDestructor.get();
        functionGroup->AddFunction(trivialClassDestructor.get());
        classTypeSymbol->AddSymbol(trivialClassDestructor.release(), fullSpan, context);
        return trivialDestructor;
    }
    BoundFunctionNode* boundDestructor = new BoundFunctionNode(destructorDefinitionSymbol.get(), fullSpan);
    FunctionGroupSymbol* functionGroup = classTypeSymbol->GetScope()->GroupScope(context)->GetOrInsertFunctionGroup("@destructor", fullSpan, context);
    functionGroup->AddFunction(destructorSymbol.get());
    FunctionSymbol* destructor = destructorDefinitionSymbol.get();
    classTypeSymbol->AddSymbol(destructorSymbol.release(), fullSpan, context);
    functionGroup->AddFunctionDefinition(destructorDefinitionSymbol.get(), context);
    classTypeSymbol->AddSymbol(destructorDefinitionSymbol.release(), fullSpan, context);
    BoundCompoundStatementNode* body = new BoundCompoundStatementNode(fullSpan);
    MakeObjectLayouts(classTypeSymbol, context, fullSpan);
    InitVTabs(classTypeSymbol, context, fullSpan);
    if (classTypeSymbol->IsPolymorphic(context))
    {
        if (classTypeSymbol->HasPolymorphicBaseClass(context))
        {
            destructor->SetOverride();
        }
        else
        {
            destructor->SetVirtual();
        }
        std::vector<ClassTypeSymbol*> vptrHolderClasses = classTypeSymbol->VPtrHolderClasses(context);
        if (vptrHolderClasses.empty())
        {
            ThrowException("no vptr holder classes for the class '" + classTypeSymbol->FullName(context) + "'", fullSpan, context);
        }
        for (ClassTypeSymbol* vptrHolderClass : vptrHolderClasses)
        {
            if (vptrHolderClass != classTypeSymbol)
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(destructor->ThisParam(context), fullSpan, destructor->ThisParam(context)->GetReferredType(context));
                FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                    vptrHolderClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
                if (conversion)
                {
                    BoundExpressionNode* thisPtrConverted = new BoundConversionNode(thisPtr, conversion, fullSpan, conversion->ReturnType(context));
                    BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtrConverted, classTypeSymbol, vptrHolderClass, fullSpan);
                    terminator->AddSetVPtrStatement(setVPtrStatement);
                }
                else
                {
                    ThrowException("vptr holder class conversion not found", fullSpan, context);
                }
            }
            else
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(destructor->ThisParam(context), fullSpan, destructor->ThisParam(context)->GetReferredType(context));
                BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtr, classTypeSymbol, classTypeSymbol, fullSpan);
                terminator->AddSetVPtrStatement(setVPtrStatement);
            }
        }
    }
    boundDestructor->SetBody(body);
    boundDestructor->SetDtorTerminator(terminator.release());
    context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(boundDestructor), context);
    return destructor;
}

void GenerateDestructors(BoundCompileUnitNode* boundCompileUnit, Context* context)
{
    for (auto* classType : boundCompileUnit->GenerateDestructorList())
    {
        soul::ast::FullSpan fullSpan = classType->GetFullSpan();
        TypeSymbol* finalType = classType->FinalType(fullSpan, context);
        if (finalType->IsClassTypeSymbol())
        {
            ClassTypeSymbol* finalClass = static_cast<ClassTypeSymbol*>(finalType);
            GenerateDestructor(finalClass, fullSpan, context);
        }
    }
}

BoundFunctionCallNode* MakeDestructorCall(ClassTypeSymbol* cls, BoundExpressionNode* arg, FunctionDefinitionSymbol* destructor,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    Symbol* dtorSymbol = nullptr;
    if (destructor)
    {
        dtorSymbol = destructor;
    }
    else
    {
        dtorSymbol = GenerateDestructor(cls, fullSpan, context);
    }
    if (dtorSymbol && (dtorSymbol->IsFunctionSymbol() || dtorSymbol->IsExplicitlyInstantiatedFunctionDefinitionSymbol()))
    {
        FunctionSymbol* dtorFunctionSymbol = static_cast<FunctionSymbol*>(dtorSymbol);
        if (dtorFunctionSymbol->GetFlag(FunctionSymbolFlags::trivialDestructor))
        {
            return nullptr;
        }
        std::unique_ptr<BoundFunctionCallNode> destructorCall(new BoundFunctionCallNode(dtorFunctionSymbol, fullSpan, cls));
        destructorCall->AddArgument(arg->Clone());
        return destructorCall.release();
    }
    return nullptr;
}

void CheckGenerateTemporaryDestructorCall(BoundConstructTemporaryNode* constructTemporary, BoundExpressionNode* arg, Context* context)
{
    if (!arg)
    {
        std::cout << "NO TEMPORARY ARG!" << "\n";
        return;
    }
    BoundExpressionNode* temporary = constructTemporary->Temporary();
    if (temporary->IsBoundAddressOfNode())
    {
        BoundAddressOfNode* addrOf = static_cast<BoundAddressOfNode*>(temporary);
        temporary = addrOf->Subject();
    }
    if (!temporary->GetType()->IsClassTypeSymbol()) return;
    ClassTypeSymbol* cls = static_cast<ClassTypeSymbol*>(temporary->GetType());
    BoundFunctionCallNode* destructorCall = MakeDestructorCall(cls, arg, nullptr, constructTemporary->GetFullSpan(), context);
    if (!destructorCall) return;
    if (context->CurrentProject()->HasDefine("PRINT_TEMPORARIES"))
    {
        if (context->GetFlag(ContextFlags::invoke))
        {
            std::cout << "INVOKE TEMPORARY:" << arg->GetType()->FullName(context) << "\n";
        }
        else
        {
            std::cout << "TEMPORARY:" << arg->GetType()->FullName(context) << "\n";
        }
    }
    context->GetBoundFunction()->AddTemporaryDestructorCall(destructorCall);
}

std::pair<bool, std::int64_t> ClassTypeSymbol::Delta(ClassTypeSymbol* base, Emitter& emitter, Context* context) noexcept
{
    if (TypesEqual(base, this, context)) return std::make_pair(true, static_cast<std::int64_t>(0));
    std::int64_t delta = 0;
    for (ClassTypeSymbol* bc : BaseClasses(context))
    {
        std::pair<bool, std::int64_t> p = bc->Delta(base, emitter, context);
        bool bcfound = p.first;
        std::int64_t bcdelta = p.second;
        if (bcfound) return std::make_pair(true, delta + bcdelta);
        otava::intermediate::Type* bctype = bc->IrType(emitter, soul::ast::FullSpan(), context);
        std::int64_t bcsize = bctype->Size();
        delta += bcsize;
    }
    return std::make_pair(false, static_cast<std::int64_t>(0));
}

std::pair<bool, std::int64_t> Delta(ClassTypeSymbol* left, ClassTypeSymbol* right, Emitter& emitter, Context* context) noexcept
{
    std::pair<bool, std::int64_t> p = left->Delta(right, emitter, context);
    bool found = p.first;
    std::int64_t delta = p.second;
    if (found) return std::make_pair(true, delta);
    std::pair<bool, std::int64_t> r = right->Delta(left, emitter, context);
    bool rfound = r.first;
    std::int64_t rdelta = r.second;
    if (rfound) return std::make_pair(true, -rdelta);
    return std::make_pair(false, static_cast<std::int64_t>(0));
}

} // namespace otava::symbol
