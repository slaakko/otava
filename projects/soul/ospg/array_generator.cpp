module soul.spg.array_generator;

namespace soul::spg {

class ArrayGeneratorVisitor : public soul::ast::spg::DefaultVisitor
{
public:
    ArrayGeneratorVisitor(util::CodeFormatter& formatter_, int& sn_);
    void Visit(soul::ast::spg::StringParser& parser) override;
    void Visit(soul::ast::spg::CharSetParser& parser) override;
private:
    util::CodeFormatter& formatter;
    int& sn;
};

ArrayGeneratorVisitor::ArrayGeneratorVisitor(util::CodeFormatter& formatter_, int& sn_) : formatter(formatter_), sn(sn_)
{
}

void ArrayGeneratorVisitor::Visit(soul::ast::spg::StringParser& parser)
{
    std::string s = "s" + std::to_string(sn++);
    parser.SetArrayName(s);
    formatter.Write("static constexpr std::int32_t " + s + "[] = {");
    bool first = true;
    const std::u32string& str = parser.Str();
    for (char32_t c : str)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        formatter.Write(std::to_string(static_cast<std::int32_t>(c)));
    }
    formatter.WriteLine("};");
    formatter.WriteLine();
}

void ArrayGeneratorVisitor::Visit(soul::ast::spg::CharSetParser& parser)
{
    std::string s = "s" + std::to_string(sn++);
    parser.SetArrayName(s);
    formatter.Write("static constexpr soul::ast::spg::Range " + s + "[] = {");
    bool first = true;
    soul::ast::spg::CharSet* charSet = parser.GetCharSet();
    for (const auto& range : charSet->Ranges())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        formatter.Write("{ " + std::to_string(range.first) + ", " + std::to_string(range.last) + " }");
    }
    formatter.WriteLine("};");
    formatter.WriteLine();
}

void GenerateArrays(soul::ast::spg::ParserFile& parserFile, util::CodeFormatter& formatter, int& sn)
{
    ArrayGeneratorVisitor visitor(formatter, sn);
    parserFile.Accept(visitor);
}

} // namespace soul::spg
