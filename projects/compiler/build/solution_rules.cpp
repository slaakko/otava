module otava.solution_spg.rules;

namespace otava::solution_spg::rules {

std::mutex ruleMtx;

std::map<std::int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<std::int64_t, std::string> ruleNameMap = {
        { 8482496186954022913, "SolutionParser.SolutionFile" },
        { 8482496186954022914, "SolutionParser.SolutionDeclaration" },
        { 8482496186954022915, "SolutionParser.FilePath" },
        { 8482496186954022916, "SolutionParser.ProjectFilePath" },
        { 8482496186954022917, "SolutionParser.QualifiedId" }
    };
    return &ruleNameMap;
}

} // otava::solution_spg::rules
