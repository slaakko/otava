// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.parser.decl_specifier_seq;

import otava.lexer;
import otava.parser.declaration;
import otava.symbols.context;
import util.unicode;

namespace otava::parser {

std::unique_ptr<otava::ast::Node> ParseDeclarationSpecifierSequence(const std::string& declsSpecifierSeqStr, otava::symbols::Context* context)
{
    std::u32string d = util::ToUtf32(declsSpecifierSeqStr);
    auto lexer = otava::lexer::MakeLexer(d.c_str(), d.c_str() + d.length(), "<declSpecifierSeq>");
    using LexerType = decltype(lexer);
    otava::symbols::Lexer* prevLexer = context->GetLexer();
    context->SetLexer(&lexer);
    std::unique_ptr<otava::ast::Node> node = otava::parser::declaration::DeclarationParser<LexerType>::Parse(lexer, context);
    context->SetLexer(prevLexer);
    return node;
}

} // otava::parser
