// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.function_type_symbol;

import otava.symbols.type_symbol;
import std;

export namespace otava::symbols {

class FunctionTypeSymbol : public TypeSymbol
{
public:
    FunctionTypeSymbol(Module* module_, SymbolId id_);
    FunctionTypeSymbol(Module* module_, SymbolId id_, const std::string& name_);
    otava::intermediate::Type* IrType(Emitter& emitter, const soul::ast::FullSpan& fullSpan, otava::symbols::Context* context) override;
};

} // namespace otava::symbols
