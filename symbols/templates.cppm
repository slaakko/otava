// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.templates;

import std;
import otava.symbols.container_symbol;
import otava.symbols.type_symbol;
import otava.ast.node;

export namespace otava::symbols {

class Context;

class TypenameConstraintSymbol : public TypeSymbol
{
public:
    TypenameConstraintSymbol();
};

class TemplateParameterSymbol : public TypeSymbol
{
public:
    TemplateParameterSymbol(Module* module_, SymbolId id_);
};

class TemplateDeclarationSymbol : public ContainerSymbol
{
public:
    TemplateDeclarationSymbol(Module* module_, SymbolId id_);
    TemplateDeclarationSymbol(Module* module_, SymbolId id_, const std::string& name_);
    void AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context) override;
    inline int Arity() const noexcept { return templateParameters.size(); }
    inline const std::vector<TemplateParameterSymbol*>& TemplateParameters() const noexcept { return templateParameters; }
private:
    std::vector<TemplateParameterSymbol*> templateParameters;
};

void BeginTemplateDeclaration(otava::ast::Node* node, Context* context);
void EndTemplateDeclaration(otava::ast::Node* node, Context* context);
void RemoveTemplateDeclaration(Context* context);
void AddTemplateParameter(otava::ast::Node* templateParameterNode, int index, Context* context);
bool TemplateArgCanBeTypeId(otava::ast::Node* templateIdNode, int index);
void ProcessExplicitInstantiation(otava::ast::Node* node, Context* context);

} // namespace otava::symbol
