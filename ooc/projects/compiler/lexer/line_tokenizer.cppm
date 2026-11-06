export module otava.lexer.line_tokenizer;

import std;
import soul.lexer;

export namespace otava::lexer {

using Token = soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>;
    
std::vector<Token> TokenizeLine(const std::string& line, std::u32string& lineContent, bool gendoc);

} // namespace soul::cpp20::lexer
