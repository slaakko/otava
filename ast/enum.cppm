// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.enums;

import std;
import otava.ast.node;

export namespace otava::ast {

class EnumSpecifierNode : public ListNode
{
public:
    EnumSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    EnumSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* enumHead_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* EnumHead() const noexcept { return enumHead.get(); }
    inline void SetLBraceSpan(const soul::ast::Span& lbSpan_) noexcept { lbSpan = lbSpan_; }
    inline void SetRBraceSpan(const soul::ast::Span& rbSpan_) noexcept { rbSpan = rbSpan_; }
    inline const soul::ast::Span& LBraceSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBraceSpan() const noexcept { return rbSpan; }
private:
    std::unique_ptr<Node> enumHead;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class EnumHeadNode : public CompoundNode
{
public:
    EnumHeadNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    EnumHeadNode(const soul::ast::Span& span_, int fileIndex_, Node* enumKey_, Node* enumHeadName_, Node* enumBase_, Node* attributes_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* EnumKey() const noexcept { return enumKey.get(); }
    inline Node* EnumHeadName() const noexcept { return enumHeadName.get(); }
    inline Node* EnumBase() const noexcept { return enumBase.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
private:
    std::unique_ptr<Node> enumKey;
    std::unique_ptr<Node> enumHeadName;
    std::unique_ptr<Node> enumBase;
    std::unique_ptr<Node> attributes;
};

class EnumBaseNode : public UnaryNode
{
public:
    EnumBaseNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    EnumBaseNode(const soul::ast::Span& span_, int fileIndex_, Node* typeSpecifiers_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class EnumClassNode : public CompoundNode
{
public:
    EnumClassNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    EnumClassNode(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& classSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& ClassSpan() const noexcept { return classSpan; }
private:
    soul::ast::Span classSpan;
};

class EnumStructNode : public CompoundNode
{
public:
    EnumStructNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    EnumStructNode(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& structSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& StructSpan() const noexcept { return structSpan; }
private:
    soul::ast::Span structSpan;
};

class EnumNode : public Node
{
public:
    EnumNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class EnumeratorDefinitionNode : public CompoundNode
{
public:
    EnumeratorDefinitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    EnumeratorDefinitionNode(const soul::ast::Span& span_, int fileIndex_, Node* enumerator_, Node* value_, const soul::ast::Span& assignSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Enumerator() const noexcept { return enumerator.get(); }
    inline Node* GetValue() const noexcept { return value.get(); }
    inline const soul::ast::Span& AssignSpan() const noexcept { return assignSpan; }
private:
    std::unique_ptr<Node> enumerator;
    std::unique_ptr<Node> value;
    soul::ast::Span assignSpan;
};

class EnumeratorNode : public CompoundNode
{
public:
    EnumeratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    EnumeratorNode(const soul::ast::Span& span_, int fileIndex_, Node* identifier_, Node* attributes_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Identifier() const noexcept { return identifier.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
private:
    std::unique_ptr<Node> identifier;
    std::unique_ptr<Node> attributes;
};

class ElaboratedEnumSpecifierNode : public UnaryNode
{
public:
    ElaboratedEnumSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ElaboratedEnumSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* enumName_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class OpaqueEnumDeclarationNode : public CompoundNode
{
public:
    OpaqueEnumDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    OpaqueEnumDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* enumKey_, Node* enumHeadName_, Node* enumBase_, Node* attributes_, 
        Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* EnumKey() const noexcept { return enumKey.get(); }
    inline Node* EnumHeadName() const noexcept { return enumHeadName.get(); }
    inline Node* EnumBase() const noexcept { return enumBase.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
private:
    std::unique_ptr<Node> enumKey;
    std::unique_ptr<Node> enumHeadName;
    std::unique_ptr<Node> enumBase;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
};

} // namespace otava::ast
