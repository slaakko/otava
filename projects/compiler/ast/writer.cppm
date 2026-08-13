// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.writer;

import std;
import util.stream;
import util.file_stream;
import util.buffered_stream;
import util.binary_stream_writer;
import soul.ast.span;
import otava.ast.node;

export namespace otava::ast {

class Writer
{
public:
    Writer(util::BinaryStreamWriter* writerPtr_);
    inline util::BinaryStreamWriter& GetBinaryStreamWriter() noexcept { return *writerPtr; }
    void Write(NodeKind nodeKind);
    void Write(const std::string& str);
    void Write(bool value);
    void Write(Node* node);
private:
    std::unique_ptr<util::FileStream> fileStream;
    std::unique_ptr<util::BufferedStream> bufferedStream;
    std::unique_ptr<util::BinaryStreamWriter> binaryStreamWriter;
    util::BinaryStreamWriter* writerPtr;
};

} // namespace otava::ast
