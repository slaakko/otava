// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.parser.token;

import util.unicode;
import otava.symbols.exception;

namespace otava::parser::token {

Suffix ParseIntegerSuffix(const char32_t*& p, const char32_t* e)
{
    Suffix suffix = Suffix::none;
    if (p != e && (*p == 'u' || *p == 'U'))
    {
        suffix = suffix | Suffix::u;
        ++p;
    }
    if (p != e && (*p == 'l' || *p == 'L'))
    {
        ++p;
        if (p != e && (*p == 'l' || *p == 'L'))
        {
            suffix = suffix | Suffix::ll;
            ++p;
        }
        else
        {
            suffix = suffix | Suffix::l;
        }
        if (p != e && (*p == 'u' || *p == 'U'))
        {
            suffix = suffix | Suffix::u;
            ++p;
        }
    }
    if (p != e && (*p == 'z' || *p == 'Z'))
    {
        if ((suffix & (Suffix::l | Suffix::ll)) == Suffix::none)
        {
            suffix = suffix | Suffix::z;
            ++p;
            if (p != e && (*p == 'u' || *p == 'U'))
            {
                suffix = suffix | Suffix::u;
                ++p;
            }
        }
    }
    return suffix;
}

Suffix ParseFloatingSuffix(const char32_t*& p, const char32_t* e)
{
    Suffix suffix = Suffix::none;
    if (p != e && (*p == 'l' || *p == 'L'))
    {
        suffix = suffix | Suffix::l;
        ++p;
    }
    else if (p != e && (*p == 'f' || *p == 'F'))
    {
        suffix = suffix | Suffix::f;
        ++p;
    }
    return suffix;
}

EncodingPrefix ParseEncodingPrefix(const char32_t*& p, const char32_t* e)
{
    EncodingPrefix encodingPrefix = EncodingPrefix::none;
    if (p != e && ((*p == 'u') || (*p == 'U') || (*p == 'L')))
    {
        char32_t c = *p;
        ++p;
        if (p != e && *p == '8')
        {
            ++p;
            encodingPrefix = EncodingPrefix::u8;
        }
        else if (c == 'u')
        {
            encodingPrefix = EncodingPrefix::u;
        }
        else if (c == 'U')
        {
            encodingPrefix = EncodingPrefix::U;
        }
        else if (c == 'L')
        {
            encodingPrefix = EncodingPrefix::L;
        }
    }
    return encodingPrefix;
}

bool IsHexChar(char32_t c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

std::uint64_t ParseHexChar(char32_t c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        return 10 + c - 'a';
    }
    else if (c >= 'A' && c <= 'F')
    {
        return 10 + c - 'A';
    }
    return 0;
}

char32_t ParseEscape(const char32_t*& p, const char32_t* e, bool& valid)
{
    char32_t value = '\0';
    valid = true;
    if (p != e)
    {
        char32_t c = *p;
        switch (c)
        {
        case '\'':
        {
            ++p;
            value = '\'';
            break;
        }
        case '\"':
        {
            ++p;
            value = '\"';
            break;
        }
        case '\?':
        {
            ++p;
            value = '\?';
            break;
        }
        case '\\':
        {
            ++p;
            value = '\\';
            break;
        }
        case 'a':
        {
            ++p;
            value = '\a';
            break;
        }
        case 'b':
        {
            ++p;
            value = '\b';
            break;
        }
        case 'f':
        {
            ++p;
            value = '\f';
            break;
        }
        case 'n':
        {
            ++p;
            value = '\n';
            break;
        }
        case 'r':
        {
            ++p;
            value = '\r';
            break;
        }
        case 't':
        {
            ++p;
            value = '\t';
            break;
        }
        case 'v':
        {
            ++p;
            value = '\v';
            break;
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        {
            value = *p - '0';
            ++p;
            if (p != e && *p >= '0' && *p <= '7')
            {
                value = 8 * value + *p - '0';
                ++p;
                if (p != e && *p >= '0' && *p <= '7')
                {
                    value = 8 * value + *p - '0';
                    ++p;
                }
            }
            break;
        }
        case 'x':
        {
            ++p;
            valid = false;
            while (p != e && IsHexChar(*p))
            {
                value = static_cast<char32_t>(16 * value + ParseHexChar(*p));
                ++p;
                valid = true;
            }
            break;
        }
        case 'u':
        {
            ++p;
            for (int i = 0; i < 4; ++i)
            {
                if (p != e && IsHexChar(*p))
                {
                    value = static_cast<char32_t>(16 * value + ParseHexChar(*p));
                }
                else
                {
                    valid = false;
                    break;
                }
                ++p;
            }
            break;
        }
        case 'U':
        {
            ++p;
            for (int i = 0; i < 8; ++i)
            {
                if (p != e && IsHexChar(*p))
                {
                    value = static_cast<char32_t>(16 * value + ParseHexChar(*p));
                }
                else
                {
                    valid = false;
                    break;
                }
                ++p;
            }
            break;
        }
        default:
        {
            value = *p;
            ++p;
            break;
        }
        }
    }
    return value;
}

otava::ast::IntegerLiteralNode* ParseIntegerLiteral(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token)
{
    std::uint64_t value = 0;
    Base base = Base::decimal;
    Suffix suffix = Suffix::none;
    const char32_t* p = token.match.begin;
    const char32_t* e = token.match.end;
    bool firstWasSingleQuote = false;
    bool lastWasSingleQuote = false;
    bool first = true;
    if (p != e && *p == '0')
    {
        ++p;
        if (p != e && (*p == 'b' || *p == 'B'))
        {
            base = Base::binary;
            ++p;
            while (p != e && (*p == '0' || *p == '1' || *p == '\''))
            {
                if (first)
                {
                    first = false;
                    if (*p == '\'')
                    {
                        firstWasSingleQuote = true;
                    }
                }
                if (*p == '0' || *p == '1')
                {
                    std::uint64_t bit = (*p - '0');
                    value = 2 * value + bit;
                    lastWasSingleQuote = false;
                }
                else if (*p == '\'')
                {
                    lastWasSingleQuote = true;
                }
                ++p;
            }
            suffix = ParseIntegerSuffix(p, e);
        }
        else if (p != e && (*p == 'x' || *p == 'X'))
        {
            base = Base::hexadecimal;
            ++p;
            while (p != e && (IsHexChar(*p) || *p == '\''))
            {
                if (first)
                {
                    first = false;
                    if (*p == '\'')
                    {
                        firstWasSingleQuote = true;
                    }
                }
                if (*p != '\'')
                {
                    value = 16 * value + ParseHexChar(*p);
                    lastWasSingleQuote = false;
                }
                else
                {
                    lastWasSingleQuote = true;
                }
                ++p;
            }
            suffix = ParseIntegerSuffix(p, e);
        }
        else
        {
            if (first)
            {
                first = false;
                if (*p == '\'')
                {
                    firstWasSingleQuote = true;
                }
            }
            base = Base::octal;
            while (p != e && ((*p >= '0' && *p <= '7') || *p == '\''))
            {
                if (*p != '\'')
                {
                    value = 8 * value + *p - '0';
                    lastWasSingleQuote = false;
                }
                else
                {
                    lastWasSingleQuote = true;
                }
                ++p;
            }
            suffix = ParseIntegerSuffix(p, e);
        }
    }
    else if (p != e && (*p >= '1' && *p <= '9'))
    {
        if (first)
        {
            first = false;
            if (*p == '\'')
            {
                firstWasSingleQuote = true;
            }
        }
        value = *p - '0';
        ++p;
        while (p != e && ((*p >= '0' && *p <= '9') || *p == '\''))
        {
            if (*p != '\'')
            {
                value = 10 * value + *p - '0';
                lastWasSingleQuote = false;
            }
            else
            {
                lastWasSingleQuote = true;
            }
            ++p;
        }
        suffix = ParseIntegerSuffix(p, e);
    }
    if (firstWasSingleQuote)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid integer literal in '" + fileName + ", literal begins with single quote: " +
            util::ToUtf8(token.ToString()));
    }
    else if (lastWasSingleQuote)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid integer literal in '" + fileName + ", literal ends with single quote: " +
            util::ToUtf8(token.ToString()));
    }
    else if (p != e)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid integer literal in '" + fileName + ": " + util::ToUtf8(token.ToString()));
    }
    return new otava::ast::IntegerLiteralNode(span, file, value, suffix, base, util::ToUtf8(token.ToString()));
}

