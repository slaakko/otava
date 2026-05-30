// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.writer;

import otava.symbols.context;
import otava.symbols.symbol;
import otava.symbols.symbol_table;
import otava.symbols.section;

namespace otava::symbols {

Writer::Writer(const std::string& fileName) :
    fileStream(fileName, util::OpenMode::binary | util::OpenMode::write), bufferedStream(fileStream), binaryStreamWriter(bufferedStream), context(nullptr)
{
}

void Writer::Write(Symbol* symbol)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    if (symbol)
    {
        FileOffset start = FileOffset(Position());
        symbol->Write(*this);
        FileOffset end = FileOffset(Position());
        Section* section = symbolTable->GetSection(symbol);
        SectionEntry entry;
        entry.fileOffset = start;
        entry.length = end - start;
        section->AddEntry(symbol, entry);
    }
    else
    {
        std::uint8_t zero = ToUnderlying(zeroSymbolId);
        binaryStreamWriter.Write(zero);
    }
}

void Writer::Seek(std::uint32_t position)
{
    bufferedStream.Seek(position, util::Origin::seekSet);
}

} // namespace otava::symbols
