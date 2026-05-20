// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.node;

import std;
import util.code.formatter;
import soul.ast.source.pos;
import soul.xml.axis;

export namespace soul::xml {

enum class NodeKind
{
    attributeNode, documentFragmentNode, documentNode, elementNode, entityNode, entityReferenceNode, notationNode, processingInstructionNode, textNode, 
    cdataSectionNode, commentNode
};

std::string NodeKindStr(NodeKind nodeKind);

class ParentNode;
class Document;
class Visitor;
class NodeOperation;

class Node
{
public:
    Node(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    virtual ~Node();
    inline NodeKind Kind() const noexcept { return kind; }
    inline const soul::ast::SourcePos& GetSourcePos() const noexcept { return sourcePos; }
    inline const std::string& Name() const noexcept { return name; }
    inline const std::string& NamespaceUri() const noexcept { return namespaceUri; }
    void SetNamespaceUri(const std::string& namespaceUri_);
    inline bool IsAttributeNode() const noexcept { return kind == NodeKind::attributeNode; }
    inline bool IsDocumentFragmentNode() const noexcept { return kind == NodeKind::documentFragmentNode; }
    inline bool IsDocumentNode() const noexcept { return kind == NodeKind::documentNode; }
    inline bool IsElementNode() const noexcept { return kind == NodeKind::elementNode; }
    inline bool IsEntityNode() const noexcept { return kind == NodeKind::entityNode; }
    inline bool IsEntityReferenceNode() const noexcept { return kind == NodeKind::entityReferenceNode; }
    inline bool IsNotationNode() const noexcept { return kind == NodeKind::notationNode; }
    inline bool IsProcessingInstructionNode() const noexcept { return kind == NodeKind::processingInstructionNode; }
    inline bool IsTextNode() const noexcept { return kind == NodeKind::textNode; }
    inline bool IsCDataSectionNode() const noexcept { return kind == NodeKind::cdataSectionNode; }
    inline bool IsCommentNode() const noexcept { return kind == NodeKind::commentNode; }
    void Walk(NodeOperation& operation, Axis axis);
    virtual void WalkChildren(NodeOperation& operation);
    virtual void WalkDescendant(NodeOperation& operation);
    virtual void WalkDescendantOrSelf(NodeOperation& operation);
    void WalkParent(NodeOperation& operation);
    virtual void WalkFollowing(NodeOperation& operation);
    virtual void WalkPreceding(NodeOperation& operation);
    virtual void WalkPrecedingOrSelf(NodeOperation& operation);
    void WalkAncestor(NodeOperation& operation);
    void WalkAncestorOrSelf(NodeOperation& operation);
    void WalkFollowingSibling(NodeOperation& operation);
    void WalkPrecedingSibling(NodeOperation& operation);
    virtual void WalkAttribute(NodeOperation& operation);
    inline ParentNode* Parent() const noexcept { return parent; }
    inline Node* Prev() const noexcept { return prev; }
    inline Node* Next() const noexcept { return next; }
    inline Document* OwnerDocument() const noexcept { return ownerDocument; }
    inline void SetOwnerDocument(Document* ownerDocumnent_) noexcept { ownerDocument = ownerDocumnent_; }
    std::string Prefix() const;
    void SetPrefix(const std::string& prefix);
    std::string LocalName() const;
    virtual void Accept(Visitor& visitor) {}
    virtual bool HasChildNodes() const noexcept { return false; }
    virtual bool HasAttributes() const noexcept { return false; }
    virtual bool ValueContainsNewLine() const noexcept { return false; }
    virtual void Write(util::CodeFormatter& formatter) = 0;
private:
    friend class ParentNode;
    inline void SetParent(ParentNode* parent_) noexcept { parent = parent_; }
    inline void SetPrev(Node* prev_) noexcept { prev = prev_; }
    inline void SetNext(Node* next_) noexcept { next = next_; }
    void LinkBefore(Node* node);
    void LinkAfter(Node* node);
    void Unlink();
    NodeKind kind;
    soul::ast::SourcePos sourcePos;
    std::string name;
    std::string namespaceUri;
    ParentNode* parent;
    Node* prev;
    Node* next;
    Document* ownerDocument;
};

} // namespace soul::xml
