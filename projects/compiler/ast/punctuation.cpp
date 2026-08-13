// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.punctuation;

import otava.ast.visitor;

namespace otava::ast {

SemicolonNode::SemicolonNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::semicolonNode, span_, fileIndex_)
{
}

Node* SemicolonNode::Clone() const
{
    SemicolonNode* clone = new SemicolonNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void SemicolonNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CommaNode::CommaNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::commaNode, span_, fileIndex_)
{
}

Node* CommaNode::Clone() const
{
    CommaNode* clone = new CommaNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void CommaNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

QuestNode::QuestNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::questNode, span_, fileIndex_)
{
}

Node* QuestNode::Clone() const
{
    QuestNode* clone = new QuestNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void QuestNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ColonNode::ColonNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::colonNode, span_, fileIndex_)
{
}

Node* ColonNode::Clone() const
{
    ColonNode* clone = new ColonNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ColonNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EllipsisNode::EllipsisNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::ellipsisNode, span_, fileIndex_)
{
}

Node* EllipsisNode::Clone() const
{
    EllipsisNode* clone = new EllipsisNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void EllipsisNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LParenNode::LParenNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::lparenNode, span_, fileIndex_)
{
}

Node* LParenNode::Clone() const
{
    LParenNode* clone = new LParenNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void LParenNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RParenNode::RParenNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::rparenNode, span_, fileIndex_)
{
}

Node* RParenNode::Clone() const
{
    RParenNode* clone = new RParenNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void RParenNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LBracketNode::LBracketNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::lbracketNode, span_, fileIndex_)
{
}

Node* LBracketNode::Clone() const
{
    LBracketNode* clone = new LBracketNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void LBracketNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RBracketNode::RBracketNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::rbracketNode, span_, fileIndex_)
{
}

Node* RBracketNode::Clone() const
{
    RBracketNode* clone = new RBracketNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void RBracketNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LBraceNode::LBraceNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::lbraceNode, span_, fileIndex_)
{
}

Node* LBraceNode::Clone() const
{
    LBraceNode* clone = new LBraceNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void LBraceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RBraceNode::RBraceNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::rbraceNode, span_, fileIndex_)
{
}

Node* RBraceNode::Clone() const
{
    RBraceNode* clone = new RBraceNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void RBraceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace otava::ast
