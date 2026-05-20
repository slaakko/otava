module otava.lexer.line_tokenizer;

import otava.token.line.lexer;
import util.unicode;

namespace otava::lexer {

std::vector<Token> TokenizeLine(const std::string& line, std::u32string& lineContent, bool gendoc)
{
    lineContent = util::ToUtf32(line);
    lineContent.append(1, '\n');
    std::vector<Token> tokens;
    try
    {
        auto lexer = otava::token::line::lexer::MakeLexer(lineContent.c_str(), lineContent.c_str() + lineContent.length(), "");
        ++lexer;
        while (*lexer != soul::lexer::END_TOKEN)
        {
            tokens.push_back(lexer.CurrentToken());
            ++lexer;
        }
    }
    catch (...)
    {
        if (!gendoc)
        {
            throw;
        }
    }
    return tokens;
}

} // namespace soul::cpp20::lexer
