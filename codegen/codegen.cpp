// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.codegen;

import otava.codegen.goto_target_map_builder;
import otava.intermediate.code;
import otava.intermediate.code_generator;
import otava.intermediate.main.parser;
import otava.intermediate.verify;
import otava.optimizer;
import otava.optimizer.optimizing_code_generator;
import otava.assembly.data;
import otava.assembly.declaration;
import otava.symbols.bound_tree;
import otava.symbols.bound_tree_visitor;
import otava.symbols.block;
import otava.symbols.classes;
import otava.symbols.emitter;
import otava.symbols.enums;
import otava.symbols.exception;
import otava.symbols.project;
import otava.symbols.stmt_parser;
import otava.symbols.statement_binder;
import otava.symbols.type_resolver;
import otava.symbols.value;
import otava.symbols.variable_symbol;
import otava.ast.identifier;
import util.path;

namespace otava::codegen {

struct SwitchTarget
{
    SwitchTarget(otava::intermediate::BasicBlock* block_, otava::symbols::BoundStatementNode* statement_, const std::vector<otava::symbols::BoundExpressionNode*>& exprs_);
    otava::intermediate::BasicBlock* block;
    otava::symbols::BoundStatementNode* statement;
    std::vector<otava::symbols::BoundExpressionNode*> exprs;
};

SwitchTarget::SwitchTarget(otava::intermediate::BasicBlock* block_, otava::symbols::BoundStatementNode* statement_,
    const std::vector<otava::symbols::BoundExpressionNode*>& exprs_) : block(block_), statement(statement_), exprs(exprs_)
{
}

class SwitchTargets
{
public:
    SwitchTargets();
    SwitchTarget* Default() const { return defaultTarget.get(); }
    void AddCase(SwitchTarget* caseTarget);
    void SetDefault(SwitchTarget* defaultTarget_);
    const std::vector<std::unique_ptr<SwitchTarget>>& Cases() const { return caseTargets; }
private:
    std::unique_ptr<SwitchTarget> defaultTarget;
    std::vector<std::unique_ptr<SwitchTarget>> caseTargets;
};

SwitchTargets::SwitchTargets()
{
}

void SwitchTargets::SetDefault(SwitchTarget* defaultTarget_)
{
    defaultTarget.reset(defaultTarget_);
}

void SwitchTargets::AddCase(SwitchTarget* caseTarget)
{
    caseTargets.push_back(std::unique_ptr<SwitchTarget>(caseTarget));
}

class SwitchTargetCollector : public otava::symbols::DefaultBoundTreeVisitor
{
public:
    SwitchTargetCollector(otava::symbols::Emitter& emitter_);
    std::unique_ptr<SwitchTargets> GetSwitchTargets() { return std::move(switchTargets); }
    void Visit(otava::symbols::BoundCaseStatementNode& node) override;
    void Visit(otava::symbols::BoundDefaultStatementNode& node) override;
private:
    otava::symbols::Emitter& emitter;
    std::unique_ptr<SwitchTargets> switchTargets;
};

SwitchTargetCollector::SwitchTargetCollector(otava::symbols::Emitter& emitter_) : emitter(emitter_), switchTargets(new SwitchTargets())
{
}

void SwitchTargetCollector::Visit(otava::symbols::BoundCaseStatementNode& node)
{
    std::vector<otava::symbols::BoundExpressionNode*> caseExprs;
    for (const auto& caseExpr : node.CaseExprs())
    {
        caseExprs.push_back(caseExpr.get());
    }
    SwitchTarget* caseTarget = new SwitchTarget(emitter.CreateBasicBlock(), node.Statement(), caseExprs);
    switchTargets->AddCase(caseTarget);
}

void SwitchTargetCollector::Visit(otava::symbols::BoundDefaultStatementNode& node)
{
    SwitchTarget* defaultTarget = new SwitchTarget(emitter.CreateBasicBlock(), node.Statement(), std::vector<otava::symbols::BoundExpressionNode*>());
    switchTargets->SetDefault(defaultTarget);
}

std::unique_ptr<SwitchTargets> CollectSwitchTargets(otava::symbols::Emitter& emitter, otava::symbols::BoundStatementNode* statement)
{
    SwitchTargetCollector collector(emitter);
    statement->Accept(collector);
    std::unique_ptr<SwitchTargets> targets = collector.GetSwitchTargets();
    return std::move(targets);
}

class ConstantExpressionEvaluator : public otava::symbols::DefaultBoundTreeVisitor
{
public:
    ConstantExpressionEvaluator(otava::symbols::Emitter& emitter_, const soul::ast::FullSpan& fullSpan_, otava::symbols::Context& context_);
    void Visit(otava::symbols::BoundLiteralNode& node) override;
    void Visit(otava::symbols::BoundEnumConstant& node) override;
    void Visit(otava::symbols::BoundVariableNode& node) override;
    void Visit(otava::symbols::BoundConversionNode& node) override;
private:
    otava::symbols::Emitter& emitter;
    soul::ast::FullSpan fullSpan;
    otava::symbols::Context& context;
};

ConstantExpressionEvaluator::ConstantExpressionEvaluator(otava::symbols::Emitter& emitter_, const soul::ast::FullSpan& fullSpan_, otava::symbols::Context& context_) :
    emitter(emitter_), fullSpan(fullSpan_), context(context_)
{
}

void ConstantExpressionEvaluator::Visit(otava::symbols::BoundLiteralNode& node)
{
    emitter.Stack().Push(node.GetValue()->IrValue(emitter, fullSpan, &context));
}

void ConstantExpressionEvaluator::Visit(otava::symbols::BoundEnumConstant& node)
{
    emitter.Stack().Push(node.EnumConstant()->GetValue(&context)->IrValue(emitter, fullSpan, &context));
}

void ConstantExpressionEvaluator::Visit(otava::symbols::BoundVariableNode& node)
{
    otava::symbols::VariableSymbol* variable = node.GetVariable();
    otava::symbols::Value* value = variable->GetValue(&context);
    if (!value)
    {
        otava::symbols::ThrowException("cannot evaluate statically", fullSpan, &context);
    }
    otava::intermediate::Value* irValue = value->IrValue(emitter, fullSpan, &context);
    emitter.Stack().Push(irValue);
}

void ConstantExpressionEvaluator::Visit(otava::symbols::BoundConversionNode& node)
{
    node.Subject()->Accept(*this);
    otava::intermediate::Value* value = emitter.Stack().Pop();
    if (value->IsIntegerValue())
    {
        std::int64_t val = value->GetIntegerValue();
        otava::intermediate::Type* type = node.GetType()->IrType(emitter, fullSpan, &context);
        if (type->IsIntegerType())
        {
            switch (type->Id())
            {
            case otava::intermediate::sbyteTypeId:
            {
                value = emitter.GetIntermediateContext()->GetSByteValue(static_cast<std::int8_t>(val));
                break;
            }
            case otava::intermediate::byteTypeId:
            {
                value = emitter.GetIntermediateContext()->GetByteValue(static_cast<std::uint8_t>(val));
                break;
            }
            case otava::intermediate::shortTypeId:
            {
                value = emitter.GetIntermediateContext()->GetShortValue(static_cast<std::int16_t>(val));
                break;
            }
            case otava::intermediate::ushortTypeId:
            {
                value = emitter.GetIntermediateContext()->GetUShortValue(static_cast<std::uint16_t>(val));
                break;
            }
            case otava::intermediate::intTypeId:
            {
                value = emitter.GetIntermediateContext()->GetIntValue(static_cast<std::int32_t>(val));
                break;
            }
            case otava::intermediate::uintTypeId:
            {
                value = emitter.GetIntermediateContext()->GetUIntValue(static_cast<std::uint32_t>(val));
                break;
            }
            case otava::intermediate::longTypeId:
            {
                value = emitter.GetIntermediateContext()->GetLongValue(static_cast<std::int64_t>(val));
                break;
            }
            case otava::intermediate::ulongTypeId:
            {
                value = emitter.GetIntermediateContext()->GetULongValue(static_cast<std::uint64_t>(val));
                break;
            }
            default:
            {
                otava::symbols::ThrowException("cannot evaluate statically", fullSpan, &context);
                break;
            }
            }
            emitter.Stack().Push(value);
        }
        else
        {
            otava::symbols::ThrowException("cannot evaluate statically", fullSpan, &context);
        }
    }
    else
    {
        otava::symbols::ThrowException("cannot evaluate statically", fullSpan, &context);
    }
}

void EvaluateConstantExpr(otava::symbols::Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context& context,
    otava::symbols::BoundExpressionNode* constantExpr)
{
    ConstantExpressionEvaluator evaluator(emitter, fullSpan, context);
    constantExpr->Accept(evaluator);
    if (emitter.Stack().IsEmpty())
    {
        otava::symbols::ThrowException("cannot evaluate statically", fullSpan, &context);
    }
}

class BlockExit
{
public:
    BlockExit();
    bool IsEmpty() const { return destructorCalls.empty(); }
    int Size() const { return destructorCalls.size(); }
    void AddDestructorCall(otava::symbols::BoundExpressionNode* destructorCall);
    void Execute(otava::symbols::Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context, bool reset);
private:
    std::vector<std::unique_ptr<otava::symbols::BoundExpressionNode>> destructorCalls;
    std::set<otava::symbols::SymbolId> variableIdSet;
};

BlockExit::BlockExit()
{
}

void BlockExit::AddDestructorCall(otava::symbols::BoundExpressionNode* destructorCall)
{
    if (destructorCall->IsBoundFunctionCallNode())
    {
        otava::symbols::BoundFunctionCallNode* fnCall = static_cast<otava::symbols::BoundFunctionCallNode*>(destructorCall);
        if (!fnCall->Args().empty())
        {
            otava::symbols::BoundExpressionNode* arg = static_cast<otava::symbols::BoundExpressionNode*>(fnCall->Args().front().get());
            if (arg->IsBoundAddressOfNode())
            {
                otava::symbols::BoundAddressOfNode* addr = static_cast<otava::symbols::BoundAddressOfNode*>(arg);
                if (addr->Subject()->IsBoundVariableNode())
                {
                    otava::symbols::BoundVariableNode* var = static_cast<otava::symbols::BoundVariableNode*>(addr->Subject());
                    if (variableIdSet.find(var->GetVariable()->Id()) != variableIdSet.end())
                    {
                        return;
                    }
                    else
                    {
                        variableIdSet.insert(var->GetVariable()->Id());
                    }
                }
                else if (addr->Subject()->IsBoundParentVariableNode())
                {
                    otava::symbols::BoundParentVariableNode* var = static_cast<otava::symbols::BoundParentVariableNode*>(addr->Subject());
                    if (variableIdSet.find(var->GetVariable()->Id()) != variableIdSet.end())
                    {
                        return;
                    }
                    else
                    {
                        variableIdSet.insert(var->GetVariable()->Id());
                    }
                }
            }
        }
    }
    destructorCalls.push_back(std::unique_ptr<otava::symbols::BoundExpressionNode>(destructorCall->Clone()));
}

void BlockExit::Execute(otava::symbols::Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context, bool reset)
{
    int n = destructorCalls.size();
    for (int i = n - 1; i >= 0; --i)
    {
        otava::symbols::BoundExpressionNode* destructorCall = destructorCalls[i].get();
        destructorCall->Load(emitter, otava::symbols::OperationFlags::none, fullSpan, context);
    }
}

class CodeGenerator : public otava::symbols::DefaultBoundTreeVisitor
{
public:
    CodeGenerator(otava::symbols::Context& context_, const std::string& config_, int optLevel_, bool verbose_, std::string& mainIrName_,
        int& mainFunctionParams_, bool globalMain, const std::vector<std::string>& compileUnitInitFnNames_, const std::set<std::string>& configurations_);
    void Reset();
    const std::string& GetAsmFileName() const { return asmFileName; }
    void Visit(otava::symbols::BoundEmptyStatementNode& node) override;
    void Visit(otava::symbols::BoundCompileUnitNode& node) override;
    void Visit(otava::symbols::BoundClassNode& node) override;
    void Visit(otava::symbols::BoundFunctionNode& node) override;
    void Visit(otava::symbols::BoundCompoundStatementNode& node) override;
    void Visit(otava::symbols::BoundIfStatementNode& node) override;
    void Visit(otava::symbols::BoundSwitchStatementNode& node) override;
    void Visit(otava::symbols::BoundCaseStatementNode& node) override;
    void Visit(otava::symbols::BoundDefaultStatementNode& node) override;
    void Visit(otava::symbols::BoundWhileStatementNode& node) override;
    void Visit(otava::symbols::BoundDoStatementNode& node) override;
    void Visit(otava::symbols::BoundForStatementNode& node) override;
    void Visit(otava::symbols::BoundSequenceStatementNode& node) override;
    void Visit(otava::symbols::BoundReturnStatementNode& node) override;
    void Visit(otava::symbols::BoundBreakStatementNode& node) override;
    void Visit(otava::symbols::BoundContinueStatementNode& node) override;
    void Visit(otava::symbols::BoundConstructionStatementNode& node) override;
    void Visit(otava::symbols::BoundExpressionStatementNode& node) override;
    void Visit(otava::symbols::BoundSetVPtrStatementNode& node) override;
    void Visit(otava::symbols::BoundLiteralNode& node) override;
    void Visit(otava::symbols::BoundStringLiteralNode& node) override;
    void Visit(otava::symbols::BoundVariableNode& node) override;
    void Visit(otava::symbols::BoundParentVariableNode& node) override;
    void Visit(otava::symbols::BoundParameterNode& node) override;
    void Visit(otava::symbols::BoundParentParameterNode& node) override;
    void Visit(otava::symbols::BoundEnumConstant& node) override;
    void Visit(otava::symbols::BoundMemberExprNode& node) override;
    void Visit(otava::symbols::BoundFunctionCallNode& node) override;
    void Visit(otava::symbols::BoundFunctionPtrCallNode& node) override;
    void Visit(otava::symbols::BoundInvokeNode& node) override;
    void Visit(otava::symbols::BoundExpressionSequenceNode& node) override;
    void Visit(otava::symbols::BoundConversionNode& node) override;
    void Visit(otava::symbols::BoundAddressOfNode& node) override;
    void Visit(otava::symbols::BoundDereferenceNode& node) override;
    void Visit(otava::symbols::BoundPtrToRefNode& node) override;
    void Visit(otava::symbols::BoundConstructTemporaryNode& node) override;
    void Visit(otava::symbols::BoundConstructExpressionNode& node) override;
    void Visit(otava::symbols::BoundDefaultInitNode& node) override;
    void Visit(otava::symbols::BoundTemporaryNode& node) override;
    void Visit(otava::symbols::BoundConjunctionNode& boundConjunction) override;
    void Visit(otava::symbols::BoundDisjunctionNode& boundDisjunction) override;
    void Visit(otava::symbols::BoundConditionalExprNode& boundConditionalExpr) override;
    void Visit(otava::symbols::BoundGlobalVariableDefinitionNode& node) override;
    void Visit(otava::symbols::BoundGotoStatementNode& node) override;
    void Visit(otava::symbols::BoundLabeledStatementNode& node) override;
private:
    void StatementPrefix();
    void GenJumpingBoolCode();
    void GenerateVTab(otava::symbols::ClassTypeSymbol* cls, const soul::ast::FullSpan& fullSpan);
    void AddClassInfo(otava::symbols::ClassTypeSymbol* cls);
    void EmitReturn(const soul::ast::FullSpan& fullSpan);
    void ExitBlocks(int sourceBlockId, int targetBlockId, const soul::ast::FullSpan& fullSpan);
    void GenerateGlobalInitializationFunction();
    void GenerateGlobalDoneFunction();
    void SetCurrentLineNumber(const soul::ast::FullSpan& fullSpan);
    void SetCurrentBlockSymbol(int blockId);
    void GenerateDestructorCallsForCurrentStatement();
    std::string optimizedIntermediateFilePath;
    otava::symbols::Context& context;
    otava::symbols::Emitter* emitter;
    std::string config;
    std::set<std::string> configurations;
    int optLevel;
    bool verbose;
    int line;
    std::string& mainIrName;
    int& mainFunctionParams;
    otava::symbols::FunctionDefinitionSymbol* functionDefinition;
    otava::intermediate::BasicBlock* entryBlock;
    otava::intermediate::BasicBlock* trueBlock;
    otava::intermediate::BasicBlock* falseBlock;
    otava::intermediate::BasicBlock* defaultBlock;
    otava::intermediate::BasicBlock* breakBlock;
    int breakBlockId;
    otava::intermediate::BasicBlock* continueBlock;
    int continueBlockId;
    int currentBlockId;
    std::vector<std::unique_ptr<BlockExit>> blockExits;
    bool genJumpingBoolCode;
    bool lastInstructionWasRet;
    bool prevWasTerminator;
    bool basicBlockOpen;
    bool destructorCallGenerated;
    bool inLineNumberCode;
    bool emitLineNumbers;
    otava::symbols::BoundFunctionNode* boundFunction;
    otava::symbols::BoundCompoundStatementNode* currentBlock;
    otava::symbols::BlockSymbol* currentBlockSymbol;
    otava::symbols::BoundStatementNode* currentStatement;
    otava::symbols::BoundReturnStatementNode* latestRet;
    otava::symbols::BoundStatementNode* sequenceSecond;
    std::string asmFileName;
    bool globalMain;
    std::vector<std::string> compileUnitInitFnNames;
    std::set<std::string> emittedVTabNames;
    std::vector<std::unique_ptr<otava::symbols::BoundFunctionCallNode>> exitCalls;
    std::set<std::pair<int, int>> statementDestructorSet;
};

CodeGenerator::CodeGenerator(otava::symbols::Context& context_, const std::string& config_, int optLevel_, bool verbose_, std::string& mainIrName_,
    int& mainFunctionParams_, bool globalMain_, const std::vector<std::string>& compileUnitInitFnNames_, const std::set<std::string>& configurations_) :
    context(context_), emitter(context.GetEmitter()), config(config_), configurations(configurations_), optLevel(optLevel_), verbose(verbose_),
    mainIrName(mainIrName_), mainFunctionParams(mainFunctionParams_),
    functionDefinition(nullptr), entryBlock(nullptr), trueBlock(nullptr), falseBlock(nullptr), defaultBlock(nullptr), breakBlock(nullptr),
    breakBlockId(-1), continueBlock(nullptr), continueBlockId(-1), genJumpingBoolCode(false), lastInstructionWasRet(false),
    prevWasTerminator(false), basicBlockOpen(false), destructorCallGenerated(false), sequenceSecond(nullptr), currentBlockId(-1), globalMain(globalMain_),
    compileUnitInitFnNames(compileUnitInitFnNames_), latestRet(nullptr), boundFunction(nullptr), currentBlock(nullptr), currentBlockSymbol(nullptr),
    currentStatement(nullptr), line(0), inLineNumberCode(false), emitLineNumbers(false)
{
    //otava::symbols::SetCurrentContext(&context);
    std::string intermediateCodeFilePath;
    if (configurations.find("release") != configurations.end())
    {
        intermediateCodeFilePath = util::GetFullPath(
            util::Path::Combine(
                util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(context.FileName()), "bin"), config),
                    std::to_string(otava::symbols::GetOptLevel(optLevel, true))), util::Path::GetFileName(context.FileName()) + ".i"));
    }
    else
    {
        intermediateCodeFilePath = util::GetFullPath(
            util::Path::Combine(
                util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(context.FileName()), "bin"), config),
                util::Path::GetFileName(context.FileName()) + ".i"));
    }
    emitter->SetFilePath(intermediateCodeFilePath);
    util::CreateDirectories(util::Path::GetDirectoryName(intermediateCodeFilePath));
    if (configurations.find("release") != configurations.end())
    {
        optimizedIntermediateFilePath = util::GetFullPath(
            util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(context.FileName()), "bin"), config),
                std::to_string(otava::symbols::GetOptLevel(optLevel, true))), util::Path::GetFileName(context.FileName()) + ".opt.i"));
    }
    else
    {
        optimizedIntermediateFilePath = util::GetFullPath(
            util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(context.FileName()), "bin"), config),
                util::Path::GetFileName(context.FileName()) + ".opt.i"));
    }
}

