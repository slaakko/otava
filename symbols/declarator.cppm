// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.declarator;

import std;
import otava.symbols.function_kind;
import otava.symbols.symbol;
import otava.ast.node;

export namespace otava::symbols {

class TypeSymbol;
class VariableSymbol;

enum class DeclaratorKind : std::uint8_t
{
    simpleDeclarator = 0, functionDeclarator = 1, arrayDeclarator = 2
};

class Value;

class Declarator
{
public:
    Declarator(DeclaratorKind kind_, const std::string& name_, otava::ast::Node* node_);
    virtual ~Declarator();
    inline bool IsSimpleDeclarator() const noexcept { return kind == DeclaratorKind::simpleDeclarator; }
    inline bool IsFunctionDeclarator() const noexcept { return kind == DeclaratorKind::functionDeclarator; }
    inline bool IsArrayDeclarator() const noexcept { return kind == DeclaratorKind::arrayDeclarator; }
    DeclaratorKind Kind() const noexcept { return kind; }
    const std::string& Name() const noexcept { return name; }
    otava::ast::Node* Node() const noexcept { return node; }
private:
    DeclaratorKind kind;
    std::string name;
    otava::ast::Node* node;
};

class SimpleDeclarator : public Declarator
{
public:
    SimpleDeclarator(const std::string& name_, otava::ast::Node* node_);
};

struct Declaration
{
    Declaration() noexcept;
    Declaration(DeclarationFlags flags_, TypeSymbol* type_, Declarator* declarator_) noexcept;
    DeclarationFlags flags;
    TypeSymbol* type;
    std::unique_ptr<Declarator> declarator;
    Value* value;
    otava::ast::Node* initializer;
    VariableSymbol* variable;
    std::u32string name;
};

struct DeclarationList
{
    std::vector<Declaration> declarations;
};

class FunctionDeclarator : public Declarator
{
public:
    FunctionDeclarator(const std::string& name_, otava::ast::Node* node_, FunctionKind kind_, FunctionQualifiers qualifiers_, Scope* scope_);
    inline FunctionKind GetFunctionKind() const noexcept { return kind; }
    inline FunctionQualifiers GetFunctionQualifiers() const noexcept { return qualifiers; }
    void AddQualifier(FunctionQualifiers qualifier);
    void AddParameterDeclaration(Declaration&& parameterDeclaration);
    inline const std::vector<Declaration>& ParameterDeclarations() const noexcept { return parameterDeclarations; }
    inline Scope* GetScope() const noexcept { return scope; }
    void SetTemplateArgs(const std::vector<TypeSymbol*>& templateArgs_);
    inline const std::vector<TypeSymbol*>& TemplateArgs() const noexcept { return templateArgs; }
private:
    FunctionKind kind;
    FunctionQualifiers qualifiers;
    std::vector<Declaration> parameterDeclarations;
    std::vector<TypeSymbol*> templateArgs;
    Scope* scope;
};

class ArrayDeclarator : public Declarator
{
public:
    ArrayDeclarator(const std::string& name_, otava::ast::Node* node_, std::int64_t size_);
    inline std::int64_t Size() const noexcept { return size; }
    inline void SetSize(std::int64_t size_) noexcept { size = size_; }
private:
    std::int64_t size;
};

class Context;

std::unique_ptr<DeclarationList> ProcessInitDeclaratorList(TypeSymbol* baseType, otava::ast::Node* declarationNode, otava::ast::Node* initDeclaratorList,
    DeclarationFlags flags, Context* context);
std::unique_ptr<DeclarationList> ProcessMemberDeclaratorList(TypeSymbol* baseType, otava::ast::Node* declarationNode, otava::ast::Node* memberDeclaratorList, DeclarationFlags flags, Context* context);
Declaration ProcessDeclarator(TypeSymbol* baseType, otava::ast::Node* declarator, otava::ast::Node* declarationNode, DeclarationFlags flags, FunctionQualifiers qualifiers,
    Context* context);

} // namespace otava::symbols
