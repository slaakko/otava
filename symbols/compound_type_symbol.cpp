// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.compound_type_symbol;

import otava.symbols.emitter;

namespace otava::symbols {

CompoundTypeSymbol::CompoundTypeSymbol(Module* module_, SymbolId id_) : TypeSymbol(module_, id_)
{
}

CompoundTypeSymbol::CompoundTypeSymbol(Module* module_, SymbolId id_, const std::string& name_) : TypeSymbol(module_, id_, name_)
{
}

otava::intermediate::Type* CompoundTypeSymbol::IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, Context* context)
{
/*
    util::uuid irId = IrId(fullSpan, context);
    otava::intermediate::Type* type = emitter.GetType(irId);
    if (!type)
    {
        if (!baseType)
        {
            ThrowException("cannot obtain base type for type '" + Name() + "' because it is incomplete at this point", fullSpan, context);
        }
        type = baseType->IrType(emitter, fullSpan, context);
        int pointerCount = otava::symbols::PointerCount(derivations);
        for (int i = 0; i < pointerCount; ++i)
        {
            type = emitter.MakePtrType(type);
        }
        if (otava::symbols::HasDerivation(derivations, Derivations::lvalueRefDerivation))
        {
            type = emitter.MakePtrType(type);
        }
        if (otava::symbols::HasDerivation(derivations, Derivations::rvalueRefDerivation))
        {
            type = emitter.MakePtrType(type);
        }
        emitter.SetType(irId, type);
    }
    return type;
*/
    return nullptr;
}

} // namespace otava::symbols
