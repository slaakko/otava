// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.literal;

import std;
import otava.ast.node;

export namespace otava::ast {

enum class Suffix : std::uint8_t
{
    none = 0,
    u = 1 << 0,
    l = 1 << 1,
    ll = 1 << 2,
    z = 1 << 3,
    f = 1 << 4
};

inline Suffix operator|(Suffix left, Suffix right) noexcept
{
    return Suffix(std::uint8_t(left) | std::uint8_t(right));
}

inline Suffix operator&(Suffix left, Suffix right) noexcept
{
    return Suffix(std::uint8_t(left) & std::uint8_t(right));
}

inline Suffix operator~(Suffix suffix) noexcept
{
    return Suffix(~std::uint8_t(suffix));
}

enum class Base : std::uint8_t
{
    binary, octal, decimal, hexadecimal
};

enum class EncodingPrefix : std::uint8_t
{
    none, u8, u, U, L
};

class LiteralNode : public Node
{
public:
    LiteralNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_) noexcept;
    LiteralNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_, const std::u32string& rep_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const std::u32string& Rep() const noexcept { return rep; }
    std::u32string Str() const override { return rep; }
private:
    std::u32string rep;
};

class IntegerLiteralNode : public LiteralNode
{
public:
    IntegerLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    IntegerLiteralNode(const soul::ast::Span& span_, int fileIndex_, std::uint64_t value_, Suffix suffix_, Base base_, const std::u32string& rep_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline std::uint64_t GetValue() const noexcept { return value; }
    inline void SetValue(std::uint64_t value_) noexcept { value = value_; }
    inline Suffix GetSuffix() const noexcept { return suffix; }
    inline Base GetBase() const noexcept { return base; }
private:
    std::uint64_t value;
    Suffix suffix;
    Base base;
};

class FloatingLiteralNode : public LiteralNode
{
public:
    FloatingLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    FloatingLiteralNode(const soul::ast::Span& span_, int fileIndex_, double value_, Suffix suffix_, Base base_, const std::u32string& rep_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline double GetValue() const noexcept { return value; }
    inline Suffix GetSuffix() const noexcept { return suffix; }
    inline Base GetBase() const noexcept { return base; }
private:
    double value;
    Suffix suffix;
    Base base;
};

class CharacterLiteralNode : public LiteralNode
{
public:
    CharacterLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    CharacterLiteralNode(const soul::ast::Span& span_, int fileIndex_, char32_t value_, EncodingPrefix encodingPrefix_, const std::u32string& rep_, 
        bool hasMultipleCharacters_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline char32_t GetValue() const noexcept { return value; }
    inline EncodingPrefix GetEncodingPrefix() const noexcept { return encodingPrefix; }
    inline bool HasMultipleCharacters() const noexcept { return hasMultipleCharacters; }
private:
    char32_t value;
    EncodingPrefix encodingPrefix;
    bool hasMultipleCharacters;
};

class StringLiteralNode : public LiteralNode
{
public:
    StringLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    StringLiteralNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_);
    StringLiteralNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& value_, EncodingPrefix encodingPrefix_, const std::u32string& rep_);
    StringLiteralNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_, const std::u32string& value_, EncodingPrefix encodingPrefix_, 
        const std::u32string& rep_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const std::u32string& GetValue() const noexcept { return value; }
    inline EncodingPrefix GetEncodingPrefix() const noexcept { return encodingPrefix; }
private:
    std::u32string value;
    EncodingPrefix encodingPrefix;
};

class RawStringLiteralNode : public StringLiteralNode
{
public:
    RawStringLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    RawStringLiteralNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& value_, EncodingPrefix encodingPrefix_,
        const std::u32string& delimSequence_, const std::u32string& rep_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const std::u32string& DelimSequence() const noexcept { return delimSequence; }
private:
    std::u32string delimSequence;
};

class BooleanLiteralNode : public LiteralNode
{
public:
    BooleanLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    BooleanLiteralNode(const soul::ast::Span& span_, int fileIndex_, bool value_, const std::u32string& rep_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline bool GetValue() const noexcept { return value; }
private:
    bool value;
};

class NullPtrLiteralNode : public LiteralNode
{
public:
    NullPtrLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    NullPtrLiteralNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& rep_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class UserDefinedLiteraNode : public BinaryNode
{
public:
    UserDefinedLiteraNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    UserDefinedLiteraNode(const soul::ast::Span& span_, int fileIndex_, Node* literalNode_, Node* udSuffix_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class LiteralOperatorIdNode : public UnaryNode
{
public:
    LiteralOperatorIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept;
    LiteralOperatorIdNode(const soul::ast::Span& span_, int fileIndex_, Node* id_, const soul::ast::Span& stringLitSpan_) noexcept;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    inline const soul::ast::Span& StringLitSpan() const noexcept { return stringLitSpan; }
private:
    soul::ast::Span stringLitSpan;
};

std::u32string EncodingPrefixStr(EncodingPrefix encodingPrefix);
EncodingPrefix CommonEncodingPrefix(otava::ast::EncodingPrefix leftEncodingPrefix, otava::ast::EncodingPrefix rightEncodingPrefix) noexcept;

} // namespace otava::ast
