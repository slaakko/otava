// this file has been automatically generated from 'D:/work/otava/lexer/otava.token.line.lexer' using soul lexer generator slg version 5.2.0

export module otava.token.line.lexer;

import std;
import soul.lexer;
import soul.ast.slg;
import soul.ast.common;
import util;
import otava.token;

export namespace otava::token::line::lexer {

enum class Tag
{
    tag
};

std::mutex& MakeLexerMtx();

template<typename Char>
struct OtavaTokenLineLexer;

template<typename Char>
soul::lexer::Lexer<OtavaTokenLineLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName);

template<typename Char>
soul::lexer::Lexer<OtavaTokenLineLexer<Char>, Char> MakeLexer(const std::string& moduleFileName, util::ResourceFlags resourceFlags, const Char* start, const Char* end, const std::string& fileName);

soul::ast::common::TokenCollection* GetTokens(otava::token::line::lexer::Tag tag);

struct OtavaTokenLineLexer_Variables : public soul::lexer::Variables
{
    OtavaTokenLineLexer_Variables();
    int langleCount;
};

template<typename Char>
struct OtavaTokenLineLexer
{
    using Variables = OtavaTokenLineLexer_Variables;

    static std::int32_t NextState(std::int32_t state, Char chr, soul::lexer::LexerBase<Char>& lexer)
    {
        soul::lexer::ClassMap<Char>* classmap = lexer.GetClassMap();
        std::int32_t cls = classmap->GetClass(chr);
        switch (state)
        {
            case 0:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
                    }
                    case 2:
                    {
                        return 3;
                    }
                    case 3:
                    case 4:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 22:
                    case 23:
                    case 24:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 40:
                    case 42:
                    case 43:
                    case 44:
                    {
                        return 4;
                    }
                    case 7:
                    case 8:
                    {
                        return 5;
                    }
                    case 9:
                    {
                        return 6;
                    }
                    case 11:
                    {
                        return 7;
                    }
                    case 12:
                    {
                        return 8;
                    }
                    case 17:
                    {
                        return 9;
                    }
                    case 18:
                    {
                        return 10;
                    }
                    case 19:
                    {
                        return 11;
                    }
                    case 20:
                    {
                        return 12;
                    }
                    case 21:
                    {
                        return 13;
                    }
                    case 25:
                    {
                        return 14;
                    }
                    case 26:
                    {
                        return 15;
                    }
                    case 27:
                    {
                        return 16;
                    }
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 30:
                    {
                        return 19;
                    }
                    case 39:
                    case 41:
                    case 46:
                    case 47:
                    {
                        return 20;
                    }
                    case 45:
                    {
                        return 21;
                    }
                    case 48:
                    {
                        return 22;
                    }
                    case 49:
                    {
                        return 23;
                    }
                    case 50:
                    {
                        return 24;
                    }
                    case 51:
                    {
                        return 25;
                    }
                    case 52:
                    {
                        return 26;
                    }
                    case 53:
                    {
                        return 27;
                    }
                    case 54:
                    {
                        return 28;
                    }
                    case 55:
                    {
                        return 29;
                    }
                    case 56:
                    {
                        return 30;
                    }
                    case 57:
                    {
                        return 31;
                    }
                    case 58:
                    {
                        return 32;
                    }
                    case 59:
                    {
                        return 33;
                    }
                    case 60:
                    {
                        return 34;
                    }
                    case 61:
                    {
                        return 35;
                    }
                    case 62:
                    {
                        return 36;
                    }
                    case 63:
                    {
                        return 37;
                    }
                    case 64:
                    {
                        return 38;
                    }
                    case 65:
                    {
                        return 39;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 39:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(61, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 38:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(60, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 37:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(58, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 36:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(55, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 35:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(54, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 34:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(53, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 33:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(49, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 32:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(48, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 31:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(52, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 40;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 40:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(33, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 30:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(29, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 41;
                    }
                    case 56:
                    {
                        return 42;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 42:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(27, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 41:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(24, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 29:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(30, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 43;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 43:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(23, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 28:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(31, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 44;
                    }
                    case 54:
                    {
                        return 45;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 45:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(28, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 44:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(22, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 27:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(37, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 46;
                    }
                    case 53:
                    {
                        return 47;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 47:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(39, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 48;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 48:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(21, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 46:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(34, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 52:
                    {
                        return 49;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 49:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(36, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 26:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(38, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 50;
                    }
                    case 52:
                    {
                        return 51;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 51:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(40, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 52;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 52:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(20, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 50:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(35, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 25:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(45, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 53;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 53:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(17, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 24:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(14, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 50:
                    {
                        return 54;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 54:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(32, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 23:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(13, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 22:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(26, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 48:
                    {
                        return 55;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 55:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(12, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 21:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 29:
                    {
                        return 56;
                    }
                    case 13:
                    {
                        return 57;
                    }
                    case 14:
                    {
                        return 58;
                    }
                    case 15:
                    case 16:
                    {
                        return 59;
                    }
                    case 17:
                    case 18:
                    {
                        return 60;
                    }
                    case 19:
                    {
                        return 61;
                    }
                    case 24:
                    {
                        return 62;
                    }
                    case 25:
                    {
                        return 63;
                    }
                    case 36:
                    {
                        return 64;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 39:
                    case 41:
                    {
                        return 66;
                    }
                    case 42:
                    {
                        return 67;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 68;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 68:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 59;
                    }
                    case 17:
                    case 18:
                    {
                        return 60;
                    }
                    case 19:
                    {
                        return 61;
                    }
                    case 24:
                    {
                        return 62;
                    }
                    case 25:
                    {
                        return 63;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 39:
                    case 41:
                    {
                        return 66;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 68;
                    }
                    case 2:
                    case 29:
                    {
                        return 69;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 69:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 59;
                    }
                    case 17:
                    case 18:
                    {
                        return 60;
                    }
                    case 19:
                    {
                        return 61;
                    }
                    case 24:
                    {
                        return 62;
                    }
                    case 25:
                    {
                        return 63;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 39:
                    case 41:
                    {
                        return 66;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 68;
                    }
                    case 2:
                    {
                        return 70;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 70:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 59;
                    }
                    case 17:
                    case 18:
                    {
                        return 60;
                    }
                    case 19:
                    {
                        return 61;
                    }
                    case 24:
                    {
                        return 62;
                    }
                    case 25:
                    {
                        return 63;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 39:
                    case 41:
                    {
                        return 66;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 68;
                    }
                    case 2:
                    {
                        return 71;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 71:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 59;
                    }
                    case 17:
                    case 18:
                    {
                        return 60;
                    }
                    case 24:
                    {
                        return 62;
                    }
                    case 25:
                    {
                        return 63;
                    }
                    case 2:
                    {
                        return 72;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 72:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 60;
                    }
                    case 24:
                    {
                        return 62;
                    }
                    case 25:
                    {
                        return 63;
                    }
                    case 2:
                    {
                        return 73;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 73:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 60;
                    }
                    case 2:
                    {
                        return 74;
                    }
                    case 24:
                    {
                        return 75;
                    }
                    case 25:
                    {
                        return 76;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 76:
            {
                switch (cls)
                {
                    case 25:
                    {
                        return 77;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 77:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 78;
                    }
                    case 17:
                    case 18:
                    {
                        return 79;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 79:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 80;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 80:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 81;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 81:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 82;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 82:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 83;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 83:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 84;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 84:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 78:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 79;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 75:
            {
                switch (cls)
                {
                    case 24:
                    {
                        return 85;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 85:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 78;
                    }
                    case 17:
                    case 18:
                    {
                        return 79;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 74:
            {
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 60;
                    }
                    case 24:
                    {
                        return 75;
                    }
                    case 25:
                    {
                        return 76;
                    }
                    case 2:
                    {
                        return 86;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 86:
            {
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 87;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 87:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 88;
                    }
                    case 24:
                    {
                        return 89;
                    }
                    case 25:
                    {
                        return 90;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 90:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 91;
                    }
                    case 25:
                    {
                        return 92;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 92:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 93;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 93:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 91;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 91:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 94;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 94:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 80;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 89:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 91;
                    }
                    case 24:
                    {
                        return 95;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 95:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 93;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 88:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 91;
                    }
                    case 24:
                    {
                        return 96;
                    }
                    case 25:
                    {
                        return 97;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 97:
            {
                switch (cls)
                {
                    case 25:
                    {
                        return 92;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 96:
            {
                switch (cls)
                {
                    case 24:
                    {
                        return 95;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 67:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 98;
                    }
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 100:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    case 2:
                    {
                        return 101;
                    }
                    case 15:
                    case 16:
                    {
                        return 102;
                    }
                    case 17:
                    case 18:
                    {
                        return 103;
                    }
                    case 24:
                    {
                        return 104;
                    }
                    case 25:
                    {
                        return 105;
                    }
                    case 29:
                    {
                        return 106;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 106:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    case 15:
                    case 16:
                    {
                        return 102;
                    }
                    case 17:
                    case 18:
                    {
                        return 103;
                    }
                    case 24:
                    {
                        return 104;
                    }
                    case 25:
                    {
                        return 105;
                    }
                    case 2:
                    {
                        return 107;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 107:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    case 15:
                    case 16:
                    {
                        return 102;
                    }
                    case 17:
                    case 18:
                    {
                        return 103;
                    }
                    case 24:
                    {
                        return 104;
                    }
                    case 25:
                    {
                        return 105;
                    }
                    case 2:
                    {
                        return 108;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 108:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    case 15:
                    case 16:
                    {
                        return 102;
                    }
                    case 17:
                    case 18:
                    {
                        return 103;
                    }
                    case 24:
                    {
                        return 104;
                    }
                    case 25:
                    {
                        return 105;
                    }
                    case 2:
                    {
                        return 109;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 109:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 103;
                    }
                    case 24:
                    {
                        return 104;
                    }
                    case 25:
                    {
                        return 105;
                    }
                    case 2:
                    {
                        return 110;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 110:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 103;
                    }
                    case 2:
                    {
                        return 111;
                    }
                    case 24:
                    {
                        return 112;
                    }
                    case 25:
                    {
                        return 113;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 113:
            {
                switch (cls)
                {
                    case 25:
                    {
                        return 114;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 114:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 115;
                    }
                    case 17:
                    case 18:
                    {
                        return 116;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 116:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 117;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 117:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 118;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 118:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 119;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 119:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 120;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 120:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 83;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 115:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 116;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 112:
            {
                switch (cls)
                {
                    case 24:
                    {
                        return 121;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 121:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 115;
                    }
                    case 17:
                    case 18:
                    {
                        return 116;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 111:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 103;
                    }
                    case 24:
                    {
                        return 112;
                    }
                    case 25:
                    {
                        return 113;
                    }
                    case 2:
                    {
                        return 122;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 122:
            {
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 123;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 123:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 124;
                    }
                    case 24:
                    {
                        return 125;
                    }
                    case 25:
                    {
                        return 126;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 126:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 127;
                    }
                    case 25:
                    {
                        return 128;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 128:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 129;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 129:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 127;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 127:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 130;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 130:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 117;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 125:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 127;
                    }
                    case 24:
                    {
                        return 131;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 131:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 129;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 124:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 127;
                    }
                    case 24:
                    {
                        return 132;
                    }
                    case 25:
                    {
                        return 133;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 133:
            {
                switch (cls)
                {
                    case 25:
                    {
                        return 128;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 132:
            {
                switch (cls)
                {
                    case 24:
                    {
                        return 131;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 105:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 25:
                    {
                        return 114;
                    }
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 134;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 134:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 118;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 104:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    {
                        return 121;
                    }
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 134;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 103:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 124;
                    }
                    case 24:
                    {
                        return 125;
                    }
                    case 25:
                    {
                        return 126;
                    }
                    case 15:
                    case 16:
                    {
                        return 135;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 135:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 130;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 102:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 136;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 136:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 119;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 101:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    case 15:
                    case 16:
                    {
                        return 102;
                    }
                    case 17:
                    case 18:
                    {
                        return 103;
                    }
                    case 24:
                    {
                        return 104;
                    }
                    case 25:
                    {
                        return 105;
                    }
                    case 2:
                    case 29:
                    {
                        return 106;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 99:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    case 2:
                    {
                        return 101;
                    }
                    case 15:
                    case 16:
                    {
                        return 102;
                    }
                    case 17:
                    case 18:
                    {
                        return 103;
                    }
                    case 24:
                    {
                        return 104;
                    }
                    case 25:
                    {
                        return 105;
                    }
                    case 29:
                    {
                        return 106;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 98:
            {
                switch (cls)
                {
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    case 2:
                    {
                        return 137;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 137:
            {
                switch (cls)
                {
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    case 2:
                    {
                        return 138;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 138:
            {
                switch (cls)
                {
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 66:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 66;
                    }
                    case 2:
                    case 29:
                    {
                        return 139;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 139:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 66;
                    }
                    case 2:
                    {
                        return 140;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 140:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 66;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 65:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 141;
                    }
                    case 20:
                    {
                        return 142;
                    }
                    case 21:
                    {
                        return 143;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 144;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 144:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 144;
                    }
                    case 2:
                    case 29:
                    {
                        return 145;
                    }
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 147:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 148;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 148:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 149;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 149:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 146:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 148;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 145:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 144;
                    }
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 2:
                    {
                        return 150;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 150:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 144;
                    }
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 2:
                    {
                        return 151;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 151:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 2:
                    {
                        return 152;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 152:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 2:
                    {
                        return 149;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 143:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 144;
                    }
                    case 2:
                    {
                        return 153;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 153:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 144;
                    }
                    case 2:
                    {
                        return 154;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 154:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 144;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 142:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 144;
                    }
                    case 2:
                    {
                        return 153;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 141:
            {
                switch (cls)
                {
                    case 20:
                    {
                        return 142;
                    }
                    case 21:
                    {
                        return 143;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 144;
                    }
                    case 2:
                    {
                        return 154;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 64:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 98;
                    }
                    case 45:
                    {
                        return 99;
                    }
                    case 47:
                    {
                        return 100;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 63:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 25:
                    {
                        return 77;
                    }
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 155;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 155:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 81;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 62:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    {
                        return 85;
                    }
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 155;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 61:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 2:
                    {
                        return 156;
                    }
                    case 37:
                    case 38:
                    {
                        return 157;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 158;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 158:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 37:
                    case 38:
                    {
                        return 157;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 158;
                    }
                    case 2:
                    case 29:
                    {
                        return 159;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 159:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 37:
                    case 38:
                    {
                        return 157;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 158;
                    }
                    case 2:
                    {
                        return 160;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 160:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 37:
                    case 38:
                    {
                        return 157;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 158;
                    }
                    case 2:
                    {
                        return 161;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 161:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 37:
                    case 38:
                    {
                        return 157;
                    }
                    case 2:
                    {
                        return 162;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 162:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 2:
                    {
                        return 151;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 157:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 163;
                    }
                    case 20:
                    {
                        return 164;
                    }
                    case 21:
                    {
                        return 165;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 166;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 166:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 166;
                    }
                    case 2:
                    case 29:
                    {
                        return 167;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 167:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 166;
                    }
                    case 2:
                    {
                        return 168;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 168:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 2:
                    {
                        return 151;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 166;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 165:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 166;
                    }
                    case 2:
                    {
                        return 169;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 169:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 166;
                    }
                    case 2:
                    {
                        return 170;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 170:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 166;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 164:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 166;
                    }
                    case 2:
                    {
                        return 169;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 163:
            {
                switch (cls)
                {
                    case 20:
                    {
                        return 164;
                    }
                    case 21:
                    {
                        return 165;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 166;
                    }
                    case 2:
                    {
                        return 170;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 156:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 146;
                    }
                    case 40:
                    case 43:
                    {
                        return 147;
                    }
                    case 37:
                    case 38:
                    {
                        return 157;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 158;
                    }
                    case 2:
                    {
                        return 162;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 60:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 88;
                    }
                    case 24:
                    {
                        return 89;
                    }
                    case 25:
                    {
                        return 90;
                    }
                    case 15:
                    case 16:
                    {
                        return 171;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 171:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 94;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 59:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 172;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 172:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 82;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 58:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 173;
                    }
                    case 19:
                    {
                        return 174;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 175;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 175:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 175;
                    }
                    case 2:
                    case 29:
                    {
                        return 176;
                    }
                    case 15:
                    case 16:
                    {
                        return 177;
                    }
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 19:
                    {
                        return 179;
                    }
                    case 22:
                    case 23:
                    {
                        return 180;
                    }
                    case 24:
                    {
                        return 181;
                    }
                    case 25:
                    {
                        return 182;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 182:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 183;
                    }
                    case 25:
                    {
                        return 184;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 184:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 185;
                    }
                    case 17:
                    case 18:
                    {
                        return 186;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 186:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 187;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 187:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 188;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 188:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 189;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 189:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 120;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 185:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 186;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 183:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 188;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 181:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 183;
                    }
                    case 24:
                    {
                        return 190;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 190:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 185;
                    }
                    case 17:
                    case 18:
                    {
                        return 186;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 180:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 191;
                    }
                    case 20:
                    {
                        return 192;
                    }
                    case 21:
                    {
                        return 193;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 194;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 194:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 194;
                    }
                    case 2:
                    case 29:
                    {
                        return 195;
                    }
                    case 24:
                    case 25:
                    {
                        return 196;
                    }
                    case 40:
                    case 43:
                    {
                        return 197;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 197:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 198;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 198:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 199;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 199:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 149;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 196:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 198;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 195:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 194;
                    }
                    case 24:
                    case 25:
                    {
                        return 196;
                    }
                    case 40:
                    case 43:
                    {
                        return 197;
                    }
                    case 2:
                    {
                        return 200;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 200:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 194;
                    }
                    case 24:
                    case 25:
                    {
                        return 196;
                    }
                    case 40:
                    case 43:
                    {
                        return 197;
                    }
                    case 2:
                    {
                        return 201;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 201:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    case 25:
                    {
                        return 196;
                    }
                    case 40:
                    case 43:
                    {
                        return 197;
                    }
                    case 2:
                    {
                        return 199;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 193:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 194;
                    }
                    case 2:
                    {
                        return 202;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 202:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 194;
                    }
                    case 2:
                    {
                        return 203;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 203:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 194;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 192:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 194;
                    }
                    case 2:
                    {
                        return 202;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 191:
            {
                switch (cls)
                {
                    case 20:
                    {
                        return 192;
                    }
                    case 21:
                    {
                        return 193;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 194;
                    }
                    case 2:
                    {
                        return 203;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 179:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 204;
                    }
                    case 22:
                    case 23:
                    {
                        return 205;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 206;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 206:
            {
                switch (cls)
                {
                    case 22:
                    case 23:
                    {
                        return 205;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 206;
                    }
                    case 2:
                    case 29:
                    {
                        return 207;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 207:
            {
                switch (cls)
                {
                    case 22:
                    case 23:
                    {
                        return 205;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 206;
                    }
                    case 2:
                    {
                        return 208;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 208:
            {
                switch (cls)
                {
                    case 22:
                    case 23:
                    {
                        return 205;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 206;
                    }
                    case 2:
                    {
                        return 209;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 209:
            {
                switch (cls)
                {
                    case 22:
                    case 23:
                    {
                        return 205;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 205:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 210;
                    }
                    case 20:
                    {
                        return 211;
                    }
                    case 21:
                    {
                        return 212;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 213;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 213:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 213;
                    }
                    case 2:
                    case 29:
                    {
                        return 214;
                    }
                    case 24:
                    case 25:
                    {
                        return 215;
                    }
                    case 40:
                    case 43:
                    {
                        return 216;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 216:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 217;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 217:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 199;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 215:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 217;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 214:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 213;
                    }
                    case 24:
                    case 25:
                    {
                        return 215;
                    }
                    case 40:
                    case 43:
                    {
                        return 216;
                    }
                    case 2:
                    {
                        return 218;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 218:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 213;
                    }
                    case 24:
                    case 25:
                    {
                        return 215;
                    }
                    case 40:
                    case 43:
                    {
                        return 216;
                    }
                    case 2:
                    {
                        return 219;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 219:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 199;
                    }
                    case 24:
                    case 25:
                    {
                        return 215;
                    }
                    case 40:
                    case 43:
                    {
                        return 216;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 212:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 213;
                    }
                    case 2:
                    {
                        return 220;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 220:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 213;
                    }
                    case 2:
                    {
                        return 221;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 221:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 213;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 211:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 213;
                    }
                    case 2:
                    {
                        return 220;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 210:
            {
                switch (cls)
                {
                    case 20:
                    {
                        return 211;
                    }
                    case 21:
                    {
                        return 212;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 213;
                    }
                    case 2:
                    {
                        return 221;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 204:
            {
                switch (cls)
                {
                    case 22:
                    case 23:
                    {
                        return 205;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 206;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 178:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 222;
                    }
                    case 15:
                    case 16:
                    {
                        return 223;
                    }
                    case 24:
                    {
                        return 224;
                    }
                    case 25:
                    {
                        return 225;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 225:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 226;
                    }
                    case 25:
                    {
                        return 227;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 227:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 228;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 228:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 226;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 226:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 229;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 229:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 187;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 224:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 226;
                    }
                    case 24:
                    {
                        return 230;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 230:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 228;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 223:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 229;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 222:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 226;
                    }
                    case 24:
                    {
                        return 231;
                    }
                    case 25:
                    {
                        return 232;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 232:
            {
                switch (cls)
                {
                    case 25:
                    {
                        return 227;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 231:
            {
                switch (cls)
                {
                    case 24:
                    {
                        return 230;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 177:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 233;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 233:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 189;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 176:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 175;
                    }
                    case 15:
                    case 16:
                    {
                        return 177;
                    }
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 19:
                    {
                        return 179;
                    }
                    case 22:
                    case 23:
                    {
                        return 180;
                    }
                    case 24:
                    {
                        return 181;
                    }
                    case 25:
                    {
                        return 182;
                    }
                    case 2:
                    {
                        return 234;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 234:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 175;
                    }
                    case 15:
                    case 16:
                    {
                        return 177;
                    }
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 19:
                    {
                        return 179;
                    }
                    case 22:
                    case 23:
                    {
                        return 180;
                    }
                    case 24:
                    {
                        return 181;
                    }
                    case 25:
                    {
                        return 182;
                    }
                    case 2:
                    {
                        return 235;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 235:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 177;
                    }
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 24:
                    {
                        return 181;
                    }
                    case 25:
                    {
                        return 182;
                    }
                    case 2:
                    {
                        return 236;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 236:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 24:
                    {
                        return 181;
                    }
                    case 25:
                    {
                        return 182;
                    }
                    case 2:
                    {
                        return 237;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 237:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 2:
                    {
                        return 238;
                    }
                    case 24:
                    {
                        return 239;
                    }
                    case 25:
                    {
                        return 240;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 240:
            {
                switch (cls)
                {
                    case 25:
                    {
                        return 184;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 239:
            {
                switch (cls)
                {
                    case 24:
                    {
                        return 190;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 238:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 24:
                    {
                        return 239;
                    }
                    case 25:
                    {
                        return 240;
                    }
                    case 2:
                    {
                        return 241;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 241:
            {
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 242;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 242:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 222;
                    }
                    case 24:
                    {
                        return 224;
                    }
                    case 25:
                    {
                        return 225;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 174:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 206;
                    }
                    case 2:
                    {
                        return 243;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 243:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 206;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 173:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 174;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 175;
                    }
                    case 2:
                    {
                        return 244;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 244:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 174;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 175;
                    }
                    case 2:
                    {
                        return 245;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 245:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 174;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 246;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 246:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 179;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 246;
                    }
                    case 2:
                    case 29:
                    {
                        return 247;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 247:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 179;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 246;
                    }
                    case 2:
                    {
                        return 248;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 248:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 179;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 246;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 57:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 173;
                    }
                    case 19:
                    {
                        return 174;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 175;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 56:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 59;
                    }
                    case 17:
                    case 18:
                    {
                        return 60;
                    }
                    case 19:
                    {
                        return 61;
                    }
                    case 24:
                    {
                        return 62;
                    }
                    case 25:
                    {
                        return 63;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 39:
                    case 41:
                    {
                        return 66;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 68;
                    }
                    case 2:
                    {
                        return 70;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 20:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 2:
                    case 29:
                    {
                        return 249;
                    }
                    case 15:
                    case 16:
                    {
                        return 250;
                    }
                    case 17:
                    case 18:
                    {
                        return 251;
                    }
                    case 24:
                    {
                        return 252;
                    }
                    case 25:
                    {
                        return 253;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 254;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 254:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 15:
                    case 16:
                    {
                        return 250;
                    }
                    case 17:
                    case 18:
                    {
                        return 251;
                    }
                    case 24:
                    {
                        return 252;
                    }
                    case 25:
                    {
                        return 253;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 254;
                    }
                    case 2:
                    case 29:
                    {
                        return 255;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 255:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 15:
                    case 16:
                    {
                        return 250;
                    }
                    case 17:
                    case 18:
                    {
                        return 251;
                    }
                    case 24:
                    {
                        return 252;
                    }
                    case 25:
                    {
                        return 253;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 254;
                    }
                    case 2:
                    {
                        return 256;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 256:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 15:
                    case 16:
                    {
                        return 250;
                    }
                    case 17:
                    case 18:
                    {
                        return 251;
                    }
                    case 24:
                    {
                        return 252;
                    }
                    case 25:
                    {
                        return 253;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 254;
                    }
                    case 2:
                    {
                        return 257;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 257:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 250;
                    }
                    case 17:
                    case 18:
                    {
                        return 251;
                    }
                    case 24:
                    {
                        return 252;
                    }
                    case 25:
                    {
                        return 253;
                    }
                    case 2:
                    {
                        return 258;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 258:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 251;
                    }
                    case 24:
                    {
                        return 252;
                    }
                    case 25:
                    {
                        return 253;
                    }
                    case 2:
                    {
                        return 259;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 259:
            {
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 251;
                    }
                    case 2:
                    {
                        return 260;
                    }
                    case 24:
                    {
                        return 261;
                    }
                    case 25:
                    {
                        return 262;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 262:
            {
                switch (cls)
                {
                    case 25:
                    {
                        return 263;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 263:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 264;
                    }
                    case 17:
                    case 18:
                    {
                        return 265;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 265:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 266;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 266:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 267;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 267:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 268;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 268:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 84;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 264:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 265;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 261:
            {
                switch (cls)
                {
                    case 24:
                    {
                        return 269;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 269:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 264;
                    }
                    case 17:
                    case 18:
                    {
                        return 265;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 260:
            {
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 251;
                    }
                    case 24:
                    {
                        return 261;
                    }
                    case 25:
                    {
                        return 262;
                    }
                    case 2:
                    {
                        return 270;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 270:
            {
                switch (cls)
                {
                    case 17:
                    case 18:
                    {
                        return 271;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 271:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 272;
                    }
                    case 24:
                    {
                        return 273;
                    }
                    case 25:
                    {
                        return 274;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 274:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 275;
                    }
                    case 25:
                    {
                        return 276;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 276:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 277;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 277:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 275;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 275:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 278;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 278:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 266;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 273:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 275;
                    }
                    case 24:
                    {
                        return 279;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 279:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 277;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 272:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 275;
                    }
                    case 24:
                    {
                        return 280;
                    }
                    case 25:
                    {
                        return 281;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 281:
            {
                switch (cls)
                {
                    case 25:
                    {
                        return 276;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 280:
            {
                switch (cls)
                {
                    case 24:
                    {
                        return 279;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 253:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 25:
                    {
                        return 263;
                    }
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 282;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 282:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 267;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 252:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 24:
                    {
                        return 269;
                    }
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 282;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 251:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 272;
                    }
                    case 24:
                    {
                        return 273;
                    }
                    case 25:
                    {
                        return 274;
                    }
                    case 15:
                    case 16:
                    {
                        return 283;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 283:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 278;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 250:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    case 17:
                    case 18:
                    {
                        return 284;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 284:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 268;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 249:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 15:
                    case 16:
                    {
                        return 250;
                    }
                    case 17:
                    case 18:
                    {
                        return 251;
                    }
                    case 24:
                    {
                        return 252;
                    }
                    case 25:
                    {
                        return 253;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 254;
                    }
                    case 2:
                    {
                        return 256;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 19:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(25, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 18:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 287:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(9, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 286:
            {
                switch (cls)
                {
                    case 13:
                    {
                        return 288;
                    }
                    case 17:
                    {
                        return 289;
                    }
                    case 18:
                    {
                        return 290;
                    }
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 42:
                    case 43:
                    case 44:
                    {
                        return 291;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 292;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 292:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 293;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 294;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 294:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 293;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 295;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 295:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 296;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 296:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 297;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 297:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 298;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 298:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 293:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 296;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 291:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 297;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 290:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 299;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 299:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 300;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 300:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 301;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 301:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 302;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 302:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 303;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 303:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 304;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 304:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 305;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 305:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 306;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 306:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 307;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 307:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 289:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 308;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 308:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 309;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 309:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 310;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 310:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 311;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 311:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 307;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 288:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 312;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 313;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 313:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 313;
                    }
                    case 2:
                    {
                        return 314;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 314:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    case 2:
                    {
                        return 298;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 313;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 312:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 313;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 285:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 285;
                    }
                    case 27:
                    {
                        return 286;
                    }
                    case 29:
                    {
                        return 287;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 17:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 317:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(11, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 316:
            {
                switch (cls)
                {
                    case 13:
                    {
                        return 318;
                    }
                    case 17:
                    {
                        return 319;
                    }
                    case 18:
                    {
                        return 320;
                    }
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 42:
                    case 43:
                    case 44:
                    {
                        return 321;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 322;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 322:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 323;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 324;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 324:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 323;
                    }
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 325;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 325:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 326;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 326:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 327;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 327:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 328;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 328:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 323:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 326;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 321:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 327;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 320:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 329;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 329:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 330;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 330:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 331;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 331:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 332;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 332:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 333;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 333:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 334;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 334:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 335;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 335:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 336;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 336:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 337;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 337:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 319:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 338;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 338:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 339;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 339:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 340;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 340:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 341;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 341:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 337;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 318:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 342;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 343;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 343:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 343;
                    }
                    case 2:
                    {
                        return 344;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 344:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    case 2:
                    {
                        return 328;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 343;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 342:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 343;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 315:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 315;
                    }
                    case 27:
                    {
                        return 316;
                    }
                    case 28:
                    {
                        return 317;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 16:
            {
                switch (cls)
                {
                    case 17:
                    {
                        return 345;
                    }
                    case 18:
                    {
                        return 346;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 346:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 347;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 347:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 348;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 348:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 349;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 349:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 350;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 350:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 351;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 351:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 352;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 352:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 353;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 353:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 354;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 354:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 355;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 357:
            {
                switch (cls)
                {
                    case 17:
                    {
                        return 358;
                    }
                    case 18:
                    {
                        return 359;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 359:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 360;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 360:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 361;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 361:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 362;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 362:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 363;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 363:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 364;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 364:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 365;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 365:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 366;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 366:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 367;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 367:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 368;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 368:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 369;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 369:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 370;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 370:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 371;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 371:
            {
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 372;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 372:
            {
                switch (cls)
                {
                    case 27:
                    {
                        return 357;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 358:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 373;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 373:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 374;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 374:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 375;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 375:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 376;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 376:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 368;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 356:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 369;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 355:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 377;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 377:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 370;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 345:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 378;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 378:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 379;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 379:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 380;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 380:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 381;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 381:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 355;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 15:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 377;
                    }
                    case 28:
                    {
                        return 382;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 382:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(10, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 14:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 383;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 384:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 369;
                    }
                    case 28:
                    {
                        return 382;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 383:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 370;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 13:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(42, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 21:
                    {
                        return 385;
                    }
                    case 50:
                    {
                        return 386;
                    }
                    case 52:
                    {
                        return 387;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 387:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(57, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 12:
                    {
                        return 388;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 388:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(47, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 386:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(19, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 385:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(51, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 12:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(41, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 20:
                    {
                        return 389;
                    }
                    case 50:
                    {
                        return 390;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 390:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(18, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 389:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(50, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 11:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(56, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 158;
                    }
                    case 2:
                    {
                        return 391;
                    }
                    case 12:
                    {
                        return 392;
                    }
                    case 19:
                    {
                        return 393;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 393:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 394;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 394:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(59, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 392:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(46, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 391:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 158;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 10:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    case 2:
                    {
                        return 395;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 395:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    case 2:
                    {
                        return 396;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 396:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 371;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 9:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    case 2:
                    {
                        return 397;
                    }
                    case 41:
                    {
                        return 398;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 398:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    case 2:
                    {
                        return 399;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 399:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    case 2:
                    {
                        return 400;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 400:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    case 2:
                    {
                        return 401;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 401:
            {
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 372;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 397:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 26:
                    {
                        return 384;
                    }
                    case 2:
                    {
                        return 400;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 8:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(43, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 11:
                    {
                        return 402;
                    }
                    case 50:
                    {
                        return 403;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 403:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(15, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 402:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(5, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 7:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(44, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 11:
                    {
                        return 404;
                    }
                    case 12:
                    {
                        return 405;
                    }
                    case 50:
                    {
                        return 406;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 406:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(16, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 405:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(4, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 404:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 407;
                    }
                    case 1:
                    {
                        return 408;
                    }
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 409;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 409:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 407;
                    }
                    case 1:
                    {
                        return 408;
                    }
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 409;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 408:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(3, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    {
                        return 410;
                    }
                    case 2:
                    {
                        return 411;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 411:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(3, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 410:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(3, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 412;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 412:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(3, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 411;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 407:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(3, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 412;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 6:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 413;
                    }
                    case 1:
                    {
                        return 414;
                    }
                    case 2:
                    {
                        return 415;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 416;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 416:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 413;
                    }
                    case 1:
                    {
                        return 414;
                    }
                    case 2:
                    {
                        return 415;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 416;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 415:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 413;
                    }
                    case 1:
                    {
                        return 414;
                    }
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 64:
                    case 65:
                    {
                        return 416;
                    }
                    case 2:
                    {
                        return 417;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 417:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 413;
                    }
                    case 1:
                    {
                        return 414;
                    }
                    case 2:
                    {
                        return 418;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 418:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 413;
                    }
                    case 1:
                    {
                        return 419;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 419:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 420;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 420:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(2, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 421;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 421:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(2, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 422;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 422:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(2, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 414:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(2, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    {
                        return 420;
                    }
                    case 2:
                    {
                        return 422;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 413:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(2, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 421;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 5:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 7:
                    case 8:
                    {
                        return 5;
                    }
                    case 9:
                    {
                        return 6;
                    }
                    case 0:
                    case 1:
                    {
                        return 423;
                    }
                    case 2:
                    {
                        return 424;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 424:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 7:
                    case 8:
                    {
                        return 5;
                    }
                    case 9:
                    {
                        return 6;
                    }
                    case 0:
                    case 1:
                    {
                        return 423;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 423:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 7:
                    case 8:
                    {
                        return 423;
                    }
                    case 2:
                    {
                        return 425;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 425:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 7:
                    case 8:
                    {
                        return 423;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 4:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 356;
                    }
                    case 27:
                    {
                        return 357;
                    }
                    case 2:
                    {
                        return 377;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 3:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
                    }
                    case 3:
                    case 4:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 22:
                    case 23:
                    case 24:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 40:
                    case 42:
                    case 43:
                    case 44:
                    {
                        return 4;
                    }
                    case 7:
                    case 8:
                    {
                        return 5;
                    }
                    case 9:
                    {
                        return 6;
                    }
                    case 11:
                    {
                        return 7;
                    }
                    case 12:
                    {
                        return 8;
                    }
                    case 17:
                    {
                        return 9;
                    }
                    case 18:
                    {
                        return 10;
                    }
                    case 19:
                    {
                        return 11;
                    }
                    case 20:
                    {
                        return 12;
                    }
                    case 21:
                    {
                        return 13;
                    }
                    case 25:
                    {
                        return 14;
                    }
                    case 26:
                    {
                        return 15;
                    }
                    case 27:
                    {
                        return 16;
                    }
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 30:
                    {
                        return 19;
                    }
                    case 39:
                    case 41:
                    case 46:
                    case 47:
                    {
                        return 20;
                    }
                    case 45:
                    {
                        return 21;
                    }
                    case 48:
                    {
                        return 22;
                    }
                    case 49:
                    {
                        return 23;
                    }
                    case 50:
                    {
                        return 24;
                    }
                    case 51:
                    {
                        return 25;
                    }
                    case 52:
                    {
                        return 26;
                    }
                    case 53:
                    {
                        return 27;
                    }
                    case 54:
                    {
                        return 28;
                    }
                    case 55:
                    {
                        return 29;
                    }
                    case 56:
                    {
                        return 30;
                    }
                    case 57:
                    {
                        return 31;
                    }
                    case 58:
                    {
                        return 32;
                    }
                    case 59:
                    {
                        return 33;
                    }
                    case 60:
                    {
                        return 34;
                    }
                    case 61:
                    {
                        return 35;
                    }
                    case 62:
                    {
                        return 36;
                    }
                    case 63:
                    {
                        return 37;
                    }
                    case 64:
                    {
                        return 38;
                    }
                    case 65:
                    {
                        return 39;
                    }
                    case 2:
                    {
                        return 426;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 426:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
                    }
                    case 3:
                    case 4:
                    case 6:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 22:
                    case 23:
                    case 24:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 40:
                    case 42:
                    case 43:
                    case 44:
                    {
                        return 4;
                    }
                    case 7:
                    case 8:
                    {
                        return 5;
                    }
                    case 9:
                    {
                        return 6;
                    }
                    case 17:
                    {
                        return 9;
                    }
                    case 18:
                    {
                        return 10;
                    }
                    case 25:
                    {
                        return 14;
                    }
                    case 26:
                    {
                        return 15;
                    }
                    case 27:
                    {
                        return 16;
                    }
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 39:
                    case 41:
                    case 46:
                    case 47:
                    {
                        return 20;
                    }
                    case 45:
                    {
                        return 21;
                    }
                    case 2:
                    {
                        return 427;
                    }
                    case 19:
                    {
                        return 428;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 428:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 158;
                    }
                    case 2:
                    {
                        return 391;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 427:
            {
                switch (cls)
                {
                    case 27:
                    {
                        return 16;
                    }
                    case 45:
                    {
                        return 21;
                    }
                    case 39:
                    case 41:
                    case 46:
                    case 47:
                    {
                        return 66;
                    }
                    case 19:
                    {
                        return 428;
                    }
                    case 0:
                    {
                        return 429;
                    }
                    case 1:
                    {
                        return 430;
                    }
                    case 2:
                    {
                        return 431;
                    }
                    case 17:
                    {
                        return 432;
                    }
                    case 18:
                    {
                        return 433;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 433:
            {
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 2:
                    {
                        return 434;
                    }
                    case 26:
                    {
                        return 435;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 435:
            {
                switch (cls)
                {
                    case 28:
                    {
                        return 382;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 434:
            {
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 26:
                    {
                        return 435;
                    }
                    case 2:
                    {
                        return 436;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 436:
            {
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 26:
                    {
                        return 435;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 432:
            {
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 26:
                    {
                        return 435;
                    }
                    case 2:
                    {
                        return 437;
                    }
                    case 41:
                    {
                        return 438;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 438:
            {
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 26:
                    {
                        return 435;
                    }
                    case 2:
                    {
                        return 437;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 437:
            {
                switch (cls)
                {
                    case 28:
                    {
                        return 17;
                    }
                    case 29:
                    {
                        return 18;
                    }
                    case 2:
                    {
                        return 434;
                    }
                    case 26:
                    {
                        return 435;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 431:
            {
                switch (cls)
                {
                    case 39:
                    case 41:
                    case 46:
                    case 47:
                    {
                        return 66;
                    }
                    case 19:
                    {
                        return 428;
                    }
                    case 17:
                    {
                        return 432;
                    }
                    case 2:
                    {
                        return 439;
                    }
                    case 45:
                    {
                        return 440;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 440:
            {
                switch (cls)
                {
                    case 13:
                    {
                        return 57;
                    }
                    case 14:
                    {
                        return 58;
                    }
                    case 19:
                    {
                        return 61;
                    }
                    case 36:
                    {
                        return 64;
                    }
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 66;
                    }
                    case 42:
                    {
                        return 67;
                    }
                    case 2:
                    case 29:
                    {
                        return 139;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 439:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 428;
                    }
                    case 39:
                    case 41:
                    case 46:
                    case 47:
                    {
                        return 441;
                    }
                    case 45:
                    {
                        return 442;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 442:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 36:
                    {
                        return 64;
                    }
                    case 42:
                    {
                        return 67;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 441;
                    }
                    case 2:
                    case 29:
                    {
                        return 443;
                    }
                    case 13:
                    {
                        return 444;
                    }
                    case 14:
                    {
                        return 445;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 445:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 446;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 447;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 447:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 177;
                    }
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 24:
                    {
                        return 181;
                    }
                    case 25:
                    {
                        return 182;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 447;
                    }
                    case 2:
                    case 29:
                    {
                        return 448;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 448:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 177;
                    }
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 24:
                    {
                        return 181;
                    }
                    case 25:
                    {
                        return 182;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 447;
                    }
                    case 2:
                    {
                        return 449;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 449:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 15:
                    case 16:
                    {
                        return 177;
                    }
                    case 17:
                    case 18:
                    {
                        return 178;
                    }
                    case 24:
                    {
                        return 181;
                    }
                    case 25:
                    {
                        return 182;
                    }
                    case 2:
                    {
                        return 235;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 447;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 446:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 447;
                    }
                    case 2:
                    {
                        return 450;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 450:
            {
                switch (cls)
                {
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 447;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 444:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 446;
                    }
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 447;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 443:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 441;
                    }
                    case 2:
                    {
                        return 451;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 451:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 441;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 441:
            {
                switch (cls)
                {
                    case 19:
                    {
                        return 61;
                    }
                    case 39:
                    case 41:
                    case 45:
                    case 46:
                    case 47:
                    {
                        return 441;
                    }
                    case 2:
                    case 29:
                    {
                        return 443;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 430:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 452;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 452:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(1, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 453;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 453:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(1, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 454;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 454:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(1, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 429:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(1, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 2:
                    {
                        return 453;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 2:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 1:
                    case 7:
                    case 8:
                    {
                        return 423;
                    }
                    case 0:
                    {
                        return 455;
                    }
                    case 2:
                    {
                        return 456;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 456:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 7:
                    case 8:
                    {
                        return 423;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 455:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 7:
                    case 8:
                    {
                        return 423;
                    }
                    case 2:
                    {
                        return 457;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 457:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 7:
                    case 8:
                    {
                        return 423;
                    }
                    case 2:
                    {
                        return 454;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 1:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                std::int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 7:
                    case 8:
                    {
                        return 423;
                    }
                    case 2:
                    {
                        return 457;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
        }
        return -1;
    }

    static std::int64_t GetTokenId(std::int32_t ruleIndex, soul::lexer::LexerBase<Char>& lexer)
    {
        switch (ruleIndex)
        {
            case 0:
            {
                lexer.Retract();
                return otava::token::WS;
                break;
            }
            case 1:
            {
                lexer.Retract();
                return otava::token::NEWLINE;
                break;
            }
            case 2:
            {
                lexer.Retract();
                return otava::token::PP_LINE;
                break;
            }
            case 3:
            {
                lexer.Retract();
                return otava::token::LINE_COMMENT;
                break;
            }
            case 4:
            {
                lexer.Retract();
                return otava::token::BLOCK_COMMENT_START;
                break;
            }
            case 5:
            {
                lexer.Retract();
                return otava::token::BLOCK_COMMENT_END;
                break;
            }
            case 6:
            {
                lexer.Retract();
                std::int64_t kw = lexer.GetKeywordToken(lexer.CurrentToken().match);
                if (kw == soul::lexer::INVALID_TOKEN) return otava::token::ID;
                else return otava::token::KEYWORD;
                break;
            }
            case 7:
            {
                lexer.Retract();
                return otava::token::INTEGER_LITERAL;
                break;
            }
            case 8:
            {
                lexer.Retract();
                return otava::token::FLOATING_LITERAL;
                break;
            }
            case 9:
            {
                lexer.Retract();
                return otava::token::CHARACTER_LITERAL;
                break;
            }
            case 10:
            {
                lexer.Retract();
                return otava::token::BEGIN_RAW_STRING_LITERAL;
                break;
            }
            case 11:
            {
                lexer.Retract();
                return otava::token::STRING_LITERAL;
                break;
            }
            case 12:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 13:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 14:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 15:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 16:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 17:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 18:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 19:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 20:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 21:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 22:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 23:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 24:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 25:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 26:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 27:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 28:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 29:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 30:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 31:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 32:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 33:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 34:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 35:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 36:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 37:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 38:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 39:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 40:
            {
                auto vars = static_cast<Variables*>(lexer.GetVariables());
                if (vars->langleCount > 0) return soul::lexer::INVALID_TOKEN;
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 41:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 42:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 43:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 44:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 45:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 46:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 47:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 48:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 49:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 50:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 51:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 52:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 53:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 54:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 55:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 56:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 57:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 58:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 59:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 60:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
            case 61:
            {
                lexer.Retract();
                return otava::token::OTHER;
                break;
            }
        }
        return soul::lexer::CONTINUE_TOKEN;
    }
};

template<typename Char>
soul::lexer::ClassMap<Char>* GetClassMap(otava::token::line::lexer::Tag tag)
{
    static std::unique_ptr<soul::lexer::ClassMap<Char>> classmap(soul::lexer::MakeClassMap<Char>("otava.token.line.lexer.classmap"));
    return classmap.get();
}

template<typename Char>
soul::lexer::ClassMap<Char>* GetClassMap(const std::string& moduleFileName, util::ResourceFlags resourceFlags, otava::token::line::lexer::Tag tag)
{
    static std::unique_ptr<soul::lexer::ClassMap<Char>> classmap(soul::lexer::MakeClassMap<Char>(moduleFileName, "otava.token.line.lexer.classmap", resourceFlags));
    return classmap.get();
}

template<typename Char>
soul::lexer::KeywordMap<Char>* GetKeywords(otava::token::line::lexer::Tag tag);

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>(otava::token::line::lexer::Tag tag);

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>(otava::token::line::lexer::Tag tag);

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>(otava::token::line::lexer::Tag tag);

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>(otava::token::line::lexer::Tag tag);

template<typename Char>
soul::lexer::Lexer<OtavaTokenLineLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(MakeLexerMtx());
    auto lexer = soul::lexer::Lexer<OtavaTokenLineLexer<Char>, Char>(start, end, fileName);
    lexer.SetClassMap(GetClassMap<Char>(otava::token::line::lexer::Tag()));
    lexer.SetTokenCollection(GetTokens(otava::token::line::lexer::Tag()));
    lexer.SetKeywordMap(GetKeywords<Char>(otava::token::line::lexer::Tag()));
    return lexer;
}

template<typename Char>
soul::lexer::Lexer<OtavaTokenLineLexer<Char>, Char> MakeLexer(const std::string& moduleFileName, util::ResourceFlags resourceFlags, const Char* start, const Char* end, const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(MakeLexerMtx());
    auto lexer = soul::lexer::Lexer<OtavaTokenLineLexer<Char>, Char>(start, end, fileName);
    lexer.SetClassMap(GetClassMap<Char>(moduleFileName, resourceFlags, otava::token::line::lexer::Tag()));
    lexer.SetTokenCollection(GetTokens(otava::token::line::lexer::Tag()));
    lexer.SetKeywordMap(GetKeywords<Char>(otava::token::line::lexer::Tag()));
    return lexer;
}

} // namespace otava::token::line::lexer
