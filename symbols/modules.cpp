// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.modules;

import otava.symbols.writer;
import otava.symbols.reader;
import otava.symbols.exception;
import otava.symbols.symbol_table;
import otava.symbols.context;
import util.binary_stream_writer;
import util.utility;
import util.path;
import util.unicode;

namespace otava::symbols {

std::string MakeModuleDirPath(const std::string& root, const std::string& config, int optLevel, const std::set<std::string>& configurations)
{
    if (configurations.find("release") != configurations.end())
    {
        return util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(root, "bin"), config),
            std::to_string(otava::symbols::GetOptLevel(optLevel, true))));
    }
    else
    {
        return util::GetFullPath(util::Path::Combine(util::Path::Combine(root, "bin"), config));
    }
}

std::string MakeModuleFilePath(const std::string& root, const std::string& config, int optLevel, const std::string& moduleName, const std::set<std::string>& configurations)
{
    if (configurations.find("release") != configurations.end())
    {
        return util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(root, "bin"), config),
            std::to_string(otava::symbols::GetOptLevel(optLevel, true))), moduleName + ".module"));
    }
    else
    {
        return util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(root, "bin"), config), moduleName + ".module"));
    }
}

std::string MakeProjectFilePath(const std::string& root, const std::string& moduleName)
{
    return util::GetFullPath(util::Path::Combine(root, moduleName + ".project"));
}

ModuleHeader::ModuleHeader() : 
    stringTableOffset(FileOffset(0)), stringTableLength(Length(0)), namespaceIdTableOffset(FileOffset(0)), namespaceIdTableLength(Length(0)),
    symbolTableOffset(FileOffset(0)), symbolTableLength(Length(0)), 
    symbolIndexMapOffset(FileOffset(0)), symbolIndexMapLength(Length(0)), importedSymbolsOffset(FileOffset(0)), importedSymbolsLength(Length(0)), 
    conversionTableOffset(FileOffset(0)), conversionTableLength(Length(0)), fundamentalTypeTableOffset(FileOffset(0)), fundamentalTypeTableLength(Length(0)),
    compoundTypeMapOffset(FileOffset(0)), compoundTypeMapLength(Length(0)), aliasTypeTemplateMapOffset(FileOffset(0)), aliasTypeTemplateMapLength(Length(0)),
    classTemplateSpecializationMapOffset(FileOffset()), classTemplateSpecializationMapLength(Length(0)), explicitInstantiationMapOffset(FileOffset(0)), 
    explicitInstantiationMapLength(Length(0)), functionTypeMapOffset(FileOffset(0)), functionTypeMapLength(Length(0)), 
    astNodeHeaderOffset(FileOffset(0)), astNodeHeaderLength(Length(0))
{
    sectionHeaders.resize(SectionKind::max - SectionKind::first);
}

