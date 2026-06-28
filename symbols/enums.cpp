// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.enums;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.evaluator;
import otava.symbols.exception;
import otava.symbols.fundamental_type_operation;
import otava.symbols.modules;
import otava.symbols.scope_ptr;
import otava.symbols.scope_resolver;
import otava.symbols.type_resolver;
import otava.symbols.value;
import otava.symbols.writer;
import otava.symbols.reader;
import otava.ast.enums;
import otava.ast.identifier;
import otava.ast.visitor;

namespace otava::symbols {

EnumeratedTypeSymbol::EnumeratedTypeSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), underlyingType(nullptr), underlyingTypeId(zeroSymbolId), bound(false), enumTypeKind(EnumTypeKind::enum_)
{
    GetScope()->SetKind(ScopeKind::enumerationScope);
}

EnumeratedTypeSymbol::EnumeratedTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), underlyingType(nullptr), underlyingTypeId(zeroSymbolId), bound(false), enumTypeKind(EnumTypeKind::enum_)
{
    GetScope()->SetKind(ScopeKind::enumerationScope);
}

bool EnumeratedTypeSymbol::IsValidDeclarationScope(ScopeKind scopeKind) const noexcept
{
    switch (scopeKind)
    {
    case ScopeKind::namespaceScope:
    case ScopeKind::classScope:
    {
        return true;
    }
    }
    return false;
}

TypeSymbol* EnumeratedTypeSymbol::UnderlyingType(Context* context) 
{
    if (underlyingType)
    {
        return underlyingType;
    }
    if (IsReadOnly() && underlyingTypeId != zeroSymbolId)
    {
        underlyingType = GetModule()->GetSymbolTable()->GetTypeSymbol(underlyingTypeId, context);
        if (!underlyingType)
        {
            ThrowException("underlying type id " + std::to_string(ToUnderlying(underlyingTypeId)) + " of enumerated type '" + Name() + "' not found", 
                GetFullSpan(), context);
        }
    }
    return underlyingType;
}

otava::intermediate::Type* EnumeratedTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (underlyingType)
    {
        return underlyingType->IrType(emitter, fullSpan, context);
    }
    else
    {
        return emitter.GetIntType();
    }
}

void EnumeratedTypeSymbol::Write(Writer& writer)
{
    TypeSymbol::Write(writer);
    if (underlyingType)
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(underlyingType->Id()));
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(ToUnderlying(zeroSymbolId));
    }
    writer.GetBinaryStreamWriter().Write(bound);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumTypeKind));
}

void EnumeratedTypeSymbol::Read(Reader& reader)
{
    TypeSymbol::Read(reader);
    underlyingTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    bound = reader.CurrentReader().ReadBool();
    enumTypeKind = EnumTypeKind(reader.CurrentReader().ReadByte());
}

int EnumeratedTypeSymbol::Rank(Context* context) 
{ 
    if (IsReadOnly())
    {
        UnderlyingType(context);
    }
    return underlyingType ? underlyingType->Rank(context) : -1; 
}

ForwardEnumDeclarationSymbol::ForwardEnumDeclarationSymbol(Module* module_, SymbolId id_) : 
    TypeSymbol(module_, id_), enumTypeSymbol(nullptr)
{
}

ForwardEnumDeclarationSymbol::ForwardEnumDeclarationSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    TypeSymbol(module_, id_, name_), enumTypeSymbol(nullptr)
{
}

EnumConstantSymbol::EnumConstantSymbol(Module* module_, SymbolId id_) : 
    Symbol(module_, id_), value(nullptr), valueId(zeroSymbolId), enumType(nullptr), enumTypeId(zeroSymbolId)
{
}

EnumConstantSymbol::EnumConstantSymbol(Module* module_, SymbolId id_, const std::string& name_) : 
    Symbol(module_, id_, name_), value(nullptr), valueId(zeroSymbolId), enumType(nullptr), enumTypeId(zeroSymbolId)
{
}

void EnumConstantSymbol::SetEnumType(EnumeratedTypeSymbol* enumType_) noexcept
{
    enumType = enumType_;
}

