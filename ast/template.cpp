// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.templates;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

TemplateDeclarationNode::TemplateDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::templateDeclarationNode, span_, fileIndex_, nullptr, nullptr)
{
}

TemplateDeclarationNode::TemplateDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* templateHead_, Node* declaration_) noexcept :
    BinaryNode(NodeKind::templateDeclarationNode, span_, fileIndex_, templateHead_, declaration_)
{
}

Node* TemplateDeclarationNode::Clone() const
{
    TemplateDeclarationNode* clone = new TemplateDeclarationNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void TemplateDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TemplateHeadNode::TemplateHeadNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::templateHeadNode, span_, fileIndex_)
{
}

void TemplateHeadNode::SetTemplateParameterList(Node* templateParamList_) noexcept
{
    templateParamList.reset(templateParamList_);
}

void TemplateHeadNode::SetRequiresClause(Node* requiresClause_) noexcept
{
    requiresClause.reset(requiresClause_);
}

Node* TemplateHeadNode::Clone() const
{
    TemplateHeadNode* clone = new TemplateHeadNode(GetSpan(), FileIndex());
    if (templateParamList)
    {
        clone->SetTemplateParameterList(templateParamList->Clone());
    }
    if (requiresClause)
    {
        clone->SetRequiresClause(requiresClause->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void TemplateHeadNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TemplateHeadNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(templateParamList.get());
    writer.Write(requiresClause.get());
}

void TemplateHeadNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    templateParamList.reset(reader.ReadNode());
    requiresClause.reset(reader.ReadNode());
}

TemplateParameterListNode::TemplateParameterListNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    ListNode(NodeKind::templateParameterListNode, span_, fileIndex_)
{
}

Node* TemplateParameterListNode::Clone() const
{
    TemplateParameterListNode* clone = new TemplateParameterListNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetLAngleSpan(laSpan);
    clone->SetRAngleSpan(raSpan);
    clone->SetId(Id());
    return clone;
}

void TemplateParameterListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TemplateParameterListNode::Write(Writer& writer)
{
    ListNode::Write(writer);
    writer.Write(laSpan);
    writer.Write(raSpan);
}

void TemplateParameterListNode::Read(Reader& reader)
{
    ListNode::Read(reader);
    laSpan = reader.ReadSpan();
    raSpan = reader.ReadSpan();
}

TypeParameterNode::TypeParameterNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::typeParameterNode, span_, fileIndex_)
{
}

TypeParameterNode::TypeParameterNode(const soul::ast::Span& span_, int fileIndex_, Node* typeConstraint_, Node* identifier_, Node* assign_, Node* typeId_,
    Node* ellipsis_, Node* templateHead_) noexcept :
    CompoundNode(NodeKind::typeParameterNode, span_, fileIndex_), typeConstraint(typeConstraint_), identifier(identifier_), assign(assign_), typeId(typeId_), 
    ellipsis(ellipsis_), templateHead(templateHead_)
{
}

Node* TypeParameterNode::Clone() const
{
    Node* clonedAssign = nullptr;
    if (assign)
    {
        clonedAssign = assign->Clone();
    }
    Node* clonedTypeId = nullptr;
    if (typeId)
    {
        clonedTypeId = typeId->Clone();
    }
    Node* clonedEllipsis = nullptr;
    if (ellipsis)
    {
        clonedEllipsis = ellipsis->Clone();
    }
    Node* clonedTemplateHead = nullptr;
    if (templateHead)
    {
        clonedTemplateHead = templateHead->Clone();
    }
    TypeParameterNode* clone = new TypeParameterNode(GetSpan(), FileIndex(), typeConstraint->Clone(), identifier->Clone(), clonedAssign, clonedTypeId, 
        clonedEllipsis, clonedTemplateHead);
    clone->SetId(Id());
    return clone;
}

void TypeParameterNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeParameterNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(typeConstraint.get());
    writer.Write(identifier.get());
    writer.Write(assign.get());
    writer.Write(typeId.get());
    writer.Write(ellipsis.get());
    writer.Write(templateHead.get());
}

void TypeParameterNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    typeConstraint.reset(reader.ReadNode());
    identifier.reset(reader.ReadNode());
    assign.reset(reader.ReadNode());
    typeId.reset(reader.ReadNode());
    ellipsis.reset(reader.ReadNode());
    templateHead.reset(reader.ReadNode());
}

TemplateIdNode::TemplateIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::templateIdNode, span_, fileIndex_)
{
}

TemplateIdNode::TemplateIdNode(const soul::ast::Span& span_, int fileIndex_, Node* templateName_) noexcept :
    ListNode(NodeKind::templateIdNode, span_, fileIndex_), templateName(templateName_)
{
}

Node* TemplateIdNode::Clone() const
{
    TemplateIdNode* clone = new TemplateIdNode(GetSpan(), FileIndex(), templateName->Clone());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetLAngleSpan(laSpan);
    clone->SetRAngleSpan(raSpan);
    clone->SetId(Id());
    return clone;
}

void TemplateIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TemplateIdNode::Write(Writer& writer)
{
    ListNode::Write(writer);
    writer.Write(templateName.get());
    writer.Write(laSpan);
    writer.Write(raSpan);
}

