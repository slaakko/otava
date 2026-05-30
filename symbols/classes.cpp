// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.classes;

import otava.symbols.class_group_symbol;
import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.exception;
import otava.symbols.modules;
import otava.symbols.templates;
import otava.symbols.type_resolver;
import otava.ast.visitor;
import otava.ast.identifier;
import otava.ast.templates;
import otava.ast.type;

namespace otava::symbols {

Symbol* GenerateDestructor(ClassTypeSymbol* classTypeSymbol, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context);

std::int32_t GetSpecialFunctionIndex(SpecialFunctionKind specialFunctionKind) noexcept
{
    switch (specialFunctionKind)
    {
    case SpecialFunctionKind::defaultCtor:
    {
        return defaultCtorIndex;
    }
    case SpecialFunctionKind::copyCtor:
    {
        return copyCtorIndex;
    }
    case SpecialFunctionKind::moveCtor:
    {
        return moveCtorIndex;
    }
    case SpecialFunctionKind::copyAssignment:
    {
        return copyAssignmentIndex;
    }
    case SpecialFunctionKind::moveAssignment:
    {
        return moveAssignmentIndex;
    }
    case SpecialFunctionKind::dtor:
    {
        return destructorIndex;
    }
    }
    return 0;
}

RecordedParseCompoundStatementFn recordedParseCompoundStatementFn = nullptr;

void SetRecordedParseCompoundStatementFn(RecordedParseCompoundStatementFn fn) noexcept
{
    recordedParseCompoundStatementFn = fn;
}

void RecordedParseCompoundStatement(otava::ast::CompoundStatementNode* compoundStatementNode, Context* context)
{
    if (recordedParseCompoundStatementFn)
    {
        recordedParseCompoundStatementFn(compoundStatementNode, context);
    }
}

RecordedParseCtorInitializerFn recordedParseInitializerFn = nullptr;

void SetRecordedParseCtorInitializerFn(RecordedParseCtorInitializerFn fn) noexcept
{
    recordedParseInitializerFn = fn;
}

void RecordedParseCtorInitializer(otava::ast::ConstructorInitializerNode* ctorInitializerNode, Context* context)
{
    recordedParseInitializerFn(ctorInitializerNode, context);
}

ClassTypeSymbol::ClassTypeSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), flags(ClassTypeSymbolFlags::none), level(0), group(nullptr), groupId(zeroSymbolId), vtabSize(0), vptrIndex(-1), deltaIndex(-1)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

ClassTypeSymbol::ClassTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), flags(ClassTypeSymbolFlags::none), level(0), group(nullptr), groupId(zeroSymbolId), vtabSize(0), vptrIndex(-1), deltaIndex(-1)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

bool ClassTypeSymbol::IsTemplate(Context* context) const noexcept
{
    return ParentTemplateDeclaration(context) != nullptr;
}

ClassGroupSymbol* ClassTypeSymbol::Group(Context* context) const
{
    if (group)
    {
        return group;
    }
    if (IsReadOnly() && groupId != zeroSymbolId)
    {
        group = GetModule()->GetSymbolTable()->GetClassGroupSymbol(groupId, context);
    }
    return group;
}

bool ClassTypeSymbol::IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType) const noexcept
{
/*
    const TypeSymbol* thisSymbol = this;
    if (visited.find(thisSymbol) != visited.end()) return true;
    visited.insert(thisSymbol);
    for (ClassTypeSymbol* baseClass : baseClasses)
    {
        if (!baseClass->IsComplete(visited, incompleteType)) return false;
    }
    for (VariableSymbol* memberVariable : memberVariables)
    {
        if (!memberVariable->GetType()->IsComplete(visited, incompleteType)) return false;
    }
*/
    return true;
}

TemplateDeclarationSymbol* ClassTypeSymbol::ParentTemplateDeclaration(Context* context) const noexcept
{
    Symbol* parentSymbol = Parent(context);
    if (parentSymbol && parentSymbol->IsTemplateDeclarationSymbol())
    {
        return static_cast<TemplateDeclarationSymbol*>(parentSymbol);
    }
    return nullptr;
}

std::int32_t ClassTypeSymbol::NextFunctionIndex() noexcept
{
    // TODO
    //return currentFunctionIndex++;
    return 0;
}

