// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_templates;

namespace otava::symbols {

FunctionSymbol* InstantiateFunctionTemplate(FunctionSymbol* functionTemplate,
    const std::map<TemplateParameterSymbol*, TypeSymbol*, TemplateParamLess>& templateParameterMap, const soul::ast::FullSpan& fullSpan, Context* context)
{
    // TODO
    return nullptr;
}

} // namespace otava::symbols
