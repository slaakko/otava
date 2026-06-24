// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_return_path_checker;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.evaluator;
import otava.symbols.value;
import otava.ast.statement;
import otava.ast.visitor;

namespace otava::symbols {

bool TerminatesFunction(otava::ast::Node* statementNode, Context* context, bool inForEverLoop);

class CaseOrDefaultTerminationChecker : public otava::ast::DefaultVisitor
{
public:
    CaseOrDefaultTerminationChecker();
    void Visit(otava::ast::CompoundStatementNode& node) override;
    void Visit(otava::ast::ReturnStatementNode& node) override;
    void Visit(otava::ast::ThrowExprNode& node) override;
    void Visit(otava::ast::BreakStatementNode& node) override;
    void Visit(otava::ast::ContinueStatementNode& node) override;
    void Visit(otava::ast::IfStatementNode& node) override;
    inline bool Terminates() const { return terminates; }
private:
    bool terminates;
};

CaseOrDefaultTerminationChecker::CaseOrDefaultTerminationChecker() : terminates(false)
{
}

void CaseOrDefaultTerminationChecker::Visit(otava::ast::CompoundStatementNode& node)
{
    int n = node.Count();
    for (int i = 0; i < n; ++i)
    {
        otava::ast::Node* statementNode = node.Nodes()[i];
        if (TerminatesCaseOrDefault(statementNode))
        {
            terminates = true;
            break;
        }
    }
}

void CaseOrDefaultTerminationChecker::Visit(otava::ast::ReturnStatementNode& node)
{
    terminates = true;
}

void CaseOrDefaultTerminationChecker::Visit(otava::ast::ThrowExprNode& node)
{
    terminates = true;
}

void CaseOrDefaultTerminationChecker::Visit(otava::ast::BreakStatementNode& node)
{
    terminates = true;
}

void CaseOrDefaultTerminationChecker::Visit(otava::ast::ContinueStatementNode& node)
{
    terminates = true;
}

void CaseOrDefaultTerminationChecker::Visit(otava::ast::IfStatementNode& node)
{
    if (terminates) return;
    if (node.ElseStatement())
    {
        terminates = TerminatesCaseOrDefault(node.ThenStatement()) && TerminatesCaseOrDefault(node.ElseStatement());
    }
}

bool TerminatesCaseOrDefault(otava::ast::Node* statementNode)
{
    CaseOrDefaultTerminationChecker checker;
    statementNode->Accept(checker);
    return checker.Terminates();
}

bool IsAlwaysTrue(otava::ast::Node* condition, Context* context)
{
    Value* value = Evaluate(condition, context);
    if (value && value->IsBoolValue())
    {
        BoolValue* boolValue = static_cast<BoolValue*>(value);
        return boolValue->GetValue();
    }
    return false;
}

class FunctionTerminationChecker : public otava::ast::DefaultVisitor
{
public:
    FunctionTerminationChecker(Context* context_, bool inForEverLoop_);
    void Visit(otava::ast::CompoundStatementNode& node) override;
    void Visit(otava::ast::IfStatementNode& node) override;
    void Visit(otava::ast::WhileStatementNode& node) override;
    void Visit(otava::ast::DoStatementNode& node) override;
    void Visit(otava::ast::ForStatementNode& node) override;
    void Visit(otava::ast::ReturnStatementNode& node) override;
    void Visit(otava::ast::ThrowExprNode& node) override;
    inline bool Terminates() const { return terminates; }
private:
    Context* context;
    bool inForEverLoop;
    bool terminates;
};

FunctionTerminationChecker::FunctionTerminationChecker(Context* context_, bool inForEverLoop_) : context(context_), inForEverLoop(inForEverLoop_), terminates(false)
{
}

void FunctionTerminationChecker::Visit(otava::ast::CompoundStatementNode& node)
{
    int n = node.Count();
    for (int i = 0; i < n; ++i)
    {
        otava::ast::Node* statementNode = node.Nodes()[i];
        if (TerminatesFunction(statementNode, context, inForEverLoop))
        {
            terminates = true;
            break;
        }
    }
}

void FunctionTerminationChecker::Visit(otava::ast::IfStatementNode& node)
{
    if (inForEverLoop || node.ElseStatement())
    {
        if (TerminatesFunction(node.ThenStatement(), context, inForEverLoop) && inForEverLoop ||
            (node.ElseStatement() && TerminatesFunction(node.ElseStatement(), context, inForEverLoop)))
        {
            terminates = true;
        }
    }
}

void FunctionTerminationChecker::Visit(otava::ast::WhileStatementNode& node)
{
    if (IsAlwaysTrue(node.Condition(), context))
    {
        if (TerminatesFunction(node.Statement(), context, true))
        {
            terminates = true;
        }
    }
}

void FunctionTerminationChecker::Visit(otava::ast::DoStatementNode& node)
{
    if (IsAlwaysTrue(node.Expression(), context))
    {
        if (TerminatesFunction(node.Statement(), context, true))
        {
            terminates = true;
        }
    }
}

void FunctionTerminationChecker::Visit(otava::ast::ForStatementNode& node)
{
    if (!node.Condition() || IsAlwaysTrue(node.Condition(), context))
    {
        if (TerminatesFunction(node.Statement(), context, true))
        {
            terminates = true;
        }
    }
}

void FunctionTerminationChecker::Visit(otava::ast::ReturnStatementNode& node)
{
    terminates = true;
}

void FunctionTerminationChecker::Visit(otava::ast::ThrowExprNode& node)
{
    terminates = true;
}

bool TerminatesFunction(otava::ast::Node* statementNode, Context* context, bool inForEverLoop)
{
    FunctionTerminationChecker checker(context, inForEverLoop);
    statementNode->Accept(checker);
    return checker.Terminates();
}

class FunctionReturnPathChecker : public otava::ast::DefaultVisitor
{
public:
    FunctionReturnPathChecker(Context* context_);
    void Visit(otava::ast::CompoundStatementNode& node) override;
private:
    Context* context;
};

FunctionReturnPathChecker::FunctionReturnPathChecker(Context* context_) : context(context_)
{
}

void FunctionReturnPathChecker::Visit(otava::ast::CompoundStatementNode& node)
{
    int n = node.Count();
    if (n > 0)
    {
        for (int i = 0; i < n; ++i)
        {
            otava::ast::Node* statementNode = node.Nodes()[i];
            if (TerminatesFunction(statementNode, context, false))
            {
                return;
            }
        }
        ThrowException("not all control paths terminate in return statement or throw expression", node.GetFullSpan(), context);
    }
}

void CheckFunctionReturnPaths(otava::ast::Node* node, Context* context)
{
    FunctionReturnPathChecker checker(context);
    node->Accept(checker);
}

} // namespace otava::symbols
