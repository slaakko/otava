// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.class_group_symbol;

import otava.symbols.classes;
import otava.symbols.class_templates;
import otava.symbols.context;
import otava.symbols.compound_type_symbol;
import otava.symbols.derivations;
import otava.symbols.exception;
import otava.symbols.lookup;
import otava.symbols.scope_resolver;
import otava.symbols.templates;
import otava.symbols.type_compare;
import otava.symbols.writer;
import otava.symbols.reader;
import soul.ast.span;

namespace otava::symbols {

struct ViableClassGreater
{
    inline bool operator()(const std::pair<ClassTypeSymbol*, TemplateMatchInfo>& left, const std::pair<ClassTypeSymbol*, TemplateMatchInfo>& right) const noexcept
    {
        return left.second.matchValue > right.second.matchValue;
    }
};

int Match(Symbol* templateArg, TypeSymbol* specialization, Index index, TemplateMatchInfo& info, Context* context)
{
    if (templateArg->IsCompoundTypeSymbol())
    {
        CompoundTypeSymbol* templateArgType = static_cast<CompoundTypeSymbol*>(templateArg);
        Derivations argDerivations = templateArgType->GetDerivations();
        CompoundTypeSymbol* specializationArgType = GetCompoundSpecializationArgType(specialization, index, context);
        if (specializationArgType)
        {
            Derivations specializationDerivations = specializationArgType->GetDerivations();
            int numMatchingDerivations = CountMatchingDerivations(argDerivations, specializationDerivations);
            if (numMatchingDerivations > 0)
            {
                info.kind = TemplateMatchKind::partialSpecialization;
                return numMatchingDerivations;
            }
        }
    }
    else if (templateArg->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* templateArgType = static_cast<ClassTemplateSpecializationSymbol*>(templateArg);
        ClassTemplateSpecializationSymbol* specializationArgType = GetClassTemplateSpecializationArgType(specialization, index, context);
        if (specializationArgType)
        {
            if (TypesEqual(templateArgType->ClassTemplate(context), specializationArgType->ClassTemplate(context), context))
            {
                int n = templateArgType->TemplateArguments(context).size();
                int m = specializationArgType->TemplateArguments(context).size();
                if (n == m)
                {
                    for (int i = 0; i < n; ++i)
                    {
                        Symbol* argSymbol = templateArgType->TemplateArguments(context)[i];
                        TypeSymbol* argTypeSymbol = nullptr;
                        if (argSymbol->IsTypeSymbol())
                        {
                            argTypeSymbol = static_cast<TypeSymbol*>(argSymbol);
                        }
                        Symbol* templateSymbol = specializationArgType->TemplateArguments(context)[i];
                        TypeSymbol* templateTypeSymbol = nullptr;
                        if (templateSymbol->IsTypeSymbol())
                        {
                            templateTypeSymbol = static_cast<TypeSymbol*>(templateSymbol);
                        }
                        if (argTypeSymbol && templateTypeSymbol)
                        {
                            TypeSymbol* templateArgumentType = nullptr;
                            if (templateTypeSymbol->GetBaseType(context)->IsTemplateParameterSymbol())
                            {
                                TemplateParameterSymbol* templateParameter = static_cast<TemplateParameterSymbol*>(argTypeSymbol->GetBaseType(context));
                                auto it = info.templateParameterMap.find(templateParameter);
                                if (it == info.templateParameterMap.end())
                                {
                                    templateArgumentType = argTypeSymbol->RemoveDerivations(templateTypeSymbol->GetDerivations(), context);
                                    if (templateArgumentType)
                                    {
                                        info.templateParameterMap[templateParameter] = templateArgumentType;
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
                            }
                            templateTypeSymbol = templateTypeSymbol->Unify(templateArgumentType, context);
                            if (!templateTypeSymbol)
                            {
                                return -1;
                            }
                            if (!TypesEqual(argTypeSymbol, templateTypeSymbol, context))
                            {
                                return -1;
                            }
                        }
                    }
                    info.kind = TemplateMatchKind::explicitSpecialization;
                    return 1;
                }
            }
        }
    }
    else if (templateArg->IsTypeSymbol())
    {
        TypeSymbol* taType = static_cast<TypeSymbol*>(templateArg);
        if (specialization->IsClassTemplateSpecializationSymbol())
        {
            ClassTemplateSpecializationSymbol* sp = static_cast<ClassTemplateSpecializationSymbol*>(specialization);
            ClassTypeSymbol* ct = sp->ClassTemplate(context);
            TemplateParameterSymbol* tps = nullptr;
            if (ct->Parent(context)->IsTemplateDeclarationSymbol())
            {
                TemplateDeclarationSymbol* tds = static_cast<TemplateDeclarationSymbol*>(ct->Parent(context));
                tps = tds->TemplateParameters(context).front();
            }
            if (sp->TemplateArguments(context).size() == 1)
            {
                Symbol* spArg = sp->TemplateArguments(context).front();
                if (spArg->IsTypeSymbol())
                {
                    if (TypesEqual(taType, static_cast<TypeSymbol*>(spArg), context))
                    {
                        info.templateParameterMap[tps] = taType;
                        info.kind = TemplateMatchKind::explicitSpecialization;
                        return 1;
                    }
                }
            }
        }
    }
    return -1;
}

ClassGroupSymbol::ClassGroupSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), contentFetched(false), classesSet(false), forwardDeclarationsSet(false)
{
}

ClassGroupSymbol::ClassGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), contentFetched(false), classesSet(false), forwardDeclarationsSet(false)
{
}

void ClassGroupSymbol::AddClass(ClassTypeSymbol* cls, Context* context)
{
    if (std::find(classes.begin(), classes.end(), cls) == classes.end())
    {
        cls->SetGroup(this);
        classes.push_back(cls);
        for (ForwardClassDeclarationSymbol* fwd : forwardDeclarations)
        {
            if (fwd->Arity(context) == cls->Arity(context))
            {
                fwd->SetClassTypeSymbol(cls);
            }
        }
    }
}

void ClassGroupSymbol::AddForwardDeclaration(ForwardClassDeclarationSymbol* fwd)
{
    if (std::find(forwardDeclarations.begin(), forwardDeclarations.end(), fwd) == forwardDeclarations.end())
    {
        fwd->SetGroup(this);
        forwardDeclarations.push_back(fwd);
    }
}

bool ClassGroupSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
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

Symbol* ClassGroupSymbol::GetSingleSymbol(Context* context) 
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    std::vector<ClassTypeSymbol*> allClasses;
    for (ClassTypeSymbol* cls : classes)
    {
        if (cls && std::find(allClasses.begin(), allClasses.end(), cls) == allClasses.end())
        {
            allClasses.push_back(cls);
        }
    }
    std::vector<ForwardClassDeclarationSymbol*> allForwardDeclarations;
    for (ForwardClassDeclarationSymbol* fwd : forwardDeclarations)
    {
        if (fwd && std::find(allForwardDeclarations.begin(), allForwardDeclarations.end(), fwd) == allForwardDeclarations.end())
        {
            allForwardDeclarations.push_back(fwd);
        }
    }
    if (allClasses.size() == 1)
    {
        Symbol* front = allClasses.front();
        return front;
    }
    else if (allForwardDeclarations.size() == 1)
    {
        Symbol* front = allForwardDeclarations.front();
        return front;
    }
    else
    {
        return this;
    }
}

