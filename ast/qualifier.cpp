// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.qualifier;

import otava.ast.visitor;

namespace otava::ast {

ConstNode::ConstNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::constNode, span_, fileIndex_)
{
}

Node* ConstNode::Clone() const
{
    ConstNode* clone = new ConstNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ConstNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

VolatileNode::VolatileNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::volatileNode, span_, fileIndex_)
{
}

Node* VolatileNode::Clone() const
{
    VolatileNode* clone = new VolatileNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void VolatileNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LvalueRefNode::LvalueRefNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::lvalueRefNode, span_, fileIndex_)
{
}

Node* LvalueRefNode::Clone() const
{
    LvalueRefNode* clone = new LvalueRefNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void LvalueRefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RvalueRefNode::RvalueRefNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::rvalueRefNode, span_, fileIndex_)
{
}

Node* RvalueRefNode::Clone() const
{
    RvalueRefNode* clone = new RvalueRefNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void RvalueRefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PtrNode::PtrNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::ptrNode, span_, fileIndex_)
{
}

Node* PtrNode::Clone() const
{
    PtrNode* clone = new PtrNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void PtrNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CVQualifierSequenceNode::CVQualifierSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept : SequenceNode(NodeKind::cvQualifierSequenceNode, span_, fileIndex_)
{
}

Node* CVQualifierSequenceNode::Clone() const
{
    CVQualifierSequenceNode* clone = new CVQualifierSequenceNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void CVQualifierSequenceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace otava::ast