void CodeGenerator::Reset()
{
    functionDefinition = nullptr;
    entryBlock = nullptr;
    trueBlock = nullptr;
    falseBlock = nullptr;
    defaultBlock = nullptr;
    breakBlock = nullptr;
    breakBlockId = -1;
    continueBlock = nullptr;
    continueBlockId = -1;
    genJumpingBoolCode = false;
    lastInstructionWasRet = false;
    prevWasTerminator = false;
    basicBlockOpen = false;
    destructorCallGenerated = false;
    latestRet = nullptr;
    sequenceSecond = nullptr;
    currentBlockId = -1;
    blockExits.clear();
    boundFunction = nullptr;
    currentBlock = nullptr;
    currentBlockSymbol = nullptr;
    currentStatement = nullptr;
    line = 0;
    emitLineNumbers = context.CurrentProject() && context.CurrentProject()->HasDefine("TRACE");
    exitCalls.clear();
    statementDestructorSet.clear();
}

void CodeGenerator::StatementPrefix()
{
    basicBlockOpen = false;
    lastInstructionWasRet = false;
    latestRet = nullptr;
    destructorCallGenerated = false;
}

void CodeGenerator::GenJumpingBoolCode()
{
    if (!genJumpingBoolCode) return;
    otava::intermediate::Value* cond = emitter->Stack().Pop();
    if (sequenceSecond)
    {
        genJumpingBoolCode = false;
        sequenceSecond->SetGenerated();
        sequenceSecond->Accept(*this);
        genJumpingBoolCode = true;
    }
    emitter->EmitBranch(cond, trueBlock, falseBlock);
}

