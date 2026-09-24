module otava.pp.parser.spg.rules;

namespace otava::pp::parser::spg::rules {

std::mutex ruleMtx;

std::map<std::int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<std::int64_t, std::string> ruleNameMap = {
        { 1180017559924113409, "PPParser.PPLIne" },
        { 1180017559924113410, "PPParser.PPCommand" },
        { 1180017559924113411, "PPParser.IfCommand" },
        { 1180017559924113412, "PPParser.ElifCommand" },
        { 1180017559924113413, "PPParser.IfdefCommand" },
        { 1180017559924113414, "PPParser.IfndefCommand" },
        { 1180017559924113415, "PPParser.ElseCommand" },
        { 1180017559924113416, "PPParser.EndifCommand" },
        { 1180017559924113417, "PPParser.DefineCommand" },
        { 1180017559924113418, "PPParser.UndefCommand" },
        { 1180017559924113419, "PPParser.IncludeCommand" },
        { 1180017559924113420, "PPParser.PragmaCommand" },
        { 1180017559924113421, "PPParser.Header" },
        { 1180017559924113422, "PPParser.Expression" },
        { 1180017559924113423, "PPParser.LogicalOrExpr" },
        { 1180017559924113424, "PPParser.LogicalAndExpr" },
        { 1180017559924113425, "PPParser.InclusiveOrExpression" },
        { 1180017559924113426, "PPParser.ExclusiveOrExpression" },
        { 1180017559924113427, "PPParser.AndExpression" },
        { 1180017559924113428, "PPParser.EqualityExpression" },
        { 1180017559924113429, "PPParser.RelationalExpression" },
        { 1180017559924113430, "PPParser.ShiftExpression" },
        { 1180017559924113431, "PPParser.AdditiveExpression" },
        { 1180017559924113432, "PPParser.MultiplicativeExpression" },
        { 1180017559924113433, "PPParser.UnaryExpression" },
        { 1180017559924113434, "PPParser.PrimaryExpr" },
        { 1180017559924113435, "PPParser.DefinedExpr" }
    };
    return &ruleNameMap;
}

} // otava::pp::parser::spg::rules
