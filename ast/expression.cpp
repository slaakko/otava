// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.expression;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;
import otava.ast.type;

namespace otava::ast {

BinaryExprNode::BinaryExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : BinaryNode(NodeKind::binaryExprNode, span_, fileIndex_, nullptr, nullptr)
{
}

BinaryExprNode::BinaryExprNode(const soul::ast::Span& span_, int fileIndex_, Node* op_, Node* left_, Node* right_) noexcept :
    BinaryNode(NodeKind::binaryExprNode, span_, fileIndex_, left_, right_), op(op_)
{
}

Node* BinaryExprNode::Clone() const
{
    BinaryExprNode* clone = new BinaryExprNode(GetSpan(), FileIndex(), op->Clone(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void BinaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BinaryExprNode::Write(Writer& writer)
{
    BinaryNode::Write(writer);
    writer.Write(op.get());
}

void BinaryExprNode::Read(Reader& reader)
{
    BinaryNode::Read(reader);
    op.reset(reader.ReadNode());
}

std::u32string BinaryExprNode::Str() const
{
    std::u32string str = Left()->Str();
    str.append(1, ' ').append(op->Str()).append(1, ' ').append(Right()->Str());
    return str;
}

UnaryExprNode::UnaryExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::unaryExprNode, span_, fileIndex_, nullptr)
{
}

UnaryExprNode::UnaryExprNode(const soul::ast::Span& span_, int fileIndex_, Node* op_, Node* child_) noexcept :
    UnaryNode(NodeKind::unaryExprNode, span_, fileIndex_, child_), op(op_)
{
}

Node* UnaryExprNode::Clone() const
{
    UnaryExprNode* clone = new UnaryExprNode(GetSpan(), FileIndex(), op->Clone(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void UnaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UnaryExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(op.get());
}

void UnaryExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    op.reset(reader.ReadNode());
}

std::u32string UnaryExprNode::Str() const
{
    std::u32string str = op->Str();
    str.append(Child()->Str());
    return str;
}

ExpressionListNode::ExpressionListNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::expressionListNode, span_, fileIndex_)
{
}

Node* ExpressionListNode::Clone() const
{
    ExpressionListNode* clone = new ExpressionListNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void ExpressionListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExpressionListNode::Write(Writer& writer)
{
    ListNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void ExpressionListNode::Read(Reader& reader)
{
    ListNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

std::u32string ExpressionListNode::Str() const
{
    std::u32string str = U"(";
    str.append(ListNode::Str()).append(U")");
    return str;
}

AssignmentInitNode::AssignmentInitNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::assignmentInitializerNode, span_, fileIndex_, nullptr)
{
}

AssignmentInitNode::AssignmentInitNode(const soul::ast::Span& span_, int fileIndex_, Node* initializer_) noexcept :
    UnaryNode(NodeKind::assignmentInitializerNode, span_, fileIndex_, initializer_)
{
}

std::u32string AssignmentInitNode::Str() const
{
    std::u32string str = U" = ";
    str.append(Child()->Str());
    return str;
}

Node* AssignmentInitNode::Clone() const
{
    AssignmentInitNode* clone = new AssignmentInitNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void AssignmentInitNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

YieldExprNode::YieldExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::yieldExprNode, span_, fileIndex_, nullptr)
{
}

YieldExprNode::YieldExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept : 
    UnaryNode(NodeKind::yieldExprNode, span_, fileIndex_, child_)
{
}

Node* YieldExprNode::Clone() const
{
    YieldExprNode* clone = new YieldExprNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void YieldExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConditionalExprNode::ConditionalExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::conditionalExprNode, span_, fileIndex_)
{
}

ConditionalExprNode::ConditionalExprNode(const soul::ast::Span& span_, int fileIndex_, Node* condition_, Node* quest_, Node* thenExpr_, Node* colon_, 
    Node* elseExpr_) noexcept :
    CompoundNode(NodeKind::conditionalExprNode, span_, fileIndex_), condition(condition_), quest(quest_), thenExpr(thenExpr_), colon(colon_), elseExpr(elseExpr_)
{
}

Node* ConditionalExprNode::Clone() const
{
    ConditionalExprNode* clone = new ConditionalExprNode(GetSpan(), FileIndex(), condition->Clone(), quest->Clone(), thenExpr->Clone(), colon->Clone(), elseExpr->Clone());
    clone->SetId(Id());
    return clone;
}

void ConditionalExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConditionalExprNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(condition.get());
    writer.Write(quest.get());
    writer.Write(thenExpr.get());
    writer.Write(colon.get());
    writer.Write(elseExpr.get());
}

void ConditionalExprNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    condition.reset(reader.ReadNode());
    quest.reset(reader.ReadNode());
    thenExpr.reset(reader.ReadNode());
    colon.reset(reader.ReadNode());
    elseExpr.reset(reader.ReadNode());
}

std::u32string ConditionalExprNode::Str() const
{
    std::u32string str = condition->Str();
    str.append(1, ' ').append(quest->Str()).append(1, ' ').append(thenExpr->Str()).
        append(1, ' ').append(colon->Str()).append(1, ' ').append(elseExpr->Str());
    return str;
}

AssignNode::AssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::assignNode, span_, fileIndex_)
{
}

Node* AssignNode::Clone() const
{
    AssignNode* clone = new AssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void AssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PlusAssignNode::PlusAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::plusAssignNode, span_, fileIndex_)
{
}

Node* PlusAssignNode::Clone() const
{
    PlusAssignNode* clone = new PlusAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void PlusAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MinusAssignNode::MinusAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::minusAssignNode, span_, fileIndex_)
{
}

Node* MinusAssignNode::Clone() const
{
    MinusAssignNode* clone = new MinusAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void MinusAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MulAssignNode::MulAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::mulAssignNode, span_, fileIndex_)
{
}

Node* MulAssignNode::Clone() const
{
    MulAssignNode* clone = new MulAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void MulAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DivAssignNode::DivAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::divAssignNode, span_, fileIndex_)
{
}

Node* DivAssignNode::Clone() const
{
    DivAssignNode* clone = new DivAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DivAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ModAssignNode::ModAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::modAssignNode, span_, fileIndex_)
{
}

Node* ModAssignNode::Clone() const
{
    ModAssignNode* clone = new ModAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ModAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

XorAssignNode::XorAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::xorAssignNode, span_, fileIndex_)
{
}

Node* XorAssignNode::Clone() const
{
    XorAssignNode* clone = new XorAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void XorAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AndAssignNode::AndAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::andAssignNode, span_, fileIndex_)
{
}

Node* AndAssignNode::Clone() const
{
    AndAssignNode* clone = new AndAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void AndAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OrAssignNode::OrAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::orAssignNode, span_, fileIndex_)
{
}

Node* OrAssignNode::Clone() const
{
    OrAssignNode* clone = new OrAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void OrAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShiftLeftAssignNode::ShiftLeftAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::shiftLeftAssignNode, span_, fileIndex_)
{
}

Node* ShiftLeftAssignNode::Clone() const
{
    ShiftLeftAssignNode* clone = new ShiftLeftAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ShiftLeftAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShiftRightAssignNode::ShiftRightAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::shiftRightAssignNode, span_, fileIndex_)
{
}

Node* ShiftRightAssignNode::Clone() const
{
    ShiftRightAssignNode* clone = new ShiftRightAssignNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ShiftRightAssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DisjunctionNode::DisjunctionNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::disjunctionNode, span_, fileIndex_)
{
}

Node* DisjunctionNode::Clone() const
{
    DisjunctionNode* clone = new DisjunctionNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DisjunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConjunctionNode::ConjunctionNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::conjunctionNode, span_, fileIndex_)
{
}

Node* ConjunctionNode::Clone() const
{
    ConjunctionNode* clone = new ConjunctionNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ConjunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InclusiveOrNode::InclusiveOrNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::inclusiveOrNode, span_, fileIndex_)
{
}

Node* InclusiveOrNode::Clone() const
{
    InclusiveOrNode* clone = new InclusiveOrNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void InclusiveOrNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExclusiveOrNode::ExclusiveOrNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::exclusiveOrNode, span_, fileIndex_)
{
}

Node* ExclusiveOrNode::Clone() const
{
    ExclusiveOrNode* clone = new ExclusiveOrNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ExclusiveOrNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AndNode::AndNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::andNode, span_, fileIndex_)
{
}

Node* AndNode::Clone() const
{
    AndNode* clone = new AndNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void AndNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EqualNode::EqualNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::equalNode, span_, fileIndex_)
{
}

Node* EqualNode::Clone() const
{
    EqualNode* clone = new EqualNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void EqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NotEqualNode::NotEqualNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::notEqualNode, span_, fileIndex_)
{
}

Node* NotEqualNode::Clone() const
{
    NotEqualNode* clone = new NotEqualNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void NotEqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LessNode::LessNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::lessNode, span_, fileIndex_)
{
}

Node* LessNode::Clone() const
{
    LessNode* clone = new LessNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void LessNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GreaterNode::GreaterNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::greaterNode, span_, fileIndex_)
{
}

Node* GreaterNode::Clone() const
{
    GreaterNode* clone = new GreaterNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void GreaterNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LessOrEqualNode::LessOrEqualNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::lessOrEqualNode, span_, fileIndex_)
{
}

Node* LessOrEqualNode::Clone() const
{
    LessOrEqualNode* clone = new LessOrEqualNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void LessOrEqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GreaterOrEqualNode::GreaterOrEqualNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::greaterOrEqualNode, span_, fileIndex_)
{
}

Node* GreaterOrEqualNode::Clone() const
{
    GreaterOrEqualNode* clone = new GreaterOrEqualNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void GreaterOrEqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CompareNode::CompareNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::compareNode, span_, fileIndex_)
{
}

Node* CompareNode::Clone() const
{
    CompareNode* clone = new CompareNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void CompareNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShiftLeftNode::ShiftLeftNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::shiftLeftNode, span_, fileIndex_)
{
}

Node* ShiftLeftNode::Clone() const
{
    ShiftLeftNode* clone = new ShiftLeftNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ShiftLeftNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShiftRightNode::ShiftRightNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::shiftRightNode, span_, fileIndex_)
{
}

Node* ShiftRightNode::Clone() const
{
    ShiftRightNode* clone = new ShiftRightNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ShiftRightNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PlusNode::PlusNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::plusNode, span_, fileIndex_)
{
}

Node* PlusNode::Clone() const
{
    PlusNode* clone = new PlusNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void PlusNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MinusNode::MinusNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::minusNode, span_, fileIndex_)
{
}

Node* MinusNode::Clone() const
{
    MinusNode* clone = new MinusNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void MinusNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MulNode::MulNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::mulNode, span_, fileIndex_)
{
}

Node* MulNode::Clone() const
{
    MulNode* clone = new MulNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void MulNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DivNode::DivNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::divNode, span_, fileIndex_)
{
}

Node* DivNode::Clone() const
{
    DivNode* clone = new DivNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DivNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ModNode::ModNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::modNode, span_, fileIndex_)
{
}

Node* ModNode::Clone() const
{
    ModNode* clone = new ModNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ModNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DotStarNode::DotStarNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::dotStarNode, span_, fileIndex_)
{
}

Node* DotStarNode::Clone() const
{
    DotStarNode* clone = new DotStarNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DotStarNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrowStarNode::ArrowStarNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::arrowStarNode, span_, fileIndex_)
{
}

Node* ArrowStarNode::Clone() const
{
    ArrowStarNode* clone = new ArrowStarNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ArrowStarNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CastExprNode::CastExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::castExprNode, span_, fileIndex_, nullptr)
{
}

CastExprNode::CastExprNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_, Node* child_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::castExprNode, span_, fileIndex_, child_), typeId(typeId_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* CastExprNode::Clone() const
{
    CastExprNode* clone = new CastExprNode(GetSpan(), FileIndex(), typeId->Clone(), Child()->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void CastExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CastExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(typeId.get());
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void CastExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    typeId.reset(reader.ReadNode());
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

std::u32string CastExprNode::Str() const
{
    std::u32string str = U"(";
    str.append(typeId->Str()).append(U")").append(Child()->Str());
    return str;
}

DerefNode::DerefNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::derefNode, span_, fileIndex_)
{
}

Node* DerefNode::Clone() const
{
    DerefNode* clone = new DerefNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DerefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AddrOfNode::AddrOfNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::addrOfNode, span_, fileIndex_)
{
}

Node* AddrOfNode::Clone() const
{
    AddrOfNode* clone = new AddrOfNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void AddrOfNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NotNode::NotNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::notNode, span_, fileIndex_)
{
}

Node* NotNode::Clone() const
{
    NotNode* clone = new NotNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void NotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ComplementNode::ComplementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::complementNode, span_, fileIndex_)
{
}

Node* ComplementNode::Clone() const
{
    ComplementNode* clone = new ComplementNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ComplementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PrefixIncNode::PrefixIncNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::prefixIncNode, span_, fileIndex_)
{
}

Node* PrefixIncNode::Clone() const
{
    PrefixIncNode* clone = new PrefixIncNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void PrefixIncNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PrefixDecNode::PrefixDecNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::prefixDecNode, span_, fileIndex_)
{
}

Node* PrefixDecNode::Clone() const
{
    PrefixDecNode* clone = new PrefixDecNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void PrefixDecNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AwaitExprNode::AwaitExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::awaitExprNode, span_, fileIndex_, nullptr)
{
}

AwaitExprNode::AwaitExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept : UnaryNode(NodeKind::awaitExprNode, span_, fileIndex_, child_)
{
}

Node* AwaitExprNode::Clone() const
{
    AwaitExprNode* clone = new AwaitExprNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void AwaitExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SizeOfTypeExprNode::SizeOfTypeExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::sizeOfTypeExprNode, span_, fileIndex_, nullptr)
{
}

SizeOfTypeExprNode::SizeOfTypeExprNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::sizeOfTypeExprNode, span_, fileIndex_, typeId_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* SizeOfTypeExprNode::Clone() const
{
    SizeOfTypeExprNode* clone = new SizeOfTypeExprNode(GetSpan(), FileIndex(), Child()->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void SizeOfTypeExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SizeOfTypeExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void SizeOfTypeExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

std::u32string SizeOfTypeExprNode::Str() const
{
    std::u32string str = U"sizeof(";
    str.append(Child()->Str()).append(1, ')');
    return str;
}

SizeOfPackExprNode::SizeOfPackExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::sizeOfPackExpNode, span_, fileIndex_, nullptr)
{
}

SizeOfPackExprNode::SizeOfPackExprNode(const soul::ast::Span& span_, int fileIndex_, Node* idNode_, const soul::ast::Span& ellipsisSpan_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::sizeOfPackExpNode, span_, fileIndex_, idNode_), ellipsisSpan(ellipsisSpan_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* SizeOfPackExprNode::Clone() const
{
    SizeOfPackExprNode* clone = new SizeOfPackExprNode(GetSpan(), FileIndex(), Child()->Clone(), ellipsisSpan, lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void SizeOfPackExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SizeOfPackExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(ellipsisSpan);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void SizeOfPackExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    ellipsisSpan = reader.ReadSpan();
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

SizeOfUnaryExprNode::SizeOfUnaryExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::sizeOfUnaryExprNode, span_, fileIndex_, nullptr)
{
}

SizeOfUnaryExprNode::SizeOfUnaryExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept :
    UnaryNode(NodeKind::sizeOfUnaryExprNode, span_, fileIndex_, child_)
{
}

Node* SizeOfUnaryExprNode::Clone() const
{
    SizeOfUnaryExprNode* clone = new SizeOfUnaryExprNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void SizeOfUnaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::u32string SizeOfUnaryExprNode::Str() const
{
    std::u32string str = U"sizeof(";
    str.append(Child()->Str()).append(1, ')');
    return str;
}

AlignOfExprNode::AlignOfExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::alignOfExprNode, span_, fileIndex_, nullptr)
{
}

AlignOfExprNode::AlignOfExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::alignOfExprNode, span_, fileIndex_, child_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* AlignOfExprNode::Clone() const
{
    AlignOfExprNode* clone = new AlignOfExprNode(GetSpan(), FileIndex(), Child()->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void AlignOfExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AlignOfExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void AlignOfExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

NoexceptExprNode::NoexceptExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    UnaryNode(NodeKind::noexceptExprNode, span_, fileIndex_, nullptr)
{
}

NoexceptExprNode::NoexceptExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::noexceptExprNode, span_, fileIndex_, child_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* NoexceptExprNode::Clone() const
{
    NoexceptExprNode* clone = new NoexceptExprNode(GetSpan(), FileIndex(), Child()->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void NoexceptExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NoexceptExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void NoexceptExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

OpNewCall::OpNewCall(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::opNewCall, span_, fileIndex_, nullptr)
{
}

OpNewCall::OpNewCall(const soul::ast::Span& span_, int fileIndex_, Node* sizeArg_) noexcept : 
    UnaryNode(NodeKind::opNewCall, span_, fileIndex_, sizeArg_)
{
}

Node* OpNewCall::Clone() const
{
    OpNewCall* clone = new OpNewCall(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void OpNewCall::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::u32string OpNewCall::Str() const
{
    std::u32string str = U"operator new(";
    str.append(Child()->Str()).append(1, ')');
    return str;
}

NewExprNode::NewExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::newExprNode, span_, fileIndex_, nullptr)
{
}

NewExprNode::NewExprNode(const soul::ast::Span& span_, int fileIndex_, Node* placement_, Node* child_, Node* initializer_, Node* colonColonHead_,
    const soul::ast::Span& newSpan_) noexcept :
    UnaryNode(NodeKind::newExprNode, span_, fileIndex_, child_), placement(placement_), initializer(initializer_), colonColonHead(colonColonHead_), newSpan(newSpan_)
{
}

Node* NewExprNode::Clone() const
{
    Node* clonedPlacement = nullptr;
    if (placement)
    {
        clonedPlacement = placement->Clone();
    }
    Node* clonedInitializer = nullptr;
    if (initializer)
    {
        clonedInitializer = initializer->Clone();
    }
    Node* clonedColonColonHead = nullptr;
    if (colonColonHead)
    {
        clonedColonColonHead = colonColonHead->Clone();
    }
    NewExprNode* clone = new NewExprNode(GetSpan(), FileIndex(), clonedPlacement, Child()->Clone(), clonedInitializer, clonedColonColonHead, newSpan);
    clone->SetId(Id());
    return clone;
}

void NewExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NewExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(colonColonHead.get());
    writer.Write(placement.get());
    writer.Write(initializer.get());
    writer.Write(newSpan);
}

void NewExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    colonColonHead.reset(reader.ReadNode());
    placement.reset(reader.ReadNode());
    initializer.reset(reader.ReadNode());
    newSpan = reader.ReadSpan();
}

std::u32string NewExprNode::Str() const
{
    std::u32string str;
    if (colonColonHead)
    {
        str.append(colonColonHead->Str());
    }
    str.append(U"new ").append(Child()->Str());
    if (placement)
    {
        str.append(1, '(').append(placement->Str()).append(1, ')');
    }
    if (initializer)
    {
        if (initializer->IsNewInitializerNode())
        {
            str.append(1, '(').append(initializer->Str()).append(1, ')');
        }
        else
        {
            str.append(initializer->Str());
        }
    }
    return str;
}

NewPlacementNode::NewPlacementNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::newPlacementNode, span_, fileIndex_)
{
}

Node* NewPlacementNode::Clone() const
{
    NewPlacementNode* clone = new NewPlacementNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetLParenSpan(lpSpan);
    clone->SetRParenSpan(rpSpan);
    clone->SetId(Id());
    return clone;
}

void NewPlacementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NewPlacementNode::Write(Writer& writer)
{
    ListNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void NewPlacementNode::Read(Reader& reader)
{
    ListNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

ParenNewTypeIdNode::ParenNewTypeIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::parenNewTypeIdNode, span_, fileIndex_, nullptr)
{
}

ParenNewTypeIdNode::ParenNewTypeIdNode(const soul::ast::Span& span_, int fileIndex_, Node* child_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::parenNewTypeIdNode, span_, fileIndex_, child_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* ParenNewTypeIdNode::Clone() const
{
    ParenNewTypeIdNode* clone = new ParenNewTypeIdNode(GetSpan(), FileIndex(), Child()->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void ParenNewTypeIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParenNewTypeIdNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void ParenNewTypeIdNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

std::u32string ParenNewTypeIdNode::Str() const
{
    std::u32string str(1, '(');
    str.append(Child()->Str()).append(1, ')');
    return str;
}

NewTypeIdNode::NewTypeIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::newTypeIdNode, span_, fileIndex_)
{
}

NewTypeIdNode::NewTypeIdNode(const soul::ast::Span& span_, int fileIndex_, Node* typeSpecifierSeq_, Node* newDeclarator_) noexcept :
    CompoundNode(NodeKind::newTypeIdNode, span_, fileIndex_), typeSpecifierSeq(typeSpecifierSeq_), newDeclarator(newDeclarator_)
{
}

Node* NewTypeIdNode::Clone() const
{
    Node* clonedNewDeclarator = nullptr;
    if (newDeclarator)
    {
        clonedNewDeclarator = newDeclarator->Clone();
    }
    NewTypeIdNode* clone = new NewTypeIdNode(GetSpan(), FileIndex(), typeSpecifierSeq->Clone(), clonedNewDeclarator);
    clone->SetId(Id());
    return clone;
}

void NewTypeIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NewTypeIdNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(typeSpecifierSeq.get());
    writer.Write(newDeclarator.get());
}

void NewTypeIdNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    typeSpecifierSeq.reset(reader.ReadNode());
    newDeclarator.reset(reader.ReadNode());
}

std::u32string NewTypeIdNode::Str() const
{
    std::u32string str;
    if (typeSpecifierSeq->IsTypeSpecifierSequenceNode())
    {
        TypeSpecifierSequenceNode* typeSpecifiers = static_cast<TypeSpecifierSequenceNode*>(typeSpecifierSeq.get());
        bool first = true;
        for (const auto& typeSpecifier : typeSpecifiers->Nodes())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                str.append(1, ' ');
            }
            str.append(typeSpecifier->Str());
        }
        if (newDeclarator)
        {
            str.append(newDeclarator->Str());
        }
    }
    return str;
}

ArrayDeletePtrNode::ArrayDeletePtrNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::arrayDeletePtrNode, span_, fileIndex_, nullptr)
{
}

ArrayDeletePtrNode::ArrayDeletePtrNode(const soul::ast::Span& span_, int fileIndex_, Node* ptr_, Node* colonColonHead_, const soul::ast::Span& deleteSpan_,
    const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    UnaryNode(NodeKind::arrayDeletePtrNode, span_, fileIndex_, ptr_), colonColonHead(colonColonHead_), deleteSpan(deleteSpan_), lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* ArrayDeletePtrNode::Clone() const
{
    Node* clonedColonColonHead = nullptr;
    if (colonColonHead)
    {
        clonedColonColonHead = colonColonHead->Clone();
    }
    ArrayDeletePtrNode* clone = new ArrayDeletePtrNode(GetSpan(), FileIndex(), Child()->Clone(), clonedColonColonHead, deleteSpan, lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void ArrayDeletePtrNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArrayDeletePtrNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(colonColonHead.get());
    writer.Write(deleteSpan);
    writer.Write(lbSpan);
    writer.Write(rbSpan);
}

void ArrayDeletePtrNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    colonColonHead.reset(reader.ReadNode());
    deleteSpan = reader.ReadSpan();
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
}

DeletePtrNode::DeletePtrNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::deletePtrNode, span_, fileIndex_, nullptr)
{
}

DeletePtrNode::DeletePtrNode(const soul::ast::Span& span_, int fileIndex_, Node* ptr_, Node* colonColonHead_, const soul::ast::Span& deleteSpan_) noexcept :
    UnaryNode(NodeKind::deletePtrNode, span_, fileIndex_, ptr_), colonColonHead(colonColonHead_), deleteSpan(deleteSpan_)
{
}

Node* DeletePtrNode::Clone() const
{
    Node* clonedColonColonHead = nullptr;
    if (colonColonHead)
    {
        clonedColonColonHead = colonColonHead->Clone();
    }
    DeletePtrNode* clone = new DeletePtrNode(GetSpan(), FileIndex(), Child()->Clone(), clonedColonColonHead, deleteSpan);
    clone->SetId(Id());
    return clone;
}

void DeletePtrNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DeletePtrNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(colonColonHead.get());
    writer.Write(deleteSpan);
}

void DeletePtrNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    colonColonHead.reset(reader.ReadNode());
    deleteSpan = reader.ReadSpan();
}

SubscriptExprNode::SubscriptExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::subscriptExprNode, span_, fileIndex_, nullptr)
{
}

SubscriptExprNode::SubscriptExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, Node* index_,
    const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    UnaryNode(NodeKind::subscriptExprNode, span_, fileIndex_, child_), index(index_), lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* SubscriptExprNode::Clone() const
{
    SubscriptExprNode* clone = new SubscriptExprNode(GetSpan(), FileIndex(), Child()->Clone(), index->Clone(), lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void SubscriptExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SubscriptExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(index.get());
    writer.Write(lbSpan);
    writer.Write(rbSpan);
}

void SubscriptExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    index.reset(reader.ReadNode());
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
}

std::u32string SubscriptExprNode::Str() const
{
    std::u32string str = Child()->Str();
    str.append(1, '[').append(index->Str()).append(1, ']');
    return str;
}

InvokeExprNode::InvokeExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::invokeExprNode, span_, fileIndex_), compileUnitInitFn(false)
{
}

InvokeExprNode::InvokeExprNode(const soul::ast::Span& span_, int fileIndex_, Node* subject_) noexcept :
    ListNode(NodeKind::invokeExprNode, span_, fileIndex_), subject(subject_), compileUnitInitFn(false)
{
}

Node* InvokeExprNode::Clone() const
{
    Node* clonedSubject = nullptr;
    if (subject)
    {
        clonedSubject = subject->Clone();
    }
    InvokeExprNode* clone = new InvokeExprNode(GetSpan(), FileIndex(), clonedSubject);
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetLParenSpan(lpSpan);
    clone->SetRParenSpan(rpSpan);
    clone->SetId(Id());
    return clone;
}

void InvokeExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void InvokeExprNode::Write(Writer& writer)
{
    ListNode::Write(writer);
    writer.Write(subject.get());
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void InvokeExprNode::Read(Reader& reader)
{
    ListNode::Read(reader);
    subject.reset(reader.ReadNode());
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

std::u32string InvokeExprNode::Str() const
{
    std::u32string str = subject->Str();
    str.append(U"(");
    bool first = true;
    for (const auto* item : Items())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(U", ");
        }
        str.append(item->Str());
    }
    str.append(U")");
    return str;
}

PairNode::PairNode(const soul::ast::Span& span_, int fileIndex_) noexcept : BinaryNode(NodeKind::pairNode, span_, fileIndex_, nullptr, nullptr)
{
}

PairNode::PairNode(const soul::ast::Span& span_, int fileIndex_, Node* left_, Node* right_) noexcept : 
    BinaryNode(NodeKind::pairNode, span_, fileIndex_, left_, right_)
{
}

Node* PairNode::Clone() const
{
    PairNode* clone = new PairNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void PairNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DotNode::DotNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::dotNode, span_, fileIndex_)
{
}

Node* DotNode::Clone() const
{
    DotNode* clone = new DotNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrowNode::ArrowNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::arrowNode, span_, fileIndex_)
{
}

Node* ArrowNode::Clone() const
{
    ArrowNode* clone = new ArrowNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ArrowNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MemberExprNode::MemberExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::memberExprNode, span_, fileIndex_, nullptr)
{
}

MemberExprNode::MemberExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, Node* op_, Node* id_) noexcept :
    UnaryNode(NodeKind::memberExprNode, span_, fileIndex_, child_), op(op_), id(id_)
{
}

Node* MemberExprNode::Clone() const
{
    MemberExprNode* clone = new MemberExprNode(GetSpan(), FileIndex(), Child()->Clone(), op->Clone(), id->Clone());
    clone->SetId(Id());
    return clone;
}

void MemberExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void MemberExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(op.get());
    writer.Write(id.get());
}

void MemberExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    op.reset(reader.ReadNode());
    id.reset(reader.ReadNode());
}

std::u32string MemberExprNode::Str() const
{
    std::u32string str = Child()->Str();
    str.append(op->Str()).append(GetId()->Str());
    return str;
}

PostfixIncExprNode::PostfixIncExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::postfixIncExprNode, span_, fileIndex_, nullptr)
{
}

PostfixIncExprNode::PostfixIncExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& opSpan_) noexcept :
    UnaryNode(NodeKind::postfixIncExprNode, span_, fileIndex_, child_), opSpan(opSpan_)
{
}

Node* PostfixIncExprNode::Clone() const
{
    PostfixIncExprNode* clone = new PostfixIncExprNode(GetSpan(), FileIndex(), Child()->Clone(), opSpan);
    clone->SetId(Id());
    return clone;
}

void PostfixIncExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PostfixIncExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(opSpan);
}

void PostfixIncExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    opSpan = reader.ReadSpan();
}

std::u32string PostfixIncExprNode::Str() const
{
    std::u32string str = Child()->Str();
    str.append(U"++");
    return str;
}

PostfixDecExprNode::PostfixDecExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::postfixDecExprNode, span_, fileIndex_, nullptr)
{
}

PostfixDecExprNode::PostfixDecExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& opSpan_) noexcept :
    UnaryNode(NodeKind::postfixDecExprNode, span_, fileIndex_, child_), opSpan(opSpan_)
{
}

