// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.lambda;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

LambdaExpressionNode::LambdaExpressionNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::lambdaExpressionNode, span_, fileIndex_)
{
}

LambdaExpressionNode::LambdaExpressionNode(const soul::ast::Span& span_, int fileIndex_, Node* introducer_, Node* templateParams_, Node* declarator_, 
    Node* body_) noexcept :
    CompoundNode(NodeKind::lambdaExpressionNode, span_, fileIndex_), introducer(introducer_), templateParams(templateParams_), declarator(declarator_), body(body_)
{
}

Node* LambdaExpressionNode::Clone() const
{
    Node* clonedTemplateParams = nullptr;
    if (templateParams)
    {
        clonedTemplateParams = templateParams->Clone();
    }
    LambdaExpressionNode* clone = new LambdaExpressionNode(GetSpan(), FileIndex(), introducer->Clone(), clonedTemplateParams, declarator->Clone(), body->Clone());
    clone->SetId(Id());
    return clone;
}

void LambdaExpressionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LambdaExpressionNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(introducer.get());
    writer.Write(templateParams.get());
    writer.Write(declarator.get());
    writer.Write(body.get());
}

void LambdaExpressionNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    introducer.reset(reader.ReadNode());
    templateParams.reset(reader.ReadNode());
    declarator.reset(reader.ReadNode());
    body.reset(reader.ReadNode());
}

LambdaIntroducerNode::LambdaIntroducerNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::lambdaIntroducerNode, span_, fileIndex_)
{
}

LambdaIntroducerNode::LambdaIntroducerNode(const soul::ast::Span& span_, int fileIndex_, Node* capture_,
    const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    CompoundNode(NodeKind::lambdaIntroducerNode, span_, fileIndex_), capture(capture_), lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* LambdaIntroducerNode::Clone() const
{
    Node* clonedCapture = nullptr;
    if (capture)
    {
        clonedCapture = capture->Clone();
    }
    LambdaIntroducerNode* clone = new LambdaIntroducerNode(GetSpan(), FileIndex(), clonedCapture, lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void LambdaIntroducerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LambdaIntroducerNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(capture.get());
    writer.Write(lbSpan);
    writer.Write(rbSpan);
}

void LambdaIntroducerNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    capture.reset(reader.ReadNode());
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
}

LambdaCaptureNode::LambdaCaptureNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::lambdaCaptureNode, span_, fileIndex_)
{
}

Node* LambdaCaptureNode::Clone() const
{
    LambdaCaptureNode* clone = new LambdaCaptureNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void LambdaCaptureNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DefaultRefCaptureNode::DefaultRefCaptureNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::defaultRefCaptureNode, span_, fileIndex_)
{
}

Node* DefaultRefCaptureNode::Clone() const
{
    DefaultRefCaptureNode* clone = new DefaultRefCaptureNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DefaultRefCaptureNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DefaultCopyCaptureNode::DefaultCopyCaptureNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::defaultCopyCaptureNode, span_, fileIndex_)
{
}

Node* DefaultCopyCaptureNode::Clone() const
{
    DefaultCopyCaptureNode* clone = new DefaultCopyCaptureNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DefaultCopyCaptureNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ByRefCaptureNode::ByRefCaptureNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::byRefCaptureNode, span_, fileIndex_)
{
}

Node* ByRefCaptureNode::Clone() const
{
    ByRefCaptureNode* clone = new ByRefCaptureNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ByRefCaptureNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SimpleCaptureNode::SimpleCaptureNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::simpleCaptureNode, span_, fileIndex_)
{
}

SimpleCaptureNode::SimpleCaptureNode(const soul::ast::Span& span_, int fileIndex_, Node* identifier_, Node* byRefCapture_, Node* ellipsis_) noexcept :
    CompoundNode(NodeKind::simpleCaptureNode, span_, fileIndex_), identifier(identifier_), byRefCapture(byRefCapture_), ellipsis(ellipsis_)
{
}

Node* SimpleCaptureNode::Clone() const
{
    Node* clonedByRefCapture = nullptr;
    if (byRefCapture)
    {
        clonedByRefCapture = byRefCapture->Clone();
    }
    Node* clonedEllipsis = nullptr;
    if (ellipsis)
    {
        clonedEllipsis = ellipsis->Clone();
    }
    SimpleCaptureNode* clone = new SimpleCaptureNode(GetSpan(), FileIndex(), identifier->Clone(), clonedByRefCapture, clonedEllipsis);
    clone->SetId(Id());
    return clone;
}

void SimpleCaptureNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SimpleCaptureNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(identifier.get());
    writer.Write(byRefCapture.get());
    writer.Write(ellipsis.get());
}

void SimpleCaptureNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    identifier.reset(reader.ReadNode());
    byRefCapture.reset(reader.ReadNode());
    ellipsis.reset(reader.ReadNode());
}

CurrentObjectCopyCapture::CurrentObjectCopyCapture(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::currentObjectCopyCapture, span_, fileIndex_)
{
}

CurrentObjectCopyCapture::CurrentObjectCopyCapture(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& thisSpan_) noexcept :
    CompoundNode(NodeKind::currentObjectCopyCapture, span_, fileIndex_), thisSpan(thisSpan_)
{
}

Node* CurrentObjectCopyCapture::Clone() const
{
    CurrentObjectCopyCapture* clone = new CurrentObjectCopyCapture(GetSpan(), FileIndex(), thisSpan);
    clone->SetId(Id());
    return clone;
}

void CurrentObjectCopyCapture::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CurrentObjectCopyCapture::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(thisSpan);
}

void CurrentObjectCopyCapture::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    thisSpan = reader.ReadSpan();
}

CurrentObjectByRefCapture::CurrentObjectByRefCapture(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::currentObjectByRefCapture, span_, fileIndex_)
{
}

CurrentObjectByRefCapture::CurrentObjectByRefCapture(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& thisSpan_) noexcept :
    CompoundNode(NodeKind::currentObjectByRefCapture, span_, fileIndex_), thisSpan(thisSpan_)
{
}

Node* CurrentObjectByRefCapture::Clone() const
{
    CurrentObjectByRefCapture* clone = new CurrentObjectByRefCapture(GetSpan(), FileIndex(), thisSpan);
    clone->SetId(Id());
    return clone;
}

void CurrentObjectByRefCapture::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CurrentObjectByRefCapture::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(thisSpan);
}

void CurrentObjectByRefCapture::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    thisSpan = reader.ReadSpan();
}

InitCaptureNode::InitCaptureNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::initCaptureNode, span_, fileIndex_)
{
}

InitCaptureNode::InitCaptureNode(const soul::ast::Span& span_, int fileIndex_, Node* identifier_, Node* initializer_, Node* byRefCapture_, Node* ellipsis_) noexcept :
    CompoundNode(NodeKind::initCaptureNode, span_, fileIndex_), identifier(identifier_), initializer(initializer_), byRefCapture(byRefCapture_), ellipsis(ellipsis_)
{
}

Node* InitCaptureNode::Clone() const
{
    Node* clonedByRefCapture = nullptr;
    if (byRefCapture)
    {
        clonedByRefCapture = byRefCapture->Clone();
    }
    Node* clonedEllipsis = nullptr;
    if (ellipsis)
    {
        clonedEllipsis = ellipsis->Clone();
    }
    InitCaptureNode* clone = new InitCaptureNode(GetSpan(), FileIndex(), identifier->Clone(), initializer->Clone(), clonedByRefCapture, clonedEllipsis);
    clone->SetId(Id());
    return clone;
}

void InitCaptureNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void InitCaptureNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(identifier.get());
    writer.Write(initializer.get());
    writer.Write(byRefCapture.get());
    writer.Write(ellipsis.get());
}

void InitCaptureNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    identifier.reset(reader.ReadNode());
    initializer.reset(reader.ReadNode());
    byRefCapture.reset(reader.ReadNode());
    ellipsis.reset(reader.ReadNode());
}

LambdaDeclaratorNode::LambdaDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::lambdaDeclaratorNode, span_, fileIndex_)
{
}

LambdaDeclaratorNode::LambdaDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* parameterList_, Node* specifiers_, Node* requiresClause_) noexcept :
    CompoundNode(NodeKind::lambdaDeclaratorNode, span_, fileIndex_), parameterList(parameterList_), specifiers(specifiers_), requiresClause(requiresClause_)
{
}

