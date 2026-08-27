// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.modules;

import otava.symbols.writer;
import otava.symbols.reader;
import otava.symbols.exception;
import otava.symbols.context;
import otava.symbols.concrete_value;
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
    length(Length(0)), nameOffset(StringOffset(0)), id(ModuleId(zeroModuleId)),
    stringTableOffset(FileOffset(0)), stringTableLength(Length(0)), namespaceIdTableOffset(FileOffset(0)), namespaceIdTableLength(Length(0)),
    symbolTableOffset(FileOffset(0)), symbolTableLength(Length(0)), evaluationContextOffset(FileOffset(0)), evaluationContextLength(Length(0)),
    symbolIndexMapOffset(FileOffset(0)), symbolIndexMapLength(Length(0)), importedSymbolsOffset(FileOffset(0)), importedSymbolsLength(Length(0)), 
    conversionTableOffset(FileOffset(0)), conversionTableLength(Length(0)), symbolIdVectorOffset(FileOffset(0)), symbolIdVectorLength(Length(0)),
    fundamentalTypeTableOffset(FileOffset(0)), fundamentalTypeTableLength(Length(0)),
    compoundTypeMapOffset(FileOffset(0)), compoundTypeMapLength(Length(0)), aliasTypeTemplateMapOffset(FileOffset(0)), aliasTypeTemplateMapLength(Length(0)),
    classTemplateSpecializationMapOffset(FileOffset()), classTemplateSpecializationMapLength(Length(0)), explicitInstantiationMapOffset(FileOffset(0)), 
    explicitInstantiationMapLength(Length(0)), functionTypeMapOffset(FileOffset(0)), functionTypeMapLength(Length(0)), 
    astNodeHeaderOffset(FileOffset(0)), astNodeHeaderLength(Length(0)), incompleteClassIdOffset(FileOffset(0)), incompleteClassIdLength(Length(0)),
    importedModuleTableOffset(FileOffset(0)), importedModuleTableLength(Length(0))
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
    binaryStreamWriter.Write(ToUnderlying(incompleteClassIdOffset));
    binaryStreamWriter.Write(ToUnderlying(incompleteClassIdLength));
    binaryStreamWriter.Write(ToUnderlying(importedModuleTableOffset));
    binaryStreamWriter.Write(ToUnderlying(importedModuleTableLength));
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
    reader.PushCurrentReader(util::Advance(reader.Start(), std::uint32_t(sizeof(length))), length);
    nameOffset = StringOffset(reader.CurrentReader().ReadUInt());
    id = ModuleId(reader.CurrentReader().ReadUInt());
    Cardinality exportedModuleNamesCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < ToIndex(exportedModuleNamesCount); ++i)
    {
        StringOffset offset = StringOffset(reader.CurrentReader().ReadUInt());
        exportedModuleNames.push_back(offset);
    }
    Cardinality importedModuleNamesCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < ToIndex(importedModuleNamesCount); ++i)
    {
        StringOffset offset = StringOffset(reader.CurrentReader().ReadUInt());
        importedModuleNames.push_back(offset);
    }
    Cardinality implementationUnitNamesCount = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < ToIndex(implementationUnitNamesCount); ++i)
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
    incompleteClassIdOffset = FileOffset(reader.CurrentReader().ReadUInt());
    incompleteClassIdLength = Length(reader.CurrentReader().ReadUInt());
    importedModuleTableOffset = FileOffset(reader.CurrentReader().ReadUInt());
    importedModuleTableLength = Length(reader.CurrentReader().ReadUInt());
    reader.PopCurrentReader();
}

void ImportedModuleTableEntry::Write(Writer& writer)
{
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    binaryStreamWriter.Write(ToUnderlying(moduleId));
    binaryStreamWriter.Write(ToUnderlying(moduleNameOffset));
}

void ImportedModuleTableEntry::Read(Reader& reader)
{
    moduleId = ModuleId(reader.CurrentReader().ReadUInt());
    moduleNameOffset = StringOffset(reader.CurrentReader().ReadUInt());
}

