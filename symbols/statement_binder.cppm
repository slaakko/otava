// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.statement_binder;

import otava.symbols.bound_tree;
import otava.ast.node;
import otava.ast.visitor;
import std;

export namespace otava::symbols {

class FunctionDefinitionSymbol;
class BoundStatementNode;
class Context;
class ClassTypeSymbol;
class TypeSymbol;
class VariableSymbol;

class StatementBinder : public otava::ast::DefaultVisitor
{
public:
    StatementBinder(Context* context_, FunctionDefinitionSymbol* functionDefinitionSymbol_);
    inline std::unique_ptr<BoundStatementNode> GetBoundStatement() { return std::move(boundStatement); }
    void Visit(otava::ast::FunctionDefinitionNode& node) override;
    void Visit(otava::ast::ConstructorNode& node) override;
    void Visit(otava::ast::ConstructorInitializerNode& node) override;
    void Visit(otava::ast::MemberInitializerListNode& node) override;
    void Visit(otava::ast::MemberInitializerNode& node) override;
    void Visit(otava::ast::TemplateIdNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::ExpressionListNode& node) override;
    void Visit(otava::ast::FunctionBodyNode& node) override;
    void Visit(otava::ast::CompoundStatementNode& node) override;
    void Visit(otava::ast::SequenceStatementNode& node) override;
    void Visit(otava::ast::IfStatementNode& node) override;
    void Visit(otava::ast::SwitchStatementNode& node) override;
    void Visit(otava::ast::CaseStatementNode& node) override;
    void Visit(otava::ast::DefaultStatementNode& node) override;
    void Visit(otava::ast::WhileStatementNode& node) override;
    void Visit(otava::ast::DoStatementNode& node) override;
    void Visit(otava::ast::RangeForStatementNode& node) override;
    void Visit(otava::ast::ForStatementNode& node) override;
    void Visit(otava::ast::BreakStatementNode& node) override;
    void Visit(otava::ast::ContinueStatementNode& node) override;
    void Visit(otava::ast::ReturnStatementNode& node) override;
    void Visit(otava::ast::ExpressionStatementNode& node) override;
    void Visit(otava::ast::DeclarationStatementNode& node) override;
    void Visit(otava::ast::TryStatementNode& node) override;
    void Visit(otava::ast::HandlerSequenceNode& node) override;
    void Visit(otava::ast::HandlerNode& node) override;
    void Visit(otava::ast::ExceptionDeclarationNode& node) override;
    void Visit(otava::ast::SimpleDeclarationNode& node) override;
    void Visit(otava::ast::AliasDeclarationNode& node) override;
    void Visit(otava::ast::BoundStatementNode& node) override;
    void Visit(otava::ast::GotoStatementNode& node) override;
    void Visit(otava::ast::LabeledStatementNode& node) override;
    inline FunctionDefinitionSymbol* GetFunctionDefinitionSymbol() const { return functionDefinitionSymbol; }
    void SetStatement(BoundStatementNode* statement);
private:
    bool HasThisInitializer() const noexcept;
    void CompleteMemberInitializers(const soul::ast::FullSpan& fullSpan);
    void AddDefaultMemberInitializer(VariableSymbol* memberVar, const soul::ast::FullSpan& fullSpan);
    int GetBaseInitializerOrTerminatorIndex(TypeSymbol* baseClass) const noexcept;
    void CompleteBaseInitializers(const soul::ast::FullSpan& fullSpan);
    void AddDefaultBaseInitializer(TypeSymbol* baseClass, int index, const soul::ast::FullSpan& fullSpan);
    void GenerateDefaultCtorInitializer(const soul::ast::FullSpan& fullSpan);
    void GenerateDestructorTerminator(const soul::ast::FullSpan& fullSpan);
    void GenerateBaseTerminators(const soul::ast::FullSpan& fullSpan);
    void AddBaseTerminator(TypeSymbol* baseClass, int index, const soul::ast::FullSpan& fullSpan);
    void GenerateMemberTerminators(const soul::ast::FullSpan& fullSpan);
    void AddMemberTerminator(VariableSymbol* memberVar, const soul::ast::FullSpan& fullSpan);
    void GenerateSetVPtrStatements(const soul::ast::FullSpan& fullSpan);
    void BindStaticLocalVariable(VariableSymbol* variable, otava::ast::Node* initializer, otava::ast::SimpleDeclarationNode* declarationNode);
    Context* context;
    ClassTypeSymbol* currentClass;
    bool hasCtorInitializer;
    std::unique_ptr<BoundCtorInitializerNode> ctorInitializer;
    std::unique_ptr<BoundDtorTerminatorNode> dtorTerminator;
    std::unique_ptr<BoundStatementNode> boundStatement;
    FunctionDefinitionSymbol* functionDefinitionSymbol;
    ClassTypeSymbol* classTypeSymbol;
    VariableSymbol* memberVariableSymbol;
    std::vector<std::unique_ptr<BoundExpressionNode>> initializerArgs;
    bool resolveClass;
    bool resolveMemberVariable;
    bool resolveInitializerArguments;
    bool setVPtrStatementsGenerated;
    bool setLineCodeGenerated;
    std::vector<std::pair<int, std::unique_ptr<BoundFunctionCallNode>>> baseInitializers;
    std::vector<std::pair<int, std::unique_ptr<BoundFunctionCallNode>>> memberInitializers;
    std::vector<std::unique_ptr<BoundStatementNode>> setVPtrStatements;
    std::vector<std::pair<int, std::unique_ptr<BoundFunctionCallNode>>> memberTerminators;
    std::vector<std::pair<int, std::unique_ptr<BoundFunctionCallNode>>> baseTerminators;
    bool postfix;
    std::unique_ptr<BoundExpressionStatementNode> constructFunctionStaticStatement;
    std::unique_ptr<BoundExpressionStatementNode> atExitStatement;
    VariableSymbol* globalStaticVariableSymbol;
    otava::ast::Node* catchBlock;
    otava::ast::CompoundStatementNode* handlerBlock;
    otava::ast::Node* lastElse;
};

FunctionDefinitionSymbol* BindFunction(otava::ast::Node* functionDefinitionNode, FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context);
std::unique_ptr<BoundStatementNode> BindStatement(otava::ast::Node* statementNode, FunctionDefinitionSymbol* functionDefinitionSymbol, Context* context);

} // namespace otava::symbols
