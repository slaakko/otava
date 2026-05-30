// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.build.config;

import otava.build_project;
import otava.symbols.exception;
import soul.xml.dom;
import soul.xml.dom_parser;
import soul.xml.xpath.evaluate;
import soul.xml.xpath.object;
import util.unicode;
import util.path;

namespace otava::build {

std::string GetVCVars64Path()
{
    std::string configFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::OtavaRoot(), "config"), "otava.config"));
    util::CreateDirectories(util::Path::GetDirectoryName(configFilePath));
    if (!util::FileExists(configFilePath))
    {
        soul::xml::Document configDoc;
        soul::xml::Element* configurationElement = soul::xml::MakeElement("configuration");
        configDoc.AppendChild(configurationElement);
        soul::xml::Element* vcvars64Element = soul::xml::MakeElement("vcvars64");
        configurationElement->AppendChild(vcvars64Element);
        vcvars64Element->SetAttribute("path", "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat)");
        std::ofstream configFile(configFilePath);
        util::CodeFormatter formatter(configFile);
        formatter.SetIndentSize(1);
        configDoc.Write(formatter);
    }
    std::unique_ptr<soul::xml::Document> configDoc = soul::xml::ParseXmlFile(configFilePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/configuration/vcvars64", configDoc.get());
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string path = element->GetAttribute("path");
            if (path.empty())
            {
                otava::symbols::SetExceptionThrown();
                throw std::runtime_error("/configuration/vcvars64 element has no 'path' attribute");
            }
            return path;
        }
    }
    return std::string();
}

void ConfigureProject(Project* project, const std::string& configFilePath, soul::xml::Document* configDoc, const std::string& configName,
    bool verbose, std::set<std::string>& configurations)
{
    configurations.insert(configName);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/configurations/configuration", configDoc);
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            if (element->GetAttribute("name") == configName)
            {
                if (verbose)
                {
                    std::cout << "> using configuration '" + configName + "' from '" + configFilePath + "'\n";
                }
                std::unique_ptr<soul::xml::xpath::NodeSet> defineNodeSet = soul::xml::xpath::EvaluateToNodeSet("define", element);
                int n = defineNodeSet->Count();
                for (int i = 0; i < n; ++i)
                {
                    soul::xml::Node* node = defineNodeSet->GetNode(i);
                    if (node->IsElementNode())
                    {
                        soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                        std::string symbol = element->GetAttribute("symbol");
                        std::int64_t value = 1;
                        if (!symbol.empty())
                        {
                            std::string defineStringValue = element->GetAttribute("value");
                            if (!defineStringValue.empty())
                            {
                                try
                                {
                                    value = std::stoll(defineStringValue);
                                }
                                catch (const std::exception& ex)
                                {
                                    std::cout << "error parsing int64 value from 'value' attribute of 'define' element in configuration '" + configName +
                                        "' of configuration file '" + configFilePath + "': " + ex.what() + ", using default value 1" << "\n";
                                }
                            }
                        }
                        else
                        {
                            otava::symbols::SetExceptionThrown();
                            throw std::runtime_error("'symbol' attribute of 'define' element in configuration '" + configName +
                                "' of configuration file '" + configFilePath + "' is empty or does not exist");
                        }
                        if (!project->HasDefine(symbol))
                        {
                            project->AddDefine(symbol, value);
                        }
                    }
                }
                std::string base = element->GetAttribute("base");
                if (!base.empty())
                {
                    ConfigureProject(project, configFilePath, configDoc, base, verbose, configurations);
                }
            }
        }
    }
}

void ConfigureProject(Project* project, const std::string& configName, bool verbose, std::set<std::string>& configurations)
{
    std::string projectFilePath = project->FilePath();
    std::string projectDir = util::Path::GetDirectoryName(projectFilePath);
    std::string projectConfigFilePath = util::GetFullPath(util::Path::Combine(projectDir, "config.xml"));
    if (util::FileExists(projectConfigFilePath))
    {
        std::unique_ptr<soul::xml::Document> configDoc = soul::xml::ParseXmlFile(projectConfigFilePath);
        ConfigureProject(project, projectConfigFilePath, configDoc.get(), configName, verbose, configurations);
    }
    std::string globalConfigFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::OtavaRoot(), "config"), "config.xml"));
    if (util::FileExists(globalConfigFilePath))
    {
        std::unique_ptr<soul::xml::Document> configDoc = soul::xml::ParseXmlFile(globalConfigFilePath);
        ConfigureProject(project, globalConfigFilePath, configDoc.get(), configName, verbose, configurations);
    }
    else
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("global Otava configuration file '" + globalConfigFilePath + "' does not exist");
    }
}

std::set<std::string> symbols;

void DefineSymbol(const std::string& symbol)
{
    symbols.insert(symbol);
}

std::vector<std::string> GetBuildSymbols()
{
    std::vector<std::string> buildSymbols;
    for (const auto& symbol : symbols)
    {
        buildSymbols.push_back(symbol);
    }
    return buildSymbols;
}

} // namespace otava::build
