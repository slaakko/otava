export module soul.spg.code_generator_visitor;

import soul.spg.nonterminal_info;
import soul.ast;
import soul.lexer.file_map;
import util;
import std;

export namespace soul::spg {

enum class CodeGenerationStage
{
    generateInterface, generateImplementation
};

class CodeGeneratorVisitor : public soul::ast::spg::DefaultVisitor
{
public:
    CodeGeneratorVisitor(soul::ast::spg::SpgFile* spgFile_, bool verbose_, bool noDebugSupport_, const std::string& version_, soul::lexer::FileMap& fileMap_);
    void Visit(soul::ast::spg::ChoiceParser& parser) override;
    void Visit(soul::ast::spg::SwitchParser& parser) override;
    void Visit(soul::ast::spg::CaseParser& parser) override;
    void Visit(soul::ast::spg::SequenceParser& parser) override;
    void Visit(soul::ast::spg::DifferenceParser& parser) override;
    void Visit(soul::ast::spg::ListParser& parser) override;
    void Visit(soul::ast::spg::LookaheadParser& parser) override;
    void Visit(soul::ast::spg::KleeneParser& parser) override;
    void Visit(soul::ast::spg::PositiveParser& parser) override;
    void Visit(soul::ast::spg::OptionalParser& parser) override;
    void Visit(soul::ast::spg::ExpectationParser& parser) override;
    void Visit(soul::ast::spg::ActionParser& parser) override;
    void Visit(soul::ast::spg::NonterminalParser& parser) override;
    void Visit(soul::ast::spg::EmptyParser& parser) override;
    void Visit(soul::ast::spg::AnyParser& parser) override;
    void Visit(soul::ast::spg::TokenParser& parser) override;
    void Visit(soul::ast::spg::CharParser& parser) override;
    void Visit(soul::ast::spg::StringParser& parser) override;
    void Visit(soul::ast::spg::CharSetParser& parser) override;
    void Visit(soul::ast::spg::GroupParser& parser) override;
    void Visit(soul::ast::spg::RuleParser& parser) override;
    void Visit(soul::ast::spg::GrammarParser& parser) override;
    void Visit(soul::ast::spg::ParserFile& parserFile) override;
protected:
    soul::ast::spg::SpgFile* spgFile;
    bool verbose;
    bool noDebugSupport;
    std::string version;
    util::CodeFormatter* formatter;
    CodeGenerationStage stage;
    soul::ast::spg::GrammarParser* currentParser;
    soul::ast::spg::RuleParser* currentRule;
    std::vector<NonterminalInfo> nonterminalInfos;
    int parentMatchNumber;
    int setParentMatchNumber;
    int sn;
    soul::lexer::FileMap& fileMap;
};

} // namespace soul::spg
