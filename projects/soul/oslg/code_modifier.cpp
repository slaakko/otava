module soul.slg.code_modifier;

namespace soul::slg {

class CodeModifierVisitor : public soul::ast::cpp::DefaultVisitor
{
public:
    CodeModifierVisitor(soul::ast::common::TokenMap& tokenMap_, soul::ast::slg::LexerFile* lexerFile_, soul::lexer::FileMap& fileMap_);
    void Visit(soul::ast::cpp::IdExprNode& node) override;
    void Visit(soul::ast::cpp::ReturnStatementNode& node) override;
private:
    soul::ast::common::TokenMap& tokenMap;
    soul::ast::slg::LexerFile* lexerFile;
    soul::lexer::FileMap& fileMap;
    bool processReturnValue;
};

CodeModifierVisitor::CodeModifierVisitor(soul::ast::common::TokenMap& tokenMap_, soul::ast::slg::LexerFile* lexerFile_, soul::lexer::FileMap& fileMap_) :
    tokenMap(tokenMap_), lexerFile(lexerFile_), fileMap(fileMap_), processReturnValue(false)
{
}

void CodeModifierVisitor::Visit(soul::ast::cpp::IdExprNode& node)
{
    if (processReturnValue)
    {
        std::vector<soul::ast::common::Token*> tokens = tokenMap.GetTokens(node.Id());
        int n = static_cast<int>(tokens.size());
        if (n == 1)
        {
            soul::ast::common::Token* tkn = tokens.front();
            node.SetId(tkn->FullCppId());
        }
        else if (n > 1)
        {
            std::string tokenString;
            for (int i = 0; i < n; ++i)
            {
                soul::ast::common::Token* token = tokens[i];
                if (i > 0)
                {
                    tokenString.append(" or\n");
                }
                tokenString.append(token->FullName());
            }
            std::string errorMessage = soul::lexer::MakeMessage("error", "ambiguous reference to token '" + node.Id() + "' in lexer file '" +
                lexerFile->FilePath() + "':\n" + tokenString, node.GetSourcePos(), fileMap);
            throw std::runtime_error(errorMessage);
        }
    }
}

void CodeModifierVisitor::Visit(soul::ast::cpp::ReturnStatementNode& node)
{
    if (node.Expr())
    {
        processReturnValue = true;
        node.Expr()->Accept(*this);
        processReturnValue = false;
    }
}

void ModifyCode(soul::ast::cpp::CompoundStatementNode* code, soul::ast::common::TokenMap& tokenMap, soul::ast::slg::LexerFile* lexerFile, soul::lexer::FileMap& fileMap)
{
    CodeModifierVisitor visitor(tokenMap, lexerFile, fileMap);
    code->Accept(visitor);
}

} // namespce soul::slg
