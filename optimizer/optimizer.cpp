// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.optimizer;

import otava.optimizer.arithmetics;
import otava.optimizer.dead_code_elimination;
import otava.optimizer.error;
import otava.optimizer.identity_calls;
import otava.optimizer.inliner;
import otava.optimizer.jumps;
import otava.optimizer.locals;
import otava.intermediate.code;

namespace otava::optimizer {

Optimizer& Optimizer::Instance()
{
    static Optimizer instance;
    return instance;
}

Optimizer::Optimizer()
{
    optLevel = "2";
    optMap["0"] = Optimizations::o0;
    optMap["1"] = Optimizations::o1;
    optMap["2"] = Optimizations::o2;
    optMap["3"] = Optimizations::o3;
}

void Optimizer::SetOptimizations(const std::string& optLevel)
{
    this->optLevel = optLevel;
    auto it = optMap.find(optLevel);
    if (it != optMap.end())
    {
        otava::optimizer::SetOptimizations(it->second);
    }
    else
    {
        SetExceptionThrown();
        throw std::runtime_error("invalid optimization level '" + optLevel + "'");
    }
}

void Optimize(otava::intermediate::IntermediateContext* context)
{
    otava::intermediate::Code* code = &context->GetCode();
    otava::intermediate::Function* fn = code->FirstFunction();
    while (fn)
    {
        otava::intermediate::Function* next = fn->Next();
        if (fn->IsDefined())
        {
            if (HasOptimization(Optimizations::deadCodeElimination))
            {
                DeadCodeElimination(fn);
            }
            if (HasOptimization(Optimizations::identity))
            {
                OptimizeIdentityCalls(fn, context);
            }
            if (HasOptimization(Optimizations::inlining))
            {
                Inline(fn, context);
            }
            if (HasOptimization(Optimizations::moveLocalsToEntryBlock))
            {
                MoveLocalsToEntryBlock(fn);
            }
            if (HasOptimization(Optimizations::arithmetic))
            {
                OptimizeArithmetics(fn, context);
            }
            if (HasOptimization(Optimizations::jump))
            {
                OptimizeJumps(fn);
            }
            if (HasOptimization(Optimizations::deadCodeElimination))
            {
                DeadCodeElimination(fn);
            }
        }
        fn = next;
    }
}

} // otava::optimizer
