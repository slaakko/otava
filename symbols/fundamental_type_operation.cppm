// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.fundamental_type_operation;

import otava.symbols.bound_tree;
import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.function_symbol;
import otava.symbols.variable_symbol;
import otava.intermediate.data;

export namespace otava::symbols {

struct FundamentalTypeNot
{
    static const char* GroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* arg);
};

struct FundamentalTypeUnaryPlus
{
    static const char* GroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* arg);
};

struct FundamentalTypeUnaryMinus
{
    static const char* GroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* arg);
};

struct FundamentalTypeComplement
{
    static const char* GroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* arg);
};

struct FundamentalTypeAdd
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeSub
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeMul
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeDiv
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeMod
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeAnd
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeOr
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeXor
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeShl
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeShr
{
    static const char* GroupName();
    static const char* AssignmentOpGroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeEqual
{
    static const char* GroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

struct FundamentalTypeLess
{
    static const char* GroupName();
    static otava::intermediate::Value* Generate(Emitter& emitter, otava::intermediate::Value* left, otava::intermediate::Value* right);
};

template<class Op>
class FundamentalTypeUnaryOperation : public FunctionSymbol
{
public:
    FundamentalTypeUnaryOperation(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_)
    {
    }
    FundamentalTypeUnaryOperation(Module* module_, SymbolId id_, TypeSymbol* type, Context* context) : FunctionSymbol(module_, id_, Op::GroupName())
    {
        SetFunctionKind(FunctionKind::function);
        SetAccess(Access::public_);
        ParameterSymbol* param = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "param");
        param->SetType(type);
        AddSymbol(param, soul::ast::FullSpan(), context);
        SetReturnType(type, soul::ast::FullSpan(), context);
        SetNoExcept();
    }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override
    {
        args[0]->Load(emitter, flags, fullSpan, context);
        otava::intermediate::Value* value = emitter.Stack().Pop();
        emitter.Stack().Push(Op::Generate(emitter, value));
    }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
};

template<class Op>
class FundamentalTypeBinaryOperation : public FunctionSymbol
{
public:
    FundamentalTypeBinaryOperation(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_)
    {
    }
    FundamentalTypeBinaryOperation(Module* module_, SymbolId id_, TypeSymbol* type, Context* context) : FunctionSymbol(module_, id_, Op::GroupName())
    {
        SetFunctionKind(FunctionKind::function);
        SetAccess(Access::public_);
        ParameterSymbol* leftParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
        leftParam->SetType(type);
        AddSymbol(leftParam, soul::ast::FullSpan(), context);
        ParameterSymbol* rightParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
        rightParam->SetType(type);
        AddSymbol(rightParam, soul::ast::FullSpan(), context);
        SetReturnType(type, soul::ast::FullSpan(), context);
        SetNoExcept();
    }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override
    {
        args[0]->Load(emitter, flags, fullSpan, context);
        otava::intermediate::Value* left = emitter.Stack().Pop();
        args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
        otava::intermediate::Value* right = emitter.Stack().Pop();
        emitter.Stack().Push(Op::Generate(emitter, left, right));
    }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
};

template<typename Op>
class FundamentalTypeAssignmentOperation : public FunctionSymbol
{
public:
    FundamentalTypeAssignmentOperation(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_), type(nullptr)
    {
    }
    FundamentalTypeAssignmentOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context) : 
        FunctionSymbol(module_, id_, Op::AssignmentOpGroupName()), type(type_)
    {
        SetFunctionKind(FunctionKind::function);
        SetAccess(Access::public_);
        ParameterSymbol* thisParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "this");
        thisParam->SetType(type);
        AddSymbol(thisParam, soul::ast::FullSpan(), context);
        ParameterSymbol* thatParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "that");
        thatParam->SetType(type);
        AddSymbol(thatParam, soul::ast::FullSpan(), context);
        SetReturnType(type->AddLValueRef(context), soul::ast::FullSpan(), context);
        SetNoExcept();
    }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
    {
        args[0]->Load(emitter, OperationFlags::none, fullSpan, context);
        otava::intermediate::Value* left = emitter.Stack().Pop();
        args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
        otava::intermediate::Value* right = emitter.Stack().Pop();
        emitter.Stack().Push(Op::Generate(emitter, left, right));
        args[0]->Store(emitter, OperationFlags::setPtr, fullSpan, context);
        emitter.Stack().Push(context->Ptr());
    }
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    TypeSymbol* type;
};

