// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.parser.decl_specifier_seq;

import std;
import otava.ast.node;
import otava.symbols.context;

export namespace otava::parser {

std::unique_ptr<otava::ast::Node> ParseDeclarationSpecifierSequence(const std::string& declsSpecifierSeqStr, otava::symbols::Context* context);

} // otava::parser
