// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.type_resolver;

import otava.symbols.exception;
import otava.symbols.bound_tree;
import otava.symbols.context;
import otava.symbols.declaration;
import otava.symbols.declarator;
import otava.symbols.derivations;
import otava.symbols.expression_binder;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.scope_resolver;
import otava.ast.declaration;
import otava.ast.expression;
import otava.ast.identifier;
import otava.ast.function;
import otava.ast.qualifier;
import otava.ast.simple_type;
import otava.ast.templates;
import otava.ast.type;
import otava.ast.visitor;

namespace otava::symbols {

void CheckDuplicateSpecifier(DeclarationFlags flags, DeclarationFlags flag, const std::string& specifierStr, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if ((flags & flag) != DeclarationFlags::none)
    {
        ThrowException("duplicate '" + specifierStr + "'", fullSpan, context);
    }
}

class TypeResolver : public otava::ast::DefaultVisitor
{
public:
    TypeResolver(Context* context_, const soul::ast::FullSpan& fullSpan_, DeclarationFlags flags_, TypeResolverFlags resolverFlags_);
    TypeSymbol* GetType();
    void ResolveBaseType(otava::ast::Node* node);
    void ResolveType();
    void Visit(otava::ast::DefiningTypeIdNode& node) override;
    void Visit(otava::ast::TypeSpecifierSequenceNode& node) override;
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
    void Visit(otava::ast::LvalueRefNode& node) override;
    void Visit(otava::ast::RvalueRefNode& node) override;
    void Visit(otava::ast::PtrNode& node) override;
    void Visit(otava::ast::TypenameSpecifierNode& node) override;
    void Visit(otava::ast::DeclTypeSpecifierNode& node) override;
    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::TemplateIdNode& node) override;
    void Visit(otava::ast::TypeIdNode& node) override;
    void Visit(otava::ast::FunctionDeclaratorNode& node) override;
    void Visit(otava::ast::NewTypeIdNode& node) override;
    void Visit(otava::ast::ArrayNewDeclaratorNode& node) override;
private:
    Context* context;
    TypeSymbol* type;
    TypeSymbol* baseType;
    DeclarationFlags flags;
    TypeResolverFlags resolverFlags;
    int pointerCount;
    bool typeResolved;
    bool createTypeSymbol;
    std::unique_ptr<BoundExpressionNode> size;
    soul::ast::FullSpan fullSpan;
};

TypeResolver::TypeResolver(Context* context_, const soul::ast::FullSpan& fullSpan_, DeclarationFlags flags_, TypeResolverFlags resolverFlags_) :
    context(context_),
    type(nullptr),
    baseType(nullptr),
    flags(flags_),
    resolverFlags(resolverFlags_),
    pointerCount(0),
    typeResolved(false),
    createTypeSymbol(false),
    size(),
    fullSpan(fullSpan_)
{
}

TypeSymbol* TypeResolver::GetType()
{
    ResolveType();
    return type;
}

void TypeResolver::ResolveBaseType(otava::ast::Node* node)
{
    DeclarationFlags fundamentalTypeFlags = flags & DeclarationFlags::fundamentalTypeFlags;
    if (fundamentalTypeFlags != DeclarationFlags::none)
    {
        if (baseType)
        {
            ThrowException("duplicate type symbol in declaration specifier sequence", node->GetFullSpan(), fullSpan, context);
        }
        baseType = GetFundamentalType(fundamentalTypeFlags, node->GetFullSpan(), context);
    }
}

void TypeResolver::ResolveType()
{
    if (typeResolved) return;
    typeResolved = true;
    Derivations derivations = Derivations::none;
    if ((flags & DeclarationFlags::constFlag) != DeclarationFlags::none)
    {
        derivations = derivations | Derivations::constDerivation;
    }
    if ((flags & DeclarationFlags::volatileFlag) != DeclarationFlags::none)
    {
        derivations = derivations | Derivations::volatileDerivation;
    }
    if (pointerCount > 0)
    {
        derivations = otava::symbols::SetPointerCount(derivations, pointerCount);
    }
    if ((flags & DeclarationFlags::lvalueRefFlag) != DeclarationFlags::none)
    {
        derivations = derivations | Derivations::lvalueRefDerivation;
    }
    else if ((flags & DeclarationFlags::rvalueRefFlag) != DeclarationFlags::none)
    {
        derivations = derivations | Derivations::rvalueRefDerivation;
    }
    if (derivations != Derivations::none && type)
    {
        type = context->GetSymbolTable()->MakeCompoundType(type, derivations, context);
    }
}

