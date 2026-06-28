// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.modules;

import otava.symbols.id;
import otava.symbols.ast_node_io;
import otava.symbols.symbol;
import otava.symbols.string_table;
import otava.symbols.symbol_table;
import otava.symbols.section;
import otava.symbols.symbol_index_map;
import otava.symbols.value;
import otava.ast.file;
import otava.ast.node;
import otava.ast.node_map;
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
    FileOffset namespaceIdTableOffset;
    Length namespaceIdTableLength;
    FileOffset symbolTableOffset;
    Length symbolTableLength;
    FileOffset evaluationContextOffset;
    Length evaluationContextLength;
    std::vector<SectionHeader> sectionHeaders;
    FileOffset symbolIndexMapOffset;
    Length symbolIndexMapLength;
    FileOffset importedSymbolsOffset;
    Length importedSymbolsLength;
    FileOffset conversionTableOffset;
    Length conversionTableLength;
    FileOffset symbolIdVectorOffset;
    Length symbolIdVectorLength;
    FileOffset fundamentalTypeTableOffset;
    Length fundamentalTypeTableLength;
    FileOffset compoundTypeMapOffset;
    Length compoundTypeMapLength;
    FileOffset aliasTypeTemplateMapOffset;
    Length aliasTypeTemplateMapLength;
    FileOffset classTemplateSpecializationMapOffset;
    Length classTemplateSpecializationMapLength;
    FileOffset explicitInstantiationMapOffset;
    Length explicitInstantiationMapLength;
    FileOffset functionTypeMapOffset;
    Length functionTypeMapLength;
    FileOffset astNodeHeaderOffset;
    Length astNodeHeaderLength;
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
    ~Module();
    void AddFunction(FunctionSymbol* fn);
    void RemoveFunction(FunctionSymbol* fn);
    void AddScope(Scope* scope);
    void RemoveScope(Scope* scope);
    void Init(Context* context);
    inline void SetKind(ModuleKind kind_) noexcept { kind = kind_; }
    inline ModuleKind Kind() const noexcept { return kind; }
    void SetInterfaceUnitName(const std::string& interfaceUnitName);
    std::string InterfaceUnitName();
    inline bool IsReadOnly() const noexcept { return symbolTable.IsReadOnly(); }
    std::string Name();
    inline const std::string& FilePath() const noexcept { return filePath; }
    void SetFilePath(const std::string& filePath_) { filePath = filePath_; }
    util::FileMapping* GetFileMapping();
    inline StringTable* GetStringTable() const noexcept { return const_cast<StringTable*>(&stringTable); }
    inline SymbolTable* GetSymbolTable() const noexcept { return const_cast<SymbolTable*>(&symbolTable); }
    inline EvaluationContext* GetEvaluationContext() const noexcept { return const_cast<EvaluationContext*>(&evaluationContext); }
    inline FileOffset GetStringTableOffset() const noexcept { return header.stringTableOffset; }
    inline Length GetStringTableLength() const noexcept { return header.stringTableLength; }
    inline FileOffset GetSymbolTableOffset() const noexcept { return header.symbolTableOffset; }
    inline Length GetSymbolTableLength() const noexcept { return header.symbolTableLength; }
    inline FileOffset GetEvaluationContextOffset() const noexcept { return header.evaluationContextOffset; }
    inline Length GetEvaluationContextLength() const noexcept { return header.evaluationContextLength; }
    SectionHeader* GetSectionHeader(SectionKind sectionKind) noexcept { return &header.sectionHeaders[sectionKind - SectionKind::first]; }
    inline FileOffset GetSymbolIndexMapOffset() const noexcept { return header.symbolIndexMapOffset; }
    inline Length GetSymbolIndexMapLength() const noexcept { return header.symbolIndexMapLength; }
    inline FileOffset GetConversionTableOffset() const noexcept { return header.conversionTableOffset; }
    inline Length GetConversionTableLength() const noexcept { return header.conversionTableLength; }
    inline FileOffset GetSymbolIdVectorOffset() const noexcept { return header.symbolIdVectorOffset; }
    inline Length GetSymbolIdVectorLength() const noexcept { return header.symbolIdVectorLength; }
    inline FileOffset GetFundamentalTypeTableOffset() const noexcept { return header.fundamentalTypeTableOffset; }
    inline Length GetFundamentalTypeTableLength() const noexcept { return header.fundamentalTypeTableLength; }
    inline FileOffset GetCompoundTypeMapOffset() const noexcept { return header.compoundTypeMapOffset; }
    inline Length GetCompoundTypeMapLength() const noexcept { return header.compoundTypeMapLength; }
    inline FileOffset GetAliasTypeTemplateMapOffset() const noexcept { return header.aliasTypeTemplateMapOffset; }
    inline Length GetAliasTypeTemplateMapLength() const noexcept { return header.aliasTypeTemplateMapLength; }
    inline FileOffset GetClassTemplateSpecializationMapOffset() const noexcept { return header.classTemplateSpecializationMapOffset; }
    inline Length GetClassTemplateSpecializationMapLength() const noexcept { return header.classTemplateSpecializationMapLength; }
    inline FileOffset GetExplicitInstantiationMapOffset() const noexcept { return header.explicitInstantiationMapOffset; }
    inline Length GetExplicitInstantiationMapLength() const noexcept { return header.explicitInstantiationMapLength; }
    inline FileOffset GetFunctionTypeMapOffset() const noexcept { return header.functionTypeMapOffset; }
    inline Length GetFunctionTypeMapLength() const noexcept { return header.functionTypeMapLength; }
    inline FileOffset GetAstNodeHeaderOffset() const noexcept { return header.astNodeHeaderOffset; }
    inline Length GetAstNodeHeaderLength() const noexcept { return header.astNodeHeaderLength; }
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
    std::string GetExportedModuleName(Index index);
    std::vector<Module*> ExportedModules(Context* context);
    void AddImportedModuleName(const std::string& importModuleName);
    inline Cardinality ImportedModuleNameCount() const noexcept { return Cardinality(header.importedModuleNames.size()); }
    std::string GetImportedModuleName(Index index);
    std::vector<Module*> ImportedModules(Context* context);
    std::vector<Module*> ImportExportModules(Context* context);
    void AddDependsOnModule(Module* dependsOnModule);
    inline const std::vector<Module*>& DependsOnModules() const noexcept { return dependsOnModules; }
    void Write(const std::string& root, const std::string& config, int optLevel, Context* context, const std::set<std::string>& configurations);
    void Write(Writer& writer);
    void WriteImportedSymbols(Writer& writer);
    void ReadImportedSymbols();
    inline const SymbolIndexMap& GetSymbolIndexMap() const noexcept { return symbolIndexMap; }
    inline void ResetNode(otava::ast::Node* astNode_) noexcept { astNode.reset(astNode_); }
    void ReadAstNode();
    inline otava::ast::NodeMap* GetAstNodeMap() const noexcept { return const_cast<otava::ast::NodeMap*>(&astNodeMap); }
    otava::ast::Node* GetAstNode(std::int64_t astNodeId) const;
    void SetImplementationUnitNames(const std::vector<std::string>& names);
    void GetAllExportedModules(Context* context);
    const std::vector<Module*>& AllExportedModules(Context* context);
    void GetAllImportedModules(Context* context);
    const std::vector<Module*>& AllImportedModules(Context* context);
    inline otava::ast::NodeIdFactory* GetNodeIdFactory() noexcept { return &nodeIdFactory; }
    void AddFunctionSymbol(FunctionSymbol* fn);
    void RemoveFunctionSymbol(FunctionSymbol* fn);
    void AddNamespaceId(SymbolId namespaceId);
    void WriteNamespaceIdTable(Writer& writer);
    void ReadNamespaceIdTable();
    void ReadNamespaceIdTable(Reader& reader);
    const std::vector<SymbolId>& NamespaceIds();
