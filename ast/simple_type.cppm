// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.simple_type;

import std;
import otava.ast.node;

export namespace otava::ast {

class CharNode : public Node
{
public:
    CharNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "char"; }
};

class Char8Node : public Node
{
public:
    Char8Node(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "char8_t"; }
};

class Char16Node : public Node
{
public:
    Char16Node(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "char16_t"; }
};

class Char32Node : public Node
{
public:
    Char32Node(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "char32_t"; }
};

class WCharNode : public Node
{
public:
    WCharNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "wchar_t"; }
};

class BoolNode : public Node
{
public:
    BoolNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "bool"; }
};

class ShortNode : public Node
{
public:
    ShortNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "short"; }
};

class IntNode : public Node
{
public:
    IntNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "int"; }
};

class LongNode : public Node
{
public:
    LongNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "long"; }
};

class SignedNode : public Node
{
public:
    SignedNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "signed"; }
};

class UnsignedNode : public Node
{
public:
    UnsignedNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "unsigned"; }
};

class FloatNode : public Node
{
public:
    FloatNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "float"; }
};

class DoubleNode : public Node
{
public:
    DoubleNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "double"; }
};

class VoidNode : public Node
{
public:
    VoidNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "void"; }
};

} // namespace otava::ast
