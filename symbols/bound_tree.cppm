// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.bound_tree;

import otava.intermediate.code;
import otava.intermediate.data;
import soul.ast.span;
import otava.ast.node;
import std;

export namespace otava::symbols {

class ClassTemplateRepository;
class NamespaceSymbol;
class NamespaceTypeSymbol;
class Emitter;

enum class OperationFlags : std::int32_t
{
    none = 0, addr = 1 << 0, deref = 1 << 1, defaultInit = 1 << 2, virtualCall = 1 << 3, setPtr = 1 << 4, dup = 1 << 5, storeDeref = 1 << 6, derefAfterConv = 1 << 7,
    derefCount = 0xFF << 8
};

constexpr OperationFlags operator|(OperationFlags left, OperationFlags right) noexcept
{
    return OperationFlags(std::int32_t(left) | std::int32_t(right));
}

constexpr OperationFlags operator&(OperationFlags left, OperationFlags right) noexcept
{
    return OperationFlags(std::int32_t(left) & std::int32_t(right));
}

constexpr OperationFlags operator~(OperationFlags flags) noexcept
{
    return OperationFlags(~std::int32_t(flags));
}

constexpr std::uint8_t GetDerefCount(OperationFlags flags) noexcept
{
    return static_cast<std::uint8_t>(static_cast<std::int32_t>(flags & OperationFlags::derefCount) >> 8);
}

constexpr OperationFlags SetDerefCount(OperationFlags flags, std::uint8_t n) noexcept
{
    return OperationFlags(flags | OperationFlags(static_cast<std::int32_t>(n) << 8));
}

class FunctionSymbol;
class FunctionGroupSymbol;
class VariableSymbol;
class ParameterSymbol;
class EnumConstantSymbol;
class TypeSymbol;
class ClassTypeSymbol;
class ClassGroupSymbol;
class AliasGroupSymbol;
class Context;
class Value;
class Scope;
class FunctionDefinitionSymbol;
class BoundTreeVisitor;
class BoundFunctionCallNode;
class OperationRepository;
class ArgumentConversionTable;
class FunctionTemplateRepository;
class InlineFunctionRepository;

enum class BoundNodeKind
{
    boundCompileUnitNode, boundClassNode, boundGlobalVariableDefinitionNode, boundFunctionNode,
    boundCompoundStatementNode, boundIfStatementNode, boundSwitchStatementNode, boundCaseStatementNode, boundDefaultStatementNode,
    boundWhileStatementNode, boundDoStatementNode, boundForStatementNode, boundBreakStatementNode, boundContinueStatementNode,
    boundReturnStatementNode, boundGotoStatementNode, boundLabeledStatementNode, boundEmptyStatementNode,
    boundConstructionStatementNode, boundExpressionStatementNode, boundSequenceStatementNode, boundSetVPtrStatementNode, boundAliasDeclarationStatementNode,
    boundValueNode, boundLiteralNode, boundStringLiteralNode, boundVariableNode, boundParentVariableNode, boundParameterNode, boundParentParameterNode,
    boundEnumConstantNode, boundFunctionGroupNode, boundClassGroupNode, boundAliasGroupNode, boundTypeNode, boundMemberExprNode, boundFunctionCallNode,
    boundInvokeNode, boundFunctionPtrCallNode, boundEmptyFunctionCallNode, boundExpressionListNode,
    boundConjunctionNode, boundDisjunctionNode, boundConditionalExprNode, boundExpressionSequenceNode, boundConstructExpressionNode,
    boundConversionNode, boundAddressOfNode, boundDereferenceNode, boundRefToPtrNode, boundPtrToRefNode, boundDefaultInitNode,
    boundTemporaryNode, boundConstructTemporaryNode, boundDestructTemporariesNode, boundCtorInitializerNode, boundDtorTerminatorNode, boundEmptyDestructorNode,
    boundHandlerNode, boundFunctionValueNode, boundVariableAsVoidPtrNode, boundAdjustedThisPtrNode, boundOperatorFnNode
};

std::string BoundNodeKindStr(BoundNodeKind nodeKind);

class BoundNode
{
public:
    BoundNode(BoundNodeKind kind_, const soul::ast::FullSpan& fullSpan_) noexcept;
    virtual ~BoundNode();
    virtual void Accept(BoundTreeVisitor& visitor) = 0;
    virtual bool MayThrow() const noexcept { return false; }
    inline BoundNodeKind Kind() const noexcept { return kind; }
    virtual Scope* GetMemberScope(otava::ast::Node* op, const soul::ast::FullSpan& fullSpan, Context* context) const { return nullptr; }
    inline const soul::ast::FullSpan& GetFullSpan() const noexcept { return fullSpan; }
    void SetFullSpan(const soul::ast::FullSpan& fullSpan_) noexcept { fullSpan = fullSpan_; }
    inline int FileIndex() const noexcept { return fullSpan.fileIndex; }
    virtual bool IsBoundExpressionNode() const noexcept { return false; }
    inline bool IsBoundAddressOfNode() const noexcept { return kind == BoundNodeKind::boundAddressOfNode; }
    inline bool IsBoundDereferenceNode() const noexcept { return kind == BoundNodeKind::boundDereferenceNode; }
    inline bool IsBoundRefToPtrNode() const noexcept { return kind == BoundNodeKind::boundRefToPtrNode; }
    inline bool IsReturnStatementNode() const noexcept { return kind == BoundNodeKind::boundReturnStatementNode; }
    virtual bool IsBoundCtorInitializerNode() const noexcept { return false; }
    virtual bool IsBoundDtorTerminatorNode() const noexcept { return false; }
    virtual bool IsBoundStatementNode() const noexcept { return false; }
    virtual bool IsReturnOrSequenceReturnStatementNode() const noexcept { return false; }
    inline bool IsBoundCaseStatementNode() const noexcept { return kind == BoundNodeKind::boundCaseStatementNode; }
    inline bool IsBoundMemberExprNode() const noexcept { return kind == BoundNodeKind::boundMemberExprNode; }
    inline bool IsBoundTypeNode() const noexcept { return kind == BoundNodeKind::boundTypeNode; }
    inline bool IsBoundFunctionGroupNode() const noexcept { return kind == BoundNodeKind::boundFunctionGroupNode; }
    inline bool IsBoundClassGroupNode() const noexcept { return kind == BoundNodeKind::boundClassGroupNode; }
    inline bool IsBoundAliasGroupNode() const noexcept { return kind == BoundNodeKind::boundAliasGroupNode; }
    inline bool IsBoundExpressionListNode() const noexcept { return kind == BoundNodeKind::boundExpressionListNode; }
    inline bool IsBoundParameterNode() const noexcept { return kind == BoundNodeKind::boundParameterNode; }
    inline bool IsBoundParentParameterNode() const noexcept { return kind == BoundNodeKind::boundParentParameterNode; }
    inline bool IsBoundVariableNode() const noexcept { return kind == BoundNodeKind::boundVariableNode; }
    inline bool IsBoundParentVariableNode() const noexcept { return kind == BoundNodeKind::boundParentVariableNode; }
    inline bool IsBoundEnumConstant() const noexcept { return kind == BoundNodeKind::boundEnumConstantNode; }
    inline bool IsBoundFunctionCallNode() const noexcept { return kind == BoundNodeKind::boundFunctionCallNode; }
    inline bool IsBoundInvokeNode() const noexcept { return kind == BoundNodeKind::boundInvokeNode; }
    inline bool IsBoundConstructTemporaryNode() const noexcept { return kind == BoundNodeKind::boundConstructTemporaryNode; }
    inline bool IsBoundFunctionPtrCallNode() const noexcept { return kind == BoundNodeKind::boundFunctionPtrCallNode; }
    inline bool IsBoundEmptyDestructorNode() const noexcept { return kind == BoundNodeKind::boundEmptyDestructorNode; }
    inline bool IsBoundConversionNode() const noexcept { return kind == BoundNodeKind::boundConversionNode; }
    inline bool IsBoundFunctionNode() const noexcept { return kind == BoundNodeKind::boundFunctionNode; }
    inline bool IsBoundCompoundStatementNode() const { return kind == BoundNodeKind::boundCompoundStatementNode; }
    inline bool IsSwitchStatementNode() const noexcept { return kind == BoundNodeKind::boundSwitchStatementNode; }
    inline bool IsWhileStatementNode() const noexcept { return kind == BoundNodeKind::boundWhileStatementNode; }
    inline bool IsDoStatementNode() const noexcept { return kind == BoundNodeKind::boundDoStatementNode; }
    inline bool IsForStatementNode() const noexcept { return kind == BoundNodeKind::boundForStatementNode; }
    inline bool IsIfStatementNode() const noexcept { return kind == BoundNodeKind::boundIfStatementNode; }
    inline bool IsBoundConstructionStatementNode() const noexcept { return kind == BoundNodeKind::boundConstructionStatementNode; }
    inline bool IsBoundExpressionStatementNode() const noexcept { return kind == BoundNodeKind::boundExpressionStatementNode; }
    inline bool IsBoundSequenceStatementNode() const noexcept { return kind == BoundNodeKind::boundSequenceStatementNode; }
    inline bool IsBoundLiteralNode() const noexcept { return kind == BoundNodeKind::boundLiteralNode; }
    inline bool IsBoundStringLiteralNode() const noexcept { return kind == BoundNodeKind::boundStringLiteralNode; }
    inline int Index() const noexcept { return index; }
    inline void SetIndex(int index_) noexcept { index = index_; }
    inline otava::ast::Node* Source() const noexcept { return source.get(); }
    inline void SetSource(otava::ast::Node* source_) noexcept { source.reset(source_); }
    inline bool TemporaryDestructorCallsObtained() const noexcept { return temporaryDestructorCallsObtained; }
    inline void SetTemporaryDestructorCallsObtained() noexcept { temporaryDestructorCallsObtained = true; }
private:
    BoundNodeKind kind;
    int index;
    soul::ast::FullSpan fullSpan;
    std::unique_ptr<otava::ast::Node> source;
    bool temporaryDestructorCallsObtained;
};

enum class BoundExpressionFlags
{
    none = 0, bindToRvalueRef = 1 << 0, virtualCall = 1 << 1, deref = 1 << 2
};

constexpr BoundExpressionFlags operator|(BoundExpressionFlags left, BoundExpressionFlags right) noexcept
{
    return BoundExpressionFlags(int(left) | int(right));
}

constexpr BoundExpressionFlags operator&(BoundExpressionFlags left, BoundExpressionFlags right) noexcept
{
    return BoundExpressionFlags(int(left) & int(right));
}

constexpr BoundExpressionFlags operator~(BoundExpressionFlags flag) noexcept
{
    return BoundExpressionFlags(~int(flag));
}

class BoundDestructTemporariesNode;

class BoundExpressionNode : public BoundNode
{
public:
    BoundExpressionNode(BoundNodeKind kind_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    ~BoundExpressionNode();
    inline BoundExpressionFlags Flags() const noexcept { return flags; }
    inline void SetFlags(BoundExpressionFlags flags_) noexcept { flags = flags_; }
    inline TypeSymbol* GetType() const noexcept { return type; }
    inline void SetType(TypeSymbol* type_) noexcept { type = type_; }
    virtual void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context);
    Scope* GetMemberScope(otava::ast::Node* op, const soul::ast::FullSpan& fullSpan, Context* context) const override;
    bool BindToRvalueRef() const noexcept { return GetFlag(BoundExpressionFlags::bindToRvalueRef); }
    virtual bool HasValue(Context* context) const noexcept { return false; }
    virtual void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context);
    virtual BoundExpressionNode* Clone() const = 0;
    inline bool GetFlag(BoundExpressionFlags flag) const noexcept { return (flags & flag) != BoundExpressionFlags::none; }
    inline void SetFlag(BoundExpressionFlags flag) noexcept { flags = flags | flag; }
    bool IsBoundExpressionNode() const noexcept override { return true; }
    virtual bool IsBoundLocalVariable(Context* context) const noexcept { return false; }
    virtual bool IsBoundMemberVariable(Context* context) const noexcept { return false; }
    virtual bool IsBoundParentLocalVariable(Context* context) const noexcept { return false; }
    virtual bool IsBoundParentMemberVariable(Context* context) const noexcept { return false; }
    virtual bool IsLvalueExpression(Context* context) const noexcept { return false; }
    virtual bool IsNoReturnFunctionCall() const noexcept { return false; }
    void SetDestructTemporariesNode(BoundDestructTemporariesNode* destructTemporariesNode_) noexcept;
    inline BoundDestructTemporariesNode* DestructTemporariesNode() const noexcept { return destructTemporariesNode; }
    void DestructTemporaries(Emitter& emitter, Context* context);
private:
    BoundExpressionFlags flags;
    TypeSymbol* type;
    BoundDestructTemporariesNode* destructTemporariesNode;
};

