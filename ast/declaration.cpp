// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.declaration;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

DeclarationSequenceNode::DeclarationSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept : SequenceNode(NodeKind::declarationSequenceNode, span_, fileIndex_)
{
}

Node* DeclarationSequenceNode::Clone() const
{
    DeclarationSequenceNode* clone = new DeclarationSequenceNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void DeclarationSequenceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SimpleDeclarationNode::SimpleDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::simpleDeclarationNode, span_, fileIndex_)
{
}

SimpleDeclarationNode::SimpleDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* declarationSpecifiers_, Node* initDeclaratorList_,
    Node* attributes_, Node* semicolon_) noexcept :
    CompoundNode(NodeKind::simpleDeclarationNode, span_, fileIndex_), declarationSpecifiers(declarationSpecifiers_), initDeclaratorList(initDeclaratorList_), 
    attributes(attributes_), semicolon(semicolon_)
{
}

Node* SimpleDeclarationNode::Clone() const
{
    Node* clonedDeclSpecifiers = nullptr;
    if (declarationSpecifiers)
    {
        clonedDeclSpecifiers = declarationSpecifiers->Clone();
    }
    Node* clonedInitDeclaratorList = nullptr;
    if (initDeclaratorList)
    {
        clonedInitDeclaratorList = initDeclaratorList->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    Node* clonedSemicolon = nullptr;
    if (semicolon)
    {
        clonedSemicolon = semicolon->Clone();
    }
    SimpleDeclarationNode* clone = new SimpleDeclarationNode(GetSpan(), FileIndex(), clonedDeclSpecifiers, clonedInitDeclaratorList, clonedAttributes, clonedSemicolon);
    clone->SetId(Id());
    return clone;
}

void SimpleDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SimpleDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(declarationSpecifiers.get());
    writer.Write(initDeclaratorList.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
}

void SimpleDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    declarationSpecifiers.reset(reader.ReadNode());
    initDeclaratorList.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

std::string SimpleDeclarationNode::Str() const
{
    std::string str = declarationSpecifiers->Str();
    if (initDeclaratorList->IsInitDeclaratorListNode())
    {
        InitDeclaratorListNode* il = static_cast<InitDeclaratorListNode*>(initDeclaratorList.get());
        if (il->Count() > 0)
        {
            Node* n = il->Nodes()[0];
            if (n->IsInitDeclaratorNode())
            {
                InitDeclaratorNode* in = static_cast<InitDeclaratorNode*>(n);
                if (!in->Left()->IsPtrDeclaratorNode())
                {
                    str.append(1, ' ');
                }
            }
        }
    }
    str.append(initDeclaratorList->Str());
    str.append(1, ';');
    return str;
}

AsmDeclarationNode::AsmDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::asmDeclarationNode, span_, fileIndex_)
{
}

AsmDeclarationNode::AsmDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* asm__, Node* asmText_, Node* semicolon_, Node* attributes_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::asmDeclarationNode, span_, fileIndex_), asm_(asm__), asmText(asmText_), semicolon(semicolon_), attributes(attributes_), 
    lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* AsmDeclarationNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    AsmDeclarationNode* clone = new AsmDeclarationNode(GetSpan(), FileIndex(), asm_->Clone(), asmText->Clone(), semicolon->Clone(), clonedAttributes, lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void AsmDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AsmDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(asm_.get());
    writer.Write(asmText.get());
    writer.Write(semicolon.get());
    writer.Write(attributes.get());
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void AsmDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    asm_.reset(reader.ReadNode());
    asmText.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

AsmNode::AsmNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::asmNode, span_, fileIndex_)
{
}

Node* AsmNode::Clone() const
{
    AsmNode* clone = new AsmNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void AsmNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LinkageSpecificationNode::LinkageSpecificationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::linkageSpecificationNode, span_, fileIndex_)
{
}

