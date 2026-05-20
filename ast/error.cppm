// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.error;

import std;

export namespace otava::ast {

void SetExceptionThrown() noexcept;
bool ExceptionThrown() noexcept;

} // namespace otava::ast
