// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.declaration;

import std;
import otava.ast.node;

export namespace otava::ast {

class DeclarationSequenceNode : public SequenceNode
{
public:
    DeclarationSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class SimpleDeclarationNode : public CompoundNode
{
public:
    SimpleDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SimpleDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* declarationSpecifiers_, Node* initDeclaratorList_, Node* attributes_, 
        Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* DeclarationSpecifiers() const noexcept { return declarationSpecifiers.get(); }
    inline void SetDeclarationSpecifiers(Node* declarationSpecifiers_) noexcept { declarationSpecifiers.reset(declarationSpecifiers_); }
    inline Node* InitDeclaratorList() const noexcept { return initDeclaratorList.get(); }
    inline void SetInitDeclaratorList(Node* initDeclaratorList_) noexcept { initDeclaratorList.reset(initDeclaratorList_); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> declarationSpecifiers;
    std::unique_ptr<Node> initDeclaratorList;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
};

class AsmDeclarationNode : public CompoundNode
{
public:
    AsmDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AsmDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* asm__, Node* asmText_, Node* semicolon_, Node* attributes_,
        const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Asm() const noexcept { return asm_.get(); }
    inline Node* AsmText() const noexcept { return asmText.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    std::unique_ptr<Node> asm_;
    std::unique_ptr<Node> asmText;
    std::unique_ptr<Node> semicolon;
    std::unique_ptr<Node> attributes;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class AsmNode : public Node
{
public:
    AsmNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class LinkageSpecificationNode : public CompoundNode
{
public:
    LinkageSpecificationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    LinkageSpecificationNode(const soul::ast::Span& span_, int fileIndex_, Node* extrn_, Node* linkage_, Node* declarations_,
        const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Extern() const noexcept { return extrn.get(); }
    inline Node* Linkage() const noexcept { return linkage.get(); }
    inline Node* Declarations() const noexcept { return declarations.get(); }
    inline const soul::ast::Span& LBraceSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBraceSpan() const noexcept { return rbSpan; }
private:
    std::unique_ptr<Node> extrn;
    std::unique_ptr<Node> linkage;
    std::unique_ptr<Node> declarations;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class NamespaceDefinitionNode : public CompoundNode
{
public:
    NamespaceDefinitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NamespaceDefinitionNode(const soul::ast::Span& span_, int fileIndex_, Node* nskw_, Node* nsName_, Node* nsBody_, Node* inln_, Node* attributes_,
        const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Namespace() const noexcept { return nskw.get(); }
    inline Node* NamespaceName() const noexcept { return nsName.get(); }
    inline Node* NamespaceBody() const noexcept { return nsBody.get(); }
    inline Node* Inline() const noexcept { return inln.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& LBraceSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBraceSpan() const noexcept { return rbSpan; }
private:
    std::unique_ptr<Node> nskw;
    std::unique_ptr<Node> nsName;
    std::unique_ptr<Node> nsBody;
    std::unique_ptr<Node> inln;
    std::unique_ptr<Node> attributes;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class NamespaceBodyNode : public CompoundNode
{
public:
    NamespaceBodyNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NamespaceBodyNode(const soul::ast::Span& span_, int fileIndex_, Node* declarations_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Declarations() const noexcept { return declarations.get(); }
private:
    std::unique_ptr<Node> declarations;
};

class NamespaceAliasDefinitionNode : public CompoundNode
{
public:
    NamespaceAliasDefinitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NamespaceAliasDefinitionNode(const soul::ast::Span& span_, int fileIndex_, Node* id_, Node* assign_, Node* qns_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Id() const noexcept { return id.get(); }
    inline Node* Assign() const noexcept { return assign.get(); }
    inline Node* QualifiedNamespaceSpecifier() const noexcept { return qns.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
private:
    std::unique_ptr<Node> id;
    std::unique_ptr<Node> assign;
    std::unique_ptr<Node> qns;
    std::unique_ptr<Node> semicolon;
};

class UsingDeclarationNode : public CompoundNode
{
public:
    UsingDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    UsingDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* usng_, Node* declarators_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Using() const noexcept { return usng.get(); }
    inline Node* Declarators() const noexcept { return declarators.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
private:
    std::unique_ptr<Node> usng;
    std::unique_ptr<Node> declarators;
    std::unique_ptr<Node> semicolon;
};

class UsingNode : public Node
{
public:
    UsingNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override { return "using"; }
};

class UsingDeclaratorListNode : public ListNode
{
public:
    UsingDeclaratorListNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class UsingEnumDeclarationNode : public CompoundNode
{
public:
    UsingEnumDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    UsingEnumDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* usng_, Node* ees_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Using() const noexcept { return usng.get(); }
    inline Node* ElaboratedEnumSpecifier() const noexcept { return ees.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
private:
    std::unique_ptr<Node> usng;
    std::unique_ptr<Node> ees;
    std::unique_ptr<Node> semicolon;
};

class UsingDirectiveNode : public CompoundNode
{
public:
    UsingDirectiveNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    UsingDirectiveNode(const soul::ast::Span& span_, int fileIndex_, Node* usng_, Node* nskw_, Node* id_, Node* semicolon_, Node* attributes_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Using() const noexcept { return usng.get(); }
    inline Node* Namespace() const noexcept { return nskw.get(); }
    inline Node* GetId() const noexcept { return id.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
private:
    std::unique_ptr<Node> usng;
    std::unique_ptr<Node> nskw;
    std::unique_ptr<Node> id;
    std::unique_ptr<Node> semicolon;
    std::unique_ptr<Node> attributes;
};

class NamespaceNode : public Node
{
public:
    NamespaceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class StaticAssertDeclarationNode : public CompoundNode
{
public:
    StaticAssertDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    StaticAssertDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* staticAssert_, Node* constantExpr_, Node* comma_, Node* stringLiteral_,
        Node* semicolon_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* StaticAssert() const noexcept { return staticAssert.get(); }
    inline Node* ConstantExpr() const noexcept { return constantExpr.get(); }
    inline Node* Comma() const noexcept { return comma.get(); }
    inline Node* StringLiteral() const noexcept { return stringLiteral.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    std::unique_ptr<Node> staticAssert;
    std::unique_ptr<Node> constantExpr;
    std::unique_ptr<Node> comma;
    std::unique_ptr<Node> stringLiteral;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class StaticAssertNode : public Node
{
public:
    StaticAssertNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class AliasDeclarationNode : public CompoundNode
{
public:
    AliasDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AliasDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* usng_, Node* identifier_, Node* assign_, Node* definingTypeId_, Node* attributes_,
        Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Using() const noexcept { return usng.get(); }
    inline Node* Identifier() const noexcept { return identifier.get(); }
    inline Node* Assign() const noexcept { return assign.get(); }
    inline Node* DefiningTypeId() const noexcept { return definingTypeId.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> usng;
    std::unique_ptr<Node> identifier;
    std::unique_ptr<Node> assign;
    std::unique_ptr<Node> definingTypeId;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
};

class EmptyDeclarationNode : public Node
{
public:
    EmptyDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class AttributeDeclarationNode : public BinaryNode
{
public:
    AttributeDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AttributeDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class InitDeclaratorListNode : public ListNode
{
public:
    InitDeclaratorListNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class InitDeclaratorNode : public BinaryNode
{
public:
    InitDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    InitDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* declarator_, Node* init_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override;
};

class TrailingFunctionDeclaratorNode : public CompoundNode
{
public:
    TrailingFunctionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TrailingFunctionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* declarator_, Node* params_, Node* trailingReturnType_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Declarator() const noexcept { return declarator.get(); }
    inline Node* Parameters() const noexcept { return params.get(); }
    inline Node* TrailingReturnType() const noexcept { return trailingReturnType.get(); }
private:
    std::unique_ptr<Node> declarator;
    std::unique_ptr<Node> params;
    std::unique_ptr<Node> trailingReturnType;
};

class ParenthesizedDeclaratorNode : public CompoundNode
{
public:
    ParenthesizedDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ParenthesizedDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* declarator_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Declarator() const noexcept { return declarator.get(); }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    std::string Str() const override;
private:
    std::unique_ptr<Node> declarator;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class AbstractDeclaratorNode : public Node
{
public:
    AbstractDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class DeclSpecifierSequenceNode : public SequenceNode
{
public:
    DeclSpecifierSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override;
};

class FriendNode : public Node
{
public:
    FriendNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class TypedefNode : public Node
{
public:
    TypedefNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ConstExprNode : public Node
{
public:
    ConstExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ConstEvalNode : public Node
{
public:
    ConstEvalNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ConstInitNode : public Node
{
public:
    ConstInitNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class InlineNode : public Node
{
public:
    InlineNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override; 
    void Accept(Visitor& visitor) override;
};

class StaticNode : public Node
{
public:
    StaticNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ThreadLocalNode : public Node
{
public:
    ThreadLocalNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ExternNode : public Node
{
public:
    ExternNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class MutableNode : public Node
{
public:
    MutableNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ExplicitNode : public Node
{
public:
    ExplicitNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ConditionalExplicitNode : public UnaryNode
{
public:
    ConditionalExplicitNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ConditionalExplicitNode(const soul::ast::Span& span_, int fileIndex_, Node* cond_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Condition() const noexcept { return cond.get(); }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    std::unique_ptr<Node> cond;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class QualifiedPtrNode : public BinaryNode
{
public:
    QualifiedPtrNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    QualifiedPtrNode(const soul::ast::Span& span_, int fileIndex_, Node* nns_, Node* ptr_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class PtrOperatorNode : public UnaryNode
{
public:
    PtrOperatorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    PtrOperatorNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class PtrDeclaratorNode : public SequenceNode
{
public:
    PtrDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Str() const override;
};

class PackDeclaratorIdNode : public BinaryNode
{
public:
    PackDeclaratorIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    PackDeclaratorIdNode(const soul::ast::Span& span_, int fileIndex_, Node* ellipsis_, Node* idExpr_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ArrayDeclaratorNode : public UnaryNode
{
public:
    ArrayDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ArrayDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, Node* dimension_, const soul::ast::Span& lbSpan_,
        const soul::ast::Span& rbSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Dimension() const noexcept { return dimension.get(); }
    inline const soul::ast::Span& LBracketSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBracketSpan() const noexcept { return rbSpan; }
    std::string Str() const override;
private:
    std::unique_ptr<Node> dimension;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class FunctionDeclaratorNode : public UnaryNode
{
public:
    FunctionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    FunctionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, Node* parameters_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Params() const noexcept { return params.get(); }
    std::string Str() const override;
private:
    std::unique_ptr<Node> params;
};

class PrefixNode : public BinaryNode
{
public:
    PrefixNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    PrefixNode(const soul::ast::Span& span_, int fileIndex_, Node* prefix_, Node* subject_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class TrailingQualifiersNode : public SequenceNode
{
public:
    TrailingQualifiersNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TrailingQualifiersNode(const soul::ast::Span& span_, int fileIndex_, Node* subject_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Subject() const noexcept { return subject.get(); }
    inline Node* ReleaseSubject() noexcept { return subject.release(); }
private:
    std::unique_ptr<Node> subject;
};

class TrailingAttributesNode : public BinaryNode
{
public:
    TrailingAttributesNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TrailingAttributesNode(const soul::ast::Span& span_, int fileIndex_, Node* left_, Node* attributes_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class NoexceptSpecifierNode : public CompoundNode
{
public:
    NoexceptSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NoexceptSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* constantExpr_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ConstantExpr() const noexcept { return constantExpr.get(); }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    std::unique_ptr<Node> constantExpr;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class ThrowSpecifierNode : public CompoundNode
{
public:
    ThrowSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ThrowSpecifierNode(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LParenSpan() const { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const { return rpSpan; }
private:
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

} // namespace otava::ast
