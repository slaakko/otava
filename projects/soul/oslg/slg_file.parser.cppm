
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/oslg/slg_file.parser' using soul parser generator ospg version 5.0.0

export module soul.slg.slg.file.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.slg;

export namespace soul::slg::slg::file::parser {

template<typename LexerT>
struct SlgFileParser
{
    static std::unique_ptr<soul::ast::slg::SlgFile> Parse(LexerT& lexer);
    static soul::parser::Match SlgFile(LexerT& lexer);
    static soul::parser::Match SlgFileDeclaration(LexerT& lexer);
    static soul::parser::Match TokenFileDeclaration(LexerT& lexer);
    static soul::parser::Match KeywordFileDeclaration(LexerT& lexer);
    static soul::parser::Match ExpressionFileDeclaration(LexerT& lexer);
    static soul::parser::Match LexerFileDeclaration(LexerT& lexer);
};

} // namespace soul::slg::slg::file::parser
