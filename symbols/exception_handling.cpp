// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.exception_handling;

import otava.symbols.context;
import otava.symbols.statement_binder;
import otava.symbols.stmt_parser;
import otava.ast.expression;
import otava.ast.identifier;
import otava.ast.node;

namespace otava::symbols {

otava::ast::Node* MakeClonedRetValExprNode(otava::ast::Node* node, bool makeAddrOfNode)
{
    if (makeAddrOfNode)
    {
        return new otava::ast::UnaryExprNode(node->GetSpan(), node->FileIndex(), new otava::ast::AddrOfNode(node->GetSpan(), node->FileIndex()), node->Clone());
    }
    else
    {
        return node->Clone();
    }
}

std::unique_ptr<BoundStatementNode> ConvertReturnStatement(otava::ast::ReturnStatementNode* returnStatement,
    FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context)
{
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    functionDefinitionSymbol->SetFlag(FunctionSymbolFlags::containsReturnStatement);
    if (returnStatement->ReturnValue())
    {
        bool makeAddrOfNode = functionDefinitionSymbol->NonChildFunctionResultType(context)->IsReferenceType();
        otava::ast::BinaryExprNode* binaryExpr = new otava::ast::BinaryExprNode(returnStatement->GetSpan(), returnStatement->FileIndex(),
            new otava::ast::AssignNode(returnStatement->GetSpan(), returnStatement->FileIndex()),
            new otava::ast::IdentifierNode(returnStatement->GetSpan(), returnStatement->FileIndex(), context->ResultVarName()),
            MakeClonedRetValExprNode(returnStatement->ReturnValue(), makeAddrOfNode));
        otava::ast::ExpressionStatementNode* exprStatementNode = new otava::ast::ExpressionStatementNode(returnStatement->GetSpan(), returnStatement->FileIndex(),
            binaryExpr, nullptr, nullptr);
        std::string setChildControlResultStmtText;
        setChildControlResultStmtText = context->ChildControlResultVarName();
        setChildControlResultStmtText.append(" = std::child_control_result::ret;");
        std::unique_ptr<otava::ast::Node> setChildControlResultStmt = ParseStatement(setChildControlResultStmtText, context);
        std::unique_ptr<BoundStatementNode> boundExpressionStatement = BindStatement(exprStatementNode, functionDefinitionSymbol, context);
        std::unique_ptr<BoundStatementNode> boundSetChildControlResultStatement = BindStatement(setChildControlResultStmt.get(), functionDefinitionSymbol, context);
        std::unique_ptr<BoundReturnStatementNode> boundReturnStatement(new BoundReturnStatementNode(returnStatement->GetFullSpan()));
        std::unique_ptr<BoundSequenceStatementNode> boundSequenceStatement(new BoundSequenceStatementNode(returnStatement->GetFullSpan(),
            new BoundSequenceStatementNode(returnStatement->GetFullSpan(), boundExpressionStatement.release(),
                boundSetChildControlResultStatement.release()), boundReturnStatement.release()));
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
        return std::unique_ptr<BoundStatementNode>(boundSequenceStatement.release());
    }
    else
    {
        std::unique_ptr<BoundReturnStatementNode> boundReturnStatement(new BoundReturnStatementNode(returnStatement->GetFullSpan()));
        context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
        return std::unique_ptr<BoundStatementNode>(boundReturnStatement.release());
    }
}

} // namespace otava::symbols
