// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module ort.trace;

import util;

namespace trace {

    TraceEntry* traceList = nullptr;

    SourceFileInfo::SourceFileInfo()
    {
    }

    SourceFileInfo::SourceFileInfo(std::int32_t moduleId, std::int32_t sourceFileIndex, const std::string& sourceFilePath_)
    {
    }

    void SourceFileInfo::Read(util::BinaryStreamReader& reader)
    {
        sourceFileId = reader.ReadLong();
        sourceFilePath = reader.ReadUtf8String();
    }

    FunctionTraceInfo::FunctionTraceInfo()
    {
    }

    FunctionTraceInfo::FunctionTraceInfo(std::int64_t sourceFileId_, std::int32_t moduleId, std::int32_t functionIndex, const std::string& functionFullName_)
    {
    }

    void FunctionTraceInfo::Read(util::BinaryStreamReader& reader)
    {
        sourceFileId = reader.ReadLong();
        functionId = reader.ReadLong();
        functionFullName = reader.ReadUtf8String();
    }

    TraceBin::TraceBin()
    {
    }

    SourceFileInfo* TraceBin::GetSourceFileInfo(std::int64_t sourceFileId) const
    {
        auto it = sourceFileInfosMap.find(sourceFileId);
        if (it != sourceFileInfosMap.end())
        {
            return it->second;
        }
        else
        {
            return nullptr;
        }
    }

    FunctionTraceInfo* TraceBin::GetFunctionTraceInfo(std::int64_t functionId) const
    {
        auto it = functionTraceInfoMap.find(functionId);
        if (it != functionTraceInfoMap.end())
        {
            return it->second;
        }
        else
        {
            return nullptr;
        }
    }

    void TraceBin::Read(util::BinaryStreamReader& reader)
    {
        std::int32_t ns = reader.ReadInt();
        for (std::int32_t i = 0; i < ns; ++i)
        {
            SourceFileInfo* sourceFileInfo = new SourceFileInfo();
            sourceFileInfo->Read(reader);
            sourceFileInfos.push_back(std::unique_ptr<SourceFileInfo>(sourceFileInfo));
            sourceFileInfosMap[sourceFileInfo->sourceFileId] = sourceFileInfo;
        }
        std::int32_t nf = reader.ReadInt();
        for (std::int32_t i = 0; i < nf; ++i)
        {
            FunctionTraceInfo* functionTraceInfo = new FunctionTraceInfo();
            functionTraceInfo->Read(reader);
            functionTraceInfos.push_back(std::unique_ptr<FunctionTraceInfo>(functionTraceInfo));
            functionTraceInfoMap[functionTraceInfo->functionId] = functionTraceInfo;
        }
    }

    std::unique_ptr<TraceBin> traceBin;

    void read_trace_bin()
    {
        if (traceBin) return;
        traceBin.reset(new TraceBin());
        std::string currentExecutableName = util::Path::GetFileName(util::GetFullPath(util::GetPathToExecutable()));
        util::BinaryResourcePtr resource(currentExecutableName, "TRACE_DATA");
        util::MemoryStream memoryStream(resource.Data(), resource.Size());
        util::BinaryStreamReader reader(memoryStream);
        traceBin->Read(reader);
    }

    struct FunctionLine
    {
        FunctionLine(std::int64_t fn_, std::int32_t line_) : fn(fn_), line(line_) {}
        std::int64_t fn;
        std::int32_t line;
    };

    class StackTrace
    {
    public:
        StackTrace();
        bool IsEmpty() const { return fnLines.empty(); }
        void AddFunctionLine(const FunctionLine& fnLine);
        const char* GetTraceStr();
    private:
        std::vector<FunctionLine> fnLines;
        std::string traceStr;
    };

    StackTrace::StackTrace()
    {
    }

    void StackTrace::AddFunctionLine(const FunctionLine& fnLine)
    {
        fnLines.push_back(fnLine);
    }

    const char* StackTrace::GetTraceStr()
    {
        try
        {
            read_trace_bin();
            if (traceStr.empty())
            {
                traceStr.append("STACK TRACE:\n");
                for (const auto& fnLine : fnLines)
                {
                    FunctionTraceInfo* functionTraceInfo = traceBin->GetFunctionTraceInfo(fnLine.fn);
                    if (functionTraceInfo)
                    {
                        std::string entryStr = functionTraceInfo->functionFullName;
                        std::int64_t sourceFileId = functionTraceInfo->sourceFileId;
                        SourceFileInfo* sourceFileInfo = traceBin->GetSourceFileInfo(sourceFileId);
                        if (sourceFileInfo)
                        {
                            entryStr.append(" ").append(sourceFileInfo->sourceFilePath).append(":").append(std::to_string(fnLine.line));
                        }
                        traceStr.append(entryStr).append("\n");
                    }
                }
            }
        }
        catch (const std::exception& ex)
        {
            traceStr.append("internal error: could not read runtime_info for generating stack trace: " + util::PlatformStringToUtf8(ex.what()));
        }
        return traceStr.c_str();
    }

    class Traces
    {
    public:
        Traces();
        static Traces& Instance();
        int GenerateTrace();
        const char* GetStackTrace(int traceId) const;
        void FreeTrace(int traceId);
    private:
        std::vector<std::unique_ptr<StackTrace>> traces;
    };

    Traces::Traces()
    {
    }

    Traces& Traces::Instance()
    {
        static Traces instance;
        return instance;
    }

    int Traces::GenerateTrace()
    {
        if (!traceList) return -1;
        int traceId = traces.size();
        StackTrace* trace = new StackTrace();
        traces.push_back(std::unique_ptr<StackTrace>(trace));
        TraceEntry* entry = traceList;
        while (entry)
        {
            trace->AddFunctionLine(FunctionLine(entry->fn, entry->line));
            entry = entry->next;
        }
        return traceId;
    }

    const char* Traces::GetStackTrace(int traceId) const
    {
        if (traceId >= 0 && traceId < traces.size())
        {
            StackTrace* trace = traces[traceId].get();
            return trace->GetTraceStr();
        }
        else
        {
            return nullptr;
        }
    }

    void Traces::FreeTrace(int traceId)
    {
        if (traceId >= 0 && traceId < traces.size())
        {
            traces[traceId].reset();
        }
    }

} // trace

void ort_push_function(void* traceEntry)
{
    trace::TraceEntry* entry = static_cast<trace::TraceEntry*>(traceEntry);
    entry->next = trace::traceList;
    trace::traceList = entry;
}

void ort_pop_function(void* traceEntry)
{
    trace::TraceEntry* entry = static_cast<trace::TraceEntry*>(traceEntry);
    trace::traceList = entry->next;
}

int ort_generate_stack_trace()
{
    int traceId = trace::Traces::Instance().GenerateTrace();
    return traceId;
}

const char* ort_get_stack_trace(int traceId)
{
    return trace::Traces::Instance().GetStackTrace(traceId);
}

void ort_free_stack_trace(int traceId)
{
    trace::Traces::Instance().FreeTrace(traceId);
}
