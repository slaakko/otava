module soul.spg.error;

namespace soul::spg::error {

void ThrowRuleNameNotUnique(const std::string& fileName, soul::ast::spg::RuleParser* rule)
{
    throw std::runtime_error("error: rule name '" + rule->Name() + "' not unique in file '" + fileName + "' line " + std::to_string(rule->GetSourcePos().line));
}

} // namespace soul::spg::error