void TypeResolver::Visit(otava::ast::DefiningTypeIdNode& node)
{
    node.DefiningTypeSpecifiers()->Accept(*this);
    if (!type)
    {
        ResolveBaseType(&node);
        type = baseType;
    }
    node.AbstractDeclarator()->Accept(*this);
    ResolveType();
}

void TypeResolver::Visit(otava::ast::TypeSpecifierSequenceNode& node)
{
    VisitSequenceContent(node);
    if (!type)
    {
        ResolveBaseType(&node);
        type = baseType;
    }
    ResolveType();
}

void TypeResolver::Visit(otava::ast::CharNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::charFlag, "char", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::charFlag;
}

void TypeResolver::Visit(otava::ast::Char8Node& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::char8Flag, "char8_t", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::char8Flag;
}

void TypeResolver::Visit(otava::ast::Char16Node& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::char16Flag, "char16_t", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::char16Flag;
}

void TypeResolver::Visit(otava::ast::Char32Node& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::char32Flag, "char32_t", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::char32Flag;
}

void TypeResolver::Visit(otava::ast::WCharNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::wcharFlag, "wchar_t", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::wcharFlag;
}

void TypeResolver::Visit(otava::ast::BoolNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::wcharFlag, "bool", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::boolFlag;
}

void TypeResolver::Visit(otava::ast::ShortNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::shortFlag, "short", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::shortFlag;
}

void TypeResolver::Visit(otava::ast::IntNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::intFlag, "int", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::intFlag;
}

void TypeResolver::Visit(otava::ast::LongNode& node)
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

void TypeResolver::Visit(otava::ast::SignedNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::signedFlag, "signed", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::signedFlag;
}

void TypeResolver::Visit(otava::ast::UnsignedNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::unsignedFlag, "unsigned", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::unsignedFlag;
}

void TypeResolver::Visit(otava::ast::FloatNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::floatFlag, "float", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::floatFlag;
}

void TypeResolver::Visit(otava::ast::DoubleNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::doubleFlag, "double", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::doubleFlag;
}

void TypeResolver::Visit(otava::ast::VoidNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::voidFlag, "void", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::voidFlag;
}

void TypeResolver::Visit(otava::ast::ConstNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::constFlag, "const", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::constFlag;
}

void TypeResolver::Visit(otava::ast::VolatileNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::volatileFlag, "volatile", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::volatileFlag;
}

void TypeResolver::Visit(otava::ast::LvalueRefNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::lvalueRefFlag, "&", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::lvalueRefFlag;
}

void TypeResolver::Visit(otava::ast::RvalueRefNode& node)
{
    CheckDuplicateSpecifier(flags, DeclarationFlags::rvalueRefFlag, "&&", node.GetFullSpan(), context);
    flags = flags | DeclarationFlags::rvalueRefFlag;
}

void TypeResolver::Visit(otava::ast::PtrNode& node)
{
    ++pointerCount;
}

void TypeResolver::Visit(otava::ast::TypenameSpecifierNode& node)
{
/*
    if (context->GetFlag(ContextFlags::processingAliasDeclation))
    {
        context->GetSymbolTable()->PushTopScopeIndex();
        BeginScope(node.NestedNameSpecifier(), context);
        createTypeSymbol = true;
        node.GetId()->Accept(*this);
        createTypeSymbol = false;
        EndScope(context);
        context->GetSymbolTable()->PopTopScopeIndex();
    }
    else
    {
        if (context->GetFlag(ContextFlags::parsingTemplateDeclaration))
        {
            type = context->GetSymbolTable()->MakeDependentTypeSymbol(node.Clone());
        }
        else
        {
            InstantiationScope instantiationScope(context->GetSymbolTable()->CurrentScope());
            std::vector<std::unique_ptr<BoundTemplateParameterSymbol>> boundTemplateParameters;
            if (context->TemplateParameterMap())
            {
                for (const auto& templateParamType : *context->TemplateParameterMap())
                {
                    TemplateParameterSymbol* templateParameter = templateParamType.first;
                    BoundTemplateParameterSymbol* boundTemplateParameter = new BoundTemplateParameterSymbol(templateParameter->Name());
                    boundTemplateParameter->SetTemplateParameterSymbol(templateParameter);
                    boundTemplateParameter->SetBoundSymbol(templateParamType.second);
                    boundTemplateParameters.push_back(std::unique_ptr<BoundTemplateParameterSymbol>(boundTemplateParameter));
                    instantiationScope.Install(boundTemplateParameter, context);
                }
            }
            context->GetSymbolTable()->PushTopScopeIndex();
            context->GetSymbolTable()->BeginScope(&instantiationScope);
            BeginScope(node.NestedNameSpecifier(), context);
            node.GetId()->Accept(*this);
            EndScope(context);
            context->GetSymbolTable()->EndScope();
            context->GetSymbolTable()->PopTopScopeIndex();
        }
    }
*/
}

