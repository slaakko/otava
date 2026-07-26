
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/ospg/parser_file.parser' using soul parser generator ospg version 5.0.0

export module soul.spg.parser.file.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.spg;

export namespace soul::spg::parser::file::parser {

template<typename LexerT>
struct ParserFileParser
{
    static std::unique_ptr<soul::ast::spg::ParserFile> Parse(LexerT& lexer);
    static soul::parser::Match ParserFile(LexerT& lexer);
    static soul::parser::Match Parser(LexerT& lexer);
    static soul::parser::Match ParserStatement(LexerT& lexer, soul::ast::spg::GrammarParser* parser);
    static soul::parser::Match LexerStatement(LexerT& lexer, soul::ast::spg::GrammarParser* parser);
    static soul::parser::Match MainStatement(LexerT& lexer, soul::ast::spg::GrammarParser* parser);
    static soul::parser::Match UsingStatement(LexerT& lexer, soul::ast::spg::GrammarParser* parser);
    static soul::parser::Match RuleStatement(LexerT& lexer, soul::ast::spg::GrammarParser* parser);
    static soul::parser::Match ParametersAndVariables(LexerT& lexer, soul::ast::spg::RuleParser* rule);
    static soul::parser::Match ParamOrVariable(LexerT& lexer);
    static soul::parser::Match ReturnType(LexerT& lexer);
    static soul::parser::Match RuleBody(LexerT& lexer);
    static soul::parser::Match Choice(LexerT& lexer);
    static soul::parser::Match Sequence(LexerT& lexer);
    static soul::parser::Match Difference(LexerT& lexer);
    static soul::parser::Match List(LexerT& lexer);
    static soul::parser::Match Prefix(LexerT& lexer);
    static soul::parser::Match Postfix(LexerT& lexer);
    static soul::parser::Match Primary(LexerT& lexer);
    static soul::parser::Match RuleCall(LexerT& lexer);
    static soul::parser::Match Nonterminal(LexerT& lexer);
    static soul::parser::Match Primitive(LexerT& lexer);
    static soul::parser::Match Grouping(LexerT& lexer);
};

} // namespace soul::spg::parser::file::parser
