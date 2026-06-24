// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.intrinsics;

import otava.symbols.function_symbol;
import otava.symbols.variable_symbol;
import std;

export namespace otava::symbols {

class GetFramePtrIntrinsic : public FunctionSymbol
{
public:
    GetFramePtrIntrinsic(Module* module_, SymbolId id_);
    GetFramePtrIntrinsic(Context* context);
    ParameterSymbol* ThisParam(Context* context) const override { return nullptr; }
    void GenerateCode(Emitter& emitter, std::vector<BoundExpressionNode*>& args, OperationFlags flags,
        const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
};

void MakeIntrinsics(Context* context);

} // namespace otava::symbols