void ModuleHeader::Write(Writer& writer)
{
    FileOffset start = FileOffset(0);
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    binaryStreamWriter.Write(ToUnderlying(length));
    binaryStreamWriter.Write(ToUnderlying(nameOffset));
    binaryStreamWriter.Write(ToUnderlying(id));
    Cardinality exportedModuleNamesCount = Cardinality(exportedModuleNames.size());
    binaryStreamWriter.Write(ToUnderlying(exportedModuleNamesCount));
    for (const auto& offset : exportedModuleNames)
    {
        binaryStreamWriter.Write(ToUnderlying(offset));
    }
    Cardinality importedModuleNamesCount = Cardinality(importedModuleNames.size());
    binaryStreamWriter.Write(ToUnderlying(importedModuleNamesCount));
    for (const auto& offset : importedModuleNames)
    {
        binaryStreamWriter.Write(ToUnderlying(offset));
    }
    Cardinality implementationUnitNamesCount = Cardinality(implementationUnitNames.size());
    binaryStreamWriter.Write(ToUnderlying(implementationUnitNamesCount));
    for (const auto& offset : implementationUnitNames)
    {
        binaryStreamWriter.Write(ToUnderlying(offset));
    }
    binaryStreamWriter.Write(ToUnderlying(stringTableOffset));
    binaryStreamWriter.Write(ToUnderlying(stringTableLength));
    binaryStreamWriter.Write(ToUnderlying(namespaceIdTableOffset));
    binaryStreamWriter.Write(ToUnderlying(namespaceIdTableLength));
    binaryStreamWriter.Write(ToUnderlying(symbolTableOffset));
    binaryStreamWriter.Write(ToUnderlying(symbolTableLength));
    for (auto& sectionHeader : sectionHeaders)
    {
        sectionHeader.Write(writer);
    }
    binaryStreamWriter.Write(ToUnderlying(symbolIndexMapOffset));
    binaryStreamWriter.Write(ToUnderlying(symbolIndexMapLength));
    binaryStreamWriter.Write(ToUnderlying(importedSymbolsOffset));
    binaryStreamWriter.Write(ToUnderlying(importedSymbolsLength));
    binaryStreamWriter.Write(ToUnderlying(conversionTableOffset));
    binaryStreamWriter.Write(ToUnderlying(conversionTableLength));
    binaryStreamWriter.Write(ToUnderlying(symbolIdVectorOffset));
    binaryStreamWriter.Write(ToUnderlying(symbolIdVectorLength));
    binaryStreamWriter.Write(ToUnderlying(fundamentalTypeTableOffset));
    binaryStreamWriter.Write(ToUnderlying(fundamentalTypeTableLength));
    binaryStreamWriter.Write(ToUnderlying(compoundTypeMapOffset));
    binaryStreamWriter.Write(ToUnderlying(compoundTypeMapLength));
    binaryStreamWriter.Write(ToUnderlying(aliasTypeTemplateMapOffset));
    binaryStreamWriter.Write(ToUnderlying(aliasTypeTemplateMapLength));
    binaryStreamWriter.Write(ToUnderlying(classTemplateSpecializationMapOffset));
    binaryStreamWriter.Write(ToUnderlying(classTemplateSpecializationMapLength));
    binaryStreamWriter.Write(ToUnderlying(explicitInstantiationMapOffset));
    binaryStreamWriter.Write(ToUnderlying(explicitInstantiationMapLength));
    binaryStreamWriter.Write(ToUnderlying(functionTypeMapOffset));
    binaryStreamWriter.Write(ToUnderlying(functionTypeMapLength));
    binaryStreamWriter.Write(ToUnderlying(astNodeHeaderOffset));
    binaryStreamWriter.Write(ToUnderlying(astNodeHeaderLength));
    FileOffset end = FileOffset(writer.Position());
    length = end - start;
    writer.Seek(ToUnderlying(start));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(length));
    writer.Seek(ToUnderlying(end));
}

