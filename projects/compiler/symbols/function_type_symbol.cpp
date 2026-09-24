// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_type_symbol;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.type_compare;
import otava.symbols.writer;
import otava.symbols.reader;
import otava.symbols.symbol_table;

namespace otava::symbols {

std::string MakeFunctionTypeName(TypeSymbol* returnType, const std::vector<TypeSymbol*>& parameterTypes, int& ptrIndex, bool makePtrType)
{
    std::string name = returnType->Name();
    name.append(" (");
    ptrIndex = int(name.length());
    if (makePtrType)
    {
        name.append(1, '*');
    }
    name.append(")(");
    bool first = true;
    for (TypeSymbol* parameterType : parameterTypes)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            name.append(", ");
        }
        name.append(parameterType->Name());
    }
    name.append(")");
    return name;
}

FunctionTypeSymbolKey::FunctionTypeSymbolKey() : returnTypeId(zeroSymbolId), parameterTypeIds()
{
}

void FunctionTypeSymbolKey::Write(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(returnTypeId));
    Cardinality count = Cardinality(parameterTypeIds.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (SymbolId parameterTypeId : parameterTypeIds)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(parameterTypeId));
    }
}

void FunctionTypeSymbolKey::Read(Reader& reader)
{
    returnTypeId = SymbolId(reader.CurrentReader().ReadULong());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < ToIndex(count); ++i)
    {
        SymbolId parameterTypeId = SymbolId(reader.CurrentReader().ReadULong());
        parameterTypeIds.push_back(parameterTypeId);
    }
}

size_t FunctionTypeSymbolKeyHash::operator()(const FunctionTypeSymbolKey& key) const noexcept
{
    size_t hashCode = std::hash<std::uint64_t>()(ToUnderlying(key.returnTypeId));
    Cardinality count = Cardinality(key.parameterTypeIds.size());
    for (Index i = Index(0); i < ToIndex(count); ++i)
    {
        SymbolId paramTypeId = key.parameterTypeIds[ToUnderlying(i)];
        size_t paramTypeHashCode = std::hash<std::uint64_t>()(ToUnderlying(paramTypeId));
        hashCode ^= (paramTypeHashCode << ToUnderlying(i + Index(1))) | (paramTypeHashCode >> ToUnderlying(ToIndex(count) - i + Index(1)));
    }
    return hashCode;
}

bool FunctionTypeSymbolKeyEqual::operator()(const FunctionTypeSymbolKey& left, const FunctionTypeSymbolKey& right) const noexcept
{
    if (left.returnTypeId != right.returnTypeId) return false;
    if (left.parameterTypeIds.size() != right.parameterTypeIds.size()) return false;
    Cardinality count = Cardinality(left.parameterTypeIds.size());
    for (Index i = Index(0); i < ToIndex(count); ++i)
    {
        SymbolId leftParamTypeId = left.parameterTypeIds[ToUnderlying(i)];
        SymbolId righParamTypeId = right.parameterTypeIds[ToUnderlying(i)];
        if (leftParamTypeId != righParamTypeId) return false;
    }
    return true;
}

FunctionTypeSymbol::FunctionTypeSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), returnType(nullptr), returnTypeId(zeroSymbolId), ptrIndex(-1), contentFetched(false), irId(zeroSymbolId)
{
}

FunctionTypeSymbol::FunctionTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), returnType(nullptr), returnTypeId(zeroSymbolId), ptrIndex(-1), contentFetched(false), irId(zeroSymbolId)
{
}

FunctionTypeSymbolKey FunctionTypeSymbol::IrKey(Context* context)
{
    FunctionTypeSymbolKey irKey;
    irKey.returnTypeId = ReturnType(context)->IrId(context);
    for (TypeSymbol* paramType : ParameterTypes(context))
    {
        irKey.parameterTypeIds.push_back(paramType->IrId(context));
    }
    return irKey;
}

SymbolId FunctionTypeSymbol::IrId(Context* context) noexcept 
{
    if (irId != zeroSymbolId)
    {
        return irId;
    }
    context->ResetHasUnresolvedForwardDeclaration();
    SymbolId stabIrId = context->GetSymbolTable()->GetIrId(this, context);
    if (stabIrId == zeroSymbolId)
    {
        stabIrId = context->GetNextSymbolId(SymbolKind::functionTypeSymbol);
    }
    if (!context->HasUnresolvedForwardDeclaration())
    {
        SetIrId(stabIrId);
        context->GetSymbolTable()->MapIrId(this, stabIrId, context);
    }
    else
    {
        context->GetSymbolTable()->MapIrId(this, stabIrId, context);
    }
    return stabIrId;
}

