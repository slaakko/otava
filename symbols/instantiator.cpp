// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.instantiator;

import otava.symbols.alias_type_symbol;
import otava.symbols.classes;
import otava.symbols.context;
import otava.symbols.declaration;
import otava.symbols.using_declaration;
import otava.symbols.using_directive;
import otava.symbols.type_symbol;
import otava.ast.classes;
import otava.ast.declaration;
import otava.ast.function;
import otava.ast.statement;

namespace otava::symbols {

Instantiator::Instantiator(Context* context_, InstantiationScope* instantiationScope_) :
    context(context_), innerClass(false), index(0), instantiationScope(instantiationScope_),
    functionNode(nullptr), specialization(nullptr), scopeCount(0)
{
}

void Instantiator::SetFunctionNode(otava::ast::Node* functionNode_)
{
    functionNode = functionNode_;
}

void Instantiator::Visit(otava::ast::ClassSpecifierNode& node)
{
    if (innerClass)
    {
        BeginClass(&node, context);
        VisitSequenceContent(node);
        EndClass(&node, context);
    }
    else
    {
        std::string name;
        ClassKind kind;
        TypeSymbol* specialization = nullptr;
        GetClassAttributes(&node, name, kind, specialization, context);
        baseClasses = ResolveBaseClasses(&node, context);
        switch (kind)
        {
        case ClassKind::class_:
        {
            context->GetSymbolTable()->PushAccess(Access::private_);
            break;
        }
        case ClassKind::struct_:
        case ClassKind::union_:
        {
            context->GetSymbolTable()->PushAccess(Access::public_);
            break;
        }
        }
        innerClass = true;
        VisitSequenceContent(node);
        innerClass = false;
        context->GetSymbolTable()->PopAccess();
    }
}

void Instantiator::Visit(otava::ast::BeginAccessGroupNode& node)
{
    switch (node.Child()->Kind())
    {
    case otava::ast::NodeKind::publicNode:
    {
        context->GetSymbolTable()->SetCurrentAccess(Access::public_);
        break;
    }
    case otava::ast::NodeKind::protectedNode:
    {
        context->GetSymbolTable()->SetCurrentAccess(Access::protected_);
        break;
    }
    case otava::ast::NodeKind::privateNode:
    {
        context->GetSymbolTable()->SetCurrentAccess(Access::private_);
        break;
    }
    }
}

void Instantiator::Visit(otava::ast::AliasDeclarationNode& node)
{
    ProcessAliasDeclaration(&node, context);
}

void Instantiator::Visit(otava::ast::MemberDeclarationNode& node)
{
    ProcessMemberDeclaration(&node, functionNode, context);
    if (functionNode)
    {
        specialization = context->GetSpecialization(functionNode);
    }
}

void Instantiator::Visit(otava::ast::SimpleDeclarationNode& node)
{
    ProcessSimpleDeclaration(&node, functionNode, context);
    if (functionNode)
    {
        specialization = context->GetSpecialization(functionNode);
    }
}

void Instantiator::Visit(otava::ast::UsingDeclarationNode& node)
{
    AddUsingDeclaration(&node, context);
}

void Instantiator::Visit(otava::ast::UsingDirectiveNode& node)
{
    AddUsingDirective(&node, context);
}

void Instantiator::Visit(otava::ast::FunctionDefinitionNode& node)
{
    if (context->GetFlag(ContextFlags::skipFunctionDefinitions)) return;
    bool get = false;
    scopeCount = BeginFunctionDefinition(node.DeclSpecifiers(), node.Declarator(), functionNode, node.Specifiers(), get, context);
    if (context->GetFlag(ContextFlags::instantiateFunctionTemplate |
        ContextFlags::instantiateMemFnOfClassTemplate |
        ContextFlags::instantiateInlineFunction))
    {
        if (functionNode)
        {
            specialization = context->GetSpecialization(functionNode);
            if (specialization)
            {
                instantiationScope->PushParentScope(specialization->GetScope());
            }
        }
    }
    if (!get)
    {
        node.FunctionBody()->Accept(*this);
        otava::symbols::EndFunctionDefinition(&node, scopeCount, context);
    }
}

void Instantiator::Visit(otava::ast::NoDeclSpecFunctionDeclarationNode& node)
{
    ProcessFunctionDeclaration(&node, context);
}

void Instantiator::Visit(otava::ast::TemplateDeclarationNode& node)
{
    // member templates not supported (they are skipped)
}

void Instantiator::Visit(otava::ast::CompoundStatementNode& node)
{
    BlockSymbol* block = BeginBlock(node.GetFullSpan(), node.BlockId(), context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PushParentBlockId(node.BlockId());
    }
    context->GetSymbolTable()->MapNode(&node, block);
    VisitSequence(node);
    EndBlock(context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PopParentBlockId();
    }
}

void Instantiator::Visit(otava::ast::SequenceStatementNode& node)
{
    node.FirstStmt()->Accept(*this);
    node.SecondStmt()->Accept(*this);
}

void Instantiator::Visit(otava::ast::IfStatementNode& node)
{
    BlockSymbol* block = BeginBlock(node.GetFullSpan(), node.BlockId(), context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PushParentBlockId(node.BlockId());
    }
    context->GetSymbolTable()->MapNode(&node, block);
    node.ThenStatement()->Accept(*this);
    if (node.ElseStatement())
    {
        node.ElseStatement()->Accept(*this);
    }
    EndBlock(context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PopParentBlockId();
    }
}

void Instantiator::Visit(otava::ast::SwitchStatementNode& node)
{
    BlockSymbol* block = BeginBlock(node.GetFullSpan(), node.BlockId(), context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PushParentBlockId(node.BlockId());
    }
    context->GetSymbolTable()->MapNode(&node, block);
    node.Statement()->Accept(*this);
    EndBlock(context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PopParentBlockId();
    }
}

void Instantiator::Visit(otava::ast::WhileStatementNode& node)
{
    BlockSymbol* block = BeginBlock(node.GetFullSpan(), node.BlockId(), context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PushParentBlockId(node.BlockId());
    }
    context->GetSymbolTable()->MapNode(&node, block);
    node.Statement()->Accept(*this);
    EndBlock(context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PopParentBlockId();
    }
}

void Instantiator::Visit(otava::ast::DoStatementNode& node)
{
    node.Statement()->Accept(*this);
}

void Instantiator::Visit(otava::ast::ForStatementNode& node)
{
    BlockSymbol* block = BeginBlock(node.GetFullSpan(), node.BlockId(), context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PushParentBlockId(node.BlockId());
    }
    context->GetSymbolTable()->MapNode(&node, block);
    if (node.InitStatement())
    {
        node.InitStatement()->Accept(*this);
    }
    node.Statement()->Accept(*this);
    EndBlock(context);
    if (context->GetFlag(ContextFlags::setParentBlockIds))
    {
        context->PopParentBlockId();
    }
}

void Instantiator::Visit(otava::ast::ExpressionStatementNode& node)
{
}

void Instantiator::Visit(otava::ast::ReturnStatementNode& node)
{
}

void Instantiator::Visit(otava::ast::ExceptionDeclarationNode& node)
{
    ProcessExceptionDeclaration(&node, context);
}

} // namespace otava::symbols