void CodeGenerator::GenerateVTab(otava::symbols::ClassTypeSymbol* cls, const soul::ast::FullSpan& fullSpan)
{
    if (!cls->IsPolymorphic(&context)) return;
/*
    if (!cls->IsClassTemplateSpecializationSymbol())
    {
        if (cls->GetFlag(otava::symbols::ClassTypeSymbolFlags::vtabGenerated)) return;
        cls->SetFlag(otava::symbols::ClassTypeSymbolFlags::vtabGenerated);
    }
*/
    if (cls->GetFlag(otava::symbols::ClassTypeSymbolFlags::vtabGenerated)) return;
    cls->SetFlag(otava::symbols::ClassTypeSymbolFlags::vtabGenerated);
    cls->ComputeVTabName(&context);
    std::string vtabName = cls->VTabName(&context);
    if (emittedVTabNames.find(vtabName) != emittedVTabNames.end()) return;
    emittedVTabNames.insert(vtabName);
    context.SetFlag(otava::symbols::ContextFlags::generatingVTab);
    cls->MakeVTab(&context, fullSpan);
    otava::intermediate::Type* voidPtrIrType = emitter->MakePtrType(emitter->GetVoidType());
    otava::intermediate::Type* arrayType = emitter->MakeArrayType(cls->VTab().size() * 2 + otava::symbols::vtabClassIdElementCount, voidPtrIrType);
    otava::intermediate::Type* arrayPtrType = emitter->MakePtrType(arrayType);
    std::vector<otava::intermediate::Value*> elements;
    otava::symbols::SymbolId classId = cls->Id();
    otava::intermediate::Value* classIdValue = emitter->EmitConversionValue(voidPtrIrType, emitter->EmitUInt(ToUnderlying(classId)));
    elements.push_back(classIdValue);
    for (otava::symbols::FunctionSymbol* functionSymbol : cls->VTab())
    {
        if (functionSymbol)
        {
            otava::intermediate::Type* irType = functionSymbol->IrType(*emitter, fullSpan, &context);
            if (irType->IsFunctionType())
            {
                if (functionSymbol->IsPure())
                {
                    otava::intermediate::Value* functionValue = emitter->EmitNull(voidPtrIrType);
                    otava::intermediate::Value* deltaValue = emitter->EmitLong(0);
                    otava::intermediate::Value* element1Value = emitter->EmitConversionValue(voidPtrIrType, functionValue);
                    elements.push_back(element1Value);
                    otava::intermediate::Value* element2Value = emitter->EmitConversionValue(voidPtrIrType, deltaValue);
                    elements.push_back(element2Value);
                }
                else
                {
                    otava::intermediate::FunctionType* functionType = static_cast<otava::intermediate::FunctionType*>(irType);
                    emitter->GetOrInsertFunction(functionSymbol->IrName(&context), functionType);
                    otava::intermediate::Value* functionValue = emitter->EmitSymbolValue(functionType, functionSymbol->IrName(&context));
                    std::pair<bool, std::int64_t> p = otava::symbols::Delta(cls, functionSymbol->ParentClassType(&context), *emitter, &context);
                    bool succeeded = p.first;
                    std::int64_t delta = p.second;
                    if (!succeeded)
                    {
                        otava::symbols::ThrowException("could not resolve delta for classes '" + cls->FullName(&context) + "' and '" +
                            functionSymbol->ParentClassType(&context)->FullName(&context) + "'", fullSpan, &context);
                    }
                    otava::intermediate::Value* deltaValue = emitter->EmitLong(delta);
                    otava::intermediate::Value* element1Value = emitter->EmitConversionValue(voidPtrIrType, functionValue);
                    elements.push_back(element1Value);
                    otava::intermediate::Value* element2Value = emitter->EmitConversionValue(voidPtrIrType, deltaValue);
                    elements.push_back(element2Value);
                }
            }
            else
            {
                otava::symbols::ThrowException("function type expected", fullSpan, &context);
            }
        }
        else
        {
            otava::intermediate::Value* element1Value = emitter->EmitNull(voidPtrIrType);
            elements.push_back(element1Value);
            otava::intermediate::Value* deltaValue = emitter->EmitLong(0);
            otava::intermediate::Value* element2Value = emitter->EmitConversionValue(voidPtrIrType, deltaValue);
            elements.push_back(element2Value);
        }
    }
    otava::intermediate::Value* arrayValue = emitter->EmitArrayValue(elements, static_cast<otava::intermediate::ArrayType*>(arrayType));
    otava::intermediate::Value* vtabVariable = emitter->EmitGlobalVariable(arrayType, vtabName, arrayValue);
    emitter->SetVTabVariable(cls->FullName(&context), vtabVariable);
    context.ResetFlag(otava::symbols::ContextFlags::generatingVTab);
    for (const auto& boundVTabFunction : context.BoundVTabFunctions())
    {
        boundVTabFunction->Accept(*this);
    }
    context.ClearBoundVTabFunctions();
}

void CodeGenerator::ExitBlocks(int sourceBlockId, int targetBlockId, const soul::ast::FullSpan& fullSpan)
{
    bool prevEmitLineNumbers = emitLineNumbers;
    emitLineNumbers = false;
    bool createBasicBlock = false;
    otava::symbols::BoundStatementNode* lastStatement = nullptr;
    if (boundFunction->Body() && !boundFunction->Body()->Statements().empty())
    {
        lastStatement = boundFunction->Body()->Statements().back().get();
    }
    otava::symbols::BoundStatementNode* currentBlockLastStatement = nullptr;
    if (currentBlock && !currentBlock->Statements().empty())
    {
        currentBlockLastStatement = currentBlock->Statements().back().get();
    }
    if (lastStatement && currentBlockLastStatement && lastStatement == currentBlockLastStatement && currentBlockLastStatement->IsReturnOrSequenceReturnStatementNode())
    {
        createBasicBlock = true;
    }
    if (targetBlockId == -1)
    {
        for (int i = sourceBlockId; i >= 0; --i)
        {
            if (i >= 0 && i < blockExits.size())
            {
                BlockExit* exit = blockExits[i].get();
                if (exit)
                {
                    if (!exit->IsEmpty())
                    {
                        if (createBasicBlock)
                        {
                            otava::intermediate::BasicBlock* nextBlock = emitter->CreateBasicBlock();
                            if (!lastInstructionWasRet)
                            {
                                emitter->EmitJump(nextBlock);
                            }
                            emitter->SetCurrentBasicBlock(nextBlock);
                            createBasicBlock = false;
                        }
                        exit->Execute(*emitter, fullSpan, &context, false);
                        destructorCallGenerated = true;
                    }
                }
            }
        }
    }
    else
    {
        for (int i = sourceBlockId; i >= targetBlockId; --i)
        {
            if (i >= 0 && i < blockExits.size())
            {
                BlockExit* exit = blockExits[i].get();
                if (exit)
                {
                    if (!exit->IsEmpty())
                    {
                        if (createBasicBlock)
                        {
                            otava::intermediate::BasicBlock* nextBlock = emitter->CreateBasicBlock();
                            if (!lastInstructionWasRet)
                            {
                                emitter->EmitJump(nextBlock);
                            }
                            emitter->SetCurrentBasicBlock(nextBlock);
                            createBasicBlock = false;
                        }
                        exit->Execute(*emitter, fullSpan, &context, true);
                        destructorCallGenerated = true;
                    }
                }
            }
        }
    }
    emitLineNumbers = prevEmitLineNumbers;
}