std::vector<Symbol*> MakeTemplateArgs(const std::map<TemplateParameterSymbol*, TypeSymbol*, TemplateParamLess>& templateParamMap)
{
    std::vector<Symbol*> templateArgs;
    for (const auto& p : templateParamMap)
    {
        templateArgs.push_back(p.second);
    }
    return templateArgs;
}

const std::vector<ClassTypeSymbol*>& ClassGroupSymbol::Classes(Context* context) const
{
    return Classes(nullptr, context);
}

const std::vector<ClassTypeSymbol*>& ClassGroupSymbol::Classes(Symbol* parent, Context* context) const
{
    if (!context->GetFlag(ContextFlags::dontLookImports))
    {
        Scope* currentSymbolScope = context->GetSymbolTable()->CurrentScope()->SymbolScope(context);
        Symbol* sym = currentSymbolScope->GetSymbol();
        std::vector<std::string> containerNames = GetContainerNames(sym, context);
        std::vector<std::string> parentContainerNames;
        if (parent)
        {
            parentContainerNames = GetContainerNames(parent, context);
        }
        std::vector<Module*> modules = context->GetModule()->ImportExportModules(context);
        for (Module* module : modules)
        {
            Scope* globalNsScope = module->GetSymbolTable()->GetGlobalNs(context)->GetScope();
            Scope* containerScope = EnterScope(globalNsScope, containerNames, GetFullSpan(), context);
            Symbol* s = containerScope->Lookup(Name(), SymbolGroupKind::classSymbolGroup, ScopeLookup::allScopes, GetFullSpan(), context, LookupFlags::dontResolveSingle);
            if (!s && !parentContainerNames.empty())
            {
                Scope* containerScope2 = EnterScope(globalNsScope, parentContainerNames, GetFullSpan(), context);
                s = containerScope2->Lookup(Name(), SymbolGroupKind::classSymbolGroup, ScopeLookup::allScopes, GetFullSpan(), context, LookupFlags::dontResolveSingle);
            }
            if (s && s->IsClassGroupSymbol())
            {
                ClassGroupSymbol* group = static_cast<ClassGroupSymbol*>(s);
                FlagSetter noImportsFlagSetter(context, ContextFlags::dontLookImports);
                std::vector<ClassTypeSymbol*> groupClasses = group->Classes(context);
                for (ClassTypeSymbol* cls : groupClasses)
                {
                    if (std::find(classes.begin(), classes.end(), cls) == classes.end())
                    {
                        classes.push_back(cls);
                    }
                }
            }
        }
    }
    if (IsReadOnly() && !classesSet)
    {
        classesSet = true;
        for (SymbolId classId : classIds)
        {
            ClassTypeSymbol* cls = GetModule()->GetSymbolTable()->GetClassTypeSymbol(classId, context);
            if (cls && std::find(classes.begin(), classes.end(), cls) == classes.end())
            {
                classes.push_back(cls);
            }
        }
    }
    return classes;
}

