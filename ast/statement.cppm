// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.statement;

import std;
import otava.ast.node;
import util.uuid;
import soul.ast.lexer.pos.pair;

export namespace otava::ast {

class LabeledStatementNode : public CompoundNode
{
public:
    LabeledStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    LabeledStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* label_, Node* stmt_, Node* attributes_, const soul::ast::Span& colonSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Label() const noexcept { return label.get(); }
    inline Node* Statement() const noexcept { return stmt.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& ColonSpan() const noexcept { return colonSpan; }
private:
    std::unique_ptr<Node> label;
    std::unique_ptr<Node> stmt;
    std::unique_ptr<Node> attributes;
    soul::ast::Span colonSpan;
};

class CaseStatementNode : public CompoundNode
{
public:
    CaseStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    CaseStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* caseExpr_, Node* stmt_, Node* attributes_, const soul::ast::Span& caseSpan_,
        const soul::ast::Span& colonSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* CaseExpression() const noexcept { return caseExpr.get(); }
    inline Node* Statement() const noexcept { return stmt.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& CaseSpan() const noexcept { return caseSpan; }
    inline const soul::ast::Span& ColonSpan() const noexcept { return colonSpan; }
private:
    std::unique_ptr<Node> caseExpr;
    std::unique_ptr<Node> stmt;
    std::unique_ptr<Node> attributes;
    soul::ast::Span caseSpan;
    soul::ast::Span colonSpan;
};

class DefaultStatementNode : public CompoundNode
{
public:
    DefaultStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DefaultStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* stmt_, Node* attributes_, const soul::ast::Span& defaultPos_,
        const soul::ast::Span& colonSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Statement() const noexcept { return stmt.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& DefaultSpan() const noexcept { return defaultSpan; }
    inline const soul::ast::Span& ColonSpan() const noexcept { return colonSpan; }
private:
    std::unique_ptr<Node> stmt;
    std::unique_ptr<Node> attributes;
    soul::ast::Span defaultSpan;
    soul::ast::Span colonSpan;
};

class CompoundStatementNode : public SequenceNode
{
public:
    CompoundStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline void SetAttributes(Node* attributes_) noexcept { attributes.reset(attributes_); }
    void SetLexerPosPair(const soul::ast::lexer::pos::pair::LexerPosPair& lexerPosPair_) noexcept;
    inline const soul::ast::lexer::pos::pair::LexerPosPair& GetLexerPosPair() const noexcept { return lexerPosPair; }
    inline void SetLBraceSpan(const soul::ast::Span& lbSpan_) noexcept { lbSpan = lbSpan_; }
    inline void SetRBraceSpan(const soul::ast::Span& rbSpan_) noexcept { rbSpan = rbSpan_; }
    inline const soul::ast::Span& LBraceSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBraceSpan() const noexcept { return rbSpan; }
    int Level() const noexcept;
    inline void* FunctionScope() const noexcept { return functionScope; }
    inline void SetFunctionScope(void* functionScope_) noexcept { functionScope = functionScope_; }
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::unique_ptr<Node> attributes;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
    soul::ast::lexer::pos::pair::LexerPosPair lexerPosPair;
    void* functionScope;
    int blockId;
};

class SequenceStatementNode : public CompoundNode
{
public:
    SequenceStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SequenceStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* firstStmt_, Node* secondStmt_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* FirstStmt() const noexcept { return firstStmt.get(); }
    inline Node* SecondStmt() const noexcept { return secondStmt.get(); }
private:
    std::unique_ptr<Node> firstStmt;
    std::unique_ptr<Node> secondStmt;
};

class IfStatementNode : public CompoundNode
{
public:
    IfStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    IfStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* cond_, Node* thenStmt_, Node* elseStmt_, Node* attributes_,
        const soul::ast::Span& ifSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_, const soul::ast::Span& constExprSpan_, 
        const soul::ast::Span& elsePos_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Condition() const noexcept { return cond.get(); }
    inline Node* ThenStatement() const noexcept { return thenStmt.get(); }
    inline Node* ElseStatement() const noexcept { return elseStmt.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& IfSpan() const noexcept { return ifSpan; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    inline const soul::ast::Span& ConstExprSpan() const noexcept { return constExprSpan; }
    inline const soul::ast::Span& ElseSpan() const noexcept { return elseSpan; }
    inline bool IsConstExprIf() const noexcept { return constExprSpan.IsValid(); }
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::unique_ptr<Node> cond;
    std::unique_ptr<Node> thenStmt;
    std::unique_ptr<Node> elseStmt;
    std::unique_ptr<Node> attributes;
    soul::ast::Span ifSpan;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
    soul::ast::Span constExprSpan;
    soul::ast::Span elseSpan;
    int blockId;
};

class SwitchStatementNode : public CompoundNode
{
public:
    SwitchStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    SwitchStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* cond_, Node* stmt_, Node* attributes_, const soul::ast::Span& switchSpan_,
        const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Condition() const noexcept { return cond.get(); }
    inline Node* Statement() const noexcept { return stmt.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& SwitchSpan() const noexcept { return switchSpan; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::unique_ptr<Node> cond;
    std::unique_ptr<Node> stmt;
    std::unique_ptr<Node> attributes;
    soul::ast::Span switchSpan;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
    int blockId;
};

class WhileStatementNode : public CompoundNode
{
public:
    WhileStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    WhileStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* cond_, Node* stmt_, Node* attributes_, const soul::ast::Span& whileSpan_,
        const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Condition() const noexcept { return cond.get(); }
    inline Node* Statement() const noexcept { return stmt.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& WhileSpan() const noexcept { return whileSpan; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::unique_ptr<Node> cond;
    std::unique_ptr<Node> stmt;
    std::unique_ptr<Node> attributes;
    soul::ast::Span whileSpan;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
    int blockId;
};

class DoStatementNode : public CompoundNode
{
public:
    DoStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DoStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* stmt_, Node* expr_, Node* attributes_, Node* semicolon_,
        const soul::ast::Span& doSpan_, const soul::ast::Span& whileSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Statement() const noexcept { return stmt.get(); }
    inline Node* Expression() const noexcept { return expr.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& DoSpan() const noexcept { return doSpan; }
    inline const soul::ast::Span& WhileSpan() const noexcept { return whileSpan; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    std::unique_ptr<Node> stmt;
    std::unique_ptr<Node> expr;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span doSpan;
    soul::ast::Span whileSpan;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class RangeForStatementNode : public CompoundNode
{
public:
    RangeForStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    RangeForStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* initStmt_, Node* declaration_, Node* initializer_, Node* stmt_, Node* attributes_,
        const soul::ast::Span& forSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_, const soul::ast::Span& colonSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* InitStatement() const noexcept { return initStmt.get(); }
    inline Node* Declaration() const noexcept { return declaration.get(); }
    inline Node* Initializer() const noexcept { return initializer.get(); }
    inline Node* Statement() const noexcept { return stmt.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& ForSpan() const noexcept { return forSpan; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    inline const soul::ast::Span& ColonSpan() const noexcept { return colonSpan; }
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
    void SetRangeForId(const util::uuid& rangeForId_) noexcept;
    inline const util::uuid& RangeForId() const noexcept { return rangeForId; }
private:
    std::unique_ptr<Node> initStmt;
    std::unique_ptr<Node> declaration;
    std::unique_ptr<Node> initializer;
    std::unique_ptr<Node> stmt;
    std::unique_ptr<Node> attributes;
    soul::ast::Span forSpan;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
    soul::ast::Span colonSpan;
    int blockId;
    util::uuid rangeForId;
};

class ForRangeDeclarationNode : public BinaryNode
{
public:
    ForRangeDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ForRangeDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* declSpecifierSeq_, Node* declarator_, Node* attributes_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Attributes() const noexcept { return attributes.get(); }
private:
    std::unique_ptr<Node> attributes;
};

class StructuredBindingNode : public CompoundNode
{
public:
    StructuredBindingNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    StructuredBindingNode(const soul::ast::Span& span_, int fileIndex_, Node* declSpecifierSeq_, Node* refQualifier_, Node* identifiers_, Node* initializer_,
        Node* attributes_, Node* semicolon_, const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* DeclSpecifiers() const noexcept { return declSpecifiers.get(); }
    inline Node* RefQualifier() const noexcept { return refQualifier.get(); }
    inline Node* Identifiers() const noexcept { return identifiers.get(); }
    inline Node* Initializer() const noexcept { return initializer.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& LBracketSpan() const noexcept { return lbSpan; }
    inline const soul::ast::Span& RBracketSpan() const noexcept { return rbSpan; }
private:
    std::unique_ptr<Node> declSpecifiers;
    std::unique_ptr<Node> refQualifier;
    std::unique_ptr<Node> identifiers;
    std::unique_ptr<Node> initializer;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span lbSpan;
    soul::ast::Span rbSpan;
};

class ForStatementNode : public CompoundNode
{
public:
    ForStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ForStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* initStmt_, Node* cond_, Node* loopExpr_, Node* stmt_, Node* attributes_,
        Node* semicolon_, const soul::ast::Span& forSpan_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* InitStatement() const noexcept { return initStmt.get(); }
    inline Node* Condition() const noexcept { return cond.get(); }
    inline Node* LoopExpr() const noexcept { return loopExpr.get(); }
    inline Node* Statement() const noexcept { return stmt.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& ForSpan() const noexcept { return forSpan; }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::unique_ptr<Node> initStmt;
    std::unique_ptr<Node> cond;
    std::unique_ptr<Node> loopExpr;
    std::unique_ptr<Node> stmt;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span forSpan;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
    int blockId;
};

class BreakStatementNode : public CompoundNode
{
public:
    BreakStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    BreakStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* semicolon_, const soul::ast::Span& breakSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& BreakSpan() const noexcept { return breakSpan; }
private:
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span breakSpan;
};

class ContinueStatementNode : public CompoundNode
{
public:
    ContinueStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ContinueStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* semicolon_, const soul::ast::Span& continueSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& ContinueSpan() const noexcept { return continueSpan; }
private:
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span continueSpan;
};

class ReturnStatementNode : public CompoundNode
{
public:
    ReturnStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ReturnStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* returnValue_, Node* attributes_, Node* semicolon_, const soul::ast::Span& returnSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ReturnValue() const noexcept { return returnValue.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& ReturnSpan() const noexcept { return returnSpan; }
private:
    std::unique_ptr<Node> returnValue;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span returnSpan;
};

class CoReturnStatementNode : public CompoundNode
{
public:
    CoReturnStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    CoReturnStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* returnValue_, Node* attributes_, Node* semicolon_,
        const soul::ast::Span& coReturnSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* ReturnValue() const noexcept { return returnValue.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& CoReturnSpan() const noexcept { return coReturnSpan; }
private:
    std::unique_ptr<Node> returnValue;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span coReturnSpan;
};

class GotoStatementNode : public CompoundNode
{
public:
    GotoStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    GotoStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* target_, Node* attributes_, Node* semicolon_, const soul::ast::Span& gotoSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Target() const noexcept { return target.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semimcolon() const noexcept { return semicolon.get(); }
    inline const soul::ast::Span& GotoSpan() const noexcept { return gotoSpan; }
private:
    std::unique_ptr<Node> target;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
    soul::ast::Span gotoSpan;
};

class TryStatementNode : public CompoundNode
{
public:
    TryStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TryStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* tryBlock_, Node* handlers_, Node* attributes_, const soul::ast::Span& trySpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* TryBlock() const noexcept { return tryBlock.get(); }
    inline Node* Handlers() const noexcept { return handlers.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline const soul::ast::Span& TrySpan() const noexcept { return trySpan; }
private:
    std::unique_ptr<Node> tryBlock;
    std::unique_ptr<Node> handlers;
    std::unique_ptr<Node> attributes;
    soul::ast::Span trySpan;
};

class HandlerSequenceNode : public SequenceNode
{
public:
    HandlerSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class HandlerNode : public CompoundNode
{
public:
    HandlerNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    HandlerNode(const soul::ast::Span& span_, int fileIndex_, Node* exception_, Node* catchBlock_,
        const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Exception() const noexcept { return exception.get(); }
    inline Node* CatchBlock() const noexcept { return catchBlock.get(); }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    std::unique_ptr<Node> exception;
    std::unique_ptr<Node> catchBlock;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class ExceptionDeclarationNode : public CompoundNode
{
public:
    ExceptionDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ExceptionDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* typeSpecifiers_, Node* declarator_, Node* ellipsis_, Node* attributes_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* TypeSpecifiers() const noexcept { return typeSpecifiers.get(); }
    inline Node* Declarator() const noexcept { return declarator.get(); }
    inline Node* Ellipsis() const noexcept { return ellipsis.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
private:
    std::unique_ptr<Node> typeSpecifiers;
    std::unique_ptr<Node> declarator;
    std::unique_ptr<Node> ellipsis;
    std::unique_ptr<Node> attributes;
};

class ExpressionStatementNode : public CompoundNode
{
public:
    ExpressionStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    ExpressionStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* expr_, Node* attributes_, Node* semicolon_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Expression() const noexcept { return expr.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
    inline Node* Semicolon() const noexcept { return semicolon.get(); }
    std::u32string Str() const override;
private:
    std::unique_ptr<Node> expr;
    std::unique_ptr<Node> attributes;
    std::unique_ptr<Node> semicolon;
};

class DeclarationStatementNode : public CompoundNode
{
public:
    DeclarationStatementNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    DeclarationStatementNode(const soul::ast::Span& span_, int fileIndex_, Node* declaration_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Declaration() const noexcept { return declaration.get(); }
    std::u32string Str() const override;
private:
    std::unique_ptr<Node> declaration;
};

class InitConditionNode : public CompoundNode
{
public:
    InitConditionNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    InitConditionNode(const soul::ast::Span& span_, int fileIndex_, Node* declSpecifiers_, Node* declarator_, Node* initializer_, Node* attributes_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* DeclSpecifiers() const noexcept { return declSpecifiers.get(); }
    inline Node* Declarator() const noexcept { return declarator.get(); }
    inline Node* Initializer() const noexcept { return initializer.get(); }
    inline Node* Attributes() const noexcept { return attributes.get(); }
private:
    std::unique_ptr<Node> declSpecifiers;
    std::unique_ptr<Node> declarator;
    std::unique_ptr<Node> initializer;
    std::unique_ptr<Node> attributes;
};

class BoundStatementNode : public Node
{
public:
    BoundStatementNode(void* boundStatementNode_, const soul::ast::Span& span_, int fileIndex_) noexcept;
    inline void* GetBoundStatementNode() const noexcept { return boundStatementNode; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    void* boundStatementNode;
};

} // namespace otava::ast
