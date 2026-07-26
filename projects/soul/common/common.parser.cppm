
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/common/common.parser' using soul parser generator ospg version 5.0.0

export module soul.common.common.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.common;

export namespace soul::common::common::parser {

template<typename LexerT>
struct CommonParser
{
    static soul::parser::Match QualifiedId(LexerT& lexer);
    static soul::parser::Match ExportModule(LexerT& lexer);
    static soul::parser::Match Import(LexerT& lexer);
    static soul::parser::Match ImportPrefix(LexerT& lexer);
    static soul::parser::Match ExprString(LexerT& lexer);
    static soul::parser::Match FilePath(LexerT& lexer);
    static soul::parser::Match ExportKeyword(LexerT& lexer);
    static soul::parser::Match ModuleKeyword(LexerT& lexer);
    static soul::parser::Match ImportKeyword(LexerT& lexer);
    static soul::parser::Match ImplementationPrefix(LexerT& lexer);
    static soul::parser::Match InterfacePrefix(LexerT& lexer);
    static soul::parser::Match ImplementationKeyword(LexerT& lexer);
    static soul::parser::Match InterfaceKeyword(LexerT& lexer);
    static soul::parser::Match ParserKeyword(LexerT& lexer);
    static soul::parser::Match LexerKeyword(LexerT& lexer);
};

} // namespace soul::common::common::parser
