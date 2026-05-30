// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.section;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.symbol;
import otava.symbols.symbol_factory;
import otava.symbols.reader;
import otava.symbols.writer;
import util.binary_stream_writer;
import util.utility;

namespace otava::symbols {

SectionKind ToSectionKind(SymbolKind symbolKind)
{
    switch (symbolKind)
    {
    case SymbolKind::namespaceSymbol:
    {
        return SectionKind::namespaceSection;
    }
    case SymbolKind::classGroupSymbol:
    case SymbolKind::conceptGroupSymbol:
    case SymbolKind::aliasGroupSymbol:
    case SymbolKind::enumGroupSymbol:
    case SymbolKind::nullPtrTypeSymbol:
    case SymbolKind::aliasTypeSymbol:
    case SymbolKind::arrayTypeSymbol:
    case SymbolKind::blockSymbol:
    case SymbolKind::classTypeSymbol:
    case SymbolKind::compoundTypeSymbol:
    case SymbolKind::conceptSymbol:
    case SymbolKind::enumTypeSymbol:
    case SymbolKind::functionTypeSymbol:
    case SymbolKind::fundamentalTypeSymbol:
    case SymbolKind::templateDeclarationSymbol:
    case SymbolKind::typenameConstraintSymbol:
    case SymbolKind::explicitInstantiationSymbol:
    case SymbolKind::templateParameterSymbol:
    case SymbolKind::varArgTypeSymbol:
    case SymbolKind::classTemplateSpecializationSymbol:
    case SymbolKind::aliasTypeTemplateSpecializationSymbol:
    case SymbolKind::nestedTypeSymbol:
    case SymbolKind::dependentTypeSymbol:
    case SymbolKind::forwardClassDeclarationSymbol:
    case SymbolKind::forwardEnumDeclarationSymbol:
    case SymbolKind::boundTemplateParameterSymbol:
    case SymbolKind::constraintExprSymbol:
    case SymbolKind::functionGroupTypeSymbol:
    case SymbolKind::classGroupTypeSymbol:
    case SymbolKind::aliasGroupTypeSymbol:
    case SymbolKind::friendSymbol:
    case SymbolKind::namespaceTypeSymbol:
    {
        return SectionKind::typeSection;
    }
    case SymbolKind::functionGroupSymbol:
    case SymbolKind::functionSymbol:
    case SymbolKind::functionDefinitionSymbol:
    case SymbolKind::explicitlyInstantiatedFunctionDefinitionSymbol:
    case SymbolKind::fundamentalTypeNot:
    case SymbolKind::fundamentalTypeUnaryPlus:
    case SymbolKind::fundamentalTypeUnaryMinus:
    case SymbolKind::fundamentalTypeComplement:
    case SymbolKind::fundamentalTypeAdd:
    case SymbolKind::fundamentalTypeSub:
    case SymbolKind::fundamentalTypeMul:
    case SymbolKind::fundamentalTypeDiv:
    case SymbolKind::fundamentalTypeMod:
    case SymbolKind::fundamentalTypeAnd:
    case SymbolKind::fundamentalTypeOr:
    case SymbolKind::fundamentalTypeXor:
    case SymbolKind::fundamentalTypeShl:
    case SymbolKind::fundamentalTypeShr:
    case SymbolKind::fundamentalTypePlusAssign:
    case SymbolKind::fundamentalTypeMinusAssign:
    case SymbolKind::fundamentalTypeMulAssign:
    case SymbolKind::fundamentalTypeDivAssign:
    case SymbolKind::fundamentalTypeModAssign:
    case SymbolKind::fundamentalTypeAndAssign:
    case SymbolKind::fundamentalTypeOrAssign:
    case SymbolKind::fundamentalTypeXorAssign:
    case SymbolKind::fundamentalTypeShlAssign:
    case SymbolKind::fundamentalTypeShrAssign:
    case SymbolKind::fundamentalTypeEqual:
    case SymbolKind::fundamentalTypeLess:
    case SymbolKind::fundamentalTypeBoolean:
    case SymbolKind::fundamentalTypeSignExtension:
    case SymbolKind::fundamentalTypeZeroExtension:
    case SymbolKind::fundamentalTypeFloatingPointExtension:
    case SymbolKind::fundamentalTypeTruncate:
    case SymbolKind::fundamentalTypeBitcast:
    case SymbolKind::fundamentalTypeIntToFloat:
    case SymbolKind::fundamentalTypeFloatToInt:
    case SymbolKind::fundamentalTypeBoolToInt:
    case SymbolKind::fundamentalTypeDefaultCtor:
    case SymbolKind::fundamentalTypeCopyCtor:
    case SymbolKind::fundamentalTypeCopyCtorLiteral:
    case SymbolKind::fundamentalTypeMoveCtor:
    case SymbolKind::fundamentalTypeCopyAssignment:
    case SymbolKind::fundamentalTypeMoveAssignment:
    case SymbolKind::enumTypeDefaultCtor:
    case SymbolKind::enumTypeCopyCtor:
    case SymbolKind::enumTypeMoveCtor:
    case SymbolKind::enumTypeCopyAssignment:
    case SymbolKind::enumTypeMoveAssignment:
    case SymbolKind::enumTypeEqual:
    case SymbolKind::enumTypeLess:
    case SymbolKind::arrayTypeDefaultCtor:
    case SymbolKind::arrayTypeCopyCtor:
    case SymbolKind::arrayTypeMoveCtor:
    case SymbolKind::arrayTypeCopyAssignment:
    case SymbolKind::arrayTypeMoveAssignment:
    case SymbolKind::arrayTypeBegin:
    case SymbolKind::arrayTypeEnd:
    case SymbolKind::defaultBool:
    case SymbolKind::defaultSByte:
    case SymbolKind::defaultByte:
    case SymbolKind::defaultShort:
    case SymbolKind::defaultUShort:
    case SymbolKind::defaultInt:
    case SymbolKind::defaultUInt:
    case SymbolKind::defaultLong:
    case SymbolKind::defaultULong:
    case SymbolKind::defaultFloat:
    case SymbolKind::defaultDouble:
    case SymbolKind::defaultChar:
    case SymbolKind::defaultChar16:
    case SymbolKind::defaultChar32:
    case SymbolKind::intrinsicGetRbp:
    {
        return SectionKind::functionSection;
    }
    case SymbolKind::variableGroupSymbol:
    case SymbolKind::boolValueSymbol:
    case SymbolKind::integerValueSymbol:
    case SymbolKind::floatingValueSymbol:
    case SymbolKind::stringValueSymbol:
    case SymbolKind::charValueSymbol:
    case SymbolKind::arrayValueSymbol:
    case SymbolKind::structureValueSymbol:
    case SymbolKind::enumConstantSymbol:
    case SymbolKind::variableSymbol:
    case SymbolKind::parameterSymbol:
    case SymbolKind::nullPtrValueSymbol:
    case SymbolKind::symbolValueSymbol:
    case SymbolKind::invokeValueSymbol:
    {
        return SectionKind::variableSection;
    }
    }
    return SectionKind::none;
}

SectionKind GetSectionKind(Symbol* symbol)
{
    return ToSectionKind(symbol->Kind());
}

SectionHeader::SectionHeader() : entryMapOffset(FileOffset(0)), entryMapLength(Length(0))
{
}

void SectionHeader::Write(Writer& writer)
{
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    binaryStreamWriter.Write(ToUnderlying(entryMapOffset));
    binaryStreamWriter.Write(ToUnderlying(entryMapLength));
}

void SectionHeader::Read(Reader& reader)
{
    reader.PushCurrentReader(reader.Start(), Length(sizeof(*this)));
    entryMapOffset = FileOffset(reader.CurrentReader().ReadUInt());
    entryMapLength = Length(reader.CurrentReader().ReadUInt());
    reader.PopCurrentReader();
}

SectionEntry::SectionEntry() : fileOffset(FileOffset(0)), length(Length(0))
{
}

void SectionEntry::Write(Writer& writer)
{
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    binaryStreamWriter.Write(ToUnderlying(fileOffset));
    binaryStreamWriter.Write(ToUnderlying(length));
}

void SectionEntry::Read(Reader& reader)
{
    fileOffset = FileOffset(reader.CurrentReader().ReadUInt());
    length = Length(reader.CurrentReader().ReadUInt());
}

Section::Section(Module* module_, SectionKind kind_) : module(module_), kind(kind_), headerRead(false), entriesRead(false), readOnly(false)
{
}

void Section::AddEntry(Symbol* symbol, const SectionEntry& entry)
{
    entryMap[symbol->Id()] = entry;
}

void Section::MapSymbol(Symbol* symbol)
{
    symbolMap[symbol->Id()] = symbol;
}

Symbol* Section::GetSymbol(SymbolId symbolId, Context* context)
{
    auto it = symbolMap.find(symbolId);
    if (it != symbolMap.end())
    {
        return it->second;
    }
    if (readOnly)
    {
        ReadEntries();
        auto fit = entryMap.find(symbolId);
        if (fit != entryMap.end())
        {
            SectionEntry entry(fit->second);
            Reader reader(GetModule()->GetFileMapping());
            reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(entry.fileOffset)), entry.length);
            Symbol* symbol = MakeSymbol(GetModule(), symbolId);
            symbol->Read(reader);
            reader.PopCurrentReader();
            symbolMap[symbol->Id()] = symbol;
            symbols.push_back(std::unique_ptr<Symbol>(symbol));
            return symbol;
        }
        ModuleId moduleId = GetModule()->GetSymbolTable()->GetModuleIdOfExportedSymbol(symbolId);
        if (moduleId != zeroModuleId && moduleId != GetModule()->Id())
        {
            Module* module = context->GetModuleMapper()->GetModule(moduleId);
            if (module)
            {
                Symbol* symbol = module->GetSymbolTable()->GetSymbol(symbolId, context);
                if (symbol)
                {
                    symbolMap[symbol->Id()] = symbol;
                    return symbol;
                }
            }
        }
    }
    return nullptr;
}

