// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.parser.token;

import std;
import soul.ast.span;
import otava.ast.identifier;
import otava.ast.literal;
import soul.lexer.token;
import soul.lexer.base;

export namespace otava::parser::token {

using Suffix = otava::ast::Suffix;
using EncodingPrefix = otava::ast::EncodingPrefix;
using Base = otava::ast::Base;

Suffix ParseIntegerSuffix(const char32_t*& p, const char32_t* e);
Suffix ParseFloatingSuffix(const char32_t*& p, const char32_t* e);
EncodingPrefix ParseEncodingPrefix(const char32_t*& p, const char32_t* e);
bool IsHexChar(char32_t c);
std::uint64_t ParseHexChar(char32_t c);
char32_t ParseEscape(const char32_t*& p, const char32_t* e, bool& valid);
otava::ast::IntegerLiteralNode* ParseIntegerLiteral(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token);
otava::ast::FloatingLiteralNode* ParseFloatingLiteral(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token);
otava::ast::CharacterLiteralNode* ParseCharacterLiteral(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token);
otava::ast::RawStringLiteralNode* ParseRawStringLiteral(const soul::ast::Span& span, int file, soul::lexer::LexerBase<char32_t>& lexer);
otava::ast::StringLiteralNode* ParseStringLiteral(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token);
otava::ast::IdentifierNode* ParseIdentifier(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token);

} // namespace otava::parser::token
