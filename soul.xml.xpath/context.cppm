// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.xpath.context;

import std;
import soul.xml.node;

export namespace soul::xml::xpath {

class Context
{
public:
    Context(soul::xml::Node* node_, int pos_, int size_) noexcept;
    inline soul::xml::Node* Node() const noexcept { return node; }
    inline int Pos() const noexcept { return pos; }
    inline int Size() const noexcept { return size; }
private:
    soul::xml::Node* node;
    int pos;
    int size;
};

} // namespace soul::xml::xpath
