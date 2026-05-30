// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.function_type_symbol;

namespace otava::symbols {

FunctionTypeSymbol::FunctionTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_)
{
}

FunctionTypeSymbol::FunctionTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_)
{
}

otava::intermediate::Type* FunctionTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context)
{
/*
    util::uuid irId = IrId(fullSpan, context);
    otava::intermediate::Type* type = emitter.GetType(irId);
    if (!type)
    {
        std::vector<otava::intermediate::Type*> paramTypes;
        for (TypeSymbol* paramType : parameterTypes)
        {
            paramTypes.push_back(paramType->IrType(emitter, fullSpan, context));
        }
        type = emitter.MakeFunctionType(returnType->IrType(emitter, fullSpan, context), paramTypes);
        emitter.SetType(irId, type);
    }
    return type;
*/
    return nullptr;
}

} // namespace otava::symbols
