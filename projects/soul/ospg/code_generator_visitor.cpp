module soul.spg.code_generator_visitor;

import soul.spg.parsing_util;
import soul.spg.code_modifier;
import soul.spg.array_generator;
import soul.lexer.error;

namespace soul::spg {

CodeGeneratorVisitor::CodeGeneratorVisitor(soul::ast::spg::SpgFile* spgFile_, bool verbose_, bool noDebugSupport_, const std::string& version_, 
    soul::lexer::FileMap& fileMap_) :
    spgFile(spgFile_), verbose(verbose_), noDebugSupport(noDebugSupport_), version(version_), formatter(nullptr),
    stage(CodeGenerationStage::generateInterface), currentParser(nullptr), currentRule(nullptr), parentMatchNumber(0), setParentMatchNumber(-1), sn(0), fileMap(fileMap_)
{
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::ChoiceParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        int prevSetParentMatchNumber0 = setParentMatchNumber;
        formatter->WriteLine("soul::parser::Match match(false);");
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("std::int64_t save = lexer.GetPos();");
        parser.Left()->Accept(*this);
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->WriteLine("if (!match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        int prevSetParentMatchNumber1 = setParentMatchNumber;
        formatter->WriteLine("soul::parser::Match match(false);");
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("lexer.SetPos(save);");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Right()->Accept(*this);
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber1;
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber0;
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::SwitchParser& parser)
{
    int prevSetParentMatchNumber0 = setParentMatchNumber;
    formatter->WriteLine("soul::parser::Match match(false);");
    setParentMatchNumber = parentMatchNumber;
    formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
    formatter->WriteLine("switch (*lexer)");
    formatter->WriteLine("{");
    formatter->IncIndent();
    for (const auto& caseParser : parser.CaseParsers())
    {
        caseParser->Accept(*this);
    }
    formatter->DecIndent();
    formatter->WriteLine("}");
    setParentMatchNumber = prevSetParentMatchNumber0;
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::CaseParser& parser)
{
    for (const auto* token : parser.First().Tokens())
    {
        if (!token->IsEpsilon() && !token->IsAny())
        {
            formatter->WriteLine("case " + token->FullCppId() + ":");
        }
    }
    formatter->WriteLine("{");
    formatter->IncIndent();
    parser.Child()->Accept(*this);
    formatter->WriteLine("if (match.hit)");
    formatter->WriteLine("{");
    formatter->IncIndent();
    formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
    formatter->DecIndent();
    formatter->WriteLine("}");
    formatter->WriteLine("break;");
    formatter->DecIndent();
    formatter->WriteLine("}");
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::SequenceParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(false);");
        int prevSetParentMatchNumber0 = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Left()->Accept(*this);
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("if (match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("soul::parser::Match match(false);");
        int prevSetParentMatchNumber1 = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Right()->Accept(*this);
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber1;
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber0;
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::DifferenceParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(false);");
        int prevSetParentMatchNumber0 = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("std::int64_t save = lexer.GetPos();");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Left()->Accept(*this);
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("if (match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("soul::parser::Match match(false);");
        int prevSetParentMatchNumber1 = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("std::int64_t tmp = lexer.GetPos();");
        formatter->WriteLine("lexer.SetPos(save);");
        formatter->WriteLine("save = tmp;");
        parser.Right()->Accept(*this);
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber1;
        formatter->WriteLine("if (!match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("lexer.SetPos(save);");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = soul::parser::Match(!match.hit, match.value);");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber0;
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::ListParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        parser.Child()->Accept(*this);
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::LookaheadParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(true);");
        formatter->WriteLine("std::int64_t save = lexer.GetPos();");
        int prevSetParentMatchNumber = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Child()->Accept(*this);
        formatter->WriteLine("if (match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->WriteLine("lexer.SetPos(save);");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("else");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = soul::parser::Match(false);");
        formatter->WriteLine("lexer.SetPos(save);");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber;
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::KleeneParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(true);");
        int prevSetParentMatchNumber = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("while (true)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("std::int64_t save = lexer.GetPos();");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Child()->Accept(*this);
        formatter->WriteLine("if (match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("else");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("lexer.SetPos(save);");
        formatter->WriteLine("break;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber;
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::PositiveParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(false);");
        int prevSetParentMatchNumber0 = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Child()->Accept(*this);
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("if (match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("soul::parser::Match match(true);");
        int prevSetParentMatchNumber1 = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("while (true)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("std::int64_t save = lexer.GetPos();");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Child()->Accept(*this);
        formatter->WriteLine("if (match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("else");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("lexer.SetPos(save);");
        formatter->WriteLine("break;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber1;
        setParentMatchNumber = prevSetParentMatchNumber0;
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::OptionalParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(true);");
        formatter->WriteLine("std::int64_t save = lexer.GetPos();");
        int prevSetParentMatchNumber = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Child()->Accept(*this);
        formatter->WriteLine("if (match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("else");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("lexer.SetPos(save);");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber;
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::ExpectationParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(true);");
        int prevSetParentMatchNumber0 = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("std::int64_t pos = lexer.GetPos();");
        parser.Child()->Accept(*this);
        formatter->WriteLine("if (match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("else");
        formatter->WriteLine("{");
        formatter->IncIndent();
        if (parser.Child()->IsNonterminalParser())
        {
            soul::ast::spg::NonterminalParser* nonterminalParser = static_cast<soul::ast::spg::NonterminalParser*>(parser.Child());
            soul::ast::spg::RuleParser* rule = nonterminalParser->Rule();
            std::string ruleInfo = rule->Info();
            if (ruleInfo.empty())
            {
                ruleInfo = rule->Name();
            }
            formatter->WriteLine("lexer.ThrowExpectationFailure(pos, \"" + ruleInfo + "\");");
        }
        else if (parser.Child()->IsTokenParser())
        {
            soul::ast::spg::TokenParser* tokenParser = static_cast<soul::ast::spg::TokenParser*>(parser.Child());
            std::string tokenName = tokenParser->GetToken()->FullCppId();
            formatter->WriteLine("lexer.ThrowExpectationFailure(pos, lexer.GetTokenInfo(" + tokenName + "));");
        }
        else
        {
            std::string parserName = parser.Child()->Name();
            formatter->WriteLine("lexer.ThrowExpectationFailure(pos, \"" + parserName + "\");");
        }
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber0;
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::ActionParser& parser)
{
    int prevSetParentMatchNumber = setParentMatchNumber;
    formatter->WriteLine("soul::parser::Match match(false);");
    setParentMatchNumber = parentMatchNumber;
    formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
    formatter->WriteLine("{");
    formatter->IncIndent();
    formatter->WriteLine("std::int64_t pos = lexer.GetPos();");
    soul::ast::cpp::CodeEvaluationVisitor codeEvaluationVisitor;
    parser.SuccessCode()->Accept(codeEvaluationVisitor);
    if (codeEvaluationVisitor.HasReturn())
    {
        currentRule->SetHasReturn();
    }
    bool hasPass = codeEvaluationVisitor.HasPass();
    bool hasVars = codeEvaluationVisitor.HasVars();
    if (hasPass)
    {
        formatter->WriteLine("bool pass = true;");
    }
    if (hasVars)
    {
        formatter->WriteLine("auto vars = static_cast<typename LexerT::VariableClassType*>(lexer.GetVariables());");
    }
    parser.Child()->Accept(*this);
    formatter->WriteLine("if (match.hit)");
    bool nonterminalValue = parser.Child()->IsNonterminalParser();
    bool ptrType = false;
    if (currentRule->ReturnType() != nullptr)
    {
        ptrType = currentRule->ReturnType()->IsPtrType();
    }
    std::string nonterminalName;
    soul::ast::cpp::TypeIdNode* returnType = nullptr;
    if (!ptrType)
    {
        returnType = currentRule->ReturnType();
    }
    if (nonterminalValue)
    {
        soul::ast::spg::NonterminalParser* nt = static_cast<soul::ast::spg::NonterminalParser*>(parser.Child());
        nonterminalName = nt->InstanceName();
    }
    soul::spg::parsing::util::CountNonterminals(parser.SuccessCode(), nonterminalInfos);
    soul::ast::common::TokenMap* tokenMap = parser.GetTokenMap();
    if (tokenMap)
    {
        ModifyCode(parser.SuccessCode(), ptrType, nonterminalName, nonterminalInfos, returnType, noDebugSupport, currentRule->Name(), tokenMap, fileMap);
    }
    else
    {
        throw std::runtime_error("token map not set");
    }
    parser.SuccessCode()->Write(*formatter);
    if (parser.FailureCode())
    {
        formatter->WriteLine("else");
        parser.FailureCode()->Write(*formatter);
    }
    if (hasPass)
    {
        formatter->WriteLine("if (match.hit && !pass)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("match = soul::parser::Match(false);");
        formatter->DecIndent();
        formatter->WriteLine("}");
    }
    formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
    setParentMatchNumber = prevSetParentMatchNumber;
    formatter->DecIndent();
    formatter->WriteLine("}");
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::NonterminalParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        soul::ast::spg::RuleParser* rule = parser.Rule();
        soul::ast::spg::GrammarParser* grammar = rule->Grammar();
        soul::ast::spg::ParserFile* parserFile = grammar->GetParserFile();
        std::string ruleName = parserFile->GetExportModule()->NamespaceName() + "::" + grammar->Name() + "<LexerT>::" + rule->Name();
        formatter->Write("soul::parser::Match match = " + ruleName + "(lexer");
        if (parser.Arguments())
        {
            for (const auto& arg : parser.Arguments()->Exprs())
            {
                formatter->Write(", ");
                arg->Write(*formatter);
            }
        }
        formatter->WriteLine(");");
        soul::ast::spg::RuleParser* calledRule = parser.Rule();
        if (calledRule->ReturnType() != nullptr)
        {
            formatter->Write(parser.InstanceName() + ".reset(");
            if (calledRule->ReturnType()->IsPtrType())
            {
                formatter->Write("static_cast<");
                calledRule->ReturnType()->Write(*formatter);
                formatter->WriteLine(">(match.value));");
            }
            else
            {
                formatter->Write("static_cast<soul::parser::Value<");
                calledRule->ReturnType()->Write(*formatter);
                formatter->WriteLine(">*>(match.value));");
            }
        }
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::EmptyParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(true);");
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::AnyParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(false);");
        formatter->WriteLine("if (*lexer != soul::lexer::END_TOKEN)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("++lexer;");
        formatter->WriteLine("match.hit = true;");
        formatter->DecIndent();
        formatter->WriteLine("}");
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::TokenParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(false);");
        formatter->WriteLine("if (*lexer == " + parser.GetToken()->FullCppId() + ")");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("++lexer;");
        formatter->WriteLine("match.hit = true;");
        formatter->DecIndent();
        formatter->WriteLine("}");
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::CharParser& parser)
{
    formatter->WriteLine("soul::parser::Match match(false);");
    formatter->WriteLine("if (*lexer == " + std::to_string(static_cast<std::int32_t>(parser.Chr())) + ")");
    formatter->WriteLine("{");
    formatter->IncIndent();
    formatter->WriteLine("++lexer;");
    formatter->WriteLine("match.hit = true;");
    formatter->DecIndent();
    formatter->WriteLine("}");
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::StringParser& parser)
{
    formatter->WriteLine("soul::parser::Match match(true);");
    formatter->WriteLine("for (std::int32_t i : " + parser.ArrayName() + ")");
    formatter->WriteLine("{");
    formatter->IncIndent();
    formatter->WriteLine("if (*lexer == i)");
    formatter->WriteLine("{");
    formatter->IncIndent();
    formatter->WriteLine("++lexer;");
    formatter->DecIndent();
    formatter->WriteLine("}");
    formatter->WriteLine("else");
    formatter->WriteLine("{");
    formatter->IncIndent();
    formatter->WriteLine("match.hit = false;");
    formatter->WriteLine("break;");
    formatter->DecIndent();
    formatter->WriteLine("}");
    formatter->DecIndent();
    formatter->WriteLine("}");
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::CharSetParser& parser)
{
    if (parser.GetCharSet()->Inverse())
    {
        formatter->WriteLine("soul::parser::Match match(lexer.Pos() != lexer.End());");
        formatter->WriteLine("for (const soul::ast::spg::Range& range : " + parser.ArrayName() + ")");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("if (*lexer >= range.first && *lexer <= range.last)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("match.hit = false;");
        formatter->WriteLine("break;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("if (match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("++lexer;");
        formatter->DecIndent();
        formatter->WriteLine("}");
    }
    else
    {
        formatter->WriteLine("soul::parser::Match match(false);");
        formatter->WriteLine("for (const soul::ast::spg::Range& range : " + parser.ArrayName() + ")");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("if (*lexer >= range.first && *lexer <= range.last)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("match.hit = true;");
        formatter->WriteLine("++lexer;");
        formatter->WriteLine("break;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->DecIndent();
        formatter->WriteLine("}");
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::GroupParser& parser)
{
    if (stage == CodeGenerationStage::generateImplementation)
    {
        formatter->WriteLine("soul::parser::Match match(false);");
        int prevSetParentMatchNumber = setParentMatchNumber;
        setParentMatchNumber = parentMatchNumber;
        formatter->WriteLine("soul::parser::Match* parentMatch" + std::to_string(parentMatchNumber++) + " = &match;");
        formatter->WriteLine("{");
        formatter->IncIndent();
        parser.Child()->Accept(*this);
        formatter->WriteLine("*parentMatch" + std::to_string(setParentMatchNumber) + " = match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        setParentMatchNumber = prevSetParentMatchNumber;
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::RuleParser& parser)
{
    parentMatchNumber = 0;
    setParentMatchNumber = -1;
    currentRule = &parser;
    if (stage == CodeGenerationStage::generateInterface)
    {
        formatter->Write("static soul::parser::Match " + parser.Name() + "(LexerT& lexer");
        for (const auto& param : parser.Params())
        {
            formatter->Write(", ");
            param->Type()->Write(*formatter);
            formatter->Write(" ");
            formatter->Write(param->Name());
        }
        formatter->WriteLine(");");
    }
    else if (stage == CodeGenerationStage::generateImplementation)
    {
        bool first = false;
        if (!currentParser->Rules().empty())
        {
            if (currentParser->Rules().front().get() == &parser)
            {
                first = true;
            }
        }
        if (!first)
        {
            formatter->WriteLine();
        }
        formatter->WriteLine("template<typename LexerT>");
        formatter->Write("soul::parser::Match " + parser.Grammar()->Name() + "<LexerT>::" + parser.Name() + "(LexerT& lexer");
        for (const auto& param : parser.Params())
        {
            formatter->Write(", ");
            param->Type()->Write(*formatter);
            formatter->Write(" ");
            formatter->Write(param->Name());
        }
        formatter->WriteLine(")");
        formatter->WriteLine("{");
        formatter->IncIndent();
        if (!noDebugSupport)
        {
            formatter->WriteLine("#ifdef SOUL_PARSER_DEBUG_SUPPORT");
            formatter->WriteLine("std::int64_t parser_debug_match_pos = 0;");
            formatter->WriteLine("bool parser_debug_write_to_log = lexer.Log() != nullptr;");
            formatter->WriteLine("if (parser_debug_write_to_log)");
            formatter->WriteLine("{");
            formatter->IncIndent();
            formatter->WriteLine("parser_debug_match_pos = lexer.GetPos();");
            formatter->WriteLine("soul::lexer::WriteBeginRuleToLog(lexer, \"" + parser.Name() + "\");");
            formatter->DecIndent();
            formatter->WriteLine("}");
            formatter->WriteLine("#endif");
        }
        formatter->WriteLine("soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, " + std::to_string(parser.Id()) + ");");
        for (const auto& variable : parser.Vars())
        {
            variable->Type()->Write(*formatter);
            formatter->Write(" ");
            formatter->Write(variable->Name());
            formatter->Write(" = ");
            if (variable->Type()->IsPtrType())
            {
                formatter->WriteLine("nullptr;");
            }
            else
            {
                variable->Type()->Write(*formatter);
                formatter->WriteLine("();");
            }
        }
        nonterminalInfos.clear();
        for (auto nonterminal : parser.Nonterminals())
        {
            bool found = false;
            for (const auto& info : nonterminalInfos)
            {
                if (info.nonterminalParser->InstanceName() == nonterminal->InstanceName())
                {
                    found = true;
                    break;
                }
            }
            if (found) continue;
            soul::ast::spg::RuleParser* calledRule = nonterminal->Rule();
            if (!calledRule)
            {
                throw std::runtime_error("rule not set");
            }
            if (calledRule->ReturnType() != nullptr)
            {
                if (calledRule->ReturnType()->IsPtrType())
                {
                    formatter->Write("std::unique_ptr<");
                    calledRule->ReturnType()->WriteNonPtrType(*formatter);
                    formatter->Write(">");
                    nonterminalInfos.push_back(NonterminalInfo(nonterminal, true));
                }
                else
                {
                    formatter->Write("std::unique_ptr<soul::parser::Value<");
                    calledRule->ReturnType()->Write(*formatter);
                    formatter->Write(">>");
                    nonterminalInfos.push_back(NonterminalInfo(nonterminal, false));
                }
                formatter->WriteLine(" " + nonterminal->InstanceName() + ";");
            }
        }
        parser.Definition()->Accept(*this);
        if (!noDebugSupport)
        {
            formatter->WriteLine("#ifdef SOUL_PARSER_DEBUG_SUPPORT");
            formatter->WriteLine("if (parser_debug_write_to_log)");
            formatter->WriteLine("{");
            formatter->IncIndent();
            formatter->WriteLine("if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, \"" + parser.Name() + "\");");
            formatter->WriteLine("else soul::lexer::WriteFailureToLog(lexer, \"" + parser.Name() + "\");");
            formatter->DecIndent();
            formatter->WriteLine("}");
            formatter->WriteLine("#endif");
        }
        formatter->WriteLine("if (!match.hit)");
        formatter->WriteLine("{");
        formatter->IncIndent();
        formatter->WriteLine("match.value = nullptr;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        formatter->WriteLine("return match;");
        formatter->DecIndent();
        formatter->WriteLine("}");
        for (const auto& info : nonterminalInfos)
        {
            if (info.ptrType && info.count > 1)
            {
                if (!soul::spg::parsing::util::IsListParserNonterminal(info))
                {
                    std::string message = soul::lexer::MakeMessage("warning",
                        "unique pointer value of nonterminal '" + info.nonterminalParser->InstanceName() + "' used " + std::to_string(info.count) +
                        " times in semantic actions of rule '" +
                        parser.Name() + "' of parser '" + parser.Grammar()->Name() + "'", parser.GetSourcePos(), fileMap);
                    for (const auto& sourcePos : info.sourcePositions)
                    {
                        message.append("\n").append(soul::lexer::MakeMessage("info", "see reference", sourcePos, fileMap));
                    }
                    std::cout << message << "\n";
                }
            }
        }
        if (parser.ReturnType() != nullptr && !parser.HasReturn())
        {
            std::cout << "warning: rule '" + parser.Name() +
                "' of parser '" + parser.Grammar()->Name() << "' returns value, but no semantic action has a return statement." << "\n";
        }
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::GrammarParser& parser)
{
    currentParser = &parser;
    if (stage == CodeGenerationStage::generateInterface)
    {
        formatter->WriteLine("template<typename LexerT>");
        formatter->WriteLine("struct " + parser.Name());
        formatter->WriteLine("{");
        formatter->IncIndent();
        if (parser.Main())
        {
            if (!parser.Rules().empty())
            {
                const std::unique_ptr<soul::ast::spg::RuleParser>& startRule = parser.Rules().front();
                if (startRule->ReturnType() != nullptr)
                {
                    formatter->Write("static ");
                    if (startRule->ReturnType()->IsPtrType())
                    {
                        formatter->Write("std::unique_ptr<");
                        startRule->ReturnType()->WriteNonPtrType(*formatter);
                        formatter->Write(">");
                    }
                    else
                    {
                        startRule->ReturnType()->Write(*formatter);
                    }
                    formatter->Write(" Parse(LexerT& lexer");
                }
                else
                {
                    formatter->Write("static void Parse(LexerT& lexer");
                }
                for (const auto& param : startRule->Params())
                {
                    formatter->Write(", ");
                    param->Type()->Write(*formatter);
                    formatter->Write(" ");
                    formatter->Write(param->Name());
                }
                formatter->WriteLine(");");
            }
        }
        for (const auto& rule : parser.Rules())
        {
            rule->Accept(*this);
        }
        formatter->DecIndent();
        formatter->WriteLine("};");
    }
    else if (stage == CodeGenerationStage::generateImplementation)
    {
        if (parser.Main())
        {
            if (!parser.Rules().empty())
            {
                formatter->WriteLine("template<typename LexerT>");
                const std::unique_ptr<soul::ast::spg::RuleParser>& startRule = parser.Rules().front();
                if (startRule->ReturnType() != nullptr)
                {
                    if (startRule->ReturnType()->IsPtrType())
                    {
                        formatter->Write("std::unique_ptr<");
                        startRule->ReturnType()->WriteNonPtrType(*formatter);
                        formatter->Write(">");
                    }
                    else
                    {
                        startRule->ReturnType()->Write(*formatter);
                    }
                    formatter->Write(" " + parser.Name() + "<LexerT>::Parse(LexerT& lexer");
                }
                else
                {
                    formatter->Write("void " + parser.Name() + "<LexerT>::Parse(LexerT& lexer");
                }
                for (const auto& param : startRule->Params())
                {
                    formatter->Write(", ");
                    param->Type()->Write(*formatter);
                    formatter->Write(" ");
                    formatter->Write(param->Name());
                }
                formatter->WriteLine(")");
                formatter->WriteLine("{");
                formatter->IncIndent();
                soul::ast::spg::RuleParser* rule = startRule.get();
                if (rule->ReturnType() != nullptr)
                {
                    formatter->Write("std::unique_ptr<");
                    if (rule->ReturnType()->IsPtrType())
                    {
                        rule->ReturnType()->WriteNonPtrType(*formatter);
                        formatter->Write(">");
                    }
                    else
                    {
                        formatter->Write("soul::parser::Value<");
                        rule->ReturnType()->Write(*formatter);
                        formatter->Write(">>");
                    }
                    formatter->WriteLine(" value;");
                }
                if (!noDebugSupport)
                {
                    formatter->WriteLine("#ifdef SOUL_PARSER_DEBUG_SUPPORT");
                    formatter->WriteLine("if (lexer.Log())");
                    formatter->WriteLine("{");
                    formatter->IncIndent();
                    formatter->WriteLine("lexer.Log()->WriteBeginRule(\"parse\");");
                    formatter->WriteLine("lexer.Log()->IncIndent();");
                    formatter->DecIndent();
                    formatter->WriteLine("}");
                    formatter->WriteLine("#endif");
                }
                formatter->WriteLine("++lexer;");
                std::string ruleName = rule->Grammar()->Name() + "<LexerT>::" + rule->Name();
                formatter->Write("soul::parser::Match match = " + ruleName + "(lexer");
                for (const auto& param : startRule->Params())
                {
                    formatter->Write(", ");
                    formatter->Write(param->Name());
                }
                formatter->WriteLine(");");
                soul::ast::spg::RuleParser* calledRule = rule;
                if (!calledRule)
                {
                    throw std::runtime_error("rule not set");
                }
                if (calledRule->ReturnType() != nullptr)
                {
                    formatter->Write("value.reset(");
                    if (calledRule->ReturnType()->IsPtrType())
                    {
                        formatter->Write("static_cast<");
                        calledRule->ReturnType()->Write(*formatter);
                        formatter->WriteLine(">(match.value));");
                    }
                    else
                    {
                        formatter->Write("static_cast<soul::parser::Value<");
                        calledRule->ReturnType()->Write(*formatter);
                        formatter->WriteLine(">*>(match.value));");
                    }
                }
                if (!noDebugSupport)
                {
                    formatter->WriteLine("#ifdef SOUL_PARSER_DEBUG_SUPPORT");
                    formatter->WriteLine("if (lexer.Log())");
                    formatter->WriteLine("{");
                    formatter->IncIndent();
                    formatter->WriteLine("lexer.Log()->DecIndent();");
                    formatter->WriteLine("lexer.Log()->WriteEndRule(\"parse\");");
                    formatter->DecIndent();
                    formatter->WriteLine("}");
                    formatter->WriteLine("#endif");
                }
                formatter->WriteLine("if (match.hit)");
                formatter->WriteLine("{");
                formatter->IncIndent();
                formatter->WriteLine("if (*lexer == soul::lexer::END_TOKEN)");
                formatter->WriteLine("{");
                formatter->IncIndent();
                if (startRule->ReturnType())
                {
                    if (startRule->ReturnType()->IsPtrType())
                    {
                        formatter->WriteLine("return value;");
                    }
                    else
                    {
                        formatter->WriteLine("return value->value;");
                    }
                }
                else
                {
                    formatter->WriteLine("return;");
                }
                formatter->DecIndent();
                formatter->WriteLine("}");
                formatter->WriteLine("else");
                formatter->WriteLine("{");
                formatter->IncIndent();
                formatter->WriteLine("lexer.ThrowFarthestError();");
                formatter->DecIndent();
                formatter->WriteLine("}");
                formatter->DecIndent();
                formatter->WriteLine("}");
                formatter->WriteLine("else");
                formatter->WriteLine("{");
                formatter->IncIndent();
                std::string ruleInfo = rule->Info();
                if (ruleInfo.empty())
                {
                    ruleInfo = rule->Name();
                }
                formatter->WriteLine("lexer.ThrowFarthestError();");
                formatter->DecIndent();
                formatter->WriteLine("}");
                if (startRule->ReturnType())
                {
                    if (startRule->ReturnType()->IsPtrType())
                    {
                        formatter->WriteLine("return value;");
                    }
                    else
                    {
                        formatter->WriteLine("return value->value;");
                    }
                }
                else
                {
                    formatter->WriteLine("return;");
                }
                formatter->DecIndent();
                formatter->WriteLine("}");
                formatter->WriteLine();
            }
        }
        for (const auto& rule : parser.Rules())
        {
            rule->Accept(*this);
        }
    }
}

void CodeGeneratorVisitor::Visit(soul::ast::spg::ParserFile& parserFile)
{
    if (parserFile.IsExternal())
    {
        if (verbose)
        {
            std::cout << "> " << parserFile.FilePath() << "\n";
            std::cout << "skipping external parser file '" << parserFile.FilePath() << "'" << "\n";
        }
        return;
    }
    if (verbose)
    {
        std::cout << "> " << parserFile.FilePath() << "\n";
        std::cout << "generating code..." << "\n";
    }
    sn = 0;
    stage = CodeGenerationStage::generateInterface;
    std::string interfaceFileName = parserFile.FilePath() + ".cppm";
    std::ofstream interfaceFile(interfaceFileName);
    util::CodeFormatter interfaceFormatter(interfaceFile);
    formatter = &interfaceFormatter;

    formatter->WriteLine();
    formatter->WriteLine("// this file has been automatically generated from '" + parserFile.FilePath() + "' using soul parser generator ospg version " + version);
    formatter->WriteLine();
    soul::ast::common::ExportModule* mod = parserFile.GetExportModule();
    formatter->WriteLine("export module " + mod->ModuleName() + ";");
    formatter->WriteLine();
    formatter->WriteLine("import std;");
    formatter->WriteLine("import soul.lexer;");
    formatter->WriteLine("import soul.parser;");
    bool hasInterfaceImports = false;
    for (const auto& imprt : parserFile.Imports())
    {
        if (imprt->Prefix() == soul::ast::common::ImportPrefix::interfacePrefix)
        {
            hasInterfaceImports = true;
            formatter->WriteLine("import " + imprt->ModuleName() + ";");
        }
    }
    formatter->WriteLine();
    formatter->WriteLine("export namespace " + soul::ast::common::ToNamespaceName(mod->ModuleName()) + " {");
    formatter->WriteLine();
    bool first = true;
    for (const auto& parser : parserFile.Parsers())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter->WriteLine();
        }
        parser->Accept(*this);
    }
    formatter->WriteLine();
    formatter->WriteLine("} // namespace " + soul::ast::common::ToNamespaceName(mod->ModuleName()));
    if (verbose)
    {
        std::cout << "==> " << interfaceFileName << "\n";
    }

    stage = CodeGenerationStage::generateImplementation;
    std::string implementationFileName = parserFile.FilePath() + ".cpp";
    std::ofstream implementationFile(implementationFileName);
    util::CodeFormatter implementationFormatter(implementationFile);
    formatter = &implementationFormatter;

    formatter->WriteLine();
    formatter->WriteLine("// this file has been automatically generated from '" + parserFile.FilePath() + "' using soul parser generator ospg version " + version);
    formatter->WriteLine();
    formatter->WriteLine("module " + mod->ModuleName() + ";");
    formatter->WriteLine();
    formatter->WriteLine("import util;");
    formatter->WriteLine("import soul.ast.common;");
    formatter->WriteLine("import soul.ast.spg;");
    formatter->WriteLine("import soul.ast.source_pos;");
    formatter->WriteLine("import soul.ast.span;");

    for (const auto& imprt : parserFile.Imports())
    {
        if (imprt->Prefix() == soul::ast::common::ImportPrefix::implementationPrefix)
        {
            formatter->WriteLine("import " + imprt->ModuleName() + ";");
        }
    }
    formatter->WriteLine();
    formatter->WriteLine("namespace " + soul::ast::common::ToNamespaceName(mod->ModuleName()) + " {");
    formatter->WriteLine();
    GenerateArrays(parserFile, *formatter, sn);
    for (const auto& parser : parserFile.Parsers())
    {
        parser->Accept(*this);
    }
    formatter->WriteLine();
    bool hasLexers = false;
    for (const auto& parser : parserFile.Parsers())
    {
        for (const auto& lexer : parser->Lexers())
        {
            if (lexer->TypeSpecifiers().size() > 0)
            {
                soul::ast::cpp::TypeSpecifierNode* firstTypeSpecifier = lexer->TypeSpecifiers()[0].get();
                std::string charTypeName = "char";
                if (firstTypeSpecifier->Kind() == soul::ast::cpp::NodeKind::typeNameNode)
                {
                    soul::ast::cpp::TypeNameNode* lexerType = static_cast<soul::ast::cpp::TypeNameNode*>(firstTypeSpecifier);
                    if (lexerType->IsTemplate() && lexerType->TemplateArgs().size() > 0)
                    {
                        soul::ast::cpp::Node* firstTemplateArg = lexerType->TemplateArgs()[0].get();
                        charTypeName = firstTemplateArg->ToString();
                    }
                }
                hasLexers = true;
                formatter->WriteLine("template struct " + parser->Name() + "<soul::lexer::Lexer<" + lexer->ToString() + ", " + charTypeName + ">>;");
            }
        }
    }
    if (hasLexers)
    {
        formatter->WriteLine();
    }
    formatter->WriteLine("} // namespace " + soul::ast::common::ToNamespaceName(mod->ModuleName()));
    if (verbose)
    {
        std::cout << "==> " << implementationFileName << "\n";
    }
}

} // soul::spg
