// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.util;

import std;

export namespace otava::ast {

std::string UniversalCharacterName(char32_t c);
std::u32string ToUniversalId(const std::u32string& id);

} // namespace otava::ast
