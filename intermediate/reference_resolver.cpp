// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.reference_resolver;

import otava.intermediate.visitor;
import otava.intermediate.code;
import otava.intermediate.context;
import otava.intermediate.error;

namespace otava::intermediate {

class ResolverVisitor : public Visitor
{
public:
    ResolverVisitor(IntermediateContext* context_);
    void Visit(Function& function) override;
    void Visit(BasicBlock& basicBlock) override;
    void Visit(JmpInstruction& inst) override;
    void Visit(BranchInstruction& inst) override;
    void Visit(SwitchInstruction& inst) override;
private:
    Function* currentFunction;
};

ResolverVisitor::ResolverVisitor(IntermediateContext* context_) : Visitor(context_), currentFunction(nullptr)
{
}

void ResolverVisitor::Visit(Function& function)
{
    currentFunction = &function;
    function.VisitBasicBlocks(*this);
}

void ResolverVisitor::Visit(BasicBlock& basicBlock)
{
    basicBlock.VisitInstructions(*this);
}

void ResolverVisitor::Visit(JmpInstruction& inst)
{
    std::int32_t target = inst.TargetLabelId();
    BasicBlock* basicBlock = inst.Parent();
    Function* function = basicBlock->Parent();
    BasicBlock* targetBasicBlock = function->GetBasicBlock(target);
    if (!targetBasicBlock)
    {
        Error("resolve error: jump target basic block " + std::to_string(target) + " not found from function '" + function->Name() + "'", inst.Span(), GetContext());
    }
    inst.SetTargetBasicBlock(targetBasicBlock);
}

void ResolverVisitor::Visit(BranchInstruction& inst)
{
    BasicBlock* trueBasicBlock = inst.Parent();
    std::int32_t trueTarget = inst.TrueTargetLabelId();
    Function* trueFunction = trueBasicBlock->Parent();
    BasicBlock* trueTargetBasicBlock = trueFunction->GetBasicBlock(trueTarget);
    if (!trueTargetBasicBlock)
    {
        Error("resolve error: branch true target basic block " + std::to_string(trueTarget) + " not found from function '" + trueFunction->Name() + "'",
            inst.Span(), GetContext());
    }
    inst.SetTrueTargetBasicBlock(trueTargetBasicBlock);
    std::int32_t falseTarget = inst.FalseTargetLabelId();
    BasicBlock* falseBasicBlock = inst.Parent();
    Function* falseFunction = falseBasicBlock->Parent();
    BasicBlock* falseTargetBasicBlock = falseFunction->GetBasicBlock(falseTarget);
    if (!falseTargetBasicBlock)
    {
        Error("resolve error: branch false target basic block " + std::to_string(falseTarget) + " not found from function '" + falseFunction->Name() + "'",
            inst.Span(), GetContext());
    }
    inst.SetFalseTargetBasicBlock(falseTargetBasicBlock);
}

void ResolverVisitor::Visit(SwitchInstruction& inst)
{
    std::int32_t defaultTarget = inst.DefaultTargetLabelId();
    BasicBlock* parent = inst.Parent();
    Function* function = parent->Parent();
    BasicBlock* defaultTargetBlock = function->GetBasicBlock(defaultTarget);
    if (!defaultTargetBlock)
    {
        Error("resolve error: switch default target basic block " + std::to_string(defaultTarget) +
            " not found from function '" + function->Name() + "'", inst.Span(), GetContext());
    }
    inst.SetDefaultTargetBlock(defaultTargetBlock);
    for (CaseTarget& caseTarget : inst.CaseTargets())
    {
        BasicBlock* caseTargetBlock = function->GetBasicBlock(caseTarget.targetLabelId);
        if (!caseTargetBlock)
        {
            Error("resolve error: switch case target basic block " + std::to_string(caseTarget.targetLabelId) +
                " not found from function '" + function->Name() + "'", inst.Span(), GetContext());
        }
        caseTarget.targetBlock = caseTargetBlock;
    }
}

void ResolveReferences(IntermediateContext& context)
{
    ResolverVisitor visitor(&context);
    context.GetCode().VisitFunctions(visitor);
}

} // otava::intermediate
