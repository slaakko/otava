// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.compile_unit;

import std;

export namespace otava::intermediate {

class IntermediateContext;
class MetadataRef;

class CompileUnit
{
public:
    CompileUnit();
    inline IntermediateContext* GetContext() const noexcept { return context; }
    inline void SetContext(IntermediateContext* context_) noexcept { context = context_; }
    inline const std::string& FilePath() const noexcept { return filePath; }
    void SetFilePath(const std::string& filePath_);
    inline const std::string& Id() const noexcept { return id; }
    void SetInfo(const std::string& id_, MetadataRef* metadataRef_);
    inline MetadataRef* GetMetadataRef() const noexcept { return metadataRef; }
    void Write();
private:
    IntermediateContext* context;
    std::string filePath;
    std::string id;
    MetadataRef* metadataRef;
};

} // otava::intermediate
