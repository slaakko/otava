// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.class_group_symbol;

import otava.symbols.classes;
import otava.symbols.class_templates;
import otava.symbols.context;
import otava.symbols.compound_type_symbol;
import otava.symbols.exception;
import otava.symbols.templates;
import otava.symbols.type_compare;
import otava.symbols.writer;
import otava.symbols.reader;

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
    return -1;
}

ClassGroupSymbol::ClassGroupSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), contentFetched(false), expanded(false), classesSet(false), forwardDeclarationsSet(false)
{
}

ClassGroupSymbol::ClassGroupSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), contentFetched(false), expanded(false), classesSet(false), forwardDeclarationsSet(false)
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
    else
    {
        Expand(context);
    }
    std::vector<ClassTypeSymbol*> allClasses;
    for (ClassTypeSymbol* cls : classes)
    {
        if (cls && std::find(allClasses.begin(), allClasses.end(), cls) == allClasses.end())
        {
            allClasses.push_back(cls);
        }
    }
    for (ClassGroupSymbol* readOnlyClassGroup : readOnlyClassGroups)
    {
        for (ClassTypeSymbol* cls : readOnlyClassGroup->Classes(context))
        {
            if (cls && std::find(allClasses.begin(), allClasses.end(), cls) == allClasses.end())
            {
                allClasses.push_back(cls);
            }
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
    for (ClassGroupSymbol* readOnlyClassGroup : readOnlyClassGroups)
    {
        for (ForwardClassDeclarationSymbol* fwd : readOnlyClassGroup->ForwardDeclarations(context))
        {
            if (fwd && std::find(allForwardDeclarations.begin(), allForwardDeclarations.end(), fwd) == allForwardDeclarations.end())
            {
                allForwardDeclarations.push_back(fwd);
            }
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

std::vector<Symbol*> MakeTemplateArgs(const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, 
    TemplateParamEqual>& templateParamMap)
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
    else
    {
        Expand(context);
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
    for (ClassGroupSymbol* readOnlyClassGroup : readOnlyClassGroups)
    {
        for (ClassTypeSymbol* cls : readOnlyClassGroup->Classes(context))
        {
            if (cls && std::find(allClasses.begin(), allClasses.end(), cls) == allClasses.end())
            {
                allClasses.push_back(cls);
            }
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
                for (Index i = Index(0); i < Index(arity); ++i)
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
    for (Index i = Index(0); i < Index(classCount); ++i)
    {
        SymbolId classId = SymbolId(reader.CurrentReader().ReadUInt());
        classIds.push_back(classId);
    }
    Cardinality fwdCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(fwdCount); ++i)
    {
        SymbolId fwdId = SymbolId(reader.CurrentReader().ReadUInt());
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

void ClassGroupSymbol::Expand(Context* context)
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
            ClassGroupSymbol* classGroup = module->GetSymbolTable()->GetClassGroupSymbol(symbolId, context);
            if (classGroup)
            {
                readOnlyClassGroups.push_back(classGroup);
            }
            else
            {
                //ThrowException("class group symbol " + std::to_string(ToUnderlying(symbolId)) + " not found from module " + module->Name());
            }
        }
        else
        {
            ThrowException("import module " + std::to_string(ToUnderlying(moduleId)) + " not found from class group '" + FullName(context) +
                "' of module " + GetModule()->Name());
        }
    }
}

bool ClassGroupSymbol::IsExportSymbol(Context* context) const noexcept
{
    return Symbol::IsExportSymbol(context) && ContainsExportClass(context);
}

bool ClassGroupSymbol::ContainsExportClass(Context* context) const noexcept
{
    for (ClassTypeSymbol* cls : classes)
    {
        if (cls->IsExportSymbol(context)) return true;
    }
    return false;
}

} // namespace otava::symbols
