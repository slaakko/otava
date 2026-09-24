// this file has been automatically generated from 'D:/work/otava/projects/compiler/build/project.lexer' using soul lexer generator oslg version 0.2.5

export module otava.build.project_lexer;

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
import otava.build.project_token;

export namespace otava::build::project_lexer {

std::mutex& MakeLexerMtx();

soul::ast::common::TokenCollection* GetTokens();

struct ProjectLexer_Variables : public soul::lexer::Variables
{
    ProjectLexer_Variables();
};

template<typename Char>
struct ProjectLexer
{
    using Vars = ProjectLexer_Variables;

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
                    case 8:
                    case 9:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
                    }
                    case 4:
                    case 5:
                    case 7:
                    {
                        return 3;
                    }
                    case 11:
                    {
                        return 4;
                    }
                    case 13:
                    {
                        return 5;
                    }
                    case 14:
                    {
                        return 6;
                    }
                    case 15:
                    {
                        return 7;
                    }
                    case 16:
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
                    case 16:
                    {
                        return 8;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 7:
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
            case 6:
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
            case 5:
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
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 9;
                    }
                    case 12:
                    {
                        return 10;
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
            case 9:
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
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 9;
                    }
                    case 12:
                    {
                        return 10;
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
                    case 16:
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
                    case 16:
                    {
                        return 11;
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
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
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
                switch (cls)
                {
                    case 2:
                    {
                        return 15;
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
                    case 16:
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
                    }
                    case 0:
                    case 3:
                    case 8:
                    case 9:
                    {
                        return 18;
                    }
                    case 1:
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
                    case 16:
                    {
                        return 14;
                    }
                    case 1:
                    {
                        return 20;
                    }
                    case 2:
                    {
                        return 21;
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
                    case 2:
                    {
                        return 15;
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
                    case 16:
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
            case 20:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 22;
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
                    case 16:
                    {
                        return 23;
                    }
                    case 2:
                    {
                        return 24;
                    }
                    case 3:
                    {
                        return 25;
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
                    }
                    case 3:
                    case 8:
                    case 9:
                    {
                        return 18;
                    }
                    case 1:
                    {
                        return 19;
                    }
                    case 0:
                    {
                        return 26;
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
                    }
                    case 0:
                    case 3:
                    case 8:
                    case 9:
                    {
                        return 18;
                    }
                    case 1:
                    {
                        return 19;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 24:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 24;
                    }
                    case 0:
                    {
                        return 27;
                    }
                    case 1:
                    {
                        return 28;
                    }
                    case 3:
                    {
                        return 29;
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
                    case 16:
                    {
                        return 30;
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
                    case 0:
                    {
                        return 22;
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
                    case 16:
                    {
                        return 23;
                    }
                    case 2:
                    {
                        return 24;
                    }
                    case 3:
                    {
                        return 25;
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
                    }
                    case 3:
                    case 8:
                    case 9:
                    {
                        return 18;
                    }
                    case 1:
                    {
                        return 19;
                    }
                    case 0:
                    {
                        return 26;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 28:
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 23;
                    }
                    case 2:
                    {
                        return 24;
                    }
                    case 0:
                    {
                        return 31;
                    }
                    case 1:
                    {
                        return 32;
                    }
                    case 3:
                    {
                        return 33;
                    }
                    case 8:
                    case 9:
                    {
                        return 34;
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 23;
                    }
                    case 2:
                    {
                        return 24;
                    }
                    case 0:
                    {
                        return 31;
                    }
                    case 1:
                    {
                        return 32;
                    }
                    case 3:
                    {
                        return 33;
                    }
                    case 8:
                    case 9:
                    {
                        return 34;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 33:
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
                    }
                    case 3:
                    case 8:
                    case 9:
                    {
                        return 18;
                    }
                    case 1:
                    {
                        return 19;
                    }
                    case 0:
                    {
                        return 26;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 32:
            {
                switch (cls)
                {
                    case 1:
                    {
                        return 20;
                    }
                    case 0:
                    {
                        return 22;
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
                    case 16:
                    {
                        return 23;
                    }
                    case 3:
                    {
                        return 25;
                    }
                    case 2:
                    {
                        return 35;
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
                    case 2:
                    {
                        return 24;
                    }
                    case 0:
                    {
                        return 27;
                    }
                    case 1:
                    {
                        return 28;
                    }
                    case 3:
                    {
                        return 29;
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
                    case 16:
                    {
                        return 30;
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
                    }
                    case 0:
                    case 3:
                    case 8:
                    case 9:
                    {
                        return 18;
                    }
                    case 1:
                    {
                        return 19;
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
                    }
                    case 0:
                    case 3:
                    case 8:
                    case 9:
                    {
                        return 18;
                    }
                    case 1:
                    {
                        return 19;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 23:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 22;
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
                    case 16:
                    {
                        return 23;
                    }
                    case 2:
                    {
                        return 24;
                    }
                    case 3:
                    {
                        return 25;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 22:
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
                    }
                    case 0:
                    case 3:
                    case 8:
                    case 9:
                    {
                        return 18;
                    }
                    case 1:
                    {
                        return 19;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 18:
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
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
                    }
                    case 0:
                    case 3:
                    case 8:
                    case 9:
                    {
                        return 18;
                    }
                    case 1:
                    {
                        return 19;
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
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
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
                    case 16:
                    {
                        return 14;
                    }
                    case 2:
                    {
                        return 15;
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
                    {
                        return 36;
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
                    case 16:
                    {
                        return 37;
                    }
                    case 3:
                    {
                        return 38;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 38:
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
                    case 8:
                    case 9:
                    {
                        return 1;
                    }
                    case 1:
                    {
                        return 2;
                    }
                    case 0:
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
                    case 8:
                    case 9:
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
            case 37:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 36;
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
                    case 16:
                    {
                        return 37;
                    }
                    case 3:
                    {
                        return 38;
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
                    case 0:
                    case 3:
                    case 8:
                    case 9:
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
                    case 8:
                    case 9:
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
                if (kw == soul::lexer::INVALID_TOKEN) return otava::build::project_token::ID;
                else return kw;
                break;
            }
            case 2:
            {
                lexer.Retract();
                return otava::build::project_token::FILEPATH;
                break;
            }
            case 3:
            {
                lexer.Retract();
                return otava::build::project_token::SEMICOLON;
                break;
            }
            case 4:
            {
                lexer.Retract();
                return otava::build::project_token::DOT;
                break;
            }
            case 5:
            {
                lexer.Retract();
                return otava::build::project_token::ASSIGN;
                break;
            }
            case 6:
            {
                lexer.Retract();
                return otava::build::project_token::VALUE;
                break;
            }
        }
        return soul::lexer::CONTINUE_TOKEN;
    }
};

template<typename Char>
soul::lexer::ClassMap<Char>* GetClassMap()
{
    static std::unique_ptr<soul::lexer::ClassMap<Char>> classmap(soul::lexer::MakeClassMap<Char>("otava.build.project_lexer.classmap"));
    return classmap.get();
}

template<typename Char>
soul::lexer::ClassMap<Char>* GetClassMap(const std::string& moduleFileName, util::ResourceFlags resourceFlags)
{
    static std::unique_ptr<soul::lexer::ClassMap<Char>> classmap(soul::lexer::MakeClassMap<Char>(moduleFileName, "otava.build.project_lexer.classmap", resourceFlags));
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
soul::lexer::Lexer<ProjectLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(otava::build::project_lexer::MakeLexerMtx()); 
    auto lxr = soul::lexer::Lexer<ProjectLexer<Char>, Char>(start, end, fileName);
    lxr.SetClassMap(otava::build::project_lexer::GetClassMap<Char>());
    lxr.SetTokenCollection(otava::build::project_lexer::GetTokens());
    lxr.SetKeywordMap(otava::build::project_lexer::GetKeywords<Char>());
    return lxr;
}

template<typename Char>
soul::lexer::Lexer<ProjectLexer<Char>, Char> MakeLexer(const std::string& moduleFileName, util::ResourceFlags resourceFlags, const Char* start, const Char* end, const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(otava::build::project_lexer::MakeLexerMtx());
    auto lxr = soul::lexer::Lexer<ProjectLexer<Char>, Char>(start, end, fileName);
    lxr.SetClassMap(otava::build::project_lexer::GetClassMap<Char>(moduleFileName, resourceFlags));
    lxr.SetTokenCollection(otava::build::project_lexer::GetTokens());
    lxr.SetKeywordMap(otava::build::project_lexer::GetKeywords<Char>());
    return lxr;
}

} // namespace otava::build::project_lexer
