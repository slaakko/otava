// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.type_resolver;

import std;
import otava.ast.node;
import otava.symbols.symbol;
import soul.ast.span;

export namespace otava::symbols {

class Context;
class TypeSymbol;

enum class TypeResolverFlags : std::int32_t
{
    none = 0, dontInstantiate = 1 << 0, dontThrow = 1 << 1
};

constexpr TypeResolverFlags operator|(TypeResolverFlags left, TypeResolverFlags right) noexcept
{
    return TypeResolverFlags(std::int32_t(left) | std::int32_t(right));
}

constexpr TypeResolverFlags operator&(TypeResolverFlags left, TypeResolverFlags right) noexcept
{
    return TypeResolverFlags(std::int32_t(left) & std::int32_t(right));
}

constexpr TypeResolverFlags operator~(TypeResolverFlags flags) noexcept
{
    return TypeResolverFlags(~std::int32_t(flags));
}

bool CheckDuplicateSpecifier(DeclarationFlags flags, DeclarationFlags flag, const std::string& specifierStr, const soul::ast::FullSpan& fullSpan,
    TypeResolverFlags resolverFlags, Context* context);

inline bool CheckDuplicateSpecifier(DeclarationFlags flags, DeclarationFlags flag, const std::string& specifierStr, const soul::ast::FullSpan& fullSpan,
    Context* context)
{
    return CheckDuplicateSpecifier(flags, flag, specifierStr, fullSpan, TypeResolverFlags::none, context);
}

TypeSymbol* ResolveType(otava::ast::Node* node, DeclarationFlags flags, Context* context);
TypeSymbol* ResolveType(otava::ast::Node* node, DeclarationFlags flags, Context* context, TypeResolverFlags resolverFlags);
TypeSymbol* ResolveFwdDeclaredType(TypeSymbol* type, const soul::ast::FullSpan& fullSpan, Context* context);
Symbol* ResolveTypeIdentifier(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
