module soul.spg.xml_printer;

import soul.ast.spg;
import soul.xml.dom;
import util.code_formatter;

namespace soul::spg {

class XmlPrinter : public soul::ast::spg::DefaultVisitor
{
public:
    XmlPrinter(bool verbose_, bool opt_);
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
    void Visit(soul::ast::spg::GroupParser& parser) override;
    void Visit(soul::ast::spg::RuleParser& parser) override;
    void Visit(soul::ast::spg::GrammarParser& parser) override;
    void Visit(soul::ast::spg::ParserFile& parserFile) override;
private:
    bool verbose;
    bool opt;
    soul::xml::Element* currentElement;
};

XmlPrinter::XmlPrinter(bool verbose_, bool opt_) : verbose(verbose_), opt(opt_), currentElement(nullptr)
{
}

void XmlPrinter::Visit(soul::ast::spg::ChoiceParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("choice");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Left()->Accept(*this);
    parser.Right()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::SwitchParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("switch");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    for (const auto& caseParser : parser.CaseParsers())
    {
        caseParser->Accept(*this);
    }
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::CaseParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("case");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Child()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::SequenceParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("sequence");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Left()->Accept(*this);
    parser.Right()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::DifferenceParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("difference");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Left()->Accept(*this);
    parser.Right()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::ListParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("list");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Child()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::LookaheadParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("lookahead");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Child()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::KleeneParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("kleene");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Child()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::PositiveParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("positive");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Child()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::OptionalParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("optional");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Child()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::ExpectationParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("expectation");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Child()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::ActionParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("action");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Child()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::NonterminalParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("nonterminal");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("ruleName", parser.RuleName());
    currentElement->SetAttribute("first", parser.First().ToString());
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::EmptyParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("empty");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::AnyParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("any");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::TokenParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("token");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("name", parser.TokenName());
    currentElement->SetAttribute("first", parser.First().ToString());
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::GroupParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("group");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Child()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::RuleParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("rule");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("name", parser.Name());
    currentElement->SetAttribute("first", parser.First().ToString());
    parser.Definition()->Accept(*this);
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::GrammarParser& parser)
{
    soul::xml::Element* parentElement = currentElement;
    currentElement = soul::xml::MakeElement("grammar");
    parentElement->AppendChild(currentElement);
    currentElement->SetAttribute("name", parser.Name());
    for (const auto& rule : parser.Rules())
    {
        rule->Accept(*this);
    }
    currentElement = parentElement;
}

void XmlPrinter::Visit(soul::ast::spg::ParserFile& parserFile)
{
    soul::xml::Document doc;
    std::string xmlFilePath;
    if (opt)
    {
        xmlFilePath = parserFile.FilePath() + ".opt.xml";
    }
    else
    {
        xmlFilePath = parserFile.FilePath() + ".xml";
    }
    std::ofstream file(xmlFilePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    currentElement = soul::xml::MakeElement("parserFile");
    doc.AppendChild(currentElement);
    for (const auto& grammar : parserFile.Parsers())
    {
        grammar->Accept(*this);
    }
    doc.Write(formatter);
    if (verbose)
    {
        std::cout << "==> " << xmlFilePath << "\n";
    }
}

void PrintXml(soul::ast::spg::SpgFile* spgFile, bool verbose, bool opt)
{
    XmlPrinter printer(verbose, opt);
    spgFile->Accept(printer);
}

} // namespace soul::spg
