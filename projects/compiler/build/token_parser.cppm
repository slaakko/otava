// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.build.token_parser;

import std;
import soul.lexer.base;
import soul.lexer.token;
import util.unicode;
import otava.ast.error;

export namespace otava::token_parser {

template<typename Char>
std::string ParseFilePath(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token)
{
    std::u32string strValue;
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    if (p != e && *p == '<')
    {
        ++p;
    }
    while (p != e && *p != '>')
    {
        strValue.append(1, *p);
        ++p;
    }
    if (p != e && *p == '>')
    {
        ++p;
    }
    if (p != e)
    {
        otava::ast::SetExceptionThrown();
        throw std::runtime_error("invalid file path at " + fileName + ":" + std::to_string(token.line) + " : '" + util::ToUtf8(token.match.ToString()) + "'");
    }
    return util::ToUtf8(strValue);
}

} // namespace otava::token_parser
