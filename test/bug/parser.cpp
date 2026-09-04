module parser;

Parser::Parser(const std::string& name_) : name(name_)
{
}

Parser::~Parser()
{
}

void Parser::Print()
{
    std::cout << name << "\n";
}

RuleParser::RuleParser(const std::string& name_) : Parser(name_)
{
}

GrammarParser::GrammarParser() : Parser("grammar")
{
}

bool GrammarParser::AddRule(RuleParser* rule)
{
    rules.push_back(std::unique_ptr<RuleParser>(rule));
    return true;
}

void GrammarParser::Print()
{
    Parser::Print();
    for (const auto& rule : rules)
    {
        rule->Print();
    }
}
