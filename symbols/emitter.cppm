// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.emitter;

import otava.symbols.ir_value_stack;
import otava.symbols.id;
import otava.intermediate.context;
import otava.intermediate.code;
import otava.intermediate.data;
import otava.intermediate.types;
import otava.intermediate.metadata;
import util.uuid;
import std;

export namespace otava::symbols {

class Emitter
{
public:
    Emitter();
    ~Emitter();
    inline void SetCheck() noexcept { check = true; }
    inline void SetFilePath(const std::string& filePath) { context->SetFilePath(filePath); }
    inline const std::string& FilePath() const noexcept { return context->FilePath(); }
    void SetCompileUnitInfo(const std::string& compileUnitId, const std::string& sourceFilePath);
    inline otava::intermediate::IntermediateContext* GetIntermediateContext() noexcept { return context; }
    void ResolveReferences();
    void Emit();
    otava::intermediate::Function* CreateFunction(const std::string& name, otava::intermediate::Type* type, bool inline_, bool once, bool child);
    void SetRegNumbers();
    inline otava::intermediate::Function* GetOrInsertFunction(const std::string& name, otava::intermediate::FunctionType* functionType)
    {
        return context->GetOrInsertFunction(name, functionType);
    }
    inline otava::intermediate::BasicBlock* CreateBasicBlock()
    {
        otava::intermediate::BasicBlock* bb = context->CurrentFunction()->CreateBasicBlock();
        return bb;
    }
    inline void SetCurrentBasicBlock(otava::intermediate::BasicBlock* bb) noexcept { context->SetCurrentBasicBlock(bb); }
    otava::intermediate::Type* MakeStructureType(const std::vector<otava::intermediate::Type*>& elementTypes, const std::string& comment);
    otava::intermediate::Type* MakeFunctionType(otava::intermediate::Type* returnType, const std::vector<otava::intermediate::Type*>& paramTypes);
    otava::intermediate::Type* MakeArrayType(std::int64_t size, otava::intermediate::Type* elementType);
    inline otava::intermediate::Type* MakeFwdDeclaredStructureType(SymbolId id, const std::string& comment)
    {
        return context->MakeFwdDeclaredStructureType(ToUnderlying(id), context->NextTypeId(), comment);
    }
    inline void AddFwdDependentType(otava::intermediate::FwdDeclaredStructureType* fwdDeclaredType, otava::intermediate::Type* type)
    {
        context->AddFwdDependentType(fwdDeclaredType, type);
    }
    otava::intermediate::Type* GetOrInsertFwdDeclaredStructureType(SymbolId id, const std::string& comment);
    void ResolveForwardReferences(SymbolId typeId, otava::intermediate::StructureType* structureType);
    inline otava::intermediate::Type* GetVoidType() noexcept { return context->GetVoidType(); }
    inline otava::intermediate::Type* GetBoolType() noexcept { return context->GetBoolType(); }
    inline otava::intermediate::Type* GetSByteType() noexcept { return context->GetSByteType(); }
    inline otava::intermediate::Type* GetByteType() noexcept { return context->GetByteType(); }
    inline otava::intermediate::Type* GetShortType() noexcept { return context->GetShortType(); }
    inline otava::intermediate::Type* GetUShortType() noexcept { return context->GetUShortType(); }
    inline otava::intermediate::Type* GetIntType() noexcept { return context->GetIntType(); }
    inline otava::intermediate::Type* GetUIntType() noexcept { return context->GetUIntType(); }
    inline otava::intermediate::Type* GetLongType() noexcept { return context->GetLongType(); }
    inline otava::intermediate::Type* GetULongType() noexcept { return context->GetULongType(); }
    inline otava::intermediate::Type* GetFloatType() noexcept { return context->GetFloatType(); }
    inline otava::intermediate::Type* GetDoubleType() noexcept { return context->GetDoubleType(); }
    inline otava::intermediate::Type* MakePtrType(otava::intermediate::Type* baseType) { return context->MakePtrType(baseType); }
    otava::intermediate::Value* EmitBool(bool value);
    inline otava::intermediate::Value* EmitSByte(std::int8_t value) { return context->GetSByteValue(value); }
    inline otava::intermediate::Value* EmitByte(std::uint8_t value) { return context->GetByteValue(value); }
    inline otava::intermediate::Value* EmitShort(std::int16_t value) { return context->GetShortValue(value); }
    inline otava::intermediate::Value* EmitUShort(std::uint16_t value) { return context->GetUShortValue(value); }
    inline otava::intermediate::Value* EmitInt(std::int32_t value) { return context->GetIntValue(value); }
    inline otava::intermediate::Value* EmitUInt(std::uint32_t value) { return context->GetUIntValue(value); }
    inline otava::intermediate::Value* EmitLong(std::int64_t value) { return context->GetLongValue(value); }
    inline otava::intermediate::Value* EmitULong(std::uint64_t value) { return context->GetULongValue(value); }
    inline otava::intermediate::Value* EmitIntegerValue(otava::intermediate::Type* type, std::int64_t value) { return context->GetIntegerValue(type, value); }
    inline otava::intermediate::Value* EmitFloat(float value) { return context->GetFloatValue(value); }
    inline otava::intermediate::Value* EmitDouble(double value) { return context->GetDoubleValue(value); }
    inline otava::intermediate::Value* EmitFloatingValue(otava::intermediate::Type* type, double value) { return context->GetFloatingValue(type, value); }
    inline otava::intermediate::Value* EmitNull(otava::intermediate::Type* type) { return context->GetNullValue(soul::ast::Span(), type); }
    inline otava::intermediate::Value* EmitArrayValue(const std::vector<otava::intermediate::Value*>& elements, otava::intermediate::ArrayType* arrayType)
    {
        return context->MakeArrayValue(soul::ast::Span(), elements, arrayType);
    }
    inline otava::intermediate::Value* EmitStructureValue(const std::vector<otava::intermediate::Value*>& fieldValues, otava::intermediate::StructureType* structureType)
    {
        return context->MakeStructureValue(soul::ast::Span(), fieldValues, structureType);
    }
    otava::intermediate::Value* EmitStringValue(const std::string& value);
    otava::intermediate::Value* EmitString16Value(const std::string& value);
    otava::intermediate::Value* EmitString32Value(const std::string& value);
    inline otava::intermediate::Value* EmitConversionValue(otava::intermediate::Type* type, otava::intermediate::Value* from)
    {
        return context->MakeConversionValue(soul::ast::Span(), type, from);
    }
    inline otava::intermediate::Value* EmitGlobalVariable(otava::intermediate::Type* type, const std::string& variableName, otava::intermediate::Value* initializer)
    {
        return context->AddGlobalVariable(soul::ast::Span(), context->MakePtrType(type), variableName, initializer);
    }
    inline otava::intermediate::Value* EmitSymbolValue(otava::intermediate::Type* type, const std::string& symbol)
    {
        return context->MakeSymbolValue(soul::ast::Span(), type, symbol);
    }
    inline otava::intermediate::Value* EmitNot(otava::intermediate::Value* value) { return context->CreateNot(value); }
    inline otava::intermediate::Value* EmitNeg(otava::intermediate::Value* value) { return context->CreateNeg(value); }
    inline otava::intermediate::Value* EmitAdd(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateAdd(left, right); }
    inline otava::intermediate::Value* EmitSub(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateSub(left, right); }
    inline otava::intermediate::Value* EmitMul(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateMul(left, right); }
    inline otava::intermediate::Value* EmitDiv(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateDiv(left, right); }
    inline otava::intermediate::Value* EmitMod(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateMod(left, right); }
    inline otava::intermediate::Value* EmitAnd(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateAnd(left, right); }
    inline otava::intermediate::Value* EmitOr(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateOr(left, right); }
    inline otava::intermediate::Value* EmitXor(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateXor(left, right); }
    inline otava::intermediate::Value* EmitShl(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateShl(left, right); }
    inline otava::intermediate::Value* EmitShr(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateShr(left, right); }
    inline otava::intermediate::Value* EmitEqual(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateEqual(left, right); }
    inline otava::intermediate::Value* EmitLess(otava::intermediate::Value* left, otava::intermediate::Value* right) { return context->CreateLess(left, right); }
    inline otava::intermediate::Value* EmitSignExtend(otava::intermediate::Value* value, otava::intermediate::Type* destType)
    {
        return context->CreateSignExtend(value, destType);
    }
    inline otava::intermediate::Value* EmitZeroExtend(otava::intermediate::Value* value, otava::intermediate::Type* destType)
    {
        return context->CreateZeroExtend(value, destType);
    }
    inline otava::intermediate::Value* EmitFPExtend(otava::intermediate::Value* value, otava::intermediate::Type* destType)
    {
        return context->CreateFloatingPointExtend(value, destType);
    }
    inline otava::intermediate::Value* EmitTruncate(otava::intermediate::Value* value, otava::intermediate::Type* destType)
    {
        return context->CreateTruncate(value, destType);
    }
    inline otava::intermediate::Value* EmitBitcast(otava::intermediate::Value* value, otava::intermediate::Type* destType)
    {
        return context->CreateBitcast(value, destType);
    }
    inline otava::intermediate::Value* EmitIntToFloat(otava::intermediate::Value* value, otava::intermediate::Type* destType)
    {
        return context->CreateIntToFloat(value, destType);
    }
    inline otava::intermediate::Value* EmitFloatToInt(otava::intermediate::Value* value, otava::intermediate::Type* destType)
    {
        return context->CreateFloatToInt(value, destType);
    }
    inline otava::intermediate::Value* EmitIntToPtr(otava::intermediate::Value* value, otava::intermediate::Type* destType)
    {
        return context->CreateIntToPtr(value, destType);
    }
    inline otava::intermediate::Value* EmitPtrToInt(otava::intermediate::Value* value, otava::intermediate::Type* destType)
    {
        return context->CreatePtrToInt(value, destType);
    }
    inline otava::intermediate::Value* EmitParam(otava::intermediate::Type* type) { return context->CreateParam(type); }
    inline otava::intermediate::Value* EmitLocal(otava::intermediate::Type* type) { return context->CreateLocal(type); }
    inline otava::intermediate::Value* EmitLocalInEntryBlock(otava::intermediate::Type* type) { return context->CreateLocalInEntryBlock(type); }
    inline otava::intermediate::Value* EmitPLocal(otava::intermediate::Type* type, int level) { return context->CreatePLocal(type, level); }
    inline otava::intermediate::Value* EmitLoad(otava::intermediate::Value* value) { return context->CreateLoad(value); }
    inline void EmitStore(otava::intermediate::Value* value, otava::intermediate::Value* ptr) { context->CreateStore(value, ptr); }
    inline otava::intermediate::Value* EmitArg(otava::intermediate::Value* value) { return context->CreateArg(value); }
    inline otava::intermediate::Value* EmitElemAddr(otava::intermediate::Value* ptr, otava::intermediate::Value* index)
    {
        return context->CreateElemAddr(ptr, index, otava::intermediate::GetElemType(ptr, index, soul::ast::Span(), context));
    }
    inline otava::intermediate::Value* EmitPtrOffset(otava::intermediate::Value* ptr, otava::intermediate::Value* offset) { return context->CreatePtrOffset(ptr, offset); }
    inline otava::intermediate::Value* EmitPtrDiff(otava::intermediate::Value* leftPtr, otava::intermediate::Value* rightPtr)
    {
        return context->CreatePtrDiff(leftPtr, rightPtr);
    }
    otava::intermediate::Value* EmitCall(otava::intermediate::Value* function, const std::vector<otava::intermediate::Value*>& args);
    inline otava::intermediate::Value* EmitRet(otava::intermediate::Value* value) { return context->CreateRet(value); }
    inline otava::intermediate::Value* EmitRetVoid() { return context->CreateRet(nullptr); }
    inline otava::intermediate::Value* GetParam(int index) const { return context->CurrentFunction()->GetParam(index); }
    inline void EmitJump(otava::intermediate::BasicBlock* dest) { context->CreateJump(dest); }
    inline void EmitBranch(otava::intermediate::Value* cond, otava::intermediate::BasicBlock* trueDest, otava::intermediate::BasicBlock* falseDest)
    {
        context->CreateBranch(cond, trueDest, falseDest);
    }
    inline otava::intermediate::SwitchInstruction* EmitSwitch(otava::intermediate::Value* cond, otava::intermediate::BasicBlock* defaultDest)
    {
        return context->CreateSwitch(cond, defaultDest);
    }
    inline otava::intermediate::Value* EmitGetRbp()
    {
        return context->CreateGetRbp();
    }
    inline void EmitNop() { context->CreateNop(); }
    otava::intermediate::Type* GetType(SymbolId id) const noexcept;
    void SetType(SymbolId id, otava::intermediate::Type* type);
    otava::intermediate::Value* GetIrObject(void* symbol) const noexcept;
    void SetIrObject(void* symbol, otava::intermediate::Value* irObject);
    otava::intermediate::Value* GetVTabVariable(const std::string& className) const noexcept;
    void SetVTabVariable(const std::string& className, otava::intermediate::Value* vtabVariable);
    IrValueStack& Stack() noexcept { return *stack; }
    otava::intermediate::Value* EmitClassPtrConversion(otava::intermediate::Value* classPtr, otava::intermediate::Value* delta, otava::intermediate::Type* destType,
        bool checkNull);
    inline otava::intermediate::MetadataStruct* CreateMetadataStruct() { return context->CreateMetadataStruct(); }
    inline otava::intermediate::MetadataBool* CreateMetadataBool(bool value) { return context->CreateMetadataBool(value); }
    inline otava::intermediate::MetadataLong* CreateMetadataLong(std::int64_t value) { return context->CreateMetadataLong(value); }
    inline otava::intermediate::MetadataString* CreateMetadataString(const std::string& value) { return context->CreateMetadataString(value, false); }
    inline otava::intermediate::MetadataRef* CreateMetadataRef(int nodeId) { return context->CreateMetadataRef(soul::ast::Span(), nodeId); }
    inline int Line() const noexcept { return line; }
    inline void SetLine(int line_) noexcept { line = line_; }
private:
    otava::intermediate::IntermediateContext* context;
    IrValueStack* stack;
    std::unordered_map<SymbolId, otava::intermediate::Type*> typeMap;
    std::map<void*, otava::intermediate::Value*> irObjectMap;
    std::map<std::string, otava::intermediate::Value*> vtabVariableMap;
    otava::intermediate::Value* retValue;
    std::map<util::uuid, otava::intermediate::StructureType*> forwardDeclarationMap;
    int line;
    bool check;
};

} // namespace otava::symbols