LinkageSpecificationNode::LinkageSpecificationNode(const soul::ast::Span& span_, int fileIndex_, Node* extrn_, Node* linkage_, Node* declarations_,
    const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    CompoundNode(NodeKind::linkageSpecificationNode, span_, fileIndex_), extrn(extrn_), linkage(linkage_), declarations(declarations_), lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* LinkageSpecificationNode::Clone() const
{
    Node* clonedDeclarations = nullptr;
    if (declarations)
    {
        clonedDeclarations = declarations->Clone();
    }
    LinkageSpecificationNode* clone = new LinkageSpecificationNode(GetSpan(), FileIndex(), extrn->Clone(), linkage->Clone(), clonedDeclarations, lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void LinkageSpecificationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LinkageSpecificationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(extrn.get());
    writer.Write(linkage.get());
    writer.Write(declarations.get());
    writer.Write(lbSpan);
    writer.Write(rbSpan);
}

void LinkageSpecificationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    extrn.reset(reader.ReadNode());
    linkage.reset(reader.ReadNode());
    declarations.reset(reader.ReadNode());
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
}

NamespaceDefinitionNode::NamespaceDefinitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::namespaceDefinitionNode, span_, fileIndex_)
{
}

NamespaceDefinitionNode::NamespaceDefinitionNode(const soul::ast::Span& span_, int fileIndex_, Node* nskw_, Node* nsName_, Node* nsBody_, Node* inln_,
    Node* attributes_, const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    CompoundNode(NodeKind::namespaceDefinitionNode, span_, fileIndex_), nskw(nskw_), nsName(nsName_), nsBody(nsBody_), inln(inln_), attributes(attributes_), 
    lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* NamespaceDefinitionNode::Clone() const
{
    Node* clonedNsName = nullptr;
    if (nsName)
    {
        clonedNsName = nsName->Clone();
    }
    Node* clonedInline = nullptr;
    if (inln)
    {
        clonedNsName = inln->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    NamespaceDefinitionNode* clone = new NamespaceDefinitionNode(GetSpan(), FileIndex(),nskw->Clone(), clonedNsName, nsBody->Clone(), clonedInline, clonedAttributes, 
        lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void NamespaceDefinitionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NamespaceDefinitionNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(nskw.get());
    writer.Write(nsName.get());
    writer.Write(nsBody.get());
    writer.Write(inln.get());
    writer.Write(attributes.get());
    writer.Write(lbSpan);
    writer.Write(rbSpan);
}

void NamespaceDefinitionNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    nskw.reset(reader.ReadNode());
    nsName.reset(reader.ReadNode());
    nsBody.reset(reader.ReadNode());
    inln.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
}

NamespaceBodyNode::NamespaceBodyNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::namespaceBodyNode, span_, fileIndex_)
{
}

NamespaceBodyNode::NamespaceBodyNode(const soul::ast::Span& span_, int fileIndex_, Node* declarations_) noexcept :
    CompoundNode(NodeKind::namespaceBodyNode, span_, fileIndex_), declarations(declarations_)
{
}

Node* NamespaceBodyNode::Clone() const
{
    Node* clonedDeclarations = nullptr;
    if (declarations)
    {
        clonedDeclarations = declarations->Clone();
    }
    NamespaceBodyNode* clone = new NamespaceBodyNode(GetSpan(), FileIndex(), clonedDeclarations);
    clone->SetId(Id());
    return clone;
}

void NamespaceBodyNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NamespaceBodyNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(declarations.get());
}

void NamespaceBodyNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    declarations.reset(reader.ReadNode());
}

NamespaceAliasDefinitionNode::NamespaceAliasDefinitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    CompoundNode(NodeKind::namespaceAliasDefinitionNode, span_, fileIndex_)
{
}

NamespaceAliasDefinitionNode::NamespaceAliasDefinitionNode(const soul::ast::Span& span_, int fileIndex_, Node* id_, Node* assign_, Node* qns_, Node* semicolon_) noexcept :
    CompoundNode(NodeKind::namespaceAliasDefinitionNode, span_, fileIndex_), id(id_), assign(assign_), qns(qns_), semicolon(semicolon_)
{
}

