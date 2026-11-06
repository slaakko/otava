// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.reader;

import otava.ast.node_map;
import otava.ast.error;

namespace otava::ast {

Reader::Reader(util::MemoryReader* readerPtr_) : readerPtr(readerPtr_), nodeMap(nullptr), fileIndex(-1)
{
}

NodeKind Reader::ReadNodeKind()
{
    std::uint16_t kind = readerPtr->ReadUShort();
    return static_cast<NodeKind>(kind);
}

std::string Reader::ReadStr()
{
    return readerPtr->ReadString();
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
        Node* node = MakeNode(kind, soul::ast::Span(), -1);
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

void Reader::SetNodeMap(NodeMap* nodeMap_) noexcept
{
    nodeMap = nodeMap_;
}

NodeMap* Reader::GetNodeMap() const noexcept
{
    return nodeMap;
}

} // namespace otava::ast
