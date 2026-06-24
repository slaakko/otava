// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.templates;

import otava.symbols.context;
import otava.symbols.declaration;
import otava.symbols.declarator;
import otava.symbols.evaluator;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.variable_symbol;
import otava.symbols.writer;
import otava.symbols.reader;
import otava.ast.identifier;
import otava.ast.function;
import otava.ast.templates;
import otava.ast.visitor;

namespace otava::symbols {

TypenameConstraintSymbol::TypenameConstraintSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_)
{
}

TemplateParameterSymbol::TemplateParameterSymbol(Module* module_, SymbolId id_) :
    TypeSymbol(module_, id_), defaultTemplateArgNode(nullptr), parameterSymbol(nullptr), parameterSymbolId(zeroSymbolId), index(-1)
{
}

TemplateParameterSymbol::TemplateParameterSymbol(Module* module_, SymbolId id_, const std::string& name_) :
    TypeSymbol(module_, id_, name_), defaultTemplateArgNode(nullptr), parameterSymbol(nullptr), parameterSymbolId(zeroSymbolId), index(-1)
{
}

void TemplateParameterSymbol::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    TypeSymbol::AddSymbol(symbol, fullSpan, context);
    if (symbol->IsTemplateParameterSymbol())
    {
        parameterSymbol = static_cast<ParameterSymbol*>(symbol);
    }
}

void TemplateParameterSymbol::SetDefaultTemplateArg(otava::ast::Node* defaultTemplateArgNode_) noexcept
{
    if (defaultTemplateArgNode_)
    {
        defaultTemplateArgNode.reset(defaultTemplateArgNode_->Clone());
    }
}

void TemplateParameterSymbol::Write(Writer& writer)
{
    TypeSymbol::Write(writer);
    otava::symbols::WriteNode(writer, defaultTemplateArgNode.get(), astNodeHeader);
    if (parameterSymbol)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(parameterSymbol->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    writer.GetBinaryStreamWriter().Write(index);
}

void TemplateParameterSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    defaultTemplateArgNode = otava::symbols::ReadNode(reader, GetModule(), astNodeHeader);
    reader.CurrentReader().Skip(ToUnderlying(astNodeHeader.nodeLength));
    parameterSymbolId = SymbolId(reader.CurrentReader().ReadUInt());
    index = reader.CurrentReader().ReadInt();
}

ParameterSymbol* TemplateParameterSymbol::GetParameterSymbol(Context* context) const
{
    if (IsReadOnly() && parameterSymbolId != zeroSymbolId)
    {
        parameterSymbol = GetModule()->GetSymbolTable()->GetParameterSymbol(parameterSymbolId, context);
        if (!parameterSymbol)
        {
            ThrowException("parameter symbol " + std::to_string(ToUnderlying(parameterSymbolId)) + " not found from module '" + GetModule()->Name() + "'");
        }
    }
    return parameterSymbol;
}

TypeSymbol* TemplateParameterSymbol::Unify(TypeSymbol* argType, Context* context)
{
    return argType;
}

TypeSymbol* TemplateParameterSymbol::UnifyTemplateArgumentType(const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>,
    TemplateParamEqual>& templateParameterMap,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    auto it = templateParameterMap.find(this);
    if (it != templateParameterMap.cend())
    {
        TypeSymbol* templateArgumentType = it->second;
        return templateArgumentType;
    }
    else
    {
        return nullptr;
    }
}

bool TemplateParameterSymbol::IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const
{
    const Symbol* thisSymbol = this;
    if (visited.find(thisSymbol) == visited.end())
    {
        visited.insert(thisSymbol);
    }
    return true;
}

BoundTemplateParameterSymbol::BoundTemplateParameterSymbol(Module* module_, SymbolId id_, const std::string& name_) :
    TypeSymbol(module_, id_, name_), templateParameterSymbol(nullptr), boundSymbol(nullptr)
{
}

TemplateDeclarationSymbol::TemplateDeclarationSymbol(Module* module_, SymbolId id_) : ContainerSymbol(module_, id_), read(false)
{
    GetScope()->SetKind(ScopeKind::templateDeclarationScope);
}

TemplateDeclarationSymbol::TemplateDeclarationSymbol(Module* module_, SymbolId id_, const std::string& name_) : ContainerSymbol(module_, id_, name_), read(false)
{
    GetScope()->SetKind(ScopeKind::templateDeclarationScope);
}

void TemplateDeclarationSymbol::AddSymbol(Symbol* symbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ContainerSymbol::AddSymbol(symbol, fullSpan, context);
    if (symbol->IsTemplateParameterSymbol())
    {
        templateParameters.push_back(static_cast<TemplateParameterSymbol*>(symbol));
    }
}

