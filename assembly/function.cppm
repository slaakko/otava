// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.assembly.function;

import otava.assembly.declaration;
import otava.assembly.macro;
import util.code_formatter;
import util.component;
import std;

export namespace otava::assembly {

class FunctionDeclaration : public Declaration
{
public:
    FunctionDeclaration(const std::string& name_);
    void Write(util::CodeFormatter& formatter) override;
};

class LinkOnceDeclaration : public Declaration
{
public:
    LinkOnceDeclaration(const std::string& name_);
    void Write(util::CodeFormatter& formatter) override;
};

class Instruction;

enum class FunctionPart
{
    prologue, body, epilogue
};

class Function : public util::Component
{
public:
    Function(const std::string& name_);
    inline const std::string& Name() const noexcept { return name; }
    void SetActiveFunctionPart(FunctionPart activeFunctionPart_);
    void AddInstruction(Instruction* inst);
    void Write(util::CodeFormatter& formatter);
    void AddMacro(Macro* macro);
    Macro* GetMacro(const std::string& name) const;
    inline const std::string& Comment() const noexcept { return comment; }
    void SetComment(const std::string& comment_);
    Instruction* FirstInstruction() const noexcept;
    Instruction* LastInstruction() const noexcept;
private:
    std::string name;
    FunctionPart activeFunctionPart;
    std::vector<std::unique_ptr<Instruction>> prologue;
    util::Container body;
    std::vector<std::unique_ptr<Instruction>> epilogue;
    std::vector<Macro*> macros;
    std::map<std::string, Macro*> macroMap;
    std::string comment;
};

} // namespace otava::assembly
