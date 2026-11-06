// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.build.config;

import otava.build_project;
import std;

export namespace otava::build {

std::string GetVCVars64Path();
void ConfigureProject(Project* project, const std::string& configName, bool verbose, std::set<std::string>& configurations);
void DefineSymbol(const std::string& symbol);
std::vector<std::string> GetBuildSymbols();

} // namespace otava::build
