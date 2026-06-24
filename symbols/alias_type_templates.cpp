// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.alias_type_templates;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.instantiator;
import otava.symbols.templates;
import otava.symbols.type_resolver;
import otava.symbols.writer;
import otava.symbols.reader;

namespace otava::symbols {

AliasTypeTemplateSpecializationSymbol::AliasTypeTemplateSpecializationSymbol(Module* module_, SymbolId id_) : 
    AliasTypeSymbol(module_, id_), instantiated(false), aliasTypeTemplate(nullptr), templateArgumentsRead(false)
{
}

AliasTypeTemplateSpecializationSymbol::AliasTypeTemplateSpecializationSymbol(Module* module_, SymbolId id_, const std::string& name_) :
    AliasTypeSymbol(module_, id_, name_), instantiated(false), aliasTypeTemplate(nullptr), templateArgumentsRead(false)
{
}

void AliasTypeTemplateSpecializationSymbol::AddTemplateArgument(Symbol* templateArgument)
{
    templateArguments.push_back(templateArgument);
}

void AliasTypeTemplateSpecializationSymbol::Write(Writer& writer)
{
    AliasTypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(instantiated);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(aliasTypeTemplate->Id()));
    Cardinality count = Cardinality(templateArguments.size());
    writer.GetBinaryStreamWriter().Write(ToUnderlying(count));
    for (const auto* templateArgument : templateArguments)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(templateArgument->Id()));
    }
}

void AliasTypeTemplateSpecializationSymbol::Read(Reader& reader)
{
    AliasTypeSymbol::Read(reader);
    instantiated = reader.CurrentReader().ReadBool();
    aliasTypeTemplateId = SymbolId(reader.CurrentReader().ReadUInt());
    Cardinality count = Cardinality(reader.CurrentReader().ReadUInt());
    for (Index i = Index(0); i < Index(count); ++i)
    {
        SymbolId templateArgumentId = SymbolId(reader.CurrentReader().ReadUInt());
        templateArgumentIds.push_back(templateArgumentId);
    }
}

TypeSymbol* AliasTypeTemplateSpecializationSymbol::AliasTypeTemplate(Context* context) 
{ 
    if (aliasTypeTemplate)
    {
        return aliasTypeTemplate;
    }
    if (IsReadOnly() && aliasTypeTemplateId != zeroSymbolId)
    {
        aliasTypeTemplate = context->GetSymbolTable()->GetTypeSymbol(aliasTypeTemplateId, context);
    }
    return aliasTypeTemplate;
}

const std::vector<Symbol*>& AliasTypeTemplateSpecializationSymbol::TemplateArguments(Context* context)
{
    if (IsReadOnly() && !templateArgumentsRead)
    {
        templateArgumentsRead = true;
        for (SymbolId templateArgumentId : templateArgumentIds)
        {
            Symbol* templateArgument = context->GetSymbolTable()->GetSymbol(templateArgumentId, context);
            templateArguments.push_back(templateArgument);
        }
    }
    return templateArguments;
}

TypeSymbol* InstantiateAliasTypeSymbol(TypeSymbol* typeSymbol, const std::vector<Symbol*>& templateArgs, otava::ast::TemplateIdNode* node, Context* context)
{
    AliasTypeTemplateSpecializationSymbol* specialization = context->GetSymbolTable()->MakeAliasTypeTemplateSpecialization(typeSymbol, templateArgs, context);
    if (specialization->Instantiated()) return specialization;
    otava::ast::Node* aliasTypeNode = context->GetSymbolTable()->GetNodeNothrow(typeSymbol);
    if (!aliasTypeNode)
    {
        Module* module = typeSymbol->GetModule();
        module->ReadAstNode();
        aliasTypeNode = module->GetAstNode(typeSymbol->AstNodeId());
    }
    if (!aliasTypeNode)
    {
        ThrowException("node for symbol '" + typeSymbol->FullName(context) + "' not found", typeSymbol->GetFullSpan(), context);
    }
    specialization->GetScope()->AddParentScope(context->GetSymbolTable()->CurrentScope());
    if (typeSymbol->IsAliasTypeSymbol())
    {
        specialization->SetInstantiated();
        AliasTypeSymbol* aliasTypeSymbol = static_cast<AliasTypeSymbol*>(typeSymbol);
        InstantiationScope instantiationScope(context->GetModule(), aliasTypeSymbol->Parent(context)->GetScope());
        std::vector<std::unique_ptr<BoundTemplateParameterSymbol>> boundTemplateParameters;
        TemplateDeclarationSymbol* templateDeclaration = aliasTypeSymbol->ParentTemplateDeclaration(context);
        if (templateDeclaration)
        {
            Cardinality arity = templateDeclaration->Arity();
            Cardinality argCount = Cardinality(templateArgs.size());
            if (argCount > arity)
            {
                ThrowException("otava.symbols.alias_type_templates: wrong number of template args for instantiating alias type template '" +
                    aliasTypeSymbol->Name() + "'", node->GetFullSpan(), context);
            }
            for (Index i = Index(0); i < Index(arity); ++i)
            {
                TemplateParameterSymbol* templateParameter = templateDeclaration->TemplateParameters(context)[ToUnderlying(i)];
                Symbol* templateArg = nullptr;
                if (i >= Index(argCount))
                {
                    otava::ast::Node* defaultTemplateArgNode = templateParameter->DefaultTemplateArg();
                    if (defaultTemplateArgNode)
                    {
                        context->GetSymbolTable()->BeginScope(&instantiationScope);
                        templateArg = ResolveType(defaultTemplateArgNode, DeclarationFlags::none, context);
                        context->GetSymbolTable()->EndScope();
                    }
                    else
                    {
                        ThrowException("otava.symbols.alias_type_templates: template parameter " + std::to_string(ToUnderlying(i)) +
                            " has no default type argument", node->GetFullSpan(), context);
                    }
                }
                else
                {
                    templateArg = templateArgs[ToUnderlying(i)];
                }
                BoundTemplateParameterSymbol* boundTemplateParameter = new BoundTemplateParameterSymbol(context->GetModule(), 
                    context->GetNextSymbolId(SymbolKind::boundTemplateParameterSymbol), templateParameter->Name());
                boundTemplateParameter->SetTemplateParameterSymbol(templateParameter);
                boundTemplateParameter->SetBoundSymbol(templateArg);
                boundTemplateParameters.push_back(std::unique_ptr<BoundTemplateParameterSymbol>(boundTemplateParameter));
                instantiationScope.Install(boundTemplateParameter, context);
                context->GetSymbolTable()->MapSymbol(boundTemplateParameter);
            }
            context->GetSymbolTable()->BeginScope(&instantiationScope);
            Instantiator instantiator(context, &instantiationScope);
            try
            {
                context->PushSetFlag(ContextFlags::instantiateAliasTypeTemplate);
                context->SetAliasType(specialization);
                aliasTypeNode->Accept(instantiator);
                context->PopFlags();
            }
            catch (const std::exception& ex)
            {
                ThrowException("otava.symbols.alias_type_templates: error instantiating specialization '" +
                    specialization->FullName(context) + "': " + std::string(ex.what()), node->GetFullSpan(), context);
            }
            context->GetSymbolTable()->EndScope();
        }
        else
        {
            ThrowException("otava.symbols.alias_type_templates: template declarator for alias type template '" +
                aliasTypeSymbol->Name() + "' not found", node->GetFullSpan(), context);
        }
    }
    else
    {
        ThrowException("otava.symbols.alias_type_templates: alias type template expected", node->GetFullSpan(), context);
    }
    return specialization;
}

} // namespace otava::symbols
