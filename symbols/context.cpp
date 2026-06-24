// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.context;

import otava.symbols.declarator;
import otava.symbols.scope;
import otava.symbols.symbol_table;

namespace otava::symbols {

int GetOptLevel(int level, bool release) noexcept
{
    if (release)
    {
        if (level == -1)
        {
            return 2;
        }
        else
        {
            return level;
        }
    }
    else
    {
        return 0;
    }
}

RangeForBlockIds::RangeForBlockIds() : compoundBlockId(-1), forActionStatementId(-1), forStatementId(-1)
{
}

Context::Context() : 
    flags(ContextFlags::none), optLevel(-1), node(nullptr), boundCompileUnit(new BoundCompileUnitNode()),
    moduleMapper(nullptr), emitter(nullptr), traceInfo(nullptr),
    lexer(nullptr), module(nullptr), stdTypeFundamentalModule(nullptr), rejectTemplateId(false), nextBlockId(0), 
    currentBlockId(-1), currentProject(nullptr), parentFn(nullptr), parentBlockId(-1), memFunDefSymbolIndex(-1), fileMap(nullptr), 
    aliasType(nullptr), debugOutputStream(nullptr), ptr(nullptr), argType(nullptr), paramType(nullptr), nodeId(-1), statementBinder(nullptr),
    totalFunctionsCompiled(0), functionCallsInlined(0), functionsInlined(0), invokes(0), unresolvedInvokes(0), argIndex(0),
    boundFunctionSerial(0), trySerial(0), invokeSerial(0), cleanupSerial(0), resultSerial(0), labelSerial(0), ehReturnFromSerial(0), childControlResultSerial(0),
    conditionVariableSerial(0), streamInitVarSerial(0), instantiationQueue(nullptr), switchCondType(nullptr), declaredInitializerType(nullptr),
    parentStatementIndex(-1)
{
}

void Context::PushFlags()
{
    flagStack.push(flags);
}

void Context::PopFlags()
{
    flags = flagStack.top();
    flagStack.pop();
}

void Context::PushSetFlag(ContextFlags flag)
{
    PushFlags();
    SetFlag(flag);
}

void Context::PushResetFlag(ContextFlags flag)
{
    PushFlags();
    ResetFlag(flag);
}

int Context::OptLevel() const noexcept
{
    return GetOptLevel(optLevel, ReleaseConfig());
}

OperationRepository* Context::GetOperationRepository() const noexcept
{
    return GetBoundCompileUnit()->GetOperationRepository();
}

void Context::PushStatementBinder(StatementBinder* statementBinder_)
{
    statementBinders.push_back(statementBinder);
    statementBinder = statementBinder_;
}

void Context::PopStatementBinder()
{
    statementBinder = statementBinders.back();
    statementBinders.pop_back();
}

StatementBinder* Context::GetParentStatementBinder() const noexcept
{
    if (!statementBinders.empty())
    {
        return statementBinders.back();
    }
    else
    {
        return nullptr;
    }
}

bool Context::EnableNoDeclSpecFunctionDeclaration() const noexcept
{
    Scope* currentScope = GetSymbolTable()->CurrentScope();
    if (currentScope->IsTemplateDeclarationScope())
    {
        Symbol* symbol = currentScope->GetSymbol();
        Symbol* parent = symbol->Parent(const_cast<Context*>(this));
        if (parent->GetScope()->IsClassScope())
        {
            return true;
        }
    }
    return false;
}

bool Context::EnableNoDeclSpecFunctionDefinition() const noexcept
{
    Scope* currentScope = GetSymbolTable()->CurrentScope();
    if (currentScope->IsTemplateDeclarationScope())
    {
        Symbol* symbol = currentScope->GetSymbol();
        Symbol* parent = symbol->Parent(const_cast<Context*>(this));
        if (parent->GetScope()->IsNamespaceScope() || parent->GetScope()->IsClassScope())
        {
            return true;
        }
    }
    return false;
}

void Context::PushNode(otava::ast::Node* node_)
{
    nodeStack.push(node);
    node = node_;
}

void Context::PopNode()
{
    node = nodeStack.top();
    nodeStack.pop();
}

BoundExpressionNode* Context::GetThisPtr(const soul::ast::FullSpan& fullSpan)
{
    FunctionDefinitionSymbol* function = boundFunction->GetFunctionDefinitionSymbol();
    if (function)
    {
        bool invokeOrTryCatch = GetFlag(ContextFlags::invoke | ContextFlags::tryCatch);
        if (invokeOrTryCatch)
        {
            FunctionDefinitionSymbol* parentFn = function->ParentFn();
            if (parentFn)
            {
                int level = -1;
                FunctionDefinitionSymbol* parent = parentFn;
                while (parent)
                {
                    parentFn = parent;
                    parent = parent->ParentFn();
                    ++level;
                }
                ParameterSymbol* parentThisParam = parentFn->ThisParam(this);
                if (parentThisParam)
                {
                    BoundParentParameterNode* parentThisPtr = new BoundParentParameterNode(parentThisParam, fullSpan, parentThisParam->GetType(this));
                    parentThisPtr->SetLevel(level);
                    return parentThisPtr;
                }
            }
        }
        ParameterSymbol* thisParam = function->ThisParam(this);
        if (thisParam)
        {
            return new BoundParameterNode(thisParam, fullSpan, thisParam->GetType(this));
        }
    }
    return nullptr;
}

void Context::SetDeclarationList(otava::ast::Node* node, DeclarationList* declarations)
{
    declarationMap[node] = std::unique_ptr<DeclarationList>(declarations);
}

std::unique_ptr<DeclarationList> Context::ReleaseDeclarationList(otava::ast::Node* node)
{
    std::unique_ptr<DeclarationList> declarationList;
    auto it = declarationMap.find(node);
    if (it != declarationMap.cend())
    {
        declarationList = std::move(it->second);
    }
    declarationMap.erase(node);
    return declarationList;
}

bool Context::IsConstructorNameNode(otava::ast::Node* node) const
{
    if (!GetFlag(ContextFlags::parsingParameters) && !GetFlag(ContextFlags::retMemberDeclSpecifiers) && !GetFlag(ContextFlags::parsingTemplateId))
    {
        Scope* currentScope = GetSymbolTable()->CurrentScope();
        if (currentScope->IsClassScope())
        {
            Symbol* symbol = currentScope->GetSymbol();
            if (symbol->Name() == node->Str())
            {
                return true;
            }
        }
    }
    return false;
}

void Context::SetCurrentConfig(const std::string& config)
{
    currentConfig = config;
}

void Context::SetCurrentConfigurations(const std::set<std::string>& configurations)
{
    currentConfigurations = configurations;
}

Module* Context::GetModule(const std::string& moduleName) const
{
    Module* currentModule = GetModule();
    if (currentModule->Name() == moduleName) return currentModule;
    return moduleMapper->GetModule(moduleName, currentConfig, OptLevel(), currentConfigurations, const_cast<Context*>(this));
}

Module* Context::GetStdTypeFundamentalModule() const
{
    if (!stdTypeFundamentalModule)
    {
        stdTypeFundamentalModule = GetModule("std.type.fundamental");
    }
    return stdTypeFundamentalModule;
}

FunctionSymbol* Context::GetSpecialization(otava::ast::Node* functionNode) const noexcept
{
    auto it = specializationMap.find(functionNode);
    if (it != specializationMap.end())
    {
        FunctionSymbol* specialization = it->second;
        return specialization;
    }
    else
    {
        return nullptr;
    }
}

void Context::SetSpecialization(FunctionSymbol* specialization, otava::ast::Node* functionNode)
{
    if (functionNode)
    {
        specializationMap[functionNode] = specialization;
    }
}

void Context::RemoveSpecialization(otava::ast::Node* functionNode)
{
    specializationMap.erase(functionNode);
}

ClassTemplateSpecializationSymbol* Context::GetClassTemplateSpecialization(otava::ast::Node* functionNode) const noexcept
{
    auto it = classTemplateSpecializationMap.find(functionNode);
    if (it != classTemplateSpecializationMap.end())
    {
        ClassTemplateSpecializationSymbol* sp = it->second;
        return sp;
    }
    else
    {
        return nullptr;
    }
}

void Context::SetClassTemplateSpecialization(otava::ast::Node* functionNode, ClassTemplateSpecializationSymbol* sp)
{
    if (functionNode)
    {
        classTemplateSpecializationMap[functionNode] = sp;
    }
}

void Context::RemoveClassTemplateSpecialization(otava::ast::Node* functionNode)
{
    classTemplateSpecializationMap.erase(functionNode);
}

void Context::PushBoundFunction(BoundFunctionNode* boundFunction_)
{
    boundFunctionStack.push(std::unique_ptr<BoundFunctionNode>(boundFunction.release()));
    boundFunction.reset(boundFunction_);
    boundFunction->SetSerial(boundFunctionSerial++);
}

void Context::PopBoundFunction()
{
    boundFunction.reset(boundFunctionStack.top().release());
    boundFunctionStack.pop();
}

void Context::AddBoundVTabFunction(BoundFunctionNode* node)
{
    boundVTabFunctions.push_back(std::unique_ptr<BoundFunctionNode>(node));
}

void Context::ClearBoundVTabFunctions()
{
    boundVTabFunctions.clear();
}

BoundFunctionNode* Context::GetBoundFunction() const noexcept
{
    return boundFunction.get();
}

BoundFunctionNode* Context::ReleaseBoundFunction() noexcept
{
    return boundFunction.release();
}

void Context::PushBlockId(int blockId)
{
    blockIdStack.push(currentBlockId);
    currentBlockId = blockId;
}

void Context::PopBlockId()
{
    currentBlockId = blockIdStack.top();
    blockIdStack.pop();
}

void Context::PushParentFn(FunctionDefinitionSymbol* parentFn_)
{
    parentFnStack.push(parentFn);
    parentFn = parentFn_;
}

void Context::PopParentFn()
{
    parentFn = parentFnStack.top();
    parentFnStack.pop();
}

void Context::PushParentBlockId(int blockId)
{
    parentBlockIdStack.push(parentBlockId);
    parentBlockId = blockId;
}

void Context::PopParentBlockId()
{
    parentBlockId = parentBlockIdStack.top();
    parentBlockIdStack.pop();
}

void Context::PushTemplateParameterMap(std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>* templateParamMap)
{
    templateParameterMapStack.push(templateParameterMap);
    templateParameterMap = templateParamMap;
}

void Context::PopTemplateParameterMap()
{
    templateParameterMap = templateParameterMapStack.top();
    templateParameterMapStack.pop();
}

void Context::PushNodeId(std::int64_t nodeId_)
{
    nodeIdStack.push(nodeId);
    nodeId = nodeId_;
}

void Context::PopNodeId()
{
    nodeId = nodeIdStack.top();
    nodeIdStack.pop();
}

void Context::SetInstantiationIrName(const std::string& instantiationIrName_)
{
    instantiationIrName = instantiationIrName_;
}

void Context::AddTemporaryAliasType(AliasTypeSymbol* temporaryAliasType)
{
    temporaryAliasTypes.push_back(temporaryAliasType);
}

void Context::ClearTemporaryAliasTypes()
{
    temporaryAliasTypes.clear();
}

void Context::SetFileName(const std::string& fileName_)
{
    fileName = fileName_;
}

std::string Context::FileName() const
{
    if (lexer)
    {
        return lexer->FileName();
    }
    else
    {
        return fileName;
    }
}

void Context::SetFunction(const std::string& function_)
{
    function = function_;
}

int Context::Line() const
{
    if (lexer)
    {
        return lexer->Line();
    }
    return 0;
}

void Context::PushSwitchCondType(TypeSymbol* switchCondType_)
{
    switchCondTypeStack.push(switchCondType);
    switchCondType = switchCondType_;
}

void Context::PopSwitchCondType()
{
    switchCondType = switchCondTypeStack.top();
    switchCondTypeStack.pop();
}

RangeForBlockIds& Context::GetRangeForBlockIds(const util::uuid& rangeForId)
{
    return rangeForBlockIdMap[rangeForId];
}

void Context::PushResultVarName(const std::string& resultVarName_)
{
    resultVariableNameStack.push(resultVariableName);
    resultVariableName = resultVarName_;
}

void Context::PopResultVarName()
{
    resultVariableName = resultVariableNameStack.top();
    resultVariableNameStack.pop();
}

void Context::PushChildControlResultVarName(const std::string& childControlResultVarName)
{
    childControlResultVariableNameStack.push(childControlResultVariableName);
    childControlResultVariableName = childControlResultVarName;
}

void Context::PopChildControlResultVarName()
{
    childControlResultVariableName = childControlResultVariableNameStack.top();
    childControlResultVariableNameStack.pop();
}

std::string Context::NextResultVarName()
{
    std::string resultVariableName = "__result" + std::to_string(NextResultSerial());
    return resultVariableName;
}

std::string Context::NextEhReturnFromVarName()
{
    std::string ehReturnFromVarName = "__eh_return_from" + std::to_string(NextEhReturnFromSerial());
    return ehReturnFromVarName;
}

std::string Context::NextChildControlResultVarName()
{
    std::string childControlResultVarName = "__child_control_result" + std::to_string(NextChildControlResultSerial());
    return childControlResultVarName;
}

std::string Context::NextConditionVariableName()
{
    std::string conditionVariableName = "__condition" + std::to_string(NextConditionVariableSerial());
    return conditionVariableName;
}

std::string Context::NextStreamInitVarName()
{
    std::string streamInitVariableName = "__sentry" + std::to_string(NextStreamInitVarSerial());
    return streamInitVariableName;
}

} // namespace otava::symbols
