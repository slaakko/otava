// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.build.parser;

import otava.build.project_lexer;
import otava.build.project_parser;
import otava.project_spg.rules;
import otava.build.solution_lexer;
import otava.build.solution_parser;
import otava.solution_spg.rules;
import util.path;
import util.file_stream;
import util.unicode;

namespace otava::build {

std::unique_ptr<Project> ParseProjectFile(const std::string& projectFilePath)
{
    std::string filePath = util::GetFullPath(projectFilePath);
    std::string content = util::ReadFile(filePath);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = otava::build::project_lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), filePath);
    lexer.SetRuleNameMapPtr(otava::project_spg::rules::GetRuleNameMapPtr());
    using LexerType = decltype(lexer);
    return otava::build::project_parser::ProjectParser<LexerType>::Parse(lexer);
}

std::unique_ptr<Solution> ParseSolutionFile(const std::string& solutionFilePath)
{
    std::string filePath = util::GetFullPath(solutionFilePath);
    std::string content = util::ReadFile(filePath);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = otava::build::solution_lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), filePath);
    lexer.SetRuleNameMapPtr(otava::solution_spg::rules::GetRuleNameMapPtr());
    using LexerType = decltype(lexer);
    return otava::build::solution_parser::SolutionParser<LexerType>::Parse(lexer);
}

} // namespace otava::build
