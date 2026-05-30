// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.classes;

import std;
import otava.ast.node;
import soul.ast.lexer_pos_pair;

export namespace otava::ast {

class ClassSpecifierNode : public SequenceNode
{
public:
    ClassSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ClassSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* classHead_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ClassHead() const noexcept { return classHead.get(); }
    inline void SetLBraceSpan(const soul::ast::Span& lbSpan_) noexcept { lbSpan = lbSpan_; }
    inline void SetRBraceSpan(const soul::ast::Span& rbSpan_) noexcept { rbSpan = rbSpan_; }
    inline const soul::ast::Span& LBraceSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBraceSpan() const noexcept { return rbSpan; }
    inline bool Complete() const noexcept { return complete; }
    inline void SetComplete() noexcept { complete = true; }
private:
    std::unique_ptr<Node> classHead;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
    bool complete;
};

class ClassHeadNode : public CompoundNode
{
public:
    ClassHeadNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ClassHeadNode(const soul::ast::Span& span_, int fileIndex_, Node* classKey_, Node* classHeadName_, Node* classVirtSpecifier_, Node* baseClause_, 
        Node* attributes_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ClassKey() const noexcept { return classKey.get(); }
    inline Node* ClassHeadName() const noexcept { return classHeadName.get(); }
    inline Node* ClassVirtSpecifier() const noexcept { return classVirtSpecifier.get(); }
    inline Node* BaseClause() const noexcept { return baseClause.get(); }
    inline Node* Attibutes() const noexcept { return attributes.get(); }
private:
    std::unique_ptr<Node> classKey;
    std::unique_ptr<Node> classHeadName;
    std::unique_ptr<Node> classVirtSpecifier;
    std::unique_ptr<Node> baseClause;
    std::unique_ptr<Node> attributes;
};

class BaseClauseNode : public UnaryNode
{
public:
    BaseClauseNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    BaseClauseNode(const soul::ast::Span& span_, int fileIndex_, Node* baseSpecifierList_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class BaseSpecifierListNode : public ListNode
{
public:
    BaseSpecifierListNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class BaseSpecifierNode : public CompoundNode
{
public:
    BaseSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    BaseSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* classOrDeclType_, Node* accessSpecifier_, Node* virtualSpecifier_, Node* attributes_,
        bool virtualFirst_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ClassOrDeclType() const noexcept { return classOrDeclType.get(); }
    inline Node* AccessSpecifier() const noexcept { return accessSpecifier.get(); }
    inline Node* VirtualSpecifier() const noexcept { return virtualSpecifier.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline bool VirtualFirst() const noexcept { return virtualFirst; }
private:
    std::unique_ptr<Node> classOrDeclType;
    std::unique_ptr<Node> accessSpecifier;
    std::unique_ptr<Node> virtualSpecifier;
    std::unique_ptr<Node> attributes;
    bool virtualFirst;
};

class BeginAccessGroupNode : public UnaryNode
{
public:
    BeginAccessGroupNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    BeginAccessGroupNode(const soul::ast::Span& span_, int fileIndex_, Node* accessSpecifier_, const soul::ast::Span& colonSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& ColonSpan() const noexcept { return colonSpan; }
private:
    soul::ast::Span colonSpan;
};

class MemberDeclarationNode : public CompoundNode
{
public:
    MemberDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    MemberDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* declSpecifiers_, Node* memberDeclarators_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* DeclSpecifiers() const noexcept { return declSpecifiers.get(); }
    inline Node* MemberDeclarators() const noexcept { return memberDeclarators.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
private:
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> declSpecifiers;
    std::unique_ptr<Node> memberDeclarators;
    std::unique_ptr<Node> semicolon;
};

class MemberDeclaratorListNode : public ListNode
{
public:
    MemberDeclaratorListNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ConstructorNode : public BinaryNode
{
public:
    ConstructorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ConstructorNode(const soul::ast::Span& span_, int fileIndex_, Node* constructorInitializer_, Node* compoundStatement_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ConstructorInitializerNode : public CompoundNode
{
public:
    ConstructorInitializerNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ConstructorInitializerNode(const soul::ast::Span& span_, int fileIndex_, Node* memberInitializerList_) noexcept;
    inline Node* GetMemberInitializerListNode() const noexcept { return memberInitializerListNode.get(); }
    void SetMemberInitializerListNode(Node* memberInitializerListNode_) noexcept;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void SetLexerPosPair(const soul::ast::lexer::pos::pair::LexerPosPair& lexerPosPair_) noexcept;
    inline const soul::ast::lexer::pos::pair::LexerPosPair& GetLexerPosPair() const noexcept { return lexerPosPair; }
    inline void* FunctionScope() const noexcept { return functionScope; }
    inline void SetFunctionScope(void* functionScope_) noexcept { functionScope = functionScope_; }
private:
    std::unique_ptr<Node> memberInitializerListNode;
    soul::ast::lexer::pos::pair::LexerPosPair lexerPosPair;
    void* functionScope;
};

class MemberInitializerListNode : public ListNode
{
public:
    MemberInitializerListNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class MemberInitializerNode : public BinaryNode
{
public:
    MemberInitializerNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    MemberInitializerNode(const soul::ast::Span& span_, int fileIndex_, Node* id_, Node* initializer_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class VirtSpecifierSequenceNode : public SequenceNode
{
public:
    VirtSpecifierSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ClassNode : public Node
{
public:
    ClassNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class StructNode : public Node
{
public:
    StructNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class UnionNode : public Node
{
public:
    UnionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class PublicNode : public Node
{
public:
    PublicNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ProtectedNode : public Node
{
public:
    ProtectedNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class PrivateNode : public Node
{
public:
    PrivateNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class VirtualNode : public Node
{
public:
    VirtualNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class OverrideNode : public Node
{
public:
    OverrideNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class FinalNode : public Node
{
public:
    FinalNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class PureSpecifierNode : public Node
{
public:
    PureSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    PureSpecifierNode(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& zeroSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& ZeroSpan() const noexcept { return zeroSpan; }
private:
    soul::ast::Span zeroSpan;
};

} // namespace otava::ast
