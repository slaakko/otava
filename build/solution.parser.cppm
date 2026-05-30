
// this file has been automatically generated from 'D:/work/otava/build/solution.parser' using soul parser generator spg version 5.2.0

export module otava.build.solution_parser;

import std;
import soul.lexer;
import soul.parser;
import otava.build_solution;

export namespace otava::build::solution_parser {

template<typename LexerT>
struct SolutionParser
{
    static std::unique_ptr<otava::build::Solution> Parse(LexerT& lexer);
    static soul::parser::Match SolutionFile(LexerT& lexer);
    static soul::parser::Match SolutionDeclaration(LexerT& lexer);
    static soul::parser::Match FilePath(LexerT& lexer, otava::build::Solution* sln);
    static soul::parser::Match ProjectFilePath(LexerT& lexer);
    static soul::parser::Match QualifiedId(LexerT& lexer);
};

} // namespace otava::build::solution_parser
