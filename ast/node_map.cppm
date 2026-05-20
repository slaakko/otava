// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.node.map;

import std;
import otava.ast.node;

export namespace otava::ast {

class NodeMap
{
public:
    NodeMap();
    void AddNode(Node* node);
    Node* GetNode(std::int64_t nodeId) const;
private:
    std::map<std::int64_t, Node*> nodeMap;
};

} // namespace otava::ast
