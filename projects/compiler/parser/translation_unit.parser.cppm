
// this file has been automatically generated from 'D:/work/otava/projects/compiler/parser/translation_unit.parser' using soul parser generator ospg version 0.1.0

export module otava.parser.translation_unit;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::translation_unit {

template<typename LexerT>
struct TranslationUnitParser
{
    static std::unique_ptr<otava::ast::TranslationUnitNode> Parse(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TranslationUnit(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ModuleUnit(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::translation_unit
