
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/oslg/lexer_file.parser' using soul parser generator ospg version 5.0.0

export module soul.slg.lexer.file.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.slg;

export namespace soul::slg::lexer::file::parser {

template<typename LexerT>
struct LexerFileParser
{
    static std::unique_ptr<soul::ast::slg::LexerFile> Parse(LexerT& lexer);
    static soul::parser::Match LexerFile(LexerT& lexer);
    static soul::parser::Match Imports(LexerT& lexer, soul::ast::slg::LexerFile* lexerFile);
    static soul::parser::Match LexerContent(LexerT& lexer, soul::ast::slg::Lexer* lxr);
    static soul::parser::Match Rules(LexerT& lexer, soul::ast::slg::Lexer* lxr);
    static soul::parser::Match Rule(LexerT& lexer);
    static soul::parser::Match Variables(LexerT& lexer, soul::ast::slg::Lexer* lxr);
    static soul::parser::Match Variable(LexerT& lexer);
    static soul::parser::Match Actions(LexerT& lexer, soul::ast::slg::Lexer* lxr);
    static soul::parser::Match Action(LexerT& lexer);
};

} // namespace soul::slg::lexer::file::parser
