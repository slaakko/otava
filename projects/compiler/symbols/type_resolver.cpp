// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.type_resolver;

import otava.symbols.alias_group_symbol;
import otava.symbols.class_group_symbol;
import otava.symbols.classes;
import otava.symbols.class_templates;
import otava.symbols.compound_type_symbol;
import otava.symbols.exception;
import otava.symbols.bound_tree;
import otava.symbols.context;
import otava.symbols.declaration;
import otava.symbols.declarator;
import otava.symbols.derivations;
import otava.symbols.evaluator;
import otava.symbols.expression_binder;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.id;
import otava.symbols.lookup;
import otava.symbols.modules;
import otava.symbols.scope;
import otava.symbols.scope_ptr;
import otava.symbols.scope_resolver;
import otava.symbols.templates;
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

bool CheckDuplicateSpecifier(DeclarationFlags flags, DeclarationFlags flag, const std::string& specifierStr, const soul::ast::FullSpan& fullSpan, 
    TypeResolverFlags resolverFlags, Context* context)
{
    if ((flags & flag) != DeclarationFlags::none)
    {
        if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
        {
            context->SetException(MakeException("duplicate '" + specifierStr + "'", fullSpan, context));
            return false;
        }
        ThrowException("duplicate '" + specifierStr + "'", fullSpan, context);
    }
    return true;
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
    inline void SetCreateTypeSymbol() noexcept { createTypeSymbol = true; }
private:
    Context* context;
    TypeSymbol* type;
    TypeSymbol* baseType;
    DeclarationFlags flags;
    TypeResolverFlags resolverFlags;
    int pointerCount;
    bool typeResolved;
    bool createTypeSymbol;
    bool failed;
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
    failed(false),
    size(),
    fullSpan(fullSpan_) 
{
}

TypeSymbol* TypeResolver::GetType()
{
    if (failed)
    {
        return nullptr;
    }
    ResolveType();
    return type;
}

void TypeResolver::ResolveBaseType(otava::ast::Node* node)
{
    if (failed) return;
    DeclarationFlags fundamentalTypeFlags = flags & DeclarationFlags::fundamentalTypeFlags;
    if (fundamentalTypeFlags != DeclarationFlags::none)
    {
        if (baseType)
        {
            if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
            {
                failed = true;
                context->SetException(MakeException("duplicate type symbol in declaration specifier sequence", node->GetFullSpan(), fullSpan, context));
                return;
            }
            ThrowException("duplicate type symbol in declaration specifier sequence", node->GetFullSpan(), fullSpan, context);
        }
        baseType = GetFundamentalType(fundamentalTypeFlags, node->GetFullSpan(), context);
    }
}

void TypeResolver::ResolveType()
{
    if (failed) return;
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
    if (failed) return;
    node.DefiningTypeSpecifiers()->Accept(*this);
    if (failed) return;
    if (!type)
    {
        ResolveBaseType(&node);
        type = baseType;
    }
    if (failed) return;
    node.AbstractDeclarator()->Accept(*this);
    if (failed) return;
    ResolveType();
}

void TypeResolver::Visit(otava::ast::TypeSpecifierSequenceNode& node)
{
    if (failed) return;
    VisitSequenceContent(node);
    if (failed) return;
    if (!type)
    {
        ResolveBaseType(&node);
        if (failed) return;
        type = baseType;
    }
    if (failed) return;
    ResolveType();
}

void TypeResolver::Visit(otava::ast::CharNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::charFlag, "char", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::charFlag;
}

void TypeResolver::Visit(otava::ast::Char8Node& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::char8Flag, "char8_t", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::char8Flag;
}

void TypeResolver::Visit(otava::ast::Char16Node& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::char16Flag, "char16_t", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::char16Flag;
}

void TypeResolver::Visit(otava::ast::Char32Node& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::char32Flag, "char32_t", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::char32Flag;
}

void TypeResolver::Visit(otava::ast::WCharNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::wcharFlag, "wchar_t", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::wcharFlag;
}

void TypeResolver::Visit(otava::ast::BoolNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::wcharFlag, "bool", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::boolFlag;
}

void TypeResolver::Visit(otava::ast::ShortNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::shortFlag, "short", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::shortFlag;
}

void TypeResolver::Visit(otava::ast::IntNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::intFlag, "int", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::intFlag;
}

void TypeResolver::Visit(otava::ast::LongNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::longLongFlag, "long long", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
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
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::signedFlag, "signed", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::signedFlag;
}

void TypeResolver::Visit(otava::ast::UnsignedNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::unsignedFlag, "unsigned", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::unsignedFlag;
}

void TypeResolver::Visit(otava::ast::FloatNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::floatFlag, "float", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::floatFlag;
}

void TypeResolver::Visit(otava::ast::DoubleNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::doubleFlag, "double", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::doubleFlag;
}

void TypeResolver::Visit(otava::ast::VoidNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::voidFlag, "void", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::voidFlag;
}

void TypeResolver::Visit(otava::ast::ConstNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::constFlag, "const", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::constFlag;
}

void TypeResolver::Visit(otava::ast::VolatileNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::volatileFlag, "volatile", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::volatileFlag;
}

void TypeResolver::Visit(otava::ast::LvalueRefNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::lvalueRefFlag, "&", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::lvalueRefFlag;
}

void TypeResolver::Visit(otava::ast::RvalueRefNode& node)
{
    if (failed) return;
    if (!CheckDuplicateSpecifier(flags, DeclarationFlags::rvalueRefFlag, "&&", node.GetFullSpan(), resolverFlags, context))
    {
        failed = true;
        return;
    }
    flags = flags | DeclarationFlags::rvalueRefFlag;
}

void TypeResolver::Visit(otava::ast::PtrNode& node)
{
    if (failed) return;
    ++pointerCount;
}

void TypeResolver::Visit(otava::ast::TypenameSpecifierNode& node)
{
    if (failed) return;
    if (context->GetFlag(ContextFlags::processingAliasDeclation))
    {
        context->GetSymbolTable()->PushTopScopeIndex();
        Scope* scope = GetScope(node.NestedNameSpecifier(), context);
        ScopePtr scopePtr(scope, context);
        createTypeSymbol = true;
        node.GetId()->Accept(*this);
        if (failed)
        {
            context->GetSymbolTable()->PopTopScopeIndex();
            return;
        }
        createTypeSymbol = false;
        context->GetSymbolTable()->PopTopScopeIndex();
    }
    else
    {
        if (context->GetFlag(ContextFlags::parsingTemplateDeclaration))
        {
            type = context->GetSymbolTable()->MakeDependentTypeSymbol(node.Clone(), context);
        }
        else
        {
            InstantiationScope instantiationScope(context->GetModule(), context->GetSymbolTable()->CurrentScope());
            std::vector<std::unique_ptr<BoundTemplateParameterSymbol>> boundTemplateParameters;
            if (context->TemplateParameterMap())
            {
                for (const auto& templateParamType : *context->TemplateParameterMap())
                {
                    TemplateParameterSymbol* templateParameter = templateParamType.first;
                    BoundTemplateParameterSymbol* boundTemplateParameter = new BoundTemplateParameterSymbol(
                        context->GetModule(), context->GetNextSymbolId(SymbolKind::boundTemplateParameterSymbol), templateParameter->Name());
                    boundTemplateParameter->SetTemplateParameterSymbol(templateParameter);
                    boundTemplateParameter->SetBoundSymbol(templateParamType.second);
                    std::unique_ptr<BoundTemplateParameterSymbol> btp(boundTemplateParameter);
                    boundTemplateParameters.push_back(std::move(btp));
                    instantiationScope.Install(boundTemplateParameter, context);
                    context->GetSymbolTable()->MapSymbol(boundTemplateParameter, context);
                }
            }
            context->GetSymbolTable()->PushTopScopeIndex();
            ScopePtr instantiationScopePtr(&instantiationScope, context);
            ScopePtr scopePtr(GetScope(node.NestedNameSpecifier(), context), context);
            node.GetId()->Accept(*this);
            if (failed)
            {
                context->GetSymbolTable()->PopTopScopeIndex();
                return;
            }
            scopePtr.Reset();
            instantiationScopePtr.Reset();
            context->GetSymbolTable()->PopTopScopeIndex();
        }
    }
}

void TypeResolver::Visit(otava::ast::DeclTypeSpecifierNode& node)
{
    if (failed) return;
    std::unique_ptr<BoundExpressionNode> expr(BindExpression(node.Expression(), context));
    if (!expr->GetType())
    {
        if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
        {
            failed = true;
            context->SetException(MakeException("type for decltype specifier not resolved", node.GetFullSpan(), context));
            return;
        }
        type = nullptr;
        return;
    }
    type = expr->GetType()->PlainType(context);
}

void TypeResolver::Visit(otava::ast::QualifiedIdNode& node)
{
    if (failed) return;
    context->GetSymbolTable()->PushTopScopeIndex();
    ScopesPtr scopesPtr(GetScopes(node.Left(), context), context);
    Scopes& scopes = context->GetScopes();
    for (Scope* scope : scopes.GetScopes())
    {
        ModulePtr modulePtr(scope->GetModule(), context);
        ScopePtr scopePtr(scope, context);
        TypeResolverFlags flags = TypeResolverFlags::dontThrow;
        if (scope->GetSymbol()->IsTemplateParameterSymbol() && node.Right()->IsIdentifierNode()  && context->GetFlag(ContextFlags::processingAliasDeclation))
        {
            flags = flags | TypeResolverFlags::createTypeSymbol;
        }
        type = otava::symbols::ResolveType(node.Right(), DeclarationFlags::none, context, flags);
        if (type)
        {
            scopePtr.Reset();
            modulePtr.Reset();
            context->ResetException();
            context->GetSymbolTable()->PopTopScopeIndex();
            return;
        }
        else
        {
            failed = false;
        }
    }
    context->GetSymbolTable()->PopTopScopeIndex();
    if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
    {
        type = nullptr;
        failed = true;
        context->SetException(MakeException("type not resolved", node.GetFullSpan(), context));
    }
    else
    {
        ThrowException("type not resolved", node.GetFullSpan(), context);
    }
}

void TypeResolver::Visit(otava::ast::IdentifierNode& node)
{
    if (failed) return;
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    Symbol* symbol = nullptr;
    Symbol* s = context->GetSymbolTable()->Lookup(node.Str(),
        SymbolGroupKind::aliasSymbolGroup |
        SymbolGroupKind::classSymbolGroup |
        SymbolGroupKind::enumSymbolGroup |
        SymbolGroupKind::templateParamSymbolGroup, fullSpan, context);
    if (s)
    {
        if (!s->IsForwardClassDeclarationSymbol() || !context->GetFlag(ContextFlags::rejectIncompleteTypes))
        {
            symbol = s;
        }
    }
    if (!symbol)
    {
        if (context->HasScopes())
        {
            const Scopes& scopes = context->GetScopes();
            for (Scope* scope : scopes.GetScopes())
            {
                Symbol* s = scope->Lookup(node.Str(),
                    otava::symbols::SymbolGroupKind::aliasSymbolGroup |
                    otava::symbols::SymbolGroupKind::classSymbolGroup |
                    otava::symbols::SymbolGroupKind::enumSymbolGroup |
                    otava::symbols::SymbolGroupKind::templateParamSymbolGroup,
                    ScopeLookup::allScopes, fullSpan, context, LookupFlags::none);
                if (s)
                {
                    if (!s->IsForwardClassDeclarationSymbol() || !context->GetFlag(ContextFlags::rejectIncompleteTypes))
                    {
                        symbol = s;
                        break;
                    }
                }
            }
        }
    }
    if (!symbol && !createTypeSymbol)
    {
        int topScopeIndex = context->GetSymbolTable()->TopScopeIndex();
        context->GetSymbolTable()->SetTopScopeIndex(0);
        Symbol* s = context->GetSymbolTable()->LookupInScopeStack(node.Str(), 
            SymbolGroupKind::aliasSymbolGroup | 
            SymbolGroupKind::classSymbolGroup | 
            SymbolGroupKind::enumSymbolGroup |
            SymbolGroupKind::templateParamSymbolGroup, fullSpan, context, LookupFlags::none);
        context->GetSymbolTable()->SetTopScopeIndex(topScopeIndex);
        if (s)
        {
            if (!s->IsForwardClassDeclarationSymbol() || !context->GetFlag(ContextFlags::rejectIncompleteTypes))
            {
                symbol = s;
            }
        }
    }
    if (!symbol && !createTypeSymbol && (resolverFlags & TypeResolverFlags::dontLookImports) == TypeResolverFlags::none)
    {
        std::vector<std::string> containerNames;
        Scope* currentScope = context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context);
        if (currentScope)
        {
            containerNames = GetContainerNames(currentScope->GetSymbol(), context);
        }
        Scope* templateScope = context->GetTemplateScope();
        if (templateScope)
        {
            Symbol* symbol = templateScope->GetSymbol();
            containerNames = GetContainerNames(symbol, context);
        }
        std::vector<Module*> importedModules = context->GetModule()->ImportExportModules(context);
        Module* templateModule = context->GetTemplateModule();
        if (templateModule)
        {
            if (std::find(importedModules.begin(), importedModules.end(), templateModule) == importedModules.end())
            {
                importedModules.push_back(templateModule);
            }
            std::vector<Module*> templateModules = templateModule->ImportExportModules(context);
            for (Module* module : templateModules)
            {
                if (std::find(importedModules.begin(), importedModules.end(), module) == importedModules.end())
                {
                    importedModules.push_back(module);
                }
            }
        }
        for (Module* module : importedModules)
        {
            ModulePtr modulePtr(module, context);
            Scope* containerScope = EnterScope(context->GetSymbolTable()->CurrentScope(), containerNames, node.GetFullSpan(), context);
            ScopePtr scopePtr(containerScope, context);
            TypeSymbol* typeSymbol = otava::symbols::ResolveType(&node, DeclarationFlags::none, context, 
                resolverFlags | TypeResolverFlags::dontThrow | TypeResolverFlags::dontLookImports);
            if (typeSymbol)
            {
                if (!typeSymbol->IsForwardClassDeclarationSymbol() || !context->GetFlag(ContextFlags::rejectIncompleteTypes))
                {
                    type = typeSymbol;
                    return;
                }
            }
        }
    }
    if (symbol)
    {
        if (symbol->IsTypeSymbol())
        {
            type = static_cast<TypeSymbol*>(symbol);
        }
        else
        {
            if (symbol->IsAliasGroupSymbol())
            {
                AliasGroupSymbol* aliasGroup = static_cast<AliasGroupSymbol*>(symbol);
                if (context->GetModule()->IsReadOnly())
                {
                    ModulePtr compileUnitModule(context->GetCompileUnitModule(), context);
                    type = context->GetSymbolTable()->MakeAliasGroupTypeSymbol(aliasGroup, context);
                }
                else
                {
                    type = context->GetSymbolTable()->MakeAliasGroupTypeSymbol(aliasGroup, context);
                }
            }
            else if (symbol->IsClassGroupSymbol())
            {
                ClassGroupSymbol* classGroup = static_cast<ClassGroupSymbol*>(symbol);
                if (context->GetModule()->IsReadOnly())
                {
                    ModulePtr compileUnitModule(context->GetCompileUnitModule(), context);
                    type = context->GetSymbolTable()->MakeClassGroupTypeSymbol(classGroup, context);
                }
                else
                {
                    type = context->GetSymbolTable()->MakeClassGroupTypeSymbol(classGroup, context);
                }
            }
            else
            {
                if ((resolverFlags & TypeResolverFlags::dontThrow) == TypeResolverFlags::none)
                {
                    ThrowException("symbol '" + symbol->Name() + "' is not a type symbol", fullSpan, context);
                }
                else
                { 
                    type = nullptr;
                    failed = true;
                    context->SetException(MakeException("symbol '" +symbol->Name() + "' is not a type symbol", fullSpan, context));
                    return;
                }
            }
        }
    }
    else if (createTypeSymbol)
    {
        Scope* scope = context->GetSymbolTable()->CurrentScope()->SymbolScope(context);
        ContainerSymbol* containerSymbol = nullptr;
        if (scope->IsContainerScope())
        {
            ContainerScope* containerScope = static_cast<ContainerScope*>(scope);
            std::vector<Symbol*> symbols;
            std::set<const Scope*> visited;
            containerScope->Lookup(node.Str(), 
                SymbolGroupKind::aliasSymbolGroup | SymbolGroupKind::classSymbolGroup | SymbolGroupKind::enumSymbolGroup | SymbolGroupKind::templateParamSymbolGroup,
                ScopeLookup::thisScope, LookupFlags::none, symbols, visited, context);
            if (!symbols.empty())
            {
                Symbol* symbol = symbols.front();
                if (symbol->IsTypeSymbol())
                {
                    type = static_cast<TypeSymbol*>(symbol);
                }
            }
            else
            {
                containerSymbol = containerScope->GetContainerSymbol();
                NestedTypeSymbol* nestedTypeSymbol = new NestedTypeSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::nestedTypeSymbol), node.Str());
                if (containerSymbol->IsReadOnly())
                {
                    context->GetModule()->GetSymbolTable()->GetGlobalNs(context)->AddSymbol(nestedTypeSymbol, fullSpan, context);
                    nestedTypeSymbol->SetParent(containerSymbol);
                }
                else
                {
                    containerSymbol->AddSymbol(nestedTypeSymbol, fullSpan, context);
                }
                type = nestedTypeSymbol;
            }
        }
    }
    else
    {
        if ((resolverFlags & TypeResolverFlags::dontThrow) == TypeResolverFlags::none)
        {
            ThrowException("symbol '" + node.Str() + "' not found", fullSpan, context);
        }
        failed = true;
        context->SetException(MakeException("symbol '" + node.Str() + "' not found", fullSpan, context));
    }
}

