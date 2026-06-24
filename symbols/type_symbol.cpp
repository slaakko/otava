// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.type_symbol;

import otava.symbols.compound_type_symbol;
import otava.symbols.context;
import otava.symbols.exception;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.symbol_table;
import otava.symbols.type_symbol;
import otava.ast.declaration;
import otava.ast.identifier;
import otava.ast.qualifier;
import otava.ast.punctuation;
import otava.ast.templates;
import otava.ast.type;
import util.text_util;

namespace otava::symbols {

TypeSymbol::TypeSymbol(Module* module_, SymbolId id_) : ContainerSymbol(module_, id_)
{ 
}

TypeSymbol::TypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : ContainerSymbol(module_, id_, name_)
{
}

TypeSymbol* TypeSymbol::RemoveDerivations(Derivations derivations, Context* context)
{
    if (IsPointerType()) return nullptr;
    return this;
}

TypeSymbol* TypeSymbol::Unify(TypeSymbol* argType, Context* context)
{
    return nullptr;
}

TypeSymbol* TypeSymbol::UnifyTemplateArgumentType(const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*, std::hash<TemplateParameterSymbol*>,
    TemplateParamEqual>& templateParameterMap, const soul::ast::FullSpan& fullSpan, Context* context)
{
    return nullptr;
}

bool TypeSymbol::IsAutoTypeSymbol() const noexcept
{
    if (IsFundamentalTypeSymbol())
    {
        const FundamentalTypeSymbol* fundamentalTypeSymbol = static_cast<const FundamentalTypeSymbol*>(this);
        return fundamentalTypeSymbol->IsAutoTypeSymbol();
    }
    return false;
}

bool TypeSymbol::IsPointerType() const noexcept
{
    if (IsCompoundTypeSymbol())
    {
        const CompoundTypeSymbol* compoundTypeSymbol = static_cast<const CompoundTypeSymbol*>(this);
        return otava::symbols::PointerCount(compoundTypeSymbol->GetDerivations()) > 0;
    }
    return false;
}

bool TypeSymbol::IsArrayType() const noexcept
{
    return IsArrayTypeSymbol();
}

bool TypeSymbol::IsConstType() const noexcept
{
    if (IsCompoundTypeSymbol())
    {
        const CompoundTypeSymbol* compoundTypeSymbol = static_cast<const CompoundTypeSymbol*>(this);
        return HasDerivation(compoundTypeSymbol->GetDerivations(), Derivations::constDerivation);
    }
    return false;
}

bool TypeSymbol::IsLValueRefType() const noexcept
{
    if (IsCompoundTypeSymbol())
    {
        const CompoundTypeSymbol* compoundTypeSymbol = static_cast<const CompoundTypeSymbol*>(this);
        return HasDerivation(compoundTypeSymbol->GetDerivations(), Derivations::lvalueRefDerivation);
    }
    return false;
}

bool TypeSymbol::IsRValueRefType() const noexcept
{
    if (IsCompoundTypeSymbol())
    {
        const CompoundTypeSymbol* compoundTypeSymbol = static_cast<const CompoundTypeSymbol*>(this);
        return HasDerivation(compoundTypeSymbol->GetDerivations(), Derivations::rvalueRefDerivation);
    }
    return false;
}

bool TypeSymbol::IsReferenceType() const noexcept
{
    return IsLValueRefType() || IsRValueRefType();
}

otava::intermediate::Type* TypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("IRTYPE not implemented for " + SymbolKindStr(Kind()), fullSpan, context);
    return nullptr;
}

TypeSymbol* TypeSymbol::AddConst(Context* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    Derivations derivations = Derivations::constDerivation;
    return symbolTable->MakeCompoundType(this, derivations, context);
}

TypeSymbol* TypeSymbol::RemoveConst(Context* context)
{
    if (IsCompoundTypeSymbol())
    {
        SymbolTable* symbolTable = context->GetSymbolTable();
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(this);
        return symbolTable->MakeCompoundType(GetBaseType(context), otava::symbols::RemoveConst(compoundTypeSymbol->GetDerivations()), context);
    }
    return this;
}

