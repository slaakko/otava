export module soul.spg.file_parsers;

import std;
import soul.lexer.file_map;
import soul.ast;

export namespace soul::spg {

std::unique_ptr<soul::ast::spg::SpgFile> ParseSpgFile(const std::string& spgFilePath, soul::lexer::FileMap& fileMap, bool verbose);
std::unique_ptr<soul::ast::spg::ParserFile> ParseParserFile(const std::string& parserFilePath, const soul::ast::SourcePos& sourcePos,
    soul::lexer::FileMap& fileMap, bool verbose, bool external);
std::unique_ptr<soul::ast::common::TokenFile> ParseTokenFile(const std::string& tokenFilePath, bool external);

} // namespace soul::spg
