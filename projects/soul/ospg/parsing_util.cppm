export module soul.spg.parsing_util;

import std;
import soul.ast;
import soul.spg.nonterminal_info;

export namespace soul::spg::parsing::util {

bool IsGetNode(const soul::ast::cpp::IdExprNode& node);
bool IsContainedBySwitchStatment(const soul::ast::cpp::IdExprNode& node);
bool IsListParserNonterminal(const NonterminalInfo& info);
void CountNonterminals(soul::ast::cpp::CompoundStatementNode* code, std::vector<NonterminalInfo>& nonterminalInfos);
soul::ast::spg::Parser* MakeParserFromStrLiteral(const soul::ast::SourcePos& sourcePos, const std::u32string& str);

} // namespace soul::spg
