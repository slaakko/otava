// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.visitor;

import otava.intermediate.code;
import otava.intermediate.context;
import otava.intermediate.types;
import otava.intermediate.value;
import otava.intermediate.data;
import std;

export namespace otava::intermediate {

class Visitor
{
public:
    Visitor(IntermediateContext* context_);
    virtual ~Visitor();
    IntermediateContext* GetContext() const { return context; }
    virtual void Visit(StructureType& structureType) {}
    virtual void Visit(FwdDeclaredStructureType& fwdDeclareStructureType) {}
    virtual void Visit(ArrayType& arrayType) {}
    virtual void Visit(FunctionType& functionType) {}
    virtual void Visit(GlobalVariable& globalVariable) {}
    virtual void Visit(Function& function) {}
    virtual void Visit(BasicBlock& basicBlock) {}
    virtual void Visit(StoreInstruction& inst) {}
    virtual void Visit(ArgInstruction& inst) {}
    virtual void Visit(JmpInstruction& inst) {}
    virtual void Visit(BranchInstruction& inst) {}
    virtual void Visit(ProcedureCallInstruction& inst) {}
    virtual void Visit(RetInstruction& inst) {}
    virtual void Visit(SwitchInstruction& inst) {}
    virtual void Visit(NotInstruction& inst) {}
    virtual void Visit(NegInstruction& inst) {}
    virtual void Visit(SignExtendInstruction& inst) {}
    virtual void Visit(ZeroExtendInstruction& inst) {}
    virtual void Visit(FloatingPointExtendInstruction& inst) {}
    virtual void Visit(TruncateInstruction& inst) {}
    virtual void Visit(BitcastInstruction& inst) {}
    virtual void Visit(IntToFloatInstruction& inst) {}
    virtual void Visit(FloatToIntInstruction& inst) {}
    virtual void Visit(IntToPtrInstruction& inst) {}
    virtual void Visit(PtrToIntInstruction& inst) {}
    virtual void Visit(AddInstruction& inst) {}
    virtual void Visit(SubInstruction& inst) {}
    virtual void Visit(MulInstruction& inst) {}
    virtual void Visit(DivInstruction& inst) {}
    virtual void Visit(ModInstruction& inst) {}
    virtual void Visit(AndInstruction& inst) {}
    virtual void Visit(OrInstruction& inst) {}
    virtual void Visit(XorInstruction& inst) {}
    virtual void Visit(ShlInstruction& inst) {}
    virtual void Visit(ShrInstruction& inst) {}
    virtual void Visit(EqualInstruction& inst) {}
    virtual void Visit(LessInstruction& inst) {}
    virtual void Visit(ParamInstruction& inst) {}
    virtual void Visit(LocalInstruction& inst) {}
    virtual void Visit(PLocalInstruction& inst) {}
    virtual void Visit(LoadInstruction& inst) {}
    virtual void Visit(ElemAddrInstruction& inst) {}
    virtual void Visit(PtrOffsetInstruction& inst) {}
    virtual void Visit(PtrDiffInstruction& inst) {}
    virtual void Visit(FunctionCallInstruction& inst) {}
    virtual void Visit(NoOperationInstruction& inst) {}
    virtual void Visit(GetRbpInstruction& inst) {}
    virtual void Visit(BoolValue& value) {}
    virtual void Visit(SByteValue& value) {}
    virtual void Visit(ByteValue& value) {}
    virtual void Visit(ShortValue& value) {}
    virtual void Visit(UShortValue& value) {}
    virtual void Visit(IntValue& value) {}
    virtual void Visit(UIntValue& value) {}
    virtual void Visit(LongValue& value) {}
    virtual void Visit(ULongValue& value) {}
    virtual void Visit(FloatValue& value) {}
    virtual void Visit(DoubleValue& value) {}
    virtual void Visit(NullValue& value) {}
    virtual void Visit(AddressValue& value) {}
    virtual void Visit(ArrayValue& value) {}
    virtual void Visit(StructureValue& value) {}
    virtual void Visit(StringValue& value) {}
    virtual void Visit(StringArrayValue& value) {}
    virtual void Visit(ConversionValue& value) {}
    virtual void Visit(ClsIdValue& value) {}
    virtual void Visit(SymbolValue& value) {}
private:
    IntermediateContext* context;
};

} // otava::intermediate