private:
    ModuleKind kind;
    ModuleId id;
    StringTable stringTable;
    StringOffset nameOffset;
    const char* name;
    StringOffset interfaceUnitNameOffset;
    const char* interfaceUnitName;
    SymbolTable symbolTable;
    EvaluationContext evaluationContext;
    SymbolIndexMap symbolIndexMap;
    std::string filePath;
    std::unique_ptr<util::FileMapping> fileMapping;
    ModuleHeader header;
    bool headerRead;
    bool importedSymbolsRead;
    bool astNodeRead;
    std::int32_t fileId;
    Index index;
    Index importIndex;
    std::vector<Module*> allExportedModules;
    bool exportedModulesAdded;
    std::vector<Module*> allImportedModules;
    bool importedModulesAdded;
    std::vector<Module*> dependsOnModules;
    AstNodeHeader astNodeHeader;
    std::unique_ptr<otava::ast::Node> astNode;
    otava::ast::NodeIdFactory nodeIdFactory;
    otava::ast::NodeMap astNodeMap;
    std::vector<FunctionSymbol*> fns;
    std::vector<Scope*> scopes;
    std::vector<SymbolId> namespaceIds;
    bool namespaceIdsRead;
    bool destructing;
    void Read();
    void GetAllExportedModules(std::vector<Module*>& targetExportedModules, Context* context);
    void GetAllImportedModules(std::vector<Module*>& targetImportedModules, Context* context);
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
    void RemoveModule(Module* module);
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
