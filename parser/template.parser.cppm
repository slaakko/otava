
// this file has been automatically generated from 'D:/work/otava/parser/template.parser' using soul parser generator spg version 5.2.0

export module otava.parser.templates;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::templates {

template<typename LexerT>
struct TemplateParser
{
    static soul::parser::Match TemplateDeclaration(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TemplateHead(LexerT& lexer, otava::symbols::Context* context, bool begin);
    static soul::parser::Match TemplateParameterList(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TemplateParameter(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeParameter(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeParameterKey(LexerT& lexer);
    static soul::parser::Match Typename(LexerT& lexer);
    static soul::parser::Match Class(LexerT& lexer);
    static soul::parser::Match TemplateId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match SimpleTemplateId(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TemplateName(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TemplateArgumentList(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* templateIdNode);
    static soul::parser::Match TemplateArgument(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* templateIdNode, int index);
    static soul::parser::Match TemplateArgNext(LexerT& lexer);
    static soul::parser::Match DeductionGuide(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Arrow(LexerT& lexer);
    static soul::parser::Match ExplicitInstantiation(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ExplicitSpecialization(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match Extern(LexerT& lexer);
    static soul::parser::Match Template(LexerT& lexer);
};

} // namespace otava::parser::templates
