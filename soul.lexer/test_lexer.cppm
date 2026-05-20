// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.test;

import std;
import util;
import soul.lexer.base;
import soul.lexer.token;
import soul.ast.slg;
import soul.ast.common;

export namespace soul::lexer {

template<typename Lexer>
void TestLexer(Lexer& lexer, util::CodeFormatter& formatter)
{
    soul::ast::common::TokenCollection* tokens = lexer.GetTokenCollection();
    ++lexer;
    while (*lexer != soul::lexer::END_TOKEN && *lexer != soul::lexer::INVALID_TOKEN)
    {
        auto token = lexer.CurrentToken();
        auto tokenInfo = tokens->GetToken(token.id);
        if (tokenInfo)
        {
            formatter.WriteLine(tokenInfo->Name() + "[" + util::ToUtf8(token.ToString()) + "]");
        }
        else
        {
            formatter.WriteLine("<unknown token>");
        }
        ++lexer;
    }
    if (*lexer == soul::lexer::END_TOKEN)
    {
        formatter.WriteLine("success");
    }
    else
    {
        formatter.WriteLine("failure");
    }
}

} // namespace soul::lexer
