// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.build_project;

import util.path;
import otava.symbols.exception;
import util.buffered_stream;
import util.file_stream;

namespace otava::build {

std::int16_t MakeProjectId(const std::string& projectName) noexcept
{
    return static_cast<std::int16_t>(std::hash<std::string>()(projectName) & 0x7FFF);
}

std::int32_t MakeFileId(std::int16_t projectId, std::int16_t fileIndex) noexcept
{
    return static_cast<std::int32_t>(projectId) << 16 | static_cast<std::int32_t>(fileIndex);
}

Define::Define(const std::string& symbol_, std::int64_t value_) : symbol(symbol_), value(value_)
{
}

bool ProjectLess::operator()(Project* left, Project* right) const noexcept
{
    if (left->Id() < right->Id()) return true;
    if (left->Id() > right->Id()) return false;
    return left->Name() < right->Name();
}

Project::Project(const std::string& filePath_, const std::string& name_) :
    fileMap(nullptr), filePath(filePath_), name(name_), initialized(false), scanned(false), loaded(false), target(Target::program)
{
    root = util::Path::GetDirectoryName(filePath);
}

std::int16_t Project::Id() const noexcept
{
    return MakeProjectId(name);
}

void Project::AddRoots(otava::symbols::ModuleMapper& moduleMapper)
{
    for (const auto& referenceFilePath : referenceFilePaths)
    {
        std::string referenceRoot = util::GetFullPath(util::Path::Combine(root, util::Path::GetDirectoryName(referenceFilePath)));
        moduleMapper.AddRoot(referenceRoot);
    }
}

void Project::AddDefine(const std::string& symbol, std::int64_t value)
{
    defines.push_back(Define(symbol, value));
}

bool Project::HasDefine(const std::string& symbol) const noexcept
{
    for (const auto& define : defines)
    {
        if (define.symbol == symbol) return true;
    }
    return false;
}

void Project::InitModules()
{
    if (initialized) return;
    initialized = true;
    MapFiles();
}

otava::symbols::Module* Project::GetModule(const std::string& moduleName) const
{
    auto it = moduleMap.find(moduleName);
    if (it != moduleMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

otava::symbols::Module* Project::GetModule(int file) const
{
    auto it = fileIdModuleMap.find(file);
    if (it != fileIdModuleMap.end())
    {
        return it->second;
    }
    else
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("invalid file id");
    }
}

void Project::SetModule(std::int32_t fileId, std::unique_ptr<otava::symbols::Module>&& m)
{
    m->SetIndex(otava::symbols::Index(modules.size()));
    otava::symbols::Module* module = m.get();
    modules.push_back(std::move(m));
    fileIdModuleMap[fileId] = module;
    if (module->Kind() == otava::symbols::ModuleKind::interfaceModule)
    {
        std::string moduleName = module->Name();
        if (std::find(moduleNames.begin(), moduleNames.end(), moduleName) == moduleNames.end())
        {
            moduleNames.push_back(moduleName);
        }
    }
}

std::unique_ptr<otava::symbols::Module> Project::ReleaseModule(int file)
{
    otava::symbols::Module* m = GetModule(file);
    return std::unique_ptr<otava::symbols::Module>(modules[otava::symbols::ToUnderlying(m->GetIndex())].release());
}

void Project::AddInterfaceFilePath(const std::string& interfaceFilePath)
{
    interfaceFilePaths.push_back(interfaceFilePath);
}

void Project::AddSourceFilePath(const std::string& sourceFilePath)
{
    sourceFilePaths.push_back(sourceFilePath);
}

void Project::AddResourceFilePath(const std::string& resourceFilePath)
{
    resourceFilePaths.push_back(resourceFilePath);
}

void Project::AddReferenceFilePath(const std::string& referenceFilePath)
{
    referenceFilePaths.push_back(referenceFilePath);
}

void Project::AddReferencedProject(Project* referencedProject)
{
    referencedProjects.push_back(std::unique_ptr<Project>(referencedProject));
}

const std::string& Project::GetModuleSourceFilePath(std::int32_t fileId) const
{
    return fileMap->GetFilePath(fileId);
}

void Project::ResolveForwardDeclarationsAndAddDerivedClasses(otava::symbols::ModuleMapper& moduleMapper, const std::string& config, int optLevel,
    const std::set<std::string>& configurations, otava::symbols::Context* context)
{
    otava::symbols::Module projectModule(Name() + ".#project");
    for (const auto& moduleName : moduleNames)
    {
        otava::symbols::Module* module = moduleMapper.GetModule(moduleName, config, optLevel, configurations, context);
        //projectModule.Import(module, moduleMapper, config, optLevel, configurations, context); TODO
    }
    //projectModule.ResolveForwardDeclarations();
    //projectModule.AddDerivedClasses();
}

void Project::ReadTraceInfo(const std::string& moduleDir)
{
    std::string traceInfoFilePath = util::GetFullPath(util::Path::Combine(moduleDir, "trace.info"));
    util::FileStream file(traceInfoFilePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream bufStream(file);
    util::BinaryStreamReader reader(bufStream);
    traceInfo.Read(reader);
}

void Project::WriteTraceInfo(const std::string& moduleDir)
{
    std::string traceInfoFilePath = util::GetFullPath(util::Path::Combine(moduleDir, "trace.info"));
    util::FileStream file(traceInfoFilePath, util::OpenMode::write | util::OpenMode::binary);
    util::BufferedStream bufStream(file);
    util::BinaryStreamWriter writer(bufStream);
    traceInfo.Write(writer);
}

void Project::ReadClassIndex(const std::string& moduleDir)
{
    std::string classIndexFilePath = util::GetFullPath(util::Path::Combine(moduleDir, "class.index"));
    util::FileStream file(classIndexFilePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream bufStream(file);
    util::BinaryStreamReader reader(bufStream);
    index.read(reader); 
}

void Project::WriteClassIndex(const std::string& moduleDir)
{
    std::string classIndexFilePath = util::GetFullPath(util::Path::Combine(moduleDir, "class.index"));
    util::FileStream file(classIndexFilePath, util::OpenMode::write | util::OpenMode::binary);
    util::BufferedStream bufStream(file);
    util::BinaryStreamWriter writer(bufStream);
    index.write(writer); 
}

void Project::LoadModules(otava::symbols::ModuleMapper& moduleMapper, const std::string& config, int optLevel, const std::set<std::string>& configurations,
    otava::symbols::Context* context)
{
    if (loaded) return;
    loaded = true;
#ifdef DEBUG_SYMBOL_IO
    std::cout << ">project '" << Name() << "' loading modules" << "\n";
#endif
    moduleMapper.SetModuleCount(otava::symbols::Cardinality(modules.size()));
    for (const auto& m : modules)
    {
        if (m)
        {
            moduleMap[m->Name()] = m.get();
            m->SetId(moduleMapper.GetNextModuleId());
            moduleMapper.MapModule(m.get());
            if (std::find(moduleNames.begin(), moduleNames.end(), m->Name()) == moduleNames.end())
            {
                moduleNames.push_back(m->Name());
            }
        }
    }
    for (const auto& m : modules)
    {
        if (!m) continue;
#ifdef DEBUG_SYMBOL_IO
        std::cout << ">" << m->Name() << "\n";
#endif
        otava::symbols::Cardinality exportedModuleNameCount = m->ExportedModuleNameCount();
        for (otava::symbols::Index i = otava::symbols::Index(0); i < otava::symbols::Index(exportedModuleNameCount); ++i)
        {
            std::string exportedModuleName = m->GetExportedModuleName(i);
            otava::symbols::Module* exportedModule = GetModule(exportedModuleName);
            if (exportedModule)
            {
                m->AddDependsOnModule(exportedModule);
            }
            else
            {
                otava::symbols::SetExceptionThrown();
                throw std::runtime_error("exported module '" + exportedModuleName + "' not found");
            }
        }
        otava::symbols::Cardinality importedModuleNameCount = m->ImportedModuleNameCount();
        for (otava::symbols::Index i = otava::symbols::Index(0); i < otava::symbols::Index(importedModuleNameCount); ++i)
        {
            std::string importedModuleName = m->GetImportedModuleName(i);
            otava::symbols::Module* importedModule = GetModule(importedModuleName);
            if (importedModule)
            {
                m->AddDependsOnModule(importedModule);
            }
        }
#ifdef DEBUG_SYMBOL_IO
        std::cout << "<" << n->Name() << "\n";
#endif
    }
#ifdef DEBUG_SYMBOL_IO
    std::cout << "<project '" << Name() << "' modules loaded" << "\n";
#endif
}

void Project::MapFiles()
{
    root = util::Path::GetDirectoryName(filePath);
    std::int16_t fileIndex = 0;
    for (const auto& interfaceFileName : interfaceFilePaths)
    {
        std::string interfaceFilePath = util::GetFullPath(util::Path::Combine(root, interfaceFileName));
        std::int32_t interfaceFileId = MakeFileId(Id(), fileIndex++);
        fileMap->MapFile(interfaceFilePath, interfaceFileId);
        interfaceFiles.push_back(interfaceFileId);
    }
    for (const auto& sourceFileName : sourceFilePaths)
    {
        std::string sourceFilePath = util::GetFullPath(util::Path::Combine(root, sourceFileName));
        std::int32_t sourceFileId = MakeFileId(Id(), fileIndex++);
        fileMap->MapFile(sourceFilePath, sourceFileId);
        sourceFiles.push_back(sourceFileId);
    }
}

bool Project::UpToDate(const std::string& config, int optLevel, const std::set<std::string>& configurations) const
{
    if (!util::FileExists(outputFilePath))
    {
        return false;
    }
    if (util::LastWriteTime(outputFilePath) < util::LastWriteTime(filePath))
    {
        return false;
    }
    for (const auto& referencedProject : referencedProjects)
    {
        Project* reference = referencedProject.get();
        if (!util::FileExists(reference->OutputFilePath()) ||
            util::LastWriteTime(outputFilePath) < util::LastWriteTime(reference->OutputFilePath()))
        {
            return false;
        }
    }
    for (const auto& m : modules)
    {
        if (m)
        {
            const std::string& moduleSourceFilePath = m->FilePath();
            if (m->Kind() == otava::symbols::ModuleKind::interfaceModule)
            {
                std::string moduleFilePath = otava::symbols::MakeModuleFilePath(root, config, optLevel, m->Name(), configurations);
                if (!util::FileExists(moduleFilePath) || util::LastWriteTime(moduleFilePath) < util::LastWriteTime(moduleSourceFilePath))
                {
                    return false;
                }
                if (util::LastWriteTime(outputFilePath) < util::LastWriteTime(moduleSourceFilePath))
                {
                    return false;
                }
            }
        }
    }
    for (const auto& sourceFileName : sourceFilePaths)
    {
        std::string sourceFilePath = util::Path::Combine(util::Path::GetDirectoryName(FilePath()), sourceFileName);
        if (!util::FileExists(sourceFilePath) ||
            util::LastWriteTime(outputFilePath) < util::LastWriteTime(sourceFilePath))
        {
            return false;
        }
    }
    return true;
}

} // namespace otava::build
