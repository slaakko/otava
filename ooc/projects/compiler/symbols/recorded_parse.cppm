// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.recorded_parse;

import std;
import otava.symbols.context;
import otava.ast.node;
import otava.ast.statement;
import otava.ast.classes;
import otava.ast.function;

export namespace otava::symbols {

using RecordedParseCompoundStatementFn = void (*)(otava::ast::CompoundStatementNode* compoundStatementNode, Context* context);

void SetRecordedParseCompoundStatementFn(RecordedParseCompoundStatementFn fn) noexcept;

void RecordedParseCompoundStatement(otava::ast::CompoundStatementNode* compoundStatementNode, Context* context);

using RecordedParseCtorInitializerFn = void (*)(otava::ast::ConstructorInitializerNode* ctorInitializerNode, Context* context);

void SetRecordedParseCtorInitializerFn(RecordedParseCtorInitializerFn fn) noexcept;

void RecordedParseCtorInitializer(otava::ast::ConstructorInitializerNode* ctorInitializerNode, Context* context);

} // namespace otava::symbol