void CodeGenerator::EmitReturn(const soul::ast::FullSpan& fullSpan)
{
    if (functionDefinition->ReturnType(&context) && !functionDefinition->ReturnType(&context)->IsVoidType() && !functionDefinition->ReturnsClass())
    {
        otava::intermediate::Value* returnValue = functionDefinition->ReturnType(&context)->DirectType(&context)->FinalType(fullSpan, &context)->IrType(
            *emitter, fullSpan, &context)->DefaultValue();
        emitter->EmitRet(returnValue);
    }
    else
    {
        emitter->EmitRetVoid();
    }
}

void CodeGenerator::GenerateGlobalInitializationFunction()
{
    Reset();
    std::string setBadAllocExStr;
    std::uint32_t ext;
    otava::symbols::FunctionDefinitionSymbol* globalInit = new otava::symbols::FunctionDefinitionSymbol(context.GetModule(), 
        context.GetNextSymbolId(otava::symbols::SymbolKind::functionDefinitionSymbol), "__global_init__");
    globalInit->SetReturnType(context.GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
        otava::symbols::FundamentalTypeKind::voidType, &context), &context);
    globalInit->SetLinkage(otava::symbols::Linkage::c_linkage);
    otava::ast::NestedNameSpecifierNode* nnsNode = new otava::ast::NestedNameSpecifierNode(soul::ast::Span(), -1);
    nnsNode->AddNode(new otava::ast::IdentifierNode(soul::ast::Span(), -1, "std"));
    otava::ast::QualifiedIdNode badAllocNode(soul::ast::Span(), -1, nnsNode, new otava::ast::IdentifierNode(soul::ast::Span(), -1, "bad_alloc"));
    otava::symbols::TypeSymbol* badAllocType = otava::symbols::ResolveType(&badAllocNode, otava::symbols::DeclarationFlags::none, &context);
    ext = otava::symbols::ToUnderlying(badAllocType->Id());
    setBadAllocExStr.append("ort_set_bad_alloc(new std::bad_alloc(), ").append(std::to_string(ext)).append("u);");
    std::unique_ptr<otava::ast::Node> setBadAllocStmtNode = otava::symbols::ParseStatement(setBadAllocExStr, &context);
    std::unique_ptr<otava::symbols::BoundFunctionNode> boundFunction(new otava::symbols::BoundFunctionNode(globalInit, soul::ast::FullSpan()));
    context.PushBoundFunction(boundFunction.release());
    std::unique_ptr<otava::symbols::BoundStatementNode> setBadAllocStmt(otava::symbols::BindStatement(setBadAllocStmtNode.get(), nullptr, &context));
    std::unique_ptr<otava::symbols::BoundCompoundStatementNode> compoundStmt(new otava::symbols::BoundCompoundStatementNode(soul::ast::FullSpan()));
    compoundStmt->AddStatement(setBadAllocStmt.release());
    context.PushSetFlag(otava::symbols::ContextFlags::makeCompileUnitInitFn);
    int n = compileUnitInitFnNames.size();
    for (int i = 0; i < n; ++i)
    {
        std::unique_ptr<otava::ast::Node> callInitFunctionNode = otava::symbols::ParseStatement(compileUnitInitFnNames[i] + "();", &context);
        std::unique_ptr<otava::symbols::BoundStatementNode> initFnCall(otava::symbols::BindStatement(callInitFunctionNode.get(), globalInit, &context));
        compoundStmt->AddStatement(initFnCall.release());
    }
    context.PopFlags();
    context.GetBoundFunction()->SetBody(compoundStmt.release());
    context.GetBoundFunction()->Accept(*this);
}

void CodeGenerator::GenerateGlobalDoneFunction()
{
    std::string deleteBadAllocStr;
    deleteBadAllocStr.append("delete static_cast<std::bad_alloc*>(ort_get_bad_alloc());");
    std::unique_ptr<otava::ast::Node> deleteBadAllocStmtNode = otava::symbols::ParseStatement(deleteBadAllocStr, &context);
    otava::symbols::FunctionDefinitionSymbol* globalDone = new otava::symbols::FunctionDefinitionSymbol(context.GetModule(),
        context.GetNextSymbolId(otava::symbols::SymbolKind::functionDefinitionSymbol), "__global_done__");
    globalDone->SetLinkage(otava::symbols::Linkage::c_linkage);
    std::unique_ptr<otava::symbols::BoundFunctionNode> boundFunction(new otava::symbols::BoundFunctionNode(globalDone, soul::ast::FullSpan()));
    context.PushBoundFunction(boundFunction.release());
    std::unique_ptr<otava::symbols::BoundStatementNode> deleteBadAllocStmt(otava::symbols::BindStatement(deleteBadAllocStmtNode.get(), nullptr, &context));
    std::unique_ptr<otava::symbols::BoundCompoundStatementNode> compoundStmt(new otava::symbols::BoundCompoundStatementNode(soul::ast::FullSpan()));
    compoundStmt->AddStatement(deleteBadAllocStmt.release());
    context.GetBoundFunction()->SetBody(compoundStmt.release());
    context.GetBoundFunction()->Accept(*this);
}

void CodeGenerator::SetCurrentLineNumber(const soul::ast::FullSpan& fullSpan)
{
    if (!emitLineNumbers) return;
    if (!fullSpan.IsValid()) return;
    if (inLineNumberCode) return;
    if (boundFunction)
    {
        const std::vector<int>* lineStartIndeces = context.GetFileMap()->LineStartIndeces(fullSpan.fileIndex);
        if (lineStartIndeces)
        {
            soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(fullSpan.span, *lineStartIndeces);
            if (line != lineColLen.line)
            {
                line = lineColLen.line;
                emitter->SetLine(line);
                if (boundFunction->GetBoundSetLineStatement())
                {
                    inLineNumberCode = true;
                    bool prevGenJumpingBoolCode = genJumpingBoolCode;
                    genJumpingBoolCode = false;
                    boundFunction->GetBoundSetLineStatement()->Accept(*this);
                    genJumpingBoolCode = prevGenJumpingBoolCode;
                    inLineNumberCode = false;
                }
                emitter->SetLine(0);
            }
        }
    }
}

void CodeGenerator::SetCurrentBlockSymbol(int blockId)
{
    otava::symbols::Symbol* block = functionDefinition->GetBlock(blockId);
    otava::symbols::BlockSymbol* blockSymbol = nullptr;
    if (block && block->IsBlockSymbol())
    {
        blockSymbol = static_cast<otava::symbols::BlockSymbol*>(block);
    }
    currentBlockSymbol = blockSymbol;
}

void CodeGenerator::GenerateDestructorCallsForCurrentStatement()
{
    if (currentBlockSymbol && currentStatement)
    {
        std::pair<int, int> statementDestructorId(currentBlockSymbol->BlockId(), currentStatement->StatementIndex());
        if (statementDestructorSet.find(statementDestructorId) != statementDestructorSet.end())
        {
            return;
        }
        statementDestructorSet.insert(statementDestructorId);
        if (currentBlockSymbol->HasDestructorCalls(currentStatement->StatementIndex()))
        {
            std::vector<otava::symbols::BoundExpressionNode*> destructorCalls = currentBlockSymbol->GetDestructorCalls(currentStatement->StatementIndex());
            for (otava::symbols::BoundExpressionNode* destructorCall : destructorCalls)
            {
                blockExits[currentBlockId]->AddDestructorCall(destructorCall);
            }
        }
    }
}

void CodeGenerator::Visit(otava::symbols::BoundEmptyStatementNode& node)
{
    emitter->EmitNop();
}

void CodeGenerator::Visit(otava::symbols::BoundCompileUnitNode& node)
{
    context.PushSetFlag(otava::symbols::ContextFlags::requireForwardResolved);
    if (globalMain)
    {
        GenerateGlobalInitializationFunction();
        GenerateGlobalDoneFunction();
    }
    node.Sort();
    emitter->SetCompileUnitInfo(node.Id(), context.FileName());
    for (int i = 0; i < node.BoundNodes().size(); ++i)
    {
        otava::symbols::BoundNode* boundNode = node.BoundNodes()[i].get();
        Reset();
        boundNode->Accept(*this);
    }
    otava::symbols::BoundFunctionNode* initFunction = node.GetCompileUnitInitializationFunction();
    if (initFunction)
    {
        Reset();
        initFunction->Accept(*this);
    }
    emitter->ResolveReferences();
    emitter->Emit();
    otava::intermediate::IntermediateContext intermediateContext;
    otava::intermediate::IntermediateContext optimizationContext;
    otava::intermediate::IntermediateContext* finalContext = &intermediateContext;
    otava::intermediate::Parse(emitter->FilePath(), intermediateContext, verbose);
    otava::intermediate::Verify(intermediateContext);
    std::string assemblyFilePath;
    if (configurations.find("release") != configurations.end())
    {
        assemblyFilePath = util::GetFullPath(
            util::Path::Combine(
                util::Path::Combine(util::Path::Combine(util::Path::Combine(
                    util::Path::GetDirectoryName(context.FileName()), "bin"), config), std::to_string(otava::symbols::GetOptLevel(optLevel, true))),
                util::Path::GetFileName(context.FileName()) + ".asm"));
    }
    else
    {
        assemblyFilePath = util::GetFullPath(
            util::Path::Combine(util::Path::Combine(
                util::Path::Combine(util::Path::GetDirectoryName(context.FileName()), "bin"), config),
                util::Path::GetFileName(context.FileName()) + ".asm"));
    }
    std::unique_ptr<otava::intermediate::CodeGenerator> codeGenerator;
    if (context.ReleaseConfig())
    {
        intermediateContext.SetFilePath(optimizedIntermediateFilePath);
        otava::optimizer::Optimize(&intermediateContext);
        intermediateContext.WriteFile();
        otava::intermediate::Parse(optimizedIntermediateFilePath, optimizationContext, verbose);
        otava::intermediate::Verify(optimizationContext);
        finalContext = &optimizationContext;
        codeGenerator.reset(new otava::optimizer::OptimizingCodeGenerator(finalContext, assemblyFilePath));
    }
    else
    {
        codeGenerator.reset(new otava::intermediate::CodeGenerator(finalContext, assemblyFilePath));
    }
    otava::intermediate::GenerateCode(*finalContext, *codeGenerator, verbose);
    asmFileName = util::Path::GetFileName(context.FileName()) + ".asm";
    context.PopFlags();
    context.SetTotalFunctionsCompiled(context.TotalFunctionsCompiled() + intermediateContext.TotalFunctions());
    context.SetTotalFunctionsCompiled(context.TotalFunctionsCompiled() + optimizationContext.TotalFunctions());
    context.SetFunctionsInlined(context.FunctionsInlined() + intermediateContext.FunctionsInlined());
    context.SetFunctionsInlined(context.FunctionsInlined() + optimizationContext.FunctionsInlined());
    context.SetFunctionCallsInlined(context.FunctionCallsInlined() + intermediateContext.InlinedFunctionCalls());
    context.SetFunctionCallsInlined(context.FunctionCallsInlined() + optimizationContext.InlinedFunctionCalls());
    for (auto* cls : context.GetSymbolTable()->Classes())
    {
        AddClassInfo(cls);
    }
}

