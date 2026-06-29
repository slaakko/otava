// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.compound_type_symbol;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.reader;
import otava.symbols.writer;

namespace otava::symbols {

std::string MakeCompoundTypeName(TypeSymbol* baseType, Derivations derivations)
{
    std::string name;
    if (HasDerivation(derivations, Derivations::constDerivation))
    {
        name.append("const ");
    }
    if (HasDerivation(derivations, Derivations::volatileDerivation))
    {
        name.append("volatile ");
    }
    name.append(baseType->Name());
    int pointerCount = PointerCount(derivations);
    if (pointerCount > 0)
    {
        if (baseType->PtrIndex() == -1)
        {
            name.append(pointerCount, '*');
        }
        else
        {
            std::string ptrStr(pointerCount, '*');
            name.insert(baseType->PtrIndex(), ptrStr);
        }
    }
    if (HasDerivation(derivations, Derivations::lvalueRefDerivation))
    {
        if (baseType->PtrIndex() == -1)
        {
            name.append("&");
        }
        else
        {
            name.insert(baseType->PtrIndex(), "&");
        }
    }
    else if (HasDerivation(derivations, Derivations::rvalueRefDerivation))
    {
        if (baseType->PtrIndex() == -1)
        {
            name.append("&&");
        }
        else
        {
            name.insert(baseType->PtrIndex(), "&&");
        }
    }
    return name;
}

CompoundTypeKey::CompoundTypeKey() : baseTypeId(zeroSymbolId), derivations(Derivations::none)
{
}

CompoundTypeKey::CompoundTypeKey(SymbolId baseTypeId_, Derivations derivations_) : baseTypeId(baseTypeId_), derivations(derivations_)
{
}

void CompoundTypeKey::Write(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(baseTypeId));
    otava::symbols::Write(writer, derivations);
}

void CompoundTypeKey::Read(Reader& reader)
{
    baseTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    otava::symbols::Read(reader, derivations);
}

CompoundTypeSymbol::CompoundTypeSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), baseType(nullptr), derivations(Derivations::none), baseTypeId(zeroSymbolId), irId(id_)
{
}

CompoundTypeSymbol::CompoundTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), baseType(nullptr), derivations(Derivations::none), baseTypeId(zeroSymbolId), irId(id_)
{
}

TypeSymbol* CompoundTypeSymbol::GetBaseType(Context* context) 
{
    if (baseType)
    {
        return baseType;
    }
    if (IsReadOnly() && baseTypeId != zeroSymbolId)
    {
        baseType = GetModule()->GetSymbolTable()->GetTypeSymbol(baseTypeId, context);
    }
    return baseType;
}

void CompoundTypeSymbol::SetBaseType(TypeSymbol* baseType_) noexcept
{
    baseType = baseType_;
    if (baseType->GetModule() != GetModule())
    {
        GetModule()->GetSymbolTable()->AddImportedSymbol(baseType->Id(), baseType->GetModule()->Id());
    }
}

TypeSymbol* CompoundTypeSymbol::PlainType(Context* context) 
{
    Derivations plainDerivations = Plain(derivations);
    return context->GetSymbolTable()->MakeCompoundType(GetBaseType(context), plainDerivations, context);
}

int CompoundTypeSymbol::PointerCount() const noexcept
{
    return otava::symbols::PointerCount(GetDerivations());
}

TypeSymbol* CompoundTypeSymbol::RemoveDerivations(Derivations sourceDerivations, Context* context)
{
    Derivations resultDerivations = Derivations::none;
    if (!HasDerivation(sourceDerivations, Derivations::constDerivation) && HasDerivation(derivations, Derivations::constDerivation))
    {
        resultDerivations = resultDerivations | Derivations::constDerivation;
    }
    int pointerDiff = otava::symbols::PointerCount(derivations) - otava::symbols::PointerCount(sourceDerivations);
    if (pointerDiff != 0)
    {
        resultDerivations = otava::symbols::SetPointerCount(resultDerivations, pointerDiff);
    }
    if (!HasDerivation(sourceDerivations, Derivations::lvalueRefDerivation) && HasDerivation(derivations, Derivations::lvalueRefDerivation))
    {
        resultDerivations = resultDerivations | Derivations::lvalueRefDerivation;
    }
    else if (!HasDerivation(sourceDerivations, Derivations::rvalueRefDerivation) && HasDerivation(derivations, Derivations::rvalueRefDerivation))
    {
        resultDerivations = resultDerivations | Derivations::rvalueRefDerivation;
    }
    return context->GetSymbolTable()->MakeCompoundType(baseType, resultDerivations, context);
}

bool CompoundTypeSymbol::IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType, Context* context) const 
{
    const TypeSymbol* thisSymbol = this;
    if (visited.find(thisSymbol) != visited.end()) return true;
    visited.insert(thisSymbol);
    if (PointerCount() > 0 || HasDerivation(derivations, Derivations::refMask))
    {
        return true;
    }
    return baseType->IsComplete(visited, incompleteType, context);
}

