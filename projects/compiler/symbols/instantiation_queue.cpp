// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.instantiation_queue;

namespace otava::symbols {

InstantiationQueue::InstantiationQueue()
{
}

void InstantiationQueue::EnqueueInstantiationRequest(FunctionSymbol* function, 
    const std::map<TemplateParameterSymbol*, TypeSymbol*, TemplateParamLess>& templateParameterMap)
{
    // TODO
}

std::vector<InstantiationRequest>* InstantiationQueue::GetRequests(FunctionSymbol* functionTemplate)
{
    // TODO
    return nullptr;
}


void InstantiateEnqueuedRequests(FunctionSymbol* functionTemplate, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
}

} // namespace otava::symbols