void TypeResolver::Visit(otava::ast::DeclTypeSpecifierNode& node)
{
    std::unique_ptr<BoundExpressionNode> expr(BindExpression(node.Expression(), context));
    if (!expr->GetType())
    {
        ThrowException("type for decltype specifier not resolved", node.GetFullSpan(), context);
    }
    type = expr->GetType()->PlainType(context);
}

void TypeResolver::Visit(otava::ast::QualifiedIdNode& node)
{
    context->GetSymbolTable()->PushTopScopeIndex();
    BeginScope(node.Left(), context);
    node.Right()->Accept(*this);
    EndScope(context);
    context->GetSymbolTable()->PopTopScopeIndex();
}

void TypeResolver::Visit(otava::ast::IdentifierNode& node)
{
/*
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    Symbol* symbol = context->GetSymbolTable()->Lookup(node.Str(), SymbolGroupKind::typeSymbolGroup, fullSpan, context);
    if (!symbol && !createTypeSymbol)
    {
        int topScopeIndex = context->GetSymbolTable()->TopScopeIndex();
        context->GetSymbolTable()->SetTopScopeIndex(0);
        symbol = context->GetSymbolTable()->LookupInScopeStack(node.Str(), SymbolGroupKind::typeSymbolGroup, fullSpan, context, LookupFlags::none);
        context->GetSymbolTable()->SetTopScopeIndex(topScopeIndex);
    }
    if (symbol)
    {
        if (symbol->IsTypeSymbol())
        {
            type = static_cast<TypeSymbol*>(symbol);
        }
        else if (symbol->IsAliasGroupSymbol())
        {
            AliasGroupSymbol* aliasGroup = static_cast<AliasGroupSymbol*>(symbol);
            type = context->GetSymbolTable()->MakeAliasGroupTypeSymbol(aliasGroup);
        }
        else if (symbol->IsClassGroupSymbol())
        {
            ClassGroupSymbol* classGroup = static_cast<ClassGroupSymbol*>(symbol);
            type = context->GetSymbolTable()->MakeClassGroupTypeSymbol(classGroup);
        }
        else
        {
            if ((resolverFlags & TypeResolverFlags::dontThrow) == TypeResolverFlags::none)
            {
                ThrowException("symbol '" + util::ToUtf8(symbol->Name()) + "' is not a type symbol", fullSpan, context);
                type = nullptr;
            }
        }
    }
    else if (createTypeSymbol)
    {
        Scope* scope = context->GetSymbolTable()->CurrentScope()->SymbolScope();
        ContainerSymbol* containerSymbol = nullptr;
        if (scope->IsContainerScope())
        {
            ContainerScope* containerScope = static_cast<ContainerScope*>(scope);
            std::vector<Symbol*> symbols;
            std::set<const Scope*> visited;
            containerScope->Lookup(node.Str(), SymbolGroupKind::typeSymbolGroup, ScopeLookup::thisScope, LookupFlags::none, symbols, visited, context);
            if (!symbols.empty())
            {
                Symbol* symbol = symbols.front();
                if (symbol->IsTypeSymbol())
                {
                    type = static_cast<TypeSymbol*>(symbol);
                }
                else
                {
                    type = context->GetSymbolTable()->GetErrorTypeSymbol();
                }
            }
            else
            {
                containerSymbol = containerScope->GetContainerSymbol();
                NestedTypeSymbol* nestedTypeSymbol = new NestedTypeSymbol(node.Str());
                containerSymbol->AddSymbol(nestedTypeSymbol, fullSpan, context);
                type = nestedTypeSymbol;
            }
        }
        else
        {
            type = context->GetSymbolTable()->GetErrorTypeSymbol();
        }
    }
    else
    {
        if ((resolverFlags & TypeResolverFlags::dontThrow) == TypeResolverFlags::none)
        {
            ThrowException("symbol '" + util::ToUtf8(node.Str()) + "' not found", fullSpan, context);
        }
        type = nullptr;
    }
*/
}

