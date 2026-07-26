
// this file has been automatically generated from 'D:/work/soul-mod/tools/otava/projects/soul/cpp/identifier.parser' using soul parser generator ospg version 5.0.0

export module soul.cpp.identifier.parser;

import std;
import soul.lexer;
import soul.parser;

export namespace soul::cpp::identifier::parser {

template<typename LexerT>
struct CppIdentifierParser
{
    static soul::parser::Match CppIdentifier(LexerT& lexer);
    static soul::parser::Match QualifiedCppId(LexerT& lexer);
};

} // namespace soul::cpp::identifier::parser
