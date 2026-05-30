
// this file has been automatically generated from 'D:/work/otava/parser/function.parser' using soul parser generator spg version 5.2.0

module otava.parser.function;

import util;
import soul.ast.common;
import soul.ast.spg;
import soul.ast.span;
import soul.ast.lexer_pos_pair;
import otava.token;
import otava.lexer;
import otava.symbols.declaration;
import otava.symbols.symbol_table;
import otava.parser.attribute;
import otava.parser.classes;
import otava.parser.concepts;
import otava.parser.declaration;
import otava.parser.initialization;
import otava.parser.punctuation;
import otava.parser.statement;
import otava.parser.type;

namespace otava::parser::function {

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::FunctionDefinition(LexerT& lexer, otava::symbols::Context* context)
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
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041793);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> declSpecifierSeqNode = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> declaratorNode = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> specifierNode = std::unique_ptr<otava::ast::Node>();
    int scopes = int();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> declSpecifiers;
    std::unique_ptr<otava::ast::Node> declarator;
    std::unique_ptr<otava::ast::Node> virtSpecifiers;
    std::unique_ptr<otava::ast::Node> requiresClause;
    std::unique_ptr<otava::ast::Node> functionBody;
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
                                soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                                attributes.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    lexer.GetSpan(pos);
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
                            soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::DeclSpecifierSeq(lexer, context);
                            declSpecifiers.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                declSpecifierSeqNode.reset(declSpecifiers.release());
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
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::DeclaratorPushClassScope(lexer, context);
                        declarator.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            if (!span.IsValid()) span = lexer.GetSpan(pos);
                            declaratorNode.reset(declarator.release());
                        }
                        else
                        {
                            context->GetSymbolTable()->CurrentScope()->PopParentScope();
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
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch12 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
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
                                case otava::token::ID:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch16 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::classes::ClassParser<LexerT>::VirtSpecifierSeq(lexer, context);
                                        virtSpecifiers.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            specifierNode.reset(virtSpecifiers.release());
                                        }
                                        *parentMatch16 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch15 = match;
                                    }
                                    break;
                                }
                                case otava::token::REQUIRES:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch17 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::concepts::ConceptParser<LexerT>::RequiresClause(lexer, context);
                                        requiresClause.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            specifierNode.reset(requiresClause.release());
                                        }
                                        *parentMatch17 = match;
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
                                soul::parser::Match* parentMatch18 = &match;
                                lexer.SetPos(save);
                                {
                                    soul::parser::Match match(true);
                                    *parentMatch18 = match;
                                }
                                *parentMatch14 = match;
                            }
                        }
                        *parentMatch13 = match;
                    }
                    if (match.hit)
                    {
                        bool get = false;
                        scopes = otava::symbols::BeginFunctionDefinition(declSpecifierSeqNode.get(), declaratorNode.get(), nullptr, specifierNode.get(), get, context);
                    }
                    *parentMatch12 = match;
                }
                *parentMatch11 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
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
                soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::FunctionBody(lexer, context);
                functionBody.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    otava::ast::Node *node = new otava::ast::FunctionDefinitionNode(span, lexer.File(), attributes.release(), declSpecifierSeqNode.release(), declaratorNode.release(), specifierNode.release(), functionBody.release());
                    otava::symbols::EndFunctionDefinition(node, scopes, context);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionDefinition");
                        #endif
                        return soul::parser::Match(true, node);
                    }
                }
                *parentMatch20 = match;
            }
            *parentMatch19 = match;
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
soul::parser::Match FunctionParser<LexerT>::NoDeclSpecFunctionDefinition(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "NoDeclSpecFunctionDefinition");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041794);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> declSpecifierSeqNode = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> declaratorNode = std::unique_ptr<otava::ast::Node>();
    std::unique_ptr<otava::ast::Node> specifierNode = std::unique_ptr<otava::ast::Node>();
    int scopes = int();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> declarator;
    std::unique_ptr<otava::ast::Node> virtSpecifiers;
    std::unique_ptr<otava::ast::Node> requiresClause;
    std::unique_ptr<otava::ast::Node> functionBody;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
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
                            soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                            attributes.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                lexer.GetSpan(pos);
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
                        std::int64_t pos = lexer.GetPos();
                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::DeclaratorPushClassScope(lexer, context);
                        declarator.reset(static_cast<otava::ast::Node*>(match.value));
                        if (match.hit)
                        {
                            if (!span.IsValid()) span = lexer.GetSpan(pos);
                            declaratorNode.reset(declarator.release());
                        }
                        else
                        {
                            context->GetSymbolTable()->CurrentScope()->PopParentScope();
                        }
                        *parentMatch7 = match;
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
            soul::parser::Match* parentMatch8 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch11 = &match;
                        {
                            std::int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch12 = &match;
                            switch (*lexer)
                            {
                                case otava::token::ID:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch13 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::classes::ClassParser<LexerT>::VirtSpecifierSeq(lexer, context);
                                        virtSpecifiers.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            specifierNode.reset(virtSpecifiers.release());
                                        }
                                        *parentMatch13 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch12 = match;
                                    }
                                    break;
                                }
                                case otava::token::REQUIRES:
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch14 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::concepts::ConceptParser<LexerT>::RequiresClause(lexer, context);
                                        requiresClause.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            specifierNode.reset(requiresClause.release());
                                        }
                                        *parentMatch14 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch12 = match;
                                    }
                                    break;
                                }
                            }
                            *parentMatch11 = match;
                            if (!match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch15 = &match;
                                lexer.SetPos(save);
                                {
                                    soul::parser::Match match(true);
                                    *parentMatch15 = match;
                                }
                                *parentMatch11 = match;
                            }
                        }
                        *parentMatch10 = match;
                    }
                    if (match.hit)
                    {
                        bool get = false;
                        scopes = otava::symbols::BeginFunctionDefinition(declSpecifierSeqNode.get(), declaratorNode.get(), nullptr, specifierNode.get(), get, context);
                    }
                    *parentMatch9 = match;
                }
                *parentMatch8 = match;
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
                soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::FunctionBody(lexer, context);
                functionBody.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    otava::ast::Node *node = new otava::ast::FunctionDefinitionNode(span, lexer.File(), attributes.release(), declSpecifierSeqNode.release(), declaratorNode.release(), specifierNode.release(), functionBody.release());
                    otava::symbols::EndFunctionDefinition(node, scopes, context);
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NoDeclSpecFunctionDefinition");
                        #endif
                        return soul::parser::Match(true, node);
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "NoDeclSpecFunctionDefinition");
        else soul::lexer::WriteFailureToLog(lexer, "NoDeclSpecFunctionDefinition");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::FunctionBody(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionBody");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041795);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> assign;
    std::unique_ptr<otava::ast::Node> def;
    std::unique_ptr<otava::ast::Node> semicolon;
    std::unique_ptr<otava::ast::Node> del;
    std::unique_ptr<otava::ast::Node> semicolon2;
    std::unique_ptr<otava::ast::Node> ctorInitializer;
    std::unique_ptr<otava::ast::Node> compoundStatement;
    std::unique_ptr<otava::ast::Node> compoundStatement2;
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
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Assign(lexer);
                    assign.reset(static_cast<otava::ast::Node*>(match.value));
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
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        switch (*lexer)
                        {
                            case otava::token::DEFAULT:
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::Default(lexer);
                                    def.reset(static_cast<otava::ast::Node*>(match.value));
                                    *parentMatch7 = match;
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
                                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                                            semicolon.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionBody");
                                                    #endif
                                                    return soul::parser::Match(true, new otava::ast::DefaultedOrDeletedFunctionNode(span, lexer.File(), assign.release(), def.release(), semicolon.release()));
                                                }
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch6 = match;
                                }
                                break;
                            }
                            case otava::token::DELETE:
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::Delete(lexer);
                                    del.reset(static_cast<otava::ast::Node*>(match.value));
                                    *parentMatch10 = match;
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
                                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Semicolon(lexer);
                                            semicolon2.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionBody");
                                                    #endif
                                                    return soul::parser::Match(true, new otava::ast::DefaultedOrDeletedFunctionNode(span, lexer.File(), assign.release(), del.release(), semicolon2.release()));
                                                }
                                            }
                                            *parentMatch12 = match;
                                        }
                                        *parentMatch11 = match;
                                    }
                                    *parentMatch10 = match;
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
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch15 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::parser::classes::ClassParser<LexerT>::CtorInitializer(lexer, context);
                            ctorInitializer.reset(static_cast<otava::ast::Node*>(match.value));
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
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch17 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::CompoundStatement(lexer, context);
                                compoundStatement.reset(static_cast<otava::ast::Node*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionBody");
                                        #endif
                                        return soul::parser::Match(true, new otava::ast::ConstructorNode(span, lexer.File(), ctorInitializer.release(), compoundStatement.release()));
                                    }
                                }
                                *parentMatch17 = match;
                            }
                            *parentMatch16 = match;
                        }
                        *parentMatch14 = match;
                    }
                    *parentMatch13 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
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
                    soul::parser::Match match = otava::parser::statement::StatementParser<LexerT>::CompoundStatement(lexer, context);
                    compoundStatement2.reset(static_cast<otava::ast::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionBody");
                            #endif
                            return soul::parser::Match(true, new otava::ast::FunctionBodyNode(lexer.GetSpan(pos), lexer.File(), compoundStatement2.release()));
                        }
                    }
                    *parentMatch19 = match;
                }
                *parentMatch18 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionBody");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionBody");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::Default(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Default");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041796);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Default");
                #endif
                return soul::parser::Match(true, new otava::ast::DefaultNode(lexer.GetSpan(pos), lexer.File()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Default");
        else soul::lexer::WriteFailureToLog(lexer, "Default");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::Delete(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Delete");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041797);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::token::DELETE)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Delete");
                #endif
                return soul::parser::Match(true, new otava::ast::DeleteNode(lexer.GetSpan(pos), lexer.File()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Delete");
        else soul::lexer::WriteFailureToLog(lexer, "Delete");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::ParameterDeclarationClause(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ParameterDeclarationClause");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041798);
    std::unique_ptr<otava::ast::Node> comma;
    std::unique_ptr<otava::ast::Node> ellipsis;
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
                context->PushSetFlag(otava::symbols::ContextFlags::parsingParameters);
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
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        std::int64_t save = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::ParameterDeclarationList(lexer, context, container);
                                *parentMatch7 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Comma(lexer);
                                    comma.reset(static_cast<otava::ast::Node*>(match.value));
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch6 = match;
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
                                    soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Ellipsis(lexer);
                                    ellipsis.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        container->AddNode(comma.release());
                                        container->AddNode(ellipsis.release());
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch11 = &match;
                            lexer.SetPos(save);
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
                                                soul::parser::Match match(true);
                                                if (match.hit)
                                                {
                                                    container->Clear();
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
                                                soul::parser::Match match(true);
                                                std::int64_t save = lexer.GetPos();
                                                soul::parser::Match* parentMatch17 = &match;
                                                {
                                                    soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::ParameterDeclarationList(lexer, context, container);
                                                    if (match.hit)
                                                    {
                                                        *parentMatch17 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.SetPos(save);
                                                    }
                                                }
                                                *parentMatch16 = match;
                                            }
                                            *parentMatch14 = match;
                                        }
                                        *parentMatch13 = match;
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
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch20 = &match;
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch21 = &match;
                                                    {
                                                        std::int64_t pos = lexer.GetPos();
                                                        soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Ellipsis(lexer);
                                                        ellipsis.reset(static_cast<otava::ast::Node*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            container->AddNode(ellipsis.release());
                                                        }
                                                        *parentMatch21 = match;
                                                    }
                                                    *parentMatch20 = match;
                                                }
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
                                        *parentMatch13 = match;
                                    }
                                    *parentMatch12 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch22 = &match;
                                    {
                                        soul::parser::Match match(true);
                                        std::int64_t save = lexer.GetPos();
                                        soul::parser::Match* parentMatch23 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch24 = &match;
                                            {
                                                soul::parser::Match match(false);
                                                soul::parser::Match* parentMatch25 = &match;
                                                {
                                                    soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Comma(lexer);
                                                    comma.reset(static_cast<otava::ast::Node*>(match.value));
                                                    *parentMatch25 = match;
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
                                                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Ellipsis(lexer);
                                                            ellipsis.reset(static_cast<otava::ast::Node*>(match.value));
                                                            if (match.hit)
                                                            {
                                                                container->AddNode(comma.release());
                                                                container->AddNode(ellipsis.release());
                                                            }
                                                            *parentMatch27 = match;
                                                        }
                                                        *parentMatch26 = match;
                                                    }
                                                    *parentMatch25 = match;
                                                }
                                                *parentMatch24 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch23 = match;
                                            }
                                            else
                                            {
                                                lexer.SetPos(save);
                                            }
                                        }
                                        *parentMatch22 = match;
                                    }
                                    *parentMatch12 = match;
                                }
                                *parentMatch11 = match;
                            }
                            *parentMatch5 = match;
                        }
                    }
                    *parentMatch4 = match;
                }
                if (match.hit)
                {
                    context->PopFlags();
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParameterDeclarationClause");
        else soul::lexer::WriteFailureToLog(lexer, "ParameterDeclarationClause");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::ParameterDeclarationList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ParameterDeclarationList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041799);
    std::unique_ptr<otava::ast::Node> first;
    std::unique_ptr<otava::ast::Node> comma;
    std::unique_ptr<otava::ast::Node> next;
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
                soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::ParameterDeclaration(lexer, context);
                first.reset(static_cast<otava::ast::Node*>(match.value));
                if (match.hit)
                {
                    container->AddNode(first.release());
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
                soul::parser::Match* parentMatch4 = &match;
                {
                    while (true)
                    {
                        std::int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch5 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch6 = &match;
                                {
                                    soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Comma(lexer);
                                    comma.reset(static_cast<otava::ast::Node*>(match.value));
                                    *parentMatch6 = match;
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
                                            soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::ParameterDeclaration(lexer, context);
                                            next.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                container->AddNode(comma.release());
                                                container->AddNode(next.release());
                                            }
                                            *parentMatch8 = match;
                                        }
                                        *parentMatch7 = match;
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
                                break;
                            }
                        }
                    }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParameterDeclarationList");
        else soul::lexer::WriteFailureToLog(lexer, "ParameterDeclarationList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::ParameterDeclaration(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ParameterDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041800);
    std::unique_ptr<otava::ast::ParameterNode> parameterNode = std::unique_ptr<otava::ast::ParameterNode>();
    std::unique_ptr<otava::ast::Node> attributes;
    std::unique_ptr<otava::ast::Node> declSpecifierSeq;
    std::unique_ptr<otava::ast::Node> declarator;
    std::unique_ptr<otava::ast::Node> abstractDeclarator;
    std::unique_ptr<otava::ast::Node> assign;
    std::unique_ptr<otava::ast::Node> initializer;
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
                parameterNode.reset(new otava::ast::ParameterNode(lexer.GetSpan(pos), lexer.File()));
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
                                            soul::parser::Match match = otava::parser::attribute::AttributeParser<LexerT>::AttributeSpecifierSeq(lexer, context);
                                            attributes.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                parameterNode->SetAttributes(attributes.release());
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
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        std::int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::DeclSpecifierSeq(lexer, context);
                                        declSpecifierSeq.reset(static_cast<otava::ast::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            parameterNode->SetDeclSpecifiers(declSpecifierSeq.release());
                                        }
                                        *parentMatch12 = match;
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
                            soul::parser::Match* parentMatch13 = &match;
                            {
                                soul::parser::Match match(true);
                                std::int64_t save = lexer.GetPos();
                                soul::parser::Match* parentMatch14 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch15 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch16 = &match;
                                        {
                                            std::int64_t save = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch17 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::Declarator(lexer, context);
                                                declarator.reset(static_cast<otava::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    parameterNode->SetDeclarator(declarator.release());
                                                }
                                                *parentMatch17 = match;
                                            }
                                            *parentMatch16 = match;
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
                                                        soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::AbstractDeclarator(lexer, context);
                                                        abstractDeclarator.reset(static_cast<otava::ast::Node*>(match.value));
                                                        if (match.hit)
                                                        {
                                                            parameterNode->SetDeclarator(abstractDeclarator.release());
                                                        }
                                                        *parentMatch19 = match;
                                                    }
                                                    *parentMatch18 = match;
                                                }
                                                *parentMatch16 = match;
                                            }
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
                                    }
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
                        soul::parser::Match* parentMatch20 = &match;
                        {
                            soul::parser::Match match(true);
                            std::int64_t save = lexer.GetPos();
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
                                            soul::parser::Match match = otava::parser::punctuation::PunctuationParser<LexerT>::Assign(lexer);
                                            assign.reset(static_cast<otava::ast::Node*>(match.value));
                                            if (match.hit)
                                            {
                                                parameterNode->SetAssign(assign.release());
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
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch26 = &match;
                                            {
                                                std::int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = otava::parser::initialization::InitializationParser<LexerT>::InitializerClause(lexer, context);
                                                initializer.reset(static_cast<otava::ast::Node*>(match.value));
                                                if (match.hit)
                                                {
                                                    parameterNode->SetInitializer(initializer.release());
                                                }
                                                *parentMatch26 = match;
                                            }
                                            *parentMatch25 = match;
                                        }
                                        *parentMatch23 = match;
                                    }
                                    *parentMatch22 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch21 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
                            }
                            *parentMatch20 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch4 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParameterDeclaration");
                        #endif
                        return soul::parser::Match(true, parameterNode.release());
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ParameterDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "ParameterDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::OperatorFunctionId(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "OperatorFunctionId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041801);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> operatorKw;
    std::unique_ptr<otava::ast::Node> op;
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
                    soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::OperatorKw(lexer);
                    operatorKw.reset(static_cast<otava::ast::Node*>(match.value));
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
                    soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::Operator(lexer, context);
                    op.reset(static_cast<otava::ast::Node*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionId");
                #endif
                return soul::parser::Match(true, new otava::ast::OperatorFunctionIdNode(span, lexer.File(), operatorKw.release(), op.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorFunctionId");
        else soul::lexer::WriteFailureToLog(lexer, "OperatorFunctionId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::ConversionFunctionId(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConversionFunctionId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041802);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> operatorKw;
    std::unique_ptr<otava::ast::Node> conversionTypeId;
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
                    soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::OperatorKw(lexer);
                    operatorKw.reset(static_cast<otava::ast::Node*>(match.value));
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
                    soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::ConversionTypeId(lexer, context);
                    conversionTypeId.reset(static_cast<otava::ast::Node*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionFunctionId");
                #endif
                return soul::parser::Match(true, new otava::ast::ConversionFunctionIdNode(span, lexer.File(), operatorKw.release(), conversionTypeId.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionFunctionId");
        else soul::lexer::WriteFailureToLog(lexer, "ConversionFunctionId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::DestructorId(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DestructorId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041803);
    std::unique_ptr<otava::ast::Node> node = std::unique_ptr<otava::ast::Node>();
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> typeName;
    std::unique_ptr<otava::ast::Node> declTypeSpecifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            std::int64_t pos = lexer.GetPos();
            soul::parser::Match match(false);
            if (*lexer == otava::token::TILDE)
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
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        std::int64_t save = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            std::int64_t pos = lexer.GetPos();
                            soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeName(lexer, context);
                            typeName.reset(static_cast<otava::ast::Node*>(match.value));
                            if (match.hit)
                            {
                                node.reset(typeName.release());
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
                                    soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::DeclTypeSpecifier(lexer, context);
                                    declTypeSpecifier.reset(static_cast<otava::ast::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        node.reset(declTypeSpecifier.release());
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch5 = match;
                        }
                    }
                    *parentMatch4 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DestructorId");
                        #endif
                        return soul::parser::Match(true, new otava::ast::DestructorIdNode(span, lexer.File(), node.release()));
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DestructorId");
        else soul::lexer::WriteFailureToLog(lexer, "DestructorId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::ConversionTypeId(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConversionTypeId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041804);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> typeSpecifierSeq;
    std::unique_ptr<otava::ast::Node> declarator;
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
                    soul::parser::Match match = otava::parser::type::TypeParser<LexerT>::TypeSpecifierSeq(lexer, context);
                    typeSpecifierSeq.reset(static_cast<otava::ast::Node*>(match.value));
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
                        soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::ConversionDeclarator(lexer, context);
                        declarator.reset(static_cast<otava::ast::Node*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionTypeId");
                #endif
                return soul::parser::Match(true, new otava::ast::ConversionTypeIdNode(span, lexer.File(), typeSpecifierSeq.release(), declarator.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionTypeId");
        else soul::lexer::WriteFailureToLog(lexer, "ConversionTypeId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::ConversionDeclarator(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConversionDeclarator");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041805);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<otava::ast::Node> op;
    std::unique_ptr<otava::ast::Node> declarator;
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
                    soul::parser::Match match = otava::parser::declaration::DeclarationParser<LexerT>::PtrOperator(lexer, context);
                    op.reset(static_cast<otava::ast::Node*>(match.value));
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
                        soul::parser::Match match = otava::parser::function::FunctionParser<LexerT>::ConversionDeclarator(lexer, context);
                        declarator.reset(static_cast<otava::ast::Node*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionDeclarator");
                #endif
                return soul::parser::Match(true, new otava::ast::ConversionDeclaratorNode(span, lexer.File(), op.release(), declarator.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConversionDeclarator");
        else soul::lexer::WriteFailureToLog(lexer, "ConversionDeclarator");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::OperatorKw(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "OperatorKw");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041806);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
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
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorKw");
                #endif
                return soul::parser::Match(true, new otava::ast::OperatorNode(lexer.GetSpan(pos), lexer.File()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OperatorKw");
        else soul::lexer::WriteFailureToLog(lexer, "OperatorKw");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::Operator(LexerT& lexer, otava::symbols::Context* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Operator");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041807);
    soul::ast::Span span = soul::ast::Span();
    soul::ast::Span lbSpan = soul::ast::Span();
    soul::ast::Span rbSpan = soul::ast::Span();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        switch (*lexer)
        {
            case otava::token::DELETE:
            case otava::token::NEW:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                {
                    std::int64_t save = lexer.GetPos();
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch3 = &match;
                    switch (*lexer)
                    {
                        case otava::token::NEW:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch4 = &match;
                            {
                                std::int64_t save = lexer.GetPos();
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
                                            if (*lexer == otava::token::NEW)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
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
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch8 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch9 = &match;
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
                                                *parentMatch9 = match;
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
                                    soul::parser::Match* parentMatch10 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch11 = &match;
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
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                                                    #endif
                                                    return soul::parser::Match(true, new otava::ast::NewArrayOpNode(span, lexer.File(), lbSpan, rbSpan));
                                                }
                                            }
                                            *parentMatch11 = match;
                                        }
                                        *parentMatch10 = match;
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
                                            std::int64_t pos = lexer.GetPos();
                                            soul::parser::Match match(false);
                                            if (*lexer == otava::token::NEW)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                {
                                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                                                    #endif
                                                    return soul::parser::Match(true, new otava::ast::NewOpNode(lexer.GetSpan(pos), lexer.File()));
                                                }
                                            }
                                            *parentMatch13 = match;
                                        }
                                        *parentMatch12 = match;
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
                        case otava::token::DELETE:
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
                                        if (*lexer == otava::token::DELETE)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            span = lexer.GetSpan(pos);
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
                                            if (*lexer == otava::token::LBRACKET)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            if (match.hit)
                                            {
                                                lbSpan = span;
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
                                        if (*lexer == otava::token::RBRACKET)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            rbSpan = span;
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                                                #endif
                                                return soul::parser::Match(true, new otava::ast::DeleteArrayOpNode(span, lexer.File(), lbSpan, rbSpan));
                                            }
                                        }
                                        *parentMatch20 = match;
                                    }
                                    *parentMatch19 = match;
                                }
                                *parentMatch14 = match;
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
                        soul::parser::Match* parentMatch21 = &match;
                        lexer.SetPos(save);
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch22 = &match;
                            {
                                std::int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == otava::token::DELETE)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                                        #endif
                                        return soul::parser::Match(true, new otava::ast::DeleteOpNode(lexer.GetSpan(pos), lexer.File()));
                                    }
                                }
                                *parentMatch22 = match;
                            }
                            *parentMatch21 = match;
                        }
                        *parentMatch2 = match;
                    }
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::CO_AWAIT:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch23 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::CO_AWAIT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::CoAwaitOpNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch23 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::LPAREN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch24 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch25 = &match;
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
                            span = lexer.GetSpan(pos);
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
                            if (*lexer == otava::token::RPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                                    #endif
                                    return soul::parser::Match(true, new otava::ast::InvokeOpNode(span, lexer.File()));
                                }
                            }
                            *parentMatch27 = match;
                        }
                        *parentMatch26 = match;
                    }
                    *parentMatch24 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::LBRACKET:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch28 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch29 = &match;
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
                            span = lexer.GetSpan(pos);
                        }
                        *parentMatch29 = match;
                    }
                    *parentMatch28 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch30 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch31 = &match;
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
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                                    #endif
                                    return soul::parser::Match(true, new otava::ast::SubscriptOpNode(span, lexer.File()));
                                }
                            }
                            *parentMatch31 = match;
                        }
                        *parentMatch30 = match;
                    }
                    *parentMatch28 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::ARROW:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch32 = &match;
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
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ArrowNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch32 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::ARROW_STAR:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch33 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::ARROW_STAR)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ArrowStarNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch33 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::TILDE:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch34 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::TILDE)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ComplementNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch34 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::EXCLAMATION:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch35 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::EXCLAMATION)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::NotNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch35 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::PLUS:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch36 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::PLUS)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::PlusNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch36 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::MINUS:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch37 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::MINUS)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::MinusNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch37 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::STAR:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch38 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::STAR)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::MulNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch38 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::DIV:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch39 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::DIV)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::DivNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch39 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::MOD:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch40 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::MOD)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ModNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch40 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::XOR:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch41 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::XOR)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ExclusiveOrNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch41 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::AMP:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch42 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::AMP)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::AndNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch42 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::OR:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch43 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::OR)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::InclusiveOrNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch43 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch44 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::AssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch44 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::ADD_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch45 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::ADD_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::PlusAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch45 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::SUB_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch46 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::SUB_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::MinusAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch46 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::MUL_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch47 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::MUL_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::MulAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch47 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::DIV_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch48 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::DIV_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::DivAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch48 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::REM_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch49 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::REM_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ModAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch49 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::XOR_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch50 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::XOR_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::XorAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch50 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::AND_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch51 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::AND_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::AndAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch51 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::OR_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch52 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::OR_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::OrAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch52 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::SHIFT_LEFT_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch53 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::SHIFT_LEFT_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ShiftLeftAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch53 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::SHIFT_RIGHT_ASSIGN:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch54 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::SHIFT_RIGHT_ASSIGN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ShiftRightAssignNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch54 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::EQ:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch55 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::EQ)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::EqualNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch55 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::NEQ:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch56 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::NEQ)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::NotEqualNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch56 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::LEQ:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch57 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::LEQ)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::LessOrEqualNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch57 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::GEQ:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch58 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::GEQ)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::GreaterOrEqualNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch58 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::SPACE_SHIP:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch59 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::SPACE_SHIP)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::CompareNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch59 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::LANGLE:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch60 = &match;
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
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::LessNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch60 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::RANGLE:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch61 = &match;
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
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::GreaterNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch61 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::AMP_AMP:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch62 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::AMP_AMP)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ConjunctionNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch62 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::OR_OR:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch63 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::OR_OR)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::DisjunctionNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch63 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::SHIFT_LEFT:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch64 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::SHIFT_LEFT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ShiftLeftNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch64 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::SHIFT_RIGHT:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch65 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::SHIFT_RIGHT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::ShiftRightNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch65 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::PLUS_PLUS:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch66 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::PLUS_PLUS)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::PrefixIncNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch66 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::MINUS_MINUS:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch67 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::MINUS_MINUS)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::PrefixDecNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch67 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case otava::token::COMMA:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch68 = &match;
                {
                    std::int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == otava::token::COMMA)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
                            #endif
                            return soul::parser::Match(true, new otava::ast::CommaNode(lexer.GetSpan(pos), lexer.File()));
                        }
                    }
                    *parentMatch68 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Operator");
        else soul::lexer::WriteFailureToLog(lexer, "Operator");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match FunctionParser<LexerT>::Noexcept(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    std::int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Noexcept");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 3334239814043041808);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        std::int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == otava::token::NOEXCEPT)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Noexcept");
                #endif
                return soul::parser::Match(true, new otava::ast::NoexceptNode(lexer.GetSpan(pos), lexer.File()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Noexcept");
        else soul::lexer::WriteFailureToLog(lexer, "Noexcept");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct FunctionParser<soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t>>;

} // namespace otava::parser::function
