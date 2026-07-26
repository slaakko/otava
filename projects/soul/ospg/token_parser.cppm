export module soul.spg.token_parser;

import util;
import std;
import soul.lexer.token;
import soul.lexer.base;
import soul.ast.spg;
import soul.ast.source_pos;
import soul.common.token.parser;

export namespace soul::spg::token::parser {

template<typename Char>
soul::ast::spg::Parser* ParseStringLiteralOrCharSet(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token, const soul::ast::SourcePos& sourcePos)
{
    std::u32string stringLiteral;
    std::unique_ptr<soul::ast::spg::CharSet> set;
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    if (p != e && *p == '"')
    {
        ++p;
        if (p != e && *p == '[')
        {
            set.reset(new soul::ast::spg::CharSet());
            ++p;
            if (p != e && *p == '^')
            {
                set->SetInverse();
                ++p;
            }
            while (p != e && *p != ']')
            {
                char32_t first = *p;
                char32_t last = first;
                if (*p == '\\')
                {
                    ++p;
                    first = soul::common::token::parser::ParseEscape(fileName, p, e, token);
                    last = first;
                }
                else
                {
                    ++p;
                }
                if (p != e && *p != ']')
                {
                    if (*p == '-')
                    {
                        ++p;
                        if (p != e && *p != ']')
                        {
                            last = *p;
                            if (*p == '\\')
                            {
                                ++p;
                                last = soul::common::token::parser::ParseEscape(fileName, p, e, token);
                            }
                            else
                            {
                                ++p;
                            }
                        }
                        else
                        {
                            soul::ast::spg::Range range;
                            range.first = first;
                            range.last = first;
                            set->AddRange(range);
                            first = '-';
                            last = '-';
                        }
                    }
                }
                soul::ast::spg::Range range;
                range.first = first;
                range.last = last;
                set->AddRange(range);
            }
            if (p != e && *p == ']')
            {
                ++p;
            }
            if (p != e && *p == '"')
            {
                ++p;
            }
            if (p != e)
            {
                throw std::runtime_error("invalid character set literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.match.ToString()));
            }
            return new soul::ast::spg::CharSetParser(sourcePos, set.release());
        }
        else
        {
            while (p != e && *p != '\r' && *p != '\n' && *p != '"')
            {
                if (*p == '\\')
                {
                    ++p;
                    stringLiteral.append(1, soul::common::token::parser::ParseEscape(fileName, p, e, token));
                }
                else
                {
                    stringLiteral.append(1, *p);
                    ++p;
                }
            }
            if (p != e && *p == '"')
            {
                ++p;
            }
            if (p != e)
            {
                throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.match.ToString()));
            }
        }
    }
    else
    {
        throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.match.ToString()));
    }
    return new soul::ast::spg::StringParser(sourcePos, stringLiteral);
}

} // namespace soul::spg::token::parser
