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

Context::Context() : 
    flags(ContextFlags::none), optLevel(-1), node(nullptr), moduleMapper(nullptr), lexer(nullptr), module(nullptr), stdTypeFundamentalModule(nullptr), 
    rejectTemplateId(false), nextBlockId(0), currentBlockId(-1), currentProject(nullptr), parentFn(nullptr), parentBlockId(-1), memFunDefSymbolIndex(-1),
    fileMap(nullptr), aliasType(nullptr), debugOutputStream(nullptr), ptr(nullptr), argType(nullptr), paramType(nullptr)
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

void Context::SetDeclarationList(otava::ast::Node* node, DeclarationList* declarations)
{
    // TODO
}

std::unique_ptr<DeclarationList> Context::ReleaseDeclarationList(otava::ast::Node* node)
{
    // TODO
    return std::unique_ptr<DeclarationList>();
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
    // TODO
    return nullptr;
}

void Context::SetSpecialization(FunctionSymbol* specialization, otava::ast::Node* functionNode)
{
    // TODO
}

void Context::RemoveSpecialization(otava::ast::Node* functionNode)
{
    // TODO
}

void Context::PushBoundFunction(BoundFunctionNode* boundFunction_)
{
    // TODO
}

void Context::PopBoundFunction()
{
    // TODO
}
BoundFunctionNode* Context::GetBoundFunction() const noexcept
{
    // TODO
    return nullptr;
}
BoundFunctionNode* Context::ReleaseBoundFunction() noexcept
{
    // TODO
    return nullptr;
}

EvaluationContext* Context::GetEvaluationContext() noexcept
{
    // TODO
    return nullptr;
}

} // namespace otava::symbols