Module::Module(util::FileMapping* fileMapping_) :
    kind(ModuleKind::none), id(zeroModuleId), 
    stringTable(this), nameOffset(), name(""), interfaceUnitNameOffset(), interfaceUnitName(""), symbolIndexMap(this), symbolTable(this, true),
    evaluationContext(this, true), fileMapping(fileMapping_), header(), headerRead(false), importedSymbolsRead(false), fileId(-1),
    index(Index(-1)), importIndex(Index(-1)), exportedModulesAdded(false), importedModulesAdded(false), astNodeRead(false),
    namespaceIdsRead(false), incompleteClassIdsRead(false), destructing(false), importedModuleTableRead(false)
{
    Read();
}

Module::Module(const std::string& name_) :
    kind(ModuleKind::none), id(zeroModuleId), stringTable(this), nameOffset(stringTable.AddString(name_)), name(stringTable.CharPtr(nameOffset)),
    interfaceUnitNameOffset(), interfaceUnitName(""),
    symbolIndexMap(this), symbolTable(this, false), evaluationContext(this, false), fileMapping(), header(), headerRead(false),
    importedSymbolsRead(false), fileId(-1), index(Index(-1)), importIndex(Index(-1)), exportedModulesAdded(false),
    importedModulesAdded(false), astNodeRead(false), namespaceIdsRead(false), incompleteClassIdsRead(false), destructing(false), importedModuleTableRead(false)
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
    for (Symbol* symbol : symbolVec)
    {
        symbol->ResetModule();
    }
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

void Module::AddSymbol(Symbol* symbol)
{
    symbolVec.push_back(symbol);
}

void Module::RemoveSymbol(Symbol* symbol)
{
    if (!destructing)
    {
        symbolVec.erase(std::remove(symbolVec.begin(), symbolVec.end(), symbol), symbolVec.end());
    }
}

void Module::Init(Context* context)
{
    symbolTable.Init(context);
    evaluationContext.Init(context);
}

void Module::AddIncompleteClassId(SymbolId classId)
{
    if (std::find(incompleteClassIds.begin(), incompleteClassIds.end(), classId) == incompleteClassIds.end())
    {
        incompleteClassIds.push_back(classId);
    }
}

std::string Module::Name()
{
    return stringTable.GetString(nameOffset);
}

void Module::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

std::string Module::InterfaceUnitName() 
{
    if (kind == ModuleKind::implementationModule)
    {
        return stringTable.GetString(interfaceUnitNameOffset);
    }
    return std::string();
}

void Module::SetInterfaceUnitName(const std::string& interfaceUnitName)
{
    interfaceUnitNameOffset = stringTable.AddString(interfaceUnitName);
}

Module* Module::InterfaceModule(Context* context) 
{
    std::string interfaceUnitName = InterfaceUnitName();
    if (!interfaceUnitName.empty())
    {
        return context->GetModule(interfaceUnitName);
    }
    return nullptr;
}

