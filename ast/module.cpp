// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.ast.modules;

import otava.ast.visitor;
import otava.ast.reader;
import otava.ast.writer;

namespace otava::ast {

ModuleDeclarationNode::ModuleDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::moduleDeclarationNode, span_, fileIndex_)
{
}

ModuleDeclarationNode::ModuleDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* exprt_, Node* modle_, Node* moduleName_,
    Node* modulePartition_, Node* attributes_, Node* semicolon_) noexcept :
    CompoundNode(NodeKind::moduleDeclarationNode, span_, fileIndex_), exprt(exprt_), module(modle_), moduleName(moduleName_), modulePartition(modulePartition_), 
    attributes(attributes_), semicolon(semicolon_)
{
}

Node* ModuleDeclarationNode::Clone() const
{
    Node* clonedExport = nullptr;
    if (exprt)
    {
        clonedExport = exprt->Clone();
    }
    Node* clonedModulePartition = nullptr;
    if (modulePartition)
    {
        clonedModulePartition = modulePartition->Clone();
    }
    Node* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = attributes->Clone();
    }
    ModuleDeclarationNode* clone = new ModuleDeclarationNode(GetSpan(), FileIndex(), clonedExport, module->Clone(), moduleName->Clone(), clonedModulePartition, 
        clonedAttributes, semicolon->Clone());
    clone->SetId(Id());
    return clone;
}

void ModuleDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ModuleDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(exprt.get());
    writer.Write(module.get());
    writer.Write(moduleName.get());
    writer.Write(modulePartition.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
}

void ModuleDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    exprt.reset(reader.ReadNode());
    module.reset(reader.ReadNode());
    moduleName.reset(reader.ReadNode());
    modulePartition.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

ExportDeclarationNode::ExportDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : CompoundNode(NodeKind::exportDeclarationNode, span_, fileIndex_)
{
}

ExportDeclarationNode::ExportDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* exprt_, Node* subject_,
    const soul::ast::Span& lbSpan_, const soul::ast::Span& rbSpan_) noexcept :
    CompoundNode(NodeKind::exportDeclarationNode, span_, fileIndex_), exprt(exprt_), subject(subject_), lbSpan(lbSpan_), rbSpan(rbSpan_)
{
}

Node* ExportDeclarationNode::Clone() const
{
    Node* clonedSubject = nullptr;
    if (subject)
    {
        clonedSubject = subject->Clone();
    }
    ExportDeclarationNode* clone = new ExportDeclarationNode(GetSpan(), FileIndex(), exprt->Clone(), clonedSubject, lbSpan, rbSpan);
    clone->SetId(Id());
    return clone;
}

void ExportDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExportDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(exprt.get());
    writer.Write(subject.get());
    //writer.Write(lbSpan);
    //writer.Write(rbSpan);
}

void ExportDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    exprt.reset(reader.ReadNode());
    subject.reset(reader.ReadNode());
    //lbSpan = reader.ReadSpan();
    //rbSpan = reader.ReadSpan();
}

ExportNode::ExportNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::exportNode, span_, fileIndex_)
{
}

Node* ExportNode::Clone() const
{
    ExportNode* clone = new ExportNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ExportNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ImportNode::ImportNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::importNode, span_, fileIndex_)
{
}

Node* ImportNode::Clone() const
{
    ImportNode* clone = new ImportNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ImportNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ImportDeclarationNode::ImportDeclarationNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::importDeclarationNode, span_, fileIndex_)
{
}

ImportDeclarationNode::ImportDeclarationNode(const soul::ast::Span& span_, int fileIndex_, Node* imprt_, Node* subject_, Node* attributes_, Node* semicolon_) noexcept :
    CompoundNode(NodeKind::importDeclarationNode, span_, fileIndex_), imprt(imprt_), subject(subject_), attributes(attributes_), semicolon(semicolon_)
{
}

Node* ImportDeclarationNode::Clone() const
{
    Node* clonedAtributes = nullptr;
    if (attributes)
    {
        clonedAtributes = attributes->Clone();
    }
    ImportDeclarationNode* clone = new ImportDeclarationNode(GetSpan(), FileIndex(), imprt->Clone(), subject->Clone(), clonedAtributes, semicolon->Clone());
    clone->SetId(Id());
    return clone;
}

void ImportDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ImportDeclarationNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(imprt.get());
    writer.Write(subject.get());
    writer.Write(attributes.get());
    writer.Write(semicolon.get());
}

void ImportDeclarationNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    imprt.reset(reader.ReadNode());
    subject.reset(reader.ReadNode());
    attributes.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
}

ModulePartitionNode::ModulePartitionNode(const soul::ast::Span& span_, int fileIndex_) noexcept : UnaryNode(NodeKind::modulePartitionNode, span_, fileIndex_, nullptr)
{
}

ModulePartitionNode::ModulePartitionNode(const soul::ast::Span& span_, int fileIndex_, Node* moduleName_) noexcept :
    UnaryNode(NodeKind::modulePartitionNode, span_, fileIndex_, moduleName_)
{
}

Node* ModulePartitionNode::Clone() const
{
    ModulePartitionNode* clone = new ModulePartitionNode(GetSpan(), FileIndex(), Child()->Clone());
    clone->SetId(Id());
    return clone;
}

void ModulePartitionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ModuleNode::ModuleNode(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::moduleNode, span_, fileIndex_)
{
}

Node* ModuleNode::Clone() const
{
    ModuleNode* clone = new ModuleNode(GetSpan(), FileIndex());
    clone->SetId(Id());
    return clone;
}

void ModuleNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GlobalModuleFragmentNode::GlobalModuleFragmentNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::globalModuleFragmentNode, span_, fileIndex_)
{
}

GlobalModuleFragmentNode::GlobalModuleFragmentNode(const soul::ast::Span& span_, int fileIndex_, Node* modle_, Node* semicolon_, Node* declarations_) noexcept :
    CompoundNode(NodeKind::globalModuleFragmentNode, span_, fileIndex_), module(modle_), semicolon(semicolon_), declarations(declarations_)
{
}

Node* GlobalModuleFragmentNode::Clone() const
{
    Node* clonedDeclarations = nullptr;
    if (declarations)
    {
        clonedDeclarations = declarations->Clone();
    }
    GlobalModuleFragmentNode* clone = new GlobalModuleFragmentNode(GetSpan(), FileIndex(), module->Clone(), semicolon->Clone(), clonedDeclarations);
    clone->SetId(Id());
    return clone;
}

void GlobalModuleFragmentNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void GlobalModuleFragmentNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(module.get());
    writer.Write(semicolon.get());
    writer.Write(declarations.get());
}

void GlobalModuleFragmentNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    module.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    declarations.reset(reader.ReadNode());
}

PrivateModuleFragmentNode::PrivateModuleFragmentNode(const soul::ast::Span& span_, int fileIndex_) noexcept : 
    CompoundNode(NodeKind::privateModuleFragmentNode, span_, fileIndex_)
{
}

PrivateModuleFragmentNode::PrivateModuleFragmentNode(const soul::ast::Span& span_, int fileIndex_, Node* modle_, Node* colon_, Node* privat_,
    Node* semicolon_, Node* declarations_) noexcept :
    CompoundNode(NodeKind::privateModuleFragmentNode, span_, fileIndex_), module(modle_), colon(colon_), privat(privat_), semicolon(semicolon_), declarations(declarations_)
{
}

Node* PrivateModuleFragmentNode::Clone() const
{
    Node* clonedDeclarations = nullptr;
    if (declarations)
    {
        clonedDeclarations = declarations->Clone();
    }
    PrivateModuleFragmentNode* clone = new PrivateModuleFragmentNode(GetSpan(), FileIndex(), module->Clone(), colon->Clone(), privat->Clone(), 
        semicolon->Clone(), clonedDeclarations);
    clone->SetId(Id());
    return clone;
}

void PrivateModuleFragmentNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PrivateModuleFragmentNode::Write(Writer& writer)
{
    CompoundNode::Write(writer);
    writer.Write(module.get());
    writer.Write(colon.get());
    writer.Write(privat.get());
    writer.Write(semicolon.get());
    writer.Write(declarations.get());
}

void PrivateModuleFragmentNode::Read(Reader& reader)
{
    CompoundNode::Read(reader);
    module.reset(reader.ReadNode());
    colon.reset(reader.ReadNode());
    privat.reset(reader.ReadNode());
    semicolon.reset(reader.ReadNode());
    declarations.reset(reader.ReadNode());
}

AngleHeaderName::AngleHeaderName(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::angleHeaderNameNode, span_, fileIndex_)
{
}

AngleHeaderName::AngleHeaderName(const soul::ast::Span& span_, int fileIndex_, const std::string& rep_) : 
    Node(NodeKind::angleHeaderNameNode, span_, fileIndex_), rep(rep_)
{
}

Node* AngleHeaderName::Clone() const
{
    AngleHeaderName* clone = new AngleHeaderName(GetSpan(), FileIndex(), rep);
    clone->SetId(Id());
    return clone;
}

void AngleHeaderName::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AngleHeaderName::Write(Writer& writer)
{
    Node::Write(writer);
    writer.Write(rep);
}

void AngleHeaderName::Read(Reader& reader)
{
    Node::Read(reader);
    rep = reader.ReadStr();
}

QuoteHeaderName::QuoteHeaderName(const soul::ast::Span& span_, int fileIndex_) noexcept : Node(NodeKind::quoteHeaderNameNode, span_, fileIndex_)
{
}

QuoteHeaderName::QuoteHeaderName(const soul::ast::Span& span_, int fileIndex_, const std::string& rep_) : 
    Node(NodeKind::quoteHeaderNameNode, span_, fileIndex_), rep(rep_)
{
}

Node* QuoteHeaderName::Clone() const
{
    QuoteHeaderName* clone = new QuoteHeaderName(GetSpan(), FileIndex(), rep);
    clone->SetId(Id());
    return clone;
}

void QuoteHeaderName::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void QuoteHeaderName::Write(Writer& writer)
{
    Node::Write(writer);
    writer.Write(rep);
}

void QuoteHeaderName::Read(Reader& reader)
{
    Node::Read(reader);
    rep = reader.ReadStr();
}

} // namespace otava::ast
