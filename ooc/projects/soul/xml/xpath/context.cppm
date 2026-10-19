export module soul.xml.xpath.context;

import std;
import soul.xml.node;
import soul.xml.parent_node;
import soul.xml.document;
import soul.ast.source_pos;

export namespace soul::xml::xpath {

class Context
{
public:
    Context(soul::xml::Node* node_, int pos_, int size_) noexcept;
    inline soul::xml::Node* GetNode() const noexcept { return node; }
    inline int Pos() const noexcept { return pos; }
    inline int Size() const noexcept { return size; }
private:
    soul::xml::Node* node;
    int pos;
    int size;
};

} // namespace soul::xml::xpath
