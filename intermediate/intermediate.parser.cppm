
// this file has been automatically generated from 'D:/work/otava/intermediate/intermediate.parser' using soul parser generator spg version 5.2.0

export module otava.intermediate.parser;

import std;
import soul.lexer;
import soul.parser;
import otava.intermediate.context;
import otava.intermediate.types;
import otava.intermediate.code;
import otava.intermediate.metadata;

export namespace otava::intermediate::parser {

template<typename LexerT>
struct IntermediateParser
{
    static void Parse(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match IntermediateFile(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match TypeDeclarations(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match TypeDeclaration(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match TypeId(LexerT& lexer);
    static soul::parser::Match FundamentalTypeId(LexerT& lexer);
    static soul::parser::Match TypeRef(LexerT& lexer);
    static soul::parser::Match StructureType(LexerT& lexer, otava::intermediate::IntermediateContext* context, std::int32_t typeId);
    static soul::parser::Match ArrayType(LexerT& lexer, otava::intermediate::IntermediateContext* context, std::int32_t typeId);
    static soul::parser::Match FunctionType(LexerT& lexer, otava::intermediate::IntermediateContext* context, std::int32_t typeId);
    static soul::parser::Match DataDefinitions(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match DataDefinition(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match Constant(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match BoolConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match SByteConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match ByteConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match ShortConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match UShortConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match IntConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match UIntConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match LongConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match ULongConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match FloatConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match DoubleConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match AddressConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match ArrayConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match StructureConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match StringConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match StringArrayConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match StringArrayPrefix(LexerT& lexer);
    static soul::parser::Match ConversionConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match ClsIdConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match SymbolConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match Value(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match RegValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match ResultRegValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match SymbolValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match LiteralValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match FunctionDefinitions(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match FunctionDeclaration(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match FunctionDefinition(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match FunctionHeader(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match BasicBlock(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Function* function);
    static soul::parser::Match Label(LexerT& lexer);
    static soul::parser::Match Operand(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match Instructions(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::BasicBlock* basicBlock);
    static soul::parser::Match Instruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match StoreInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match ArgInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match JmpInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match BranchInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match ProcedureCallInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match RetInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match SwitchInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match ValueInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match Operation(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match UnaryInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match BinaryInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match ParamInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match LocalInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PLocalInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match LoadInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match ElemAddrInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PtrOffsetInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PtrDiffInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match GetRbpInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match FunctionCallInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match BlockValue(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match NoOperationInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match Metadata(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match MetadataStruct(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match MetadataField(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::MetadataStruct* parent);
    static soul::parser::Match MetadataItem(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match MetadataBool(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match MetadataLong(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match MetadataString(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match MetadataRef(LexerT& lexer, otava::intermediate::IntermediateContext* context);
};

} // namespace otava::intermediate::parser