void ClassTypeSymbol::MapFunction(FunctionSymbol* function)
{
    // TODO
    //functionIndexMap[function->Index()] = function;
}

void ClassTypeSymbol::SetMemFnDefSymbol(FunctionDefinitionSymbol* memFnDefSymbol)
{
/*
    if (memFnDefSymbol->DefIndex() == -1)
    {
        memFnDefSymbol->SetDefIndex(nextMemFnDefIndex++);
    }
    memFnDefSymbolMap[memFnDefSymbol->DefIndex()] = memFnDefSymbol;
    nextMemFnDefIndex = std::max(nextMemFnDefIndex, memFnDefSymbol->DefIndex() + 1);
*/
}

FunctionDefinitionSymbol* ClassTypeSymbol::GetMemFnDefSymbol(int32_t defIndex) const noexcept
{
    /*
    auto it = memFnDefSymbolMap.find(defIndex);
    if (it != memFnDefSymbolMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
    */
    return nullptr;
}

FunctionSymbol* ClassTypeSymbol::GetFunctionByIndex(std::int32_t functionIndex) const noexcept
{
/*
    auto it = functionIndexMap.find(functionIndex);
    if (it != functionIndexMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
*/
    return nullptr;
}

otava::intermediate::Type* ClassTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
/*
    util::uuid irId = IrId(fullSpan, context);
    otava::intermediate::Type* irType = emitter.GetType(irId);
    if (!irType)
    {
        irType = emitter.GetOrInsertFwdDeclaredStructureType(irId, util::ToUtf8(FullName()));
        emitter.SetType(irId, irType);
        MakeObjectLayout(fullSpan, context);
        int n = objectLayout.size();
        std::vector<otava::intermediate::Type*> elementTypes;
        for (int i = 0; i < n; ++i)
        {
            TypeSymbol* type = objectLayout[i];
            elementTypes.push_back(type->IrType(emitter, fullSpan, context));
        }
        otava::intermediate::MetadataStruct* metadataStruct = emitter.CreateMetadataStruct();
        metadataStruct->AddItem("fullName", emitter.CreateMetadataString(util::ToUtf8(FullName())));
        otava::intermediate::MetadataRef* metadataRef = emitter.CreateMetadataRef(metadataStruct->Id());
        otava::intermediate::Type* type = emitter.MakeStructureType(elementTypes, util::ToUtf8(FullName()));
        otava::intermediate::StructureType* structureType = static_cast<otava::intermediate::StructureType*>(type);
        structureType->SetMetadataRef(metadataRef);
        irType = type;
        emitter.SetType(irId, irType);
        emitter.ResolveForwardReferences(irId, structureType);
    }
    return irType;
*/
    return nullptr;
}

void ClassTypeSymbol::MakeVTab(Context* context, const soul::ast::FullSpan& fullSpan)
{
    // TODO
}

void ClassTypeSymbol::InitVTab(std::vector<FunctionSymbol*>& vtab, Context* context, const soul::ast::FullSpan& fullSpan, bool clear) 
{
    // TODO
}

bool ClassTypeSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::templateDeclarationScope:
    case ScopeKind::classScope:
    case ScopeKind::blockScope:
    {
        return true;
    }
    }
    return false;
}

std::vector<ClassTypeSymbol*> ClassTypeSymbol::VPtrHolderClasses() const
{
    // TODO
    return std::vector<ClassTypeSymbol*>();
}

otava::intermediate::Type* ClassTypeSymbol::VPtrType(Emitter& emitter) const noexcept
{
    otava::intermediate::Type* voidPtrIrType = emitter.MakePtrType(emitter.GetVoidType());
    otava::intermediate::Type* vptrType = emitter.MakePtrType(emitter.MakeArrayType(vtabSize * 2 + otava::symbols::vtabClassIdElementCount, voidPtrIrType));
    return vptrType;
}

ForwardClassDeclarationSymbol::ForwardClassDeclarationSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), classTypeSymbol(nullptr)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

ForwardClassDeclarationSymbol::ForwardClassDeclarationSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), classTypeSymbol(nullptr)
{
    GetScope()->SetKind(ScopeKind::classScope);
}

bool ForwardClassDeclarationSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::templateDeclarationScope:
    case ScopeKind::classScope:
    case ScopeKind::blockScope:
    {
        return true;
    }
    }
    return false;
}

