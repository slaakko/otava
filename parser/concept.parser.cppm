
// this file has been automatically generated from 'D:/work/otava/parser/concept.parser' using soul parser generator spg version 5.2.0

export module otava.parser.concepts;

import std;
import soul.lexer;
import soul.parser;
import otava.symbols.context;
import otava.ast;

export namespace otava::parser::concepts {

template<typename LexerT>
struct ConceptParser
{
    static soul::parser::Match ConceptDefinition(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ConceptName(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ConceptNameChecked(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ConstraintExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RequiresClause(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ConstraintLogicalOrExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ConstraintLogicalAndExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RequiresExpression(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RequirementParameterList(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RequirementBody(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match RequirementSeq(LexerT& lexer, otava::symbols::Context* context, otava::ast::Node* requirementBody);
    static soul::parser::Match Requirement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match SimpleRequirement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeRequirement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match CompoundRequirement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match ReturnTypeRequirement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match NestedRequirement(LexerT& lexer, otava::symbols::Context* context);
    static soul::parser::Match TypeConstraint(LexerT& lexer, otava::symbols::Context* context);
};

} // namespace otava::parser::concepts