EnumeratedTypeSymbol* EnumConstantSymbol::GetEnumType(Context* context)
{
    if (enumType)
    {
        return enumType;
    }
    if (IsReadOnly() && enumTypeId != zeroSymbolId)
    {
        enumType = GetModule()->GetSymbolTable()->GetEnumeratedTypeSymbol(enumTypeId, context);
        if (!enumType)
        {
            ThrowException("enumterated type id " + std::to_string(ToUnderlying(enumTypeId)) + " not found");
        }
    }
    return enumType;
}

Value* EnumConstantSymbol::GetValue(Context* context)
{
    if (value)
    {
        return value;
    }
    if (IsReadOnly() && valueId != zeroSymbolId)
    {
        value = GetModule()->GetSymbolTable()->GetValue(valueId, context);
        if (!value)
        {
            ThrowException("enumeration constant value id " + std::to_string(ToUnderlying(valueId)) + " not found");
        }
    }
    return value;
}

void EnumConstantSymbol::SetValue(Value* value_) noexcept
{
    value = value_;
}

void EnumConstantSymbol::Write(Writer& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumType->Id()));
    writer.GetBinaryStreamWriter().Write(ToUnderlying(value->Id()));
}

void EnumConstantSymbol::Read(Reader& reader)
{
    Symbol::Read(reader);
    enumTypeId = SymbolId(reader.CurrentReader().ReadUInt());
    valueId = SymbolId(reader.CurrentReader().ReadUInt());
}

bool EnumTypeLessFunctor::operator()(EnumeratedTypeSymbol* left, EnumeratedTypeSymbol* right) const noexcept
{
    return left->NameOffset() < right->NameOffset();
}

EnumTypeDefaultCtor::EnumTypeDefaultCtor(Module* module_, SymbolId id_) : 
    FunctionSymbol(module_, id_), enumType(nullptr), enumTypeId(zeroSymbolId)
{
}

EnumTypeDefaultCtor::EnumTypeDefaultCtor(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context) : 
    FunctionSymbol(module_, id_, "@enumDefaultCtor"), enumType(enumType_), enumTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(enumType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    SetNoExcept();
}

void EnumTypeDefaultCtor::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumType->Id()));
}

void EnumTypeDefaultCtor::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    enumTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void EnumTypeDefaultCtor::Resolve(Context* context)
{
    if (IsReadOnly() && enumTypeId != zeroSymbolId && !enumType)
    {
        enumType = GetModule()->GetSymbolTable()->GetEnumeratedTypeSymbol(enumTypeId, context);
        if (!enumType)
        {
            ThrowException("enumerated type id " + std::to_string(ToUnderlying(enumTypeId)) + " not found", GetFullSpan(), context);
        }
    }
}

void EnumTypeDefaultCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) 
{
    if ((flags & OperationFlags::defaultInit) != OperationFlags::none)
    {
        Resolve(context);
        emitter.Stack().Push(enumType->IrType(emitter, fullSpan, context)->DefaultValue());
        OperationFlags storeFlags = OperationFlags::none;
        if ((flags & OperationFlags::storeDeref) != OperationFlags::none)
        {
            storeFlags = storeFlags | OperationFlags::deref;
        }
        args[0]->Store(emitter, storeFlags, fullSpan, context);
    }
}

EnumTypeCopyCtor::EnumTypeCopyCtor(Module* module_, SymbolId id_) :
    FunctionSymbol(module_, id_), enumType(nullptr), enumTypeId(zeroSymbolId)
{
}

EnumTypeCopyCtor::EnumTypeCopyCtor(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context) :
    FunctionSymbol(module_, id_, "@enumCopyCtor"), enumType(enumType_), enumTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(enumType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(enumType, context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetNoExcept();
}

void EnumTypeCopyCtor::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumType->Id()));
}

void EnumTypeCopyCtor::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    enumTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void EnumTypeCopyCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    OperationFlags storeFlags = OperationFlags::none;
    if ((flags & OperationFlags::storeDeref) != OperationFlags::none)
    {
        storeFlags = storeFlags | OperationFlags::deref;
    }
    args[0]->Store(emitter, storeFlags, fullSpan, context);
}

