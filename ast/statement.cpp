// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.statement;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

LabeledStatementNode::LabeledStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::labeledStatementNode, span_, fileIndex_)
{
}

LabeledStatementNode::LabeledStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* label_, Node* stmt_, Node* attributes_,
    const soul::ast::Span& colonSpan_) noexcept :
    CompoundNode(NodeKind::labeledStatementNode, span_, fileIndex_), label(label_), stmt(stmt_), attributes(attributes_), colonSpan(colonSpan_)
{
}

Node* LabeledStatementNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    LabeledStatementNode* clone = new LabeledStatementNode(GetSpan(), FileIndex(), label->Clone(), stmt->Clone(), clonedAttributes, colonSpan);
    clone->SetId(Id());
    return clone;
}

void LabeledStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LabeledStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(label.get());
    writer.Write(stmt.get());
    writer.Write(attributes.get());
    writer.Write(colonSpan);
}

void LabeledStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    label.reset(reader.ReadNode());
    stmt.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    colonSpan = reader.ReadSpan();
}

CaseStatementNode::CaseStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::caseStatmentNode, span_, fileIndex_)
{
}

CaseStatementNode::CaseStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* caseExpr_, Node* stmt_, Node* attributes_,
    const soul::ast::Span& caseSpan_, const soul::ast::Span& colonSpan_) noexcept :
    CompoundNode(NodeKind::caseStatmentNode, span_, fileIndex_), caseExpr(caseExpr_), stmt(stmt_), attributes(attributes_), caseSpan(caseSpan_), colonSpan(colonSpan_)
{
}

Node* CaseStatementNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    CaseStatementNode* clone = new CaseStatementNode(GetSpan(), FileIndex(), caseExpr->Clone(), stmt->Clone(), clonedAttributes, caseSpan, colonSpan);
    clone->SetId(Id());
    return clone;
}

void CaseStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CaseStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(caseExpr.get());
    writer.Write(stmt.get());
    writer.Write(attributes.get());
    writer.Write(caseSpan);
    writer.Write(colonSpan);
}

void CaseStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    caseExpr.reset(reader.ReadNode());
    stmt.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    caseSpan = reader.ReadSpan();
    colonSpan = reader.ReadSpan();
}

DefaultStatementNode::DefaultStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::defaultStatementNode, span_, fileIndex_)
{
}

DefaultStatementNode::DefaultStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* stmt_, Node* attributes_, const soul::ast::Span& defaultSpan_,
    const soul::ast::Span& colonSpan_) noexcept :
    CompoundNode(NodeKind::defaultStatementNode, span_, fileIndex_), stmt(stmt_), attributes(attributes_), defaultSpan(defaultSpan_), colonSpan(colonSpan_)
{
}

Node* DefaultStatementNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    DefaultStatementNode* clone = new DefaultStatementNode(GetSpan(), FileIndex(), stmt->Clone(), clonedAttributes, defaultSpan, colonSpan);
    clone->SetId(Id());
    return clone;
}

void DefaultStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DefaultStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(stmt.get());
    writer.Write(attributes.get());
    writer.Write(defaultSpan);
    writer.Write(colonSpan);
}

void DefaultStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    stmt.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    defaultSpan = reader.ReadSpan();
    colonSpan = reader.ReadSpan();
}

CompoundStatementNode::CompoundStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    SequenceNode(NodeKind::compoundStatementNode, span_, fileIndex_), functionScope(nullptr), blockId(-1)
{
}

int CompoundStatementNode::Level() const noexcept
{
    Node* parent = Parent();
    while (parent)
    {
        if (parent->Kind() == NodeKind::compoundStatementNode)
        {
            CompoundStatementNode* compoundStatement = static_cast<CompoundStatementNode*>(parent);
            return compoundStatement->Level() + 1;
        }
        parent = parent->Parent();
    }
    return 0;
}

Node* CompoundStatementNode::Clone() const
{
    CompoundStatementNode* clone = new CompoundStatementNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    if (attributes)
    {
        clone->SetAttributes(attributes->Clone());
    }
    clone->SetLBraceSpan(lbSpan);
    clone->SetRBraceSpan(rbSpan);
    clone->SetBlockId(blockId);
    clone->SetId(Id());
    return clone;
}

void CompoundStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CompoundStatementNode::Write(Writer& writer)
{
    SequenceNode::Write(writer);
    writer.Write(attributes.get());
    writer.Write(lbSpan);
    writer.Write(rbSpan);
    writer.GetBinaryStreamWriter().Write(blockId);
}

void CompoundStatementNode::Read(Reader& reader)
{
    SequenceNode::Read(reader);
    attributes.reset(reader.ReadNode());
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
    blockId = reader.GetBinaryStreamReader().ReadInt();
}

void CompoundStatementNode::SetLexerPosPair(const soul::ast::lexer::pos::pair::LexerPosPair& lexerPosPair_) noexcept
{
    lexerPosPair = lexerPosPair_;
}

SequenceStatementNode::SequenceStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::sequenceStatementNode, span_, fileIndex_)
{
}

SequenceStatementNode::SequenceStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* firstStmt_, Node* secondStmt_) noexcept :
    CompoundNode(NodeKind::sequenceStatementNode, span_, fileIndex_), firstStmt(firstStmt_), secondStmt(secondStmt_)
{
}

Node* SequenceStatementNode::Clone() const
{
    SequenceStatementNode* clone = new SequenceStatementNode(GetSpan(), FileIndex(), firstStmt->Clone(), secondStmt->Clone());
    clone->SetId(Id());
    return clone;
}

void SequenceStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SequenceStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(firstStmt.get());
    writer.Write(secondStmt.get());
}

void SequenceStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    firstStmt.reset(reader.ReadNode());
    secondStmt.reset(reader.ReadNode());
}

IfStatementNode::IfStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::ifStatementNode, span_, fileIndex_), blockId(-1)
{
}

IfStatementNode::IfStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* cond_, Node* thenStmt_, Node* elseStmt_, Node* attributes_,
    const soul::ast::Span& ifSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_, const soul::ast::Span& constExprSpan_, 
    const soul::ast::Span& elseSpan_) noexcept :
    CompoundNode(NodeKind::ifStatementNode, span_, fileIndex_), cond(cond_), thenStmt(thenStmt_), elseStmt(elseStmt_), attributes(attributes_),
    ifSpan(ifSpan_), lpSpan(lpSpan_), rpSpan(rpSpan_), constExprSpan(constExprSpan_), elseSpan(elseSpan_), blockId(-1)
{
}

Node* IfStatementNode::Clone() const
{
    Node* clonedElseStmt = nullptr;
    if (elseStmt)
    {
        clonedElseStmt = elseStmt->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    IfStatementNode* clone = new IfStatementNode(GetSpan(), FileIndex(), cond->Clone(), thenStmt->Clone(), clonedElseStmt, clonedAttributes,
        ifSpan, lpSpan, rpSpan, constExprSpan, elseSpan);
    clone->SetBlockId(blockId);
    clone->SetId(Id());
    return clone;
}

void IfStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IfStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(cond.get());
    writer.Write(thenStmt.get());
    writer.Write(elseStmt.get());
    writer.Write(attributes.get());
    writer.Write(ifSpan);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
    writer.Write(constExprSpan);
    writer.Write(elseSpan);
    writer.GetBinaryStreamWriter().Write(blockId);
}

void IfStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    cond.reset(reader.ReadNode());
    thenStmt.reset(reader.ReadNode());
    elseStmt.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    ifSpan = reader.ReadSpan();
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
    constExprSpan = reader.ReadSpan();
    elseSpan = reader.ReadSpan();
    blockId = reader.GetBinaryStreamReader().ReadInt();
}

SwitchStatementNode::SwitchStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::switchStatemeNode, span_, fileIndex_), blockId(-1)
{
}

SwitchStatementNode::SwitchStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* cond_, Node* stmt_, Node* attributes_, const soul::ast::Span& switchSpan_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::switchStatemeNode, span_, fileIndex_), cond(cond_), stmt(stmt_), attributes(attributes_), switchSpan(switchSpan_), 
    lpSpan(lpSpan_), rpSpan(rpSpan_), blockId(-1)
{
}

Node* SwitchStatementNode::Clone() const
{
    Node* clonedInitStmt = nullptr;
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    SwitchStatementNode* clone = new SwitchStatementNode(GetSpan(), FileIndex(), cond->Clone(), stmt->Clone(), clonedAttributes, switchSpan, lpSpan, rpSpan);
    clone->SetBlockId(blockId);
    clone->SetId(Id());
    return clone;
}

void SwitchStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SwitchStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(cond.get());
    writer.Write(stmt.get());
    writer.Write(attributes.get());
    writer.Write(switchSpan);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
    writer.GetBinaryStreamWriter().Write(blockId);
}

void SwitchStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    cond.reset(reader.ReadNode());
    stmt.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    switchSpan = reader.ReadSpan();
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
    blockId = reader.GetBinaryStreamReader().ReadInt();
}

WhileStatementNode::WhileStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::whileStatementNode, span_, fileIndex_), blockId(-1)
{
}

WhileStatementNode::WhileStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* cond_, Node* stmt_, Node* attributes_, const soul::ast::Span& whileSpan_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::whileStatementNode, span_, fileIndex_), cond(cond_), stmt(stmt_), attributes(attributes_), whileSpan(whileSpan_), 
    lpSpan(lpSpan_), rpSpan(rpSpan_), blockId(-1)
{
}

Node* WhileStatementNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    WhileStatementNode* clone = new WhileStatementNode(GetSpan(), FileIndex(), cond->Clone(), stmt->Clone(), clonedAttributes, whileSpan, lpSpan, rpSpan);
    clone->SetBlockId(blockId);
    clone->SetId(Id());
    return clone;
}

void WhileStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void WhileStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(cond.get());
    writer.Write(stmt.get());
    writer.Write(attributes.get());
    writer.Write(whileSpan);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
    writer.GetBinaryStreamWriter().Write(blockId);
}

void WhileStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    cond.reset(reader.ReadNode());
    stmt.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    whileSpan = reader.ReadSpan();
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
    blockId = reader.GetBinaryStreamReader().ReadInt();
}

DoStatementNode::DoStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::doStatementNode, span_, fileIndex_)
{
}

DoStatementNode::DoStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* stmt_, Node* expr_, Node* attributes_, Node* semicolon_,
    const soul::ast::Span& doSpan_, const soul::ast::Span& whileSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::doStatementNode, span_, fileIndex_), stmt(stmt_), expr(expr_), attributes(attributes_), semicolon(semicolon_), doSpan(doSpan_), 
    whileSpan(whileSpan_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* DoStatementNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    DoStatementNode* clone = new DoStatementNode(GetSpan(), FileIndex(), stmt->Clone(), expr->Clone(), clonedAttributes, semicolon->Clone(), doSpan, 
        whileSpan, lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void DoStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DoStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(stmt.get());
    writer.Write(expr.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
    writer.Write(doSpan);
    writer.Write(whileSpan);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void DoStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    stmt.reset(reader.ReadNode());
    expr.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    doSpan = reader.ReadSpan();
    whileSpan = reader.ReadSpan();
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

RangeForStatementNode::RangeForStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::rangeForStatementNode, span_, fileIndex_), blockId(-1), rangeForId(util::random_uuid())
{
}

RangeForStatementNode::RangeForStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* initStmt_, Node* declaration_, Node* initializer_, Node* stmt_,
    Node* attributes_, const soul::ast::Span& forSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_, 
    const soul::ast::Span& colonSpan_) noexcept :
    CompoundNode(NodeKind::rangeForStatementNode, span_, fileIndex_), initStmt(initStmt_), declaration(declaration_), initializer(initializer_), stmt(stmt_), 
    attributes(attributes_), forSpan(forSpan_), lpSpan(lpSpan_), rpSpan(rpSpan_), colonSpan(colonSpan_), blockId(-1), rangeForId(util::random_uuid())
{
}

Node* RangeForStatementNode::Clone() const
{
    Node* clonedInitStmt = nullptr;
    if (initStmt)
    {
        clonedInitStmt = initStmt->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    RangeForStatementNode* clone = new RangeForStatementNode(GetSpan(), FileIndex(), clonedInitStmt, declaration->Clone(), initializer->Clone(), 
        stmt->Clone(), clonedAttributes, forSpan, lpSpan, rpSpan, colonSpan);
    clone->SetBlockId(blockId);
    clone->SetRangeForId(rangeForId);
    clone->SetId(Id());
    return clone;
}

void RangeForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RangeForStatementNode::SetRangeForId(const util::uuid& rangeForId_) noexcept
{
    rangeForId = rangeForId_;
}

void RangeForStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(initStmt.get());
    writer.Write(declaration.get());
    writer.Write(initializer.get());
    writer.Write(stmt.get());
    writer.Write(attributes.get());
    writer.Write(forSpan);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
    writer.Write(colonSpan);
    writer.GetBinaryStreamWriter().Write(blockId);
    writer.GetBinaryStreamWriter().Write(rangeForId);
}

void RangeForStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    initStmt.reset(reader.ReadNode());
    declaration.reset(reader.ReadNode());
    initializer.reset(reader.ReadNode());
    stmt.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    forSpan = reader.ReadSpan();
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
    colonSpan = reader.ReadSpan();
    blockId = reader.GetBinaryStreamReader().ReadInt();
    reader.GetBinaryStreamReader().ReadUuid(rangeForId);
}