void ModuleHeader::Read(Reader& reader)
{
    reader.PushCurrentReader(reader.Start(), Length(sizeof(Length)));
    length = Length(reader.CurrentReader().ReadUInt());
    reader.PopCurrentReader();
    reader.PushCurrentReader(util::Advance(reader.Start(), sizeof(length)), length);
    nameOffset = StringOffset(reader.CurrentReader().ReadUInt());
    id = ModuleId(reader.CurrentReader().ReadUInt());
    Cardinality exportedModuleNamesCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(exportedModuleNamesCount); ++i)
    {
        StringOffset offset = StringOffset(reader.CurrentReader().ReadUInt());
        exportedModuleNames.push_back(offset);
    }
    Cardinality importedModuleNamesCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(importedModuleNamesCount); ++i)
    {
        StringOffset offset = StringOffset(reader.CurrentReader().ReadUInt());
        importedModuleNames.push_back(offset);
    }
    Cardinality implementationUnitNamesCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(implementationUnitNamesCount); ++i)
    {
        StringOffset offset = StringOffset(reader.CurrentReader().ReadUInt());
        implementationUnitNames.push_back(offset);
    }
    stringTableOffset = FileOffset(reader.CurrentReader().ReadUInt());
    stringTableLength = Length(reader.CurrentReader().ReadUInt());
    namespaceIdTableOffset = FileOffset(reader.CurrentReader().ReadUInt());
    namespaceIdTableLength = Length(reader.CurrentReader().ReadUInt());
    symbolTableOffset = FileOffset(reader.CurrentReader().ReadUInt());
    symbolTableLength = Length(reader.CurrentReader().ReadUInt());
    for (auto& sectionHeader : sectionHeaders)
    {
        sectionHeader.Read(reader);
    }
    symbolIndexMapOffset = FileOffset(reader.CurrentReader().ReadUInt());
    symbolIndexMapLength = Length(reader.CurrentReader().ReadUInt());
    importedSymbolsOffset = FileOffset(reader.CurrentReader().ReadUInt());
    importedSymbolsLength = Length(reader.CurrentReader().ReadUInt());
    conversionTableOffset = FileOffset(reader.CurrentReader().ReadUInt());
    conversionTableLength = Length(reader.CurrentReader().ReadUInt());
    symbolIdVectorOffset = FileOffset(reader.CurrentReader().ReadUInt());
    symbolIdVectorLength = Length(reader.CurrentReader().ReadUInt());
    fundamentalTypeTableOffset = FileOffset(reader.CurrentReader().ReadUInt());
    fundamentalTypeTableLength = Length(reader.CurrentReader().ReadUInt());
    compoundTypeMapOffset = FileOffset(reader.CurrentReader().ReadUInt());
    compoundTypeMapLength = Length(reader.CurrentReader().ReadUInt());
    aliasTypeTemplateMapOffset = FileOffset(reader.CurrentReader().ReadUInt());
    aliasTypeTemplateMapLength = Length(reader.CurrentReader().ReadUInt());
    classTemplateSpecializationMapOffset = FileOffset(reader.CurrentReader().ReadUInt());
    classTemplateSpecializationMapLength = Length(reader.CurrentReader().ReadUInt());
    explicitInstantiationMapOffset = FileOffset(reader.CurrentReader().ReadUInt());
    explicitInstantiationMapLength = Length(reader.CurrentReader().ReadUInt());
    functionTypeMapOffset = FileOffset(reader.CurrentReader().ReadUInt());
    functionTypeMapLength = Length(reader.CurrentReader().ReadUInt());
    astNodeHeaderOffset = FileOffset(reader.CurrentReader().ReadUInt());
    astNodeHeaderLength = Length(reader.CurrentReader().ReadUInt());
    reader.PopCurrentReader();
}

Module::Module(util::FileMapping* fileMapping_) :
    kind(ModuleKind::none), stringTable(this), nameOffset(), name(""), symbolIndexMap(this), symbolTable(this, true),
    evaluationContext(this, true), fileMapping(fileMapping_), header(), headerRead(false), importedSymbolsRead(false), fileId(-1), 
    index(Index(-1)), importIndex(Index(-1)), exportedModulesAdded(false), importedModulesAdded(false), astNodeRead(false), 
    namespaceIdsRead(false), destructing(false)
{
    Read();
}

Module::Module(const std::string& name_) :
    kind(ModuleKind::none), stringTable(this), nameOffset(stringTable.AddString(name_)), name(stringTable.CharPtr(nameOffset)), 
    symbolIndexMap(this), symbolTable(this, false), evaluationContext(this, false), fileMapping(), header(), headerRead(false), 
    importedSymbolsRead(false), fileId(-1), index(Index(-1)), importIndex(Index(-1)), exportedModulesAdded(false), 
    importedModulesAdded(false), astNodeRead(false), namespaceIdsRead(false), destructing(false)
{
}

Module::~Module()
{
    destructing = true;
    for (FunctionSymbol* fn : fns)
    {
        fn->ResetModule();
    }
    for (Scope* scope : scopes)
    {
        scope->ResetModule();
    }
    std::cout << "~module:" << Name() << "\n";
}

void Module::AddFunction(FunctionSymbol* fn)
{
    fns.push_back(fn);
}

void Module::RemoveFunction(FunctionSymbol* fn)
{
    if (!destructing)
    {
        fns.erase(std::remove(fns.begin(), fns.end(), fn), fns.end());
    }
}

