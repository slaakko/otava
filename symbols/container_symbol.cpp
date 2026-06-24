// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.container_symbol;

import otava.symbols.context;
import otava.symbols.writer;
import otava.symbols.reader;
import otava.symbols.modules;
import otava.symbols.symbol_table;
import otava.symbols.exception;
import util.binary_stream_writer;
import util.utility;

namespace otava::symbols {

ContainerSymbolHeader::ContainerSymbolHeader() : 
    memberCount(Cardinality(0)), bodyOffset(FileOffset(0)), bodyLength(Length(0)), scopeOffset(FileOffset(0)), scopeLength(Length(0))
{
}

void ContainerSymbolHeader::Write(Writer& writer)
{
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    binaryStreamWriter.Write(ToUnderlying(memberCount));
    binaryStreamWriter.Write(ToUnderlying(bodyOffset));
    binaryStreamWriter.Write(ToUnderlying(bodyLength));
    binaryStreamWriter.Write(ToUnderlying(scopeOffset));
    binaryStreamWriter.Write(ToUnderlying(scopeLength));
}

void ContainerSymbolHeader::Read(Reader& reader)
{
    memberCount = Cardinality(reader.CurrentReader().ReadUInt());
    bodyOffset = FileOffset(reader.CurrentReader().ReadUInt());
    bodyLength = Length(reader.CurrentReader().ReadUInt());
    scopeOffset = FileOffset(reader.CurrentReader().ReadUInt());
    scopeLength = Length(reader.CurrentReader().ReadUInt());
}

ContainerSymbol::ContainerSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), scope(module_), bodyRead(false)
{
    scope.SetContainerSymbol(this);
}

ContainerSymbol::ContainerSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), scope(module_), bodyRead(false)
{
    scope.SetContainerSymbol(this);
}

Scope* ContainerSymbol::GetScope()
{
    return &scope;
}

void ContainerSymbol::Write(Writer& writer)
{
    FileOffset start = FileOffset(writer.Position());
    header.Write(writer);
    Symbol::Write(writer);
    header.bodyOffset = FileOffset(writer.Position());
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    std::vector<Symbol*> exportSymbols;
    for (const auto& member : symbols)
    {
        if (member->IsExportSymbol(writer.GetContext()))
        {
            exportSymbols.push_back(member.get());
        }
    }
    Cardinality count = Cardinality(exportSymbols.size());
    header.memberCount = count;
    binaryStreamWriter.Write(ToUnderlying(count));
    for (Index i = Index(0); i < Index(count); ++i)
    {
        binaryStreamWriter.Write(ToUnderlying(exportSymbols[ToUnderlying(i)]->Id()));
    }
    for (Index i = Index(0); i < Index(count); ++i)
    {
        writer.Write(exportSymbols[ToUnderlying(i)]);
    }
    header.bodyLength = Length(writer.Position() - ToUnderlying(header.bodyOffset));
    header.scopeOffset = FileOffset(writer.Position());
    scope.Write(writer);
    header.scopeLength = Length(writer.Position() - ToUnderlying(header.scopeOffset));
    FileOffset end = FileOffset(writer.Position());
    writer.Seek(ToUnderlying(start));
    header.Write(writer);
    writer.Seek(ToUnderlying(end));
}

void ContainerSymbol::Read(Reader& reader) 
{
    header.Read(reader);
    Symbol::Read(reader);
    reader.CurrentReader().Skip(ToUnderlying(header.bodyLength) + ToUnderlying(header.scopeLength));
}

void ContainerSymbol::ReadBody()
{
    if (bodyRead) return;
    bodyRead = true;
    Module* m = GetModule();
    Reader reader(m->GetFileMapping());
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(header.bodyOffset)), header.bodyLength);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        memberIds.push_back(symbolId);
    }
    reader.PopCurrentReader();
}

Cardinality ContainerSymbol::MemberCount() const noexcept
{
    if (IsReadOnly())
    {
        return header.memberCount;
    }
    else
    {
        return Cardinality(symbols.size());
    }
}

Symbol* ContainerSymbol::GetMember(Index index, Context* context)
{
    if (IsReadOnly())
    {
        ReadBody();
        SymbolId symbolId = memberIds[ToUnderlying(index)];
        return GetModule()->GetSymbolTable()->GetSymbol(symbolId, context);
    }
    else
    {
        return symbols[ToUnderlying(index)].get();
    }
}

void ContainerSymbol::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (IsReadOnly())
    {
        ThrowException("cannot add member to container symbol '" + Name() + "' because it is read-only", fullSpan, context);
    }
    else
    {
        symbol->SetParent(this);
        Scope* childScope = symbol->GetScope();
        if (childScope)
        {
            childScope->AddParentScope(&scope);
        }
        symbols.push_back(std::unique_ptr<Symbol>(symbol));
        Module* m = GetModule();
        SymbolTable* symbolTable = m->GetSymbolTable();
        symbolTable->AddSymbol(symbol);
        symbolTable->MapSymbol(symbol);
        if (symbol->IsFundamentalTypeSymbol())
        {
            FundamentalTypeSymbol* fundamentalTypeSymbol = static_cast<FundamentalTypeSymbol*>(symbol);
            symbolTable->MapFundamentalType(fundamentalTypeSymbol);
        }
        if (symbol->IsClassTypeSymbol())
        {
            context->GetSymbolTable()->AddClass(static_cast<ClassTypeSymbol*>(symbol));
        }
        if (symbol->IsFunctionSymbol())
        {
            FunctionSymbol* function = static_cast<FunctionSymbol*>(symbol);
            if (function->IsConversion())
            {
                context->GetSymbolTable()->GetConversionTable()->AddConversion(function, context);
            }
        }
        if (symbol->CanInstall())
        {
            scope.Install(symbol, context);
        }
    }
}

} // namespace otava::symbols