TypeSymbol* TypeSymbol::AddPointer(Context* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    Derivations derivations = Derivations::none;
    derivations = otava::symbols::SetPointerCount(derivations, 1);
    return symbolTable->MakeCompoundType(this, derivations, context);
}

TypeSymbol* TypeSymbol::RemovePointer(Context* context)
{
    if (IsCompoundTypeSymbol())
    {
        SymbolTable* symbolTable = context->GetSymbolTable();
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(this);
        return symbolTable->MakeCompoundType(GetBaseType(context), otava::symbols::RemovePointer(compoundTypeSymbol->GetDerivations()), context);
    }
    return this;
}

TypeSymbol* TypeSymbol::AddLValueRef(Context* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    Derivations derivations = Derivations::lvalueRefDerivation;
    return symbolTable->MakeCompoundType(this, derivations, context);
}

TypeSymbol* TypeSymbol::RemoveLValueRef(Context* context)
{
    if (IsCompoundTypeSymbol())
    {
        SymbolTable* symbolTable = context->GetSymbolTable();
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(this);
        return symbolTable->MakeCompoundType(GetBaseType(context), otava::symbols::RemoveLValueRef(compoundTypeSymbol->GetDerivations()), context);
    }
    return this;
}

TypeSymbol* TypeSymbol::AddRValueRef(Context* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    Derivations derivations = Derivations::rvalueRefDerivation;
    return symbolTable->MakeCompoundType(this, derivations, context);
}

TypeSymbol* TypeSymbol::RemoveRValueRef(Context* context)
{
    if (IsCompoundTypeSymbol())
    {
        SymbolTable* symbolTable = context->GetSymbolTable();
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(this);
        return symbolTable->MakeCompoundType(GetBaseType(context), otava::symbols::RemoveRValueRef(compoundTypeSymbol->GetDerivations()), context);
    }
    return this;
}

TypeSymbol* TypeSymbol::RemoveReference(Context* context)
{
    if (IsLValueRefType())
    {
        return RemoveLValueRef(context);
    }
    else if (IsRValueRefType())
    {
        return RemoveRValueRef(context);
    }
    else
    {
        return this;
    }
}

TypeSymbol* TypeSymbol::RemoveRefOrPtr(Context* context)
{
    if (IsReferenceType())
    {
        return PlainType(context);
    }
    else if (IsPointerType())
    {
        return RemovePointer(context);
    }
    return this;
}

ClassGroupTypeSymbol::ClassGroupTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_), classGroup(nullptr)
{
}

ClassGroupTypeSymbol::ClassGroupTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_), classGroup(nullptr)
{
}

AliasGroupTypeSymbol::AliasGroupTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_), aliasGroup(nullptr)
{
}

AliasGroupTypeSymbol::AliasGroupTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_), aliasGroup(nullptr)
{
}

FunctionGroupTypeSymbol::FunctionGroupTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_), functionGroup(nullptr)
{
}

FunctionGroupTypeSymbol::FunctionGroupTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_), functionGroup(nullptr)
{
}

NestedTypeSymbol::NestedTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_)
{
}

NestedTypeSymbol::NestedTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_)
{
}

DependentTypeSymbol::DependentTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_)
{
}

DependentTypeSymbol::DependentTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_)
{
}

void DependentTypeSymbol::ResetNode(otava::ast::Node* node_)
{
    node.reset(node_);
    if (node->IsTypenameSpecifierNode())
    {
        otava::ast::TypenameSpecifierNode* s = static_cast<otava::ast::TypenameSpecifierNode*>(node.get());
        SetName(s->GetId()->Str());
    }
}

void DependentTypeSymbol::Write(Writer& writer)
{
    TypeSymbol::Write(writer);
    otava::symbols::WriteNode(writer, node.get(), astNodeHeader);
}

void DependentTypeSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    node = otava::symbols::ReadNode(reader, GetModule(), astNodeHeader);
}

