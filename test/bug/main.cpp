import std;
import parser;

int main()
{
    RuleParser* rule = new RuleParser("rule");
    GrammarParser* grammar = new GrammarParser();
    grammar->AddRule(rule);
    grammar->Print();
}