otava::ast::FloatingLiteralNode* ParseFloatingLiteral(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token)
{
    std::string s;
    double value = 0.0;
    Suffix suffix = Suffix::none;
    Base base = Base::decimal;
    const char32_t* p = token.match.begin;
    const char32_t* e = token.match.end;
    if (p != e && *p == '0')
    {
        s.append(1, static_cast<char>(*p));
        ++p;
        if (p != e && (*p == 'x' || *p == 'X'))
        {
            base = Base::hexadecimal;
            s.append(1, static_cast<char>(*p));
            ++p;
            while (p != e && ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F') || *p == '\''))
            {
                if (*p != '\'')
                {
                    s.append(1, static_cast<char>(*p));
                }
                ++p;
            }
            if (p != e && *p == '.')
            {
                s.append(1, static_cast<char>(*p));
                ++p;
            }
            while (p != e && ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F') || *p == '\''))
            {
                if (*p != '\'')
                {
                    s.append(1, static_cast<char>(*p));
                }
                ++p;
            }
            if (p != e && (*p == 'p' || *p == 'P'))
            {
                s.append(1, static_cast<char>(*p));
                ++p;
            }
            if (p != e && (*p == '-' || *p == '+'))
            {
                s.append(1, static_cast<char>(*p));
                ++p;
            }
            while (p != e && ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F') || *p == '\''))
            {
                if (*p != '\'')
                {
                    s.append(1, static_cast<char>(*p));
                }
                ++p;
            }
        }
    }
    if (base == Base::decimal)
    {
        while (p != e && ((*p >= '0' && *p <= '9') || *p == '\''))
        {
            if (*p != '\'')
            {
                s.append(1, static_cast<char>(*p));
            }
            ++p;
        }
        if (p != e && *p == '.')
        {
            s.append(1, static_cast<char>(*p));
            ++p;
        }
        while (p != e && ((*p >= '0' && *p <= '9') || *p == '\''))
        {
            if (*p != '\'')
            {
                s.append(1, static_cast<char>(*p));
            }
            ++p;
        }
        if (p != e && (*p == 'e' || *p == 'E'))
        {
            s.append(1, static_cast<char>(*p));
            ++p;
        }
        if (p != e && (*p == '-' || *p == '+'))
        {
            s.append(1, static_cast<char>(*p));
            ++p;
        }
        while (p != e && ((*p >= '0' && *p <= '9') || *p == '\''))
        {
            if (*p != '\'')
            {
                s.append(1, static_cast<char>(*p));
            }
            ++p;
        }
    }
    value = std::strtod(s.c_str(), nullptr);
    suffix = ParseFloatingSuffix(p, e);
    if (p != e)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid floating literal in '" + fileName + ": " + util::ToUtf8(token.ToString()));
    }
    return new otava::ast::FloatingLiteralNode(span, file, value, suffix, base, util::ToUtf8(token.ToString()));
}

