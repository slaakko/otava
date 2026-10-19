module soul.spg.first_set_computer;

import std;

namespace soul::spg {

class FirstSetComputer : public soul::ast::spg::DefaultVisitor
{
public:
    FirstSetComputer();
    bool Changed() const noexcept { return changed; }
    void Visit(soul::ast::spg::ChoiceParser& parser) override;
    void Visit(soul::ast::spg::CaseParser& parser) override;
    void Visit(soul::ast::spg::SwitchParser& parser) override;
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
    void Visit(soul::ast::spg::CharParser& parser) override;
    void Visit(soul::ast::spg::StringParser& parser) override;
    void Visit(soul::ast::spg::CharSetParser& parser) override;
    void Visit(soul::ast::spg::GroupParser& parser) override;
    void Visit(soul::ast::spg::RuleParser& parser) override;
    void Visit(soul::ast::spg::GrammarParser& parser) override;
private:
    bool changed;
    std::set<soul::ast::spg::Parser*> visited;
};

FirstSetComputer::FirstSetComputer() : changed(false)
{
}

void FirstSetComputer::Visit(soul::ast::spg::ChoiceParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::CaseParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::SwitchParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::SequenceParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::DifferenceParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::ListParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::LookaheadParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::KleeneParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::PositiveParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::OptionalParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::ExpectationParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::ActionParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::NonterminalParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::EmptyParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::AnyParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::TokenParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::CharParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::StringParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::CharSetParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::GroupParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::RuleParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void FirstSetComputer::Visit(soul::ast::spg::GrammarParser& parser)
{
    parser.ComputeFirst(changed, visited);
}

void ComputeFirstSets(soul::ast::spg::SpgFile* spgFile, bool verbose)
{
    if (verbose)
    {
        std::cout << "computing first sets..." << "\n";
    }
    int round = 1;
    bool changed = true;
    while (changed)
    {
        if (verbose)
        {
            std::cout << "round " << round << "..." << "\n";
        }
        FirstSetComputer firstSetComputer;
        spgFile->Accept(firstSetComputer);
        changed = firstSetComputer.Changed();
        ++round;
    }
}

} // namespace soul::spg