template<class Op>
class FundamentalTypeComparisonOperation : public FunctionSymbol
{
public:
    FundamentalTypeComparisonOperation(Module* module_, SymbolId id_) : 
        FunctionSymbol(module_, id_)
    {
    }
    FundamentalTypeComparisonOperation(Module* module_, SymbolId id_, TypeSymbol* type, TypeSymbol* boolType, Context* context) : 
        FunctionSymbol(module_, id_, Op::GroupName())
    {
        SetFunctionKind(FunctionKind::function);
        SetAccess(Access::public_);
        ParameterSymbol* leftParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "left");
        leftParam->SetType(type);
        AddSymbol(leftParam, soul::ast::FullSpan(), context);
        ParameterSymbol* rightParam = new ParameterSymbol(module_, context->GetNextSymbolId(SymbolKind::parameterSymbol), "right");
        rightParam->SetType(type);
        AddSymbol(rightParam, soul::ast::FullSpan(), context);
        SetReturnType(boolType, soul::ast::FullSpan(), context);
        SetNoExcept();
    }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override
    {
        args[0]->Load(emitter, flags, fullSpan, context);
        otava::intermediate::Value* left = emitter.Stack().Pop();
        args[1]->Load(emitter, OperationFlags::none, fullSpan, context);
        otava::intermediate::Value* right = emitter.Stack().Pop();
        emitter.Stack().Push(Op::Generate(emitter, left, right));
    }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
};