ForRangeDeclarationNode::ForRangeDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::forRangeDeclarationNode, span_, fileIndex_, nullptr, nullptr)
{
}

ForRangeDeclarationNode::ForRangeDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* declSpecifierSeq_, Node* declarator_, Node* attributes_) noexcept :
    BinaryNode(NodeKind::forRangeDeclarationNode, span_, fileIndex_, declSpecifierSeq_, declarator_), attributes(attributes_)
{
}

Node* ForRangeDeclarationNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    ForRangeDeclarationNode* clone = new ForRangeDeclarationNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone(), clonedAttributes);
    clone->SetId(Id());
    return clone;
}

void ForRangeDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ForRangeDeclarationNode::Write(Writer& writer)
{
    BinaryNode::Write(writer);
    writer.Write(attributes.get());
}

void ForRangeDeclarationNode::Read(Reader& reader)
{
    BinaryNode::Read(reader);
    attributes.reset(reader.ReadNode());
}

StructuredBindingNode::StructuredBindingNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::structuredBindingNode, span_, fileIndex_)
{
}

StructuredBindingNode::StructuredBindingNode(const soul::ast::Span& span_, int fileIndex_, Node* declSpecifierSeq_, Node* refQualifier_, 
    Node* identifiers_, Node* initializer_, Node* attributes_, Node* semicolon_, const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    CompoundNode(NodeKind::structuredBindingNode, span_, fileIndex_), declSpecifiers(declSpecifierSeq_), refQualifier(refQualifier_), identifiers(identifiers_), 
    initializer(initializer_), attributes(attributes_), semicolon(semicolon_), lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* StructuredBindingNode::Clone() const
{
    Node* clonedRefQualifier = nullptr;
    if (refQualifier)
    {
        clonedRefQualifier = refQualifier->Clone();
    }
    Node* clonedInitializer = nullptr;
    if (initializer)
    {
        clonedInitializer = initializer->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    Node* clonedSemicolon = nullptr;
    if (semicolon)
    {
        clonedSemicolon = semicolon->Clone();
    }
    StructuredBindingNode* clone = new StructuredBindingNode(GetSpan(), FileIndex(), declSpecifiers->Clone(), clonedRefQualifier, identifiers->Clone(), 
        clonedInitializer, clonedAttributes, clonedSemicolon, lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void StructuredBindingNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StructuredBindingNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(declSpecifiers.get());
    writer.Write(refQualifier.get());
    writer.Write(identifiers.get());
    writer.Write(initializer.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
    writer.Write(lbSpan);
    writer.Write(rbSpan);
}

void StructuredBindingNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    declSpecifiers.reset(reader.ReadNode());
    refQualifier.reset(reader.ReadNode());
    identifiers.reset(reader.ReadNode());
    initializer.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
}

ForStatementNode::ForStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::forStatementNode, span_, fileIndex_), blockId(-1)
{
}

ForStatementNode::ForStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* initStmt_, Node* cond_, Node* loopExpr_, Node* stmt_, 
    Node* attributes_, Node* semicolon_, const soul::ast::Span& forSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::forStatementNode, span_, fileIndex_), initStmt(initStmt_), cond(cond_), loopExpr(loopExpr_), stmt(stmt_), attributes(attributes_), 
    semicolon(semicolon_), forSpan(forSpan_), lpSpan(lpSpan_), rpSpan(rpSpan_), blockId(-1)
{
}

Node* ForStatementNode::Clone() const
{
    Node* clonedCond = nullptr;
    if (cond)
    {
        clonedCond = cond->Clone();
    }
    Node* clonedLoopExpr = nullptr;
    if (loopExpr)
    {
        clonedLoopExpr = loopExpr->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    Node* clonedSemicolon = nullptr;
    if (semicolon)
    {
        clonedSemicolon = semicolon->Clone();
    }
    ForStatementNode* clone = new ForStatementNode(GetSpan(), FileIndex(), initStmt->Clone(), clonedCond, clonedLoopExpr, stmt->Clone(), clonedAttributes, 
        clonedSemicolon, forSpan, lpSpan, rpSpan);
    clone->SetBlockId(blockId);
    clone->SetId(Id());
    return clone;
}

void ForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ForStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(initStmt.get());
    writer.Write(cond.get());
    writer.Write(loopExpr.get());
    writer.Write(stmt.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
    writer.Write(forSpan);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
    writer.GetBinaryStreamWriter().Write(blockId);
}

void ForStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    initStmt.reset(reader.ReadNode());
    cond.reset(reader.ReadNode());
    loopExpr.reset(reader.ReadNode());
    stmt.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    forSpan = reader.ReadSpan();
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
    blockId = reader.GetBinaryStreamReader().ReadInt();
}

BreakStatementNode::BreakStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::breakStatementNode, span_, fileIndex_)
{
}

