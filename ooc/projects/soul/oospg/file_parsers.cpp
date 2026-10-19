module soul.spg.file_parsers;

import util;
import soul.lexer;
import soul.lex.spg;
import soul.spg.spg.file.parser;
import soul.spg.parser.file.parser;
import soul.spg.parsers.rules;
import soul.common.token.file.parser;
import common.parser.rules;

namespace soul::spg {

std::unique_ptr<soul::ast::spg::SpgFile> ParseSpgFile(const std::string& spgFilePath, soul::lexer::FileMap& fileMap, bool verbose)
{
    if (!util::FileExists(spgFilePath))
    {
        throw std::runtime_error("SPG file '" + spgFilePath + "' not found");
    }
    if (verbose)
    {
        std::cout << "> " << spgFilePath << "\n";
    }
    std::int32_t file = fileMap.MapFile(spgFilePath);
    std::string spgFileContent = util::ReadFile(spgFilePath);
    std::u32string content = util::ToUtf32(spgFileContent);
    auto lexer = soul::lex::spg::MakeLexer(content.c_str(), content.c_str() + content.length(), spgFilePath);
    lexer.SetRuleNameMapPtr(soul::spg::parsers::rules::GetRuleNameMapPtr());
    lexer.SetFile(file);
    using LexerType = decltype(lexer);
    soul::lex::spg::SpgLexer_Variables* vars = static_cast<soul::lex::spg::SpgLexer_Variables*>(lexer.GetVariables());
    vars->matchFilePath = true;
    std::unique_ptr<soul::ast::spg::SpgFile> spgFile = soul::spg::spg::file::parser::SpgFileParser<soul::lexer::Lexer<soul::lex::spg::SpgLexer<char32_t>, char32_t>>::Parse(lexer);
    fileMap.AddFileContent(file, std::move(content), std::move(lexer.GetLineStartIndeces()));
    return spgFile;
}

std::unique_ptr<soul::ast::spg::ParserFile> ParseParserFile(const std::string& parserFilePath, const soul::ast::SourcePos& sourcePos, soul::lexer::FileMap& fileMap,
    bool verbose, bool external)
{
    if (!util::FileExists(parserFilePath))
    {
        std::string errorMessage = soul::lexer::MakeMessage("error", "parser file '" + parserFilePath + "' not found", sourcePos, fileMap);
        throw std::runtime_error(errorMessage);
    }
    if (verbose)
    {
        std::cout << "> " << parserFilePath << "\n";
    }
    std::int32_t file = fileMap.MapFile(parserFilePath);
    std::string parserFileContent = util::ReadFile(parserFilePath);
    std::u32string content = util::ToUtf32(parserFileContent);
    auto lexer = soul::lex::spg::MakeLexer(content.c_str(), content.c_str() + content.length(), parserFilePath);
    lexer.SetRuleNameMapPtr(soul::spg::parsers::rules::GetRuleNameMapPtr());
    lexer.SetFile(file);
    using LexerType = decltype(lexer);
    std::unique_ptr<soul::ast::spg::ParserFile> parserFile = soul::spg::parser::file::parser::ParserFileParser<LexerType>::Parse(lexer);
    fileMap.AddFileContent(file, std::move(content), std::move(lexer.GetLineStartIndeces()));
    if (external)
    {
        parserFile->SetExternal();
    }
    return parserFile;
}

std::unique_ptr<soul::ast::common::TokenFile> ParseTokenFile(const std::string& tokenFilePath, bool external)
{
    std::string tokenFileContent = util::ReadFile(tokenFilePath);
    std::u32string content = util::ToUtf32(tokenFileContent);
    auto lexer = soul::lex::spg::MakeLexer(content.c_str(), content.c_str() + content.length(), tokenFilePath);
    lexer.SetRuleNameMapPtr(::common::parser::rules::GetRuleNameMapPtr());
    using LexerType = decltype(lexer);
    std::unique_ptr<soul::ast::common::TokenFile> tokenFile = soul::common::token::file::parser::TokenFileParser<LexerType>::Parse(lexer);
    if (external)
    {
        tokenFile->SetExternal();
    }
    return tokenFile;
}

} // namespace soul::spg
