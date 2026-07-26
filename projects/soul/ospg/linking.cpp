module soul.spg.linking;

import util;
import soul.lexer.error;

namespace soul::spg {

enum class LinkingStage
{
    makeTokenMap, addParsers, resolveRulesAndTokens
};

class LinkingVisitor : public soul::ast::spg::DefaultVisitor
{
public:
    LinkingVisitor(soul::ast::spg::SpgFile* spgFile_, soul::lexer::FileMap& fileMap_);
    void SetStage(LinkingStage stage_) { stage = stage_; }
    void Visit(soul::ast::spg::TokenParser& parser) override;
    void Visit(soul::ast::spg::NonterminalParser& parser) override;
    void Visit(soul::ast::spg::RuleParser& parser) override;
    void Visit(soul::ast::spg::GrammarParser& parser) override;
    void Visit(soul::ast::spg::ParserFile& parserFile) override;
private:
    soul::ast::spg::SpgFile* spgFile;
    soul::ast::spg::ParserFile* currentParserFile;
    soul::ast::spg::GrammarParser* currentParser;
    soul::ast::spg::RuleParser* currentRule;
    LinkingStage stage;
    soul::lexer::FileMap& fileMap;
};

LinkingVisitor::LinkingVisitor(soul::ast::spg::SpgFile* spgFile_, soul::lexer::FileMap& fileMap_) :
    spgFile(spgFile_), currentParserFile(nullptr), currentParser(nullptr), currentRule(nullptr), stage(LinkingStage::addParsers),
    fileMap(fileMap_)
{
}

void LinkingVisitor::Visit(soul::ast::spg::TokenParser& parser)
{
    if (stage == LinkingStage::resolveRulesAndTokens)
    {
        soul::ast::common::TokenMap* tokenMap = currentParserFile->GetTokenMap();
        if (tokenMap)
        {
            std::vector<soul::ast::common::Token*> tokens = tokenMap->GetTokens(parser.TokenName());
            if (!tokens.empty())
            {
                int n = static_cast<int>(tokens.size());
                if (n > 1)
                {
                    std::string tokenString;
                    for (int i = 0; i < n; ++i)
                    {
                        soul::ast::common::Token* token = tokens[i];
                        if (i > 0)
                        {
                            tokenString.append(" or\n");
                        }
                        tokenString.append(token->FullName());
                    }
                    std::string errorMessage = soul::lexer::MakeMessage("error", "ambiguous reference to token '" + parser.TokenName() + "' in parser file '" +
                        currentParserFile->FilePath() + "':\n" + tokenString, parser.GetSourcePos(), fileMap);
                    throw std::runtime_error(errorMessage);
                }
                else
                {
                    soul::ast::common::Token* token = tokens.front();
                    parser.SetToken(token);
                }
            }
            else
            {
                std::string errorMessage = soul::lexer::MakeMessage("error", "token '" + parser.TokenName() + "' not found from parser file '" + currentParserFile->FilePath() + "'",
                    parser.GetSourcePos(), fileMap);
                throw std::runtime_error(errorMessage);
            }
        }
        else
        {
            throw std::runtime_error("token map not found");
        }
    }
}

void LinkingVisitor::Visit(soul::ast::spg::NonterminalParser& parser)
{
    if (stage == LinkingStage::resolveRulesAndTokens)
    {
        if (!currentRule)
        {
            throw std::runtime_error("current rule not set");
        }
        currentRule->AddNonterminal(&parser);
        if (!currentParser)
        {
            throw std::runtime_error("current parser not set");
        }
        soul::ast::spg::RuleParser* rule = currentParser->GetRule(parser.RuleName());
        if (!rule)
        {
            std::string errorMessage = soul::lexer::MakeMessage("error", "rule '" + parser.RuleName() + "' not found from parser '" + currentParser->Name() + "'",
                parser.GetSourcePos(), fileMap);
            throw std::runtime_error(errorMessage);
        }
        parser.SetRule(rule);
        int argCount = 0;
        if (parser.Arguments())
        {
            argCount = parser.Arguments()->Count();
        }
        if (rule->Params().size() != argCount)
        {
            std::string errorMessage = soul::lexer::MakeMessage("error",
                "Rule '" + rule->Name() + "' takes " + std::to_string(rule->Params().size()) + " parameters (" +
                std::to_string(argCount) + " arguments supplied). Detected in parser '" + currentParser->Name() + "' rule '" + currentRule->Name() + "'.",
                parser.GetSourcePos(), fileMap);
            errorMessage.append("\n:").append(soul::lexer::MakeMessage("info", "see reference", rule->GetSourcePos(), fileMap));
            throw std::runtime_error(errorMessage);
        }
    }
    Visitor::Visit(parser);
}

void LinkingVisitor::Visit(soul::ast::spg::RuleParser& parser)
{
    currentRule = &parser;
    if (stage == LinkingStage::resolveRulesAndTokens)
    {
        std::int64_t ruleId = (std::int64_t(parser.Grammar()->Id()) << 32) | std::int64_t(parser.Index() + 1);
        parser.SetId(ruleId);
        spgFile->AddRule(&parser);
    }
    DefaultVisitor::Visit(parser);
}

void LinkingVisitor::Visit(soul::ast::spg::GrammarParser& parser)
{
    if (parser.Lexers().empty())
    {
        std::string errorMessage = soul::lexer::MakeMessage("error", "Parser '" + parser.Name() + "' has no lexer statements.", parser.GetSourcePos(), fileMap);
        throw std::runtime_error(errorMessage);
    }
    currentParser = &parser;
    if (parser.Id() == -1)
    {
        std::string fullGrammarName = parser.GetParserFile()->GetExportModule()->ModuleName();
        fullGrammarName.append(1, '.').append(parser.Name());
        std::int32_t grammarId = std::hash<std::string>()(fullGrammarName) & 0x7FFFFFFF;
        parser.SetId(grammarId);
    }
    if (stage == LinkingStage::addParsers)
    {
        if (!spgFile->AddParser(&parser))
        {
            std::string errorMessage = soul::lexer::MakeMessage(
                "error", "Parser '" + parser.Name() + "' name not unique. Detected in SPG file '" + spgFile->FilePath() + "'", parser.GetSourcePos(), fileMap);
            throw std::runtime_error(errorMessage);
        }
    }
    else if (stage == LinkingStage::resolveRulesAndTokens)
    {
        for (const auto& usng : parser.Usings())
        {
            soul::ast::common::TokenMap* tokenMap = currentParserFile->GetTokenMap();
            if (tokenMap)
            {
                soul::ast::common::Token* token = tokenMap->GetToken(usng.fullName);
                if (token)
                {
                    tokenMap->AddUsingToken(token);
                }
                else
                {
                    std::vector<std::string> components = util::Split(usng.fullName, '.');
                    if (components.size() != 2)
                    {
                        std::string errorMessage = soul::lexer::MakeMessage("error", "invalid using '" + usng.fullName + "' in parser '" + parser.Name() +
                            "': format 'PARSER.RULE' or 'TOKEN.FULL.NAME' expected",
                            usng.sourcePos, fileMap);
                        throw std::runtime_error(errorMessage);
                    }
                    const std::string& parserName = components[0];
                    const std::string& ruleName = components[1];
                    soul::ast::spg::GrammarParser* targetParser = spgFile->GetParser(parserName);
                    if (!targetParser)
                    {
                        std::string errorMessage = soul::lexer::MakeMessage("error", "parser '" + parserName + "' not found", usng.sourcePos, fileMap);
                        throw std::runtime_error(errorMessage);
                    }
                    soul::ast::spg::RuleParser* rule = targetParser->GetRule(ruleName);
                    if (!rule)
                    {
                        std::string errorMessage = soul::lexer::MakeMessage("error", "rule '" + ruleName + "' not found from parser '" + targetParser->Name() + "'",
                            usng.sourcePos, fileMap);
                        throw std::runtime_error(errorMessage);
                    }
                    if (!parser.MapRule(rule))
                    {
                        std::string errorMessage = soul::lexer::MakeMessage("error", "rule '" + ruleName + "' already added to parser '" + parser.Name() + "'",
                            usng.sourcePos, fileMap);
                        throw std::runtime_error(errorMessage);
                    }
                }
            }
            else
            {
                throw std::runtime_error("token map not found");
            }
        }
    }
    DefaultVisitor::Visit(parser);
}

void LinkingVisitor::Visit(soul::ast::spg::ParserFile& parserFile)
{
    if (!parserFile.GetExportModule())
    {
        throw std::runtime_error("export module declaration missing from parser file '" + parserFile.FilePath() + "'");
    }
    currentParserFile = &parserFile;
    if (stage == LinkingStage::makeTokenMap)
    {
        int n = static_cast<int>(parserFile.Imports().size());
        for (int i = 0; i < n; ++i)
        {
            soul::ast::common::Import* import = parserFile.Imports()[i].get();
            soul::ast::common::TokenCollection* tokenCollection = spgFile->GetTokenCollection(import->ModuleName());
            if (tokenCollection)
            {
                for (const auto& token : tokenCollection->GetTokens())
                {
                    try
                    {
                        soul::ast::common::TokenMap* tokenMap = parserFile.GetTokenMap();
                        if (tokenMap)
                        {
                            tokenMap->AddToken(token.get());
                        }
                        else
                        {
                            throw std::runtime_error("token map not found");
                        }
                    }
                    catch (const std::runtime_error& ex)
                    {
                        throw std::runtime_error(std::string(ex.what()) + ", detected in parser file '" + parserFile.FilePath() + "'");
                    }
                }
            }
        }
    }
    else
    {
        DefaultVisitor::Visit(parserFile);
    }
}

void Link(soul::ast::spg::SpgFile* spgFile, bool verbose, soul::lexer::FileMap& fileMap)
{
    if (verbose)
    {
        std::cout << "linking..." << "\n";
    }
    LinkingVisitor visitor(spgFile, fileMap);
    visitor.SetStage(LinkingStage::makeTokenMap);
    spgFile->Accept(visitor);
    visitor.SetStage(LinkingStage::addParsers);
    spgFile->Accept(visitor);
    visitor.SetStage(LinkingStage::resolveRulesAndTokens);
    spgFile->Accept(visitor);
}

} // namespace soul::spg