void TemplateDeclarationSymbol::Write(Writer& writer)
{
    ContainerSymbol::Write(writer);
    Cardinality count = Cardinality(templateParameters.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (TemplateParameterSymbol* templateParameter : templateParameters)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(templateParameter->Id()));
    }
}

void TemplateDeclarationSymbol::Read(Reader& reader)
{
    ContainerSymbol::Read(reader);
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId templateParameterId = SymbolId(reader.CurrentReader().ReadUInt());
        templateParameterIds.push_back(templateParameterId);
    }
}

const std::vector<TemplateParameterSymbol*>& TemplateDeclarationSymbol::TemplateParameters(Context* context) const
{
    if (IsReadOnly() && !read)
    {
        read = true;
        for (SymbolId templateParameterId : templateParameterIds)
        {
            TemplateParameterSymbol* templateParameterSymbol = GetModule()->GetSymbolTable()->GetTemplateParameterSymbol(templateParameterId, context);
            if (!templateParameterSymbol)
            {
                ThrowException("template parameter symbol id " + std::to_string(ToUnderlying(templateParameterId)) + " not found from module '" + 
                    GetModule()->Name() + "'");
            }
            templateParameters.push_back(templateParameterSymbol);
        }
    }
    return templateParameters;
}

Cardinality TemplateDeclarationSymbol::Arity() const
{
    if (IsReadOnly())
    {
        if (read)
        {
            return Cardinality(templateParameters.size());
        }
        else
        {
            return Cardinality(templateParameterIds.size());
        }
    }
    return Cardinality(templateParameters.size());
}

void BeginTemplateDeclaration(otava::ast::Node* node, Context* context)
{
    context->GetSymbolTable()->BeginTemplateDeclaration(node, context);
    context->PushSetFlag(ContextFlags::parsingTemplateDeclaration);
}

void EndTemplateDeclaration(otava::ast::Node* node, Context* context)
{
    Symbol* symbol = context->GetSymbolTable()->CurrentScope()->GetSymbol();
    if (!symbol->IsTemplateDeclarationSymbol())
    {
        ThrowException("otava.symbols.templates: EndTemplateDeclaration(): template scope expected", node->GetFullSpan(), context);
    }
    context->PopFlags();
    context->GetSymbolTable()->EndTemplateDeclaration();
}

void RemoveTemplateDeclaration(Context* context)
{
    context->GetSymbolTable()->RemoveTemplateDeclaration();
}

class TemplateParameterCreator : public otava::ast::DefaultVisitor
{
public:
    TemplateParameterCreator(Context* context_, int index_);
    void Visit(otava::ast::TypeParameterNode& node) override;
    void Visit(otava::ast::TypenameNode& node) override;
    void Visit(otava::ast::ClassNode& node) override;
    void Visit(otava::ast::ParameterNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
private:
    Context* context;
    int index;
    Symbol* constraint;
    bool resolveConstraint;
    bool resolveTemplateParamName;
    std::string templateParamName;
};

TemplateParameterCreator::TemplateParameterCreator(Context* context_, int index_) :
    context(context_), index(index_), constraint(nullptr), resolveConstraint(false), resolveTemplateParamName(false)
{
}

void TemplateParameterCreator::Visit(otava::ast::TypeParameterNode& node)
{
    resolveConstraint = true;
    node.TypeConstraint()->Accept(*this);
    resolveConstraint = false;
    if (node.Identifier())
    {
        resolveTemplateParamName = true;
        node.Identifier()->Accept(*this);
        resolveTemplateParamName = false;
    }
    otava::ast::Node* defaultTemplateArgNode = nullptr;
    if (node.TypeId())
    {
        defaultTemplateArgNode = node.TypeId();
    }
    context->GetSymbolTable()->AddTemplateParameter(templateParamName, &node, constraint, index, nullptr, defaultTemplateArgNode, context);
}

void TemplateParameterCreator::Visit(otava::ast::TypenameNode& node)
{
    constraint = context->GetSymbolTable()->GetTypenameConstraintSymbol();
}

void TemplateParameterCreator::Visit(otava::ast::ClassNode& node)
{
    constraint = context->GetSymbolTable()->GetTypenameConstraintSymbol();
}

void TemplateParameterCreator::Visit(otava::ast::ParameterNode& node)
{
    Declaration declaration = ProcessParameterDeclaration(&node, context);
    TypeSymbol* type = declaration.type;
    if (declaration.declarator->Kind() == DeclaratorKind::simpleDeclarator)
    {
        SimpleDeclarator* declarator = static_cast<SimpleDeclarator*>(declaration.declarator.get());
        std::string templateParamName = declarator->Name();
        Value* value = nullptr;
        if (node.Initializer())
        {
            value = Evaluate(node.Initializer(), context);
        }
        ParameterSymbol* parameter = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), templateParamName);
        parameter->SetType(type, context);
        parameter->SetDefaultValue(node.Initializer());
        context->GetSymbolTable()->AddTemplateParameter(templateParamName, &node, nullptr, index, parameter, nullptr, context);
    }
    else
    {
        ThrowException("simple declarator expected", node.GetFullSpan(), context);
    }
}

