// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_group_symbol;

import otava.symbols.compound_type_symbol;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.function_symbol;
import otava.symbols.modules;
import otava.symbols.templates;
import otava.symbols.type_compare;
import otava.symbols.variable_symbol;
import otava.symbols.writer;
import otava.symbols.reader;

namespace otava::symbols {

FunctionGroupSymbol::FunctionGroupSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), contentFetched(false), expanded(false)
{
}

FunctionGroupSymbol::FunctionGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), contentFetched(false), expanded(false)
{
}

FunctionGroupSymbol::~FunctionGroupSymbol()
{
    for (FunctionSymbol* fn : functions)
    {
        fn->ResetGroup();
    }
    for (FunctionDefinitionSymbol* def : definitions)
    {
        def->ResetGroup();
    }
}

bool FunctionGroupSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::templateDeclarationScope:
    case ScopeKind::classScope:
    case ScopeKind::enumerationScope:
    case ScopeKind::arrayScope:
    {
        return true;
    }
    }
    return false;
}

Symbol* FunctionGroupSymbol::GetSingleSymbol(Context* context) 
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    else
    {
        Expand(context);
    }
    std::vector<Symbol*> fns;
    for (FunctionSymbol* fn : functions)
    {
        fns.push_back(fn);
    }
    for (FunctionGroupSymbol* readOnlyFunctionGroup : readOnlyFunctionGroups)
    {
        fns.push_back(readOnlyFunctionGroup->GetSingleSymbol(context));
    }
    if (fns.size() == 1)
    {
        return fns.front();
    }
    else
    {
        return this;
    }
}

FunctionDefinitionSymbol* FunctionGroupSymbol::GetSingleDefinition(Context* context)
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    else
    {
        Expand(context);
    }
    std::vector<FunctionDefinitionSymbol*> defs;
    for (FunctionDefinitionSymbol* def : definitions)
    {
        defs.push_back(def);
    }
    for (FunctionGroupSymbol* readOnlyFunctionGroup : readOnlyFunctionGroups)
    {
        FunctionDefinitionSymbol* def = readOnlyFunctionGroup->GetSingleDefinition(context);
        defs.push_back(def);
    }
    if (defs.size() == 1)
    {
        return defs.front();
    }
    else
    {
        return nullptr;
    }
}

void FunctionGroupSymbol::AddFunction(FunctionSymbol* function)
{
    function->SetGroup(this);
    functions.push_back(function);
}

void FunctionGroupSymbol::AddFunctionDefinition(FunctionDefinitionSymbol* definition, Context* context)
{
    definition->SetGroup(this);
    definitions.push_back(definition);
}

void FunctionGroupSymbol::Remove(FunctionSymbol* fn)
{
    functions.erase(std::remove(functions.begin(), functions.end(), fn), functions.end());
    definitions.erase(std::remove(definitions.begin(), definitions.end(), fn), definitions.end());
}

void FunctionGroupSymbol::CollectViableFunctions(Cardinality arity, const std::vector<TypeSymbol*>& templateArgs,
    std::vector<FunctionSymbol*>& viableFunctions, Context* context)
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    else
    {
        Expand(context);
    }
    for (FunctionGroupSymbol* readOnlyFunctionGroup : readOnlyFunctionGroups)
    {
        readOnlyFunctionGroup->CollectViableFunctions(arity, templateArgs, viableFunctions, context);
    }
    if (!templateArgs.empty())
    {
        CollectBestMatchingViableFunctionTemplates(arity, templateArgs, viableFunctions, context);
    }
    else
    {
        for (FunctionSymbol* function : Functions(context))
        {
            if (!function->GetModule()) continue;
            if (arity >= function->MinMemFnArity(context) && arity <= function->MemFnArity(context))
            {
                if (std::find(viableFunctions.begin(), viableFunctions.end(), function) == viableFunctions.end())
                {
                    if (function->Skip()) continue;
                    if (context->GetFlag(ContextFlags::skipNonstaticMemberFunctions) && !function->IsStatic() && function->IsMemberFunction(context)) continue;
                    viableFunctions.push_back(function);
                }
            }
        }
        for (FunctionDefinitionSymbol* functionDefinition : Definitions(context))
        {
            if (!functionDefinition->GetModule()) continue;
            if ((functionDefinition->Qualifiers() & FunctionQualifiers::isDeleted) != FunctionQualifiers::none)
            {
                continue;
            }
            if (functionDefinition->Skip()) continue;
            if (context->GetFlag(ContextFlags::skipNonstaticMemberFunctions) && functionDefinition->IsStatic() && functionDefinition->IsMemberFunction(context)) continue;
            if (arity >= functionDefinition->MinMemFnArity(context) && arity <= functionDefinition->MemFnArity(context))
            {
                if (std::find(viableFunctions.begin(), viableFunctions.end(), functionDefinition) == viableFunctions.end())
                {
                    viableFunctions.push_back(functionDefinition);
                }
            }
        }
    }
}

