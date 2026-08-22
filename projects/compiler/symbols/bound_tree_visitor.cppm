// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.bound_tree_visitor;

import std;

export namespace otava::symbols {

class BoundCompileUnitNode;
class BoundFunctionNode;
class BoundClassNode;
class BoundCtorInitializerNode;
class BoundDtorTerminatorNode;
class BoundEmptyStatementNode;
class BoundCompoundStatementNode;
class BoundIfStatementNode;
class BoundSwitchStatementNode;
class BoundCaseStatementNode;
class BoundDefaultStatementNode;
class BoundWhileStatementNode;
class BoundDoStatementNode;
class BoundForStatementNode;
class BoundSequenceStatementNode;
class BoundBreakStatementNode;
class BoundContinueStatementNode;
class BoundReturnStatementNode;
class BoundGotoStatementNode;
class BoundLabeledStatementNode;
class BoundConstructionStatementNode;
class BoundExpressionStatementNode;
class BoundSetVPtrStatementNode;
class BoundAliasDeclarationStatementNode;
class BoundValueExpressionNode;
class BoundLiteralNode;
class BoundStringLiteralNode;
class BoundVariableNode;
class BoundParentVariableNode;
class BoundParameterNode;
class BoundParentParameterNode;
class BoundEnumConstant;
class BoundFunctionGroupNode;
class BoundClassGroupNode;
class BoundAliasGroupNode;
class BoundTypeNode;
class BoundMemberExprNode;
class BoundFunctionCallNode;
class BoundEmptyFunctionCallNode;
class BoundFunctionPtrCallNode;
class BoundInvokeNode;
class BoundExpressionSequenceNode;
class BoundExpressionListNode;
class BoundConjunctionNode;
class BoundDisjunctionNode;
class BoundConditionalExprNode;
class BoundConversionNode;
class BoundAddressOfNode;
class BoundDereferenceNode;
class BoundRefToPtrNode;
class BoundPtrToRefNode;
class BoundDefaultInitNode;
class BoundTemporaryNode;
class BoundConstructExpressionNode;
class BoundConstructTemporaryNode;
class BoundDestructTemporariesNode;
class BoundGlobalVariableDefinitionNode;
class BoundEmptyDestructorNode;
class BoundFunctionValueNode;
class BoundVariableAsVoidPtrNode;
class BoundOperatorFnNode;

class BoundTreeVisitor
{
public:
    virtual ~BoundTreeVisitor();
    virtual void Visit(BoundCompileUnitNode& node) {}
    virtual void Visit(BoundFunctionNode& node) {}
    virtual void Visit(BoundClassNode& node) {}
    virtual void Visit(BoundCtorInitializerNode& node) {}
    virtual void Visit(BoundDtorTerminatorNode& node) {}
    virtual void Visit(BoundEmptyStatementNode& node) {}
    virtual void Visit(BoundCompoundStatementNode& node) {}
    virtual void Visit(BoundIfStatementNode& node) {}
    virtual void Visit(BoundSwitchStatementNode& node) {}
    virtual void Visit(BoundCaseStatementNode& node) {}
    virtual void Visit(BoundDefaultStatementNode& node) {}
    virtual void Visit(BoundWhileStatementNode& node) {}
    virtual void Visit(BoundDoStatementNode& node) {}
    virtual void Visit(BoundForStatementNode& node) {}
    virtual void Visit(BoundSequenceStatementNode& node) {}
    virtual void Visit(BoundBreakStatementNode& node) {}
    virtual void Visit(BoundContinueStatementNode& node) {}
    virtual void Visit(BoundReturnStatementNode& node) {}
    virtual void Visit(BoundGotoStatementNode& node) {}
    virtual void Visit(BoundLabeledStatementNode& node) {}
    virtual void Visit(BoundConstructionStatementNode& node) {}
    virtual void Visit(BoundExpressionStatementNode& node) {}
    virtual void Visit(BoundSetVPtrStatementNode& node) {}
    virtual void Visit(BoundAliasDeclarationStatementNode& node) {}
    virtual void Visit(BoundValueExpressionNode& node) {}
    virtual void Visit(BoundLiteralNode& node) {}
    virtual void Visit(BoundStringLiteralNode& node) {}
    virtual void Visit(BoundVariableNode& node) {}
    virtual void Visit(BoundParentVariableNode& node) {}
    virtual void Visit(BoundParameterNode& node) {}
    virtual void Visit(BoundParentParameterNode& node) {}
    virtual void Visit(BoundEnumConstant& node) {}
    virtual void Visit(BoundFunctionGroupNode& node) {}
    virtual void Visit(BoundClassGroupNode& node) {}
    virtual void Visit(BoundAliasGroupNode& node) {}
    virtual void Visit(BoundTypeNode& node) {}
    virtual void Visit(BoundMemberExprNode& node) {}
    virtual void Visit(BoundFunctionCallNode& node) {}
    virtual void Visit(BoundEmptyFunctionCallNode& node) {}
    virtual void Visit(BoundFunctionPtrCallNode& node) {}
    virtual void Visit(BoundInvokeNode& node) {}
    virtual void Visit(BoundExpressionSequenceNode& node) {}
    virtual void Visit(BoundExpressionListNode& node) {}
    virtual void Visit(BoundConjunctionNode& node) {}
    virtual void Visit(BoundDisjunctionNode& node) {}
    virtual void Visit(BoundConditionalExprNode& node) {}
    virtual void Visit(BoundConversionNode& node) {}
    virtual void Visit(BoundAddressOfNode& node) {}
    virtual void Visit(BoundDereferenceNode& node) {}
    virtual void Visit(BoundRefToPtrNode& node) {}
    virtual void Visit(BoundPtrToRefNode& node) {}
    virtual void Visit(BoundDefaultInitNode& node) {}
    virtual void Visit(BoundTemporaryNode& node) {}
    virtual void Visit(BoundConstructTemporaryNode& node) {}
    virtual void Visit(BoundDestructTemporariesNode& node) {}
    virtual void Visit(BoundConstructExpressionNode& node) {}
    virtual void Visit(BoundGlobalVariableDefinitionNode& node) {}
    virtual void Visit(BoundEmptyDestructorNode& node) {}
    virtual void Visit(BoundFunctionValueNode& node) {}
    virtual void Visit(BoundVariableAsVoidPtrNode& node) {}
    virtual void Visit(BoundOperatorFnNode& node) {}
};

class DefaultBoundTreeVisitor : public BoundTreeVisitor
{
public:
    void Visit(BoundCompileUnitNode& node) override;
    void Visit(BoundFunctionNode& node) override;
    void Visit(BoundCompoundStatementNode& node) override;
    void Visit(BoundIfStatementNode& node) override;
    void Visit(BoundSwitchStatementNode& node) override;
    void Visit(BoundCaseStatementNode& node) override;
    void Visit(BoundDefaultStatementNode& node) override;
    void Visit(BoundWhileStatementNode& node) override;
    void Visit(BoundDoStatementNode& node) override;
    void Visit(BoundForStatementNode& node) override;
    void Visit(BoundSequenceStatementNode& node) override;
    void Visit(BoundReturnStatementNode& node) override;
    void Visit(BoundConstructionStatementNode& node) override;
    void Visit(BoundExpressionStatementNode& node) override;
    void Visit(BoundMemberExprNode& node) override;
};

} // namespace otava::symbols