void CodeGenerator::Visit(otava::symbols::BoundClassNode& node)
{
    otava::symbols::ClassTypeSymbol* cls = node.GetClass();
    cls->MakeObjectLayout(soul::ast::FullSpan(), &context);
    GenerateVTab(cls, node.GetFullSpan());
}

void CodeGenerator::AddClassInfo(otava::symbols::ClassTypeSymbol* cls)
{
    std::set<const otava::symbols::Symbol*> visited;
    if (cls->IsTemplate(&context) || cls->IsTemplateParameterInstantiation(&context, visited)) return;
    otava::symbols::class_id classId = otava::symbols::class_id(ToUnderlying(cls->Id()));
    otava::symbols::SymbolTable* symbolTable = context.GetSymbolTable();
    otava::symbols::class_index& index = symbolTable->ClassIndex();
    if (index.get_class_info(classId)) return;
    otava::intermediate::Type* clsType = cls->IrType(*emitter, soul::ast::FullSpan(), &context);
    otava::symbols::ClassKind kind = cls->GetClassKind();
    otava::symbols::class_key key = otava::symbols::class_key::cls;
    switch (kind)
    {
    case otava::symbols::ClassKind::class_: { key = otava::symbols::class_key::cls; break; }
    case otava::symbols::ClassKind::struct_: { key = otava::symbols::class_key::strct; break; }
    case otava::symbols::ClassKind::union_: { key = otava::symbols::class_key::uni; break; }
    }
    std::unique_ptr<otava::symbols::class_info> info(new otava::symbols::class_info(classId, key, cls->FullName(&context), clsType->Size()));
    for (otava::symbols::ClassTypeSymbol* base : cls->BaseClasses(&context))
    {
        otava::symbols::class_id baseId = otava::symbols::class_id(otava::symbols::ToUnderlying(base->Id()));
        info->add_base(baseId);
    }
    index.add_class(info);
}

void CodeGenerator::Visit(otava::symbols::BoundFunctionNode& node)
{
    boundFunction = &node;
    functionDefinition = node.GetFunctionDefinitionSymbol();
    if ((functionDefinition->Qualifiers() & otava::symbols::FunctionQualifiers::isDeleted) != otava::symbols::FunctionQualifiers::none)
    {
        return;
    }
    if (functionDefinition->ContainsGotosOrLabels())
    {
        BuildGotoTargetMap(node.Body(), &context);
    }
    std::string functionDefinitionName = functionDefinition->IrName(&context);
    if (functionDefinition->GroupName() == "main")
    {
        mainIrName = functionDefinitionName;
        mainFunctionParams = int(functionDefinition->Arity());
    }
    otava::intermediate::Type* functionType = functionDefinition->IrType(*emitter, node.GetFullSpan(), &context);
    bool once = false;
    bool inline_ = context.ReleaseConfig() && functionDefinition->IsInline();
    bool child = functionDefinition->ParentFn() != nullptr;
    otava::intermediate::Function* function = emitter->CreateFunction(functionDefinitionName, functionType, inline_, once, child);
    if (child)
    {
        function->SetComment(functionDefinition->FullName(&context) + ", parent=" + functionDefinition->ParentFn()->FullName(&context));
    }
    else
    {
        function->SetComment(functionDefinition->FullName(&context));
    }
    otava::intermediate::MetadataStruct* mdStruct = emitter->CreateMetadataStruct();
    mdStruct->AddItem("nodeType", emitter->CreateMetadataLong(otava::intermediate::funcInfoNodeType));
    mdStruct->AddItem("fullName", emitter->CreateMetadataString(functionDefinition->FullName(&context)));
    if (child)
    {
        mdStruct->AddItem("parentName", emitter->CreateMetadataString(functionDefinition->ParentFn()->FullName(&context)));
    }
    function->SetMdId(mdStruct->Id());
    otava::intermediate::MetadataRef* mdRef = emitter->CreateMetadataRef(mdStruct->Id());
    function->SetMetadataRef(mdRef);
    entryBlock = emitter->CreateBasicBlock();
    emitter->SetCurrentBasicBlock(entryBlock);
    if (functionDefinition->ParentFn())
    {
        otava::symbols::FunctionDefinitionSymbol* parentFn = functionDefinition->ParentFn();
        int level = 0;
        while (parentFn)
        {
            int np = parentFn->MemFnParameters(&context).size();
            for (int i = 0; i < np; ++i)
            {
                otava::symbols::ParameterSymbol* parameter = parentFn->MemFnParameters(&context)[i];
                otava::symbols::TypeSymbol* type = parameter->GetReferredType(&context);
                if (type)
                {
                    otava::intermediate::Value* plocal = emitter->EmitPLocal(type->IrType(*emitter, node.GetFullSpan(), &context), level);
                    emitter->SetIrObject(parameter, plocal);
                }
                else
                {
                    otava::symbols::PrintWarning("type of parameter '" + parameter->Name() + "' not set", node.GetFullSpan(), &context);
                }
            }
            if (parentFn->ReturnsClass())
            {
                otava::symbols::ParameterSymbol* parameter = parentFn->ReturnValueParam(&context);
                otava::symbols::TypeSymbol* type = parameter->GetReferredType(&context);
                if (type)
                {
                    otava::intermediate::Value* plocal = emitter->EmitPLocal(
                        parameter->GetReferredType(&context)->IrType(*emitter, node.GetFullSpan(), &context), level);
                    emitter->SetIrObject(parameter, plocal);
                }
                else
                {
                    otava::symbols::PrintWarning("type of parameter '" + parameter->Name() + "' not set", node.GetFullSpan(), &context);
                }
            }
            int nlv = parentFn->LocalVariables().size();
            for (int i = 0; i < nlv; ++i)
            {
                otava::symbols::VariableSymbol* localVariable = parentFn->LocalVariables()[i];
                otava::symbols::TypeSymbol* type = localVariable->GetReferredType(&context);
                if (type)
                {
                    otava::intermediate::Value* plocal = emitter->EmitPLocal(type->IrType(*emitter, node.GetFullSpan(), &context), level);
                    emitter->SetIrObject(localVariable, plocal);
                }
                else
                {
                    //otava::symbols::PrintWarning("type of local variable '" + localVariable->Name() + "' not set", node.GetFullSpan(), &context);
                }
            }
            parentFn = parentFn->ParentFn();
            ++level;
        }
    }
    int np = functionDefinition->MemFnParameters(&context).size();
    for (int i = 0; i < np; ++i)
    {
        otava::symbols::ParameterSymbol* parameter = functionDefinition->MemFnParameters(&context)[i];
        otava::symbols::TypeSymbol* type = parameter->GetReferredType(&context);
        if (type)
        {
            otava::intermediate::Value* local = emitter->EmitLocal(type->IrType(*emitter, node.GetFullSpan(), &context));
            emitter->SetIrObject(parameter, local);
        }
        else
        {
            otava::symbols::PrintWarning("type of parameter '" + parameter->Name() + "' not set", node.GetFullSpan(), &context);
        }
    }
    if (functionDefinition->ReturnsClass())
    {
        otava::symbols::ParameterSymbol* parameter = functionDefinition->ReturnValueParam(&context);
        otava::symbols::TypeSymbol* type = parameter->GetReferredType(&context);
        if (type)
        {
            otava::intermediate::Value* local = emitter->EmitLocal(parameter->GetReferredType(&context)->IrType(*emitter, node.GetFullSpan(), &context));
            emitter->SetIrObject(parameter, local);
        }
        else
        {
            otava::symbols::PrintWarning("type of parameter '" + parameter->Name() + "' not set", node.GetFullSpan(), &context);
        }
    }
    int nlv = functionDefinition->LocalVariables().size();
    for (int i = 0; i < nlv; ++i)
    {
        otava::symbols::VariableSymbol* localVariable = functionDefinition->LocalVariables()[i];
        std::string name = localVariable->Name();
        otava::symbols::TypeSymbol* type = localVariable->GetReferredType(&context);
        if (type)
        {
            otava::intermediate::Value* local = emitter->EmitLocal(type->IrType(*emitter, node.GetFullSpan(), &context));
            emitter->SetIrObject(localVariable, local);
        }
        else
        {
            //otava::symbols::PrintWarning("type of local variable '" + localVariable->Name() + "' not set", node.GetFullSpan(), &context);
        }
    }
    for (int i = 0; i < np; ++i)
    {
        otava::intermediate::Value* param = emitter->GetParam(i);
        otava::symbols::ParameterSymbol* parameter = functionDefinition->MemFnParameters(&context)[i];
        if (parameter->GetType(&context)->IsClassTypeSymbol())
        {
            otava::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<otava::symbols::ClassTypeSymbol*>(parameter->GetType(&context));
            if (classTypeSymbol->CopyCtor())
            {
                otava::intermediate::FunctionType* copyCtorType = static_cast<otava::intermediate::FunctionType*>(classTypeSymbol->CopyCtor()->IrType(
                    *emitter, node.GetFullSpan(), &context));
                otava::intermediate::Function* copyCtor = emitter->GetOrInsertFunction(classTypeSymbol->CopyCtor()->IrName(&context), copyCtorType);
                std::vector<otava::intermediate::Value*> args;
                args.push_back(static_cast<otava::intermediate::Value*>(parameter->IrObject(*emitter, node.GetFullSpan(), &context)));
                args.push_back(param);
                emitter->EmitCall(copyCtor, args);
                otava::symbols::Symbol* dtorSymbol = otava::symbols::GenerateDestructor(classTypeSymbol, node.GetFullSpan(), &context);
                if (dtorSymbol && dtorSymbol->IsFunctionSymbol())
                {
                    otava::symbols::FunctionSymbol* dtorFn = static_cast<otava::symbols::FunctionSymbol*>(dtorSymbol);
                    otava::symbols::BoundFunctionCallNode* dtorCall = new otava::symbols::BoundFunctionCallNode(dtorFn, node.GetFullSpan(), classTypeSymbol);
                    dtorCall->AddArgument(new otava::symbols::BoundValueExpressionNode(
                        static_cast<otava::intermediate::Value*>(parameter->IrObject(*emitter, node.GetFullSpan(), &context)),
                        classTypeSymbol->AddPointer(&context)));
                    exitCalls.push_back(std::unique_ptr<otava::symbols::BoundFunctionCallNode>(dtorCall));
                }
            }
            else
            {
                otava::symbols::ThrowException("class type '" + classTypeSymbol->FullName(&context) + "' has no copy constructor needed by function '" +
                    functionDefinition->FullName(&context) + "'", node.GetFullSpan(), &context);
            }
        }
        else
        {
            emitter->EmitStore(param, static_cast<otava::intermediate::Value*>(parameter->IrObject(*emitter, node.GetFullSpan(), &context)));
        }
    }
    if (functionDefinition->ReturnsClass())
    {
        otava::intermediate::Value* param = emitter->GetParam(np);
        otava::symbols::ParameterSymbol* parameter = functionDefinition->ReturnValueParam(&context);
        emitter->EmitStore(param, static_cast<otava::intermediate::Value*>(parameter->IrObject(*emitter, node.GetFullSpan(), &context)));
    }
    otava::symbols::BoundCtorInitializerNode* ctorInitializer = node.CtorInitializer();
    if (ctorInitializer)
    {
        ctorInitializer->GenerateCode(*this, *emitter, &context);
    }
    otava::symbols::BoundDtorTerminatorNode* dtorTerminator = node.DtorTerminator();
    if (dtorTerminator)
    {
        for (const auto& setVPtrStatement : dtorTerminator->SetVPtrStatements())
        {
            setVPtrStatement->Accept(*this);
        }
    }
    node.Body()->Accept(*this);
    if (dtorTerminator)
    {
        dtorTerminator->GenerateCode(*this, *emitter, &context);
        for (const auto& setVPtrStatement : dtorTerminator->SetVPtrStatements())
        {
            setVPtrStatement->Accept(*this);
        }
    }
    StatementPrefix();
    otava::symbols::BoundStatementNode* lastStatement = nullptr;
    if (!node.Body()->Statements().empty())
    {
        lastStatement = node.Body()->Statements().back().get();
    }
    if (!lastStatement || !lastStatement->IsReturnOrSequenceReturnStatementNode() || lastStatement->IsReturnOrSequenceReturnStatementNode() && destructorCallGenerated)
    {
        if (functionDefinition->ReturnType(&context) && !functionDefinition->ReturnType(&context)->IsVoidType() && !functionDefinition->ReturnsClass())
        {
            otava::intermediate::Value* returnValue = functionDefinition->ReturnType(&context)->DirectType(&context)->FinalType(node.GetFullSpan(), &context)->IrType(
                *emitter, node.GetFullSpan(), &context)->DefaultValue();
            emitter->EmitRet(returnValue);
            lastInstructionWasRet = true;
        }
        else
        {
            emitter->EmitRetVoid();
            lastInstructionWasRet = true;
        }
    }
    emitter->SetRegNumbers();
}