EnumTypeMoveCtor::EnumTypeMoveCtor(Module* module_, SymbolId id_) :
    FunctionSymbol(module_, id_), enumType(nullptr), enumTypeId(zeroSymbolId)
{
}

EnumTypeMoveCtor::EnumTypeMoveCtor(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context) :
    FunctionSymbol(module_, id_, "@enumMoveCtor"), enumType(enumType_), enumTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::constructor);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(enumType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(enumType->AddRValueRef(context), context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetNoExcept();
}

void EnumTypeMoveCtor::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumType->Id()));
}

void EnumTypeMoveCtor::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    enumTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void EnumTypeMoveCtor::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitLoad(rvalueRefValue));
    OperationFlags storeFlags = OperationFlags::none;
    if ((flags & OperationFlags::storeDeref) != OperationFlags::none)
    {
        storeFlags = storeFlags | OperationFlags::deref;
    }
    args[0]->Store(emitter, storeFlags, fullSpan, context);
}

EnumTypeCopyAssignment::EnumTypeCopyAssignment(Module* module_, SymbolId id_) :
    FunctionSymbol(module_, id_), enumType(nullptr), enumTypeId(zeroSymbolId)
{
}

EnumTypeCopyAssignment::EnumTypeCopyAssignment(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context) :
    FunctionSymbol(module_, id_, "@enumCopyAssignment"), enumType(enumType_), enumTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::special);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(enumType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(enumType, context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetReturnType(enumType->AddLValueRef(context), context);
    SetNoExcept();
}

void EnumTypeCopyAssignment::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumType->Id()));
}

void EnumTypeCopyAssignment::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    enumTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void EnumTypeCopyAssignment::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    args[0]->Store(emitter, OperationFlags::none, fullSpan, context);
    emitter.Stack().Push(context->Ptr());
}

EnumTypeMoveAssignment::EnumTypeMoveAssignment(Module* module_, SymbolId id_) :
    FunctionSymbol(module_, id_), enumType(nullptr), enumTypeId(zeroSymbolId)
{
}

EnumTypeMoveAssignment::EnumTypeMoveAssignment(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context) :
    FunctionSymbol(module_, id_, "@enumMoveAssignment"), enumType(enumType_), enumTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::special);
    SetAccess(Access::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
    thisParam->SetType(enumType->AddPointer(context), context);
    AddSymbol(thisParam, soul::ast::FullSpan(), context);
    ParameterSymbol* thatParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
    thatParam->SetType(enumType->AddRValueRef(context), context);
    AddSymbol(thatParam, soul::ast::FullSpan(), context);
    SetReturnType(enumType->AddLValueRef(context), context);
    SetNoExcept();
}

void EnumTypeMoveAssignment::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumType->Id()));
}

void EnumTypeMoveAssignment::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    enumTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void EnumTypeMoveAssignment::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* refValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitLoad(refValue));
    args[0]->Store(emitter, OperationFlags::setPtr, fullSpan, context);
    emitter.Stack().Push(context->Ptr());
}

EnumTypeEqual::EnumTypeEqual(Module* module_, SymbolId id_) :
    FunctionSymbol(module_, id_), enumType(nullptr), enumTypeId(zeroSymbolId)
{
}

EnumTypeEqual::EnumTypeEqual(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context) :
    FunctionSymbol(module_, id_, "@enumEqual"), enumType(enumType_), enumTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
    leftParam->SetType(enumType, context);
    AddSymbol(leftParam, soul::ast::FullSpan(), context);
    ParameterSymbol* rightParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
    rightParam->SetType(enumType, context);
    AddSymbol(rightParam, soul::ast::FullSpan(), context);
    SetReturnType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context), context);
    SetNoExcept();
}

void EnumTypeEqual::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumType->Id()));
}

void EnumTypeEqual::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    enumTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void EnumTypeEqual::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, flags, fullSpan, context);
    otava::intermediate::Value* left = emitter.Stack().Pop();
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitEqual(left, right));
}

EnumTypeLess::EnumTypeLess(Module* module_, SymbolId id_) :
    FunctionSymbol(module_, id_), enumType(nullptr), enumTypeId(zeroSymbolId)
{
}

