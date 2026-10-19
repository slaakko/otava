export module soul.slg.code_modifier;

import std;
import soul.ast;
import soul.lexer;

export namespace soul::slg {

void ModifyCode(soul::ast::cpp::CompoundStatementNode* code, soul::ast::common::TokenMap& tokenMap, soul::ast::slg::LexerFile* lexerFile, soul::lexer::FileMap& fileMap);

} // namespce soul::slg
