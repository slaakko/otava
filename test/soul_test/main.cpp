import std;
import soul.ast.source_pos;
import soul.ast.spg;
import soul.ast.cpp;

void foo()
{
    std::unique_ptr<soul::ast::spg::GrammarParser> grammar(new soul::ast::spg::GrammarParser(soul::ast::SourcePos(), "grammar"));
    std::unique_ptr<soul::ast::spg::RuleParser> rule(new soul::ast::spg::RuleParser(soul::ast::SourcePos(), "rule"));
    std::unique_ptr<soul::ast::spg::EmptyParser> empty(new soul::ast::spg::EmptyParser(soul::ast::SourcePos()));
    rule->SetDefinition(empty.release());
    grammar->AddRule(rule.release());
}

int main()
{
    int n = 10000;
    for (int i = 0; i < n; ++i)
    {
        foo();
    }
}