void TemplateIdNode::Read(Reader& reader)
{
    ListNode::Read(reader);
    templateName.reset(reader.ReadNode());
    laSpan = reader.ReadSpan();
    raSpan = reader.ReadSpan();
}

void TemplateIdNode::SetTemplateArgKinds(const std::vector<bool>& templateArgKinds_)
{
    templateArgKinds = templateArgKinds_;
}

std::string TemplateIdNode::Str() const
{
    std::string str = templateName->Str();
    str.append(1, '<');
    str.append(ListNode::Str());
    str.append(1, '>');
    return str;
}

TypenameNode::TypenameNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::typenameNode, span_, fileIndex_)
{
}

Node* TypenameNode::Clone() const
{
    TypenameNode* clone = new TypenameNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void TypenameNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DeductionGuideNode::DeductionGuideNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::deductionGuideNode, span_, fileIndex_)
{
}

DeductionGuideNode::DeductionGuideNode(const soul::ast::Span& span_, int fileIndex_, Node* templateName_, Node* params_, Node* arrow_, Node* templateId_,
    Node* explicitSpecifier_, Node* semicolon_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::deductionGuideNode, span_, fileIndex_), templateName(templateName_), params(params_), arrow(arrow_), explicitSpecifier(explicitSpecifier_), 
    semicolon(semicolon_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* DeductionGuideNode::Clone() const
{
    Node* clonedExplicitSpecifier = nullptr;
    if (explicitSpecifier)
    {
        clonedExplicitSpecifier = explicitSpecifier->Clone();
    }
    DeductionGuideNode* clone = new DeductionGuideNode(GetSpan(), FileIndex(), templateName->Clone(), params->Clone(), arrow->Clone(), 
        templateId->Clone(), clonedExplicitSpecifier, semicolon->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void DeductionGuideNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DeductionGuideNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(templateName.get());
    writer.Write(params.get());
    writer.Write(arrow.get());
    writer.Write(templateId.get());
    writer.Write(explicitSpecifier.get());
    writer.Write(semicolon.get());
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void DeductionGuideNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    templateName.reset(reader.ReadNode());
    params.reset(reader.ReadNode());
    arrow.reset(reader.ReadNode());
    templateId.reset(reader.ReadNode());
    explicitSpecifier.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

ExplicitInstantiationNode::ExplicitInstantiationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::explicitInstantiationNode, span_, fileIndex_)
{
}

ExplicitInstantiationNode::ExplicitInstantiationNode(const soul::ast::Span& span_, int fileIndex_, Node* extrn_, Node* tmp_, Node* declaration_) noexcept :
    CompoundNode(NodeKind::explicitInstantiationNode, span_, fileIndex_), extrn(extrn_), tmp(tmp_), declaration(declaration_)
{
}

Node* ExplicitInstantiationNode::Clone() const
{
    Node* clonedExtrn = nullptr;
    if (extrn)
    {
        clonedExtrn = extrn->Clone();
    }
    ExplicitInstantiationNode* clone = new ExplicitInstantiationNode(GetSpan(), FileIndex(), clonedExtrn, tmp->Clone(), declaration->Clone());
    clone->SetId(Id());
    return clone;
}

void ExplicitInstantiationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExplicitInstantiationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(extrn.get());
    writer.Write(tmp.get());
    writer.Write(declaration.get());
}

void ExplicitInstantiationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    extrn.reset(reader.ReadNode());
    tmp.reset(reader.ReadNode());
    declaration.reset(reader.ReadNode());
}

TemplateNode::TemplateNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::templateNode, span_, fileIndex_)
{
}

Node* TemplateNode::Clone() const
{
    TemplateNode* clone = new TemplateNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void TemplateNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExplicitSpecializationNode::ExplicitSpecializationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::explicitSpecializationNode, span_, fileIndex_)
{
}

ExplicitSpecializationNode::ExplicitSpecializationNode(const soul::ast::Span& span_, int fileIndex_, Node* tmp_, Node* templateHeadNode_, Node* declaration_,
    const soul::ast::Span& laSpan_, const soul::ast::Span& raSpan_) noexcept :
    CompoundNode(NodeKind::explicitSpecializationNode, span_, fileIndex_), tmp(tmp_), templateHeadNode(templateHeadNode_), declaration(declaration_), 
    laSpan(laSpan_), raSpan(raSpan_)
{
}

Node* ExplicitSpecializationNode::Clone() const
{
    ExplicitSpecializationNode* clone = new ExplicitSpecializationNode(GetSpan(), FileIndex(), tmp->Clone(), templateHeadNode->Clone(), 
        declaration->Clone(), laSpan, raSpan);
    clone->SetId(Id());
    return clone;
}

void ExplicitSpecializationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExplicitSpecializationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(tmp.get());
    writer.Write(declaration.get());
    writer.Write(laSpan);
    writer.Write(raSpan);
}

void ExplicitSpecializationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    tmp.reset(reader.ReadNode());
    declaration.reset(reader.ReadNode());
    laSpan = reader.ReadSpan();
    raSpan = reader.ReadSpan();
}

} // namespace otava::ast