Node* PostfixDecExprNode::Clone() const
{
    PostfixDecExprNode* clone = new PostfixDecExprNode(GetSpan(), FileIndex(), Child()->Clone(), opSpan);
    clone->SetId(Id());
    return clone;
}

void PostfixDecExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PostfixDecExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(opSpan);
}

void PostfixDecExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    opSpan = reader.ReadSpan();
}

std::u32string PostfixDecExprNode::Str() const
{
    std::u32string str = Child()->Str();
    str.append(U"--");
    return str;
}

TypeIdExprNode::TypeIdExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::typeIdExprNode, span_, fileIndex_, nullptr)
{
}

TypeIdExprNode::TypeIdExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::typeIdExprNode, span_, fileIndex_, child_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* TypeIdExprNode::Clone() const
{
    TypeIdExprNode* clone = new TypeIdExprNode(GetSpan(), FileIndex(), Child()->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void TypeIdExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeIdExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void TypeIdExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

std::u32string TypeIdExprNode::Str() const
{
    std::u32string str = U"typeid";
    str.append(1, '(').append(Child()->Str()).append(1, ')');
    return str;
}

DynamicCastNode::DynamicCastNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::dynamicCastNode, span_, fileIndex_)
{
}

Node* DynamicCastNode::Clone() const
{
    DynamicCastNode* clone = new DynamicCastNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DynamicCastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StaticCastNode::StaticCastNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::staticCastNode, span_, fileIndex_)
{
}

Node* StaticCastNode::Clone() const
{
    StaticCastNode* clone = new StaticCastNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void StaticCastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ReinterpretCastNode::ReinterpretCastNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::reinterpretCastNode, span_, fileIndex_)
{
}

Node* ReinterpretCastNode::Clone() const
{
    ReinterpretCastNode* clone = new ReinterpretCastNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ReinterpretCastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstCastNode::ConstCastNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::constCastNode, span_, fileIndex_)
{
}

Node* ConstCastNode::Clone() const
{
    ConstCastNode* clone = new ConstCastNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ConstCastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CppCastExprNode::CppCastExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::cppCastExprNode, span_, fileIndex_, nullptr)
{
}

CppCastExprNode::CppCastExprNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_, Node* child_, Node* op_,
    const soul::ast::Span& laSpan_, const soul::ast::Span& raSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::cppCastExprNode, span_, fileIndex_, child_), typeId(typeId_), op(op_), laSpan(laSpan_), raSpan(raSpan_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* CppCastExprNode::Clone() const
{
    CppCastExprNode* clone = new CppCastExprNode(GetSpan(), FileIndex(), typeId->Clone(), Child()->Clone(), op->Clone(), laSpan, raSpan, lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void CppCastExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CppCastExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(typeId.get());
    writer.Write(op.get());
    writer.Write(laSpan);
    writer.Write(raSpan);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void CppCastExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    typeId.reset(reader.ReadNode());
    op.reset(reader.ReadNode());
    laSpan = reader.ReadSpan();
    raSpan = reader.ReadSpan();
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

std::u32string CppCastExprNode::Str() const
{
    std::u32string str = op->Str();
    str.append(1, '<').append(typeId->Str()).append(U">(").append(Child()->Str()).append(1, ')');
    return str;
}

ThisNode::ThisNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::thisNode, span_, fileIndex_)
{
}

Node* ThisNode::Clone() const
{
    ThisNode* clone = new ThisNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ThisNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ParenthesizedExprNode::ParenthesizedExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::parenExprNode, span_, fileIndex_, nullptr)
{
}

ParenthesizedExprNode::ParenthesizedExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::parenExprNode, span_, fileIndex_, child_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* ParenthesizedExprNode::Clone() const
{
    ParenthesizedExprNode* clone = new ParenthesizedExprNode(GetSpan(), FileIndex(), Child()->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void ParenthesizedExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParenthesizedExprNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void ParenthesizedExprNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

std::u32string ParenthesizedExprNode::Str() const
{
    std::u32string str(1, '(');
    str.append(Child()->Str()).append(1, ')');
    return str;
}

FoldExprNode::FoldExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : SequenceNode(NodeKind::foldExprNode, span_, fileIndex_)
{
}

Node* FoldExprNode::Clone() const
{
    FoldExprNode* clone = new FoldExprNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetLParenSpan(lpSpan);
    clone->SetRParenSpan(rpSpan);
    clone->SetId(Id());
    return clone;
}

void FoldExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FoldExprNode::Write(Writer& writer)
{
    SequenceNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void FoldExprNode::Read(Reader& reader)
{
    SequenceNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

NewDeclaratorNode::NewDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept : SequenceNode(NodeKind::newDeclaratorNode, span_, fileIndex_)
{
}

Node* NewDeclaratorNode::Clone() const
{
    NewDeclaratorNode* clone = new NewDeclaratorNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void NewDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrayNewDeclaratorNode::ArrayNewDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept : SequenceNode(NodeKind::arrayNewDeclaratorNode, span_, fileIndex_)
{
}

Node* ArrayNewDeclaratorNode::Clone() const
{
    ArrayNewDeclaratorNode* clone = new ArrayNewDeclaratorNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void ArrayNewDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NewInitializerNode::NewInitializerNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::newInitializerNode, span_, fileIndex_)
{
}

Node* NewInitializerNode::Clone() const
{
    NewInitializerNode* clone = new NewInitializerNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void NewInitializerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NewInitializerNode::Write(Writer& writer)
{
    ListNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void NewInitializerNode::Read(Reader& reader)
{
    ListNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

BracedInitListNode::BracedInitListNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::bracedInitListNode, span_, fileIndex_)
{
}

Node* BracedInitListNode::Clone() const
{
    BracedInitListNode* clone = new BracedInitListNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void BracedInitListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DesignatedInitializerNode::DesignatedInitializerNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::designatedInitializerNode, span_, fileIndex_, nullptr, nullptr)
{
}

DesignatedInitializerNode::DesignatedInitializerNode(const soul::ast::Span& span_, int fileIndex_, Node* designator_, Node* initializer_) noexcept :
    BinaryNode(NodeKind::designatedInitializerNode, span_, fileIndex_, designator_, initializer_)
{
}

Node* DesignatedInitializerNode::Clone() const
{
    DesignatedInitializerNode* clone = new DesignatedInitializerNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void DesignatedInitializerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DesignatorNode::DesignatorNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::designatorNode, span_, fileIndex_, nullptr)
{
}

DesignatorNode::DesignatorNode(const soul::ast::Span& span_, int fileIndex_, Node* identifier_) noexcept : 
    UnaryNode(NodeKind::designatorNode, span_, fileIndex_, identifier_)
{
}

Node* DesignatorNode::Clone() const 
{
    DesignatorNode* clone = new DesignatorNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void DesignatorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ThrowExprNode::ThrowExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::throwExprNode, span_, fileIndex_, nullptr)
{
}

ThrowExprNode::ThrowExprNode(const soul::ast::Span& span_, int fileIndex_, Node* expr_) noexcept : UnaryNode(NodeKind::throwExprNode, span_, fileIndex_, expr_)
{
}

Node* ThrowExprNode::Clone() const
{
    Node* clonedChild = nullptr;
    if (Child())
    {
        clonedChild = Child()->Clone();
    }
    ThrowExprNode* clone = new ThrowExprNode(GetSpan(), FileIndex(), clonedChild);
    clone->SetId(Id());
    return clone;
}

void ThrowExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::u32string ThrowExprNode::Str() const
{
    std::u32string str = U"throw ";
    str.append(Child()->Str());
    return str;
}

} // namespace otava::ast