Node* NamespaceAliasDefinitionNode::Clone() const
{
    NamespaceAliasDefinitionNode* clone = new NamespaceAliasDefinitionNode(GetSpan(), FileIndex(), id->Clone(), assign->Clone(), qns->Clone(), semicolon->Clone());
    return clone;
}

void NamespaceAliasDefinitionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NamespaceAliasDefinitionNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(id.get());
    writer.Write(assign.get());
    writer.Write(qns.get());
    writer.Write(semicolon.get());
}

void NamespaceAliasDefinitionNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    id.reset(reader.ReadNode());
    assign.reset(reader.ReadNode());
    qns.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

UsingDeclarationNode::UsingDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::usingDeclarationNode, span_, fileIndex_)
{
}

UsingDeclarationNode::UsingDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* usng_, Node* declarators_, Node* semicolon_) noexcept :
    CompoundNode(NodeKind::usingDeclarationNode, span_, fileIndex_), usng(usng_), declarators(declarators_), semicolon(semicolon_)
{
}

Node* UsingDeclarationNode::Clone() const
{
    UsingDeclarationNode* clone = new UsingDeclarationNode(GetSpan(), FileIndex(), usng->Clone(), declarators->Clone(), semicolon->Clone());
    clone->SetId(Id());
    return clone;
}

void UsingDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UsingDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(usng.get());
    writer.Write(declarators.get());
    writer.Write(semicolon.get());
}

void UsingDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    usng.reset(reader.ReadNode());
    declarators.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

UsingNode::UsingNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::usingNode, span_, fileIndex_)
{
}

Node* UsingNode::Clone() const
{
    UsingNode* clone = new UsingNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void UsingNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UsingDeclaratorListNode::UsingDeclaratorListNode(const soul::ast::Span& span_, int fileIndex_) noexcept : ListNode(NodeKind::usingDeclaratorListNode, span_, fileIndex_)
{
}

Node* UsingDeclaratorListNode::Clone() const
{
    UsingDeclaratorListNode* clone = new UsingDeclaratorListNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void UsingDeclaratorListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UsingEnumDeclarationNode::UsingEnumDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::usingEnumDeclarationNode, span_, fileIndex_)
{
}

UsingEnumDeclarationNode::UsingEnumDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* usng_, Node* ees_, Node* semicolon_) noexcept :
    CompoundNode(NodeKind::usingEnumDeclarationNode, span_, fileIndex_), usng(usng_), ees(ees_), semicolon(semicolon_)
{
}

Node* UsingEnumDeclarationNode::Clone() const
{
    UsingEnumDeclarationNode* clone = new UsingEnumDeclarationNode(GetSpan(), FileIndex(), usng->Clone(), ees->Clone(), semicolon->Clone());
    clone->SetId(Id());
    return clone;
}

void UsingEnumDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UsingEnumDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(usng.get());
    writer.Write(ees.get());
    writer.Write(semicolon.get());
}

void UsingEnumDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    usng.reset(reader.ReadNode());
    ees.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

UsingDirectiveNode::UsingDirectiveNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::usingDirectiveNode, span_, fileIndex_)
{
}

UsingDirectiveNode::UsingDirectiveNode(const soul::ast::Span& span_, int fileIndex_, Node* usng_, Node* nskw_, Node* id_, Node* semicolon_, Node* attributes_) noexcept :
    CompoundNode(NodeKind::usingDirectiveNode, span_, fileIndex_), usng(usng_), nskw(nskw_), id(id_), semicolon(semicolon_), attributes(attributes_)
{
}

Node* UsingDirectiveNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    UsingDirectiveNode* clone = new UsingDirectiveNode(GetSpan(), FileIndex(), usng->Clone(), nskw->Clone(), id->Clone(), semicolon->Clone(), clonedAttributes);
    clone->SetId(Id());
    return clone;
}

void UsingDirectiveNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UsingDirectiveNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(usng.get());
    writer.Write(nskw.get());
    writer.Write(id.get());
    writer.Write(semicolon.get());
    writer.Write(attributes.get());
}

void UsingDirectiveNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    usng.reset(reader.ReadNode());
    nskw.reset(reader.ReadNode());
    id.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
}

NamespaceNode::NamespaceNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::namespaceNode, span_, fileIndex_)
{
}

Node* NamespaceNode::Clone() const
{
    NamespaceNode* clone = new NamespaceNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void NamespaceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StaticAssertDeclarationNode::StaticAssertDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    CompoundNode(NodeKind::staticAssertDeclarationNode, span_, fileIndex_)
{
}

StaticAssertDeclarationNode::StaticAssertDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* staticAssert_, Node* constantExpr_, Node* comma_,
    Node* stringLiteral_, Node* semicolon_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::staticAssertDeclarationNode, span_, fileIndex_), staticAssert(staticAssert_), constantExpr(constantExpr_), comma(comma_), 
    stringLiteral(stringLiteral_), semicolon(semicolon_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* StaticAssertDeclarationNode::Clone() const
{
    Node* clonedComma = nullptr;
    if (comma)
    {
        clonedComma = comma->Clone();
    }
    Node* clonedStringLiteral = nullptr;
    if (stringLiteral)
    {
        clonedStringLiteral = stringLiteral->Clone();
    }
    StaticAssertDeclarationNode* clone = new StaticAssertDeclarationNode(GetSpan(), FileIndex(), staticAssert->Clone(), constantExpr->Clone(), 
        clonedComma, clonedStringLiteral, semicolon->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void StaticAssertDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StaticAssertDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(staticAssert.get());
    writer.Write(constantExpr.get());
    writer.Write(comma.get());
    writer.Write(stringLiteral.get());
    writer.Write(semicolon.get());
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void StaticAssertDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    staticAssert.reset(reader.ReadNode());
    constantExpr.reset(reader.ReadNode());
    comma.reset(reader.ReadNode());
    stringLiteral.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

StaticAssertNode::StaticAssertNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::staticAssertNode, span_, fileIndex_)
{
}

Node* StaticAssertNode::Clone() const
{
    StaticAssertNode* clone = new StaticAssertNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void StaticAssertNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AliasDeclarationNode::AliasDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::aliasDeclarationNode, span_, fileIndex_)
{
}

AliasDeclarationNode::AliasDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* usng_, Node* identifier_, Node* assign_, Node* definingTypeId_,
    Node* attributes_, Node* semicolon_) noexcept :
    CompoundNode(NodeKind::aliasDeclarationNode, span_, fileIndex_), usng(usng_), identifier(identifier_), assign(assign_), definingTypeId(definingTypeId_), 
    attributes(attributes_), semicolon(semicolon_)
{
}

Node* AliasDeclarationNode::Clone() const
{
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    AliasDeclarationNode* clone = new AliasDeclarationNode(GetSpan(), FileIndex(), usng->Clone(), identifier->Clone(), assign->Clone(), 
        definingTypeId->Clone(), clonedAttributes, semicolon->Clone());
    clone->SetId(Id());
    return clone;
}

void AliasDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AliasDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(usng.get());
    writer.Write(identifier.get());
    writer.Write(assign.get());
    writer.Write(definingTypeId.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
}

void AliasDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    usng.reset(reader.ReadNode());
    identifier.reset(reader.ReadNode());
    assign.reset(reader.ReadNode());
    definingTypeId.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

std::string AliasDeclarationNode::Str() const
{
    std::string str = usng->Str();
    str.append(1, ' ').append(identifier->Str());
    str.append(" = ").append(definingTypeId->Str()).append(1, ';');
    return str;
}

EmptyDeclarationNode::EmptyDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::emptyDeclarationNode, span_, fileIndex_)
{
}

Node* EmptyDeclarationNode::Clone() const
{
    EmptyDeclarationNode* clone = new EmptyDeclarationNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void EmptyDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AttributeDeclarationNode::AttributeDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::attributeDeclarationNode, span_, fileIndex_, nullptr, nullptr)
{
}

