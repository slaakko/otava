// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.intermediate.main.parser;

import std;

export namespace otava::intermediate {

class IntermediateContext;

void Parse(const std::string& filePath, IntermediateContext& context, bool verbose);

} // otava::intermediate