Node* LambdaDeclaratorNode::Clone() const
{
    Node* clonedRequiresClause = nullptr;
    if (requiresClause)
    {
        clonedRequiresClause = requiresClause->Clone();
    }
    Node* clonedParameterList = nullptr;
    if (parameterList)
    {
        clonedParameterList = parameterList->Clone();
    }
    LambdaDeclaratorNode* clone = new LambdaDeclaratorNode(GetSpan(), FileIndex(), clonedParameterList, specifiers->Clone(), clonedRequiresClause);
    clone->SetId(Id());
    return clone;
}

void LambdaDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LambdaDeclaratorNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(parameterList.get());
    writer.Write(specifiers.get());
    writer.Write(requiresClause.get());
}

void LambdaDeclaratorNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    parameterList.reset(reader.ReadNode());
    specifiers.reset(reader.ReadNode());
    requiresClause.reset(reader.ReadNode());
}

LambdaSpecifiersNode::LambdaSpecifiersNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::lambdaSpecifiersNode, span_, fileIndex_)
{
}

LambdaSpecifiersNode::LambdaSpecifiersNode(const soul::ast::Span& span_, int fileIndex_, Node* declSpecifiers_, Node* noexceptSpecifier_, Node* attributes_,
    Node* trailingReturnType_) noexcept :
    CompoundNode(NodeKind::lambdaSpecifiersNode, span_, fileIndex_), declSpecifiers(declSpecifiers_), noexceptSpecifier(noexceptSpecifier_),
    attributes(attributes_), trailingReturnType(trailingReturnType_)
{
}

Node* LambdaSpecifiersNode::Clone() const
{
    Node* clonedDeclSpecifiers = nullptr;
    if (declSpecifiers)
    {
        clonedDeclSpecifiers = declSpecifiers->Clone();
    }
    Node* clonedNoexceptSpecifier = nullptr;
    if (noexceptSpecifier)
    {
        clonedNoexceptSpecifier = noexceptSpecifier->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    Node* clonedTrailingReturnType = nullptr;
    if (trailingReturnType)
    {
        clonedTrailingReturnType = trailingReturnType->Clone();
    }
    LambdaSpecifiersNode* clone = new LambdaSpecifiersNode(GetSpan(), FileIndex(), clonedDeclSpecifiers, clonedNoexceptSpecifier, 
        clonedAttributes, clonedTrailingReturnType);
    clone->SetId(Id());
    return clone;
}

void LambdaSpecifiersNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LambdaSpecifiersNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(declSpecifiers.get());
    writer.Write(noexceptSpecifier.get());
    writer.Write(attributes.get());
    writer.Write(trailingReturnType.get());
}

void LambdaSpecifiersNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    declSpecifiers.reset(reader.ReadNode());
    noexceptSpecifier.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    trailingReturnType.reset(reader.ReadNode());
}

bool LambdaSpecifiersNode::IsEmpty() const
{
    return !declSpecifiers && !noexceptSpecifier && !attributes && !trailingReturnType;
}

LambdaTemplateParamsNode::LambdaTemplateParamsNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    CompoundNode(NodeKind::lambdaTemplateParamsNode, span_, fileIndex_)
{
}

LambdaTemplateParamsNode::LambdaTemplateParamsNode(const soul::ast::Span& span_, int fileIndex_, Node* templateParams_, Node* requiresClause_) noexcept :
    CompoundNode(NodeKind::lambdaTemplateParamsNode, span_, fileIndex_), templateParams(templateParams_), requiresClause(requiresClause_)
{
}

Node* LambdaTemplateParamsNode::Clone() const
{
    Node* clonedRequiresClause = nullptr;
    if (requiresClause)
    {
        clonedRequiresClause = requiresClause->Clone();
    }
    LambdaTemplateParamsNode* clone = new LambdaTemplateParamsNode(GetSpan(), FileIndex(), templateParams->Clone(), clonedRequiresClause);
    clone->SetId(Id());
    return clone;
}

void LambdaTemplateParamsNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LambdaTemplateParamsNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(templateParams.get());
    writer.Write(requiresClause.get());
}

void LambdaTemplateParamsNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    templateParams.reset(reader.ReadNode());
    requiresClause.reset(reader.ReadNode());
}

} // namespace otava::ast
