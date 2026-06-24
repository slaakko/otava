// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.concepts;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

ConceptDefinitionNode::ConceptDefinitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::conceptDefinitionNode, span_, fileIndex_)
{
}

ConceptDefinitionNode::ConceptDefinitionNode(const soul::ast::Span& span_, int fileIndex_, Node* conceptName_, Node* assign_, Node* constraintExpr_, 
    Node* semicolon_) noexcept :
    CompoundNode(NodeKind::conceptDefinitionNode, span_, fileIndex_), conceptName(conceptName_), assign(assign_), constraintExpr(constraintExpr_), semicolon(semicolon_)
{
}

Node* ConceptDefinitionNode::Clone() const
{
    ConceptDefinitionNode* clone = new ConceptDefinitionNode(GetSpan(), FileIndex(), conceptName->Clone(), assign->Clone(), constraintExpr->Clone(), semicolon->Clone());
    clone->SetId(Id());
    return clone;
}

void ConceptDefinitionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConceptDefinitionNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(conceptName.get());
    writer.Write(assign.get());
    writer.Write(constraintExpr.get());
    writer.Write(semicolon.get());
}

void ConceptDefinitionNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    conceptName.reset(reader.ReadNode());
    assign.reset(reader.ReadNode());
    constraintExpr.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

RequiresExprNode::RequiresExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::requiresExprNode, span_, fileIndex_)
{
}

RequiresExprNode::RequiresExprNode(const soul::ast::Span& span_, int fileIndex_, Node* params_, Node* body_) noexcept :
    CompoundNode(NodeKind::requiresExprNode, span_, fileIndex_), params(params_), body(body_)
{
}

Node* RequiresExprNode::Clone() const
{
    Node* clonedParams = nullptr;
    if (params)
    {
        clonedParams = params->Clone();
    }
    RequiresExprNode* clone = new RequiresExprNode(GetSpan(), FileIndex(), clonedParams, body->Clone());
    clone->SetId(Id());
    return clone;
}

void RequiresExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RequiresExprNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(params.get());
    writer.Write(body.get());
}

void RequiresExprNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    params.reset(reader.ReadNode());
    body.reset(reader.ReadNode());
}

RequirementBodyNode::RequirementBodyNode(const soul::ast::Span& span_, int fileIndex_) noexcept : SequenceNode(NodeKind::requirementBodyNode, span_, fileIndex_)
{
}

Node* RequirementBodyNode::Clone() const
{
    RequirementBodyNode* clone = new RequirementBodyNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetLBraceSpan(lbSpan);
    clone->SetRBraceSpan(rbSpan);
    clone->SetId(Id());
    return clone;
}

void RequirementBodyNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RequirementBodyNode::Write(Writer& writer)
{
    SequenceNode::Write(writer);
    //writer.Write(lbSpan);
    //writer.Write(rbSpan);
}

void RequirementBodyNode::Read(Reader& reader)
{
    SequenceNode::Read(reader);
    //lbSpan = reader.ReadSpan();
    //rbSpan = reader.ReadSpan();
}

void RequirementBodyNode::SetLBraceSpan(const soul::ast::Span& lbSpan_) noexcept
{
    lbSpan = lbSpan_;
}

void RequirementBodyNode::SetRBraceSpan(const soul::ast::Span& rbSpan_) noexcept
{
    rbSpan = rbSpan_;
}

SimpleRequirementNode::SimpleRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::simpleRequirementNode, span_, fileIndex_, nullptr, nullptr)
{
}

SimpleRequirementNode::SimpleRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* expr_, Node* semicolon_) noexcept :
    BinaryNode(NodeKind::simpleRequirementNode, span_, fileIndex_, expr_, semicolon_)
{
}

Node* SimpleRequirementNode::Clone() const
{
    SimpleRequirementNode* clone = new SimpleRequirementNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void SimpleRequirementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeRequirementNode::TypeRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::typeRequirementNode, span_, fileIndex_)
{
}

TypeRequirementNode::TypeRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* nns_, Node* typeName_, Node* semicolon_) noexcept :
    CompoundNode(NodeKind::typeRequirementNode, span_, fileIndex_), nns(nns_), typeName(typeName_), semicolon(semicolon_)
{
}

Node* TypeRequirementNode::Clone() const
{
    Node* clonedNns = nullptr;
    if (nns)
    {
        clonedNns = nns->Clone();
    }
    TypeRequirementNode* clone = new TypeRequirementNode(GetSpan(), FileIndex(), clonedNns, typeName->Clone(), semicolon->Clone());
    clone->SetId(Id());
    return clone;
}

void TypeRequirementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeRequirementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(nns.get());
    writer.Write(typeName.get());
    writer.Write(semicolon.get());
}

void TypeRequirementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    nns.reset(reader.ReadNode());
    typeName.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

