// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.intrinsics;

import otava.symbols.context;
import otava.symbols.emitter;
import otava.symbols.function_kind;
import otava.symbols.fundamental_type_kind;
import otava.symbols.fundamental_type_symbol;
import otava.symbols.modules;

namespace otava::symbols {

GetFramePtrIntrinsic::GetFramePtrIntrinsic(Module* module_, SymbolId id_) : FunctionSymbol(module_, id_)
{
}

GetFramePtrIntrinsic::GetFramePtrIntrinsic(Context* context) : FunctionSymbol(context->GetModule(), 
    context->GetNextSymbolId(SymbolKind::intrinsicGetRbp), "__intrinsic_get_frame_ptr")
{
    SetFunctionKind(FunctionKind::function);
    SetAccess(Access::public_);
    SetReturnType(context->GetStdTypeFundamentalModule()->GetSymbolTable()->GetFundamentalTypeSymbol(FundamentalTypeKind::voidType, context)->AddPointer(context), context);
    SetNoExcept();
}

void GetFramePtrIntrinsic::GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
    const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
    emitter.Stack().Push(emitter.EmitGetRbp());
}

void MakeIntrinsics(Context* context)
{
    context->GetSymbolTable()->AddFunctionSymbol(context->GetSymbolTable()->GlobalNs()->GetScope(), new GetFramePtrIntrinsic(context), soul::ast::FullSpan(), context);
}

} // namespace otava::symbols