EnumTypeLess::EnumTypeLess(Module* module_, SymbolId id_, EnumeratedTypeSymbol* enumType_, Context* context) :
    FunctionSymbol(module_, id_, "@enumLess"), enumType(enumType_), enumTypeId(zeroSymbolId)
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
    leftParam->SetType(enumType, context);
    AddSymbol(leftParam, soul::ast::FullSpan(), context);
    ParameterSymbol* rightParam = new ParameterSymbol(context->GetModule(), context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
    rightParam->SetType(enumType, context);
    AddSymbol(rightParam, soul::ast::FullSpan(), context);
    SetReturnType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::boolType, context), context);
    SetNoExcept();
}

void EnumTypeLess::Write(Writer& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(ToUnderlying(enumType->Id()));
}

void EnumTypeLess::Read(Reader& reader)
{
    FunctionSymbol::Read(reader);
    enumTypeId = SymbolId(reader.CurrentReader().ReadUInt());
}

void EnumTypeLess::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    args[0]->Load(emitter, flags, fullSpan, context);
    otava::intermediate::Value* left = emitter.Stack().Pop();
    args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
    otava::intermediate::Value* right = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.EmitLess(left, right));
}

class EnumCreator : public otava::ast::DefaultVisitor
{
public:
    EnumCreator(Context* context_, bool createEnumeratedType_, bool createEnumerators_);
    void Visit(otava::ast::EnumSpecifierNode& node) override;
    void Visit(otava::ast::EnumHeadNode& node) override;
    void Visit(otava::ast::EnumBaseNode& node) override;
    void Visit(otava::ast::EnumClassNode& node) override;
    void Visit(otava::ast::EnumStructNode& node) override;
    void Visit(otava::ast::EnumNode& node) override;
    void Visit(otava::ast::EnumeratorDefinitionNode& node) override;
    void Visit(otava::ast::EnumeratorNode& node) override;
    void Visit(otava::ast::ElaboratedEnumSpecifierNode& node) override;
    void Visit(otava::ast::OpaqueEnumDeclarationNode& node) override;
    void Visit(otava::ast::QualifiedIdNode& node) override;
    void Visit(otava::ast::IdentifierNode& node) override;
    void Visit(otava::ast::UnnamedNode& node) override;
private:
    Context* context;
    EnumTypeKind enumTypeKind;
    TypeSymbol* underlyingType;
    Scope* scope;
    Value* value;
    std::int64_t prevValue;
    bool first;
    bool createEnumeratedType;
    bool createEnumerators;
    bool opaque;
};

EnumCreator::EnumCreator(Context* context_, bool createEnumeratedType_, bool createEnumerators_) :
    context(context_),
    enumTypeKind(EnumTypeKind::enum_),
    underlyingType(nullptr),
    scope(context->GetSymbolTable()->CurrentScope()),
    value(nullptr),
    prevValue(0),
    first(false),
    createEnumeratedType(createEnumeratedType_),
    createEnumerators(createEnumerators_),
    opaque(false)
{
}

void EnumCreator::Visit(otava::ast::EnumSpecifierNode& node)
{
    if (createEnumeratedType)
    {
        node.EnumHead()->Accept(*this);
    }
    else if (createEnumerators)
    {
        first = true;
        VisitListContent(node);
    }
}

void EnumCreator::Visit(otava::ast::EnumHeadNode& node)
{
    node.EnumKey()->Accept(*this);
    if (node.EnumBase())
    {
        node.EnumBase()->Accept(*this);
    }
    node.EnumHeadName()->Accept(*this);
}

void EnumCreator::Visit(otava::ast::EnumBaseNode& node)
{
    underlyingType = ResolveType(node.Child(), DeclarationFlags::none, context);
}

void EnumCreator::Visit(otava::ast::EnumClassNode& node)
{
    enumTypeKind = EnumTypeKind::enumClass;
}

void EnumCreator::Visit(otava::ast::EnumStructNode& node)
{
    enumTypeKind = EnumTypeKind::enumStruct;
}

void EnumCreator::Visit(otava::ast::EnumNode& node)
{
    enumTypeKind = EnumTypeKind::enum_;
}

