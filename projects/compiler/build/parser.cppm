// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.build.parser;

import otava.build_project;
import otava.build_solution;
import std;

export namespace otava::build {

std::unique_ptr<Project> ParseProjectFile(const std::string& projectFilePath);
std::unique_ptr<Solution> ParseSolutionFile(const std::string& solutionFilePath);

} // namespace otava::build