void CodeGenerator::Visit(otava::symbols::BoundCompoundStatementNode& node)
{
    otava::symbols::BoundCompoundStatementNode* prevBlock = currentBlock;
    currentBlock = &node;
    ++currentBlockId;
    while (currentBlockId >= blockExits.size()) blockExits.push_back(std::unique_ptr<BlockExit>());
    blockExits[currentBlockId].reset(new BlockExit());
    if (!prevBlock)
    {
        for (const auto& exitCall : exitCalls)
        {
            blockExits[currentBlockId]->AddDestructorCall(exitCall->Clone());
        }
    }
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    otava::symbols::BlockSymbol* prevBlockSymbol = currentBlockSymbol;
    SetCurrentBlockSymbol(node.BlockId());
    otava::symbols::BoundStatementNode* prevStatement = currentStatement;
    int n = node.Statements().size();
    for (int i = 0; i < n; ++i)
    {
        otava::symbols::BoundStatementNode* statement = node.Statements()[i].get();
        currentStatement = statement;
        statement->Accept(*this);
        GenerateDestructorCallsForCurrentStatement();
        prevWasTerminator = statement->EndsWithTerminator();
    }
    if (!prevWasTerminator && !blockExits[currentBlockId]->IsEmpty())
    {
        ExitBlocks(currentBlockId, currentBlockId, node.GetFullSpan());
    }
    --currentBlockId;
    currentBlock = prevBlock;
    currentBlockSymbol = prevBlockSymbol;
    currentStatement = prevStatement;
    node.DestructTemporaries(*emitter, &context);
}

void CodeGenerator::Visit(otava::symbols::BoundIfStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    otava::symbols::BlockSymbol* prevBlockSymbol = currentBlockSymbol;
    SetCurrentBlockSymbol(node.BlockId());
    currentStatement = &node;
    otava::intermediate::BasicBlock* prevTrueBlock = trueBlock;
    otava::intermediate::BasicBlock* prevFalseBlock = falseBlock;
    trueBlock = emitter->CreateBasicBlock();
    otava::intermediate::BasicBlock* nextBlock = emitter->CreateBasicBlock();
    if (node.ElseStatement())
    {
        falseBlock = emitter->CreateBasicBlock();
    }
    else
    {
        falseBlock = nextBlock;
    }
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    node.GetCondition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    prevWasTerminator = false;
    node.ThenStatement()->Accept(*this);
    if (!node.ThenStatement()->EndsWithTerminator())
    {
        emitter->EmitJump(nextBlock);
    }
    if (node.ElseStatement())
    {
        emitter->SetCurrentBasicBlock(falseBlock);
        prevWasTerminator = false;
        node.ElseStatement()->Accept(*this);
        emitter->EmitJump(nextBlock);
        emitter->SetCurrentBasicBlock(nextBlock);
    }
    else
    {
        emitter->SetCurrentBasicBlock(nextBlock);
    }
    basicBlockOpen = true;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
    node.DestructTemporaries(*emitter, &context);
    currentBlockSymbol = prevBlockSymbol;
}

void CodeGenerator::Visit(otava::symbols::BoundSwitchStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    otava::symbols::BlockSymbol* prevBlockSymbol = currentBlockSymbol;
    SetCurrentBlockSymbol(node.BlockId());
    currentStatement = &node;
    bool prevEmitLineNumbers = emitLineNumbers;
    emitLineNumbers = false;
    node.GetCondition()->Accept(*this);
    otava::intermediate::BasicBlock* prevDefaultBlock = defaultBlock;
    otava::intermediate::BasicBlock* prevBreakBlock = breakBlock;
    int prevBreakBlockId = breakBlockId;
    otava::intermediate::Value* condition = emitter->Stack().Pop();
    std::unique_ptr<SwitchTargets> switchTargets = CollectSwitchTargets(*emitter, node.Statement());
    otava::intermediate::BasicBlock* nextBlock = nullptr;
    if (switchTargets->Default())
    {
        defaultBlock = switchTargets->Default()->block;
        nextBlock = emitter->CreateBasicBlock();
    }
    else
    {
        nextBlock = emitter->CreateBasicBlock();
        defaultBlock = nextBlock;
    }
    breakBlock = nextBlock;
    breakBlockId = currentBlockId;
    otava::intermediate::SwitchInstruction* switchInst = emitter->EmitSwitch(condition, defaultBlock);
    int n = switchTargets->Cases().size();
    for (int i = 0; i < n; ++i)
    {
        const auto& caseTarget = switchTargets->Cases()[i];
        emitter->SetCurrentBasicBlock(caseTarget->block);
        for (auto* expr : caseTarget->exprs)
        {
            EvaluateConstantExpr(*emitter, node.GetFullSpan(), context, expr);
            otava::intermediate::Value* caseValue = emitter->Stack().Pop();
            otava::intermediate::CaseTarget target(caseValue, caseTarget->block->Id());
            switchInst->AddCaseTarget(target);
        }
        prevWasTerminator = false;
        caseTarget->statement->Accept(*this);
        if (basicBlockOpen)
        {
            emitter->EmitJump(nextBlock);
            basicBlockOpen = false;
        }
        /*
                if (!caseTarget->statement->EndsWithTerminator())
                {
                    if (i < n - 1)
                    {
                        emitter->EmitJump(switchTargets->Cases()[i + 1]->block);
                    }
                    else
                    {
                        emitter->EmitJump(nextBlock);
                    }
                }
        */
    }
    if (switchTargets->Default())
    {
        emitter->SetCurrentBasicBlock(defaultBlock);
        switchTargets->Default()->statement->Accept(*this);
        if (basicBlockOpen)
        {
            emitter->EmitJump(nextBlock);
            basicBlockOpen = false;
        }
        /*
                if (!switchTargets->Default()->statement->EndsWithTerminator())
                {
                    emitter->EmitJump(nextBlock);
                }
        */
    }
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
    defaultBlock = prevDefaultBlock;
    breakBlock = prevBreakBlock;
    breakBlockId = prevBreakBlockId;
    emitLineNumbers = prevEmitLineNumbers;
    currentBlockSymbol = prevBlockSymbol;
    node.DestructTemporaries(*emitter, &context);
}

