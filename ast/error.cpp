// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.error;

namespace otava::ast {

bool exceptionThrown = false;

void SetExceptionThrown() noexcept
{
    exceptionThrown = true;
}

bool ExceptionThrown() noexcept
{
    return exceptionThrown;
}

} // namespace otava::ast
