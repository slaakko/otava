// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.symbols.decl_specifier_seq_parser;

namespace otava::symbols {

DeclarationSpecifierSequenceParser declSpecifierSeqParser;

void SetDeclarationSpecifierSequenceParser(DeclarationSpecifierSequenceParser declSpecifierSeqParser_) noexcept
{
    declSpecifierSeqParser = declSpecifierSeqParser_;
}

std::unique_ptr<otava::ast::Node> ParseDeclarationSpecifierSequence(const std::string& declSpecifierSeqStr, Context* context)
{
    return declSpecifierSeqParser(declSpecifierSeqStr, context);
}

} // namespace otava::symbols
