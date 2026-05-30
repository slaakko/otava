// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.assembly.function;

import otava.assembly.instruction;
import otava.assembly.literal;
import otava.assembly.error;
import otava.assembly.context;

namespace otava::assembly {

FunctionDeclaration::FunctionDeclaration(const std::string& name_) : Declaration(name_)
{
}

void FunctionDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("EXTRN " + Name() + ":PROC");
}

LinkOnceDeclaration::LinkOnceDeclaration(const std::string& name_) : Declaration(name_)
{
}

void LinkOnceDeclaration::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("LINK_ONCE " + Name());
}

Function::Function(const std::string& name_) : name(name_), activeFunctionPart(FunctionPart::body), body(this)
{
}

void Function::AddInstruction(Instruction* inst)
{
    switch (activeFunctionPart)
    {
    case FunctionPart::prologue:
    {
        prologue.push_back(std::unique_ptr<Instruction>(inst));
        break;
    }
    case FunctionPart::body:
    {
        body.AddChild(inst);
        break;
    }
    case FunctionPart::epilogue:
    {
        epilogue.push_back(std::unique_ptr<Instruction>(inst));
        break;
    }
    }
}

void Function::SetActiveFunctionPart(FunctionPart activeFunctionPart_)
{
    activeFunctionPart = activeFunctionPart_;
}

void Function::SetComment(const std::string& comment_)
{
    comment = comment_;
}

Instruction* Function::FirstInstruction() const noexcept
{
    return static_cast<Instruction*>(body.FirstChild());
}

Instruction* Function::LastInstruction() const noexcept
{
    return static_cast<Instruction*>(body.LastChild());
}

void Function::Write(util::CodeFormatter& formatter)
{
    for (const auto& macro : macros)
    {
        formatter.WriteLine(macro->ToString());
    }
    if (!macros.empty())
    {
        formatter.WriteLine();
    }
    if (!comment.empty())
    {
        std::vector<std::string> commentLines;
        while (comment.length() > maxAssemblyLineLength - 2)
        {
            int splitPos = maxAssemblyLineLength - 2;
            for (int i = splitPos; i >= 0; --i)
            {
                if (!std::isalnum(comment[i]))
                {
                    splitPos = i;
                    break;
                }
            }
            commentLines.push_back(comment.substr(0, splitPos));
            comment = comment.substr(splitPos);
        }
        if (!comment.empty())
        {
            commentLines.push_back(comment);
        }
        for (const auto& commentLine : commentLines)
        {
            formatter.WriteLine("; " + commentLine);
        }
        formatter.WriteLine();
    }
    formatter.WriteLine(name + " PROC FRAME");
    formatter.IncIndent();
    for (const auto& instruction : prologue)
    {
        instruction->Write(formatter);
    }
    Instruction* instruction = FirstInstruction();
    while (instruction)
    {
        instruction->Write(formatter);
        instruction = instruction->Next();
    }
    for (const auto& instruction : epilogue)
    {
        instruction->Write(formatter);
    }
    formatter.DecIndent();
    formatter.WriteLine(name + " ENDP");
}

void Function::AddMacro(Macro* macro)
{
    macros.push_back(macro);
    macroMap[macro->Name()] = macro;
}

Macro* Function::GetMacro(const std::string& name) const
{
    auto it = macroMap.find(name);
    if (it != macroMap.end())
    {
        return it->second;
    }
    else
    {
        SetExceptionThrown();
        throw std::runtime_error("macro '" + name + "' not found");
    }
}

} // namespace otava::assembly
