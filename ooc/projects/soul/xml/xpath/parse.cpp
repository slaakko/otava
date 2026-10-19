module soul.xml.xpath.parse;

import soul.xml.xpath.evaluate;

namespace soul::xml::xpath {

std::unique_ptr<soul::xml::xpath::expr::Expr> TryParseXPathExpr(const std::string& xpathExpr, std::string& parsingError)
{
    try
    {
        return ParseXPathExpr(xpathExpr);
    }
    catch (const std::exception& ex)
    {
        parsingError = ex.what();
    }
    return std::unique_ptr<soul::xml::xpath::expr::Expr>();
}

} // namespace soul::xml::xpath
