module soul.xml.xpath.context;

namespace soul::xml::xpath {

Context::Context(soul::xml::Node* node_, int pos_, int size_) noexcept : node(node_), pos(pos_), size(size_)
{
}

} // namespace soul::xml::xpath
