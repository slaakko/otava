export module soul.spg.code_modifier;

import std;
import soul.spg.nonterminal_info;
import soul.ast.cpp;
import soul.ast.common;
import soul.lexer.file_map;

export namespace soul::spg {

void ModifyCode(soul::ast::cpp::CompoundStatementNode* code, bool ptrType, const std::string& nonterminalName, const std::vector<NonterminalInfo>& nonterminalInfos,
    soul::ast::cpp::TypeIdNode* returnType, bool noDebugSupport, const std::string& currentRuleName, soul::ast::common::TokenMap* tokenMap,
    soul::lexer::FileMap& fileMap);

} // namespace soul::spg
