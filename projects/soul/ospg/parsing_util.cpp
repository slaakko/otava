module soul.spg.parsing_util;

namespace soul::spg::parsing::util {

bool IsGetNode(const soul::ast::cpp::IdExprNode& node)
{
    soul::ast::cpp::Node* parent = node.Parent();
    if (parent && parent->Kind() == soul::ast::cpp::NodeKind::memberAccessNode)
    {
        soul::ast::cpp::MemberAccessNode* memberAccessNode = static_cast<soul::ast::cpp::MemberAccessNode*>(parent);
        if (memberAccessNode->Member()->Kind() == soul::ast::cpp::NodeKind::idExprNode)
        {
            soul::ast::cpp::IdExprNode* memberId = static_cast<soul::ast::cpp::IdExprNode*>(memberAccessNode->Member());
            if (memberId->Id() == "get")
            {
                return true;
            }
        }
    }
    return false;
}

bool IsContainedBySwitchStatment(const soul::ast::cpp::IdExprNode& node)
{
    soul::ast::cpp::Node* parent = node.Parent();
    while (parent)
    {
        if (parent->Kind() == soul::ast::cpp::NodeKind::switchStatementNode)
        {
            return true;
        }
        parent = parent->Parent();
    }
    return false;
}

class NonterminalCountingVisitor : public soul::ast::cpp::DefaultVisitor
{
public:
    NonterminalCountingVisitor(std::vector<NonterminalInfo>& nonterminalInfos_) : nonterminalInfos(nonterminalInfos_) {}
    void Visit(soul::ast::cpp::IdExprNode& node) override;
private:
    std::vector<NonterminalInfo>& nonterminalInfos;
};

void NonterminalCountingVisitor::Visit(soul::ast::cpp::IdExprNode& node)
{
    for (auto& info : nonterminalInfos)
    {
        if (node.Id() == info.nonterminalParser->InstanceName() && !IsGetNode(node) && !IsContainedBySwitchStatment(node))
        {
            ++info.count;
            info.sourcePositions.push_back(node.GetSourcePos());
        }
    }
}

void CountNonterminals(soul::ast::cpp::CompoundStatementNode* code, std::vector<NonterminalInfo>& nonterminalInfos)
{
    NonterminalCountingVisitor visitor(nonterminalInfos);
    code->Accept(visitor);
}

bool IsListParserNonterminal(const NonterminalInfo& info)
{
    if (info.count != 2)
    {
        return false;
    }
    soul::ast::spg::NonterminalParser* nonterminalParser = info.nonterminalParser;
    soul::ast::spg::Parser* parent = nonterminalParser->Parent();
    while (parent)
    {
        if (parent->IsListParser())
        {
            return true;
        }
        parent = parent->Parent();
    }
    return false;
}

soul::ast::spg::Parser* MakeParserFromStrLiteral(const soul::ast::SourcePos& sourcePos, const std::u32string& str)
{
    return nullptr; // todo
}

} // namespace soul::spg
