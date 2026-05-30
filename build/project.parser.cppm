
// this file has been automatically generated from 'D:/work/otava/build/project.parser' using soul parser generator spg version 5.2.0

export module otava.build.project_parser;

import std;
import soul.lexer;
import soul.parser;
import otava.build_project;

export namespace otava::build::project_parser {

template<typename LexerT>
struct ProjectParser
{
    static std::unique_ptr<otava::build::Project> Parse(LexerT& lexer);
    static soul::parser::Match ProjectFile(LexerT& lexer);
    static soul::parser::Match ProjectDeclaration(LexerT& lexer);
    static soul::parser::Match ParseDefine(LexerT& lexer, otava::build::Project* projectFile);
    static soul::parser::Match TargetDeclaration(LexerT& lexer, otava::build::Project* projectFile);
    static soul::parser::Match ParseTarget(LexerT& lexer);
    static soul::parser::Match FilePath(LexerT& lexer, otava::build::Project* projectFile);
    static soul::parser::Match InterfaceFilePath(LexerT& lexer);
    static soul::parser::Match SourceFilePath(LexerT& lexer);
    static soul::parser::Match ResourceFilePath(LexerT& lexer);
    static soul::parser::Match ReferenceFilePath(LexerT& lexer);
    static soul::parser::Match QualifiedId(LexerT& lexer);
};

} // namespace otava::build::project_parser
