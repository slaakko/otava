// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.node.map;

import otava.ast.error;

namespace otava::ast {

NodeMap::NodeMap()
{
}

void NodeMap::AddNode(Node* node)
{
    nodeMap[node->InternalId()] = node;
}

Node* NodeMap::GetNode(std::int64_t nodeId) const
{
    auto it = nodeMap.find(nodeId);
    if (it != nodeMap.cend())
    {
        return it->second;
    }
    else
    {
        otava::ast::SetExceptionThrown();
        throw std::runtime_error("otava.ast.node_map: node id " + std::to_string(nodeId) + " not found");
    }
}

} // namespace otava::ast
