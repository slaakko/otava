module soul.spg.code_modifier;

import soul.ast;
import soul.lexer.error;
import soul.spg.parsing_util;

namespace soul::spg {

class CodeModifierVisitor : public soul::ast::cpp::DefaultVisitor
{
public:
    CodeModifierVisitor(bool ptrType_, const std::vector<NonterminalInfo>& nonterminalInfos_, soul::ast::cpp::TypeIdNode* returnType_, bool noDebugSupport_,
        const std::string& ruleName_, soul::ast::common::TokenMap* tokenMap_, soul::lexer::FileMap& fileMap_);
    void Visit(soul::ast::cpp::IdExprNode& node) override;
    void Visit(soul::ast::cpp::ReturnStatementNode& node) override;
private:
    bool ptrType;
    const std::vector<NonterminalInfo>& nonterminalInfos;
    soul::ast::cpp::TypeIdNode* returnType;
    bool noDebugSupport;
    std::string ruleName;
    soul::ast::common::TokenMap* tokenMap;
    soul::lexer::FileMap& fileMap;
};

CodeModifierVisitor::CodeModifierVisitor(
    bool ptrType_, const std::vector<NonterminalInfo>& nonterminalInfos_, soul::ast::cpp::TypeIdNode* returnType_, bool noDebugSupport_, const std::string& ruleName_,
    soul::ast::common::TokenMap* tokenMap_, soul::lexer::FileMap& fileMap_) :
    ptrType(ptrType_), nonterminalInfos(nonterminalInfos_), returnType(returnType_), noDebugSupport(noDebugSupport_), ruleName(ruleName_), tokenMap(tokenMap_),
    fileMap(fileMap_)
{
}

void CodeModifierVisitor::Visit(soul::ast::cpp::IdExprNode& node)
{
    soul::ast::cpp::Visitor::Visit(node);
    for (const auto& info : nonterminalInfos)
    {
        if (node.Id() == info.nonterminalParser->InstanceName())
        {
            if (info.ptrType)
            {
                if (!soul::spg::parsing::util::IsGetNode(node))
                {
                    node.SetId(info.nonterminalParser->InstanceName() + ".release()");
                }
            }
            else
            {
                node.SetId(info.nonterminalParser->InstanceName() + "->value");
            }
        }
    }
    std::vector<soul::ast::common::Token*> tokens = tokenMap->GetTokens(node.Id());
    if (tokens.size() == 1)
    {
        soul::ast::common::Token* token = tokens.front();
        node.SetId(token->FullCppId());
    }
}

void CodeModifierVisitor::Visit(soul::ast::cpp::ReturnStatementNode& node)
{
    soul::ast::SourcePos sourcePos = node.GetSourcePos();
    soul::ast::cpp::DefaultVisitor::Visit(node);
    if (node.Expr())
    {
        std::vector<soul::ast::cpp::Node*> matchArgs;
        matchArgs.push_back(new soul::ast::cpp::LiteralNode(sourcePos, "true"));
        if (!ptrType)
        {
            if (!returnType)
            {
                std::string errorMessage = soul::lexer::MakeMessage("error", "return type is null", node.GetSourcePos(), fileMap);
                throw std::runtime_error(errorMessage);
            }
            soul::ast::cpp::TypeIdNode* typeId = new soul::ast::cpp::TypeIdNode(sourcePos);
            typeId->Add(new soul::ast::cpp::TypeSpecifierNode(sourcePos, "soul::parser::Value<" + returnType->ToString() + ">"));
            soul::ast::cpp::NewNode* newNode = new soul::ast::cpp::NewNode(sourcePos, false);
            newNode->SetTypeId(typeId);
            newNode->BeginAddToInitializer();
            newNode->Add(node.Expr()->Clone());
            newNode->EndAddToInitializer();
            matchArgs.push_back(newNode);
        }
        else
        {
            matchArgs.push_back(node.Expr()->Clone());
        }
        soul::ast::cpp::InvokeNode* invokeMatch = new soul::ast::cpp::InvokeNode(sourcePos, new soul::ast::cpp::IdExprNode(sourcePos, "soul::parser::Match"));
        for (auto arg : matchArgs)
        {
            invokeMatch->Add(arg);
        }
        if (!noDebugSupport)
        {
            soul::ast::cpp::StatementNode* parent = static_cast<soul::ast::cpp::StatementNode*>(node.Parent());
            if (parent->IsCompoundStatementNode())
            {
                soul::ast::cpp::CompoundStatementNode* debugBlock = new soul::ast::cpp::CompoundStatementNode(sourcePos);
                soul::ast::cpp::IfdefStatementNode* ifdefStatementNode = new soul::ast::cpp::IfdefStatementNode(sourcePos,
                    new soul::ast::cpp::LiteralNode(sourcePos, "SOUL_PARSER_DEBUG_SUPPORT"));
                debugBlock->Add(ifdefStatementNode);
                soul::ast::cpp::InvokeNode* invokeNode = new soul::ast::cpp::InvokeNode(sourcePos, 
                    new soul::ast::cpp::IdExprNode(sourcePos, "soul::lexer::WriteSuccessToLog"));
                invokeNode->Add(new soul::ast::cpp::IdExprNode(sourcePos, "lexer"));
                invokeNode->Add(new soul::ast::cpp::IdExprNode(sourcePos, "parser_debug_match_pos"));
                invokeNode->Add(new soul::ast::cpp::LiteralNode(sourcePos, "\"" + ruleName + "\""));
                soul::ast::cpp::ExpressionStatementNode* expressionStatementNode = new soul::ast::cpp::ExpressionStatementNode(sourcePos, invokeNode);
                soul::ast::cpp::IfStatementNode* ifStatementNode = new soul::ast::cpp::IfStatementNode(sourcePos,
                    new soul::ast::cpp::LiteralNode(sourcePos, "parser_debug_write_to_log"),
                    expressionStatementNode, nullptr);
                debugBlock->Add(ifStatementNode);
                soul::ast::cpp::EndIfStatementNode* endifStatementNode = new soul::ast::cpp::EndIfStatementNode(sourcePos);
                debugBlock->Add(endifStatementNode);
                soul::ast::cpp::StatementNode* returnStmtNode = new soul::ast::cpp::ReturnStatementNode(sourcePos, invokeMatch);
                debugBlock->Add(returnStmtNode);
                parent->Replace(&node, debugBlock);
            }
            else
            {
                std::string errorMessage = soul::lexer::MakeMessage("error", "return statement must be inside a compound statement", node.GetSourcePos(), fileMap);
                throw std::runtime_error(errorMessage);
            }
        }
        else
        {
            node.SetExpr(invokeMatch);
            return;
        }
    }
}

void ModifyCode(soul::ast::cpp::CompoundStatementNode* code, bool ptrType, const std::string& nonterminalName, const std::vector<NonterminalInfo>& nonterminalInfos,
    soul::ast::cpp::TypeIdNode* returnType, bool noDebugSupport, const std::string& currentRuleName, soul::ast::common::TokenMap* tokenMap,
    soul::lexer::FileMap& fileMap)
{
    CodeModifierVisitor visitor(ptrType, nonterminalInfos, returnType, noDebugSupport, currentRuleName, tokenMap, fileMap);
    code->Accept(visitor);
}

} // namespace soul::spg