void EnumCreator::Visit(otava::ast::EnumeratorDefinitionNode& node)
{
    Symbol* currentSymbol = context->GetSymbolTable()->CurrentScope()->GetSymbol();
    if (!currentSymbol->IsEnumeratedTypeSymbol())
    {
        ThrowException("otava.symbols.enums: EnumCreator(): enum scope expected", node.GetFullSpan(), context);
    }
    EnumeratedTypeSymbol* enumType = static_cast<EnumeratedTypeSymbol*>(currentSymbol);
    TypeSymbol* valueType = enumType->UnderlyingType(context);
    if (!valueType)
    {
        valueType = context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::intType, context);
    }
    if (node.GetValue())
    {
        Value* val = Evaluate(node.GetValue(), context);
        if (val->IsIntegerValue())
        {
            IntegerValue* intVal = static_cast<IntegerValue*>(val);
            value = context->GetEvaluationContext()->GetIntegerValue(intVal->GetValue(), std::to_string(intVal->GetValue()), valueType, context);
        }
        if (value && value->IsIntegerValue())
        {
            IntegerValue* integerValue = static_cast<IntegerValue*>(value);
            prevValue = integerValue->GetValue();
        }
        if (first)
        {
            first = false;
        }
    }
    else
    {
        if (first)
        {
            value = context->GetEvaluationContext()->GetIntegerValue(0, std::to_string(0), valueType, context);
            prevValue = 0;
            first = false;
        }
        else
        {
            value = context->GetEvaluationContext()->GetIntegerValue(prevValue + 1, std::to_string(prevValue + 1), valueType, context);
            ++prevValue;
        }
    }
    node.Enumerator()->Accept(*this);
}

void EnumCreator::Visit(otava::ast::EnumeratorNode& node)
{
    node.Identifier()->Accept(*this);
}

void EnumCreator::Visit(otava::ast::ElaboratedEnumSpecifierNode& node)
{
    opaque = true;
    node.Child()->Accept(*this);
    opaque = false;
}

void EnumCreator::Visit(otava::ast::OpaqueEnumDeclarationNode& node)
{
    opaque = true;
    node.EnumKey()->Accept(*this);
    if (node.EnumBase())
    {
        node.EnumBase()->Accept(*this);
    }
    node.EnumHeadName()->Accept(*this);
    opaque = false;
}

void EnumCreator::Visit(otava::ast::QualifiedIdNode& node)
{
    scope = ResolveScope(node.Left(), context);
    node.Right()->Accept(*this);
}

void EnumCreator::Visit(otava::ast::IdentifierNode& node)
{
    if (createEnumeratedType)
    {
        context->GetSymbolTable()->BeginScope(scope, context);
        if (opaque)
        {
            context->GetSymbolTable()->AddForwardEnumDeclaration(node.Str(), enumTypeKind, underlyingType, &node, context);
        }
        else
        {
            context->GetSymbolTable()->BeginEnumeratedType(node.Str(), enumTypeKind, underlyingType, &node, context);
        }
    }
    if (createEnumerators)
    {
        context->GetSymbolTable()->AddEnumerator(node.Str(), value, &node, context);
    }
}

void EnumCreator::Visit(otava::ast::UnnamedNode& node)
{
    context->GetSymbolTable()->BeginScope(scope, context);
    context->GetSymbolTable()->BeginEnumeratedType(std::string(), enumTypeKind, underlyingType, &node, context);
}

void BeginEnumType(otava::ast::Node* node, Context* context)
{
    EnumCreator creator(context, true, false);
    node->Accept(creator);
}

void AddEnumerators(otava::ast::Node* node, Context* context)
{
    EnumCreator creator(context, false, true);
    node->Accept(creator);
}

void EndEnumType(otava::ast::Node* node, Context* context)
{
    Symbol* currentSymbol = context->GetSymbolTable()->CurrentScope()->GetSymbol();
    if (!currentSymbol->IsEnumeratedTypeSymbol())
    {
        ThrowException("cpp20.symbols.enums: EndEnumeratedType(): enum scope expected", node->GetFullSpan(), context);
    }
    EnumeratedTypeSymbol* enumType = static_cast<EnumeratedTypeSymbol*>(currentSymbol);
    context->GetSymbolTable()->EndEnumeratedType(context);
    context->GetSymbolTable()->EndScope(context);
    BindEnumType(enumType, node->GetFullSpan(), context);
}

