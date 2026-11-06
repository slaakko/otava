// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.reader;

import std;
import util.memory_reader;
import soul.ast.span;
import otava.ast.node;

export namespace otava::ast {

class NodeMap;

class Reader
{
public:
    Reader(util::MemoryReader* readerPtr_);
    inline util::MemoryReader& GetMemoryReader() noexcept { return *readerPtr; }
    NodeKind ReadNodeKind();
    std::string ReadStr();
    bool ReadBool();
    Node* ReadNode();
    void SetNodeMap(NodeMap* nodeMap_) noexcept;
    NodeMap* GetNodeMap() const noexcept;
    inline int FileIndex() const noexcept { return fileIndex; }
    inline void SetFileIndex(int fileIndex_) noexcept { fileIndex = fileIndex_; }
private:
    util::MemoryReader* readerPtr;
    NodeMap* nodeMap;
    int fileIndex;
};

} // namespace otava::ast
