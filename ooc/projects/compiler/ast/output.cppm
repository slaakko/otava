// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.ast.output;

import std;
import otava.ast.node;

export namespace otava::ast {

void PrintSource(Node& node, std::ostream& stream, std::vector<int>& lineStartIndeces);
void PrintXml(Node& node, std::ostream& stream);

} // namespace otava::ast
