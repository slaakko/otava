
// this file has been automatically generated from 'D:/work/otava/parser/module.parser' using soul parser generator spg version 5.2.0

export module otava.parser.modules;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;

export namespace otava::parser::modules {

template<typename LexerT>
struct ModuleParser
{
    static soul::parser::Match ModuleDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ModuleName(LexerT& lexer);
    static soul::parser::Match ExportDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ImportDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ModulePartition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ExportKeyword(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ImportKeyword(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ModuleKeyword(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match GlobalModuleFragment(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match PrivateModuleFragment(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match HeaderName(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::modules