void Module::SetId(ModuleId id_) noexcept
{ 
    id = id_; 
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
    for (Index i = Index(0); i < ToIndex(count); ++i)
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
    if (Kind() == ModuleKind::implementationModule)
    {
        std::string interfaceUnitName = InterfaceUnitName();
        Module* interfaceUnit = context->GetModule(interfaceUnitName);
        if (interfaceUnit != this)
        {
            if (std::find(importedModules.begin(), importedModules.end(), interfaceUnit) == importedModules.end())
            {
                importedModules.push_back(interfaceUnit);
            }
            std::vector<Module*> interfaceUnitImports = interfaceUnit->ImportedModules(context);
            for (Module* importedModule : interfaceUnitImports)
            {
                if (importedModule != this)
                {
                    if (std::find(importedModules.begin(), importedModules.end(), importedModule) == importedModules.end())
                    {
                        importedModules.push_back(importedModule);
                    }
                }
            }
        }
    }
    Cardinality count = ImportedModuleNameCount();
    for (Index i = Index(0); i < ToIndex(count); ++i)
    {
        std::string importedModuleName = GetImportedModuleName(i);
        Module* m = context->GetModule(importedModuleName);
        if (std::find(importedModules.begin(), importedModules.end(), m) == importedModules.end())
        {
            importedModules.push_back(m);
        }
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

Module* Module::GetImportedModuleById(ModuleId moduleId, Context* context)
{
    ReadImportedModuleTable();
    for (const auto& entry : importedModuleTable)
    {
        if (entry.moduleId == moduleId)
        {
            std::string moduleName = stringTable.GetString(entry.moduleNameOffset);
            Module* module = context->GetModule(moduleName);
            return module;
        }
    }
    return nullptr;
}

void Module::AddImportedModule(Module* module)
{
    for (const auto& entry : importedModuleTable)
    {
        if (entry.moduleId == module->Id())
        {
            return;
        }
    }
    StringOffset moduleNameOffset = stringTable.AddString(module->Name());
    ImportedModuleTableEntry entry(module->Id(), moduleNameOffset);
    importedModuleTable.push_back(entry);
}

void Module::SetModuleFilePath(const std::string& root, const std::string& config, int optLevel, Context* context, const std::set<std::string>& configurations)
{
    moduleFilePath = MakeModuleFilePath(root, config, optLevel, Name(), configurations);
}

void Module::Write(const std::string& root, const std::string& config, int optLevel, Context* context, const std::set<std::string>& configurations)
{
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
    symbolTable.WriteCompoundTypeMaps(writer);
    Length compoundTypeMapLength = Length(writer.Position() - ToUnderlying(header.compoundTypeMapOffset));
    header.compoundTypeMapLength = compoundTypeMapLength;
    header.aliasTypeTemplateMapOffset = FileOffset(writer.Position());
    symbolTable.WriteAliasTypeTemplateMap(writer);
    Length aliasTypeTemplateMapLength = Length(writer.Position() - ToUnderlying(header.aliasTypeTemplateMapOffset));
    header.aliasTypeTemplateMapLength = aliasTypeTemplateMapLength;
    header.classTemplateSpecializationMapOffset = FileOffset(writer.Position());
    symbolTable.WriteClassTemplateSpecializationMaps(writer);
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
    header.incompleteClassIdOffset = FileOffset(writer.Position());
    WriteIncompleteClassIdTable(writer);
    Length incompleteClassIdLength = Length(writer.Position() - ToUnderlying(header.incompleteClassIdOffset));
    header.incompleteClassIdLength = incompleteClassIdLength;
    header.importedModuleTableOffset = FileOffset(writer.Position());
    WriteImportedModuleTable(writer);
    Length importedModuleTableLength = Length(writer.Position() - ToUnderlying(header.importedModuleTableOffset));
    header.importedModuleTableLength = importedModuleTableLength;
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
    for (Index i = Index(0); i < ToIndex(count); ++i)
    {
        SymbolId symbolId = SymbolId(reader.CurrentReader().ReadULong());
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
    for (Index i = Index(0); i < ToIndex(n); ++i)
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
    for (Index i = Index(0); i < ToIndex(n); ++i)
    {
        SymbolId namespaceId = SymbolId(reader.CurrentReader().ReadULong());
        namespaceIds.push_back(namespaceId);
    }
}

void Module::WriteIncompleteClassIdTable(Writer& writer)
{
    Cardinality n = Cardinality(incompleteClassIds.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(n));
    for (Index i = Index(0); i < ToIndex(n); ++i)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(incompleteClassIds[ToUnderlying(i)]));
    }
}

void Module::ReadIncompleteClassIdTable()
{
    if (incompleteClassIdsRead) return;
    incompleteClassIdsRead = true;
    Reader reader(GetFileMapping());
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(header.incompleteClassIdOffset)), header.incompleteClassIdLength);
    ReadIncompleteClassIdTable(reader);
    reader.PopCurrentReader();
}

