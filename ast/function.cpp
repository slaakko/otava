// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.function;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

FunctionDefinitionNode::FunctionDefinitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    CompoundNode(NodeKind::functionDefinitionNode, span_, fileIndex_), index(0)
{
}

FunctionDefinitionNode::FunctionDefinitionNode(const soul::ast::Span& span_, int fileIndex_,
    Node* attributes_, Node* declSpecifiers_, Node* declarator_, Node* specifiers_, Node* functionBody_) noexcept :
    CompoundNode(NodeKind::functionDefinitionNode, span_, fileIndex_), 
    attributes(attributes_), 
    declSpecifiers(declSpecifiers_), 
    declarator(declarator_), 
    specifiers(specifiers_), 
    functionBody(functionBody_), 
    index(0)
{
}

Node* FunctionDefinitionNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    Node* clonedDeclSpecifiers = nullptr;
    if (declSpecifiers)
    {
        clonedDeclSpecifiers = declSpecifiers->Clone();
    }
    Node* clonedSpecifiers = nullptr;
    if (specifiers)
    {
        clonedSpecifiers = specifiers->Clone();
    }
    FunctionDefinitionNode* clone = new FunctionDefinitionNode(GetSpan(), FileIndex(), clonedAttributes, clonedDeclSpecifiers, declarator->Clone(), clonedSpecifiers, 
        functionBody->Clone());
    clone->SetId(Id());
    return clone;
}

void FunctionDefinitionNode::ResetFunctionBody(Node* functionBody_) noexcept
{
    functionBody.reset(functionBody_);
}

void FunctionDefinitionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionDefinitionNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(attributes.get());
    writer.Write(declSpecifiers.get());
    writer.Write(declarator.get());
    writer.Write(specifiers.get());
    writer.Write(functionBody.get());
}

void FunctionDefinitionNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    attributes.reset(reader.ReadNode());
    declSpecifiers.reset(reader.ReadNode());
    declarator.reset(reader.ReadNode());
    specifiers.reset(reader.ReadNode());
    functionBody.reset(reader.ReadNode());
}

FunctionBodyNode::FunctionBodyNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::functionBodyNode, span_, fileIndex_, nullptr)
{
}

FunctionBodyNode::FunctionBodyNode(const soul::ast::Span& span_, int fileIndex_, Node* compoundStatement_) noexcept :
    UnaryNode(NodeKind::functionBodyNode, span_, fileIndex_, compoundStatement_)
{
}

Node* FunctionBodyNode::Clone() const
{
    FunctionBodyNode* clone = new FunctionBodyNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void FunctionBodyNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DefaultNode::DefaultNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::defaultNode, span_, fileIndex_)
{
}

Node* DefaultNode::Clone() const
{
    DefaultNode* clone = new DefaultNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DefaultNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DeleteNode::DeleteNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::deleteNode, span_, fileIndex_)
{
}

Node* DeleteNode::Clone() const
{
    DeleteNode* clone = new DeleteNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DeleteNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DefaultedOrDeletedFunctionNode::DefaultedOrDeletedFunctionNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    CompoundNode(NodeKind::defaultedOrDeletedFunctionNode, span_, fileIndex_)
{
}

DefaultedOrDeletedFunctionNode::DefaultedOrDeletedFunctionNode(const soul::ast::Span& span_, int fileIndex_, Node* assign_, Node* defaultOrDelete_, 
    Node* semicolon_) noexcept :
    CompoundNode(NodeKind::defaultedOrDeletedFunctionNode, span_, fileIndex_), assign(assign_), defaultOrDelete(defaultOrDelete_), semicolon(semicolon_)
{
}

Node* DefaultedOrDeletedFunctionNode::Clone() const
{
    DefaultedOrDeletedFunctionNode* clone = new DefaultedOrDeletedFunctionNode(GetSpan(), FileIndex(), assign->Clone(), defaultOrDelete->Clone(), semicolon->Clone());
    clone->SetId(Id());
    return clone;
}

void DefaultedOrDeletedFunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DefaultedOrDeletedFunctionNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(assign.get());
    writer.Write(defaultOrDelete.get());
    writer.Write(semicolon.get());
}

void DefaultedOrDeletedFunctionNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    assign.reset(reader.ReadNode());
    defaultOrDelete.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

NoDeclSpecFunctionDeclarationNode::NoDeclSpecFunctionDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    CompoundNode(NodeKind::noDeclSpecFunctionDeclarationNode, span_, fileIndex_)
{
}

NoDeclSpecFunctionDeclarationNode::NoDeclSpecFunctionDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* declarator_,
    Node* semicolon_) noexcept :
    CompoundNode(NodeKind::noDeclSpecFunctionDeclarationNode, span_, fileIndex_), attributes(attributes_), declarator(declarator_), semicolon(semicolon_)
{
}

Node* NoDeclSpecFunctionDeclarationNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    NoDeclSpecFunctionDeclarationNode* clone = new NoDeclSpecFunctionDeclarationNode(GetSpan(), FileIndex(), clonedAttributes, declarator->Clone(), semicolon->Clone());
    clone->SetId(Id());
    return clone;
}

void NoDeclSpecFunctionDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NoDeclSpecFunctionDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(declarator.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
}

void NoDeclSpecFunctionDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    declarator.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

OperatorNode::OperatorNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::operatorNode, span_, fileIndex_)
{
}

Node* OperatorNode::Clone() const 
{
    OperatorNode* clone = new OperatorNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void OperatorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NewArrayOpNode::NewArrayOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::newArrayOpNode, span_, fileIndex_)
{
}

NewArrayOpNode::NewArrayOpNode(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    CompoundNode(NodeKind::newArrayOpNode, span_, fileIndex_), lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* NewArrayOpNode::Clone() const
{
    NewArrayOpNode* clone = new NewArrayOpNode(GetSpan(), FileIndex(), lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void NewArrayOpNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NewArrayOpNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(lbSpan);
    writer.Write(rbSpan);

}

void NewArrayOpNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
}

NewOpNode::NewOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::newOpNode, span_, fileIndex_)
{
}

Node* NewOpNode::Clone() const
{
    NewOpNode* clone = new NewOpNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void NewOpNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DeleteArrayOpNode::DeleteArrayOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::deleteArrayOpNode, span_, fileIndex_)
{
}

DeleteArrayOpNode::DeleteArrayOpNode(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    CompoundNode(NodeKind::deleteArrayOpNode, span_, fileIndex_)
{
}

Node* DeleteArrayOpNode::Clone() const
{
    DeleteArrayOpNode* clone = new DeleteArrayOpNode(GetSpan(), FileIndex(), lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void DeleteArrayOpNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DeleteArrayOpNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(lbSpan);
    writer.Write(rbSpan);
}

void DeleteArrayOpNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
}

DeleteOpNode::DeleteOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::deleteOpNode, span_, fileIndex_)
{
}

Node* DeleteOpNode::Clone() const
{
    DeleteOpNode* clone = new DeleteOpNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void DeleteOpNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CoAwaitOpNode::CoAwaitOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::coAwaitOpNode, span_, fileIndex_)
{
}

Node* CoAwaitOpNode::Clone() const
{
    CoAwaitOpNode* clone = new CoAwaitOpNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void CoAwaitOpNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InvokeOpNode::InvokeOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::invokeOpNode, span_, fileIndex_)
{
}

Node* InvokeOpNode::Clone() const
{
    InvokeOpNode* clone = new InvokeOpNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void InvokeOpNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SubscriptOpNode::SubscriptOpNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::subscriptOpNode, span_, fileIndex_)
{
}

