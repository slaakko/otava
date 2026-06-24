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

namespace otava::symbols {

std::string MakeFunctionTypeName(TypeSymbol* returnType, const std::vector<TypeSymbol*>& parameterTypes, int& ptrIndex, bool makePtrType)
{
    std::string name = returnType->Name();
    name.append(" (");
    ptrIndex = name.length();
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
    returnTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId parameterTypeId = SymbolId(reader.CurrentReader().ReadUInt());
        parameterTypeIds.push_back(parameterTypeId);
    }
}

size_t FunctionTypeSymbolKeyHash::operator()(const FunctionTypeSymbolKey& key) const noexcept
{
    size_t hashCode = std::hash<std::uint32_t>()(ToUnderlying(key.returnTypeId));
    Cardinality count = Cardinality(key.parameterTypeIds.size());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId paramTypeId = key.parameterTypeIds[ToUnderlying(i)];
        size_t paramTypeHashCode = std::hash<std::uint32_t>()(ToUnderlying(paramTypeId));
        hashCode ^= (paramTypeHashCode << ToUnderlying(i + Index(1))) | (paramTypeHashCode >> ToUnderlying(Index(count) - i + Index(1)));
    }
    return hashCode;
}

bool FunctionTypeSymbolKeyEqual::operator()(const FunctionTypeSymbolKey& left, const FunctionTypeSymbolKey& right) const noexcept
{
    if (left.returnTypeId != right.returnTypeId) return false;
    if (left.parameterTypeIds.size() != right.parameterTypeIds.size()) return false;
    Cardinality count = Cardinality(left.parameterTypeIds.size());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId leftParamTypeId = left.parameterTypeIds[ToUnderlying(i)];
        SymbolId righParamTypeId = right.parameterTypeIds[ToUnderlying(i)];
        if (leftParamTypeId != righParamTypeId) return false;
    }
    return true;
}

FunctionTypeSymbol::FunctionTypeSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), returnType(nullptr), returnTypeId(zeroSymbolId), ptrIndex(-1), contentFetched(false)
{
}

FunctionTypeSymbol::FunctionTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), returnType(nullptr), returnTypeId(zeroSymbolId), ptrIndex(-1), contentFetched(false)
{
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
}

void FunctionTypeSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    returnTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId parameterTypeId = SymbolId(reader.CurrentReader().ReadUInt());
        parameterTypeIds.push_back(parameterTypeId);
    }
    ptrIndex = reader.CurrentReader().ReadInt();
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

void FunctionTypeSymbol::AddParameterType(TypeSymbol* parameterType)
{
    parameterTypes.push_back(parameterType);
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
    SymbolId id = Id();
    otava::intermediate::Type* type = emitter.GetType(id);
    if (!type)
    {
        std::vector<otava::intermediate::Type*> paramTypes;
        for (TypeSymbol* paramType : parameterTypes)
        {
            paramTypes.push_back(paramType->IrType(emitter, fullSpan, context));
        }
        type = emitter.MakeFunctionType(returnType->IrType(emitter, fullSpan, context), paramTypes);
        emitter.SetType(id, type);
    }
    return type;
}

bool FunctionTypesEqual(FunctionTypeSymbol* left, FunctionTypeSymbol* right, Context* context) noexcept
{
    Cardinality n = Cardinality(left->ParameterTypes(context).size());
    if (n != Cardinality(right->ParameterTypes(context).size())) return false;
    for (Index i = Index(0); i < Index(n); ++i)
    {
        TypeSymbol* leftParamType = left->ParameterTypes(context)[ToUnderlying(i)];
        TypeSymbol* rightParamType = right->ParameterTypes(context)[ToUnderlying(i)];
        if (!TypesEqual(leftParamType, rightParamType, context)) return false;
    }
    return TypesEqual(left->ReturnType(context), right->ReturnType(context), context);
}

} // namespace otava::symbols
