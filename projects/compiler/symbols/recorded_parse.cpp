// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================
 
module otava.symbols.recorded_parse;

namespace otava::symbols {

RecordedParseCompoundStatementFn recordedParseCompoundStatementFn = nullptr;

void SetRecordedParseCompoundStatementFn(RecordedParseCompoundStatementFn fn) noexcept
{
    recordedParseCompoundStatementFn = fn;
}

void RecordedParseCompoundStatement(otava::ast::CompoundStatementNode* compoundStatementNode, Context* context)
{
    if (recordedParseCompoundStatementFn)
    {
        recordedParseCompoundStatementFn(compoundStatementNode, context);
    }
}

RecordedParseCtorInitializerFn recordedParseInitializerFn = nullptr;

void SetRecordedParseCtorInitializerFn(RecordedParseCtorInitializerFn fn) noexcept
{
    recordedParseInitializerFn = fn;
}

void RecordedParseCtorInitializer(otava::ast::ConstructorInitializerNode* ctorInitializerNode, Context* context)
{
    recordedParseInitializerFn(ctorInitializerNode, context);
}

} // namespace otava::symbol
