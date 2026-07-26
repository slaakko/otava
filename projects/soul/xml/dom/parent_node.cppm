export module soul.xml.parent_node;

import std;
import soul.xml.node;
import soul.xml.node_operation;
import util.code_formatter;
import soul.ast.source_pos;

export namespace soul::xml {

class Visitor;

class ParentNode : public Node
{
public:
    ParentNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    ~ParentNode();
    virtual void AppendChild(Node* child);
    virtual void InsertBefore(Node* newChild, Node* refChild);
    virtual std::unique_ptr<Node> RemoveChild(Node* child);
    virtual std::unique_ptr<Node> ReplaceChild(Node* newChild, Node* oldChild);
    bool HasChildNodes() const noexcept override { return firstChild != nullptr; }
    inline Node* FirstChild() const noexcept { return firstChild; }
    inline Node* LastChild() const noexcept { return lastChild; }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
    void WalkChildren(NodeOperation& operation) override;
    void WalkDescendant(NodeOperation& operation) override;
    void WalkDescendantOrSelf(NodeOperation& operation) override;
    void WalkPreceding(NodeOperation& operation) override;
    void WalkPrecedingOrSelf(NodeOperation& operation) override;
private:
    Node* firstChild;
    Node* lastChild;
};

} // namespace soul::xml
