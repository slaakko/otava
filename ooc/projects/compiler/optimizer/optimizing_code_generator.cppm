// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.optimizer.optimizing_code_generator;

import otava.intermediate.code_generator;
import otava.intermediate.compile_unit;
import otava.intermediate.visitor;
import otava.intermediate.code;
import otava.intermediate.context;
import otava.intermediate.types;
import otava.intermediate.value;
import otava.intermediate.data;
import otava.intermediate.metadata;
import otava.intermediate.register_allocator;
import otava.assembly.context;
import otava.assembly.data;
import otava.assembly.file;
import otava.assembly.function;
import otava.assembly.instruction;
import otava.assembly.reg;
import otava.assembly.value;
import otava.assembly.declaration;
import otava.assembly.data;
import otava.assembly.macro;
import otava.assembly.reg;
import soul.lexer;
import soul.ast.span;
import util.component;
import util.code_formatter;
import std;

export namespace otava::optimizer {

const int jumpTableSwitchThreshold = 4;
const int maxJumpTableEntries = 1024;

class OptimizingCodeGenerator : public otava::intermediate::CodeGenerator
{
public:
    OptimizingCodeGenerator(otava::intermediate::IntermediateContext* context_, const std::string& assemblyFilePath_);
    void Emit(otava::assembly::Instruction* assemblyInstruction) override;
    void Visit(otava::intermediate::SwitchInstruction& inst) override;
    void Visit(otava::intermediate::JmpInstruction& inst) override;
    void Visit(otava::intermediate::RetInstruction& inst) override;
    void Visit(otava::intermediate::NoOperationInstruction& inst) override;
    int ExitLabelId() const noexcept override;
    void EmitJumpToExit(otava::intermediate::RetInstruction& retInst) override;
    void EmitBranchJumps(otava::intermediate::BranchInstruction& branchInst) override;
private:
    void EmitXorInst(const std::string& label, otava::assembly::Register* reg);
};

} // otava::optimizer
