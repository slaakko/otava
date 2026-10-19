module common.parser.rules;

namespace common::parser::rules {

std::mutex ruleMtx;

std::map<std::int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<std::int64_t, std::string> ruleNameMap = {
        { 2231932063773097985, "CommonParser.QualifiedId" },
        { 2231932063773097986, "CommonParser.ExportModule" },
        { 2231932063773097987, "CommonParser.Import" },
        { 2231932063773097988, "CommonParser.ImportPrefix" },
        { 2231932063773097989, "CommonParser.ExprString" },
        { 2231932063773097990, "CommonParser.FilePath" },
        { 2231932063773097991, "CommonParser.ExportKeyword" },
        { 2231932063773097992, "CommonParser.ModuleKeyword" },
        { 2231932063773097993, "CommonParser.ImportKeyword" },
        { 2231932063773097994, "CommonParser.ImplementationPrefix" },
        { 2231932063773097995, "CommonParser.InterfacePrefix" },
        { 2231932063773097996, "CommonParser.ImplementationKeyword" },
        { 2231932063773097997, "CommonParser.InterfaceKeyword" },
        { 2231932063773097998, "CommonParser.ParserKeyword" },
        { 2231932063773097999, "CommonParser.LexerKeyword" },
        { 8614059994532282369, "TokenFileParser.TokenFile" },
        { 8614059994532282370, "TokenFileParser.Token" }
    };
    return &ruleNameMap;
}

} // common::parser::rules