Node* SubscriptOpNode::Clone() const
{
    SubscriptOpNode* clone = new SubscriptOpNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void SubscriptOpNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OperatorFunctionIdNode::OperatorFunctionIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    BinaryNode(NodeKind::operatorFnIdNode, span_, fileIndex_, nullptr, nullptr)
{
}

OperatorFunctionIdNode::OperatorFunctionIdNode(const soul::ast::Span& span_, int fileIndex_, Node* operatorKwNode_, Node* op_) noexcept :
    BinaryNode(NodeKind::operatorFnIdNode, span_, fileIndex_, operatorKwNode_, op_)
{
}

Node* OperatorFunctionIdNode::Clone() const
{
    OperatorFunctionIdNode* clone = new OperatorFunctionIdNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void OperatorFunctionIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string OperatorFunctionIdNode::Str() const
{
    std::string str = Left()->Str();
    str.append(1, ' ').append(Right()->Str());
    return str;
}

ConversionFunctionIdNode::ConversionFunctionIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::conversionFnIdNode, span_, fileIndex_, nullptr, nullptr)
{
}

ConversionFunctionIdNode::ConversionFunctionIdNode(const soul::ast::Span& span_, int fileIndex_, Node* operatorKwNode_, Node* conversionTypeId_) noexcept :
    BinaryNode(NodeKind::conversionFnIdNode, span_, fileIndex_, operatorKwNode_, conversionTypeId_)
{
}

Node* ConversionFunctionIdNode::Clone() const
{
    ConversionFunctionIdNode* clone = new ConversionFunctionIdNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void ConversionFunctionIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ConversionFunctionIdNode::Str() const
{
    std::string str = Left()->Str();
    str.append(1, ' ').append(Right()->Str()).append("()");
    return str;
}

ConversionTypeIdNode::ConversionTypeIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::conversionTypeIdNode, span_, fileIndex_)
{
}

ConversionTypeIdNode::ConversionTypeIdNode(const soul::ast::Span& span_, int fileIndex_, Node* typeSpecifierSeq_, Node* conversionDeclarator_) noexcept :
    CompoundNode(NodeKind::conversionTypeIdNode, span_, fileIndex_), typeSpecifierSeq(typeSpecifierSeq_), conversionDeclarator(conversionDeclarator_)
{
}

Node* ConversionTypeIdNode::Clone() const
{
    Node* clonedConversionDeclarator = nullptr;
    if (conversionDeclarator)
    {
        clonedConversionDeclarator = conversionDeclarator->Clone();
    }
    ConversionTypeIdNode* clone = new ConversionTypeIdNode(GetSpan(), FileIndex(), typeSpecifierSeq->Clone(), clonedConversionDeclarator);
    clone->SetId(Id());
    return clone;
}

void ConversionTypeIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConversionTypeIdNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(typeSpecifierSeq.get());
    writer.Write(conversionDeclarator.get());
}

void ConversionTypeIdNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    typeSpecifierSeq.reset(reader.ReadNode());
    conversionDeclarator.reset(reader.ReadNode());
}

ConversionDeclaratorNode::ConversionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::conversionDeclaratorNode, span_, fileIndex_)
{
}

ConversionDeclaratorNode::ConversionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* ptrOp_, Node* conversionDeclarator_) noexcept :
    CompoundNode(NodeKind::conversionDeclaratorNode, span_, fileIndex_), ptrOp(ptrOp_), conversionDeclarator(conversionDeclarator_)
{
}

Node* ConversionDeclaratorNode::Clone() const
{
    Node* clonedConversionDeclarator = nullptr;
    if (conversionDeclarator)
    {
        clonedConversionDeclarator = conversionDeclarator->Clone();
    }
    ConversionDeclaratorNode* clone = new ConversionDeclaratorNode(GetSpan(), FileIndex(), ptrOp->Clone(), clonedConversionDeclarator);
    clone->SetId(Id());
    return clone;
}

void ConversionDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConversionDeclaratorNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(ptrOp.get());
    writer.Write(conversionDeclarator.get());
}

void ConversionDeclaratorNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    ptrOp.reset(reader.ReadNode());
    conversionDeclarator.reset(reader.ReadNode());
}

DestructorIdNode::DestructorIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::destructorIdNode, span_, fileIndex_, nullptr)
{
}

DestructorIdNode::DestructorIdNode(const soul::ast::Span& span_, int fileIndex_, Node* typeName_) noexcept : 
    UnaryNode(NodeKind::destructorIdNode, span_, fileIndex_, typeName_)
{
}

Node* DestructorIdNode::Clone() const
{
    DestructorIdNode* clone = new DestructorIdNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void DestructorIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ParameterNode::ParameterNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::parameterNode, span_, fileIndex_)
{
}

