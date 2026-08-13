// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.expression;

import std;
import otava.ast.node;
import soul.ast.span;

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
    std::string Str() const override;
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
    CastExprNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_, Node* child_) noexcept;
    inline Node* TypeId() const noexcept { return typeId.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string Str() const override;
private:
    std::unique_ptr<Node> typeId;
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
    SizeOfTypeExprNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string Str() const override;
};

class SizeOfPackExprNode : public UnaryNode
{
public:
    SizeOfPackExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SizeOfPackExprNode(const soul::ast::Span& span_, int fileIndex_, Node* idNode_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
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
    AlignOfExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
};

class NoexceptExprNode : public UnaryNode
{
public:
    NoexceptExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NoexceptExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
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
    NewExprNode(const soul::ast::Span& span_, int fileIndex_, Node* placement_, Node* child_, Node* initializer_, Node* colonColonHead_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ColonColonHead() const noexcept { return colonColonHead.get(); }
    inline Node* Placement() const noexcept { return placement.get(); }
    inline Node* Initializer() const noexcept { return initializer.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> colonColonHead;
    std::unique_ptr<Node> placement;
    std::unique_ptr<Node> initializer;
};

class NewPlacementNode : public ListNode
{
public:
    NewPlacementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
};

class ParenNewTypeIdNode : public UnaryNode
{
public:
    ParenNewTypeIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ParenNewTypeIdNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string Str() const override;
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
    ArrayDeletePtrNode(const soul::ast::Span& span_, int fileIndex_, Node* ptr_, Node* colonColonHead_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ColonColonHead() const noexcept { return colonColonHead.get(); }
private:
    std::unique_ptr<Node> colonColonHead;
};

class DeletePtrNode : public UnaryNode
{
public:
    DeletePtrNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DeletePtrNode(const soul::ast::Span& span_, int fileIndex_, Node* ptr_, Node* colonColonHead_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ColonColonHead() const noexcept { return colonColonHead.get(); }
private:
    std::unique_ptr<Node> colonColonHead;
};

class SubscriptExprNode : public UnaryNode
{
public:
    SubscriptExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SubscriptExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, Node* index_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Index() const noexcept { return index.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> index;
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
    inline bool IsCompileUnitInitFn() const noexcept { return compileUnitInitFn; }
    inline void SetCompileUnitInitFn() noexcept { compileUnitInitFn = true; }
    std::string Str() const override;
private:
    std::unique_ptr<Node> subject;
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
    PostfixIncExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string Str() const override;
};

class PostfixDecExprNode : public UnaryNode
{
public:
    PostfixDecExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    PostfixDecExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string Str() const override;
};

class TypeIdExprNode : public UnaryNode
{
public:
    TypeIdExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TypeIdExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string Str() const override;
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
    CppCastExprNode(const soul::ast::Span& span_, int fileIndex_, Node* typeId_, Node* child_, Node* op_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* TypeId() const noexcept { return typeId.get(); }
    inline Node* Op() const noexcept { return op.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> typeId;
    std::unique_ptr<Node> op;
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
    ParenthesizedExprNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string Str() const override;
};

class FoldExprNode : public SequenceNode
{
public:
    FoldExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
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