otava::ast::CharacterLiteralNode* ParseCharacterLiteral(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token)
{
    bool valid = false;
    bool hasMultipleCharacters = false;
    char32_t value = '\0';
    const char32_t* p = token.match.begin;
    const char32_t* e = token.match.end;
    EncodingPrefix encodingPrefix = ParseEncodingPrefix(p, e);
    bool valueParsed = false;
    if (p != e && '\'')
    {
        ++p;
        while (p != e && *p != '\'')
        {
            if (p != e && *p == '\\')
            {
                ++p;
                bool escapeValid = false;
                value = ParseEscape(p, e, escapeValid);
                if (escapeValid)
                {
                    valueParsed = true;
                }
            }
            else if (p != e && *p != '\'' && *p != '\\' && *p != '\n')
            {
                if (value == '\0')
                {
                    value = *p;
                    valueParsed = true;
                }
                else
                {
                    hasMultipleCharacters = true;
                }
                ++p;
            }
        }
        if (p != e && valueParsed && *p == '\'')
        {
            ++p;
            valid = true;
        }
    }
    if (p != e || !valid)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid character literal in '" + fileName + ": " + util::ToUtf8(token.ToString()));
    }
    return new otava::ast::CharacterLiteralNode(span, file, value, encodingPrefix, util::ToUtf8(token.ToString()), hasMultipleCharacters);
}

