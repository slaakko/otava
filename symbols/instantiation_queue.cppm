// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.instantiation_queue;

import std;
import otava.symbols.template_param_compare;
import soul.ast.span;

export namespace otava::symbols {

class FunctionSymbol;
class TypeSymbol;
class Context;

class InstantiationRequest
{
public:
    InstantiationRequest(FunctionSymbol* function_, const std::map<TemplateParameterSymbol*, TypeSymbol*, TemplateParamLess>& templateParameterMap_);
    inline FunctionSymbol* Function() const noexcept { return function; }
    inline const std::map<TemplateParameterSymbol*, TypeSymbol*, TemplateParamLess>& TemplateParamMap() const noexcept { return templateParameterMap; }
private:
    FunctionSymbol* function;
    std::map<TemplateParameterSymbol*, TypeSymbol*, TemplateParamLess> templateParameterMap;
};

bool operator==(const InstantiationRequest& left, const InstantiationRequest& right) noexcept;

class InstantiationQueue
{
public:
    InstantiationQueue();
    void EnqueueInstantiationRequest(FunctionSymbol* function, const std::map<TemplateParameterSymbol*, TypeSymbol*, TemplateParamLess>& templateParameterMap);
    std::vector<InstantiationRequest>* GetRequests(FunctionSymbol* functionTemplate);
private:
    std::map<std::string, std::map<std::u32string, std::vector<InstantiationRequest>>> requestMap;
};

void InstantiateEnqueuedRequests(FunctionSymbol* functionTemplate, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