AttributeDeclarationNode::AttributeDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* attributes_, Node* semicolon_) noexcept :
    BinaryNode(NodeKind::attributeDeclarationNode, span_, fileIndex_, attributes_, semicolon_)
{
}

Node* AttributeDeclarationNode::Clone() const
{
    AttributeDeclarationNode* clone = new AttributeDeclarationNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void AttributeDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InitDeclaratorListNode::InitDeclaratorListNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    ListNode(NodeKind::initDeclaratorListNode, span_, fileIndex_)
{
}

Node* InitDeclaratorListNode::Clone() const
{
    InitDeclaratorListNode* clone = new InitDeclaratorListNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void InitDeclaratorListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InitDeclaratorNode::InitDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::initDeclaratorNode, span_, fileIndex_, nullptr, nullptr)
{
}

InitDeclaratorNode::InitDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* declarator_, Node* init_) noexcept :
    BinaryNode(NodeKind::initDeclaratorNode, span_, fileIndex_, declarator_, init_)
{
}

std::string InitDeclaratorNode::Str() const
{
    std::string str = Left()->Str();
    if (Right()->IsAssignmentInitializerNode())
    {
        str.append(Right()->Str());
    }
    else
    {
        str.append(" = ").append(Right()->Str());
    }
    return str;
}

Node* InitDeclaratorNode::Clone() const
{
    InitDeclaratorNode* clone = new InitDeclaratorNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void InitDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TrailingFunctionDeclaratorNode::TrailingFunctionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    CompoundNode(NodeKind::trailingFunctionDeclaratorNode, span_, fileIndex_)
{
}

TrailingFunctionDeclaratorNode::TrailingFunctionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* declarator_, Node* params_,
    Node* trailingReturnType_) noexcept :
    CompoundNode(NodeKind::trailingFunctionDeclaratorNode, span_, fileIndex_), declarator(declarator_), params(params_), trailingReturnType(trailingReturnType_)
{
}

Node* TrailingFunctionDeclaratorNode::Clone() const
{
    TrailingFunctionDeclaratorNode* clone = new TrailingFunctionDeclaratorNode(GetSpan(), FileIndex(), declarator->Clone(), params->Clone(), trailingReturnType->Clone());
    clone->SetId(Id());
    return clone;
}

void TrailingFunctionDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TrailingFunctionDeclaratorNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(declarator.get());
    writer.Write(params.get());
    writer.Write(trailingReturnType.get());
}

void TrailingFunctionDeclaratorNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    declarator.reset(reader.ReadNode());
    params.reset(reader.ReadNode());
    trailingReturnType.reset(reader.ReadNode());
}

ParenthesizedDeclaratorNode::ParenthesizedDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    CompoundNode(NodeKind::parenthesizedDeclaratorNode, span_, fileIndex_)
{
}

