// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.build.gen_main_unit;

import std;
import otava.symbols.modules;
import otava.symbols.context;
import otava.build_project;

export namespace otava::build {

std::string GenerateMainUnit(otava::symbols::ModuleMapper& moduleMapper, const std::string& mainFilePath, const std::string& mainFunctionIrName, int numParams,
    const std::vector<std::string>& compileUnitInitFnNames, const std::string& config, int optLevel, Project* project, const std::set<std::string>& configurations,
    otava::symbols::Context* context);

} // namespace otava::build
