// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.templates;

import std;
import otava.symbols.ast_node_io;
import otava.symbols.container_symbol;
import otava.symbols.type_symbol;
import otava.ast.node;

export namespace otava::symbols {

class Context;
class FunctionDefinitionSymbol;
class ExplicitlyInstantiatedFunctionDefinitionSymbol;
class ParameterSymbol;

class TypenameConstraintSymbol : public TypeSymbol
{
public:
    TypenameConstraintSymbol(Module* module_, SymbolId id_);
};

class TemplateParameterSymbol : public TypeSymbol
{
public:
    TemplateParameterSymbol(Module* module_, SymbolId id_);
    TemplateParameterSymbol(Module* module_, SymbolId id_, const std::string& name_);
    void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context) override;
    void SetDefaultTemplateArg(otava::ast::Node* defaultTemplateArgNode_) noexcept;
    inline otava::ast::Node* DefaultTemplateArg() const noexcept { return defaultTemplateArgNode.get(); }
    ParameterSymbol* GetParameterSymbol(Context* context) const;
    inline void SetIndex(int index_) noexcept { index = index_; }
    inline int GetIndex() const noexcept { return index; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    TypeSymbol* Unify(TypeSymbol* argType, Context* context) override;
    TypeSymbol* UnifyTemplateArgumentType(const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>,
        TemplateParamEqual>& templateParameterMap, const soul::ast::FullSpan& fullSpan, Context* context) override;
    bool IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const override;
private:
    AstNodeHeader astNodeHeader;
    std::unique_ptr<otava::ast::Node> defaultTemplateArgNode;
    mutable ParameterSymbol* parameterSymbol;
    SymbolId parameterSymbolId;
    int index;
};

class BoundTemplateParameterSymbol : public TypeSymbol
{
public:
    BoundTemplateParameterSymbol(Module* module_, SymbolId id_, const std::string& name_);
    bool IsExportSymbol(Context* context) const noexcept override { return false; }
    inline void SetTemplateParameterSymbol(TemplateParameterSymbol* templateParameterSymbol_) noexcept { templateParameterSymbol = templateParameterSymbol_; }
    inline TemplateParameterSymbol* GetTemplateParameterSymbol() const noexcept { return templateParameterSymbol; }
    inline void SetBoundSymbol(Symbol* symbol) noexcept { boundSymbol = symbol; }
    inline Symbol* BoundSymbol() const noexcept { return boundSymbol; }
private:
    TemplateParameterSymbol* templateParameterSymbol;
    Symbol* boundSymbol;
};

class TemplateDeclarationSymbol : public ContainerSymbol
{
public:
    TemplateDeclarationSymbol(Module* module_, SymbolId id_);
    TemplateDeclarationSymbol(Module* module_, SymbolId id_, const std::string& name_);
    void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context) override;
    Cardinality Arity() const;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    const std::vector<TemplateParameterSymbol*>& TemplateParameters(Context* context) const;
private:
    mutable std::vector<TemplateParameterSymbol*> templateParameters;
    std::vector<SymbolId> templateParameterIds;
    mutable bool read;
};

class ExplicitInstantiationSymbol : public Symbol
{
public:
    ExplicitInstantiationSymbol(Module* module_, SymbolId id_);
    ExplicitInstantiationSymbol(Module* module_, SymbolId id_, ClassTemplateSpecializationSymbol* specialization_);
    ~ExplicitInstantiationSymbol();
    ClassTemplateSpecializationSymbol* Specialization(Context* context) const;
    void AddFunctionDefinitionSymbol(FunctionDefinitionSymbol* functionDefinitionSymbol, const soul::ast::FullSpan& fullSpan, Context* context);
    FunctionDefinitionSymbol* GetFunctionDefinitionSymbol(int index, Context* context);
    FunctionDefinitionSymbol* Destructor() const noexcept;
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    mutable ClassTemplateSpecializationSymbol* specialization;
    SymbolId specializationId;
    std::vector<ExplicitlyInstantiatedFunctionDefinitionSymbol*> functionDefinitionSymbols;
    std::vector<SymbolId> functionDefinitionSymbolIds;
    std::unordered_map<std::int32_t, ExplicitlyInstantiatedFunctionDefinitionSymbol*> functionDefinitionSymbolMap;
    FunctionDefinitionSymbol* destructor;
    bool contentFetched;
    void GetContent(Context* context);
};

void BeginTemplateDeclaration(otava::ast::Node* node, Context* context);
void EndTemplateDeclaration(otava::ast::Node* node, Context* context);
void RemoveTemplateDeclaration(Context* context);
void AddTemplateParameter(otava::ast::Node* templateParameterNode, int index, Context* context);
bool TemplateArgCanBeTypeId(otava::ast::Node* templateIdNode, int index);
void ProcessExplicitInstantiation(otava::ast::Node* node, Context* context);

} // namespace otava::symbol
