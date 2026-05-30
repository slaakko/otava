// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.punctuation;

import std;
import otava.ast.node;

export namespace otava::ast {

class SemicolonNode : public Node
{
public:
    SemicolonNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ";"; }
};

class CommaNode : public Node
{
public:
    CommaNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ", "; }
};

class QuestNode : public Node
{
public:
    QuestNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "?"; }
};

class ColonNode : public Node
{
public:
    ColonNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ":"; }
};

class EllipsisNode : public Node
{
public:
    EllipsisNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ":::"; }
};

class LParenNode : public Node
{
public:
    LParenNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "("; }
};

class RParenNode : public Node
{
public:
    RParenNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ")"; }
};

class LBracketNode : public Node
{
public:
    LBracketNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "["; }
};

class RBracketNode : public Node
{
public:
    RBracketNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "]"; }
};

class LBraceNode : public Node
{
public:
    LBraceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "{"; }
};

class RBraceNode : public Node
{
public:
    RBraceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "}"; }
};

} // namespace otava::ast
