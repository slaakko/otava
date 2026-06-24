// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module ort.trace;

import std;
import util;

export namespace trace {

struct TraceEntry
{
    TraceEntry* next;
    std::int64_t fn;
    std::int32_t line;
};

struct SourceFileInfo
{
    SourceFileInfo();
    SourceFileInfo(std::int32_t moduleId, std::int32_t sourceFileIndex, const std::string& sourceFilePath_);
    void Read(util::BinaryStreamReader& reader);
    std::int64_t sourceFileId;
    std::string sourceFilePath;
};

struct FunctionTraceInfo
{
    FunctionTraceInfo();
    FunctionTraceInfo(std::int64_t sourceFileId_, std::int32_t moduleId, std::int32_t functionIndex, const std::string& functionFullName_);
    void Read(util::BinaryStreamReader& reader);
    std::int64_t sourceFileId;
    std::int64_t functionId;
    std::string functionFullName;
};

class TraceBin
{
public:
    TraceBin();
    void Read(util::BinaryStreamReader& reader);
    SourceFileInfo* GetSourceFileInfo(std::int64_t sourceFileId) const;
    FunctionTraceInfo* GetFunctionTraceInfo(std::int64_t functionId) const;
private:
    std::vector<std::unique_ptr<SourceFileInfo>> sourceFileInfos;
    std::map<std::int64_t, SourceFileInfo*> sourceFileInfosMap;
    std::vector<std::unique_ptr<FunctionTraceInfo>> functionTraceInfos;
    std::map<std::int64_t, FunctionTraceInfo*> functionTraceInfoMap;
};

} // namespace trace

export extern "C" void ort_push_function(void* traceEntry);
export extern "C" void ort_pop_function(void* traceEntry);
export extern "C" int ort_generate_stack_trace();
export extern "C" const char* ort_get_stack_trace(int traceId);
export extern "C" void ort_free_stack_trace(int traceId);
