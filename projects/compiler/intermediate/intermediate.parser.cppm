
// this file has been automatically generated from 'D:/work/otava/projects/compiler/intermediate/intermediate.parser' using soul parser generator ospg version 0.1.0

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
    static soul::parser::Match PIntermediateFile(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PTypeDeclarations(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PTypeDeclaration(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PTypeId(LexerT& lexer);
    static soul::parser::Match PFundamentalTypeId(LexerT& lexer);
    static soul::parser::Match PTypeRef(LexerT& lexer);
    static soul::parser::Match PStructureType(LexerT& lexer, otava::intermediate::IntermediateContext* context, std::int32_t typeId);
    static soul::parser::Match PArrayType(LexerT& lexer, otava::intermediate::IntermediateContext* context, std::int32_t typeId);
    static soul::parser::Match PFunctionType(LexerT& lexer, otava::intermediate::IntermediateContext* context, std::int32_t typeId);
    static soul::parser::Match PDataDefinitions(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PDataDefinition(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match PBoolConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PSByteConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PByteConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PShortConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PUShortConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PIntConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PUIntConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PLongConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PULongConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PFloatConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PDoubleConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PAddressConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PArrayConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match PStructureConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match PStringConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PStringArrayConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PStringArrayPrefix(LexerT& lexer);
    static soul::parser::Match PConversionConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PClsIdConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PSymbolConstant(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match PRegValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match PResultRegValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match PSymbolValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match PLiteralValue(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Type* type);
    static soul::parser::Match PFunctionDefinitions(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PFunctionDeclaration(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PFunctionDefinition(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PFunctionHeader(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PBasicBlock(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::Function* function);
    static soul::parser::Match PLabel(LexerT& lexer);
    static soul::parser::Match POperand(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PInstructions(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::BasicBlock* basicBlock);
    static soul::parser::Match PInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PStoreInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PArgInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PJmpInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PBranchInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PProcedureCallInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PRetInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PSwitchInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PValueInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match POperation(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PUnaryInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PBinaryInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PParamInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PLocalInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PPLocalInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PLoadInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PElemAddrInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PPtrOffsetInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PPtrDiffInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PGetRbpInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PFunctionCallInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::RegValue* result);
    static soul::parser::Match PBlockValue(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PNoOperationInstruction(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PMetadata(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PMetadataStruct(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PMetadataField(LexerT& lexer, otava::intermediate::IntermediateContext* context, otava::intermediate::MetadataStruct* parent);
    static soul::parser::Match PMetadataItem(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PMetadataBool(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PMetadataLong(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PMetadataString(LexerT& lexer, otava::intermediate::IntermediateContext* context);
    static soul::parser::Match PMetadataRef(LexerT& lexer, otava::intermediate::IntermediateContext* context);
};

} // namespace otava::intermediate::parser
