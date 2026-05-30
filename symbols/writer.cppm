// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.writer;

import std;
import otava.symbols.id;
import util.binary_stream_writer;
import util.file_stream;
import util.buffered_stream;
import otava.symbols.symbol;

export namespace otava::symbols {

class Symbol;
class Context;

class Writer
{
public:
    Writer(const std::string& fileName);
    inline util::BinaryStreamWriter& GetBinaryStreamWriter() { return binaryStreamWriter; }
    void Write(Symbol* symbol);
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