CompoundRequirementNode::CompoundRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::compoundRequirementNode, span_, fileIndex_)
{
}

CompoundRequirementNode::CompoundRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* expr_, Node* noexcept_, Node* returnTypeRequirement_, Node* semicolon_,
    const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    CompoundNode(NodeKind::compoundRequirementNode, span_, fileIndex_), expr(expr_), noexceptNode(noexcept_), returnTypeRequirement(returnTypeRequirement_), 
    semicolon(semicolon_), lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* CompoundRequirementNode::Clone() const
{
    Node* clonedNoExcept = nullptr;
    if (noexceptNode)
    {
        clonedNoExcept = noexceptNode->Clone();
    }
    Node* clonedReturnTypeRequirement = nullptr;
    if (returnTypeRequirement)
    {
        clonedReturnTypeRequirement = returnTypeRequirement->Clone();
    }
    CompoundRequirementNode* clone = new CompoundRequirementNode(GetSpan(), FileIndex(), expr->Clone(), clonedNoExcept, clonedReturnTypeRequirement, 
        semicolon->Clone(), lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void CompoundRequirementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CompoundRequirementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(expr.get());
    writer.Write(noexceptNode.get());
    writer.Write(returnTypeRequirement.get());
    writer.Write(semicolon.get());
    //writer.Write(lbSpan);
    //writer.Write(rbSpan);
}

void CompoundRequirementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    expr.reset(reader.ReadNode());
    noexceptNode.reset(reader.ReadNode());
    returnTypeRequirement.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    //lbSpan = reader.ReadSpan();
    //rbSpan = reader.ReadSpan();
}

ReturnTypeRequirementNode::ReturnTypeRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    UnaryNode(NodeKind::returnTypeRequirementNode, span_, fileIndex_, nullptr)
{
}

ReturnTypeRequirementNode::ReturnTypeRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* typeConstraint_) noexcept :
    UnaryNode(NodeKind::returnTypeRequirementNode, span_, fileIndex_, typeConstraint_)
{
}

Node* ReturnTypeRequirementNode::Clone() const
{
    ReturnTypeRequirementNode* clone = new ReturnTypeRequirementNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void ReturnTypeRequirementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NestedRequirementNode::NestedRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::nestedRequirementNode, span_, fileIndex_, nullptr, nullptr)
{
}

NestedRequirementNode::NestedRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* constraintExpression_, Node* semicolon_) noexcept :
    BinaryNode(NodeKind::nestedRequirementNode, span_, fileIndex_, constraintExpression_, semicolon_)
{
}

Node* NestedRequirementNode::Clone() const
{
    NestedRequirementNode* clone = new NestedRequirementNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void NestedRequirementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeConstraintNode::TypeConstraintNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    ListNode(NodeKind::typeConstraintNode, span_, fileIndex_), hasTemplateArgumentList(false)
{
}

TypeConstraintNode::TypeConstraintNode(const soul::ast::Span& span_, int fileIndex_, Node* conceptName_) noexcept :
    ListNode(NodeKind::typeConstraintNode, span_, fileIndex_), conceptName(conceptName_), hasTemplateArgumentList(false)
{
}

Node* TypeConstraintNode::Clone() const
{
    TypeConstraintNode* clone = new TypeConstraintNode(GetSpan(), FileIndex(), conceptName->Clone());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void TypeConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeConstraintNode::Write(Writer& writer)
{
    ListNode::Write(writer);
    writer.Write(conceptName.get());
    writer.Write(hasTemplateArgumentList);
    //writer.Write(laSpan);
    //writer.Write(raSpan);
}

void TypeConstraintNode::Read(Reader& reader)
{
    ListNode::Read(reader);
    conceptName.reset(reader.ReadNode());
    hasTemplateArgumentList = reader.ReadBool();
    //laSpan = reader.ReadSpan();
    //raSpan = reader.ReadSpan();
}

void TypeConstraintNode::SetLAngleSpan(const soul::ast::Span& laSpan_) noexcept
{
    laSpan = laSpan_;
}

void TypeConstraintNode::SetRAngleSpan(const soul::ast::Span& raSpan_) noexcept
{
    raSpan = raSpan_;
}

RequiresClauseNode::RequiresClauseNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::requiresClauseNode, span_, fileIndex_, nullptr)
{
}

RequiresClauseNode::RequiresClauseNode(const soul::ast::Span& span_, int fileIndex_, Node* constraintLogicalOrExpr_) noexcept :
    UnaryNode(NodeKind::requiresClauseNode, span_, fileIndex_, constraintLogicalOrExpr_)
{
}

Node* RequiresClauseNode::Clone() const
{
    RequiresClauseNode* clone = new RequiresClauseNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void RequiresClauseNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace otava::ast
