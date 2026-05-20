// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.lexing.util;

import std;
import soul.lexer.token;
import soul.lexer.base;

export namespace soul::lexer {

template<typename Char>
inline bool NoWhiteSpaceBetweenTokens(const soul::lexer::Token<Char, LexerBase<Char>>& left, const soul::lexer::Token<Char, LexerBase<Char>>& right) noexcept
{
    if (left.match.end == right.match.begin) return true;
    return false;
}

} // namespace soul::lexer