void Module::AddScope(Scope* scope)
{
    scopes.push_back(scope);
}

void Module::RemoveScope(Scope* scope)
{
    if (!destructing)
    {
        scopes.erase(std::remove(scopes.begin(), scopes.end(), scope), scopes.end());
    }
}

void Module::Init(Context* context)
{
    symbolTable.Init(context);
    evaluationContext.Init(context);
}

std::string Module::Name() 
{
    return stringTable.GetString(nameOffset);
}

void Module::AddExportedModuleName(const std::string& exportModuleName)
{
    header.exportedModuleNames.push_back(stringTable.AddString(exportModuleName));
}

std::string Module::GetExportedModuleName(Index index) 
{
    return stringTable.GetString(header.exportedModuleNames[ToUnderlying(index)]);
}

std::vector<Module*> Module::ExportedModules(Context* context)
{
    std::vector<Module*> exportedModules;
    Cardinality count = ExportedModuleNameCount();
    for (Index i = Index(0); i < Index(count); ++i)
    {
        std::string exportedModuleName = GetExportedModuleName(i);
        Module* m = context->GetModule(exportedModuleName);
        exportedModules.push_back(m);
    }
    return exportedModules;
}

std::vector<Module*> Module::ImportedModules(Context* context)
{
    std::vector<Module*> importedModules;
    Cardinality count = ImportedModuleNameCount();
    for (Index i = Index(0); i < Index(count); ++i)
    {
        std::string importedModuleName = GetImportedModuleName(i);
        Module* m = context->GetModule(importedModuleName);
        importedModules.push_back(m);
    }
    return importedModules;
}

std::vector<Module*> Module::ImportExportModules(Context* context)
{
    std::vector<Module*> importExportModules;
    std::vector<Module*> importedModules = ImportedModules(context);
    for (Module* importedModule : importedModules)
    {
        if (std::find(importExportModules.begin(), importExportModules.end(), importedModule) == importExportModules.end())
        {
            importExportModules.push_back(importedModule);
        }
        const std::vector<Module*>& allExportedModules = importedModule->AllExportedModules(context);
        for (Module* exportedModule : allExportedModules)
        {
            if (std::find(importExportModules.begin(), importExportModules.end(), exportedModule) == importExportModules.end())
            {
                importExportModules.push_back(exportedModule);
            }
        }
    }
    return importExportModules;
}

void Module::AddImportedModuleName(const std::string& importModuleName)
{
    header.importedModuleNames.push_back(stringTable.AddString(importModuleName));
}

std::string Module::GetImportedModuleName(Index index)
{
    return stringTable.GetString(header.importedModuleNames[ToUnderlying(index)]);
}

void Module::AddDependsOnModule(Module* dependsOnModule)
{
    if (std::find(dependsOnModules.cbegin(), dependsOnModules.cend(), dependsOnModule) == dependsOnModules.cend())
    {
        dependsOnModules.push_back(dependsOnModule);
    }
}

void Module::Write(const std::string& root, const std::string& config, int optLevel, Context* context, const std::set<std::string>& configurations)
{
    std::string moduleFilePath = MakeModuleFilePath(root, config, optLevel, Name(), configurations);
    Writer writer(moduleFilePath);
    writer.SetContext(context);
    Write(writer);
}

