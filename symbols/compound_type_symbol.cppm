// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.compound_type_symbol;

import otava.symbols.derivations;
import otava.symbols.id;
import otava.symbols.type_symbol;
import std;

export namespace otava::symbols {

std::string MakeCompoundTypeName(TypeSymbol* baseType, Derivations derivations);

struct CompoundTypeKey
{
    CompoundTypeKey();
    CompoundTypeKey(SymbolId baseTypeId_, Derivations derivations_);
    void Write(Writer& writer);
    void Read(Reader& reader);
    SymbolId baseTypeId;
    Derivations derivations;
};

struct CompoundTypeKeyHash
{
    inline size_t operator()(const CompoundTypeKey& key) const noexcept
    {
        return std::hash<std::uint32_t>()(ToUnderlying(key.baseTypeId)) ^ std::hash<std::uint8_t>()(ToUnderlying(key.derivations));
    }
};

struct CompoundTypeKeyEqual
{
    inline size_t operator()(const CompoundTypeKey& left, const CompoundTypeKey& right) const noexcept
    {
        return left.baseTypeId == right.baseTypeId && left.derivations == right.derivations;
    }
};

class CompoundTypeSymbol : public TypeSymbol
{
public:
    CompoundTypeSymbol(Module* module_, SymbolId id_);
    CompoundTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    TypeSymbol* GetBaseType(Context* context) override;
    void SetBaseType(TypeSymbol* baseType_) noexcept;
    TypeSymbol* PlainType(Context* context) override;
    int PointerCount() const noexcept override;
    Derivations GetDerivations() const noexcept override { return derivations; }
    inline void SetDerivations(Derivations derivations_) noexcept { derivations = derivations_; }
    TypeSymbol* RemoveDerivations(Derivations sourceDerivations, Context* context) override;
    bool IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType, Context* context) const override;
    TypeSymbol* FinalType(const soul::ast::FullSpan& fullSpan, Context* context) override;
    TypeSymbol* DirectType(Context* context) override;
    void ResolveBaseType(Context* context);
    std::string FullName(Context* context) const override;
    TypeSymbol* Unify(TypeSymbol* argType, Context* context) override;
    TypeSymbol* UnifyTemplateArgumentType(const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>,
        TemplateParamEqual>& templateParameterMap,
        const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const override;
    void Write(Writer& writer);
    void Read(Reader& reader);
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    TypeSymbol* baseType;
    SymbolId baseTypeId;
    Derivations derivations;
};

} // namespace otava::symbols