otava::ast::RawStringLiteralNode* ParseRawStringLiteral(const soul::ast::Span& span, int file, soul::lexer::LexerBase<char32_t>& lexer)
{
    auto token = lexer.GetToken(lexer.GetPos());
    const char32_t* start = token.match.begin;
    const char32_t* p = start;
    const char32_t* e = lexer.End();
    bool valid = false;
    bool stop = false;
    std::u32string value;
    std::u32string rep;
    EncodingPrefix encodingPrefix = ParseEncodingPrefix(p, e);
    rep.append(start, p);
    int state = 0;
    int index = 0;
    std::u32string dm;
    std::u32string tmp;
    while (p != e && !stop && !valid)
    {
        switch (state)
        {
        case 0:
        {
            if (*p == 'R')
            {
                state = 1;
            }
            else
            {
                stop = true;
            }
            break;
        }
        case 1:
        {
            if (*p == '"')
            {
                state = 2;
            }
            else
            {
                stop = true;
            }
            break;
        }
        case 2:
        {
            switch (*p)
            {
            case ' ': case ')': case '\\': case '\t': case '\v': case '\f': case '\n':
            {
                stop = true;
                break;
            }
            case '(':
            {
                dm.append(1, '"');
                state = 3;
                break;
            }
            default:
            {
                dm.append(1, *p);
                break;
            }
            }
            break;
        }
        case 3:
        {
            switch (*p)
            {
            case ')':
            {
                tmp = U")";
                index = 0;
                state = 4;
                break;
            }
            case '\n':
            {
                value.append(1, *p);
                lexer.SetLine(lexer.Line() + 1);
                break;
            }
            default:
            {
                value.append(1, *p);
                break;
            }
            }
            break;
        }
        case 4:
        {
            if (index == dm.length())
            {
                valid = true;
            }
            else if (*p == dm[index])
            {
                tmp.append(1, *p);
                ++index;
            }
            else
            {
                value.append(tmp);
                switch (*p)
                {
                case ')':
                {
                    tmp = U")";
                    index = 0;
                    break;
                }
                default:
                {
                    value.append(1, *p);
                    state = 3;
                    break;
                }
                }
            }
            break;
        }
        }
        if (!valid)
        {
            rep.append(1, *p);
            ++p;
        }
    }
    if (state == 4 && index == dm.length())
    {
        valid = true;
    }
    if (valid)
    {
        lexer.CurrentToken().match.end = p;
        lexer.CurrentLexeme().end = p;
        lexer.SetCurrentMatchEnd(p);
        lexer.EraseTail();
        lexer.SetPos(p);
        lexer.Increment();
    }
    else
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid raw string literal in '" + lexer.FileName() + "' at line " + std::to_string(lexer.Line()));
    }
    return new otava::ast::RawStringLiteralNode(span, file, util::ToUtf8(value), encodingPrefix, util::ToUtf8(dm), util::ToUtf8(rep));
}

otava::ast::StringLiteralNode* ParseStringLiteral(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token)
{
    bool valid = false;
    std::u32string value;
    const char32_t* p = token.match.begin;
    const char32_t* e = token.match.end;
    EncodingPrefix encodingPrefix = ParseEncodingPrefix(p, e);
    if (p != e && *p == '"')
    {
        ++p;
        while (p != e)
        {
            if (*p == '\\')
            {
                ++p;
                bool escapeValid = false;
                char32_t escapeValue = ParseEscape(p, e, escapeValid);
                if (escapeValid)
                {
                    value.append(1, escapeValue);
                }
                else
                {
                    break;
                }
            }
            else if (*p != '"' && *p != '\\' && *p != '\n')
            {
                value.append(1, *p);
                ++p;
            }
            else
            {
                break;
            }
        }
        if (p != e && *p == '"')
        {
            ++p;
            valid = true;
        }
    }
    if (p != e || !valid)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid string literal in '" + fileName + ": " + util::ToUtf8(token.ToString()));
    }
    return new otava::ast::StringLiteralNode(span, file, util::ToUtf8(value), encodingPrefix, util::ToUtf8(token.ToString()));
}

otava::ast::IdentifierNode* ParseIdentifier(const soul::ast::Span& span, int file, const std::string& fileName,
    const soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>& token)
{
    std::u32string s;
    const char32_t* p = token.match.begin;
    const char32_t* e = token.match.end;
    bool valid = true;
    while (p != e && valid)
    {
        char32_t value = '\0';
        if (*p == '\\')
        {
            ++p;
            if (p != e)
            {
                if (*p == 'u')
                {
                    ++p;
                    for (int i = 0; i < 4; ++i)
                    {
                        if (p != e && IsHexChar(*p))
                        {
                            value = static_cast<char32_t>(16 * value + ParseHexChar(*p));
                        }
                        else
                        {
                            valid = false;
                            break;
                        }
                        ++p;
                    }
                }
                else if (*p == 'U')
                {
                    ++p;
                    for (int i = 0; i < 8; ++i)
                    {
                        if (p != e && IsHexChar(*p))
                        {
                            value = static_cast<char32_t>(16 * value + ParseHexChar(*p));
                        }
                        else
                        {
                            valid = false;
                            break;
                        }
                        ++p;
                    }
                }
            }
        }
        else
        {
            value = *p;
            ++p;
        }
        s.append(1, value);
    }
    if (p != e || !valid)
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid identifier in '" + fileName + ": " + util::ToUtf8(token.ToString()));
    }
    return new otava::ast::IdentifierNode(span, file, util::ToUtf8(s));
}

} // namespace otava::parser::token
