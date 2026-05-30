
// this file has been automatically generated from 'D:/work/otava/parser/class.parser' using soul parser generator spg version 5.2.0

export module otava.parser.classes;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::classes {

template<typename LexerT>
struct ClassParser
{
    static soul::parser::Match ClassSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ClassHead(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ClassKey(LexerT& lexer);
    static soul::parser::Match ClassHeadName(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ClassName(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ClassVirtSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match VirtSpecifierSeq(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match VirtSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match OverrideKeyword(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match FinalKeyword(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match BaseClause(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match BaseSpecifierList(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match BaseSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ClassOrDeclType(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MemberSpecification(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match MemberDeclarationReset(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MemberDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RBraceNext(LexerT& lexer);
    static soul::parser::Match MemberDeclSpecifiers(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RetMemberDeclSpecifiers(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MemberFunctionDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RetMemberFunctionDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MemberTemplateDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MemberDeclaratorList(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MemberDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CtorInitializer(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CtorInitializerGuarded(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CtorInitializerUnguarded(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CtorInitializerSaved(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MemberInitializerList(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MemberInitializer(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match MemberInitializerId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match PureSpecifier(LexerT& lexer);
    static soul::parser::Match AccessSpecifier(LexerT& lexer);
    static soul::parser::Match Private(LexerT& lexer);
    static soul::parser::Match VirtualSpecifier(LexerT& lexer);
};

} // namespace otava::parser::classes
