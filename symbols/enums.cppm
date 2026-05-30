// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.enums;

import std;
import otava.symbols.type_symbol;
import otava.ast.node;

export namespace otava::symbols {

class TypeSymbol;

enum class EnumTypeKind : std::uint8_t
{
    enum_, enumClass, enumStruct
};

class EnumeratedTypeSymbol : public TypeSymbol
{
public:
    EnumeratedTypeSymbol(Module* module_, SymbolId id_);
    EnumeratedTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsValidDeclarationScope(ScopeKind scopeKind) const noexcept override;
    TypeSymbol* UnderlyingType() const;
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    TypeSymbol* underlyingType;
};

class Context;

void BeginEnumType(otava::ast::Node* node, Context* context);
void AddEnumerators(otava::ast::Node* node, Context* context);
void EndEnumType(otava::ast::Node* node, Context* context);
void ProcessEnumForwardDeclaration(otava::ast::Node* node, Context* context);
void BindEnumType(EnumeratedTypeSymbol* enumType, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