BreakStatementNode::BreakStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* semicolon_, const soul::ast::Span& breakSpan_) noexcept :
    CompoundNode(NodeKind::breakStatementNode, span_, fileIndex_), attributes(attributes_), semicolon(semicolon_), breakSpan(breakSpan_)
{
}

Node* BreakStatementNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    BreakStatementNode* clone = new BreakStatementNode(GetSpan(), FileIndex(), clonedAttributes, semicolon->Clone(), breakSpan);
    clone->SetId(Id());
    return clone;
}

void BreakStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BreakStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
    writer.Write(breakSpan);
}

void BreakStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    breakSpan = reader.ReadSpan();
}

ContinueStatementNode::ContinueStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::continueStatementNode, span_, fileIndex_)
{
}

ContinueStatementNode::ContinueStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* semicolon_,
    const soul::ast::Span& continueSpan_) noexcept :
    CompoundNode(NodeKind::continueStatementNode, span_, fileIndex_), attributes(attributes_), semicolon(semicolon_), continueSpan(continueSpan_)
{
}

Node* ContinueStatementNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    ContinueStatementNode* clone = new ContinueStatementNode(GetSpan(), FileIndex(), clonedAttributes, semicolon->Clone(), continueSpan);
    clone->SetId(Id());
    return clone;
}

void ContinueStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ContinueStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
    writer.Write(continueSpan);
}

void ContinueStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    continueSpan = reader.ReadSpan();
}

ReturnStatementNode::ReturnStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::returnStatementNode, span_, fileIndex_)
{
}

ReturnStatementNode::ReturnStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* returnValue_, Node* attributes_, Node* semicolon_,
    const soul::ast::Span& returnSpan_) noexcept :
    CompoundNode(NodeKind::returnStatementNode, span_, fileIndex_), returnValue(returnValue_), attributes(attributes_), semicolon(semicolon_), returnSpan(returnSpan_)
{
}

Node* ReturnStatementNode::Clone() const
{
    Node* clonedReturnValue = nullptr;
    if (returnValue)
    {
        clonedReturnValue = returnValue->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    Node* clonedSemicolon = nullptr;
    if (semicolon)
    {
        clonedSemicolon = semicolon->Clone();
    }
    ReturnStatementNode* clone = new ReturnStatementNode(GetSpan(), FileIndex(), clonedReturnValue, clonedAttributes, clonedSemicolon, returnSpan);
    clone->SetId(Id());
    return clone;
}

void ReturnStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ReturnStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(returnValue.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
    writer.Write(returnSpan);
}

void ReturnStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    returnValue.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    returnSpan = reader.ReadSpan();
}

CoReturnStatementNode::CoReturnStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::coReturnStatementNode, span_, fileIndex_)
{
}

CoReturnStatementNode::CoReturnStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* returnValue_, Node* attributes_, Node* semicolon_,
    const soul::ast::Span& coReturnSpan_) noexcept :
    CompoundNode(NodeKind::coReturnStatementNode, span_, fileIndex_), returnValue(returnValue_), attributes(attributes_), semicolon(semicolon_), coReturnSpan(coReturnSpan_)
{
}

