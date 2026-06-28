// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.symbol_factory;

import otava.symbols.alias_group_symbol;
import otava.symbols.alias_type_symbol;
import otava.symbols.alias_type_templates;
import otava.symbols.array_type_symbol;
import otava.symbols.alias_type_symbol;
import otava.symbols.block;
import otava.symbols.classes;
import otava.symbols.class_group_symbol;
import otava.symbols.class_templates;
import otava.symbols.compound_type_symbol;
import otava.symbols.enums;
import otava.symbols.enum_group_symbol;
import otava.symbols.exception;
import otava.symbols.function_symbol;
import otava.symbols.function_group_symbol;
import otava.symbols.function_type_symbol;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.fundamental_type_conversion;
import otava.symbols.fundamental_type_operation;
import otava.symbols.intrinsics;
import otava.symbols.variable_group_symbol;
import otava.symbols.namespaces;
import otava.symbols.templates;
import otava.symbols.value;
import otava.symbols.variable_symbol;

namespace otava::symbols {

Symbol* MakeSymbol(Module* module, SymbolId symbolId)
{
    SymbolKind kind = GetSymbolKind(symbolId);
    switch (kind)
    {
    case SymbolKind::namespaceSymbol:
    {
        return new NamespaceSymbol(module, symbolId);
    }
    case SymbolKind::fundamentalTypeSymbol:
    {
        return new FundamentalTypeSymbol(module, symbolId);
    }
    case SymbolKind::functionGroupSymbol:
    {
        return new FunctionGroupSymbol(module, symbolId);
    }
    case SymbolKind::variableGroupSymbol:
    {
        return new VariableGroupSymbol(module, symbolId);
    }
    case SymbolKind::aliasGroupSymbol:
    {
        return new AliasGroupSymbol(module, symbolId);
    }
    case SymbolKind::classGroupSymbol:
    {
        return new ClassGroupSymbol(module, symbolId);
    }
    case SymbolKind::enumGroupSymbol:
    {
        return new EnumGroupSymbol(module, symbolId);
    }
    case SymbolKind::boolValueSymbol:
    {
        return new BoolValue(module, symbolId);
    }
    case SymbolKind::integerValueSymbol:
    {
        return new IntegerValue(module, symbolId);
    }
    case SymbolKind::floatingValueSymbol:
    {
        return new FloatingValue(module, symbolId);
    }
    case SymbolKind::nullPtrValueSymbol:
    {
        return new NullPtrValue(module, symbolId);
    }
    case SymbolKind::stringValueSymbol:
    {
        return new StringValue(module, symbolId);
    }
    case SymbolKind::charValueSymbol:
    {
        return new CharValue(module, symbolId);
    }
    case SymbolKind::arrayValueSymbol:
    {
        return new ArrayValue(module, symbolId);
    }
    case SymbolKind::structureValueSymbol:
    {
        return new StructureValue(module, symbolId);
    }
    case SymbolKind::aliasTypeSymbol:
    {
        return new AliasTypeSymbol(module, symbolId);
    }
    case SymbolKind::arrayTypeSymbol:
    {
        return new ArrayTypeSymbol(module, symbolId);
    }
    case SymbolKind::blockSymbol:
    {
        return new BlockSymbol(module, symbolId);
    }
    case SymbolKind::classTypeSymbol:
    {
        return new ClassTypeSymbol(module, symbolId);
    }
    case SymbolKind::forwardClassDeclarationSymbol:
    {
        return new ForwardClassDeclarationSymbol(module, symbolId);
    }
    case SymbolKind::compoundTypeSymbol:
    {
        return new CompoundTypeSymbol(module, symbolId);
    }
    case SymbolKind::explicitInstantiationSymbol:
    {
        return new ExplicitInstantiationSymbol(module, symbolId);
    }
    case SymbolKind::enumTypeSymbol:
    {
        return new EnumeratedTypeSymbol(module, symbolId);
    }
    case SymbolKind::forwardEnumDeclarationSymbol:
    {
        return new ForwardEnumDeclarationSymbol(module, symbolId);
    }
    case SymbolKind::enumConstantSymbol:
    {
        return new EnumConstantSymbol(module, symbolId);
    }
    case SymbolKind::functionSymbol:
    {
        return new FunctionSymbol(module, symbolId);
    }
    case SymbolKind::functionDefinitionSymbol:
    {
        return new FunctionDefinitionSymbol(module, symbolId);
    }
    case SymbolKind::explicitlyInstantiatedFunctionDefinitionSymbol:
    {
        return new ExplicitlyInstantiatedFunctionDefinitionSymbol(module, symbolId);
    }
    case SymbolKind::functionTypeSymbol:
    {
        return new FunctionTypeSymbol(module, symbolId);
    }
    case SymbolKind::templateDeclarationSymbol:
    {
        return new TemplateDeclarationSymbol(module, symbolId);
    }
    case SymbolKind::typenameConstraintSymbol:
    {
        return new TypenameConstraintSymbol(module, symbolId);
    }
    case SymbolKind::templateParameterSymbol:
    {
        return new TemplateParameterSymbol(module, symbolId);
    }
    case SymbolKind::varArgTypeSymbol:
    {
        otava::symbols::SetExceptionThrown();
        throw std::runtime_error("varArgTypeSymbol not implemented");
    }
    case SymbolKind::variableSymbol:
    {
        return new VariableSymbol(module, symbolId);
    }
    case SymbolKind::parameterSymbol:
    {
        return new ParameterSymbol(module, symbolId);
    }
    case SymbolKind::classTemplateSpecializationSymbol:
    {
        ClassTemplateSpecializationSymbol* specialization = new ClassTemplateSpecializationSymbol(module, symbolId);
        return specialization;
    }
    case SymbolKind::aliasTypeTemplateSpecializationSymbol:
    {
        AliasTypeTemplateSpecializationSymbol* specialization = new AliasTypeTemplateSpecializationSymbol(module, symbolId);
        return specialization;
    }
    case SymbolKind::nestedTypeSymbol:
    {
        return new NestedTypeSymbol(module, symbolId);
    }
    case SymbolKind::dependentTypeSymbol:
    {
        return new DependentTypeSymbol(module, symbolId);
    }
    case SymbolKind::symbolValueSymbol:
    {
        return new SymbolValue(module, symbolId);
    }
    case SymbolKind::invokeValueSymbol:
    {
        return new InvokeValue(module, symbolId);
    }
    case SymbolKind::fundamentalTypeNot:
    {
        return new FundamentalTypeNotOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeUnaryPlus:
    {
        return new FundamentalTypeUnaryPlusOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeUnaryMinus:
    {
        return new FundamentalTypeUnaryMinusOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeComplement:
    {
        return new FundamentalTypeComplementOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeAdd:
    {
        return new FundamentalTypeAddOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeSub:
    {
        return new FundamentalTypeSubOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeMul:
    {
        return new FundamentalTypeMulOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeDiv:
    {
        return new FundamentalTypeDivOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeMod:
    {
        return new FundamentalTypeModOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeAnd:
    {
        return new FundamentalTypeAndOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeOr:
    {
        return new FundamentalTypeOrOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeXor:
    {
        return new FundamentalTypeXorOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeShl:
    {
        return new FundamentalTypeShlOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeShr:
    {
        return new FundamentalTypeShrOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypePlusAssign:
    {
        return new FundamentalTypePlusAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeMinusAssign:
    {
        return new FundamentalTypeMinusAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeMulAssign:
    {
        return new FundamentalTypeMulAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeDivAssign:
    {
        return new FundamentalTypeDivAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeModAssign:
    {
        return new FundamentalTypeModAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeAndAssign:
    {
        return new FundamentalTypeAndAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeOrAssign:
    {
        return new FundamentalTypeOrAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeXorAssign:
    {
        return new FundamentalTypeXorAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeShlAssign:
    {
        return new FundamentalTypeShlAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeShrAssign:
    {
        return new FundamentalTypeShrAssignOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeEqual:
    {
        return new FundamentalTypeEqualOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeLess:
    {
        return new FundamentalTypeLessOperation(module, symbolId);
    }
    case SymbolKind::fundamentalTypeBoolean:
    {
        return new FundamentalTypeBooleanConversion(module, symbolId);
    }
    case SymbolKind::fundamentalTypeSignExtension:
    {
        return new FundamentalTypeSignExtendConversion(module, symbolId);
    }
    case SymbolKind::fundamentalTypeZeroExtension:
    {
        return new FundamentalTypeZeroExtendConversion(module, symbolId);
    }
    case SymbolKind::fundamentalTypeFloatingPointExtension:
    {
        return new FundamentalTypeFloatingPointExtendConversion(module, symbolId);
    }
    case SymbolKind::fundamentalTypeTruncate:
    {
        return new FundamentalTypeTruncateConversion(module, symbolId);
    }
    case SymbolKind::fundamentalTypeBitcast:
    {
        return new FundamentalTypeBitcastConversion(module, symbolId);
    }
    case SymbolKind::fundamentalTypeIntToFloat:
    {
        return new FundamentalTypeIntToFloatConversion(module, symbolId);
    }
    case SymbolKind::fundamentalTypeFloatToInt:
    {
        return new FundamentalTypeFloatToIntConversion(module, symbolId);
    }
    case SymbolKind::fundamentalTypeBoolToInt:
    {
        return new FundamentalTypeBoolToIntConversion(module, symbolId);
    }
    case SymbolKind::fundamentalTypeDefaultCtor:
    {
        return new FundamentalTypeDefaultCtor(module, symbolId);
    }
    case SymbolKind::fundamentalTypeCopyCtor:
    {
        return new FundamentalTypeCopyCtor(module, symbolId);
    }
    case SymbolKind::fundamentalTypeMoveCtor:
    {
        return new FundamentalTypeMoveCtor(module, symbolId);
    }
    case SymbolKind::fundamentalTypeCopyAssignment:
    {
        return new FundamentalTypeCopyAssignment(module, symbolId);
    }
    case SymbolKind::fundamentalTypeMoveAssignment:
    {
        return new FundamentalTypeMoveAssignment(module, symbolId);
    }
    case SymbolKind::enumTypeDefaultCtor:
    {
        return new EnumTypeDefaultCtor(module, symbolId);
    }
    case SymbolKind::enumTypeCopyCtor:
    {
        return new EnumTypeCopyCtor(module, symbolId);
    }
    case SymbolKind::enumTypeMoveCtor:
    {
        return new EnumTypeMoveCtor(module, symbolId);
    }
    case SymbolKind::enumTypeCopyAssignment:
    {
        return new EnumTypeCopyAssignment(module, symbolId);
    }
    case SymbolKind::enumTypeMoveAssignment:
    {
        return new EnumTypeMoveAssignment(module, symbolId);
    }
    case SymbolKind::enumTypeEqual:
    {
        return new EnumTypeEqual(module, symbolId);
    }
    case SymbolKind::enumTypeLess:
    {
        return new EnumTypeLess(module, symbolId);
    }
    case SymbolKind::arrayTypeDefaultCtor:
    {
        return new ArrayTypeDefaultCtor(module, symbolId);
    }
    case SymbolKind::arrayTypeCopyCtor:
    {
        return new ArrayTypeCopyCtor(module, symbolId);
    }
    case SymbolKind::arrayTypeMoveCtor:
    {
        return new ArrayTypeMoveCtor(module, symbolId);
    }
    case SymbolKind::arrayTypeCopyAssignment:
    {
        return new ArrayTypeCopyAssignment(module, symbolId);
    }
    case SymbolKind::arrayTypeMoveAssignment:
    {
        return new ArrayTypeMoveAssignment(module, symbolId);
    }
    case SymbolKind::arrayTypeBegin:
    {
        return new ArrayTypeBegin(module, symbolId);
    }
    case SymbolKind::arrayTypeEnd:
    {
        return new ArrayTypeEnd(module, symbolId);
    }
    case SymbolKind::functionGroupTypeSymbol:
    {
        return new FunctionGroupTypeSymbol(module, symbolId);
    }
/*
    case SymbolKind::friendSymbol:
    {
        return new FriendSymbol(module, symbolId);
    }
*/
    case SymbolKind::intrinsicGetRbp:
    {
        return new GetFramePtrIntrinsic(module, symbolId);
    }
    }
    ThrowException("symbol factory: could not make symbol for symbol kind '" + SymbolKindStr(kind) + "': invalid kind");
}

} // namespace otava::symbols
