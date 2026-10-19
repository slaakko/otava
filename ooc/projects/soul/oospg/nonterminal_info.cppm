export module soul.spg.nonterminal_info;

import std;
import soul.ast;

export namespace soul::spg {

struct NonterminalInfo
{
    NonterminalInfo(soul::ast::spg::NonterminalParser* nonterminalParser_, bool ptrType_);
    soul::ast::spg::NonterminalParser* nonterminalParser;
    bool ptrType;
    int count;
    std::vector<soul::ast::SourcePos> sourcePositions;
};

} // namespace soul::spg
