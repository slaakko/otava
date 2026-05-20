// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.intrusive.list;

import std;

export namespace util {

template<typename T>
class IntrusiveListNode
{
public:
    using NodePtr = IntrusiveListNode<T>*;
    IntrusiveListNode(T* ptr_) noexcept : ptr(ptr_), parent(nullptr), nextSibling(nullptr), prevSibling(nullptr), firstChild(nullptr), lastChild(nullptr) {}
    inline T* NextSibling() const noexcept { if (nextSibling) return nextSibling->Get(); else return nullptr; }
    inline void SetNextSibling(NodePtr nextSibling_) noexcept { nextSibling = nextSibling_; }
    inline T* PrevSibling() const noexcept { if (prevSibling) return prevSibling->Get(); else return nullptr; }
    inline void SetPrevSibling(NodePtr prevSibling_) noexcept { prevSibling = prevSibling_; }
    void LinkBefore(NodePtr node) noexcept
    {
        if (prevSibling)
        {
            prevSibling->nextSibling = node;
        }
        node->prevSibling = prevSibling;
        node->nextSibling = this;
        prevSibling = node;
    }
    void LinkAfter(NodePtr node) noexcept
    {
        if (nextSibling)
        {
            nextSibling->prevSibling = node;
        }
        node->prevSibling = this;
        node->nextSibling = nextSibling;
        nextSibling = node;
    }
    void Unlink() noexcept
    {
        if (prevSibling)
        {
            prevSibling->nextSibling = nextSibling;
        }
        if (nextSibling)
        {
            nextSibling->prevSibling = prevSibling;
        }
    }
    inline T* Parent() const noexcept { if (parent) return parent->Get(); else return nullptr; }
    inline void SetParent(NodePtr parent_) noexcept { parent = parent_; }
    inline T* FirstChild() const noexcept { if (firstChild) return firstChild->Get(); else return nullptr; }
    inline T* LastChild() const noexcept { if (lastChild) return lastChild->Get(); else return nullptr; }
    void AddChild(NodePtr child) noexcept
    {
        if (lastChild)
        {
            lastChild->LinkAfter(child);
        }
        if (!firstChild)
        {
            firstChild = child;
        }
        child->SetParent(this);
        lastChild = child;
    }
    NodePtr RemoveChild(NodePtr child) noexcept
    {
        child->Unlink();
        if (child == firstChild)
        {
            firstChild = child->NextSibling();
        }
        if (child == lastChild)
        {
            lastChild = child->PrevSibling();
        }
        child->SetParent(nullptr);
        child->SetNextSibling(nullptr);
        child->SetPrevSibling(nullptr);
        return child;
    }
    void InsertBefore(NodePtr child, NodePtr before) noexcept
    {
        if (!before)
        {
            AddChild(child);
        }
        else
        {
            child->SetParent(this);
            if (firstChild == before)
            {
                firstChild = child;
            }
            before->LinkBefore(child);
        }
    }
    void InsertAfter(NodePtr child, NodePtr after) noexcept
    {
        if (!after)
        {
            AddChild(child);
        }
        else
        {
            child->SetParent(this);
            after->LinkAfter(child);
            if (after == lastChild)
            {
                lastChild = child;
            }
        }
    }
    inline T* Get() const
    {
        return ptr;
    }
private:
    T* ptr;
    NodePtr parent;
    NodePtr nextSibling;
    NodePtr prevSibling;
    NodePtr firstChild;
    NodePtr lastChild;
};

} // namespace util