TypeSymbol* ConvertRefToPtrType(TypeSymbol* type, Context* context)
{
    if (type->IsLValueRefType())
    {
        return type->RemoveLValueRef(context)->AddPointer(context);
    }
    else if (type->IsRValueRefType())
    {
        return type->RemoveRValueRef(context)->AddPointer(context);
    }
    return type;
}

std::pair<std::unique_ptr<otava::ast::Node>, std::unique_ptr<otava::ast::Node>> TypeToAst(TypeSymbol* type, const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::unique_ptr<otava::ast::Node> node;
    std::unique_ptr<otava::ast::Node> declarator;
    if (type->GetBaseType(context)->IsFundamentalTypeSymbol())
    {
        FundamentalTypeSymbol* fundamentalType = static_cast<FundamentalTypeSymbol*>(type->GetBaseType(context));
        std::unique_ptr<otava::ast::TypeSpecifierSequenceNode> typeSpecifiers(new otava::ast::TypeSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex));
        if (type->IsConstType())
        {
            typeSpecifiers->AddNode(new otava::ast::ConstNode(fullSpan.span, fullSpan.fileIndex));
        }
        MakeFundamentaTypeSequence(fundamentalType, fullSpan, typeSpecifiers.get());
        node.reset(typeSpecifiers.release());
    }
    else if (type->GetBaseType(context)->IsEnumeratedTypeSymbol())
    {
        node.reset(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, type->GetBaseType(context)->Name()));
    }
    else if (type->GetBaseType(context)->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(type->GetBaseType(context));
        node.reset(new otava::ast::TemplateIdNode(fullSpan.span, fullSpan.fileIndex,
            new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, specialization->ClassTemplate(context)->Name())));
        bool first = true;
        for (auto* templateArg : specialization->TemplateArguments(context))
        {
            if (first)
            {
                first = false;
            }
            else
            {
                node->AddNode(new otava::ast::CommaNode(fullSpan.span, fullSpan.fileIndex));
            }
            if (templateArg->IsTypeSymbol())
            {
                TypeSymbol* templateArgType = static_cast<TypeSymbol*>(templateArg);
                std::pair<std::unique_ptr<otava::ast::Node>, std::unique_ptr<otava::ast::Node>> nodeDeclarator = TypeToAst(templateArgType, fullSpan, context);
                std::unique_ptr<otava::ast::Node> argNode = std::move(nodeDeclarator.first);
                std::unique_ptr<otava::ast::Node> argDeclarator = std::move(nodeDeclarator.second);
                std::unique_ptr<otava::ast::TypeSpecifierSequenceNode> typeSpecifiers;
                if (argNode->IsTypeSpecifierSequenceNode())
                {
                    typeSpecifiers.reset(static_cast<otava::ast::TypeSpecifierSequenceNode*>(argNode.release()));
                }
                else
                {
                    typeSpecifiers.reset(new otava::ast::TypeSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex));
                    if (templateArgType->IsConstType())
                    {
                        typeSpecifiers->AddNode(new otava::ast::ConstNode(fullSpan.span, fullSpan.fileIndex));
                    }
                    typeSpecifiers->AddNode(argNode.release());
                }
                std::unique_ptr<otava::ast::TypeIdNode> typeIdNode(new otava::ast::TypeIdNode(fullSpan.span, fullSpan.fileIndex,
                    typeSpecifiers.release(), argDeclarator.release()));
                node->AddNode(typeIdNode.release());
            }
        }
    }
    else if (type->GetBaseType(context)->IsClassTypeSymbol())
    {
        node.reset(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, type->GetBaseType(context)->Name()));
    }
    NamespaceSymbol* ns = type->ParentNamespace(context);
    if (ns)
    {
        std::unique_ptr<otava::ast::NestedNameSpecifierNode> nns(new otava::ast::NestedNameSpecifierNode(fullSpan.span, fullSpan.fileIndex));
        std::string nsFullName = ns->FullName(context);
        if (!nsFullName.empty())
        {
            std::vector<std::string> components = util::Split(nsFullName, std::string("::"));
            bool first = true;
            for (const auto& component : components)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    nns->AddNode(new otava::ast::ColonColonNode(fullSpan.span, fullSpan.fileIndex));
                }
                nns->AddNode(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, component));
            }
            nns->AddNode(new otava::ast::ColonColonNode(fullSpan.span, fullSpan.fileIndex));
            otava::ast::QualifiedIdNode* qid = new otava::ast::QualifiedIdNode(fullSpan.span, fullSpan.fileIndex, nns.release(), node.release());
            node.reset(qid);
        }
    }
    declarator.reset(new otava::ast::AbstractDeclaratorNode(fullSpan.span, fullSpan.fileIndex));
    if (type->IsPointerType() || type->IsReferenceType())
    {
        declarator.reset(new otava::ast::PtrDeclaratorNode(fullSpan.span, fullSpan.fileIndex));
        int n = type->PointerCount();
        for (int i = 0; i < n; ++i)
        {
            declarator->AddNode(new otava::ast::PtrNode(fullSpan.span, fullSpan.fileIndex));
        }
        if (type->IsLValueRefType())
        {
            declarator->AddNode(new otava::ast::LvalueRefNode(fullSpan.span, fullSpan.fileIndex));
        }
        else if (type->IsRValueRefType())
        {
            declarator->AddNode(new otava::ast::RvalueRefNode(fullSpan.span, fullSpan.fileIndex));
        }
        declarator->AddNode(new otava::ast::AbstractDeclaratorNode(fullSpan.span, fullSpan.fileIndex));
    }
    return std::make_pair(std::move(node), std::move(declarator));
}

