// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.reader;

namespace otava::symbols {

Reader::Reader(util::FileMapping* fileMapping_) : fileMapping(fileMapping_), currentReader(fileMapping->Start(), fileMapping->Length())
{
}

void Reader::PushCurrentReader(const std::uint8_t* start, Length length)
{
    readerStack.push(currentReader);
    currentReader = util::MemoryReader(start, ToUnderlying(length));
}

void Reader::PopCurrentReader()
{
    currentReader = readerStack.top();
    readerStack.pop();
}

soul::ast::FullSpan Reader::ReadFullSpan()
{
    int fileIndex = currentReader.ReadInt();
    if (fileIndex != -1)
    {
        int pos = currentReader.ReadInt();
        int len = currentReader.ReadInt();
        soul::ast::Span span(pos, len);
        return soul::ast::FullSpan(fileIndex, span);
    }
    return soul::ast::FullSpan();
}

} // namespace otava::symbols
