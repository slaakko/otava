// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.expression_binder;

import otava.symbols.bound_tree;

namespace otava::symbols {

std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context, SymbolGroupKind symbolGroups, Scope*& scope)
{
/*
    ExpressionBinder binder(context, symbolGroups);
    node->Accept(binder);
    scope = binder.GetScope();
    return binder.GetBoundExpression();
*/
    return std::unique_ptr<BoundExpressionNode>();
}

std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context)
{
    return BindExpression(node, context, false);
}

std::unique_ptr<BoundExpressionNode> BindExpression(otava::ast::Node* node, Context* context, bool booleanChild)
{
/*
    soul::ast::FullSpan fullSpan = node->GetFullSpan();
    Scope* scope = nullptr;
    bool acquireTemporaryDestructorCalls = context->GetFlag(ContextFlags::acquireTemporaryDestructorCalls);
    if (acquireTemporaryDestructorCalls)
    {
        context->PushResetFlag(ContextFlags::acquireTemporaryDestructorCalls);
    }
    std::unique_ptr<BoundExpressionNode> expr = BindExpression(node, context, SymbolGroupKind::all, scope);
    if (booleanChild && !expr->GetType()->IsBoolType())
    {
        FunctionSymbol* conversionFunction = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            context->GetSymbolTable()->GetFundamentalType(otava::symbols::FundamentalTypeKind::boolType), expr->GetType(),
            fullSpan, context);
        if (!conversionFunction)
        {
            ThrowException("expression must be convertible to Boolean type value", expr->GetFullSpan(), context);
        }
        if (conversionFunction->IsFunctionDefinitionSymbol())
        {
            std::unique_ptr<BoundFunctionCallNode> fnCall = CreateBoundConversionFunctionCall(conversionFunction, expr.release(), fullSpan, context);
            expr.reset(fnCall.release());
        }
        else
        {
            expr.reset(new BoundConversionNode(expr.release(), conversionFunction, fullSpan));
        }
    }
    if (expr)
    {
        expr->SetSource(node->Clone());
    }
    if (acquireTemporaryDestructorCalls)
    {
        if (!context->GetFlag(ContextFlags::invoke))
        {
            if (context->GetBoundFunction()->HasTemporaryDestructorCalls())
            {
                expr->SetDestructTemporariesNode(
                    new BoundDestructTemporariesNode(fullSpan, context->GetBoundFunction()->GetTemporaryDestructorCalls()));
                expr->SetTemporaryDestructorCallsObtained();
            }
        }
        context->PopFlags();
    }
    return expr;
*/
    return std::unique_ptr<BoundExpressionNode>();
}

void InitExpressionBinder()
{
    //OperatorGroupNameMap::Instance().Init();
}

bool MultiplicativeRightIdOperandNotFound(otava::ast::Node* op, otava::ast::Node* rightOperand, const soul::ast::FullSpan& fullSpan, Context* context)
{
/*
    if (!op->IsMulNode()) return false;
    if (!rightOperand->IsInvokeExprNode()) return false;
    otava::ast::InvokeExprNode* rightinvoke = static_cast<otava::ast::InvokeExprNode*>(rightOperand);
    if (!rightinvoke->Subject()->IsIdentifierNode()) return false;
    otava::ast::IdentifierNode* id = static_cast<otava::ast::IdentifierNode*>(rightinvoke->Subject());
    Symbol* symbol = context->GetSymbolTable()->CurrentScope()->Lookup(id->Str(), SymbolGroupKind::all, ScopeLookup::allScopes, fullSpan, context, LookupFlags::none);
    if (!symbol) return true;
    return false;
*/
    return false;
}

} // namespace otava::symbols
