
// this file has been automatically generated from 'D:/work/otava/parser/template.parser' using soul parser generator spg version 5.2.0

module otava.parser.templates;

import util;
import soul.ast.common;
import soul.ast.spg;
import soul.ast.span;
import soul.ast.lexer_pos_pair;
import otava.token;
import otava.lexer;
import otava.symbols.templates;
import otava.parser.concepts;
import otava.parser.declaration;
import otava.parser.function;
import otava.parser.expression;
import otava.parser.identifier;
import otava.parser.literal;
import otava.parser.punctuation;
import otava.parser.type;

namespace otava::parser::templates {

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateDeclaration(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107585);
    std::unique_ptr<otava::ast::Node> templateHeadNode = std::unique_ptr<otava::ast::Node>();
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> templateHead;
    std::unique_ptr<otava::ast::Node> concepDefinition;
    std::unique_ptr<otava::ast::Node> declaration;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateHead(lexer, context, true);
            templateHead.reset(static_cast<otava::ast::Node*>(match.value));
            if (match.hit)
            {
                templateHeadNode.reset(templateHead.release());
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
                        std::int64_t save = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::parser::concepts::ConceptParser<LexerT>::ConceptDefinition(lexer, context);
                            concepDefinition.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                otava::symbols::EndTemplateDeclaration(templateHeadNode.get(), context);
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateDeclaration");
                                    #endif
                                    return soul::parser::Match(true, new otava::ast::TemplateDeclarationNode(span, lexer.File(), templateHeadNode.release(), concepDefinition.release()));
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
                                    soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::Declaration(lexer, context);
                                    declaration.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        otava::symbols::EndTemplateDeclaration(templateHeadNode.get(), context);
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateDeclaration");
                                            #endif
                                            return soul::parser::Match(true, new otava::ast::TemplateDeclarationNode(span, lexer.File(), templateHeadNode.release(), declaration.release()));
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
                                bool pass = true;
                                soul::parser::Match match(true);
                                if (match.hit)
                                {
                                    otava::symbols::RemoveTemplateDeclaration(context);
                                    pass = false;
                                }
                                if (match.hit && !pass)
                                {
                                    match = soul::parser::Match(false);
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch9 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateHead(LexerT& lexer, otava::symbols::Context* context, bool begin)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateHead");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107586);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::TemplateHeadNode> templateHeadNode = std::unique_ptr<otava::ast::TemplateHeadNode>();
    std::unique_ptr<otava::ast::Node> templateParamList;
    std::unique_ptr<otava::ast::Node> requiresClause;
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
                        if (*lexer == otava::token::TEMPLATE)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            span = lexer.GetSpan(pos);
                            templateHeadNode.reset(new otava::ast::TemplateHeadNode(span, lexer.File()));
                            if (begin)
                            {
                                otava::symbols::BeginTemplateDeclaration(templateHeadNode.get(), context);
                            }
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
                            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateParameterList(lexer, context);
                            templateParamList.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                templateHeadNode->SetTemplateParameterList(templateParamList.release());
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch10 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::concepts::ConceptParser<LexerT>::RequiresClause(lexer, context);
                                requiresClause.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    templateHeadNode->SetRequiresClause(requiresClause.release());
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
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateHead");
                #endif
                return soul::parser::Match(true, templateHeadNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateHead");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateHead");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateParameterList(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateParameterList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107587);
    int index = int();
    std::unique_ptr<otava::ast::Node> templateParameterNode = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::TemplateParameterListNode> node = std::unique_ptr<otava::ast::TemplateParameterListNode>();
    soul::ast::Span laSpan = soul::ast::Span();
    soul::ast::Span raSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> first;
    std::unique_ptr<otava::ast::Node> comma;
    std::unique_ptr<otava::ast::Node> next;
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
                            if (*lexer == otava::token::LANGLE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                soul::ast::Span span = lexer.GetSpan(pos);
                                node.reset(new otava::ast::TemplateParameterListNode(span, lexer.File()));
                                laSpan = span;
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
                                soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateParameter(lexer, context);
                                first.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    templateParameterNode.reset(first.release());
                                    otava::symbols::AddTemplateParameter(templateParameterNode.get(), index, context);
                                    node->AddNode(templateParameterNode.release());
                                    index = index + 1;
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
                                            soul::parser::Match* parentMatch12 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Comma(lexer);
                                                comma.reset(static_cast<otava::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    node->AddNode(comma.release());
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
                                                    soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateParameter(lexer, context);
                                                    next.reset(static_cast<otava::ast::Node*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        templateParameterNode.reset(next.release());
                                                        otava::symbols::AddTemplateParameter(templateParameterNode.get(), index, context);
                                                        node->AddNode(templateParameterNode.release());
                                                        index = index + 1;
                                                    }
                                                    *parentMatch14 = match;
                                                }
                                                *parentMatch13 = match;
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
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
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
                        soul::parser::Match match(false);
                        if (*lexer == otava::token::RANGLE)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            raSpan = lexer.GetSpan(pos);
                        }
                        *parentMatch16 = match;
                    }
                    *parentMatch15 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            node->SetLAngleSpan(laSpan);
            node->SetRAngleSpan(raSpan);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateParameterList");
                #endif
                return soul::parser::Match(true, node.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateParameterList");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateParameterList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateParameter(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateParameter");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107588);
    std::unique_ptr<otava::ast::Node> typeParam;
    std::unique_ptr<otava::ast::Node> param;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TypeParameter(lexer, context);
            typeParam.reset(static_cast<otava::ast::Node*>(match.value));
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateParameter");
                    #endif
                    return soul::parser::Match(true, typeParam.release());
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
                    soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::ParameterDeclaration(lexer, context);
                    param.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateParameter");
                            #endif
                            return soul::parser::Match(true, param.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateParameter");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateParameter");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TypeParameter(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeParameter");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107589);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> ellipsisNode = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> identifierNode = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> templateHead;
    std::unique_ptr<otava::ast::Node> key;
    std::unique_ptr<otava::ast::Node> id;
    std::unique_ptr<otava::ast::Node> asgn;
    std::unique_ptr<otava::ast::Node> idExpr;
    std::unique_ptr<otava::ast::Node> typeId;
    std::unique_ptr<otava::ast::Node> ellipsis;
    std::unique_ptr<otava::ast::Node> id2;
    std::unique_ptr<otava::ast::Node> constraint;
    std::unique_ptr<otava::ast::Node> id3;
    std::unique_ptr<otava::ast::Node> asgn2;
    std::unique_ptr<otava::ast::Node> typeId2;
    std::unique_ptr<otava::ast::Node> ellipsis2;
    std::unique_ptr<otava::ast::Node> id4;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(true);
        std::int64_t save = lexer.GetPos();
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateHead(lexer, context, false);
                    templateHead.reset(static_cast<otava::ast::Node*>(match.value));
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
                *parentMatch1 = match;
            }
            else
            {
                lexer.SetPos(save);
            }
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    std::int64_t save = lexer.GetPos();
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TypeParameterKey(lexer);
                            key.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                if (!span.IsValid()) span = lexer.GetSpan(pos);
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
                                            soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Identifier(lexer, context);
                                            id.reset(static_cast<otava::ast::Node*>(match.value));
                                            *parentMatch13 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch14 = &match;
                                            {
                                                soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Assign(lexer);
                                                asgn.reset(static_cast<otava::ast::Node*>(match.value));
                                                *parentMatch14 = match;
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch15 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch16 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch17 = &match;
                                                {
                                                    std::int64_t save = lexer.GetPos();
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch18 = &match;
                                                    {
                                                        std::int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = otava::parser::expression::ExpressionParser<LexerT>::IdExpression(lexer, context);
                                                        idExpr.reset(static_cast<otava::ast::Node*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeParameter");
                                                                #endif
                                                                return soul::parser::Match(true, new otava::ast::TypeParameterNode(span, lexer.File(), key.release(), id.release(), asgn.release(), idExpr.release(), nullptr, templateHead.release()));
                                                            }
                                                        }
                                                        *parentMatch18 = match;
                                                    }
                                                    *parentMatch17 = match;
                                                    if (!match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch19 = &match;
                                                        lexer.SetPos(save);
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch20 = &match;
                                                            {
                                                                std::int64_t pos = lexer.GetPos();
                                                                soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeId(lexer, context);
                                                                typeId.reset(static_cast<otava::ast::Node*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    {
                                                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeParameter");
                                                                        #endif
                                                                        return soul::parser::Match(true, new otava::ast::TypeParameterNode(span, lexer.File(), key.release(), id.release(), asgn.release(), typeId.release(), nullptr, templateHead.release()));
                                                                    }
                                                                }
                                                                *parentMatch20 = match;
                                                            }
                                                            *parentMatch19 = match;
                                                        }
                                                        *parentMatch17 = match;
                                                    }
                                                }
                                                *parentMatch16 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch11 = match;
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
                                                bool pass = true;
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch23 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch24 = &match;
                                                    {
                                                        soul::parser::Match match(true);
                                                        std::int64_t save = lexer.GetPos();
                                                        soul::parser::Match* parentMatch25 = &match;
                                                        {
                                                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Ellipsis(lexer);
                                                            ellipsis.reset(static_cast<otava::ast::Node*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                *parentMatch25 = match;
                                                            }
                                                            else
                                                            {
                                                                lexer.SetPos(save);
                                                            }
                                                        }
                                                        *parentMatch24 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch26 = &match;
                                                        {
                                                            soul::parser::Match match(true);
                                                            std::int64_t save = lexer.GetPos();
                                                            soul::parser::Match* parentMatch27 = &match;
                                                            {
                                                                soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Identifier(lexer, context);
                                                                id2.reset(static_cast<otava::ast::Node*>(match.value));
                                                                if (match.hit)
                                                                {
                                                                    *parentMatch27 = match;
                                                                }
                                                                else
                                                                {
                                                                    lexer.SetPos(save);
                                                                }
                                                            }
                                                            *parentMatch26 = match;
                                                        }
                                                        *parentMatch24 = match;
                                                    }
                                                    *parentMatch23 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    ellipsisNode.reset(ellipsis.release());
                                                    identifierNode.reset(id2.release());
                                                    if (!ellipsisNode && !identifierNode)
                                                    {
                                                        pass = false;
                                                    }
                                                    else
                                                    {
                                                        {
                                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeParameter");
                                                            #endif
                                                            return soul::parser::Match(true, new otava::ast::TypeParameterNode(span, lexer.File(), key.release(), identifierNode.release(), nullptr, nullptr, ellipsisNode.release(), templateHead.release()));
                                                        }
                                                    }
                                                }
                                                if (match.hit && !pass)
                                                {
                                                    match = soul::parser::Match(false);
                                                }
                                                *parentMatch22 = match;
                                            }
                                            *parentMatch21 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch9 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                    if (!match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch28 = &match;
                        lexer.SetPos(save);
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch29 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch30 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::parser::concepts::ConceptParser<LexerT>::TypeConstraint(lexer, context);
                                    constraint.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        if (!span.IsValid()) span = lexer.GetSpan(pos);
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
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch33 = &match;
                                        {
                                            std::int64_t save = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch34 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch35 = &match;
                                                {
                                                    soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Identifier(lexer, context);
                                                    id3.reset(static_cast<otava::ast::Node*>(match.value));
                                                    *parentMatch35 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch36 = &match;
                                                    {
                                                        soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Assign(lexer);
                                                        asgn2.reset(static_cast<otava::ast::Node*>(match.value));
                                                        *parentMatch36 = match;
                                                    }
                                                    *parentMatch35 = match;
                                                }
                                                *parentMatch34 = match;
                                            }
                                            if (match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch37 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch38 = &match;
                                                    {
                                                        std::int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeId(lexer, context);
                                                        typeId2.reset(static_cast<otava::ast::Node*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            {
                                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeParameter");
                                                                #endif
                                                                return soul::parser::Match(true, new otava::ast::TypeParameterNode(span, lexer.File(), constraint.release(), id3.release(), asgn2.release(), typeId2.release(), nullptr, templateHead.release()));
                                                            }
                                                        }
                                                        *parentMatch38 = match;
                                                    }
                                                    *parentMatch37 = match;
                                                }
                                                *parentMatch34 = match;
                                            }
                                            *parentMatch33 = match;
                                            if (!match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch39 = &match;
                                                lexer.SetPos(save);
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch40 = &match;
                                                    {
                                                        std::int64_t pos = lexer.GetPos();
                                                        bool pass = true;
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch41 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch42 = &match;
                                                            {
                                                                soul::parser::Match match(true);
                                                                std::int64_t save = lexer.GetPos();
                                                                soul::parser::Match* parentMatch43 = &match;
                                                                {
                                                                    soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Ellipsis(lexer);
                                                                    ellipsis2.reset(static_cast<otava::ast::Node*>(match.value));
                                                                    if (match.hit)
                                                                    {
                                                                        *parentMatch43 = match;
                                                                    }
                                                                    else
                                                                    {
                                                                        lexer.SetPos(save);
                                                                    }
                                                                }
                                                                *parentMatch42 = match;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch44 = &match;
                                                                {
                                                                    soul::parser::Match match(true);
                                                                    std::int64_t save = lexer.GetPos();
                                                                    soul::parser::Match* parentMatch45 = &match;
                                                                    {
                                                                        soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Identifier(lexer, context);
                                                                        id4.reset(static_cast<otava::ast::Node*>(match.value));
                                                                        if (match.hit)
                                                                        {
                                                                            *parentMatch45 = match;
                                                                        }
                                                                        else
                                                                        {
                                                                            lexer.SetPos(save);
                                                                        }
                                                                    }
                                                                    *parentMatch44 = match;
                                                                }
                                                                *parentMatch42 = match;
                                                            }
                                                            *parentMatch41 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            ellipsisNode.reset(ellipsis2.release());
                                                            identifierNode.reset(id4.release());
                                                            if (!ellipsisNode && !identifierNode)
                                                            {
                                                                pass = false;
                                                            }
                                                            else
                                                            {
                                                                {
                                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeParameter");
                                                                    #endif
                                                                    return soul::parser::Match(true, new otava::ast::TypeParameterNode(span, lexer.File(), constraint.release(), identifierNode.release(), nullptr, nullptr, ellipsisNode.release(), templateHead.release()));
                                                                }
                                                            }
                                                        }
                                                        if (match.hit && !pass)
                                                        {
                                                            match = soul::parser::Match(false);
                                                        }
                                                        *parentMatch40 = match;
                                                    }
                                                    *parentMatch39 = match;
                                                }
                                                *parentMatch33 = match;
                                            }
                                        }
                                        *parentMatch32 = match;
                                    }
                                    *parentMatch31 = match;
                                }
                                *parentMatch29 = match;
                            }
                            *parentMatch28 = match;
                        }
                        *parentMatch6 = match;
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeParameter");
        else soul::lexer::WriteFailureToLog(lexer, "TypeParameter");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TypeParameterKey(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeParameterKey");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107590);
    std::unique_ptr<otava::ast::Node> typename_;
    std::unique_ptr<otava::ast::Node> cls;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::token::TYPENAME:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::Typename(lexer);
                typename_.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeParameterKey");
                        #endif
                        return soul::parser::Match(true, typename_.release());
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
        case otava::token::CLASS:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::Class(lexer);
                cls.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeParameterKey");
                        #endif
                        return soul::parser::Match(true, cls.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeParameterKey");
        else soul::lexer::WriteFailureToLog(lexer, "TypeParameterKey");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::Typename(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Typename");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107591);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Typename");
                #endif
                return soul::parser::Match(true, new otava::ast::TypenameNode(lexer.GetSpan(pos), lexer.File()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Typename");
        else soul::lexer::WriteFailureToLog(lexer, "Typename");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::Class(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Class");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107592);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::token::CLASS)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Class");
                #endif
                return soul::parser::Match(true, new otava::ast::ClassNode(lexer.GetSpan(pos), lexer.File()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Class");
        else soul::lexer::WriteFailureToLog(lexer, "Class");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateId(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107593);
    std::unique_ptr<otava::ast::TemplateIdNode> node = std::unique_ptr<otava::ast::TemplateIdNode>();
    std::unique_ptr<otava::ast::Node> templateName = std::unique_ptr<otava::ast::Node>();
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span laSpan = soul::ast::Span();
    soul::ast::Span raSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> operatorFunctionId;
    std::unique_ptr<otava::ast::Node> literalOperatorId;
    std::unique_ptr<otava::ast::Node> simpleTemplateId;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case otava::token::OPERATOR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
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
                                            std::int64_t save = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::OperatorFunctionId(lexer, context);
                                                operatorFunctionId.reset(static_cast<otava::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    templateName.reset(operatorFunctionId.release());
                                                    span = lexer.GetSpan(pos);
                                                }
                                                *parentMatch9 = match;
                                            }
                                            *parentMatch8 = match;
                                            if (!match.hit)
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch10 = &match;
                                                lexer.SetPos(save);
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch11 = &match;
                                                    {
                                                        std::int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = otava::parser::literal::LiteralParser<LexerT>::LiteralOperatorId(lexer, context);
                                                        literalOperatorId.reset(static_cast<otava::ast::Node*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            templateName.reset(literalOperatorId.release());
                                                            span = lexer.GetSpan(pos);
                                                        }
                                                        *parentMatch11 = match;
                                                    }
                                                    *parentMatch10 = match;
                                                }
                                                *parentMatch8 = match;
                                            }
                                        }
                                        *parentMatch7 = match;
                                    }
                                    *parentMatch6 = match;
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
                                            auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                            soul::parser::Match match(false);
                                            if (*lexer == otava::token::LANGLE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                laSpan = lexer.GetSpan(pos);
                                                ++vars->langleCount;
                                                context->PushSetFlag(otava::symbols::ContextFlags::parsingTemplateId);
                                                node.reset(new otava::ast::TemplateIdNode(span, lexer.File(), templateName.release()));
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                                *parentMatch5 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    soul::parser::Match match(true);
                                    std::int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch15 = &match;
                                    {
                                        soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateArgumentList(lexer, context, node.get());
                                        if (match.hit)
                                        {
                                            *parentMatch15 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                        }
                                    }
                                    *parentMatch14 = match;
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
                                soul::parser::Match* parentMatch17 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::token::RANGLE)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        raSpan = lexer.GetSpan(pos);
                                        --vars->langleCount;
                                        context->PopFlags();
                                    }
                                    else
                                    {
                                        --vars->langleCount;
                                        context->PopFlags();
                                    }
                                    *parentMatch17 = match;
                                }
                                *parentMatch16 = match;
                            }
                            *parentMatch4 = match;
                        }
                        *parentMatch3 = match;
                    }
                    if (match.hit)
                    {
                        node->SetLAngleSpan(laSpan);
                        node->SetRAngleSpan(raSpan);
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateId");
                            #endif
                            return soul::parser::Match(true, node.release());
                        }
                    }
                    *parentMatch2 = match;
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case otava::token::ID:
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
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateId");
                        #endif
                        return soul::parser::Match(true, simpleTemplateId.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateId");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::SimpleTemplateId(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SimpleTemplateId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107594);
    std::unique_ptr<otava::ast::TemplateIdNode> node = std::unique_ptr<otava::ast::TemplateIdNode>();
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span laSpan = soul::ast::Span();
    soul::ast::Span raSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> templateName;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        bool pass = true;
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
                            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateName(lexer, context);
                            templateName.reset(static_cast<otava::ast::Node*>(match.value));
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
                                auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                                soul::parser::Match match(false);
                                if (*lexer == otava::token::LANGLE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    laSpan = lexer.GetSpan(pos);
                                    ++vars->langleCount;
                                    context->PushSetFlag(otava::symbols::ContextFlags::parsingTemplateId);
                                    context->ResetRejectTemplateId();
                                    node.reset(new otava::ast::TemplateIdNode(span, lexer.File(), templateName.release()));
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
                        soul::parser::Match match(true);
                        std::int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateArgumentList(lexer, context, node.get());
                            if (match.hit)
                            {
                                *parentMatch9 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
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
                soul::parser::Match* parentMatch10 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch11 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());
                        soul::parser::Match match(false);
                        if (*lexer == otava::token::RANGLE)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            raSpan = lexer.GetSpan(pos);
                            --vars->langleCount;
                            context->PopFlags();
                        }
                        else
                        {
                            --vars->langleCount;
                            context->PopFlags();
                        }
                        *parentMatch11 = match;
                    }
                    *parentMatch10 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            if (context->RejectTemplateId())
            {
                pass = false;
            }
            else
            {
                node->SetLAngleSpan(laSpan);
                node->SetRAngleSpan(raSpan);
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTemplateId");
                    #endif
                    return soul::parser::Match(true, node.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleTemplateId");
        else soul::lexer::WriteFailureToLog(lexer, "SimpleTemplateId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateName(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107595);
    std::unique_ptr<otava::ast::Node> identifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Identifier(lexer, context);
        identifier.reset(static_cast<otava::ast::Node*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateName");
                #endif
                return soul::parser::Match(true, identifier.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateName");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateArgumentList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* templateIdNode)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateArgumentList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107596);
    int index = int();
    std::unique_ptr<otava::ast::Node> left;
    std::unique_ptr<otava::ast::Node> comma;
    std::unique_ptr<otava::ast::Node> right;
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
                    soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateArgument(lexer, context, templateIdNode, index);
                    left.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        templateIdNode->AddNode(left.release());
                        ++index;
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
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        while (true)
                        {
                            std::int64_t save = lexer.GetPos();
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
                                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Comma(lexer);
                                            comma.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                templateIdNode->AddNode(comma.release());
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
                                                soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateArgument(lexer, context, templateIdNode, index);
                                                right.reset(static_cast<otava::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    templateIdNode->AddNode(right.release());
                                                    ++index;
                                                }
                                                *parentMatch10 = match;
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
                                    break;
                                }
                            }
                        }
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch13 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch14 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == otava::token::ELLIPSIS)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            templateIdNode->AddNode(new otava::ast::EllipsisNode(lexer.GetSpan(pos), lexer.File()));
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
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateArgumentList");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateArgumentList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateArgument(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* templateIdNode, int index)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateArgument");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107597);
    std::unique_ptr<otava::ast::Node> typeId;
    std::unique_ptr<otava::ast::Node> constantExpr;
    std::unique_ptr<otava::ast::Node> idExpr;
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        bool pass = true;
                        soul::parser::Match match(true);
                        if (match.hit)
                        {
                            pass = otava::symbols::TemplateArgCanBeTypeId(templateIdNode, index);
                        }
                        if (match.hit && !pass)
                        {
                            match = soul::parser::Match(false);
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
                        soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeId(lexer, context);
                        typeId.reset(static_cast<otava::ast::Node*>(match.value));
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
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateArgNext(lexer);
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateArgument");
                                #endif
                                return soul::parser::Match(true, typeId.release());
                            }
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
                soul::parser::Match* parentMatch8 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch9 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::expression::ExpressionParser<LexerT>::ConstantExpression(lexer, context);
                        constantExpr.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateArgument");
                                #endif
                                return soul::parser::Match(true, constantExpr.release());
                            }
                        }
                        *parentMatch9 = match;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch10 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch11 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::parser::expression::ExpressionParser<LexerT>::IdExpression(lexer, context);
                    idExpr.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateArgument");
                            #endif
                            return soul::parser::Match(true, idExpr.release());
                        }
                    }
                    *parentMatch11 = match;
                }
                *parentMatch10 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateArgument");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateArgument");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::TemplateArgNext(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TemplateArgNext");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107598);
    soul::parser::Match match(true);
    std::int64_t save = lexer.GetPos();
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            switch (*lexer)
            {
                case otava::token::RANGLE:
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::RANGLE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case otava::token::COMMA:
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::COMMA)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case otava::token::ELLIPSIS:
                {
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::ELLIPSIS)
                    {
                        ++lexer;
                        match.hit = true;
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
            *parentMatch0 = match;
            lexer.SetPos(save);
        }
        else
        {
            *parentMatch0 = soul::parser::Match(false);
            lexer.SetPos(save);
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TemplateArgNext");
        else soul::lexer::WriteFailureToLog(lexer, "TemplateArgNext");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::DeductionGuide(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DeductionGuide");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107599);
    std::unique_ptr<otava::ast::ParameterListNode> params = std::unique_ptr<otava::ast::ParameterListNode>();
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> explicitSpecifier;
    std::unique_ptr<otava::ast::Node> templateName;
    std::unique_ptr<otava::ast::Node> arrow;
    std::unique_ptr<otava::ast::Node> templateId;
    std::unique_ptr<otava::ast::Node> semicolon;
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
                                        soul::parser::Match match(true);
                                        std::int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch9 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch11 = &match;
                                                {
                                                    std::int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::ExplicitSpecifier(lexer, context);
                                                    explicitSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        span = lexer.GetSpan(pos);
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
                                            }
                                        }
                                        *parentMatch8 = match;
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
                                                soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::TemplateName(lexer, context);
                                                templateName.reset(static_cast<otava::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    if (!span.IsValid()) span = lexer.GetSpan(pos);
                                                }
                                                *parentMatch13 = match;
                                            }
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
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
                                            if (*lexer == otava::token::LPAREN)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                lpSpan = lexer.GetSpan(pos);
                                                params.reset(new otava::ast::ParameterListNode(lexer.GetSpan(pos), lexer.File()));
                                            }
                                            *parentMatch15 = match;
                                        }
                                        *parentMatch14 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch16 = &match;
                                {
                                    soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::ParameterDeclarationClause(lexer, context, params.get());
                                    *parentMatch16 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
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
                                    if (*lexer == otava::token::RPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        rpSpan = lexer.GetSpan(pos);
                                    }
                                    *parentMatch18 = match;
                                }
                                *parentMatch17 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch19 = &match;
                        {
                            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::Arrow(lexer);
                            arrow.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch19 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch20 = &match;
                    {
                        soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::SimpleTemplateId(lexer, context);
                        templateId.reset(static_cast<otava::ast::Node*>(match.value));
                        *parentMatch20 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch21 = &match;
                {
                    soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                    semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                    *parentMatch21 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeductionGuide");
                #endif
                return soul::parser::Match(true, new otava::ast::DeductionGuideNode(span, lexer.File(), templateName.release(), params.release(), arrow.release(), templateId.release(), explicitSpecifier.release(), semicolon.release(), lpSpan, rpSpan));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeductionGuide");
        else soul::lexer::WriteFailureToLog(lexer, "DeductionGuide");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::Arrow(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Arrow");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107600);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::token::ARROW)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Arrow");
                #endif
                return soul::parser::Match(true, new otava::ast::ArrowNode(lexer.GetSpan(pos), lexer.File()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Arrow");
        else soul::lexer::WriteFailureToLog(lexer, "Arrow");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::ExplicitInstantiation(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ExplicitInstantiation");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107601);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> extrn;
    std::unique_ptr<otava::ast::Node> tmp;
    std::unique_ptr<otava::ast::Node> declaration;
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
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::Extern(lexer);
                                extrn.reset(static_cast<otava::ast::Node*>(match.value));
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
                            *parentMatch4 = match;
                        }
                        else
                        {
                            lexer.SetPos(save);
                        }
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
                            soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::Template(lexer);
                            tmp.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                if (!span.IsValid()) span = lexer.GetSpan(pos);
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
                    soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::Declaration(lexer, context);
                    declaration.reset(static_cast<otava::ast::Node*>(match.value));
                    *parentMatch9 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            otava::ast::ExplicitInstantiationNode *node = new otava::ast::ExplicitInstantiationNode(span, lexer.File(), extrn.release(), tmp.release(), declaration.release());
            otava::symbols::ProcessExplicitInstantiation(node, context);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExplicitInstantiation");
                #endif
                return soul::parser::Match(true, node);
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExplicitInstantiation");
        else soul::lexer::WriteFailureToLog(lexer, "ExplicitInstantiation");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::ExplicitSpecialization(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ExplicitSpecialization");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107602);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span laSpan = soul::ast::Span();
    soul::ast::Span raSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::TemplateHeadNode> templateHeadNode = std::unique_ptr<otava::ast::TemplateHeadNode>();
    std::unique_ptr<otava::ast::Node> tmp;
    std::unique_ptr<otava::ast::Node> declaration;
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
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::templates::TemplateParser<LexerT>::Template(lexer);
                                tmp.reset(static_cast<otava::ast::Node*>(match.value));
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
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::token::LANGLE)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        laSpan = lexer.GetSpan(pos);
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
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch10 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == otava::token::RANGLE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    raSpan = lexer.GetSpan(pos);
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch9 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
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
                            soul::parser::Match match(true);
                            if (match.hit)
                            {
                                templateHeadNode.reset(new otava::ast::TemplateHeadNode(lexer.GetSpan(pos), lexer.File()));
                                otava::symbols::BeginTemplateDeclaration(templateHeadNode.get(), context);
                                context->PushSetFlag(otava::symbols::ContextFlags::parsingTemplateDeclaration);
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch11 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
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
                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::Declaration(lexer, context);
                        declaration.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            context->PopFlags();
                        }
                        else
                        {
                            otava::symbols::EndTemplateDeclaration(templateHeadNode.get(), context);
                            context->PopFlags();
                        }
                        *parentMatch14 = match;
                    }
                    *parentMatch13 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            otava::symbols::EndTemplateDeclaration(templateHeadNode.get(), context);
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExplicitSpecialization");
                #endif
                return soul::parser::Match(true, new otava::ast::ExplicitSpecializationNode(span, lexer.File(), tmp.release(), templateHeadNode.release(), declaration.release(), laSpan, raSpan));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExplicitSpecialization");
        else soul::lexer::WriteFailureToLog(lexer, "ExplicitSpecialization");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::Extern(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Extern");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107603);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::token::EXTERN)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Extern");
                #endif
                return soul::parser::Match(true, new otava::ast::ExternNode(lexer.GetSpan(pos), lexer.File()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Extern");
        else soul::lexer::WriteFailureToLog(lexer, "Extern");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TemplateParser<LexerT>::Template(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Template");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 9050966169783107604);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::token::TEMPLATE)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Template");
                #endif
                return soul::parser::Match(true, new otava::ast::TemplateNode(lexer.GetSpan(pos), lexer.File()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Template");
        else soul::lexer::WriteFailureToLog(lexer, "Template");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct TemplateParser<soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t>>;

} // namespace otava::parser::templates
