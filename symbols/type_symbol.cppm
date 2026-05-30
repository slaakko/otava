// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.type_symbol;

import std;
import otava.symbols.derivations;
import otava.symbols.container_symbol;
import otava.intermediate.types;

export namespace otava::symbols {

class Emitter;

class TypeSymbol : public ContainerSymbol
{
public:
    TypeSymbol(Module* module_, SymbolId id_);
    TypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    virtual bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType) const noexcept { return true; }
    virtual TypeSymbol* GetBaseType() const noexcept { return const_cast<TypeSymbol*>(this); }
    inline bool IsCompoundType() const noexcept { return Kind() == SymbolKind::compoundTypeSymbol; }
    bool IsAutoTypeSymbol() const noexcept;
    bool IsPointerType() const noexcept;
    bool IsArrayType() const noexcept;
    bool IsConstType() const noexcept;
    bool IsLValueRefType() const noexcept;
    bool IsRValueRefType() const noexcept;
    bool IsReferenceType() const noexcept;
    virtual TypeSymbol* PlainType(Context* context) { return this; }
    virtual TypeSymbol* FinalType(const soul::ast::FullSpan& fullSpan, Context* context) { return this; }
    virtual TypeSymbol* DirectType(Context* context) { return this; }
    TypeSymbol* AddConst(Context* context);
    TypeSymbol* RemoveConst(Context* context);
    TypeSymbol* AddPointer(Context* context);
    TypeSymbol* RemovePointer(Context* context);
    TypeSymbol* AddLValueRef(Context* context);
    TypeSymbol* RemoveLValueRef(Context* context);
    TypeSymbol* AddRValueRef(Context* context);
    TypeSymbol* RemoveRValueRef(Context* context);
    TypeSymbol* RemoveReference(Context* context);
    TypeSymbol* RemoveRefOrPtr(Context* context);
    virtual bool HasBaseClass(TypeSymbol* baseClass, int& distance, Context* context) const noexcept { return false; }
    virtual bool IsVoidType() const noexcept { return false; }
    virtual bool IsBoolType() const noexcept { return false; }
    virtual bool IsIntType() const noexcept { return false; }
    virtual bool IsUnsignedShortType() const noexcept { return false; }
    virtual bool IsDoubleType() const noexcept { return false; }
    virtual bool IsFloatType() const noexcept { return false; }
    virtual bool IsNullPtrType() const noexcept { return false; }
    virtual bool IsVoidPtrType() const noexcept { return false; }
    virtual bool IsIntegralType() const noexcept { return false; }
    virtual bool IsPolymorphic() const noexcept { return false; }
    virtual bool IsFunctionType() const noexcept { return false; }
    virtual bool IsBasicStringCharType(Context* context) noexcept { return false; }
    virtual bool IsBasicStringChar16Type(Context* context) noexcept { return false; }
    virtual bool IsBasicStringChar32Type(Context* context) noexcept { return false; }
    inline bool IsConstCharPtrType() const noexcept
    {
        return IsConstType() &&
            IsPointerType() &&
            PointerCount() == 1 &&
            (GetBaseType()->IsCharTypeSymbol() || GetBaseType()->IsChar8TypeSymbol());
    }
    inline bool IsConstChar16PtrType() const noexcept { return IsConstType() && IsPointerType() && PointerCount() == 1 && GetBaseType()->IsChar16TypeSymbol(); }
    inline bool IsConstChar32PtrType() const noexcept { return IsConstType() && IsPointerType() && PointerCount() == 1 && GetBaseType()->IsChar32TypeSymbol(); }
    inline bool IsFunctionPtrType() noexcept { return IsPointerType() && PointerCount() == 1 && GetBaseType()->IsFunctionType(); }
    virtual int Rank() const noexcept { return -1; }
    virtual bool IsSignedIntegerType() const noexcept { return false; }
    virtual bool IsUnsignedIntegerType() const noexcept { return false; }
    virtual int PointerCount() const noexcept { return 0; }
    virtual otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual Derivations GetDerivations() const noexcept { return Derivations::none; }
    virtual TypeSymbol* RemoveDerivations(Derivations sourceDerivations, Context* context);
};

class NestedTypeSymbol : public TypeSymbol
{
public:
    NestedTypeSymbol(Module* module_, SymbolId id_);
    NestedTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
};

class DependentTypeSymbol : public TypeSymbol
{
public:
    DependentTypeSymbol(Module* module_, SymbolId id_);
    DependentTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
private:
};

} // namespace otava::symbols