const std::vector<ForwardClassDeclarationSymbol*>& ClassGroupSymbol::ForwardDeclarations(Context* context) const
{
    if (IsReadOnly() && !forwardDeclarationsSet)
    {
        forwardDeclarationsSet = true;
        for (SymbolId fwdId : fwdDeclIds)
        {
            ForwardClassDeclarationSymbol* fwd = GetModule()->GetSymbolTable()->GetForwardClassDeclarationSymbol(fwdId, context);
            if (fwd)
            {
                forwardDeclarations.push_back(fwd);
            }
        }
    }
    return forwardDeclarations;
}

ClassTypeSymbol* ClassGroupSymbol::GetClass(Cardinality arity, Context* context) const
{
    const std::vector<ClassTypeSymbol*>& classes = Classes(context);
    for (ClassTypeSymbol* cls : classes)
    {
        if (cls->Arity(context) == arity)
        {
            return cls;
        }
    }
    return nullptr;
}

ClassTypeSymbol* ClassGroupSymbol::GetClass(Cardinality arity, Symbol* parent, Context* context) const
{
    const std::vector<ClassTypeSymbol*>& classes = Classes(parent, context);
    for (ClassTypeSymbol* cls : classes)
    {
        if (cls->Arity(context) == arity)
        {
            return cls;
        }
    }
    return nullptr;
}

ForwardClassDeclarationSymbol* ClassGroupSymbol::GetForwardDeclaration(Cardinality arity, Context* context) const
{
    const std::vector<ForwardClassDeclarationSymbol*>& forwardDeclarations = ForwardDeclarations(context);
    for (ForwardClassDeclarationSymbol* fwd : forwardDeclarations)
    {
        if (fwd->Arity(context) == arity)
        {
            return fwd;
        }
    }
    return nullptr;
}

