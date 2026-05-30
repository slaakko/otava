// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.alias_type_symbol;

import otava.symbols.context;
import otava.symbols.modules;
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
    }
    return referredType;
}

otava::intermediate::Type* AliasTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    return DirectType(context)->IrType(emitter, fullSpan, context);
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
/*
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    if (aliasDeclarationNode->IsAliasDeclarationNode())
    {
        otava::ast::AliasDeclarationNode* node = static_cast<otava::ast::AliasDeclarationNode*>(aliasDeclarationNode);
        otava::ast::Node* idNode = node->Identifier();
        AliasTypeSymbol* temporaryAlias = context->GetSymbolTable()->AddAliasType(idNode, aliasDeclarationNode, GenericTypeSymbol::Instance(), context);
        context->AddTemporaryAliasType(temporaryAlias);
    }
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
*/
}

void RemoveTemporaryAliasTypeSymbols(Context* context)
{
/*
    for (AliasTypeSymbol* temporaryAlias : context->TemporaryAliasTypes())
    {
        temporaryAlias->Group()->RemoveAliasType(temporaryAlias);
        Scope* scope = temporaryAlias->Parent()->GetScope()->SymbolScope();
        if (scope->IsContainerScope())
        {
            scope->RemoveSymbol(temporaryAlias);
        }
    }
    context->ClearTemporaryAliasTypes();
*/
}

} // namespace otava::symbols