void Module::Write(Writer& writer)
{
    FileOffset start = FileOffset(writer.Position());
    header.nameOffset = nameOffset;
    header.id = id;
    header.Write(writer);
    Length headerLength = header.length;
    header.stringTableOffset = FileOffset(writer.Position());
    GetStringTable()->Write(writer);
    Length stringTableLength = Length(writer.Position() - ToUnderlying(header.stringTableOffset));
    header.namespaceIdTableOffset = FileOffset(writer.Position());
    WriteNamespaceIdTable(writer);
    Length namespaceIdTableLength = Length(writer.Position() - ToUnderlying(header.namespaceIdTableOffset));
    header.namespaceIdTableLength = namespaceIdTableLength;
    header.stringTableLength = stringTableLength;
    Context* context = writer.GetContext();
    header.evaluationContextOffset = FileOffset(writer.Position());
    evaluationContext.Write(writer, context);
    header.evaluationContextLength = Length(writer.Position() - ToUnderlying(header.evaluationContextOffset));
    header.symbolTableOffset = FileOffset(writer.Position());
    symbolTable.Write(writer);
    Length symbolTableLength = Length(writer.Position() - ToUnderlying(header.symbolTableOffset));
    header.symbolTableLength = symbolTableLength;
    symbolIndexMap.Import(*context->GetSymbolIndexMap());
    header.symbolIndexMapOffset = FileOffset(writer.Position());
    symbolIndexMap.Write(writer);
    Length symbolIndexMapLength = Length(writer.Position() - ToUnderlying(header.symbolIndexMapOffset));
    header.symbolIndexMapLength = symbolIndexMapLength;
    header.importedSymbolsOffset = FileOffset(writer.Position());
    WriteImportedSymbols(writer);
    header.importedSymbolsLength = Length(FileOffset(writer.Position()) - header.importedSymbolsOffset);
    header.conversionTableOffset = FileOffset(writer.Position());
    GetSymbolTable()->GetConversionTable()->Write(writer);
    header.conversionTableLength = Length(FileOffset(writer.Position()) - header.conversionTableOffset);
    header.symbolIdVectorOffset = FileOffset(writer.Position());
    GetSymbolTable()->WriteSymbolIdVector(writer);
    header.symbolIdVectorLength = Length(FileOffset(writer.Position()) - header.symbolIdVectorOffset);
    header.fundamentalTypeTableOffset = FileOffset(writer.Position());
    symbolTable.WriteFundamentalTypeMap(writer);
    Length fundamentalTypeTableLength = Length(writer.Position() - ToUnderlying(header.fundamentalTypeTableOffset));
    header.fundamentalTypeTableLength = fundamentalTypeTableLength;
    header.compoundTypeMapOffset = FileOffset(writer.Position());
    symbolTable.WriteCompoundTypeMap(writer);
    Length compoundTypeMapLength = Length(writer.Position() - ToUnderlying(header.compoundTypeMapOffset));
    header.compoundTypeMapLength = compoundTypeMapLength;
    header.aliasTypeTemplateMapOffset = FileOffset(writer.Position());
    symbolTable.WriteAliasTypeTemplateMap(writer);
    Length aliasTypeTemplateMapLength = Length(writer.Position() - ToUnderlying(header.aliasTypeTemplateMapOffset));
    header.aliasTypeTemplateMapLength = aliasTypeTemplateMapLength;
    header.classTemplateSpecializationMapOffset = FileOffset(writer.Position());
    symbolTable.WriteClassTemplateSpecializationMap(writer);
    Length classTemplateSpecializationMapLength = Length(writer.Position() - ToUnderlying(header.classTemplateSpecializationMapOffset));
    header.classTemplateSpecializationMapLength = classTemplateSpecializationMapLength;
    header.explicitInstantiationMapOffset = FileOffset(writer.Position());
    symbolTable.WriteExplicitInstantiationMap(writer);
    Length explicitInstantiationMapLength = Length(writer.Position() - ToUnderlying(header.explicitInstantiationMapOffset));
    header.explicitInstantiationMapLength = explicitInstantiationMapLength;
    header.functionTypeMapOffset = FileOffset(writer.Position());
    symbolTable.WriteFunctionTypeMap(writer);
    Length functionTypeMapLength = Length(writer.Position() - ToUnderlying(header.functionTypeMapOffset));
    header.functionTypeMapLength = functionTypeMapLength;
    header.astNodeHeaderOffset = FileOffset(writer.Position());
    otava::symbols::WriteNode(writer, astNode.get(), astNodeHeader);
    Length astNodeHeaderLength = Length(writer.Position() - ToUnderlying(header.astNodeHeaderOffset));
    header.astNodeHeaderLength = astNodeHeaderLength;
    FileOffset end = FileOffset(writer.Position());
    writer.Seek(ToUnderlying(start));
    header.Write(writer);
    writer.Seek(ToUnderlying(end));
    if (header.length != headerLength)
    {
        ThrowException("module '" + Name() + "' header length mismatch");
    }
}

