// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.optimizer.identity_calls;

import std;
import otava.intermediate.code;
import otava.intermediate.context;

export namespace otava::optimizer {

void OptimizeIdentityCalls(otava::intermediate::Function* fn, otava::intermediate::IntermediateContext* context);

} // otava::optimizer
