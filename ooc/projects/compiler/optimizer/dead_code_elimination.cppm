// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.optimizer.dead_code_elimination;

import std;
import otava.intermediate.code;

export namespace otava::optimizer {

bool DeadCodeElimination(otava::intermediate::Function* fn);

} // otava::optimizer