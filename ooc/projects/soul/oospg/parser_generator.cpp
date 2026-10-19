module soul.spg.parser_generator;

import util;
import soul.spg.file_parsers;
import soul.spg.linking;
import soul.spg.optimizer;
import soul.spg.code_generator;
import soul.spg.xml_printer;
import soul.ast.common;

namespace soul::spg {

void GenerateRuleNameModule(soul::ast::spg::SpgFile* spgFile, bool verbose)
{
    if (verbose)
    {
        std::cout << "generating rule name module..." << "\n";
    }
    std::string spgFilePath = spgFile->FilePath();
    std::string root = util::Path::GetDirectoryName(spgFilePath);
    std::string interfaceFilePath = util::Path::Combine(root, util::Path::GetFileNameWithoutExtension(spgFilePath) + "_rules.cppm");
    std::ofstream interfaceFile(interfaceFilePath);
    util::CodeFormatter interfaceFormatter(interfaceFile);
    std::string moduleName = spgFile->ProjectName() + ".rules";
    interfaceFormatter.WriteLine("export module " + moduleName + ";");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("import std;");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("export namespace " + soul::ast::common::ToNamespaceName(moduleName) + " {");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("std::map<std::int64_t, std::string>* GetRuleNameMapPtr();");
    interfaceFormatter.WriteLine();
    interfaceFormatter.WriteLine("} // " + soul::ast::common::ToNamespaceName(moduleName));
    if (verbose)
    {
        std::cout << "==> " << interfaceFilePath << "\n";
    }

    std::string implementationFilePath = util::Path::Combine(root, util::Path::GetFileNameWithoutExtension(spgFilePath) + "_rules.cpp");
    std::ofstream implementationFile(implementationFilePath);
    util::CodeFormatter implementationFormatter(implementationFile);
    implementationFormatter.WriteLine("module " + moduleName + ";");
    implementationFormatter.WriteLine();
    implementationFormatter.WriteLine("namespace " + soul::ast::common::ToNamespaceName(moduleName) + " {");
    implementationFormatter.WriteLine();
    implementationFormatter.WriteLine("std::mutex ruleMtx;");
    implementationFormatter.WriteLine();
    implementationFormatter.WriteLine("std::map<std::int64_t, std::string>* GetRuleNameMapPtr()");
    implementationFormatter.WriteLine("{");
    implementationFormatter.IncIndent();
    implementationFormatter.WriteLine("std::lock_guard<std::mutex> lock(ruleMtx);");
    implementationFormatter.WriteLine("static std::map<std::int64_t, std::string> ruleNameMap = {");
    implementationFormatter.IncIndent();
    int n = spgFile->Rules().size();
    for (int i = 0; i < n; ++i)
    {
        soul::ast::spg::RuleParser* rule = spgFile->Rules()[i];
        std::string ruleName = rule->Grammar()->Name() + "." + rule->Name();
        implementationFormatter.Write("{ " + std::to_string(rule->Id()) + ", \"" + ruleName + "\" }");
        if (i != n - 1)
        {
            implementationFormatter.Write(",");
        }
        implementationFormatter.WriteLine();
    }
    implementationFormatter.DecIndent();
    implementationFormatter.WriteLine("};");
    implementationFormatter.WriteLine("return &ruleNameMap;");
    implementationFormatter.DecIndent();
    implementationFormatter.WriteLine("}");
    implementationFormatter.WriteLine();
    implementationFormatter.WriteLine("} // " + soul::ast::common::ToNamespaceName(moduleName));
    if (verbose)
    {
        std::cout << "==> " << implementationFilePath << "\n";
    }
}

void GenerateParsers(soul::ast::spg::SpgFile* spgFile, soul::lexer::FileMap& fileMap, bool verbose, bool noDebugSupport, bool optimize, bool xml, 
    const std::string& version)
{
    std::cout << "generating parsers for project '" << spgFile->ProjectName() << "'..." << "\n";
    std::string root = util::Path::GetDirectoryName(spgFile->FilePath());
    for (const auto& declaration : spgFile->Declarations())
    {
        switch (declaration->GetFileKind())
        {
            case soul::ast::common::FileKind::parserFile:
            {
                soul::ast::spg::ParserFileDeclaration* parserFileDeclaration = static_cast<soul::ast::spg::ParserFileDeclaration*>(declaration.get());
                std::string parserFilePath = util::GetFullPath(util::Path::Combine(root, parserFileDeclaration->FilePath()));
                std::unique_ptr<soul::ast::spg::ParserFile> parserFile = soul::spg::ParseParserFile(parserFilePath, parserFileDeclaration->GetSourcePos(),
                    fileMap, verbose, parserFileDeclaration->External());
                spgFile->AddParserFile(parserFile.release());
                break;
            }
            case soul::ast::common::FileKind::tokenFile:
            {
                soul::ast::spg::TokenFileDeclaration* tokenFileDeclaration = static_cast<soul::ast::spg::TokenFileDeclaration*>(declaration.get());
                std::string tokenFilePath = util::GetFullPath(util::Path::Combine(root, tokenFileDeclaration->FilePath()));
                std::unique_ptr<soul::ast::common::TokenFile> tokenFile = soul::spg::ParseTokenFile(tokenFilePath, tokenFileDeclaration->External());
                spgFile->AddTokenFile(tokenFile.release());
                break;
            }
        }
    }
    Link(spgFile, verbose, fileMap);
    std::unique_ptr<soul::ast::spg::SpgFile> optimizedSpg;
    if (optimize)
    {
        optimizedSpg = Optimize(spgFile, verbose, xml, fileMap);
        spgFile = optimizedSpg.get();
    }
    if (xml)
    {
        PrintXml(spgFile, verbose, optimize);
    }
    GenerateCode(spgFile, verbose, noDebugSupport, version, fileMap);
    GenerateRuleNameModule(spgFile, verbose);
    std::cout << "parsers for project '" << spgFile->ProjectName() << "' generated successfully." << "\n";
    optimizedSpg.reset();
}

} // namespace soul::spg