void TemplateParameterCreator::Visit(otava::ast::IdentifierNode& node)
{
    if (resolveTemplateParamName)
    {
        templateParamName = node.Str();
    }
}

void AddTemplateParameter(otava::ast::Node* templateParameterNode, int index, Context* context)
{
    TemplateParameterCreator creator(context, index);
    templateParameterNode->Accept(creator);
}

bool TemplateArgCanBeTypeId(otava::ast::Node* templateIdNode, int index)
{
    return true; // TODO
}

ExplicitInstantiationSymbol::ExplicitInstantiationSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), specialization(nullptr), destructor(nullptr), specializationId(zeroSymbolId), contentFetched(false)
{
}

ExplicitInstantiationSymbol::ExplicitInstantiationSymbol(Module* module_, SymbolId id_, ClassTemplateSpecializationSymbol* specialization_) :
    Symbol(module_, id_, "@explicitInstantiation"), specialization(specialization_), destructor(nullptr), specializationId(zeroSymbolId), contentFetched(false)
{
}

ExplicitInstantiationSymbol::~ExplicitInstantiationSymbol()
{
/*
    for (ExplicitlyInstantiatedFunctionDefinitionSymbol* functionDefinitionSymbol : functionDefinitionSymbols)
    {
        delete functionDefinitionSymbol;
    }
*/
}

ClassTemplateSpecializationSymbol* ExplicitInstantiationSymbol::Specialization(Context* context) const
{
    if (specialization)
    {
        return specialization;
    }
    if (IsReadOnly() && specializationId != zeroSymbolId)
    {
        specialization = GetModule()->GetSymbolTable()->GetClassTemplateSpecializationSymbol(specializationId, context);
    }
    return specialization;
}

void ExplicitInstantiationSymbol::AddFunctionDefinitionSymbol(FunctionDefinitionSymbol* functionDefinitionSymbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
    functionDefinitionSymbol->SetFlag(FunctionSymbolFlags::fixedIrName);
    if (functionDefinitionSymbol->Declaration())
    {
        FunctionSymbol* declaration = functionDefinitionSymbol->Declaration();
        declaration->SetFlag(FunctionSymbolFlags::fixedIrName);
    }
    ExplicitlyInstantiatedFunctionDefinitionSymbol* explicitlyInstantiatedSymbol = new ExplicitlyInstantiatedFunctionDefinitionSymbol(
        context->GetModule(), context->GetNextSymbolId(SymbolKind::explicitlyInstantiatedFunctionDefinitionSymbol), functionDefinitionSymbol, fullSpan, context);
    explicitlyInstantiatedSymbol->SetFunctionKind(functionDefinitionSymbol->GetFunctionKind());
    functionDefinitionSymbols.push_back(explicitlyInstantiatedSymbol);
    functionDefinitionSymbolMap[functionDefinitionSymbol->DefIndex()] = explicitlyInstantiatedSymbol;
    context->GetModule()->GetSymbolTable()->GlobalNs()->AddSymbol(explicitlyInstantiatedSymbol, fullSpan, context);
    if (explicitlyInstantiatedSymbol->GetFunctionKind() == FunctionKind::destructor)
    {
        destructor = explicitlyInstantiatedSymbol;
    }
}

FunctionDefinitionSymbol* ExplicitInstantiationSymbol::GetFunctionDefinitionSymbol(int index, Context* context)  
{
    if (IsReadOnly() && !contentFetched)
    {
        GetContent(context);
    }
    auto it = functionDefinitionSymbolMap.find(index);
    if (it != functionDefinitionSymbolMap.end())
    {
        FunctionDefinitionSymbol* functionDefinitionSymbol = it->second;
        ClassTemplateSpecializationSymbol* sp = Specialization(context);
        functionDefinitionSymbol->SetParent(sp);
        return functionDefinitionSymbol;
    }
    else
    {
        ThrowException("explicitly instantiated function " + std::to_string(index) + " not found");
    }
    return nullptr;
}

