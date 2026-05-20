// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.identifier;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

IdentifierNode::IdentifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::identifierNode, span_, fileIndex_)
{
}

IdentifierNode::IdentifierNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& str_) noexcept :
    CompoundNode(NodeKind::identifierNode, span_, fileIndex_), str(str_)
{
}

Node* IdentifierNode::Clone() const
{
    IdentifierNode* clone = new IdentifierNode(GetSpan(), FileIndex(), str);
    clone->SetId(Id());
    return clone;
}

void IdentifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IdentifierNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(str);
}

void IdentifierNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    str = reader.ReadStr();
}

UnnamedNode::UnnamedNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::unnamedNode, span_, fileIndex_)
{
}

Node* UnnamedNode::Clone() const
{
    UnnamedNode* clone = new UnnamedNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void UnnamedNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ColonColonNode::ColonColonNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::colonColonNode, span_, fileIndex_)
{
}

Node* ColonColonNode::Clone() const
{
    ColonColonNode* clone = new ColonColonNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ColonColonNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NestedNameSpecifierNode::NestedNameSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    SequenceNode(NodeKind::nestedNameSpecifierNode, span_, fileIndex_)
{
}

Node* NestedNameSpecifierNode::Clone() const
{
    NestedNameSpecifierNode* clone = new NestedNameSpecifierNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void NestedNameSpecifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

QualifiedIdNode::QualifiedIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept : BinaryNode(NodeKind::qualifiedIdNode, span_, fileIndex_, nullptr, nullptr)
{
}

QualifiedIdNode::QualifiedIdNode(const soul::ast::Span& span_, int fileIndex_, Node* nns_, Node* unqualifiedId_) noexcept :
    BinaryNode(NodeKind::qualifiedIdNode, span_, fileIndex_, nns_, unqualifiedId_)
{
}

Node* QualifiedIdNode::Clone() const
{
    QualifiedIdNode* clone = new QualifiedIdNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void QualifiedIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IdentifierListNode::IdentifierListNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::identifierListNode, span_, fileIndex_)
{
}

Node* IdentifierListNode::Clone() const
{
    IdentifierListNode* clone = new IdentifierListNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void IdentifierListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ModuleNameNode::ModuleNameNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::moduleNameNode, span_, fileIndex_)
{
}

ModuleNameNode::ModuleNameNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& str_) : Node(NodeKind::moduleNameNode, span_, fileIndex_), str(str_)
{
}

Node* ModuleNameNode::Clone() const
{
    ModuleNameNode* clone = new ModuleNameNode(GetSpan(), FileIndex(), str);
    clone->SetId(Id());
    return clone;
}

void ModuleNameNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ModuleNameNode::Write(Writer& writer)
{
    Node::Write(writer);
    writer.Write(str);
}

void ModuleNameNode::Read(Reader& reader)
{
    Node::Read(reader);
    str = reader.ReadStr();
}

} // namespace otava::ast
