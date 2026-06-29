export module soul.ast.spg;

import std;
import soul.ast.common;
import soul.ast.cpp;
import soul.ast.common;
import soul.ast.source_pos;

export namespace soul::ast::spg {

struct Range
{
    std::int32_t first;
    std::int32_t last;
};

enum class ParamVarKind
{
    parameter, variable
};

class ParamVar
{
public:
    ParamVar(const soul::ast::SourcePos& sourcePos_, ParamVarKind kind_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    virtual ~ParamVar();
    inline const soul::ast::SourcePos& GetSourcePos() const noexcept { return sourcePos; }
    inline ParamVarKind Kind() const noexcept { return kind; }
    inline soul::ast::cpp::TypeIdNode* Type() const noexcept { return type.get(); }
    inline const std::string& Name() const noexcept { return name; }
    virtual ParamVar* Clone() const = 0;
private:
    soul::ast::SourcePos sourcePos;
    ParamVarKind kind;
    std::unique_ptr<soul::ast::cpp::TypeIdNode> type;
    std::string name;
};

class Parameter : public ParamVar
{
public:
    Parameter(const soul::ast::SourcePos& sourcePos_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    ParamVar* Clone() const override;
};

class Variable : public ParamVar
{
public:
    Variable(const soul::ast::SourcePos& sourcePos_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    ParamVar* Clone() const override;
};

class CharSet
{
public:
    CharSet();
    CharSet* Clone() const;
    inline bool Inverse() const noexcept { return inverse; }
    inline void SetInverse() noexcept { inverse = true; }
    const std::vector<Range>& Ranges() const noexcept { return ranges; }
    void AddRange(const Range& range);
private:
    bool inverse;
    std::vector<Range> ranges;
};

struct TokenIdLess
{
    inline bool operator()(const soul::ast::common::Token* left, const soul::ast::common::Token* right) const noexcept
    {
        return left->Id() < right->Id();
    }
};

class TokenSet
{
public:
    TokenSet();
    bool AddToken(const soul::ast::common::Token* token);
    bool Merge(const TokenSet& that);
    bool Contains(const soul::ast::common::Token* token) const noexcept;
    bool Intersects(const TokenSet& that) const noexcept;
    std::string ToString() const;
    inline const std::set<const soul::ast::common::Token*, TokenIdLess>& Tokens() const noexcept { return tokens; }
private:
    std::set<const soul::ast::common::Token*, TokenIdLess> tokens;
};

class Visitor;

enum class ParserKind
{
    alternativeParser, sequenceParser, differenceParser, listParser, lookaheadParser, kleeneParser, positiveParser, optionalParser, expectationParser, actionParser,
    nonterminalParser, emptyParser, anyParser, tokenParser, charParser, stringParser, charSetParser, groupParser, switchParser, caseParser, ruleParser, grammarParser
};

std::string ParserKindStr(ParserKind kind);

class TokenMap;

class Parser
{
public:
    Parser(const soul::ast::SourcePos& sourcePos_, ParserKind kind_);
    inline Parser* Parent() const noexcept { return parent; }
    inline void SetParent(Parser* parent_) noexcept { parent = parent_; }
    inline const soul::ast::SourcePos& GetSourcePos() const noexcept { return sourcePos; }
    virtual ~Parser();
    virtual Parser* Clone() const = 0;
    virtual void Accept(Visitor& visitor) = 0;
    virtual std::string Name() const = 0;
    virtual void ComputeFirst(bool& changed, std::set<Parser*>& visited) = 0;
    inline ParserKind Kind() const noexcept { return kind; }
    inline bool IsActionParser() const noexcept { return kind == ParserKind::actionParser; }
    inline bool IsNonterminalParser() const noexcept { return kind == ParserKind::nonterminalParser; }
    inline bool IsTokenParser() const noexcept { return kind == ParserKind::tokenParser; }
    inline bool IsListParser() const noexcept { return kind == ParserKind::listParser; }
    inline bool IsSwitchParser() const noexcept { return kind == ParserKind::switchParser; }
    inline bool IsDifferenceParser() const noexcept { return kind == ParserKind::differenceParser; }
    inline bool IsGrammarParser() const noexcept { return kind == ParserKind::grammarParser; }
    inline const TokenSet& First() const noexcept { return first; }
    inline TokenSet& First() noexcept { return first; }
    soul::ast::common::TokenMap* GetTokenMap() noexcept;
private:
    Parser* parent;
    soul::ast::SourcePos sourcePos;
    ParserKind kind;
    TokenSet first;
};

class UnaryParser : public Parser
{
public:
    UnaryParser(const soul::ast::SourcePos& sourcePos_, ParserKind kind_, Parser* child_) noexcept;
    Parser* Child() const noexcept { return child.get(); }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::unique_ptr<Parser> child;
};

class BinaryParser : public Parser
{
public:
    BinaryParser(const soul::ast::SourcePos& sourcePos_, ParserKind kind_, Parser* left_, Parser* right_) noexcept;
    inline Parser* Left() const noexcept { return left.get(); }
    inline Parser* Right() const noexcept { return right.get(); }
private:
    std::unique_ptr<Parser> left;
    std::unique_ptr<Parser> right;
};

class ChoiceParser : public BinaryParser
{
public:
    ChoiceParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "choice"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
    void SetOptimizationFlag(int& count);
    inline bool Optimize() const noexcept { return optimize; }
private:
    bool optimize;
};

class CaseParser : public UnaryParser
{
public:
    CaseParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "case"; }
};

class SwitchParser : public Parser
{
public:
    SwitchParser(const soul::ast::SourcePos& sourcePos_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "switch"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
    inline const std::vector<std::unique_ptr<CaseParser>>& CaseParsers() const noexcept { return caseParsers; }
    void AddCaseParser(CaseParser* caseParser);
private:
    std::vector<std::unique_ptr<CaseParser>> caseParsers;
};

class SequenceParser : public BinaryParser
{
public:
    SequenceParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "sequence"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class DifferenceParser : public BinaryParser
{
public:
    DifferenceParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "difference"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class ListParser : public UnaryParser
{
public:
    ListParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_) noexcept;
    Parser* Clone() const override;
    inline Parser* Left() const noexcept { return left; }
    inline Parser* Right() const noexcept { return right; }
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "list"; }
private:
    Parser* left;
    Parser* right;
};

class LookaheadParser : public UnaryParser
{
public:
    LookaheadParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "lookahead"; }
};

class KleeneParser : public UnaryParser
{
public:
    KleeneParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "kleene"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class PositiveParser : public UnaryParser
{
public:
    PositiveParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "positive"; }
};

class OptionalParser : public UnaryParser
{
public:
    OptionalParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "optional"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class ExpectationParser : public UnaryParser
{
public:
    ExpectationParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "expectation"; }
};

class ActionParser : public UnaryParser
{
public:
    ActionParser(const soul::ast::SourcePos& sourcePos_, Parser* child_, soul::ast::cpp::CompoundStatementNode* successCode_,
        soul::ast::cpp::CompoundStatementNode* failureCode_) noexcept;
    inline soul::ast::cpp::CompoundStatementNode* SuccessCode() const noexcept { return successCode.get(); }
    inline soul::ast::cpp::CompoundStatementNode* FailureCode() const noexcept { return failureCode.get(); }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "action"; }
private:
    std::unique_ptr<soul::ast::cpp::CompoundStatementNode> successCode;
    std::unique_ptr<soul::ast::cpp::CompoundStatementNode> failureCode;
};

class RuleParser;

class NonterminalParser : public Parser
{
public:
    NonterminalParser(const soul::ast::SourcePos& sourcePos_, const std::string& ruleName_, const std::string& instanceName_, soul::ast::cpp::ExprListNode* args_);
    inline const std::string& RuleName() const noexcept { return ruleName; }
    inline const std::string& InstanceName() const noexcept { return instanceName; }
    inline const std::unique_ptr<soul::ast::cpp::ExprListNode>& Arguments() const noexcept { return arguments; }
    inline void SetRule(RuleParser* rule_) noexcept { rule = rule_; }
    inline RuleParser* Rule() const noexcept { return rule; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "nonterminal"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::string ruleName;
    std::string instanceName;
    std::unique_ptr<soul::ast::cpp::ExprListNode> arguments;
    RuleParser* rule;
    bool recursive;
};

class EmptyParser : public Parser
{
public:
    EmptyParser(const soul::ast::SourcePos& sourcePos_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "empty"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class AnyParser : public Parser
{
public:
    AnyParser(const soul::ast::SourcePos& sourcePos_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "any"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class TokenParser : public Parser
{
public:
    TokenParser(const soul::ast::SourcePos& sourcePos_, const std::string& tokenName_);
    const std::string& TokenName() const noexcept { return tokenName; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "token"; }
    inline soul::ast::common::Token* GetToken() const noexcept { return token; }
    inline void SetToken(soul::ast::common::Token* token_) noexcept { token = token_; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::string tokenName;
    soul::ast::common::Token* token;
};

class CharParser : public Parser
{
public:
    CharParser(const soul::ast::SourcePos& sourcePos_, char32_t chr_) noexcept;
    inline char32_t Chr() const noexcept { return chr; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "char"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    char32_t chr;
};

class StringParser : public Parser
{
public:
    StringParser(const soul::ast::SourcePos& sourcePos_, const std::u32string& str_);
    inline const std::u32string& Str() const noexcept { return str; }
    inline const std::string& ArrayName() const noexcept { return arrayName; }
    void SetArrayName(const std::string& arrayName_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "string"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::u32string str;
    std::string arrayName;
};

class CharSetParser : public Parser
{
public:
    CharSetParser(const soul::ast::SourcePos& sourcePos_, CharSet* charSet_) noexcept;
    inline CharSet* GetCharSet() const noexcept { return charSet.get(); }
    inline const std::string& ArrayName() const noexcept { return arrayName; }
    void SetArrayName(const std::string& arrayName_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "charSet"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::unique_ptr<CharSet> charSet;
    std::string arrayName;
};

class GroupParser : public UnaryParser
{
public:
    GroupParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) noexcept;
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "group"; }
};

class GrammarParser;

class RuleParser : public Parser
{
public:
    RuleParser(const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    std::string Name() const override { return name; }
    inline void SetIndex(std::int32_t index_) noexcept { index = index_; }
    inline std::int32_t Index() const noexcept { return index; }
    inline void SetId(std::int64_t id_) noexcept { id = id_; }
    inline std::int64_t Id() const noexcept { return id; }
    void SetInfo(const std::string& info_);
    inline const std::string& Info() const noexcept { return info; }
    void AddParamOrVariable(ParamVar* paramVar);
    inline const std::vector<std::unique_ptr<Parameter>>& Params() const noexcept { return params; }
    inline const std::vector<std::unique_ptr<Variable>>& Vars() const noexcept { return vars; }
    void SetDefinition(Parser* definition_) noexcept;
    inline Parser* Definition() const noexcept { return definition.get(); }
    void SetReturnType(soul::ast::cpp::TypeIdNode* returnType_) noexcept;
    inline soul::ast::cpp::TypeIdNode* ReturnType() const noexcept { return returnType.get(); }
    void AddNonterminal(NonterminalParser* nonterminal);
    inline const std::vector<NonterminalParser*>& Nonterminals() const noexcept { return nonterminals; }
    void SetGrammar(GrammarParser* grammar_) noexcept;
    inline GrammarParser* Grammar() const noexcept { return grammar; }
    inline bool HasReturn() const noexcept { return hasReturn; }
    inline void SetHasReturn() noexcept { hasReturn = true; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::string name;
    std::int32_t index;
    std::int64_t id;
    std::string info;
    std::vector<std::unique_ptr<Parameter>> params;
    std::vector<std::unique_ptr<Variable>> vars;
    std::unique_ptr<Parser> definition;
    std::unique_ptr<soul::ast::cpp::TypeIdNode> returnType;
    std::vector<NonterminalParser*> nonterminals;
    GrammarParser* grammar;
    bool hasReturn;
    bool computingFirst;
};

struct Using
{
    Using(const soul::ast::SourcePos& sourcePos_, const std::string& fullName_);
    soul::ast::SourcePos sourcePos;
    std::string fullName;
};

class ParserFile;

class GrammarParser : public Parser
{
public:
    GrammarParser(const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    std::string Name() const override { return name; }
    inline bool Main() const noexcept { return main; }
    inline void SetMain() noexcept { main = true; }
    inline std::int32_t Id() const noexcept { return id; }
    inline void SetId(std::int32_t id_) noexcept { id = id_; }
    inline ParserFile* GetParserFile() const noexcept { return parserFile; }
    inline void SetParserFile(ParserFile* parserFile_) noexcept { parserFile = parserFile_; }
    void AddLexer(soul::ast::cpp::TypeIdNode* lexerTypeId);
    inline const std::vector<std::unique_ptr<soul::ast::cpp::TypeIdNode>>& Lexers() const noexcept { return lexers; }
    void AddUsing(const soul::ast::SourcePos& sourcePos, const std::string& fullName);
    inline const std::vector<Using>& Usings() const noexcept { return usings; }
    bool AddRule(RuleParser* rule);
    bool MapRule(RuleParser* rule);
    RuleParser* GetRule(const std::string& ruleName) const noexcept;
    inline const std::vector<std::unique_ptr<RuleParser>>& Rules() const noexcept { return rules; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::string name;
    bool main;
    std::int32_t id;
    ParserFile* parserFile;
    std::vector<std::unique_ptr<soul::ast::cpp::TypeIdNode>> lexers;
    std::vector<Using> usings;
    std::vector<std::unique_ptr<RuleParser>> rules;
    std::map<std::string, RuleParser*> ruleMap;
};

class SpgFileDeclaration
{
public:
    SpgFileDeclaration(const soul::ast::SourcePos& sourcePos_, soul::ast::common::FileKind fileKind_, const std::string& filePath_);
    virtual ~SpgFileDeclaration();
    inline const soul::ast::SourcePos& GetSourcePos() const noexcept { return sourcePos; }
    inline soul::ast::common::FileKind GetFileKind() const noexcept { return fileKind; }
    inline const std::string& FilePath() const noexcept { return filePath; }
private:
    soul::ast::SourcePos sourcePos;
    soul::ast::common::FileKind fileKind;
    std::string filePath;
};

class ParserFileDeclaration : public SpgFileDeclaration
{
public:
    ParserFileDeclaration(const soul::ast::SourcePos& sourcePos_, const std::string& filePath_, bool external_);
    inline bool External() const noexcept { return external; }
private:
    bool external;
};

class TokenFileDeclaration : public SpgFileDeclaration
{
public:
    TokenFileDeclaration(const soul::ast::SourcePos& sourcePos_, const std::string& filePath_, bool external_);
    inline bool External() const noexcept { return external; }
private:
    bool external;
};

class ParserFile : public soul::ast::common::File
{
public:
    ParserFile(const std::string& filePath_);
    void SetExportModule(soul::ast::common::ExportModule* exportModule_);
    inline soul::ast::common::ExportModule* GetExportModule() const noexcept { return exportModule.get(); }
    void AddImport(soul::ast::common::Import* imprt);
    inline const std::vector<std::unique_ptr<soul::ast::common::Import>>& Imports() const noexcept { return imports; }
    void AddParser(soul::ast::spg::GrammarParser* parser);
    inline const std::vector<std::unique_ptr<soul::ast::spg::GrammarParser>>& Parsers() const noexcept { return parsers; }
    inline bool IsExternal() const noexcept { return external; }
    inline void SetExternal() noexcept { external = true; }
    void Accept(soul::ast::common::Visitor& visitor) override;
    inline soul::ast::common::TokenMap* GetTokenMap() noexcept { return &tokenMap; }
private:
    std::unique_ptr<soul::ast::common::ExportModule> exportModule;
    std::vector<std::unique_ptr<soul::ast::common::Import>> imports;
    std::vector<std::unique_ptr<soul::ast::spg::GrammarParser>> parsers;
    soul::ast::common::TokenMap tokenMap;
    bool external;
};

class SpgFile : public soul::ast::common::File
{
public:
    SpgFile(const std::string& filePath_, const std::string& projectName_);
    const std::string& ProjectName() const noexcept { return projectName; }
    void AddDeclaration(SpgFileDeclaration* declaration);
    inline const std::vector<std::unique_ptr<SpgFileDeclaration>>& Declarations() const noexcept { return declarations; }
    void AddParserFile(ParserFile* parserFile);
    inline const std::vector<std::unique_ptr<ParserFile>>& ParserFiles() const noexcept { return parserFiles; }
    void AddTokenFile(soul::ast::common::TokenFile* tokenFile);
    inline const std::vector<std::unique_ptr<soul::ast::common::TokenFile>>& TokenFiles() const noexcept { return tokenFiles; }
    bool AddParser(GrammarParser* parser);
    GrammarParser* GetParser(const std::string& name) const noexcept;
    void AddRule(RuleParser* rule);
    inline const std::vector<RuleParser*>& Rules() const noexcept { return rules; }
    soul::ast::common::TokenCollection* GetTokenCollection(const std::string& tokenCollectionName) const noexcept;
    void Accept(soul::ast::common::Visitor& visitor) override;
private:
    std::string projectName;
    std::vector<std::unique_ptr<SpgFileDeclaration>> declarations;
    std::vector<std::unique_ptr<ParserFile>> parserFiles;
    std::vector<std::unique_ptr<soul::ast::common::TokenFile>> tokenFiles;
    std::map<std::string, soul::ast::common::TokenCollection*> tokenCollectionMap;
    std::map<std::string, GrammarParser*> parserMap;
    std::vector<RuleParser*> rules;
};

class Visitor : public soul::ast::common::Visitor
{
public:
    virtual void Visit(ChoiceParser& parser) {}
    virtual void Visit(SwitchParser& parser) {}
    virtual void Visit(CaseParser& parser) {}
    virtual void Visit(SequenceParser& parser) {}
    virtual void Visit(DifferenceParser& parser) {}
    virtual void Visit(ListParser& parser) {}
    virtual void Visit(LookaheadParser& parser) {}
    virtual void Visit(KleeneParser& parser) {}
    virtual void Visit(PositiveParser& parser) {}
    virtual void Visit(OptionalParser& parser) {}
    virtual void Visit(ExpectationParser& parser) {}
    virtual void Visit(ActionParser& parser) {}
    virtual void Visit(NonterminalParser& parser) {}
    virtual void Visit(EmptyParser& parser) {}
    virtual void Visit(AnyParser& parser) {}
    virtual void Visit(TokenParser& parser) {}
    virtual void Visit(CharParser& parser) {}
    virtual void Visit(StringParser& parser) {}
    virtual void Visit(CharSetParser& parser) {}
    virtual void Visit(GroupParser& parser) {}
    virtual void Visit(RuleParser& parser) {}
    virtual void Visit(GrammarParser& parser) {}
    virtual void Visit(ParserFile& parserFile) {}
    virtual void Visit(SpgFile& spgFile) {}
};

class DefaultVisitor : public Visitor
{
public:
    void Visit(ChoiceParser& parser) override;
    void Visit(SwitchParser& parser) override;
    void Visit(CaseParser& parser) override;
    void Visit(SequenceParser& parser) override;
    void Visit(DifferenceParser& parser) override;
    void Visit(ListParser& parser) override;
    void Visit(LookaheadParser& parser) override;
    void Visit(KleeneParser& parser) override;
    void Visit(PositiveParser& parser) override;
    void Visit(OptionalParser& parser) override;
    void Visit(ExpectationParser& parser) override;
    void Visit(ActionParser& parser) override;
    void Visit(GroupParser& parser) override;
    void Visit(RuleParser& parser) override;
    void Visit(GrammarParser& parser) override;
    void Visit(ParserFile& parserFile) override;
    void Visit(SpgFile& spgFile) override;
};

} // namespace soul::ast::spg
