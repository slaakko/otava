// this file has been automatically generated from 'D:/work/otava/projects/compiler/build/solution.lexer' using soul lexer generator oslg version 0.2.5

export module otava.build.solution_lexer;

import std;
import soul.lexer.cls;
import soul.lexer.base;
import soul.lexer.lexeme;
import soul.lexer.token;
import soul.lexer.classmap;
import soul.lexer.keyword;
import soul.ast.slg;
import soul.ast.common;
import util.binary_resource_ptr;
import util.system;
import util.path;
import otava.build.solution_token;

export namespace otava::build::solution_lexer {

std::mutex& MakeLexerMtx();

soul::ast::common::TokenCollection* GetTokens();

struct SolutionLexer_Variables : public soul::lexer::Variables
{
    SolutionLexer_Variables();
};

template<typename Char>
struct SolutionLexer
{
    using Vars = SolutionLexer_Variables;

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
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
                    }
                    case 5:
                    case 6:
                    case 8:
                    {
                        return 3;
                    }
                    case 12:
                    {
                        return 4;
                    }
                    case 14:
                    {
                        return 5;
                    }
                    case 15:
                    {
                        return 6;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 6:
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
            case 5:
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
            case 4:
            {
                switch (cls)
                {
                    case 1:
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
                    case 14:
                    case 15:
                    {
                        return 7;
                    }
                    case 13:
                    {
                        return 8;
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
            case 7:
            {
                switch (cls)
                {
                    case 1:
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
                    case 14:
                    case 15:
                    {
                        return 7;
                    }
                    case 13:
                    {
                        return 8;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 3:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    {
                        return 9;
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    {
                        return 9;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 2:
            {
                switch (cls)
                {
                    case 1:
                    {
                        return 10;
                    }
                    case 2:
                    {
                        return 11;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 11:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
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
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 13:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 13;
                    }
                    case 0:
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
                    {
                        return 14;
                    }
                    case 1:
                    {
                        return 15;
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    case 0:
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 16;
                    }
                    case 1:
                    {
                        return 17;
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
                    case 0:
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
                    {
                        return 12;
                    }
                    case 1:
                    {
                        return 18;
                    }
                    case 2:
                    {
                        return 19;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 19:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 13;
                    }
                    case 0:
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
                    {
                        return 14;
                    }
                    case 1:
                    {
                        return 15;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 18:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 20;
                    }
                    case 1:
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
                    {
                        return 21;
                    }
                    case 2:
                    {
                        return 22;
                    }
                    case 3:
                    {
                        return 23;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 23:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 16;
                    }
                    case 1:
                    {
                        return 17;
                    }
                    case 0:
                    {
                        return 24;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 24:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    case 0:
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 16;
                    }
                    case 1:
                    {
                        return 17;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 22:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 22;
                    }
                    case 0:
                    {
                        return 25;
                    }
                    case 1:
                    {
                        return 26;
                    }
                    case 3:
                    {
                        return 27;
                    }
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
                    {
                        return 28;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 28:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 20;
                    }
                    case 1:
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
                    {
                        return 21;
                    }
                    case 2:
                    {
                        return 22;
                    }
                    case 3:
                    {
                        return 23;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 27:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 16;
                    }
                    case 1:
                    {
                        return 17;
                    }
                    case 0:
                    {
                        return 24;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 26:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 21;
                    }
                    case 2:
                    {
                        return 22;
                    }
                    case 0:
                    {
                        return 29;
                    }
                    case 1:
                    {
                        return 30;
                    }
                    case 3:
                    {
                        return 31;
                    }
                    case 9:
                    case 10:
                    {
                        return 32;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 32:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 21;
                    }
                    case 2:
                    {
                        return 22;
                    }
                    case 0:
                    {
                        return 29;
                    }
                    case 1:
                    {
                        return 30;
                    }
                    case 3:
                    {
                        return 31;
                    }
                    case 9:
                    case 10:
                    {
                        return 32;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 31:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 16;
                    }
                    case 1:
                    {
                        return 17;
                    }
                    case 0:
                    {
                        return 24;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 30:
            {
                switch (cls)
                {
                    case 1:
                    {
                        return 18;
                    }
                    case 0:
                    {
                        return 20;
                    }
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
                    {
                        return 21;
                    }
                    case 3:
                    {
                        return 23;
                    }
                    case 2:
                    {
                        return 33;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 33:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 22;
                    }
                    case 0:
                    {
                        return 25;
                    }
                    case 1:
                    {
                        return 26;
                    }
                    case 3:
                    {
                        return 27;
                    }
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
                    {
                        return 28;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 29:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    case 0:
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 16;
                    }
                    case 1:
                    {
                        return 17;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 25:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    case 0:
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 16;
                    }
                    case 1:
                    {
                        return 17;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 21:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 20;
                    }
                    case 1:
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
                    {
                        return 21;
                    }
                    case 2:
                    {
                        return 22;
                    }
                    case 3:
                    {
                        return 23;
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    case 0:
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 16;
                    }
                    case 1:
                    {
                        return 17;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 16:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    case 0:
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 16;
                    }
                    case 1:
                    {
                        return 17;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 14:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
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
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 12:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
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
                    {
                        return 12;
                    }
                    case 2:
                    {
                        return 13;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 10:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 34;
                    }
                    case 1:
                    case 2:
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
                    {
                        return 35;
                    }
                    case 3:
                    {
                        return 36;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 36:
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
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
                    }
                    case 0:
                    {
                        return 37;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 37:
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
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 35:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 34;
                    }
                    case 1:
                    case 2:
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
                    {
                        return 35;
                    }
                    case 3:
                    {
                        return 36;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 34:
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
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
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
                    case 3:
                    case 9:
                    case 10:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
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
                break;
            }
            case 1:
            {
                lexer.Retract();
                std::int64_t kw = lexer.GetKeywordToken(lexer.CurrentToken().match);
                if (kw == soul::lexer::INVALID_TOKEN) return otava::build::solution_token::ID;
                else return kw;
                break;
            }
            case 2:
            {
                lexer.Retract();
                return otava::build::solution_token::FILEPATH;
                break;
            }
            case 3:
            {
                lexer.Retract();
                return otava::build::solution_token::SEMICOLON;
                break;
            }
            case 4:
            {
                lexer.Retract();
                return otava::build::solution_token::DOT;
                break;
            }
        }
        return soul::lexer::CONTINUE_TOKEN;
    }
};

template<typename Char>
soul::lexer::ClassMap<Char>* GetClassMap()
{
    static std::unique_ptr<soul::lexer::ClassMap<Char>> classmap(soul::lexer::MakeClassMap<Char>("otava.build.solution_lexer.classmap"));
    return classmap.get();
}

template<typename Char>
soul::lexer::ClassMap<Char>* GetClassMap(const std::string& moduleFileName, util::ResourceFlags resourceFlags)
{
    static std::unique_ptr<soul::lexer::ClassMap<Char>> classmap(soul::lexer::MakeClassMap<Char>(moduleFileName, "otava.build.solution_lexer.classmap", resourceFlags));
    return classmap.get();
}

template<typename Char>
soul::lexer::KeywordMap<Char>* GetKeywords();

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>();

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>();

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>();

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>();

template<typename Char>
soul::lexer::Lexer<SolutionLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(otava::build::solution_lexer::MakeLexerMtx()); 
    auto lxr = soul::lexer::Lexer<SolutionLexer<Char>, Char>(start, end, fileName);
    lxr.SetClassMap(otava::build::solution_lexer::GetClassMap<Char>());
    lxr.SetTokenCollection(otava::build::solution_lexer::GetTokens());
    lxr.SetKeywordMap(otava::build::solution_lexer::GetKeywords<Char>());
    return lxr;
}

template<typename Char>
soul::lexer::Lexer<SolutionLexer<Char>, Char> MakeLexer(const std::string& moduleFileName, util::ResourceFlags resourceFlags, const Char* start, const Char* end, const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(otava::build::solution_lexer::MakeLexerMtx());
    auto lxr = soul::lexer::Lexer<SolutionLexer<Char>, Char>(start, end, fileName);
    lxr.SetClassMap(otava::build::solution_lexer::GetClassMap<Char>(moduleFileName, resourceFlags));
    lxr.SetTokenCollection(otava::build::solution_lexer::GetTokens());
    lxr.SetKeywordMap(otava::build::solution_lexer::GetKeywords<Char>());
    return lxr;
}

} // namespace otava::build::solution_lexer
