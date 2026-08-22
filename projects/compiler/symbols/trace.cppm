// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.trace;

import otava.ast.node;
import util.binary_stream_writer;
import util.binary_stream_reader;
import std;

export namespace otava::symbols {

class FunctionDefinitionSymbol;
class Context;

struct SourceFileInfo
{
    SourceFileInfo();
    SourceFileInfo(std::int32_t moduleId, std::int32_t sourceFileIndex, const std::string& sourceFilePath_);
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
    std::int64_t sourceFileId;
    std::string sourceFilePath;
};

struct FunctionTraceInfo
{
    FunctionTraceInfo();
    FunctionTraceInfo(std::int64_t sourceFileId_, std::int32_t moduleId, std::int32_t functionIndex, const std::string& functionFullName_);
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
    std::int64_t sourceFileId;
    std::int64_t functionId;
    std::string functionFullName;
};

class TraceInfo
{
public:
    TraceInfo();
    void AddSourceFileInfo(std::int32_t moduleId, const std::string& sourceFilePath);
    std::int64_t GetSourceFileId(const std::string& sourceFilePath) const noexcept;
    void AddFunctionTraceInfo(std::int64_t sourceFileId, std::int32_t moduleId, const std::string& functionFullName);
    inline int NumSourceFileInfos() const noexcept { return sourceFileInfos.size(); }
    std::int64_t GetFunctionId(const std::string& functionFullName) const noexcept;
    SourceFileInfo* ReleaseSourceFileInfo(int index);
    inline int NumFunctionTraceInfos() const noexcept { return functionTraceInfos.size(); }
    FunctionTraceInfo* ReleaseFunctionTraceInfo(int index);
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
private:
    std::map<std::string, SourceFileInfo*> sourceFileInfoMap;
    std::vector<std::unique_ptr<SourceFileInfo>> sourceFileInfos;
    std::map<std::string, FunctionTraceInfo*> functionTraceInfoMap;
    std::vector<std::unique_ptr<FunctionTraceInfo>> functionTraceInfos;
};

class TraceBin
{
public:
    TraceBin();
    void Import(TraceInfo& traceInfo);
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
    SourceFileInfo* GetSourceFileInfo(std::int64_t sourceFileId) const noexcept;
    FunctionTraceInfo* GetFunctionTraceInfo(std::int64_t functionId) const noexcept;
private:
    std::vector<std::unique_ptr<SourceFileInfo>> sourceFileInfos;
    std::map<std::int64_t, SourceFileInfo*> sourceFileInfosMap;
    std::vector<std::unique_ptr<FunctionTraceInfo>> functionTraceInfos;
    std::map<std::int64_t, FunctionTraceInfo*> functionTraceInfoMap;
};

void WriteTraceBin(TraceBin& traceBin, const std::string& traceBinPath);

void GenerateEnterFunctionCode(otava::ast::Node* functionDefinitionNode, FunctionDefinitionSymbol* fn, Context* context);

} // namespace otava::symbols
