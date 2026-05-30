// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.assembly.data;

import otava.assembly.instruction;

namespace otava::assembly {

Data::Data()
{
}

void Data::AddInstruction(std::unique_ptr<Instruction>&& instruction)
{
    if (instruction->HasOperands())
    {
        instructions.push_back(std::move(instruction));
    }
}

void Data::Write(util::CodeFormatter& formatter)
{
    for (const auto& instruction : instructions)
    {
        instruction->Write(formatter);
    }
}

PublicDeclaration::PublicDeclaration(const std::string& name_) : Declaration(name_)
{
}

void PublicDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.Write("PUBLIC ");
    formatter.WriteLine(Name());
}

ExternalDataDeclaration::ExternalDataDeclaration(const std::string& name_) : Declaration(name_)
{
}

void ExternalDataDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.Write("EXTRN ");
    formatter.Write(Name());
    formatter.WriteLine(":BYTE");
}

} // otava::assembly
