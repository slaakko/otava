
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/cpp/declarator.parser' using soul parser generator ospg version 5.0.0

export module soul.cpp.declarator.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.cpp;
import soul.ast.common;

export namespace soul::cpp::declarator::parser {

template<typename LexerT>
struct DeclaratorParser
{
    static soul::parser::Match InitDeclaratorList(LexerT& lexer);
    static soul::parser::Match InitDeclarator(LexerT& lexer);
    static soul::parser::Match Declarator(LexerT& lexer);
    static soul::parser::Match DirectDeclarator(LexerT& lexer);
    static soul::parser::Match DeclaratorId(LexerT& lexer);
    static soul::parser::Match TypeId(LexerT& lexer);
    static soul::parser::Match Type(LexerT& lexer);
    static soul::parser::Match TypeSpecifierSeq(LexerT& lexer, soul::ast::cpp::TypeIdNode* typeId);
    static soul::parser::Match AbstractDeclarator(LexerT& lexer);
    static soul::parser::Match DirectAbstractDeclarator(LexerT& lexer);
    static soul::parser::Match PtrOperator(LexerT& lexer);
    static soul::parser::Match CVQualifierSeq(LexerT& lexer);
    static soul::parser::Match Initializer(LexerT& lexer);
    static soul::parser::Match InitializerClause(LexerT& lexer);
    static soul::parser::Match InitializerList(LexerT& lexer, soul::ast::cpp::AssignInitNode* init);
};

} // namespace soul::cpp::declarator::parser
