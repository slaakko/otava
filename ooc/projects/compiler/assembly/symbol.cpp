// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.assembly.symbol;

namespace otava::assembly {

Symbol::Symbol(const std::string& name_) : Value(ValueKind::symbol, name_)
{
}

} // namespace otava::assembly
