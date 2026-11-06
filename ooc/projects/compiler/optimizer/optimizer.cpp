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
import util.code_formatter;

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

void TryOptimize(otava::intermediate::Function* fn, otava::intermediate::IntermediateContext* context, util::CodeFormatter& formatter, const std::string& optFn)
{
    bool print = optFn == fn->Name();
    if (print)
    {
        formatter.WriteLine("====================================================================");
        formatter.WriteLine("before optimizations");
        formatter.WriteLine("====================================================================");
        formatter.WriteLine();
        fn->Write(formatter);
    }
    if (HasOptimization(Optimizations::deadCodeElimination))
    {
        if (DeadCodeElimination(fn))
        {
            if (print)
            {
                formatter.WriteLine();
                formatter.WriteLine("====================================================================");
                formatter.WriteLine("after dead code elimination");
                formatter.WriteLine("====================================================================");
                formatter.WriteLine();
                fn->Write(formatter);
            }
        }
    }
    if (HasOptimization(Optimizations::identity))
    {
        if (OptimizeIdentityCalls(fn, context))
        {
            if (print)
            {
                formatter.WriteLine();
                formatter.WriteLine("====================================================================");
                formatter.WriteLine("after removing identity calls");
                formatter.WriteLine("====================================================================");
                formatter.WriteLine();
                fn->Write(formatter);
            }
        }
    }
    if (HasOptimization(Optimizations::inlining))
    {
        if (Inline(fn, context))
        {
            if (print)
            {
                formatter.WriteLine();
                formatter.WriteLine("====================================================================");
                formatter.WriteLine("after inlining");
                formatter.WriteLine("====================================================================");
                formatter.WriteLine();
                fn->Write(formatter);
            }
        }
    }
    if (HasOptimization(Optimizations::moveLocalsToEntryBlock))
    {
        if (MoveLocalsToEntryBlock(fn))
        {
            if (print)
            {
                formatter.WriteLine();
                formatter.WriteLine("====================================================================");
                formatter.WriteLine("after moving locals to entry block");
                formatter.WriteLine("====================================================================");
                formatter.WriteLine();
                fn->Write(formatter);
            }
        }
    }
    if (HasOptimization(Optimizations::arithmetic))
    {
        if (OptimizeArithmetics(fn, context))
        {
            if (print)
            {
                formatter.WriteLine();
                formatter.WriteLine("====================================================================");
                formatter.WriteLine("after optimizing arithmetics");
                formatter.WriteLine("====================================================================");
                formatter.WriteLine();
                fn->Write(formatter);
            }
        }
    }
    if (HasOptimization(Optimizations::jump))
    {
        if (OptimizeJumps(fn))
        {
            if (print)
            {
                formatter.WriteLine();
                formatter.WriteLine("====================================================================");
                formatter.WriteLine("after optimizing jumps");
                formatter.WriteLine("====================================================================");
                formatter.WriteLine();
                fn->Write(formatter);
            }
        }
    }
    if (HasOptimization(Optimizations::deadCodeElimination))
    {
        DeadCodeElimination(fn);
    }
    if (print)
    {
        formatter.WriteLine();
        formatter.WriteLine("====================================================================");
        formatter.WriteLine("after all optimizations");
        formatter.WriteLine("====================================================================");
        formatter.WriteLine();
        fn->Write(formatter);
    }
}

void Optimize(otava::intermediate::IntermediateContext* context, std::ostream* outFile, const std::string& optFn)
{
    if (!outFile)
    {
        outFile = &std::cout;
    }
    util::CodeFormatter formatter(*outFile);
    otava::intermediate::Code* code = &context->GetCode();
    otava::intermediate::Function* fn = code->FirstFunction();
    while (fn)
    {
        otava::intermediate::Function* next = fn->Next();
        if (fn->IsDefined())
        {
            try
            {
                TryOptimize(fn, context, formatter, optFn);
            }
            catch (const std::exception& ex)
            {
                std::cout << "optimization of function '" << fn->Name() << "' failed: " << ex.what() << "\n";
            }
        }
        fn = next;
    }
}

} // otava::optimizer
