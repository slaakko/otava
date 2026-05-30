// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.intermediate.context;

import otava.intermediate.compile_unit;
import otava.intermediate.error;
import otava.intermediate.lexer;

namespace otava::intermediate {

IntermediateContext::IntermediateContext() :
    compileUnit(), inlineDepth(4), maxArithmeticOptimizationCount(8),
    flags(ContextFlags::none), functionsInlined(0), totalFunctions(0), currentBasicBlock(nullptr), currentLineNumber(0), lexer(nullptr), fileId(-1),
    assemblyContext(), debugOutputStream(nullptr)
{
    compileUnit.SetContext(this);
    types.SetContext(this);
    types.Init();
    data.SetContext(this);
    code.SetContext(this);
    metadata.SetContext(this);
}

void IntermediateContext::SetFilePath(const std::string& filePath_)
{
    compileUnit.SetFilePath(filePath_);
}

const std::string& IntermediateContext::FilePath() const
{
    return compileUnit.FilePath();
}

std::string IntermediateContext::ErrorLines(const soul::ast::LineColLen& lineColLen)
{
    if (lineColLen.IsValid())
    {
        std::string errorLines = util::ToUtf8(fileMap.GetFileLine(fileId, lineColLen.line));
        errorLines.append("\n").append(lineColLen.col - 1, ' ').append(lineColLen.len, '^').append("\n");
        return errorLines;
    }
    else
    {
        return std::string();
    }
}

void IntermediateContext::SetCompileUnitInfo(const std::string& compileUnitId, MetadataRef* mdRef)
{
    compileUnit.SetInfo(compileUnitId, mdRef);
}

void IntermediateContext::SetCompileUnitInfo(const std::string& compileUnitId_, const std::string& sourceFilePath)
{
    MetadataStruct* sourceFileStruct = metadata.CreateMetadataStruct();
    sourceFileStruct->AddItem("nodeType", metadata.CreateMetadataLong(fileInfoNodeType));
    sourceFileStruct->AddItem("sourceFileName", metadata.CreateMetadataString(sourceFilePath, false));
    MetadataRef* sourceFileNameRef = metadata.CreateMetadataRef(soul::ast::Span(), sourceFileStruct->Id());
    compileUnit.SetInfo(compileUnitId_, sourceFileNameRef);
}

void IntermediateContext::AddStructureType(const soul::ast::Span& span, std::int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs, MetadataRef* mdRef)
{
    types.AddStructureType(span, typeId, fieldTypeRefs, mdRef);
}

void IntermediateContext::AddArrayType(const soul::ast::Span& span, std::int32_t typeId, std::int64_t size, const TypeRef& elementTypeRef)
{
    types.AddArrayType(span, typeId, size, elementTypeRef);
}

void IntermediateContext::AddFunctionType(const soul::ast::Span& span, std::int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs)
{
    types.AddFunctionType(span, typeId, returnTypeRef, paramTypeRefs);
}

GlobalVariable* IntermediateContext::AddGlobalVariable(const soul::ast::Span& span, Type* type, const std::string& variableName, Value* initializer)
{
    return data.AddGlobalVariable(span, type, variableName, initializer, this);
}

GlobalVariable* IntermediateContext::GetGlobalVariableForString(otava::intermediate::Value* stringValue, Type* charType)
{
    return data.GetGlobalVariableForString(stringValue, charType);
}

void IntermediateContext::ResolveTypes()
{
    types.Resolve(this);
}

void IntermediateContext::ResolveType(TypeRef& typeRef)
{
    types.ResolveType(typeRef, this);
}

Value* IntermediateContext::GetBooleanLiteral(const soul::ast::Span& span, Type* type, bool value)
{
    if (type->IsBooleanType())
    {
        if (value)
        {
            return GetTrueValue();
        }
        else
        {
            return GetFalseValue();
        }
    }
    else
    {
        Error("error making literal value: Boolean type expected", span, this);
    }
    return nullptr;
}

Value* IntermediateContext::GetNullValue(const soul::ast::Span& span, Type* type)
{
    if (type->IsPointerType())
    {
        return data.GetNullValue(type);
    }
    else
    {
        Error("error making null literal value: pointer type expected", span, this);
    }
    return nullptr;
}

Function* IntermediateContext::AddFunctionDefinition(const soul::ast::Span& span, Type* type, const std::string& functionId, bool inline_, bool linkOnce, bool child, bool createEntry,
    otava::intermediate::MetadataRef* metadataRef)
{
    if (type->IsPointerType())
    {
        type = type->RemovePointer(span, this);
    }
    if (type->IsFunctionType())
    {
        FunctionType* functionType = static_cast<FunctionType*>(type);
        return code.AddFunctionDefinition(span, functionType, functionId, inline_, linkOnce, child, createEntry, metadataRef);
    }
    else
    {
        Error("error adding function '" + functionId + "' definition: invalid type '" + type->Name() + "': function type expected", span, this);
    }
    return nullptr;
}

Function* IntermediateContext::AddFunctionDeclaration(const soul::ast::Span& span, Type* type, const std::string& functionId)
{
    if (type->IsPointerType())
    {
        type = type->RemovePointer(span, this);
    }
    if (type->IsFunctionType())
    {
        FunctionType* functionType = static_cast<FunctionType*>(type);
        return code.AddFunctionDeclaration(span, functionType, functionId);
    }
    else
    {
        Error("error adding function '" + functionId + "' declaration: invalid type '" + type->Name() + "': function type expected", span, this);
    }
    return nullptr;
}

Instruction* IntermediateContext::CreateNot(Value* arg)
{
    Instruction* inst = new NotInstruction(soul::ast::Span(), MakeRegValue(arg->GetType()), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateNeg(Value* arg)
{
    Instruction* inst = new NegInstruction(soul::ast::Span(), MakeRegValue(arg->GetType()), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateAdd(Value* left, Value* right)
{
    Instruction* inst = new AddInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateSub(Value* left, Value* right)
{
    Instruction* inst = new SubInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateMul(Value* left, Value* right)
{
    Instruction* inst = new MulInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateDiv(Value* left, Value* right)
{
    Instruction* inst = new DivInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateMod(Value* left, Value* right)
{
    Instruction* inst = new ModInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateAnd(Value* left, Value* right)
{
    Instruction* inst = new AndInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateOr(Value* left, Value* right)
{
    Instruction* inst = new OrInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateXor(Value* left, Value* right)
{
    Instruction* inst = new XorInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateShl(Value* left, Value* right)
{
    Instruction* inst = new ShlInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateShr(Value* left, Value* right)
{
    Instruction* inst = new ShrInstruction(soul::ast::Span(), MakeRegValue(left->GetType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateEqual(Value* left, Value* right)
{
    Instruction* inst = new EqualInstruction(soul::ast::Span(), MakeRegValue(GetBoolType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateLess(Value* left, Value* right)
{
    Instruction* inst = new LessInstruction(soul::ast::Span(), MakeRegValue(GetBoolType()), left, right);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateSignExtend(Value* arg, Type* destType)
{
    Instruction* inst = new SignExtendInstruction(soul::ast::Span(), MakeRegValue(destType), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateZeroExtend(Value* arg, Type* destType)
{
    Instruction* inst = new ZeroExtendInstruction(soul::ast::Span(), MakeRegValue(destType), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateFloatingPointExtend(Value* arg, Type* destType)
{
    Instruction* inst = new FloatingPointExtendInstruction(soul::ast::Span(), MakeRegValue(destType), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateTruncate(Value* arg, Type* destType)
{
    Instruction* inst = new TruncateInstruction(soul::ast::Span(), MakeRegValue(destType), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateBitcast(Value* arg, Type* destType)
{
    Instruction* inst = new BitcastInstruction(soul::ast::Span(), MakeRegValue(destType), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateIntToFloat(Value* arg, Type* destType)
{
    Instruction* inst = new IntToFloatInstruction(soul::ast::Span(), MakeRegValue(destType), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateFloatToInt(Value* arg, Type* destType)
{
    Instruction* inst = new FloatToIntInstruction(soul::ast::Span(), MakeRegValue(destType), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateIntToPtr(Value* arg, Type* destType)
{
    Instruction* inst = new IntToPtrInstruction(soul::ast::Span(), MakeRegValue(destType), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreatePtrToInt(Value* arg, Type* destType)
{
    Instruction* inst = new PtrToIntInstruction(soul::ast::Span(), MakeRegValue(destType), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateParam(Type* type)
{
    Instruction* inst = new ParamInstruction(soul::ast::Span(), MakeRegValue(type));
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateLocal(Type* type)
{
    Type* ptrType = MakePtrType(type);
    Instruction* inst = new LocalInstruction(soul::ast::Span(), MakeRegValue(ptrType), type);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateLocalInEntryBlock(Type* type)
{
    Type* ptrType = MakePtrType(type);
    Instruction* inst = new LocalInstruction(soul::ast::Span(), MakeRegValue(ptrType), type);
    AddLineInfo(inst);
    BasicBlock* entryBlock = code.CurrentFunction()->GetBasicBlock(0);
    entryBlock->AddInstruction(inst);
    return inst;
}

Instruction* IntermediateContext::CreatePLocal(Type* type, int level)
{
    Type* ptrType = MakePtrType(type);
    Instruction* inst = new PLocalInstruction(soul::ast::Span(), MakeRegValue(ptrType), type, level);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateLoad(Value* ptr)
{
    Instruction* inst = new LoadInstruction(soul::ast::Span(), MakeRegValue(ptr->GetType()->RemovePointer(soul::ast::Span(), this)), ptr);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateStore(Value* value, Value* ptr)
{
    Instruction* inst = new StoreInstruction(soul::ast::Span(), value, ptr);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateArg(Value* arg)
{
    Instruction* inst = new ArgInstruction(soul::ast::Span(), arg);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateElemAddr(Value* ptr, Value* index, Type* type)
{
    Instruction* inst = new ElemAddrInstruction(soul::ast::Span(), MakeRegValue(type), ptr, index);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreatePtrOffset(Value* ptr, Value* offset)
{
    Instruction* inst = new PtrOffsetInstruction(soul::ast::Span(), MakeRegValue(ptr->GetType()), ptr, offset);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreatePtrDiff(Value* leftPtr, Value* rightPtr)
{
    Instruction* inst = new PtrDiffInstruction(soul::ast::Span(), MakeRegValue(GetLongType()), leftPtr, rightPtr);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateCall(Value* callee)
{
    Type* type = callee->GetType();
    if (type->IsFunctionPtrType())
    {
        PointerType* ptrType = static_cast<PointerType*>(type);
        type = ptrType->BaseType();
    }
    if (type->IsFunctionType())
    {
        FunctionType* functionType = static_cast<FunctionType*>(type);
        Type* returnType = functionType->ReturnType();
        if (returnType->IsVoidType())
        {
            Instruction* inst = new ProcedureCallInstruction(soul::ast::Span(), callee);
            AddLineInfo(inst);
            if (currentBasicBlock)
            {
                currentBasicBlock->AddInstruction(inst);
            }
            return inst;
        }
        else
        {
            Instruction* inst = new FunctionCallInstruction(soul::ast::Span(), MakeRegValue(returnType), callee);
            AddLineInfo(inst);
            if (currentBasicBlock)
            {
                currentBasicBlock->AddInstruction(inst);
            }
            return inst;
        }
    }
    else
    {
        SetExceptionThrown();
        throw std::runtime_error("invalid call");
    }
}

Instruction* IntermediateContext::CreateRet(Value* value)
{
    Instruction* inst = new RetInstruction(soul::ast::Span(), value);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateJump(BasicBlock* dest)
{
    JmpInstruction* inst = new JmpInstruction(soul::ast::Span(), dest->Id());
    inst->SetTargetBasicBlock(dest);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateBranch(Value* cond, BasicBlock* trueDest, BasicBlock* falseDest)
{
    BranchInstruction* inst = new BranchInstruction(soul::ast::Span(), cond, trueDest->Id(), falseDest->Id());
    inst->SetTrueTargetBasicBlock(trueDest);
    inst->SetFalseTargetBasicBlock(falseDest);
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

SwitchInstruction* IntermediateContext::CreateSwitch(Value* cond, BasicBlock* defaultDest)
{
    SwitchInstruction* inst = new SwitchInstruction(soul::ast::Span(), cond, defaultDest->Id());
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateNop()
{
    Instruction* inst = new NoOperationInstruction(soul::ast::Span());
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

Instruction* IntermediateContext::CreateGetRbp()
{
    Instruction* inst = new GetRbpInstruction(soul::ast::Span(), MakeRegValue(types.GetVoidType()->AddPointer(this)));
    AddLineInfo(inst);
    if (currentBasicBlock)
    {
        currentBasicBlock->AddInstruction(inst);
    }
    return inst;
}

void IntermediateContext::AddLineInfo(Instruction* inst)
{
    if (currentLineNumber != 0 && !inst->IsNopInstruction())
    {
        MetadataRef* lineNumberInfo = nullptr;
        auto it = lineNumberInfoMap.find(currentLineNumber);
        if (it != lineNumberInfoMap.cend())
        {
            lineNumberInfo = it->second;
        }
        else
        {
            MetadataStruct* lineNumberStruct = CreateMetadataStruct();
            lineNumberStruct->AddItem("nodeType", CreateMetadataLong(lineInfoNodeType));
            lineNumberStruct->AddItem("line", CreateMetadataLong(currentLineNumber));
            lineNumberInfo = CreateMetadataRef(soul::ast::Span(), lineNumberStruct->Id());
            lineNumberInfoMap[currentLineNumber] = lineNumberInfo;
        }
        inst->SetMetadataRef(lineNumberInfo);
    }
}

RegValue* IntermediateContext::MakeRegValue(Type* type)
{
    Function* currentFunction = CurrentFunction();
    RegValue* regValue = currentFunction->MakeRegValue(soul::ast::Span(), type, currentFunction->NextRegNumber(), this);
    return regValue;
}

void IntermediateContext::WriteFile()
{
    compileUnit.Write();
}

} // otava::intermediate
