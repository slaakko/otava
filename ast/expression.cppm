// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.expression;

import std;
import otava.ast.node;

export namespace otava::ast {

class BinaryExprNode : public BinaryNode
{
public:
    BinaryExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    BinaryExprNode(const soul::ast::Span& span_, int fileIndex_, Node* op_, Node* left_, Node* right_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Op() const noexcept { return op.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> op;
};

class UnaryExprNode : public UnaryNode
{
public:
    UnaryExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    UnaryExprNode(const soul::ast::Span& span_, int fileIndex_, Node* op_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Op() const noexcept { return op.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> op;
};

class ExpressionListNode : public ListNode
{
public:
    ExpressionListNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline void SetLParenSpan(const soul::ast::Span& lpSpan_) noexcept { lpSpan = lpSpan_; }
    inline void SetRParenSpan(const soul::ast::Span& rpSpan_) noexcept { rpSpan = rpSpan_; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    std::string Str() const override;
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class AssignmentInitNode : public UnaryNode
{
public:
    AssignmentInitNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AssignmentInitNode(const soul::ast::Span& span_, int fileIndex_, Node* initializer_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override;
};

class YieldExprNode : public UnaryNode
{
public:
    YieldExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    YieldExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ConditionalExprNode : public CompoundNode
{
public:
    ConditionalExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ConditionalExprNode(const soul::ast::Span& span_, int fileIndex_, Node* condition_, Node* quest_, Node* thenExpr_, Node* colon_, Node* elseExpr_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Condition() const noexcept { return condition.get(); }
    inline Node* Quest() const noexcept { return quest.get(); }
    inline Node* ThenExpr() const noexcept { return thenExpr.get(); }
    inline Node* Colon() const noexcept { return colon.get(); }
    inline Node* ElseExpr() const noexcept { return elseExpr.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> quest;
    std::unique_ptr<Node> thenExpr;
    std::unique_ptr<Node> colon;
    std::unique_ptr<Node> elseExpr;
};

class AssignNode : public Node
{
public:
    AssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "="; };
};

class PlusAssignNode : public Node
{
public:
    PlusAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "+="; };
};

class MinusAssignNode : public Node
{
public:
    MinusAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "-="; };
};

class MulAssignNode : public Node
{
public:
    MulAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "*="; };
};

class DivAssignNode : public Node
{
public:
    DivAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "/="; };
};

class ModAssignNode : public Node
{
public:
    ModAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "%="; };
};

class XorAssignNode : public Node
{
public:
    XorAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "^="; };
};

class AndAssignNode : public Node
{
public:
    AndAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "&="; };
};

class OrAssignNode : public Node
{
public:
    OrAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "|="; };
};

class ShiftLeftAssignNode : public Node
{
public:
    ShiftLeftAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "<<="; };
};

class ShiftRightAssignNode : public Node
{
public:
    ShiftRightAssignNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ">>="; };
};

class DisjunctionNode : public Node
{
public:
    DisjunctionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "||"; };
};

class ConjunctionNode : public Node
{
public:
    ConjunctionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "&&"; };
};

class InclusiveOrNode : public Node
{
public:
    InclusiveOrNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "|"; };
};

class ExclusiveOrNode : public Node
{
public:
    ExclusiveOrNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "^"; };
};

class AndNode : public Node
{
public:
    AndNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "&"; };
};

class EqualNode : public Node
{
public:
    EqualNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "=="; };
};

class NotEqualNode : public Node
{
public:
    NotEqualNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "!=="; };
};

class LessNode : public Node
{
public:
    LessNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "<"; };
};

class GreaterNode : public Node
{
public:
    GreaterNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ">"; };
};

class LessOrEqualNode : public Node
{
public:
    LessOrEqualNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "<="; };
};

class GreaterOrEqualNode : public Node
{
public:
    GreaterOrEqualNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ">="; };
};

class CompareNode : public Node
{
public:
    CompareNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "<=>"; };
};

class ShiftLeftNode : public Node
{
public:
    ShiftLeftNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "<<"; };
};

class ShiftRightNode : public Node
{
public:
    ShiftRightNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ">>"; };
};

class PlusNode : public Node
{
public:
    PlusNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "+"; };
};

class MinusNode : public Node
{
public:
    MinusNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "-"; };
};

class MulNode : public Node
{
public:
    MulNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "*"; };
};

class DivNode : public Node
{
public:
    DivNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "/"; };
};

class ModNode : public Node
{
public:
    ModNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "%"; };
};

class DotStarNode : public Node
{
public:
    DotStarNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return ".*"; };
};

class ArrowStarNode : public Node
{
public:
    ArrowStarNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "->"; };
};

class CastExprNode : public UnaryNode
{
public:
    CastExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    CastExprNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_, Node* child_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    inline Node* TypeId() const noexcept { return typeId.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    std::string Str() const override;
private:
    std::unique_ptr<Node> typeId;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class DerefNode : public Node
{
public:
    DerefNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "*"; }
};

class AddrOfNode : public Node
{
public:
    AddrOfNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "&"; }
};