bool ForwardClassDeclarationSymbol::IsComplete(std::set<const TypeSymbol*>& visited, const TypeSymbol*& incompleteType) const noexcept
{
    const TypeSymbol* thisSymbol = this;
    if (visited.find(thisSymbol) != visited.end()) return true;
    visited.insert(thisSymbol);
    if (classTypeSymbol)
    {
        return classTypeSymbol->IsComplete(visited, incompleteType);
    }
    else
    {
        const TypeSymbol* ict = incompleteType;
        if (!ict)
        {
            incompleteType = this;
        }
        return false;
    }
}

TypeSymbol* ForwardClassDeclarationSymbol::FinalType(const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (classTypeSymbol)
    {
        return classTypeSymbol->FinalType(fullSpan, context);
    }
    else
    {
        return this;
    }
}

otava::intermediate::Type* ForwardClassDeclarationSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    TypeSymbol* finalType = FinalType(fullSpan, context);
    if (finalType->IsForwardClassDeclarationSymbol())
    {
        return context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(
            FundamentalTypeKind::voidType, context)->IrType(emitter, fullSpan, context);
    }
    else
    {
        return finalType->IrType(emitter, fullSpan, context);
    }
}

void ThrowMemberDeclarationParsingError(const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ThrowException("class member declaration parsing error", fullSpan, context);
}

void ThrowStatementParsingError(const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    ThrowException("statement parsing error", fullSpan, context);
}

class ClassResolver : public otava::ast::DefaultVisitor
{
public:
    ClassResolver(Context* context_);
    std::string GetName() const { return name; }
    otava::symbols::ClassKind GetClassKind() const noexcept { return classKind; }
    TypeSymbol* Specialization() const noexcept { return specialization; }
    void Visit(otava::ast::ClassSpecifierNode& node) override;
    void Visit(otava::ast::ClassHeadNode& node) override;
    void Visit(otava::ast::ElaboratedTypeSpecifierNode& node) override;
    void Visit(otava::ast::ClassNode& node) override;
    void Visit(otava::ast::StructNode& node) override;
    void Visit(otava::ast::UnionNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::TemplateIdNode& node) override;
private:
    Context* context;
    std::string name;
    otava::symbols::ClassKind classKind;
    TypeSymbol* specialization;
};

ClassResolver::ClassResolver(Context* context_) : context(context_), classKind(otava::symbols::ClassKind::class_), specialization(nullptr)
{
}

void ClassResolver::Visit(otava::ast::ClassSpecifierNode& node)
{
    node.ClassHead()->Accept(*this);
}

void ClassResolver::Visit(otava::ast::ClassHeadNode& node)
{
    node.ClassKey()->Accept(*this);
    node.ClassHeadName()->Accept(*this);
}

void ClassResolver::Visit(otava::ast::IdentifierNode& node)
{
    name = node.Str();
}

void ClassResolver::Visit(otava::ast::TemplateIdNode& node)
{
    node.TemplateName()->Accept(*this);
    specialization = ResolveType(&node, DeclarationFlags::none, context, TypeResolverFlags::dontInstantiate);
}

void ClassResolver::Visit(otava::ast::ElaboratedTypeSpecifierNode& node)
{
    node.ClassKey()->Accept(*this);
    name = node.GetId()->Str();
}

void ClassResolver::Visit(otava::ast::ClassNode& node)
{
    classKind = otava::symbols::ClassKind::class_;
}

void ClassResolver::Visit(otava::ast::StructNode& node)
{
    classKind = otava::symbols::ClassKind::struct_;
}

void ClassResolver::Visit(otava::ast::UnionNode& node)
{
    classKind = otava::symbols::ClassKind::union_;
}

class BaseClassResolver : public otava::ast::DefaultVisitor
{
public:
    BaseClassResolver(Context* context_);
    void Visit(otava::ast::BaseSpecifierNode& node) override;
    std::vector<ClassTypeSymbol*> BaseClasses() const { return std::move(baseClasses); }
private:
    Context* context;
    std::vector<ClassTypeSymbol*> baseClasses;
};

BaseClassResolver::BaseClassResolver(Context* context_) : context(context_)
{
}