ClassTypeSymbol* ClassGroupSymbol::GetBestMatchingClass(const std::vector<Symbol*>& templateArgs, TemplateMatchInfo& matchInfo, Context* context) 
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    std::vector<std::pair<ClassTypeSymbol*, TemplateMatchInfo>> viableClasses;
    Cardinality arity = Cardinality(templateArgs.size());
    std::vector<ClassTypeSymbol*> allClasses;
    for (ClassTypeSymbol* cls : classes)
    {
        if (cls)
        {
            allClasses.push_back(cls);
        }
    }
    for (ClassTypeSymbol* cls : allClasses)
    {
        if (cls->Arity(context) == arity)
        {
            TypeSymbol* specialization = cls->Specialization(context);
            if (specialization)
            {
                int score = -1;
                TemplateMatchInfo info;
                for (Index i = Index(0); i < ToIndex(arity); ++i)
                {
                    Symbol* templateArg = templateArgs[ToUnderlying(i)];
                    int matchValue = Match(templateArg, specialization, i, info, context);
                    if (matchValue >= 0)
                    {
                        score += 2 * matchValue;
                    }
                }
                info.matchValue = score;
                viableClasses.push_back(std::make_pair(cls, info));
            }
            else
            {
                TemplateMatchInfo info;
                viableClasses.push_back(std::make_pair(cls, info));
            }
        }
    }
    std::sort(viableClasses.begin(), viableClasses.end(), ViableClassGreater());
    if (!viableClasses.empty())
    {
        matchInfo = viableClasses[0].second;
        if (matchInfo.kind == TemplateMatchKind::explicitSpecialization)
        {
            matchInfo.templateArgs = MakeTemplateArgs(matchInfo.templateParameterMap);
        }
        return viableClasses[0].first;
    }
    return nullptr;
}

void ClassGroupSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    Cardinality classCount = Cardinality(classes.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(classCount));
    for (ClassTypeSymbol* cls : classes)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(cls->Id()));
    }
    Cardinality fwdCount = Cardinality(forwardDeclarations.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(fwdCount));
    for (ForwardClassDeclarationSymbol* fwd : forwardDeclarations)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(fwd->Id()));
    }
}

void ClassGroupSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    Cardinality classCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < ToIndex(classCount); ++i)
    {
        SymbolId classId = SymbolId(reader.CurrentReader().ReadULong());
        classIds.push_back(classId);
    }
    Cardinality fwdCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < ToIndex(fwdCount); ++i)
    {
        SymbolId fwdId = SymbolId(reader.CurrentReader().ReadULong());
        fwdDeclIds.push_back(fwdId);
    }
}

void ClassGroupSymbol::GetContent(Context* context)
{
    if (contentFetched) return;
    contentFetched = true;
    for (SymbolId classId : classIds)
    {
        ClassTypeSymbol* cls = GetModule()->GetSymbolTable()->GetClassTypeSymbol(classId, context);
        if (!cls)
        {
            ThrowException("class id " + std::to_string(ToUnderlying(classId)) + " not found from class group '" + FullName(context) + "' from module '" + 
                GetModule()->Name() + "'");
        }
        if (std::find(classes.begin(), classes.end(), cls) == classes.end())
        {
            classes.push_back(cls);
        }
    }
    for (SymbolId fwdId : fwdDeclIds)
    {
        ForwardClassDeclarationSymbol* fwd = GetModule()->GetSymbolTable()->GetForwardClassDeclarationSymbol(fwdId, context);
        if (!fwd)
        {
            ThrowException("class forward declartion id " + std::to_string(ToUnderlying(fwdId)) + " not found from class group '" + 
                FullName(context) + "' from module '" + GetModule()->Name() + "'");
        }
        if (std::find(forwardDeclarations.begin(), forwardDeclarations.end(), fwd) == forwardDeclarations.end())
        {
            forwardDeclarations.push_back(fwd);
        }
    }
}

bool ClassGroupSymbol::IsExportSymbol(Context* context) const noexcept
{
    return Symbol::IsExportSymbol(context) && ContainsExportClassOrFwdDeclaration(context);
}

bool ClassGroupSymbol::ContainsExportClassOrFwdDeclaration(Context* context) const noexcept
{
    for (ClassTypeSymbol* cls : classes)
    {
        if (cls->IsExportSymbol(context)) return true;
    }
    for (ForwardClassDeclarationSymbol* fwd : forwardDeclarations)
    {
        if (fwd->IsExportSymbol(context)) return true;
    }
    return false;
}

} // namespace otava::symbols