FunctionSymbol* FunctionGroupSymbol::GetMatchingSpecialization(FunctionSymbol* specialization, Context* context)
{
    for (FunctionSymbol* function : Functions(context))
    {
        if (function->IsSpecialization() && FunctionMatches(function, specialization, context))
        {
            return function;
        }
    }
    for (FunctionDefinitionSymbol* functionDefinition : Definitions(context))
    {
        if (functionDefinition->IsSpecialization() && FunctionMatches(functionDefinition, specialization, context))
        {
            return functionDefinition;
        }
    }
    return nullptr;
}

void FunctionGroupSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    Cardinality fnCount = Cardinality(functions.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(fnCount));
    for (FunctionSymbol* fn : functions)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(fn->Id()));
    }
    Cardinality defCount = Cardinality(definitions.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(defCount));
    for (FunctionDefinitionSymbol* def : definitions)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(def->Id()));
    }
}

void FunctionGroupSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    Cardinality fnCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(fnCount); ++i)
    {
        SymbolId fnId = SymbolId(reader.CurrentReader().ReadUInt());
        functionIds.push_back(fnId);
    }
    Cardinality defCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(defCount); ++i)
    {
        SymbolId defId = SymbolId(reader.CurrentReader().ReadUInt());
        definitionIds.push_back(defId);
    }
}

FunctionSymbol* FunctionGroupSymbol::ResolveFunction(const std::vector<TypeSymbol*>& parameterTypes, FunctionQualifiers qualifiers, 
    const std::vector<TypeSymbol*>& specialization, TemplateDeclarationSymbol* templateDeclaration, bool isSpecialization, Context* context) 
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    else
    {
        Expand(context);
    }
    std::vector<FunctionSymbol*> fns;
    for (FunctionSymbol* function : functions)
    {
        fns.push_back(function);
    }
    for (FunctionGroupSymbol* readOnlyFunctionGroup : readOnlyFunctionGroups)
    {
        FunctionSymbol* fn = readOnlyFunctionGroup->ResolveFunction(parameterTypes, qualifiers, specialization, templateDeclaration, isSpecialization, context);
        if (fn)
        {
            fns.push_back(fn);
        }
    }
    for (FunctionSymbol* function : fns)
    {
        TemplateDeclarationSymbol* functionTemplateDeclaration = function->ParentTemplateDeclaration(context);
        if (templateDeclaration && !functionTemplateDeclaration || !templateDeclaration && functionTemplateDeclaration)
        {
            continue;
        }
        if (templateDeclaration)
        {
            if (templateDeclaration->Arity() != functionTemplateDeclaration->Arity()) continue;
            for (Index i = Index(0); i < Index(templateDeclaration->Arity()); ++i)
            {
                if (!TypesEqual(templateDeclaration->TemplateParameters(context)[ToUnderlying(i)], 
                    functionTemplateDeclaration->TemplateParameters(context)[ToUnderlying(i)], context)) continue;
            }
        }
        if (!function->IsMemberFunction(context) && function->IsSpecialization() != isSpecialization) continue;
        if (function->Arity() == Cardinality(parameterTypes.size()))
        {
            bool found = (qualifiers & FunctionQualifiers::isConst) == (function->Qualifiers() & FunctionQualifiers::isConst);
            if (found)
            {
                if (!specialization.empty())
                {
                    int n = specialization.size();
                    if (n != function->Specialization().size())
                    {
                        found = false;
                    }
                    else
                    {
                        for (int i = 0; i < n; ++i)
                        {
                            if (!TypesEqual(specialization[i], function->Specialization()[i], context))
                            {
                                found = false;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    for (Index i = Index(0); i < Index(function->Arity()); ++i)
                    {
                        if (!TypesEqual(function->Parameters(context)[ToUnderlying(i)]->GetType(context), parameterTypes[ToUnderlying(i)], context))
                        {
                            found = false;
                            break;
                        }
                    }
                }
                if (found)
                {
                    return function;
                }
            }
        }
    }
    return nullptr;
}

std::vector<FunctionSymbol*> FunctionGroupSymbol::Functions(Context* context)
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    else
    {
        Expand(context);
    }
    std::vector<FunctionSymbol*> fns;
    for (FunctionSymbol* fn : functions)
    {
        fns.push_back(fn);
    }
    for (FunctionGroupSymbol* readOnlyFunctionGroup : readOnlyFunctionGroups)
    {
        std::vector<FunctionSymbol*> roFns = readOnlyFunctionGroup->Functions(context);
        for (FunctionSymbol* fn : roFns)
        {
            fns.push_back(fn);
        }
    }
    return fns;
}

std::vector<FunctionDefinitionSymbol*> FunctionGroupSymbol::Definitions(Context* context)
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    else
    {
        Expand(context);
    }
    std::vector<FunctionDefinitionSymbol*> defs;
    for (FunctionDefinitionSymbol* def : definitions)
    {
        defs.push_back(def);
    }
    for (FunctionGroupSymbol* readOnlyFunctionGroup : readOnlyFunctionGroups)
    {
        std::vector<FunctionDefinitionSymbol*> roDefs = readOnlyFunctionGroup->Definitions(context);
        for (FunctionDefinitionSymbol* def : roDefs)
        {
            defs.push_back(def);
        }
    }
    return defs;
}