class FundamentalTypeNotOperation : public FundamentalTypeUnaryOperation<FundamentalTypeNot>
{
public:
    FundamentalTypeNotOperation(Module* module_, SymbolId id_);
    FundamentalTypeNotOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeUnaryPlusOperation : public FundamentalTypeUnaryOperation<FundamentalTypeUnaryPlus>
{
public:
    FundamentalTypeUnaryPlusOperation(Module* module_, SymbolId id_);
    FundamentalTypeUnaryPlusOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeUnaryMinusOperation : public FundamentalTypeUnaryOperation<FundamentalTypeUnaryMinus>
{
public:
    FundamentalTypeUnaryMinusOperation(Module* module_, SymbolId id_);
    FundamentalTypeUnaryMinusOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeComplementOperation : public FundamentalTypeUnaryOperation<FundamentalTypeComplement>
{
public:
    FundamentalTypeComplementOperation(Module* module_, SymbolId id_);
    FundamentalTypeComplementOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeAddOperation : public FundamentalTypeBinaryOperation<FundamentalTypeAdd>
{
public:
    FundamentalTypeAddOperation(Module* module_, SymbolId id_);
    FundamentalTypeAddOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeSubOperation : public FundamentalTypeBinaryOperation<FundamentalTypeSub>
{
public:
    FundamentalTypeSubOperation(Module* module_, SymbolId id_);
    FundamentalTypeSubOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeMulOperation : public FundamentalTypeBinaryOperation<FundamentalTypeMul>
{
public:
    FundamentalTypeMulOperation(Module* module_, SymbolId id_);
    FundamentalTypeMulOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeDivOperation : public FundamentalTypeBinaryOperation<FundamentalTypeDiv>
{
public:
    FundamentalTypeDivOperation(Module* module_, SymbolId id_);
    FundamentalTypeDivOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeModOperation : public FundamentalTypeBinaryOperation<FundamentalTypeMod>
{
public:
    FundamentalTypeModOperation(Module* module_, SymbolId id_);
    FundamentalTypeModOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeAndOperation : public FundamentalTypeBinaryOperation<FundamentalTypeAnd>
{
public:
    FundamentalTypeAndOperation(Module* module_, SymbolId id_);
    FundamentalTypeAndOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeOrOperation : public FundamentalTypeBinaryOperation<FundamentalTypeOr>
{
public:
    FundamentalTypeOrOperation(Module* module_, SymbolId id_);
    FundamentalTypeOrOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeXorOperation : public FundamentalTypeBinaryOperation<FundamentalTypeXor>
{
public:
    FundamentalTypeXorOperation(Module* module_, SymbolId id_);
    FundamentalTypeXorOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeShlOperation : public FundamentalTypeBinaryOperation<FundamentalTypeShl>
{
public:
    FundamentalTypeShlOperation(Module* module_, SymbolId id_);
    FundamentalTypeShlOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeShrOperation : public FundamentalTypeBinaryOperation<FundamentalTypeShr>
{
public:
    FundamentalTypeShrOperation(Module* module_, SymbolId id_);
    FundamentalTypeShrOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypePlusAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeAdd>
{
public:
    FundamentalTypePlusAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypePlusAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeMinusAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeSub>
{
public:
    FundamentalTypeMinusAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypeMinusAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeMulAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeMul>
{
public:
    FundamentalTypeMulAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypeMulAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeDivAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeDiv>
{
public:
    FundamentalTypeDivAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypeDivAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeModAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeMod>
{
public:
    FundamentalTypeModAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypeModAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeAndAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeAnd>
{
public:
    FundamentalTypeAndAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypeAndAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeOrAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeOr>
{
public:
    FundamentalTypeOrAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypeOrAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeXorAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeXor>
{
public:
    FundamentalTypeXorAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypeXorAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeShlAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeShl>
{
public:
    FundamentalTypeShlAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypeShlAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeShrAssignOperation : public FundamentalTypeAssignmentOperation<FundamentalTypeShr>
{
public:
    FundamentalTypeShrAssignOperation(Module* module_, SymbolId id_);
    FundamentalTypeShrAssignOperation(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
};

class FundamentalTypeEqualOperation : public FundamentalTypeComparisonOperation<FundamentalTypeEqual>
{
public:
    FundamentalTypeEqualOperation(Module* module_, SymbolId id_);
    FundamentalTypeEqualOperation(Module* module_, SymbolId id_, TypeSymbol* type_, TypeSymbol* boolType_, Context* context);
};

class FundamentalTypeLessOperation : public FundamentalTypeComparisonOperation<FundamentalTypeLess>
{
public:
    FundamentalTypeLessOperation(Module* module_, SymbolId id_);
    FundamentalTypeLessOperation(Module* module_, SymbolId id_, TypeSymbol* type_, TypeSymbol* boolType_, Context* context);
};

class FundamentalTypeDefaultCtor : public FunctionSymbol
{
public:
    FundamentalTypeDefaultCtor(Module* module_, SymbolId id_);
    FundamentalTypeDefaultCtor(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
private:
    TypeSymbol* type;
};

class FundamentalTypeCopyCtor : public FunctionSymbol
{
public:
    FundamentalTypeCopyCtor(Module* module_, SymbolId id_);
    FundamentalTypeCopyCtor(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    TypeSymbol* type;
};

class FundamentalTypeMoveCtor : public FunctionSymbol
{
public:
    FundamentalTypeMoveCtor(Module* module_, SymbolId id_);
    FundamentalTypeMoveCtor(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    TypeSymbol* type;
};

class FundamentalTypeCopyAssignment : public FunctionSymbol
{
public:
    FundamentalTypeCopyAssignment(Module* module_, SymbolId id_);
    FundamentalTypeCopyAssignment(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    TypeSymbol* type;
};

class FundamentalTypeMoveAssignment : public FunctionSymbol
{
public:
    FundamentalTypeMoveAssignment(Module* module_, SymbolId id_);
    FundamentalTypeMoveAssignment(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    bool IsCtorAssignmentOrArrow() const noexcept override { return true; }
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    TypeSymbol* type;
};

class TrivialDestructor : public FunctionSymbol
{
public:
    TrivialDestructor(Module* module_, SymbolId id_);
    TrivialDestructor(Module* module_, SymbolId id_, TypeSymbol* type_, Context* context);
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
private:
    TypeSymbol* type;
};

void AddFundamentalTypeOperationsToSymbolTable(Context* context);


} // namespace otava::symbols
