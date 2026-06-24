// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.symbol;

import otava.symbols.emitter;
import otava.symbols.modules;
import otava.symbols.writer;
import otava.symbols.reader;
import otava.symbols.exception;
import otava.symbols.symbol_table;
import otava.symbols.modules;
import util.binary_stream_writer;

namespace otava::symbols {
    
std::string AccessStr(Access access)
{
    switch (access)
    {
    case Access::private_:
    {
        return "private";
    }
    case Access::protected_:
    {
        return "protected";
    }
    case Access::public_:
    {
        return "public";
    }
    }
    return std::string();
}

std::string SymbolKindStr(SymbolKind kind)
{
    switch (kind)
    {
    case SymbolKind::null: return "null";
    case SymbolKind::classGroupSymbol: return "classGroup";
    case SymbolKind::conceptGroupSymbol: return "conceptGroup";
    case SymbolKind::functionGroupSymbol: return "functionGroup";
    case SymbolKind::variableGroupSymbol: return "variableGroup";
    case SymbolKind::aliasGroupSymbol: return "aliasGroup";
    case SymbolKind::enumGroupSymbol: return "enumGroup";
    case SymbolKind::boolValueSymbol: return "boolValue";
    case SymbolKind::integerValueSymbol: return "integerValue";
    case SymbolKind::floatingValueSymbol: return "floatingValue";
    case SymbolKind::stringValueSymbol: return "stringValue";
    case SymbolKind::charValueSymbol: return "charValue";
    case SymbolKind::nullPtrTypeSymbol: return "nullPtrType";
    case SymbolKind::arrayValueSymbol: return "arrayValue";
    case SymbolKind::structureValueSymbol: return "structureValue";
    case SymbolKind::aliasTypeSymbol: return "aliasType";
    case SymbolKind::arrayTypeSymbol: return "arrayType";
    case SymbolKind::blockSymbol: return "block";
    case SymbolKind::classTypeSymbol: return "classType";
    case SymbolKind::compoundTypeSymbol: return "compoundType";
    case SymbolKind::conceptSymbol: return "concept";
    case SymbolKind::enumTypeSymbol: return "enumType";
    case SymbolKind::enumConstantSymbol: return "enumConstant";
    case SymbolKind::functionSymbol: return "function";
    case SymbolKind::functionTypeSymbol: return "functionType";
    case SymbolKind::functionDefinitionSymbol: return "functionDefinition";
    case SymbolKind::explicitlyInstantiatedFunctionDefinitionSymbol: return "explicitlyInstantiatedFunctionDefinition";
    case SymbolKind::fundamentalTypeSymbol: return "fundamentalType";
    case SymbolKind::namespaceSymbol: return "namespace";
    case SymbolKind::templateDeclarationSymbol: return "templateDeclaration";
    case SymbolKind::typenameConstraintSymbol: return "typenameConstraint";
    case SymbolKind::explicitInstantiationSymbol: return "explicitInstantiation";
    case SymbolKind::templateParameterSymbol: return "templateParameter";
    case SymbolKind::templateParamGroupSymbol: return "templateParamGroup";
    case SymbolKind::varArgTypeSymbol: return "varArgType";
    case SymbolKind::variableSymbol: return "variable";
    case SymbolKind::parameterSymbol: return "parameter";
    case SymbolKind::classTemplateSpecializationSymbol: return "classTemplateSpecialization";
    case SymbolKind::aliasTypeTemplateSpecializationSymbol: return "aliasTypeTemplateSpecialization";
    case SymbolKind::nestedTypeSymbol: return "nestedType";
    case SymbolKind::dependentTypeSymbol: return "dependentType";
    case SymbolKind::nullPtrValueSymbol: return "nullPtrValue";
    case SymbolKind::symbolValueSymbol: return "symbolValue";
    case SymbolKind::invokeValueSymbol: return "invokeValue";
    case SymbolKind::forwardClassDeclarationSymbol: return "forwardClassDeclaration";
    case SymbolKind::forwardEnumDeclarationSymbol: return "forwardEnumDeclaration";
    case SymbolKind::boundTemplateParameterSymbol: return "boundTemplateParameter";
    case SymbolKind::constraintExprSymbol: return "constraintExpr";
    case SymbolKind::fundamentalTypeNot: return "fundamentalTypeNot";
    case SymbolKind::fundamentalTypeUnaryPlus: return "fundamentalTypeUnaryPlus";
    case SymbolKind::fundamentalTypeUnaryMinus: return "fundamentalTypeUnaryMinus";
    case SymbolKind::fundamentalTypeComplement: return "fundamentalTypeComplement";
    case SymbolKind::fundamentalTypeAdd: return "fundamentalTypeAdd";
    case SymbolKind::fundamentalTypeSub: return "fundamentalTypeSub";
    case SymbolKind::fundamentalTypeMul: return "fundamentalTypeMul";
    case SymbolKind::fundamentalTypeDiv: return "fundamentalTypeDiv";
    case SymbolKind::fundamentalTypeMod: return "fundamentalTypeMod";
    case SymbolKind::fundamentalTypeAnd: return "fundamentalTypeAnd";
    case SymbolKind::fundamentalTypeOr: return "fundamentalTypeOr";
    case SymbolKind::fundamentalTypeXor: return "fundamentalTypeXor";
    case SymbolKind::fundamentalTypeShl: return "fundamentalTypeShl";
    case SymbolKind::fundamentalTypeShr: return "fundamentalTypeShr";
    case SymbolKind::fundamentalTypePlusAssign: return "fundamentalTypePlusAssign";
    case SymbolKind::fundamentalTypeMinusAssign: return "fundamentalTypeMinusAssign";
    case SymbolKind::fundamentalTypeMulAssign: return "fundamentalTypeMulAssign";
    case SymbolKind::fundamentalTypeDivAssign: return "fundamentalTypeDivAssign";
    case SymbolKind::fundamentalTypeModAssign: return "fundamentalTypeModAssign";
    case SymbolKind::fundamentalTypeAndAssign: return "fundamentalTypeAndAssign";
    case SymbolKind::fundamentalTypeOrAssign: return "fundamentalTypeOrAssign";
    case SymbolKind::fundamentalTypeXorAssign: return "fundamentalTypeXorAssign";
    case SymbolKind::fundamentalTypeShlAssign: return "fundamentalTypeShlAssign";
    case SymbolKind::fundamentalTypeShrAssign: return "fundamentalTypeShrAssign";
    case SymbolKind::fundamentalTypeEqual: return "fundamentalTypeEqual";
    case SymbolKind::fundamentalTypeLess: return "fundamentalTypeLess";
    case SymbolKind::fundamentalTypeBoolean: return "fundamentalTypeBoolean";
    case SymbolKind::fundamentalTypeSignExtension: return "fundamentalTypeSignExtension";
    case SymbolKind::fundamentalTypeZeroExtension: return "fundamentalTypeZeroExtension";
    case SymbolKind::fundamentalTypeFloatingPointExtension: return "fundamentalTypeFloatingPointExtension";
    case SymbolKind::fundamentalTypeTruncate: return "fundamentalTypeTruncate";
    case SymbolKind::fundamentalTypeBitcast: return "fundamentalTypeBitcast";
    case SymbolKind::fundamentalTypeIntToFloat: return "fundamentalTypeIntToFloat";
    case SymbolKind::fundamentalTypeFloatToInt: return "fundamentalTypeFloatToInt";
    case SymbolKind::fundamentalTypeBoolToInt: return "fundamentalTypeBoolToInt";
    case SymbolKind::fundamentalTypeDefaultCtor: return "fundamentalTypeDefaultCtor";
    case SymbolKind::fundamentalTypeCopyCtor: return "fundamentalTypeCopyCtor";
    case SymbolKind::fundamentalTypeCopyCtorLiteral: return "fundamentalTypeCopyCtorLiteral";
    case SymbolKind::fundamentalTypeMoveCtor: return "fundamentalTypeMoveCtor";
    case SymbolKind::fundamentalTypeCopyAssignment: return "fundamentalTypeCopyAssignment";
    case SymbolKind::fundamentalTypeMoveAssignment: return "fundamentalTypeMoveAssignment";
    case SymbolKind::enumTypeDefaultCtor: return "enumTypeDefaultCtor";
    case SymbolKind::enumTypeCopyCtor: return "enumTypeCopyCtor";
    case SymbolKind::enumTypeMoveCtor: return "enumTypeMoveCtor";
    case SymbolKind::enumTypeCopyAssignment: return "enumTypeCopyAssignment";
    case SymbolKind::enumTypeMoveAssignment: return "enumTypeMoveAssignment";
    case SymbolKind::enumTypeEqual: return "enumTypeEqual";
    case SymbolKind::enumTypeLess: return "enumTypeLess";
    case SymbolKind::arrayTypeDefaultCtor: return "arrayTypeDefaultCtor";
    case SymbolKind::arrayTypeCopyCtor: return "arrayTypeCopyCtor";
    case SymbolKind::arrayTypeMoveCtor: return "arrayTypeMoveCtor";
    case SymbolKind::arrayTypeCopyAssignment: return "arrayTypeCopyAssignment";
    case SymbolKind::arrayTypeMoveAssignment: return "arrayTypeMoveAssignment";
    case SymbolKind::arrayTypeBegin: return "arrayTypeBegin";
    case SymbolKind::arrayTypeEnd: return "arrayTypeEnd";
    case SymbolKind::defaultBool: return "defaultBool";
    case SymbolKind::defaultSByte: return "defaultSByte";
    case SymbolKind::defaultShort: return "defaultShort";
    case SymbolKind::defaultUShort: return "defaultUShort";
    case SymbolKind::defaultInt: return "defaultInt";
    case SymbolKind::defaultUInt: return "defaultUInt";
    case SymbolKind::defaultLong: return "defaultLong";
    case SymbolKind::defaultULong: return "defaultULong";
    case SymbolKind::defaultFloat: return "defaultFloat";
    case SymbolKind::defaultDouble: return "defaultDouble";
    case SymbolKind::defaultChar: return "defaultChar";
    case SymbolKind::defaultChar16: return "defaultChar16";
    case SymbolKind::defaultChar32: return "defaultChar32";
    case SymbolKind::functionGroupTypeSymbol: return "functionGroupType";
    case SymbolKind::classGroupTypeSymbol: return "classGroupType";
    case SymbolKind::aliasGroupTypeSymbol: return "aliasGroupType";
    case SymbolKind::friendSymbol: return "friend";
    case SymbolKind::namespaceTypeSymbol: return "namespaceType";
    case SymbolKind::intrinsicGetRbp: return "intrinsicGetRbp";
    }
    return "<symbol>";
}

Symbol::Symbol(Module* module_, SymbolId id_) : 
    module(module_), id(id_), flags(SymbolFlags::readOnly), nameOffset(StringOffset(0)), name(""),
    kind(GetSymbolKind(id)), parentId(zeroSymbolId), parent(nullptr), declarationFlags(DeclarationFlags::none), access(Access::none), astNodeId(-1)
{
}

Symbol::Symbol(Module* module_, SymbolId id_, const std::string& name_) : 
    module(module_), id(id_), flags(SymbolFlags::project), nameOffset(module->GetStringTable()->AddString(name_)), name(module->GetStringTable()->CharPtr(nameOffset)),
    kind(GetSymbolKind(id)), parentId(zeroSymbolId), parent(nullptr), declarationFlags(DeclarationFlags::none), access(Access::none), astNodeId(-1)
{
}

Symbol::~Symbol()
{
}

void* Symbol::IrObject(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
    return emitter.GetIrObject(this);
}

bool Symbol::IsExtern() const noexcept
{
    return (declarationFlags & DeclarationFlags::externFlag) != DeclarationFlags::none;
}

std::string Symbol::Name() const
{
    return GetModule()->GetStringTable()->GetString(nameOffset);
}

void Symbol::SetName(const std::string& name_)
{
    nameOffset = GetModule()->GetStringTable()->AddString(name_);
}

std::string Symbol::FullName(Context* context) const
{
    std::string fullName;
    Symbol* p = Parent(context);
    if (p)
    {
        fullName = p->FullName(context);
    }
    std::string name = Name();
    if (!fullName.empty() && !name.empty())
    {
        fullName.append("::");
    }
    if (!name.empty())
    {
        fullName.append(name);
    }
    return fullName;
}

Symbol* Symbol::Parent(Context* context) const
{
    if (parent)
    {
        return parent;
    }
    if (IsReadOnly())
    {
        if (parentId != zeroSymbolId)
        {
            parent = GetModule()->GetSymbolTable()->GetSymbol(parentId, context);
        }
    }
    return parent;
}

void Symbol::SetParent(Symbol* parent_)
{
    bool isExplicitlyInstantiatedFunctionDefinitionSymbol = IsExplicitlyInstantiatedFunctionDefinitionSymbol();
    if (IsReadOnly() && !isExplicitlyInstantiatedFunctionDefinitionSymbol)
    {
        ThrowException("symbol: cannot set parent because symbol is read-only");
    }
    parent = parent_;
}

bool Symbol::IsTemplateParameterInstantiation(Context* context, std::set<const Symbol*>& visited) const
{
    const Symbol* thisSymbol = this;
    if (visited.find(thisSymbol) == visited.end())
    {
        visited.insert(thisSymbol);
    }
    return false;
}

FunctionSymbol* Symbol::ParentFunction(Context* context) const noexcept
{
    Symbol* parentSymbol = Parent(context);
    while (parentSymbol)
    {
        if (parentSymbol->IsFunctionSymbol())
        {
            return static_cast<FunctionSymbol*>(parentSymbol);
        }
        parentSymbol = parentSymbol->Parent(context);
    }
    return nullptr;
}

ClassTypeSymbol* Symbol::ParentClassType(Context* context) const noexcept
{
    Symbol* parentSymbol = Parent(context);
    while (parentSymbol)
    {
        if (parentSymbol->IsClassTypeSymbol())
        {
            return static_cast<ClassTypeSymbol*>(parentSymbol);
        }
        parentSymbol = parentSymbol->Parent(context);
    }
    return nullptr;
}

NamespaceSymbol* Symbol::ParentNamespace(Context* context) const noexcept
{
    Symbol* parentSymbol = Parent(context);
    while (parentSymbol)
    {
        if (parentSymbol->IsNamespaceSymbol())
        {
            return static_cast<NamespaceSymbol*>(parentSymbol);
        }
        parentSymbol = parentSymbol->Parent(context);
    }
    return nullptr;
}

bool Symbol::IsLocalVariableSymbol(Context* context) 
{
    if (IsVariableSymbol())
    {
        VariableSymbol* symbol = static_cast<VariableSymbol*>(this);
        return symbol->IsLocalVariable(context);
    }
    return false;
}

bool Symbol::IsMemberVariableSymbol(Context* context) 
{
    if (IsVariableSymbol())
    {
        VariableSymbol* symbol = static_cast<VariableSymbol*>(this);
        return symbol->IsMemberVariable(context);
    }
    return false;
}

bool Symbol::IsGlobalVariableSymbol(Context* context) 
{
    if (IsVariableSymbol())
    {
        VariableSymbol* symbol = static_cast<VariableSymbol*>(this);
        return symbol->IsGlobalVariable(context);
    }
    return false;
}

bool Symbol::IsTypeSymbol() const noexcept
{
    switch (kind)
    {
    case SymbolKind::aliasTypeSymbol:
    case SymbolKind::arrayTypeSymbol:
    case SymbolKind::classTypeSymbol:
    case SymbolKind::forwardClassDeclarationSymbol:
    case SymbolKind::classTemplateSpecializationSymbol:
    case SymbolKind::aliasTypeTemplateSpecializationSymbol:
    case SymbolKind::compoundTypeSymbol:
    case SymbolKind::enumTypeSymbol:
    case SymbolKind::forwardEnumDeclarationSymbol:
    case SymbolKind::nestedTypeSymbol:
    case SymbolKind::dependentTypeSymbol:
    case SymbolKind::functionTypeSymbol:
    case SymbolKind::fundamentalTypeSymbol:
    case SymbolKind::nullPtrTypeSymbol:
    case SymbolKind::templateParameterSymbol:
    case SymbolKind::templateParamGroupSymbol:
    case SymbolKind::boundTemplateParameterSymbol:
    case SymbolKind::varArgTypeSymbol:
    case SymbolKind::namespaceTypeSymbol:
    {
        return true;
    }
    }
    return false;
}

bool Symbol::IsFunctionSymbol() const noexcept
{
    switch (kind)
    {
    case SymbolKind::functionSymbol:
    case SymbolKind::functionDefinitionSymbol:
    case SymbolKind::fundamentalTypeNot:
    case SymbolKind::fundamentalTypeUnaryPlus:
    case SymbolKind::fundamentalTypeUnaryMinus:
    case SymbolKind::fundamentalTypeComplement:
    case SymbolKind::fundamentalTypeAdd:
    case SymbolKind::fundamentalTypeSub:
    case SymbolKind::fundamentalTypeMul:
    case SymbolKind::fundamentalTypeDiv:
    case SymbolKind::fundamentalTypeMod:
    case SymbolKind::fundamentalTypeAnd:
    case SymbolKind::fundamentalTypeOr:
    case SymbolKind::fundamentalTypeXor:
    case SymbolKind::fundamentalTypeShl:
    case SymbolKind::fundamentalTypeShr:
    case SymbolKind::fundamentalTypePlusAssign:
    case SymbolKind::fundamentalTypeMinusAssign:
    case SymbolKind::fundamentalTypeMulAssign:
    case SymbolKind::fundamentalTypeDivAssign:
    case SymbolKind::fundamentalTypeModAssign:
    case SymbolKind::fundamentalTypeAndAssign:
    case SymbolKind::fundamentalTypeOrAssign:
    case SymbolKind::fundamentalTypeXorAssign:
    case SymbolKind::fundamentalTypeShlAssign:
    case SymbolKind::fundamentalTypeShrAssign:
    case SymbolKind::fundamentalTypeEqual:
    case SymbolKind::fundamentalTypeLess:
    case SymbolKind::fundamentalTypeSignExtension:
    case SymbolKind::fundamentalTypeZeroExtension:
    case SymbolKind::fundamentalTypeFloatingPointExtension:
    case SymbolKind::fundamentalTypeTruncate:
    case SymbolKind::fundamentalTypeBitcast:
    case SymbolKind::fundamentalTypeIntToFloat:
    case SymbolKind::fundamentalTypeFloatToInt:
    case SymbolKind::fundamentalTypeBoolToInt:
    case SymbolKind::fundamentalTypeBoolean:
    case SymbolKind::fundamentalTypeDefaultCtor:
    case SymbolKind::fundamentalTypeCopyCtor:
    case SymbolKind::fundamentalTypeMoveCtor:
    case SymbolKind::fundamentalTypeCopyAssignment:
    case SymbolKind::fundamentalTypeMoveAssignment:
    case SymbolKind::enumTypeDefaultCtor:
    case SymbolKind::enumTypeCopyCtor:
    case SymbolKind::enumTypeMoveCtor:
    case SymbolKind::enumTypeCopyAssignment:
    case SymbolKind::enumTypeMoveAssignment:
    case SymbolKind::enumTypeEqual:
    case SymbolKind::enumTypeLess:
    case SymbolKind::arrayTypeDefaultCtor:
    case SymbolKind::arrayTypeCopyCtor:
    case SymbolKind::arrayTypeMoveCtor:
    case SymbolKind::arrayTypeCopyAssignment:
    case SymbolKind::arrayTypeMoveAssignment:
    case SymbolKind::arrayTypeBegin:
    case SymbolKind::arrayTypeEnd:
    case SymbolKind::intrinsicGetRbp:
    {
        return true;
    }
    }
    return false;
}

bool Symbol::IsValueSymbol() const noexcept
{
    switch (kind)
    {
    case SymbolKind::boolValueSymbol:
    case SymbolKind::integerValueSymbol:
    case SymbolKind::floatingValueSymbol:
    case SymbolKind::nullPtrValueSymbol:
    case SymbolKind::stringValueSymbol:
    case SymbolKind::charValueSymbol:
    case SymbolKind::symbolValueSymbol:
    case SymbolKind::invokeValueSymbol:
    case SymbolKind::arrayValueSymbol:
    case SymbolKind::structureValueSymbol:
    {
        return true;
    }
    }
    return false;
}

SymbolGroupKind Symbol::GetSymbolGroupKind() const noexcept
{
    switch (kind)
    {
    case SymbolKind::namespaceSymbol:
    {
        return SymbolGroupKind::namespaceSymbolGroup;
    }
    case SymbolKind::aliasGroupSymbol:
    {
        return SymbolGroupKind::aliasSymbolGroup;
    }
    case SymbolKind::classGroupSymbol:
    {
        return SymbolGroupKind::classSymbolGroup;
    }
    case SymbolKind::enumGroupSymbol:
    {
        return SymbolGroupKind::enumSymbolGroup;
    }
    case SymbolKind::templateParameterSymbol:
    case SymbolKind::templateParamGroupSymbol:
    case SymbolKind::boundTemplateParameterSymbol:
    {
        return SymbolGroupKind::templateParamSymbolGroup;
    }
    case SymbolKind::aliasTypeSymbol:
    {
        return SymbolGroupKind::aliasSymbolGroup;
    }
    case SymbolKind::classTypeSymbol:
    {
        return SymbolGroupKind::classSymbolGroup;
    }
    //case SymbolKind::functionTypeSymbol:
    //case SymbolKind::forwardClassDeclarationSymbol:
    //case SymbolKind::compoundTypeSymbol:
    case SymbolKind::enumTypeSymbol:
    case SymbolKind::forwardEnumDeclarationSymbol:
    {
        return SymbolGroupKind::enumSymbolGroup;
    }
    //case SymbolKind::fundamentalTypeSymbol:
    //case SymbolKind::templateParameterSymbol:
    //case SymbolKind::boundTemplateParameterSymbol:
    //case SymbolKind::nestedTypeSymbol:
    //{
        //return SymbolGroupKind::typeSymbolGroup;
    //}
    case SymbolKind::functionGroupSymbol:
    case SymbolKind::functionSymbol:
    case SymbolKind::functionDefinitionSymbol:
    case SymbolKind::fundamentalTypeNot:
    case SymbolKind::fundamentalTypeUnaryPlus:
    case SymbolKind::fundamentalTypeUnaryMinus:
    case SymbolKind::fundamentalTypeComplement:
    case SymbolKind::fundamentalTypeAdd:
    case SymbolKind::fundamentalTypeSub:
    case SymbolKind::fundamentalTypeMul:
    case SymbolKind::fundamentalTypeDiv:
    case SymbolKind::fundamentalTypeMod:
    case SymbolKind::fundamentalTypeAnd:
    case SymbolKind::fundamentalTypeOr:
    case SymbolKind::fundamentalTypeXor:
    case SymbolKind::fundamentalTypeShl:
    case SymbolKind::fundamentalTypeShr:
    case SymbolKind::fundamentalTypeEqual:
    case SymbolKind::fundamentalTypeLess:
    case SymbolKind::fundamentalTypeBoolean:
    case SymbolKind::fundamentalTypePlusAssign:
    case SymbolKind::fundamentalTypeMinusAssign:
    case SymbolKind::fundamentalTypeMulAssign:
    case SymbolKind::fundamentalTypeDivAssign:
    case SymbolKind::fundamentalTypeModAssign:
    case SymbolKind::fundamentalTypeAndAssign:
    case SymbolKind::fundamentalTypeOrAssign:
    case SymbolKind::fundamentalTypeXorAssign:
    case SymbolKind::fundamentalTypeShlAssign:
    case SymbolKind::fundamentalTypeShrAssign:
    {
        return SymbolGroupKind::functionSymbolGroup;
    }
    case SymbolKind::variableGroupSymbol:
    case SymbolKind::variableSymbol:
    case SymbolKind::parameterSymbol:
    case SymbolKind::enumConstantSymbol:
    {
        return SymbolGroupKind::variableSymbolGroup;
    }
    }
    return SymbolGroupKind::none;
}

bool Symbol::CanInstall() const noexcept
{
    switch (kind)
    {
    case SymbolKind::aliasTypeSymbol:
    case SymbolKind::blockSymbol:
    case SymbolKind::classTypeSymbol:
    case SymbolKind::classTemplateSpecializationSymbol:
    case SymbolKind::conceptSymbol:
    case SymbolKind::compoundTypeSymbol:
    case SymbolKind::enumTypeSymbol:
    case SymbolKind::functionSymbol:
    case SymbolKind::functionDefinitionSymbol:
    case SymbolKind::fundamentalTypeUnaryPlus:
    case SymbolKind::fundamentalTypeUnaryMinus:
    case SymbolKind::fundamentalTypeComplement:
    case SymbolKind::fundamentalTypeNot:
    case SymbolKind::fundamentalTypeAdd:
    case SymbolKind::fundamentalTypeSub:
    case SymbolKind::fundamentalTypeMul:
    case SymbolKind::fundamentalTypeDiv:
    case SymbolKind::fundamentalTypeMod:
    case SymbolKind::fundamentalTypeAnd:
    case SymbolKind::fundamentalTypeOr:
    case SymbolKind::fundamentalTypeXor:
    case SymbolKind::fundamentalTypeShl:
    case SymbolKind::fundamentalTypeShr:
    case SymbolKind::fundamentalTypePlusAssign:
    case SymbolKind::fundamentalTypeMinusAssign:
    case SymbolKind::fundamentalTypeMulAssign:
    case SymbolKind::fundamentalTypeDivAssign:
    case SymbolKind::fundamentalTypeModAssign:
    case SymbolKind::fundamentalTypeAndAssign:
    case SymbolKind::fundamentalTypeOrAssign:
    case SymbolKind::fundamentalTypeXorAssign:
    case SymbolKind::fundamentalTypeShlAssign:
    case SymbolKind::fundamentalTypeShrAssign:
    case SymbolKind::fundamentalTypeEqual:
    case SymbolKind::fundamentalTypeLess:
    case SymbolKind::fundamentalTypeSignExtension:
    case SymbolKind::fundamentalTypeZeroExtension:
    case SymbolKind::fundamentalTypeTruncate:
    case SymbolKind::fundamentalTypeBitcast:
    case SymbolKind::fundamentalTypeIntToFloat:
    case SymbolKind::fundamentalTypeFloatToInt:
    case SymbolKind::fundamentalTypeBoolToInt:
    case SymbolKind::fundamentalTypeBoolean:
    case SymbolKind::fundamentalTypeDefaultCtor:
    case SymbolKind::fundamentalTypeCopyCtor:
    case SymbolKind::fundamentalTypeMoveCtor:
    case SymbolKind::fundamentalTypeCopyAssignment:
    case SymbolKind::fundamentalTypeMoveAssignment:
    case SymbolKind::enumTypeDefaultCtor:
    case SymbolKind::enumTypeCopyCtor:
    case SymbolKind::enumTypeMoveCtor:
    case SymbolKind::enumTypeCopyAssignment:
    case SymbolKind::enumTypeMoveAssignment:
    case SymbolKind::enumTypeEqual:
    case SymbolKind::enumTypeLess:
    case SymbolKind::arrayTypeDefaultCtor:
    case SymbolKind::arrayTypeCopyCtor:
    case SymbolKind::arrayTypeMoveCtor:
    case SymbolKind::arrayTypeCopyAssignment:
    case SymbolKind::arrayTypeMoveAssignment:
    case SymbolKind::arrayTypeBegin:
    case SymbolKind::arrayTypeEnd:
    case SymbolKind::templateDeclarationSymbol:
    case SymbolKind::typenameConstraintSymbol:
    case SymbolKind::forwardClassDeclarationSymbol:
    case SymbolKind::friendSymbol:
    case SymbolKind::variableSymbol:
    case SymbolKind::intrinsicGetRbp:
    case SymbolKind::templateParameterSymbol:
    {
        return false;
    }
    case SymbolKind::parameterSymbol:
    case SymbolKind::functionTypeSymbol:
    case SymbolKind::boundTemplateParameterSymbol:
    {
        return !Name().empty();
    }
    }
    return true;
}

void Symbol::Write(Writer& writer)
{
    util::BinaryStreamWriter& binaryStreamWriter = writer.GetBinaryStreamWriter();
    binaryStreamWriter.Write(ToUnderlying(nameOffset));
    binaryStreamWriter.Write(ToUnderlying(flags & ~SymbolFlags::project));
    if (parent)
    {
        binaryStreamWriter.Write(ToUnderlying(parent->Id()));
    }
    else
    {
        binaryStreamWriter.Write(ToUnderlying(zeroSymbolId));
    }
    binaryStreamWriter.Write(astNodeId);
    writer.Write(fullSpan);
    binaryStreamWriter.Write(ToUnderlying(declarationFlags));
}

void Symbol::Read(Reader& reader)
{
    nameOffset = StringOffset(reader.CurrentReader().ReadUInt());
    name = GetModule()->GetStringTable()->CharPtr(nameOffset);
    flags = SymbolFlags(reader.CurrentReader().ReadByte());
    parentId = SymbolId(reader.CurrentReader().ReadUInt());
    astNodeId = reader.CurrentReader().ReadLong();
    fullSpan = reader.ReadFullSpan();
    declarationFlags = DeclarationFlags(reader.CurrentReader().ReadInt());
}

std::string Symbol::IrName(Context* context) const
{
    return Name();
}

void Symbol::Expand(Context* context)
{
}

void Symbol::AddModuleSymbolId(const ModuleSymbolId& moduleSymbolId)
{
    if (moduleSymbolId.moduleId == ModuleId(1))
    {
        int x = 0;
    }
    moduleSymbolIds.push_back(moduleSymbolId);
}

} // namespace otava::symbols
