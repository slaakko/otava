module re.rules;

namespace re::rules {

std::mutex ruleMtx;

std::map<std::int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<std::int64_t, std::string> ruleNameMap = {
        { 7216077787843526657, "RegExParser.RegularExpression" },
        { 7216077787843526658, "RegExParser.Alternative" },
        { 7216077787843526659, "RegExParser.Catenation" },
        { 7216077787843526660, "RegExParser.Repetition" },
        { 7216077787843526661, "RegExParser.Primary" },
        { 7216077787843526662, "RegExParser.Class" },
        { 7216077787843526663, "RegExParser.Range" },
        { 7216077787843526664, "RegExParser.Char" },
        { 7216077787843526665, "RegExParser.ExpressionReference" }
    };
    return &ruleNameMap;
}

} // re::rules
