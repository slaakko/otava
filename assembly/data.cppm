// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.assembly.data;

import otava.assembly.declaration;
import util.code_formatter;
import std;

export namespace otava::assembly {

class Instruction;

class Data
{
public:
    Data();
    void AddInstruction(std::unique_ptr<Instruction>&& instruction);
    void Write(util::CodeFormatter& formatter);
private:
    std::vector<std::unique_ptr<Instruction>> instructions;
};

class PublicDeclaration : public Declaration
{
public:
    PublicDeclaration(const std::string& name_);
    void Write(util::CodeFormatter& formatter) override;
};

class ExternalDataDeclaration : public Declaration
{
public:
    ExternalDataDeclaration(const std::string& name_);
    void Write(util::CodeFormatter& formatter) override;
};

} // otava::assembly
