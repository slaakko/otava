export module ctx;

import std;

class Ctx
{
public:
    void SetCurrentConfigurations(const std::set<std::string>& configurations);
private:
    std::set<std::string> currentConfigurations;
};