class NotNode : public Node
{
public:
    NotNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "!"; }
};

class ComplementNode : public Node
{
public:
    ComplementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "~"; }
};

class PrefixIncNode : public Node
{
public:
    PrefixIncNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "++"; }
};

class PrefixDecNode : public Node
{
public:
    PrefixDecNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "--"; }
};

class AwaitExprNode : public UnaryNode
{
public:
    AwaitExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AwaitExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class SizeOfTypeExprNode : public UnaryNode
{
public:
    SizeOfTypeExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SizeOfTypeExprNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    std::string Str() const override;
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class SizeOfPackExprNode : public UnaryNode
{
public:
    SizeOfPackExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SizeOfPackExprNode(const soul::ast::Span& span_, int fileIndex_, Node* idNode_, const soul::ast::Span& ellipsisSpan_,
        const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& EllipsisSpan() const noexcept { return ellipsisSpan; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    soul::ast::Span ellipsisSpan;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class SizeOfUnaryExprNode : public UnaryNode
{
public:
    SizeOfUnaryExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SizeOfUnaryExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override;
};

class AlignOfExprNode : public UnaryNode
{
public:
    AlignOfExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AlignOfExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class NoexceptExprNode : public UnaryNode
{
public:
    NoexceptExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NoexceptExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class OpNewCall : public UnaryNode
{
public:
    OpNewCall(const soul::ast::Span& span_, int fileIndex_) noexcept;
    OpNewCall(const soul::ast::Span& span_, int fileIndex_, Node* sizeArg_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override;
};

class NewExprNode : public UnaryNode
{
public:
    NewExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NewExprNode(const soul::ast::Span& span_, int fileIndex_, Node* placement_, Node* child_, Node* initializer_, Node* colonColonHead_,
        const soul::ast::Span& newSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ColonColonHead() const noexcept { return colonColonHead.get(); }
    inline Node* Placement() const noexcept { return placement.get(); }
    inline Node* Initializer() const noexcept { return initializer.get(); }
    inline const soul::ast::Span& NewPos() const noexcept { return newSpan; }
    std::string Str() const override;
private:
    std::unique_ptr<Node> colonColonHead;
    std::unique_ptr<Node> placement;
    std::unique_ptr<Node> initializer;
    soul::ast::Span newSpan;
};

class NewPlacementNode : public ListNode
{
public:
    NewPlacementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline void SetLParenSpan(const soul::ast::Span& lpSpan_) noexcept { lpSpan = lpSpan_; }
    inline void SetRParenSpan(const soul::ast::Span& rpSpan_) noexcept { rpSpan = rpSpan_; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class ParenNewTypeIdNode : public UnaryNode
{
public:
    ParenNewTypeIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ParenNewTypeIdNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    std::string Str() const override;
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class NewTypeIdNode : public CompoundNode
{
public:
    NewTypeIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NewTypeIdNode(const soul::ast::Span& span_, int fileIndex_, Node* typeSpecifierSeq_, Node* newDeclarator_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* TypeSpecifierSeq() const noexcept { return typeSpecifierSeq.get(); }
    inline Node* NewDeclarator() const noexcept { return newDeclarator.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> typeSpecifierSeq;
    std::unique_ptr<Node> newDeclarator;
};

class ArrayDeletePtrNode : public UnaryNode
{
public:
    ArrayDeletePtrNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ArrayDeletePtrNode(const soul::ast::Span& span_, int fileIndex_, Node* ptr_, Node* colonColonHead_, const soul::ast::Span& deleteSpan_,
        const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ColonColonHead() const noexcept { return colonColonHead.get(); }
    inline const soul::ast::Span& DeleteSpan() const noexcept { return deleteSpan; }
    inline const soul::ast::Span& LBracketSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBracketSpan() const noexcept { return rbSpan; }
private:
    std::unique_ptr<Node> colonColonHead;
    soul::ast::Span deleteSpan;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class DeletePtrNode : public UnaryNode
{
public:
    DeletePtrNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DeletePtrNode(const soul::ast::Span& span_, int fileIndex_, Node* ptr_, Node* colonColonHead_, const soul::ast::Span& deleteSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ColonColonHead() const noexcept { return colonColonHead.get(); }
    inline const soul::ast::Span& DeleteSpan() const noexcept { return deleteSpan; }
private:
    std::unique_ptr<Node> colonColonHead;
    soul::ast::Span deleteSpan;
};

class SubscriptExprNode : public UnaryNode
{
public:
    SubscriptExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SubscriptExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, Node* index_, const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Index() const noexcept { return index.get(); }
    inline const soul::ast::Span& LBracketSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBracketSpan() const noexcept { return rbSpan; }
    std::string Str() const override;
private:
    std::unique_ptr<Node> index;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class InvokeExprNode : public ListNode
{
public:
    InvokeExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    InvokeExprNode(const soul::ast::Span& span_, int fileIndex_, Node* subject_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Subject() const noexcept { return subject.get(); }
    inline void SetLParenSpan(const soul::ast::Span& lpSpan_) noexcept { lpSpan = lpSpan_; }
    inline void SetRParenSpan(const soul::ast::Span& rpSpan_) noexcept { rpSpan = rpSpan_; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    inline bool IsCompileUnitInitFn() const noexcept { return compileUnitInitFn; }
    inline void SetCompileUnitInitFn() noexcept { compileUnitInitFn = true; }
    std::string Str() const override;
private:
    std::unique_ptr<Node> subject;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
    bool compileUnitInitFn;
};

class PairNode : public BinaryNode
{
public:
    PairNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    PairNode(const soul::ast::Span& span_, int fileIndex_, Node* left_, Node* right_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class DotNode : public Node
{
public:
    DotNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "."; }
};

class ArrowNode : public Node
{
public:
    ArrowNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "->"; }
};

class MemberExprNode : public UnaryNode
{
public:
    MemberExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    MemberExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, Node* op_, Node* id_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Op() const noexcept { return op.get(); }
    inline Node* GetId() const noexcept { return id.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> op;
    std::unique_ptr<Node> id;
};

class PostfixIncExprNode : public UnaryNode
{
public:
    PostfixIncExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    PostfixIncExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& opSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& OpSpan() const noexcept { return opSpan; }
    std::string Str() const override;
private:
    soul::ast::Span opSpan;
};

class PostfixDecExprNode : public UnaryNode
{
public:
    PostfixDecExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    PostfixDecExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& opSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& OpSpan() const noexcept { return opSpan; }
    std::string Str() const override;
private:
    soul::ast::Span opSpan;
};

class TypeIdExprNode : public UnaryNode
{
public:
    TypeIdExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TypeIdExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    std::string Str() const override;
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class DynamicCastNode : public Node
{
public:
    DynamicCastNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "dynamic_cast"; }
};

class StaticCastNode : public Node
{
public:
    StaticCastNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "static_cast"; }
};

class ReinterpretCastNode : public Node
{
public:
    ReinterpretCastNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "reinterpret_cast"; }
};

class ConstCastNode : public Node
{
public:
    ConstCastNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "const_cast"; }
};

class CppCastExprNode : public UnaryNode
{
public:
    CppCastExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    CppCastExprNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_, Node* child_, Node* op_,
        const soul::ast::Span& laSpan_, const soul::ast::Span& raSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* TypeId() const noexcept { return typeId.get(); }
    inline Node* Op() const noexcept { return op.get(); }
    inline const soul::ast::Span& LAngleSpan() const noexcept { return laSpan; }
    inline const soul::ast::Span& RAngleSpan() const noexcept { return raSpan; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    std::string Str() const override;
private:
    std::unique_ptr<Node> typeId;
    std::unique_ptr<Node> op;
    soul::ast::Span laSpan;
    soul::ast::Span raSpan;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class ThisNode : public Node
{
public:
    ThisNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "this"; }
};

class ParenthesizedExprNode : public UnaryNode
{
public:
    ParenthesizedExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ParenthesizedExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    std::string Str() const override;
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class FoldExprNode : public SequenceNode
{
public:
    FoldExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline void SetLParenSpan(const soul::ast::Span& lpSpan_) noexcept { lpSpan = lpSpan_; }
    inline void SetRParenSpan(const soul::ast::Span& rpSpan_) noexcept { rpSpan = rpSpan_; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class NewDeclaratorNode : public SequenceNode
{
public:
    NewDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ArrayNewDeclaratorNode : public SequenceNode
{
public:
    ArrayNewDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class NewInitializerNode : public ListNode
{
public:
    NewInitializerNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline void SetLParenSpan(const soul::ast::Span& lpSpan_) noexcept { lpSpan = lpSpan_; }
    inline void SetRParenSpan(const soul::ast::Span& rpSpan_) noexcept { rpSpan = rpSpan_; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class BracedInitListNode : public ListNode
{
public:
    BracedInitListNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class DesignatedInitializerNode : public BinaryNode
{
public:
    DesignatedInitializerNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DesignatedInitializerNode(const soul::ast::Span& span_, int fileIndex_, Node* designator_, Node* initializer_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class DesignatorNode : public UnaryNode
{
public:
    DesignatorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DesignatorNode(const soul::ast::Span& span_, int fileIndex_, Node* identifier_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ThrowExprNode : public UnaryNode
{
public:
    ThrowExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ThrowExprNode(const soul::ast::Span& span_, int fileIndex_, Node* expr__) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override;
};

} // namespace otava::ast
