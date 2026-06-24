// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.specialization;

import otava.symbols.writer;
import otava.symbols.reader;
import otava.symbols.type_symbol;

namespace otava::symbols {

SpecializationKey::SpecializationKey() : typeSymbolId(zeroSymbolId), templateArgumentIds()
{
}

void SpecializationKey::Write(Writer& writer)
{
    writer.GetBinaryStreamWriter().Write(ToUnderlying(typeSymbolId));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(Cardinality(templateArgumentIds.size())));
    for (const auto& templateArgumentId : templateArgumentIds)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(templateArgumentId));
    }
}

void SpecializationKey::Read(Reader& reader)
{
    typeSymbolId = SymbolId(reader.CurrentReader().ReadUInt());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        templateArgumentIds.push_back(symbolId);
    }
}

size_t SpecializationKeyHash::operator()(const SpecializationKey& key) const noexcept
{
    size_t hashCode = std::hash<std::uint32_t>()(ToUnderlying(key.typeSymbolId));
    Cardinality count = Cardinality(key.templateArgumentIds.size());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId argId = key.templateArgumentIds[ToUnderlying(i)];
        size_t argHashCode = std::hash<std::uint32_t>()(ToUnderlying(argId));
        hashCode ^= (argHashCode << ToUnderlying(i + Index(1))) | (argHashCode >> ToUnderlying(Index(count) - i + Index(1)));
    }
    return hashCode;
}

bool SpecializationKeyEqual::operator()(const SpecializationKey& left, const SpecializationKey& right) const noexcept
{
    if (left.typeSymbolId != right.typeSymbolId) return false;
    if (left.templateArgumentIds.size() != right.templateArgumentIds.size()) return false;
    Cardinality count = Cardinality(left.templateArgumentIds.size());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId leftArgId = left.templateArgumentIds[ToUnderlying(i)];
        SymbolId rightArgId = right.templateArgumentIds[ToUnderlying(i)];
        if (leftArgId != rightArgId) return false;
    }
    return true;
}

std::string MakeSpecializationName(TypeSymbol* templateTypeSymbol, const std::vector<Symbol*>& templateArguments)
{
    std::string specializationName;
    specializationName.append(templateTypeSymbol->Name());
    specializationName.append(1, '<');
    bool first = true;
    for (Symbol* templateArg : templateArguments)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            specializationName.append(", ");
        }
        specializationName.append(templateArg->Name());
    }
    specializationName.append(1, '>');
    return specializationName;
}

} // namespace otava::symbols
