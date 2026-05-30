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
    stringTableOffset(FileOffset(0)), stringTableLength(Length(0)), symbolTableOffset(FileOffset(0)), symbolTableLength(Length(0)), 
    symbolIndexMapOffset(FileOffset(0)), symbolIndexMapLength(Length(0)), exportedSymbolsOffset(FileOffset(0)), exportedSymbolsLength(Length(0)), 
    conversionTableOffset(FileOffset(0)), conversionTableLength(Length(0)), fundamentalTypeTableOffset(FileOffset(0)), fundamentalTypeTableLength(Length(0))
{
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
    binaryStreamWriter.Write(ToUnderlying(symbolTableOffset));
    binaryStreamWriter.Write(ToUnderlying(symbolTableLength));
    binaryStreamWriter.Write(ToUnderlying(symbolIndexMapOffset));
    binaryStreamWriter.Write(ToUnderlying(symbolIndexMapLength));
    binaryStreamWriter.Write(ToUnderlying(exportedSymbolsOffset));
    binaryStreamWriter.Write(ToUnderlying(exportedSymbolsLength));
    binaryStreamWriter.Write(ToUnderlying(conversionTableOffset));
    binaryStreamWriter.Write(ToUnderlying(conversionTableLength));
    binaryStreamWriter.Write(ToUnderlying(symbolIdVectorOffset));
    binaryStreamWriter.Write(ToUnderlying(symbolIdVectorLength));
    binaryStreamWriter.Write(ToUnderlying(fundamentalTypeTableOffset));
    binaryStreamWriter.Write(ToUnderlying(fundamentalTypeTableLength));
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
    symbolTableOffset = FileOffset(reader.CurrentReader().ReadUInt());
    symbolTableLength = Length(reader.CurrentReader().ReadUInt());
    symbolIndexMapOffset = FileOffset(reader.CurrentReader().ReadUInt());
    symbolIndexMapLength = Length(reader.CurrentReader().ReadUInt());
    exportedSymbolsOffset = FileOffset(reader.CurrentReader().ReadUInt());
    exportedSymbolsLength = Length(reader.CurrentReader().ReadUInt());
    conversionTableOffset = FileOffset(reader.CurrentReader().ReadUInt());
    conversionTableLength = Length(reader.CurrentReader().ReadUInt());
    symbolIdVectorOffset = FileOffset(reader.CurrentReader().ReadUInt());
    symbolIdVectorLength = Length(reader.CurrentReader().ReadUInt());
    fundamentalTypeTableOffset = FileOffset(reader.CurrentReader().ReadUInt());
    fundamentalTypeTableLength = Length(reader.CurrentReader().ReadUInt());
    reader.PopCurrentReader();
}

Module::Module(util::FileMapping* fileMapping_) :
    kind(ModuleKind::none), stringTable(), nameOffset(), symbolIndexMap(this), symbolTable(this, true), fileMapping(fileMapping_),
    header(), headerRead(false), exportedSymbolsRead(false), fileId(-1), index(Index(-1)), importIndex(Index(-1))
{
    Read();
}

Module::Module(const std::string& name_) :
    kind(ModuleKind::none), stringTable(), nameOffset(stringTable.AddString(name_)), symbolIndexMap(this), symbolTable(this, false),
    fileMapping(), header(), headerRead(false), exportedSymbolsRead(false), fileId(-1), index(Index(-1)), importIndex(Index(-1))
{
}

void Module::Init(Context* context)
{
    symbolTable.Init(context);
}

std::string Module::Name() const
{
    return stringTable.GetString(nameOffset);
}

void Module::AddExportedModuleName(const std::string& exportModuleName)
{
    header.exportedModuleNames.push_back(stringTable.AddString(exportModuleName));
}

std::string Module::GetExportedModuleName(Index index) const
{
    return stringTable.GetString(header.exportedModuleNames[ToUnderlying(index)]);
}

void Module::AddExportedModule(Module* exportedModule)
{
    if (std::find(exportedModules.cbegin(), exportedModules.cend(), exportedModule) == exportedModules.cend())
    {
        exportedModules.push_back(exportedModule);
    }
}

void Module::AddImportedModuleName(const std::string& importModuleName)
{
    header.importedModuleNames.push_back(stringTable.AddString(importModuleName));
}

std::string Module::GetImportedModuleName(Index index) const
{
    return stringTable.GetString(header.importedModuleNames[ToUnderlying(index)]);
}

void Module::AddImportedModule(Module* importedModule)
{
    if (std::find(importedModules.cbegin(), importedModules.cend(), importedModule) == importedModules.cend())
    {
        importedModules.push_back(importedModule);
    }
}

