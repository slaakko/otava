// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.literal;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

LiteralNode::LiteralNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_) noexcept : Node(kind_, span_, fileIndex_), rep()
{
}

LiteralNode::LiteralNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_, const std::u32string& rep_) : Node(kind_, span_, fileIndex_), rep(rep_)
{
}

void LiteralNode::Write(Writer& writer)
{
    Node::Write(writer);
    writer.Write(rep);
}

void LiteralNode::Read(Reader& reader)
{
    Node::Read(reader);
    rep = reader.ReadStr();
}

IntegerLiteralNode::IntegerLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    LiteralNode(NodeKind::integerLiteralNode, span_, fileIndex_), value(), suffix(), base()
{
}

IntegerLiteralNode::IntegerLiteralNode(const soul::ast::Span& span_, int fileIndex_, std::uint64_t value_, Suffix suffix_, Base base_, const std::u32string& rep_) :
    LiteralNode(NodeKind::integerLiteralNode, span_, fileIndex_, rep_), value(value_), suffix(suffix_), base(base_)
{
}

Node* IntegerLiteralNode::Clone() const
{
    IntegerLiteralNode* clone = new IntegerLiteralNode(GetSpan(), FileIndex(), value, suffix, base, Rep());
    clone->SetId(Id());
    return clone;
}

void IntegerLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IntegerLiteralNode::Write(Writer& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
    writer.GetBinaryStreamWriter().Write(static_cast<std::uint8_t>(suffix));
    writer.GetBinaryStreamWriter().Write(static_cast<std::uint8_t>(base));
}

void IntegerLiteralNode::Read(Reader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadULong();
    suffix = static_cast<Suffix>(reader.GetBinaryStreamReader().ReadByte());
    base = static_cast<Base>(reader.GetBinaryStreamReader().ReadByte());
}

FloatingLiteralNode::FloatingLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    LiteralNode(NodeKind::floatingLiteralNode, span_, fileIndex_), value(), suffix(), base()
{
}

FloatingLiteralNode::FloatingLiteralNode(const soul::ast::Span& span_, int fileIndex_, double value_, Suffix suffix_, Base base_, const std::u32string& rep_) :
    LiteralNode(NodeKind::floatingLiteralNode, span_, fileIndex_, rep_), value(value_), suffix(suffix_), base(base_)
{
}

Node* FloatingLiteralNode::Clone() const
{
    FloatingLiteralNode* clone = new FloatingLiteralNode(GetSpan(), FileIndex(), value, suffix, base, Rep());
    clone->SetId(Id());
    return clone;
}

void FloatingLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FloatingLiteralNode::Write(Writer& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
    writer.GetBinaryStreamWriter().Write(static_cast<std::uint8_t>(suffix));
    writer.GetBinaryStreamWriter().Write(static_cast<std::uint8_t>(base));
}

void FloatingLiteralNode::Read(Reader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadDouble();
    suffix = static_cast<Suffix>(reader.GetBinaryStreamReader().ReadByte());
    base = static_cast<Base>(reader.GetBinaryStreamReader().ReadByte());
}

CharacterLiteralNode::CharacterLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    LiteralNode(NodeKind::characterLiteralNode, span_, fileIndex_), value(), encodingPrefix(), hasMultipleCharacters(false)
{
}

CharacterLiteralNode::CharacterLiteralNode(const soul::ast::Span& span_, int fileIndex_, char32_t value_, EncodingPrefix encodingPrefix_,
    const std::u32string& rep_, bool hasMultipleCharacters_) :
    LiteralNode(NodeKind::characterLiteralNode, span_, fileIndex_, rep_), value(value_), encodingPrefix(encodingPrefix_), hasMultipleCharacters(hasMultipleCharacters_)
{
}

Node* CharacterLiteralNode::Clone() const
{
    CharacterLiteralNode* clone = new CharacterLiteralNode(GetSpan(), FileIndex(), value, encodingPrefix, Rep(), hasMultipleCharacters);
    clone->SetId(Id());
    return clone;
}

void CharacterLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CharacterLiteralNode::Write(Writer& writer)
{
    LiteralNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(value);
    writer.GetBinaryStreamWriter().Write(static_cast<std::uint8_t>(encodingPrefix));
    writer.GetBinaryStreamWriter().Write(hasMultipleCharacters);
}

void CharacterLiteralNode::Read(Reader& reader)
{
    LiteralNode::Read(reader);
    value = reader.GetBinaryStreamReader().ReadUChar();
    encodingPrefix = static_cast<EncodingPrefix>(reader.GetBinaryStreamReader().ReadByte());
    hasMultipleCharacters = reader.GetBinaryStreamReader().ReadBool();
}

StringLiteralNode::StringLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    LiteralNode(NodeKind::stringLiteralNode, span_, fileIndex_), value(), encodingPrefix()
{
}

StringLiteralNode::StringLiteralNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_) : 
    LiteralNode(kind_, span_, fileIndex_), value(), encodingPrefix()
{
}

StringLiteralNode::StringLiteralNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& value_, EncodingPrefix encodingPrefix_, 
    const std::u32string& rep_) :
    LiteralNode(NodeKind::stringLiteralNode, span_, fileIndex_, rep_), value(value_), encodingPrefix(encodingPrefix_)
{
}

StringLiteralNode::StringLiteralNode(NodeKind kind_, const soul::ast::Span& span_, int fileIndex_, const std::u32string& value_, EncodingPrefix encodingPrefix_,
    const std::u32string& rep_) :
    LiteralNode(kind_, span_, fileIndex_, rep_), value(value_), encodingPrefix(encodingPrefix_)
{
}

Node* StringLiteralNode::Clone() const
{
    StringLiteralNode* clone = new StringLiteralNode(GetSpan(), FileIndex(), value, encodingPrefix, Rep());
    clone->SetId(Id());
    return clone;
}

void StringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StringLiteralNode::Write(Writer& writer)
{
    LiteralNode::Write(writer);
    writer.Write(value);
    writer.GetBinaryStreamWriter().Write(static_cast<std::uint8_t>(encodingPrefix));
}

void StringLiteralNode::Read(Reader& reader)
{
    LiteralNode::Read(reader);
    value = reader.ReadStr();
    encodingPrefix = static_cast<EncodingPrefix>(reader.GetBinaryStreamReader().ReadByte());
}

RawStringLiteralNode::RawStringLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept : StringLiteralNode(NodeKind::rawStringLiteralNode, span_, fileIndex_)
{
}

RawStringLiteralNode::RawStringLiteralNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& value_, EncodingPrefix encodingPrefix_,
    const std::u32string& delimSequence_, const std::u32string& rep_) :
    StringLiteralNode(NodeKind::rawStringLiteralNode, span_, fileIndex_, value_, encodingPrefix_, rep_), delimSequence(delimSequence_)
{
}

Node* RawStringLiteralNode::Clone() const
{
    RawStringLiteralNode* clone = new RawStringLiteralNode(GetSpan(), FileIndex(), GetValue(), EncodingPrefix(), delimSequence, Rep());
    clone->SetId(Id());
    return clone;
}

void RawStringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RawStringLiteralNode::Write(Writer& writer)
{
    StringLiteralNode::Write(writer);
    writer.Write(delimSequence);
}

void RawStringLiteralNode::Read(Reader& reader)
{
    StringLiteralNode::Read(reader);
    delimSequence = reader.ReadStr();
}

BooleanLiteralNode::BooleanLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept : LiteralNode(NodeKind::booleanLiteralNode, span_, fileIndex_), value()
{
}

BooleanLiteralNode::BooleanLiteralNode(const soul::ast::Span& span_, int fileIndex_, bool value_, const std::u32string& rep_) :
    LiteralNode(NodeKind::booleanLiteralNode, span_, fileIndex_, rep_), value(value_)
{
}

Node* BooleanLiteralNode::Clone() const
{
    BooleanLiteralNode* clone = new BooleanLiteralNode(GetSpan(), FileIndex(), value, Rep());
    clone->SetId(Id());
    return clone;
}

void BooleanLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BooleanLiteralNode::Write(Writer& writer)
{
    LiteralNode::Write(writer);
    writer.Write(value);
}

void BooleanLiteralNode::Read(Reader& reader)
{
    LiteralNode::Read(reader);
    value = reader.ReadBool();
}

NullPtrLiteralNode::NullPtrLiteralNode(const soul::ast::Span& span_, int fileIndex_) noexcept : LiteralNode(NodeKind::nullPtrLiteralNode, span_, fileIndex_)
{
}

NullPtrLiteralNode::NullPtrLiteralNode(const soul::ast::Span& span_, int fileIndex_, const std::u32string& rep_) :
    LiteralNode(NodeKind::nullPtrLiteralNode, span_, fileIndex_, rep_)
{
}

Node* NullPtrLiteralNode::Clone() const
{
    NullPtrLiteralNode* clone = new NullPtrLiteralNode(GetSpan(), FileIndex(), Rep());
    clone->SetId(Id());
    return clone;
}

void NullPtrLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UserDefinedLiteraNode::UserDefinedLiteraNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::userDefinedLiteralNode, span_, fileIndex_, nullptr, nullptr)
{
}

UserDefinedLiteraNode::UserDefinedLiteraNode(const soul::ast::Span& span_, int fileIndex_, Node* literalNode_, Node* udSuffix_) noexcept :
    BinaryNode(NodeKind::userDefinedLiteralNode, span_, fileIndex_, literalNode_, udSuffix_)
{
}

Node* UserDefinedLiteraNode::Clone() const
{
    UserDefinedLiteraNode* clone = new UserDefinedLiteraNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void UserDefinedLiteraNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LiteralOperatorIdNode::LiteralOperatorIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    UnaryNode(NodeKind::literalOperatorIdNode, span_, fileIndex_, nullptr)
{
}

LiteralOperatorIdNode::LiteralOperatorIdNode(const soul::ast::Span& span_, int fileIndex_, Node* id_, const soul::ast::Span& stringLitSpan_) noexcept :
    UnaryNode(NodeKind::literalOperatorIdNode, span_, fileIndex_, id_), stringLitSpan(stringLitSpan_)
{
}

Node* LiteralOperatorIdNode::Clone() const
{
    LiteralOperatorIdNode* clone = new LiteralOperatorIdNode(GetSpan(), FileIndex(), Child()->Clone(), stringLitSpan);
    clone->SetId(Id());
    return clone;
}

void LiteralOperatorIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LiteralOperatorIdNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(stringLitSpan);
}

void LiteralOperatorIdNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    stringLitSpan = reader.ReadSpan();
}

std::u32string EncodingPrefixStr(EncodingPrefix encodingPrefix)
{
    switch (encodingPrefix)
    {
        case EncodingPrefix::none: return std::u32string();
        case EncodingPrefix::u8: return std::u32string(U"u8");
        case EncodingPrefix::u: return std::u32string(U"u");
        case EncodingPrefix::U: return std::u32string(U"U");
        case EncodingPrefix::L: return std::u32string(U"L");
    }
    return std::u32string();
}

EncodingPrefix CommonEncodingPrefix(otava::ast::EncodingPrefix leftEncodingPrefix, otava::ast::EncodingPrefix rightEncodingPrefix) noexcept
{
    switch (leftEncodingPrefix)
    {
        case EncodingPrefix::none:
        {
            switch (rightEncodingPrefix)
            {
                case EncodingPrefix::none:
                {
                    return EncodingPrefix::none;
                }
                default:
                {
                    return rightEncodingPrefix;
                }
            }
            break;
        }
        default:
        {
            switch (rightEncodingPrefix)
            {
                case EncodingPrefix::none:
                {
                    return leftEncodingPrefix;
                }
                default:
                {
                    return EncodingPrefix::u8;
                }
            }
            break;
        }
    }
    return EncodingPrefix::u8;
}

} // namespace otava::ast
