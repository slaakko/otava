
// this file has been automatically generated from 'D:/work/otava/parser/statement.parser' using soul parser generator spg version 5.2.0

module otava.parser.statement;

import util;
import soul.ast.common;
import soul.ast.spg;
import soul.ast.span;
import soul.ast.lexer_pos_pair;
import otava.token;
import otava.lexer;
import otava.symbols.block;
import otava.symbols.classes;
import otava.symbols.scope_ptr;
import otava.symbols.symbol_table;
import otava.parser.attribute;
import otava.parser.declaration;
import otava.parser.expression;
import otava.parser.identifier;
import otava.parser.initialization;
import otava.parser.token;
import otava.parser.recorded_parse;
import otava.parser.guard;
import otava.parser.punctuation;
import otava.parser.type;

namespace otava::parser::statement {

template<typename LexerT>
std::unique_ptr<otava::ast::Node> StatementParser<LexerT>::Parse(LexerT& lexer, otava::symbols::Context* context)
{
    std::unique_ptr<otava::ast::Node> value;
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif
    ++lexer;
    soul::parser::Match match = StatementParser<LexerT>::Statement(lexer, context);
    value.reset(static_cast<otava::ast::Node*>(match.value));
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
            return value;
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
    return value;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::Statement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Statement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585281);
    std::unique_ptr<otava::ast::Node> labeledStatement;
    std::unique_ptr<otava::ast::Node> compoundStatement;
    std::unique_ptr<otava::ast::Node> selectionStatement;
    std::unique_ptr<otava::ast::Node> iterationStatement;
    std::unique_ptr<otava::ast::Node> jumpStatement;
    std::unique_ptr<otava::ast::Node> expressionStatement;
    std::unique_ptr<otava::ast::Node> declarationStatement;
    std::unique_ptr<otava::ast::Node> tryStatement;
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
                                std::int64_t save = lexer.GetPos();
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::LabeledStatement(lexer, context);
                                    labeledStatement.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                            #endif
                                            return soul::parser::Match(true, labeledStatement.release());
                                        }
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
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
                                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::CompoundStatement(lexer, context);
                                            compoundStatement.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                    #endif
                                                    return soul::parser::Match(true, compoundStatement.release());
                                                }
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch6 = match;
                                }
                            }
                            *parentMatch5 = match;
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
                                        soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::SelectionStatement(lexer, context);
                                        selectionStatement.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                                #endif
                                                return soul::parser::Match(true, selectionStatement.release());
                                            }
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch5 = match;
                            }
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
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::IterationStatement(lexer, context);
                                    iterationStatement.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                            #endif
                                            return soul::parser::Match(true, iterationStatement.release());
                                        }
                                    }
                                    *parentMatch13 = match;
                                }
                                *parentMatch12 = match;
                            }
                            *parentMatch4 = match;
                        }
                    }
                    *parentMatch3 = match;
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
                                soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::JumpStatement(lexer, context);
                                jumpStatement.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                        #endif
                                        return soul::parser::Match(true, jumpStatement.release());
                                    }
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch14 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch16 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::ExpressionStatement(lexer, context);
                            expressionStatement.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                    #endif
                                    return soul::parser::Match(true, expressionStatement.release());
                                }
                            }
                            *parentMatch17 = match;
                        }
                        *parentMatch16 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
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
                        soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::DeclarationStatement(lexer, context);
                        declarationStatement.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                                #endif
                                return soul::parser::Match(true, declarationStatement.release());
                            }
                        }
                        *parentMatch19 = match;
                    }
                    *parentMatch18 = match;
                }
                *parentMatch1 = match;
            }
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
                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::TryStatement(lexer, context);
                    tryStatement.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
                            #endif
                            return soul::parser::Match(true, tryStatement.release());
                        }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Statement");
        else soul::lexer::WriteFailureToLog(lexer, "Statement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::LabeledStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "LabeledStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585282);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span labelPos = soul::ast::Span();
    std::int64_t labelP = std::int64_t();
    soul::ast::Span colonPos = soul::ast::Span();
    soul::ast::Span casePos = soul::ast::Span();
    soul::ast::Span defaultPos = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> stmt;
    std::unique_ptr<otava::ast::Node> caseExpr;
    std::unique_ptr<otava::ast::Node> stmt2;
    std::unique_ptr<otava::ast::Node> stmt3;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                switch (*lexer)
                {
                    case otava::token::ID:
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
                                    if (*lexer == otava::token::ID)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                        labelPos = span;
                                        labelP = pos;
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
                                        soul::parser::Match match(false);
                                        if (*lexer == otava::token::COLON)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            colonPos = lexer.GetSpan(pos);
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch13 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                                    stmt.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        otava::ast::IdentifierNode *identifierNode = otava::parser::token::ParseIdentifier(labelPos, lexer.File(), lexer.FileName(), lexer.GetToken(labelP));
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LabeledStatement");
                                            #endif
                                            return soul::parser::Match(true, new otava::ast::LabeledStatementNode(span, lexer.File(), identifierNode, stmt.release(), attributes.release(), colonPos));
                                        }
                                    }
                                    *parentMatch13 = match;
                                }
                                *parentMatch12 = match;
                            }
                            *parentMatch7 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch6 = match;
                        }
                        break;
                    }
                    case otava::token::CASE:
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
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch17 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == otava::token::CASE)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            casePos = lexer.GetSpan(pos);
                                            if (!span.IsValid()) span = casePos;
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
                                        soul::parser::Match match = otava::parser::expression::ExpressionParser<LexerT>::ConstantExpression(lexer, context);
                                        caseExpr.reset(static_cast<otava::ast::Node*>(match.value));
                                        *parentMatch18 = match;
                                    }
                                    *parentMatch16 = match;
                                }
                                *parentMatch15 = match;
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
                                        if (*lexer == otava::token::COLON)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            colonPos = lexer.GetSpan(pos);
                                        }
                                        *parentMatch20 = match;
                                    }
                                    *parentMatch19 = match;
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch14 = match;
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
                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                                    stmt2.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LabeledStatement");
                                            #endif
                                            return soul::parser::Match(true, new otava::ast::CaseStatementNode(span, lexer.File(), caseExpr.release(), stmt2.release(), attributes.release(), casePos, colonPos));
                                        }
                                    }
                                    *parentMatch22 = match;
                                }
                                *parentMatch21 = match;
                            }
                            *parentMatch14 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch6 = match;
                        }
                        break;
                    }
                    case otava::token::DEFAULT:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch23 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch24 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch25 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match(false);
                                    if (*lexer == otava::token::DEFAULT)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        defaultPos = lexer.GetSpan(pos);
                                        if (!span.IsValid()) span = defaultPos;
                                    }
                                    *parentMatch25 = match;
                                }
                                *parentMatch24 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch26 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch27 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == otava::token::COLON)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            colonPos = lexer.GetSpan(pos);
                                        }
                                        *parentMatch27 = match;
                                    }
                                    *parentMatch26 = match;
                                }
                                *parentMatch24 = match;
                            }
                            *parentMatch23 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch28 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch29 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                                    stmt3.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LabeledStatement");
                                            #endif
                                            return soul::parser::Match(true, new otava::ast::DefaultStatementNode(span, lexer.File(), stmt3.release(), attributes.release(), defaultPos, colonPos));
                                        }
                                    }
                                    *parentMatch29 = match;
                                }
                                *parentMatch28 = match;
                            }
                            *parentMatch23 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch6 = match;
                        }
                        break;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "LabeledStatement");
        else soul::lexer::WriteFailureToLog(lexer, "LabeledStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::CompoundStatementUnguarded(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CompoundStatementUnguarded");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585283);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::CompoundStatementNode> compoundStatementNode = std::unique_ptr<otava::ast::CompoundStatementNode>();
    std::unique_ptr<otava::ast::Node> statementNode = std::unique_ptr<otava::ast::Node>();
    soul::ast::Span lbSpan = soul::ast::Span();
    soul::ast::Span rbSpan = soul::ast::Span();
    bool nextIsRBrace = bool();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> stmt;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                if (*lexer == otava::token::LBRACE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    lbSpan = lexer.GetSpan(pos);
                                    if (!span.IsValid()) span = lbSpan;
                                    compoundStatementNode.reset(new otava::ast::CompoundStatementNode(span, lexer.File()));
                                    compoundStatementNode->SetAttributes(attributes.release());
                                    compoundStatementNode->SetBlockId(context->NextBlockId());
                                    otava::symbols::BeginBlock(lexer.GetFullSpan(pos), compoundStatementNode->BlockId(), context);
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
                                                    std::int64_t save = lexer.GetPos();
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch14 = &match;
                                                    {
                                                        std::int64_t save = lexer.GetPos();
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch15 = &match;
                                                        {
                                                            std::int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                                                            stmt.reset(static_cast<otava::ast::Node*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                statementNode.reset(stmt.release());
                                                                compoundStatementNode->AddNode(statementNode.release());
                                                            }
                                                            *parentMatch15 = match;
                                                        }
                                                        *parentMatch14 = match;
                                                        if (!match.hit)
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch16 = &match;
                                                            lexer.SetPos(save);
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch17 = &match;
                                                                {
                                                                    std::int64_t pos = lexer.GetPos();
                                                                    bool pass = true;
                                                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::RBraceNext(lexer);
                                                                    if (match.hit)
                                                                    {
                                                                        nextIsRBrace = true;
                                                                        pass = false;
                                                                    }
                                                                    if (match.hit && !pass)
                                                                    {
                                                                        match = soul::parser::Match(false);
                                                                    }
                                                                    *parentMatch17 = match;
                                                                }
                                                                *parentMatch16 = match;
                                                            }
                                                            *parentMatch14 = match;
                                                        }
                                                    }
                                                    *parentMatch13 = match;
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
                                                                bool pass = true;
                                                                soul::parser::Match match(true);
                                                                if (match.hit)
                                                                {
                                                                    if (!nextIsRBrace)
                                                                    {
                                                                        otava::symbols::ThrowStatementParsingError(lexer.GetFullSpan(pos), context);
                                                                    }
                                                                    else
                                                                    {
                                                                        pass = false;
                                                                    }
                                                                }
                                                                if (match.hit && !pass)
                                                                {
                                                                    match = soul::parser::Match(false);
                                                                }
                                                                *parentMatch19 = match;
                                                            }
                                                            *parentMatch18 = match;
                                                        }
                                                        *parentMatch13 = match;
                                                    }
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch20 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch21 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == otava::token::RBRACE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    rbSpan = lexer.GetSpan(pos);
                                }
                                *parentMatch21 = match;
                            }
                            *parentMatch20 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    compoundStatementNode->SetLBraceSpan(lbSpan);
                    compoundStatementNode->SetRBraceSpan(rbSpan);
                    otava::symbols::MapNode(compoundStatementNode.get(), context);
                    otava::symbols::EndBlock(context);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatementUnguarded");
                        #endif
                        return soul::parser::Match(true, compoundStatementNode.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatementUnguarded");
        else soul::lexer::WriteFailureToLog(lexer, "CompoundStatementUnguarded");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::RBraceNext(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RBraceNext");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585284);
    soul::parser::Match match(true);
    std::int64_t save = lexer.GetPos();
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == otava::token::RBRACE)
            {
                ++lexer;
                match.hit = true;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RBraceNext");
        else soul::lexer::WriteFailureToLog(lexer, "RBraceNext");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::CompoundStatementGuarded(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CompoundStatementGuarded");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585285);
    std::unique_ptr<otava::ast::CompoundStatementNode> compoundStatementNode = std::unique_ptr<otava::ast::CompoundStatementNode>();
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
                std::int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    bool pass = true;
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        soul::ast::Span span = lexer.GetSpan(pos);
                        soul::ast::lexer::pos::pair::LexerPosPair lexerPosPair = otava::parser::recorded::parse::RecordCompoundStatement(lexer);
                        if (lexerPosPair.IsValid())
                        {
                            compoundStatementNode.reset(new otava::ast::CompoundStatementNode(span, lexer.File()));
                            compoundStatementNode->SetLexerPosPair(lexerPosPair);
                            compoundStatementNode->SetAttributes(attributes.release());
                            compoundStatementNode->SetFunctionScope(context->GetSymbolTable()->CurrentScope());
                            compoundStatementNode->SetBlockId(context->NextBlockId());
                        }
                        else
                        {
                            pass = false;
                        }
                    }
                    if (match.hit && !pass)
                    {
                        match = soul::parser::Match(false);
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
                            bool pass = true;
                            soul::parser::Match match(true);
                            if (match.hit)
                            {
                                soul::ast::Span span = lexer.GetSpan(pos);
                                soul::ast::lexer::pos::pair::LexerPosPair lexerPosPair = otava::parser::recorded::parse::RecordCompoundStatement(lexer);
                                if (lexerPosPair.IsValid())
                                {
                                    compoundStatementNode.reset(new otava::ast::CompoundStatementNode(span, lexer.File()));
                                    compoundStatementNode->SetLexerPosPair(lexerPosPair);
                                    compoundStatementNode->SetFunctionScope(context->GetSymbolTable()->CurrentScope());
                                    compoundStatementNode->SetBlockId(context->NextBlockId());
                                }
                                else
                                {
                                    pass = false;
                                }
                            }
                            if (match.hit && !pass)
                            {
                                match = soul::parser::Match(false);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatementGuarded");
                #endif
                return soul::parser::Match(true, compoundStatementNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatementGuarded");
        else soul::lexer::WriteFailureToLog(lexer, "CompoundStatementGuarded");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::CompoundStatementSaved(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CompoundStatementSaved");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585286);
    otava::ast::CompoundStatementNode* compoundStatementNode = nullptr;
    std::unique_ptr<otava::ast::Node> statementNode = std::unique_ptr<otava::ast::Node>();
    otava::symbols::ScopePtr scopePtr = otava::symbols::ScopePtr();
    std::unique_ptr<otava::ast::Node> stmt;
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
                std::int64_t pos = lexer.GetPos();
                bool pass = true;
                soul::parser::Match match(true);
                if (match.hit)
                {
                    compoundStatementNode = otava::parser::recorded::parse::GetSavedCompoundStatementNode(context);
                    if (!compoundStatementNode)
                    {
                        pass = false;
                    }
                    lexer.BeginRecordedParse(compoundStatementNode->GetLexerPosPair());
                    otava::symbols::Scope *functionScope = static_cast<otava::symbols::Scope*>(compoundStatementNode->FunctionScope());
                    scopePtr.Reset(functionScope, context);
                }
                if (match.hit && !pass)
                {
                    match = soul::parser::Match(false);
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
                                soul::parser::Match match(false);
                                if (*lexer == otava::token::LBRACE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    soul::ast::Span span = lexer.GetSpan(pos);
                                    compoundStatementNode->SetLBraceSpan(span);
                                    otava::symbols::BeginBlock(lexer.GetFullSpan(pos), compoundStatementNode->BlockId(), context);
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
                                                    std::int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                                                    stmt.reset(static_cast<otava::ast::Node*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        statementNode.reset(stmt.release());
                                                        compoundStatementNode->AddNode(statementNode.release());
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == otava::token::RBRACE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    compoundStatementNode->SetRBraceSpan(lexer.GetSpan(pos));
                                    lexer.EndRecordedParse();
                                    otava::symbols::MapNode(compoundStatementNode, context);
                                    otava::symbols::EndBlock(context);
                                    scopePtr.Reset();
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatementSaved");
                                        #endif
                                        return soul::parser::Match(true, compoundStatementNode);
                                    }
                                }
                                *parentMatch13 = match;
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                    bool pass = true;
                    soul::parser::Match match(true);
                    if (match.hit)
                    {
                        lexer.EndRecordedParse();
                        scopePtr.Reset();
                        pass = false;
                    }
                    if (match.hit && !pass)
                    {
                        match = soul::parser::Match(false);
                    }
                    *parentMatch15 = match;
                }
                *parentMatch14 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatementSaved");
        else soul::lexer::WriteFailureToLog(lexer, "CompoundStatementSaved");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::CompoundStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CompoundStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585287);
    std::unique_ptr<otava::ast::Node> guardedCompoundStatement;
    std::unique_ptr<otava::ast::Node> savedCompoundStatement;
    std::unique_ptr<otava::ast::Node> unguardedCompoundStatement;
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
                std::int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match = otava::parser::guard::GuardParser<LexerT>::MemberFunctionGuard(lexer, context);
                    *parentMatch3 = match;
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
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::CompoundStatementGuarded(lexer, context);
                            guardedCompoundStatement.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatement");
                                    #endif
                                    return soul::parser::Match(true, guardedCompoundStatement.release());
                                }
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch6 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch7 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch8 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::guard::GuardParser<LexerT>::SavedMemberFunctionBodyGuard(lexer, context);
                                if (match.hit)
                                {
                                    context->ResetFlag(otava::symbols::ContextFlags::parseSavedMemberFunctionBody);
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
                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::CompoundStatementSaved(lexer, context);
                                    savedCompoundStatement.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatement");
                                            #endif
                                            return soul::parser::Match(true, savedCompoundStatement.release());
                                        }
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch7 = match;
                        }
                        *parentMatch6 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
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
                        soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::CompoundStatementUnguarded(lexer, context);
                        unguardedCompoundStatement.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatement");
                                #endif
                                return soul::parser::Match(true, unguardedCompoundStatement.release());
                            }
                        }
                        *parentMatch12 = match;
                    }
                    *parentMatch11 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CompoundStatement");
        else soul::lexer::WriteFailureToLog(lexer, "CompoundStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::SelectionStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SelectionStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585288);
    std::unique_ptr<otava::ast::Node> ifStmt;
    std::unique_ptr<otava::ast::Node> switchStmt;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::IfStatement(lexer, context);
            ifStmt.reset(static_cast<otava::ast::Node*>(match.value));
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SelectionStatement");
                    #endif
                    return soul::parser::Match(true, ifStmt.release());
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
                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::SwitchStatement(lexer, context);
                    switchStmt.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SelectionStatement");
                            #endif
                            return soul::parser::Match(true, switchStmt.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SelectionStatement");
        else soul::lexer::WriteFailureToLog(lexer, "SelectionStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::IfStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "IfStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585289);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span ifPos = soul::ast::Span();
    soul::ast::Span constExprPos = soul::ast::Span();
    soul::ast::Span elsePos = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    int blockId = int();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> cond;
    std::unique_ptr<otava::ast::Node> thenStmt;
    std::unique_ptr<otava::ast::Node> elseStmt;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch12 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch13 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == otava::token::IF)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    ifPos = lexer.GetSpan(pos);
                                                    if (!span.IsValid()) span = ifPos;
                                                    blockId = context->NextBlockId();
                                                    otava::symbols::BeginBlock(lexer.GetFullSpan(pos), blockId, context);
                                                }
                                                *parentMatch13 = match;
                                            }
                                            *parentMatch12 = match;
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
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch16 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch17 = &match;
                                                        {
                                                            std::int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match(false);
                                                            if (*lexer == otava::token::CONSTEXPR)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            if (match.hit)
                                                            {
                                                                constExprPos = lexer.GetSpan(pos);
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch16 = match;
                                                    }
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
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch11 = match;
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
                                                if (*lexer == otava::token::LPAREN)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    lpSpan = lexer.GetSpan(pos);
                                                }
                                                *parentMatch19 = match;
                                            }
                                            *parentMatch18 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch20 = &match;
                                    {
                                        soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Condition(lexer, context);
                                        cond.reset(static_cast<otava::ast::Node*>(match.value));
                                        *parentMatch20 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
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
                                *parentMatch9 = match;
                            }
                            *parentMatch8 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch23 = &match;
                            {
                                soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                                thenStmt.reset(static_cast<otava::ast::Node*>(match.value));
                                *parentMatch23 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch24 = &match;
                        {
                            soul::parser::Match match(true);
                            std::int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch25 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch26 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch27 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch28 = &match;
                                        {
                                            std::int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == otava::token::ELSE)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                elsePos = lexer.GetSpan(pos);
                                            }
                                            *parentMatch28 = match;
                                        }
                                        *parentMatch27 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch29 = &match;
                                        {
                                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                                            elseStmt.reset(static_cast<otava::ast::Node*>(match.value));
                                            *parentMatch29 = match;
                                        }
                                        *parentMatch27 = match;
                                    }
                                    *parentMatch26 = match;
                                }
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
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    otava::ast::IfStatementNode *node = new otava::ast::IfStatementNode(span, lexer.File(), cond.release(), thenStmt.release(), elseStmt.release(), attributes.release(), ifPos, lpSpan, rpSpan, constExprPos, elsePos);
                    node->SetBlockId(blockId);
                    otava::symbols::MapNode(node, context);
                    otava::symbols::EndBlock(context);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IfStatement");
                        #endif
                        return soul::parser::Match(true, node);
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IfStatement");
        else soul::lexer::WriteFailureToLog(lexer, "IfStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::SwitchStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SwitchStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585290);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span switchPos = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    int blockId = int();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> cond;
    std::unique_ptr<otava::ast::Node> stmt;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == otava::token::SWITCH)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            switchPos = lexer.GetSpan(pos);
                                            if (!span.IsValid()) span = switchPos;
                                            blockId = context->NextBlockId();
                                            otava::symbols::BeginBlock(lexer.GetFullSpan(pos), blockId, context);
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
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Condition(lexer, context);
                                    cond.reset(static_cast<otava::ast::Node*>(match.value));
                                    *parentMatch14 = match;
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch8 = match;
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
                                    if (*lexer == otava::token::RPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        rpSpan = lexer.GetSpan(pos);
                                    }
                                    *parentMatch16 = match;
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                            stmt.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch17 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    otava::ast::SwitchStatementNode *node = new otava::ast::SwitchStatementNode(span, lexer.File(), cond.release(), stmt.release(), attributes.release(), switchPos, lpSpan, rpSpan);
                    node->SetBlockId(blockId);
                    otava::symbols::MapNode(node, context);
                    otava::symbols::EndBlock(context);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SwitchStatement");
                        #endif
                        return soul::parser::Match(true, node);
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SwitchStatement");
        else soul::lexer::WriteFailureToLog(lexer, "SwitchStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::IterationStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "IterationStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585291);
    std::unique_ptr<otava::ast::Node> whileStmt;
    std::unique_ptr<otava::ast::Node> doStmt;
    std::unique_ptr<otava::ast::Node> rangeForStmt;
    std::unique_ptr<otava::ast::Node> forStmt;
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
                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::WhileStatement(lexer, context);
                    whileStmt.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IterationStatement");
                            #endif
                            return soul::parser::Match(true, whileStmt.release());
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
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::DoStatement(lexer, context);
                            doStmt.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IterationStatement");
                                    #endif
                                    return soul::parser::Match(true, doStmt.release());
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
                        soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::RangeForStatement(lexer, context);
                        rangeForStmt.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IterationStatement");
                                #endif
                                return soul::parser::Match(true, rangeForStmt.release());
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
                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::ForStatement(lexer, context);
                    forStmt.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IterationStatement");
                            #endif
                            return soul::parser::Match(true, forStmt.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "IterationStatement");
        else soul::lexer::WriteFailureToLog(lexer, "IterationStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::WhileStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "WhileStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585292);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span whilePos = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    int blockId = int();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> cond;
    std::unique_ptr<otava::ast::Node> stmt;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == otava::token::WHILE)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            whilePos = lexer.GetSpan(pos);
                                            if (!span.IsValid()) span = whilePos;
                                            blockId = context->NextBlockId();
                                            otava::symbols::BeginBlock(lexer.GetFullSpan(pos), blockId, context);
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
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Condition(lexer, context);
                                    cond.reset(static_cast<otava::ast::Node*>(match.value));
                                    *parentMatch14 = match;
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch8 = match;
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
                                    if (*lexer == otava::token::RPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        rpSpan = lexer.GetSpan(pos);
                                    }
                                    *parentMatch16 = match;
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                            stmt.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch17 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    otava::ast::WhileStatementNode *node = new otava::ast::WhileStatementNode(span, lexer.File(), cond.release(), stmt.release(), attributes.release(), whilePos, lpSpan, rpSpan);
                    node->SetBlockId(blockId);
                    otava::symbols::MapNode(node, context);
                    otava::symbols::EndBlock(context);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "WhileStatement");
                        #endif
                        return soul::parser::Match(true, node);
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "WhileStatement");
        else soul::lexer::WriteFailureToLog(lexer, "WhileStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::DoStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DoStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585293);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span doPos = soul::ast::Span();
    soul::ast::Span whilePos = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> stmt;
    std::unique_ptr<otava::ast::Node> expr;
    std::unique_ptr<otava::ast::Node> semicolon;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch12 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch13 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == otava::token::DO)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    doPos = lexer.GetSpan(pos);
                                                    if (!span.IsValid()) span = doPos;
                                                }
                                                *parentMatch13 = match;
                                            }
                                            *parentMatch12 = match;
                                        }
                                        if (match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch14 = &match;
                                            {
                                                soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                                                stmt.reset(static_cast<otava::ast::Node*>(match.value));
                                                *parentMatch14 = match;
                                            }
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch11 = match;
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
                                                if (*lexer == otava::token::WHILE)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    whilePos = lexer.GetSpan(pos);
                                                }
                                                *parentMatch16 = match;
                                            }
                                            *parentMatch15 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
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
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch19 = &match;
                                {
                                    soul::parser::Match match = otava::parser::expression::ExpressionParser<LexerT>::Expression(lexer, context);
                                    expr.reset(static_cast<otava::ast::Node*>(match.value));
                                    *parentMatch19 = match;
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch8 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch20 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch21 = &match;
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
                                    *parentMatch21 = match;
                                }
                                *parentMatch20 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch22 = &match;
                        {
                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                            semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch22 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DoStatement");
                        #endif
                        return soul::parser::Match(true, new otava::ast::DoStatementNode(span, lexer.File(), stmt.release(), expr.release(), attributes.release(), semicolon.release(), doPos, whilePos, lpSpan, rpSpan));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DoStatement");
        else soul::lexer::WriteFailureToLog(lexer, "DoStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::RangeForStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RangeForStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585294);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span forPos = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    soul::ast::Span colonPos = soul::ast::Span();
    int blockId = int();
    bool blockOpen = bool();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> initStmt;
    std::unique_ptr<otava::ast::Node> declaration;
    std::unique_ptr<otava::ast::Node> initializer;
    std::unique_ptr<otava::ast::Node> stmt;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
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
                                                    std::int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::token::FOR)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        forPos = lexer.GetSpan(pos);
                                                        if (!span.IsValid()) span = forPos;
                                                        blockId = context->NextBlockId();
                                                        otava::symbols::BeginBlock(lexer.GetFullSpan(pos), blockId, context);
                                                        blockOpen = true;
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
                                                soul::parser::Match match(true);
                                                std::int64_t save = lexer.GetPos();
                                                soul::parser::Match* parentMatch18 = &match;
                                                {
                                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::InitStatement(lexer, context);
                                                    initStmt.reset(static_cast<otava::ast::Node*>(match.value));
                                                    if (match.hit)
                                                    {
                                                        *parentMatch18 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.SetPos(save);
                                                    }
                                                }
                                                *parentMatch17 = match;
                                            }
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch19 = &match;
                                        {
                                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::ForRangeDeclaration(lexer, context);
                                            declaration.reset(static_cast<otava::ast::Node*>(match.value));
                                            *parentMatch19 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch20 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch21 = &match;
                                        {
                                            std::int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == otava::token::COLON)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                colonPos = lexer.GetSpan(pos);
                                            }
                                            *parentMatch21 = match;
                                        }
                                        *parentMatch20 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch22 = &match;
                                {
                                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::ForRangeInitializer(lexer, context);
                                    initializer.reset(static_cast<otava::ast::Node*>(match.value));
                                    *parentMatch22 = match;
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch8 = match;
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
                                    if (*lexer == otava::token::RPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        rpSpan = lexer.GetSpan(pos);
                                    }
                                    *parentMatch24 = match;
                                }
                                *parentMatch23 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch25 = &match;
                        {
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                            stmt.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch25 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    otava::ast::RangeForStatementNode *node = new otava::ast::RangeForStatementNode(span, lexer.File(), initStmt.release(), declaration.release(), initializer.release(), stmt.release(), attributes.release(), forPos, lpSpan, rpSpan, colonPos);
                    node->SetBlockId(blockId);
                    otava::symbols::MapNode(node, context);
                    otava::symbols::EndBlock(context);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RangeForStatement");
                        #endif
                        return soul::parser::Match(true, node);
                    }
                }
                else
                {
                    if (blockOpen)
                    {
                        otava::symbols::RemoveBlock(context);
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RangeForStatement");
        else soul::lexer::WriteFailureToLog(lexer, "RangeForStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ForRangeDeclaration(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ForRangeDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585295);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> node = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> declSpecifierSeq = std::unique_ptr<otava::ast::Node>();
    soul::ast::Span lbSpan = soul::ast::Span();
    soul::ast::Span rbSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> declSpecifierSeq1;
    std::unique_ptr<otava::ast::Node> declarator;
    std::unique_ptr<otava::ast::Node> refQualifier;
    std::unique_ptr<otava::ast::Node> identifiers;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                            soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::DeclSpecifierSeq(lexer, context);
                            declSpecifierSeq1.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                if (!span.IsValid()) span = lexer.GetSpan(pos);
                                declSpecifierSeq.reset(declSpecifierSeq1.release());
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
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::Declarator(lexer, context);
                                        declarator.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            node.reset(new otava::ast::ForRangeDeclarationNode(span, lexer.File(), declSpecifierSeq.release(), declarator.release(), attributes.release()));
                                        }
                                        *parentMatch12 = match;
                                    }
                                    *parentMatch11 = match;
                                    if (!match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch13 = &match;
                                        lexer.SetPos(save);
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch14 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch15 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch16 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch17 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch18 = &match;
                                                            {
                                                                soul::parser::Match match(true);
                                                                std::int64_t save = lexer.GetPos();
                                                                soul::parser::Match* parentMatch19 = &match;
                                                                {
                                                                    soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::RefQualifier(lexer);
                                                                    refQualifier.reset(static_cast<otava::ast::Node*>(match.value));
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
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch20 = &match;
                                                                {
                                                                    soul::parser::Match match(false);
                                                                    soul::parser::Match* parentMatch21 = &match;
                                                                    {
                                                                        std::int64_t pos = lexer.GetPos();
                                                                        soul::parser::Match match(false);
                                                                        if (*lexer == otava::token::LBRACKET)
                                                                        {
                                                                            ++lexer;
                                                                            match.hit = true;
                                                                        }
                                                                        if (match.hit)
                                                                        {
                                                                            lbSpan = lexer.GetSpan(pos);
                                                                        }
                                                                        *parentMatch21 = match;
                                                                    }
                                                                    *parentMatch20 = match;
                                                                }
                                                                *parentMatch18 = match;
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch22 = &match;
                                                            {
                                                                soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::IdentifierList(lexer, context);
                                                                identifiers.reset(static_cast<otava::ast::Node*>(match.value));
                                                                *parentMatch22 = match;
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch16 = match;
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
                                                                if (*lexer == otava::token::RBRACKET)
                                                                {
                                                                    ++lexer;
                                                                    match.hit = true;
                                                                }
                                                                if (match.hit)
                                                                {
                                                                    rbSpan = lexer.GetSpan(pos);
                                                                }
                                                                *parentMatch24 = match;
                                                            }
                                                            *parentMatch23 = match;
                                                        }
                                                        *parentMatch16 = match;
                                                    }
                                                    *parentMatch15 = match;
                                                }
                                                if (match.hit)
                                                {
                                                    node.reset(new otava::ast::StructuredBindingNode(span, lexer.File(), declSpecifierSeq.release(), refQualifier.release(), identifiers.release(), nullptr, attributes.release(), nullptr, lbSpan, rbSpan));
                                                }
                                                *parentMatch14 = match;
                                            }
                                            *parentMatch13 = match;
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
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForRangeDeclaration");
                        #endif
                        return soul::parser::Match(true, node.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForRangeDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "ForRangeDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ForRangeInitializer(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ForRangeInitializer");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585296);
    std::unique_ptr<otava::ast::Node> initializer;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match = otava::parser::initialization::InitializationParser<LexerT>::ExprOrBracedInitList(lexer, context);
        initializer.reset(static_cast<otava::ast::Node*>(match.value));
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForRangeInitializer");
                #endif
                return soul::parser::Match(true, initializer.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForRangeInitializer");
        else soul::lexer::WriteFailureToLog(lexer, "ForRangeInitializer");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ForStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ForStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585297);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span forPos = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    int blockId = int();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> initStmt;
    std::unique_ptr<otava::ast::Node> cond;
    std::unique_ptr<otava::ast::Node> semicolon;
    std::unique_ptr<otava::ast::Node> loopExpr;
    std::unique_ptr<otava::ast::Node> stmt;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
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
                                                    std::int64_t pos = lexer.GetPos();
                                                    soul::parser::Match match(false);
                                                    if (*lexer == otava::token::FOR)
                                                    {
                                                        ++lexer;
                                                        match.hit = true;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        forPos = lexer.GetSpan(pos);
                                                        if (!span.IsValid()) span = forPos;
                                                        blockId = context->NextBlockId();
                                                        otava::symbols::BeginBlock(lexer.GetFullSpan(pos), blockId, context);
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
                                                soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::InitStatement(lexer, context);
                                                initStmt.reset(static_cast<otava::ast::Node*>(match.value));
                                                *parentMatch17 = match;
                                            }
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch18 = &match;
                                        {
                                            soul::parser::Match match(true);
                                            std::int64_t save = lexer.GetPos();
                                            soul::parser::Match* parentMatch19 = &match;
                                            {
                                                soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Condition(lexer, context);
                                                cond.reset(static_cast<otava::ast::Node*>(match.value));
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
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch20 = &match;
                                    {
                                        soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                                        semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                                        *parentMatch20 = match;
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch21 = &match;
                                {
                                    soul::parser::Match match(true);
                                    std::int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch22 = &match;
                                    {
                                        soul::parser::Match match = otava::parser::expression::ExpressionParser<LexerT>::Expression(lexer, context);
                                        loopExpr.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            *parentMatch22 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                        }
                                    }
                                    *parentMatch21 = match;
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch8 = match;
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
                                    if (*lexer == otava::token::RPAREN)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    if (match.hit)
                                    {
                                        rpSpan = lexer.GetSpan(pos);
                                    }
                                    *parentMatch24 = match;
                                }
                                *parentMatch23 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch25 = &match;
                        {
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Statement(lexer, context);
                            stmt.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch25 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    otava::ast::ForStatementNode *node = new otava::ast::ForStatementNode(span, lexer.File(), initStmt.release(), cond.release(), loopExpr.release(), stmt.release(), attributes.release(), semicolon.release(), forPos, lpSpan, rpSpan);
                    node->SetBlockId(blockId);
                    otava::symbols::MapNode(node, context);
                    otava::symbols::EndBlock(context);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForStatement");
                        #endif
                        return soul::parser::Match(true, node);
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ForStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ForStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::JumpStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "JumpStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585298);
    std::unique_ptr<otava::ast::Node> breakStatement;
    std::unique_ptr<otava::ast::Node> continueStatement;
    std::unique_ptr<otava::ast::Node> returnStatement;
    std::unique_ptr<otava::ast::Node> coRoutineReturnStatement;
    std::unique_ptr<otava::ast::Node> gotoStatement;
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
                    std::int64_t save = lexer.GetPos();
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::BreakStatement(lexer, context);
                        breakStatement.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JumpStatement");
                                #endif
                                return soul::parser::Match(true, breakStatement.release());
                            }
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
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
                                soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::ContinueStatement(lexer, context);
                                continueStatement.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JumpStatement");
                                        #endif
                                        return soul::parser::Match(true, continueStatement.release());
                                    }
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
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
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::ReturnStatement(lexer, context);
                            returnStatement.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JumpStatement");
                                    #endif
                                    return soul::parser::Match(true, returnStatement.release());
                                }
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch2 = match;
                }
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
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::CoroutineReturnStatement(lexer, context);
                        coRoutineReturnStatement.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JumpStatement");
                                #endif
                                return soul::parser::Match(true, coRoutineReturnStatement.release());
                            }
                        }
                        *parentMatch10 = match;
                    }
                    *parentMatch9 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
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
                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::GotoStatement(lexer, context);
                    gotoStatement.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JumpStatement");
                            #endif
                            return soul::parser::Match(true, gotoStatement.release());
                        }
                    }
                    *parentMatch12 = match;
                }
                *parentMatch11 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "JumpStatement");
        else soul::lexer::WriteFailureToLog(lexer, "JumpStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::BreakStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "BreakStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585299);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span breakPos = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> semicolon;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                            if (*lexer == otava::token::BREAK)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                breakPos = lexer.GetSpan(pos);
                                if (!span.IsValid()) span = breakPos;
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
                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                            semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch9 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BreakStatement");
                        #endif
                        return soul::parser::Match(true, new otava::ast::BreakStatementNode(span, lexer.File(), attributes.release(), semicolon.release(), breakPos));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "BreakStatement");
        else soul::lexer::WriteFailureToLog(lexer, "BreakStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ContinueStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ContinueStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585300);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span continuePos = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> semicolon;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                            if (*lexer == otava::token::CONTINUE)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                continuePos = lexer.GetSpan(pos);
                                if (!span.IsValid()) span = continuePos;
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
                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                            semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch9 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ContinueStatement");
                        #endif
                        return soul::parser::Match(true, new otava::ast::ContinueStatementNode(span, lexer.File(), attributes.release(), semicolon.release(), continuePos));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ContinueStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ContinueStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ReturnStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ReturnStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585301);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span returnPos = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> returnValue;
    std::unique_ptr<otava::ast::Node> semicolon;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                if (*lexer == otava::token::RETURN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    returnPos = lexer.GetSpan(pos);
                                    if (!span.IsValid()) span = returnPos;
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
                                std::int64_t save = lexer.GetPos();
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    soul::parser::Match match = otava::parser::initialization::InitializationParser<LexerT>::ExprOrBracedInitList(lexer, context);
                                    returnValue.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        *parentMatch11 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                            semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch12 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ReturnStatement");
                        #endif
                        return soul::parser::Match(true, new otava::ast::ReturnStatementNode(span, lexer.File(), returnValue.release(), attributes.release(), semicolon.release(), returnPos));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ReturnStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ReturnStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::CoroutineReturnStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "CoroutineReturnStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585302);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span coReturnPos = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> returnValue;
    std::unique_ptr<otava::ast::Node> semicolon;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                if (*lexer == otava::token::CO_RETURN)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    coReturnPos = lexer.GetSpan(pos);
                                    if (!span.IsValid()) span = coReturnPos;
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
                                std::int64_t save = lexer.GetPos();
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    soul::parser::Match match = otava::parser::initialization::InitializationParser<LexerT>::ExprOrBracedInitList(lexer, context);
                                    returnValue.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        *parentMatch11 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                            semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch12 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CoroutineReturnStatement");
                        #endif
                        return soul::parser::Match(true, new otava::ast::CoReturnStatementNode(span, lexer.File(), returnValue.release(), attributes.release(), semicolon.release(), coReturnPos));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "CoroutineReturnStatement");
        else soul::lexer::WriteFailureToLog(lexer, "CoroutineReturnStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::GotoStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "GotoStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585303);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span gotoPos = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> target;
    std::unique_ptr<otava::ast::Node> semicolon;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                if (*lexer == otava::token::GOTO)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    gotoPos = lexer.GetSpan(pos);
                                    if (!span.IsValid()) span = gotoPos;
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
                                soul::parser::Match match = otava::parser::identifier::IdentifierParser<LexerT>::Identifier(lexer, context);
                                target.reset(static_cast<otava::ast::Node*>(match.value));
                                *parentMatch10 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch11 = &match;
                        {
                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                            semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch11 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GotoStatement");
                        #endif
                        return soul::parser::Match(true, new otava::ast::GotoStatementNode(span, lexer.File(), target.release(), attributes.release(), semicolon.release(), gotoPos));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GotoStatement");
        else soul::lexer::WriteFailureToLog(lexer, "GotoStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ExpressionStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ExpressionStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585304);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> expr = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> expr1;
    std::unique_ptr<otava::ast::Node> semicolon;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                std::int64_t save = lexer.GetPos();
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::parser::expression::ExpressionParser<LexerT>::Expression(lexer, context);
                                    expr1.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        expr.reset(expr1.release());
                                        if (!span.IsValid()) span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                                if (!match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch11 = &match;
                                    lexer.SetPos(save);
                                    {
                                        soul::parser::Match match(true);
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch9 = match;
                                }
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
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                                semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    if (!span.IsValid()) span = lexer.GetSpan(pos);
                                }
                                *parentMatch13 = match;
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExpressionStatement");
                        #endif
                        return soul::parser::Match(true, new otava::ast::ExpressionStatementNode(span, lexer.File(), expr.release(), attributes.release(), semicolon.release()));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExpressionStatement");
        else soul::lexer::WriteFailureToLog(lexer, "ExpressionStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::DeclarationStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DeclarationStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585305);
    soul::ast::Span span = soul::ast::Span();
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
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(true);
                    if (match.hit)
                    {
                        context->PushSetFlag(otava::symbols::ContextFlags::saveDeclarations);
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
                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::BlockDeclaration(lexer, context);
                        declaration.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            span = lexer.GetSpan(pos);
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
            context->PopFlags();
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeclarationStatement");
                #endif
                return soul::parser::Match(true, new otava::ast::DeclarationStatementNode(span, lexer.File(), declaration.release()));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeclarationStatement");
        else soul::lexer::WriteFailureToLog(lexer, "DeclarationStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::TryStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TryStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585306);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span tryPos = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> tryBlock;
    std::unique_ptr<otava::ast::Node> handlers;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
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
                                if (*lexer == otava::token::TRY)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    tryPos = lexer.GetSpan(pos);
                                    if (!span.IsValid()) span = tryPos;
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
                                soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::CompoundStatement(lexer, context);
                                tryBlock.reset(static_cast<otava::ast::Node*>(match.value));
                                *parentMatch10 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch11 = &match;
                        {
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::HandlerSequence(lexer, context);
                            handlers.reset(static_cast<otava::ast::Node*>(match.value));
                            *parentMatch11 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TryStatement");
                        #endif
                        return soul::parser::Match(true, new otava::ast::TryStatementNode(span, lexer.File(), tryBlock.release(), handlers.release(), attributes.release(), tryPos));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TryStatement");
        else soul::lexer::WriteFailureToLog(lexer, "TryStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::HandlerSequence(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "HandlerSequence");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585307);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::HandlerSequenceNode> handlers = std::unique_ptr<otava::ast::HandlerSequenceNode>();
    std::unique_ptr<otava::ast::Node> first;
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
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Handler(lexer, context);
                    first.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                        handlers.reset(new otava::ast::HandlerSequenceNode(span, lexer.File()));
                        handlers->AddNode(first.release());
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
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::Handler(lexer, context);
                                        next.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            handlers->AddNode(next.release());
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
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "HandlerSequence");
                #endif
                return soul::parser::Match(true, handlers.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "HandlerSequence");
        else soul::lexer::WriteFailureToLog(lexer, "HandlerSequence");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::Handler(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Handler");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585308);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span lpSpan = soul::ast::Span();
    soul::ast::Span rpSpan = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> exception;
    std::unique_ptr<otava::ast::Node> catchBlock;
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
                                soul::parser::Match match(false);
                                if (*lexer == otava::token::CATCH)
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
                            soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::ExceptionDeclaration(lexer, context);
                            exception.reset(static_cast<otava::ast::Node*>(match.value));
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
                        soul::parser::Match* parentMatch11 = &match;
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
                            *parentMatch11 = match;
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
                soul::parser::Match* parentMatch12 = &match;
                {
                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::CompoundStatement(lexer, context);
                    catchBlock.reset(static_cast<otava::ast::Node*>(match.value));
                    *parentMatch12 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Handler");
                #endif
                return soul::parser::Match(true, new otava::ast::HandlerNode(span, lexer.File(), exception.release(), catchBlock.release(), lpSpan, rpSpan));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Handler");
        else soul::lexer::WriteFailureToLog(lexer, "Handler");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::ExceptionDeclaration(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ExceptionDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585309);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> declarator = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> typeSpecifiers;
    std::unique_ptr<otava::ast::Node> decl;
    std::unique_ptr<otava::ast::Node> abstractDeclarator;
    std::unique_ptr<otava::ast::Node> ellipsis;
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
                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
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
                std::int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        std::int64_t save = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeSpecifierSeqReset(lexer, context);
                                typeSpecifiers.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    if (!span.IsValid()) span = lexer.GetSpan(pos);
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
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        std::int64_t save = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch13 = &match;
                                        {
                                            std::int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::Declarator(lexer, context);
                                            decl.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                declarator.reset(decl.release());
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
                                        if (!match.hit)
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch14 = &match;
                                            lexer.SetPos(save);
                                            {
                                                soul::parser::Match match(true);
                                                std::int64_t save = lexer.GetPos();
                                                soul::parser::Match* parentMatch15 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch16 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch17 = &match;
                                                        {
                                                            std::int64_t pos = lexer.GetPos();
                                                            soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::AbstractDeclarator(lexer, context);
                                                            abstractDeclarator.reset(static_cast<otava::ast::Node*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                declarator.reset(abstractDeclarator.release());
                                                            }
                                                            *parentMatch17 = match;
                                                        }
                                                        *parentMatch16 = match;
                                                    }
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
                                            *parentMatch12 = match;
                                        }
                                    }
                                    *parentMatch11 = match;
                                }
                                *parentMatch10 = match;
                            }
                            *parentMatch8 = match;
                        }
                        *parentMatch7 = match;
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
                                    soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Ellipsis(lexer);
                                    ellipsis.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        if (!span.IsValid()) span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch19 = match;
                                }
                                *parentMatch18 = match;
                            }
                            *parentMatch7 = match;
                        }
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExceptionDeclaration");
                        #endif
                        return soul::parser::Match(true, new otava::ast::ExceptionDeclarationNode(span, lexer.File(), typeSpecifiers.release(), declarator.release(), ellipsis.release(), attributes.release()));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ExceptionDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "ExceptionDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::InitStatement(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "InitStatement");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585310);
    std::unique_ptr<otava::ast::Node> simpleDeclaration;
    std::unique_ptr<otava::ast::Node> expressionStatement;
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
                soul::parser::Match match(true);
                if (match.hit)
                {
                    context->PushSetFlag(otava::symbols::ContextFlags::saveDeclarations);
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
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            std::int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::SimpleDeclaration(lexer, context);
                                simpleDeclaration.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InitStatement");
                                        #endif
                                        return soul::parser::Match(true, simpleDeclaration.release());
                                    }
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
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
                                        soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::ExpressionStatement(lexer, context);
                                        expressionStatement.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InitStatement");
                                                #endif
                                                return soul::parser::Match(true, expressionStatement.release());
                                            }
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch6 = match;
                            }
                        }
                        *parentMatch5 = match;
                    }
                    if (match.hit)
                    {
                        context->PopFlags();
                    }
                    else
                    {
                        context->PopFlags();
                    }
                    *parentMatch4 = match;
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InitStatement");
        else soul::lexer::WriteFailureToLog(lexer, "InitStatement");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match StatementParser<LexerT>::Condition(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Condition");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 4569043890895585311);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> declSpecifiers = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> declSpecifierSeq;
    std::unique_ptr<otava::ast::Node> declarator;
    std::unique_ptr<otava::ast::Node> initializer;
    std::unique_ptr<otava::ast::Node> expr;
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
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
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
                                    std::int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                                    attributes.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
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
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch8 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::DeclSpecifierSeq(lexer, context);
                                declSpecifierSeq.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    if (!span.IsValid()) span = lexer.GetSpan(pos);
                                    declSpecifiers.reset(declSpecifierSeq.release());
                                }
                                *parentMatch9 = match;
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
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::Declarator(lexer, context);
                        declarator.reset(static_cast<otava::ast::Node*>(match.value));
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
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch12 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::initialization::InitializationParser<LexerT>::BraceOrEqualInitializer(lexer, context);
                        initializer.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Condition");
                                #endif
                                return soul::parser::Match(true, new otava::ast::InitConditionNode(span, lexer.File(), declSpecifiers.release(), declarator.release(), initializer.release(), attributes.release()));
                            }
                        }
                        *parentMatch12 = match;
                    }
                    *parentMatch11 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch13 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch14 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::expression::ExpressionParser<LexerT>::Expression(lexer, context);
                        expr.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Condition");
                                #endif
                                return soul::parser::Match(true, expr.release());
                            }
                        }
                        *parentMatch14 = match;
                    }
                    *parentMatch13 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Condition");
        else soul::lexer::WriteFailureToLog(lexer, "Condition");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct StatementParser<soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t>>;

} // namespace otava::parser::statement
