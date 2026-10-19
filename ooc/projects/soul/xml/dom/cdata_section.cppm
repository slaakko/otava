export module soul.xml.cdata_section;

import std;
import soul.xml.text;
import soul.ast.source_pos;
import util.code_formatter;

export namespace soul::xml {

class Visitor;

class CDataSection : public Text
{
public:
    CDataSection(const soul::ast::SourcePos& sourcePos_);
    CDataSection(const soul::ast::SourcePos& sourcePos_, const std::string& text_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

CDataSection* MakeCDataSection(const std::string& text);

} // namespace soul::xml