void TypeResolver::Visit(otava::ast::TemplateIdNode& node)
{
    if (failed) return;
    std::vector<std::string> containerNames;
    Scope* currentScope = context->GetSymbolTable()->CurrentScope()->GetNamespaceScope(context);
    if (currentScope)
    {
        containerNames = GetContainerNames(currentScope->GetSymbol(), context);
    }
    Scope* templateScope = context->GetTemplateScope();
    if (templateScope)
    {
        Symbol* symbol = templateScope->GetSymbol();
        containerNames = GetContainerNames(symbol, context);
    }
    std::vector<Module*> importedModules = context->GetModule()->ImportExportModules(context);
    Module* templateModule = context->GetTemplateModule();
    if (templateModule)
    {
        if (std::find(importedModules.begin(), importedModules.end(), templateModule) == importedModules.end())
        {
            importedModules.push_back(templateModule);
        }
        std::vector<Module*> templateModules = templateModule->ImportExportModules(context);
        for (Module* module : templateModules)
        {
            if (std::find(importedModules.begin(), importedModules.end(), module) == importedModules.end())
            {
                importedModules.push_back(module);
            }
        }
    }
    soul::ast::FullSpan fullSpan = node.GetFullSpan();
    TypeSymbol* typeSymbol = otava::symbols::ResolveType(node.TemplateName(), DeclarationFlags::none, context, TypeResolverFlags::dontThrow);
    if (!typeSymbol)
    {
        ModulePtr compileUnitModulePtr(context->GetCompileUnitModule(), context);
        typeSymbol = otava::symbols::ResolveType(node.TemplateName(), DeclarationFlags::none, context, TypeResolverFlags::dontThrow);
    }
    if (!typeSymbol)
    {
        for (Module* module : importedModules)
        {
            ModulePtr modulePtr(module, context);
            Scope* containerScope = EnterScope(context->GetSymbolTable()->CurrentScope(), containerNames, node.GetFullSpan(), context);
            ScopePtr scopePtr(containerScope, context);
            typeSymbol = otava::symbols::ResolveType(node.TemplateName(), DeclarationFlags::none, context, TypeResolverFlags::dontThrow);
            if (typeSymbol)
            {
                break;
            }
        }
    }
    if (!typeSymbol)
    {
        if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
        {
            failed = true;
            context->SetException(MakeException("template name '" + node.TemplateName()->Str() + "' not resolved", node.GetFullSpan(), context));
            return;
        }
        else
        {
            ThrowException("template name '" + node.TemplateName()->Str() + "' not resolved", node.GetFullSpan(), context);
        }
    }
    ClassTypeSymbol* classTemplate = nullptr;
    TemplateDeclarationSymbol* templateDeclaration = nullptr;
    if (typeSymbol->IsClassTypeSymbol())
    {
        classTemplate = static_cast<ClassTypeSymbol*>(typeSymbol);
        templateDeclaration = classTemplate->ParentTemplateDeclaration(context);
    }
    std::vector<Symbol*> templateArgs;
    Cardinality n = Cardinality(node.Items().size());
    for (Index i = Index(0); i < ToIndex(n); ++i)
    {
        otava::ast::Node* argItem = node.Items()[ToUnderlying(i)];
        TemplateParameterSymbol* templateParameter = nullptr;
        if (templateDeclaration && i < ToIndex(templateDeclaration->Arity()))
        {
            templateParameter = templateDeclaration->TemplateParameters(context)[ToUnderlying(i)];
            ParameterSymbol* parameter = templateParameter->GetParameterSymbol(context);
            if (parameter)
            {
                Value* value = Evaluate(argItem, context);
                templateArgs.push_back(value);
                continue;
            }
        }
        TypeSymbol* templateArg = otava::symbols::ResolveType(argItem, DeclarationFlags::none, context, TypeResolverFlags::dontThrow);
        if (!templateArg)
        {
            ModulePtr compileUnitModulePtr(context->GetCompileUnitModule(), context);
            templateArg = otava::symbols::ResolveType(argItem, DeclarationFlags::none, context, TypeResolverFlags::dontThrow);
        }
        if (!templateArg)
        {
            for (Module* module : importedModules)
            {
                ModulePtr modulePtr(module, context);
                Scope* containerScope = EnterScope(context->GetSymbolTable()->CurrentScope(), containerNames, node.GetFullSpan(), context);
                ScopePtr scopePtr(containerScope, context);
                templateArg = otava::symbols::ResolveType(argItem, DeclarationFlags::none, context, TypeResolverFlags::dontThrow);
                if (templateArg)
                {
                    break;
                }
            }
        }
        if (templateArg)
        {
            ModulePtr compileUnitModule(context->GetCompileUnitModule(), context);
            templateArg = templateArg->DirectType(context)->FinalType(fullSpan, context);
        }
        if (templateArg)
        {
            templateArgs.push_back(templateArg);
        }
        else
        {
            if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
            {
                failed = true;
                context->SetException(MakeException("template argument " + std::to_string(ToUnderlying(i)) + " of type '" +
                    typeSymbol->FullName(context) + "' not resolved", fullSpan, context));
                return;
            }
            ThrowException("template argument " + std::to_string(ToUnderlying(i)) + " of type '" + 
                typeSymbol->FullName(context) + "' not resolved", fullSpan, context);
        }
    }
    if (typeSymbol->IsClassGroupTypeSymbol())
    {
        ClassGroupTypeSymbol* classGroupType = static_cast<ClassGroupTypeSymbol*>(typeSymbol);
        ClassGroupSymbol* classGroup = classGroupType->GetClassGroup();
        TemplateMatchInfo matchInfo;
        typeSymbol = classGroup->GetBestMatchingClass(templateArgs, matchInfo, context);
        if (!typeSymbol)
        {
            if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
            {
                failed = true;
                context->SetException(MakeException("no matching class found from class group '" + classGroup->Name() + "'", fullSpan, context));
                return;
            }
            ThrowException("no matching class found from class group '" + classGroup->Name() + "'", fullSpan, context);
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
        AliasGroupSymbol* aliasGroup = aliasGroupType->GetAliasGroup();
        typeSymbol = aliasGroup->GetBestMatchingAliasType(templateArgs, context);
        if (!typeSymbol)
        {
            if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
            {
                failed = true;
                context->SetException(MakeException("no matching alias type found from alias group '" + aliasGroup->Name() + "'", fullSpan, context));
                return;
            }
            ThrowException("no matching alias type found from alias group '" + aliasGroup->Name() + "'", fullSpan, context);
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
            ModulePtr compileUnitModule(context->GetCompileUnitModule(), context);
            TypeSymbol* specialization = InstantiateAliasTypeSymbol(typeSymbol, templateArgs, &node, context);
            type = specialization;
        }
        else if (typeSymbol->IsClassTypeSymbol())
        {
            ModulePtr compileUnitModule(context->GetCompileUnitModule(), context);
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
            if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
            {
                failed = true;
                context->SetException(MakeException("alias type or class type expected", fullSpan, context));
                return;
            }
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
            if ((resolverFlags & TypeResolverFlags::dontThrow) != TypeResolverFlags::none)
            {
                failed = true;
                context->SetException(MakeException("alias type or class type expected", fullSpan, context));
                return;
            }
            ThrowException("alias type or class type expected", fullSpan, context);
        }
    }
}

void TypeResolver::Visit(otava::ast::TypeIdNode& node)
{
    if (failed) return;
    node.TypeSpecifiers()->Accept(*this);
    if (failed) return;
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
    if (type)
    {
        context->ResetException();
    }
}

void TypeResolver::Visit(otava::ast::FunctionDeclaratorNode& node)
{
    if (failed) return;
    ResolveType();
    Declaration declaration = ProcessDeclarator(type, &node, &node, flags, FunctionQualifiers::none, context);
    type = declaration.type;
}

void TypeResolver::Visit(otava::ast::NewTypeIdNode& node)
{
    if (failed) return;
    node.TypeSpecifierSeq()->Accept(*this);
    TypeSymbol* newType = type;
    if (node.NewDeclarator())
    {
        node.NewDeclarator()->Accept(*this);
    }
}

void TypeResolver::Visit(otava::ast::ArrayNewDeclaratorNode& node)
{
    if (failed) return;
    size = BindExpression(&node, context);
}

TypeSymbol* ResolveType(otava::ast::Node* node, DeclarationFlags flags, Context* context)
{
    FlagSetter rejectIncompleteTypeFlagSetter(context, ContextFlags::rejectIncompleteTypes);
    TypeSymbol* type = ResolveType(node, flags, context, TypeResolverFlags::dontThrow);
    if (type)
    {
        return type;
    }
    rejectIncompleteTypeFlagSetter.Reset();
    return ResolveType(node, flags, context, TypeResolverFlags::none);
}

TypeSymbol* ResolveType(otava::ast::Node* node, DeclarationFlags flags, Context* context, TypeResolverFlags resolverFlags)
{
    TypeResolver resolver(context, node->GetFullSpan(), flags, resolverFlags);
    if ((resolverFlags & TypeResolverFlags::createTypeSymbol) != TypeResolverFlags::none)
    {
        resolver.SetCreateTypeSymbol();
    }
    node->Accept(resolver);
    TypeSymbol* type = resolver.GetType();
    if (type)
    {
        context->ResetException();
    }
    return type;
}

TypeSymbol* ResolveFwdDeclaredType(TypeSymbol* type, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (type->IsCompoundTypeSymbol())
    {
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(type);
        TypeSymbol* resolvedType = context->GetSymbolTable()->MakeCompoundType(ResolveFwdDeclaredType(compoundTypeSymbol->GetBaseType(context), fullSpan, context),
            compoundTypeSymbol->GetDerivations(), context);
        return resolvedType;
    }
    if (type->IsForwardClassDeclarationSymbol())
    {
        ForwardClassDeclarationSymbol* fwdClassDeclarationSymbol = static_cast<ForwardClassDeclarationSymbol*>(type);
        if (fwdClassDeclarationSymbol->GetClassTypeSymbol(context))
        {
            return fwdClassDeclarationSymbol->GetClassTypeSymbol(context);
        }
        else
        {
            Symbol* type = context->GetSymbolTable()->Lookup(fwdClassDeclarationSymbol->Name(), SymbolGroupKind::classSymbolGroup, fullSpan, context,
                LookupFlags::noFwdDeclarationSymbol);
            if (type && type->IsClassTypeSymbol())
            {
                fwdClassDeclarationSymbol->SetClassTypeSymbol(static_cast<ClassTypeSymbol*>(type));
                return static_cast<TypeSymbol*>(type);
            }
        }
    }
    return type;
}

Symbol* ResolveTypeIdentifier(const std::string& name, const soul::ast::FullSpan& fullSpan, Context* context)
{
    otava::symbols::Symbol* symbol = context->GetSymbolTable()->Lookup(name,
        otava::symbols::SymbolGroupKind::aliasSymbolGroup |
        otava::symbols::SymbolGroupKind::classSymbolGroup |
        otava::symbols::SymbolGroupKind::enumSymbolGroup |
        otava::symbols::SymbolGroupKind::templateParamSymbolGroup,
        fullSpan,
        context);
    if (symbol)
    {
        return symbol;
    }
    if (context->HasScopes())
    {
        const Scopes& scopes = context->GetScopes();
        for (Scope* scope : scopes.GetScopes())
        {
            symbol = scope->Lookup(name, 
                otava::symbols::SymbolGroupKind::aliasSymbolGroup |
                otava::symbols::SymbolGroupKind::classSymbolGroup |
                otava::symbols::SymbolGroupKind::enumSymbolGroup |
                otava::symbols::SymbolGroupKind::templateParamSymbolGroup,
                ScopeLookup::allScopes, fullSpan, context, LookupFlags::none);
            if (symbol)
            {
                return symbol;
            }
        }
    }
    Scope* currentSymbolScope = context->GetSymbolTable()->CurrentScope()->SymbolScope(context);
    Symbol* sym = currentSymbolScope->GetSymbol();
    std::vector<std::string> containerNames = GetContainerNames(sym, context);
    std::vector<Module*> importedModules = context->GetModule()->ImportExportModules(context);
    for (Module* importedModule : importedModules)
    {
        ModulePtr modulePtr(importedModule, context);
        Scope* containerScope = EnterScope(importedModule->GetSymbolTable()->CurrentScope(), containerNames, fullSpan, context);
        ScopePtr scopePtr(containerScope, context);
        symbol = context->GetSymbolTable()->Lookup(name,
            otava::symbols::SymbolGroupKind::aliasSymbolGroup |
            otava::symbols::SymbolGroupKind::classSymbolGroup |
            otava::symbols::SymbolGroupKind::enumSymbolGroup |
            otava::symbols::SymbolGroupKind::templateParamSymbolGroup,
            fullSpan, context);
        if (symbol)
        {
            return symbol;
        }
    }
    return nullptr;
}

} // namespace otava::symbols
