// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.build.project_file;

import std;

export namespace otava::build {

enum class ProjectTarget
{
    program, library
};

class Project;

void MakeProjectFile(Project* project, const std::string& projectFilePath, const std::vector<std::string>& asmFiles, const std::vector<std::string>& cppFiles,
    const std::vector<std::string>& resourceFiles,
    const std::string& libraryDirs, const std::vector<std::unique_ptr<Project>>& referencedProjects, const std::string& config, int optLevel,
    const std::string& classIndexFilePath, const std::string& traceBinFilePath, ProjectTarget target, bool verbose, const std::set<std::string>& configurations);

} // namespace otava::build
