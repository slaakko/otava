// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

module otava.pp;
import otava.pp.lexer;
import otava.pp.parser;
import otava.pp.parser.spg.rules;
import otava.pp.state;

namespace otava::pp {

void PreprocessPPLine(soul::lexer::Lexer<otava::lexer::OtavaLexer<char32_t>, char32_t>* lexer, soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>* token)
{
    otava::pp::state::State* state = otava::pp::state::LexerStateMap::Instance().GetState(lexer);
    auto ppLexer = otava::pp::lexer::MakeLexer(token->match.begin, token->match.end, lexer->FileName());
    lexer->SetRuleNameMapPtr(otava::pp::parser::spg::rules::GetRuleNameMapPtr());
    using LexerType = decltype(ppLexer);
    otava::pp::parser::PPParser<LexerType>::Parse(ppLexer, state);
}

} // namespace soul::cpp20::pp