FunctionDefinitionSymbol* ExplicitInstantiationSymbol::Destructor() const noexcept
{
    if (destructor)
    {
        destructor->SetParent(specialization);
    }
    return destructor;
}

void ExplicitInstantiationSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(specialization->Id()));
    Cardinality n = Cardinality(functionDefinitionSymbols.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(n));
    for (auto* fnDefSymbol : functionDefinitionSymbols)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(fnDefSymbol->Id()));
    }
}

void ExplicitInstantiationSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    specializationId = SymbolId(reader.CurrentReader().ReadUInt());
    Cardinality n = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(n); ++i)
    {
        SymbolId id = SymbolId(reader.CurrentReader().ReadUInt());
        functionDefinitionSymbolIds.push_back(id);
    }
}

void ExplicitInstantiationSymbol::GetContent(Context* context)
{
    if (contentFetched) return;
    contentFetched = true;
    for (SymbolId functionDefinitionSymbolId : functionDefinitionSymbolIds)
    {
        ExplicitlyInstantiatedFunctionDefinitionSymbol* 
            functionSymbol =GetModule()->GetSymbolTable()->GetExplicitlyInstantiatedFunctionDefinitionSymbol(functionDefinitionSymbolId, context);
        if (!functionSymbol)
        {
            ThrowException("explicitly instantiated function definition symbol id " + std::to_string(ToUnderlying(functionDefinitionSymbolId)) + " not found");
        }
        functionDefinitionSymbols.push_back(functionSymbol);
        functionDefinitionSymbolMap[functionSymbol->DefIndex()] = functionSymbol;
    }
}

class ExplicitInstantiationProcessor : public otava::ast::DefaultVisitor
{
public:
    ExplicitInstantiationProcessor(Context* context_);
    void Visit(otava::ast::ExplicitInstantiationNode& node) override;
private:
    Context* context;
};

ExplicitInstantiationProcessor::ExplicitInstantiationProcessor(Context* context_) : context(context_)
{
}

void ExplicitInstantiationProcessor::Visit(otava::ast::ExplicitInstantiationNode& node)
{
    TypeSymbol* type = ProcessExplicitInstantiationDeclaration(node.Declaration(), context);
    if (type && type->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(type);
        SpecializationKey key;
        key.typeSymbolId = specialization->ClassTemplate(context)->Id();
        for (Symbol* templateArgument : specialization->TemplateArguments(context))
        {
            key.templateArgumentIds.push_back(templateArgument->Id());
        }
        ExplicitInstantiationSymbol* explicitInstantiationSymbol = context->GetSymbolTable()->GetExplicitInstantiation(key, context);
        if (!explicitInstantiationSymbol)
        {
            specialization->MakeVTab(context, node.GetFullSpan());
            explicitInstantiationSymbol = new ExplicitInstantiationSymbol(context->GetModule(), 
                context->GetNextSymbolId(SymbolKind::explicitInstantiationSymbol), specialization);
            ClassTypeSymbol* classTemplate = specialization->ClassTemplate(context);
            bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
            context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
            for (const auto& memFnDefSymbolPair : classTemplate->MemFnDefSymbolMap())
            {
                FunctionDefinitionSymbol* memFnDefSymbol = memFnDefSymbolPair.second;
                std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>, TemplateParamEqual> templateParameterMap;
                FunctionSymbol* instantiatedFunctionSymbol = InstantiateMemFnOfClassTemplate(
                    memFnDefSymbol, specialization, templateParameterMap, node.GetFullSpan(), context);
                if (instantiatedFunctionSymbol->IsFunctionDefinitionSymbol())
                {
                    FunctionDefinitionSymbol* instantiatedMemFnDefSymbol = static_cast<FunctionDefinitionSymbol*>(instantiatedFunctionSymbol);
                    explicitInstantiationSymbol->AddFunctionDefinitionSymbol(instantiatedMemFnDefSymbol, node.GetFullSpan(), context);
                }
                else
                {
                    ThrowException("function definition symbol expected", node.GetFullSpan(), context);
                }
            }
            context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
            context->GetSymbolTable()->AddExplicitInstantiation(explicitInstantiationSymbol, node.GetFullSpan(), context);
        }
    }
    else
    {
        ThrowException("class template specialization expected", node.GetFullSpan(), context);
    }
}

void ProcessExplicitInstantiation(otava::ast::Node* node, Context* context)
{
    ExplicitInstantiationProcessor processor(context);
    node->Accept(processor);
}

} // namespace otava::symbols
