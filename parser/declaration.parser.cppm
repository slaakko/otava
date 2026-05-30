
// this file has been automatically generated from 'D:/work/otava/parser/declaration.parser' using soul parser generator spg version 5.2.0

export module otava.parser.declaration;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::declaration {

template<typename LexerT>
struct DeclarationParser
{
    static std::unique_ptr<otava::ast::Node> Parse(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DeclarationSpecifierSequence(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DeclarationSeq(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NamespaceDeclarationSeq(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RBraceNext(LexerT& lexer);
    static soul::parser::Match Declaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match BlockDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NoDeclSpecFunctionDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match EmptyDeclaration(LexerT& lexer);
    static soul::parser::Match SimpleDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AsmDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Asm(LexerT& lexer);
    static soul::parser::Match LinkageSpecification(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NamespaceDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NamedNamespaceDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UnnamedNamespaceDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NestedNamespaceDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NamespaceAliasDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match QualifiedNamespaceSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NamespaceBody(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NestedNamespaceSpecifier(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* container);
    static soul::parser::Match Namespace(LexerT& lexer);
    static soul::parser::Match NamespaceName(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UsingDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Using(LexerT& lexer);
    static soul::parser::Match UsingDeclaratorList(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UsingDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UsingEnumDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match UsingDirective(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match StaticAssertDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match StaticAssert(LexerT& lexer);
    static soul::parser::Match AliasDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DeclSpecifierSeq(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Inline(LexerT& lexer);
    static soul::parser::Match DeclSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match StorageClassSpecifier(LexerT& lexer);
    static soul::parser::Match FunctionSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ExplicitSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match InitDeclaratorList(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match InitDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DeclaratorPushClassScope(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Declarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match PtrDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NoPtrDeclarator(LexerT& lexer, otava::symbols::Context* context, bool trailingDecl);
    static soul::parser::Match ParametersAndQualifiers(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TrailingReturnType(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match PtrOperator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AbstractDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NoPtrAbstractDeclarator(LexerT& lexer, otava::symbols::Context* context, bool trailingDecl);
    static soul::parser::Match PtrAbstractDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AbstractPackDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NoPtrAbstractPackDeclarator(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match DeclaratorId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Ptr(LexerT& lexer);
    static soul::parser::Match LvalueRef(LexerT& lexer);
    static soul::parser::Match RvalueRef(LexerT& lexer);
    static soul::parser::Match RefQualifier(LexerT& lexer);
    static soul::parser::Match CVQualifierSeq(LexerT& lexer);
    static soul::parser::Match CVQualifier(LexerT& lexer);
    static soul::parser::Match NoexceptSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ThrowSpecifier(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match AttributeDeclaration(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::declaration
