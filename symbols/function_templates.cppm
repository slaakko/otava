// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.function_templates;

import std;
import otava.symbols.id;
import otava.symbols.template_param_compare;
import soul.ast.span;
import otava.ast.node;

export namespace otava::symbols {

class FunctionDefinitionSymbol;
class FunctionSymbol;
class TemplateParameterSymbol;
class TypeSymbol;
class Context;

struct FunctionTemplateKey
{
    FunctionTemplateKey();
    FunctionTemplateKey(SymbolId functionTemplateId_, const std::vector<SymbolId>& templateArgumentTypeIds);
    SymbolId functionTemplateId;
    std::vector<SymbolId> templateArgumentTypeIds;
};

struct FunctionTemplateKeyHash
{
    size_t operator()(const FunctionTemplateKey& key) const noexcept;
};


struct FunctionTemplateKeyEqual
{
    bool operator()(const FunctionTemplateKey& left, const FunctionTemplateKey& right) const noexcept;
};

class FunctionTemplateRepository
{
public:
    FunctionTemplateRepository();
    FunctionDefinitionSymbol* GetFunctionDefinition(const FunctionTemplateKey& key, Context* context);
    void AddFunctionDefinition(const FunctionTemplateKey& key, FunctionDefinitionSymbol* functionDefinitionSymbol, otava::ast::Node* functionDefinitionNode);
private:
    std::unordered_map<FunctionTemplateKey, SymbolId, FunctionTemplateKeyHash, FunctionTemplateKeyEqual> functionTemplateMap;
    std::vector<std::unique_ptr<otava::ast::Node>> functionDefinitionNodes;
};

FunctionSymbol* InstantiateFunctionTemplate(FunctionSymbol* functionTemplate,
    const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual>& templateParameterMap, 
    const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
