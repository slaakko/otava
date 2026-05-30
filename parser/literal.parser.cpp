
// this file has been automatically generated from 'D:/work/otava/parser/literal.parser' using soul parser generator spg version 5.2.0

module otava.parser.literal;

import util;
import soul.ast.common;
import soul.ast.spg;
import otava.ast;
import otava.token;
import otava.lexer;
import otava.parser.token;

namespace otava::parser::literal {

template<typename LexerT>
soul::parser::Match LiteralParser<LexerT>::Literal(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Literal");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2723703907904651265);
    std::unique_ptr<otava::ast::Node> udLiteral;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::token::FLOATING_LITERAL:
        case otava::token::INTEGER_LITERAL:
        case otava::token::CHARACTER_LITERAL:
        case otava::token::STRING_LITERAL:
        case otava::token::BEGIN_RAW_STRING_LITERAL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                switch (*lexer)
                {
                    case otava::token::FLOATING_LITERAL:
                    case otava::token::INTEGER_LITERAL:
                    case otava::token::CHARACTER_LITERAL:
                    case otava::token::STRING_LITERAL:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch3 = &match;
                        {
                            std::int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch4 = &match;
                            {
                                std::int64_t save = lexer.GetPos();
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch5 = &match;
                                {
                                    std::int64_t save = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch6 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::literal::LiteralParser<LexerT>::UserDefinedLiteral(lexer, context);
                                        udLiteral.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                                                #endif
                                                return soul::parser::Match(true, udLiteral.release());
                                            }
                                        }
                                        *parentMatch6 = match;
                                    }
                                    *parentMatch5 = match;
                                    if (!match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch7 = &match;
                                        lexer.SetPos(save);
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch8 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == otava::token::INTEGER_LITERAL)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    {
                                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                                                        #endif
                                                        return soul::parser::Match(true, otava::parser::token::ParseIntegerLiteral(lexer.GetSpan(pos), lexer.File(), lexer.FileName(), lexer.GetToken(pos)));
                                                    }
                                                }
                                                *parentMatch8 = match;
                                            }
                                            *parentMatch7 = match;
                                        }
                                        *parentMatch5 = match;
                                    }
                                }
                                *parentMatch4 = match;
                                if (!match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    lexer.SetPos(save);
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            std::int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == otava::token::FLOATING_LITERAL)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                                                    #endif
                                                    return soul::parser::Match(true, otava::parser::token::ParseFloatingLiteral(lexer.GetSpan(pos), lexer.File(), lexer.FileName(), lexer.GetToken(pos)));
                                                }
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch4 = match;
                                }
                            }
                            *parentMatch3 = match;
                            if (!match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch11 = &match;
                                lexer.SetPos(save);
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == otava::token::CHARACTER_LITERAL)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                                                #endif
                                                return soul::parser::Match(true, otava::parser::token::ParseCharacterLiteral(lexer.GetSpan(pos), lexer.File(), lexer.FileName(), lexer.GetToken(pos)));
                                            }
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch11 = match;
                                }
                                *parentMatch3 = match;
                            }
                        }
                        if (match.hit)
                        {
                            *parentMatch2 = match;
                        }
                        break;
                    }
                    case otava::token::BEGIN_RAW_STRING_LITERAL:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch13 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::token::BEGIN_RAW_STRING_LITERAL)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                lexer.SetPos(pos);
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                                    #endif
                                    return soul::parser::Match(true, otava::parser::token::ParseRawStringLiteral(lexer.GetSpan(pos), lexer.File(), lexer));
                                }
                            }
                            *parentMatch13 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch2 = match;
                        }
                        break;
                    }
                }
                *parentMatch1 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch14 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch15 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::token::STRING_LITERAL)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                                    #endif
                                    return soul::parser::Match(true, otava::parser::token::ParseStringLiteral(lexer.GetSpan(pos), lexer.File(), lexer.FileName(), lexer.GetToken(pos)));
                                }
                            }
                            *parentMatch15 = match;
                        }
                        *parentMatch14 = match;
                    }
                    *parentMatch1 = match;
                }
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::token::TRUE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch16 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::token::TRUE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                        #endif
                        return soul::parser::Match(true, new otava::ast::BooleanLiteralNode(lexer.GetSpan(pos), lexer.File(), true, util::ToUtf8(lexer.GetToken(pos).ToString())));
                    }
                }
                *parentMatch16 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::token::FALSE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch17 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::token::FALSE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                        #endif
                        return soul::parser::Match(true, new otava::ast::BooleanLiteralNode(lexer.GetSpan(pos), lexer.File(), false, util::ToUtf8(lexer.GetToken(pos).ToString())));
                    }
                }
                *parentMatch17 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::token::NULLPTR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch18 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == otava::token::NULLPTR)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
                        #endif
                        return soul::parser::Match(true, new otava::ast::NullPtrLiteralNode(lexer.GetSpan(pos), lexer.File(), util::ToUtf8(lexer.GetToken(pos).ToString())));
                    }
                }
                *parentMatch18 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Literal");
        else soul::lexer::WriteFailureToLog(lexer, "Literal");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match LiteralParser<LexerT>::UserDefinedLiteral(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UserDefinedLiteral");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2723703907904651266);
    soul::ast::Span span = soul::ast::Span();
    std::int64_t p = std::int64_t();
    std::unique_ptr<otava::ast::Node> udLiteralNode = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> udSuffix;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            switch (*lexer)
            {
                case otava::token::INTEGER_LITERAL:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch3 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::token::INTEGER_LITERAL)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                span = lexer.GetSpan(pos);
                                p = pos;
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
                                soul::parser::Match match = otava::parser::literal::LiteralParser<LexerT>::UdSuffix(lexer);
                                udSuffix.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    udLiteralNode.reset(new otava::ast::UserDefinedLiteraNode(span, lexer.File(), otava::parser::token::ParseIntegerLiteral(span, lexer.File(), lexer.FileName(), lexer.GetToken(p)), udSuffix.release()));
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch3 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case otava::token::FLOATING_LITERAL:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::token::FLOATING_LITERAL)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                span = lexer.GetSpan(pos);
                                p = pos;
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
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch10 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::literal::LiteralParser<LexerT>::UdSuffix(lexer);
                                udSuffix.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    udLiteralNode.reset(new otava::ast::UserDefinedLiteraNode(span, lexer.File(), otava::parser::token::ParseFloatingLiteral(span, lexer.File(), lexer.FileName(), lexer.GetToken(p)), udSuffix.release()));
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch9 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case otava::token::STRING_LITERAL:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch11 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::token::STRING_LITERAL)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                span = lexer.GetSpan(pos);
                                p = pos;
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch11 = match;
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
                                soul::parser::Match match = otava::parser::literal::LiteralParser<LexerT>::UdSuffix(lexer);
                                udSuffix.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    udLiteralNode.reset(new otava::ast::UserDefinedLiteraNode(span, lexer.File(), otava::parser::token::ParseStringLiteral(span, lexer.File(), lexer.FileName(), lexer.GetToken(p)), udSuffix.release()));
                                }
                                *parentMatch14 = match;
                            }
                            *parentMatch13 = match;
                        }
                        *parentMatch11 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case otava::token::CHARACTER_LITERAL:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch15 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch16 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::token::CHARACTER_LITERAL)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                span = lexer.GetSpan(pos);
                                p = pos;
                            }
                            *parentMatch16 = match;
                        }
                        *parentMatch15 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch18 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::literal::LiteralParser<LexerT>::UdSuffix(lexer);
                                udSuffix.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    udLiteralNode.reset(new otava::ast::UserDefinedLiteraNode(span, lexer.File(), otava::parser::token::ParseCharacterLiteral(span, lexer.File(), lexer.FileName(), lexer.GetToken(p)), udSuffix.release()));
                                }
                                *parentMatch18 = match;
                            }
                            *parentMatch17 = match;
                        }
                        *parentMatch15 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UserDefinedLiteral");
                #endif
                return soul::parser::Match(true, udLiteralNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UserDefinedLiteral");
        else soul::lexer::WriteFailureToLog(lexer, "UserDefinedLiteral");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match LiteralParser<LexerT>::UdSuffix(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "UdSuffix");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2723703907904651267);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::token::ID)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UdSuffix");
                #endif
                return soul::parser::Match(true, otava::parser::token::ParseIdentifier(lexer.GetSpan(pos), lexer.File(), lexer.FileName(), lexer.GetToken(pos)));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "UdSuffix");
        else soul::lexer::WriteFailureToLog(lexer, "UdSuffix");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match LiteralParser<LexerT>::LiteralOperatorId(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LiteralOperatorId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2723703907904651268);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span stringLitPos = soul::ast::Span();
    soul::ast::Span idPos = soul::ast::Span();
    std::int64_t idp = std::int64_t();
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
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == otava::token::OPERATOR)
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
                            bool pass = true;
                            soul::parser::Match match(false);
                            if (*lexer == otava::token::STRING_LITERAL)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                stringLitPos = lexer.GetSpan(pos);
                                pass = lexer.GetToken(pos).ToString() == U"";
                            }
                            if (match.hit && !pass)
                            {
                                match = soul::parser::Match(false);
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
                        soul::parser::Match match(false);
                        if (*lexer == otava::token::ID)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            idPos = lexer.GetSpan(pos);
                            idp = pos;
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralOperatorId");
                #endif
                return soul::parser::Match(true, new otava::ast::LiteralOperatorIdNode(span, lexer.File(), otava::parser::token::ParseIdentifier(idPos, lexer.File(), lexer.FileName(), lexer.GetToken(idp)), stringLitPos));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LiteralOperatorId");
        else soul::lexer::WriteFailureToLog(lexer, "LiteralOperatorId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match LiteralParser<LexerT>::StringLiteral(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StringLiteral");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2723703907904651269);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::token::STRING_LITERAL)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringLiteral");
                #endif
                return soul::parser::Match(true, otava::parser::token::ParseStringLiteral(lexer.GetSpan(pos), lexer.File(), lexer.FileName(), lexer.GetToken(pos)));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringLiteral");
        else soul::lexer::WriteFailureToLog(lexer, "StringLiteral");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct LiteralParser<soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t>>;

} // namespace otava::parser::literal
