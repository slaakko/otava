// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.declaration;

import std;
import otava.ast.attribute;
import otava.ast.classes;
import otava.ast.identifier;
import otava.ast.visitor;
import otava.ast.declaration;
import otava.ast.expression;
import otava.ast.function;
import otava.ast.qualifier;
import otava.ast.simple_type;
import otava.ast.literal;
import otava.ast.templates;
import otava.ast.type;
import otava.symbols.alias_type_symbol;
import otava.symbols.array_type_symbol;
import otava.symbols.classes;
import otava.symbols.class_templates;
import otava.symbols.context;
import otava.symbols.declarator;
import otava.symbols.derivations;
import otava.symbols.evaluator;
import otava.symbols.exception;
import otava.symbols.expression_binder;
import otava.symbols.function_kind;
import otava.symbols.function_templates;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.instantiation_queue;
import otava.symbols.statement_binder;
import otava.symbols.type_compare;
import otava.symbols.type_resolver;

namespace otava::symbols {

struct ArraySizeComputer : public otava::ast::DefaultVisitor
{
    ArraySizeComputer();
    void Visit(otava::ast::InitDeclaratorNode& node) override;
    void Visit(otava::ast::AssignmentInitNode& node) override;
    void Visit(otava::ast::BracedInitListNode& node) override;
    std::int64_t size;
};

ArraySizeComputer::ArraySizeComputer() : size(-1)
{
}

void ArraySizeComputer::Visit(otava::ast::InitDeclaratorNode& node)
{
    node.Right()->Accept(*this);
}

void ArraySizeComputer::Visit(otava::ast::AssignmentInitNode& node)
{
    node.Child()->Accept(*this);
}

void ArraySizeComputer::Visit(otava::ast::BracedInitListNode& node)
{
    std::int64_t count = 0;
    for (otava::ast::Node* element : node.Items())
    {
        if (element->IsLBraceNode() || element->IsRBraceNode()) continue;
        ++count;
    }
    size = count;
}

std::int64_t GetSizeFromInitializer(otava::ast::Node* node)
{
    if (node->Parent())
    {
        ArraySizeComputer visitor;
        node->Parent()->Accept(visitor);
        return visitor.size;
    }
    return -1;
}

class NoReturnAttributeMatcher : public otava::ast::DefaultVisitor
{
public:
    NoReturnAttributeMatcher();
    void Visit(otava::ast::AttributeNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    inline bool GetValue() const { return value; }
private:
    bool matchId;
    bool value;
};

NoReturnAttributeMatcher::NoReturnAttributeMatcher() : matchId(false), value(false)
{
}

void NoReturnAttributeMatcher::Visit(otava::ast::AttributeNode& node)
{
    matchId = true;
    node.AttributeToken()->Accept(*this);
    matchId = false;
}

void NoReturnAttributeMatcher::Visit(otava::ast::IdentifierNode& node)
{
    if (!matchId) return;
    if (node.Str() == "noreturn")
    {
        value = true;
    }
}

bool HasNoReturnAttribute(otava::ast::Node* attributes) noexcept
{
    if (!attributes) return false;
    NoReturnAttributeMatcher noreturnAttributeMatcher;
    attributes->Accept(noreturnAttributeMatcher);
    return noreturnAttributeMatcher.GetValue();
}

std::string DeclarationFlagStr(DeclarationFlags flags)
{
    std::string str;
    if ((flags & DeclarationFlags::typedefFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("typedef");
    }
    if ((flags & DeclarationFlags::staticFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("static");
    }
    if ((flags & DeclarationFlags::threadLocalFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("thread_local");
    }
    if ((flags & DeclarationFlags::externFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("extern");
    }
    if ((flags & DeclarationFlags::mutableFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("mutable");
    }
    if ((flags & DeclarationFlags::virtualFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("virtual");
    }
    if ((flags & DeclarationFlags::explicitFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("explicit");
    }
    if ((flags & DeclarationFlags::inlineFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("inline");
    }
    if ((flags & DeclarationFlags::friendFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("friend");
    }
    if ((flags & DeclarationFlags::constExprFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("constexpr");
    }
    if ((flags & DeclarationFlags::constEvalFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("consteval");
    }
    if ((flags & DeclarationFlags::constInitFlag) != DeclarationFlags::none)
    {
        if (!str.empty())
        {
            str.append(1, ' ');
        }
        str.append("constinit");
    }
    return str;
}

class FunctionQualifierExtractor : public otava::ast::DefaultVisitor
{
public:
    FunctionQualifierExtractor();
    inline FunctionQualifiers Qualifiers() const { return qualifiers; }
    void Visit(otava::ast::VirtSpecifierSequenceNode& node) override;
    void Visit(otava::ast::OverrideNode& node) override;
    void Visit(otava::ast::FinalNode& node) override;
private:
    FunctionQualifiers qualifiers;
};

FunctionQualifierExtractor::FunctionQualifierExtractor() : qualifiers(FunctionQualifiers::none)
{
}

void FunctionQualifierExtractor::Visit(otava::ast::VirtSpecifierSequenceNode& node)
{
    for (const auto& specifierNode : node.Nodes())
    {
        specifierNode->Accept(*this);
    }
}

void FunctionQualifierExtractor::Visit(otava::ast::OverrideNode& node)
{
    qualifiers = qualifiers | FunctionQualifiers::isOverride;
}

void FunctionQualifierExtractor::Visit(otava::ast::FinalNode& node)
{
    qualifiers = qualifiers | FunctionQualifiers::isFinal;
}

FunctionQualifiers GetQualifiers(otava::ast::Node* specifierNode)
{
    FunctionQualifierExtractor extractor;
    if (specifierNode)
    {
        specifierNode->Accept(extractor);
    }
    return extractor.Qualifiers();
}

class DeclarationProcessor : public otava::ast::DefaultVisitor
{
public:
    DeclarationProcessor(Context* context_);
    std::unique_ptr<DeclarationList> GetDeclarations();
    inline TypeSymbol* GetType() const { return type; }
    void BeginProcessFunctionDefinition(otava::ast::Node* declSpecifierSeq, otava::ast::Node* declarator, otava::ast::Node* specifierNode);
    void Visit(otava::ast::SimpleDeclarationNode& node) override;
    void Visit(otava::ast::MemberDeclarationNode& node) override;
    void Visit(otava::ast::NoDeclSpecFunctionDeclarationNode& node) override;
    void Visit(otava::ast::FunctionDefinitionNode& node) override;
    void Visit(otava::ast::ParameterNode& node) override;
    void Visit(otava::ast::ExceptionDeclarationNode& node) override;
    void Visit(otava::ast::ClassSpecifierNode& node) override;
    void Visit(otava::ast::EnumSpecifierNode& node) override;
    void Visit(otava::ast::ElaboratedTypeSpecifierNode& node) override;
    void Visit(otava::ast::InitConditionNode& node) override;
    TypeSymbol* ResolveBaseType(otava::ast::Node* node);
    void Visit(otava::ast::CharNode& node) override;
    void Visit(otava::ast::Char8Node& node) override;
    void Visit(otava::ast::Char16Node& node) override;
    void Visit(otava::ast::Char32Node& node) override;
    void Visit(otava::ast::WCharNode& node) override;
    void Visit(otava::ast::BoolNode& node) override;
    void Visit(otava::ast::ShortNode& node) override;
    void Visit(otava::ast::IntNode& node) override;
    void Visit(otava::ast::LongNode& node) override;
    void Visit(otava::ast::SignedNode& node) override;
    void Visit(otava::ast::UnsignedNode& node) override;
    void Visit(otava::ast::FloatNode& node) override;
    void Visit(otava::ast::DoubleNode& node) override;
    void Visit(otava::ast::VoidNode& node) override;
    void Visit(otava::ast::ConstNode& node) override;
    void Visit(otava::ast::VolatileNode& node) override;
    void Visit(otava::ast::PlaceholderTypeSpecifierNode& node) override;

    void Visit(otava::ast::FriendNode& node) override;
    void Visit(otava::ast::TypedefNode& node) override;
    void Visit(otava::ast::ConstExprNode& node) override;
    void Visit(otava::ast::ConstEvalNode& node) override;
    void Visit(otava::ast::ConstInitNode& node) override;
    void Visit(otava::ast::InlineNode& node) override;
    void Visit(otava::ast::StaticNode& node) override;
    void Visit(otava::ast::ThreadLocalNode& node) override;
    void Visit(otava::ast::ExternNode& node) override;
    void Visit(otava::ast::VirtualNode& node) override;
    void Visit(otava::ast::ExplicitNode& node) override;

    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::TemplateIdNode& node) override;

    void Visit(otava::ast::TypenameSpecifierNode& node) override;
private:
    Context* context;
    DeclarationFlags flags;
    bool isClassSpecifier;
    TypeSymbol* type;
    std::unique_ptr<DeclarationList> declarationList;
};

DeclarationProcessor::DeclarationProcessor(Context* context_) :
    context(context_), flags(DeclarationFlags::none), isClassSpecifier(false), type(nullptr), declarationList(new DeclarationList())
{
}

std::unique_ptr<DeclarationList> DeclarationProcessor::GetDeclarations()
{
    return std::move(declarationList);
}

void DeclarationProcessor::BeginProcessFunctionDefinition(otava::ast::Node* declSpecifierSeq, otava::ast::Node* declarator, otava::ast::Node* specifierNode)
{
    if (declSpecifierSeq)
    {
        declSpecifierSeq->Accept(*this);
    }
    TypeSymbol* baseType = ResolveBaseType(declSpecifierSeq);
    FunctionQualifiers qualifiers = GetQualifiers(specifierNode);
    Declaration declaration = ProcessDeclarator(baseType, declarator, nullptr, flags, qualifiers, context);
    declarationList->declarations.push_back(std::move(declaration));
}

TypeSymbol* DeclarationProcessor::ResolveBaseType(otava::ast::Node* node)
{
    TypeSymbol* baseType = nullptr;
    DeclarationFlags fundamentalTypeFlags = flags & DeclarationFlags::fundamentalTypeFlags;
    if (fundamentalTypeFlags != DeclarationFlags::none)
    {
        baseType = GetFundamentalType(fundamentalTypeFlags, node->GetFullSpan(), context);
    }
    else
    {
        while (type && type->IsAliasTypeSymbol())
        {
            AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(type);
            type = aliasType->ReferredType(context);
        }
        baseType = type;
    }
    if (baseType)
    {
        Derivations derivations = Derivations::none;
        if ((flags & DeclarationFlags::constFlag) != DeclarationFlags::none)
        {
            derivations = derivations | Derivations::constDerivation;
        }
        if ((flags & DeclarationFlags::volatileFlag) != DeclarationFlags::none)
        {
            derivations = derivations | Derivations::volatileDerivation;
        }
        if (derivations != Derivations::none)
        {
            baseType = context->GetSymbolTable()->MakeCompoundType(baseType, derivations, context);
        }
    }
    return baseType;
}

void DeclarationProcessor::Visit(otava::ast::SimpleDeclarationNode& node)
{
    node.DeclarationSpecifiers()->Accept(*this);
    if (node.InitDeclaratorList())
    {
        TypeSymbol* baseType = ResolveBaseType(&node);
        declarationList = ProcessInitDeclaratorList(baseType, &node, node.InitDeclaratorList(), flags, context);
    }
}

void DeclarationProcessor::Visit(otava::ast::MemberDeclarationNode& node)
{
    if (node.DeclSpecifiers())
    {
        node.DeclSpecifiers()->Accept(*this);
    }
    if (node.MemberDeclarators())
    {
        TypeSymbol* baseType = ResolveBaseType(&node);
        declarationList = ProcessMemberDeclaratorList(baseType, &node, node.MemberDeclarators(), flags, context);
    }
}

void DeclarationProcessor::Visit(otava::ast::NoDeclSpecFunctionDeclarationNode& node)
{
    declarationList->declarations.push_back(ProcessDeclarator(nullptr, &node, &node, flags, FunctionQualifiers::none, context));
}

void DeclarationProcessor::Visit(otava::ast::FunctionDefinitionNode& node)
{
    if (node.DeclSpecifiers())
    {
        node.DeclSpecifiers()->Accept(*this);
    }
    TypeSymbol* baseType = ResolveBaseType(&node);
    if (!baseType)
    {
        return;
    }
    while (baseType->IsAliasTypeSymbol())
    {
        AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(baseType);
        baseType = aliasType->ReferredType(context);
    }
    FunctionQualifiers qualifiers = GetQualifiers(node.Specifiers());
    Declaration declaration = ProcessDeclarator(baseType, node.Declarator(), &node, flags, qualifiers, context);
    if (node.FunctionBody() && node.FunctionBody()->Kind() == otava::ast::NodeKind::defaultedOrDeletedFunctionNode)
    {
        if (declaration.declarator->IsFunctionDeclarator())
        {
            FunctionDeclarator* functionDeclarator = static_cast<FunctionDeclarator*>(declaration.declarator.get());
            otava::ast::DefaultedOrDeletedFunctionNode* ddNode = static_cast<otava::ast::DefaultedOrDeletedFunctionNode*>(node.FunctionBody());
            switch (ddNode->DefaultOrDelete()->Kind())
            {
            case otava::ast::NodeKind::defaultNode:
            {
                functionDeclarator->AddQualifier(FunctionQualifiers::isDefault);
                break;
            }
            case otava::ast::NodeKind::deleteNode:
            {
                functionDeclarator->AddQualifier(FunctionQualifiers::isDeleted);
                break;
            }
            }
        }
    }
    declarationList->declarations.push_back(std::move(declaration));
}

void DeclarationProcessor::Visit(otava::ast::ParameterNode& node)
{
    node.DeclSpecifiers()->Accept(*this);
    TypeSymbol* baseType = ResolveBaseType(&node);
    Declaration declaration = ProcessDeclarator(baseType, node.Declarator(), &node, flags, FunctionQualifiers::none, context);
    if (node.Initializer())
    {
        declaration.value = Evaluate(node.Initializer(), context);
        declaration.initializer = node.Initializer();
    }
    declarationList->declarations.push_back(std::move(declaration));
}

void DeclarationProcessor::Visit(otava::ast::ExceptionDeclarationNode& node)
{
    if (node.Ellipsis())
    {
        Declaration emptyDeclaration;
        declarationList->declarations.push_back(std::move(emptyDeclaration));
    }
    else
    {
        node.TypeSpecifiers()->Accept(*this);
        TypeSymbol* baseType = ResolveBaseType(&node);
        Declaration declaration = ProcessDeclarator(baseType, node.Declarator(), &node, flags, FunctionQualifiers::none, context);
        declarationList->declarations.push_back(std::move(declaration));
    }
}

void DeclarationProcessor::Visit(otava::ast::ClassSpecifierNode& node)
{
}

void DeclarationProcessor::Visit(otava::ast::EnumSpecifierNode& node)
{
}

void DeclarationProcessor::Visit(otava::ast::ElaboratedTypeSpecifierNode& node)
{
    type = ResolveType(node.GetId(), DeclarationFlags::none, context, TypeResolverFlags::dontThrow);
}

void DeclarationProcessor::Visit(otava::ast::InitConditionNode& node)
{
    node.DeclSpecifiers()->Accept(*this);
    TypeSymbol* baseType = ResolveBaseType(node.DeclSpecifiers());
    FunctionQualifiers qualifiers = FunctionQualifiers::none;
    Declaration declaration = ProcessDeclarator(baseType, node.Declarator(), nullptr, flags, qualifiers, context);
    declarationList->declarations.push_back(std::move(declaration));
}

void DeclarationProcessor::Visit(otava::ast::CharNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::charFlag, "char", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::charFlag;
}

void DeclarationProcessor::Visit(otava::ast::Char8Node& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::char8Flag, "char8_t", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::char8Flag;
}

void DeclarationProcessor::Visit(otava::ast::Char16Node& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::char16Flag, "char16_t", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::char16Flag;
}

void DeclarationProcessor::Visit(otava::ast::Char32Node& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::char32Flag, "char32_t", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::char32Flag;
}

void DeclarationProcessor::Visit(otava::ast::WCharNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::wcharFlag, "wchar_t", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::wcharFlag;
}

void DeclarationProcessor::Visit(otava::ast::BoolNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::wcharFlag, "bool", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::boolFlag;
}

void DeclarationProcessor::Visit(otava::ast::ShortNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::shortFlag, "short", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::shortFlag;
}

void DeclarationProcessor::Visit(otava::ast::IntNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::intFlag, "int", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::intFlag;
}

void DeclarationProcessor::Visit(otava::ast::LongNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::longLongFlag, "long long", node.GetFullSpan(), context);
    if ((flags & DeclarationFlags::longFlag) != DeclarationFlags::none)
    {
        flags = (flags | DeclarationFlags::longLongFlag) & ~DeclarationFlags::longFlag;
    }
    else
    {
        flags = flags | DeclarationFlags::longFlag;
    }
}

void DeclarationProcessor::Visit(otava::ast::SignedNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::signedFlag, "signed", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::signedFlag;
}

void DeclarationProcessor::Visit(otava::ast::UnsignedNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::unsignedFlag, "unsigned", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::unsignedFlag;
}

void DeclarationProcessor::Visit(otava::ast::FloatNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::floatFlag, "float", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::floatFlag;
}

void DeclarationProcessor::Visit(otava::ast::DoubleNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::doubleFlag, "double", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::doubleFlag;
}

void DeclarationProcessor::Visit(otava::ast::VoidNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::voidFlag, "void", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::voidFlag;
}

void DeclarationProcessor::Visit(otava::ast::ConstNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::constFlag, "const", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::constFlag;
}

void DeclarationProcessor::Visit(otava::ast::VolatileNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::volatileFlag, "volatile", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::volatileFlag;
}

void DeclarationProcessor::Visit(otava::ast::PlaceholderTypeSpecifierNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::autoFlag, "auto", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::autoFlag;
}

void DeclarationProcessor::Visit(otava::ast::FriendNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::friendFlag, "friend", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::friendFlag;
}

void DeclarationProcessor::Visit(otava::ast::TypedefNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::typedefFlag, "typedef", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::typedefFlag;
}

void DeclarationProcessor::Visit(otava::ast::ConstExprNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::constExprFlag, "constexpr", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::constExprFlag;
}

void DeclarationProcessor::Visit(otava::ast::ConstEvalNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::constEvalFlag, "consteval", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::constEvalFlag;
}

void DeclarationProcessor::Visit(otava::ast::ConstInitNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::constInitFlag, "constinit", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::constInitFlag;
}

void DeclarationProcessor::Visit(otava::ast::InlineNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::inlineFlag, "inline", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::inlineFlag;
}

void DeclarationProcessor::Visit(otava::ast::StaticNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::staticFlag, "static", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::staticFlag;
}

void DeclarationProcessor::Visit(otava::ast::ThreadLocalNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::threadLocalFlag, "thread_local", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::threadLocalFlag;
}

void DeclarationProcessor::Visit(otava::ast::ExternNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::externFlag, "extern", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::externFlag;
}

void DeclarationProcessor::Visit(otava::ast::VirtualNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::externFlag, "virtual", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::virtualFlag;
}

void DeclarationProcessor::Visit(otava::ast::ExplicitNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::explicitFlag, "explicit", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::explicitFlag;
}

void DeclarationProcessor::Visit(otava::ast::QualifiedIdNode& node)
{
    type = ResolveType(&node, flags, context);
}

void DeclarationProcessor::Visit(otava::ast::IdentifierNode& node)
{
    type = ResolveType(&node, flags, context);
}

void DeclarationProcessor::Visit(otava::ast::TemplateIdNode& node)
{
    type = ResolveType(&node, flags, context);
}

void DeclarationProcessor::Visit(otava::ast::TypenameSpecifierNode& node)
{
    type = ResolveType(&node, flags, context);
}

VariableSymbol* ProcessSimpleDeclarator(SimpleDeclarator* simpleDeclarator, TypeSymbol* type, TypeSymbol* initializerType,
    Value* value, DeclarationFlags flags, Context* context)
{
    VariableSymbol* variable = context->GetSymbolTable()->AddVariable(simpleDeclarator->Name(), simpleDeclarator->Node(), type, initializerType, value, flags, context);
    return variable;
}

VariableSymbol* ProcessArrayDeclarator(ArrayDeclarator* arrayDeclarator, TypeSymbol* elementType, DeclarationFlags flags, Context* context)
{
    if (arrayDeclarator->Size() == -1)
    {
        otava::ast::Node* node = arrayDeclarator->Node();
        arrayDeclarator->SetSize(GetSizeFromInitializer(node));
    }
    ArrayTypeSymbol* arrayTypeSymbol = context->GetSymbolTable()->MakeArrayType(elementType, arrayDeclarator->Size(), arrayDeclarator->Node());
    arrayTypeSymbol->Bind(arrayDeclarator->Node()->GetFullSpan(), context);
    VariableSymbol* variable = context->GetSymbolTable()->AddVariable(arrayDeclarator->Name(), arrayDeclarator->Node(), arrayTypeSymbol, nullptr, nullptr, flags, context);
    return variable;
}

void ProcessFunctionDeclarator(FunctionDeclarator* functionDeclarator, TypeSymbol* type, DeclarationFlags flags, otava::ast::Node* functionNode, Context* context)
{
/*
    FunctionSymbol* functionSymbol = context->GetSymbolTable()->AddFunction(
        functionDeclarator->Name(),
        functionDeclarator->TemplateArgs(),
        functionDeclarator->Node(),
        functionDeclarator->GetFunctionKind(),
        functionDeclarator->GetFunctionQualifiers(),
        flags,
        context);
    context->SetSpecialization(functionSymbol, functionNode);
    TypeSymbol* returnType = MapType(functionSymbol, type, context);
    functionSymbol->SetReturnType(returnType, context);
    for (const auto& parameterDeclaration : functionDeclarator->ParameterDeclarations())
    {
        soul::ast::FullSpan fullSpan;
        std::u32string name;
        otava::ast::Node* node = nullptr;
        if (parameterDeclaration.declarator)
        {
            name = parameterDeclaration.declarator->Name();
            node = parameterDeclaration.declarator->Node();
            fullSpan = parameterDeclaration.declarator->Node()->GetFullSpan();
        }
        TypeSymbol* parameterType = MapType(functionSymbol, parameterDeclaration.type, context);
        ParameterSymbol* parameter = context->GetSymbolTable()->CreateParameter(name, node, parameterType, context);
        if (parameterDeclaration.initializer)
        {
            parameter->SetDefaultValue(parameterDeclaration.initializer);
        }
        functionSymbol->AddParameter(parameter, fullSpan, context);
    }
    ClassTypeSymbol* classType = functionSymbol->ParentClassType();
    if (classType)
    {
        std::int32_t functionIndex = 0;
        SpecialFunctionKind specialFunctionKind = functionSymbol->GetSpecialFunctionKind(context);
        if (specialFunctionKind != SpecialFunctionKind::none)
        {
            functionIndex = GetSpecialFunctionIndex(specialFunctionKind);
            if (specialFunctionKind == SpecialFunctionKind::dtor)
            {
                classType->SetHasUserDefinedDestructor();
            }
        }
        else
        {
            functionIndex = classType->NextFunctionIndex();
        }
        functionSymbol->SetIndex(functionIndex);
        classType->MapFunction(functionSymbol);
        context->GetSymbolTable()->MapFunction(functionSymbol);
    }
    if (functionSymbol->IsExplicitSpecializationDeclaration())
    {
        std::map<TemplateParameterSymbol*, TypeSymbol*, TemplateParamLess> templateParameterMap;
        InstantiateFunctionTemplate(functionSymbol, templateParameterMap, functionDeclarator->Node()->GetFullSpan(), context);
    }
    AddConvertingConstructorToConversionTable(functionSymbol, functionDeclarator->Node()->GetFullSpan(), context);
    if (!(context->GetFlag(ContextFlags::instantiateMemFnOfClassTemplate | ContextFlags::instantiateFunctionTemplate | ContextFlags::instantiateInlineFunction)))
    {
        functionSymbol->SetFlag(FunctionSymbolFlags::fixedIrName);
        functionSymbol->IrName(context);
    }
*/
}

VariableSymbol* ResolveParentVariable(SimpleDeclarator* simpleDeclarator, const soul::ast::FullSpan& fullSpan, Context* context)
{
    int blockId = context->ParentBlockId();
    int level = 0;
    FunctionDefinitionSymbol* fnDefSymbol = context->ParentFn();
    if (fnDefSymbol)
    {
        while (fnDefSymbol)
        {
            Symbol* block = fnDefSymbol->GetBlock(blockId);
            if (block)
            {
                Scope* blockScope = block->GetScope();
                if (blockScope)
                {
                    Symbol* symbol = blockScope->Lookup(simpleDeclarator->Name(),
                        SymbolGroupKind::variableSymbolGroup, ScopeLookup::thisAndBaseAndParentScope, fullSpan, context, LookupFlags::none);
                    if (symbol && symbol->IsVariableSymbol())
                    {
                        VariableSymbol* variableSymbol = static_cast<VariableSymbol*>(symbol);
                        variableSymbol->SetFoundFromParent();
                        variableSymbol->SetLevel(level);
                        return variableSymbol;
                    }
                    else
                    {
                        fnDefSymbol = fnDefSymbol->ParentFn();
                        ++level;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    ThrowException("parent function variable '" + simpleDeclarator->Name() + "' not resolved", fullSpan, context);
    return nullptr;
}

void ProcessSimpleDeclaration(otava::ast::Node* node, otava::ast::Node* functionNode, Context* context)
{
/*
    if (context->GetFlag(ContextFlags::linkageDeclaration)) return;
    if (context->GetFlag(ContextFlags::dontProcess)) return;
    DeclarationProcessor processor(context);
    node->Accept(processor);
    std::unique_ptr<DeclarationList> declarationList = processor.GetDeclarations();
    for (auto& declaration : declarationList->declarations)
    {
        Declarator* declarator = declaration.declarator.get();
        switch (declarator->Kind())
        {
        case DeclaratorKind::simpleDeclarator:
        {
            SimpleDeclarator* simpleDeclarator = static_cast<SimpleDeclarator*>(declarator);
            TypeSymbol* initializerType = nullptr;
            if (!simpleDeclarator->Name().empty())
            {
                VariableSymbol* variable = nullptr;
                if (context->GetFlag(ContextFlags::invoke | ContextFlags::tryCatch))
                {
                    variable = ResolveParentVariable(simpleDeclarator, node->GetFullSpan(), context);
                }
                else
                {
                    variable = ProcessSimpleDeclarator(simpleDeclarator, declaration.type, initializerType, declaration.value, declaration.flags, context);
                    if (variable->IsGlobalVariable())
                    {
                        std::unique_ptr<BoundExpressionNode> variableInitializer(nullptr);
                        if (declaration.initializer)
                        {
                            variableInitializer = BindExpression(declaration.initializer, context);
                        }
                        if (!variable->IsExtern())
                        {
                            context->GetBoundCompileUnit()->AddBoundNode(
                                std::unique_ptr<BoundNode>(new BoundGlobalVariableDefinitionNode(variable, node->GetFullSpan())), context);
                            GenerateDynamicInitialization(variable, variableInitializer.get(), node->GetFullSpan(), context);
                        }
                    }
                }
                declaration.variable = variable;
            }
            break;
        }
        case DeclaratorKind::functionDeclarator:
        {
            FunctionDeclarator* functionDeclarator = static_cast<FunctionDeclarator*>(declarator);
            ProcessFunctionDeclarator(functionDeclarator, declaration.type, declaration.flags, functionNode, context);
            break;
        }
        case DeclaratorKind::arrayDeclarator:
        {
            ArrayDeclarator* arrayDeclarator = static_cast<ArrayDeclarator*>(declarator);
            ArrayTypeSymbol* arrayType = static_cast<ArrayTypeSymbol*>(declaration.type);
            VariableSymbol* variable = ProcessArrayDeclarator(arrayDeclarator, arrayType->ElementType(), declaration.flags, context);
            if (declaration.value)
            {
                variable->SetValue(declaration.value);
                variable->SetDeclaredType(declaration.value->GetType());
            }
            if (variable->IsGlobalVariable())
            {
                context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(
                    new BoundGlobalVariableDefinitionNode(variable, node->GetFullSpan())), context);
            }
            declaration.variable = variable;
            break;
        }
        }
    }
    if (context->GetFlag(ContextFlags::saveDeclarations))
    {
        context->SetDeclarationList(node, declarationList.release());
    }
*/
}

Declaration ProcessParameterDeclaration(otava::ast::Node* node, Context* context)
{
    DeclarationProcessor processor(context);
    node->Accept(processor);
    std::unique_ptr<DeclarationList> declarationList = processor.GetDeclarations();
    if (declarationList->declarations.size() == 1)
    {
        return std::move(declarationList->declarations.front());
    }
    else
    {
        SetExceptionThrown();
        throw std::runtime_error("otava.symbols.declaration: single declaration expected");
    }
}

Declaration ProcessExceptionDeclaration(otava::ast::Node* node, Context* context)
{
    DeclarationProcessor processor(context);
    node->Accept(processor);
    std::unique_ptr<DeclarationList> declarationList = processor.GetDeclarations();
    if (declarationList->declarations.size() == 1)
    {
        return std::move(declarationList->declarations.front());
    }
    else
    {
        SetExceptionThrown();
        throw std::runtime_error("otava.symbols.declaration: single declaration expected");
    }
}

Declaration ProcessInitCondition(otava::ast::InitConditionNode* initCondition, Context* context)
{
    DeclarationProcessor processor(context);
    initCondition->Accept(processor);
    std::unique_ptr<DeclarationList> declarationList = processor.GetDeclarations();
    if (declarationList->declarations.size() == 1)
    {
        return std::move(declarationList->declarations.front());
    }
    else
    {
        SetExceptionThrown();
        throw std::runtime_error("otava.symbols.declaration: single declaration expected");
    }
}

void ProcessMemberDeclaration(otava::ast::Node* node, otava::ast::Node* functionNode, Context* context)
{
    context->PushResetFlag(ContextFlags::invoke | ContextFlags::tryCatch);
    ProcessSimpleDeclaration(node, functionNode, context);
    context->PopFlags();
}

Declaration ProcessFunctionDeclaration(otava::ast::Node* node, Context* context)
{
    DeclarationProcessor processor(context);
    node->Accept(processor);
    std::unique_ptr<DeclarationList> declarationList = processor.GetDeclarations();
    if (declarationList->declarations.size() == 1)
    {
        return std::move(declarationList->declarations.front());
    }
    else
    {
        SetExceptionThrown();
        throw std::runtime_error("otava.symbols.declaration: single declaration expected");
    }
}

bool IsTokenSwitchDeclarator(otava::ast::Node* declarator)
{
    if (declarator->IsFunctionDeclaratorNode())
    {
        otava::ast::FunctionDeclaratorNode* fnd = static_cast<otava::ast::FunctionDeclaratorNode*>(declarator);
        if (fnd->Child()->Str() == "IsTokenSwitch")
        {
            return true;
        }
    }
    return false;
}

int BeginFunctionDefinition(otava::ast::Node* declSpecifierSequence, otava::ast::Node* declarator, otava::ast::Node* functionNode,
    otava::ast::Node* specifierNode, bool& get, Context* context)
{
/*
    get = false;
    int scopes = 0;
    if (!context->GetFlag(ContextFlags::instantiateFunctionTemplate |
        ContextFlags::instantiateMemFnOfClassTemplate |
        ContextFlags::instantiateInlineFunction))
    {
        context->GetSymbolTable()->CurrentScope()->PopParentScope();
    }
    DeclarationProcessor processor(context);
    processor.BeginProcessFunctionDefinition(declSpecifierSequence, declarator, specifierNode);
    std::unique_ptr<DeclarationList> declarationList = processor.GetDeclarations();
    if (declarationList->declarations.size() == 1)
    {
        Declaration declaration = std::move(declarationList->declarations.front());
        if (declaration.declarator->Kind() == DeclaratorKind::functionDeclarator)
        {
            FunctionDeclarator* functionDeclarator = static_cast<FunctionDeclarator*>(declaration.declarator.get());
            FunctionQualifiers qualifiers = functionDeclarator->GetFunctionQualifiers();
            FunctionKind kind = functionDeclarator->GetFunctionKind();
            std::vector<TypeSymbol*> parameterTypes;
            for (const auto& parameterDeclaration : functionDeclarator->ParameterDeclarations())
            {
                parameterTypes.push_back(parameterDeclaration.type);
            }
            FunctionDefinitionSymbol* definition = context->GetSymbolTable()->AddOrGetFunctionDefinition(
                functionDeclarator->GetScope(), functionDeclarator->Name(), functionDeclarator->TemplateArgs(), parameterTypes, qualifiers, kind,
                declaration.flags, declarator, functionNode, get, context);
            TypeSymbol* fnDeclarationReturnType = nullptr;
            FunctionSymbol* fnDeclaration = definition->Declaration();
            if (fnDeclaration)
            {
                if (fnDeclaration->IsInline())
                {
                    definition->SetInline();
                }
                fnDeclarationReturnType = fnDeclaration->ReturnType();
            }
            if (context->GetFlag(ContextFlags::instantiateFunctionTemplate |
                ContextFlags::instantiateMemFnOfClassTemplate |
                ContextFlags::instantiateInlineFunction |
                ContextFlags::generateMainWrapper))
            {
                context->SetSpecialization(definition, functionNode);
            }
            if (get)
            {
                return 0;
            }
            int parameterIndex = 0;
            for (const auto& parameterDeclaration : functionDeclarator->ParameterDeclarations())
            {
                soul::ast::FullSpan fullSpan;
                std::string name;
                otava::ast::Node* node = nullptr;
                if (parameterDeclaration.declarator)
                {
                    name = parameterDeclaration.declarator->Name();
                    node = parameterDeclaration.declarator->Node();
                    fullSpan = parameterDeclaration.declarator->Node()->GetFullSpan();
                }
                TypeSymbol* parameterType = MapType(definition, parameterDeclaration.type, context);
                ParameterSymbol* parameter = context->GetSymbolTable()->CreateParameter(name, node, parameterType, context);
                if (parameterDeclaration.initializer)
                {
                    parameter->SetDefaultValue(parameterDeclaration.initializer);
                }
                FunctionSymbol* declaration = definition->Declaration();
                if (declaration)
                {
                    if (parameterIndex >= 0 && parameterIndex < declaration->Parameters().size())
                    {
                        ParameterSymbol* declarationParam = declaration->Parameters()[parameterIndex];
                        if (declarationParam->DefaultValue() && !parameter->DefaultValue())
                        {
                            parameter->SetDefaultValue(declarationParam->DefaultValue());
                        }
                    }
                }
                definition->AddParameter(parameter, fullSpan, context);
                ++parameterIndex;
            }
            definition->AddDefinitionToGroup(context);
            TypeSymbol* returnType = MapType(definition, declaration.type, context);
            definition->SetReturnType(returnType, context);
            if (fnDeclarationReturnType && returnType && !TypesEqual(fnDeclarationReturnType, returnType, context))
            {
                std::set<const Symbol*> visited;
                bool isTemplateParameterInstantiation = fnDeclaration->IsTemplateParameterInstantiation(context, visited) ||
                    fnDeclarationReturnType->IsTemplateParameterInstantiation(context, visited) ||
                    definition->IsTemplateParameterInstantiation(context, visited) ||
                    returnType->IsTemplateParameterInstantiation(context, visited);
                if (!isTemplateParameterInstantiation)
                {
                    ThrowException("the return type of function '" + definition->FullName(context) +
                        "' definition differs from the return type of function declaration",
                        definition->GetFullSpan(), fnDeclaration->GetFullSpan(), context);
                }
            }
            context->GetSymbolTable()->BeginScopeGeneric(definition->GetScope(), context);
            if (!context->GetFlag(ContextFlags::instantiateFunctionTemplate |
                ContextFlags::instantiateMemFnOfClassTemplate |
                ContextFlags::instantiateInlineFunction))
            {
                definition->GetScope()->AddParentScope(functionDeclarator->GetScope());
            }
            ++scopes;
            BoundFunctionNode* boundFunctionNode = new BoundFunctionNode(definition, declarator->GetFullSpan());
            context->PushBoundFunction(boundFunctionNode);
        }
    }
    else
    {
        SetExceptionThrown();
        throw std::runtime_error("otava.symbols.declaration: single declaration expected");
    }
    return scopes;
*/
    return 0;
}

void EndFunctionDefinition(otava::ast::Node* node, int scopes, Context* context)
{
/*
    if (node->IsFunctionDefinitionNode())
    {
        otava::ast::FunctionDefinitionNode* functionDefinitionNode = static_cast<otava::ast::FunctionDefinitionNode*>(node);
        FunctionDefinitionSymbol* functionDefinitionSymbol = nullptr;
        Symbol* symbol = context->GetSymbolTable()->CurrentScope()->SymbolScope()->GetSymbol();
        if (symbol && symbol->IsFunctionDefinitionSymbol())
        {
            functionDefinitionSymbol = static_cast<FunctionDefinitionSymbol*>(symbol);
            if (HasNoReturnAttribute(functionDefinitionNode->Attributes()))
            {
                functionDefinitionSymbol->SetFunctionQualifiers(functionDefinitionSymbol->Qualifiers() | FunctionQualifiers::noreturn);
            }
            context->GetSymbolTable()->MapNode(functionDefinitionNode, functionDefinitionSymbol);
            ClassTypeSymbol* classType = functionDefinitionSymbol->ParentClassType();
            if (classType)
            {
                if (classType->IsClassTemplateSpecializationSymbol())
                {
                    std::set<const Symbol*> visited;
                    if (classType->IsTemplateParameterInstantiation(context, visited))
                    {
                        ClassTemplateSpecializationSymbol* specialiazation = static_cast<ClassTemplateSpecializationSymbol*>(classType);
                        classType = specialiazation->ClassTemplate();
                    }
                }
                std::int32_t functionIndex = 0;
                SpecialFunctionKind specialFunctionKind = functionDefinitionSymbol->GetSpecialFunctionKind(context);
                if (specialFunctionKind != SpecialFunctionKind::none)
                {
                    functionIndex = GetSpecialFunctionIndex(specialFunctionKind);
                }
                else
                {
                    functionIndex = classType->NextFunctionIndex();
                }
                functionDefinitionSymbol->SetIndex(functionIndex);
                functionDefinitionNode->SetIndex(functionIndex);
                if (!classType->GetFunctionByIndex(functionIndex))
                {
                    classType->MapFunction(functionDefinitionSymbol);
                }
                if (context->MemFunDefSymbolIndex() != -1 && functionDefinitionSymbol->DefIndex() == -1)
                {
                    functionDefinitionSymbol->SetDefIndex(context->MemFunDefSymbolIndex());
                    context->SetMemFunDefSymbolIndex(-1);
                }
                classType->SetMemFnDefSymbol(functionDefinitionSymbol);
                context->GetSymbolTable()->MapFunctionDefinition(functionDefinitionSymbol);
            }
        }
        for (int i = 0; i < scopes; ++i)
        {
            context->GetSymbolTable()->EndScopeGeneric(context);
        }
        if (!context->GetFlag(ContextFlags::dontBind) && functionDefinitionSymbol)
        {
            functionDefinitionSymbol = BindFunction(functionDefinitionNode, functionDefinitionSymbol, context);
        }
        if (functionDefinitionSymbol && functionDefinitionSymbol->IsTemplate())
        {
            InstantiateEnqueuedRequests(functionDefinitionSymbol, node->GetFullSpan(), context);
        }
        if (functionDefinitionSymbol && functionDefinitionSymbol->IsExplicitSpecializationDefinitionSymbol())
        {
            std::map<TemplateParameterSymbol*, TypeSymbol*, TemplateParamLess> templateParameterMap;
            InstantiateFunctionTemplate(functionDefinitionSymbol, templateParameterMap, node->GetFullSpan(), context);
        }
    }
    if (context->GetBoundFunction())
    {
        if (context->GetBoundFunction()->GetFunctionDefinitionSymbol()->IsBound())
        {
            context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(context->ReleaseBoundFunction()), context);
        }
        context->PopBoundFunction();
    }
*/
}

void ProcessMemberFunctionDefinition(otava::ast::Node* node, Context* context)
{
    ProcessSimpleDeclaration(node, nullptr, context);
}

void Write(Writer& writer, DeclarationFlags flags)
{
    writer.GetBinaryStreamWriter().Write(static_cast<std::int32_t>(flags));
}

void Read(Reader& reader, DeclarationFlags& flags)
{
    //flags = static_cast<DeclarationFlags>(reader.GetBinaryStreamReader().ReadInt()); TODO
}

void ThrowDeclarationParsingError(const soul::ast::FullSpan& fullSpan, Context* context)
{
    ThrowException("declaration parsing error", fullSpan, context);
}

class LinkageProcessor : public otava::ast::DefaultVisitor
{
public:
    LinkageProcessor(Context* context_);
    void Visit(otava::ast::LinkageSpecificationNode& node) override;
    void Visit(otava::ast::StringLiteralNode& node) override;
    void Visit(otava::ast::SimpleDeclarationNode& node) override;
private:
    Context* context;
    Linkage linkage;
};

LinkageProcessor::LinkageProcessor(Context* context_) : context(context_), linkage(Linkage::cpp_linkage)
{
}

void LinkageProcessor::Visit(otava::ast::LinkageSpecificationNode& node)
{
    node.Linkage()->Accept(*this);
    context->GetSymbolTable()->PushLinkage(linkage);
    node.Declarations()->Accept(*this);
    context->GetSymbolTable()->PopLinkage();
}

void LinkageProcessor::Visit(otava::ast::StringLiteralNode& node)
{
    if (node.GetValue() == "C")
    {
        linkage = Linkage::c_linkage;
    }
    else if (node.GetValue() == "C++")
    {
        linkage = Linkage::cpp_linkage;
    }
    else
    {
        ThrowException("unknown linkage", node.GetFullSpan(), context);
    }
}

void LinkageProcessor::Visit(otava::ast::SimpleDeclarationNode& node)
{
    ProcessSimpleDeclaration(&node, nullptr, context);
}

void ProcessLinkageSpecification(otava::ast::Node* node, Context* context)
{
    LinkageProcessor linkageProcessor(context);
    node->Accept(linkageProcessor);
}

TypeSymbol* ProcessExplicitInstantiationDeclaration(otava::ast::Node* node, Context* context)
{
    DeclarationProcessor processor(context);
    node->Accept(processor);
    return processor.GetType();
}

TypeSymbol* MapType(FunctionSymbol* functionSymbol, TypeSymbol* type, Context* context)
{
    if (type)
    {
        ClassTypeSymbol* parentClassType = functionSymbol->ParentClassType(context);
        if (parentClassType && parentClassType->IsClassTemplateSpecializationSymbol())
        {
            ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(parentClassType);
            if (type->GetBaseType()->IsClassTypeSymbol())
            {
                if (TypesEqual(type->GetBaseType(), specialization->ClassTemplate(), context))
                {
                    type = context->GetSymbolTable()->MakeCompoundType(specialization, type->GetDerivations(), context);
                }
            }
        }
    }
    return type;
}

void GenerateDynamicInitialization(VariableSymbol* variable, BoundExpressionNode* initializer, const soul::ast::FullSpan& fullSpan, Context* context)
{
/*
    if (!variable->GetType()->IsClassTypeSymbol()) return;
    if (context->GetFlag(ContextFlags::noDynamicInit)) return;
    bool prevInternallyMapped = context->GetModule()->GetNodeIdFactory()->IsInternallyMapped();
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(true);
    BoundVariableNode* boundGlobalVariable = new BoundVariableNode(variable, fullSpan);
    std::vector<std::unique_ptr<BoundExpressionNode>> args;
    args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundGlobalVariable, fullSpan, variable->GetType()->AddPointer(context))));
    if (initializer)
    {
        if (initializer->IsBoundExpressionListNode())
        {
            BoundExpressionListNode* expressionList = static_cast<BoundExpressionListNode*>(initializer);
            int n = expressionList->Count();
            for (int i = 0; i < n; ++i)
            {
                args.push_back(std::unique_ptr<BoundExpressionNode>(expressionList->ReleaseExpr(i)));
            }
        }
        else
        {
            args.push_back(std::unique_ptr<BoundExpressionNode>(initializer->Clone()));
        }
    }
    Exception ex;
    std::vector<TypeSymbol*> templateArgs;
    std::unique_ptr<BoundFunctionCallNode> constructorCall = ResolveOverload(
        context->GetSymbolTable()->CurrentScope(), U"@constructor", templateArgs, args, fullSpan, context, ex);
    if (constructorCall)
    {
        std::unique_ptr<BoundFunctionCallNode> atExitCall = MakeAtExitForVariable(variable, fullSpan, context);
        context->GetBoundCompileUnit()->AddDynamicInitialization(constructorCall.release(), atExitCall.release(), fullSpan, context);
    }
    context->GetModule()->GetNodeIdFactory()->SetInternallyMapped(prevInternallyMapped);
*/
}

std::unique_ptr<BoundFunctionCallNode> MakeAtExitForVariable(VariableSymbol* variable, const soul::ast::FullSpan& fullSpan, Context* context)
{
/*
    std::vector<std::unique_ptr<BoundExpressionNode>> dtorArgs;
    BoundVariableNode* boundGlobalVariable = new BoundVariableNode(variable, fullSpan);
    dtorArgs.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundGlobalVariable, fullSpan, variable->GetType()->AddPointer(context))));
    Exception ex;
    std::vector<TypeSymbol*> templateArgs;
    std::unique_ptr<BoundFunctionCallNode> destructorCall = ResolveOverload(
        context->GetSymbolTable()->CurrentScope(), U"@destructor", templateArgs, dtorArgs, fullSpan, context, ex);
    std::unique_ptr<BoundFunctionCallNode> atExitCall;
    if (destructorCall && !destructorCall->GetFunctionSymbol()->IsTrivialDestructor())
    {
        TypeSymbol* voidPtrType = context->GetSymbolTable()->GetFundamentalType(FundamentalTypeKind::voidType)->AddPointer(context);
        std::vector<std::unique_ptr<BoundExpressionNode>> atExitArgs;
        atExitArgs.push_back(std::unique_ptr<BoundExpressionNode>(new BoundFunctionValueNode(destructorCall->GetFunctionSymbol(), fullSpan, voidPtrType)));
        BoundVariableNode* boundGlobalVariable = new BoundVariableNode(variable, fullSpan);
        atExitArgs.push_back(std::unique_ptr<BoundExpressionNode>(new BoundVariableAsVoidPtrNode(new BoundAddressOfNode(
            boundGlobalVariable, fullSpan, boundGlobalVariable->GetType()->AddPointer(context)), fullSpan, voidPtrType)));
        Exception ex;
        Scope* stdScope = context->GetSymbolTable()->GetNamespaceScope(U"std", fullSpan, context);
        std::vector<TypeSymbol*> templateArgs;
        atExitCall = ResolveOverload(stdScope, U"at_exit", templateArgs, atExitArgs, fullSpan, context, ex);
    }
    return atExitCall;
*/
    return std::unique_ptr<BoundFunctionCallNode>();
}

void AddConvertingConstructorToConversionTable(FunctionSymbol* functionSymbol, const soul::ast::FullSpan& fullSpan, Context* context)
{
/*
    if (!functionSymbol->IsExplicit() && functionSymbol->GetFunctionKind() == FunctionKind::constructor && functionSymbol->MemFunArity(context) == 2)
    {
        TypeSymbol* conversionParamType = functionSymbol->MemFunParameters(context)[0]->GetType()->RemovePointer(context)->DirectType(
            context)->FinalType(fullSpan, context);
        TypeSymbol* conversionArgType = functionSymbol->MemFunParameters(context)[1]->GetType()->PlainType(context)->DirectType(context)->FinalType(fullSpan, context);
        if (!TypesEqual(conversionParamType, conversionArgType, context))
        {
            FunctionSymbol* conversion = context->GetSymbolTable()->GetConversionTable().GetConversion(conversionParamType, conversionArgType, context);
            if (!conversion)
            {
                functionSymbol->SetConversion();
                functionSymbol->SetConversionParamType(conversionParamType);
                functionSymbol->SetConversionArgType(conversionArgType);
                functionSymbol->SetConversionDistance(10);
                context->GetSymbolTable()->GetConversionTable().AddConversion(functionSymbol);
            }
        }
    }
*/
}

} // namespace otava::symbol
