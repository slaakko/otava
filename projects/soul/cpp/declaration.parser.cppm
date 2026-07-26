
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/cpp/declaration.parser' using soul parser generator ospg version 5.0.0

export module soul.cpp.declaration.parser;

import std;
import soul.lexer;
import soul.parser;
import soul.ast.cpp;
import soul.ast.common;

export namespace soul::cpp::declaration::parser {

template<typename LexerT>
struct DeclarationParser
{
    static soul::parser::Match BlockDeclaration(LexerT& lexer);
    static soul::parser::Match SimpleDeclaration(LexerT& lexer);
    static soul::parser::Match DeclSpecifierSeq(LexerT& lexer, soul::ast::cpp::SimpleDeclarationNode* declaration);
    static soul::parser::Match DeclSpecifier(LexerT& lexer);
    static soul::parser::Match StorageClassSpecifier(LexerT& lexer);
    static soul::parser::Match TypeSpecifier(LexerT& lexer);
    static soul::parser::Match SimpleTypeSpecifier(LexerT& lexer);
    static soul::parser::Match TypeName(LexerT& lexer);
    static soul::parser::Match TemplateArgumentList(LexerT& lexer, soul::ast::cpp::TypeNameNode* typeName);
    static soul::parser::Match TemplateArgument(LexerT& lexer);
    static soul::parser::Match Typedef(LexerT& lexer);
    static soul::parser::Match CVQualifier(LexerT& lexer);
    static soul::parser::Match NamespaceAliasDefinition(LexerT& lexer);
    static soul::parser::Match UsingDeclaration(LexerT& lexer);
    static soul::parser::Match UsingDirective(LexerT& lexer);
};

} // namespace soul::cpp::declaration::parser