class BoundValueExpressionNode : public BoundExpressionNode
{
public:
    BoundValueExpressionNode(otava::intermediate::Value* value_, TypeSymbol* type);
    bool HasValue(Context* context) const noexcept override { return true; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    otava::intermediate::Value* value;
};

class BoundFunctionNode;

class BoundCompileUnitNode : public BoundNode
{
public:
    BoundCompileUnitNode();
    ~BoundCompileUnitNode();
    void Sort();
    inline OperationRepository* GetOperationRepository() const { return operationRepository.get(); }
    inline ArgumentConversionTable* GetArgumentConversionTable() const { return argumentConversionTable.get(); }
    inline FunctionTemplateRepository* GetFunctionTemplateRepository() const { return functionTemplateRepository.get(); }
    inline ClassTemplateRepository* GetClassTemplateRepository() const { return classTemplateRepository.get(); }
    inline InlineFunctionRepository* GetInlineFunctionRepository() const { return inlineFunctionRepository.get(); }
    inline BoundFunctionNode* GetCompileUnitInitializationFunction() { return compileUnitInitializationFunction; }
    void AddDynamicInitialization(BoundExpressionNode* dynamicInitialization, BoundExpressionNode* atExitCall, const soul::ast::FullSpan& fullSpan, Context* context);
    void Accept(BoundTreeVisitor& visitor) override;
    void AddBoundNode(std::unique_ptr<BoundNode>&& node, Context* context);
    void AddBoundNodeForClass(ClassTypeSymbol* cls, const soul::ast::FullSpan& fullSpan, Context* context);
    inline const std::vector<std::unique_ptr<BoundNode>>& BoundNodes() const { return boundNodes; }
    inline void SetId(const std::string& id_) { id = id_; }
    inline const std::string& Id() const { return id; }
    otava::intermediate::Value* CreateBoundGlobalVariable(
        VariableSymbol* globalVariableSymbol, Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context, bool definition);
    void AddClassToGenerateDestructorList(ClassTypeSymbol* classType);
    inline const std::vector<ClassTypeSymbol*>& GenerateDestructorList() const { return generateDestructorList; }
private:
    std::string id;
    std::vector<std::unique_ptr<BoundNode>> boundNodes;
    std::unique_ptr<OperationRepository> operationRepository;
    std::unique_ptr<ArgumentConversionTable> argumentConversionTable;
    std::unique_ptr<FunctionTemplateRepository> functionTemplateRepository;
    std::unique_ptr<ClassTemplateRepository> classTemplateRepository;
    std::unique_ptr<InlineFunctionRepository> inlineFunctionRepository;
    std::set<ClassTypeSymbol*> boundClasses;
    BoundFunctionNode* compileUnitInitializationFunction;
    std::vector<ClassTypeSymbol*> generateDestructorList;
};

class BoundStatementNode;

class BoundCtorInitializerNode : public BoundNode
{
public:
    BoundCtorInitializerNode(const soul::ast::FullSpan& fullSpan_);
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    bool IsBoundCtorInitializerNode() const noexcept override { return true; }
    void AddBaseInitializer(BoundExpressionNode* baseInitializer);
    const std::vector<std::unique_ptr<BoundExpressionNode>>& BaseInitializers() const { return baseInitializers; }
    void AddSetVPtrStatement(BoundStatementNode* setVPtrStatement);
    const std::vector<std::unique_ptr<BoundStatementNode>>& SetVPtrStatements() const { return setVPtrStatements; }
    void AddMemberInitializer(BoundExpressionNode* memberInitializer);
    const std::vector<std::unique_ptr<BoundExpressionNode>>& MemberInitializers() const { return memberInitializers; }
    void GenerateCode(BoundTreeVisitor& visitor, Emitter& emitter, Context* context);
private:
    std::vector<std::unique_ptr<BoundExpressionNode>> baseInitializers;
    std::vector<std::unique_ptr<BoundStatementNode>> setVPtrStatements;
    std::vector<std::unique_ptr<BoundExpressionNode>> memberInitializers;
};

class BoundDtorTerminatorNode : public BoundNode
{
public:
    BoundDtorTerminatorNode(const soul::ast::FullSpan& fullSpan_);
    void Accept(BoundTreeVisitor& visitor) override;
    bool IsBoundDtorTerminatorNode() const noexcept override { return true; }
    void AddSetVPtrStatement(BoundStatementNode* setVPtrStatement);
    const std::vector<std::unique_ptr<BoundStatementNode>>& SetVPtrStatements() const { return setVPtrStatements; }
    void AddMemberTerminator(BoundExpressionNode* memberTerminator);
    const std::vector<std::unique_ptr<BoundExpressionNode>>& MemberTerminators() const { return memberTerminators; }
    void GenerateCode(BoundTreeVisitor& visitor, Emitter& emitter, Context* context);
private:
    std::vector<std::unique_ptr<BoundStatementNode>> setVPtrStatements;
    std::vector<std::unique_ptr<BoundExpressionNode>> memberTerminators;
};

class BoundClassNode : public BoundNode
{
public:
    BoundClassNode(ClassTypeSymbol* cls_, const soul::ast::FullSpan& fullSpan_);
    void Accept(BoundTreeVisitor& visitor) override;
    inline ClassTypeSymbol* GetClass() const { return cls; }
private:
    ClassTypeSymbol* cls;
};

class BoundCompoundStatementNode;
class BoundReturnStatementNode;
class BoundConstructionStatementNode;

class BoundFunctionNode : public BoundNode
{
public:
    BoundFunctionNode(FunctionDefinitionSymbol* functionDefinitionSymbol_, const soul::ast::FullSpan& fullSpan_);
    ~BoundFunctionNode();
    void Accept(BoundTreeVisitor& visitor) override;
    void SetBody(BoundCompoundStatementNode* body_);
    inline const BoundCompoundStatementNode* Body() const { return body.get(); }
    inline BoundCompoundStatementNode* Body() { return body.get(); }
    void SetCtorInitializer(BoundCtorInitializerNode* ctorInitializer_);
    void SetDtorTerminator(BoundDtorTerminatorNode* dtorTerminator_);
    inline BoundCtorInitializerNode* CtorInitializer() const { return ctorInitializer.get(); }
    inline BoundDtorTerminatorNode* DtorTerminator() const { return dtorTerminator.get(); }
    FunctionDefinitionSymbol* GetFunctionDefinitionSymbol() { return functionDefinitionSymbol; }
    void AddDefaultFunctionSymbol(FunctionSymbol* defaultFunctionSymbol);
    inline void SetSerial(int serial_) { serial = serial_; };
    inline int Serial() const { return serial; }
    void SetSetLineStatementNode(otava::ast::Node* setLineStatementNode_);
    inline otava::ast::Node* GetSetLineStatementNode() const { return setLineStatementNode.get(); }
    void SetBoundSetLineStatement(BoundStatementNode* boundSetLineStatement_);
    inline BoundStatementNode* GetBoundSetLineStatement() const { return boundSetLineStatement.get(); }
    void AddTemporaryDestructorCall(BoundFunctionCallNode* temporaryDestructorCall);
    inline bool HasTemporaryDestructorCalls() const noexcept { return !temporaryDestructorCalls.empty(); }
    std::vector<std::unique_ptr<BoundFunctionCallNode>> GetTemporaryDestructorCalls();
private:
    FunctionDefinitionSymbol* functionDefinitionSymbol;
    std::unique_ptr<BoundCtorInitializerNode> ctorInitializer;
    std::unique_ptr<BoundDtorTerminatorNode> dtorTerminator;
    std::unique_ptr<BoundCompoundStatementNode> body;
    std::vector<std::unique_ptr<FunctionSymbol>> defaultFunctionSymbols;
    int serial;
    std::unique_ptr<otava::ast::Node> setLineStatementNode;
    std::unique_ptr<BoundStatementNode> boundSetLineStatement;
    std::vector<std::unique_ptr<BoundFunctionCallNode>> temporaryDestructorCalls;
};

class BoundStatementNode : public BoundNode
{
public:
    BoundStatementNode(BoundNodeKind kind_, const soul::ast::FullSpan& fullSpan_) noexcept;
    ~BoundStatementNode();
    virtual std::string Name() const = 0;
    virtual BoundStatementNode* Clone() const = 0;
    bool IsBoundStatementNode() const noexcept override { return true; }
    virtual bool EndsWithTerminator() const noexcept { return IsTerminator(); }
    virtual bool IsTerminator() const noexcept { return false; }
    virtual int IndexOf(BoundStatementNode* stmt) noexcept { return -1; }
    virtual bool ContainsLocalVariableWithDestructor() const noexcept { return false; }
    inline BoundStatementNode* Parent() const noexcept { return parent; }
    inline void SetParent(BoundStatementNode* parent_) noexcept { parent = parent_; }
    inline bool Generated() const noexcept { return generated; }
    inline void SetGenerated() noexcept { generated = true; }
    inline bool Postfix() const noexcept { return postfix; }
    inline void SetPostfix() noexcept { postfix = true; }
    inline int StatementIndex() const { return statementIndex; }
    inline void SetStatementIndex(int statementIndex_) { statementIndex = statementIndex_; }
    bool IsConditionalStatementInBlock(BoundCompoundStatementNode* block) const noexcept;
    BoundCompoundStatementNode* Block() noexcept;
    void SetDestructTemporariesNode(BoundDestructTemporariesNode* destructTemporariesNode_) noexcept;
    inline BoundDestructTemporariesNode* DestructTemporariesNode() const noexcept { return destructTemporariesNode; }
    void DestructTemporaries(Emitter& emitter, Context* context);
private:
    BoundStatementNode* parent;
    bool generated;
    bool postfix;
    int statementIndex;
    BoundDestructTemporariesNode* destructTemporariesNode;
};

class BoundEmptyStatementNode : public BoundStatementNode
{
public:
    BoundEmptyStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "empty"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
};

class BoundCompoundStatementNode : public BoundStatementNode
{
public:
    BoundCompoundStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "compound"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    int IndexOf(BoundStatementNode* stmt) noexcept override;
    inline int NextStatementIndex() const { return statements.size(); }
    void AddStatement(BoundStatementNode* statement);
    inline const std::vector<std::unique_ptr<BoundStatementNode>>& Statements() const noexcept { return statements; }
    bool EndsWithTerminator() const noexcept override;
    bool ContainsLocalVariableWithDestructor() const noexcept override;
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::vector<std::unique_ptr<BoundStatementNode>> statements;
    std::vector<std::unique_ptr<BoundConstructionStatementNode>> invokeStatementsWithDestructor;
    int blockId;
};

class BoundIfStatementNode : public BoundStatementNode
{
public:
    BoundIfStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "if"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    void SetCondition(BoundExpressionNode* condition_) noexcept;
    inline BoundExpressionNode* GetCondition() const noexcept { return condition.get(); }
    inline BoundStatementNode* ThenStatement() const noexcept { return thenStatement.get(); }
    void SetThenStatement(BoundStatementNode* thenStatement_) noexcept;
    inline BoundStatementNode* ElseStatement() const noexcept { return elseStatement.get(); }
    void SetElseStatement(BoundStatementNode* elseStatement_) noexcept;
    bool ContainsLocalVariableWithDestructor() const noexcept override;
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::unique_ptr<BoundStatementNode> thenStatement;
    std::unique_ptr<BoundStatementNode> elseStatement;
    int blockId;
};

class BoundSwitchStatementNode : public BoundStatementNode
{
public:
    BoundSwitchStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "switch"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    void SetCondition(BoundExpressionNode* condition_) noexcept;
    inline BoundExpressionNode* GetCondition() const noexcept { return condition.get(); }
    void SetStatement(BoundStatementNode* statement) noexcept;
    inline BoundStatementNode* Statement() const noexcept { return statement.get(); }
    bool ContainsLocalVariableWithDestructor() const noexcept override;
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::unique_ptr<BoundStatementNode> statement;
    int blockId;
};

class BoundCaseStatementNode : public BoundStatementNode
{
public:
    BoundCaseStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "case"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    inline const std::vector<std::unique_ptr<BoundExpressionNode>>& CaseExprs() const noexcept { return caseExprs; }
    void InsertCaseExprToFront(BoundExpressionNode* caseExpr);
    void AddCaseExpr(BoundExpressionNode* caseExpr);
    inline BoundStatementNode* Statement() const noexcept { return stmt.get(); }
    void SetStatement(BoundStatementNode* stmt_) noexcept;
    bool ContainsLocalVariableWithDestructor() const noexcept override;
private:
    std::vector<std::unique_ptr<BoundExpressionNode>> caseExprs;
    std::unique_ptr<BoundStatementNode> stmt;
};

class BoundDefaultStatementNode : public BoundStatementNode
{
public:
    BoundDefaultStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "default"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    inline BoundStatementNode* Statement() const noexcept { return stmt.get(); }
    void SetStatement(BoundStatementNode* stmt_) noexcept;
    bool ContainsLocalVariableWithDestructor() const noexcept override;
private:
    std::unique_ptr<BoundStatementNode> stmt;
};

class BoundWhileStatementNode : public BoundStatementNode
{
public:
    BoundWhileStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "while"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    void SetCondition(BoundExpressionNode* condition_) noexcept;
    inline BoundExpressionNode* GetCondition() const noexcept { return condition.get(); }
    void SetStatement(BoundStatementNode* statement) noexcept;
    inline BoundStatementNode* Statement() const noexcept { return statement.get(); }
    bool ContainsLocalVariableWithDestructor() const noexcept override;
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::unique_ptr<BoundStatementNode> statement;
    int blockId;
};

class BoundDoStatementNode : public BoundStatementNode
{
public:
    BoundDoStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "do"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    void SetExpr(BoundExpressionNode* condition_) noexcept;
    inline BoundExpressionNode* GetExpr() const noexcept { return expr.get(); }
    void SetStatement(BoundStatementNode* statement) noexcept;
    inline BoundStatementNode* Statement() const noexcept { return statement.get(); }
    bool ContainsLocalVariableWithDestructor() const noexcept override;
private:
    std::unique_ptr<BoundExpressionNode> expr;
    std::unique_ptr<BoundStatementNode> statement;
};

class BoundForStatementNode : public BoundStatementNode
{
public:
    BoundForStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "for"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    void SetInitStatement(BoundStatementNode* initStatement_) noexcept;
    inline BoundStatementNode* InitStatement() const noexcept { return initStatement.get(); }
    void SetCondition(BoundExpressionNode* condition_) noexcept;
    inline BoundExpressionNode* GetCondition() const noexcept { return condition.get(); }
    void SetLoopExpr(BoundExpressionNode* loopExpr_) noexcept;
    inline BoundExpressionNode* GetLoopExpr() const noexcept { return loopExpr.get(); }
    void SetStatement(BoundStatementNode* statement) noexcept;
    inline BoundStatementNode* Statement() const noexcept { return statement.get(); }
    bool ContainsLocalVariableWithDestructor() const noexcept override;
    inline int BlockId() const noexcept { return blockId; }
    inline void SetBlockId(int blockId_) noexcept { blockId = blockId_; }
private:
    std::unique_ptr<BoundStatementNode> initStatement;
    std::unique_ptr<BoundExpressionNode> condition;
    std::unique_ptr<BoundExpressionNode> loopExpr;
    std::unique_ptr<BoundStatementNode> statement;
    int blockId;
};

class BoundSequenceStatementNode : public BoundStatementNode
{
public:
    BoundSequenceStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    BoundSequenceStatementNode(const soul::ast::FullSpan& fullSpan_, BoundStatementNode* first_, BoundStatementNode* second_) noexcept;
    std::string Name() const override { return "sequence"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    inline BoundStatementNode* First() const noexcept { return first.get(); }
    inline void SetFirst(BoundStatementNode* first_)noexcept { first.reset(first_); }
    inline BoundStatementNode* Second() const noexcept { return second.get(); }
    inline void SetSecond(BoundStatementNode* second_) noexcept { second.reset(second_); }
    bool IsTerminator() const noexcept override { return second->IsTerminator(); }
    bool IsReturnOrSequenceReturnStatementNode() const noexcept override { return second->IsReturnStatementNode(); }
    bool ContainsLocalVariableWithDestructor() const noexcept override;
private:
    std::unique_ptr<BoundStatementNode> first;
    std::unique_ptr<BoundStatementNode> second;
};

class BoundBreakStatementNode : public BoundStatementNode
{
public:
    BoundBreakStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "break"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool IsTerminator() const noexcept override { return true; }
};

class BoundContinueStatementNode : public BoundStatementNode
{
public:
    BoundContinueStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "continue"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool IsTerminator() const noexcept override { return true; }
};

class BoundReturnStatementNode : public BoundStatementNode
{
public:
    BoundReturnStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "return"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    void SetExpr(BoundExpressionNode* expr_) noexcept;
    void SetExpr(BoundExpressionNode* expr_, const soul::ast::FullSpan& fullSpan, Context* context);
    inline BoundExpressionNode* GetExpr() const noexcept { return expr.get(); }
    bool IsTerminator() const noexcept override { return true; }
    bool IsReturnOrSequenceReturnStatementNode() const noexcept override { return true; }
private:
    std::unique_ptr<BoundExpressionNode> expr;
};

class BoundLabeledStatementNode : public BoundStatementNode
{
public:
    BoundLabeledStatementNode(const soul::ast::FullSpan& fullSpan_);
    BoundLabeledStatementNode(const soul::ast::FullSpan& fullSpan_, const std::string& label_, BoundStatementNode* stmt_);
    std::string Name() const override { return "labeled"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    const std::string& Label() const noexcept { return label; }
    void SetLabel(const std::string& label_) { label = label_; }
    inline void SetStatement(BoundStatementNode* stmt_) noexcept { stmt.reset(stmt_); }
    BoundStatementNode* Statement() const noexcept { return stmt.get(); }
    otava::intermediate::BasicBlock* GetBB(Emitter& emitter);
    bool ContainsLocalVariableWithDestructor() const noexcept override;
private:
    std::string label;
    std::unique_ptr<BoundStatementNode> stmt;
    otava::intermediate::BasicBlock* bb;
};

class BoundGotoStatementNode : public BoundStatementNode
{
public:
    BoundGotoStatementNode(const soul::ast::FullSpan& fullSpan_);
    BoundGotoStatementNode(const soul::ast::FullSpan& fullSpan_, const std::string& target_);
    std::string Name() const override { return "goto"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool IsTerminator() const noexcept override { return true; }
    inline const std::string& Target() const noexcept { return target; }
    void SetTarget(const std::string& target_) { target = target_; }
    inline void SetLabeledStatement(BoundLabeledStatementNode* labeledStatement_) noexcept { labeledStatement = labeledStatement_; }
    inline BoundLabeledStatementNode* GetLabeledStatement() const noexcept { return labeledStatement; }
    otava::intermediate::BasicBlock* GetBB(Emitter& emitter);
private:
    std::string target;
    BoundLabeledStatementNode* labeledStatement;
};

class BoundConstructionStatementNode : public BoundStatementNode
{
public:
    BoundConstructionStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    BoundConstructionStatementNode(const soul::ast::FullSpan& fullSpan_, BoundExpressionNode* constructorCall_) noexcept;
    std::string Name() const override { return "construction"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    inline BoundExpressionNode* ConstructorCall() const noexcept { return constructorCall.get(); }
    void SetConstructorCall(BoundExpressionNode* constructorCall_) { constructorCall.reset(constructorCall_); }
    BoundExpressionNode* ReleaseConstructorCall() noexcept { return constructorCall.release(); }
    void SetDestructorCall(BoundExpressionNode* destructorCall_) noexcept;
    inline BoundExpressionNode* DestructorCall() const noexcept { return destructorCall.get(); }
    inline BoundExpressionNode* ReleaseDestructorCall() noexcept { return destructorCall.release(); }
    inline VariableSymbol* Variable() const noexcept { return variable; }
    inline void SetVariable(VariableSymbol* variable_) noexcept { variable = variable_; }
    bool ContainsLocalVariableWithDestructor() const noexcept override;
private:
    std::unique_ptr<BoundExpressionNode> constructorCall;
    std::unique_ptr<BoundExpressionNode> destructorCall;
    VariableSymbol* variable;
};

class BoundExpressionStatementNode : public BoundStatementNode
{
public:
    BoundExpressionStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "expression"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    bool MayThrow() const noexcept override;
    void SetExpr(BoundExpressionNode* expr_) noexcept;
    void SetExpr(BoundExpressionNode* expr_, const soul::ast::FullSpan& fullSpan, Context* context);
    inline BoundExpressionNode* GetExpr() const noexcept { return expr.get(); }
    bool IsTerminator() const noexcept override;
private:
    std::unique_ptr<BoundExpressionNode> expr;
};

class BoundSetVPtrStatementNode : public BoundStatementNode
{
public:
    BoundSetVPtrStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    BoundSetVPtrStatementNode(BoundExpressionNode* thisPtr_, ClassTypeSymbol* forClass_, ClassTypeSymbol* vptrHolderClass_,
        const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "setvptr"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
    inline BoundExpressionNode* ThisPtr() const noexcept { return thisPtr.get(); }
    void SetThisPtr(BoundExpressionNode* thisPtr_) { thisPtr.reset(thisPtr_); }
    inline ClassTypeSymbol* GetClass() const noexcept { return forClass; }
    void SetForClass(ClassTypeSymbol* forClass_) { forClass = forClass_; }
    inline ClassTypeSymbol* GetVPtrHolderClass() const noexcept { return vptrHolderClass; }
    void SetVPtrHolderClass(ClassTypeSymbol* vptrHolderClass_) { vptrHolderClass = vptrHolderClass_; }
private:
    std::unique_ptr<BoundExpressionNode> thisPtr;
    ClassTypeSymbol* forClass;
    ClassTypeSymbol* vptrHolderClass;
};

class BoundAliasDeclarationStatementNode : public BoundStatementNode
{
public:
    BoundAliasDeclarationStatementNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    std::string Name() const override { return "alias"; }
    void Accept(BoundTreeVisitor& visitor) override;
    BoundStatementNode* Clone() const override;
};

class BoundLiteralNode : public BoundExpressionNode
{
public:
    BoundLiteralNode(Value* value_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool HasValue(Context* context) const noexcept override { return true; }
    Value* GetValue() const noexcept { return value; }
    void SetValue(Value* value_) noexcept { value = value_; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
private:
    Value* value;
};

class BoundStringLiteralNode : public BoundExpressionNode
{
public:
    BoundStringLiteralNode(Value* value_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool HasValue(Context* context) const noexcept override { return true; }
    inline Value* GetValue() const noexcept { return value; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
private:
    Value* value;
    otava::intermediate::Value* irValue;
};

class BoundVariableNode : public BoundExpressionNode
{
public:
    BoundVariableNode(VariableSymbol* variable_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* referredType_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool HasValue(Context* context) const noexcept override { return true; }
    VariableSymbol* GetVariable() const noexcept { return variable; }
    BoundExpressionNode* ThisPtr() const noexcept { return thisPtr.get(); }
    void SetThisPtr(BoundExpressionNode* thisPtr_) noexcept;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool IsBoundLocalVariable(Context* context) const noexcept override;
    bool IsBoundMemberVariable(Context* context) const noexcept override;
    bool IsLvalueExpression(Context* context) const noexcept override { return true; }
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    VariableSymbol* variable;
    std::unique_ptr<BoundExpressionNode> thisPtr;
};

class BoundParentVariableNode : public BoundExpressionNode
{
public:
    BoundParentVariableNode(VariableSymbol* variable_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* referredType_) noexcept;
    inline int Level() const noexcept { return level; }
    inline void SetLevel(int level_) noexcept { level = level_; }
    void Accept(BoundTreeVisitor& visitor) override;
    bool HasValue(Context* context) const noexcept override { return true; }
    VariableSymbol* GetVariable() const noexcept { return variable; }
    BoundExpressionNode* ThisPtr() const noexcept { return thisPtr.get(); }
    void SetThisPtr(BoundExpressionNode* thisPtr_) noexcept;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool IsBoundParentLocalVariable(Context* context) const noexcept override;
    bool IsBoundParentMemberVariable(Context* context) const noexcept override;
    bool IsLvalueExpression(Context* context) const noexcept override { return true; }
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    VariableSymbol* variable;
    int level;
    std::unique_ptr<BoundExpressionNode> thisPtr;
};

class BoundParameterNode : public BoundExpressionNode
{
public:
    BoundParameterNode(ParameterSymbol* parameter_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool HasValue(Context* context) const noexcept override { return true; }
    inline ParameterSymbol* GetParameter() const noexcept { return parameter; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool IsLvalueExpression(Context* context) const noexcept override { return true; }
    BoundExpressionNode* Clone() const override;
private:
    ParameterSymbol* parameter;
};

class BoundParentParameterNode : public BoundExpressionNode
{
public:
    BoundParentParameterNode(ParameterSymbol* parameter_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type) noexcept;
    inline int Level() const noexcept { return level; }
    inline void SetLevel(int level_) noexcept { level = level_; }
    void Accept(BoundTreeVisitor& visitor) override;
    bool HasValue(Context* context) const noexcept override { return true; }
    inline ParameterSymbol* GetParameter() const noexcept { return parameter; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool IsLvalueExpression(Context* context) const noexcept override { return true; }
    BoundExpressionNode* Clone() const override;
private:
    ParameterSymbol* parameter;
    int level;
};

class BoundEnumConstant : public BoundExpressionNode
{
public:
    BoundEnumConstant(EnumConstantSymbol* enumConstant_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* enumType_);
    void Accept(BoundTreeVisitor& visitor) override;
    bool HasValue(Context* context) const noexcept override { return true; }
    EnumConstantSymbol* EnumConstant() const noexcept { return enumConstant; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
private:
    EnumConstantSymbol* enumConstant;
};

class BoundFunctionGroupNode : public BoundExpressionNode
{
public:
    BoundFunctionGroupNode(FunctionGroupSymbol* functionGroupSymbol_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    inline FunctionGroupSymbol* GetFunctionGroupSymbol() const noexcept { return functionGroupSymbol; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    void AddTemplateArg(TypeSymbol* templateArg);
    inline const std::vector<TypeSymbol*>& TemplateArgs() const noexcept { return templateArgs; }
private:
    FunctionGroupSymbol* functionGroupSymbol;
    std::vector<TypeSymbol*> templateArgs;
};

class BoundClassGroupNode : public BoundExpressionNode
{
public:
    BoundClassGroupNode(ClassGroupSymbol* classGroupSymbol_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    inline ClassGroupSymbol* GetClassGroupSymbol() const noexcept { return classGroupSymbol; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    void AddTemplateArg(TypeSymbol* templateArg);
    inline const std::vector<TypeSymbol*>& TemplateArgs() const noexcept { return templateArgs; }
private:
    ClassGroupSymbol* classGroupSymbol;
    std::vector<TypeSymbol*> templateArgs;
};

class BoundAliasGroupNode : public BoundExpressionNode
{
public:
    BoundAliasGroupNode(AliasGroupSymbol* aliasGroupSymbol_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    inline AliasGroupSymbol* GetAliasGroupSymbol() const noexcept { return aliasGroupSymbol; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    void AddTemplateArg(TypeSymbol* templateArg);
    inline const std::vector<TypeSymbol*>& TemplateArgs() const noexcept { return templateArgs; }
private:
    AliasGroupSymbol* aliasGroupSymbol;
    std::vector<TypeSymbol*> templateArgs;
};

class BoundTypeNode : public BoundExpressionNode
{
public:
    BoundTypeNode(TypeSymbol* type_, const soul::ast::FullSpan& fullSpan_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
};

class BoundMemberExprNode : public BoundExpressionNode
{
public:
    BoundMemberExprNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundMemberExprNode(BoundExpressionNode* subject_, BoundExpressionNode* member_, otava::ast::NodeKind op_, const soul::ast::FullSpan& fullSpan_,
        TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    inline BoundExpressionNode* Subject() const noexcept { return subject.get(); }
    inline void SetSubject(BoundExpressionNode* subject_) { subject.reset(subject_); }
    inline BoundExpressionNode* Member() const noexcept { return member.get(); }
    inline void SetMember(BoundExpressionNode* member_) { member.reset(member_); }
    inline otava::ast::NodeKind Op() const noexcept { return op; }
    inline void SetOp(otava::ast::NodeKind op_) { op = op_; }
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
    std::unique_ptr<BoundExpressionNode> member;
    otava::ast::NodeKind op;
};

class BoundFunctionCallNode : public BoundExpressionNode
{
public:
    BoundFunctionCallNode(FunctionSymbol* functionSymbol_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    bool HasValue(Context* context) const noexcept override;
    inline FunctionSymbol* GetFunctionSymbol() const noexcept { return functionSymbol; }
    void AddArgument(BoundExpressionNode* arg);
    inline const std::vector<std::unique_ptr<BoundExpressionNode>>& Args() const noexcept { return args; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool IsLvalueExpression(Context* context) const noexcept override;
    BoundExpressionNode* Clone() const override;
    bool CallsClassConstructor(ClassTypeSymbol*& cls, BoundExpressionNode*& firstArg, FunctionDefinitionSymbol*& destructor) const noexcept;
    bool IsNoReturnFunctionCall() const noexcept override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    FunctionSymbol* functionSymbol;
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
};

class BoundEmptyFunctionCallNode : public BoundExpressionNode
{
public:
    BoundEmptyFunctionCallNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
};

class BoundFunctionPtrCallNode : public BoundExpressionNode
{
public:
    BoundFunctionPtrCallNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool HasValue(Context* context) const noexcept override { return true; }
    void AddArgument(BoundExpressionNode* arg);
    inline const std::vector<std::unique_ptr<BoundExpressionNode>>& Args() const noexcept { return args; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
};

class BoundInvokeNode : public BoundExpressionNode
{
public:
    BoundInvokeNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool HasValue(Context* context) const noexcept override;
    BoundExpressionNode* Clone() const override;
    inline BoundVariableNode* Result() const noexcept { return result.get(); }
    inline void SetResult(BoundVariableNode* result_) noexcept { result.reset(result_); }
    inline BoundExpressionNode* InvokeCall() const noexcept { return invokeCall.get(); }
    inline void SetInvokeCall(BoundExpressionNode* invokeCall_) noexcept { invokeCall.reset(invokeCall_); }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundVariableNode> result;
    std::unique_ptr<BoundExpressionNode> invokeCall;
};

class BoundExpressionSequenceNode : public BoundExpressionNode
{
public:
    BoundExpressionSequenceNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type);
    BoundExpressionSequenceNode(const soul::ast::FullSpan& fullSpan_, BoundExpressionNode* left_, BoundExpressionNode* right_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    bool HasValue(Context* context) const noexcept override;
    bool IsLvalueExpression(Context* context) const noexcept override;
    BoundExpressionNode* Clone() const override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    inline BoundExpressionNode* Left() const noexcept { return left.get(); }
    inline void SetLeft(BoundExpressionNode* left_) { left.reset(left_); }
    inline BoundExpressionNode* Right() const noexcept { return right.get(); }
    inline void SetRight(BoundExpressionNode* right_) { right.reset(right_); }
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> left;
    std::unique_ptr<BoundExpressionNode> right;
};

class BoundExpressionListNode : public BoundExpressionNode
{
public:
    BoundExpressionListNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
    void AddExpression(BoundExpressionNode* expr);
    inline int Count() const noexcept { return exprs.size(); }
    inline const std::vector<std::unique_ptr<BoundExpressionNode>>& Exprs() const noexcept { return exprs; }
    inline BoundExpressionNode* ReleaseExpr(int i) noexcept { return exprs[i].release(); }
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::vector<std::unique_ptr<BoundExpressionNode>> exprs;
};

class BoundConjunctionNode : public BoundExpressionNode
{
public:
    BoundConjunctionNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* boolType) noexcept;
    BoundConjunctionNode(BoundExpressionNode* left_, BoundExpressionNode* right_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* boolType) noexcept;
    inline BoundExpressionNode* Left() const noexcept { return left.get(); }
    inline void SetLeft(BoundExpressionNode* left_) { left.reset(left_); }
    inline BoundExpressionNode* Right() const noexcept { return right.get(); }
    inline void SetRight(BoundExpressionNode* right_) { right.reset(right_); }
    inline BoundVariableNode* Temporary() const noexcept { return temporary.get(); }
    void SetTemporary(BoundVariableNode* temporary_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    bool HasValue(Context* context) const noexcept override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> left;
    std::unique_ptr<BoundExpressionNode> right;
    std::unique_ptr<BoundVariableNode> temporary;
};

class BoundDisjunctionNode : public BoundExpressionNode
{
public:
    BoundDisjunctionNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* boolType) noexcept;
    BoundDisjunctionNode(BoundExpressionNode* left_, BoundExpressionNode* right_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* boolType) noexcept;
    inline BoundExpressionNode* Left() const noexcept { return left.get(); }
    inline void SetLeft(BoundExpressionNode* left_) { left.reset(left_); }
    inline BoundExpressionNode* Right() const noexcept { return right.get(); }
    inline void SetRight(BoundExpressionNode* right_) { right.reset(right_); }
    inline BoundVariableNode* Temporary() const noexcept { return temporary.get(); }
    void SetTemporary(BoundVariableNode* temporary_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    bool HasValue(Context* context) const noexcept override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> left;
    std::unique_ptr<BoundExpressionNode> right;
    std::unique_ptr<BoundVariableNode> temporary;
};

class BoundConditionalExprNode : public BoundExpressionNode
{
public:
    BoundConditionalExprNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type) noexcept;
    BoundConditionalExprNode(BoundExpressionNode* condition_, BoundExpressionNode* thenExpr_, BoundExpressionNode* elseExpr_, TypeSymbol* type_) noexcept;
    inline BoundExpressionNode* GetCondition() const noexcept { return condition.get(); }
    void SetCondition(BoundExpressionNode* condition_) noexcept { condition.reset(condition_); }
    inline BoundExpressionNode* GetThenExpr() const noexcept { return thenExpr.get(); }
    void SetThenExpr(BoundExpressionNode* thenExpr_) noexcept { thenExpr.reset(thenExpr_); }
    inline BoundExpressionNode* GetElseExpr() const noexcept { return elseExpr.get(); }
    void SetElseExpr(BoundExpressionNode* elseExpr_) noexcept { elseExpr.reset(elseExpr_); }
    inline BoundVariableNode* Temporary() const noexcept { return temporary.get(); }
    void SetTemporary(BoundVariableNode* temporary_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    bool HasValue(Context* context) const noexcept override { return true; }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::unique_ptr<BoundExpressionNode> thenExpr;
    std::unique_ptr<BoundExpressionNode> elseExpr;
    std::unique_ptr<BoundVariableNode> temporary;
};

class BoundConversionNode : public BoundExpressionNode
{
public:
    BoundConversionNode(FunctionSymbol* conversionFunction_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundConversionNode(BoundExpressionNode* subject_, FunctionSymbol* conversionFunction_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool IsLvalueExpression(Context* context) const noexcept override;
    inline BoundExpressionNode* Subject() const noexcept { return subject.get(); }
    inline void SetSubject(BoundExpressionNode* subject_) { subject.reset(subject_); }
    inline FunctionSymbol* ConversionFunction() const noexcept { return conversionFunction; }
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
    FunctionSymbol* conversionFunction;
};

class BoundAddressOfNode : public BoundExpressionNode
{
public:
    BoundAddressOfNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundAddressOfNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    inline BoundExpressionNode* Subject() noexcept { return subject.get(); }
    inline BoundExpressionNode* ReleaseSubject() noexcept { return subject.release(); }
    inline void SetSubject(BoundExpressionNode* subject_) noexcept { subject.reset(subject_); }
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
};

class BoundDereferenceNode : public BoundExpressionNode
{
public:
    BoundDereferenceNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundDereferenceNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundDereferenceNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_, OperationFlags kind_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool IsLvalueExpression(Context* context) const noexcept override { return true; }
    inline BoundExpressionNode* Subject() noexcept { return subject.get(); }
    inline void SetSubject(BoundExpressionNode* subject_) noexcept { subject.reset(subject_); }
    inline OperationFlags Kind() const noexcept { return kind; }
    inline void SetKind(OperationFlags kind_) { kind = kind_; }
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    OperationFlags kind;
    std::unique_ptr<BoundExpressionNode> subject;
};

class BoundRefToPtrNode : public BoundExpressionNode
{
public:
    BoundRefToPtrNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundRefToPtrNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    inline BoundExpressionNode* Subject() noexcept { return subject.get(); }
    inline void SetSubject(BoundExpressionNode* subject_) noexcept { subject.reset(subject_); }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
};

class BoundPtrToRefNode : public BoundExpressionNode
{
public:
    BoundPtrToRefNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundPtrToRefNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    inline BoundExpressionNode* Subject() noexcept { return subject.get(); }
    inline void SetSubject(BoundExpressionNode* subject_) noexcept { subject.reset(subject_); }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    inline BoundExpressionNode* Subject() const noexcept { return subject.get(); }
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
};

class BoundDefaultInitNode : public BoundExpressionNode
{
public:
    BoundDefaultInitNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundDefaultInitNode(BoundExpressionNode* subject_, const soul::ast::FullSpan& fullSpan_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    inline BoundExpressionNode* Subject() noexcept { return subject.get(); }
    inline void SetSubject(BoundExpressionNode* subject_) noexcept { subject.reset(subject_); }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
};

class BoundTemporaryNode : public BoundExpressionNode
{
public:
    BoundTemporaryNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundTemporaryNode(BoundExpressionNode* rvalueExpr_, BoundVariableNode* backingStore_, const soul::ast::FullSpan& fullSpan_) noexcept;
    inline BoundExpressionNode* RvalueExpr() noexcept { return rvalueExpr.get(); }
    inline void SetRvalueExpr(BoundExpressionNode* rvalueExpr_) noexcept { rvalueExpr.reset(rvalueExpr_); }
    inline BoundVariableNode* BackingStore() noexcept { return backingStore.get(); }
    inline void SetBackingStore(BoundVariableNode* backingStore_) noexcept { backingStore.reset(backingStore_); }
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    bool HasValue(Context* context) const noexcept override { return true; }
    bool IsLvalueExpression(Context* context) const noexcept override { return true; }
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> rvalueExpr;
    std::unique_ptr<BoundVariableNode> backingStore;
};

class BoundConstructTemporaryNode : public BoundExpressionNode
{
public:
    BoundConstructTemporaryNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundConstructTemporaryNode(BoundExpressionNode* constructorCall_, BoundExpressionNode* temporary_, const soul::ast::FullSpan& fullSpan_) noexcept;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Store(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    bool HasValue(Context* context) const noexcept override { return true; }
    bool IsLvalueExpression(Context* context) const noexcept override { return true; }
    inline BoundExpressionNode* ConstructorCall() const noexcept { return constructorCall.get(); }
    inline void SetConstructorCall(BoundExpressionNode* constructorCall_) noexcept { constructorCall.reset(constructorCall_); }
    inline BoundExpressionNode* Temporary() const noexcept { return temporary.get(); }
    inline void SetTemporary(BoundExpressionNode* temporary_) noexcept { temporary.reset(temporary_); }
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> constructorCall;
    std::unique_ptr<BoundExpressionNode> temporary;
};

class BoundDestructTemporariesNode : public BoundExpressionNode
{
public:
    BoundDestructTemporariesNode(const soul::ast::FullSpan& fullSpan_, std::vector<std::unique_ptr<BoundFunctionCallNode>>&& temporaryDestructorCalls_);
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Accept(BoundTreeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
    void Merge(BoundDestructTemporariesNode* that);
private:
    std::vector<std::unique_ptr<BoundFunctionCallNode>> temporaryDestructorCalls;
};

class BoundConstructExpressionNode : public BoundExpressionNode
{
public:
    BoundConstructExpressionNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundConstructExpressionNode(BoundExpressionNode* allocation_, BoundExpressionNode* constructObjectCall_, TypeSymbol* type_, bool hasPlacement_,
        const soul::ast::FullSpan& fullSpan_) noexcept;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void Accept(BoundTreeVisitor& visitor) override;
    bool MayThrow() const noexcept override;
    bool HasValue(Context* context) const noexcept override { return !hasPlacement; }
    BoundExpressionNode* Clone() const override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
    inline BoundExpressionNode* Allocation() const noexcept { return allocation.get(); }
    inline void SetAllocation(BoundExpressionNode* allocation_) noexcept { allocation.reset(allocation_); }
    inline BoundExpressionNode* ConstructObjectCall() const noexcept { return constructObjectCall.get(); }
    inline void SetConstructObjectCall(BoundExpressionNode* constructObjectCall_) noexcept { constructObjectCall.reset(constructObjectCall_); }
    inline bool HasPlacement() const noexcept { return hasPlacement; }
    inline void SetHasPlacement(bool hasPlacement_) noexcept { hasPlacement = hasPlacement_; }
private:
    std::unique_ptr<BoundExpressionNode> allocation;
    std::unique_ptr<BoundExpressionNode> constructObjectCall;
    bool hasPlacement;
};

class BoundGlobalVariableDefinitionNode : public BoundNode
{
public:
    BoundGlobalVariableDefinitionNode(VariableSymbol* globalVariable_, const soul::ast::FullSpan& fullSpan_) noexcept;
    VariableSymbol* GetGlobalVariable() const noexcept { return globalVariable; }
    void Accept(BoundTreeVisitor& visitor) override;
private:
    VariableSymbol* globalVariable;
};

class BoundEmptyDestructorNode : public BoundExpressionNode
{
public:
    BoundEmptyDestructorNode(const soul::ast::FullSpan& fullSpan_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
};

class BoundFunctionValueNode : public BoundExpressionNode
{
public:
    BoundFunctionValueNode(FunctionSymbol* function_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    FunctionSymbol* GetFunction() const { return function; }
private:
    FunctionSymbol* function;
};

class BoundVariableAsVoidPtrNode : public BoundExpressionNode
{
public:
    BoundVariableAsVoidPtrNode(const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    BoundVariableAsVoidPtrNode(BoundExpressionNode* addrOfBoundVariable_, const soul::ast::FullSpan& fullSpan_, TypeSymbol* type_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    inline BoundExpressionNode* AddrOfBoundVariable() const noexcept { return addrOfBoundVariable.get(); }
    inline void SetAddrOfBoundVariable(BoundExpressionNode* addrOfBoundVariable_) noexcept { addrOfBoundVariable.reset(addrOfBoundVariable_); }
    BoundExpressionNode* Clone() const override;
    void Load(Emitter& emitter, OperationFlags flags, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void ModifyTypes(const soul::ast::FullSpan& fullSpan, Context* context) override;
private:
    std::unique_ptr<BoundExpressionNode> addrOfBoundVariable;
};

class BoundOperatorFnNode : public BoundExpressionNode
{
public:
    BoundOperatorFnNode(otava::ast::NodeKind operatorFnNodeKind_, const soul::ast::FullSpan& fullSpan_) noexcept;
    void Accept(BoundTreeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
    inline otava::ast::NodeKind OperatorFnNodeKind() const { return operatorFnNodeKind; }
private:
    otava::ast::NodeKind operatorFnNodeKind;
};

bool InDirectSwitchStatement(BoundStatementNode* statement);

} // namespace otava::symbols
