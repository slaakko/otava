// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.attribute;

import std;
import otava.ast.node;

export namespace otava::ast {

class AttributeSpecifierSequenceNode : public SequenceNode
{
public:
    AttributeSpecifierSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class AttributeSpecifierNode : public ListNode
{
public:
    AttributeSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline void SetUsingPrefix(Node* usingPrefix_) noexcept { usingPrefix.reset(usingPrefix_); }
    inline Node* UsingPrefix() const noexcept { return usingPrefix.get(); }
    void SetBracketSpans(const soul::ast::Span& lbSpan1_, const soul::ast::Span& lbSpan2_, const soul::ast::Span& rbSpan1_,
        const soul::ast::Span& rbSpan2_) noexcept;
    inline const soul::ast::Span& LBracketSpan1() const noexcept { return lbSpan1; }
    inline const soul::ast::Span& LBracketSpan2() const noexcept { return lbSpan2; }
    inline const soul::ast::Span& RBracketSpan1() const noexcept { return rbSpan1; }
    inline const soul::ast::Span& RBracketSpan2() const noexcept { return rbSpan2; }
private:
    std::unique_ptr<Node> usingPrefix;
    soul::ast::Span lbSpan1;
    soul::ast::Span lbSpan2;
    soul::ast::Span rbSpan1;
    soul::ast::Span rbSpan2;
};

class AttributeUsingPrefixNode : public UnaryNode
{
public:
    AttributeUsingPrefixNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AttributeUsingPrefixNode(const soul::ast::Span& span_, int fileIndex_, Node* attributeNamespace_, const soul::ast::Span& colonSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& ColonSpan() const noexcept { return colonSpan; }
private:
    soul::ast::Span colonSpan;
};

class AttributeNode : public CompoundNode
{
public:
    AttributeNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AttributeNode(const soul::ast::Span& span_, int fileIndex_, Node* attributeToken_, Node* attributeArgs_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* AttributeToken() const noexcept { return attributeToken.get(); }
    inline Node* AttributeArgs() const noexcept { return attributeArgs.get(); }
private:
    std::unique_ptr<Node> attributeToken;
    std::unique_ptr<Node> attributeArgs;
};

class AttributeScopedTokenNode : public CompoundNode
{
public:
    AttributeScopedTokenNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AttributeScopedTokenNode(const soul::ast::Span& span_, int fileIndex_, Node* ns_, Node* colonColon_, Node* identifier_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Ns() const noexcept { return ns.get(); }
    inline Node* ColonColon() const noexcept { return colonColon.get(); }
    inline Node* Identifier() const noexcept { return identifier.get(); }
private:
    std::unique_ptr<Node> ns;
    std::unique_ptr<Node> colonColon;
    std::unique_ptr<Node> identifier;
};

class AttributeArgumentsNode : public CompoundNode
{
public:
    AttributeArgumentsNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AttributeArgumentsNode(const soul::ast::Span& span_, int fileIndex_, Node* balancedTokenSequence_, const soul::ast::Span& lpSpan_,
        const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* BalancedTokenSequence() const noexcept { return balancedTokenSequence.get(); }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    std::unique_ptr<Node> balancedTokenSequence;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

class BalancedTokenSequenceNode : public SequenceNode
{
public:
    BalancedTokenSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class TokenNode : public CompoundNode
{
public:
    TokenNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    TokenNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& str_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::u32string Str() const override { return str; }
private:
    std::u32string str;
};

class AlignmentSpecifierNode : public CompoundNode
{
public:
    AlignmentSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    AlignmentSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* alignment_, Node* ellipsis_, const soul::ast::Span& lpSpan_, 
        const soul::ast::Span& rpSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline Node* Alignment() const noexcept { return alignment.get(); }
    inline Node* Ellipsis() const noexcept { return ellipsis.get(); }
    inline const soul::ast::Span& LParenSpan() const noexcept { return lpSpan; }
    inline const soul::ast::Span& RParenSpan() const noexcept { return rpSpan; }
private:
    std::unique_ptr<Node> alignment;
    std::unique_ptr<Node> ellipsis;
    soul::ast::Span lpSpan;
    soul::ast::Span rpSpan;
};

} // namespace otava::ast