Node* CoReturnStatementNode::Clone() const
{
    Node* clonedReturnValue = nullptr;
    if (returnValue)
    {
        clonedReturnValue = returnValue->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    CoReturnStatementNode* clone = new CoReturnStatementNode(GetSpan(), FileIndex(), clonedReturnValue, clonedAttributes, semicolon->Clone(), coReturnSpan);
    clone->SetId(Id());
    return clone;
}

void CoReturnStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CoReturnStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(returnValue.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
    writer.Write(coReturnSpan);
}

void CoReturnStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    returnValue.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    coReturnSpan = reader.ReadSpan();
}

GotoStatementNode::GotoStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::gotoStatementNode, span_, fileIndex_)
{
}

GotoStatementNode::GotoStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* target_, Node* attributes_, Node* semicolon_,
    const soul::ast::Span& gotoSpan_) noexcept :
    CompoundNode(NodeKind::gotoStatementNode, span_, fileIndex_), target(target_), attributes(attributes_), semicolon(semicolon_), gotoSpan(gotoSpan_)
{
}

Node* GotoStatementNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    Node* clonedSemicolon = nullptr;
    if (semicolon)
    {
        clonedSemicolon = semicolon->Clone();
    }
    GotoStatementNode* clone = new GotoStatementNode(GetSpan(), FileIndex(), target->Clone(), clonedAttributes, clonedSemicolon, gotoSpan);
    clone->SetId(Id());
    return clone;
}

void GotoStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void GotoStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(target.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
    writer.Write(gotoSpan);
}

void GotoStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    target.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    gotoSpan = reader.ReadSpan();
}

TryStatementNode::TryStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::tryStatementNode, span_, fileIndex_)
{
}

TryStatementNode::TryStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* tryBlock_, Node* handlers_, Node* attributes_,
    const soul::ast::Span& trySpan_) noexcept :
    CompoundNode(NodeKind::tryStatementNode, span_, fileIndex_), tryBlock(tryBlock_), handlers(handlers_), attributes(attributes_), trySpan(trySpan_)
{
}

Node* TryStatementNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    TryStatementNode* clone = new TryStatementNode(GetSpan(), FileIndex(), tryBlock->Clone(), handlers->Clone(), clonedAttributes, trySpan);
    clone->SetId(Id());
    return clone;
}

void TryStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TryStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(tryBlock.get());
    writer.Write(handlers.get());
    writer.Write(attributes.get());
    writer.Write(trySpan);
}

void TryStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    tryBlock.reset(reader.ReadNode());
    handlers.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    trySpan = reader.ReadSpan();
}

HandlerSequenceNode::HandlerSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept : SequenceNode(NodeKind::handlerSequenceNode, span_, fileIndex_)
{
}

Node* HandlerSequenceNode::Clone() const
{
    HandlerSequenceNode* clone = new HandlerSequenceNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void HandlerSequenceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

HandlerNode::HandlerNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::handlerNode, span_, fileIndex_)
{
}

HandlerNode::HandlerNode(const soul::ast::Span& span_, int fileIndex_, Node* exception_, Node* catchBlock_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::handlerNode, span_, fileIndex_), exception(exception_), catchBlock(catchBlock_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* HandlerNode::Clone() const
{
    HandlerNode* clone = new HandlerNode(GetSpan(), FileIndex(), exception->Clone(), catchBlock->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void HandlerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void HandlerNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(exception.get());
    writer.Write(catchBlock.get());
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void HandlerNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    exception.reset(reader.ReadNode());
    catchBlock.reset(reader.ReadNode());
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

ExceptionDeclarationNode::ExceptionDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::exceptionDeclarationNode, span_, fileIndex_)
{
}

ExceptionDeclarationNode::ExceptionDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* typeSpecifiers_, Node* declarator_, Node* ellipsis_,
    Node* attributes_) noexcept :
    CompoundNode(NodeKind::exceptionDeclarationNode, span_, fileIndex_), typeSpecifiers(typeSpecifiers_), declarator(declarator_), ellipsis(ellipsis_), 
    attributes(attributes_)
{
}

Node* ExceptionDeclarationNode::Clone() const
{
    Node* clonedTypeSpecifiers = nullptr;
    if (typeSpecifiers)
    {
        clonedTypeSpecifiers = typeSpecifiers->Clone();
    }
    Node* clonedDeclarator = nullptr;
    if (declarator)
    {
        clonedDeclarator = declarator->Clone();
    }
    Node* clonedEllipsis = nullptr;
    if (ellipsis)
    {
        clonedEllipsis = ellipsis->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    ExceptionDeclarationNode* clone = new ExceptionDeclarationNode(GetSpan(), FileIndex(), clonedTypeSpecifiers, clonedDeclarator, clonedEllipsis, clonedAttributes);
    clone->SetId(Id());
    return clone;
}

void ExceptionDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExceptionDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(typeSpecifiers.get());
    writer.Write(declarator.get());
    writer.Write(ellipsis.get());
    writer.Write(attributes.get());
}

void ExceptionDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    typeSpecifiers.reset(reader.ReadNode());
    declarator.reset(reader.ReadNode());
    ellipsis.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
}

