
// this file has been automatically generated from 'D:/work/otava/parser/module_dependency.parser' using soul parser generator spg version 5.2.0

export module otava.parser.module_dependency;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast.node;

export namespace otava::parser::module_dependency {

template<typename LexerT>
struct ModuleDependencyParser
{
    static std::unique_ptr<otava::ast::Node> Parse(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ModuleUnit(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match GlobalModuleFragment(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DeclarationSeq(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Declaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ExportImportDeclaration(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::module_dependency