Node* ParameterNode::Clone() const
{
    ParameterNode* clone = new ParameterNode(GetSpan(), FileIndex());
    if (attributes)
    {
        clone->SetAttributes(attributes->Clone());
    }
    clone->SetDeclSpecifiers(declSpecifiers->Clone());
    if (declarator)
    {
        clone->SetDeclarator(declarator->Clone());
    }
    if (assign)
    {
        clone->SetAssign(assign->Clone());
    }
    if (initializer)
    {
        clone->SetInitializer(initializer->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void ParameterNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParameterNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(attributes.get());
    writer.Write(declSpecifiers.get());
    writer.Write(declarator.get());
    writer.Write(assign.get());
    writer.Write(initializer.get());
}

void ParameterNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    attributes.reset(reader.ReadNode());
    declSpecifiers.reset(reader.ReadNode());
    declarator.reset(reader.ReadNode());
    assign.reset(reader.ReadNode());
    initializer.reset(reader.ReadNode());
}

void ParameterNode::SetAttributes(Node* attributes_) noexcept
{
    attributes.reset(attributes_);
}

void ParameterNode::SetDeclSpecifiers(Node* declSpecifiers_) noexcept
{
    declSpecifiers.reset(declSpecifiers_);
}

void ParameterNode::SetDeclarator(Node* declarator_) noexcept
{
    declarator.reset(declarator_);
}

void ParameterNode::SetAssign(Node* assign_) noexcept
{
    assign.reset(assign_);
}

void ParameterNode::SetInitializer(Node* initializer_) noexcept
{
    initializer.reset(initializer_);
}

std::string ParameterNode::Str() const
{
    std::string str = declSpecifiers->Str();
    if (declarator)
    {
        str.append(1, ' ').append(declarator->Str());
    }
    if (assign)
    {
        str.append(1, ' ').append(assign->Str()).append(1, ' ');
    }
    if (initializer)
    {
        str.append(initializer->Str());
    }
    return str;
}

ParameterListNode::ParameterListNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::parameterListNode, span_, fileIndex_)
{
}

Node* ParameterListNode::Clone() const
{
    ParameterListNode* clone = new ParameterListNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetLParenSpan(lpSpan);
    clone->SetRParenSpan(rpSpan);
    clone->SetId(Id());
    return clone;
}

void ParameterListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParameterListNode::Write(Writer& writer)
{
    ListNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void ParameterListNode::Read(Reader& reader)
{
    ListNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

NoexceptNode::NoexceptNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::noexceptNode, span_, fileIndex_)
{
}

Node* NoexceptNode::Clone() const
{
    NoexceptNode* clone = new NoexceptNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void NoexceptNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FunctionTryBlockNode::FunctionTryBlockNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::functionTryBlock, span_, fileIndex_)
{
}

FunctionTryBlockNode::FunctionTryBlockNode(const soul::ast::Span& span_, int fileIndex_, Node* ctorInitializer_, Node* tryBlock_, Node* handlers_) noexcept :
    CompoundNode(NodeKind::functionTryBlock, span_, fileIndex_), ctorInitializer(ctorInitializer_), tryBlock(tryBlock_), handlers(handlers_)
{
}

Node* FunctionTryBlockNode::Clone() const
{
    Node* clonedCtorInitializer = nullptr;
    if (ctorInitializer)
    {
        clonedCtorInitializer = ctorInitializer->Clone();
    }
    FunctionTryBlockNode* clone = new FunctionTryBlockNode(GetSpan(), FileIndex(), clonedCtorInitializer, tryBlock->Clone(), handlers->Clone());
    clone->SetId(Id());
    return clone;
}

void FunctionTryBlockNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionTryBlockNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(ctorInitializer.get());
    writer.Write(tryBlock.get());
    writer.Write(handlers.get());
}

void FunctionTryBlockNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    ctorInitializer.reset(reader.ReadNode());
    tryBlock.reset(reader.ReadNode());
    handlers.reset(reader.ReadNode());
}

} // namespace otava::ast
