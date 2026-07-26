export module soul.spg.error;

import std;
import soul.ast.spg;

export namespace soul::spg::error {

[[noreturn]]
void ThrowRuleNameNotUnique(const std::string& fileName, soul::ast::spg::RuleParser* rule);

} // namespace soul::spg::error