TypeSymbol* CompoundTypeSymbol::FinalType(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (IsReadOnly() && !baseType && baseTypeId != zeroSymbolId)
    {
        baseType = GetModule()->GetSymbolTable()->GetTypeSymbol(baseTypeId, context);
    }
    if (!baseType)
    {
        ThrowException("CompoundTypeSymbol::FinalType: base type not resolved");
    }
    TypeSymbol* finalBaseType = baseType->FinalType(fullSpan, context);
    return context->GetSymbolTable()->MakeCompoundType(finalBaseType, derivations, context);
}

TypeSymbol* CompoundTypeSymbol::DirectType(Context* context)
{
    if (IsReadOnly() && !baseType && baseTypeId != zeroSymbolId)
    {
        baseType = GetModule()->GetSymbolTable()->GetTypeSymbol(baseTypeId, context);
    }
    if (!baseType)
    {
        ThrowException("CompoundTypeSymbol::FinalType: base type not resolved");
    }
    TypeSymbol* directBaseType = baseType->DirectType(context);
    return context->GetSymbolTable()->MakeCompoundType(directBaseType, derivations, context);
}

void CompoundTypeSymbol::ResolveBaseType(Context* context)
{
    TypeSymbol* baseType = GetBaseType(context);
    if (!baseType)
    {
        ThrowException("compound type symbol: base type not resolved", GetFullSpan(), context);
    }
}

std::string CompoundTypeSymbol::FullName(Context* context) const
{
    std::string fullName;;
    if (HasDerivation(derivations, Derivations::constDerivation))
    {
        fullName.append("const ");
    }
    if (HasDerivation(derivations, Derivations::volatileDerivation))
    {
        fullName.append("volatile ");
    }
    const_cast<CompoundTypeSymbol*>(this)->ResolveBaseType(context);
    fullName.append(baseType->FullName(context));
    int pointerCount = PointerCount();
    if (pointerCount > 0)
    {
        if (baseType->PtrIndex() == -1)
        {
            fullName.append(pointerCount, '*');
        }
        else
        {
            std::string ptrStr(pointerCount, '*');
            fullName.insert(baseType->PtrIndex(), ptrStr);
        }
    }
    if (HasDerivation(derivations, Derivations::lvalueRefDerivation))
    {
        if (baseType->PtrIndex() == -1)
        {
            fullName.append("&");
        }
        else
        {
            fullName.insert(baseType->PtrIndex(), "&");
        }
    }
    else if (HasDerivation(derivations, Derivations::rvalueRefDerivation))
    {
        if (baseType->PtrIndex() == -1)
        {
            fullName.append("&&");
        }
        else
        {
            fullName.insert(baseType->PtrIndex(), "&&");
        }
    }
    return fullName;
}

TypeSymbol* CompoundTypeSymbol::Unify(TypeSymbol* argType, Context* context)
{
    TypeSymbol* newBaseType = baseType->Unify(argType->GetBaseType(context), context);
    return context->GetSymbolTable()->MakeCompoundType(newBaseType, UnifyDerivations(derivations, argType->GetDerivations()), context);
}

TypeSymbol* CompoundTypeSymbol::UnifyTemplateArgumentType(const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>,
    TemplateParamEqual>& templateParameterMap, const soul::ast::FullSpan& fullSpan, Context* context)
{
    TypeSymbol* newBaseType = baseType->UnifyTemplateArgumentType(templateParameterMap, fullSpan, context);
    return context->GetSymbolTable()->MakeCompoundType(newBaseType, GetDerivations(), context);
}

bool CompoundTypeSymbol::IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const
{
    const Symbol* thisSymbol = this;
    if (visited.find(thisSymbol) == visited.end())
    {
        visited.insert(thisSymbol);
        return baseType->IsTemplateParameterInstantiation(context, visited);
    }
    return false;
}

void CompoundTypeSymbol::Write(Writer& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(baseType->Id()));
    otava::symbols::Write(writer, derivations);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(irId));
}

void CompoundTypeSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    baseTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    otava::symbols::Read(reader, derivations);
    irId = SymbolId(reader.CurrentReader().ReadUInt());
}

otava::intermediate::Type* CompoundTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    SymbolId irId = IrId();
    otava::intermediate::Type* type = emitter.GetType(irId);
    if (!type)
    {
        TypeSymbol* btype = GetBaseType(context);
        if (!btype)
        {
            ThrowException("cannot obtain base type for type '" + Name() + "' because it is incomplete at this point", fullSpan, context);
        }
        type = btype->IrType(emitter, fullSpan, context);
        int pointerCount = otava::symbols::PointerCount(derivations);
        for (int i = 0; i < pointerCount; ++i)
        {
            type = emitter.MakePtrType(type);
        }
        if (otava::symbols::HasDerivation(derivations, Derivations::lvalueRefDerivation))
        {
            type = emitter.MakePtrType(type);
        }
        if (otava::symbols::HasDerivation(derivations, Derivations::rvalueRefDerivation))
        {
            type = emitter.MakePtrType(type);
        }
        emitter.SetType(irId, type);
    }
    return type;
}

} // namespace otava::symbols