bool FunctionTypeSymbol::HasForwardClassDeclarationSymbol(Context* context) 
{
    if (ReturnType(context)->HasForwardClassDeclarationSymbol(context)) return true;
    for (TypeSymbol* paramType : ParameterTypes(context))
    {
        if (paramType->HasForwardClassDeclarationSymbol(context)) return true;
    }
    return false;
}

void FunctionTypeSymbol::Write(Writer& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(returnType->Id()));
    Cardinality count = Cardinality(parameterTypes.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (TypeSymbol* parameterType : parameterTypes)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(parameterType->Id()));
    }
    writer.GetBinaryStreamWriter().Write(ptrIndex);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(irId));
}

void FunctionTypeSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    returnTypeId = SymbolId(reader.CurrentReader().ReadULong());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < ToIndex(count); ++i)
    {
        SymbolId parameterTypeId = SymbolId(reader.CurrentReader().ReadULong());
        parameterTypeIds.push_back(parameterTypeId);
    }
    ptrIndex = reader.CurrentReader().ReadInt();
    irId = SymbolId(reader.CurrentReader().ReadULong());
}

TypeSymbol* FunctionTypeSymbol::ReturnType(Context* context)
{
    if (returnType)
    {
        return returnType;
    }
    if (IsReadOnly() && returnTypeId != zeroSymbolId)
    {
        GetContent(context);
    }
    return returnType;
}

void FunctionTypeSymbol::SetReturnType(TypeSymbol* returnType_, Context* context) noexcept
{
    returnType = returnType_;
    if (returnType && returnType->GetModule() != GetModule())
    {
        GetModule()->GetSymbolTable()->AddImportedSymbol(returnType->Id(), returnType->GetModule());
    }
}

void FunctionTypeSymbol::AddParameterType(TypeSymbol* parameterType, Context* context)
{
    parameterTypes.push_back(parameterType);
    if (parameterType->GetModule() != GetModule())
    {
        GetModule()->GetSymbolTable()->AddImportedSymbol(parameterType->Id(), parameterType->GetModule());
    }
}

const std::vector<TypeSymbol*>& FunctionTypeSymbol::ParameterTypes(Context* context)
{
    if (IsReadOnly())
    {
        GetContent(context);
    }
    return parameterTypes;
}

void FunctionTypeSymbol::GetContent(Context* context)
{
    if (contentFetched) return;
    contentFetched = true;
    if (returnTypeId != zeroSymbolId)
    {
        returnType = GetModule()->GetSymbolTable()->GetTypeSymbol(returnTypeId, context);
        if (!returnType)
        {
            ThrowException("FunctionTypeSymbol::GetContent(): return type not found");
        }
    }
    for (SymbolId paramTypeId : parameterTypeIds)
    {
        TypeSymbol* paramType = GetModule()->GetSymbolTable()->GetTypeSymbol(paramTypeId, context);
        if (!paramType)
        {
            ThrowException("FunctionTypeSymbol::GetContent():  parameter type not found");
        }
        parameterTypes.push_back(paramType);
    }
}

otava::intermediate::Type* FunctionTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    SymbolId irId = IrId(context);
    otava::intermediate::Type* type = emitter.GetType(irId);
    if (!type)
    {
        std::vector<otava::intermediate::Type*> paramTypes;
        for (TypeSymbol* paramType : ParameterTypes(context))
        {
            paramTypes.push_back(paramType->IrType(emitter, fullSpan, context));
        }
        type = emitter.MakeFunctionType(ReturnType(context)->IrType(emitter, fullSpan, context), paramTypes);
        emitter.SetType(irId, type);
    }
    return type;
}

bool FunctionTypesEqual(FunctionTypeSymbol* left, FunctionTypeSymbol* right, Context* context) noexcept
{
    Cardinality n = Cardinality(left->ParameterTypes(context).size());
    if (n != Cardinality(right->ParameterTypes(context).size())) return false;
    for (Index i = Index(0); i < ToIndex(n); ++i)
    {
        TypeSymbol* leftParamType = left->ParameterTypes(context)[ToUnderlying(i)];
        TypeSymbol* rightParamType = right->ParameterTypes(context)[ToUnderlying(i)];
        if (!TypesEqual(leftParamType, rightParamType, context)) return false;
    }
    return TypesEqual(left->ReturnType(context), right->ReturnType(context), context);
}

} // namespace otava::symbols
