// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.optimizer.inliner;

import std;
import otava.intermediate.code;
import otava.intermediate.context;

export namespace otava::optimizer {

void Inline(otava::intermediate::Function* fn, otava::intermediate::IntermediateContext* context);

} // otava::optimizer
