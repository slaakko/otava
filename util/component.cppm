// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.component;

import std;

export namespace util {

class Container;

class Component
{
public:
    Component() noexcept;
    virtual ~Component();
    inline Container* GetContainer() const noexcept { return container; }
    inline void SetContainer(Container* container_) noexcept { container = container_; }
    inline Component* NextSibling() const noexcept { return nextSibling; }
    inline void SetNextSibling(Component* nextSibling_) noexcept { nextSibling = nextSibling_; }
    inline Component* PrevSibling() const noexcept { return prevSibling; }
    inline void SetPrevSibling(Component* prevSibling_) noexcept { prevSibling = prevSibling_; }
    void LinkBefore(Component* component) noexcept
    {
        if (prevSibling)
        {
            prevSibling->nextSibling = component;
        }
        component->prevSibling = prevSibling;
        component->nextSibling = this;
        prevSibling = component;
    }
    void LinkAfter(Component* component) noexcept
    {
        if (nextSibling)
        {
            nextSibling->prevSibling = component;
        }
        component->prevSibling = this;
        component->nextSibling = nextSibling;
        nextSibling = component;
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
private:
    Container* container;
    Component* nextSibling;
    Component* prevSibling;
};

class Container
{
public:
    Container(Component* parent_) noexcept;
    virtual ~Container();
    inline bool IsEmpty() const noexcept { return firstChild == nullptr; }
    inline Component* Parent() const noexcept { return parent; }
    inline Component* FirstChild() const noexcept { return firstChild; }
    inline Component* LastChild() const noexcept { return lastChild; }
    void AddChild(Component* child) noexcept;
    std::unique_ptr<Component> RemoveChild(Component* child) noexcept;
    void InsertBefore(Component* child, Component* before) noexcept;
    void InsertAfter(Component* child, Component* after) noexcept;
private:
    Component* parent;
    Component* firstChild;
    Component* lastChild;
};


} // namespace util
