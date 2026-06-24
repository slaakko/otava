// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.variable_symbol;

import otava.symbols.alias_type_symbol;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.templates;
import otava.symbols.type_resolver;
import otava.symbols.type_symbol;
import otava.symbols.writer;
import otava.symbols.reader;
import util.sha1;

namespace otava::symbols {

VariableSymbol::VariableSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), level(0), foundFromParent(false), nodeId(-1), temporary(false), value(nullptr), global(nullptr), layoutIndex(-1), 
    declaredType(nullptr), declaredTypeId(zeroSymbolId), initializerType(nullptr), initializerTypeId(zeroSymbolId), contentFetched(false), valueId(zeroSymbolId),
    index(-1)
{
}

VariableSymbol::VariableSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), level(0), foundFromParent(false), nodeId(-1), temporary(false), value(nullptr), global(nullptr), layoutIndex(-1),
    declaredType(nullptr), declaredTypeId(zeroSymbolId), initializerType(nullptr), initializerTypeId(zeroSymbolId), contentFetched(false), valueId(zeroSymbolId),
    index(-1)
{
    if (Name() == "SEEK_SET")
    {
        int x = 0;
    }
}

bool VariableSymbol::IsLocalVariable(Context* context)
{
    return Parent(context)->IsFunctionSymbol() || Parent(context)->IsBlockSymbol();
}

bool VariableSymbol::IsMemberVariable(Context* context)
{
    return Parent(context)->IsClassTypeSymbol();
}

bool VariableSymbol::IsGlobalVariable(Context* context) 
{
    return Parent(context)->IsNamespaceSymbol();
}

bool VariableSymbol::IsStatic() const noexcept
{
    return (GetDeclarationFlags() & DeclarationFlags::staticFlag) != DeclarationFlags::none;
}

TypeSymbol* VariableSymbol::GetDeclaredType(Context* context) 
{
    if (declaredType)
    {
        return declaredType;
    }
    if (IsReadOnly() && declaredTypeId != zeroSymbolId)
    {
        GetContent(context);
    }
    return declaredType;
}

std::string VariableSymbol::IrName(Context* context) const
{
    std::string irName = "variable_";
    irName.append(Name()).append("_");
    if (IsStatic())
    {
        irName.append(util::GetSha1MessageDigest(FullName(context) + context->GetBoundCompileUnit()->Id()));
    }
    else
    {
        irName.append(util::GetSha1MessageDigest(FullName(context)));
    }
    return irName;
}

void VariableSymbol::SetDeclaredType(TypeSymbol* declaredType_, Context* context) noexcept
{
    declaredType = declaredType_;
    if (declaredType->GetModule() != context->GetModule())
    {
        context->GetModule()->GetSymbolTable()->AddImportedSymbol(declaredType->Id(), declaredType->GetModule()->Id());
    }
}

TypeSymbol* VariableSymbol::GetInitializerType(Context* context) 
{
    if (initializerType)
    {
        return initializerType;
    }
    if (IsReadOnly() && initializerTypeId != zeroSymbolId)
    {
        GetContent(context);
    }
    return initializerType;
}

void VariableSymbol::SetInitializerType(TypeSymbol* initializerType_, Context* context) noexcept
{
    initializerType = initializerType_;
    if (initializerType && initializerType->GetModule() != context->GetModule())
    {
        context->GetModule()->GetSymbolTable()->AddImportedSymbol(initializerType->Id(), initializerType->GetModule()->Id());
    }
}

TypeSymbol* VariableSymbol::GetType(Context* context) 
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    if (declaredType->GetBaseType(context)->IsAutoTypeSymbol())
    {
        return initializerType;
    }
    else
    {
        return declaredType;
    }
}

TypeSymbol* VariableSymbol::GetReferredType(Context* context) 
{
    TypeSymbol* referredType = GetType(context);
    if (!referredType)
    {
        return nullptr;
    }
    while (referredType->IsAliasTypeSymbol())
    {
        AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(referredType);
        referredType = aliasType->ReferredType(context);
    }
    return referredType;
}

Value* VariableSymbol::GetValue(Context* context)
{
    if (value)
    {
        return value;
    }
    if (IsReadOnly() && valueId != zeroSymbolId)
    {
        GetContent(context);
    }
    return value;
}

void VariableSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    if (declaredType)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(declaredType->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    if (initializerType)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(initializerType->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    if (value)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(value->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
}

void VariableSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    declaredTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    initializerTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    valueId = SymbolId(reader.CurrentReader().ReadUInt());
}

void VariableSymbol::GetContent(Context* context)
{
    if (contentFetched) return;
    contentFetched = true;
    if (declaredTypeId != zeroSymbolId)
    {
        declaredType = GetModule()->GetSymbolTable()->GetTypeSymbol(declaredTypeId, context);
        if (!declaredType)
        {
            ThrowException("variable type id " + std::to_string(ToUnderlying(declaredTypeId)) + " not found", GetFullSpan(), context);
        }
    }
    if (initializerTypeId != zeroSymbolId)
    {
        initializerType = GetModule()->GetSymbolTable()->GetTypeSymbol(initializerTypeId, context);
        if (!initializerType)
        {
            ThrowException("variable inistializer type id " + std::to_string(ToUnderlying(initializerTypeId)) + " not found", GetFullSpan(), context);
        }
    }
    if (valueId != zeroSymbolId)
    {
        value = GetModule()->GetSymbolTable()->GetValue(valueId, context);
    }
}

ParameterSymbol::ParameterSymbol(Module* module_, SymbolId id_) :
    Symbol(module_, id_), defaultValue(nullptr), type(nullptr), typeId(zeroSymbolId), parameterKind(ParameterKind::regular)
{
}

ParameterSymbol::ParameterSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), defaultValue(nullptr), type(nullptr), typeId(zeroSymbolId), parameterKind(ParameterKind::regular)
{
}

