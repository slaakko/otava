
// this file has been automatically generated from 'D:/work/otava/intermediate/intermediate.parser' using soul parser generator spg version 5.2.0

module otava.intermediate.parser;

import util;
import soul.ast.common;
import soul.ast.spg;
import otava.intermediate.error;
import otava.intermediate.token;
import otava.intermediate.lexer;

namespace otava::intermediate::parser {

template<typename LexerT>
void IntermediateParser<LexerT>::Parse(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif
    ++lexer;
    soul::parser::Match match = IntermediateParser<LexerT>::IntermediateFile(lexer, context);
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->DecIndent();
        lexer.Log()->WriteEndRule("parse");
    }
    #endif
    if (match.hit)
    {
        if (*lexer == soul::lexer::END_TOKEN)
        {
            return;
        }
        else
        {
            lexer.ThrowFarthestError();
        }
    }
    else
    {
        lexer.ThrowFarthestError();
    }
    return;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::IntermediateFile(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "IntermediateFile");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303745);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match(true);
                        if (match.hit)
                        {
                            context->SetFilePath(lexer.FileName());
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(true);
                        std::int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeDeclarations(lexer, context);
                            if (match.hit)
                            {
                                *parentMatch6 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(true);
                    std::int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::DataDefinitions(lexer, context);
                        if (match.hit)
                        {
                            *parentMatch8 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                        }
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                soul::parser::Match match(true);
                std::int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch10 = &match;
                {
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::FunctionDefinitions(lexer, context);
                    if (match.hit)
                    {
                        *parentMatch10 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch9 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch11 = &match;
        {
            soul::parser::Match match(true);
            std::int64_t save = lexer.GetPos();
            soul::parser::Match* parentMatch12 = &match;
            {
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Metadata(lexer, context);
                if (match.hit)
                {
                    *parentMatch12 = match;
                }
                else
                {
                    lexer.SetPos(save);
                }
            }
            *parentMatch11 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IntermediateFile");
        else soul::lexer::WriteFailureToLog(lexer, "IntermediateFile");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::TypeDeclarations(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeDeclarations");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303746);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::TYPES)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch5 = &match;
                {
                    while (true)
                    {
                        std::int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeDeclaration(lexer, context);
                            if (match.hit)
                            {
                                *parentMatch5 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                                break;
                            }
                        }
                    }
                }
                *parentMatch4 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch6 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    context->ResolveTypes();
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeDeclarations");
        else soul::lexer::WriteFailureToLog(lexer, "TypeDeclarations");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::TypeDeclaration(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303747);
    std::int32_t tid = std::int32_t();
    std::unique_ptr<soul::parser::Value<std::int32_t>> typeId;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeId(lexer);
                    typeId.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                    if (match.hit)
                    {
                        tid = typeId->value;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::TYPE)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch6 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch8 = &match;
                switch (*lexer)
                {
                    case otava::intermediate::token::LBRACE:
                    {
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::StructureType(lexer, context, tid);
                        if (match.hit)
                        {
                            *parentMatch8 = match;
                        }
                        break;
                    }
                    case otava::intermediate::token::LBRACKET:
                    {
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ArrayType(lexer, context, tid);
                        if (match.hit)
                        {
                            *parentMatch8 = match;
                        }
                        break;
                    }
                    case otava::intermediate::token::FUNCTION:
                    {
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::FunctionType(lexer, context, tid);
                        if (match.hit)
                        {
                            *parentMatch8 = match;
                        }
                        break;
                    }
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "TypeDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::TypeId(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303748);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::TYPEID)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeId");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::MakeUserTypeId(util::ToUtf8(lexer.GetToken(pos).ToString()))));
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeId");
        else soul::lexer::WriteFailureToLog(lexer, "TypeId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::FundamentalTypeId(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FundamentalTypeId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303749);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::intermediate::token::VOID:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::VOID)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::voidTypeId));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::BOOL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::BOOL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::boolTypeId));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::SBYTE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::SBYTE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::sbyteTypeId));
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::BYTE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::BYTE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::byteTypeId));
                    }
                }
                *parentMatch4 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::SHORT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::SHORT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::shortTypeId));
                    }
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::USHORT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::USHORT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::ushortTypeId));
                    }
                }
                *parentMatch6 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::INT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::INT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::intTypeId));
                    }
                }
                *parentMatch7 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::UINT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::UINT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::uintTypeId));
                    }
                }
                *parentMatch8 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::LONG:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::LONG)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::longTypeId));
                    }
                }
                *parentMatch9 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::ULONG:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::ULONG)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::ulongTypeId));
                    }
                }
                *parentMatch10 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::FLOAT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::FLOAT)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::floatTypeId));
                    }
                }
                *parentMatch11 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::DOUBLE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch12 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::DOUBLE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(otava::intermediate::doubleTypeId));
                    }
                }
                *parentMatch12 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FundamentalTypeId");
        else soul::lexer::WriteFailureToLog(lexer, "FundamentalTypeId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::TypeRef(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeRef");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303750);
    soul::ast::Span span = soul::ast::Span();
    std::int32_t baseTypeId = std::int32_t();
    std::int8_t pointerCount = std::int8_t();
    std::unique_ptr<soul::parser::Value<std::int32_t>> typeId;
    std::unique_ptr<soul::parser::Value<std::int32_t>> fundamentalTypeId;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    switch (*lexer)
                    {
                        case otava::intermediate::token::TYPEID:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch5 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeId(lexer);
                                typeId.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                                if (match.hit)
                                {
                                    span = lexer.GetSpan(pos);
                                    baseTypeId = typeId->value;
                                }
                                *parentMatch5 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch4 = match;
                            }
                            break;
                        }
                        case otava::intermediate::token::VOID:
                        case otava::intermediate::token::BOOL:
                        case otava::intermediate::token::SBYTE:
                        case otava::intermediate::token::BYTE:
                        case otava::intermediate::token::SHORT:
                        case otava::intermediate::token::USHORT:
                        case otava::intermediate::token::INT:
                        case otava::intermediate::token::UINT:
                        case otava::intermediate::token::LONG:
                        case otava::intermediate::token::ULONG:
                        case otava::intermediate::token::FLOAT:
                        case otava::intermediate::token::DOUBLE:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::FundamentalTypeId(lexer);
                                fundamentalTypeId.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                                if (match.hit)
                                {
                                    span = lexer.GetSpan(pos);
                                    baseTypeId = fundamentalTypeId->value;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch4 = match;
                            }
                            break;
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        while (true)
                        {
                            std::int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch9 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch10 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == otava::intermediate::token::AST)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            ++pointerCount;
                                        }
                                        *parentMatch10 = match;
                                    }
                                    *parentMatch9 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch8 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeRef");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<otava::intermediate::TypeRef>(otava::intermediate::MakeTypeRef(span, baseTypeId, pointerCount)));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeRef");
        else soul::lexer::WriteFailureToLog(lexer, "TypeRef");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::StructureType(LexerT& lexer, otava::intermediate::IntermediateContext* context, std::int32_t typeId)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StructureType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303751);
    soul::ast::Span span = soul::ast::Span();
    std::vector<otava::intermediate::TypeRef> fieldTypeRefs = std::vector<otava::intermediate::TypeRef>();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> first;
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> next;
    std::unique_ptr<otava::intermediate::MetadataRef> mdRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(true);
                    std::int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                    first.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                    if (match.hit)
                                    {
                                        fieldTypeRefs.push_back(first->value);
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch9 = &match;
                                {
                                    soul::parser::Match match(true);
                                    soul::parser::Match* parentMatch10 = &match;
                                    {
                                        while (true)
                                        {
                                            std::int64_t save = lexer.GetPos();
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch11 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch12 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        if (*lexer == otava::intermediate::token::COMMA)
                                                        {
                                                            ++lexer;
                                                            match.hit = true;
                                                        }
                                                        *parentMatch12 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch13 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch14 = &match;
                                                            {
                                                                std::int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                                                next.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    fieldTypeRefs.push_back(next->value);
                                                                }
                                                                *parentMatch14 = match;
                                                            }
                                                            *parentMatch13 = match;
                                                        }
                                                        *parentMatch12 = match;
                                                    }
                                                    *parentMatch11 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch10 = match;
                                                }
                                                else
                                                {
                                                    lexer.SetPos(save);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    *parentMatch9 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch5 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch15 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch15 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch16 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch17 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch18 = &match;
                {
                    soul::parser::Match match(true);
                    std::int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch19 = &match;
                    {
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataRef(lexer, context);
                        mdRef.reset(static_cast<otava::intermediate::MetadataRef*>(match.value));
                        if (match.hit)
                        {
                            *parentMatch19 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                        }
                    }
                    *parentMatch18 = match;
                }
                if (match.hit)
                {
                    context->AddStructureType(span, typeId, fieldTypeRefs, mdRef.release());
                }
                *parentMatch17 = match;
            }
            *parentMatch16 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StructureType");
        else soul::lexer::WriteFailureToLog(lexer, "StructureType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ArrayType(LexerT& lexer, otava::intermediate::IntermediateContext* context, std::int32_t typeId)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ArrayType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303752);
    soul::ast::Span span = soul::ast::Span();
    std::int64_t size = std::int64_t();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> elementTypeRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::LBRACKET)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            span = lexer.GetSpan(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::NUMBER)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                size = lexer.GetToken(pos).ToLong();
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        bool pass = true;
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::ID)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            pass = util::ToUtf8(lexer.GetToken(pos).ToString()) == "x";
                        }
                        if (match.hit && !pass)
                        {
                            match = soul::parser::Match(false);
                        }
                        *parentMatch8 = match;
                    }
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                elementTypeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                *parentMatch9 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch10 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RBRACKET)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    context->AddArrayType(span, typeId, size, elementTypeRef->value);
                }
                *parentMatch11 = match;
            }
            *parentMatch10 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArrayType");
        else soul::lexer::WriteFailureToLog(lexer, "ArrayType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::FunctionType(LexerT& lexer, otava::intermediate::IntermediateContext* context, std::int32_t typeId)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303753);
    soul::ast::Span span = soul::ast::Span();
    std::vector<otava::intermediate::TypeRef> paramTypeRefs = std::vector<otava::intermediate::TypeRef>();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> returnTypeRef;
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> first;
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> next;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::FUNCTION)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            span = lexer.GetSpan(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                        returnTypeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::LPAREN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                soul::parser::Match match(true);
                std::int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch8 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch9 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch10 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch11 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                first.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                if (match.hit)
                                {
                                    paramTypeRefs.push_back(first->value);
                                }
                                *parentMatch11 = match;
                            }
                            *parentMatch10 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch12 = &match;
                            {
                                soul::parser::Match match(true);
                                soul::parser::Match* parentMatch13 = &match;
                                {
                                    while (true)
                                    {
                                        std::int64_t save = lexer.GetPos();
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch14 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch15 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::intermediate::token::COMMA)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    *parentMatch15 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch16 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch17 = &match;
                                                        {
                                                            std::int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                                            next.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                paramTypeRefs.push_back(next->value);
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch16 = match;
                                                    }
                                                    *parentMatch15 = match;
                                                }
                                                *parentMatch14 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch13 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                                break;
                                            }
                                        }
                                    }
                                }
                                *parentMatch12 = match;
                            }
                            *parentMatch10 = match;
                        }
                        *parentMatch9 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch8 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch7 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch18 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch19 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RPAREN)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    context->AddFunctionType(span, typeId, returnTypeRef->value, paramTypeRefs);
                }
                *parentMatch19 = match;
            }
            *parentMatch18 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionType");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::DataDefinitions(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DataDefinitions");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303754);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::DATA)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch5 = &match;
                {
                    while (true)
                    {
                        std::int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::DataDefinition(lexer, context);
                            if (match.hit)
                            {
                                *parentMatch5 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                                break;
                            }
                        }
                    }
                }
                *parentMatch4 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch6 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    context->ResolveData();
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataDefinitions");
        else soul::lexer::WriteFailureToLog(lexer, "DataDefinitions");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::DataDefinition(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DataDefinition");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303755);
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    soul::ast::Span span = soul::ast::Span();
    std::string variableName = std::string();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> initializer;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                if (match.hit)
                {
                    tref = typeRef->value;
                    context->ResolveType(tref);
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::ID)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                        variableName = util::ToUtf8(lexer.GetToken(pos).ToString());
                    }
                    *parentMatch4 = match;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch5 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                switch (*lexer)
                {
                    case otava::intermediate::token::SEMICOLON:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::SEMICOLON)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                context->AddGlobalVariable(span, tref.GetType(), variableName, nullptr);
                            }
                            *parentMatch8 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch7 = match;
                        }
                        break;
                    }
                    case otava::intermediate::token::ASSIGN:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::ASSIGN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch9 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch10 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Constant(lexer, context, tref.GetType());
                                    initializer.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    if (match.hit)
                                    {
                                        context->AddGlobalVariable(span, tref.GetType(), variableName, initializer.release());
                                    }
                                    *parentMatch11 = match;
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch9 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch7 = match;
                        }
                        break;
                    }
                }
                *parentMatch6 = match;
            }
            *parentMatch5 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DataDefinition");
        else soul::lexer::WriteFailureToLog(lexer, "DataDefinition");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::Constant(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Constant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303756);
    std::unique_ptr<otava::intermediate::Value> boolConstant;
    std::unique_ptr<otava::intermediate::Value> sbyteConstant;
    std::unique_ptr<otava::intermediate::Value> byteConstant;
    std::unique_ptr<otava::intermediate::Value> shortConstant;
    std::unique_ptr<otava::intermediate::Value> ushortConstant;
    std::unique_ptr<otava::intermediate::Value> intConstant;
    std::unique_ptr<otava::intermediate::Value> uintConstant;
    std::unique_ptr<otava::intermediate::Value> longConstant;
    std::unique_ptr<otava::intermediate::Value> ulongConstant;
    std::unique_ptr<otava::intermediate::Value> floatConstant;
    std::unique_ptr<otava::intermediate::Value> doubleConstant;
    std::unique_ptr<otava::intermediate::Value> addressConstant;
    std::unique_ptr<otava::intermediate::Value> arrayConstant;
    std::unique_ptr<otava::intermediate::Value> structureConstant;
    std::unique_ptr<otava::intermediate::Value> stringConstant;
    std::unique_ptr<otava::intermediate::Value> stringArrayConstant;
    std::unique_ptr<otava::intermediate::Value> conversionConstant;
    std::unique_ptr<otava::intermediate::Value> clsidConstant;
    std::unique_ptr<otava::intermediate::Value> symbolConstant;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                switch (*lexer)
                {
                    case otava::intermediate::token::VOID:
                    case otava::intermediate::token::BOOL:
                    case otava::intermediate::token::SBYTE:
                    case otava::intermediate::token::BYTE:
                    case otava::intermediate::token::SHORT:
                    case otava::intermediate::token::USHORT:
                    case otava::intermediate::token::INT:
                    case otava::intermediate::token::UINT:
                    case otava::intermediate::token::LONG:
                    case otava::intermediate::token::ULONG:
                    case otava::intermediate::token::FLOAT:
                    case otava::intermediate::token::DOUBLE:
                    case otava::intermediate::token::TYPEID:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            std::int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch5 = &match;
                            switch (*lexer)
                            {
                                case otava::intermediate::token::BOOL:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch6 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::BoolConstant(lexer, context);
                                        boolConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, boolConstant.release());
                                            }
                                        }
                                        *parentMatch6 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::SBYTE:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::SByteConstant(lexer, context);
                                        sbyteConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, sbyteConstant.release());
                                            }
                                        }
                                        *parentMatch7 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::BYTE:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ByteConstant(lexer, context);
                                        byteConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, byteConstant.release());
                                            }
                                        }
                                        *parentMatch8 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::SHORT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ShortConstant(lexer, context);
                                        shortConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, shortConstant.release());
                                            }
                                        }
                                        *parentMatch9 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::USHORT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch10 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::UShortConstant(lexer, context);
                                        ushortConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, ushortConstant.release());
                                            }
                                        }
                                        *parentMatch10 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::INT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::IntConstant(lexer, context);
                                        intConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, intConstant.release());
                                            }
                                        }
                                        *parentMatch11 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::UINT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::UIntConstant(lexer, context);
                                        uintConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, uintConstant.release());
                                            }
                                        }
                                        *parentMatch12 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::LONG:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch13 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::LongConstant(lexer, context);
                                        longConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, longConstant.release());
                                            }
                                        }
                                        *parentMatch13 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::ULONG:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch14 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ULongConstant(lexer, context);
                                        ulongConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, ulongConstant.release());
                                            }
                                        }
                                        *parentMatch14 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::FLOAT:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch15 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::FloatConstant(lexer, context);
                                        floatConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, floatConstant.release());
                                            }
                                        }
                                        *parentMatch15 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                                case otava::intermediate::token::DOUBLE:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch16 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::DoubleConstant(lexer, context);
                                        doubleConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, doubleConstant.release());
                                            }
                                        }
                                        *parentMatch16 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch5 = match;
                                    }
                                    break;
                                }
                            }
                            *parentMatch4 = match;
                            if (!match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch17 = &match;
                                lexer.SetPos(save);
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch18 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::AddressConstant(lexer, context);
                                        addressConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                                #endif
                                                return soul::parser::Match(true, addressConstant.release());
                                            }
                                        }
                                        *parentMatch18 = match;
                                    }
                                    *parentMatch17 = match;
                                }
                                *parentMatch4 = match;
                            }
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case otava::intermediate::token::LBRACKET:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch19 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ArrayConstant(lexer, context, type);
                            arrayConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, arrayConstant.release());
                                }
                            }
                            *parentMatch19 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case otava::intermediate::token::LBRACE:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch20 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::StructureConstant(lexer, context, type);
                            structureConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, structureConstant.release());
                                }
                            }
                            *parentMatch20 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case otava::intermediate::token::STRING:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch21 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::StringConstant(lexer, context);
                            stringConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, stringConstant.release());
                                }
                            }
                            *parentMatch21 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case otava::intermediate::token::ID:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch22 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::StringArrayConstant(lexer, context);
                            stringArrayConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, stringArrayConstant.release());
                                }
                            }
                            *parentMatch22 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch23 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch24 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ConversionConstant(lexer, context);
                            conversionConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                    #endif
                                    return soul::parser::Match(true, conversionConstant.release());
                                }
                            }
                            *parentMatch24 = match;
                        }
                        *parentMatch23 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch25 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch26 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ClsIdConstant(lexer, context);
                        clsidConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                                #endif
                                return soul::parser::Match(true, clsidConstant.release());
                            }
                        }
                        *parentMatch26 = match;
                    }
                    *parentMatch25 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch27 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch28 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::SymbolConstant(lexer, context);
                    symbolConstant.reset(static_cast<otava::intermediate::Value*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
                            #endif
                            return soul::parser::Match(true, symbolConstant.release());
                        }
                    }
                    *parentMatch28 = match;
                }
                *parentMatch27 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Constant");
        else soul::lexer::WriteFailureToLog(lexer, "Constant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::BoolConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BoolConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303757);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::BOOL)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                switch (*lexer)
                {
                    case otava::intermediate::token::TRUE:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::TRUE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BoolConstant");
                                    #endif
                                    return soul::parser::Match(true, context->GetTrueValue());
                                }
                            }
                            *parentMatch4 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case otava::intermediate::token::FALSE:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::FALSE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BoolConstant");
                                    #endif
                                    return soul::parser::Match(true, context->GetFalseValue());
                                }
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BoolConstant");
        else soul::lexer::WriteFailureToLog(lexer, "BoolConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::SByteConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SByteConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303758);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::SBYTE)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SByteConstant");
                        #endif
                        return soul::parser::Match(true, context->GetSByteValue(lexer.GetToken(pos).ToSByte()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SByteConstant");
        else soul::lexer::WriteFailureToLog(lexer, "SByteConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ByteConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ByteConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303759);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::BYTE)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ByteConstant");
                        #endif
                        return soul::parser::Match(true, context->GetByteValue(lexer.GetToken(pos).ToByte()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ByteConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ByteConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ShortConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ShortConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303760);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::SHORT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ShortConstant");
                        #endif
                        return soul::parser::Match(true, context->GetShortValue(lexer.GetToken(pos).ToShort()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ShortConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ShortConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::UShortConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UShortConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303761);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::USHORT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UShortConstant");
                        #endif
                        return soul::parser::Match(true, context->GetUShortValue(lexer.GetToken(pos).ToUShort()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UShortConstant");
        else soul::lexer::WriteFailureToLog(lexer, "UShortConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::IntConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "IntConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303762);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::INT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IntConstant");
                        #endif
                        return soul::parser::Match(true, context->GetIntValue(lexer.GetToken(pos).ToInt()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IntConstant");
        else soul::lexer::WriteFailureToLog(lexer, "IntConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::UIntConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UIntConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303763);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::UINT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UIntConstant");
                        #endif
                        return soul::parser::Match(true, context->GetUIntValue(lexer.GetToken(pos).ToUInt()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UIntConstant");
        else soul::lexer::WriteFailureToLog(lexer, "UIntConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::LongConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LongConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303764);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::LONG)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LongConstant");
                        #endif
                        return soul::parser::Match(true, context->GetLongValue(lexer.GetToken(pos).ToLong()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LongConstant");
        else soul::lexer::WriteFailureToLog(lexer, "LongConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ULongConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ULongConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303765);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::ULONG)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ULongConstant");
                        #endif
                        return soul::parser::Match(true, context->GetULongValue(lexer.GetToken(pos).ToULong()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ULongConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ULongConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::FloatConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FloatConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303766);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::FLOAT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FloatConstant");
                        #endif
                        return soul::parser::Match(true, context->GetFloatValue(lexer.GetToken(pos).ToFloat()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FloatConstant");
        else soul::lexer::WriteFailureToLog(lexer, "FloatConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::DoubleConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DoubleConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303767);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::DOUBLE)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DoubleConstant");
                        #endif
                        return soul::parser::Match(true, context->GetDoubleValue(lexer.GetToken(pos).ToDouble()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DoubleConstant");
        else soul::lexer::WriteFailureToLog(lexer, "DoubleConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::AddressConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "AddressConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303768);
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
            typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
            if (match.hit)
            {
                tref = typeRef->value;
                context->ResolveType(tref);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                switch (*lexer)
                {
                    case otava::intermediate::token::NULL_TOKEN:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::NULL_TOKEN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AddressConstant");
                                    #endif
                                    return soul::parser::Match(true, context->GetNullValue(lexer.GetSpan(pos), tref.GetType()));
                                }
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch4 = match;
                        }
                        break;
                    }
                    case otava::intermediate::token::ID:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::ID)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AddressConstant");
                                    #endif
                                    return soul::parser::Match(true, context->MakeAddressLiteral(lexer.GetSpan(pos), tref.GetType(), util::ToUtf8(lexer.GetToken(pos).ToString()), false));
                                }
                            }
                            *parentMatch6 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch4 = match;
                        }
                        break;
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "AddressConstant");
        else soul::lexer::WriteFailureToLog(lexer, "AddressConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ArrayConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ArrayConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303769);
    otava::intermediate::ArrayType* arrayType = nullptr;
    otava::intermediate::Type* elementType = nullptr;
    soul::ast::Span span = soul::ast::Span();
    std::vector<otava::intermediate::Value*> elements = std::vector<otava::intermediate::Value*>();
    std::unique_ptr<otava::intermediate::Value> first;
    std::unique_ptr<otava::intermediate::Value> next;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::LBRACKET)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    span = lexer.GetSpan(pos);
                    arrayType = static_cast<otava::intermediate::ArrayType*>(type->BaseType());
                    elementType = arrayType->ElementType();
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(true);
                std::int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Constant(lexer, context, elementType);
                                first.reset(static_cast<otava::intermediate::Value*>(match.value));
                                if (match.hit)
                                {
                                    elements.push_back(first.release());
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch8 = &match;
                            {
                                soul::parser::Match match(true);
                                soul::parser::Match* parentMatch9 = &match;
                                {
                                    while (true)
                                    {
                                        std::int64_t save = lexer.GetPos();
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch11 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::intermediate::token::COMMA)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    *parentMatch11 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch12 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch13 = &match;
                                                        {
                                                            std::int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Constant(lexer, context, elementType);
                                                            next.reset(static_cast<otava::intermediate::Value*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                elements.push_back(next.release());
                                                            }
                                                            *parentMatch13 = match;
                                                        }
                                                        *parentMatch12 = match;
                                                    }
                                                    *parentMatch11 = match;
                                                }
                                                *parentMatch10 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch9 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                                break;
                                            }
                                        }
                                    }
                                }
                                *parentMatch8 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch4 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch14 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch15 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RBRACKET)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArrayConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeArrayValue(span, elements, arrayType));
                    }
                }
                *parentMatch15 = match;
            }
            *parentMatch14 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArrayConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ArrayConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::StructureConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StructureConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303770);
    otava::intermediate::StructureType* structureType = nullptr;
    int index = int();
    soul::ast::Span span = soul::ast::Span();
    std::vector<otava::intermediate::Value*> fieldValues = std::vector<otava::intermediate::Value*>();
    std::unique_ptr<otava::intermediate::Value> first;
    std::unique_ptr<otava::intermediate::Value> next;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::LBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    span = lexer.GetSpan(pos);
                    structureType = static_cast<otava::intermediate::StructureType*>(type->BaseType());
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(true);
                std::int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Constant(lexer, context, structureType->FieldType(index++));
                                first.reset(static_cast<otava::intermediate::Value*>(match.value));
                                if (match.hit)
                                {
                                    fieldValues.push_back(first.release());
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch8 = &match;
                            {
                                soul::parser::Match match(true);
                                soul::parser::Match* parentMatch9 = &match;
                                {
                                    while (true)
                                    {
                                        std::int64_t save = lexer.GetPos();
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch11 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::intermediate::token::COMMA)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    *parentMatch11 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch12 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch13 = &match;
                                                        {
                                                            std::int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Constant(lexer, context, structureType->FieldType(index++));
                                                            next.reset(static_cast<otava::intermediate::Value*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                fieldValues.push_back(next.release());
                                                            }
                                                            *parentMatch13 = match;
                                                        }
                                                        *parentMatch12 = match;
                                                    }
                                                    *parentMatch11 = match;
                                                }
                                                *parentMatch10 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch9 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                                break;
                                            }
                                        }
                                    }
                                }
                                *parentMatch8 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch4 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch14 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch15 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StructureConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeStructureValue(span, fieldValues, structureType));
                    }
                }
                *parentMatch15 = match;
            }
            *parentMatch14 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StructureConstant");
        else soul::lexer::WriteFailureToLog(lexer, "StructureConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::StringConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StringConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303771);
    soul::ast::Span span = soul::ast::Span();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::STRING)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringConstant");
                #endif
                return soul::parser::Match(true, context->MakeStringValue(lexer.GetSpan(pos), util::ToUtf8(lexer.GetToken(pos).ToString()), true));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringConstant");
        else soul::lexer::WriteFailureToLog(lexer, "StringConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::StringArrayConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StringArrayConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303772);
    soul::ast::Span span = soul::ast::Span();
    std::vector<otava::intermediate::Value*> elements = std::vector<otava::intermediate::Value*>();
    std::unique_ptr<soul::parser::Value<char>> prefix;
    std::unique_ptr<otava::intermediate::Value> first;
    std::unique_ptr<otava::intermediate::Value> next;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::StringArrayPrefix(lexer);
                    prefix.reset(static_cast<soul::parser::Value<char>*>(match.value));
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::LBRACKET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(true);
                std::int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Constant(lexer, context, nullptr);
                                first.reset(static_cast<otava::intermediate::Value*>(match.value));
                                if (match.hit)
                                {
                                    elements.push_back(first.release());
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch8 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch10 = &match;
                            {
                                soul::parser::Match match(true);
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    while (true)
                                    {
                                        std::int64_t save = lexer.GetPos();
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch12 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch13 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::intermediate::token::COMMA)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    *parentMatch13 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch14 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch15 = &match;
                                                        {
                                                            std::int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Constant(lexer, context, nullptr);
                                                            next.reset(static_cast<otava::intermediate::Value*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                elements.push_back(next.release());
                                                            }
                                                            *parentMatch15 = match;
                                                        }
                                                        *parentMatch14 = match;
                                                    }
                                                    *parentMatch13 = match;
                                                }
                                                *parentMatch12 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch11 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                                break;
                                            }
                                        }
                                    }
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch6 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch16 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch17 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RBRACKET)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringArrayConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeStringArrayValue(span, prefix->value, elements));
                    }
                }
                *parentMatch17 = match;
            }
            *parentMatch16 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringArrayConstant");
        else soul::lexer::WriteFailureToLog(lexer, "StringArrayConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::StringArrayPrefix(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StringArrayPrefix");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303773);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        bool pass = true;
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::ID)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            std::string p = util::ToUtf8(lexer.GetToken(pos).ToString());
            pass = p == "u" || p == "w" || p == "b";
            if (pass)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringArrayPrefix");
                    #endif
                    return soul::parser::Match(true, new soul::parser::Value<char>(p[0]));
                }
            }
        }
        if (match.hit && !pass)
        {
            match = soul::parser::Match(false);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringArrayPrefix");
        else soul::lexer::WriteFailureToLog(lexer, "StringArrayPrefix");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ConversionConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConversionConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303774);
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> from;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                        typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                        if (match.hit)
                        {
                            tref = typeRef->value;
                            context->ResolveType(tref);
                            span = lexer.GetSpan(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::CONV)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::LPAREN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Constant(lexer, context, nullptr);
                from.reset(static_cast<otava::intermediate::Value*>(match.value));
                *parentMatch7 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch8 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RPAREN)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeConversionValue(span, tref.GetType(), from.release()));
                    }
                }
                *parentMatch9 = match;
            }
            *parentMatch8 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ConversionConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ClsIdConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ClsIdConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303775);
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
            typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
            if (match.hit)
            {
                tref = typeRef->value;
                context->ResolveType(tref);
                span = lexer.GetSpan(pos);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::CLSID)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClsIdConstant");
                        #endif
                        return soul::parser::Match(true, context->MakeClsIdValue(span, tref.GetType(), util::ToUtf8(lexer.GetToken(pos).ToString())));
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ClsIdConstant");
        else soul::lexer::WriteFailureToLog(lexer, "ClsIdConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::SymbolConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SymbolConstant");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303776);
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> symbolValue;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
            typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
            if (match.hit)
            {
                tref = typeRef->value;
                context->ResolveType(tref);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::SymbolValue(lexer, context, tref.GetType());
                symbolValue.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolConstant");
                        #endif
                        return soul::parser::Match(true, symbolValue.release());
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolConstant");
        else soul::lexer::WriteFailureToLog(lexer, "SymbolConstant");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::Value(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Value");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303777);
    std::unique_ptr<otava::intermediate::Value> regValue;
    std::unique_ptr<otava::intermediate::Value> symbolValue;
    std::unique_ptr<otava::intermediate::Value> literalValue;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::intermediate::token::DOLLAR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::RegValue(lexer, context, type);
                regValue.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, regValue.release());
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::AT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::SymbolValue(lexer, context, type);
                symbolValue.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, symbolValue.release());
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::ID:
        case otava::intermediate::token::TRUE:
        case otava::intermediate::token::FALSE:
        case otava::intermediate::token::NULL_TOKEN:
        case otava::intermediate::token::NUMBER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::LiteralValue(lexer, context, type);
                literalValue.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
                        #endif
                        return soul::parser::Match(true, literalValue.release());
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Value");
        else soul::lexer::WriteFailureToLog(lexer, "Value");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::RegValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RegValue");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303778);
    soul::ast::Span span = soul::ast::Span();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::DOLLAR)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                span = lexer.GetSpan(pos);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    span.Union(lexer.GetSpan(pos));
                    std::int32_t reg = lexer.GetToken(pos).ToInt();
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RegValue");
                        #endif
                        return soul::parser::Match(true, context->CurrentFunction()->GetRegRef(span, type, reg, context));
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RegValue");
        else soul::lexer::WriteFailureToLog(lexer, "RegValue");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ResultRegValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ResultRegValue");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303779);
    soul::ast::Span span = soul::ast::Span();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::DOLLAR)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                span = lexer.GetSpan(pos);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    span.Union(lexer.GetSpan(pos));
                    std::int32_t reg = lexer.GetToken(pos).ToInt();
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ResultRegValue");
                        #endif
                        return soul::parser::Match(true, context->CurrentFunction()->MakeRegValue(span, type, reg, context));
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ResultRegValue");
        else soul::lexer::WriteFailureToLog(lexer, "ResultRegValue");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::SymbolValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SymbolValue");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303780);
    soul::ast::Span span = soul::ast::Span();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::AT)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                span = lexer.GetSpan(pos);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::ID)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    span.Union(lexer.GetSpan(pos));
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolValue");
                        #endif
                        return soul::parser::Match(true, context->MakeSymbolValue(span, type, util::ToUtf8(lexer.GetToken(pos).ToString())));
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SymbolValue");
        else soul::lexer::WriteFailureToLog(lexer, "SymbolValue");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::LiteralValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LiteralValue");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303781);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::intermediate::token::TRUE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::TRUE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->GetBooleanLiteral(lexer.GetSpan(pos), type, true));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::FALSE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::FALSE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->GetBooleanLiteral(lexer.GetSpan(pos), type, false));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::NULL_TOKEN:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NULL_TOKEN)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->GetNullValue(lexer.GetSpan(pos), type));
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::NUMBER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->MakeIntegerLiteral(lexer.GetSpan(pos), type, util::ToUtf8(lexer.GetToken(pos).ToString())));
                    }
                }
                *parentMatch4 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::ID:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::ID)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
                        #endif
                        return soul::parser::Match(true, context->MakeAddressLiteral(lexer.GetSpan(pos), type, util::ToUtf8(lexer.GetToken(pos).ToString()), true));
                    }
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralValue");
        else soul::lexer::WriteFailureToLog(lexer, "LiteralValue");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::FunctionDefinitions(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionDefinitions");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303782);
    soul::parser::Match match(true);
    soul::parser::Match* parentMatch0 = &match;
    {
        while (true)
        {
            std::int64_t save = lexer.GetPos();
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch1 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch2 = &match;
                    {
                        std::int64_t save = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::FunctionDeclaration(lexer, context);
                        *parentMatch2 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch3 = &match;
                            lexer.SetPos(save);
                            {
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::FunctionDefinition(lexer, context);
                                *parentMatch3 = match;
                            }
                            *parentMatch2 = match;
                        }
                    }
                    *parentMatch1 = match;
                }
                if (match.hit)
                {
                    *parentMatch0 = match;
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionDefinitions");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionDefinitions");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::FunctionDeclaration(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303783);
    otava::intermediate::TypeRef functionTypeRef = otava::intermediate::TypeRef();
    soul::ast::Span span = soul::ast::Span();
    std::string functionId = std::string();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::EXTERN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                span = lexer.GetSpan(pos);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::FUNCTION)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                            typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                            if (match.hit)
                            {
                                functionTypeRef = typeRef->value;
                                context->ResolveType(functionTypeRef);
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::ID)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            functionId = util::ToUtf8(lexer.GetToken(pos).ToString());
                        }
                        *parentMatch10 = match;
                    }
                    *parentMatch9 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            context->AddFunctionDeclaration(span, functionTypeRef.GetType(), functionId);
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::FunctionDefinition(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionDefinition");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303784);
    otava::intermediate::Function* function = nullptr;
    std::unique_ptr<otava::intermediate::Function> header;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::FunctionHeader(lexer, context);
                    header.reset(static_cast<otava::intermediate::Function*>(match.value));
                    if (match.hit)
                    {
                        function = header.release();
                        context->SetCurrentFunction(function);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch6 = &match;
                {
                    while (true)
                    {
                        std::int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::BasicBlock(lexer, context, function);
                            if (match.hit)
                            {
                                *parentMatch6 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                                break;
                            }
                        }
                    }
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch7 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::RBRACE)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch7 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionDefinition");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionDefinition");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::FunctionHeader(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionHeader");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303785);
    otava::intermediate::TypeRef functionTypeRef = otava::intermediate::TypeRef();
    soul::ast::Span span = soul::ast::Span();
    std::string functionId = std::string();
    bool inline_ = bool();
    bool linkOnce = bool();
    bool child = bool();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::MetadataRef> mdRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(true);
                                    std::int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == otava::intermediate::token::INLINE)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    inline_ = true;
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch8 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                        }
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        soul::parser::Match match(true);
                                        std::int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch12 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch13 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch14 = &match;
                                                {
                                                    std::int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::intermediate::token::LINK_ONCE)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        linkOnce = true;
                                                    }
                                                    *parentMatch14 = match;
                                                }
                                                *parentMatch13 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch12 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                            }
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch15 = &match;
                                {
                                    soul::parser::Match match(true);
                                    std::int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch16 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch17 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch18 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == otava::intermediate::token::CHILD)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    child = true;
                                                }
                                                *parentMatch18 = match;
                                            }
                                            *parentMatch17 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch16 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                        }
                                    }
                                    *parentMatch15 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch19 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch20 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::FUNCTION)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch20 = match;
                                }
                                *parentMatch19 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch21 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch22 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                if (match.hit)
                                {
                                    functionTypeRef = typeRef->value;
                                    context->ResolveType(functionTypeRef);
                                }
                                *parentMatch22 = match;
                            }
                            *parentMatch21 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch23 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch24 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::ID)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                functionId = util::ToUtf8(lexer.GetToken(pos).ToString());
                            }
                            *parentMatch24 = match;
                        }
                        *parentMatch23 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch25 = &match;
                {
                    soul::parser::Match match(true);
                    std::int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch26 = &match;
                    {
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataRef(lexer, context);
                        mdRef.reset(static_cast<otava::intermediate::MetadataRef*>(match.value));
                        if (match.hit)
                        {
                            *parentMatch26 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                        }
                    }
                    *parentMatch25 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionHeader");
                #endif
                return soul::parser::Match(true, context->AddFunctionDefinition(span, functionTypeRef.GetType(), functionId, inline_, linkOnce, child, false, mdRef.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionHeader");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionHeader");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::BasicBlock(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Function* function)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BasicBlock");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303786);
    otava::intermediate::BasicBlock* basicBlock = nullptr;
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<std::int32_t>> id;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Label(lexer);
            id.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
            if (match.hit)
            {
                span = lexer.GetSpan(pos);
                basicBlock = function->AddBasicBlock(span, id->value, context);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Instructions(lexer, context, basicBlock);
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BasicBlock");
        else soul::lexer::WriteFailureToLog(lexer, "BasicBlock");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::Label(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Label");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303787);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::AT)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Label");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<std::int32_t>(lexer.GetToken(pos).ToInt()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Label");
        else soul::lexer::WriteFailureToLog(lexer, "Label");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::Operand(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Operand");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303788);
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> value;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
            typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
            if (match.hit)
            {
                tref = typeRef->value;
                context->ResolveType(tref);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, tref.GetType());
                value.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operand");
                        #endif
                        return soul::parser::Match(true, value.release());
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operand");
        else soul::lexer::WriteFailureToLog(lexer, "Operand");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::Instructions(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::BasicBlock* basicBlock)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Instructions");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303789);
    std::unique_ptr<otava::intermediate::Instruction> instruction;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Instruction(lexer, context);
                instruction.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    basicBlock->AddInstruction(instruction.release());
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(true);
        soul::parser::Match* parentMatch3 = &match;
        while (true)
        {
            std::int64_t save = lexer.GetPos();
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Instruction(lexer, context);
                        instruction.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                        if (match.hit)
                        {
                            basicBlock->AddInstruction(instruction.release());
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                }
                if (match.hit)
                {
                    *parentMatch3 = match;
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instructions");
        else soul::lexer::WriteFailureToLog(lexer, "Instructions");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::Instruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Instruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303790);
    std::unique_ptr<otava::intermediate::Instruction> store;
    std::unique_ptr<otava::intermediate::Instruction> arg;
    std::unique_ptr<otava::intermediate::Instruction> jmp;
    std::unique_ptr<otava::intermediate::Instruction> branch;
    std::unique_ptr<otava::intermediate::Instruction> procedureCall;
    std::unique_ptr<otava::intermediate::Instruction> ret;
    std::unique_ptr<otava::intermediate::Instruction> switch_;
    std::unique_ptr<otava::intermediate::Instruction> valueInst;
    std::unique_ptr<otava::intermediate::Instruction> nopInst;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::intermediate::token::STORE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::StoreInstruction(lexer, context);
                store.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, store.release());
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::ARG:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ArgInstruction(lexer, context);
                arg.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, arg.release());
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::JMP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::JmpInstruction(lexer, context);
                jmp.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, jmp.release());
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::BRANCH:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::BranchInstruction(lexer, context);
                branch.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, branch.release());
                    }
                }
                *parentMatch4 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::CALL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ProcedureCallInstruction(lexer, context);
                procedureCall.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, procedureCall.release());
                    }
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::RET:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::RetInstruction(lexer, context);
                ret.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, ret.release());
                    }
                }
                *parentMatch6 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::SWITCH:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::SwitchInstruction(lexer, context);
                switch_.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, switch_.release());
                    }
                }
                *parentMatch7 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::VOID:
        case otava::intermediate::token::BOOL:
        case otava::intermediate::token::SBYTE:
        case otava::intermediate::token::BYTE:
        case otava::intermediate::token::SHORT:
        case otava::intermediate::token::USHORT:
        case otava::intermediate::token::INT:
        case otava::intermediate::token::UINT:
        case otava::intermediate::token::LONG:
        case otava::intermediate::token::ULONG:
        case otava::intermediate::token::FLOAT:
        case otava::intermediate::token::DOUBLE:
        case otava::intermediate::token::TYPEID:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ValueInstruction(lexer, context);
                valueInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, valueInst.release());
                    }
                }
                *parentMatch8 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::NOP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::NoOperationInstruction(lexer, context);
                nopInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
                        #endif
                        return soul::parser::Match(true, nopInst.release());
                    }
                }
                *parentMatch9 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Instruction");
        else soul::lexer::WriteFailureToLog(lexer, "Instruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::StoreInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StoreInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303791);
    soul::ast::Span span = soul::ast::Span();
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    otava::intermediate::TypeRef ptref = otava::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> value;
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> ptrTypeRef;
    std::unique_ptr<otava::intermediate::Value> ptr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::STORE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                span = lexer.GetSpan(pos);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                if (match.hit)
                                {
                                    tref = typeRef->value;
                                    context->ResolveType(tref);
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, tref.GetType());
                        value.reset(static_cast<otava::intermediate::Value*>(match.value));
                        *parentMatch8 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::COMMA)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch9 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch11 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                    ptrTypeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        ptref = ptrTypeRef->value;
                        context->ResolveType(ptref);
                    }
                    *parentMatch11 = match;
                }
                *parentMatch10 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch12 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch13 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, ptref.GetType());
                ptr.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StoreInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::StoreInstruction(span, value.release(), ptr.release()));
                    }
                }
                *parentMatch13 = match;
            }
            *parentMatch12 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StoreInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "StoreInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ArgInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ArgInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303792);
    soul::ast::Span span = soul::ast::Span();
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> arg;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::ARG)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    span = lexer.GetSpan(pos);
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                    typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        tref = typeRef->value;
                        context->ResolveType(tref);
                    }
                    *parentMatch4 = match;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch5 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, tref.GetType());
                arg.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArgInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::ArgInstruction(span, arg.release()));
                    }
                }
                *parentMatch6 = match;
            }
            *parentMatch5 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArgInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ArgInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::JmpInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "JmpInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303793);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<std::int32_t>> target;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::JMP)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                span = lexer.GetSpan(pos);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Label(lexer);
                target.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JmpInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::JmpInstruction(span, target->value));
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JmpInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "JmpInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::BranchInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BranchInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303794);
    soul::ast::Span span = soul::ast::Span();
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> cond;
    std::unique_ptr<soul::parser::Value<std::int32_t>> trueTarget;
    std::unique_ptr<soul::parser::Value<std::int32_t>> falseTarget;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::BRANCH)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    span = lexer.GetSpan(pos);
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                    typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                    if (match.hit)
                                    {
                                        tref = typeRef->value;
                                        context->ResolveType(tref);
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, tref.GetType());
                            cond.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch9 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::COMMA)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch10 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch11 = &match;
                {
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Label(lexer);
                    trueTarget.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                    *parentMatch11 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch12 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::COMMA)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch12 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch13 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch14 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Label(lexer);
                falseTarget.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BranchInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::BranchInstruction(span, cond.release(), trueTarget->value, falseTarget->value));
                    }
                }
                *parentMatch14 = match;
            }
            *parentMatch13 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BranchInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "BranchInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ProcedureCallInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ProcedureCallInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303795);
    soul::ast::Span span = soul::ast::Span();
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> callee;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::CALL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    span = lexer.GetSpan(pos);
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                    typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        tref = typeRef->value;
                        context->ResolveType(tref);
                    }
                    *parentMatch4 = match;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch5 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, tref.GetType());
                callee.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcedureCallInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::ProcedureCallInstruction(span, callee.release()));
                    }
                }
                *parentMatch6 = match;
            }
            *parentMatch5 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcedureCallInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ProcedureCallInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::RetInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RetInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303796);
    soul::ast::Span span = soul::ast::Span();
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> returnValue;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::RET)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                span = lexer.GetSpan(pos);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    std::int64_t save = lexer.GetPos();
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            std::int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::VOID)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch7 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    std::int64_t tmp = lexer.GetPos();
                                    lexer.SetPos(save);
                                    save = tmp;
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == otava::intermediate::token::VOID)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch11 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == otava::intermediate::token::AST)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch11 = match;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch8 = match;
                                }
                                if (!match.hit)
                                {
                                    lexer.SetPos(save);
                                }
                                *parentMatch7 = soul::parser::Match(!match.hit, match.value);
                            }
                            *parentMatch6 = match;
                        }
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RetInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::RetInstruction(span, nullptr));
                            }
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                    if (!match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        lexer.SetPos(save);
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                    typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                    if (match.hit)
                                    {
                                        tref = typeRef->value;
                                        context->ResolveType(tref);
                                    }
                                    *parentMatch14 = match;
                                }
                                *parentMatch13 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch15 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch16 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, tref.GetType());
                                        returnValue.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RetInstruction");
                                                #endif
                                                return soul::parser::Match(true, new otava::intermediate::RetInstruction(span, returnValue.release()));
                                            }
                                        }
                                        *parentMatch16 = match;
                                    }
                                    *parentMatch15 = match;
                                }
                                *parentMatch13 = match;
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch4 = match;
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RetInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "RetInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::SwitchInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SwitchInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303797);
    soul::ast::Span span = soul::ast::Span();
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    otava::intermediate::TypeRef caseTref = otava::intermediate::TypeRef();
    otava::intermediate::SwitchInstruction* inst = nullptr;
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> cond;
    std::unique_ptr<soul::parser::Value<std::int32_t>> defaultTarget;
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> caseTypeRef;
    std::unique_ptr<otava::intermediate::Value> caseValue;
    std::unique_ptr<soul::parser::Value<std::int32_t>> caseTarget;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            std::int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == otava::intermediate::token::SWITCH)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                span = lexer.GetSpan(pos);
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch11 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                                typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                                if (match.hit)
                                                {
                                                    tref = typeRef->value;
                                                    context->ResolveType(tref);
                                                }
                                                *parentMatch11 = match;
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, tref.GetType());
                                        cond.reset(static_cast<otava::intermediate::Value*>(match.value));
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch13 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch14 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Label(lexer);
                                        defaultTarget.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                                        if (match.hit)
                                        {
                                            inst = new otava::intermediate::SwitchInstruction(span, cond.release(), defaultTarget->value);
                                        }
                                        *parentMatch14 = match;
                                    }
                                    *parentMatch13 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch15 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch16 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::LBRACKET)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch16 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch17 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch18 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch19 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch20 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch21 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch22 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch23 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch24 = &match;
                                                {
                                                    std::int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                                    caseTypeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        caseTref = caseTypeRef->value;
                                                        context->ResolveType(caseTref);
                                                    }
                                                    *parentMatch24 = match;
                                                }
                                                *parentMatch23 = match;
                                            }
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch25 = &match;
                                                {
                                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, caseTref.GetType());
                                                    caseValue.reset(static_cast<otava::intermediate::Value*>(match.value));
                                                    *parentMatch25 = match;
                                                }
                                                *parentMatch23 = match;
                                            }
                                            *parentMatch22 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch26 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                if (*lexer == otava::intermediate::token::COMMA)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                *parentMatch26 = match;
                                            }
                                            *parentMatch22 = match;
                                        }
                                        *parentMatch21 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch27 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch28 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Label(lexer);
                                                caseTarget.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                                                if (match.hit)
                                                {
                                                    inst->AddCaseTarget(otava::intermediate::CaseTarget(caseValue.release(), caseTarget->value));
                                                }
                                                *parentMatch28 = match;
                                            }
                                            *parentMatch27 = match;
                                        }
                                        *parentMatch21 = match;
                                    }
                                    *parentMatch20 = match;
                                }
                                *parentMatch19 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch29 = &match;
                                {
                                    soul::parser::Match match(true);
                                    soul::parser::Match* parentMatch30 = &match;
                                    {
                                        while (true)
                                        {
                                            std::int64_t save = lexer.GetPos();
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch31 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::intermediate::token::COLON)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    *parentMatch31 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch32 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch33 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch34 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch35 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch36 = &match;
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        soul::parser::Match* parentMatch37 = &match;
                                                                        {
                                                                            std::int64_t pos = lexer.GetPos();
                                                                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                                                            caseTypeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                                                            if (match.hit)
                                                                            {
                                                                                caseTref = caseTypeRef->value;
                                                                                context->ResolveType(caseTref);
                                                                            }
                                                                            *parentMatch37 = match;
                                                                        }
                                                                        *parentMatch36 = match;
                                                                    }
                                                                    if (match.hit)
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        soul::parser::Match* parentMatch38 = &match;
                                                                        {
                                                                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, caseTref.GetType());
                                                                            caseValue.reset(static_cast<otava::intermediate::Value*>(match.value));
                                                                            *parentMatch38 = match;
                                                                        }
                                                                        *parentMatch36 = match;
                                                                    }
                                                                    *parentMatch35 = match;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch39 = &match;
                                                                    {
                                                                        soul::parser::Match match(false);
                                                                        if (*lexer == otava::intermediate::token::COMMA)
                                                                        {
                                                                            ++lexer;
                                                                            match.hit = true;
                                                                        }
                                                                        *parentMatch39 = match;
                                                                    }
                                                                    *parentMatch35 = match;
                                                                }
                                                                *parentMatch34 = match;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch40 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch41 = &match;
                                                                    {
                                                                        std::int64_t pos = lexer.GetPos();
                                                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Label(lexer);
                                                                        caseTarget.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                                                                        if (match.hit)
                                                                        {
                                                                            inst->AddCaseTarget(otava::intermediate::CaseTarget(caseValue.release(), caseTarget->value));
                                                                        }
                                                                        *parentMatch41 = match;
                                                                    }
                                                                    *parentMatch40 = match;
                                                                }
                                                                *parentMatch34 = match;
                                                            }
                                                            *parentMatch33 = match;
                                                        }
                                                        *parentMatch32 = match;
                                                    }
                                                    *parentMatch31 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    *parentMatch30 = match;
                                                }
                                                else
                                                {
                                                    lexer.SetPos(save);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    *parentMatch29 = match;
                                }
                                *parentMatch19 = match;
                            }
                            *parentMatch18 = match;
                        }
                        *parentMatch17 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch42 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::RBRACKET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch42 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SwitchInstruction");
                #endif
                return soul::parser::Match(true, inst);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SwitchInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "SwitchInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ValueInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ValueInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303798);
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    otava::intermediate::RegValue* rslt = nullptr;
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::RegValue> result;
    std::unique_ptr<otava::intermediate::Instruction> inst;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                    typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        tref = typeRef->value;
                        context->ResolveType(tref);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ResultRegValue(lexer, context, tref.GetType());
                        result.reset(static_cast<otava::intermediate::RegValue*>(match.value));
                        if (match.hit)
                        {
                            rslt = result.release();
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::ASSIGN)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch6 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch7 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operation(lexer, context, rslt);
                inst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ValueInstruction");
                        #endif
                        return soul::parser::Match(true, inst.release());
                    }
                }
                *parentMatch8 = match;
            }
            *parentMatch7 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ValueInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ValueInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::Operation(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Operation");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303799);
    std::unique_ptr<otava::intermediate::Instruction> unaryInst;
    std::unique_ptr<otava::intermediate::Instruction> binaryInst;
    std::unique_ptr<otava::intermediate::Instruction> paramInst;
    std::unique_ptr<otava::intermediate::Instruction> localInst;
    std::unique_ptr<otava::intermediate::Instruction> plocalInst;
    std::unique_ptr<otava::intermediate::Instruction> loadInst;
    std::unique_ptr<otava::intermediate::Instruction> elemAddrInst;
    std::unique_ptr<otava::intermediate::Instruction> ptrOffsetInst;
    std::unique_ptr<otava::intermediate::Instruction> ptrDiffInst;
    std::unique_ptr<otava::intermediate::Instruction> getRbpInst;
    std::unique_ptr<otava::intermediate::Instruction> functionCallInst;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::intermediate::token::NOT:
        case otava::intermediate::token::NEG:
        case otava::intermediate::token::SIGNEXTEND:
        case otava::intermediate::token::ZEROEXTEND:
        case otava::intermediate::token::FPEXTEND:
        case otava::intermediate::token::TRUNCATE:
        case otava::intermediate::token::BITCAST:
        case otava::intermediate::token::INTTOFLOAT:
        case otava::intermediate::token::FLOATTOINT:
        case otava::intermediate::token::INTTOPTR:
        case otava::intermediate::token::PTRTOINT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::UnaryInstruction(lexer, context, result);
                unaryInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, unaryInst.release());
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::ADD:
        case otava::intermediate::token::SUB:
        case otava::intermediate::token::MUL:
        case otava::intermediate::token::DIV:
        case otava::intermediate::token::MOD:
        case otava::intermediate::token::AND:
        case otava::intermediate::token::OR:
        case otava::intermediate::token::XOR:
        case otava::intermediate::token::SHL:
        case otava::intermediate::token::SHR:
        case otava::intermediate::token::EQUAL:
        case otava::intermediate::token::LESS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::BinaryInstruction(lexer, context, result);
                binaryInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, binaryInst.release());
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::PARAM:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ParamInstruction(lexer, context, result);
                paramInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, paramInst.release());
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::LOCAL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::LocalInstruction(lexer, context, result);
                localInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, localInst.release());
                    }
                }
                *parentMatch4 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::PLOCAL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::PLocalInstruction(lexer, context, result);
                plocalInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, plocalInst.release());
                    }
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::LOAD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::LoadInstruction(lexer, context, result);
                loadInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, loadInst.release());
                    }
                }
                *parentMatch6 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::ELEMADDR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::ElemAddrInstruction(lexer, context, result);
                elemAddrInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, elemAddrInst.release());
                    }
                }
                *parentMatch7 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::PTROFFSET:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::PtrOffsetInstruction(lexer, context, result);
                ptrOffsetInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, ptrOffsetInst.release());
                    }
                }
                *parentMatch8 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::PTRDIFF:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::PtrDiffInstruction(lexer, context, result);
                ptrDiffInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, ptrDiffInst.release());
                    }
                }
                *parentMatch9 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::GETRBP:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::GetRbpInstruction(lexer, context, result);
                getRbpInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, getRbpInst.release());
                    }
                }
                *parentMatch10 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::CALL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::FunctionCallInstruction(lexer, context, result);
                functionCallInst.reset(static_cast<otava::intermediate::Instruction*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
                        #endif
                        return soul::parser::Match(true, functionCallInst.release());
                    }
                }
                *parentMatch11 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operation");
        else soul::lexer::WriteFailureToLog(lexer, "Operation");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::UnaryInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UnaryInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303800);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::intermediate::Value> notOp;
    std::unique_ptr<otava::intermediate::Value> negOp;
    std::unique_ptr<otava::intermediate::Value> sextOp;
    std::unique_ptr<otava::intermediate::Value> zextOp;
    std::unique_ptr<otava::intermediate::Value> fpextOp;
    std::unique_ptr<otava::intermediate::Value> truncOp;
    std::unique_ptr<otava::intermediate::Value> bcOp;
    std::unique_ptr<otava::intermediate::Value> ifOp;
    std::unique_ptr<otava::intermediate::Value> fiOp;
    std::unique_ptr<otava::intermediate::Value> ipOp;
    std::unique_ptr<otava::intermediate::Value> piOp;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::intermediate::token::NOT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::NOT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch2 = match;
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        notOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::NotInstruction(span, result, notOp.release()));
                            }
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::NEG:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::NEG)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch6 = match;
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        negOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::NegInstruction(span, result, negOp.release()));
                            }
                        }
                        *parentMatch8 = match;
                    }
                    *parentMatch7 = match;
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::SIGNEXTEND:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch10 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::SIGNEXTEND)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch10 = match;
                }
                *parentMatch9 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch11 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch12 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        sextOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::SignExtendInstruction(span, result, sextOp.release()));
                            }
                        }
                        *parentMatch12 = match;
                    }
                    *parentMatch11 = match;
                }
                *parentMatch9 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::ZEROEXTEND:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch13 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch14 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::ZEROEXTEND)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch14 = match;
                }
                *parentMatch13 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch15 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch16 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        zextOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::ZeroExtendInstruction(span, result, zextOp.release()));
                            }
                        }
                        *parentMatch16 = match;
                    }
                    *parentMatch15 = match;
                }
                *parentMatch13 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::FPEXTEND:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch17 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch18 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::FPEXTEND)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch18 = match;
                }
                *parentMatch17 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch19 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch20 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        fpextOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::FloatingPointExtendInstruction(span, result, fpextOp.release()));
                            }
                        }
                        *parentMatch20 = match;
                    }
                    *parentMatch19 = match;
                }
                *parentMatch17 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::TRUNCATE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch21 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch22 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::TRUNCATE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch22 = match;
                }
                *parentMatch21 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch23 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch24 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        truncOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::TruncateInstruction(span, result, truncOp.release()));
                            }
                        }
                        *parentMatch24 = match;
                    }
                    *parentMatch23 = match;
                }
                *parentMatch21 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::BITCAST:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch25 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch26 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::BITCAST)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch26 = match;
                }
                *parentMatch25 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch27 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch28 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        bcOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::BitcastInstruction(span, result, bcOp.release()));
                            }
                        }
                        *parentMatch28 = match;
                    }
                    *parentMatch27 = match;
                }
                *parentMatch25 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::INTTOFLOAT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch29 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch30 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::INTTOFLOAT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch30 = match;
                }
                *parentMatch29 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch31 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch32 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        ifOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::IntToFloatInstruction(span, result, ifOp.release()));
                            }
                        }
                        *parentMatch32 = match;
                    }
                    *parentMatch31 = match;
                }
                *parentMatch29 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::FLOATTOINT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch33 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch34 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::FLOATTOINT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch34 = match;
                }
                *parentMatch33 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch35 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch36 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        fiOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::FloatToIntInstruction(span, result, fiOp.release()));
                            }
                        }
                        *parentMatch36 = match;
                    }
                    *parentMatch35 = match;
                }
                *parentMatch33 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::INTTOPTR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch37 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch38 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::INTTOPTR)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch38 = match;
                }
                *parentMatch37 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch39 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch40 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        ipOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::IntToPtrInstruction(span, result, ipOp.release()));
                            }
                        }
                        *parentMatch40 = match;
                    }
                    *parentMatch39 = match;
                }
                *parentMatch37 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::PTRTOINT:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch41 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch42 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::PTRTOINT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch42 = match;
                }
                *parentMatch41 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch43 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch44 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                        piOp.reset(static_cast<otava::intermediate::Value*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
                                #endif
                                return soul::parser::Match(true, new otava::intermediate::PtrToIntInstruction(span, result, piOp.release()));
                            }
                        }
                        *parentMatch44 = match;
                    }
                    *parentMatch43 = match;
                }
                *parentMatch41 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UnaryInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "UnaryInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::BinaryInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BinaryInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303801);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::intermediate::Value> leftAdd;
    std::unique_ptr<otava::intermediate::Value> rightAdd;
    std::unique_ptr<otava::intermediate::Value> leftSub;
    std::unique_ptr<otava::intermediate::Value> rightSub;
    std::unique_ptr<otava::intermediate::Value> leftMul;
    std::unique_ptr<otava::intermediate::Value> rightMul;
    std::unique_ptr<otava::intermediate::Value> leftDiv;
    std::unique_ptr<otava::intermediate::Value> rightDiv;
    std::unique_ptr<otava::intermediate::Value> leftMod;
    std::unique_ptr<otava::intermediate::Value> rightMod;
    std::unique_ptr<otava::intermediate::Value> leftAnd;
    std::unique_ptr<otava::intermediate::Value> rightAnd;
    std::unique_ptr<otava::intermediate::Value> leftOr;
    std::unique_ptr<otava::intermediate::Value> rightOr;
    std::unique_ptr<otava::intermediate::Value> leftXor;
    std::unique_ptr<otava::intermediate::Value> rightXor;
    std::unique_ptr<otava::intermediate::Value> leftShl;
    std::unique_ptr<otava::intermediate::Value> rightShl;
    std::unique_ptr<otava::intermediate::Value> leftShr;
    std::unique_ptr<otava::intermediate::Value> rightShr;
    std::unique_ptr<otava::intermediate::Value> leftEqual;
    std::unique_ptr<otava::intermediate::Value> rightEqual;
    std::unique_ptr<otava::intermediate::Value> leftLess;
    std::unique_ptr<otava::intermediate::Value> rightLess;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::intermediate::token::ADD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch3 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch5 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch6 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::ADD)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch6 = match;
                                }
                                *parentMatch5 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftAdd.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch7 = match;
                                }
                                *parentMatch5 = match;
                            }
                            *parentMatch4 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch8 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch8 = match;
                            }
                            *parentMatch4 = match;
                        }
                        *parentMatch3 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightAdd.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch9 = match;
                        }
                        *parentMatch3 = match;
                    }
                    *parentMatch2 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::AddInstruction(span, result, leftAdd.release(), rightAdd.release()));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::SUB:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch11 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch12 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch13 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch14 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch15 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::SUB)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch15 = match;
                                }
                                *parentMatch14 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch16 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftSub.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch16 = match;
                                }
                                *parentMatch14 = match;
                            }
                            *parentMatch13 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch17 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch17 = match;
                            }
                            *parentMatch13 = match;
                        }
                        *parentMatch12 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch18 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightSub.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch18 = match;
                        }
                        *parentMatch12 = match;
                    }
                    *parentMatch11 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::SubInstruction(span, result, leftSub.release(), rightSub.release()));
                    }
                }
                *parentMatch10 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::MUL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch19 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch20 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch21 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch22 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch23 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch24 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::MUL)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch24 = match;
                                }
                                *parentMatch23 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch25 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftMul.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch25 = match;
                                }
                                *parentMatch23 = match;
                            }
                            *parentMatch22 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch26 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch26 = match;
                            }
                            *parentMatch22 = match;
                        }
                        *parentMatch21 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch27 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightMul.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch27 = match;
                        }
                        *parentMatch21 = match;
                    }
                    *parentMatch20 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::MulInstruction(span, result, leftMul.release(), rightMul.release()));
                    }
                }
                *parentMatch19 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::DIV:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch28 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch29 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch30 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch31 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch32 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch33 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::DIV)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch33 = match;
                                }
                                *parentMatch32 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch34 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftDiv.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch34 = match;
                                }
                                *parentMatch32 = match;
                            }
                            *parentMatch31 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch35 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch35 = match;
                            }
                            *parentMatch31 = match;
                        }
                        *parentMatch30 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch36 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightDiv.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch36 = match;
                        }
                        *parentMatch30 = match;
                    }
                    *parentMatch29 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::DivInstruction(span, result, leftDiv.release(), rightDiv.release()));
                    }
                }
                *parentMatch28 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::MOD:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch37 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch38 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch39 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch40 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch41 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch42 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::MOD)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch42 = match;
                                }
                                *parentMatch41 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch43 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftMod.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch43 = match;
                                }
                                *parentMatch41 = match;
                            }
                            *parentMatch40 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch44 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch44 = match;
                            }
                            *parentMatch40 = match;
                        }
                        *parentMatch39 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch45 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightMod.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch45 = match;
                        }
                        *parentMatch39 = match;
                    }
                    *parentMatch38 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::ModInstruction(span, result, leftMod.release(), rightMod.release()));
                    }
                }
                *parentMatch37 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::AND:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch46 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch47 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch48 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch49 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch50 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch51 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::AND)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch51 = match;
                                }
                                *parentMatch50 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch52 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftAnd.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch52 = match;
                                }
                                *parentMatch50 = match;
                            }
                            *parentMatch49 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch53 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch53 = match;
                            }
                            *parentMatch49 = match;
                        }
                        *parentMatch48 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch54 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightAnd.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch54 = match;
                        }
                        *parentMatch48 = match;
                    }
                    *parentMatch47 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::AndInstruction(span, result, leftAnd.release(), rightAnd.release()));
                    }
                }
                *parentMatch46 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::OR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch55 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch56 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch57 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch58 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch59 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch60 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::OR)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch60 = match;
                                }
                                *parentMatch59 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch61 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftOr.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch61 = match;
                                }
                                *parentMatch59 = match;
                            }
                            *parentMatch58 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch62 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch62 = match;
                            }
                            *parentMatch58 = match;
                        }
                        *parentMatch57 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch63 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightOr.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch63 = match;
                        }
                        *parentMatch57 = match;
                    }
                    *parentMatch56 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::OrInstruction(span, result, leftOr.release(), rightOr.release()));
                    }
                }
                *parentMatch55 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::XOR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch64 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch65 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch66 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch67 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch68 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch69 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::XOR)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch69 = match;
                                }
                                *parentMatch68 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch70 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftXor.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch70 = match;
                                }
                                *parentMatch68 = match;
                            }
                            *parentMatch67 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch71 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch71 = match;
                            }
                            *parentMatch67 = match;
                        }
                        *parentMatch66 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch72 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightXor.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch72 = match;
                        }
                        *parentMatch66 = match;
                    }
                    *parentMatch65 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::XorInstruction(span, result, leftXor.release(), rightXor.release()));
                    }
                }
                *parentMatch64 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::SHL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch73 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch74 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch75 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch76 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch77 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch78 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::SHL)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch78 = match;
                                }
                                *parentMatch77 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch79 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftShl.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch79 = match;
                                }
                                *parentMatch77 = match;
                            }
                            *parentMatch76 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch80 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch80 = match;
                            }
                            *parentMatch76 = match;
                        }
                        *parentMatch75 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch81 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightShl.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch81 = match;
                        }
                        *parentMatch75 = match;
                    }
                    *parentMatch74 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::ShlInstruction(span, result, leftShl.release(), rightShl.release()));
                    }
                }
                *parentMatch73 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::SHR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch82 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch83 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch84 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch85 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch86 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch87 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::SHR)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch87 = match;
                                }
                                *parentMatch86 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch88 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftShr.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch88 = match;
                                }
                                *parentMatch86 = match;
                            }
                            *parentMatch85 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch89 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch89 = match;
                            }
                            *parentMatch85 = match;
                        }
                        *parentMatch84 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch90 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightShr.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch90 = match;
                        }
                        *parentMatch84 = match;
                    }
                    *parentMatch83 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::ShrInstruction(span, result, leftShr.release(), rightShr.release()));
                    }
                }
                *parentMatch82 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::EQUAL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch91 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch92 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch93 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch94 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch95 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch96 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::EQUAL)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch96 = match;
                                }
                                *parentMatch95 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch97 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftEqual.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch97 = match;
                                }
                                *parentMatch95 = match;
                            }
                            *parentMatch94 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch98 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch98 = match;
                            }
                            *parentMatch94 = match;
                        }
                        *parentMatch93 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch99 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightEqual.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch99 = match;
                        }
                        *parentMatch93 = match;
                    }
                    *parentMatch92 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::EqualInstruction(span, result, leftEqual.release(), rightEqual.release()));
                    }
                }
                *parentMatch91 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::LESS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch100 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch101 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch102 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch103 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch104 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch105 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::intermediate::token::LESS)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch105 = match;
                                }
                                *parentMatch104 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch106 = &match;
                                {
                                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                    leftLess.reset(static_cast<otava::intermediate::Value*>(match.value));
                                    *parentMatch106 = match;
                                }
                                *parentMatch104 = match;
                            }
                            *parentMatch103 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch107 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::COMMA)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch107 = match;
                            }
                            *parentMatch103 = match;
                        }
                        *parentMatch102 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch108 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                            rightLess.reset(static_cast<otava::intermediate::Value*>(match.value));
                            *parentMatch108 = match;
                        }
                        *parentMatch102 = match;
                    }
                    *parentMatch101 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::LessInstruction(span, result, leftLess.release(), rightLess.release()));
                    }
                }
                *parentMatch100 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BinaryInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "BinaryInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ParamInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ParamInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303802);
    soul::ast::Span span = soul::ast::Span();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::PARAM)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            span = lexer.GetSpan(pos);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParamInstruction");
                #endif
                return soul::parser::Match(true, new otava::intermediate::ParamInstruction(span, result));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParamInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ParamInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::LocalInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LocalInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303803);
    soul::ast::Span span = soul::ast::Span();
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::LOCAL)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                span = lexer.GetSpan(pos);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                if (match.hit)
                {
                    tref = typeRef->value;
                    context->ResolveType(tref);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LocalInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::LocalInstruction(span, result, tref.GetType()));
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LocalInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "LocalInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::PLocalInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PLocalInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303804);
    soul::ast::Span span = soul::ast::Span();
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    int level = int();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::PLOCAL)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                span = lexer.GetSpan(pos);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                                typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                                if (match.hit)
                                {
                                    tref = typeRef->value;
                                    context->ResolveType(tref);
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::COMMA)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch8 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::NUMBER)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            level = lexer.GetToken(pos).ToInt();
                        }
                        *parentMatch10 = match;
                    }
                    *parentMatch9 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PLocalInstruction");
                #endif
                return soul::parser::Match(true, new otava::intermediate::PLocalInstruction(span, result, tref.GetType(), level));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PLocalInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "PLocalInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::LoadInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LoadInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303805);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::intermediate::Value> ptr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::LOAD)
            {
                ++lexer;
                match.hit = true;
            }
            if (match.hit)
            {
                span = lexer.GetSpan(pos);
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                ptr.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LoadInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::LoadInstruction(span, result, ptr.release()));
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LoadInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "LoadInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::ElemAddrInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ElemAddrInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303806);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::intermediate::Value> ptr;
    std::unique_ptr<otava::intermediate::Value> index;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::ELEMADDR)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                    ptr.reset(static_cast<otava::intermediate::Value*>(match.value));
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::COMMA)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch6 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                index.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElemAddrInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::ElemAddrInstruction(span, result, ptr.release(), index.release()));
                    }
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElemAddrInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "ElemAddrInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::PtrOffsetInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PtrOffsetInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303807);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::intermediate::Value> ptr;
    std::unique_ptr<otava::intermediate::Value> offset;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::PTROFFSET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                    ptr.reset(static_cast<otava::intermediate::Value*>(match.value));
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::COMMA)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch6 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                offset.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PtrOffsetInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::PtrOffsetInstruction(span, result, ptr.release(), offset.release()));
                    }
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PtrOffsetInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "PtrOffsetInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::PtrDiffInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PtrDiffInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303808);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::intermediate::Value> leftPtr;
    std::unique_ptr<otava::intermediate::Value> rightPtr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::PTRDIFF)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                    leftPtr.reset(static_cast<otava::intermediate::Value*>(match.value));
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::COMMA)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch6 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                rightPtr.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PtrDiffInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::PtrDiffInstruction(span, result, leftPtr.release(), rightPtr.release()));
                    }
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PtrDiffInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "PtrDiffInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::GetRbpInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "GetRbpInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303809);
    soul::ast::Span span = soul::ast::Span();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::GETRBP)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    span = lexer.GetSpan(pos);
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GetRbpInstruction");
                #endif
                return soul::parser::Match(true, new otava::intermediate::GetRbpInstruction(span, result));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GetRbpInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "GetRbpInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::FunctionCallInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionCallInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303810);
    otava::intermediate::TypeRef tref = otava::intermediate::TypeRef();
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<soul::parser::Value<otava::intermediate::TypeRef>> typeRef;
    std::unique_ptr<otava::intermediate::Value> callee;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::CALL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    span = lexer.GetSpan(pos);
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::TypeRef(lexer);
                    typeRef.reset(static_cast<soul::parser::Value<otava::intermediate::TypeRef>*>(match.value));
                    if (match.hit)
                    {
                        tref = typeRef->value;
                        context->ResolveType(tref);
                    }
                    *parentMatch4 = match;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch5 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Value(lexer, context, tref.GetType());
                callee.reset(static_cast<otava::intermediate::Value*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionCallInstruction");
                        #endif
                        return soul::parser::Match(true, new otava::intermediate::FunctionCallInstruction(span, result, callee.release()));
                    }
                }
                *parentMatch6 = match;
            }
            *parentMatch5 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionCallInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionCallInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::BlockValue(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BlockValue");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303811);
    std::unique_ptr<otava::intermediate::Value> operand;
    std::unique_ptr<soul::parser::Value<std::int32_t>> blockId;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::LBRACKET)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Operand(lexer, context);
                                operand.reset(static_cast<otava::intermediate::Value*>(match.value));
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch7 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == otava::intermediate::token::COMMA)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch7 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::Label(lexer);
                        blockId.reset(static_cast<soul::parser::Value<std::int32_t>*>(match.value));
                        *parentMatch8 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::RBRACKET)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch9 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BlockValue");
                #endif
                return soul::parser::Match(true, new soul::parser::Value<otava::intermediate::BlockValue>(otava::intermediate::BlockValue(operand.release(), blockId->value)));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BlockValue");
        else soul::lexer::WriteFailureToLog(lexer, "BlockValue");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::NoOperationInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NoOperationInstruction");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303812);
    soul::ast::Span span = soul::ast::Span();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::NOP)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            span = lexer.GetSpan(pos);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NoOperationInstruction");
                #endif
                return soul::parser::Match(true, new otava::intermediate::NoOperationInstruction(span));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NoOperationInstruction");
        else soul::lexer::WriteFailureToLog(lexer, "NoOperationInstruction");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::Metadata(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Metadata");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303813);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::METADATA)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                soul::parser::Match match(true);
                soul::parser::Match* parentMatch5 = &match;
                {
                    while (true)
                    {
                        std::int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataStruct(lexer, context);
                            if (match.hit)
                            {
                                *parentMatch5 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                                break;
                            }
                        }
                    }
                }
                *parentMatch4 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch6 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::RBRACE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    context->ResolveMetadataReferences();
                }
                *parentMatch7 = match;
            }
            *parentMatch6 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Metadata");
        else soul::lexer::WriteFailureToLog(lexer, "Metadata");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::MetadataStruct(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataStruct");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303814);
    otava::intermediate::MetadataStruct* value = nullptr;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::EXCLAMATION)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == otava::intermediate::token::NUMBER)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    value = context->AddMetadataStruct(lexer.GetSpan(pos), lexer.GetToken(pos).ToInt());
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == otava::intermediate::token::ASSIGN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch8 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::intermediate::token::LBRACE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch9 = &match;
            {
                soul::parser::Match match(true);
                std::int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch10 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch11 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataField(lexer, context, value);
                            *parentMatch12 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                soul::parser::Match match(true);
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    while (true)
                                    {
                                        std::int64_t save = lexer.GetPos();
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch15 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch16 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::intermediate::token::COMMA)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    *parentMatch16 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch17 = &match;
                                                    {
                                                        soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataField(lexer, context, value);
                                                        *parentMatch17 = match;
                                                    }
                                                    *parentMatch16 = match;
                                                }
                                                *parentMatch15 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch14 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                                break;
                                            }
                                        }
                                    }
                                }
                                *parentMatch13 = match;
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch11 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch10 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch9 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch18 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == otava::intermediate::token::RBRACE)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch18 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataStruct");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataStruct");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::MetadataField(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::MetadataStruct* parent)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataField");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303815);
    std::string fieldName = std::string();
    std::unique_ptr<otava::intermediate::MetadataItem> item;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::ID)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    fieldName = util::ToUtf8(lexer.GetToken(pos).ToString());
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::COLON)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch4 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataItem(lexer, context);
                item.reset(static_cast<otava::intermediate::MetadataItem*>(match.value));
                if (match.hit)
                {
                    parent->AddItem(fieldName, item.release());
                }
                *parentMatch5 = match;
            }
            *parentMatch4 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataField");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataField");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::MetadataItem(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataItem");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303816);
    std::unique_ptr<otava::intermediate::MetadataItem> mdBool;
    std::unique_ptr<otava::intermediate::MetadataItem> mdLong;
    std::unique_ptr<otava::intermediate::MetadataItem> mdString;
    std::unique_ptr<otava::intermediate::MetadataRef> mdRef;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::intermediate::token::TRUE:
        case otava::intermediate::token::FALSE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataBool(lexer, context);
                mdBool.reset(static_cast<otava::intermediate::MetadataItem*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
                        #endif
                        return soul::parser::Match(true, mdBool.release());
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::NUMBER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataLong(lexer, context);
                mdLong.reset(static_cast<otava::intermediate::MetadataItem*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
                        #endif
                        return soul::parser::Match(true, mdLong.release());
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::STRING:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataString(lexer, context);
                mdString.reset(static_cast<otava::intermediate::MetadataItem*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
                        #endif
                        return soul::parser::Match(true, mdString.release());
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::EXCLAMATION:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::intermediate::parser::IntermediateParser<LexerT>::MetadataRef(lexer, context);
                mdRef.reset(static_cast<otava::intermediate::MetadataRef*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
                        #endif
                        return soul::parser::Match(true, mdRef.release());
                    }
                }
                *parentMatch4 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataItem");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataItem");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::MetadataBool(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataBool");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303817);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::intermediate::token::TRUE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::TRUE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataBool");
                        #endif
                        return soul::parser::Match(true, context->CreateMetadataBool(true));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::intermediate::token::FALSE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::FALSE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataBool");
                        #endif
                        return soul::parser::Match(true, context->CreateMetadataBool(false));
                    }
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataBool");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataBool");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::MetadataLong(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataLong");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303818);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::NUMBER)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataLong");
                #endif
                return soul::parser::Match(true, context->CreateMetadataLong(lexer.GetToken(pos).ToLong()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataLong");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataLong");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::MetadataString(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataString");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303819);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::STRING)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataString");
                #endif
                return soul::parser::Match(true, context->CreateMetadataString(util::ToUtf8(lexer.GetToken(pos).ToString()), true));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataString");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataString");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match IntermediateParser<LexerT>::MetadataRef(LexerT& lexer, otava::intermediate::IntermediateContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MetadataRef");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2652886897990303820);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        if (*lexer == otava::intermediate::token::EXCLAMATION)
        {
            ++lexer;
            match.hit = true;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::intermediate::token::NUMBER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataRef");
                        #endif
                        return soul::parser::Match(true, context->CreateMetadataRef(lexer.GetSpan(pos), lexer.GetToken(pos).ToInt()));
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MetadataRef");
        else soul::lexer::WriteFailureToLog(lexer, "MetadataRef");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct IntermediateParser<soul::lexer::Lexer<otava::intermediate::lexer::IntermediateLexer<char32_t>, char32_t>>;

} // namespace otava::intermediate::parser
