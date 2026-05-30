// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.reader;

import otava.ast.node_map;
import otava.ast.error;

namespace otava::ast {

Reader::Reader(const std::string& fileName) : 
    fileStream(new util::FileStream(fileName, util::OpenMode::binary | util::OpenMode::read)), 
    bufferedStream(new util::BufferedStream(*fileStream)), 
    binaryStreamReader(new util::BinaryStreamReader(*bufferedStream)),
    readerPtr(binaryStreamReader.get()),
    nodeMap(nullptr),
    fileIndex(-1)
{
}

Reader::Reader(util::BinaryStreamReader* readerPtr_) : readerPtr(readerPtr_), nodeMap(nullptr), fileIndex(-1)
{
}

soul::ast::Span Reader::ReadSpan()
{
    int len = readerPtr->ReadULEB128UInt();
    if (len == 0) return soul::ast::Span();
    int pos = readerPtr->ReadULEB128UInt();
    return soul::ast::Span(pos, len);
}

NodeKind Reader::ReadNodeKind()
{
    std::uint32_t kind = readerPtr->ReadULEB128UInt();
    return static_cast<NodeKind>(static_cast<std::uint16_t>(kind));
}

std::string Reader::ReadStr()
{
    return readerPtr->ReadUtf8String();
}

bool Reader::ReadBool()
{
    return readerPtr->ReadBool();
}

Node* Reader::ReadNode()
{
    NodeKind kind = ReadNodeKind();
    if (kind == NodeKind::nullNode)
    {
        return nullptr;
    }
    else
    {
        soul::ast::Span span = ReadSpan();
        Node* node = CreateNode(kind, span, fileIndex);
        node->SetId(-1);
        node->Read(*this);
        if (node->InternalId() == -1)
        {
            otava::ast::SetExceptionThrown();
            throw std::runtime_error("otava.ast.Reader: node id not set");
        }
        nodeMap->AddNode(node);
        return node;
    }
}

} // namespace otava::ast
