module soul.slg.token.generator;

import soul.ast.common;
import util;

namespace soul::slg {

void GenerateTokenModule(soul::ast::common::TokenFile* tokenFile, bool verbose)
{
    if (tokenFile->IsExternal())
    {
        if (verbose)
        {
            std::cout << "skipping external token file '" + tokenFile->FilePath() + "'" << "\n";
        }
        return;
    }
    std::string interfaceFilePath;
    interfaceFilePath = tokenFile->FilePath() + ".cppm";
    std::ofstream interfaceFile(interfaceFilePath);
    if (!interfaceFile)
    {
        throw std::runtime_error("could not create file '" + interfaceFilePath + "'");
    }
    util::CodeFormatter formatter(interfaceFile);
    soul::ast::common::TokenCollection* tokenCollection = tokenFile->GetTokenCollection();
    formatter.WriteLine();
    formatter.WriteLine("// this file has been automatically generated from '" + tokenFile->FilePath() + "' using soul lexer generator version " + util::OtavaVersionStr());
    formatter.WriteLine();
    std::string includeGuard;
    formatter.WriteLine("export module " + tokenCollection->Name() + ";");
    formatter.WriteLine();
    formatter.WriteLine("import std;");
    formatter.WriteLine();
    std::string tokenNamespaceName = soul::ast::common::ToNamespaceName(tokenCollection->Name());
    formatter.WriteLine("export namespace " + tokenNamespaceName + " {");
    formatter.WriteLine();
    formatter.WriteLine("constexpr std::int32_t tokenSetID = " + std::to_string(tokenCollection->Id()) + ";");
    formatter.WriteLine();
    int n = tokenCollection->GetTokens().size();
    for (int i = 0; i < n; ++i)
    {
        soul::ast::common::Token* token = tokenCollection->GetTokens()[i].get();
        formatter.WriteLine("constexpr std::int64_t " + token->Name() + " = (static_cast<std::int64_t>(tokenSetID) << 32) | " + std::to_string(i + 1) + ";");
    }
    formatter.WriteLine();
    formatter.WriteLine("} // " + tokenNamespaceName);
    if (verbose)
    {
        std::cout << "==> " << interfaceFilePath << "\n";
    }
}

} // namespace soul::slg
