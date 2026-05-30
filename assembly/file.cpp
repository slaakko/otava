// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.assembly.file;

import otava.assembly.function;
import otava.assembly.instruction;
import otava.assembly.data;
import util.sha1;

namespace otava::assembly {

void DeclarationSection::AddFunctionDeclaration(FunctionDeclaration* declaration)
{
    std::unique_ptr<Declaration> declarationPtr(declaration);
    if (externalFunctionDeclarations.find(declaration->Name()) != externalFunctionDeclarations.end()) return;
    externalFunctionDeclarations.insert(declaration->Name());
    declarations.push_back(std::unique_ptr<Declaration>(declarationPtr.release()));
}

void DeclarationSection::AddPublicDeclaration(PublicDeclaration* declaration)
{
    declarations.push_back(std::unique_ptr<Declaration>(declaration));
}

void DeclarationSection::AddLinkOnceDeclaration(LinkOnceDeclaration* declaration)
{
    declarations.push_back(std::unique_ptr<Declaration>(declaration));
}

void DeclarationSection::AddExternalDataDeclaration(ExternalDataDeclaration* declaration)
{
    std::unique_ptr<Declaration> declarationPtr(declaration);
    if (externalDataDeclarations.find(declaration->Name()) != externalDataDeclarations.end()) return;
    externalDataDeclarations.insert(declaration->Name());
    declarations.push_back(std::unique_ptr<Declaration>(declarationPtr.release()));
}

void DeclarationSection::Write(util::CodeFormatter& formatter)
{
    for (const auto& declaration : declarations)
    {
        declaration->Write(formatter);
    }
    formatter.WriteLine();
}

void DataSection::AddData(Data* data)
{
    dataVec.push_back(std::unique_ptr<Data>(data));
}

void DataSection::Write(util::CodeFormatter& formatter)
{
    if (dataVec.empty()) return;
    formatter.WriteLine(".DATA");
    formatter.WriteLine();
    for (const auto& data : dataVec)
    {
        data->Write(formatter);
    }
    formatter.WriteLine();
}

Function* CodeSection::CreateFunction(const std::string& name)
{
    Function* function = new Function(name);
    functions.push_back(std::unique_ptr<Function>(function));
    return function;
}

void CodeSection::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine(".CODE");
    formatter.WriteLine();
    for (const auto& function : functions)
    {
        function->Write(formatter);
        formatter.WriteLine();
    }
}

std::string MakeFileId(const std::string& filePath)
{
    return util::GetSha1MessageDigest(filePath);
}

File::File(const std::string& filePath_) : filePath(filePath_), file(filePath), formatter(file), id(MakeFileId(filePath))
{
    formatter.SetIndentSize(8);
}

void File::Write()
{
    declarationSection.Write(formatter);
    dataSection.Write(formatter);
    codeSection.Write(formatter);
    formatter.WriteLine("END");
}

} // otava::assembly