void Module::WriteImportedSymbols(Writer& writer)
{
    const std::unordered_map<SymbolId, ModuleId>& addedImportedSymbolMap = symbolTable.AddedImportedSymbolMap();
    Cardinality count = Cardinality(addedImportedSymbolMap.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (const auto& importedSymbol : addedImportedSymbolMap)
    {
        SymbolId symbolId = importedSymbol.first;
        ModuleId moduleId = importedSymbol.second;
        writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
        writer.GetBinaryStreamWriter().Write(ToUnderlying(moduleId));
    }
}

void Module::ReadImportedSymbols()
{
    if (importedSymbolsRead) return;
    importedSymbolsRead = true;
    Reader reader(GetFileMapping());
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(header.importedSymbolsOffset)), header.importedSymbolsLength);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        ModuleId moduleId = ModuleId(reader.CurrentReader().ReadUInt());
        GetSymbolTable()->MapImportedSymbolId(symbolId, moduleId);
    }
    reader.PopCurrentReader();
}

void Module::ReadAstNode()
{
    if (!IsReadOnly()) return;
    if (astNodeRead) return;
    astNodeRead = true;
    Reader reader(GetFileMapping());
    reader.PushCurrentReader(util::Advance(fileMapping->Start(), ToUnderlying(header.astNodeHeaderOffset)), header.astNodeHeaderLength);
    astNode = otava::symbols::ReadNode(reader, this, astNodeHeader);
    reader.PopCurrentReader();
}

otava::ast::Node* Module::GetAstNode(std::int64_t astNodeId) const
{
    otava::ast::Node* astNode = astNodeMap.GetNodeNoThrow(astNodeId);
    return astNode;
}

void Module::SetImplementationUnitNames(const std::vector<std::string>& names)
{
    for (const auto& name : names)
    {
        header.implementationUnitNames.push_back(GetStringTable()->AddString(name));
    }
}

void Module::GetAllExportedModules(std::vector<Module*>& targetExportedModules, Context* context)
{
    std::vector<Module*> exportedModules = ExportedModules(context);
    for (Module* m : exportedModules)
    {
        if (std::find(targetExportedModules.begin(), targetExportedModules.end(), m) == targetExportedModules.end())
        {
            targetExportedModules.push_back(m);
        }
        m->GetAllExportedModules(targetExportedModules, context);
    }
}

void Module::GetAllExportedModules(Context* context)
{
    if (exportedModulesAdded) return;
    exportedModulesAdded = true;
    GetAllExportedModules(allExportedModules, context);
}

const std::vector<Module*>& Module::AllExportedModules(Context* context)
{
    GetAllExportedModules(context);
    return allExportedModules;
}

void Module::GetAllImportedModules(std::vector<Module*>& targetImportedModules, Context* context)
{
    std::vector<Module*> importedModules = ImportedModules(context);
    for (Module* m : importedModules)
    {
        if (std::find(targetImportedModules.begin(), targetImportedModules.end(), m) == targetImportedModules.end())
        {
            targetImportedModules.push_back(m);
        }
        m->GetAllExportedModules(targetImportedModules, context);
        m->GetAllImportedModules(targetImportedModules, context);
    }
}

void Module::GetAllImportedModules(Context* context)
{
    if (importedModulesAdded) return;
    importedModulesAdded = true;
    GetAllImportedModules(allImportedModules, context);
}

const std::vector<Module*>& Module::AllImportedModules(Context* context)
{
    GetAllImportedModules(context);
    return allImportedModules;
}

void Module::AddFunctionSymbol(FunctionSymbol* fn)
{
    fns.push_back(fn);
}

void Module::RemoveFunctionSymbol(FunctionSymbol* fn)
{
    fns.erase(std::remove(fns.begin(), fns.end(), fn), fns.end());
}

