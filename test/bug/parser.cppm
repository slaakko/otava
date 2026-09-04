export module parser;

import std;

class Parser
{
public:
    Parser(const std::string& name_);
    virtual ~Parser();
    virtual void Print();
private:
    std::string name;
};

class RuleParser : public Parser
{
public:
    RuleParser(const std::string& name_);
};

class GrammarParser : public Parser
{
public:
    GrammarParser();
    bool AddRule(RuleParser* rule);
    void Print() override;
private:
    std::vector<std::unique_ptr<RuleParser>> rules;
};
