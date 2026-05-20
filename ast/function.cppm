// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.function;

import std;
import otava.ast.node;

export namespace otava::ast {

class FunctionDefinitionNode : public CompoundNode
{
public:
    FunctionDefinitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    FunctionDefinitionNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* declSpecifiers_, Node* declarator_, Node* specifiers_,
        Node* functionBody_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* DeclSpecifiers() const noexcept { return declSpecifiers.get(); }
    inline Node* Declarator() const noexcept { return declarator.get(); }
    inline Node* Specifiers() const noexcept { return specifiers.get(); }
    inline Node* FunctionBody() const noexcept { return functionBody.get(); }
    void ResetFunctionBody(Node* functionBody_) noexcept;
    inline std::int32_t Index() const noexcept { return index; }
    inline void SetIndex(std::int32_t index_) noexcept { index = index_; }
private:
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> declSpecifiers;
    std::unique_ptr<Node> declarator;
    std::unique_ptr<Node> specifiers;
    std::unique_ptr<Node> functionBody;
    std::int32_t index;
};

class FunctionBodyNode : public UnaryNode
{
public:
    FunctionBodyNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    FunctionBodyNode(const soul::ast::Span& span_, int fileIndex_, Node* compoundStatement_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class DefaultNode : public Node
{
public:
    DefaultNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class DeleteNode : public Node
{
public:
    DeleteNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class DefaultedOrDeletedFunctionNode : public CompoundNode
{
public:
    DefaultedOrDeletedFunctionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DefaultedOrDeletedFunctionNode(const soul::ast::Span& span_, int fileIndex_, Node* assign_, Node* defaultOrDelete_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Assign() const noexcept { return assign.get(); }
    inline Node* DefaultOrDelete() const noexcept { return defaultOrDelete.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
private:
    std::unique_ptr<Node> assign;
    std::unique_ptr<Node> defaultOrDelete;
    std::unique_ptr<Node> semicolon;
};

class NoDeclSpecFunctionDeclarationNode : public CompoundNode
{
public:
    NoDeclSpecFunctionDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NoDeclSpecFunctionDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* declarator_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Declarator() const noexcept { return declarator.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
private:
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> declarator;
    std::unique_ptr<Node> semicolon;
};

class OperatorNode : public Node
{
public:
    OperatorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"operator"; }
};

class NewArrayOpNode : public CompoundNode
{
public:
    NewArrayOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NewArrayOpNode(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LBracketSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBracketSpan() const noexcept { return rbSpan; }
    std::u32string Str() const override { return U"new[]"; }
private:
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class NewOpNode : public Node
{
public:
    NewOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"new"; }
};

class DeleteArrayOpNode : public CompoundNode
{
public:
    DeleteArrayOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DeleteArrayOpNode(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& LBracketSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBracketSpan() const noexcept { return rbSpan; }
    std::u32string Str() const override { return U"delete[]"; }
private:
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class DeleteOpNode : public Node
{
public:
    DeleteOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"delete"; }
};

class CoAwaitOpNode : public Node
{
public:
    CoAwaitOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"co_await"; }
};

class InvokeOpNode : public Node
{
public:
    InvokeOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"()"; }
};

class SubscriptOpNode : public Node
{
public:
    SubscriptOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override { return U"[]"; }
};

class OperatorFunctionIdNode : public BinaryNode
{
public:
    OperatorFunctionIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    OperatorFunctionIdNode(const soul::ast::Span& span_, int fileIndex_, Node* operatorKwNode_, Node* op_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override;
};

class ConversionFunctionIdNode : public BinaryNode
{
public:
    ConversionFunctionIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ConversionFunctionIdNode(const soul::ast::Span& span_, int fileIndex_, Node* operatorKwNode_, Node* conversionTypeId_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::u32string Str() const override;
};

class ConversionTypeIdNode : public CompoundNode
{
public:
    ConversionTypeIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ConversionTypeIdNode(const soul::ast::Span& span_, int fileIndex_, Node* typeSpecifierSeq_, Node* conversionDeclarator_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* TypeSpecifierSeq() const noexcept { return typeSpecifierSeq.get(); }
    inline Node* ConversionDeclarator() const noexcept { return conversionDeclarator.get(); }
private:
    std::unique_ptr<Node> typeSpecifierSeq;
    std::unique_ptr<Node> conversionDeclarator;
};

class ConversionDeclaratorNode : public CompoundNode
{
public:
    ConversionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ConversionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* ptrOp_, Node* conversionDeclarator_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* PtrOp() const noexcept { return ptrOp.get(); }
    inline Node* ConversionDeclarator() const noexcept { return conversionDeclarator.get(); }
private:
    std::unique_ptr<Node> ptrOp;
    std::unique_ptr<Node> conversionDeclarator;
};

class DestructorIdNode : public UnaryNode
{
public:
    DestructorIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DestructorIdNode(const soul::ast::Span& span_, int fileIndex_, Node* typeName_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ParameterNode : public CompoundNode
{
public:
    ParameterNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Attributes() const noexcept { return attributes.get(); }
    void SetAttributes(Node* attributes_) noexcept;
    inline Node* DeclSpecifiers() const noexcept { return declSpecifiers.get(); }
    void SetDeclSpecifiers(Node* declSpecifiers_) noexcept;
    inline Node* Declarator() const noexcept { return declarator.get(); }
    void SetDeclarator(Node* declarator_) noexcept;
    inline Node* Assign() const noexcept { return assign.get(); }
    void SetAssign(Node* assign_) noexcept;
    inline Node* Initializer() const noexcept { return initializer.get(); }
    void SetInitializer(Node* initializer_) noexcept;
    std::u32string Str() const override;
private:
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> declSpecifiers;
    std::unique_ptr<Node> declarator;
    std::unique_ptr<Node> assign;
    std::unique_ptr<Node> initializer;
};

class ParameterListNode : public ListNode
{
public:
    ParameterListNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
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

class NoexceptNode : public Node
{
public:
    NoexceptNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class FunctionTryBlockNode : public CompoundNode
{
public:
    FunctionTryBlockNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    FunctionTryBlockNode(const soul::ast::Span& span_, int fileIndex_, Node* ctorInitializer_, Node* tryBlock_, Node* handlers_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* CtorInitializer() const noexcept { return ctorInitializer.get(); }
    inline Node* TryBlock() const noexcept { return tryBlock.get(); }
    inline Node* Handlers() const noexcept { return handlers.get(); }
private:
    std::unique_ptr<Node> ctorInitializer;
    std::unique_ptr<Node> tryBlock;
    std::unique_ptr<Node> handlers;
};

} // namespace otava::ast
