// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.writer;

import otava.symbols.id;
import otava.symbols.symbol;
import util.binary_stream_writer;
import util.stream;
import util.file_stream;
import util.buffered_stream;
import soul.ast.span;
import std;

export namespace otava::symbols {

class Context;

class Writer
{
public:
    Writer(const std::string& fileName);
    inline util::BinaryStreamWriter& GetBinaryStreamWriter() { return binaryStreamWriter; }
    void Write(Symbol* symbol);
    void Write(soul::ast::FullSpan& fullSpan);
    inline Context* GetContext() noexcept { return context; }
    inline void SetContext(Context* context_) noexcept { context = context_; }
    void Seek(std::uint32_t position);
    std::uint32_t Position() const { return static_cast<std::uint32_t>(binaryStreamWriter.Position()); }
private:
    util::FileStream fileStream;
    util::BufferedStream bufferedStream;
    util::BinaryStreamWriter binaryStreamWriter;
    Context* context;
};

} // namespace otava::symbols
