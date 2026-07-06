export module soul.lexer.lexing_util;

import std;
import soul.lexer.token;
import soul.lexer.base;
import soul.lexer.classmap;
import soul.lexer.keyword;
import soul.lexer.lexeme;

export namespace soul::lexer {

template<typename Char>
inline bool NoWhiteSpaceBetweenTokens(const soul::lexer::Token<Char, LexerBase<Char>>& left, const soul::lexer::Token<Char, LexerBase<Char>>& right) noexcept
{
    if (left.match.end == right.match.begin) return true;
    return false;
}

} // namespace soul::lexer