void TypeResolver::Visit(otava::ast::TemplateIdNode& node)
{
/*
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    TypeSymbol* typeSymbol = otava::symbols::ResolveType(node.TemplateName(), DeclarationFlags::none, context, resolverFlags);
    if (!typeSymbol)
    {
        type = nullptr;
        return;
    }
    TemplateDeclarationSymbol* templateDeclaration = nullptr;
    if (typeSymbol->IsClassTypeSymbol())
    {
        ClassTypeSymbol* classTemplate = static_cast<ClassTypeSymbol*>(typeSymbol);
        templateDeclaration = classTemplate->ParentTemplateDeclaration();
    }
    std::vector<Symbol*> templateArgs;
    int n = node.Items().size();
    for (int i = 0; i < n; ++i)
    {
        otava::ast::Node* argItem = node.Items()[i];
        TemplateParameterSymbol* templateParameter = nullptr;
        if (templateDeclaration && i < templateDeclaration->Arity())
        {
            templateParameter = templateDeclaration->TemplateParameters()[i];
            ParameterSymbol* parameter = templateParameter->GetParameterSymbol();
            if (parameter)
            {
                Value* value = Evaluate(argItem, context);
                templateArgs.push_back(value);
                continue;
            }
        }
        TypeSymbol* templateArg = otava::symbols::ResolveType(argItem, DeclarationFlags::none, context, resolverFlags);
        if (!templateArg)
        {
            type = nullptr;
            return;
        }
        templateArg = templateArg->DirectType(context)->FinalType(fullSpan, context);
        templateArgs.push_back(templateArg);
    }
    if (typeSymbol->IsClassGroupTypeSymbol())
    {
        ClassGroupTypeSymbol* classGroupType = static_cast<ClassGroupTypeSymbol*>(typeSymbol);
        ClassGroupSymbol* classGroup = classGroupType->ClassGroup();
        TemplateMatchInfo matchInfo;
        typeSymbol = classGroup->GetBestMatchingClass(templateArgs, matchInfo, context);
        if (!typeSymbol)
        {
            ThrowException("no matching class found from class group '" + util::ToUtf8(classGroup->Name()) + "'", fullSpan, context);
        }
        else
        {
            if (matchInfo.kind == TemplateMatchKind::explicitSpecialization)
            {
                templateArgs = matchInfo.templateArgs;
            }
        }
    }
    else if (typeSymbol->IsAliasGroupTypeSymbol())
    {
        AliasGroupTypeSymbol* aliasGroupType = static_cast<AliasGroupTypeSymbol*>(typeSymbol);
        AliasGroupSymbol* aliasGroup = aliasGroupType->AliasGroup();
        typeSymbol = aliasGroup->GetBestMatchingAliasType(templateArgs, context);
        if (!typeSymbol)
        {
            ThrowException("no matching alias type found from alias group '" + util::ToUtf8(aliasGroup->Name()) + "'", fullSpan, context);
        }
    }
    else if (typeSymbol->IsForwardClassDeclarationSymbol())
    {
        typeSymbol = ResolveFwdDeclaredType(typeSymbol, fullSpan, context);
    }
    if ((resolverFlags & TypeResolverFlags::dontInstantiate) == TypeResolverFlags::none)
    {
        if (typeSymbol->IsAliasTypeSymbol())
        {
            TypeSymbol* specialization = InstantiateAliasTypeSymbol(typeSymbol, templateArgs, &node, context);
            type = specialization;
        }
        else if (typeSymbol->IsClassTypeSymbol())
        {
            ClassTypeSymbol* classTemplate = static_cast<ClassTypeSymbol*>(typeSymbol);
            TypeSymbol* specialization = InstantiateClassTemplate(classTemplate, templateArgs, fullSpan, context);
            type = specialization;
        }
        else if (typeSymbol->IsForwardClassDeclarationSymbol())
        {
            type = typeSymbol;
        }
        else
        {
            ThrowException("alias type or class type expected", fullSpan, context);
        }
    }
    else
    {
        if (typeSymbol->IsClassTypeSymbol())
        {
            ClassTypeSymbol* classTemplate = static_cast<ClassTypeSymbol*>(typeSymbol);
            ClassTemplateSpecializationSymbol* specialization = context->GetSymbolTable()->MakeClassTemplateSpecialization(classTemplate, templateArgs, fullSpan, context);
            type = specialization;
        }
        else
        {
            ThrowException("alias type or class type expected", fullSpan, context);
        }
    }
*/
}

