// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.alias_type_symbol;

import std;
import otava.symbols.type_symbol;
import otava.ast.node;

export namespace otava::symbols {

class AliasGroupSymbol;
class Context;

class AliasTypeSymbol : public TypeSymbol
{
public:
    AliasTypeSymbol(Module* module_, SymbolId id_);
    AliasTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    TypeSymbol* ReferredType(Context* context) const;
    inline void SetReferredType(TypeSymbol* referredType_) noexcept { referredType = referredType_; }
    inline AliasGroupSymbol* Group() const noexcept { return group; }
    inline void SetGroup(AliasGroupSymbol* group_) noexcept { group = group_; }
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    SymbolId referredTypeId;
    mutable TypeSymbol* referredType;
    AliasGroupSymbol* group;
};

struct AliasTypeLess
{
    bool operator()(AliasTypeSymbol* left, AliasTypeSymbol* right) const noexcept;
};

void ProcessAliasDeclaration(otava::ast::Node* aliasDeclarationNode, Context* context);

void AddTemporaryTypeAlias(otava::ast::Node* aliasDeclarationNode, Context* context);

void RemoveTemporaryAliasTypeSymbols(Context* context);

} // namespace otava::symbols