ExpressionStatementNode::ExpressionStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::expressionStatementNode, span_, fileIndex_)
{
}

ExpressionStatementNode::ExpressionStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* expr_, Node* attributes_, Node* semicolon_) noexcept :
    CompoundNode(NodeKind::expressionStatementNode, span_, fileIndex_), expr(expr_), attributes(attributes_), semicolon(semicolon_)
{
}

Node* ExpressionStatementNode::Clone() const
{
    Node* clonedExpr = nullptr;
    if (expr)
    {
        clonedExpr = expr->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    Node* clonedSemicolon = nullptr;
    if (semicolon)
    {
        clonedSemicolon = semicolon->Clone();
    }
    ExpressionStatementNode* clone = new ExpressionStatementNode(GetSpan(), FileIndex(), clonedExpr, clonedAttributes, clonedSemicolon);
    clone->SetId(Id());
    return clone;
}

void ExpressionStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExpressionStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(expr.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
}

void ExpressionStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    expr.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

std::u32string ExpressionStatementNode::Str() const
{
    std::u32string str = expr->Str();
    str.append(1, ';');
    return str;
}

DeclarationStatementNode::DeclarationStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::declarationStatementNode, span_, fileIndex_)
{
}

DeclarationStatementNode::DeclarationStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* declaration_) noexcept :
    CompoundNode(NodeKind::declarationStatementNode, span_, fileIndex_), declaration(declaration_)
{
}

Node* DeclarationStatementNode::Clone() const
{
    DeclarationStatementNode* clone = new DeclarationStatementNode(GetSpan(), FileIndex(), declaration->Clone());
    clone->SetId(Id());
    return clone;
}

void DeclarationStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DeclarationStatementNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(declaration.get());
}

void DeclarationStatementNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    declaration.reset(reader.ReadNode());
}

std::u32string DeclarationStatementNode::Str() const
{
    std::u32string str = declaration->Str();
    return str;
}

InitConditionNode::InitConditionNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::initConditionNode, span_, fileIndex_)
{
}

InitConditionNode::InitConditionNode(const soul::ast::Span& span_, int fileIndex_, Node* declSpecifiers_, Node* declarator_, Node* initializer_, 
    Node* attributes_) noexcept :
    CompoundNode(NodeKind::initConditionNode, span_, fileIndex_), declSpecifiers(declSpecifiers_), declarator(declarator_), initializer(initializer_), 
    attributes(attributes_)
{
}

Node* InitConditionNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    InitConditionNode* clone = new InitConditionNode(GetSpan(), FileIndex(), declSpecifiers->Clone(), declarator->Clone(), initializer->Clone(), clonedAttributes);
    clone->SetId(Id());
    return clone;
}

void InitConditionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void InitConditionNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(declSpecifiers.get());
    writer.Write(declarator.get());
    writer.Write(initializer.get());
    writer.Write(attributes.get());
}

void InitConditionNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    declSpecifiers.reset(reader.ReadNode());
    declarator.reset(reader.ReadNode());
    initializer.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
}

BoundStatementNode::BoundStatementNode(void* boundStatementNode_, const soul::ast::Span& span_, int fileIndex_) noexcept :
    Node(NodeKind::boundStatementNode, span_, fileIndex_), boundStatementNode(boundStatementNode_)
{
}

Node* BoundStatementNode::Clone() const
{
    BoundStatementNode* clone = new BoundStatementNode(boundStatementNode, GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void BoundStatementNode::Accept(Visitor& visitor) 
{
    visitor.Visit(*this);
}

} // namespace otava::ast
