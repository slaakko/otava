// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.build.msbuild;

import std;

export namespace otava::build {

void MSBuild(const std::string& projectFilePath, const std::string& config, const std::set<std::string>& configurations);

} // namespace otava::build