void CodeGenerator::Visit(otava::symbols::BoundCaseStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    currentStatement = &node;
    node.Statement()->Accept(*this);
}

void CodeGenerator::Visit(otava::symbols::BoundDefaultStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    currentStatement = &node;
    node.Statement()->Accept(*this);
}

void CodeGenerator::Visit(otava::symbols::BoundWhileStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    otava::symbols::BlockSymbol* prevBlockSymbol = currentBlockSymbol;
    SetCurrentBlockSymbol(node.BlockId());
    currentStatement = &node;
    otava::intermediate::BasicBlock* prevTrueBlock = trueBlock;
    otava::intermediate::BasicBlock* prevFalseBlock = falseBlock;
    otava::intermediate::BasicBlock* prevBreakBlock = breakBlock;
    int prevBreakBlockId = breakBlockId;
    otava::intermediate::BasicBlock* prevContinueBlock = continueBlock;
    int prevContinueBlockId = continueBlockId;
    trueBlock = emitter->CreateBasicBlock();
    falseBlock = emitter->CreateBasicBlock();
    breakBlock = falseBlock;
    breakBlockId = currentBlockId;
    otava::intermediate::BasicBlock* condBlock = emitter->CreateBasicBlock();
    emitter->EmitJump(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    continueBlock = condBlock;
    continueBlockId = currentBlockId;
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    node.GetCondition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    node.Statement()->Accept(*this);
    emitter->EmitJump(condBlock);
    emitter->SetCurrentBasicBlock(falseBlock);
    continueBlock = prevContinueBlock;
    continueBlockId = prevContinueBlockId;
    breakBlock = prevBreakBlock;
    breakBlockId = prevBreakBlockId;
    falseBlock = prevFalseBlock;
    trueBlock = prevTrueBlock;
    currentBlockSymbol = prevBlockSymbol;
    node.DestructTemporaries(*emitter, &context);
}

void CodeGenerator::Visit(otava::symbols::BoundDoStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    currentStatement = &node;
    otava::intermediate::BasicBlock* prevTrueBlock = trueBlock;
    otava::intermediate::BasicBlock* prevFalseBlock = falseBlock;
    otava::intermediate::BasicBlock* prevBreakBlock = breakBlock;
    int prevBreakBlockId = breakBlockId;
    otava::intermediate::BasicBlock* prevContinueBlock = continueBlock;
    int prevContinueBlockId = continueBlockId;
    otava::intermediate::BasicBlock* doBlock = emitter->CreateBasicBlock();
    otava::intermediate::BasicBlock* condBlock = emitter->CreateBasicBlock();
    trueBlock = doBlock;
    falseBlock = emitter->CreateBasicBlock();
    breakBlock = falseBlock;
    breakBlockId = currentBlockId;
    continueBlock = condBlock;
    continueBlockId = currentBlockId;
    emitter->EmitJump(doBlock);
    emitter->SetCurrentBasicBlock(doBlock);
    node.Statement()->Accept(*this);
    emitter->EmitJump(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    node.GetExpr()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(falseBlock);
    basicBlockOpen = true;
    continueBlock = prevContinueBlock;
    continueBlockId = prevContinueBlockId;
    breakBlock = prevBreakBlock;
    breakBlockId = prevBreakBlockId;
    falseBlock = prevFalseBlock;
    trueBlock = prevTrueBlock;
    node.DestructTemporaries(*emitter, &context);
}

void CodeGenerator::Visit(otava::symbols::BoundForStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    otava::symbols::BlockSymbol* prevBlockSymbol = currentBlockSymbol;
    SetCurrentBlockSymbol(node.BlockId());
    currentStatement = &node;
    otava::intermediate::BasicBlock* prevTrueBlock = trueBlock;
    otava::intermediate::BasicBlock* prevFalseBlock = falseBlock;
    otava::intermediate::BasicBlock* prevBreakBlock = breakBlock;
    int prevBreakBlockId = breakBlockId;
    otava::intermediate::BasicBlock* prevContinueBlock = continueBlock;
    int prevContinueBlockId = continueBlockId;
    if (node.InitStatement())
    {
        node.InitStatement()->Accept(*this);
    }
    otava::intermediate::BasicBlock* condBlock = emitter->CreateBasicBlock();
    otava::intermediate::BasicBlock* actionBlock = emitter->CreateBasicBlock();
    otava::intermediate::BasicBlock* loopBlock = emitter->CreateBasicBlock();
    trueBlock = actionBlock;
    falseBlock = emitter->CreateBasicBlock();
    breakBlock = falseBlock;
    breakBlockId = currentBlockId;
    continueBlock = loopBlock;
    continueBlockId = currentBlockId;
    emitter->EmitJump(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    if (node.GetCondition())
    {
        node.GetCondition()->Accept(*this);
    }
    else
    {
        emitter->EmitBool(true);
        GenJumpingBoolCode();
    }
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(actionBlock);
    node.Statement()->Accept(*this);
    emitter->EmitJump(loopBlock);
    emitter->SetCurrentBasicBlock(loopBlock);
    if (node.GetLoopExpr())
    {
        node.GetLoopExpr()->Accept(*this);
    }
    emitter->EmitJump(condBlock);
    emitter->SetCurrentBasicBlock(falseBlock);
    basicBlockOpen = true;
    continueBlock = prevContinueBlock;
    continueBlockId = prevContinueBlockId;
    breakBlock = prevBreakBlock;
    breakBlockId = prevBreakBlockId;
    falseBlock = prevFalseBlock;
    trueBlock = prevTrueBlock;
    currentBlockSymbol = prevBlockSymbol;
    node.DestructTemporaries(*emitter, &context);
}

void CodeGenerator::Visit(otava::symbols::BoundSequenceStatementNode& node)
{
    StatementPrefix();
    currentStatement = &node;
    otava::symbols::BoundStatementNode* prevSequenceSecond = sequenceSecond;
    sequenceSecond = node.Second();
    node.First()->Accept(*this);
    GenerateDestructorCallsForCurrentStatement();
    sequenceSecond = prevSequenceSecond;
    if (!node.Second()->Generated())
    {
        node.Second()->Accept(*this);
    }
    node.DestructTemporaries(*emitter, &context);
}

void CodeGenerator::Visit(otava::symbols::BoundReturnStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    currentStatement = &node;
    bool prevEmitLineNumbers = emitLineNumbers;
    emitLineNumbers = false;
    if (node.GetExpr())
    {
        node.GetExpr()->Accept(*this);
        GenerateDestructorCallsForCurrentStatement();
        otava::intermediate::Value* returnValue = emitter->Stack().Pop();
        if (sequenceSecond)
        {
            sequenceSecond->SetGenerated();
            node.DestructTemporaries(*emitter, &context);
            sequenceSecond->Accept(*this);
        }
        ExitBlocks(currentBlockId, -1, node.GetFullSpan());
        emitter->EmitRet(returnValue);
        lastInstructionWasRet = true;
    }
    else
    {
        ExitBlocks(currentBlockId, -1, node.GetFullSpan());
        node.DestructTemporaries(*emitter, &context);
        emitter->EmitRetVoid();
        lastInstructionWasRet = true;
    }
    otava::symbols::BoundCompoundStatementNode* body = boundFunction->Body();
    otava::symbols::BoundStatementNode* lastStatement = nullptr;
    if (!body->Statements().empty())
    {
        lastStatement = body->Statements().back().get();
    }
    if (lastStatement && lastStatement != &node)
    {
        otava::intermediate::BasicBlock* nextBlock = emitter->CreateBasicBlock();
        emitter->SetCurrentBasicBlock(nextBlock);
        basicBlockOpen = true;
        lastInstructionWasRet = false;
    }
    latestRet = &node;
    emitLineNumbers = prevEmitLineNumbers;
}

void CodeGenerator::Visit(otava::symbols::BoundBreakStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    currentStatement = &node;
    ExitBlocks(currentBlockId, breakBlockId + 1, node.GetFullSpan());
    node.DestructTemporaries(*emitter, &context);
    if (node.Parent())
    {
        node.Parent()->DestructTemporaries(*emitter, &context);
        int index = node.Parent()->IndexOf(&node);
        if (index > 0)
        {
            otava::symbols::BoundCompoundStatementNode* parent = static_cast<otava::symbols::BoundCompoundStatementNode*>(node.Parent());
            otava::symbols::BoundStatementNode* prev = parent->Statements()[index - 1].get();
            if (prev->EndsWithTerminator())
            {
                return;
            }
        }
    }
    emitter->EmitJump(breakBlock);
    if (!otava::symbols::InDirectSwitchStatement(&node) || (latestRet && !latestRet->IsConditionalStatementInBlock(latestRet->Block())))
    {
        otava::intermediate::BasicBlock* nextBlock = emitter->CreateBasicBlock();
        emitter->SetCurrentBasicBlock(nextBlock);
        basicBlockOpen = true;
    }
}

void CodeGenerator::Visit(otava::symbols::BoundContinueStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    currentStatement = &node;
    ExitBlocks(currentBlockId, continueBlockId + 1, node.GetFullSpan());
    node.DestructTemporaries(*emitter, &context);
    if (node.Parent())
    {
        node.Parent()->DestructTemporaries(*emitter, &context);
    }
    emitter->EmitJump(continueBlock);
}

void CodeGenerator::Visit(otava::symbols::BoundConstructionStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    currentStatement = &node;
    node.ConstructorCall()->Accept(*this);
    if (node.DestructorCall())
    {
        blockExits[currentBlockId]->AddDestructorCall(node.DestructorCall());
    }
    GenerateDestructorCallsForCurrentStatement();
    node.DestructTemporaries(*emitter, &context);
}

void CodeGenerator::Visit(otava::symbols::BoundExpressionStatementNode& node)
{
    if (!inLineNumberCode)
    {
        StatementPrefix();
        SetCurrentLineNumber(node.GetFullSpan());
    }
    if (!node.GetExpr()) return;
    currentStatement = &node;
    node.GetExpr()->Accept(*this);
    if (node.GetExpr()->HasValue(&context))
    {
        emitter->Stack().Pop();
    }
    GenerateDestructorCallsForCurrentStatement();
    node.DestructTemporaries(*emitter, &context);
}

void CodeGenerator::Visit(otava::symbols::BoundSetVPtrStatementNode& node)
{
    StatementPrefix();
    SetCurrentLineNumber(node.GetFullSpan());
    currentStatement = &node;
    node.ThisPtr()->Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    otava::intermediate::Value* thisPtr = emitter->Stack().Pop();
    otava::symbols::TypeSymbol* thisPtrType = node.ThisPtr()->GetType()->GetBaseType(&context);
    if (thisPtrType->IsClassTypeSymbol())
    {
        otava::symbols::ClassTypeSymbol* classType = static_cast<otava::symbols::ClassTypeSymbol*>(thisPtrType);
        std::int32_t vptrIndex = classType->VPtrIndex();
        otava::symbols::ClassTypeSymbol* forClass = node.GetClass();
        otava::intermediate::Value* ptr = emitter->EmitElemAddr(thisPtr, emitter->EmitLong(vptrIndex));
        otava::intermediate::Value* vptr = emitter->EmitBitcast(forClass->GetVTabVariable(*emitter, &context),
            emitter->MakePtrType(emitter->GetVoidType()));
        emitter->EmitStore(vptr, ptr);
        otava::symbols::ClassTypeSymbol* vptrHolderClass = node.GetVPtrHolderClass();
        std::pair<bool, std::int64_t> p = otava::symbols::Delta(vptrHolderClass, forClass, *emitter, &context);
        bool succeeded = p.first;
        std::int64_t delta = p.second;
        if (!succeeded)
        {
            otava::symbols::ThrowException("classes '" + forClass->FullName(&context) + "' and '" + vptrHolderClass->FullName(&context) +
                "' have no inheritance relationship", node.GetFullSpan(), &context);
        }
        std::int32_t deltaIndex = classType->DeltaIndex();
        otava::intermediate::Value* deltaPtrElem = emitter->EmitElemAddr(thisPtr, emitter->EmitLong(deltaIndex));
        otava::intermediate::Value* deltaPtr = emitter->EmitBitcast(deltaPtrElem, emitter->MakePtrType(emitter->GetLongType()));
        otava::intermediate::Value* deltaValue = emitter->EmitLong(delta);
        emitter->EmitStore(deltaValue, deltaPtr);
    }
    else
    {
        otava::symbols::ThrowException("class type symbol expected", node.GetFullSpan(), &context);
    }
    node.DestructTemporaries(*emitter, &context);
}

void CodeGenerator::Visit(otava::symbols::BoundLiteralNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    if (inLineNumberCode)
    {
        node.SetValue(context.GetEvaluationContext()->GetIntegerValue(line, std::to_string(line),
            context.GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(otava::symbols::FundamentalTypeKind::intType, &context), &context));
    }
    else
    {
        node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    }
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundStringLiteralNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundVariableNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundParentVariableNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundParameterNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundParentParameterNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundEnumConstant& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundMemberExprNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundFunctionCallNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    if (node.IsNoReturnFunctionCall())
    {
        EmitReturn(node.GetFullSpan());
    }
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundFunctionPtrCallNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundInvokeNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundExpressionSequenceNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundConversionNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundAddressOfNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundDereferenceNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundPtrToRefNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundConstructTemporaryNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundConstructExpressionNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundDefaultInitNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundTemporaryNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    node.Load(*emitter, otava::symbols::OperationFlags::none, node.GetFullSpan(), &context);
    GenJumpingBoolCode();
}

void CodeGenerator::Visit(otava::symbols::BoundConjunctionNode& boundConjunction)
{
    SetCurrentLineNumber(boundConjunction.GetFullSpan());
    if (genJumpingBoolCode)
    {
        otava::intermediate::BasicBlock* rightBlock = emitter->CreateBasicBlock();
        otava::intermediate::BasicBlock* prevTrueBlock = trueBlock;
        trueBlock = rightBlock;
        boundConjunction.Left()->Accept(*this);
        trueBlock = prevTrueBlock;
        emitter->SetCurrentBasicBlock(rightBlock);
        boundConjunction.Right()->Accept(*this);
    }
    else
    {
        boundConjunction.Load(*emitter, otava::symbols::OperationFlags::none, boundConjunction.GetFullSpan(), &context);
    }
}

void CodeGenerator::Visit(otava::symbols::BoundDisjunctionNode& boundDisjunction)
{
    SetCurrentLineNumber(boundDisjunction.GetFullSpan());
    if (genJumpingBoolCode)
    {
        otava::intermediate::BasicBlock* rightBlock = emitter->CreateBasicBlock();
        otava::intermediate::BasicBlock* prevFalseBlock = falseBlock;
        falseBlock = rightBlock;
        boundDisjunction.Left()->Accept(*this);
        emitter->SetCurrentBasicBlock(rightBlock);
        falseBlock = prevFalseBlock;
        boundDisjunction.Right()->Accept(*this);
    }
    else
    {
        boundDisjunction.Load(*emitter, otava::symbols::OperationFlags::none, boundDisjunction.GetFullSpan(), &context);
    }
}

void CodeGenerator::Visit(otava::symbols::BoundConditionalExprNode& boundConditionalExpr)
{
    SetCurrentLineNumber(boundConditionalExpr.GetFullSpan());
    boundConditionalExpr.Load(*emitter, otava::symbols::OperationFlags::none, boundConditionalExpr.GetFullSpan(), &context);
}

void CodeGenerator::Visit(otava::symbols::BoundGlobalVariableDefinitionNode& node)
{
    SetCurrentLineNumber(node.GetFullSpan());
    otava::symbols::VariableSymbol* variable = node.GetGlobalVariable();
    otava::symbols::TypeSymbol* type = otava::symbols::ResolveFwdDeclaredType(variable->GetType(&context), node.GetFullSpan(), &context);
    if (type->IsForwardClassDeclarationSymbol()) return;
    variable->SetDeclaredType(type, &context);
    otava::intermediate::Value* initializer = nullptr;
    otava::intermediate::Type* irType = variable->GetType(&context)->IrType(*emitter, node.GetFullSpan(), &context);
    if (variable->GetValue(&context) && !variable->GetType(&context)->IsClassTypeSymbol())
    {
        initializer = variable->GetValue(&context)->IrValue(*emitter, node.GetFullSpan(), &context);
    }
    else
    {
        initializer = irType->MakeDefaultValue(*emitter->GetIntermediateContext());
    }
    otava::intermediate::Value* irVariable = emitter->EmitGlobalVariable(irType, variable->IrName(&context), initializer);
    emitter->SetIrObject(variable, irVariable);
}

void CodeGenerator::Visit(otava::symbols::BoundGotoStatementNode& node)
{
    otava::intermediate::BasicBlock* bb = node.GetBB(*emitter);
    emitter->EmitJump(bb);
}

void CodeGenerator::Visit(otava::symbols::BoundLabeledStatementNode& node)
{
    otava::intermediate::BasicBlock* bb = node.GetBB(*emitter);
    emitter->EmitJump(bb);
    emitter->SetCurrentBasicBlock(bb);
    basicBlockOpen = true;
    node.Statement()->Accept(*this);
}

std::string GenerateCode(otava::symbols::Context& context, const std::string& config, int optLevel, bool verbose, std::string& mainIrName,
    int& mainFunctionParams, bool globalMain, const std::vector<std::string>& compileUnitInitFnNames, const std::set<std::string>& configurations)
{
    CodeGenerator codeGenerator(context, config, optLevel, verbose, mainIrName, mainFunctionParams, globalMain, compileUnitInitFnNames, configurations);
    context.GetBoundCompileUnit()->Accept(codeGenerator);
    return codeGenerator.GetAsmFileName();
}

} // namespace otava::codegen
