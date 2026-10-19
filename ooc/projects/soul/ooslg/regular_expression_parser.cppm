export module soul.slg.regular.expression.parser;

import std;
import soul.ast.re;

export namespace soul::slg {

class RegularExpressionParser : public soul::ast::re::ExprParser
{
public:
    soul::ast::re::Nfa* Parse(const std::string& expression, soul::ast::re::LexerContext* lexerContext, const std::string& fileName, int line) override;
};

} // namespace soul::slg
