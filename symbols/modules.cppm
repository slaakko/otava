// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.modules;

import otava.symbols.id;
import otava.symbols.symbol;
import otava.symbols.string_table;
import otava.symbols.symbol_table;
import otava.symbols.symbol_index_map;
import otava.ast.file;
import util.file_mapping;
import std;

export namespace otava::symbols {

std::string MakeModuleDirPath(const std::string& root, const std::string& config, int optLevel, const std::set<std::string>& configurations);
std::string MakeModuleFilePath(const std::string& root, const std::string& config, int optLevel, const std::string& moduleName, 
    const std::set<std::string>& configurations);

class Context;

struct ModuleHeader
{
    ModuleHeader();
    void Write(Writer& writer);
    void Read(Reader& reader);
    Length length;
    StringOffset nameOffset;
    ModuleId id;
    std::vector<StringOffset> exportedModuleNames;
    std::vector<StringOffset> importedModuleNames;
    std::vector<StringOffset> implementationUnitNames;
    FileOffset stringTableOffset;
    Length stringTableLength;
    FileOffset symbolTableOffset;
    Length symbolTableLength;
    FileOffset symbolIndexMapOffset;
    Length symbolIndexMapLength;
    FileOffset exportedSymbolsOffset;
    Length exportedSymbolsLength;
    FileOffset conversionTableOffset;
    Length conversionTableLength;
    FileOffset symbolIdVectorOffset;
    Length symbolIdVectorLength;
    FileOffset fundamentalTypeTableOffset;
    Length fundamentalTypeTableLength;
};

enum class ModuleKind : std::uint8_t
{
    none, interfaceModule, implementationModule
};

class Module
{
public:
    Module(util::FileMapping* fileMapping_);
    Module(const std::string& name_);
    void Init(Context* context);
    inline void SetKind(ModuleKind kind_) noexcept { kind = kind_; }
    inline ModuleKind Kind() const noexcept { return kind; }
    inline bool IsReadOnly() const noexcept { return symbolTable.IsReadOnly(); }
    std::string Name() const;
    inline const std::string& FilePath() const noexcept { return filePath; }
    void SetFilePath(const std::string& filePath_) { filePath = filePath_; }
    util::FileMapping* GetFileMapping();
    inline StringTable* GetStringTable() const noexcept { return const_cast<StringTable*>(&stringTable); }
    inline SymbolTable* GetSymbolTable() const noexcept { return const_cast<SymbolTable*>(&symbolTable); }
    inline FileOffset GetStringTableOffset() const noexcept { return header.stringTableOffset; }
    inline Length GetStringTableLength() const noexcept { return header.stringTableLength; }
    inline FileOffset GetSymbolTableOffset() const noexcept { return header.symbolTableOffset; }
    inline Length GetSymbolTableLength() const noexcept { return header.symbolTableLength; }
    inline FileOffset GetSymbolIndexMapOffset() const noexcept { return header.symbolIndexMapOffset; }
    inline Length GetSymbolIndexMapLength() const noexcept { return header.symbolIndexMapLength; }
    inline FileOffset GetConversionTableOffset() const noexcept { return header.conversionTableOffset; }
    inline Length GetConversionTableLength() const noexcept { return header.conversionTableLength; }
    inline FileOffset GetSymbolIdVectorOffset() const noexcept { return header.symbolIdVectorOffset; }
    inline Length GetSymbolIdVectorLength() const noexcept { return header.symbolIdVectorLength; }
    inline FileOffset GetFundamentalTypeTableOffset() const noexcept { return header.fundamentalTypeTableOffset; }
    inline Length GetFundamentalTypeTableLength() const noexcept { return header.fundamentalTypeTableLength; }
    inline std::int32_t FileId() const noexcept { return fileId; }
    inline void SetFileId(std::int32_t fileId_) noexcept { fileId = fileId_; }
    inline ModuleId Id() const noexcept { return id; }
    inline void SetId(ModuleId id_) noexcept { id = id_; }
    inline Index GetIndex() const noexcept { return index; }
    inline void SetIndex(Index index_) noexcept { index = index_; }
    inline Index ImportIndex() const noexcept { return importIndex; }
    inline void SetImportIndex(Index importIndex_) noexcept { importIndex = importIndex_; }
    void AddExportedModuleName(const std::string& exportModuleName);
    inline Cardinality ExportedModuleNameCount() const noexcept { return Cardinality(header.exportedModuleNames.size()); }
    std::string GetExportedModuleName(Index index) const;
    void AddExportedModule(Module* exportedModule);
    inline const std::vector<Module*>& ExportedModules() const noexcept { return exportedModules; }
    void AddImportedModuleName(const std::string& importModuleName);
    inline Cardinality ImportedModuleNameCount() const noexcept { return Cardinality(header.importedModuleNames.size()); }
    std::string GetImportedModuleName(Index index) const;
    void AddImportedModule(Module* importedModule);
    inline const std::vector<Module*>& ImportedModules() const noexcept { return importedModules; }
    void AddDependsOnModule(Module* dependsOnModule);
    inline const std::vector<Module*>& DependsOnModules() const noexcept { return dependsOnModules; }
    void Load(const std::string& config, int optLevel, const std::set<std::string>& configurations, Context* context);
    void Write(const std::string& root, const std::string& config, int optLevel, Context* context, const std::set<std::string>& configurations);
    void Write(Writer& writer);
    void ReadExportedSymbols();
    inline const SymbolIndexMap& GetSymbolIndexMap() const noexcept { return symbolIndexMap; }
    void SetFile(otava::ast::File* astFile_);
    void SetImplementationUnitNames(const std::vector<std::string>& names);
private:
    ModuleKind kind;
    ModuleId id;
    StringTable stringTable;
    StringOffset nameOffset;
    SymbolTable symbolTable;
    SymbolIndexMap symbolIndexMap;
    std::string filePath;
    std::unique_ptr<util::FileMapping> fileMapping;
    ModuleHeader header;
    bool headerRead;
    bool exportedSymbolsRead;
    std::int32_t fileId;
    Index index;
    Index importIndex;
    std::vector<Module*> exportedModules;
    std::vector<Module*> importedModules;
    std::vector<Module*> dependsOnModules;
    std::unique_ptr<otava::ast::File> astFile;
    void Read();
};

class ModuleMapper
{
public:
    ModuleMapper();
    void AddRoot(const std::string& root);
    Module* GetModule(const std::string& moduleName, const std::string& config, int optLevel, const std::set<std::string>& configurations, Context* context);
    Module* LoadModule(const std::string& moduleName, const std::string& config, int optLevel, const std::set<std::string>& configurations, Context* context);
    void MapModule(Module* module);
    Module* GetModule(ModuleId moduleId) const noexcept;
    inline SymbolIndexMap* GetSymbolIndexMap() const noexcept { return const_cast<SymbolIndexMap*>(&symbolIndexMap); }
    std::string GetProjectFilePath(const std::string& moduleName) const;
    inline Cardinality ModuleCount() const noexcept { return moduleCount; }
    inline void SetModuleCount(Cardinality moduleCount_) noexcept { moduleCount = moduleCount_; }
    inline ModuleId GetNextModuleId() noexcept { return nextModuleId++; }
    inline ModuleId NextModuleId() noexcept { return nextModuleId;  }
    inline void SetNextModuleId(ModuleId moduleId) noexcept { nextModuleId = moduleId; }
private:
    std::vector<std::string> roots;
    std::vector<std::unique_ptr<Module>> modules;
    std::unordered_map<std::string, Module*> moduleNameMap;
    std::unordered_map<ModuleId, Module*> moduleIdMap;
    SymbolIndexMap symbolIndexMap;
    Cardinality moduleCount;
    ModuleId nextModuleId;
};

} // namespace otava::symbols
