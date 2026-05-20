// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.identifier;

import std;
import otava.ast.node;

export namespace otava::ast {

class IdentifierNode : public CompoundNode
{
public:
    IdentifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    IdentifierNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& str_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::u32string Str() const override { return str; }
private:
    std::u32string str;
};

class UnnamedNode : public Node
{
public:
    UnnamedNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ColonColonNode : public Node
{
public:
    ColonColonNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"::"; }
};

class NestedNameSpecifierNode : public SequenceNode
{
public:
    NestedNameSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class QualifiedIdNode : public BinaryNode
{
public:
    QualifiedIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    QualifiedIdNode(const soul::ast::Span& span_, int fileIndex_, Node* nns_, Node* unqualifiedId_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class IdentifierListNode : public ListNode
{
public:
    IdentifierListNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ModuleNameNode : public Node
{
public:
    ModuleNameNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ModuleNameNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& str_);
    std::u32string Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    std::u32string str;
};

} // namespace otava::ast