void BindEnumType(EnumeratedTypeSymbol* enumType, const soul::ast::FullSpan& fullSpan, Context* context)
{
    if (enumType->IsBound()) return;
    enumType->SetBound();
    Scope* scope = enumType->GetScope();
    FunctionGroupSymbol* constructorGroup = scope->GetOrInsertFunctionGroup("@constructor", fullSpan, context);
    EnumTypeDefaultCtor* enumTypeDefaultCtor = new EnumTypeDefaultCtor(
        context->GetModule(), context->GetNextSymbolId(SymbolKind::enumTypeDefaultCtor), enumType, context);
    scope->AddSymbol(enumTypeDefaultCtor, fullSpan, context);
    constructorGroup->AddFunction(enumTypeDefaultCtor);
    EnumTypeCopyCtor* enumTypeCopyCtor = new EnumTypeCopyCtor(
        context->GetModule(), context->GetNextSymbolId(SymbolKind::enumTypeCopyCtor), enumType, context);
    scope->AddSymbol(enumTypeCopyCtor, fullSpan, context);
    constructorGroup->AddFunction(enumTypeCopyCtor);
    EnumTypeMoveCtor* enumTypeMoveCtor = new EnumTypeMoveCtor(context->GetModule(), context->GetNextSymbolId(SymbolKind::enumTypeMoveCtor), enumType, context);
    scope->AddSymbol(enumTypeMoveCtor, fullSpan, context);
    constructorGroup->AddFunction(enumTypeMoveCtor);

    FunctionGroupSymbol* destructorGroup = scope->GetOrInsertFunctionGroup("@destructor", fullSpan, context);
    TrivialDestructor* trivialDestructor = new TrivialDestructor(context->GetModule(), context->GetNextSymbolId(SymbolKind::functionSymbol), enumType, context);
    scope->AddSymbol(trivialDestructor, fullSpan, context);
    destructorGroup->AddFunction(trivialDestructor);

    FunctionGroupSymbol* assignmentGroup = scope->GetOrInsertFunctionGroup("operator=", fullSpan, context);
    EnumTypeCopyAssignment* enumTypeCopyAssignment = new EnumTypeCopyAssignment(
        context->GetModule(), context->GetNextSymbolId(SymbolKind::enumTypeCopyAssignment), enumType, context);
    scope->AddSymbol(enumTypeCopyAssignment, fullSpan, context);
    assignmentGroup->AddFunction(enumTypeCopyAssignment);
    EnumTypeMoveAssignment* enumTypeMoveAssignment = new EnumTypeMoveAssignment(
        context->GetModule(), context->GetNextSymbolId(SymbolKind::enumTypeMoveAssignment), enumType, context);
    scope->AddSymbol(enumTypeMoveAssignment, fullSpan, context);
    assignmentGroup->AddFunction(enumTypeMoveAssignment);

    FunctionGroupSymbol* equalGroup = scope->GetOrInsertFunctionGroup("operator==", fullSpan, context);
    EnumTypeEqual* enumTypeEqual = new EnumTypeEqual(
        context->GetModule(), context->GetNextSymbolId(SymbolKind::enumTypeEqual), enumType, context);
    scope->AddSymbol(enumTypeEqual, fullSpan, context);
    equalGroup->AddFunction(enumTypeEqual);

    FunctionGroupSymbol* lessGroup = scope->GetOrInsertFunctionGroup("operator<", fullSpan, context);
    EnumTypeLess* enumTypeLess = new EnumTypeLess(
        context->GetModule(), context->GetNextSymbolId(SymbolKind::enumTypeLess), enumType, context);
    scope->AddSymbol(enumTypeLess, fullSpan, context);
    lessGroup->AddFunction(enumTypeLess);
}

void ProcessEnumForwardDeclaration(otava::ast::Node* node, Context* context)
{
    EnumCreator creator(context, true, false);
    node->Accept(creator);
    context->GetSymbolTable()->EndScope(context);
}

} // namespace otava::symbols
