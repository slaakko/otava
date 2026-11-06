module otava.project_spg.rules;

namespace otava::project_spg::rules {

std::mutex ruleMtx;

std::map<std::int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<std::int64_t, std::string> ruleNameMap = {
        { 2957164066528821249, "ProjectParser.ProjectFile" },
        { 2957164066528821250, "ProjectParser.ProjectDeclaration" },
        { 2957164066528821251, "ProjectParser.ParseDefine" },
        { 2957164066528821252, "ProjectParser.TargetDeclaration" },
        { 2957164066528821253, "ProjectParser.ParseTarget" },
        { 2957164066528821254, "ProjectParser.FilePath" },
        { 2957164066528821255, "ProjectParser.InterfaceFilePath" },
        { 2957164066528821256, "ProjectParser.SourceFilePath" },
        { 2957164066528821257, "ProjectParser.ResourceFilePath" },
        { 2957164066528821258, "ProjectParser.ReferenceFilePath" },
        { 2957164066528821259, "ProjectParser.QualifiedId" }
    };
    return &ruleNameMap;
}

} // otava::project_spg::rules
