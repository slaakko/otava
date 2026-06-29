export module soul.ast.slg;

import std;
import soul.ast.common;
import soul.ast.cpp;
import soul.ast.source_pos;

export namespace soul::ast::slg {

class Keyword
{
public:
    Keyword(const soul::ast::SourcePos& sourcePos_, const std::string& str_, const std::string& tokenName_, std::int64_t tokenId_);
    Keyword(const soul::ast::SourcePos& sourcePos_, const std::string& str_, const std::string& tokenName_);
    inline const soul::ast::SourcePos& GetSourcePos() const noexcept { return sourcePos; }
    inline void SetCollection(soul::ast::common::Collection* collection_) noexcept { collection = collection_; }
    inline soul::ast::common::Collection* GetCollection() const noexcept { return collection; }
    inline const std::string& Str() const noexcept { return str; }
    inline const std::string& TokenName() const noexcept { return tokenName; }
    inline std::int64_t TokenId() const noexcept { return tokenId; }
    inline void SetTokenId(std::int64_t tokenId_) noexcept { tokenId = tokenId_; }
private:
    soul::ast::SourcePos sourcePos;
    std::string str;
    std::string tokenName;
    std::int64_t tokenId;
    soul::ast::common::Collection* collection;
};

class KeywordCollection : public soul::ast::common::Collection
{
public:
    KeywordCollection(const std::string& name_);
    void AddKeyword(Keyword* keyword);
    inline const std::vector<std::unique_ptr<Keyword>>& GetKeywords() const noexcept { return keywords; }
private:
    std::vector<std::unique_ptr<Keyword>> keywords;
};

class KeywordFile : public soul::ast::common::File
{
public:
    KeywordFile(const std::string& filePath_);
    void SetKeywordCollection(KeywordCollection* keywordCollection_) noexcept;
    inline KeywordCollection* GetKeywordCollection() const noexcept { return keywordCollection.get(); }
    void AddImport(soul::ast::common::Import* imp);
    inline const std::vector<std::unique_ptr<soul::ast::common::Import>>& Imports() const noexcept { return imports; }
private:
    std::unique_ptr<KeywordCollection> keywordCollection;
    std::vector<std::unique_ptr<soul::ast::common::Import>> imports;
};

class Expression
{
public:
    Expression(const std::string& id_, const std::string& value_, int line_);
    inline void SetCollection(soul::ast::common::Collection* collection_) noexcept { collection = collection_; }
    inline soul::ast::common::Collection* GetCollection() const noexcept { return collection; }
    inline int Index() const noexcept { return index; }
    inline void SetIndex(int index_) noexcept { index = index_; }
    inline const std::string& Id() const noexcept { return id; }
    inline const std::string& Value() const noexcept { return value; }
    const std::string& FileName() const noexcept;
    inline int Line() const noexcept { return line; }
private:
    int index;
    std::string id;
    std::string value;
    int line;
    soul::ast::common::Collection* collection;
};

class ExpressionCollection : public soul::ast::common::Collection
{
public:
    ExpressionCollection(const std::string& name_);
    void AddExpression(Expression* expression);
    inline const std::vector<std::unique_ptr<Expression>>& GetExpressions() const noexcept { return expressions; }
private:
    std::vector<std::unique_ptr<Expression>> expressions;
};

class ExpressionFile : public soul::ast::common::File
{
public:
    ExpressionFile(const std::string& filePath_);
    void SetExpressionCollection(ExpressionCollection* expressionCollection_) noexcept;
    inline ExpressionCollection* GetExpressionCollection() const noexcept { return expressionCollection.get(); }
private:
    std::unique_ptr<ExpressionCollection> expressionCollection;
};

class Rule
{
public:
    Rule(const std::string& expr_, soul::ast::cpp::CompoundStatementNode* code_, int action_, int line_);
    inline int Index() const noexcept { return index; }
    inline void SetIndex(int index_) noexcept { index = index_; }
    inline void SetCollection(soul::ast::common::Collection* collection_) noexcept { collection = collection_; }
    inline soul::ast::common::Collection* GetCollection() const noexcept { return collection; }
    inline const std::string& Expr() const noexcept { return expr; }
    inline soul::ast::cpp::CompoundStatementNode* Code() const noexcept { return code.get(); }
    inline int GetAction() const noexcept { return action; }
    inline int Line() const noexcept { return line; }
    inline int NfaIndex() const noexcept { return nfaIndex; }
    inline void SetNfaIndex(int nfaIndex_) noexcept { nfaIndex = nfaIndex_; }
private:
    int index;
    std::string expr;
    std::unique_ptr<soul::ast::cpp::CompoundStatementNode> code;
    int action;
    int line;
    soul::ast::common::Collection* collection;
    int nfaIndex;
};

class Variable
{
public:
    Variable(soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    inline soul::ast::cpp::TypeIdNode* Type() const noexcept { return type.get(); }
    inline const std::string& Name() const noexcept { return name; }
private:
    std::unique_ptr<soul::ast::cpp::TypeIdNode> type;
    std::string name;
};

class Action
{
public:
    Action(int id_, soul::ast::cpp::CompoundStatementNode* code_) noexcept;
    inline int Id() const noexcept { return id; }
    inline soul::ast::cpp::CompoundStatementNode* Code() const noexcept { return code.get(); }
private:
    int id;
    std::unique_ptr<soul::ast::cpp::CompoundStatementNode> code;
};

class Actions
{
public:
    void AddAction(Action* action);
    Action* GetAction(int id) const noexcept;
private:
    std::vector<std::unique_ptr<Action>> actions;
    std::map<int, Action*> actionMap;
};

struct Using
{
    Using(const soul::ast::SourcePos& sourcePos_, const std::string& fullName_);
    soul::ast::SourcePos sourcePos;
    std::string fullName;
};

class Lexer : public soul::ast::common::Collection
{
public:
    Lexer(const std::string& name_);
    void AddRule(Rule* rule);
    inline const std::vector<std::unique_ptr<Rule>>& Rules() const noexcept { return rules; }
    void AddVariable(Variable* variable);
    inline const std::vector<std::unique_ptr<Variable>>& Variables() const noexcept { return variables; }
    void AddAction(Action* action);
    inline const Actions& GetActions() const noexcept { return actions; }
    inline const std::string& VariableClassName() const noexcept { return variableClassName; }
    void SetVariableClassName(const std::string& variableClassName_);
    void AddUsing(const soul::ast::SourcePos& sourcePos, const std::string& fullName);
    inline const std::vector<Using>& Usings() const noexcept { return usings; }
private:
    std::vector<std::unique_ptr<Rule>> rules;
    std::vector<std::unique_ptr<Variable>> variables;
    Actions actions;
    std::string variableClassName;
    std::vector<Using> usings;
};

class LexerFile : public soul::ast::common::File
{
public:
    LexerFile(const std::string& filePath_);
    void SetExportModule(soul::ast::common::ExportModule* exportModule_) noexcept;
    inline soul::ast::common::ExportModule* GetExportModule() const noexcept { return exportModule.get(); }
    void AddImport(soul::ast::common::Import* imp);
    inline const std::vector<std::unique_ptr<soul::ast::common::Import>>& Imports() const noexcept { return imports; }
    void SetLexer(Lexer* lexer_);
    Lexer* GetLexer() const noexcept { return lexer.get(); }
    inline soul::ast::common::TokenMap* GetTokenMap() noexcept { return &tokenMap; }
private:
    std::unique_ptr<soul::ast::common::ExportModule> exportModule;
    std::vector<std::unique_ptr<soul::ast::common::Import>> imports;
    std::unique_ptr<Lexer> lexer;
    soul::ast::common::TokenMap tokenMap;
};

enum class SlgFileDeclarationKind
{
    tokenFileDeclaration, keywordFileDeclaration, expressionFileDeclaration, lexerFileDeclararation
};

class SlgFileDeclaration
{
public:
    SlgFileDeclaration(SlgFileDeclarationKind kind_, const std::string& filePath_);
    virtual ~SlgFileDeclaration();
    inline SlgFileDeclarationKind Kind() const noexcept { return kind; }
    inline const std::string& FilePath() const noexcept { return filePath; }
private:
    SlgFileDeclarationKind kind;
    std::string filePath;
};

class TokenFileDeclaration : public SlgFileDeclaration
{
public:
    TokenFileDeclaration(const std::string& filePath_, bool external_);
    inline bool External() const noexcept { return external; }
private:
    bool external;
};

class KeywordFileDeclaration : public SlgFileDeclaration
{
public:
    KeywordFileDeclaration(const std::string& filePath_);
};

class ExpressionFileDeclaration : public SlgFileDeclaration
{
public:
    ExpressionFileDeclaration(const std::string& filePath_);
};

class LexerFileDeclaration : public SlgFileDeclaration
{
public:
    LexerFileDeclaration(const std::string& filePath_);
};

class SlgFile : public soul::ast::common::File
{
public:
    SlgFile(const std::string& filePath_, const std::string& projectName_);
    inline const std::string& ProjectName() const noexcept { return projectName; }
    void AddDeclaration(SlgFileDeclaration* declaration);
    inline const std::vector<std::unique_ptr<SlgFileDeclaration>>& Declarations() const noexcept { return declarations; }
    void AddTokenFile(soul::ast::common::TokenFile* tokenFile);
    inline const std::vector<std::unique_ptr<soul::ast::common::TokenFile>>& TokenFiles() const noexcept { return tokenFiles; }
    void AddKeywordFile(KeywordFile* keywordFile);
    inline const std::vector<std::unique_ptr<KeywordFile>>& KeywordFiles() const noexcept { return keywordFiles; }
    void AddExpressionFile(ExpressionFile* expressionFile);
    inline const std::vector<std::unique_ptr<ExpressionFile>>& ExpressionFiles() const noexcept { return expressionFiles; }
    void AddLexerFile(LexerFile* lexerFile);
    inline const std::vector<std::unique_ptr<LexerFile>>& LexerFiles() const noexcept { return lexerFiles; }
    inline const std::vector<soul::ast::common::Collection*>& Collections() const noexcept { return collections; }
    soul::ast::common::Collection* GetCollection(const std::string& name) const noexcept;
private:
    std::string projectName;
    std::vector<std::unique_ptr<SlgFileDeclaration>> declarations;
    std::vector<std::unique_ptr<soul::ast::common::TokenFile>> tokenFiles;
    std::vector<std::unique_ptr<KeywordFile>> keywordFiles;
    std::vector<std::unique_ptr<ExpressionFile>> expressionFiles;
    std::vector<std::unique_ptr<LexerFile>> lexerFiles;
    std::vector<soul::ast::common::Collection*> collections;
    std::map<std::string, soul::ast::common::Collection*> collectionMap;
};

class Tokens
{
public:
    Tokens();
    void AddToken(soul::ast::common::Token* token);
    inline const std::vector<soul::ast::common::Token*>& GetTokens() const noexcept { return tokens; }
private:
    std::vector<soul::ast::common::Token*> tokens;
};

class Keywords
{
public:
    Keywords();
    void AddKeyword(Keyword* keyword);
    inline const std::vector<Keyword*>& GetKeywords() const noexcept { return keywords; }
private:
    std::vector<Keyword*> keywords;
};

class Expressions
{
public:
    Expressions();
    void AddExpression(Expression* expression);
    Expression* GetExpression(const std::string& id) const noexcept;
private:
    std::vector<Expression*> expressions;
    std::map<std::string, Expression*> expressionMap;
};

} // namespace soul::ast::slg
