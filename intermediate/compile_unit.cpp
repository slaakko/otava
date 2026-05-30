// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.compile_unit;

import otava.intermediate.context;
import util.code_formatter;

namespace otava::intermediate {

CompileUnit::CompileUnit() : context(nullptr), id(), metadataRef()
{
}

void CompileUnit::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

void CompileUnit::SetInfo(const std::string& id_, MetadataRef* metadataRef_)
{
    id = id_;
    metadataRef = metadataRef_;
}

void CompileUnit::Write()
{
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(8);
    context->GetTypes().Write(formatter);
    context->GetData().Write(formatter);
    bool first = true;
    Function* fn = context->GetCode().FirstFunction();
    while (fn)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.WriteLine();
        }
        fn->Write(formatter);
        fn = fn->Next();
    }
    context->GetMetadata().Write(formatter);
}

} // otava::intermediate