void BaseClassResolver::Visit(otava::ast::BaseSpecifierNode& node)
{
    TypeSymbol* baseClassType = ResolveType(node.ClassOrDeclType(), DeclarationFlags::none, context);
    if (baseClassType->IsClassTypeSymbol())
    {
        ClassTypeSymbol* baseClass = static_cast<ClassTypeSymbol*>(baseClassType);
        baseClasses.push_back(baseClass);
    }
    else
    {
        ThrowException("class type symbol expected", node.GetFullSpan(), context);
    }
}

std::vector<ClassTypeSymbol*> ResolveBaseClasses(otava::ast::Node* node, Context* context)
{
    BaseClassResolver resolver(context);
    node->Accept(resolver);
    return resolver.BaseClasses();
}

void GetClassAttributes(otava::ast::Node* node, std::string& name, otava::symbols::ClassKind& kind, TypeSymbol*& specialization, Context* context)
{
    ClassResolver resolver(context);
    node->Accept(resolver);
    name = resolver.GetName();
    kind = resolver.GetClassKind();
    specialization = resolver.Specialization();
}

void SetCurrentAccess(otava::ast::Node* node, otava::symbols::Context* context)
{
    switch (node->Kind())
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

void BeginClass(otava::ast::Node* node, Context* context)
{
    std::string name;
    otava::symbols::ClassKind kind;
    TypeSymbol* specialization = nullptr;
    GetClassAttributes(node, name, kind, specialization, context);
    context->GetSymbolTable()->BeginClass(name, kind, specialization, node, context);
    std::vector<ClassTypeSymbol*> baseClasses = ResolveBaseClasses(node, context);
    for (ClassTypeSymbol* baseClass : baseClasses)
    {
        context->GetSymbolTable()->AddBaseClass(baseClass, node->GetFullSpan(), context);
    }
    context->PushSetFlag(ContextFlags::parseMemberFunction);
}

void EndClass(otava::ast::Node* node, Context* context)
{
    Symbol* symbol = context->GetSymbolTable()->CurrentScope()->GetSymbol();
    if (!symbol->IsClassTypeSymbol())
    {
        ThrowException("otava.symbols.classes: EndClass(): class scope expected", node->GetFullSpan(), context);
    }
    ClassTypeSymbol* classTypeSymbol = static_cast<ClassTypeSymbol*>(symbol);
    otava::ast::Node* specNode = context->GetSymbolTable()->GetSpecifierNode(classTypeSymbol);
    if (specNode && specNode->IsClassSpecifierNode())
    {
        otava::ast::ClassSpecifierNode* specifierNode = static_cast<otava::ast::ClassSpecifierNode*>(specNode);
        specifierNode->SetComplete();
    }
    else
    {
        ThrowException("otava.symbols.classes: EndClass(): class specifier node expected", node->GetFullSpan(), context);
    }
/*
    if (!classTypeSymbol->IsTemplate() && !classTypeSymbol->HasUserDefinedDestructor())
    {
        std::set<const TypeSymbol*> visited;
        const TypeSymbol* incompleteType = nullptr;
        if (classTypeSymbol->IsComplete(visited, incompleteType))
        {
            GenerateDestructor(classTypeSymbol, node->GetFullSpan(), context);
        }
        else
        {
            context->GetBoundCompileUnit()->AddClassToGenerateDestructorList(classTypeSymbol);
        }
    }
    context->PopFlags();
    context->GetSymbolTable()->EndClass();
    if (classTypeSymbol->Level() == 0)
    {
        ParseInlineMemberFunctions(specNode, classTypeSymbol, context);
    }
    std::set<const Symbol*> visited;
    if (!classTypeSymbol->IsTemplate() && !classTypeSymbol->IsTemplateParameterInstantiation(context, visited))
    {
        context->GetBoundCompileUnit()->AddBoundNodeForClass(classTypeSymbol, node->GetFullSpan(), context);
    }
*/
}

void ProcessElaboratedClassDeclaration(otava::ast::Node* node, otava::symbols::Context* context)
{
    std::string name;
    otava::symbols::ClassKind kind;
    TypeSymbol* specialization = nullptr;
    GetClassAttributes(node, name, kind, specialization, context);
    if (context->GetFlag(ContextFlags::friendSpecifier))
    {
        context->GetSymbolTable()->AddFriend(name, node, context);
    }
    else
    {
        context->GetSymbolTable()->AddForwardClassDeclaration(name, kind, specialization, node, context);
    }
}

void ParseInlineMemberFunctions(otava::ast::Node* classSpecifierNode, ClassTypeSymbol* classTypeSymbol, Context* context)
{
/*
    context->GetSymbolTable()->BeginScope(classTypeSymbol->GetScope());
    FunctionDefinitionMapBuilderVisitor visitor(context);
    classSpecifierNode->Accept(visitor);
    ClassParsingMap* classParsingMap = visitor.GetClassParsingMap();
    for (auto* fn : classParsingMap->Functions())
    {
        ParseInlineMemberFunction(context, fn);
    }
    context->GetSymbolTable()->EndScope();
*/
}

void ParseInlineMemberFunction(Context* context, FunctionSymbol* memfn)
{
/*
    ClassParsingMap* classParsingMap = memfn->GetClassParsingMap();
    if (!classParsingMap) return;
    if (!memfn->IsUnparsed()) return;
    if (memfn->Parsing() && memfn->IsInline())
    {
        ThrowException("inline member function cannot be recursive", memfn->GetFullSpan(), context);
    }
    memfn->SetParsing();
    otava::ast::FunctionDefinitionNode* node = classParsingMap->GetFunctionDefnitionNode(memfn);
    if (!node)
    {
        ThrowException("error parsing inline member function: function definition node not found", memfn->GetFullSpan(), context);
    }
    try
    {
        otava::ast::Node* fnBody = node->FunctionBody();
        otava::ast::ConstructorInitializerNode* ctorInitializerNode = nullptr;
        otava::ast::CompoundStatementNode* compoundStatementNode = nullptr;
        otava::ast::ConstructorNode* constructorNode = nullptr;
        otava::ast::FunctionBodyNode* functionBodyNode = nullptr;
        if (fnBody->IsConstructorNode())
        {
            constructorNode = static_cast<otava::ast::ConstructorNode*>(fnBody);
            ctorInitializerNode = static_cast<otava::ast::ConstructorInitializerNode*>(constructorNode->Left());
            compoundStatementNode = static_cast<otava::ast::CompoundStatementNode*>(constructorNode->Right());
        }
        else if (fnBody->IsFunctionBodyNode())
        {
            functionBodyNode = static_cast<otava::ast::FunctionBodyNode*>(node->FunctionBody());
            compoundStatementNode = static_cast<otava::ast::CompoundStatementNode*>(functionBodyNode->Child());
        }
        if (ctorInitializerNode)
        {
            if (ctorInitializerNode->GetLexerPosPair().IsValid())
            {
                RecordedParseCtorInitializer(ctorInitializerNode, context);
            }
        }
        if (compoundStatementNode)
        {
            if (compoundStatementNode->GetLexerPosPair().IsValid())
            {
                RecordedParseCompoundStatement(compoundStatementNode, context);
            }
        }
        if (!context->GetFlag(ContextFlags::parsingTemplateDeclaration) && memfn->IsFunctionDefinitionSymbol())
        {
            FunctionDefinitionSymbol* functionDefinitionSymbol = static_cast<FunctionDefinitionSymbol*>(memfn);
            context->PushBoundFunction(new BoundFunctionNode(functionDefinitionSymbol, node->GetFullSpan()));
            functionDefinitionSymbol = BindFunction(node, functionDefinitionSymbol, context);
            std::unique_ptr<BoundNode> boundFunctionNode(context->ReleaseBoundFunction());
            if (functionDefinitionSymbol->IsBound())
            {
                context->GetBoundCompileUnit()->AddBoundNode(std::move(boundFunctionNode), context);
            }
            context->PopBoundFunction();
        }
        context->GetLexer()->SetLog(nullptr);
    }
    catch (const std::exception& ex)
    {
        ThrowException("error parsing inline member function body: " + std::string(ex.what()), node->GetFullSpan(), context);
    }
    memfn->ResetUnparsed();
    memfn->ResetParsing();
    memfn->SetClassParsingMap(nullptr);
*/
}

Symbol* GenerateDestructor(ClassTypeSymbol* classTypeSymbol, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
/*
    Symbol* dtorFunctionGroupSymbol = classTypeSymbol->GetScope()->Lookup("@destructor", SymbolGroupKind::functionSymbolGroup, ScopeLookup::thisScope,
        fullSpan, context, LookupFlags::dontResolveSingle);
    Symbol* destructorFn = nullptr;
    if (dtorFunctionGroupSymbol && dtorFunctionGroupSymbol->IsFunctionGroupSymbol())
    {
        FunctionGroupSymbol* destructorGroup = static_cast<FunctionGroupSymbol*>(dtorFunctionGroupSymbol);
        destructorFn = destructorGroup->GetSingleDefinition();
        if (destructorFn)
        {
            if (destructorFn->IsFunctionSymbol())
            {
                FunctionSymbol* dtor = static_cast<FunctionSymbol*>(destructorFn);
                if (classTypeSymbol->IsPolymorphic())
                {
                    if (classTypeSymbol->HasPolymorphicBaseClass())
                    {
                        dtor->SetOverride();
                    }
                    else
                    {
                        dtor->SetVirtual();
                    }
                }
                dtor->SetNoExcept();
            }
            return destructorFn;
        }
        destructorFn = destructorGroup->GetSingleSymbol(context);
        if (destructorFn && destructorFn != destructorGroup)
        {
            if (destructorFn->IsFunctionSymbol())
            {
                FunctionSymbol* dtor = static_cast<FunctionSymbol*>(destructorFn);
                if (classTypeSymbol->IsPolymorphic())
                {
                    if (classTypeSymbol->HasPolymorphicBaseClass())
                    {
                        dtor->SetOverride();
                    }
                    else
                    {
                        dtor->SetVirtual();
                    }
                }
                dtor->SetNoExcept();
            }
            return destructorFn;
        }
    }
    if (classTypeSymbol->IsClassTemplateSpecializationSymbol())
    {
        ClassTemplateSpecializationSymbol* sp = static_cast<ClassTemplateSpecializationSymbol*>(classTypeSymbol);
        std::set<const Symbol*> visited;
        if (!sp->IsTemplateParameterInstantiation(context, visited))
        {
            if (!sp->Destructor())
            {
                std::set<const TypeSymbol*> visited;
                const TypeSymbol* incompleteType = nullptr;
                if (sp->IsComplete(visited, incompleteType))
                {
                    InstantiateDestructor(sp, fullSpan, context);
                }
                else
                {
                    std::string note;
                    if (incompleteType)
                    {
                        note.append(": note incomplete type is '" + util::ToUtf8(incompleteType->FullName(context)) + "'");
                    }
                    ThrowException("cannot create destructor for class template specialization '" +
                        util::ToUtf8(sp->FullName(context)) + "' because it is incomplete at this point" +
                        note, fullSpan, context);
                }
            }
            destructorFn = sp->Destructor();
            if (destructorFn)
            {
                if (destructorFn->IsFunctionSymbol())
                {
                    FunctionSymbol* dtor = static_cast<FunctionSymbol*>(destructorFn);
                    if (classTypeSymbol->IsPolymorphic())
                    {
                        if (classTypeSymbol->HasPolymorphicBaseClass())
                        {
                            dtor->SetOverride();
                        }
                        else
                        {
                            dtor->SetVirtual();
                        }
                    }
                    dtor->SetNoExcept();
                }
                return destructorFn;
            }
        }
    }
    std::unique_ptr<TrivialClassDtor> trivialClassDestructor(new TrivialClassDtor());
    int nm = classTypeSymbol->MemberVariables().size();
    int nb = classTypeSymbol->BaseClasses().size();
    if (nm == 0 && nb == 0)
    {
        FunctionGroupSymbol* functionGroup = classTypeSymbol->GetScope()->GroupScope()->GetOrInsertFunctionGroup(U"@destructor", fullSpan, context);
        Symbol* destructorSymbol = trivialClassDestructor.get();
        functionGroup->AddFunction(trivialClassDestructor.get());
        classTypeSymbol->AddSymbol(trivialClassDestructor.release(), fullSpan, context);
        return destructorSymbol;
    }
    bool hasNonTrivialDestructor = false;
    std::unique_ptr<FunctionSymbol> destructorSymbol(new FunctionSymbol(U"@destructor"));
    destructorSymbol->SetParent(classTypeSymbol);
    destructorSymbol->SetFunctionKind(FunctionKind::destructor);
    destructorSymbol->SetAccess(Access::public_);
    destructorSymbol->SetCompileUnitId(context->GetBoundCompileUnit()->Id());
    destructorSymbol->SetFixedIrName(destructorSymbol->IrName(context));
    destructorSymbol->SetNoExcept();
    std::unique_ptr<FunctionDefinitionSymbol> destructorDefinitionSymbol(new FunctionDefinitionSymbol(U"@destructor"));
    destructorDefinitionSymbol->SetParent(classTypeSymbol);
    destructorDefinitionSymbol->SetFunctionKind(FunctionKind::destructor);
    destructorDefinitionSymbol->SetAccess(Access::public_);
    destructorDefinitionSymbol->SetDeclaration(destructorSymbol.get());
    destructorDefinitionSymbol->SetCompileUnitId(context->GetBoundCompileUnit()->Id());
    destructorDefinitionSymbol->SetFixedIrName(destructorSymbol->IrName(context));
    destructorDefinitionSymbol->SetNoExcept();
    std::unique_ptr<BoundDtorTerminatorNode> terminator(new BoundDtorTerminatorNode(fullSpan));
    for (int i = nm - 1; i >= 0; --i)
    {
        VariableSymbol* memberVar = classTypeSymbol->MemberVariables()[i];
        if (memberVar->GetType()->IsPointerType() || memberVar->GetType()->IsReferenceType()) continue;
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        if (memberVar->GetType()->IsClassTypeSymbol())
        {
            ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(memberVar->GetType());
            Symbol* destructorFn = GenerateDestructor(classType, fullSpan, context);
            if (destructorFn->IsFunctionSymbol())
            {
                FunctionSymbol* dtor = static_cast<FunctionSymbol*>(destructorFn);
                if (classType->IsPolymorphic())
                {
                    if (classType->HasPolymorphicBaseClass())
                    {
                        dtor->SetOverride();
                    }
                    else
                    {
                        dtor->SetVirtual();
                    }
                }
                dtor->SetNoExcept();
            }
        }
        BoundVariableNode* boundVariableNode = new BoundVariableNode(memberVar, fullSpan);
        ParameterSymbol* thisParam = destructorDefinitionSymbol->ThisParam(context);
        BoundExpressionNode* thisPtr = new BoundParameterNode(thisParam, fullSpan, thisParam->GetReferredType(context));
        boundVariableNode->SetThisPtr(thisPtr);
        args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundAddressOfNode(boundVariableNode, fullSpan, boundVariableNode->GetType()->AddPointer(context))));
        Exception ex;
        std::vector<TypeSymbol*> templateArgs;
        std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverload(
            context->GetSymbolTable()->CurrentScope(), U"@destructor", templateArgs, args, fullSpan, context, ex);
        if (boundFunctionCall)
        {
            if (boundFunctionCall->GetFunctionSymbol()->IsVirtual() || boundFunctionCall->GetFunctionSymbol()->IsOverride() ||
                boundFunctionCall->GetFunctionSymbol()->IsFinal())
            {
                boundFunctionCall->SetFlag(BoundExpressionFlags::virtualCall);
            }
            if (!boundFunctionCall->GetFunctionSymbol()->GetFlag(FunctionSymbolFlags::trivialDestructor))
            {
                hasNonTrivialDestructor = true;
                terminator->AddMemberTerminator(boundFunctionCall.release());
            }
        }
    }
    for (int i = nb - 1; i >= 0; --i)
    {
        ClassTypeSymbol* baseClass = classTypeSymbol->BaseClasses()[i];
        std::vector<std::unique_ptr<BoundExpressionNode>> args;
        ParameterSymbol* thisParam = destructorDefinitionSymbol->ThisParam(context);
        BoundExpressionNode* thisPtr = new BoundParameterNode(thisParam, fullSpan, thisParam->GetType());
        FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
            baseClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
        if (conversion)
        {
            Symbol* destructorFn = GenerateDestructor(baseClass, fullSpan, context);
            args.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(thisPtr, conversion, fullSpan)));
            Exception ex;
            std::vector<TypeSymbol*> templateArgs;
            std::unique_ptr<BoundFunctionCallNode> boundFunctionCall = ResolveOverload(
                context->GetSymbolTable()->CurrentScope(), U"@destructor", templateArgs, args, fullSpan, context, ex);
            if (boundFunctionCall)
            {
                if (!boundFunctionCall->GetFunctionSymbol()->GetFlag(FunctionSymbolFlags::trivialDestructor))
                {
                    hasNonTrivialDestructor = true;
                    terminator->AddMemberTerminator(boundFunctionCall.release());
                }
            }
        }
        else
        {
            ThrowException("base class conversion not found", fullSpan, context);
        }
    }
    if (!hasNonTrivialDestructor)
    {
        FunctionGroupSymbol* functionGroup = classTypeSymbol->GetScope()->GroupScope()->GetOrInsertFunctionGroup(U"@destructor", fullSpan, context);
        FunctionSymbol* trivialDestructor = trivialClassDestructor.get();
        functionGroup->AddFunction(trivialClassDestructor.get());
        classTypeSymbol->AddSymbol(trivialClassDestructor.release(), fullSpan, context);
        return trivialDestructor;
    }
    BoundFunctionNode* boundDestructor = new BoundFunctionNode(destructorDefinitionSymbol.get(), fullSpan);
    FunctionGroupSymbol* functionGroup = classTypeSymbol->GetScope()->GroupScope()->GetOrInsertFunctionGroup(U"@destructor", fullSpan, context);
    functionGroup->AddFunction(destructorSymbol.get());
    FunctionSymbol* destructor = destructorDefinitionSymbol.get();
    classTypeSymbol->AddSymbol(destructorSymbol.release(), fullSpan, context);
    functionGroup->AddFunctionDefinition(destructorDefinitionSymbol.get(), context);
    classTypeSymbol->AddSymbol(destructorDefinitionSymbol.release(), fullSpan, context);
    BoundCompoundStatementNode* body = new BoundCompoundStatementNode(fullSpan);
    MakeObjectLayouts(classTypeSymbol, context, fullSpan);
    InitVTabs(classTypeSymbol, context, fullSpan);
    if (classTypeSymbol->IsPolymorphic())
    {
        if (classTypeSymbol->HasPolymorphicBaseClass())
        {
            destructor->SetOverride();
        }
        else
        {
            destructor->SetVirtual();
        }
        std::vector<ClassTypeSymbol*> vptrHolderClasses = classTypeSymbol->VPtrHolderClasses();
        if (vptrHolderClasses.empty())
        {
            ThrowException("no vptr holder classes for the class '" + util::ToUtf8(classTypeSymbol->FullName(context)) + "'", fullSpan, context);
        }
        for (ClassTypeSymbol* vptrHolderClass : vptrHolderClasses)
        {
            if (vptrHolderClass != classTypeSymbol)
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(destructor->ThisParam(context), fullSpan, destructor->ThisParam(context)->GetReferredType(context));
                FunctionSymbol* conversion = context->GetBoundCompileUnit()->GetArgumentConversionTable()->GetArgumentConversion(
                    vptrHolderClass->AddPointer(context), thisPtr->GetType(), fullSpan, context);
                if (conversion)
                {
                    BoundExpressionNode* thisPtrConverted = new BoundConversionNode(thisPtr, conversion, fullSpan);
                    BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtrConverted, classTypeSymbol, vptrHolderClass, fullSpan);
                    terminator->AddSetVPtrStatement(setVPtrStatement);
                }
                else
                {
                    ThrowException("vptr holder class conversion not found", fullSpan, context);
                }
            }
            else
            {
                BoundExpressionNode* thisPtr = new BoundParameterNode(destructor->ThisParam(context), fullSpan, destructor->ThisParam(context)->GetReferredType(context));
                BoundSetVPtrStatementNode* setVPtrStatement = new BoundSetVPtrStatementNode(thisPtr, classTypeSymbol, classTypeSymbol, fullSpan);
                terminator->AddSetVPtrStatement(setVPtrStatement);
            }
        }
    }
    boundDestructor->SetBody(body);
    boundDestructor->SetDtorTerminator(terminator.release());
    context->GetBoundCompileUnit()->AddBoundNode(std::unique_ptr<BoundNode>(boundDestructor), context);
    return destructor;
*/
    return nullptr;
}

void GenerateDestructors(BoundCompileUnitNode* boundCompileUnit, Context* context)
{
    for (auto* classType : boundCompileUnit->GenerateDestructorList())
    {
        soul::ast::FullSpan fullSpan = classType->GetFullSpan();
        TypeSymbol* finalType = classType->FinalType(fullSpan, context);
        if (finalType->IsClassTypeSymbol())
        {
            ClassTypeSymbol* finalClass = static_cast<ClassTypeSymbol*>(finalType);
            GenerateDestructor(finalClass, fullSpan, context);
        }
    }
}

} // namespace otava::symbol
