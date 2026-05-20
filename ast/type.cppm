// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.type;

import std;
import otava.ast.node;

export namespace otava::ast {

class TypeSpecifierSequenceNode : public SequenceNode
{
public:
    TypeSpecifierSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override;
};

class TypenameSpecifierNode : public CompoundNode
{
public:
    TypenameSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TypenameSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* nns_, Node* id_, Node* templateNode_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* NestedNameSpecifier() const noexcept { return nns.get(); }
    inline Node* GetId() const noexcept { return id.get(); }
    inline Node* TemplateNode() const noexcept { return templateNode.get(); }
private:
    std::unique_ptr<Node> nns;
    std::unique_ptr<Node> id;
    std::unique_ptr<Node> templateNode;
};

class TypeIdNode : public CompoundNode
{
public:
    TypeIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TypeIdNode(const soul::ast::Span& span_, int fileIndex_, Node* typeSpecifiers_, Node* declarator_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* TypeSpecifiers() const noexcept { return typeSpecifiers.get(); }
    inline Node* Declarator() const noexcept { return declarator.get(); }
    std::u32string Str() const override;
private:
    std::unique_ptr<Node> typeSpecifiers;
    std::unique_ptr<Node> declarator;
};

class DefiningTypeIdNode : public CompoundNode
{
public:
    DefiningTypeIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DefiningTypeIdNode(const soul::ast::Span& span_, int fileIndex_, Node* definingTypeSpecifiers_, Node* abstractDeclarator_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* DefiningTypeSpecifiers() const noexcept { return definingTypeSpecifiers.get(); }
    inline Node* AbstractDeclarator() const noexcept { return abstractDeclarator.get(); }
    std::u32string Str() const override;
private:
    std::unique_ptr<Node> definingTypeSpecifiers;
    std::unique_ptr<Node> abstractDeclarator;
};

class DefiningTypeSpecifierSequenceNode : public SequenceNode
{
public:
    DefiningTypeSpecifierSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override;
};

class TrailingReturnTypeNode : public UnaryNode
{
public:
    TrailingReturnTypeNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TrailingReturnTypeNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ElaboratedTypeSpecifierNode : public CompoundNode
{
public:
    ElaboratedTypeSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ElaboratedTypeSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* classKey_, Node* id_, Node* attributes_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ClassKey() const noexcept { return classKey.get(); }
    inline Node* GetId() const noexcept { return id.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    std::u32string Str() const override;
private:
    std::unique_ptr<Node> classKey;
    std::unique_ptr<Node> id;
    std::unique_ptr<Node> attributes;
};

class DeclTypeSpecifierNode : public CompoundNode
{
public:
    DeclTypeSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DeclTypeSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* expr_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Expression() const noexcept { return expr.get(); }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    std::unique_ptr<Node> expr;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class PlaceholderTypeSpecifierNode : public CompoundNode
{
public:
    PlaceholderTypeSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    PlaceholderTypeSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* typeConstraint_, const soul::ast::Span& dtSpan_,
        const soul::ast::Span& autoSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* TypeConstraint() const { return typeConstraint.get(); }
    inline const soul::ast::Span& DeclTypeSpan() const noexcept { return dtSpan; }
    inline const soul::ast::Span& AutoSpan() const noexcept { return autoSpan; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    inline bool IsDeclType() const noexcept { return dtSpan.IsValid(); };
private:
    std::unique_ptr<Node> typeConstraint;
    soul::ast::Span dtSpan;
    soul::ast::Span autoSpan;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

} // namespace otava::ast