void FunctionGroupSymbol::SetVTabIndex(FunctionSymbol* function, int vtabIndex, Context* context)
{
    std::vector<FunctionSymbol*> fns = Functions(context);
    for (FunctionSymbol* decl : fns)
    {
        if (decl != function && FunctionMatches(decl, function, context))
        {
            decl->SetVTabIndex(vtabIndex);
        }
    }
    std::vector<FunctionDefinitionSymbol*> defs = Definitions(context);
    for (FunctionDefinitionSymbol* def : defs)
    {
        if (function != def && FunctionMatches(def, function, context))
        {
            def->SetVTabIndex(vtabIndex);
        }
    }
}

struct FunctionScoreGreater
{
    bool operator()(const std::pair<FunctionSymbol*, int>& left, const std::pair<FunctionSymbol*, int>& right) const
    {
        return left.second > right.second;
    }
};

int MatchFunctionTemplate(FunctionSymbol* function, const std::vector<TypeSymbol*>& templateArgs, Context* context)
{
    if (function->Specialization().empty())
    {
        return 0;
    }
    else
    {
        int score = 0;
        Cardinality n = Cardinality(templateArgs.size());
        std::map<TemplateParameterSymbol*, TypeSymbol*> templateParameterMap;
        for (Index i = Index(0); i < Index(n); ++i)
        {
            TypeSymbol* templateArg = templateArgs[ToUnderlying(i)];
            if (Index(function->Specialization().size()) >= i)
            {
                TypeSymbol* specializationType = function->Specialization()[ToUnderlying(i)];
                if (templateArg->IsCompoundTypeSymbol())
                {
                    CompoundTypeSymbol* templateArgCompoundType = static_cast<CompoundTypeSymbol*>(templateArg);
                    Derivations argDerivations = templateArgCompoundType->GetDerivations();
                    if (specializationType->IsCompoundTypeSymbol())
                    {
                        CompoundTypeSymbol* specializationCompoundType = static_cast<CompoundTypeSymbol*>(specializationType);
                        Derivations specializationDerivations = specializationCompoundType->GetDerivations();
                        int numMatchingDerivations = CountMatchingDerivations(argDerivations, specializationDerivations);
                        if (numMatchingDerivations > 0)
                        {
                            score += numMatchingDerivations;
                        }
                    }
                    else
                    {
                        return -1;
                    }
                }
                else
                {
                    TypeSymbol* templateArgumentType = nullptr;
                    if (specializationType->GetBaseType(context)->IsTemplateParameterSymbol())
                    {
                        TemplateParameterSymbol* templateParameter = static_cast<TemplateParameterSymbol*>(specializationType->GetBaseType(context));
                        auto it = templateParameterMap.find(templateParameter);
                        if (it == templateParameterMap.end())
                        {
                            templateArgumentType = templateArg->RemoveDerivations(specializationType->GetDerivations(), context);
                            if (templateArgumentType)
                            {
                                templateParameterMap[templateParameter] = templateArgumentType;
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        else
                        {
                            templateArgumentType = it->second;
                        }
                        specializationType = specializationType->Unify(templateArgumentType, context);
                        if (!specializationType)
                        {
                            return -1;
                        }
                    }
                    if (!TypesEqual(templateArg, specializationType, context))
                    {
                        return -1;
                    }
                    else
                    {
                        score += 1;
                    }
                }
            }
            else
            {
                return -1;
            }
        }
        return score;
    }
    return -1;
}

void FunctionGroupSymbol::CollectBestMatchingViableFunctionTemplates(Cardinality arity, 
    const std::vector<TypeSymbol*>& templateArgs, std::vector<FunctionSymbol*>& viableFunctions, Context* context)
{
    std::vector<std::pair<FunctionSymbol*, int>> functionScores;
    std::vector<FunctionSymbol*> fns;
    for (FunctionSymbol* function : Functions(context))
    {
        fns.push_back(function);
    }
    for (FunctionGroupSymbol* readOnlyFunctionGroup : readOnlyFunctionGroups)
    {
        for (FunctionSymbol* function : readOnlyFunctionGroup->Functions(context))
        {
            fns.push_back(function);
        }
    }
    for (FunctionSymbol* function : fns)
    {
        if (!function->IsTemplate(context)) continue;
        int score = MatchFunctionTemplate(function, templateArgs, context);
        if (score >= 0)
        {
            functionScores.push_back(std::make_pair(function, score));
        }
    }
    std::sort(functionScores.begin(), functionScores.end(), FunctionScoreGreater());
    if (!functionScores.empty())
    {
        FunctionSymbol* function = functionScores[0].first;
        if (arity >= function->MinMemFnArity(context) && arity <= function->MemFnArity(context))
        {
            if (std::find(viableFunctions.begin(), viableFunctions.end(), function) == viableFunctions.end())
            {
                viableFunctions.push_back(function);
            }
        }
    }
    std::vector<std::pair<FunctionSymbol*, int>> functionDefScores;
    std::vector<FunctionDefinitionSymbol*> defs;
    for (FunctionDefinitionSymbol* functionDefinition : Definitions(context))
    {
        defs.push_back(functionDefinition);
    }
    for (FunctionGroupSymbol* readOnlyFunctionGroup : readOnlyFunctionGroups)
    {
        for (FunctionDefinitionSymbol* functionDefinition : readOnlyFunctionGroup->Definitions(context))
        {
            defs.push_back(functionDefinition);
        }
    }
    for (FunctionDefinitionSymbol* functionDefinition : defs)
    {
        if (!functionDefinition->IsTemplate(context)) continue;
        int score = MatchFunctionTemplate(functionDefinition, templateArgs, context);
        if (score >= 0)
        {
            functionDefScores.push_back(std::make_pair(static_cast<FunctionSymbol*>(functionDefinition), score));
        }
    }
    std::sort(functionDefScores.begin(), functionDefScores.end(), FunctionScoreGreater());
    if (!functionDefScores.empty())
    {
        FunctionSymbol* function = functionDefScores[0].first;
        if (arity >= function->MinMemFnArity(context) && arity <= function->MemFnArity(context))
        {
            if (std::find(viableFunctions.begin(), viableFunctions.end(), function) == viableFunctions.end())
            {
                viableFunctions.push_back(function);
            }
        }
    }
}

void FunctionGroupSymbol::GetContent(Context* context)
{
    if (contentFetched) return;
    contentFetched = true;
    for (SymbolId fnId : functionIds)
    {
        FunctionSymbol* fn = GetModule()->GetSymbolTable()->GetFunctionSymbol(fnId, context);
        fn->SetGroup(this);
        functions.push_back(fn);
    }
    for (SymbolId defId : definitionIds)
    {
        FunctionDefinitionSymbol* def = GetModule()->GetSymbolTable()->GetFunctionDefinitionSymbol(defId, context);
        def->SetGroup(this);
        definitions.push_back(def);
    }
}

void FunctionGroupSymbol::Expand(Context* context)
{
    if (expanded) return;
    expanded = true;
    for (const auto& moduleSymbolId : ModuleSymbolIds())
    {
        ModuleId moduleId = moduleSymbolId.moduleId;
        Module* module = context->GetModuleMapper()->GetModule(moduleId);
        if (module)
        {
            SymbolId symbolId = moduleSymbolId.symbolId;
            FunctionGroupSymbol* functionGroup = module->GetSymbolTable()->GetFunctionGroupSymbol(symbolId, context);
            if (functionGroup)
            {
                readOnlyFunctionGroups.push_back(functionGroup);
            }
        }
    }
}

bool FunctionGroupSymbol::IsExportSymbol(Context* context) const noexcept
{
    return Symbol::IsExportSymbol(context) && (ContainsExportFunction(context) || ContainsExportDefinition(context));
}

bool FunctionGroupSymbol::ContainsExportFunction(Context* context) const noexcept
{
    for (FunctionSymbol* functionSymbol : functions)
    {
        if (functionSymbol->IsExportSymbol(context)) return true;
    }
    return false;
}

bool FunctionGroupSymbol::ContainsExportDefinition(Context* context) const noexcept
{
    for (FunctionDefinitionSymbol* functionDefinitionSymbol : definitions)
    {
        if (functionDefinitionSymbol->IsExportSymbol(context)) return true;
    }
    return false;
}

} // namespace otava::symbols
