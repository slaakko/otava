// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.alias_type_symbol;

import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.templates;
import otava.symbols.type_resolver;
import otava.ast.declaration;
import otava.ast.visitor;

namespace otava::symbols {

AliasTypeSymbol::AliasTypeSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), referredTypeId(zeroSymbolId), referredType(nullptr), group(nullptr)
{
}

AliasTypeSymbol::AliasTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), referredTypeId(zeroSymbolId), referredType(nullptr), group(nullptr)
{
}

TemplateDeclarationSymbol* AliasTypeSymbol::ParentTemplateDeclaration(Context* context) const noexcept
{
    Symbol* parentSymbol = const_cast<AliasTypeSymbol*>(this)->Parent(context);
    if (parentSymbol->IsTemplateDeclarationSymbol())
    {
        return static_cast<TemplateDeclarationSymbol*>(parentSymbol);
    }
    return nullptr;
}

TypeSymbol* AliasTypeSymbol::ReferredType(Context* context) const
{
    if (referredType)
    {
        return referredType;
    }
    if (IsReadOnly() && referredTypeId != zeroSymbolId)
    {
        TypeSymbol* typeSymbol = GetModule()->GetSymbolTable()->GetTypeSymbol(referredTypeId, context);
        referredType = typeSymbol;
        if (!referredType)
        {
            ThrowException("referred type id " + std::to_string(ToUnderlying(referredTypeId)) + " of alias type '" + FullName(context) + "' not found", GetFullSpan(), context);
        }
    }
    return referredType;
}

TypeSymbol* AliasTypeSymbol::DirectType(Context* context)
{
    return ReferredType(context)->DirectType(context);
}

Cardinality AliasTypeSymbol::Arity(Context* context) noexcept
{
    TemplateDeclarationSymbol* templateDeclaration = ParentTemplateDeclaration(context);
    if (templateDeclaration)
    {
        return templateDeclaration->Arity();
    }
    else
    {
        return Cardinality(0);
    }
}

otava::intermediate::Type* AliasTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    return DirectType(context)->IrType(emitter, fullSpan, context);
}

void AliasTypeSymbol::Write(Writer& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(referredType->Id()));
}

void AliasTypeSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    referredTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

class AliasDeclarationProcessor : public otava::ast::DefaultVisitor
{
public:
    AliasDeclarationProcessor(Context* context_);
    void Visit(otava::ast::AliasDeclarationNode& node) override;
    otava::ast::Node* GetIdNode() const noexcept { return idNode; }
    TypeSymbol* GetType() const noexcept { return type; }
private:
    Context* context;
    otava::ast::Node* idNode;
    TypeSymbol* type;
};

AliasDeclarationProcessor::AliasDeclarationProcessor(Context* context_) : context(context_), idNode(nullptr), type(nullptr)
{
}

void AliasDeclarationProcessor::Visit(otava::ast::AliasDeclarationNode& node)
{
    context->PushSetFlag(ContextFlags::processingAliasDeclation);
    idNode = node.Identifier();
    type = ResolveType(node.DefiningTypeId(), DeclarationFlags::none, context);
    if (!type)
    {
        ThrowException("alias declaration processor: type not resolved", node.GetFullSpan(), context);
    }
    while (type->IsAliasTypeSymbol())
    {
        AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(type);
        type = aliasType->ReferredType(context);
    }
    context->PopFlags();
}

void ProcessAliasDeclaration(otava::ast::Node* aliasDeclarationNode, Context* context)
{
    bool instantiate = context->GetFlag(ContextFlags::instantiateAliasTypeTemplate);
    if (instantiate)
    {
        context->PushResetFlag(ContextFlags::instantiateAliasTypeTemplate);
    }
    AliasTypeSymbol* aliasType = context->GetAliasType();
    AliasDeclarationProcessor processor(context);
    aliasDeclarationNode->Accept(processor);
    otava::ast::Node* idNode = processor.GetIdNode();
    TypeSymbol* type = processor.GetType();
    if (instantiate)
    {
        if (!aliasType->ReferredType(context))
        {
            aliasType->SetReferredType(type);
        }
        context->GetSymbolTable()->MapNode(aliasDeclarationNode, aliasType);
    }
    else
    {
        context->GetSymbolTable()->AddAliasType(idNode, aliasDeclarationNode, type, context);
    }
    if (instantiate)
    {
        context->PopFlags();
    }
}

bool AliasTypeLess::operator()(AliasTypeSymbol* left, AliasTypeSymbol* right) const noexcept
{
    return left->Name() < right->Name();
}

void AddTemporaryTypeAlias(otava::ast::Node* aliasDeclarationNode, Context* context)
{
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    if (aliasDeclarationNode->IsAliasDeclarationNode())
    {
        otava::ast::AliasDeclarationNode* node = static_cast<otava::ast::AliasDeclarationNode*>(aliasDeclarationNode);
        otava::ast::Node* idNode = node->Identifier();
        TypeSymbol* type = ResolveType(node->DefiningTypeId(), DeclarationFlags::none, context);
        AliasTypeSymbol* temporaryAlias = context->GetSymbolTable()->AddAliasType(idNode, aliasDeclarationNode, type, context);
        context->AddTemporaryAliasType(temporaryAlias);
    }
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
}

void RemoveTemporaryAliasTypeSymbols(Context* context)
{
    for (AliasTypeSymbol* temporaryAlias : context->TemporaryAliasTypes())
    {
        temporaryAlias->Group()->RemoveAliasType(temporaryAlias);
        Scope* scope = temporaryAlias->Parent(context)->GetScope()->SymbolScope(context);
        if (scope->IsContainerScope())
        {
            scope->RemoveSymbol(temporaryAlias);
        }
    }
    context->ClearTemporaryAliasTypes();
}

} // namespace otava::symbols