void Module::ReadIncompleteClassIdTable(Reader& reader)
{
    Cardinality n = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < ToIndex(n); ++i)
    {
        SymbolId classId = SymbolId(reader.CurrentReader().ReadULong());
        incompleteClassIds.push_back(classId);
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

SectionHeader* Module::GetSectionHeader(SectionKind sectionKind) noexcept
{
    std::uint8_t sectionIndex = std::uint8_t(sectionKind) - std::uint8_t(SectionKind::first);
    SectionHeader& sh = header.sectionHeaders[sectionIndex];
    SectionHeader* p = &sh;
    return p;
}

void Module::WriteImportedModuleTable(Writer& writer)
{
    Cardinality n = Cardinality(importedModuleTable.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(n));
    for (Index i = Index(0); i < ToIndex(n); ++i)
    {
        importedModuleTable[ToUnderlying(i)].Write(writer);
    }
}

void Module::ReadImportedModuleTable()
{
    if (importedModuleTableRead) return;
    importedModuleTableRead = true;
    Reader reader(GetFileMapping());
    reader.PushCurrentReader(util::Advance(reader.Start(), ToUnderlying(header.importedModuleTableOffset)), header.importedModuleTableLength);
    ReadImportedModuleTable(reader);
    reader.PopCurrentReader(); 
}

void Module::ReadImportedModuleTable(Reader& reader)
{
    Cardinality n = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < ToIndex(n); ++i)
    {
        ImportedModuleTableEntry entry;
        entry.Read(reader);
        importedModuleTable.push_back(entry);
    }
}

bool Module::UpToDate() const noexcept
{
    if (!util::FileExists(moduleFilePath) || util::LastWriteTime(filePath) > util::LastWriteTime(moduleFilePath)) return false;
    return true;
}

ModuleMapper::ModuleMapper()
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

ModuleId ModuleMapper::MakeModuleId(const std::string& moduleName) noexcept
{
    ModuleId moduleId = ModuleId(std::hash<std::string>()(moduleName) & 0xFFFFFFFFu);
    while (moduleIdMap.find(moduleId) != moduleIdMap.end())
    {
        ++moduleId;
    }
    return moduleId;
}

void ModuleMapper::AddBuiltModule(Module* module)
{
    builtModules.push_back(std::unique_ptr<Module>(module));
}

struct ById
{
    bool operator()(const std::pair<std::string, ModuleId>& left, const std::pair<std::string, ModuleId>& right) const noexcept
    {
        return left.second < right.second;
    }
};

void ModuleMapper::PrintModules(std::ostream& s)
{
    std::vector<std::pair<std::string, ModuleId>> m;
    for (const auto& module : modules)
    {
        m.push_back(std::make_pair(module->Name(), module->Id()));
    }
    for (const auto& module : builtModules)
    {
        m.push_back(std::make_pair(module->Name(), module->Id()));
    }
    std::sort(m.begin(), m.end(), ById());
    m.erase(std::unique(m.begin(), m.end()), m.end());
    for (const auto& p : m)
    {
        s << ToUnderlying(p.second) << ":" << p.first << "\n";
    }
}

ProjectId ModuleMapper::MakeProjectId(const std::string& projectName)
{
    ProjectId projectId = ProjectId(std::hash<std::string>()(projectName) & 0xFFFFFFFFu);
    while (projectIds.find(projectId) != projectIds.end())
    {
        ++projectId;
    }
    projectIds.insert(projectId);
    return projectId;
}

void ModuleMapper::AddProjectId(const std::string& projectName, ProjectId projectId)
{
    if (projectIds.find(projectId) != projectIds.end())
    {
        otava::symbols::ThrowException("project id " + std::to_string(otava::symbols::ToUnderlying(projectId)) + " not unique: please change the name of the project '" +
            projectName);
    }
    projectIds.insert(projectId);
}

ModulePtr::ModulePtr(Module* module_, Context* context_) noexcept : module(module_), context(context_), prevModule(nullptr)
{
    prevModule = context->GetModule();
    if (!module->GetSymbolTable()->CurrentScope())
    {
        module->GetSymbolTable()->SetCurrentScope(module->GetSymbolTable()->GetGlobalNs(context)->GetScope());
    }
    context->SetModule(module);
}

ModulePtr::~ModulePtr()
{
    if (context)
    {
        context->SetModule(prevModule);
    }
}

void ModulePtr::Reset()
{
    if (context)
    {
        context->SetModule(prevModule);
        context = nullptr;
    }
}

} // namespace otava::symbols
