// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.node.list;

import std;

export namespace otava::ast {

template<typename T>
class NodeList
{
public:
    inline NodeList() {}
    NodeList(const NodeList&) = delete;
    NodeList& operator=(const NodeList&) = delete;
    inline int Count() const { return nodes.size(); }
    inline bool IsEmpty() const { return nodes.empty(); }
    inline void Clear() { nodes.clear(); }
    inline T* operator[](int index) const { return nodes[index].get(); }
    inline void Add(T* node)
    {
        nodes.push_back(std::unique_ptr<T>(node));
    }
    inline T* Release(int index)
    {
        return nodes[index].release();
    }
    void Insert(int index, T* node)
    {
        nodes.insert(nodes.begin() + index, std::unique_ptr<T>(node));
    }
    inline std::vector<std::unique_ptr<T>>& Nodes() { return nodes; }
    inline std::vector<std::unique_ptr<T>>::iterator begin() { return nodes.begin(); }
    inline std::vector<std::unique_ptr<T>>::const_iterator begin() const { return nodes.begin(); }
    inline std::vector<std::unique_ptr<T>>::const_iterator cbegin() const { return nodes.cbegin(); }
    inline std::vector<std::unique_ptr<T>>::iterator end() { return nodes.end(); }
    inline std::vector<std::unique_ptr<T>>::const_iterator end() const { return nodes.end(); }
    inline std::vector<std::unique_ptr<T>>::const_iterator cehnd() const { return nodes.cend(); }
private:
    std::vector<std::unique_ptr<T>> nodes;
};

} // namespace otava::ast
