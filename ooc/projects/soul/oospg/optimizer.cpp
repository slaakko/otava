module soul.spg.optimizer;

import soul.spg.first_set_computer;
import soul.spg.xml_printer;
import soul.spg.linking;
import soul.ast.cpp;

namespace soul::spg {

class OptimizationFlagsSetter : public soul::ast::spg::DefaultVisitor
{
public:
    OptimizationFlagsSetter();
    int Count() const noexcept { return count; }
    void Visit(soul::ast::spg::ChoiceParser& parser) override;
private:
    int count;
};

OptimizationFlagsSetter::OptimizationFlagsSetter() : count(0)
{
}

void OptimizationFlagsSetter::Visit(soul::ast::spg::ChoiceParser& parser)
{
    soul::ast::spg::DefaultVisitor::Visit(parser);
    parser.SetOptimizationFlag(count);
}

class Optimizer : public soul::ast::spg::DefaultVisitor
{
public:
    Optimizer(soul::ast::spg::SwitchParser* switchParser_);
    inline soul::ast::spg::Parser* GetOptimizedParser() noexcept { return optimizedParser; }
    inline soul::ast::spg::SpgFile* GetOptimizedSpgFile() const noexcept { return optimizedSpgFile; }
    void Visit(soul::ast::spg::ChoiceParser& parser) override;
    void Visit(soul::ast::spg::SequenceParser& parser) override;
    void Visit(soul::ast::spg::DifferenceParser& parser) override;
    void Visit(soul::ast::spg::ListParser& parser) override;
    void Visit(soul::ast::spg::LookaheadParser& parser) override;
    void Visit(soul::ast::spg::KleeneParser& parser) override;
    void Visit(soul::ast::spg::PositiveParser& parser) override;
    void Visit(soul::ast::spg::OptionalParser& parser) override;
    void Visit(soul::ast::spg::ExpectationParser& parser) override;
    void Visit(soul::ast::spg::ActionParser& parser) override;
    void Visit(soul::ast::spg::NonterminalParser& parser) override;
    void Visit(soul::ast::spg::EmptyParser& parser) override;
    void Visit(soul::ast::spg::AnyParser& parser) override;
    void Visit(soul::ast::spg::TokenParser& parser) override;
    void Visit(soul::ast::spg::GroupParser& parser) override;
    void Visit(soul::ast::spg::RuleParser& parser) override;
    void Visit(soul::ast::spg::GrammarParser& parser) override;
    void Visit(soul::ast::spg::ParserFile& parserFile) override;
    void Visit(soul::ast::spg::SpgFile& spgFile) override;
private:
    soul::ast::spg::SpgFile* optimizedSpgFile;
    soul::ast::spg::ParserFile* optimizedParserFile;
    soul::ast::spg::GrammarParser* optimizedGrammar;
    soul::ast::spg::RuleParser* optimizedRule;
    soul::ast::spg::Parser* optimizedParser;
    soul::ast::spg::SwitchParser* switchParser;
};

soul::ast::spg::Parser* GenerateOptimizedParser(soul::ast::spg::Parser* from, soul::ast::spg::SwitchParser* switchParser)
{
    Optimizer optimizer(switchParser);
    from->Accept(optimizer);
    return optimizer.GetOptimizedParser();
}

soul::ast::spg::SwitchParser* GenerateSwitch(soul::ast::spg::ChoiceParser& fromChoice)
{
    soul::ast::spg::SwitchParser* switchParser = new soul::ast::spg::SwitchParser(fromChoice.GetSourcePos());
    soul::ast::spg::Parser* left = GenerateOptimizedParser(fromChoice.Left(), switchParser);
    if (left)
    {
        switchParser->AddCaseParser(new soul::ast::spg::CaseParser(fromChoice.GetSourcePos(), left));
    }
    soul::ast::spg::Parser* right = GenerateOptimizedParser(fromChoice.Right(), switchParser);
    if (right)
    {
        switchParser->AddCaseParser(new soul::ast::spg::CaseParser(fromChoice.GetSourcePos(), right));
    }
    return switchParser;
}

Optimizer::Optimizer(soul::ast::spg::SwitchParser* switchParser_) :
    switchParser(switchParser_),
    optimizedSpgFile(nullptr),
    optimizedParserFile(nullptr),
    optimizedGrammar(nullptr),
    optimizedRule(nullptr),
    optimizedParser(nullptr)
{
}

void Optimizer::Visit(soul::ast::spg::ChoiceParser& parser)
{
    if (parser.Optimize())
    {
        if (switchParser)
        {
            soul::ast::spg::Parser* left = GenerateOptimizedParser(parser.Left(), switchParser);
            if (left)
            {
                switchParser->AddCaseParser(new soul::ast::spg::CaseParser(parser.GetSourcePos(), left));
            }
            soul::ast::spg::Parser* right = GenerateOptimizedParser(parser.Right(), switchParser);
            if (right)
            {
                switchParser->AddCaseParser(new soul::ast::spg::CaseParser(parser.GetSourcePos(), right));
            }
            optimizedParser = nullptr;
        }
        else
        {
            optimizedParser = GenerateSwitch(parser);
        }
    }
    else
    {
        soul::ast::spg::Parser* left = GenerateOptimizedParser(parser.Left(), nullptr);
        soul::ast::spg::Parser* right = GenerateOptimizedParser(parser.Right(), nullptr);
        if (left && right)
        {
            optimizedParser = new soul::ast::spg::ChoiceParser(parser.GetSourcePos(), left, right);
        }
        else if (left)
        {
            optimizedParser = left;
        }
        else if (right)
        {
            optimizedParser = right;
        }
        else
        {
            optimizedParser = nullptr;
        }
    }
}

void Optimizer::Visit(soul::ast::spg::SequenceParser& parser)
{
    soul::ast::spg::Parser* left = GenerateOptimizedParser(parser.Left(), nullptr);
    soul::ast::spg::Parser* right = GenerateOptimizedParser(parser.Right(), nullptr);
    if (left && right)
    {
        optimizedParser = new soul::ast::spg::SequenceParser(parser.GetSourcePos(), left, right);
    }
    else if (left)
    {
        optimizedParser = left;
    }
    else if (right)
    {
        optimizedParser = right;
    }
    else
    {
        optimizedParser = nullptr;
    }
}

void Optimizer::Visit(soul::ast::spg::DifferenceParser& parser)
{
    soul::ast::spg::Parser* left = GenerateOptimizedParser(parser.Left(), nullptr);
    soul::ast::spg::Parser* right = GenerateOptimizedParser(parser.Right(), nullptr);
    if (left && right)
    {
        optimizedParser = new soul::ast::spg::DifferenceParser(parser.GetSourcePos(), left, right);
    }
    else if (left)
    {
        optimizedParser = left;
    }
    else if (right)
    {
        optimizedParser = right;
    }
    else
    {
        optimizedParser = nullptr;
    }
}

void Optimizer::Visit(soul::ast::spg::ListParser& parser)
{
    soul::ast::spg::Parser* left = GenerateOptimizedParser(parser.Left(), nullptr);
    soul::ast::spg::Parser* right = GenerateOptimizedParser(parser.Right(), nullptr);
    if (left && right)
    {
        optimizedParser = new soul::ast::spg::ListParser(parser.GetSourcePos(), left, right);
    }
    else if (left)
    {
        optimizedParser = left;
    }
    else if (right)
    {
        optimizedParser = right;
    }
    else
    {
        optimizedParser = nullptr;
    }
}

void Optimizer::Visit(soul::ast::spg::LookaheadParser& parser)
{
    parser.Child()->Accept(*this);
    if (optimizedParser)
    {
        optimizedParser = new soul::ast::spg::LookaheadParser(parser.GetSourcePos(), optimizedParser);
    }
}

void Optimizer::Visit(soul::ast::spg::KleeneParser& parser)
{
    parser.Child()->Accept(*this);
    if (optimizedParser)
    {
        optimizedParser = new soul::ast::spg::KleeneParser(parser.GetSourcePos(), optimizedParser);
    }
}

void Optimizer::Visit(soul::ast::spg::PositiveParser& parser)
{
    parser.Child()->Accept(*this);
    if (optimizedParser)
    {
        optimizedParser = new soul::ast::spg::PositiveParser(parser.GetSourcePos(), optimizedParser);
    }
}

void Optimizer::Visit(soul::ast::spg::OptionalParser& parser)
{
    parser.Child()->Accept(*this);
    if (optimizedParser)
    {
        optimizedParser = new soul::ast::spg::OptionalParser(parser.GetSourcePos(), optimizedParser);
    }
}

void Optimizer::Visit(soul::ast::spg::ExpectationParser& parser)
{
    parser.Child()->Accept(*this);
    if (optimizedParser)
    {
        optimizedParser = new soul::ast::spg::ExpectationParser(parser.GetSourcePos(), optimizedParser);
    }
}

void Optimizer::Visit(soul::ast::spg::ActionParser& parser)
{
    parser.Child()->Accept(*this);
    if (optimizedParser)
    {
        soul::ast::cpp::CompoundStatementNode* clonedSuccessCode = static_cast<soul::ast::cpp::CompoundStatementNode*>(parser.SuccessCode()->Clone());
        soul::ast::cpp::CompoundStatementNode* clonedFailureCode = nullptr;
        if (parser.FailureCode())
        {
            clonedFailureCode = static_cast<soul::ast::cpp::CompoundStatementNode*>(parser.FailureCode()->Clone());
        }
        optimizedParser = new soul::ast::spg::ActionParser(parser.GetSourcePos(), optimizedParser, clonedSuccessCode, clonedFailureCode);
    }
}

void Optimizer::Visit(soul::ast::spg::NonterminalParser& parser)
{
    optimizedParser = parser.Clone();
}

void Optimizer::Visit(soul::ast::spg::EmptyParser& parser)
{
    optimizedParser = parser.Clone();
}

void Optimizer::Visit(soul::ast::spg::AnyParser& parser)
{
    optimizedParser = parser.Clone();
}

void Optimizer::Visit(soul::ast::spg::TokenParser& parser)
{
    optimizedParser = parser.Clone();
}

void Optimizer::Visit(soul::ast::spg::GroupParser& parser)
{
    parser.Child()->Accept(*this);
    if (optimizedParser)
    {
        optimizedParser = new soul::ast::spg::GroupParser(parser.GetSourcePos(), optimizedParser);
    }
}

void Optimizer::Visit(soul::ast::spg::RuleParser& parser)
{
    optimizedRule = new soul::ast::spg::RuleParser(parser.GetSourcePos(), parser.Name());
    for (const auto& param : parser.Params())
    {
        soul::ast::spg::ParamVar* clonedParamVar = static_cast<soul::ast::spg::ParamVar*>(param->Clone());
        optimizedRule->AddParamOrVariable(clonedParamVar);
    }
    for (const auto& var : parser.Vars())
    {
        soul::ast::spg::ParamVar* clonedParamVar = static_cast<soul::ast::spg::ParamVar*>(var->Clone());
        optimizedRule->AddParamOrVariable(clonedParamVar);
    }
    if (parser.ReturnType())
    {
        optimizedRule->SetReturnType(static_cast<soul::ast::cpp::TypeIdNode*>(parser.ReturnType()->Clone()));
    }
    if (parser.HasReturn())
    {
        optimizedRule->SetHasReturn();
    }
    parser.Definition()->Accept(*this);
    optimizedRule->SetDefinition(optimizedParser);
}

void Optimizer::Visit(soul::ast::spg::GrammarParser& parser)
{
    optimizedGrammar = new soul::ast::spg::GrammarParser(parser.GetSourcePos(), parser.Name());
    if (parser.Main())
    {
        optimizedGrammar->SetMain();
    }
    optimizedGrammar->SetId(parser.Id());
    for (const auto& lexer : parser.Lexers())
    {
        optimizedGrammar->AddLexer(static_cast<soul::ast::cpp::TypeIdNode*>(lexer->Clone()));
    }
    for (const auto& usng : parser.Usings())
    {
        optimizedGrammar->AddUsing(usng.sourcePos, usng.fullName);
    }
    for (const auto& rule : parser.Rules())
    {
        rule->Accept(*this);
        optimizedGrammar->AddRule(optimizedRule);
    }
}

void Optimizer::Visit(soul::ast::spg::ParserFile& parserFile)
{
    optimizedParserFile = new soul::ast::spg::ParserFile(parserFile.FilePath());
    if (parserFile.IsExternal())
    {
        optimizedParserFile->SetExternal();
    }
    soul::ast::common::ExportModule* exportModule = new soul::ast::common::ExportModule(parserFile.GetExportModule()->ModuleName());
    optimizedParserFile->SetExportModule(exportModule);
    for (const auto& import : parserFile.Imports())
    {
        soul::ast::common::Import* imp = new soul::ast::common::Import(import->ModuleName(), import->Prefix());
        optimizedParserFile->AddImport(imp);
    }
    for (const auto& parser : parserFile.Parsers())
    {
        parser->Accept(*this);
        optimizedParserFile->AddParser(optimizedGrammar);
    }
}

void Optimizer::Visit(soul::ast::spg::SpgFile& spgFile)
{
    optimizedSpgFile = new soul::ast::spg::SpgFile(spgFile.FilePath(), spgFile.ProjectName());
    for (const auto& parserFile : spgFile.ParserFiles())
    {
        parserFile->Accept(*this);
        optimizedSpgFile->AddParserFile(optimizedParserFile);
    }
}

soul::ast::spg::SpgFile* GenerateOptimizedSpg(soul::ast::spg::SpgFile* spgFile, bool verbose)
{
    if (verbose)
    {
        std::cout << "generating optimized parsers..." << "\n";
    }
    Optimizer optimizer(nullptr);
    spgFile->Accept(optimizer);
    soul::ast::spg::SpgFile* optimizedSpgFile = optimizer.GetOptimizedSpgFile();
    int n = static_cast<int>(spgFile->TokenFiles().size());
    for (int i = 0; i < n; ++i)
    {
        optimizedSpgFile->AddTokenFile(spgFile->TokenFiles()[i]->Clone());
    }
    return optimizedSpgFile;
}

void SetOptimizationFlags(soul::ast::spg::SpgFile* spgFile, bool verbose)
{
    if (verbose)
    {
        std::cout << "setting optimization flags..." << "\n";
    }
    OptimizationFlagsSetter flagSetter;
    spgFile->Accept(flagSetter);
    if (verbose)
    {
        std::cout << flagSetter.Count() << " choice parsers can be optimized" << "\n";
    }
}

std::unique_ptr<soul::ast::spg::SpgFile> Optimize(soul::ast::spg::SpgFile* spgFile, bool verbose, bool xml, soul::lexer::FileMap& fileMap)
{
    if (verbose)
    {
        std::cout << "optimizing..." << "\n";
    }
    ComputeFirstSets(spgFile, verbose);
    if (xml)
    {
        PrintXml(spgFile, verbose, false);
    }
    SetOptimizationFlags(spgFile, verbose);
    soul::ast::spg::SpgFile* optimizedSpg = GenerateOptimizedSpg(spgFile, verbose);
    Link(optimizedSpg, verbose, fileMap);
    ComputeFirstSets(optimizedSpg, verbose);
    return std::unique_ptr<soul::ast::spg::SpgFile>(optimizedSpg);
}

} // namespace soul::spg
