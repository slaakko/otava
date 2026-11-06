// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.attribute;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

AttributeSpecifierSequenceNode::AttributeSpecifierSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    SequenceNode(NodeKind::attributeSpecifierSequenceNode, span_, fileIndex_)
{
}

Node* AttributeSpecifierSequenceNode::Clone() const
{
    AttributeSpecifierSequenceNode* clone = new AttributeSpecifierSequenceNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void AttributeSpecifierSequenceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AttributeSpecifierNode::AttributeSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::attributeSpecifierNode, span_, fileIndex_)
{
}

Node* AttributeSpecifierNode::Clone() const
{
    AttributeSpecifierNode* clone = new AttributeSpecifierNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void AttributeSpecifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AttributeSpecifierNode::Write(Writer& writer)
{
    ListNode::Write(writer);
    writer.Write(usingPrefix.get());
}

void AttributeSpecifierNode::Read(Reader& reader)
{
    ListNode::Read(reader);
    usingPrefix.reset(reader.ReadNode());
}

AttributeUsingPrefixNode::AttributeUsingPrefixNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    UnaryNode(NodeKind::attributeUsingPrefixNode, span_, fileIndex_, nullptr)
{
}

AttributeUsingPrefixNode::AttributeUsingPrefixNode(const soul::ast::Span& span_, int fileIndex_, Node* atttributeNamespace_) noexcept :
    UnaryNode(NodeKind::attributeUsingPrefixNode, span_, fileIndex_, atttributeNamespace_)
{
}

Node* AttributeUsingPrefixNode::Clone() const
{
    AttributeUsingPrefixNode* clone = new AttributeUsingPrefixNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void AttributeUsingPrefixNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AttributeUsingPrefixNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
}

void AttributeUsingPrefixNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
}

AttributeNode::AttributeNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::attributeNode, span_, fileIndex_)
{
}

AttributeNode::AttributeNode(const soul::ast::Span& span_, int fileIndex_, Node* attributeToken_, Node* attributeArgs_) noexcept :
    CompoundNode(NodeKind::attributeNode, span_, fileIndex_), attributeToken(attributeToken_), attributeArgs(attributeArgs_)
{
}

Node* AttributeNode::Clone() const
{
    AttributeNode* clone = new AttributeNode(GetSpan(), FileIndex(), attributeToken->Clone(), attributeArgs->Clone());
    clone->SetId(Id());
    return clone;
}

void AttributeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AttributeNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(attributeToken.get());
    writer.Write(attributeArgs.get());
}

void AttributeNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    attributeToken.reset(reader.ReadNode());
    attributeArgs.reset(reader.ReadNode());
}

AttributeScopedTokenNode::AttributeScopedTokenNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::attributeScopedTokenNode, span_, fileIndex_)
{
}

AttributeScopedTokenNode::AttributeScopedTokenNode(const soul::ast::Span& span_, int fileIndex_, Node* ns_, Node* colonColon_, Node* identifier_) noexcept :
    CompoundNode(NodeKind::attributeScopedTokenNode, span_, fileIndex_), ns(ns_), colonColon(colonColon_), identifier(identifier_)
{
}

Node* AttributeScopedTokenNode::Clone() const
{
    AttributeScopedTokenNode* clone = new AttributeScopedTokenNode(GetSpan(), FileIndex(), ns->Clone(), colonColon->Clone(), identifier->Clone());
    clone->SetId(Id());
    return clone;
}

void AttributeScopedTokenNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AttributeScopedTokenNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(ns.get());
    writer.Write(colonColon.get());
    writer.Write(identifier.get());
}

void AttributeScopedTokenNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    ns.reset(reader.ReadNode());
    colonColon.reset(reader.ReadNode());
    identifier.reset(reader.ReadNode());
}

AttributeArgumentsNode::AttributeArgumentsNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::attributeArgumentsNode, span_, fileIndex_)
{
}

AttributeArgumentsNode::AttributeArgumentsNode(const soul::ast::Span& span_, int fileIndex_, Node* balancedTokenSequence_) noexcept :
    CompoundNode(NodeKind::attributeArgumentsNode, span_, fileIndex_), balancedTokenSequence(balancedTokenSequence_)
{
}

Node* AttributeArgumentsNode::Clone() const
{
    AttributeArgumentsNode* clone = new AttributeArgumentsNode(GetSpan(), FileIndex(), balancedTokenSequence->Clone());
    clone->SetId(Id());
    return clone;
}

void AttributeArgumentsNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AttributeArgumentsNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(balancedTokenSequence.get());
}

void AttributeArgumentsNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    balancedTokenSequence.reset(reader.ReadNode());
}

BalancedTokenSequenceNode::BalancedTokenSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    SequenceNode(NodeKind::balancedTokenSequenceNode, span_, fileIndex_)
{
}

Node* BalancedTokenSequenceNode::Clone() const
{
    BalancedTokenSequenceNode* clone = new BalancedTokenSequenceNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void BalancedTokenSequenceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TokenNode::TokenNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::tokenNode, span_, fileIndex_)
{
}

TokenNode::TokenNode(const soul::ast::Span& span_, int fileIndex_, const std::string& str_) : CompoundNode(NodeKind::tokenNode, span_, fileIndex_), str(str_)
{
}

Node* TokenNode::Clone() const
{
    TokenNode* clone = new TokenNode(GetSpan(), FileIndex(), str);
    clone->SetId(Id());
    return clone;
}

void TokenNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TokenNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(str);
}

void TokenNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    str = reader.ReadStr();
}

AlignmentSpecifierNode::AlignmentSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::alignmentSpecifierNode, span_, fileIndex_)
{
}

AlignmentSpecifierNode::AlignmentSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* alignment_, Node* ellipsis_) noexcept :
    CompoundNode(NodeKind::alignmentSpecifierNode, span_, fileIndex_), alignment(alignment_), ellipsis(ellipsis_)
{
}

Node* AlignmentSpecifierNode::Clone() const
{
    Node* clonedEllipsis = nullptr;
    if (ellipsis)
    {
        clonedEllipsis = ellipsis->Clone();
    }
    AlignmentSpecifierNode* clone = new AlignmentSpecifierNode(GetSpan(), FileIndex(), alignment->Clone(), clonedEllipsis);
    clone->SetId(Id());
    return clone;
}

void AlignmentSpecifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AlignmentSpecifierNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(alignment.get());
    writer.Write(ellipsis.get());
}

void AlignmentSpecifierNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    alignment.reset(reader.ReadNode());
    ellipsis.reset(reader.ReadNode());
}

} // namespace otava::ast