ParenthesizedDeclaratorNode::ParenthesizedDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* declarator_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::parenthesizedDeclaratorNode, span_, fileIndex_), declarator(declarator_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* ParenthesizedDeclaratorNode::Clone() const
{
    ParenthesizedDeclaratorNode* clone = new ParenthesizedDeclaratorNode(GetSpan(), FileIndex(), declarator->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void ParenthesizedDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParenthesizedDeclaratorNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(declarator.get());
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void ParenthesizedDeclaratorNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    declarator.reset(reader.ReadNode());
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

std::string ParenthesizedDeclaratorNode::Str() const
{
    std::string str(1, '(');
    str.append(declarator->Str()).append(1, ')');
    return str;
}

AbstractDeclaratorNode::AbstractDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::abstractDeclaratorNode, span_, fileIndex_)
{
}

Node* AbstractDeclaratorNode::Clone() const
{
    AbstractDeclaratorNode* clone = new AbstractDeclaratorNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void AbstractDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DeclSpecifierSequenceNode::DeclSpecifierSequenceNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    SequenceNode(NodeKind::declSpecifierSequenceNode, span_, fileIndex_)
{
}

Node* DeclSpecifierSequenceNode::Clone() const
{
    DeclSpecifierSequenceNode* clone = new DeclSpecifierSequenceNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void DeclSpecifierSequenceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string DeclSpecifierSequenceNode::Str() const
{
    std::string str;
    bool first = true;
    for (const auto& n : Nodes())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(1, ' ');
        }
        str.append(n->Str());
    }
    return str;
}

FriendNode::FriendNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::friendNode, span_, fileIndex_)
{
}

Node* FriendNode::Clone() const
{
    FriendNode* clone = new FriendNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void FriendNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypedefNode::TypedefNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::typedefNode, span_, fileIndex_)
{
}

Node* TypedefNode::Clone() const
{
    TypedefNode* clone = new TypedefNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void TypedefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstExprNode::ConstExprNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::constExprNode, span_, fileIndex_)
{
}

Node* ConstExprNode::Clone() const
{
    ConstExprNode* clone = new ConstExprNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ConstExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstEvalNode::ConstEvalNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::constEvalNode, span_, fileIndex_)
{
}

Node* ConstEvalNode::Clone() const
{
    ConstEvalNode* clone = new ConstEvalNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ConstEvalNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstInitNode::ConstInitNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::constInitNode, span_, fileIndex_)
{
}

Node* ConstInitNode::Clone() const
{
    ConstInitNode* clone = new ConstInitNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ConstInitNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InlineNode::InlineNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::inlineNode, span_, fileIndex_)
{
}

Node* InlineNode::Clone() const
{
    InlineNode* clone = new InlineNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void InlineNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StaticNode::StaticNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::staticNode, span_, fileIndex_)
{
}

Node* StaticNode::Clone() const
{
    StaticNode* clone = new StaticNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void StaticNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ThreadLocalNode::ThreadLocalNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::threadLocalNode, span_, fileIndex_)
{
}

Node* ThreadLocalNode::Clone() const
{
    ThreadLocalNode* clone = new ThreadLocalNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ThreadLocalNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExternNode::ExternNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::externNode, span_, fileIndex_)
{
}

Node* ExternNode::Clone() const
{
    ExternNode* clone = new ExternNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ExternNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MutableNode::MutableNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::mutableNode, span_, fileIndex_)
{
}

Node* MutableNode::Clone() const
{
    MutableNode* clone = new MutableNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void MutableNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExplicitNode::ExplicitNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::explicitNode, span_, fileIndex_)
{
}

Node* ExplicitNode::Clone() const
{
    ExplicitNode* clone = new ExplicitNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ExplicitNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConditionalExplicitNode::ConditionalExplicitNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    UnaryNode(NodeKind::conditionalExplicitNode, span_, fileIndex_, nullptr)
{
}

ConditionalExplicitNode::ConditionalExplicitNode(const soul::ast::Span& span_, int fileIndex_, Node* cond_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    UnaryNode(NodeKind::conditionalExplicitNode, span_, fileIndex_, cond_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* ConditionalExplicitNode::Clone() const
{
    ConditionalExplicitNode* clone = new ConditionalExplicitNode(GetSpan(), FileIndex(), cond->Clone(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void ConditionalExplicitNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConditionalExplicitNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(cond.get());
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void ConditionalExplicitNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    cond.reset(reader.ReadNode());
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

QualifiedPtrNode::QualifiedPtrNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::qualifiedPtrNode, span_, fileIndex_, nullptr, nullptr)
{
}

QualifiedPtrNode::QualifiedPtrNode(const soul::ast::Span& span_, int fileIndex_, Node* nns_, Node* ptr_) noexcept :
    BinaryNode(NodeKind::qualifiedPtrNode, span_, fileIndex_, nns_, ptr_)
{
}

Node* QualifiedPtrNode::Clone() const
{
    QualifiedPtrNode* clone = new QualifiedPtrNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void QualifiedPtrNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PtrOperatorNode::PtrOperatorNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::ptrOperatorNode, span_, fileIndex_, nullptr)
{
}

PtrOperatorNode::PtrOperatorNode(const soul::ast::Span& span_, int fileIndex_, Node* child_) noexcept : UnaryNode(NodeKind::ptrOperatorNode, span_, fileIndex_, child_)
{
}

Node* PtrOperatorNode::Clone() const
{
    PtrOperatorNode* clone = new PtrOperatorNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void PtrOperatorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PtrDeclaratorNode::PtrDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept : SequenceNode(NodeKind::ptrDeclaratorNode, span_, fileIndex_)
{
}

Node* PtrDeclaratorNode::Clone() const
{
    PtrDeclaratorNode* clone = new PtrDeclaratorNode(GetSpan(), FileIndex());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void PtrDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string PtrDeclaratorNode::Str() const
{
    std::string s;
    int n = Count();
    for (int i = 0; i < n; ++i)
    {
        Node* node = Nodes()[i];
        if (node->IsIdentifierNode())
        {
            s.append(1, ' ');
        }
        s.append(node->Str());
    }
    return s;
}

PackDeclaratorIdNode::PackDeclaratorIdNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::packDeclaratorIdNode, span_, fileIndex_, nullptr, nullptr)
{
}

PackDeclaratorIdNode::PackDeclaratorIdNode(const soul::ast::Span& span_, int fileIndex_, Node* ellipsis_, Node* idExpr_) noexcept :
    BinaryNode(NodeKind::packDeclaratorIdNode, span_, fileIndex_, ellipsis_, idExpr_)
{
}

Node* PackDeclaratorIdNode::Clone() const
{
    PackDeclaratorIdNode* clone = new PackDeclaratorIdNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void PackDeclaratorIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrayDeclaratorNode::ArrayDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    UnaryNode(NodeKind::arrayDeclaratorNode, span_, fileIndex_, nullptr)
{
}

ArrayDeclaratorNode::ArrayDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, Node* dimension_,
    const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    UnaryNode(NodeKind::arrayDeclaratorNode, span_, fileIndex_, child_), dimension(dimension_), lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* ArrayDeclaratorNode::Clone() const
{
    Node* clonedDimension = nullptr;
    if (dimension)
    {
        clonedDimension = dimension->Clone();
    }
    ArrayDeclaratorNode* clone = new ArrayDeclaratorNode(GetSpan(), FileIndex(), Child()->Clone(), clonedDimension, lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void ArrayDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArrayDeclaratorNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(dimension.get());
    writer.Write(lbSpan);
    writer.Write(rbSpan);
}

void ArrayDeclaratorNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    dimension.reset(reader.ReadNode());
    lbSpan = reader.ReadSpan();
    rbSpan = reader.ReadSpan();
}

std::string ArrayDeclaratorNode::Str() const
{
    std::string str = Child()->Str();
    str.append(1, '[').append(dimension->Str()).append(1, ']');
    return str;
}

FunctionDeclaratorNode::FunctionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    UnaryNode(NodeKind::functionDeclaratorNode, span_, fileIndex_, nullptr)
{
}

FunctionDeclaratorNode::FunctionDeclaratorNode(const soul::ast::Span& span_, int fileIndex_, Node* child_, Node* parameters_) noexcept :
    UnaryNode(NodeKind::functionDeclaratorNode, span_, fileIndex_, child_), params(parameters_)
{
}

Node* FunctionDeclaratorNode::Clone() const
{
    FunctionDeclaratorNode* clone = new FunctionDeclaratorNode(GetSpan(), FileIndex(), Child()->Clone(), params->Clone());
    clone->SetId(Id());
    return clone;
}

void FunctionDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionDeclaratorNode::Write(Writer& writer)
{
    UnaryNode::Write(writer);
    writer.Write(params.get());
}

void FunctionDeclaratorNode::Read(Reader& reader)
{
    UnaryNode::Read(reader);
    params.reset(reader.ReadNode());
}

std::string FunctionDeclaratorNode::Str() const
{
    std::string str = Child()->Str();
    str.append(1, '(').append(params->Str()).append(1, ')');
    return str;
}

PrefixNode::PrefixNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::prefixNode, span_, fileIndex_, nullptr, nullptr)
{
}

PrefixNode::PrefixNode(const soul::ast::Span& span_, int fileIndex_, Node* prefix_, Node* subject_) noexcept :
    BinaryNode(NodeKind::prefixNode, span_, fileIndex_, prefix_, subject_)
{
}

Node* PrefixNode::Clone() const
{
    PrefixNode* clone = new PrefixNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void PrefixNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TrailingQualifiersNode::TrailingQualifiersNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    SequenceNode(NodeKind::trailingQualifiersNode, span_, fileIndex_)
{
}

TrailingQualifiersNode::TrailingQualifiersNode(const soul::ast::Span& span_, int fileIndex_, Node* subject_) noexcept :
    SequenceNode(NodeKind::trailingQualifiersNode, span_, fileIndex_), subject(subject_)
{
}

Node* TrailingQualifiersNode::Clone() const
{
    TrailingQualifiersNode* clone = new TrailingQualifiersNode(GetSpan(), FileIndex(), subject->Clone());
    for (const auto& node : Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetId(Id());
    return clone;
}

void TrailingQualifiersNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TrailingQualifiersNode::Write(Writer& writer)
{
    SequenceNode::Write(writer);
    writer.Write(subject.get());
}

void TrailingQualifiersNode::Read(Reader& reader)
{
    SequenceNode::Read(reader);
    subject.reset(reader.ReadNode());
}

TrailingAttributesNode::TrailingAttributesNode(const soul::ast::Span& span_, int fileIndex_) noexcept :
    BinaryNode(NodeKind::trailingAttributesNode, span_, fileIndex_, nullptr, nullptr)
{
}

TrailingAttributesNode::TrailingAttributesNode(const soul::ast::Span& span_, int fileIndex_, Node* left_, Node* attributes_) noexcept :
    BinaryNode(NodeKind::trailingAttributesNode, span_, fileIndex_, left_, attributes_)
{
}

Node* TrailingAttributesNode::Clone() const
{
    TrailingAttributesNode* clone = new TrailingAttributesNode(GetSpan(), FileIndex(), Left()->Clone(), Right()->Clone());
    clone->SetId(Id());
    return clone;
}

void TrailingAttributesNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NoexceptSpecifierNode::NoexceptSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::noexceptSpecifierNode, span_, fileIndex_)
{
}

NoexceptSpecifierNode::NoexceptSpecifierNode(const soul::ast::Span& span_, int fileIndex_, Node* constantExpr_,
    const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::noexceptSpecifierNode, span_, fileIndex_), constantExpr(constantExpr_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* NoexceptSpecifierNode::Clone() const
{
    Node* clonedConstantExpr = nullptr;
    if (constantExpr)
    {
        clonedConstantExpr = constantExpr->Clone();
    }
    NoexceptSpecifierNode* clone = new NoexceptSpecifierNode(GetSpan(), FileIndex(), clonedConstantExpr, lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void NoexceptSpecifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NoexceptSpecifierNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(constantExpr.get());
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void NoexceptSpecifierNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    constantExpr.reset(reader.ReadNode());
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

ThrowSpecifierNode::ThrowSpecifierNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::throwSpecifierNode, span_, fileIndex_)
{
}

ThrowSpecifierNode::ThrowSpecifierNode(const soul::ast::Span& span_, int fileIndex_, const soul::ast::Span& lpSpan_, const soul::ast::Span& rpSpan_) noexcept :
    CompoundNode(NodeKind::throwSpecifierNode, span_, fileIndex_), lpSpan(lpSpan_), rpSpan(rpSpan_)
{
}

Node* ThrowSpecifierNode::Clone() const
{
    ThrowSpecifierNode* clone = new ThrowSpecifierNode(GetSpan(), FileIndex(), lpSpan, rpSpan);
    clone->SetId(Id());
    return clone;
}

void ThrowSpecifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ThrowSpecifierNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(lpSpan);
    writer.Write(rpSpan);
}

void ThrowSpecifierNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    lpSpan = reader.ReadSpan();
    rpSpan = reader.ReadSpan();
}

} // namespace otava::ast
