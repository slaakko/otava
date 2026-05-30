// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module otava.symbols.decl_specifier_seq_parser;

import std;
import otava.ast.node;

export namespace otava::symbols {

class Context;

using DeclarationSpecifierSequenceParser = std::unique_ptr<otava::ast::Node>(*)(const std::string&, Context*);

void SetDeclarationSpecifierSequenceParser(DeclarationSpecifierSequenceParser declSpecifierSeqParser_) noexcept;

std::unique_ptr<otava::ast::Node> ParseDeclarationSpecifierSequence(const std::string& declSpecifierSeqStr, Context* context);

} // namespace otava::symbols