void Module::AddNamespaceId(SymbolId namespaceId)
{
    if (std::find(namespaceIds.begin(), namespaceIds.end(), namespaceId) == namespaceIds.end())
    {
        namespaceIds.push_back(namespaceId);
    }
}

void Module::WriteNamespaceIdTable(Writer& writer)
{
    Cardinality n = Cardinality(namespaceIds.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(n));
    for (Index i = Index(0); i < Index(n); ++i)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(namespaceIds[ToUnderlying(i)]));
    }
}

void Module::ReadNamespaceIdTable()
{
    if (namespaceIdsRead) return;
    namespaceIdsRead = true;
    Reader reader(GetFileMapping());
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(header.namespaceIdTableOffset)), header.namespaceIdTableLength);
    ReadNamespaceIdTable(reader);
    reader.PopCurrentReader();
}

void Module::ReadNamespaceIdTable(Reader& reader)
{
    Cardinality n = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        SymbolId namespaceId = SymbolId(reader.CurrentReader().ReadUInt());
        namespaceIds.push_back(namespaceId);
    }
}

const std::vector<SymbolId>& Module::NamespaceIds()
{
    if (IsReadOnly())
    {
        ReadNamespaceIdTable();
    }
    return namespaceIds;
}

util::FileMapping* Module::GetFileMapping()
{
    if (!fileMapping)
    {
        ThrowException("module: file mapping not set");
    }
    return fileMapping.get();
}

void Module::Read()
{
    if (headerRead) return;
    headerRead = true;
    Reader reader(GetFileMapping());
    header.Read(reader);
    nameOffset = header.nameOffset;
    name = stringTable.CharPtr(nameOffset);
    id = header.id;
    symbolIndexMap.Read(reader);
}

ModuleMapper::ModuleMapper() : moduleCount(Cardinality(0)), nextModuleId(ModuleId(1))
{
    roots.push_back(util::GetFullPath(util::Path::Combine(util::OtavaRoot(), "std")));
}

void ModuleMapper::AddRoot(const std::string& root)
{
    if (std::find(roots.cbegin(), roots.cend(), root) == roots.cend())
    {
        roots.push_back(root);
    }
}

Module* ModuleMapper::GetModule(const std::string& moduleName, const std::string& config, int optLevel, const std::set<std::string>& configurations, Context* context)
{
    auto it = moduleNameMap.find(moduleName);
    if (it != moduleNameMap.end())
    {
        return it->second;
    }
    return LoadModule(moduleName, config, optLevel, configurations, context);
}

Module* ModuleMapper::LoadModule(const std::string& moduleName, const std::string& config, int optLevel, const std::set<std::string>& configurations, Context* context)
{
    for (const auto& root : roots)
    {
        std::string moduleFilePath = MakeModuleFilePath(root, config, optLevel, moduleName, configurations);
        if (util::FileExists(moduleFilePath))
        {
            Module* m = new Module(new util::FileMapping(moduleFilePath));
            symbolIndexMap.Import(m->GetSymbolIndexMap());
            moduleNameMap[moduleName] = m;
            MapModule(m);
            modules.push_back(std::unique_ptr<Module>(m));
            return m;
        }
    }
    SetExceptionThrown();
    throw std::runtime_error("module '" + moduleName + "' not found");
}

void ModuleMapper::MapModule(Module* module)
{
    moduleIdMap[module->Id()] = module;
}

Module* ModuleMapper::GetModule(ModuleId moduleId) const noexcept
{
    auto it = moduleIdMap.find(moduleId);
    if (it != moduleIdMap.end())
    {
        return it->second;
    }
    return nullptr;
}

void ModuleMapper::RemoveModule(Module* module)
{
    moduleNameMap.erase(module->Name());
    moduleIdMap.erase(module->Id());
}

std::string ModuleMapper::GetProjectFilePath(const std::string& moduleName) const
{
    for (const auto& root : roots)
    {
        std::string projectFilePath = MakeProjectFilePath(root, moduleName);
        if (util::FileExists(projectFilePath))
        {
            return projectFilePath;
        }
    }
    return std::string();
}

} // namespace otava::symbols
