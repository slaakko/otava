module soul.slg.regular.expression.parser;

import soul.lex.re;
import soul.re.parser;
import soul.lexer;
import util;

namespace soul::slg {

soul::ast::re::Nfa* TryParse(const std::string& expression, soul::ast::re::LexerContext* lexerContext, const std::string& fileName, int line)
{
    std::u32string expr = util::ToUtf32(expression);
    auto lexer = soul::lex::re::MakeLexer(expr.c_str(), expr.c_str() + expr.length(), fileName);
    using LexerType = decltype(lexer);
    soul::ast::re::Nfa nfa = soul::re::parser::RegExParser<LexerType>::Parse(lexer, lexerContext);
    return new soul::ast::re::Nfa(nfa.Start(), nfa.End());
}

soul::ast::re::Nfa* RegularExpressionParser::Parse(const std::string& expression, soul::ast::re::LexerContext* lexerContext, const std::string& fileName, int line)
{
    try
    {
        return TryParse(expression, lexerContext, fileName, line);
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("error parsing regular expression at " + fileName + ":" + std::to_string(line) + ": " + ex.what());
    }
}

} // namespace soul::slg