void Module::AddDependsOnModule(Module* dependsOnModule)
{
    if (std::find(dependsOnModules.cbegin(), dependsOnModules.cend(), dependsOnModule) == dependsOnModules.cend())
    {
        dependsOnModules.push_back(dependsOnModule);
    }
}

void Module::Load(const std::string& config, int optLevel, const std::set<std::string>& configurations, Context* context)
{
    ModuleMapper* moduleMapper = context->GetModuleMapper();
    Cardinality importedModuleNameCount = ImportedModuleNameCount();
    for (Index i = Index(0); i < Index(importedModuleNameCount); ++i)
    {
        std::string importedModuleName = GetImportedModuleName(i);
        Module* importedModule = moduleMapper->GetModule(importedModuleName, config, optLevel, configurations, context);
        AddImportedModule(importedModule);
        AddDependsOnModule(importedModule);
    }
    Cardinality exportedModuleNameCount = ExportedModuleNameCount();
    for (Index i = Index(0); i < Index(exportedModuleNameCount); ++i)
    {
        std::string exportedModuleName = GetExportedModuleName(i);
        Module* exportedModule = moduleMapper->GetModule(exportedModuleName, config, optLevel, configurations, context);
        AddExportedModule(exportedModule);
        AddDependsOnModule(exportedModule);
    }
}

void Module::Write(const std::string& root, const std::string& config, int optLevel, Context* context, const std::set<std::string>& configurations)
{
    if (Name() == "std.core")
    {
        int x = 0;
    }
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
    header.stringTableLength = stringTableLength;
    header.symbolTableOffset = FileOffset(writer.Position());
    symbolTable.Write(writer);
    Length symbolTableLength = Length(writer.Position() - ToUnderlying(header.symbolTableOffset));
    header.symbolTableLength = symbolTableLength;
    Context* context = writer.GetContext();
    symbolIndexMap.Import(*context->GetSymbolIndexMap());
    header.symbolIndexMapOffset = FileOffset(writer.Position());
    symbolIndexMap.Write(writer);
    Length symbolIndexMapLength = Length(writer.Position() - ToUnderlying(header.symbolIndexMapOffset));
    header.symbolIndexMapLength = symbolIndexMapLength;
    header.exportedSymbolsOffset = FileOffset(writer.Position());
    for (const auto& exportedModuleNameOffset : header.exportedModuleNames)
    {
        std::string exportedModuleName = GetStringTable()->GetString(exportedModuleNameOffset);
        Module* exportedModule = context->GetModule(exportedModuleName);
        SymbolTable* exportedModuleSymbolTable = exportedModule->GetSymbolTable();
        const std::vector<SymbolId>& symbolIds = exportedModuleSymbolTable->SymbolIds();
        Cardinality count = Cardinality(symbolIds.size());
        writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
        for (SymbolId symbolId : symbolIds)
        {
            writer.GetBinaryStreamWriter().Write(ToUnderlying(symbolId));
            writer.GetBinaryStreamWriter().Write(ToUnderlying(exportedModule->Id()));
        }
    }
    header.exportedSymbolsLength = Length(FileOffset(writer.Position()) - header.exportedSymbolsOffset);
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
    FileOffset end = FileOffset(writer.Position());
    writer.Seek(ToUnderlying(start));
    header.Write(writer);
    writer.Seek(ToUnderlying(end));
    if (header.length != headerLength)
    {
        ThrowException("module '" + Name() + "' header length mismatch");
    }
}

void Module::ReadExportedSymbols()
{
    if (exportedSymbolsRead) return;
    exportedSymbolsRead = true;
    Reader reader(GetFileMapping());
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(header.exportedSymbolsOffset)), header.exportedSymbolsLength);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadUInt());
        ModuleId moduleId = ModuleId(reader.CurrentReader().ReadUInt());
        GetSymbolTable()->MapExportedSymbolId(symbolId, moduleId);
    }
    reader.PopCurrentReader();
}

void Module::SetFile(otava::ast::File* astFile_)
{
    astFile.reset(astFile_);
}

void Module::SetImplementationUnitNames(const std::vector<std::string>& names)
{
    for (const auto& name : names)
    {
        header.implementationUnitNames.push_back(GetStringTable()->AddString(name));
    }
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
    stringTable.SetOffsets(util::Advance(reader.Start(), ToUnderlying(header.stringTableOffset)), header.stringTableLength);
    nameOffset = header.nameOffset;
    id = header.id;
    symbolIndexMap.Read(reader);
    symbolTable.ReadFundamentalTypeMap(reader);
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
            m->Load(config, optLevel, configurations, context);
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
