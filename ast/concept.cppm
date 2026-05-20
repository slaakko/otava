// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.concepts;

import std;
import otava.ast.node;

export namespace otava::ast {

class ConceptDefinitionNode : public CompoundNode
{
public:
    ConceptDefinitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ConceptDefinitionNode(const soul::ast::Span& span_, int fileIndex_, Node* conceptName_, Node* assign_, Node* constraintExpr_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ConceptName() const noexcept { return conceptName.get(); }
    inline Node* Assign() const noexcept { return assign.get(); }
    inline Node* ConstraintExpression() const noexcept { return constraintExpr.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
private:
    std::unique_ptr<Node> conceptName;
    std::unique_ptr<Node> assign;
    std::unique_ptr<Node> constraintExpr;
    std::unique_ptr<Node> semicolon;
};

class RequiresExprNode : public CompoundNode
{
public:
    RequiresExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    RequiresExprNode(const soul::ast::Span& span_, int fileIndex_, Node* params_, Node* body_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Params() const noexcept { return params.get(); }
    inline Node* Body() const noexcept { return body.get(); }
private:
    std::unique_ptr<Node> params;
    std::unique_ptr<Node> body;
};

class RequirementBodyNode : public SequenceNode
{
public:
    RequirementBodyNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void SetLBraceSpan(const soul::ast::Span& lbSpan_) noexcept;
    void SetRBraceSpan(const soul::ast::Span& rbSpan_) noexcept;
    inline const soul::ast::Span& LBraceSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBraceSpan() const noexcept { return rbSpan; }
private:
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class SimpleRequirementNode : public BinaryNode
{
public:
    SimpleRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SimpleRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* expr_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class TypeRequirementNode : public CompoundNode
{
public:
    TypeRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TypeRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* nns_, Node* typeName_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* NestedNameSpecifier() const noexcept { return nns.get(); }
    inline Node* TypeName() const noexcept { return typeName.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
private:
    std::unique_ptr<Node> nns;
    std::unique_ptr<Node> typeName;
    std::unique_ptr<Node> semicolon;
};

class CompoundRequirementNode : public CompoundNode
{
public:
    CompoundRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    CompoundRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* expr_, Node* noexcept_, Node* returnTypeRequirement_, Node* semicolon_,
        const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Expr() const noexcept { return expr.get(); }
    inline Node* NoExcept() const noexcept { return noexceptNode.get(); }
    inline Node* ReturnTypeRequirement() const noexcept { return returnTypeRequirement.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& LBraceSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBraceSpan() const noexcept { return rbSpan; }
private:
    std::unique_ptr<Node> expr;
    std::unique_ptr<Node> noexceptNode;
    std::unique_ptr<Node> returnTypeRequirement;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class ReturnTypeRequirementNode : public UnaryNode
{
public:
    ReturnTypeRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ReturnTypeRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* typeConstraint_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class NestedRequirementNode : public BinaryNode
{
public:
    NestedRequirementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NestedRequirementNode(const soul::ast::Span& span_, int fileIndex_, Node* constraintExpression_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class TypeConstraintNode : public ListNode
{
public:
    TypeConstraintNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TypeConstraintNode(const soul::ast::Span& span_, int fileIndex_, Node* conceptName_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline void SetHasTemplateArgumentList() noexcept { hasTemplateArgumentList = true; }
    inline bool HasTemplateArgumentList() const noexcept { return hasTemplateArgumentList; }
    inline Node* ConceptName() const noexcept { return conceptName.get(); }
    void SetLAngleSpan(const soul::ast::Span& laSpan_) noexcept;
    void SetRAngleSpan(const soul::ast::Span& raSpan_) noexcept;
    inline const soul::ast::Span& LAngleSpan() const noexcept { return laSpan; }
    inline const soul::ast::Span& RAngleSpan() const noexcept { return laSpan; }
private:
    std::unique_ptr<Node> conceptName;
    bool hasTemplateArgumentList;
    soul::ast::Span laSpan;
    soul::ast::Span raSpan;
};

class RequiresClauseNode : public UnaryNode
{
public:
    RequiresClauseNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    RequiresClauseNode(const soul::ast::Span& span_, int fileIndex_, Node* constraintLogicalOrExpr_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

} // namespace otava::ast