void Section::ReadHeader()
{
    if (headerRead) return;
    headerRead = true;
    Reader reader(GetModule()->GetFileMapping());
    header.Read(reader);
}

void Section::ReadEntries()
{
    if (entriesRead) return;
    ReadHeader();
    entriesRead = true;
    Reader reader(GetModule()->GetFileMapping());
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(header.entryMapOffset)), header.entryMapLength);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        SectionEntry entry;
        entry.Read(reader);
        entryMap[symbolId] = entry;
    }
    reader.PopCurrentReader();
}

void Section::Write(Writer& writer)
{
    FileOffset start = FileOffset(writer.Position());
    header.Write(writer);
    header.entryMapOffset = FileOffset(writer.Position());
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    binaryStreamWriter.Write(ToUnderlying(Cardinality(entryMap.size())));
    for (const auto& e : entryMap)
    {
        SymbolId symbolId = e.first;
        SectionEntry entry = e.second;
        binaryStreamWriter.Write(ToUnderlying(symbolId));
        entry.Write(writer);
    }
    Length entryMapLength = Length(writer.Position() - ToUnderlying(header.entryMapOffset));
    header.entryMapLength = entryMapLength;
    FileOffset end = FileOffset(writer.Position());
    writer.Seek(ToUnderlying(start));
    header.Write(writer);
    writer.Seek(ToUnderlying(end));
}

} // namespace otava::symbols
