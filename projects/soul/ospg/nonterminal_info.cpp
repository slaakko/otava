module soul.spg.nonterminal_info;

namespace soul::spg {

NonterminalInfo::NonterminalInfo(soul::ast::spg::NonterminalParser* nonterminalParser_, bool ptrType_) : nonterminalParser(nonterminalParser_), ptrType(ptrType_), count(0)
{
}

} // namespace soul::spg
