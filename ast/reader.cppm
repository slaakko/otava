// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.reader;

import std;
import util;
import soul.ast.span;
import otava.ast.node;

export namespace otava::ast {

class NodeMap;

class Reader
{
public:
    //Reader(const std::string& fileName);
    Reader(util::MemoryReader* readerPtr_);
    //inline util::BinaryStreamReader& GetBinaryStreamReader() noexcept { return *readerPtr; }
    //soul::ast::Span ReadSpan();
    inline util::MemoryReader& GetMemoryReader() noexcept { return *readerPtr; }
    NodeKind ReadNodeKind();
    std::string ReadStr();
    bool ReadBool();
    Node* ReadNode();
    void SetNodeMap(NodeMap* nodeMap_) noexcept { nodeMap = nodeMap_; }
    inline NodeMap* GetNodeMap() const noexcept { return nodeMap; }
    inline int FileIndex() const noexcept { return fileIndex; }
    inline void SetFileIndex(int fileIndex_) noexcept { fileIndex = fileIndex_; }
private:
    //std::unique_ptr<util::FileStream> fileStream;
    //std::unique_ptr<util::BufferedStream> bufferedStream;
    //std::unique_ptr<util::BinaryStreamReader> binaryStreamReader;
    util::MemoryReader* readerPtr;
    NodeMap* nodeMap;
    int fileIndex;
};

} // namespace otava::ast
