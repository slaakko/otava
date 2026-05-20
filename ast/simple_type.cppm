// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.simple.type;

import std;
import otava.ast.node;

export namespace otava::ast {

class CharNode : public Node
{
public:
    CharNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"char"; }
};

class Char8Node : public Node
{
public:
    Char8Node(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"char8_t"; }
};

class Char16Node : public Node
{
public:
    Char16Node(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"char16_t"; }
};

class Char32Node : public Node
{
public:
    Char32Node(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"char32_t"; }
};

class WCharNode : public Node
{
public:
    WCharNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"wchar_t"; }
};

class BoolNode : public Node
{
public:
    BoolNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"bool"; }
};

class ShortNode : public Node
{
public:
    ShortNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"short"; }
};

class IntNode : public Node
{
public:
    IntNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"int"; }
};

class LongNode : public Node
{
public:
    LongNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"long"; }
};

class SignedNode : public Node
{
public:
    SignedNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"signed"; }
};

class UnsignedNode : public Node
{
public:
    UnsignedNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"unsigned"; }
};

class FloatNode : public Node
{
public:
    FloatNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"float"; }
};

class DoubleNode : public Node
{
public:
    DoubleNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"double"; }
};

class VoidNode : public Node
{
public:
    VoidNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"void"; }
};

} // namespace otava::ast