void TypeResolver::Visit(otava::ast::TypeIdNode& node)
{
    node.TypeSpecifiers()->Accept(*this);
    while (type && type->IsAliasTypeSymbol())
    {
        AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(type);
        type = aliasType->ReferredType(context);
    }
    if (!type)
    {
        ResolveBaseType(&node);
        type = baseType;
    }
    DeclarationFlags prevFlags = flags;
    node.Declarator()->Accept(*this);
    typeResolved = false;
    ResolveType();
}

void TypeResolver::Visit(otava::ast::FunctionDeclaratorNode& node)
{
    ResolveType();
    Declaration declaration = ProcessDeclarator(type, &node, &node, flags, FunctionQualifiers::none, context);
    type = declaration.type;
}

void TypeResolver::Visit(otava::ast::NewTypeIdNode& node)
{
    node.TypeSpecifierSeq()->Accept(*this);
    TypeSymbol* newType = type;
    if (node.NewDeclarator())
    {
        node.NewDeclarator()->Accept(*this);
    }
}

void TypeResolver::Visit(otava::ast::ArrayNewDeclaratorNode& node)
{
    size = BindExpression(&node, context);
}

TypeSymbol* ResolveType(otava::ast::Node* node, DeclarationFlags flags, Context* context)
{
    return ResolveType(node, flags, context, TypeResolverFlags::none);
}

TypeSymbol* ResolveType(otava::ast::Node* node, DeclarationFlags flags, Context* context, TypeResolverFlags resolverFlags)
{
    TypeResolver resolver(context, node->GetFullSpan(), flags, resolverFlags);
    node->Accept(resolver);
    TypeSymbol* type = resolver.GetType();
    return type;
}

TypeSymbol* ResolveFwdDeclaredType(TypeSymbol* type, const soul::ast::FullSpan& fullSpan, Context* context)
{
/*
    if (type->IsCompoundTypeSymbol())
    {
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(type);
        TypeSymbol* resolvedType = context->GetSymbolTable()->MakeCompoundType(ResolveFwdDeclaredType(compoundTypeSymbol->GetBaseType(), fullSpan, context),
            compoundTypeSymbol->GetDerivations(), context);
        return resolvedType;
    }
    if (type->IsForwardClassDeclarationSymbol())
    {
        ForwardClassDeclarationSymbol* fwdClassDeclarationSymbol = static_cast<ForwardClassDeclarationSymbol*>(type);
        if (fwdClassDeclarationSymbol->GetClassTypeSymbol())
        {
            return fwdClassDeclarationSymbol->GetClassTypeSymbol();
        }
        else
        {
            Symbol* type = context->GetSymbolTable()->Lookup(fwdClassDeclarationSymbol->Name(), SymbolGroupKind::typeSymbolGroup, fullSpan, context,
                LookupFlags::noFwdDeclarationSymbol);
            if (type && type->IsClassTypeSymbol())
            {
                fwdClassDeclarationSymbol->SetClassTypeSymbol(static_cast<ClassTypeSymbol*>(type));
                return static_cast<TypeSymbol*>(type);
            }
        }
    }
    else if (type->IsForwardEnumDeclarationSymbol())
    {
        ForwardEnumDeclarationSymbol* fwdEnumDeclarationSymbol = static_cast<ForwardEnumDeclarationSymbol*>(type);
        if (fwdEnumDeclarationSymbol->GetEnumeratedTypeSymbol())
        {
            return fwdEnumDeclarationSymbol->GetEnumeratedTypeSymbol();
        }
        else
        {
            Symbol* type = context->GetSymbolTable()->Lookup(fwdEnumDeclarationSymbol->Name(), SymbolGroupKind::typeSymbolGroup, fullSpan, context,
                LookupFlags::noFwdDeclarationSymbol);
            if (type && type->IsEnumeratedTypeSymbol())
            {
                fwdEnumDeclarationSymbol->SetEnumeratedTypeSymbol(static_cast<EnumeratedTypeSymbol*>(type));
                return static_cast<TypeSymbol*>(type);
            }
        }
    }
    return type;
*/
    return nullptr;
}

} // namespace otava::symbols
