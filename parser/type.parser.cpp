
// this file has been automatically generated from 'D:/work/otava/parser/type.parser' using soul parser generator spg version 5.2.0

module otava.parser.type;

import util;
import soul.ast.common;
import soul.ast.spg;
import otava.ast;
import otava.token;
import otava.lexer;
import otava.symbols.scope_ptr;
import otava.symbols.scope_resolver;
import otava.symbols.classes;
import otava.parser.attribute;
import otava.parser.classes;
import otava.parser.concepts;
import otava.parser.declaration;
import otava.parser.enums;
import otava.parser.expression;
import otava.parser.function;
import otava.parser.guard;
import otava.parser.identifier;
import otava.parser.simple.type;
import otava.parser.templates;

namespace otava::parser::type {

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::TypeSpecifierSeq(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeSpecifierSeq");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754753);
    std::unique_ptr<otava::ast::Node> sequence = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> first;
    std::unique_ptr<otava::ast::Node> next;
    std::unique_ptr<otava::ast::Node> attributes;
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
                        soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeSpecifier(lexer, context);
                        first.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            sequence.reset(new otava::ast::TypeSpecifierSequenceNode(lexer.GetSpan(pos), lexer.File()));
                            sequence->AddNode(first.release());
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
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            while (true)
                            {
                                std::int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            std::int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeSpecifier(lexer, context);
                                            next.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                sequence->AddNode(next.release());
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
                                        break;
                                    }
                                }
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
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                                attributes.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    sequence->AddNode(attributes.release());
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeSpecifierSeq");
                #endif
                return soul::parser::Match(true, sequence.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeSpecifierSeq");
        else soul::lexer::WriteFailureToLog(lexer, "TypeSpecifierSeq");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::TypeSpecifierSeqReset(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeSpecifierSeqReset");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754754);
    std::unique_ptr<otava::ast::Node> typeSpecifierSeq;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(true);
            if (match.hit)
            {
                context->PushResetFlag(otava::symbols::ContextFlags::hasDefiningTypeSpecifier);
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
                soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeSpecifierSeq(lexer, context);
                typeSpecifierSeq.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    context->PopFlags();
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeSpecifierSeqReset");
                        #endif
                        return soul::parser::Match(true, typeSpecifierSeq.release());
                    }
                }
                else
                {
                    context->PopFlags();
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeSpecifierSeqReset");
        else soul::lexer::WriteFailureToLog(lexer, "TypeSpecifierSeqReset");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::DefiningTypeId(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DefiningTypeId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754755);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> definingTypeSpecifierSeq;
    std::unique_ptr<otava::ast::Node> abstractDeclararator;
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
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::DefiningTypeSpecifierSeq(lexer, context);
                    definingTypeSpecifierSeq.reset(static_cast<otava::ast::Node*>(match.value));
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
                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::AbstractDeclarator(lexer, context);
                        abstractDeclararator.reset(static_cast<otava::ast::Node*>(match.value));
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeId");
                #endif
                return soul::parser::Match(true, new otava::ast::DefiningTypeIdNode(span, lexer.File(), definingTypeSpecifierSeq.release(), abstractDeclararator.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeId");
        else soul::lexer::WriteFailureToLog(lexer, "DefiningTypeId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::DefiningTypeSpecifierSeq(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DefiningTypeSpecifierSeq");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754756);
    std::unique_ptr<otava::ast::Node> node = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> first;
    std::unique_ptr<otava::ast::Node> next;
    std::unique_ptr<otava::ast::Node> attributes;
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
                        soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::DefiningTypeSpecifierReset(lexer, context);
                        first.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            node.reset(new otava::ast::DefiningTypeSpecifierSequenceNode(lexer.GetSpan(pos), lexer.File()));
                            node->AddNode(first.release());
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
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            while (true)
                            {
                                std::int64_t save = lexer.GetPos();
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch7 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            std::int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::DefiningTypeSpecifierReset(lexer, context);
                                            next.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                node->AddNode(next.release());
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
                                        break;
                                    }
                                }
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
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                                attributes.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    node->AddNode(attributes.release());
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeSpecifierSeq");
                #endif
                return soul::parser::Match(true, node.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeSpecifierSeq");
        else soul::lexer::WriteFailureToLog(lexer, "DefiningTypeSpecifierSeq");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::DefiningTypeSpecifierReset(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DefiningTypeSpecifierReset");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754757);
    std::unique_ptr<otava::ast::Node> definingTypeSpecifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(true);
            if (match.hit)
            {
                context->PushResetFlag(otava::symbols::ContextFlags::hasDefiningTypeSpecifier);
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
                soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::DefiningTypeSpecifier(lexer, context);
                definingTypeSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    context->PopFlags();
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeSpecifierReset");
                        #endif
                        return soul::parser::Match(true, definingTypeSpecifier.release());
                    }
                }
                else
                {
                    context->PopFlags();
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeSpecifierReset");
        else soul::lexer::WriteFailureToLog(lexer, "DefiningTypeSpecifierReset");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::DefiningTypeSpecifier(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DefiningTypeSpecifier");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754758);
    std::unique_ptr<otava::ast::Node> classSpecifier;
    std::unique_ptr<otava::ast::Node> enumSpecifier;
    std::unique_ptr<otava::ast::Node> typeSpecifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            switch (*lexer)
            {
                case otava::token::CLASS:
                case otava::token::STRUCT:
                case otava::token::UNION:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch3 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::classes::ClassParser<LexerT>::ClassSpecifier(lexer, context);
                        classSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            context->SetFlag(otava::symbols::ContextFlags::hasDefiningTypeSpecifier);
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeSpecifier");
                                #endif
                                return soul::parser::Match(true, classSpecifier.release());
                            }
                        }
                        *parentMatch3 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case otava::token::ENUM:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::enums::EnumParser<LexerT>::EnumSpecifier(lexer, context);
                        enumSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            context->SetFlag(otava::symbols::ContextFlags::hasDefiningTypeSpecifier);
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeSpecifier");
                                #endif
                                return soul::parser::Match(true, enumSpecifier.release());
                            }
                        }
                        *parentMatch4 = match;
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
                soul::parser::Match* parentMatch5 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch6 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeSpecifier(lexer, context);
                        typeSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeSpecifier");
                                #endif
                                return soul::parser::Match(true, typeSpecifier.release());
                            }
                        }
                        *parentMatch6 = match;
                    }
                    *parentMatch5 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DefiningTypeSpecifier");
        else soul::lexer::WriteFailureToLog(lexer, "DefiningTypeSpecifier");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::TypeSpecifier(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeSpecifier");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754759);
    std::unique_ptr<otava::ast::Node> simpleTypeSpecifier;
    std::unique_ptr<otava::ast::Node> elaboratedTypeSpecifier;
    std::unique_ptr<otava::ast::Node> typenameSpecifier;
    std::unique_ptr<otava::ast::Node> cvQualifier;
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
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::SimpleTypeSpecifier(lexer, context);
                    simpleTypeSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        context->SetFlag(otava::symbols::ContextFlags::hasDefiningTypeSpecifier);
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeSpecifier");
                            #endif
                            return soul::parser::Match(true, simpleTypeSpecifier.release());
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::ElaboratedTypeSpecifier(lexer, context);
                            elaboratedTypeSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                context->SetFlag(otava::symbols::ContextFlags::hasDefiningTypeSpecifier);
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeSpecifier");
                                    #endif
                                    return soul::parser::Match(true, elaboratedTypeSpecifier.release());
                                }
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypenameSpecifier(lexer, context);
                        typenameSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            context->SetFlag(otava::symbols::ContextFlags::hasDefiningTypeSpecifier);
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeSpecifier");
                                #endif
                                return soul::parser::Match(true, typenameSpecifier.release());
                            }
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch8 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::CVQualifier(lexer);
                    cvQualifier.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeSpecifier");
                            #endif
                            return soul::parser::Match(true, cvQualifier.release());
                        }
                    }
                    *parentMatch9 = match;
                }
                *parentMatch8 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeSpecifier");
        else soul::lexer::WriteFailureToLog(lexer, "TypeSpecifier");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::ElaboratedTypeSpecifier(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ElaboratedTypeSpecifier");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754760);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span nnsSpan = soul::ast::Span();
    soul::ast::Span tmpPos = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> classKey;
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> nns;
    std::unique_ptr<otava::ast::Node> tmp;
    std::unique_ptr<otava::ast::Node> simpleTemplateId;
    std::unique_ptr<otava::ast::Node> simpleTemplateId2;
    std::unique_ptr<otava::ast::Node> identifier;
    std::unique_ptr<otava::ast::Node> simpleTemplateId3;
    std::unique_ptr<otava::ast::Node> identifier2;
    std::unique_ptr<otava::ast::Node> elaboratedEnumSpecifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::token::CLASS:
        case otava::token::STRUCT:
        case otava::token::UNION:
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
                        soul::parser::Match match = otava::parser::classes::ClassParser<LexerT>::ClassKey(lexer);
                        classKey.reset(static_cast<otava::ast::Node*>(match.value));
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
                            soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                            attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            std::int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                std::int64_t save = lexer.GetPos();
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::NestedNameSpecifier(lexer, context);
                                        nns.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            nnsSpan = lexer.GetSpan(pos);
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
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch13 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch14 = &match;
                                            {
                                                std::int64_t save = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch15 = &match;
                                                switch (*lexer)
                                                {
                                                    case otava::token::TEMPLATE:
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch16 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch17 = &match;
                                                            {
                                                                std::int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Template(lexer);
                                                                tmp.reset(static_cast<otava::ast::Node*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    tmpPos = lexer.GetSpan(pos);
                                                                }
                                                                *parentMatch17 = match;
                                                            }
                                                            *parentMatch16 = match;
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
                                                                    soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::SimpleTemplateId(lexer, context);
                                                                    simpleTemplateId.reset(static_cast<otava::ast::Node*>(match.value));
                                                                    if (match.hit)
                                                                    {
                                                                        otava::ast::Node *node = new otava::ast::ElaboratedTypeSpecifierNode(span, lexer.File(), classKey.release(), new otava::ast::QualifiedIdNode(nnsSpan, lexer.File(), nns.release(), new otava::ast::PrefixNode(tmpPos, lexer.File(), tmp.release(), simpleTemplateId.release())), attributes.release());
                                                                        otava::symbols::ProcessElaboratedClassDeclaration(node, context);
                                                                        {
                                                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElaboratedTypeSpecifier");
                                                                            #endif
                                                                            return soul::parser::Match(true, node);
                                                                        }
                                                                    }
                                                                    *parentMatch19 = match;
                                                                }
                                                                *parentMatch18 = match;
                                                            }
                                                            *parentMatch16 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            *parentMatch15 = match;
                                                        }
                                                        break;
                                                    }
                                                    case otava::token::ID:
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch20 = &match;
                                                        {
                                                            std::int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::SimpleTemplateId(lexer, context);
                                                            simpleTemplateId2.reset(static_cast<otava::ast::Node*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                otava::ast::Node *node = new otava::ast::ElaboratedTypeSpecifierNode(span, lexer.File(), classKey.release(), new otava::ast::QualifiedIdNode(nnsSpan, lexer.File(), nns.release(), simpleTemplateId2.release()), attributes.release());
                                                                otava::symbols::ProcessElaboratedClassDeclaration(node, context);
                                                                {
                                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElaboratedTypeSpecifier");
                                                                    #endif
                                                                    return soul::parser::Match(true, node);
                                                                }
                                                            }
                                                            *parentMatch20 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            *parentMatch15 = match;
                                                        }
                                                        break;
                                                    }
                                                }
                                                *parentMatch14 = match;
                                                if (!match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch21 = &match;
                                                    lexer.SetPos(save);
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch22 = &match;
                                                        {
                                                            std::int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Identifier(lexer, context);
                                                            identifier.reset(static_cast<otava::ast::Node*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                otava::ast::Node *node = new otava::ast::ElaboratedTypeSpecifierNode(span, lexer.File(), classKey.release(), new otava::ast::QualifiedIdNode(nnsSpan, lexer.File(), nns.release(), identifier.release()), attributes.release());
                                                                otava::symbols::ProcessElaboratedClassDeclaration(node, context);
                                                                {
                                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElaboratedTypeSpecifier");
                                                                    #endif
                                                                    return soul::parser::Match(true, node);
                                                                }
                                                            }
                                                            *parentMatch22 = match;
                                                        }
                                                        *parentMatch21 = match;
                                                    }
                                                    *parentMatch14 = match;
                                                }
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
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
                                            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::SimpleTemplateId(lexer, context);
                                            simpleTemplateId3.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                otava::ast::Node *node = new otava::ast::ElaboratedTypeSpecifierNode(span, lexer.File(), classKey.release(), simpleTemplateId3.release(), attributes.release());
                                                otava::symbols::ProcessElaboratedClassDeclaration(node, context);
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElaboratedTypeSpecifier");
                                                    #endif
                                                    return soul::parser::Match(true, node);
                                                }
                                            }
                                            *parentMatch24 = match;
                                        }
                                        *parentMatch23 = match;
                                    }
                                    *parentMatch9 = match;
                                }
                            }
                            *parentMatch8 = match;
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
                                        soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Identifier(lexer, context);
                                        identifier2.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            otava::ast::Node *node = new otava::ast::ElaboratedTypeSpecifierNode(span, lexer.File(), classKey.release(), identifier2.release(), attributes.release());
                                            otava::symbols::ProcessElaboratedClassDeclaration(node, context);
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElaboratedTypeSpecifier");
                                                #endif
                                                return soul::parser::Match(true, node);
                                            }
                                        }
                                        *parentMatch26 = match;
                                    }
                                    *parentMatch25 = match;
                                }
                                *parentMatch8 = match;
                            }
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::token::ENUM:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch27 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::parser::enums::EnumParser<LexerT>::ElaboratedEnumSpecifier(lexer, context);
                elaboratedEnumSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElaboratedTypeSpecifier");
                        #endif
                        return soul::parser::Match(true, elaboratedEnumSpecifier.release());
                    }
                }
                *parentMatch27 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ElaboratedTypeSpecifier");
        else soul::lexer::WriteFailureToLog(lexer, "ElaboratedTypeSpecifier");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::SimpleTypeSpecifierReset(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SimpleTypeSpecifierReset");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754761);
    std::unique_ptr<otava::ast::Node> simpleTypeSpecifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(true);
            if (match.hit)
            {
                context->PushResetFlag(otava::symbols::ContextFlags::hasDefiningTypeSpecifier);
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
                soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::SimpleTypeSpecifier(lexer, context);
                simpleTypeSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    context->PopFlags();
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTypeSpecifierReset");
                        #endif
                        return soul::parser::Match(true, simpleTypeSpecifier.release());
                    }
                }
                else
                {
                    context->PopFlags();
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTypeSpecifierReset");
        else soul::lexer::WriteFailureToLog(lexer, "SimpleTypeSpecifierReset");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::SimpleTypeSpecifier(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SimpleTypeSpecifier");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754762);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span tmpPos = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> nns = std::unique_ptr<otava::ast::Node>();
    bool isConstructorNameNode = bool();
    otava::symbols::ScopePtr scopePtr = otava::symbols::ScopePtr();
    std::unique_ptr<otava::ast::Node> simpleType;
    std::unique_ptr<otava::ast::Node> nns1;
    std::unique_ptr<otava::ast::Node> tmp;
    std::unique_ptr<otava::ast::Node> simpleTemplateId;
    std::unique_ptr<otava::ast::Node> nns2;
    std::unique_ptr<otava::ast::Node> typeName;
    std::unique_ptr<otava::ast::Node> typeName2;
    std::unique_ptr<otava::ast::Node> declTypeSpecifier;
    std::unique_ptr<otava::ast::Node> placeholderTypeSpecifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::parser::simple::type::SimpleTypeParser<LexerT>::SimpleType(lexer);
                simpleType.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTypeSpecifier");
                        #endif
                        return soul::parser::Match(true, simpleType.release());
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match = otava::parser::guard::GuardParser<LexerT>::NotDefiningTypeSpecifierGuard(lexer, context);
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        std::int64_t save = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            std::int64_t save = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                std::int64_t save = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch11 = &match;
                                                {
                                                    std::int64_t save = lexer.GetPos();
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch12 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch13 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch14 = &match;
                                                            {
                                                                std::int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::NestedNameSpecifier(lexer, context);
                                                                nns1.reset(static_cast<otava::ast::Node*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    nns.reset(nns1.release());
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
                                                                    soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Template(lexer);
                                                                    tmp.reset(static_cast<otava::ast::Node*>(match.value));
                                                                    if (match.hit)
                                                                    {
                                                                        tmpPos = lexer.GetSpan(pos);
                                                                    }
                                                                    *parentMatch16 = match;
                                                                }
                                                                *parentMatch15 = match;
                                                            }
                                                            *parentMatch13 = match;
                                                        }
                                                        *parentMatch12 = match;
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
                                                                soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::SimpleTemplateId(lexer, context);
                                                                simpleTemplateId.reset(static_cast<otava::ast::Node*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    {
                                                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTypeSpecifier");
                                                                        #endif
                                                                        return soul::parser::Match(true, new otava::ast::QualifiedIdNode(span, lexer.File(), nns.release(), new otava::ast::PrefixNode(tmpPos, lexer.File(), tmp.release(), simpleTemplateId.release())));
                                                                    }
                                                                }
                                                                *parentMatch18 = match;
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch12 = match;
                                                    }
                                                    *parentMatch11 = match;
                                                    if (!match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch19 = &match;
                                                        lexer.SetPos(save);
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch20 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch21 = &match;
                                                                {
                                                                    std::int64_t pos = lexer.GetPos();
                                                                    soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::NestedNameSpecifier(lexer, context);
                                                                    nns2.reset(static_cast<otava::ast::Node*>(match.value));
                                                                    if (match.hit)
                                                                    {
                                                                        nns.reset(nns2.release());
                                                                        scopePtr.Reset(otava::symbols::GetScope(nns.get(), context), context);
                                                                    }
                                                                    *parentMatch21 = match;
                                                                }
                                                                *parentMatch20 = match;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch22 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch23 = &match;
                                                                    {
                                                                        std::int64_t pos = lexer.GetPos();
                                                                        bool pass = true;
                                                                        soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeName(lexer, context);
                                                                        typeName.reset(static_cast<otava::ast::Node*>(match.value));
                                                                        if (match.hit)
                                                                        {
                                                                            std::unique_ptr<otava::ast::Node> typeNameNode;
                                                                            typeNameNode.reset(typeName.release());
                                                                            isConstructorNameNode = context->IsConstructorNameNode(typeNameNode.get());
                                                                            scopePtr.Reset();
                                                                            if (isConstructorNameNode)
                                                                            {
                                                                                pass = false;
                                                                            }
                                                                            else
                                                                            {
                                                                                {
                                                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTypeSpecifier");
                                                                                    #endif
                                                                                    return soul::parser::Match(true, new otava::ast::QualifiedIdNode(span, lexer.File(), nns.release(), typeNameNode.release()));
                                                                                }
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            scopePtr.Reset();
                                                                        }
                                                                        if (match.hit && !pass)
                                                                        {
                                                                            match = soul::parser::Match(false);
                                                                        }
                                                                        *parentMatch23 = match;
                                                                    }
                                                                    *parentMatch22 = match;
                                                                }
                                                                *parentMatch20 = match;
                                                            }
                                                            *parentMatch19 = match;
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                }
                                                *parentMatch10 = match;
                                                if (!match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch24 = &match;
                                                    lexer.SetPos(save);
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch25 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch26 = &match;
                                                            {
                                                                std::int64_t pos = lexer.GetPos();
                                                                bool pass = true;
                                                                soul::parser::Match match(true);
                                                                if (match.hit)
                                                                {
                                                                    if (isConstructorNameNode) pass = false;
                                                                }
                                                                if (match.hit && !pass)
                                                                {
                                                                    match = soul::parser::Match(false);
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
                                                                    bool pass = true;
                                                                    soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeName(lexer, context);
                                                                    typeName2.reset(static_cast<otava::ast::Node*>(match.value));
                                                                    if (match.hit)
                                                                    {
                                                                        std::unique_ptr<otava::ast::Node> typeNameNode;
                                                                        typeNameNode.reset(typeName2.release());
                                                                        if (context->IsConstructorNameNode(typeNameNode.get()))
                                                                        {
                                                                            pass = false;
                                                                        }
                                                                        else
                                                                        {
                                                                            {
                                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTypeSpecifier");
                                                                                #endif
                                                                                return soul::parser::Match(true, typeNameNode.release());
                                                                            }
                                                                        }
                                                                    }
                                                                    if (match.hit && !pass)
                                                                    {
                                                                        match = soul::parser::Match(false);
                                                                    }
                                                                    *parentMatch28 = match;
                                                                }
                                                                *parentMatch27 = match;
                                                            }
                                                            *parentMatch25 = match;
                                                        }
                                                        *parentMatch24 = match;
                                                    }
                                                    *parentMatch10 = match;
                                                }
                                            }
                                            *parentMatch9 = match;
                                            if (!match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch29 = &match;
                                                lexer.SetPos(save);
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch30 = &match;
                                                    {
                                                        std::int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::DeclTypeSpecifier(lexer, context);
                                                        declTypeSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTypeSpecifier");
                                                                #endif
                                                                return soul::parser::Match(true, declTypeSpecifier.release());
                                                            }
                                                        }
                                                        *parentMatch30 = match;
                                                    }
                                                    *parentMatch29 = match;
                                                }
                                                *parentMatch9 = match;
                                            }
                                        }
                                        *parentMatch8 = match;
                                        if (!match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch31 = &match;
                                            lexer.SetPos(save);
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch32 = &match;
                                                {
                                                    std::int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::PlaceholderTypeSpecifier(lexer, context);
                                                    placeholderTypeSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        {
                                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTypeSpecifier");
                                                            #endif
                                                            return soul::parser::Match(true, placeholderTypeSpecifier.release());
                                                        }
                                                    }
                                                    *parentMatch32 = match;
                                                }
                                                *parentMatch31 = match;
                                            }
                                            *parentMatch8 = match;
                                        }
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTypeSpecifier");
        else soul::lexer::WriteFailureToLog(lexer, "SimpleTypeSpecifier");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::TypeName(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754763);
    std::unique_ptr<otava::ast::Node> templateId;
    std::unique_ptr<otava::ast::Node> typeIdentifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::SimpleTemplateId(lexer, context);
            templateId.reset(static_cast<otava::ast::Node*>(match.value));
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
                    #endif
                    return soul::parser::Match(true, templateId.release());
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::TypeIdentifier(lexer, context);
                    typeIdentifier.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
                            #endif
                            return soul::parser::Match(true, typeIdentifier.release());
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
        else soul::lexer::WriteFailureToLog(lexer, "TypeName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::TypeId(LexerT& lexer, otava::symbols::Context* context)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754764);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> typeSpecifiers;
    std::unique_ptr<otava::ast::Node> abstractDeclarator;
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
                        soul::parser::Match match(true);
                        if (match.hit)
                        {
                            context->PushResetFlag(otava::symbols::ContextFlags::hasDefiningTypeSpecifier);
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
                            soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeSpecifierSeq(lexer, context);
                            typeSpecifiers.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                span = lexer.GetSpan(pos);
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
                    soul::parser::Match match(true);
                    std::int64_t save = lexer.GetPos();
                    soul::parser::Match* parentMatch8 = &match;
                    {
                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::AbstractDeclarator(lexer, context);
                        abstractDeclarator.reset(static_cast<otava::ast::Node*>(match.value));
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
            context->PopFlags();
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeId");
                #endif
                return soul::parser::Match(true, new otava::ast::TypeIdNode(span, lexer.File(), typeSpecifiers.release(), abstractDeclarator.release()));
            }
        }
        else
        {
            context->PopFlags();
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
soul::parser::Match TypeParser<LexerT>::TypenameSpecifier(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypenameSpecifier");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754765);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> templateNode = std::unique_ptr<otava::ast::Node>();
    bool typenameSeen = bool();
    std::unique_ptr<otava::ast::Node> leaf = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> nns;
    std::unique_ptr<otava::ast::Node> t;
    std::unique_ptr<otava::ast::Node> simpleTemplateId;
    std::unique_ptr<otava::ast::Node> identifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t save = lexer.GetPos();
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
                        if (*lexer == otava::token::TYPENAME)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            span = lexer.GetSpan(pos);
                            context->PushSetFlag(otava::symbols::ContextFlags::assumeType);
                            typenameSeen = true;
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
                            soul::parser::Match match(true);
                            if (match.hit)
                            {
                                context->PushResetFlag(otava::symbols::ContextFlags::assumeType);
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
                        soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::NestedNameSpecifier(lexer, context);
                        nns.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            context->PopFlags();
                        }
                        else
                        {
                            context->PopFlags();
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch10 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch11 = &match;
                    {
                        std::int64_t save = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            soul::parser::Match match(true);
                            std::int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch15 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Template(lexer);
                                        t.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            templateNode.reset(t.release());
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
                                }
                            }
                            *parentMatch12 = match;
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
                                    soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::SimpleTemplateId(lexer, context);
                                    simpleTemplateId.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        context->PopFlags();
                                        leaf.reset(simpleTemplateId.release());
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypenameSpecifier");
                                            #endif
                                            return soul::parser::Match(true, new otava::ast::TypenameSpecifierNode(span, lexer.File(), nns.release(), leaf.release(), templateNode.release()));
                                        }
                                    }
                                    *parentMatch17 = match;
                                }
                                *parentMatch16 = match;
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch11 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch18 = &match;
                            lexer.SetPos(save);
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch19 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Identifier(lexer, context);
                                    identifier.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        context->PopFlags();
                                        leaf.reset(identifier.release());
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypenameSpecifier");
                                            #endif
                                            return soul::parser::Match(true, new otava::ast::TypenameSpecifierNode(span, lexer.File(), nns.release(), leaf.release(), nullptr));
                                        }
                                    }
                                    *parentMatch19 = match;
                                }
                                *parentMatch18 = match;
                            }
                            *parentMatch11 = match;
                        }
                    }
                    *parentMatch10 = match;
                }
                *parentMatch9 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch20 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch21 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    bool pass = true;
                    soul::parser::Match match(true);
                    if (match.hit)
                    {
                        if (typenameSeen)
                        {
                            context->PopFlags();
                        }
                        pass = false;
                    }
                    if (match.hit && !pass)
                    {
                        match = soul::parser::Match(false);
                    }
                    *parentMatch21 = match;
                }
                *parentMatch20 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypenameSpecifier");
        else soul::lexer::WriteFailureToLog(lexer, "TypenameSpecifier");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::DeclTypeSpecifier(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DeclTypeSpecifier");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754766);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> expr;
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
                            if (*lexer == otava::token::DECLTYPE)
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
                                soul::parser::Match match(false);
                                if (*lexer == otava::token::LPAREN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    lpSpan = lexer.GetSpan(pos);
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
                        soul::parser::Match match = otava::parser::expression::ExpressionParser<LexerT>::Expression(lexer, context);
                        expr.reset(static_cast<otava::ast::Node*>(match.value));
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
                        if (*lexer == otava::token::RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            rpSpan = lexer.GetSpan(pos);
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeclTypeSpecifier");
                #endif
                return soul::parser::Match(true, new otava::ast::DeclTypeSpecifierNode(span, lexer.File(), expr.release(), lpSpan, rpSpan));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeclTypeSpecifier");
        else soul::lexer::WriteFailureToLog(lexer, "DeclTypeSpecifier");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::PlaceholderTypeSpecifier(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PlaceholderTypeSpecifier");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 2330114084151754767);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span dtPos = soul::ast::Span();
    soul::ast::Span autoPos = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> typeConstraint;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(true);
                std::int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::parser::concepts::ConceptParser<LexerT>::TypeConstraint(lexer, context);
                            typeConstraint.reset(static_cast<otava::ast::Node*>(match.value));
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
                        *parentMatch3 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch2 = match;
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
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match(false);
                            if (*lexer == otava::token::AUTO)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                if (!span.IsValid()) span = lexer.GetSpan(pos);
                                autoPos = lexer.GetSpan(pos);
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PlaceholderTypeSpecifier");
                                    #endif
                                    return soul::parser::Match(true, new otava::ast::PlaceholderTypeSpecifierNode(span, lexer.File(), typeConstraint.release(), dtPos, autoPos, lpSpan, rpSpan));
                                }
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch11 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
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
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch16 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == otava::token::DECLTYPE)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    dtPos = lexer.GetSpan(pos);
                                                    if (!span.IsValid()) span = lexer.GetSpan(pos);
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
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::token::LPAREN)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        lpSpan = lexer.GetSpan(pos);
                                                    }
                                                    *parentMatch18 = match;
                                                }
                                                *parentMatch17 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        *parentMatch14 = match;
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
                                                if (*lexer == otava::token::AUTO)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    autoPos = lexer.GetSpan(pos);
                                                }
                                                *parentMatch20 = match;
                                            }
                                            *parentMatch19 = match;
                                        }
                                        *parentMatch14 = match;
                                    }
                                    *parentMatch13 = match;
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
                                            soul::parser::Match match(false);
                                            if (*lexer == otava::token::RPAREN)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                rpSpan = lexer.GetSpan(pos);
                                            }
                                            *parentMatch22 = match;
                                        }
                                        *parentMatch21 = match;
                                    }
                                    *parentMatch13 = match;
                                }
                                *parentMatch12 = match;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PlaceholderTypeSpecifier");
                                    #endif
                                    return soul::parser::Match(true, new otava::ast::PlaceholderTypeSpecifierNode(span, lexer.File(), typeConstraint.release(), dtPos, autoPos, lpSpan, rpSpan));
                                }
                            }
                            *parentMatch11 = match;
                        }
                        *parentMatch10 = match;
                    }
                    *parentMatch9 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PlaceholderTypeSpecifier");
        else soul::lexer::WriteFailureToLog(lexer, "PlaceholderTypeSpecifier");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct TypeParser<soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t>>;

} // namespace otava::parser::type
