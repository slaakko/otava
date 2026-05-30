// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.assembly.error;

bool exceptionThrown = false;

namespace otava::assembly {

bool ExceptionThrown()
{
    return exceptionThrown;
}

void SetExceptionThrown()
{
    exceptionThrown = true;
}

} // otava::assembly