void ParameterSymbol::SetDefaultValue(otava::ast::Node* defaultValue_) noexcept 
{ 
    if (defaultValue_)
    {
        defaultValue.reset(defaultValue_->Clone());
    }
}

TypeSymbol* ParameterSymbol::GetType(Context* context) 
{
    if (type)
    {
        return type;
    }
    if (IsReadOnly() && typeId != zeroSymbolId)
    {
        type = GetModule()->GetSymbolTable()->GetTypeSymbol(typeId, context);
        if (!type)
        {
            ThrowException("parameter '" + Name() + "' type id " + std::to_string(ToUnderlying(typeId)) + " not found", GetFullSpan(), context);
        }
    }
    return type;
}

void ParameterSymbol::SetType(TypeSymbol* type_, Context* context) noexcept
{
    type = type_;
    if (type->GetModule() != context->GetModule())
    {
        context->GetModule()->GetSymbolTable()->AddImportedSymbol(type->Id(), type->GetModule()->Id());
    }
}

TypeSymbol* ParameterSymbol::GetReferredType(Context* context)
{
    TypeSymbol* t = GetType(context);
    TypeSymbol* referredType = t->GetBaseType(context)->DirectType(context)->FinalType(t->GetFullSpan(), context);
    if (context->GetFlag(ContextFlags::resolveNestedTypes) && referredType->IsNestedTypeSymbol())
    {
        if (context->TemplateParameterMap())
        {
            if (referredType->Parent(context)->IsTemplateParameterSymbol())
            {
                TemplateParameterSymbol* templateParam = static_cast<TemplateParameterSymbol*>(referredType->Parent(context));
                auto it = context->TemplateParameterMap()->find(templateParam);
                if (it != context->TemplateParameterMap()->end())
                {
                    TypeSymbol* tp = it->second;
                    Symbol* symbol = tp->GetScope()->Lookup(referredType->Name(), 
                        SymbolGroupKind::aliasSymbolGroup | SymbolGroupKind::classSymbolGroup | SymbolGroupKind::enumSymbolGroup, ScopeLookup::thisScope,
                        soul::ast::FullSpan(), context, LookupFlags::none);
                    if (symbol && symbol->IsTypeSymbol())
                    {
                        referredType = static_cast<TypeSymbol*>(symbol);
                    }
                }
            }
        }
    }
    while (referredType->IsAliasTypeSymbol())
    {
        AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(referredType);
        referredType = aliasType->ReferredType(context);
        if (context->GetFlag(ContextFlags::resolveDependentTypes) && referredType->IsDependentTypeSymbol())
        {
            DependentTypeSymbol* dependentType = static_cast<DependentTypeSymbol*>(referredType);
            referredType = ResolveType(dependentType->GetNode(), DeclarationFlags::none, context);
        }
        else if (context->GetFlag(ContextFlags::resolveNestedTypes) && referredType->IsNestedTypeSymbol())
        {
            if (context->TemplateParameterMap())
            {
                if (referredType->Parent(context)->IsTemplateParameterSymbol())
                {
                    TemplateParameterSymbol* templateParam = static_cast<TemplateParameterSymbol*>(referredType->Parent(context));
                    auto it = context->TemplateParameterMap()->find(templateParam);
                    if (it != context->TemplateParameterMap()->end())
                    {
                        TypeSymbol* tp = it->second;
                        Symbol* symbol = tp->GetScope()->Lookup(referredType->Name(), 
                            SymbolGroupKind::aliasSymbolGroup | SymbolGroupKind::classSymbolGroup | SymbolGroupKind::enumSymbolGroup, ScopeLookup::thisScope,
                            soul::ast::FullSpan(), context, LookupFlags::none);
                        if (symbol && symbol->IsTypeSymbol())
                        {
                            referredType = static_cast<TypeSymbol*>(symbol);
                        }
                    }
                }
            }
        }
    }
    if (t->IsCompoundType())
    {
        referredType = context->GetSymbolTable()->MakeCompoundType(referredType, t->GetDerivations(), context);
    }
    if (context->GetFlag(ContextFlags::resolveDependentTypes) && referredType->IsDependentTypeSymbol())
    {
        DependentTypeSymbol* dependentType = static_cast<DependentTypeSymbol*>(referredType);
        TypeSymbol* type = ResolveType(dependentType->GetNode(), DeclarationFlags::none, context);
        referredType = type;
        while (referredType->IsAliasTypeSymbol())
        {
            AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(referredType);
            referredType = aliasType->ReferredType(context);
        }
    }
    return referredType;
}

void ParameterSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(type->Id()));
    otava::symbols::WriteNode(writer, defaultValue.get(), astNodeHeader);
}

void ParameterSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    typeId = SymbolId(reader.CurrentReader().ReadUInt());
    defaultValue = otava::symbols::ReadNode(reader, GetModule(), astNodeHeader);
}

} // namespace otava::symbols
