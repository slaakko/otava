// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.reader;

import std;
import soul.ast.span;
import otava.symbols.id;
import util.file_mapping;
import util.memory_reader;

export namespace otava::symbols {

class Reader
{
public:
    Reader(util::FileMapping* fileMapping_);
    inline const std::string& FilePath() const noexcept { return fileMapping->FileName(); }
    inline const std::uint8_t* Start() const noexcept { return fileMapping->Start(); }
    inline Length GetLength() const noexcept { return Length(fileMapping->Length()); }
    inline util::MemoryReader& CurrentReader() noexcept{ return currentReader; }
    void PushCurrentReader(const std::uint8_t* start, Length length);
    void PopCurrentReader();
    soul::ast::FullSpan ReadFullSpan();
private:
    util::FileMapping* fileMapping;
    util::MemoryReader currentReader;
    std::stack<util::MemoryReader> readerStack;
};

} // namespace otava::symbols
