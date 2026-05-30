// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.type_symbol;

import otava.symbols.compound_type_symbol;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.symbol_table;

namespace otava::symbols {

TypeSymbol::TypeSymbol(Module* module_, SymbolId id_) : ContainerSymbol(module_, id_)
{ 
}

TypeSymbol::TypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : ContainerSymbol(module_, id_, name_)
{
}

TypeSymbol* TypeSymbol::RemoveDerivations(Derivations derivations, Context* context)
{
    if (IsPointerType()) return nullptr;
    return this;
}

bool TypeSymbol::IsAutoTypeSymbol() const noexcept
{
    if (IsFundamentalTypeSymbol())
    {
        const FundamentalTypeSymbol* fundamentalTypeSymbol = static_cast<const FundamentalTypeSymbol*>(this);
        return fundamentalTypeSymbol->IsAutoTypeSymbol();
    }
    return false;
}

bool TypeSymbol::IsPointerType() const noexcept
{
    if (IsCompoundTypeSymbol())
    {
        const CompoundTypeSymbol* compoundTypeSymbol = static_cast<const CompoundTypeSymbol*>(this);
        return otava::symbols::PointerCount(compoundTypeSymbol->GetDerivations()) > 0;
    }
    return false;
}

bool TypeSymbol::IsArrayType() const noexcept
{
    return IsArrayTypeSymbol();
}

bool TypeSymbol::IsConstType() const noexcept
{
    if (IsCompoundTypeSymbol())
    {
        const CompoundTypeSymbol* compoundTypeSymbol = static_cast<const CompoundTypeSymbol*>(this);
        return HasDerivation(compoundTypeSymbol->GetDerivations(), Derivations::constDerivation);
    }
    return false;
}

bool TypeSymbol::IsLValueRefType() const noexcept
{
    if (IsCompoundTypeSymbol())
    {
        const CompoundTypeSymbol* compoundTypeSymbol = static_cast<const CompoundTypeSymbol*>(this);
        return HasDerivation(compoundTypeSymbol->GetDerivations(), Derivations::lvalueRefDerivation);
    }
    return false;
}

bool TypeSymbol::IsRValueRefType() const noexcept
{
    if (IsCompoundTypeSymbol())
    {
        const CompoundTypeSymbol* compoundTypeSymbol = static_cast<const CompoundTypeSymbol*>(this);
        return HasDerivation(compoundTypeSymbol->GetDerivations(), Derivations::rvalueRefDerivation);
    }
    return false;
}

bool TypeSymbol::IsReferenceType() const noexcept
{
    return IsLValueRefType() || IsRValueRefType();
}

otava::intermediate::Type* TypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("IRTYPE not implemented for " + SymbolKindStr(Kind()), fullSpan, context);
    return nullptr;
}

TypeSymbol* TypeSymbol::AddConst(Context* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    Derivations derivations = Derivations::constDerivation;
    return symbolTable->MakeCompoundType(this, derivations, context);
}

TypeSymbol* TypeSymbol::RemoveConst(Context* context)
{
    if (IsCompoundTypeSymbol())
    {
        SymbolTable* symbolTable = context->GetSymbolTable();
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(this);
        return symbolTable->MakeCompoundType(GetBaseType(), otava::symbols::RemoveConst(compoundTypeSymbol->GetDerivations()), context);
    }
    return this;
}

TypeSymbol* TypeSymbol::AddPointer(Context* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    Derivations derivations = Derivations::none;
    derivations = otava::symbols::SetPointerCount(derivations, 1);
    return symbolTable->MakeCompoundType(this, derivations, context);
}

TypeSymbol* TypeSymbol::RemovePointer(Context* context)
{
    if (IsCompoundTypeSymbol())
    {
        SymbolTable* symbolTable = context->GetSymbolTable();
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(this);
        return symbolTable->MakeCompoundType(GetBaseType(), otava::symbols::RemovePointer(compoundTypeSymbol->GetDerivations()), context);
    }
    return this;
}

TypeSymbol* TypeSymbol::AddLValueRef(Context* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    Derivations derivations = Derivations::lvalueRefDerivation;
    return symbolTable->MakeCompoundType(this, derivations, context);
}

TypeSymbol* TypeSymbol::RemoveLValueRef(Context* context)
{
    if (IsCompoundTypeSymbol())
    {
        SymbolTable* symbolTable = context->GetSymbolTable();
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(this);
        return symbolTable->MakeCompoundType(GetBaseType(), otava::symbols::RemoveLValueRef(compoundTypeSymbol->GetDerivations()), context);
    }
    return this;
}

TypeSymbol* TypeSymbol::AddRValueRef(Context* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    Derivations derivations = Derivations::rvalueRefDerivation;
    return symbolTable->MakeCompoundType(this, derivations, context);
}

TypeSymbol* TypeSymbol::RemoveRValueRef(Context* context)
{
    if (IsCompoundTypeSymbol())
    {
        SymbolTable* symbolTable = context->GetSymbolTable();
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(this);
        return symbolTable->MakeCompoundType(GetBaseType(), otava::symbols::RemoveRValueRef(compoundTypeSymbol->GetDerivations()), context);
    }
    return this;
}

TypeSymbol* TypeSymbol::RemoveReference(Context* context)
{
    if (IsLValueRefType())
    {
        return RemoveLValueRef(context);
    }
    else if (IsRValueRefType())
    {
        return RemoveRValueRef(context);
    }
    else
    {
        return this;
    }
}

TypeSymbol* TypeSymbol::RemoveRefOrPtr(Context* context)
{
    if (IsReferenceType())
    {
        return PlainType(context);
    }
    else if (IsPointerType())
    {
        return RemovePointer(context);
    }
    return this;
}

NestedTypeSymbol::NestedTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_)
{
}

NestedTypeSymbol::NestedTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_)
{
}

DependentTypeSymbol::DependentTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_)
{
}

DependentTypeSymbol::DependentTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_)
{
}

} // namespace otava::symbols
