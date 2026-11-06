// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.operation_repository;

import otava.symbols.id;
import soul.ast.span;
import std;

export namespace otava::symbols {

constexpr Cardinality inlineClassOperationsThreshold = Cardinality(4);

class BoundExpressionNode;
class FunctionSymbol;
class FunctionDefinitionSymbol;
class ClassTypeSymbol;
class Context;

class Operation
{
public:
    Operation(const std::string& groupName_, int arity_);
    virtual ~Operation();
    inline const std::string& GroupName() const noexcept { return groupName; }
    inline int Arity() const noexcept { return arity; }
    virtual FunctionSymbol* Get(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) = 0;
private:
    std::string groupName;
    int arity;
};

class OperationGroup
{
public:
    OperationGroup(const std::string& name_);
    inline const std::string& Name() const noexcept { return name; }
    void AddOperation(Operation* operation);
    FunctionSymbol* GetOperation(std::vector<std::unique_ptr<BoundExpressionNode>>& args, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
private:
    std::string name;
    std::map<int, std::vector<Operation*>> arityOperationsMap;
    std::vector<Operation*> anyArityOperations;
};

class OperationRepository
{
public:
    OperationRepository();
    void AddOperation(Operation* operation);
    FunctionSymbol* GetOperation(const std::string& groupName, std::vector<std::unique_ptr<BoundExpressionNode>>& args,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);
private:
    OperationGroup* GetOrInsertOperationGroup(const std::string& operationGroupName);
    std::map<std::string, OperationGroup*> groupMap;
    std::vector<std::unique_ptr<OperationGroup>> groups;
    std::vector<std::unique_ptr<Operation>> operations;
};

FunctionDefinitionSymbol* GenerateClassDefaultCtor(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context);
FunctionDefinitionSymbol* GenerateClassCopyCtor(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context);
FunctionDefinitionSymbol* GenerateClassMoveCtor(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context);
FunctionDefinitionSymbol* GenerateClassCopyAssignment(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context);
FunctionDefinitionSymbol* GenerateClassMoveAssignment(ClassTypeSymbol* classType, const soul::ast::FullSpan& fullSpan, Context* context);

} // namespace otava::symbols