std::unique_ptr<otava::ast::SimpleDeclarationNode> DeclarationToSimpleDeclarationAst(TypeSymbol* type, const std::string& variableName, otava::ast::Node* initializer,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::unique_ptr<otava::ast::SimpleDeclarationNode> node(new otava::ast::SimpleDeclarationNode(fullSpan.span, fullSpan.fileIndex));
    std::unique_ptr<otava::ast::DeclSpecifierSequenceNode> sequence(new otava::ast::DeclSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex));
    if (type->IsConstType())
    {
        sequence->AddNode(new otava::ast::ConstNode(fullSpan.span, fullSpan.fileIndex));
    }
    std::unique_ptr<otava::ast::Node> name;
    if (type->GetBaseType(context)->IsFundamentalTypeSymbol())
    {
        FundamentalTypeSymbol* fundamentalType = static_cast<FundamentalTypeSymbol*>(type->GetBaseType(context));
        MakeFundamentaTypeSequence(fundamentalType, fullSpan, sequence.get());
    }
    else if (type->GetBaseType(context)->IsEnumeratedTypeSymbol())
    {
        name.reset(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, type->GetBaseType(context)->Name()));
    }
    else if (type->GetBaseType(context)->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(type->GetBaseType(context));
        name.reset(new otava::ast::TemplateIdNode(fullSpan.span, fullSpan.fileIndex,
            new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, specialization->ClassTemplate(context)->Name())));
        bool first = true;
        for (auto* templateArg : specialization->TemplateArguments(context))
        {
            if (first)
            {
                first = false;
            }
            else
            {
                name->AddNode(new otava::ast::CommaNode(fullSpan.span, fullSpan.fileIndex));
            }
            if (templateArg->IsTypeSymbol())
            {
                TypeSymbol* templateArgType = static_cast<TypeSymbol*>(templateArg);
                std::pair<std::unique_ptr<otava::ast::Node>, std::unique_ptr<otava::ast::Node>> nodeDeclarator = TypeToAst(templateArgType, fullSpan, context);
                std::unique_ptr<otava::ast::Node> argNode = std::move(nodeDeclarator.first);
                std::unique_ptr<otava::ast::Node> argDeclarator = std::move(nodeDeclarator.second);
                std::unique_ptr<otava::ast::TypeSpecifierSequenceNode> typeSpecifiers;
                if (argNode->IsTypeSpecifierSequenceNode())
                {
                    typeSpecifiers.reset(static_cast<otava::ast::TypeSpecifierSequenceNode*>(argNode.release()));
                }
                else
                {
                    typeSpecifiers.reset(new otava::ast::TypeSpecifierSequenceNode(fullSpan.span, fullSpan.fileIndex));
                    if (templateArgType->IsConstType())
                    {
                        typeSpecifiers->AddNode(new otava::ast::ConstNode(fullSpan.span, fullSpan.fileIndex));
                    }
                    typeSpecifiers->AddNode(argNode.release());
                }
                std::unique_ptr<otava::ast::TypeIdNode> typeIdNode(new otava::ast::TypeIdNode(fullSpan.span, fullSpan.fileIndex,
                    typeSpecifiers.release(), argDeclarator.release()));
                name->AddNode(typeIdNode.release());
            }
        }
    }
    else if (type->GetBaseType(context)->IsClassTypeSymbol())
    {
        name.reset(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, type->GetBaseType(context)->Name()));
    }
    NamespaceSymbol* ns = type->ParentNamespace(context);
    if (ns)
    {
        std::unique_ptr<otava::ast::NestedNameSpecifierNode> nns(new otava::ast::NestedNameSpecifierNode(fullSpan.span, fullSpan.fileIndex));
        std::string nsFullName = ns->FullName(context);
        if (!nsFullName.empty())
        {
            std::vector<std::string> components = util::Split(nsFullName, std::string("::"));
            bool first = true;
            for (const auto& component : components)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    nns->AddNode(new otava::ast::ColonColonNode(fullSpan.span, fullSpan.fileIndex));
                }
                nns->AddNode(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, component));
            }
            nns->AddNode(new otava::ast::ColonColonNode(fullSpan.span, fullSpan.fileIndex));
            otava::ast::QualifiedIdNode* qid = new otava::ast::QualifiedIdNode(fullSpan.span, fullSpan.fileIndex, nns.release(), name.release());
            sequence->AddNode(qid);
        }
    }
    node->SetDeclarationSpecifiers(sequence.release());
    std::unique_ptr<otava::ast::InitDeclaratorListNode> initDeclarators(new otava::ast::InitDeclaratorListNode(fullSpan.span, fullSpan.fileIndex));
    std::unique_ptr<otava::ast::Node> declarator;
    if (type->IsReferenceType() || type->IsPointerType())
    {
        declarator.reset(new otava::ast::PtrDeclaratorNode(fullSpan.span, fullSpan.fileIndex));
        int n = type->PointerCount();
        for (int i = 0; i < n; ++i)
        {
            declarator->AddNode(new otava::ast::PtrNode(fullSpan.span, fullSpan.fileIndex));
        }
        if (type->IsLValueRefType())
        {
            declarator->AddNode(new otava::ast::LvalueRefNode(fullSpan.span, fullSpan.fileIndex));
        }
        else if (type->IsRValueRefType())
        {
            declarator->AddNode(new otava::ast::RvalueRefNode(fullSpan.span, fullSpan.fileIndex));
        }
        declarator->AddNode(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, variableName));
    }
    else
    {
        declarator.reset(new otava::ast::IdentifierNode(fullSpan.span, fullSpan.fileIndex, variableName));
    }
    std::unique_ptr<otava::ast::InitDeclaratorNode> initDeclarator(new otava::ast::InitDeclaratorNode(fullSpan.span, fullSpan.fileIndex,
        declarator.release(), initializer));
    initDeclarators->AddNode(initDeclarator.release());
    node->SetInitDeclaratorList(initDeclarators.release());
    return node;
}

std::unique_ptr<otava::ast::DeclarationStatementNode> DeclarationToAst(TypeSymbol* type, const std::string& variableName, otava::ast::Node* initializer,
    const soul::ast::FullSpan& fullSpan, Context* context)
{
    std::unique_ptr<otava::ast::SimpleDeclarationNode> simpleDeclarationNode = DeclarationToSimpleDeclarationAst(type, variableName, initializer, fullSpan, context);
    std::unique_ptr<otava::ast::DeclarationStatementNode> statementNode(new otava::ast::DeclarationStatementNode(fullSpan.span,
        fullSpan.fileIndex, simpleDeclarationNode.release()));
    return statementNode;
}

} // namespace otava::symbols
