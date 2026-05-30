// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.qualifier;

import std;
import otava.ast.node;

export namespace otava::ast {

class ConstNode : public Node
{
public:
    ConstNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "const"; }
};

class VolatileNode : public Node
{
public:
    VolatileNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "volatile"; }
};

class LvalueRefNode : public Node
{
public:
    LvalueRefNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "&"; }
};

class RvalueRefNode : public Node
{
public:
    RvalueRefNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "&&"; }
};

class PtrNode : public Node
{
public:
    PtrNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "*"; }
};

class CVQualifierSequenceNode : public SequenceNode
{
public:
    CVQualifierSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

} // namespace otava::ast
