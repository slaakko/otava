// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.build_solution;

namespace otava::build {

Solution::Solution(const std::string& filePath_, const std::string& name_)
{
}

void Solution::AddProjectFilePath(const std::string& projectFilePath)
{
    projectFilePaths.push_back(projectFilePath);
}

void Solution::AddProject(Project* project)
{
    AddProject(project, true);
}

void Solution::AddProject(Project* project, bool own)
{
    if (own)
    {
        ownedProjects.push_back(std::unique_ptr<Project>(project));
    }
    projects.push_back(project);
    projectMap[project->FilePath()] = project;
}

Project* Solution::GetProject(const std::string& projectFilePath) const
{
    auto it = projectMap.find(projectFilePath);
    if (it != projectMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

} // namespace otava::build_solution
