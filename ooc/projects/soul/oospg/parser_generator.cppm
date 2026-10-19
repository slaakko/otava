export module soul.spg.parser_generator;

import std;
import soul.lexer.file_map;
import soul.ast.spg;

export namespace soul::spg {

void GenerateParsers(soul::ast::spg::SpgFile* spgFile, soul::lexer::FileMap& fileMap, bool verbose, bool noDebugSupport, bool optimize, bool xml, 
    const std::string& version);

} // namespace soul::spg
